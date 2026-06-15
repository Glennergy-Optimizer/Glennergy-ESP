#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "../app_types.h"

/**
 * @file sensor.h
 * @brief Public interface for the sensor worker task.
 *
 * Declares the FreeRTOS task entry point used by the sensor module.
 *
 * @ingroup SENSOR
 */

// sensor.h needs to be a normal header since our main.c/program is in C.
// So sensor.cpp includes this, but Extern C tells compiler to use C linkage instead of C++ linkage
#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Sensor worker task entry point.
 *
 * Runs in FreeRTOS task context and performs the sensor module work loop.
 *
 * @param[in] parameter Pointer to the application state passed to the task.
 */
void Sensor_Work(void* parameter);

#ifdef __cplusplus
    }
#endif


#endif
