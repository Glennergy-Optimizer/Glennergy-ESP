//#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fake_sensor.hpp"
#include "freertos/queue.h"
#include "sensor.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "../app_queues.h"

#include "bme280.h"
#include "i2c_bus.h"

//#include "../hal/bme280_sensor.hpp"
#include "../hal/temperature_sensor_c_api.h"
#include "../hal/temperature_sensor.hpp"
#include "../hal/humidity_sensor.hpp"
#include "../hal/humidity_sensor_c_api.h"
//#include "../hal/temperature_sensor_c_api.cpp"
#include "../app_types.h"

#define BME280_SDA_GPIO GPIO_NUM_8 // The I2C SDA 
#define BME280_SCL_GPIO GPIO_NUM_9 // SCL
#define BME280_I2C_PORT I2C_NUM_0 // Use ESP32 I2C controller 0
#define BME280_I2C_FREQ_HZ 100000 // 100 000 means 100Khz, normal safe I2C speed
#define BME280_WAVESHARE_DEFAULT_ADDRESS 0x77 // The default address for the sensor when the ADDR cable is not connected

// If we fail to read data from sensor X times, attempt to reconnect every so often
#define BME280_MAX_READ_FAILURES 5
#define BME280_RECONNECT_INTERVAL_MS 5000

static uint8_t bme280_read_failures = 0;
static int64_t last_reconnect_attempt_ms = 0;


static i2c_bus_handle_t bme280_bus = NULL;
static bme280_handle_t bme280 = NULL;
static bool bme280_ready = false;



static constexpr char* TAG = "Sensor.cpp";
// Reminder, static so only this file can see it
static bool fake_mode = false;

//static hal::BME280Sensor bmeSensor;






// Publishes/sends the data to UI via queue
static void publish_sensor_data(sensor_data_t* sensor)
{
    if (Sensor_Queue != NULL) {
        xQueueOverwrite(Sensor_Queue, sensor);
    }
}

static void publish_temperature_data(TemperatureReadingInC* tempReadInC)
{
    if (Sensor_Queue != NULL) {
        xQueueOverwrite(Sensor_Queue, tempReadInC);
    }
}

static void publish_humidity_data(HumidityReadingInC* humidityReadingInC)
{
    if (Humidity_Queue != NULL) {
        xQueueOverwrite(Humidity_Queue, humidityReadingInC);
    }
}


// Init bme280 at a specfic I2C adress
const static bool bme280_init_at_address(uint8_t address)
{
    // Connect the driver object to the I2C buss at the specific adress
    bme280 = bme280_create(bme280_bus, address);
    if (bme280 == NULL) {
        ESP_LOGE(TAG, "Failed to create BME280 handle at address 0x%02X", address);
        return false;
    }

    // Simple check that reads chip ID of sensor and configues measurement settings. Fails if no response from sensor
    esp_err_t err = bme280_default_init(bme280);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "BME280 init failed at address 0x%02X", address);
        bme280_delete(&bme280); // Delete sensor object if init failed
        return false;
    }

    ESP_LOGI(TAG, "BME280 initialized at address 0x%02X", address);
    return true;
}

