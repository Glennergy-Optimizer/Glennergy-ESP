#ifndef FAKE_SENSOR_HPP
#define FAKE_SENSOR_HPP

#include "uart_diag_shell.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t Sensor_Queue;


void fake_sensor_fill(sensor_data_t* sensor);

void fake_sensor_update(sensor_data_t* sensor);

#endif