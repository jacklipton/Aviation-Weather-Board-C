# Copy this file to config.py and fill in your settings.
# This file is imported by MicroPython on the device.

# Wi-Fi
WIFI_SSID = "your-ssid"
WIFI_PASSWORD = "your-password"

# CheckWX API
CHECKWX_API_KEY = "your-api-key"

# LEDs
LED_PIN = 14         # GPIO pin connected to the NeoPixel data line
LED_COUNT = 50       # Number of LEDs

# Behavior
REFRESH_MINUTES = 15 # How often to refresh METARs (minutes)
WEBREPL_ENABLED = False  # Set True if you've run webrepl_setup() and enabled it
