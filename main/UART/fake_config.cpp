#include <iostream>
#include "fake_config.hpp"
#include "uart_diag_shell.hpp"

void fake_config_data(config_data_t* config) {
    config->fetch_interval_minutes = 30;
    config->test_mode = false;
}
