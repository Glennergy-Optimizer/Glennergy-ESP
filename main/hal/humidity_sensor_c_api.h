#ifndef HUMIDITY_SENSOR_C_API_H
#define HUMIDITY_SENSOR_C_API_H
#include "../app_types.h"


#ifdef __cplusplus
    #include "humidity_sensor.hpp"
    #include "bme280_sensor.hpp"
    hal::SensorError sensor_read(hal::HumidityReading& humidityReading);
    HumidityReadingInC humidity_sensor_get_celcius(hal::HumidityReading& reading);
    extern "C" {

#endif

extern hal::BME280Sensor& humidity_sensor;
extern hal::HumidityReading humidity;

bool humidity_sensor_is_available(void);

#ifndef __cpluscplus
    }
#endif


#endif