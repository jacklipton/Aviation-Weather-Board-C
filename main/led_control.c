#include "led_control.h"
#include "led_strip.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string.h>

static const char *TAG = "led_control";
static led_strip_handle_t led_strip;

void led_control_init(void)
{
    ESP_LOGI(TAG, "Initializing LED strip");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = CONFIG_LED_GPIO,
        .max_leds = CONFIG_LED_COUNT, 
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LEDs off to clear all pixels */
    led_strip_clear(led_strip);
}

void led_set_category(const char* category, int index)
{
    if (index >= CONFIG_LED_COUNT) return;

    // Python: (G, R, B)
    // VFR: (20, 0, 0) -> Green
    // MVFR: (0, 0, 35) -> Blue
    // IFR: (0, 25, 0) -> Red
    // LIFR: (0, 25, 25) -> Magenta

    int r=0, g=0, b=0;

    if (strcmp(category, "VFR") == 0) {
        g = 20;
    } else if (strcmp(category, "MVFR") == 0) {
        b = 35;
    } else if (strcmp(category, "IFR") == 0) {
        r = 25;
    } else if (strcmp(category, "LIFR") == 0) {
        r = 25; b = 25;
    }

    led_strip_set_pixel(led_strip, index, r, g, b);
    led_strip_refresh(led_strip);
}

void led_turn_off(void)
{
    led_strip_clear(led_strip);
}

void led_turn_on_color(const char* color)
{
    // Python:
    // ltOrder = [1,4,7,8,14,17,19,21,26,31,32,35,36,38,41,44,48]
    // green: (25, 0, 0) -> G=25
    // red: (0, 25, 0) -> R=25

    int ltOrder[] = {1,4,7,8,14,17,19,21,26,31,32,35,36,38,41,44,48};
    int r=0, g=0, b=0;

    if (strcmp(color, "green") == 0) {
        g = 25;
    } else if (strcmp(color, "red") == 0) {
        r = 25;
    }

    for (int i=0; i < sizeof(ltOrder)/sizeof(ltOrder[0]); i++) {
        led_strip_set_pixel(led_strip, ltOrder[i], r, g, b);
    }
    led_strip_refresh(led_strip);
}
