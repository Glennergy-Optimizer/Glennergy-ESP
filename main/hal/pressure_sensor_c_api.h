#ifndef PRESSURE_SENSOR_C_API
#define PRESSURE_SENSOR_C_API
#include "../app_types.h"
 

#ifdef __cplusplus
    #include "pressure_sensor.hpp"
    #include "bme280_sensor.hpp"
    
    // obs - dessa ska vara innan extern
    hal::SensorError sensor_read(hal::PressureReading& pressure);    
    PressureReadingInC pressure_sensor_get_celcius(hal::PressureReading& reading);
    extern "C" {
    
#endif

bool pressure_sensor_is_available(void);

#ifdef __cplusplus
}
#endif

#endif