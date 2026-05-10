//#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fake_sensor.hpp"
#include "sensor.h"
#include "esp_log.h"
#include "../app_queues.h"

static const char* TAG = "Sensor.cpp";
// Reminder, static so only this file can see it
static bool fake_mode = true;



QueueHandle_t Sensor_Queue = NULL;


void Sensor_Init(app_state_t* app)
{
    // Do it matter if we have fake or true when initializing?
    if (fake_mode == true)
    {
        fake_sensor_fill(&app->sensor_data);
    }
    else {
        // case of true data

    }

    Sensor_Queue = xQueueCreate(1, sizeof(sensor_data_t));

    if (Sensor_Queue == NULL)
    {
        ESP_LOGI(TAG, "Failed to create sensor queue!");
    }

}

// Fake-data for now, should use i2C to read BME280 later
// True on success, false on failure to let consumer know if we successfully read valid data or failed to read.
bool Sensor_Read(sensor_data_t* sensor)
{
    if (fake_mode == true)
    {
        fake_sensor_update(sensor);
        xQueueSend(Sensor_Queue, sensor, portMAX_DELAY);
        return true;
    }
    else {
        // real sensor data
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