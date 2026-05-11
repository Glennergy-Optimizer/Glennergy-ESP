#include "HTTP.h"
#include <stdlib.h>
// #include <string.h>

#include "freertos/FreeRTOS.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "WiFi.h"

static const char *TAG = "HTTPClient";

esp_err_t HTTPClient_EventHandler(esp_http_client_event_handle_t event)
{
    HTTPResponse *resp = (HTTPResponse *)event->user_data;

    switch (event->event_id)
    {
    case HTTP_EVENT_ON_CONNECTED:

        resp->data = NULL;
        resp->length = 0;

        break;
    case HTTP_EVENT_ON_DATA:
        // ESP_LOGI(TAG, "%s", (char *)event->data);
        if (event->data_len > 0)
        {
            char *new_ptr = realloc(resp->data, resp->length + event->data_len + 1);

            if (new_ptr == NULL)
            {
                ESP_LOGE(TAG, "Failed to allocate memory");
                return ESP_FAIL;
            }

            resp->data = new_ptr;

            memcpy(resp->data + resp->length, event->data, event->data_len);

            resp->length += event->data_len;

            resp->data[resp->length] = '\0';
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        break;
    default:
        break;
    }

    return ESP_OK;
}

void HTTPClient_GET(const char *url, HTTPResponse *http_response)
{
    esp_http_client_config_t http_config = {0};
    http_config.url = url;
    http_config.method = HTTP_METHOD_GET;
    http_config.cert_pem = NULL;
    http_config.event_handler = HTTPClient_EventHandler;
    http_config.user_data = http_response;

    if (WiFi_IsConnected())
    {
        esp_http_client_handle_t http_client = esp_http_client_init(&http_config);
        esp_http_client_perform(http_client);
        esp_http_client_cleanup(http_client);
    }
    else
    {
        ESP_LOGW(TAG, "WiFi is not connected, couldnt fetch data");
        return;
    }
}

void HTTPClient_Dispose(HTTPResponse *http_response)
{
    if (http_response != NULL && http_response->data != NULL)
        free(http_response->data);
}