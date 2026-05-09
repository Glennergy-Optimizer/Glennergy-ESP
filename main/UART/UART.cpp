#include <iostream>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart_diag_shell.hpp"
#include "esp_log.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "fake_leop.hpp"
#include "fake_sensor.hpp"
#include "fake_system_status.hpp"
#include "fake_config.hpp"

static app_state_t app;

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

extern "C" void UART_Init(void)
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

    //app_state_t app; // Lagt till den som en static istället för att inte få stack overflow då leop_data_t med leop entries blir mycket.
    // app.sensor_data.temperature = 1;
    // app.sensor_data.humidity = 4;
    // app.sensor_data.pressure = 80;
    // Nu med fake producer
    fake_sensor_fill(&app.sensor_data);


    // app.system_status.wifi_connected = false;
    // app.system_status.leop_connected = true;
    // app.system_status.uptime_seconds = 123;
    // app.system_status.sensor_ok = true;
    // Nu med fake producer
    fake_system_status_fill(&app.system_status);


    // app.config_data.test_mode = false;
    // app.config_data.fetch_interval_minutes = 30;
    // Nu med fake producer
    fake_config_data(&app.config_data);


    fake_leop_fill(&app.leop_data);



    while (1)
    {
        // Continously update our fake data to better mimic runtime behavior
        fake_system_status_update(&app.system_status);
        fake_sensor_update(&app.sensor_data);


        if (prompt_needed) {
            //std::cout << "UART_DIAGNOSTICS_INTERFACE, cpp mode> " << std::endl;
            printf("UART_DIGNOASTICS_INTERFACE> ");
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
                    handle_input(to_lower_copy(line), &app);
                    /*if (strcmp(line, "help") == 0) {
                        handle_help(false);
                    } else if (strcmp(line, "help immersive") == 0) {
                        handle_help(true);
                    }*/

                    /* if (line_pos > 0 && VISUAL_MODE == true) {
                        ESP_LOGI(TAG, "Message so far: %s", line);
                        handle_input(line);
                    }
                    */

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

        // if (std::getline(std::cin, input, '\n')) {
        //     if (input.back() == '\n' || input.back() == '\r')
        //     {
        //         ESP_LOGI(TAG, "Complete msg: %s", input);
        //     }
        //     // for (int i = 0; input[i] != '\0'; i++) {
        //     //     char c = input[i];
        //     //     if (c == '\n' || c == '\r')
        //     //     {
        //     //         line[line_pos] = '\0';
        //     //         ESP_LOGI(TAG, "Complete msg: %s", line);
        //     //         handle_input(line);
        //     //         line_pos = 0;
        //     //         prompt_needed = true;
        //     //     }
        //     //     else {
        //     //         if (line_pos < sizeof(line)) {
        //     //             line[line_pos] = c;
        //     //             line_pos;;
        //     //         }
        //     //     }
        //     // }
        // }
        // else {
        //     vTaskDelay(pdMS_TO_TICKS(100));
        // }
    }
}
