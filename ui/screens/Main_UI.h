#ifndef MAIN_UI_H
#define MAIN_UI_H
#include "../../main/WiFi.h"
#include "../../main/app_queues.h"
#include "../app_types.h"

extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Glennergy_Label;
extern lv_obj_t * ui_Tab_Main;
extern lv_obj_t * ui_TabPage_Home;
extern lv_obj_t * ui_TabPage_Electricity;
extern lv_obj_t * ui_TabPage_Weather;
extern lv_obj_t * ui_Chart_Weather;
extern lv_obj_t * ui_TabPage_Settings;
extern lv_obj_t * ui_Group_Settings; 
extern lv_obj_t * ui_TabPage_WiFi;


void Main_UI_Initialize();

#endif