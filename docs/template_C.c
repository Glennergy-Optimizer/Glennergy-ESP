/**
 * @file Template.c
 * @brief Implementation of the <MODULENAME> module.
 *
 * @ingroup <MODULENAME>
 */

#define MODULE_NAME "<MODULENAME>"
#include "<MODULENAME>.h"
#include "esp_log.h"

/**
 * @brief Internal helper function (example).
 *
 * Performs <short description of the helper>.
 *
 * @param[in] input Example input parameter.
 * @return Computed result.
 */
static int InternalHelper(int input)
{
    return input * 2;
}

/**
 * @brief Implementation of Example_Initialize.
 *
 * See header for full contract documentation.
 */
esp_err_t Example_Initialize(uint32_t config_value)
{
    (void)config_value;
    ESP_LOGI(MODULE_NAME, "Example initialize");
    return ESP_OK;
}

/**
 * @brief Prints ExampleState for debugging.
 *
 * @param state Pointer to state to print.
 */
void ExampleState_Print(const ExampleState *state)
{
    if (!state)
    {
        return;
    }

    ESP_LOGI(MODULE_NAME, "count=%lu value=%f", (unsigned long)state->sample_count, (double)state->value);
}
