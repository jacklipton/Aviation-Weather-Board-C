# Aviation Weather Board (ESP-IDF)

This project has converted this [project](https://github.com/jacklipton/Aviation-weather-board) from MicroPython to ESP-IDF (C/C++).

## Prerequisites

*   VS Code with Espressif IDF Extension installed.
*   ESP-IDF v5.0 or later.

## Setup

1.  Open this folder in VS Code.
2.  Configure the project using the ESP-IDF extension (select target device, e.g., ESP32).
3.  Run `idf.py menuconfig` (or use the extension's configuration menu - ESP-IDF: SDK Configuration Editor) to set:
    *   WiFi SSID and Password
    *   CheckWX API Key
    *   LED GPIO and Count
    *   Refresh Interval
    *   (Look under "Aviation Weather Configuration")

## Configuration & Secrets

The project configuration is stored in `sdkconfig`. This file is **ignored by git** because it contains sensitive information (WiFi credentials, API keys).

*   **`sdkconfig`**: Generated build configuration (Ignored).
*   **`sdkconfig.defaults`**: Default project configuration (Tracked). Use this to set non-sensitive defaults.
*   **`.vscode/`**: VS Code workspace settings (Ignored). Contains local paths to ESP-IDF tools.

To persist configuration changes without committing secrets:
1.  Set your secrets in `menuconfig` (they go into `sdkconfig`).
2.  If you change project-wide settings (e.g., stack size, partition table), manually add them to `sdkconfig.defaults`.

## Build and Flash

1.  Build the project using the extension or `idf.py build`.
2.  Flash to the device using the extension or `idf.py flash`.
3.  Monitor the output using the extension or `idf.py monitor`.

## Legacy Python Code

The original MicroPython code has been moved to the `legacy_python/` directory.
