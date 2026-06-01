#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include <stdint.h>
#include "Hal_classes.h"


namespace hal {
    // float: celcius, uint32_t: timestamp.
    struct TemperatureReading { 
        float celcius;
        uint32_t timestamp;
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