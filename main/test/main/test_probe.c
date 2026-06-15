/**
 * @file test_probe.c
 * @brief Test linkage probe for forcing selected test objects into the build.
 *
 * This file references test entry points so the linker retains the corresponding
 * objects during test builds.
 *
 * @ingroup TEST
 */

#include "unity.h"

/**
 * @brief Forces the recommendation test object to link.
 */
void force_link_recommendation(void);
/**
 * @brief Forces the dataparser price test object to link.
 */
void force_link_dataparser_price(void);
/**
 * @brief Forces the dataparser recommendation test object to link.
 */
void force_link_dataparser_recommendation(void);
/**
 * @brief Forces the dataparser weather test object to link.
 */
void force_link_dataparser_weather(void);

/**
 * @brief Calls the link probe functions used by the test build.
 */
void force_link_all_tests(void)
{
    force_link_recommendation();
    force_link_dataparser_price();
    force_link_dataparser_recommendation();
    force_link_dataparser_weather();
}
