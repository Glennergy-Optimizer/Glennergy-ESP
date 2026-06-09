#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include <stdint.h>
#include "Hal_classes.h"
#include <time.h>


namespace hal {
    // float: celcius, uint32_t: monotonic_timestamp.
    struct TemperatureReading { 
        float celcius;
        uint32_t monotonic_timestamp;
        time_t unix_timestamp;
    };

    // 0 is used as OK/it works
    // ~ITemperatureSensor(), SensorError read() and bool is_prevent
    class ITemperatureSensor{
        public:            
            virtual ~ITemperatureSensor() = default;            
            virtual SensorError read(TemperatureReading& reading) = 0;
            virtual bool is_present() = 0;
    };
}

#endif