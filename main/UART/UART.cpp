//#include <iostream>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart_diag_shell.hpp"
#include "esp_log.h"
//#include <stdbool.h>
//#include <string.h>
#include <stdio.h>
// TODO - Should be extracted away from UART.
#include "../leop/fake_leop.hpp"
#include "../fake/fake_system_status.hpp"
#include "../fake/fake_config.hpp"
#include "esp_timer.h"
#include "../app_types.h"
#include "driver/uart.h"

#define UART_PORT UART_NUM_0
#define UART_BAUD 115200
#define BUF_SIZE 1024

//static app_state_t app;

static const char* TAG = "UART";

// Helper function - std::tolower can work to handle inputs while being case-insensitive, but may not work well when dealing with negative char values.
// This helper function is future proof
std::string to_lower_copy(std::string str)
{
    for (char& c : str){
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return str;
}

std::string trim_copy(const std::string& str)
{
    const char* whitespace = " \t\r\n";
    // Searches from beginning of the first none-whitespace character index
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    // Same, but reversed - Seacrhing from end of string to find the first non-white space index
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}



extern "C" void UART_Init(app_state_t* app)
{
    ESP_LOGI(TAG, "UART diagnostics start, cpp mode.");
    ESP_LOGI(TAG, "Type HELP for commands.");

    // std::string input;
    // std::string line;
    // It seems that std::cin acts different than stdin.
    // Raw C seems to be more reliable and stable than C++'s ways of handling inputs?
    char input[8];
    char line[128];
    int line_pos = 0;
    bool prompt_needed = true;

    // Nu med fake producers
    fake_system_status_fill(&app->system_status);
    fake_config_data(&app->config_data);

    fake_leop_fill(&app->leop_data);


    static uint32_t last_fake_update = 0;
    
    while (1)
    {
        uint32_t now_seconds = esp_timer_get_time() / 1000000ULL;
        
        // Continously update our fake data to better mimic runtime behavior
        // Our loops may run several times a second, but we can trottle it to only update our fake data once every new second
        // TODO - Sensor fully extracted, rest are todo to be extracted away.
        // UART should no interaction how fake data is produced or updated.
        if (now_seconds != last_fake_update) {
            last_fake_update = now_seconds;
            fake_system_status_update(&app->system_status);
        }

        if (prompt_needed) {
            printf("UART_DIAGNOSTICS_INTERFACE> ");
            fflush(stdout);
            prompt_needed = false;
        }
        

        if (fgets(input, sizeof(input), stdin) != NULL) {  
            for (int i = 0; input[i] != '\0'; i++) {
                char c = input[i];
                if (c == '\n' || c == '\r')
                {
                    line[line_pos] = '\0';
                    ESP_LOGI(TAG, "Complete msg: %s", line);
                    // trim and lowercase input.
                    handle_input(to_lower_copy(trim_copy(line)), app);

                    line_pos = 0;
                    prompt_needed = true;
                }
                else {
                    if (line_pos < sizeof(line) -1) {
                        line[line_pos] = c;
                        line_pos++;
                    }
                }
            }

        }
        else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void UART_Init_new(void)
{
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));

    // Set GPIO pins, which is mostly uncanged in our current case since we don't want to connect to another physical device, but use the built in to get access to esp terminal
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    ));

    // Installing drivers.
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, 
        BUF_SIZE * 2, // RX buffer, *2 why?
        0, //TX buffer so it becomes blocking
        0, // event queue size - No queue
        NULL, // event queue handle, dont want this atm
        0 //interupt flags, lets start minimum for now
    ));

    ESP_LOGI(TAG, "UART%d initialized at %d baud", UART_PORT, UART_BAUD);
}

void UART_RX_TASK(void *args) {

}


extern "C" void UART_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;

   // UART_Init(app);
    UART_Init_new();

   ESP_LOGI(TAG, "UART_Work started.");
   ESP_LOGI(TAG, "Type HELP for commands");

   char input[8];
   char line[128];
   int line_pos = 0;
   bool prompt_needed = true;

   uint8_t *buf = new uint8_t[BUF_SIZE];
   
   ESP_LOGI(TAG, "Uart WORK now entering main loop.");   

    while(1) {
        // This for failstuff
        //uint32_t now_seconds = esp_timer_get_time() / 1000000ULL;
        uint8_t byte;
        int len = uart_read_bytes(UART_PORT, &byte, 1, portMAX_DELAY);

        if (len > 0)
        {
            if (byte == '\n' || byte == '\r')
            {
                line[line_pos] = '\0';
                ESP_LOGI(TAG, "Complete msg: %s", line);
                // trim and lowercase input.
                handle_input(to_lower_copy(trim_copy(line)), app);

                line_pos = 0;
                prompt_needed = true;
            }
            else {
                if (line_pos < sizeof(line) -1) {
                    line[line_pos++] = byte;
                    // line[line_pos] = c;
                    // line_pos++;
                }
            }

        }
        else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        // if (prompt_needed) {
        //     printf("UART DIAGNOSTICS INTERFACE> ");
        //     //fflush(stdout);
        //     prompt_needed = false;
        // }

    }


}