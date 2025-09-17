# Note: This repository is meant as an example. The code is incomplete and not the latest version. 

# Note: The global InfluxDB instance is planned to be replaced by Fiware.

# Air Quality

## Sensors List (Not Complete)

+ **PPD42NS** (Dust Concentration)

+ **BME280** (Humidity, Air Pressure, Temperature)

+ **CCS811** (TVOC Value, CO2 Equivalent)

+ **MAX31855PMB1#** (Temperature)

+ **SHT35** (Temperature, Humidity)

+ **GUVA-S12D** (UV)

+ **TCS34725** Light Sensor (RGB, Color Temperature, Brightness, Reference Value)

+ **MH-Z19C** (CO2 Value Infrared Sensor, Temperature)

+ **SPH** (DB)

+ **HP206C** ()

## Images and Render

<table>
  <tr>
    <td>
      <img src="Dokumentation\Arduino\2025-09-16-PCB-V3-Blender-Render-Case.gif" alt="Front Image">
      <figcaption>Blender Case with PCB Render</figcaption>
    </td>
    <td>
      <img src="Dokumentation\Arduino\2025-09-16-PCB-V3-Blender-Render-Case-Base.gif" alt="Front Image">
      <figcaption>Blender Base with PCB Render</figcaption>
    </td>
    <td>
      <img src="Dokumentation\Arduino\2025-09-16-PCB-V3-Blender-Render-PCB.gif" alt="Front Image">
      <figcaption>Blender PCB Render</figcaption>
    </td>
  </tr>
  <tr>
    <td>
      <img src="Dokumentation\RPI\2024-05-15_RPI_Front.jpg" alt="Front Image">
      <figcaption>Front View</figcaption>
    </td>
    <td>
      <img src="Dokumentation\RPI\2024-05-15_RPI_Side.jpg" alt="Side Image">
      <figcaption>Side View</figcaption>
    </td>
    <td>
      <img src="Dokumentation\RPI\2024-05-15_RPI_Side.jpg" alt="Side Image">
      <figcaption>Placeholder</figcaption>
    </td>
  </tr>
</table>

## Structure

```
.
├── Arduino-Code/          # The main folder for all microcontroller code.
│   ├── Main/              # Code for the primary Arduino prototype (likely ESP8266).
│   │   ├── lib/           # Libraries for this specific project.
│   │   └── Main.ino # The main program file for the Arduino.
│
├── Dokumentation/         # Images.
│
├── Files/                 # Specific files relevant to hardware and measurements.
│   ├── Gehaeuse/          # 3D models and design files for the project's enclosure.
│   └── Platinen/          # Design files (Gerber files, etc.) for the printed circuit boards (PCBs).
│
└── Raspberry-Installer/   # Scripts and files for setting up the Raspberry Pi.
    └── Docker/            # Docker configurations and source code for installing services on the Pi.
        └── src/           # Source code used by the Docker containers.
```

## Function

```
┌── Raspberry Pi
│   └── Creates a Wi-Fi network
│
└── Arduino
    ├── Connects to the Wi-Fi network
    │
    ├── Reads data from the sensor
    │
    └── Sends data over Wi-Fi to the API
        │
        └── API
            ├── Collects the sent data
            │
            ├── Inserts the data into a local database (InfluxDB)
            │
            └── Inserts the data into a global database (InfluxDB)
```

## Example JSON-Structure


```
{
  "arduino_board": "<arduino_id>",
  "sensors": [
    {
      "name": "<sensor_name>",
      "sensor_model": "<sensor_model>",
      "readings": [
        {
          "type": "<influx measurement>",
          "value": VALUE,
          "unit": "<unit>"
          "label": "<label optional>"
        },
        {
          "type": "<influx measurement>",
          "value": VALUE,
          "unit": "<unit>"
          "label": "<label optional>"
        }
      ]
    },
    {
      "name": "bme280",
      "sensor_model": "",
      "readings": [
        {
          "type": "temperature",
          "value": VALUE,
          "unit": "celsius"
        },
        {
          "type": "pressure",
          "value": VALUE,
          "unit": "hpa"
        },
        {
          "type": "humidity",
          "value": VALUE,
          "unit": "%"
        }
      ]
    }
  ]
}
```

---

## Instalation

1. Download: 

```bash
git clone <url> luftqualitaet
```

