#ifndef ELECTRICITY_UI_H
#define ELECTRICITY_UI_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t* ui_Chart_Electricity;
} Electricity_UI;

void Electricity_UI_Initialize();

void Electricity_UI_Update(void);

#endif