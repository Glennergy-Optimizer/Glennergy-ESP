#ifndef FAKE_SENSOR_HPP
#define FAKE_SENSOR_HPP

#include "../app_types.h"

/**
 * @file fake_sensor.hpp
 * @brief Public interface for the fake sensor test helper.
 *
 * Provides simple fill and update helpers for synthetic sensor data used by
 * the sensor module.
 *
 * @ingroup SENSOR
 */

/**
 * @defgroup SENSOR SENSOR
 * @brief Sensor-related helpers and data handling.
 *
 * Includes synthetic sensor helpers used to seed and update sensor data for
 * testing or simulation paths.
 * @{
 */

/**
 * @brief Initializes synthetic sensor data with baseline values.
 *
 * @param[in,out] sensor Pointer to the sensor data structure to populate.
 */
void fake_sensor_fill(sensor_data_t* sensor);

/**
 * @brief Updates synthetic sensor data with a drifting sample.
 *
 * @param[in,out] sensor Pointer to the sensor data structure to update.
 */
void fake_sensor_update(sensor_data_t* sensor);

/** @} */

#endif
