#include "fake_sensor.hpp"
#include "esp_timer.h"


static const char* TAG = "Sensor";

static int counter = 0;
static int temp_direction = 1;
static int humidity_direction = 1;


void fake_sensor_fill(sensor_data_t* sensor) {
    sensor->temperature = 5;
    sensor->humidity = 4;
    sensor->pressure = 80;
    sensor->valid = false;    
    sensor->last_update_seconds = 0;
}

void fake_sensor_update(sensor_data_t* sensor) {
    counter += 1;
    sensor->temperature = sensor->temperature + 0.01 * temp_direction;
    sensor->humidity = sensor->humidity + 0.05 * humidity_direction;
    sensor->valid = true;
    sensor->last_update_seconds = esp_timer_get_time() / 1000000ULL;


    // Mock a drifting temperature between 1 and 8 degrees.
    if (sensor-> temperature >= 8) {
        temp_direction = -1;
    }
    if (sensor->temperature <= 1 ) {
        temp_direction = 1;
    }
    
    if (sensor->humidity >= 100) {
        humidity_direction = -1;
    }
    if (sensor->humidity <= 0) {
        humidity_direction = 1;
    }
}