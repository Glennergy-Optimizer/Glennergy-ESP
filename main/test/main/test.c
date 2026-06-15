/**
 * @file test.c
 * @brief Application entry point for the Unity test runner.
 *
 * @ingroup APP
 *
 * Links and executes the full test suite during startup.
 *
 * @note Runs in task context as part of the application entry point.
 */

#include "unity.h"

extern void force_link_all_tests(void);

/**
 * @brief Application entry point for the test application.
 *
 * Forces test linkage, then runs the Unity test suite.
 */
void app_main(void)
{
    force_link_all_tests();
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}
