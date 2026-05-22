#ifndef QUEUES_H
#define QUEUES_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t Sensor_Queue;

extern QueueHandle_t Humidity_Queue;

#endif