/**
 * @file main/test/main/test.c
 * @brief Test application entry point for Unity-based test execution.
 *
 * @ingroup TEST
 *
 * Links the test suite, starts Unity, runs all registered tests, and finalizes
 * the test session.
 *
 * @note Intended for test execution in task context during firmware bring-up or CI runs.
 */

#include "unity.h"

extern void force_link_all_tests(void);

/**
 * @brief Application entry point for the test runner.
 *
 * Forces test linkage, runs the Unity test suite, and closes the session.
 */
void app_main(void)
{
    force_link_all_tests();
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}
