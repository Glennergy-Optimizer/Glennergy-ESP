#include "unity.h"

void force_link_recommendation(void);
void force_link_dataparser_price(void);
void force_link_dataparser_recommendation(void);
void force_link_dataparser_weather(void);

void force_link_all_tests(void)
{
    force_link_recommendation();
    force_link_dataparser_price();
    force_link_dataparser_recommendation();
    force_link_dataparser_weather();
}