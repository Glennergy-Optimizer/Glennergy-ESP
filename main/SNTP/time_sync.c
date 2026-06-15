/**
 * @file time_sync.c
 * @brief Implementation of the SNTP time synchronization module.
 *
 * @ingroup SNTP
 */

#include "time_sync.h"

/**
 * @brief Implementation of TimeSync_Start.
 *
 * See header for full contract documentation.
 */
esp_err_t TimeSync_Start() {
    // Init the default SNTP config 
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");

    // Init and start the SNTP service
    esp_err_t sntp_result = esp_netif_sntp_init(&config);

    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) == ESP_OK) {
        ESP_LOGI("SNTP", "Time successfully syncronised.");
    }
    else {
        ESP_LOGI("SNTP", "Could not successfully sync time.");
    }

    return sntp_result;
}
