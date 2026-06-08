#ifndef PRICE_UI_H
#define PRICE_UI_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t* price_panel;
    lv_obj_t *hourLabel[24];
    lv_obj_t *priceLabel[24];
} Price_UI;

void Price_UI_Initialize();

void Price_UI_Update();

#endif