#ifndef BME280_SENSOR_HPP
#define BME280_SENSOR_HPP

#include "temperature_sensor.hpp"
#include "humidity_sensor.hpp"
#include "pressure_sensor.hpp"
#include "i2c_bus.h"
#include "bme280.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "../app_queues.h"

/**
 * @file bme280_sensor.hpp
 * @brief Public API for the BME280 sensor HAL wrapper.
 *
 * Provides temperature, humidity, and pressure readings through the BME280
 * sensor over I2C.
 */

/**
 * @defgroup BME280_SENSOR BME280_SENSOR
 * @brief BME280 sensor HAL wrapper.
 *
 * Manages the BME280 I2C bus, sensor handle, and reconnect attempts after
 * communication failures.
 *
 * @note Initialization depends on the target board wiring and the configured
 * I2C pins and address.
 * @{
 */

namespace hal {
    /**
     * @brief BME280 sensor wrapper for temperature, humidity, and pressure reads.
     *
     * Maintains the I2C bus, sensor handle, and reconnect state for the
     * underlying BME280 device.
     */
    class BME280Sensor : public ITemperatureSensor {
        public:
            //BME280Sensor(i2c_port_t i2c_port, uint8_t address) : port_(i2c_port), address_(address);
            /**
             * @brief Creates a BME280 sensor wrapper.
             */
            BME280Sensor();

            /**
             * @brief Reads temperature from the BME280.
             *
             * @param[out] reading Temperature reading output.
             *
             * @return Sensor status for the read operation.
             */
            SensorError read(TemperatureReading& reading) override;

            /**
             * @brief Reads humidity from the BME280.
             *
             * @param[out] reading Humidity reading output.
             *
             * @return Sensor status for the read operation.
             */
            SensorError read(HumidityReading& reading);

            /**
             * @brief Reads pressure from the BME280.
             *
             * @param[out] reading Pressure reading output.
             *
             * @return Sensor status for the read operation.
             */
            SensorError read(PressureReading& reading);

            /**
             * @brief Reports whether the BME280 is present.
             *
             * @return True if the sensor is considered present.
             */
            bool is_present() override;

            /**
             * @brief Increments the internal read failure counter.
             */
            void increment_read_failure();

            /**
             * @brief Initializes or reconnects the BME280 device.
             *
             * @return True on success.
             */
            bool bme280_sensor_init();
        private:
            uint8_t bme280_read_failures = 0;
            int64_t last_reconnect_attempt_ms = 0;

            i2c_bus_handle_t bme280_bus = NULL;
            bme280_handle_t bme280 = NULL;
            bool bme280_ready = false;

            i2c_config_t i2c_config;
            i2c_port_t i2c_port;
            uint8_t adress; 
            
            /**
             * @brief Configures the I2C settings used by the BME280 sensor.
             */
            void BME280Sensor_init_i2c_config();

            /**
             * @brief Creates and initializes a BME280 instance at the given address.
             *
             * @param[in] address I2C address to probe.
             *
             * @return True on success.
             */
            bool bme280_init_at_address(uint8_t address);
    };
}

/** @} */

#endif
