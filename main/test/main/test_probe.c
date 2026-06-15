/**
 * @file test_probe.c
 * @brief Test probe used to force-link selected test objects.
 *
 * @ingroup TEST
 */

#include "unity.h"

/**
 * @brief Forces the recommendation test object to be linked.
 */
void force_link_recommendation(void);
/**
 * @brief Forces the price parser test object to be linked.
 */
void force_link_dataparser_price(void);
/**
 * @brief Forces the recommendation parser test object to be linked.
 */
void force_link_dataparser_recommendation(void);
/**
 * @brief Forces the weather parser test object to be linked.
 */
void force_link_dataparser_weather(void);

/**
 * @brief Calls the force-link helpers for all referenced test objects.
 *
 * This file exists to keep the linker from discarding selected test code.
 */
void force_link_all_tests(void)
{
    force_link_recommendation();
    force_link_dataparser_price();
    force_link_dataparser_recommendation();
    force_link_dataparser_weather();
}
