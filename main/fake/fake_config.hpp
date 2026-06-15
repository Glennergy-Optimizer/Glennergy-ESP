#ifndef FAKE_CONFIG_HPP
#define FAKE_CONFIG_HPP

#include "../app_types.h"

/**
 * @file fake_config.hpp
 * @brief Public interface for fake configuration defaults.
 *
 * Provides the helper used by the fake configuration module to populate a
 * config_data_t structure with test values.
 *
 * @defgroup FAKE_CONFIG Fake configuration
 * @brief Helpers for populating test configuration data.
 *
 * This module is used by the fake configuration path to set deterministic
 * values for testing and development.
 * @{
 */

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Populates a configuration structure with fake defaults.
 *
 * @param[in,out] config Pointer to the configuration structure to update.
 */
void fake_config_data(config_data_t* config);

#ifdef __cplusplus
    }
#endif

/** @} */

#endif
