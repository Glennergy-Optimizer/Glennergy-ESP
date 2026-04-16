#ifndef WIFI_H
#define WIFI_H

#include "esp_err.h"
#include "esp_log.h"
#include <inttypes.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "freertos/FreeRTOS.h"

typedef struct
{
    uint16_t number;
    wifi_ap_record_t ap_info[10]
} WiFi;

esp_err_t WiFi_Initialize(WiFi* _WiFi);

esp_err_t WiFi_Scan(WiFi *_Wifi);

esp_err_t WiFi_Connect(char *ssid, char *password);

esp_err_t WiFi_Disconnect(void);

esp_err_t WiFi_Dispose(void);

#endif