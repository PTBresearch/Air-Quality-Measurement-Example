#!/bin/bash

# ==============================================================================
# All-in-One Appliance Setup Script for Raspberry Pi 4
#
# FOR A FRESHLY INSTALLED RASPBERRY PI OS (BOOKWORM OR NEWER)
#
# This script configures the system, including a full proxy for Docker
# and uses NetworkManager (nmcli) for robust hotspot creation.
# Run: sudo ./setup.sh
# ==============================================================================

# --- Script starts here ---

# Load configuration file
CONFIG_FILE="./config.sh"
if [ -f "$CONFIG_FILE" ]; then
    # shellcheck source=./config.sh
    source "$CONFIG_FILE"
else
    echo "FEHLER: Die Konfigurationsdatei '$CONFIG_FILE' wurde nicht gefunden." 1>&2
    exit 1
fi

# Check if the script is running as root
if [ "$(id -u)" != "0" ]; then
   echo "Dieses Skript muss mit sudo-Rechten ausgeführt werden." 1>&2
   exit 1
fi

SUDO_USER=${SUDO_USER:-$(whoami)}

echo "=== Umfassendes Initial-Setup für Raspberry Pi 4 wird gestartet ==="
echo ">>> Konfiguration aus '$CONFIG_FILE' geladen."


# Function to activate the serial interface for GPS
enable_uart() {
    echo ">>> Aktiviere Hardware UART für GPS (L76X)..."
    raspi-config nonint do_serial_cons 1
    raspi-config nonint do_serial_hw 0
    echo "Hardware UART wurde vollständig nicht-interaktiv konfiguriert."
}

# Function to set the proxy for System, APT and Docker
set_proxy() {
    if [ -n "$PROXY_URL" ]; then
        echo ">>> Setze Proxy-Einstellungen für System, APT und Docker..."
        local no_proxy_string
        no_proxy_string=$(IFS=,; echo "${NO_PROXY_ENTRIES[*]},$WLAN_IP")
        
        # 1. System Proxy
        grep -qF "http_proxy" /etc/environment || {
            echo "export http_proxy=\"$PROXY_URL\"" >> /etc/environment
            echo "export https_proxy=\"$PROXY_URL\"" >> /etc/environment
            echo "export no_proxy=\"$no_proxy_string\"" >> /etc/environment
        }
        
        # 2. Proxy für APT
        cat > /etc/apt/apt.conf.d/99proxy <<EOF
Acquire::http::Proxy "$PROXY_URL";
Acquire::https::Proxy "$PROXY_URL";
EOF

        # 3. Proxy for the Docker service (systemd) - for 'docker pull'
        local docker_proxy_dir="/etc/systemd/system/docker.service.d"
        mkdir -p "$docker_proxy_dir"
        cat > "${docker_proxy_dir}/http-proxy.conf" <<EOF
[Service]
Environment="HTTP_PROXY=${PROXY_URL}"
Environment="HTTPS_PROXY=${PROXY_URL}"
Environment="NO_PROXY=${no_proxy_string}"
EOF

        # 4. Proxy for Docker clients (for 'docker build')
        local user_docker_dir="/home/${KIOSK_USER}/.docker"
        local root_docker_dir="/root/.docker"
        mkdir -p "$user_docker_dir"
        mkdir -p "$root_docker_dir"
        
        local docker_config_json
        docker_config_json=$(cat <<EOF
{
  "proxies": {
    "default": {
      "httpProxy": "${PROXY_URL}",
      "httpsProxy": "${PROXY_URL}",
      "noProxy": "${no_proxy_string}"
    }
  }
}
EOF
)
        echo "$docker_config_json" > "${user_docker_dir}/config.json"
        echo "$docker_config_json" > "${root_docker_dir}/config.json"
        
        # Correct the permissions for the user
        chown -R "$KIOSK_USER":"$KIOSK_USER" "$user_docker_dir"

        echo "Proxy wurde gesetzt. Exportiere für die aktuelle Sitzung..."
        export http_proxy="$PROXY_URL"
        export https_proxy="$PROXY_URL"
        export no_proxy="$no_proxy_string"
    else
        echo ">>> Überspringe Proxy-Einstellungen (keine URL angegeben)."
    fi
}

