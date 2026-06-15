/**
 * @file main/test/main/test_recommendation.c
 * @brief Unity test entry point for the recommendation test group.
 *
 * @ingroup TEST
 */

#include "unity.h"

/**
 * @brief Forces the recommendation test object to link into the test binary.
 *
 * This helper is intentionally empty and exists only to keep the translation
 * unit from being discarded by the linker.
 */
void force_link_recommendation(void)
{
    
}

TEST_CASE("probe", "[probe]")
{
    TEST_ASSERT_TRUE(1);
}
