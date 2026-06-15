/**
 * @file test_dataparser_recommendation.c
 * @brief Unit tests for DataParser recommendation parsing.
 *
 * @ingroup DataParser
 */

#include "unity.h"
#include "../../JSONParser/DataParser.h"

static const char *valid_recommendation_json =
    "["
    "{\"id\":1,\"type\":0.5,\"timestamp\":\"2026-01-01\"},"
    "{\"id\":2,\"type\":0.8,\"timestamp\":\"2026-01-02\"}"
    "]";

static const char *invalid_json = "{not valid json";

static const char *not_array_json = "{\"id\":1}";

static const char *empty_array_json = "[]";

/**
 * @brief Keeps the translation unit linked for test discovery.
 */
void force_link_dataparser_recommendation(void)
{

}

/**
 * @brief Verifies that valid recommendation JSON is parsed into a list.
 */
TEST_CASE("DataParser recommendation valid", "[DataParser]")
{
    RecommendationList list = {0};

    int result = DataParser_ParseRecommendation(valid_recommendation_json, &list);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(2, list.count);

    TEST_ASSERT_EQUAL_INT(1, list.rec[0].id);
    TEST_ASSERT_EQUAL_DOUBLE(0.5, list.rec[0].recommendation);
    TEST_ASSERT_EQUAL_STRING("2026-01-01", list.rec[0].timestamp);

    TEST_ASSERT_EQUAL_INT(2, list.rec[1].id);
    TEST_ASSERT_EQUAL_DOUBLE(0.8, list.rec[1].recommendation);
    TEST_ASSERT_EQUAL_STRING("2026-01-02", list.rec[1].timestamp);
}


/**
 * @brief Verifies that invalid JSON is rejected.
 */
TEST_CASE("DataParser recommendation invalid", "[DataParser]")
{
    RecommendationList list = {0};

    int result = DataParser_ParseRecommendation(invalid_json, &list);

    TEST_ASSERT_EQUAL_INT(1, result);
}

/**
 * @brief Verifies that non-array JSON is rejected.
 */
TEST_CASE("DataParser recommendation bot array", "[DataParser]")
{
    RecommendationList list = {0};

    int result = DataParser_ParseRecommendation(not_array_json, &list);

    TEST_ASSERT_EQUAL_INT(2, result);
}

/**
 * @brief Verifies that an empty array is rejected.
 */
TEST_CASE("DataParser recommendation empty array", "[DataParser]")
{
    RecommendationList list = {0};

    int result = DataParser_ParseRecommendation(empty_array_json, &list);

    TEST_ASSERT_EQUAL_INT(3, result);
}
