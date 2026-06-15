#ifndef QUEUES_H
#define QUEUES_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/**
 * @file app_queues.h
 * @brief Shared FreeRTOS queue handles used by the application.
 *
 * Declares queue handles that are created and owned by the application
 * startup code and used by multiple modules for task-to-task communication.
 *
 * @ingroup APP
 */

/**
 * @defgroup APP Application
 * @brief Application-level startup and shared runtime interfaces.
 *
 * Contains application-owned handles and coordination points used across
 * multiple modules.
 * @{
 */

/**
 * @brief Shared queue handle for sensor data.
 *
 * Created during application initialization and used for task-to-task
 * communication.
 */
extern QueueHandle_t Sensor_Queue;

/** @} */

#endif
