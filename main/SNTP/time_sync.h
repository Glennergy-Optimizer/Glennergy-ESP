#ifndef TIME_SYNC_H
#define TIME_SYNC_H

#include "esp_check.h"
#include "esp_netif.h"
#include "esp_sntp.h"
#include "esp_netif_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t TimeSync_Start();
bool TimeSync_IsSynced();


#endif