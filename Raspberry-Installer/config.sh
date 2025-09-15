#!/bin/bash

# ==============================================================================
# Configuration file for the Raspberry Pi Appliance setup script
#
# Adjust the values in this file to match your environment.
# ==============================================================================


# --- PROXY SETTINGS ---
# Leave the URL empty (PROXY_URL="") if no proxy is needed.
# This URL is used for both HTTP and HTTPS.
# Example: PROXY_URL="http://proxy.example.com:8080"
PROXY_URL=""

# Addresses that should bypass the proxy. Add more entries here
# using the "entry1" "entry2" pattern.
NO_PROXY_ENTRIES=(
    "localhost"
    "127.0.0.1"
    ".local"
    ".bs.ptb.de"
    ".berlin.ptb.de"
    "192.168.1.0/24"
)
 

# --- KIOSK MODE ---
# The system user under which the graphical interface (kiosk) should run.
# The default user on Raspberry Pi OS is "pi".
KIOSK_USER="pi"

# The URL that should be displayed in full-screen mode.
# Typically a web application running on the Pi.
KIOSK_URL="http://localhost:3000"


# --- GPS SETTINGS ---
# The serial device for your GPS module (e.g., L76X).
# /dev/ttyS0 is the default for the primary UART on the GPIO pins.
GPS_DEVICE="/dev/ttyS0"

# --- WIFI HOTSPOT SETTINGS ---
# NEW: Country code for WLAN. This is mandatory.
# Use the two-digit ISO code (e.g., DE for Germany,
# AT for Austria, CH for Switzerland, US for the USA).
WIFI_COUNTRY="DE"

# --- WIFI HOTSPOT SETTINGS ---
# The name (SSID) of your Wi-Fi hotspot.
WLAN_SSID=""

# The password for your Wi-Fi hotspot (must be at least 8 characters long).
WLAN_PASSWORD="luftqualitaet"

# The network interface for Wi-Fi (usually wlan0).
WLAN_INTERFACE="wlan0"

# The static IP address of the Raspberry Pi within the hotspot network.
# This is also the gateway for connected devices.
WLAN_IP="192.168.1.1"