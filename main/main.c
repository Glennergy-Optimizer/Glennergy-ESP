/*****************************************************************************
 * | File       :   main.c
 * | Author     :   Waveshare team
 * | Function   :   Main function
 * | Info       :
 * |                Create a button using LVGL to control an external LED
 *----------------
 * | Version    :   V1.0
 * | Date       :   2024-12-06
 * | Info       :   Basic version
 *
 ******************************************************************************/
#define LV_CONF_INCLUDE_SIMPLE 1

#include "rgb_lcd_port.h" // Header for Waveshare RGB LCD driver
#include "gt911.h"        // Header for touch screen operations (GT911)
#include "lvgl_port.h"
#include "lvgl_demo.h"
#include "../ui/ui.h"
#include "freertos/task.h"
#include "WiFi.h"
#include "HTTP.h"
#include "jansson.h"
#include "UART/UART.hpp"

#define WIFI_PASS "rockyunit953"
#define WIFI_SSID "NETGEAR49"

static const char *TAG = "main";

// Main application function
void app_main()
{
    
    static esp_lcd_panel_handle_t panel_handle = NULL; // Declare a handle for the LCD panel
    static esp_lcd_touch_handle_t tp_handle = NULL;
    // Initialize the GT911 touch screen controller
    tp_handle = touch_gt911_init();

    vTaskDelay(pdMS_TO_TICKS(2000));

    // Initialize the Waveshare ESP32-S3 RGB LCD hardware
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();


    // Turn on the LCD backlight
    wavesahre_rgb_lcd_bl_on();

    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle)); // Initialize LVGL with the panel and touch handles

    WiFi_Initialize();

    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1))
    {
        ui_init();
        // Release the mutex
        lvgl_port_unlock();
    }

    xTaskCreate(WiFi_Work, "WIFI_Work", 4096, NULL, 5, NULL);

    xTaskCreate(ui_update_task, "UI_Update", 4096, NULL, 5, NULL);

    xTaskCreate(UART_Init, "UART", 4096, NULL, 4, NULL);

    //ESP_ERROR_CHECK(WiFi_Dispose());
}
