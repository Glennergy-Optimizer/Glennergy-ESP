#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../UART/fake_sensor.hpp"
#include "sensor.h"

// Reminder, static so only this file can see it
static bool fake_mode = true;

void Sensor_Init(sensor_data_t* sensor);



void Sensor_Init(app_state_t* app)
{
    if (fake_mode == true)
    {
        fake_sensor_fill(&app->sensor_data);
    }

}

// Fake-data for now, should use i2C to read BME280 later
// True on success, false on failure to let consumer know if we successfully read valid data or failed to read.
bool Sensor_Read(sensor_data_t* sensor)
{
    if (fake_mode == true)
    {
        fake_sensor_update(sensor);
        return true;
    }

    return false;
}


void Sensor_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;

    Sensor_Init(app);

    while (1) {
        Sensor_Read(&app->sensor_data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}