#ifndef HAL_CLASSES_H
#define HAL_CLASSES_H

namespace hal {
    enum class SensorError : uint8_t {
        Ok = 0,
        CommunicationFailure,
        DeviceNotFound,
        InvalidReading,
        Timeout
    };
}


#endif
