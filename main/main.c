#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "wifi_connect.h"
#include "led_control.h"
#include "weather_client.h"
#include "http_server.h"

static const char *TAG = "main";

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize LEDs
    led_control_init();
    led_turn_on_color("red"); // Indicate startup/no connection

    // Initialize WiFi
    wifi_init_sta();
    
    // Connected
    led_turn_on_color("green");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    led_turn_off();

    // Start Webserver
    start_webserver();

    // Initial Weather Update
    if (is_weather_service_enabled()) {
        weather_client_fetch_and_update();
    }

    TickType_t last_update = xTaskGetTickCount();
    const int refresh_ticks = CONFIG_REFRESH_MINUTES * 60 * 1000 / portTICK_PERIOD_MS;

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Check every second

        if (is_weather_service_enabled()) {
            if ((xTaskGetTickCount() - last_update) > refresh_ticks) {
                ESP_LOGI(TAG, "Performing scheduled weather update");
                weather_client_fetch_and_update();
                last_update = xTaskGetTickCount();
            }
        }
    }
}
