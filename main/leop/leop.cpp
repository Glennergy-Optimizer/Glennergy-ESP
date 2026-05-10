#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fake_leop.hpp"
#include "leop.h"

static const char *TAG = "leop";

static bool fake_mode = true;

void Leop_Init(app_state_t* app)
{
    if (fake_mode == true)
    {
        fake_leop_fill(&app->leop_data);
    }
}

bool Leop_GetData(leop_data_t* leop)
{
    if (fake_mode == true)
    {
        // TODO - fake_leop_update(leop);
        return true;
    }

    return false;
}

void Leop_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;

    Leop_Init(app);

}
