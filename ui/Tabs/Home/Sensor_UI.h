#ifndef SENSOR_UI_H
#define SENSOR_UI_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t *arc_temperature_dyn;
    lv_obj_t *arc_humidity_dyn;
    lv_obj_t *arc_pressure_dyn;
    lv_obj_t *temperature_label_dyn;
    lv_obj_t *pressure_label_dyn;
    lv_obj_t *humidity_label_dyn;
} Sensor_UI;

void Sensor_UI_Initialize();

void Sensor_UI_Update(void);

#endif