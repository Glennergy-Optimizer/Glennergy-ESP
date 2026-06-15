/**
 * @file TemplateMain.c
 * @brief Application entry point for the <MODULENAME> test or bring-up flow.
 *
 * @ingroup <MODULENAME>
 *
 * Initializes dependencies, runs the embedded integration path, and reports
 * the result.
 *
 * @note Intended for testing, bring-up, or orchestration rather than reusable API logic.
 * @warning Mention blocking I/O or startup-order caveats only when they are truly relevant.
 */

#include "<MODULENAME>.h"
#include "esp_log.h"

static const char *TAG = "TemplateMain";

/**
 * @brief Application entry point.
 *
 * Performs startup and launches the module test path.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Starting example bring-up flow");
    ESP_ERROR_CHECK(Example_Initialize(42));
}
