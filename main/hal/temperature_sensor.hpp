#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include <stdint.h>


namespace hal {
    // Ok = 0, CommunicationFailure, DeviceNotFound, InvalidReading, Timeout
    enum class SensorError : uint8_t {
        Ok = 0,
        CommunicationFailure,
        DeviceNotFound,
        InvalidReading,
        Timeout
    };
    
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


typedef struct {
    //hal::TemperatureReading temptest;
    float celciustest;
} Ttest;



#endif