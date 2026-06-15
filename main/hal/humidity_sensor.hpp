#ifndef HUMIDITY_SENSOR_HPP
#define HUMIDITY_SENSOR_HPP

#include <stdint.h>
#include "hal_classes.h"
#include <time.h>

namespace hal {
    // Ok = 0, CommunicationFailure, DeviceNotFound, InvalidReading, Timeout
    struct HumidityReading {
        float humidity;
        uint32_t monotonic_timestamp;
        time_t unix_timestamp;
    };

    class IHumiditySensor{
        public:
            virtual ~IHumiditySensor() = default;
            virtual SensorError read(HumidityReading& reading) = 0;
            virtual bool is_present() = 0;        
    };
}

#endif