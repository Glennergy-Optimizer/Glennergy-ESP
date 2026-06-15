#include "unity.h"
#include "../../JSONParser/DataParser.h"

static const char *price_json =
    "["
    "{\"price SEK\":10.5,\"timestamp\":\"2026-01-01\"},"
    "{\"price SEK\":12.0,\"timestamp\":\"2026-01-02\"}"
    "]";
static const char *invalid_json = "{not valid json";

static const char *not_array_json = "{\"id\":1}";

static const char *empty_array_json = "[]";

void force_link_dataparser_price(void)
{

}

TEST_CASE("DataParser price valid", "[DataParser]")
{
    PriceList list = {0};

    int result = DataParser_ParsePrice(price_json, &list);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(2, list.count);

    TEST_ASSERT_EQUAL_DOUBLE(10.5, list.price[0].current_prices);
    TEST_ASSERT_EQUAL_STRING("2026-01-01", list.price[0].timestamp);

    TEST_ASSERT_EQUAL_DOUBLE(12.0, list.price[1].current_prices);
    TEST_ASSERT_EQUAL_STRING("2026-01-02", list.price[1].timestamp);
}

TEST_CASE("DataParser price invalid", "[DataParser]")
{
    PriceList list = {0};

    int result = DataParser_ParsePrice(invalid_json, &list);

    TEST_ASSERT_EQUAL_INT(1, result);
}

TEST_CASE("DataParser price not array", "[DataParser]")
{
    PriceList list = {0};

    int result = DataParser_ParsePrice(not_array_json, &list);

    TEST_ASSERT_EQUAL_INT(2, result);
}

