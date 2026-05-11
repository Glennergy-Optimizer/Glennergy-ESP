
#include "WiFi_UI.h"
#include "../../main/WiFi.h"
#include "../../screens/ui_Screen1.h"
#include "lvgl_port.h"

static const char* TAG = "WiFi_UI";

static WiFi_UI wifi_ui = {
    .network_dropdown_dyn = NULL,
    .password_textarea_dyn = NULL,
    .scan_button_dyn = NULL,
    .status_label_dyn = NULL,
    .status_label_sta = NULL,
};

void WiFi_UI_Set_Callbacks(void);

void WiFi_UI_Initialize()
{

    wifi_ui.network_dropdown_dyn = lv_dropdown_create(ui_Group_Settings);
    lv_obj_set_width(wifi_ui.network_dropdown_dyn, 364);
    lv_obj_set_height(wifi_ui.network_dropdown_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(wifi_ui.network_dropdown_dyn, -4);
    lv_obj_set_y(wifi_ui.network_dropdown_dyn, -83);
    lv_obj_set_align(wifi_ui.network_dropdown_dyn, LV_ALIGN_CENTER);
    lv_obj_add_flag(wifi_ui.network_dropdown_dyn, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags

    wifi_ui.password_textarea_dyn = lv_textarea_create(ui_Group_Settings);
    lv_obj_set_width(wifi_ui.password_textarea_dyn, 368);
    lv_obj_set_height(wifi_ui.password_textarea_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(wifi_ui.password_textarea_dyn, -4);
    lv_obj_set_y(wifi_ui.password_textarea_dyn, -26);
    lv_obj_set_align(wifi_ui.password_textarea_dyn, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(wifi_ui.password_textarea_dyn, "Password");
    lv_textarea_set_one_line(wifi_ui.password_textarea_dyn, true);
    lv_textarea_set_password_mode(wifi_ui.password_textarea_dyn, true);

    wifi_ui.status_label_sta = lv_label_create(ui_Group_Settings);
    lv_obj_set_width(wifi_ui.status_label_sta, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_ui.status_label_sta, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(wifi_ui.status_label_sta, -308);
    lv_obj_set_y(wifi_ui.status_label_sta, -107);
    lv_obj_set_align(wifi_ui.status_label_sta, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(wifi_ui.status_label_sta, "Status: ");

    wifi_ui.status_label_dyn = lv_label_create(ui_Group_Settings);
    lv_obj_set_width(wifi_ui.status_label_dyn, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_ui.status_label_dyn, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(wifi_ui.status_label_dyn, -200);
    lv_obj_set_y(wifi_ui.status_label_dyn, -107);
    lv_obj_set_align(wifi_ui.status_label_dyn, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(wifi_ui.status_label_dyn, "Disconnected");
    lv_label_set_recolor(wifi_ui.status_label_dyn, "true");
    lv_obj_set_style_text_color(wifi_ui.status_label_dyn, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(wifi_ui.status_label_dyn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    wifi_ui.scan_button_dyn = lv_btn_create(ui_Group_Settings);
    lv_obj_set_width(wifi_ui.scan_button_dyn, 50);
    lv_obj_set_height(wifi_ui.scan_button_dyn, 25);
    lv_obj_set_x(wifi_ui.scan_button_dyn, -109);
    lv_obj_set_y(wifi_ui.scan_button_dyn, -131);
    lv_obj_set_align(wifi_ui.scan_button_dyn, LV_ALIGN_CENTER);
    lv_obj_add_flag(wifi_ui.scan_button_dyn, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(wifi_ui.scan_button_dyn, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(wifi_ui.scan_button_dyn, lv_color_hex(0x370859), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_ui.scan_button_dyn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(wifi_ui.scan_button_dyn, lv_color_hex(0x5F06CF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(wifi_ui.scan_button_dyn, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    WiFi_UI_Set_Callbacks();
}

void WiFi_UI_Scan_cb(lv_event_t *_Event)
{
    wifi_data w_data;
    w_data.cmd = WIFI_CMD_SCAN;
    xQueueSend(wifi_cmd_queue, &w_data, 0);
}

void WiFi_UI_Keyboard_cb(lv_event_t *_Event)
{
    lv_event_code_t code = lv_event_get_code(_Event);
    lv_obj_t *kb = lv_event_get_target(_Event);
    lv_obj_t *ta = lv_event_get_user_data(_Event);
    wifi_data w_data;
    w_data.cmd = WIFI_CMD_CONNECT;

    if (code == LV_EVENT_READY) // user pressed OK
    {
        const char *pass = lv_textarea_get_text(ta);

        w_data.wifi_info.password = pass;
        w_data.wifi_info.ssid = wifi_ui.selected_ssid;

        xQueueSend(wifi_cmd_queue, &w_data, 0);

        lv_obj_del(kb);
    }

    if (code == LV_EVENT_CANCEL)
    {
        lv_obj_del(kb);
    }
}

void WiFi_UI_TextArea_cb(lv_event_t *_Event)
{
    lv_obj_t *ta = lv_event_get_target(_Event);
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(kb, LV_HOR_RES, 120);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_keyboard_set_textarea(kb, ta);

    lv_obj_add_event_cb(kb, WiFi_UI_Keyboard_cb, LV_EVENT_ALL, ta);
}

void WiFi_UI_Dropdown_cb(lv_event_t *_Event)
{

    lv_obj_t *obj = lv_event_get_target(_Event);

    lv_dropdown_get_selected_str(obj, wifi_ui.selected_ssid, sizeof(wifi_ui.selected_ssid));
}

void WiFi_UI_Update(void)
{
    wifi_data w_data;
    if (wifi_result_queue != NULL && xQueueReceive(wifi_result_queue, &w_data, 0) == pdPASS)
    {
        if (w_data.status == WIFI_STATUS_SCAN_DONE)
        {
            ESP_LOGI(TAG, "Scan finished!");
            if (lvgl_port_lock(-1))
            {
                lv_dropdown_clear_options(wifi_ui.network_dropdown_dyn);
                for (int i = 0; i < w_data.number; i++)
                {
                    lv_dropdown_add_option(wifi_ui.network_dropdown_dyn, (char *)w_data.ap_info[i].ssid, LV_DROPDOWN_POS_LAST);
                }
                lvgl_port_unlock();
            }
        }
        if (w_data.status == WIFI_STATUS_CONNECTED)
        {
            ESP_LOGI(TAG, "Connection finished!");
            lv_label_set_text(wifi_ui.status_label_dyn, "Connected");
            lv_obj_set_style_text_color(wifi_ui.status_label_dyn, lv_color_hex(0x66FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        //xQueueSend(wifi_queue, &w_data, portMAX_DELAY);
    }
}

void WiFi_UI_Set_Callbacks(void)
{
    lv_obj_add_event_cb(wifi_ui.network_dropdown_dyn, WiFi_UI_Dropdown_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(wifi_ui.password_textarea_dyn, WiFi_UI_TextArea_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(wifi_ui.scan_button_dyn, WiFi_UI_Scan_cb, LV_EVENT_PRESSED, NULL);
}
