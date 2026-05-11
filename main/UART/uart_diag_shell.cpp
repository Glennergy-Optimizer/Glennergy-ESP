#include "esp_log.h"
#include <iostream>
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart_diag_shell.hpp"
#include <vector>
#include <sstream>
#include <cstdlib>
#include "esp_timer.h"
#include "esp_heap_caps.h"
#include "../app_types.h"


static const char* TAG = "UART_DIAG_SHELL.CPP";

void handle_sensor(app_state_t* state);
void handle_status(app_state_t* state);
void handle_leop(app_state_t* state);
void handle_config(std::vector<std::string> tokens, app_state_t* state);
void handle_diag();
void print_config(app_state_t* state);

void handle_help(bool wait_for_enter);
void print_help_line(const std::string message, bool wait_for_enter);
void print_and_wait_for_enter(const std::string);

std::vector<std::string> split(const std::string& str, char delimiter);
bool parse_int(const std::string& str, int& out);


// *** HELPER FUNCTIONS ***
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

// todo - maybe change to enum instead of simple format helpers?
const char* connected_text(bool value)
{
    return value ? "Connected" : "Disconnected";
}

const char* enabled_text(bool value)
{
    return value ? "Enabled" : "Disabled";
}

const char* ok_text(bool value)
{
    return value ? "OK" : "Not OK";
}


// Takes in a std::string,
// .c_str() converts to C-style char*,
// so std::strtol:
// Converts our c++ string to a C char *
// Reds and attempts to convert char to long.
// It gives saves two values: val(our actual value), and end(pointer to where we stopped tracking).
// 10 is the number base we working with(10 gives us the usual 0-9 numbers).
// If it stopped reading before a null-terminator, then our string includes characters that could not be converted to long.
// But if no issues, then the number can be parsed as int.
bool parse_int(const std::string& str, int& out) {
    char* end;
    // Reads string from start to end. "val" is the number it managed to read, "end" is where it stopped reading
    // 
    long val = std::strtol(str.c_str(), &end, 10);

    if (*end != '\0') {
        return false;
    }
    out = static_cast<int>(val);
    return true;
}


 // *** MAIN FUNCTIONS *** 
void handle_input(const std::string& input, app_state_t* state)
{
    int msg_len = input.length();
    ESP_LOGI(TAG, "msg len: %d\n", msg_len);
    std::vector<std::string> tokens = split(input, ' ');
    if (tokens.size() == 0) {
        std::cout << "No or incorrect input. Try again. " << std::endl;
        return;
    }
    const std::string& cmd = tokens[0];
    
    if (tokens.size() == 2 && cmd == "help" && tokens[1] == "immersive") {
        handle_help(true);
    }
    else if (cmd == "help") {
        handle_help(false);
    }
    else if (cmd == "status") {
        handle_status(state);
    }
    else if (cmd == "sensor") {
        handle_sensor(state);
    }
    else if (cmd == "leop") {
        handle_leop(state);
    }
    else if (cmd == "config") {
        handle_config(tokens, state);
    }
    else if (cmd == "pconfig") {
        print_config(state);
    }
    else if (cmd == "diag") {
        handle_diag();
    }
    else {
        std::cout << "Unknown command: " << input << std::endl;
    } 
}


// TODO - Only sensor is mocked right now
// TODO - "Status" systemtillstånd som Wifi, LEOP anslutning, sensor, uptime osv
// Status - Mer face-value till user, tex "är wifi ok"
void handle_status(app_state_t* state)
{
    std::cout << "Wifi: " << connected_text(state->system_status.wifi_connected) << std::endl;
    std::cout << "LEOP: " << connected_text(state->system_status.leop_connected) << std::endl;
    std::cout << "Sensor: " << ok_text(state->system_status.sensor_ok) << std::endl;
    std::cout << "Update counter: " << state->system_status.update_counter << std::endl;
}

void handle_sensor(app_state_t* state)
{
    std::cout << "Debug valid: " << state->sensor_data.valid << std::endl;
    std::cout << "Temperature raw: " << state->sensor_data.temperature << std::endl;

    if (!state->sensor_data.valid) {
        std::cout << "Sensor: No valid data yet." << std::endl;
        return;
    }

    std::cout << "Last updated time: " << state->sensor_data.last_update_seconds << std::endl;
    std::cout << "Temperature - " << state->sensor_data.temperature << std::endl;
    std::cout << "Pressure    - " << state->sensor_data.pressure << std::endl;
    std::cout << "Humidity    - " << state->sensor_data.humidity << std::endl;
    std::cout << "End of status. Not fully implemented yet." << std::endl;
}

void print_config(app_state_t* state) {
    std::cout << "fetch_interval_minutes: " << state->config_data.fetch_interval_minutes << std::endl;
    std::cout << "test_mode: " << enabled_text(state->config_data.test_mode) << std::endl;
}

