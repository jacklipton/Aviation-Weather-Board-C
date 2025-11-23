idf_component_register(SRCS "main.c" "wifi_connect.c" "led_control.c" "weather_client.c" "http_server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash esp_wifi esp_event esp_http_client esp_http_server json driver esp-tls)
