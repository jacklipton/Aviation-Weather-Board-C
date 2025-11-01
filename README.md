<div id="top"></div>

<h3 align="center">Aviation Weather Map</h3>

  <p align="center">
    A weather dependant light up aviation map.
    <br />
    <a href="https://github.com/jacklipton/Aviation-weather-board"><strong>Explore the docs »</strong></a>
    <br />
  </p>
</div>


https://github.com/jacklipton/Aviation-weather-board/assets/83594679/f8df7533-afa8-4296-8fbb-ba53487697fc

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#prerequisites">Prerequisites</a></li>
    <li><a href="#installation">Installation</a></li>
    <li><a href="#configuration">Configuration</a></li>
    <li><a href="#contributing">Contributing</a></li>
  </ol>
</details>

<br/>


## About The Project

This project aimed to ad live weather data to a standard souther ontario VNC (VFR Navigation Chart) from NavCanada. Each airport has an LED at its location on the map and the colour of the LED corresponds to the current weather at that airport. It is designed to mimic the flight catagory feature on the maps tab in foreflight.

The board flashes either green or red when initially booted-up to indicate a successful or failed wifi connection. Each light corresponds to a specfic weather status:

* Green = VFR (Visusal Flight Rules): <i> Ceiling greater than 3000' and/or visibility greater than 5 miles</i>
* Blue = MVFR (Marginal Visual Flight Rules): <i> Ceiling between 1000' - 3000' and/or visibility between 3 - 5 miles</i>
* Red = IFR (Instrument Flight Rules): <i> Ceiling between 500' - 1000' and/or visibility between 1 - 3 miles</i>
* Pink = LIFR (Low Instrument Flight Rules): <i> Ceiling less than 500' and/or visibility less than 1</i>


### Built With

* [Micropython](https://micropython.org/)
* [CheckWX API](https://www.checkwxapi.com/)


## Prerequisites

Before you begin, ensure you have met the following requirements:

* Create an account with CheckWX and obtain an API key
* Download the MicroPython firmware for your microcontroller (ESP32 or ESP8266)
* Install tooling (macOS):

```bash
python3 -m pip install --user esptool mpremote
```

If your board does not appear under `/dev/tty.*`, install the appropriate USB-Serial driver (CP210x or CH34x).

## Quick Start (Flash → Configure → Deploy → Monitor)

This repo includes small helper scripts under `tools/` to make flashing and deploying easy.

1) Flash MicroPython to the board

```bash
# ESP32 example
tools/flash.sh esp32 ~/Downloads/esp32-[version].bin

# ESP8266 example
tools/flash.sh esp8266 ~/Downloads/esp8266-[version].bin
```

Notes:
- Put the board in bootloader mode (ESP32: hold BOOT, tap EN/RST; ESP8266: often automatic or hold FLASH + RST).
- If the script cannot auto-detect the port, pass it as the 3rd argument, e.g. `/dev/tty.usbserial-0001`.

2) Create your configuration (writes `config.py`)

```bash
python3 tools/setup_config.py
```

You’ll be prompted for Wi‑Fi SSID/password, CheckWX API key, LED pin/count, and refresh minutes. The script writes `config.py` (ignored by Git). Optionally, it can push `config.py` to the device.

3) Deploy project files to the board

```bash
tools/deploy.sh
```

This uploads `boot.py`, `connect_to_wifi.py`, `light_control.py`, `server.py`, `AviationWeatherServer.py`, and `config.py` to the device and resets it.

4) Monitor logs / REPL

```bash
tools/repl.sh
```

You’ll see prints from `boot.py` and `server.py`. Use `Ctrl-]` to exit.

## Configuration

All runtime settings live in `config.py` (created by `tools/setup_config.py`). Template: `config_template.py`.

Settings include:

- `WIFI_SSID`, `WIFI_PASSWORD`
- `CHECKWX_API_KEY`
- `LED_PIN`, `LED_COUNT`
- `REFRESH_MINUTES` (how often to refresh METARs)
- `WEBREPL_ENABLED` (set True if you have enabled WebREPL via `webrepl_setup` on the device)

Security: `config.py` is in `.gitignore` by default so your credentials are not committed.

## Contributing

Contributions are welcome! If you have any improvements, bug fixes, or feature suggestions, feel free to fork the repository and submit a pull request.


