#include <Arduino.h>
#include "WiFiManager.h"
#include "WeatherServer.h"
#include "LightControl.h"
#include "WebServer.h"

WiFiManager wifiManager;
WeatherServer weatherServer;
LightControl lightControl;
WebServer server;

void setup() {
    Serial.begin(115200);
    delay(1000); // Allow time for serial monitor to initialize

    // Initialize WiFi
    if (!wifiManager.connect()) {
        Serial.println("WiFi Connection Failed. Indicating failure with red lights.");
        lightControl.turnOn("red");
        delay(3000);
        lightControl.turnOff();
        ESP.restart();
    } else {
        Serial.println("WiFi Connected successfully. Indicating success with green lights.");
        lightControl.turnOn("green");
        delay(3000);
        lightControl.turnOff();
    }

    // Initialize Light Control
    lightControl.setupLights();

    // Initialize Weather Server
    weatherServer.setup();

    // Initialize Web Server
    server.setup();
}

void loop() {
    // Handle web server tasks
    server.handleClient();

    // Periodically fetch and update weather data
    weatherServer.updateWeather();

    delay(10000); // Wait for 10 seconds before next update
}


