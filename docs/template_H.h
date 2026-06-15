#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

/**
 * @file Template.h
 * @brief Public API for the <MODULENAME> module.
 *
 * Provides the public types and functions for <short embedded module purpose>.
 */

/**
 * @defgroup <MODULENAME> <MODULENAME>
 * @brief <Short embedded module description>
 *
 * <Describe what the module controls or coordinates, plus any important
 * initialization, hardware, or runtime constraints.>
 *
 * @note Add notes only when they communicate meaningful context, blocking,
 * lifecycle, units, or hardware behavior.
 * @{
 */

/**
 * @brief Example state snapshot for <what the struct models>.
 *
 * Describe key semantics at the struct level and keep individual field
 * comments short unless a field is subtle or safety-critical.
 */
typedef struct {
    uint32_t sample_count; /**< Number of valid samples processed. */
    float value;           /**< Latest processed value in project-defined units. */
} ExampleState;

/**
 * @brief Initializes the module.
 *
 * @param[in] config_value Example configuration value.
 *
 * @return
 * - `ESP_OK` on success
 * - an ESP-IDF error code on failure
 *
 * @pre Call from task context during initialization.
 * @note Add context, blocking, or hardware notes only when useful.
 */
esp_err_t Example_Initialize(uint32_t config_value);

/**
 * @brief Prints ExampleState for debugging.
 *
 * @param state Pointer to state to print.
 */
void ExampleState_Print(const ExampleState *state);

/** @} */

#endif
