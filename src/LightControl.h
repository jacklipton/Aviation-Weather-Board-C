#ifndef LIGHTCONTROL_H
#define LIGHTCONTROL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LightControl {
public:
    LightControl();
    void setupLights();
    void colours(const String& category, int lightNum);
    void turnOff();
    void turnOn(const String& color);
private:
    static const int NEOPIXEL_PIN = 14; // GPIO14
    static const int NUM_PIXELS = 50;
    Adafruit_NeoPixel strip;
};

#endif // LIGHTCONTROL_H
