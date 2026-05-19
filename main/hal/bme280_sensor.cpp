#include "bme280_sensor.hpp"
#include "../app_queues.h"

#define BME280_SDA_GPIO GPIO_NUM_8 // The I2C SDA 
#define BME280_SCL_GPIO GPIO_NUM_9 // SCL
#define BME280_I2C_PORT I2C_NUM_0 // Use ESP32 I2C controller 0
#define BME280_I2C_FREQ_HZ 100000 // 100 000 means 100Khz, normal safe I2C speed
#define BME280_WAVESHARE_DEFAULT_ADDRESS 0x77 // The default address for the sensor when the ADDR cable is not connected

// If we fail to read data from sensor X times, attempt to reconnect every so often
#define BME280_MAX_READ_FAILURES 5
#define BME280_RECONNECT_INTERVAL_MS 5000


/*
static uint8_t bme280_read_failures = 0;
static int64_t last_reconnect_attempt_ms = 0;


static i2c_bus_handle_t bme280_bus = NULL;
static bme280_handle_t bme280 = NULL;
static bool bme280_ready = false;

*/
QueueHandle_t Sensor_Queue = NULL;

static constexpr char* TAG = "bme280_sensor.cpp";
// Reminder, static so only this file can see it
static bool fake_mode = false;

void hal::BME280Sensor::publish_temperature_data(hal::TemperatureReading& reading) {
    if (Sensor_Queue != NULL) {
        xQueueOverwrite(Sensor_Queue, &reading);
    }
}


bool hal::BME280Sensor::is_present () {
    return 0;
}


hal::SensorError hal::BME280Sensor::read(hal::TemperatureReading& reading) {

    if (!this->bme280_ready || this->bme280 == NULL) {
        int64_t now_ms = esp_timer_get_time() / 1000;
        if (now_ms - this->last_reconnect_attempt_ms >= BME280_RECONNECT_INTERVAL_MS) {
            this->last_reconnect_attempt_ms = now_ms;
            ESP_LOGI(TAG, "Trying to reconnect BME280...");
            this->bme280_ready = bme280_sensor_init();

            if (this->bme280_ready) {
                ESP_LOGI(TAG, "BME280 reconnected :)");
            }
        }
        //sensor->valid = false;
        //publish_sensor_data(sensor);
        return hal::SensorError::CommunicationFailure;
        //return false;
    }

    float temperature = 0.0f;
    //float humidity = 0.0f;
    //float pressure = 0.0f;

    esp_err_t temp_result = bme280_read_temperature(bme280, &temperature);
    //esp_err_t humidity_result = bme280_read_humidity(bme280, &humidity);
    //esp_err_t pressure_result = bme280_read_pressure(bme280, &pressure);

    //if (temp_result != ESP_OK || humidity_result != ESP_OK || pressure_result != ESP_OK) {
    if (temp_result != ESP_OK) {
        this->bme280_read_failures++;

        //ESP_LOGW(TAG, "BME280 read failed %u/%u: temp=%s humidity=%s pressure=%s",
        ESP_LOGW(TAG, "BME280 read failed %u/%u: temp=%s",
            this->bme280_read_failures,
            BME280_MAX_READ_FAILURES,     
            esp_err_to_name(temp_result)
            //esp_err_to_name(humidity_result),
            //esp_err_to_name(pressure_result)
        );

        if (this->bme280_read_failures >= BME280_MAX_READ_FAILURES) {
            ESP_LOGW(TAG, "BME280 marked disconnected after repeated read failures. Please restart application.");
            this->bme280_ready = false;
            this->bme280_read_failures = 0;
            if (this->bme280 != NULL) {
                bme280_delete(&this->bme280);
            }
        }
        //sensor->valid = false;
        //publish_sensor_data(sensor);
        return hal::SensorError::CommunicationFailure;
    }
    
    this->bme280_read_failures = 0;
    
    bme280_read_temperature(this->bme280, &reading.celcius);
    publish_temperature_data(reading);

    /*
    sensor->temperature = temperature;
    sensor->humidity = humidity;
    sensor->pressure = pressure;
    sensor->valid = true;
    */
    reading.timestamp = esp_timer_get_time() / 1000000ULL;
    //sensor->last_update_seconds = esp_timer_get_time() / 1000000ULL;

    ESP_LOGI(TAG, "BME280: %.f C", reading.celcius);
    //ESP_LOGI(TAG, "BME280: %.2f C, %.2f %%RH, %.2f hPa",
    //         temperature, humidity, pressure);

    //publish_sensor_data(sensor);
    return hal::SensorError::Ok;
}



hal::BME280Sensor::BME280Sensor()
{
    BME280Sensor_init_i2c_config();
    bme280_sensor_init();
}

bool hal::BME280Sensor::bme280_init_at_address(uint8_t address)
{
    // Connect the driver object to the I2C buss at the specific adress
    this->bme280 = bme280_create(this->bme280_bus, address);
    if (this->bme280 == NULL) {
        ESP_LOGE(TAG, "Failed to create BME280 handle at address 0x%02X", address);
        return false;
    }

    // Simple check that reads chip ID of sensor and configues measurement settings. Fails if no response from sensor
    esp_err_t err = bme280_default_init(this->bme280);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "BME280 init failed at address 0x%02X", address);
        bme280_delete(&this->bme280); // Delete sensor object if init failed
        return false;
    }

    ESP_LOGI(TAG, "BME280 initialized at address 0x%02X", address);
    return true;
}

void hal::BME280Sensor::BME280Sensor_init_i2c_config()
{
    this->i2c_config.mode = I2C_MODE_MASTER; // ESP I2C buss is master
    this->i2c_config.sda_io_num = BME280_SDA_GPIO; // sets the SDA to the previously defined GPIO pin
    // Still not entirely sure about PULLUPs and PULLDOWNs. But BME280 needs enabled pullup on SCL and SDA
    this->i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE; 
    this->i2c_config.scl_io_num = BME280_SCL_GPIO; // sets the SCL to the previously defined GPIO pin
    this->i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    this->i2c_config.master.clk_speed = BME280_I2C_FREQ_HZ; // sets clock speed to previously defined clock speed
}

bool hal::BME280Sensor::bme280_sensor_init() {
    // This scans for devices, logs adresses. Mainly used for debugging so we can see if the 0x77 appears, or the fallback 0x76
    // If bme280 bus is set it means we're trying to attempting to reconnect, so don't create duplicated bus.
    // if not set(NULL), then create it for the first time 
    if (bme280_bus == NULL) {
        bme280_bus = i2c_bus_create(BME280_I2C_PORT, &this->i2c_config);
        if (bme280_bus == NULL) {
            ESP_LOGE(TAG, "Failed to create I2C bus for BME280");
            return false;
        }
    }
    // Debugging only so we know what connections we have
    uint8_t found_devices[8] = {};
    uint8_t device_count = i2c_bus_scan(bme280_bus, found_devices, sizeof(found_devices));
    ESP_LOGI(TAG, "I2C scan found %u device(s)", device_count);

    for (uint8_t i = 0; i < device_count && i < sizeof(found_devices); i++) {
        ESP_LOGI(TAG, "I2C device found at address 0x%02X", found_devices[i]);
    }

    // Waveshare BME280 defaults to 0x77 when ADDR is left unconnected.
    if (bme280_init_at_address(BME280_WAVESHARE_DEFAULT_ADDRESS)) {
        return true;
    }

    // If ADDR is connected to GND, the address is 0x76.
    return bme280_init_at_address(BME280_I2C_ADDRESS_DEFAULT);
}

