#ifndef WIFI_UI_H
#define WIFI_UI_H

#include "lvgl.h"

typedef struct{
    lv_obj_t* network_dropdown_dyn;
    lv_obj_t* password_textarea_dyn;
    lv_obj_t* status_label_sta;
    lv_obj_t* status_label_dyn;
    lv_obj_t* scan_button_dyn;
    char selected_ssid[64];
}WiFi_UI;

void WiFi_UI_Initialize();

void WiFi_UI_Update();


#endif