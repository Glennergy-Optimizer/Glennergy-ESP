#include "unity.h"

extern void force_link_all_tests(void);

void app_main(void)
{
    force_link_all_tests();
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}