2. Edit ```WLAN_SSID``` in ```luftqualitaet/Raspberry-Installer/config.sh``` 
```bash
export replace=<Hostname>
sed -i -e 's/WLAN_SSID = Hostname/WLAN_SSID = ${replace}/g' luftqualitaet/Raspberry-Installer/config.sh
```


3. (Optional) Set Proxy Settings ```luftqualitaet/Raspberry-Installer/Docker/.env```
```bash
echo "http_proxy=http://webproxy:8080
https_proxy=http://webproxy:8080
no_proxy=influx,localhost" > /home/pi/luftqualitaet/Raspberry-Installer/Docker/.env
```

4. Login as Root
```bash
sudo -i
```

5. Change directory to Installer Location
```bash
cd /home/pi/luftqualitaet/Raspberry-Installer
```

6. Make Script executeable
```bash
chmod +x config.sh setup.sh
```

7. Set Current Time
```bash
HTTP_DATE=$(curl -x http://webproxy:8080  -ksI 'https://ptb.de' | grep -i '^date:' | sed 's/Date: //i' | sed 's/ (GMT)//')
if [ -n "$HTTP_DATE" ]; then date -s "$HTTP_DATE" && echo "Systemzeit erfolgreich gesetzt."; else echo "WARNUNG: Konnte die Zeit nicht über HTTP abrufen."; fi
```

8. Run Script
```
./setup.sh
```

9. Change directory to Docker Location
```bash
cd /home/pi/luftqualitaet/Raspberry-Installer/Docker
```

10. Start Docker
```bash
docker compose up -d
```