void handle_config(std::vector<std::string> tokens, app_state_t* state)
{
    std::string help_msg = "syntax: \"config <config_name> <value>\".\nFields (name) (value):\n \tfetch_interval_minutes uint32_t\n\ttest_mode bool\n";
    if (tokens.size() != 3) {
        std::cout << help_msg;
        return;
    }
    const std::string& key = tokens[1];
    const std::string& value = tokens[2];
    if (key == "fetch_interval_minutes") {
        // production TODO - set a minimum and maximum value? 15min, 24h?
        int int_value;
        // Use helper function to see if we can parse something as int
        if (parse_int(value, int_value))
        {
            
            std::cout << "Now setting \"fetch_interval_minutes\" to \"" << int_value << "\"." << std::endl;
            state->config_data.fetch_interval_minutes = int_value;
        }
        
        // Nähäpp, man får inte try-catch i embedded.
        // Behöver C-variant.
        // try {
        //     int int_value = std::stoi(value);
        //     state->config_data.fetch_interval_minutes = int_value;
        //     //test_value = v;
        // } catch (...) {
        //     std::cout << "Invalid integer. Try again.\n";
        // } 
    }
    else if (key == "test_mode") {
        if (value == "true") {
            std::cout << "Now setting \test_mode\" to \"true\"." << std::endl;
            state->config_data.test_mode = true;
        }
        else if (value == "false") {
            std::cout << "Now setting \test_mode\" to \"false\"." << std::endl;
            state->config_data.test_mode = false;
        }
    }
    else {
        std::cout << help_msg;
    }
}


void handle_leop(app_state_t* app)
{
    leop_data_t& leop = app->leop_data;
    uint32_t total_entries= leop.entry_count;
    std::cout << "Latest leop data: " << std::endl;
    std::cout << "ID: " << leop.id << std::endl;
    std::cout << "Number of entries: " << total_entries << std::endl;
    std::cout << "Now printing entries timestamp, normalized recommendation and recommendation type" << std::endl;
    for (int i = 0; i < total_entries; i++) {
        std::cout << leop.entries[i].timestamp << ", " << leop.entries[i].recommendation << ", " << leop.entries[i].recommendation_type << std::endl;
    }
    //std::cout << "LEOP IS TODO." << std::endl;
}


// We could easily create a summary of the app-state, which would be a summarized version of all other help commands?
// Later on, we could add stuff like task statistics, stack usage, heap usage?
// If something is wrong, show the latest successful update times from LEOP or something?
// Mer fokus på detaljer om varför något är ok eller inte?
void handle_diag()
{
    // Mirkoseconds since boot, then converted to seconds and divide by matching type (unsigned long long)
    uint64_t uptime_seconds = esp_timer_get_time() / 1000000ULL;

    // current normal heap available
    size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    // returns the lowest heap has ever gotten since we started the program
    size_t min_free_heap = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);

    UBaseType_t task_count = uxTaskGetNumberOfTasks();

    std::cout << "Diagnostics: " << std::endl;
    std::cout << "Uptime: " << uptime_seconds << " seconds." << std::endl;
    std::cout << "Free Heap: " << free_heap << " bytes. " << std::endl;
    std::cout << "Minimum free heap: " << min_free_heap << " bytes." << std::endl;
    std::cout << "Task count: " << task_count << std::endl;
}


// *** HELP helper functions ***
void handle_help(bool wait_for_enter) 
{
    if (wait_for_enter == true)
    {
        print_and_wait_for_enter("So, you're looking for a little bit of help, huh?");
        print_and_wait_for_enter("Well, people rarely come here having their shit together. You're no different, from what I can tell.");
        print_and_wait_for_enter("I'll let you know beforehand, those who come seeking advice, but tries to slither away when it comes to collecting payment, they usually end up...worse. Worse than when they came to find me.");
        print_and_wait_for_enter("Now let me tell you the three pieces of advice I've learned while soaring from these filthy streets to now becoming..a king, of sorts.");
        print_and_wait_for_enter("#1. Invest in index funds. Small incremental steps and stacking interest will eventually help you living, not just surviving, when you are older.");
        print_and_wait_for_enter("#2. Don't neglect your family, friends or partner. Relationships are what makes life worth living after all.");
        print_and_wait_for_enter("#3. If someone who claims to be a prince contacts you and wants your financial help in securing his inheritage, turn around and don't look back. You'll lose everything you have. Been there, done that.");
        print_and_wait_for_enter(".");
        print_and_wait_for_enter("..");
        print_and_wait_for_enter("...");
        print_and_wait_for_enter("What? You're looking for some other sort of help?");
        print_and_wait_for_enter("Well, I do know a little bit of this, a little bit of that...though I doubt it'll be helpful for you.");
        print_and_wait_for_enter("Last week I helper a grandma collect 8 blue cranberries, and she did give me a note from her late husband, saying it was the only thing she should repay the honorary action with.");
        print_and_wait_for_enter("(He gives you the note)");
    }
    std::cout << "Here are the most common commands when attempting to use the UART-diagnostics interface when working with ESP32S3 units:" << std::endl;
    std::cout << "STATUS - Shows system health(WiFi, LEOP-connection, sensor, uptime)" << std::endl;
    std::cout << "LEOP - Shows latest data received from the LEOP-server." << std::endl;
    std::cout << "SENSOR - Shows current BME280-readings." << std::endl;
    std::cout << "CONFIG <param> <value> - Change config values." << std::endl;
    std::cout << "DIAG - shows system diagnostics(Task-statistics, heap-usage etc.)" << std::endl;
    std::cout << "HELP - This command =)" << std::endl;
}


void print_and_wait_for_enter(const std::string message)
{
    char input[8];

    std::cout << message << std::endl;
    std::cout.flush();

    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            for (int i = 0; input[i] != '\0'; i++) {
                if (input[i] == '\n' || input[i] == '\r') {
                    std::cout << std::endl;
                    return;
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}
