#ifndef WEATHER_UI_H
#define WEATHER_UI_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t *hourLabel[24];
    lv_obj_t *tempLabel[24];
    lv_obj_t *iconLabel[24];
    lv_obj_t *weather_code[24];
} Weather_UI;

typedef struct
{
    lv_obj_t *row;
    lv_obj_t *icon;
    lv_obj_t *temp;
    lv_obj_t *time;
} forecast_row_t;

typedef struct
{
    lv_obj_t *icon;
    lv_obj_t *current_temp;
    lv_obj_t *current_weather;
    lv_obj_t *forecast_list;

    lv_obj_t* left_card;
    lv_obj_t* right_card;

    lv_obj_t *forecast_rows1[24];
    lv_obj_t *forecast_temp[24];
    lv_obj_t *forecast_time[24];
    lv_obj_t *forecast_icon[24];

    forecast_row_t forecast_rows[24];

} Weather_UI_test;

void Weather_UI_Initialize();

void Weather_UI_Update();

void Weather_UI_Update_test();

void weather_dashboard_create(void);

#endif