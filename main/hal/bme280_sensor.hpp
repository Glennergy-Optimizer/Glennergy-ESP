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
 * @brief Public API for the BME280 sensor HAL.
 *
 * Provides temperature, humidity, and pressure reads over I2C, including
 * bus setup and reconnect handling for the supported sensor addresses.
 *
 * @ingroup HAL
 */

/**
 * @defgroup HAL HAL
 * @brief Hardware abstraction layer for peripheral and sensor access.
 *
 * Includes modules that configure hardware, manage bus ownership, and provide
 * sensor readout helpers used by the application tasks.
 * @{
 */

namespace hal {
    /**
     * @brief BME280 sensor interface over the shared I2C bus.
     *
     * The class supports temperature, humidity, and pressure reads from the
     * sensor, and keeps minimal internal state for reconnect attempts and
     * failure tracking.
     */
    class BME280Sensor : public ITemperatureSensor {
        public:
            //BME280Sensor(i2c_port_t i2c_port, uint8_t address) : port_(i2c_port), address_(address);
            /**
             * @brief Creates a BME280 sensor instance.
             */
            BME280Sensor();

            /**
             * @brief Reads temperature data from the BME280 sensor.
             *
             * @param[in,out] reading Output reading that receives the latest value
             * and timestamps.
             *
             * @return Sensor status for the temperature read operation.
             */
            SensorError read(TemperatureReading& reading) override;

            /**
             * @brief Reads humidity data from the BME280 sensor.
             *
             * @param[in,out] reading Output reading that receives the latest value
             * and timestamps.
             *
             * @return Sensor status for the humidity read operation.
             */
            SensorError read(HumidityReading& reading);

            /**
             * @brief Reads pressure data from the BME280 sensor.
             *
             * @param[in,out] reading Output reading that receives the latest value
             * and timestamps.
             *
             * @return Sensor status for the pressure read operation.
             */
            SensorError read(PressureReading& reading);

            /**
             * @brief Checks whether the sensor is currently present.
             *
             * @return Always returns `false` in the current implementation.
             */
            bool is_present() override;

            /**
             * @brief Increments the consecutive read failure counter.
             */
            void increment_read_failure();

            /**
             * @brief Initializes or reconnects the BME280 sensor.
             *
             * Creates the I2C bus on first use, then probes the supported
             * BME280 addresses.
             *
             * @return `true` when the sensor is initialized successfully;
             * otherwise `false`.
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
             * @brief Initializes the BME280 driver at a specific I2C address.
             *
             * @param address I2C address to probe.
             *
             * @return `true` on success; otherwise `false`.
             */
            bool bme280_init_at_address(uint8_t address);
    };
}

/** @} */

#endif
