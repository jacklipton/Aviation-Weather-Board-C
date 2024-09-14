#include "WiFiManager.h"

WiFiManager::WiFiManager() {}

bool WiFiManager::connect() {
    Serial.print("Connecting to WiFi SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();

    // Keep trying to connect for 30 seconds
    while (WiFi.status() != WL_CONNECTED && 
           millis() - startAttemptTime < 30000) {
        Serial.print(".");
        delay(1000);
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nFailed to Connect to WiFi.");
        return false;
    }
}