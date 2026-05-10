#ifndef FAKE_SENSOR_H
#define FAKE_SENSOR_H

typedef struct {
    bool valid;
    uint32_t last_update_seconds;
    double temperature;
    double pressure;
    double humidity;
} sensor_data_t;

#endif