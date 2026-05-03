#include "HTTP.h"

#include "freertos/FreeRTOS.h"
#include "esp_http_client.h"
#include "esp_log.h"


static const char* TAG = "HTTPClient";

esp_err_t HTTPClient_EventHandler(esp_http_client_event_handle_t event)
{
    switch (event->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "%s", (char*)event->data);
        break;
    default:
        break;
    }

    return ESP_OK;
}

void HTTPClient_GET(const char *url)
{
    esp_http_client_config_t http_config = {0};
    http_config.url = url;
    http_config.method = HTTP_METHOD_GET;
    http_config.cert_pem = NULL;
    http_config.event_handler = HTTPClient_EventHandler;

    esp_http_client_handle_t http_client = esp_http_client_init(&http_config);
    esp_http_client_perform(http_client);
    esp_http_client_cleanup(http_client);
}