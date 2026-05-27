#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP

#include <stdint.h>
#include "Hal_classes.h"


namespace hal {
    struct PressureReading { 
        float pressure;
        uint32_t timestamp;
    };

    class IPressureSensor{
        public:
            
            virtual ~IPressureSensor() = default;
            
            virtual SensorError read(PressureReading& reading) = 0;

            virtual bool is_present() = 0;
    };

}


#endif