# Feature: Synchronize time over HTTP to solve the proxy problem
sync_time_via_http() {
    echo ">>> Versuche, die Zeit über HTTP zu synchronisieren..."
    if ! command -v curl &> /dev/null; then apt-get update && apt-get install -y curl; fi
    HTTP_DATE=$(curl -x http://webproxy.bs.ptb.de:8080  -ksI 'https://ptb.de' | grep -i '^date:' | sed 's/Date: //i' | sed 's/ (GMT)//')
    if [ -n "$HTTP_DATE" ]; then date -s "$HTTP_DATE" && echo "Systemzeit erfolgreich gesetzt."; else echo "WARNUNG: Konnte die Zeit nicht über HTTP abrufen."; fi
}

# Function to perform updates and installations
install_dependencies() {
    echo ">>> Führe Systemupdates durch..."
    apt-get update && apt-get full-upgrade -y
    echo ">>> Installiere alle Abhängigkeiten..."
    apt-get install -y \
        curl gpsd gpsd-clients chrony iptables-persistent \
        xserver-xorg x11-xserver-utils xinit openbox \
        chromium-browser unclutter
    echo ">>> Bereinige nicht mehr benötigte Pakete..."
    apt-get autoremove -y && apt-get clean
    echo "Systemupdates und Installationen abgeschlossen."
}

# Function to install Docker
install_docker() {
    if command -v docker &> /dev/null; then echo ">>> Docker ist bereits installiert. Überspringe."; else
        echo ">>> Installiere Docker..."
        curl -fsSL https://get.docker.com -o get-docker.sh && sh get-docker.sh && rm get-docker.sh
        echo ">>> Füge Benutzer '$SUDO_USER' zur Docker-Gruppe hinzu..."
        usermod -aG docker "$SUDO_USER"
        
        # Falls ein Proxy gesetzt wurde, systemd neu laden und Docker neu starten, um es anzuwenden
        if [ -n "$PROXY_URL" ]; then
            echo ">>> Wende Proxy-Einstellungen auf Docker-Dienst an..."
            systemctl daemon-reload
            systemctl restart docker
        fi
        
        systemctl enable docker && systemctl start docker
        echo "Docker-Installation abgeschlossen."
    fi
}

# Function: Set WLAN country
set_wifi_country() {
    if [ -n "$WIFI_COUNTRY" ]; then
        echo ">>> Setze WLAN-Ländercode auf '$WIFI_COUNTRY'..."
        raspi-config nonint do_wifi_country "$WIFI_COUNTRY" [1]
        echo "WLAN-Land wurde gesetzt."
    else
        echo "WARNUNG: Kein WLAN-Ländercode (WIFI_COUNTRY) in config.sh gesetzt. Hotspot könnte blockiert sein."
    fi
}

# Function for setting up GPS time synchronization
setup_gps_time() {
    echo ">>> Konfiguriere GPSd und Chrony..."
    cat > /etc/default/gpsd <<EOF
START_DAEMON="true"
USBAUTO="false"
DEVICES="$GPS_DEVICE"
GPSD_OPTIONS="-n"
EOF
    sed -i -e 's/^\(pool .*\)$/# \1/' /etc/chrony/chrony.conf
    grep -qF "refclock SHM 0" /etc/chrony/chrony.conf || cat >> /etc/chrony/chrony.conf <<EOF
refclock SHM 0 poll 4 refid GPS offset 0.5 delay 0.2
EOF
    echo "GPS-Zeitsynchronisation konfiguriert."
}

# Function: GPS function test
check_gps_function() {
    echo "---------------------------------------------------------------------"
    echo ">>> Führe GPS-Funktionstest durch..."
    systemctl restart gpsd && sleep 3
    local gps_output
    gps_output=$(timeout 15s gpspipe -w)
    if [ -n "$gps_output" ]; then
        echo "*** ERFOLG: GPS-Modul ist korrekt angeschlossen und sendet Daten. ***"
        echo "$gps_output" | head -n 5
    else
        echo "!!! WARNUNG: Konnte keine Daten vom GPS-Modul empfangen. Bitte nach Neustart manuell prüfen. !!!"
    fi
    echo "---------------------------------------------------------------------"
}

# Function for setting up kiosk mode
setup_kiosk_mode() {
    echo ">>> Konfiguriere den Kiosk-Modus..."
    raspi-config nonint do_boot_behaviour B2
    BASH_PROFILE="/home/$KIOSK_USER/.bash_profile"
    grep -qF "startx" "$BASH_PROFILE" || echo "[[ -z \$DISPLAY && \$XDG_VTNR -eq 1 ]] && startx -- -nocursor" >> "$BASH_PROFILE"
    XINITRC="/home/$KIOSK_USER/.xinitrc"
    cat > "$XINITRC" <<EOF
#!/bin/bash
xset s noblank; xset s off; xset -dpms
unclutter -idle 5 -root &
sed -i 's/"exited_cleanly":false/"exited_cleanly":true/' /home/$KIOSK_USER/.config/chromium/Default/Preferences
sed -i 's/"exit_type":"Crashed"/"exit_type":"Normal"/' /home/$KIOSK_USER/.config/chromium/Default/Preferences
openbox &
/usr/bin/chromium-browser --no-first-run --start-maximized --disable-infobars --disable-features=Translate --kiosk "$KIOSK_URL"
EOF
    chown "$KIOSK_USER":"$KIOSK_USER" "$BASH_PROFILE" "$XINITRC"
    chmod +x "$XINITRC"
    echo "Kiosk-Modus konfiguriert."
}

# Function for installing and configuring the hotspot
setup_hotspot() {
    echo ">>> Konfiguriere den isolierten WLAN-Hotspot mit NetworkManager..."
    nmcli con delete id "$WLAN_SSID" 2>/dev/null || true
    nmcli con add type wifi ifname "$WLAN_INTERFACE" con-name "$WLAN_SSID" autoconnect yes ssid "$WLAN_SSID"
    nmcli con modify "$WLAN_SSID" 802-11-wireless.mode ap 802-11-wireless.band bg
    nmcli con modify "$WLAN_SSID" ipv4.method shared ipv4.addresses "$WLAN_IP/24"
    nmcli con modify "$WLAN_SSID" wifi-sec.key-mgmt wpa-psk
    nmcli con modify "$WLAN_SSID" wifi-sec.psk "$WLAN_PASSWORT"
    echo ">>> Konfiguriere Firewall zur Netzwerkisolation..."
    iptables -A FORWARD -i "$WLAN_INTERFACE" -o eth0 -j DROP
    netfilter-persistent save
    echo "Hotspot-Konfiguration mit NetworkManager abgeschlossen."
}

# Function to start the services and to complete
final_steps_and_reboot() {
    echo ">>> Aktiviere alle Dienste..."
    systemctl disable hostapd 2>/dev/null || true
    systemctl disable dnsmasq 2>/dev/null || true
    systemctl enable chrony
    echo "====================================================================="
    echo "=== Setup abgeschlossen! Der Pi wird jetzt neu gestartet.         ==="
    echo "====================================================================="
}

# --- Start Code ---
enable_uart
set_proxy
sync_time_via_http
install_dependencies
install_docker
set_wifi_country
setup_gps_time
check_gps_function
setup_kiosk_mode
setup_hotspot
final_steps_and_reboot

# Reboot
reboot