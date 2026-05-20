#ifndef TEMPERATURE_SENSOR_C_API_H
#define TEMPERATURE_SENSOR_C_API_H
#include "../app_types.h"


#ifdef __cplusplus
    #include "temperature_sensor.hpp"
    #include "bme280_sensor.hpp"

    extern "C" {
    
    hal::SensorError sensor_read(hal::TemperatureReading& temperature);
    
    
    TemperatureReadingInC temperature_sensor_get_celcius(hal::TemperatureReading& reading);
#endif

extern hal::BME280Sensor sensor;
extern hal::TemperatureReading temp;



bool temperature_sensor_is_available(void);

#ifdef __cplusplus
}
#endif

#endif