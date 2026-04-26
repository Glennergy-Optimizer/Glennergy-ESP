#include "WiFi.h"

#include <string.h>

#include "freertos/event_groups.h"
#include "freertos/task.h"

#define TAG "WiFi"

#define WIFI_AUTH_MODE WIFI_AUTH_WPA2_PSK

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

// static const int WIFI_RETRY_ATTEMPT = 3;

static int wifi_retry_count = 0;

static esp_netif_t *netif = NULL;

static esp_event_handler_instance_t ip_event;
static esp_event_handler_instance_t wifi_event;

static EventGroupHandle_t wifi_event_group = NULL;

QueueHandle_t wifi_queue = NULL;

QueueHandle_t event_queue = NULL;

esp_err_t WiFi_Connect(wifi_data *w_info);

static void ip_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Handling IP event, event code 0x%" PRIx32, event_id);
    switch (event_id)
    {
    case (IP_EVENT_STA_GOT_IP):
        ip_event_got_ip_t *event_ip = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event_ip->ip_info.ip));
        wifi_retry_count = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case (IP_EVENT_STA_LOST_IP):
        ESP_LOGI(TAG, "Lost IP");
        break;
    case (IP_EVENT_GOT_IP6):
        ip_event_got_ip6_t *event_ip6 = (ip_event_got_ip6_t *)event_data;
        ESP_LOGI(TAG, "Got IPv6: " IPV6STR, IPV62STR(event_ip6->ip6_info.ip));
        wifi_retry_count = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        ESP_LOGI(TAG, "IP event not handled");
        break;
    }
}

static void wifi_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Handling Wi-Fi event, event code 0x%" PRIx32, event_id);

    wifi_status status;

    switch (event_id)
    {
    case (WIFI_EVENT_WIFI_READY):
        ESP_LOGI(TAG, "Wi-Fi ready");
        break;
    case (WIFI_EVENT_SCAN_DONE):
        ESP_LOGI(TAG, "Wi-Fi scan done");
        status = WIFI_STATUS_SCAN_DONE;
        xQueueSend(event_queue, &status, portMAX_DELAY);
        break;
    case (WIFI_EVENT_STA_START):
        ESP_LOGI(TAG, "Wi-Fi started, connecting to AP...");
        // esp_wifi_connect();
        break;
    case (WIFI_EVENT_STA_STOP):
        ESP_LOGI(TAG, "Wi-Fi stopped");
        break;
    case (WIFI_EVENT_STA_CONNECTED):
        ESP_LOGI(TAG, "Wi-Fi connected");
        status = WIFI_STATUS_CONNECTED;
        xQueueSend(event_queue, &status, portMAX_DELAY);
        break;
    case (WIFI_EVENT_STA_DISCONNECTED):
        ESP_LOGI(TAG, "Wi-Fi disconnected");

        break;
    case (WIFI_EVENT_STA_AUTHMODE_CHANGE):
        ESP_LOGI(TAG, "Wi-Fi authmode changed");
        break;
    default:
        ESP_LOGI(TAG, "Wi-Fi event not handled");
        break;
    }
}

esp_err_t WiFi_Initialize(wifi_data *w_data)
{
    esp_err_t ret_value = nvs_flash_init();

    if (ret_value == ESP_ERR_NVS_NO_FREE_PAGES || ret_value == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret_value = nvs_flash_init();
    }

    wifi_event_group = xEventGroupCreate();

    ret_value = esp_netif_init();
    if (ret_value != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize TCP/IP network stack");
        return ret_value;
    }

    ret_value = esp_event_loop_create_default();
    if (ret_value != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create default event loop");
        return ret_value;
    }

    netif = esp_netif_create_default_wifi_sta();
    if (netif == NULL)
    {
        ESP_LOGE(TAG, "Failed to create default WiFi STA interface");
        return ESP_FAIL;
    }

    event_queue = xQueueCreate(1, sizeof(wifi_status));

    if (event_queue == NULL)
    {
        ESP_LOGI(TAG, "Failed to create queue!");
    }

    wifi_queue = xQueueCreate(1, sizeof(wifi_data));

    if (wifi_queue == NULL)
    {
        ESP_LOGI(TAG, "Failed to create queue!");
    }

    // Wi-Fi stack configuration parameters
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_cb, NULL, &wifi_event));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_cb, NULL, &ip_event));
    return ESP_OK;
}

esp_err_t WiFi_Scan(wifi_data *w_data)
{
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // ESP_LOGI(TAG, "Connection to WiFi network [%s]...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_start());

    vTaskDelay(100);

    wifi_scan_config_t scan_config = {0};

    uint16_t max_number = 10;
    uint16_t number = 0;

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, 1));

    vTaskDelay(pdMS_TO_TICKS(100));

    esp_wifi_scan_get_ap_num(&number);
    if (number > max_number)
        number = max_number;

    esp_err_t res = esp_wifi_scan_get_ap_records(&number, w_data->ap_info);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to scan wifi");
        return res;
    }

    w_data->number = number;

    for (int i = 0; i < w_data->number; i++)
    {
        ESP_LOGI(TAG, "Hittade: %s (RSSI: %d)", w_data->ap_info[i].ssid, w_data->ap_info[i].rssi);
    }

    return ESP_OK;
}

void WiFi_Work(void *arg)
{
    wifi_status status;
    wifi_data w_data;

    while (1)
    {

        if (xQueueReceive(wifi_queue, &w_data, portMAX_DELAY))
        {
            switch (w_data.cmd)
            {
            case WIFI_CMD_SCAN:
                WiFi_Scan(&w_data);
                if (xQueueReceive(event_queue, &status, portMAX_DELAY))
                {
                    if (status == WIFI_STATUS_SCAN_DONE)
                    {
                        w_data.status = status;
                        xQueueSend(wifi_queue, &w_data, portMAX_DELAY);
                    }
                }
                break;
            case WIFI_CMD_CONNECT:
                WiFi_Connect(&w_data);
                if (xQueueReceive(event_queue, &status, portMAX_DELAY))
                {
                    if (status == WIFI_STATUS_CONNECTED)
                    {
                        w_data.status = status;
                        xQueueSend(wifi_queue, &w_data, portMAX_DELAY);
                    }
                }
                break;
            case WIFI_CMD_DISCONNECT:
                break;
            }
            // ESP_LOGI(TAG, "WiFi Work: %d", wifi->number);
            // xQueueSend(wifi_queue, wifi, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

esp_err_t WiFi_Connect(wifi_data *w_data)
{
    wifi_config_t wifi_config = {0};

    strcpy((char *)wifi_config.sta.ssid, w_data->wifi_info.ssid);
    strcpy((char *)wifi_config.sta.password, w_data->wifi_info.password);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_err_t err = esp_wifi_connect();

    if (err != ESP_OK)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t WiFi_Disconnect(void)
{
    if (wifi_event_group)
    {
        vEventGroupDelete(wifi_event_group);
    }

    return esp_wifi_disconnect();
}

esp_err_t WiFi_Dispose(void)
{
    esp_err_t ret_value = esp_wifi_stop();
    if (ret_value == ESP_ERR_WIFI_NOT_INIT)
    {
        ESP_LOGE(TAG, "WiFi not initialize!");
        return ret_value;
    }

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(netif));
    esp_netif_destroy(netif);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, ip_event));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event));

    return ESP_OK;
}