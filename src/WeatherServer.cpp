#include "WeatherServer.h"
#include <HTTPClient.h>

WeatherServer::WeatherServer() : lightControl(nullptr) {}

void WeatherServer::setup() {
    lightControl = new LightControl();
}

void WeatherServer::updateWeather() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String("https://api.checkwx.com/metar/") + airports + "/decoded";
        http.begin(url);
        http.addHeader("X-API-Key", apiKey);
        
        int httpCode = http.GET();
        if (httpCode > 0) { // Check for the returning code
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                parseAndUpdate(payload);
            }
        } else {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
        Serial.println("WiFi not connected. Cannot fetch weather data.");
    }
}

void WeatherServer::parseAndUpdate(const String& payload) {
    const size_t capacity = JSON_ARRAY_SIZE(100) + 1000;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    JsonArray data = doc["data"];
    int i = 0;
    for (JsonObject cy : data) {
        if (i >= 17) break; // Prevent out-of-bounds
        String category = cy["flight_category"] | "UNKNOWN";
        if (category == "VFR") {
            lightControl->colours("VFR", lightOrder[i]);
        } else if (category == "MVFR") {
            lightControl->colours("MVFR", lightOrder[i]);
        } else if (category == "IFR") {
            lightControl->colours("IFR", lightOrder[i]);
        } else if (category == "LIFR") {
            lightControl->colours("LIFR", lightOrder[i]);
        }
        i++;
    }
}