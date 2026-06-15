#include "fake_config.hpp"


void fake_config_data(config_data_t* config) {
    config->fetch_interval_minutes = 1;
    config->test_mode = false;
    config->sensor_interval_ms = 1000; 
}
