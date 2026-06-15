/**
 * @file fake_config.cpp
 * @brief Implementation of the fake configuration helper.
 *
 * @ingroup FAKE_CONFIG
 */

#include "fake_config.hpp"

/**
 * @brief Implementation of fake_config_data.
 *
 * See header for full contract documentation.
 */
void fake_config_data(config_data_t* config) {
    config->fetch_interval_minutes = 1;
    config->test_mode = false;
    config->sensor_interval_ms = 1000; 
}
