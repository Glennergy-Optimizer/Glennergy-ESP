#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP

#include <stdint.h>
#include "Hal_classes.h"
#include <time.h>


namespace hal {
    struct PressureReading { 
        float pressure;
        uint32_t monotonic_timestamp;
        time_t unix_timestamp;
    };

    class IPressureSensor{
        public:
            
            virtual ~IPressureSensor() = default;
            
            virtual SensorError read(PressureReading& reading) = 0;

            virtual bool is_present() = 0;
    };
}

#endif