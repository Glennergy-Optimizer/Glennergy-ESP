#ifndef UART_DIAG_SHELL_HPP
#define UART_DIAG_SHELL_HPP

#include <string>
#include "fake_sensor.h"

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

// Senaste LEOP datan,
// Många fel:
// - Stack överflow, troligtvis en mix av följande anledningar:
// - 3 separata arrayer istället för en array av 128 entries, SoA vs AoS
// - std::string tar mycket, kan köra med char eller enum?
// - float tar mindre än double
// typedef struct {
//     int id;
//     double recommendation[128]; // Normaliserad data 0-1
//     std::string recommendation_type[128]; // BUY/HOLD/SELL
//     std::string time[128];  //Timestamp string (YYYY-MM-DD HH:MM)
// } leop_data_t;


// Senaste LEOP data
typedef struct {
    int id;
    leop_entry_t entries[128];
    uint32_t entry_count;

} leop_data_t;

// Senaste BME280-readings
// float eller double?



// App config, med runtime values som tex internal på fetch, dvs hur ofta kunden sätter att klienten ska uppdatera sin data från server.
typedef struct {
    uint32_t fetch_interval_minutes; // TODO - Default should be 15 unless changed
    bool test_mode;

} config_data_t;

// State 
typedef struct {
    bool wifi_connected;
    bool leop_connected;
    bool sensor_ok;
    uint32_t uptime_seconds;
} system_status_t;


// I think the flow should be:
// Main creates this and passes the access to this ot the other modules.
// UART only writes to small safe area first, like app_config_t, and eventually other modules should update their own parts later.
// So main creates it, the uart_diag_shell should parse commands, and reads app_state_t, possibly update config
// Then the leop-part should update leop_data_t, the sensor part should update sensor_data_t etc
typedef struct {
    leop_data_t leop_data;
    sensor_data_t sensor_data;
    config_data_t config_data;
    system_status_t system_status;
} app_state_t;

// So we need a function which reads the app state.
void handle_input(const std::string& input, app_state_t* state);

#endif
