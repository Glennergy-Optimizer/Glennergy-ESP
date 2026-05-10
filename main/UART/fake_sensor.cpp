#include <iostream>
#include "fake_sensor.hpp"
#include "Queues.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "../app_types.h"


static const char* TAG = "Sensor";

static int counter = 0;
static int temp_direction = 1;
static int humidity_direction = 1;

QueueHandle_t Sensor_Queue = NULL;

void fake_sensor_fill(sensor_data_t* sensor) {
    sensor->temperature = 5;
    sensor->humidity = 4;
    sensor->pressure = 80;
    sensor->valid = false;    
    sensor->last_update_seconds = 0;

    Sensor_Queue = xQueueCreate(1, sizeof(sensor_data_t));
    
    if (Sensor_Queue == NULL)
    {
        ESP_LOGI(TAG, "Failed to create sensor queue!");
    }
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

    xQueueSend(Sensor_Queue, sensor, portMAX_DELAY);
}