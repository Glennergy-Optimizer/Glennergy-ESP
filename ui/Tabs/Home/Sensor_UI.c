#include "Sensor_UI.h"
#include "lvgl_port.h"
#include "../../screens/ui_Screen1.h"
#include "../../../main/app_queues.h"
#include "../../../main/sensor/sensor.h"

static Sensor_UI sensor_ui = {
    .arc_humidity_dyn = NULL,
    .arc_pressure_dyn = NULL,
    .arc_temperature_dyn = NULL,
    .humidity_label_dyn = NULL,
    .pressure_label_dyn = NULL,
    .temperature_label_dyn = NULL,
};

void Sensor_UI_Initialize()
{
    sensor_ui.temperature_label_dyn = lv_label_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.temperature_label_dyn, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(sensor_ui.temperature_label_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(sensor_ui.temperature_label_dyn, -321);
    lv_obj_set_y(sensor_ui.temperature_label_dyn, -79);
    lv_obj_set_align(sensor_ui.temperature_label_dyn, LV_ALIGN_CENTER);
    lv_label_set_text(sensor_ui.temperature_label_dyn, "98.76°");
    lv_obj_set_style_text_font(sensor_ui.temperature_label_dyn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    sensor_ui.pressure_label_dyn = lv_label_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.pressure_label_dyn, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(sensor_ui.pressure_label_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(sensor_ui.pressure_label_dyn, 315);
    lv_obj_set_y(sensor_ui.pressure_label_dyn, -77);
    lv_obj_set_align(sensor_ui.pressure_label_dyn, LV_ALIGN_CENTER);
    lv_label_set_text(sensor_ui.pressure_label_dyn, "9999 hPa");
    lv_obj_set_style_text_font(sensor_ui.pressure_label_dyn, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    sensor_ui.humidity_label_dyn = lv_label_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.humidity_label_dyn, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(sensor_ui.humidity_label_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(sensor_ui.humidity_label_dyn, 5);
    lv_obj_set_y(sensor_ui.humidity_label_dyn, -83);
    lv_obj_set_align(sensor_ui.humidity_label_dyn, LV_ALIGN_CENTER);
    lv_label_set_text(sensor_ui.humidity_label_dyn, "99.999%");
    lv_obj_set_style_text_font(sensor_ui.humidity_label_dyn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    sensor_ui.arc_temperature_dyn = lv_arc_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.arc_temperature_dyn, 226);
    lv_obj_set_height(sensor_ui.arc_temperature_dyn, 214);
    lv_obj_set_x(sensor_ui.arc_temperature_dyn, 34);
    lv_obj_set_y(sensor_ui.arc_temperature_dyn, -66);
    lv_obj_set_align(sensor_ui.arc_temperature_dyn, LV_ALIGN_LEFT_MID);
    lv_arc_set_value(sensor_ui.arc_temperature_dyn, 50);
    lv_obj_set_style_border_color(sensor_ui.arc_temperature_dyn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(sensor_ui.arc_temperature_dyn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(sensor_ui.arc_temperature_dyn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(sensor_ui.arc_temperature_dyn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(sensor_ui.arc_temperature_dyn, lv_color_hex(0xC86868), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(sensor_ui.arc_temperature_dyn, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(sensor_ui.arc_temperature_dyn, lv_color_hex(0xC36A6A), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sensor_ui.arc_temperature_dyn, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    sensor_ui.arc_humidity_dyn = lv_arc_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.arc_humidity_dyn, 226);
    lv_obj_set_height(sensor_ui.arc_humidity_dyn, 214);
    lv_obj_set_x(sensor_ui.arc_humidity_dyn, 323);
    lv_obj_set_y(sensor_ui.arc_humidity_dyn, -63);
    lv_obj_set_align(sensor_ui.arc_humidity_dyn, LV_ALIGN_CENTER);
    lv_arc_set_value(sensor_ui.arc_humidity_dyn, 50);

    lv_obj_set_style_arc_color(sensor_ui.arc_humidity_dyn, lv_color_hex(0x95C08B), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(sensor_ui.arc_humidity_dyn, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(sensor_ui.arc_humidity_dyn, lv_color_hex(0x95C08B), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sensor_ui.arc_humidity_dyn, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    sensor_ui.arc_pressure_dyn = lv_arc_create(ui_TabPage_Home);
    lv_obj_set_width(sensor_ui.arc_pressure_dyn, 226);
    lv_obj_set_height(sensor_ui.arc_pressure_dyn, 214);
    lv_obj_set_x(sensor_ui.arc_pressure_dyn, -339);
    lv_obj_set_y(sensor_ui.arc_pressure_dyn, -64);
    lv_obj_set_align(sensor_ui.arc_pressure_dyn, LV_ALIGN_RIGHT_MID);
    lv_arc_set_value(sensor_ui.arc_pressure_dyn, 50);
}

void Sensor_UI_Update(void)
{
    sensor_data_t sensor_data;
    if (Sensor_Queue != NULL && xQueueReceive(Sensor_Queue, &sensor_data, 0) == pdPASS)
    {
        if (sensor_data.valid) {
            char temp[50];
            char relative_humidity[50];
            char barometric_preassure[50];
            snprintf(temp, sizeof(temp), "%2.1f", sensor_data.temperature);
            snprintf(relative_humidity, sizeof(relative_humidity), "%2.1f%%", sensor_data.humidity);
            snprintf(barometric_preassure, sizeof(barometric_preassure), "%.1fpHa", sensor_data.pressure);
            if (lvgl_port_lock(-1))
            {
                lv_label_set_text(sensor_ui.temperature_label_dyn, temp);
                lv_label_set_text(sensor_ui.humidity_label_dyn, relative_humidity);
                lv_label_set_text(sensor_ui.pressure_label_dyn, barometric_preassure);
                lvgl_port_unlock();
            }
        }
        else {
            if (lvgl_port_lock(-1))
            {

                lv_label_set_text(sensor_ui.temperature_label_dyn, "--(invalid)");
                lv_label_set_text(sensor_ui.humidity_label_dyn, "--(invalid)");
                lv_label_set_text(sensor_ui.pressure_label_dyn, "--(invalid)");
                lvgl_port_unlock();
            }
        }
    }
}