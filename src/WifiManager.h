#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include "config.h" // Include the configuration header

class WiFiManager {
public:
    WiFiManager();
    bool connect();
private:
    // Use WiFi credentials from config.h
    const char* ssid = WIFI_SSID;
    const char* password = WIFI_PASSWORD;
};

#endif // WIFIMANAGER_H