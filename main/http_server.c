#include "http_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "weather_client.h"
#include "led_control.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "http_server";
static bool weather_service_enabled = true;

bool is_weather_service_enabled(void) {
    return weather_service_enabled;
}

static const char* html_content = 
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"    <title>Control Panel</title>"
"    <style>"
"        body {"
"            font-family: Arial, sans-serif;"
"            text-align: center;"
"            margin-top: 50px;"
"        }"
"        h1 {"
"            color: #333;"
"        }"
"        button {"
"            background-color: #4CAF50; /* Green */"
"            border: none;"
"            color: white;"
"            padding: 20px 40px;"
"            text-align: center;"
"            text-decoration: none;"
"            display: inline-block;"
"            font-size: 20px;"
"            margin: 20px 2px;"
"            cursor: pointer;"
"            border-radius: 12px;"
"            transition-duration: 0.4s;"
"        }"
"        button:hover {"
"            background-color: #45a049;"
"        }"
"    </style>"
"</head>"
"<body>"
"    <h1>Weather Board Control Panel</h1>"
"    <button onclick=\"location.href='/?buttonA'\">Activate Weather Service</button>"
"    <button onclick=\"location.href='/?buttonB'\">Turn Off Lights</button>"
"</body>"
"</html>";

static esp_err_t root_get_handler(httpd_req_t *req)
{
    // Check query params
    char* buf;
    size_t buf_len;
    if (httpd_req_get_url_query_len(req) > 0) {
        buf_len = httpd_req_get_url_query_len(req) + 1;
        if (buf_len > 1) {
            buf = malloc(buf_len);
            if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query => %s", buf);
                if (strstr(buf, "buttonA")) {
                    ESP_LOGI(TAG, "Button A pressed");
                    weather_service_enabled = true;
                    weather_client_fetch_and_update();
                }
                if (strstr(buf, "buttonB")) {
                    ESP_LOGI(TAG, "Button B pressed");
                    weather_service_enabled = false;
                    led_turn_off();
                }
            }
            free(buf);
        }
    }

    httpd_resp_send(req, html_content, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static httpd_handle_t server = NULL;

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.stack_size = 8192; // Increase stack for handlers calling weather client

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &root);
    }
}

void stop_webserver(void)
{
    if (server) {
        httpd_stop(server);
    }
}
