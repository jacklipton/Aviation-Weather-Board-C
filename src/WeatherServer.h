#ifndef WEATHERSERVER_H
#define WEATHERSERVER_H

#include <Arduino.h>
#include <ESP32HTTPClient.h> // Use appropriate HTTP client based on your environment
#include <ArduinoJson.h>
#include "LightControl.h"
#include "config.h" // Include the configuration header

class WeatherServer {
public:
    WeatherServer();
    void setup();
    void updateWeather();
private:
    // Use the API key from config.h
    const char* apiKey = WEATHER_API_KEY;
    const char* airports = "CYGK,CYHM,CYKF,CYND,CYOO,CYOW,CYPQ,CYQA,CYSN,CYTR,CYTZ,CYXU,CYYZ,KBUF,KIAG,KMSS,KOGS";
    int lightOrder[17] = {14, 41, 44, 8, 21, 7, 19, 26, 35, 17, 31, 48, 32, 38, 36, 4, 1};
    LightControl* lightControl;
    void parseAndUpdate(const String& payload);
};

#endif // WEATHERSERVER_H