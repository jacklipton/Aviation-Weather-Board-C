# ESP32 Aviation Weather LED Indicator

This project connects to WiFi, fetches aviation weather data from CheckWX, and displays flight categories using NeoPixels (LEDs) on an ESP32 board. It also hosts a simple web server to indicate the system's status.

## Project Structure

esp32-weather-project/
├── src/
│ ├── main.cpp
│ ├── WiFiManager.h
│ ├── WiFiManager.cpp
│ ├── WeatherServer.h
│ ├── WeatherServer.cpp
│ ├── LightControl.h
│ ├── LightControl.cpp
│ ├── WebServer.h
│ ├── WebServer.cpp
│ ├── config.example.h
│ └── config.h (excluded from Git)
├── lib/
│ └── (additional libraries if needed)
├── include/
│ └── (header files if separating further)
├── README.md
├── platformio.ini
└── .gitignore


## Setup

### Prerequisites

- **PlatformIO**: Install [PlatformIO](https://platformio.org/install) for your preferred IDE (e.g., Visual Studio Code).
- **ESP32 Board**: Ensure you have an ESP32 development board.

### Steps

1. **Clone the Repository**

    ```bash
    git clone https://github.com/yourusername/esp32-weather-project.git
    cd esp32-weather-project
    ```

2. **Create `config.h` from Template**

    The repository includes a `config.example.h` file as a template. Create your own `config.h` by copying the template and filling in your credentials.

    ```bash
    cp src/config.example.h src/config.h
    ```

    Then, open `src/config.h` and replace the placeholders with your actual API key and WiFi credentials.

3. **Configure WiFi and API Key**

    - **WiFi Credentials**: Open `src/config.h` and replace `"your_wifi_ssid"` and `"your_wifi_password"` with your actual WiFi SSID and password.
    - **API Key**: Replace `"your_actual_api_key_here"` with your actual API key from [CheckWX](https://www.checkwx.com/).

4. **Install Dependencies**

    PlatformIO will automatically install the required libraries specified in `platformio.ini`. Ensure your `platformio.ini` includes:

    ```ini
    [env:esp32dev]
    platform = espressif32
    board = esp32dev
    framework = arduino
    monitor_speed = 115200

    lib_deps =
        adafruit/Adafruit NeoPixel@^1.10.6
        bblanchon/ArduinoJson@^6.18.5
        me-no-dev/ESP Async WebServer@^1.2.3
    ```

5. **Build and Upload**

    - Connect your ESP32 board to your computer via USB.
    - Use PlatformIO to build and upload the firmware:

        ```bash
        pio run --target upload
        ```

6. **Monitor Serial Output**

    Open the PlatformIO serial monitor to view debug messages:

    ```bash
    pio device monitor
    ```

## Important Security Note

**Never** commit your `config.h` file to GitHub or any other public repository. This file contains sensitive information such as your API keys and WiFi credentials. By following the steps above and ensuring `config.h` is listed in your `.gitignore`, you protect your credentials from being exposed.
