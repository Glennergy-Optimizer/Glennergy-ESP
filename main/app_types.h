#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <task.h>
#include "Price.h"
#include "Recommendation.h"
#include "LEOP_Fetcher.h"
#include "Weather.h"

/**
 * @file app_types.h
 * @brief Shared application data types for system state, configuration, and task handles.
 *
 * Defines the common data structures used by the application to exchange
 * recommendation, sensor, configuration, and task state.
 *
 * @ingroup APP
 */

/**
 * @brief Application data types used across modules.
 *
 * Groups the shared state containers and task handle metadata used by the
 * application runtime.
 *
 * @note These types are intended for cross-module state sharing in task context.
 */

/**
 * @brief Recommendation classification values used by the application.
 */
typedef enum {
    RECOMMENDATION_BUY = 1,
    RECOMMENDATION_SELL = 2,
    RECOMMENDATION_HOLD = 3, 
    RECOMMENDATION_INVALID = 0,
    RECOMMENDATION_ERROR = -1
} recommendation_type_t;

/**
 * @brief One LEOP recommendation entry.
 *
 * Stores the formatted timestamp, normalized recommendation value, and the
 * classification used by the application.
 */
typedef struct {
    char timestamp[20]; /**< Timestamp string in "YYYY-MM-DD HH:MM" format. */
    float recommendation; /**< Normalized recommendation value in the 0 to 1 range. */
    recommendation_type_t recommendation_type; /**< BUY, HOLD, SELL, INVALID, or ERROR. */
} leop_entry_t;

/**
 * @brief Latest LEOP data snapshot.
 *
 * Holds the most recent LEOP identifier and up to 128 entries.
 */
typedef struct {
    int id;
    leop_entry_t entries[128];
    uint32_t entry_count;

} leop_data_t;

/**
 * @brief Latest BME280 sensor readings and validity flags.
 *
 * Contains the last sampled values together with time metadata that indicates
 * whether wall-clock time is available.
 */
typedef struct {
    bool valid;
    uint32_t last_update_seconds;
    time_t last_unix_time;
    bool wall_time_valid;
    double temperature;
    double pressure;
    double humidity;
} sensor_data_t;



/**
 * @brief Runtime application configuration.
 *
 * Stores fetch timing and sensor update interval values that can be adjusted
 * while the application is running.
 */
typedef struct {
    uint32_t fetch_interval_minutes; /**< Fetch interval in minutes. */
    bool test_mode;
    uint32_t sensor_interval_ms; /**< Sensor interval in milliseconds. */

} config_data_t;

/**
 * @brief Summary of system status flags.
 *
 * Tracks connectivity and sensor health together with a simple update counter.
 */
typedef struct {
    bool wifi_connected;
    bool leop_connected;
    bool sensor_ok;
    uint32_t update_counter;
} system_status_t;

/**
 * @brief Metadata for a FreeRTOS task.
 *
 * Stores the task name, handle, and stack allocation used by the application.
 */
typedef struct {
    const char * name;
    TaskHandle_t handle;
    uint32_t stack_size;
} task_info_t;


/**
 * @brief Collection of application task handles.
 *
 * Provides access to the known background tasks managed by the application.
 */
typedef struct {
    task_info_t wifi_task;
    task_info_t ui_task;
    task_info_t uart_task;
    task_info_t sensor_task;
    task_info_t leop_task;
} system_task_handlers_t;


/**
 * @brief Shared application state owned by the main application flow.
 *
 * Main creates this structure and passes it to modules that read or update
 * their respective state areas.
 *
 * @note Access patterns depend on the module; shared-state updates should be
 * coordinated by the application.
 */
// I think the flow should be:
// Main creates this and passes the access to this ot the other modules.
// UART only writes to small safe area first, like app_config_t, and eventually other modules should update their own parts later.
// So main creates it, the uart_diag_shell should parse commands, and reads app_state_t, possibly update config
// Then the leop-part should update leop_data_t, the sensor part should update sensor_data_t etc
typedef struct {
    LEOPData leop_data;
    sensor_data_t sensor_data;
    config_data_t config_data;
    system_status_t system_status;
    system_task_handlers_t system_task_handlers;
} app_state_t;

#endif
