/**
 * @file main/test/main/test_dataparser_weather.c
 * @brief Unity test cases for DataParser weather JSON parsing.
 *
 * Exercises valid, invalid, non-array, and empty-array inputs for the weather
 * parser.
 */

#include "unity.h"
#include "../../JSONParser/DataParser.h"

static const char *weather_json =
    "["
    "{\"temp\":21.5,\"uv_index\":3,\"weather_code\":100,\"timestamp\":\"2026-01-01\"},"
    "{\"temp\":18.0,\"uv_index\":1,\"weather_code\":200,\"timestamp\":\"2026-01-02\"}"
    "]";

static const char *invalid_json = "{not valid json";

static const char *not_array_json = "{\"id\":1}";

static const char *empty_array_json = "[]";

/**
 * @brief Forces the linker to include the weather parser test object.
 */
void force_link_dataparser_weather(void)
{

}

/**
 * @brief Verifies that valid weather JSON is parsed successfully.
 */
TEST_CASE("DataParser weather valid", "[DataParser]")
{
    WeatherList list = {0};

    int result = DataParser_ParseWeather(weather_json, &list);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(2, list.count);

    TEST_ASSERT_EQUAL_DOUBLE(21.5, list.weather[0].temp);
    TEST_ASSERT_EQUAL_INT(3, list.weather[0].uv_index);
    TEST_ASSERT_EQUAL_INT(100, list.weather[0].weather_code);
    TEST_ASSERT_EQUAL_STRING("2026-01-01", list.weather[0].timestamp);
}

/**
 * @brief Verifies that invalid weather JSON is rejected.
 */
TEST_CASE("DataParser weather invalid", "[DataParser]")
{
    WeatherList list = {0};

    int result = DataParser_ParseWeather(invalid_json, &list);

    TEST_ASSERT_EQUAL_INT(1, result);
}

/**
 * @brief Verifies that non-array weather JSON is rejected.
 */
TEST_CASE("DataParser weather not array", "[DataParser]")
{
    WeatherList list = {0};

    int result = DataParser_ParseWeather(not_array_json, &list);

    TEST_ASSERT_EQUAL_INT(2, result);
}

/**
 * @brief Verifies that an empty weather array is rejected.
 */
TEST_CASE("DataParser weather empty array", "[DataParser]")
{
    WeatherList list = {0};

    int result = DataParser_ParseWeather(empty_array_json, &list);

    TEST_ASSERT_EQUAL_INT(3, result);
}