static bool bme280_sensor_init()
{
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER; // ESP I2C buss is master
    conf.sda_io_num = BME280_SDA_GPIO; // sets the SDA to the previously defined GPIO pin
    // Still not entirely sure about PULLUPs and PULLDOWNs. But BME280 needs enabled pullup on SCL and SDA
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE; 
    conf.scl_io_num = BME280_SCL_GPIO; // sets the SCL to the previously defined GPIO pin
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = BME280_I2C_FREQ_HZ; // sets clock speed to previously defined clock speed

    // This scans for devices, logs adresses. Mainly used for debugging so we can see if the 0x77 appears, or the fallback 0x76
    
    // If bme280 bus is set it means we're trying to attempting to reconnect, so don't create duplicated bus.
    // if not set(NULL), then create it for the first time 
    if (bme280_bus == NULL) {
        bme280_bus = i2c_bus_create(BME280_I2C_PORT, &conf);
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


void Sensor_Init(app_state_t* app)
{
    if (fake_mode == true)
    {
        fake_sensor_fill(&app->sensor_data);
    }
    else {
        app->sensor_data.valid = false;
        app->sensor_data.last_update_seconds = 0;
        app->sensor_data.temperature = 0;
        app->sensor_data.pressure = 0;
        app->sensor_data.humidity = 0;
        bme280_ready = bme280_sensor_init();
    }

    Sensor_Queue = xQueueCreate(1, sizeof(sensor_data_t));

    if (Sensor_Queue == NULL)
    {
        ESP_LOGI(TAG, "Failed to create sensor queue!");
    }

}

//bool Sensor_Read_v2(hal::BME280Sensor& bmeSensor)
bool Sensor_Read_v2()
{
    //sensor_read();
    //hal::BME280Sensor bmeSensor();
    hal::TemperatureReading temperatur = hal::TemperatureReading();
    hal::SensorError result = sensor_read(temperatur);

    hal::HumidityReading humidityReading = hal::HumidityReading();
    hal::SensorError humidityResult = sensor_read(humidityReading);
    
    // Todo - validation of SensorError code
    TemperatureReadingInC Ctemperature;
    Ctemperature.celcius = temperatur.celcius;

    HumidityReadingInC Chumidity;
    Chumidity.humidity = humidityReading.humidity;

    publish_temperature_data(&Ctemperature);
    publish_humidity_data(&Chumidity);

    //ESP_LOGI(TAG, "temperature: %.f, Ctemperature: %.f", temperatur.celcius, Ctemperature.celcius);
    // ESP_LOGI(TAG, "Ctemperature: %.f", Ctemperature.celcius);

    return true;
}



// True on success, false on failure to let consumer know if we successfully read valid data or failed to read.
bool Sensor_Read(sensor_data_t* sensor)
{
    if (fake_mode == true)
    {
        fake_sensor_update(sensor);
        publish_sensor_data(sensor);
        return true;
    }
    else {
        if (!bme280_ready || bme280 == NULL) {
            int64_t now_ms = esp_timer_get_time() / 1000;
            if (now_ms - last_reconnect_attempt_ms >= BME280_RECONNECT_INTERVAL_MS) {
                last_reconnect_attempt_ms = now_ms;
                ESP_LOGI(TAG, "Trying to reconnect BME280...");
                bme280_ready = bme280_sensor_init();

                if (bme280_ready) {
                    ESP_LOGI(TAG, "BME280 reconnected :)");
                }
            }
            sensor->valid = false;
            publish_sensor_data(sensor);
            return false;
        }

        float temperature = 0.0f;
        float humidity = 0.0f;
        float pressure = 0.0f;

        esp_err_t temp_result = bme280_read_temperature(bme280, &temperature);
        esp_err_t humidity_result = bme280_read_humidity(bme280, &humidity);
        esp_err_t pressure_result = bme280_read_pressure(bme280, &pressure);

        if (temp_result != ESP_OK || humidity_result != ESP_OK || pressure_result != ESP_OK) {
            bme280_read_failures++;

            ESP_LOGW(TAG, "BME280 read failed %u/%u: temp=%s humidity=%s pressure=%s",
                bme280_read_failures,
                BME280_MAX_READ_FAILURES,     
                esp_err_to_name(temp_result),
                esp_err_to_name(humidity_result),
                esp_err_to_name(pressure_result));

            if (bme280_read_failures >= BME280_MAX_READ_FAILURES) {
                ESP_LOGW(TAG, "BME280 marked disconnected after repeated read failures. Please restart application.");
                bme280_ready = false;
                bme280_read_failures = 0;
                if (bme280 != NULL) {
                    bme280_delete(&bme280);
                }
            }
            sensor->valid = false;
            publish_sensor_data(sensor);
            return false;
        }

        bme280_read_failures = 0;

        sensor->temperature = temperature;
        sensor->humidity = humidity;
        sensor->pressure = pressure;
        sensor->valid = true;
        sensor->last_update_seconds = esp_timer_get_time() / 1000000ULL;

        ESP_LOGI(TAG, "BME280: %.2f C, %.2f %%RH, %.2f hPa",
                 temperature, humidity, pressure);

        publish_sensor_data(sensor);
        return true;
    }
}


void Sensor_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;

    //Sensor_Init(app);

    //Sensor_Queue = xQueueCreate(1, sizeof(hal::TemperatureReading));
    Sensor_Queue = xQueueCreate(1, sizeof(TemperatureReadingInC));

    if (Sensor_Queue == NULL)
    {
        ESP_LOGW(TAG, "Failed to create sensor queue!");
    }

    Humidity_Queue = xQueueCreate(1, sizeof(HumidityReadingInC));
    if (Humidity_Queue == NULL)
    {
        ESP_LOGW(TAG, "Failed to create humidity queue!");
    }

    temperature_sensor = hal::BME280Sensor();
    
    hal::BME280Sensor BME280_sensor_object = hal::BME280Sensor();

    hal::BME280Sensor& humidity_sensor = BME280_sensor_object;

    // humidity_sensor = &hal::BME280Sensor();
    // humidity_sensor = &temperature_sensor;

    temperature_sensor.bme280_sensor_init();

    //hal::BME280Sensor bmeSensor = hal::BME280Sensor();
    while (1) {
        //Sensor_Read_v2(bmeSensor);
        Sensor_Read_v2();
        //Sensor_Read(&app->sensor_data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
