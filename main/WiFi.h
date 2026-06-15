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

/**
 * @file WiFi.h
 * @brief Public API for the Wi-Fi module.
 *
 * Provides the command, status, and data types used to control the ESP32 Wi-Fi
 * station workflow and its worker-task handoff.
 *
 * @defgroup WIFI WiFi
 * @brief Wi-Fi control and worker-task interface.
 *
 * The module owns the Wi-Fi station lifecycle, event handling, and queue-based
 * command/result exchange used by the background worker task.
 * @{
 */

extern QueueHandle_t wifi_cmd_queue;

extern QueueHandle_t wifi_result_queue;

/**
 * @brief Wi-Fi commands accepted by the worker task.
 */
typedef enum
{
    WIFI_CMD_START,
    WIFI_CMD_SCAN,
    WIFI_CMD_CONNECT,
    WIFI_CMD_DISCONNECT,
    WIFI_CMD_STOP,
} wifi_cmd_t;

/**
 * @brief Wi-Fi status values exchanged through the module queues.
 */
typedef enum
{
    WIFI_STATUS_INITIALIZED,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_SCAN_DONE,
    WIFI_STATUS_DISCONNECTED,
} wifi_status;

/**
 * @brief Shared Wi-Fi connection state.
 */
typedef struct 
{
    bool is_connected; /**< True when the station is connected. */
}wifi_state;



/**
 * @brief Wi-Fi credentials and scan result payload.
 */
typedef struct
{
    char *ssid; /**< Pointer to the SSID string. */
    char *password; /**< Pointer to the password string. */
} wifi_info;

/**
 * @brief Command payload and result buffer used by the Wi-Fi worker task.
 *
 * The scan result array is limited to 10 access points.
 */
typedef struct
{
    wifi_cmd_t cmd; /**< Command to execute. */
    wifi_status status; /**< Result status reported by the worker. */
    uint16_t number; /**< Number of valid entries in ap_info. */
    wifi_ap_record_t ap_info[10]; /**< Scan results buffer. */
    wifi_info wifi_info; /**< SSID and password used for connection requests. */
} wifi_data;

/**
 * @brief Initializes the Wi-Fi stack and module queues.
 *
 * @return `ESP_OK` on success, or an ESP-IDF error code on failure.
 */
esp_err_t WiFi_Initialize();

/**
 * @brief Wi-Fi worker task entry point.
 *
 * Waits for commands on wifi_cmd_queue and publishes results to
 * wifi_result_queue.
 *
 * @param[in] arg Task argument, currently unused.
 *
 * @note Runs in task context and blocks on queue operations and delays.
 */
void WiFi_Work(void *arg);

/**
 * @brief Starts a Wi-Fi connection attempt using the provided credentials.
 *
 * @param[in] w_data Connection request containing SSID and password.
 *
 * @return `ESP_OK` on success, or `ESP_FAIL` if the connection request fails.
 */
esp_err_t WiFi_Connect(wifi_data *w_data);

/**
 * @brief Returns the current Wi-Fi connection state.
 *
 * @return `true` when connected, otherwise `false`.
 */
bool WiFi_IsConnected();

/**
 * @brief Disconnects the Wi-Fi station.
 *
 * @return `ESP_OK` on success, or an ESP-IDF error code on failure.
 */
esp_err_t WiFi_Disconnect(void);

/**
 * @brief Releases Wi-Fi resources and unregisters handlers.
 *
 * @return `ESP_OK` on success, or an ESP-IDF error code on failure.
 */
esp_err_t WiFi_Dispose(void);

/** @} */

#endif
