#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"
#include "esp_log.h"
#include <inttypes.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdbool.h>

extern QueueHandle_t wifi_queue;

typedef enum
{
    WIFI_CMD_SCAN,
    WIFI_CMD_CONNECT,
    WIFI_CMD_DISCONNECT,
} wifi_cmd_t;

typedef enum
{
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_SCAN_DONE,
    WIFI_STATUS_DISCONNECTED,
} wifi_status;

typedef struct
{
    char *ssid;
    char *password;
} wifi_info;

typedef struct
{
    wifi_cmd_t cmd;
    wifi_status status;
    uint16_t number;
    wifi_ap_record_t ap_info[10];
    wifi_info wifi_info;
} wifi_data;

esp_err_t WiFi_Initialize(wifi_data *w_data);

esp_err_t WiFi_Scan(wifi_data *w_data);

void WiFi_Work(void *arg);

esp_err_t WiFi_Connect(wifi_data *w_data);

esp_err_t WiFi_Disconnect(void);

esp_err_t WiFi_Dispose(void);

#endif