/**
 * @file hal_classes.h
 * @brief Shared HAL types used by the hardware abstraction layer.
 *
 * Defines lightweight common types for HAL modules.
 *
 * @defgroup HAL_CLASSES HAL_CLASSES
 * @brief Shared types for HAL modules.
 *
 * Provides common enums and helper types used across HAL code.
 * @{
 */

#ifndef HAL_CLASSES_H
#define HAL_CLASSES_H

namespace hal {
    /**
     * @brief Error codes reported by sensor-oriented HAL operations.
     */
    enum class SensorError : uint8_t {
        Ok = 0,
        CommunicationFailure,
        DeviceNotFound,
        InvalidReading,
        Timeout
    };
}


#endif

/** @} */
