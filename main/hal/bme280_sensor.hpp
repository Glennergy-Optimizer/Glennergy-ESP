#ifndef BME280_SENSOR_HPP
#define BME280_SENSOR_HPP

#include "temperature_sensor.hpp"
#include "humidity_sensor.hpp"
#include "pressure_sensor.hpp"
#include "i2c_bus.h"
#include "bme280.h"
#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "fake_sensor.hpp"
#include "freertos/queue.h"
//#include "sensor.h"
//#include "driver/gpio.h"  
//#include "esp_err.h"
//#include "esp_log.h"
//#include "esp_timer.h"
#include "../app_queues.h"


extern QueueHandle_t Sensor_Queue;
extern QueueHandle_t Humidity_Queue;
extern QueueHandle_t Pressure_Queue;

namespace hal {
    class BME280Sensor : public ITemperatureSensor {
        public:
            //BME280Sensor(i2c_port_t i2c_port, uint8_t address) : port_(i2c_port), address_(address);
            BME280Sensor();

            SensorError read(TemperatureReading& reading) override;
            SensorError read(HumidityReading& reading);
            SensorError read(PressureReading& reading);

            bool is_present() override;
            void increment_read_failure();   
            //void increment_read_failure_v2(hal::BME280Sensor& sensor);

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
            
            void BME280Sensor_init_i2c_config();
            bool bme280_init_at_address(uint8_t address);
            void publish_temperature_data(hal::TemperatureReading& reading);
            void publish_humidity_data(hal::HumidityReading& reading);
            void publish_pressure_data(hal::PressureReading& reading);
    };
}

#endif