#include "weather_client.h"
#include "led_control.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "sdkconfig.h"
#include "esp_crt_bundle.h"

static const char *TAG = "weather_client";

#define MAX_HTTP_OUTPUT_BUFFER 20480 

static int ltOrder[] = {14,41,44,8,21,7,19,26,35,17,31,48,32,38,36,4,1};

void weather_client_fetch_and_update(void)
{
    char *buffer = malloc(MAX_HTTP_OUTPUT_BUFFER);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Cannot allocate buffer");
        return;
    }
    
    esp_http_client_config_t config = {
        .url = "https://api.checkwx.com/metar/CYGK,CYHM,CYKF,CYND,CYOO,CYOW,CYPQ,CYQA,CYSN,CYTR,CYTZ,CYXU,CYYZ,KBUF,KIAG,KMSS,KOGS/decoded",
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 10000,
    };
    
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "X-API-Key", CONFIG_CHECKWX_API_KEY);

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        free(buffer);
        esp_http_client_cleanup(client);
        return;
    }

    int content_length = esp_http_client_fetch_headers(client);
    int total_read_len = 0;
    int read_len;
    
    if (content_length >= MAX_HTTP_OUTPUT_BUFFER) {
        ESP_LOGE(TAG, "Content length too large: %d", content_length);
        free(buffer);
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return;
    }

    // Read response
    while (1) {
        int size_to_read = MAX_HTTP_OUTPUT_BUFFER - total_read_len - 1;
        if (size_to_read <= 0) {
            break;
        }
        read_len = esp_http_client_read_response(client, buffer + total_read_len, size_to_read);
        if (read_len <= 0) {
            if (esp_http_client_is_complete_data_received(client)) {
                break;
            }
            ESP_LOGE(TAG, "Error reading data");
            break;
        }
        total_read_len += read_len;
    }
    
    buffer[total_read_len] = 0; // Null terminate

    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    ESP_LOGI(TAG, "Read %d bytes", total_read_len);

    // Parse JSON
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL) {
        ESP_LOGE(TAG, "Failed to parse JSON");
        free(buffer);
        return;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (cJSON_IsArray(data)) {
        int count = cJSON_GetArraySize(data);
        for (int i = 0; i < count && i < sizeof(ltOrder)/sizeof(ltOrder[0]); i++) {
            cJSON *item = cJSON_GetArrayItem(data, i);
            cJSON *cat = cJSON_GetObjectItem(item, "flight_category");
            if (cJSON_IsString(cat) && (cat->valuestring != NULL)) {
                ESP_LOGI(TAG, "Airport %d: %s", i, cat->valuestring);
                led_set_category(cat->valuestring, ltOrder[i]);
            }
        }
    }

    cJSON_Delete(root);
    free(buffer);
}
