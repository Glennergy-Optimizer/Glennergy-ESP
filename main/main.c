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
#include "UART/UART.hpp"
#include "sensor/sensor.h"
#include "LEOP/LEOP_Fetcher.h"
#include "Memory/Spiffs.h"
#include <stdlib.h>
#include <time.h>


#define WIFI_PASS "rockyunit953"
#define WIFI_SSID "NETGEAR49"


static app_state_t app;

// Handlers so we can get stack statistics per task
static TaskHandle_t wifi_task_handle = NULL;
static TaskHandle_t ui_task_handle = NULL;
static TaskHandle_t uart_task_handle = NULL;
static TaskHandle_t sensor_task_handle = NULL;
static TaskHandle_t leop_task_handle = NULL;


static const char *TAG = "main";

// Main application function
void app_main()
{
    // Time stuff
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();

    //app.system_task_handlers->
    // Sensor_Init(&app.sensor_data);

    

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


    static Spiffs_t spiffs;
    Spiffs_Initialize(&spiffs, "test.txt");
    Spiffs_WriteToFile(&spiffs, "Hej 123");
    char buffer[128];
    Spiffs_ReadFromFile(&spiffs, buffer, sizeof(buffer));
    ESP_LOGE(TAG, "%s", buffer);

    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1))
    {
        ui_init();
        // Release the mutex
        lvgl_port_unlock();
    }

    xTaskCreate(WiFi_Work, "WIFI_Work", 4096, NULL, 5, &wifi_task_handle);

    xTaskCreate(ui_update_task, "UI_Update", 12288, NULL, 5, &ui_task_handle);

    xTaskCreate(UART_Work, "UART", 4096, &app, 4, &uart_task_handle);

    xTaskCreate(Sensor_Work, "Sensor", 4096, &app, 4, &sensor_task_handle);

    static LEOPData leop_data;

    //LEOPFetcher_Initialize(&leop_data, 3000);

    //xTaskCreate(LEOPFetcher_Work, "LEOP", 4096, &leop_data, 4, NULL);
    //  ESP_ERROR_CHECK(WiFi_Dispose());

    // Set the task handles after the tasks has been started, so we actually store info/data instead of NULL
    app.system_task_handlers.wifi_handle = wifi_task_handle;
    app.system_task_handlers.ui_handle = ui_task_handle;
    app.system_task_handlers.uart_handle = uart_task_handle;
    app.system_task_handlers.sensor_handle = sensor_task_handle;
    app.system_task_handlers.leop_handle = leop_task_handle;
}