11. Setup Influxdb (http://hostname:8086) and Grafan (http://hostname:3000) 

12. Set InfluxDB Token in Config ```/home/pi/Raspberry-Installer/Docker/src/config.yml````

13. Restart Docker
```bash
docker compose down
docker compose up -d
```

14. Restart Raspberry PI
```bash
systemctl reboot
```

---


## Components

### Raspberry PI

| Component | Order Link | Datasheet Link(s) | Further Information |
|---|---|---|---|
| Pi 4B | [https://www.reichelt.de/das-reichelt-raspberry-pi-4-b-4-gb-all-in-bundle-rpi-4b-4gb-allin-p263086.html](https://www.reichelt.de/das-reichelt-raspberry-pi-4-b-4-gb-all-in-bundle-rpi-4b-4gb-allin-p263086.html) | | |
| Micro SD | [https://www.reichelt.de/de/de/shop/produkt/microsdxc-speicherkarte_128gb_samsung_pro_plus-313955](https://www.reichelt.de/de/de/shop/produkt/microsdxc-speicherkarte_128gb_samsung_pro_plus-313955) | | |
| GPS | [https://www.reichelt.de/de/de/shop/produkt/raspberry_pi_shield_-_multi-gnss_l76x-266060](https://www.reichelt.de/de/de/shop/produkt/raspberry_pi_shield_-_multi-gnss_l76x-266060) | | |
| Raspberry Pi Shield | [https://www.reichelt.de/de/de/shop/produkt/raspberry_pi_shield_-_display_lcd-touch_7_800x480_pixel-282508](https://www.reichelt.de/de/de/shop/produkt/raspberry_pi_shield_-_display_lcd-touch_7_800x480_pixel-282508) | | |


### Sensor Station

| Component | Sensor Type | Order Link | Datasheet Link(s) | Notes | Ambient Temperature | Humidity | Air Pressure |
|---|---|---|---|---|---|---|---|
| **BME280** | Temp & Pressure Sensor | [https://www.reichelt.de/ch/de/shop/produkt/entwicklerboards\_-\_temperatur-\_feuchtigkeits-*und\_drucksensor*-253982](https://www.reichelt.de/ch/de/shop/produkt/entwicklerboards_-_temperatur-_feuchtigkeits-_und_drucksensor_-253982) | [https://cdn-reichelt.de/documents/datenblatt/A300/BST-BME280-DS002-1509607.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/BST-BME280-DS002-1509607.pdf) | Inaccurate temperature (error tolerance of +3°C) | Accuracy: +1.0°C \<br\> Range: -40°C to +85°C \<br\> Max Deviation: Up to +1.5°C | Accuracy: +3% rF \<br\> Range: 0% to 100% rF \<br\> Max Deviation: up to +5% | Accuracy: +1 hPa \<br\> Range: 300 hPa \<br\> Max Deviation: Up to +1.7 hPa |
| **CCS811** | TVOC | [https://www.reichelt.de/ch/de/shop/produkt/entwicklerboards\_-\_sensor\_fuer\_luftqualitaet\_ccs811-253655](https://www.reichelt.de/ch/de/shop/produkt/entwicklerboards_-_sensor_fuer_luftqualitaet_ccs811-253655) | [https://cdn-reichelt.de/documents/datenblatt/A300/SEN-CCS811V1-DATENBLATT-02-11-2020.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/SEN-CCS811V1-DATENBLATT-02-11-2020.pdf) | Has a burn-in period (48 hours) and a warm-up time (20 min) before it becomes accurate | Accuracy: +3°C \<br\> Range: 0°C to +50°C \<br\> Max Deviation: approx. +5°C | Range: 10% to 95% | |
| **PPD42NS** | Dust | [https://www.reichelt.de/ch/de/shop/produkt/arduino\_-\_staubsensor\_ppd42ns-191264](https://www.reichelt.de/ch/de/shop/produkt/arduino_-_staubsensor_ppd42ns-191264) | [https://cdn-reichelt.de/documents/datenblatt/A300/101020012\_01.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/101020012_01.pdf) | Must be straight, requires 3 min to warm up, arbitrary operation can lead to damage, standard settings should not be changed | Recommended Range: -10°C to +65°C | Range: 0% to 70% | |
| **MAX31855PMB1\#** | Typk Temp | [https://de.farnell.com/analog-devices/max31855pmb1/peripheriemodul-thermoel-digit/dp/2530496](https://de.farnell.com/analog-devices/max31855pmb1/peripheriemodul-thermoel-digit/dp/2530496) | [https://www.farnell.com/datasheets/2025616.pdf](https://www.farnell.com/datasheets/2025616.pdf) | Inaccurate temperature (error tolerance of +2°C) | Recommended Range: -20°C to +85°C | Range: 0% to 95% | |
| **D1 Mini** | D1 Mini | [https://www.reichelt.de/de/de/shop/produkt/d1\_mini\_-\_kompatibles\_esp8266\_board\_v2\_0-253978](https://www.reichelt.de/de/de/shop/produkt/d1_mini_-_kompatibles_esp8266_board_v2_0-253978) | | | | | |
| **D1 Mini** | D1 Mini Pro | https:www.reichelt.de/de/de/shop/produkt/d1\_mini\_pro\_-\_kompatibles\_esp8266\_board\_cp2104-266066 | | | | | |
| **Power Supply** | Breadboard | [https://www.reichelt.de/de/de/shop/produkt/entwicklerboards\_-\_spannungsversorgung\_f\_steckboards-202832](https://www.reichelt.de/de/de/shop/produkt/entwicklerboards_-_spannungsversorgung_f_steckboards-202832) | [https://cdn-reichelt.de/documents/datenblatt/A300/DATENBLATTSBC-POW-BB.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/DATENBLATTSBC-POW-BB.pdf) | Too high voltage can damage the device or even lead to a fire | Recommended Range: 0°C to +70°C | Range: 10% to 60% (Absolutely not intended for humid rooms) | |
| **GSM18E09-P1J** | Plug-in Power Supply | [https://www.conrad.de/de/p/mean-well-gsm18e09-p1j-steckernetzteil-festspannung-9-v-dc-2-a-18-w-1837961.html](https://www.conrad.de/de/p/mean-well-gsm18e09-p1j-steckernetzteil-festspannung-9-v-dc-2-a-18-w-1837961.html) | [https://asset.conrad.com/media10/add/160267/c1/-/en/001837961DS01/datenblatt-1837961-mean-well-gsm18e09-p1j-steckernetzteil-festspannung-9-vdc-2-a-18-w.pdf](https://asset.conrad.com/media10/add/160267/c1/-/en/001837961DS01/datenblatt-1837961-mean-well-gsm18e09-p1j-steckernetzteil-festspannung-9-vdc-2-a-18-w.pdf) | Only lasts 3 years and is only 88% efficient | Recommended Range: -25 to +60 | Range: 20% to 90% | |
| **SHT35** | Temp & Humidity | [https://de.farnell.com/seeed-studio/101020212/sensorplatine-arduino-board/dp/3932104](https://de.farnell.com/seeed-studio/101020212/sensorplatine-arduino-board/dp/3932104) | [https://www.farnell.com/datasheets/3708357.pdf](https://www.farnell.com/datasheets/3708357.pdf) | Environmentally sensitive and should not be touched with fingers as it can be damaged | Recommended Range: -40°C to 125°C | Range: 10% to 90% | |
| **ADC** | Analog Digital Converter | [https://www.conrad.de/de/p/joy-it-ky053adc-raspberry-pi-erweiterungs-platine-passend-fuer-einplatinen-computer-raspberry-pi-raspberry-pi-a-b-1656368.html](https://www.conrad.de/de/p/joy-it-ky053adc-raspberry-pi-erweiterungs-platine-passend-fuer-einplatinen-computer-raspberry-pi-raspberry-pi-a-b-1656368.html) | [https://asset.conrad.com/media10/add/160267/c1/-/de/001656368DS01/datenblatt-1656368-joy-it-ky053adc-raspberry-pi-erweiterungs-platine-passend-fuer-einplatinen-computer-raspberry-pi-raspberry-pi-a-b.pdf](https://asset.conrad.com/media10/add/160267/c1/-/de/001656368DS01/datenblatt-1656368-joy-it-ky053adc-raspberry-pi-erweiterungs-platine-passend-fuer-einplatinen-computer-raspberry-pi-raspberry-pi-a-b.pdf) | Sensitive to interference due to its high resolution, which can make it slower to measure rapidly changing signals or values | Recommended Range: -40°C to 125°C | Range: 0% to 95% | |
| **GUVA-S12D** | UV Sensor | [https://www.reichelt.de/de/de/shop/produkt/arduino\_-\_uv-sensor\_guva-s12d-191215](https://www.reichelt.de/de/de/shop/produkt/arduino_-_uv-sensor_guva-s12d-191215) | [https://cdn-reichelt.de/documents/datenblatt/A300/101020043\_01.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/101020043_01.pdf) | Can be altered by temperature, sensitive to environmental influences, not precise | Recommended Range: -30°C to +85°C | Range: 0% to 95% | |
| **TCS34725** | Light Sensor | [https://www.conrad.de/de/p/az-delivery-tcs34725-rgb-farb-sensor-mit-infrarot-filter-diy-modul-zur-farb-erkennung-kompatibel-mit-arduino-850037694.html](https://www.conrad.de/de/p/az-delivery-tcs34725-rgb-farb-sensor-mit-infrarot-filter-diy-modul-zur-farb-erkennung-kompatibel-mit-arduino-850037694.html) | [https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-tcs34725-color-sensor](https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-tcs34725-color-sensor) | Light can change measurement values, the sensor has a certain amount of background noise | Recommended Range: -40°C to +85°C | Range: 0% to 60% | |
| **MH-Z19C** | CO2 Sensor | [https://www.reichelt.de/de/de/shop/produkt/infrarot\_co2-sensor\_mh-z19c\_pinleiste\_rm\_2\_54-297320](https://www.reichelt.de/de/de/shop/produkt/infrarot_co2-sensor_mh-z19c_pinleiste_rm_2_54-297320) | [https://cdn-reichelt.de/documents/datenblatt/C150/MH-Z19CDATENBLATT.pdf](https://cdn-reichelt.de/documents/datenblatt/C150/MH-Z19CDATENBLATT.pdf) | Only works at optimal temperatures (-10 \~ 50 °C) and is sensitive to heat and sun | Recommended Range: -10°C to 50°C | Range: 0% to 95% | |
| **Teensy4.1** | Teensy4.1 | [https://www.reichelt.de/de/de/shop/produkt/teensy\_4\_1\_usb\_ohne\_header-283580](https://www.reichelt.de/de/de/shop/produkt/teensy_4_1_usb_ohne_header-283580) | [https://cdn-reichelt.de/documents/datenblatt/A300/TEENSY\_41\_DE.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/TEENSY_41_DE.pdf) | Consumes a lot of power, lacks a Wi-Fi or Bluetooth connection, and can overheat during complex tasks | Recommended Range: -40°C to +85°C | Range: 0% to 95% | |
| **HP206C** | Grove - Barometer | [https://de.farnell.com/seeed-studio/101020068/sensor-barometer-arduino-raspberry/dp/4007687](https://de.farnell.com/seeed-studio/101020068/sensor-barometer-arduino-raspberry/dp/4007687) | [https://www.farnell.com/datasheets/3759062.pdf](https://www.farnell.com/datasheets/3759062.pdf) | The chip only accepts an input voltage of 1.8 V to 3.6 V. | Recommended Range: -40°C to +85°C | Range: 5% to 90% | |

### Mic

| Component | Sensor Type | Order Link | Datasheet Link(s) | Notes | Ambient Temperature | Humidity | Air Pressure |
|---|---|---|---|---|---|---|---|
| SPH0645 | Mems Mic | [https://www.reichelt.de/de/de/shop/produkt/entwicklerboards_-_mikrofone_sph0645-271874](https://www.reichelt.de/de/de/shop/produkt/entwicklerboards_-_mikrofone_sph0645-271874) | [https://cdn-reichelt.de/documents/datenblatt/A300/DEBO_MEMS_MIC_DB_EN.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/DEBO_MEMS_MIC_DB_EN.pdf) | Sensitive to air, especially at the porthole (may require further investigation later) | Recommended Range: -40°C to +85°C | Range: 5% to 95% | |
| D1 Mini | D1 Mini | [https://www.reichelt.de/de/de/shop/produkt/d1_mini_-_kompatibles_esp8266_board_v2_0-253978](https://www.reichelt.de/de/de/shop/produkt/d1_mini_-_kompatibles_esp8266_board_v2_0-253978) | | | | | |
| D1 Mini | D1 Mini Pro | https:www.reichelt.de/de/de/shop/produkt/d1_mini_pro_-_kompatibles_esp8266_board_cp2104-266066 | | | | | |
| D1 Mini | D1 Mini Lite | [https://www.reichelt.de/de/de/shop/produkt/d1_mini_lite_-_kompatibles_esp8285_board-291438](https://www.reichelt.de/de/de/shop/produkt/d1_mini_lite_-_kompatibles_esp8285_board-291438) | | | | | |
| Teensy4.1 | Teensy4.1 | [https://www.reichelt.de/de/de/shop/produkt/teensy_4_1_usb_ohne_header-283580](https://www.reichelt.de/de/de/shop/produkt/teensy_4_1_usb_ohne_header-283580) | [https://cdn-reichelt.de/documents/datenblatt/A300/TEENSY_41_DE.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/TEENSY_41_DE.pdf) | Consumes a lot of power, lacks a Wi-Fi or Bluetooth connection, and can overheat during complex tasks | Recommended Range: -40°C to +85°C | Range: 0% to 95% | |
| Power Supply | USB Charger | [https://www.reichelt.de/de/de/shop/produkt/usb-ladegeraet_5_v_1_a_usb-c_schwarz-268087?PROVID=2788](https://www.reichelt.de/de/de/shop/produkt/usb-ladegeraet_5_v_1_a_usb-c_schwarz-268087?PROVID=2788) | | | | | |


---

## Notes on Individual Sensors (Regarding Placement)

+ **BME280 Temperature, Air Pressure, Humidity Sensor:**
    + [https://www.makershop.de/download/bst-bme280-ds002.pdf](https://www.makershop.de/download/bst-bme280-ds002.pdf)
    + Do not place near heaters.

+ **CCS811 Carbon Dioxide Gas Sensor Metal Oxide:**
    + [https://www.makershop.de/download/CCS811_Datasheet-DS000459.pdf](https://www.makershop.de/download/CCS811_Datasheet-DS000459.pdf)
    + No special measures required.

+ **TCS3472 Color Sensor:**
    + [https://cdn.shopify.com/s/files/1/1509/1638/files/TCS3472_datasheet_1692bf77-3213-4bd2-9a66-8a9e70545e7e.pdf?v=1705651929](https://cdn.shopify.com/s/files/1/1509/1638/files/TCS3472_datasheet_1692bf77-3213-4bd2-9a66-8a9e70545e7e.pdf?v=1705651929)
    + The sensor should have access to light.
    + Not next to an LED on the ESP microcontroller.

+ **GRV Loudness Sensor:**
    + [https://cdn-reichelt.de/documents/datenblatt/A300/101020063_01.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/101020063_01.pdf)
    + The sensor should have external access.
    + Should be able to capture ambient sound.
    + Do not place it behind a cover.

+ **GRV Dust Sensor:**
    + [https://cdn-reichelt.de/documents/datenblatt/A300/101020012_01.pdf](https://cdn-reichelt.de/documents/datenblatt/A300/101020012_01.pdf)
    + The chip on the back gets hot occasionally.
    + Has a built-in fan, so do not place it directly next to the GRV Loudness sensor.

---

## General Notes

+ The enclosure must be sufficiently open to allow air to flow in for measurements.
+ Ensure thermal, optical, and acoustic decoupling of the microcontroller from the sensors (use a partition).
+ Leave space for 2-3 additional sensors (check sensor dimensions with a reference image).
+ Leave space for 2 more ESPs (D1 Mini Lite), for a total of 3.