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

typedef enum {
    RECOMMENDATION_BUY = 1,
    RECOMMENDATION_SELL = 2,
    RECOMMENDATION_HOLD = 3, 
    RECOMMENDATION_INVALID = 0,
    RECOMMENDATION_ERROR = -1
} recommendation_type_t;

typedef struct {
    char timestamp[20]; // 20 chars long for "YYYY-MM-DD HH:MM" format
    float recommendation;  // Normaliserad data 0-1
    recommendation_type_t recommendation_type; // BUY/HOLD/SELL//INVALID/ERROR
} leop_entry_t;

// Senaste LEOP data
typedef struct {
    int id;
    leop_entry_t entries[128];
    uint32_t entry_count;

} leop_data_t;

// Senaste BME280-readings
// float eller double?
typedef struct {
    bool valid;
    uint32_t last_update_seconds;
    time_t last_unix_time;
    bool wall_time_valid;
    double temperature;
    double pressure;
    double humidity;
} sensor_data_t;



// App config, med runtime values som tex internal på fetch, dvs hur ofta kunden sätter att klienten ska uppdatera sin data från server.
typedef struct {
    uint32_t fetch_interval_minutes; // TODO - Default should be 15 unless changed
    bool test_mode;
    uint32_t sensor_interval_ms; //Default to once a second

} config_data_t;

// State 
typedef struct {
    bool wifi_connected;
    bool leop_connected;
    bool sensor_ok;
    uint32_t update_counter;
} system_status_t;

typedef struct {
    const char * name;
    TaskHandle_t handle;
    uint32_t stack_size;
} task_info_t;


typedef struct {
    task_info_t wifi_task;
    task_info_t ui_task;
    task_info_t uart_task;
    task_info_t sensor_task;
    task_info_t leop_task;
} system_task_handlers_t;


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