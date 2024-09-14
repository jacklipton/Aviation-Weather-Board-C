#include "LightControl.h"

LightControl::LightControl() : strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800) {}

void LightControl::setupLights() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void LightControl::colours(const String& category, int lightNum) {
    if (lightNum < 0 || lightNum >= NUM_PIXELS) {
        Serial.printf("Invalid light number: %d\n", lightNum);
        return;
    }

    if (category == "VFR") {
        strip.setPixelColor(lightNum, strip.Color(20, 0, 0)); // Red
    } else if (category == "MVFR") {
        strip.setPixelColor(lightNum, strip.Color(0, 0, 35)); // Blue
    } else if (category == "IFR") {
        strip.setPixelColor(lightNum, strip.Color(0, 25, 0)); // Green
    } else if (category == "LIFR") {
        strip.setPixelColor(lightNum, strip.Color(0, 25, 25)); // Cyan
    } else {
        strip.setPixelColor(lightNum, strip.Color(0, 0, 0)); // Off
    }
    strip.show();
}

void LightControl::turnOff() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}

void LightControl::turnOn(const String& color) {
    int lightOrder[17] = {1, 4, 7, 8, 14, 17, 19, 21, 26, 31, 32, 35, 36, 38, 41, 44, 48};
    uint32_t ledColor;

    if (color == "green") {
        ledColor = strip.Color(25, 0, 0);
    } else if (color == "red") {
        ledColor = strip.Color(0, 25, 0);
    } else {
        ledColor = strip.Color(0, 0, 0); // Default to off
    }

    for (int i = 0; i < 17; i++) {
        int lt_num = lightOrder[i];
        if (lt_num >= 0 && lt_num < strip.numPixels()) {
            strip.setPixelColor(lt_num, ledColor);
        }
    }
    strip.show();
}