#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensor.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "../app_queues.h"
#include "../hal/bme280_sensor.hpp"
// the C api is currently unused
/*
#include "../hal/temperature_sensor_c_api.h"
#include "../hal/temperature_sensor.hpp"
#include "../hal/humidity_sensor.hpp"
#include "../hal/humidity_sensor_c_api.h"
#include "../hal/pressure_sensor.hpp"
#include "../hal/pressure_sensor_c_api.h"
*/
//#include "../app_types.h"

static constexpr char* TAG = "Sensor.cpp";
static bool fake_mode = false;


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

static void publish_pressure_data(PressureReadingInC* pressureReadingInC)
{
    if (Pressure_Queue != NULL) {
        xQueueOverwrite(Pressure_Queue, pressureReadingInC);
    }
}


void Sensor_Init_v2(app_state_t* app) 
{
    app->sensor_data.valid = false;
    app->sensor_data.last_update_seconds = 0;
    app->sensor_data.temperature = 0;
    app->sensor_data.pressure = 0;
    app->sensor_data.humidity = 0;

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

    Pressure_Queue = xQueueCreate(1, sizeof(PressureReadingInC));
    if (Pressure_Queue == NULL) {
        ESP_LOGW(TAG, "Failed to create pressure queue!");
    }
}

// Todo as of 2026-05-25 - Let's not overabstract since we only have a single bme280 sensor for now.
// 
bool Sensor_Read_v2(sensor_data_t* sensor, hal::BME280Sensor& environment_sensor)
{
    hal::TemperatureReading temperatur = hal::TemperatureReading();
    hal::HumidityReading humidityReading = hal::HumidityReading();
    hal::PressureReading pressureReading = hal::PressureReading();

    hal::SensorError result = environment_sensor.read(temperatur);
    hal::SensorError humidityResult = environment_sensor.read(humidityReading);
    hal::SensorError pressureResult = environment_sensor.read(pressureReading);

    if (result != hal::SensorError::Ok || humidityResult != hal::SensorError::Ok || pressureResult != hal::SensorError::Ok) {
        ESP_LOGW(TAG, "Something went wrong with reading data from sensor.\nTemperature code: %d, humidity code: %d", static_cast<int>(result), static_cast<int>(humidityResult));
        // Om något inte är okej med error codes för SensorError så hanteras det här.
        // Om något inte är okej, oavsett anledning, så blir sensor->valid false.
        // Då uppdateras inte värderna i strukten.
        // UIn visar då de senaste värderna, med (TODO-fix this) en timestamp och något typ av errornotering, så användaren lätt vet att tex 15:37:02 var senaste OK sensorreaden
        sensor->valid = false;
        // Flyttat ut adderingen av last_reconnect_attempt_ms här, så den bara körs 1 gång om något failar, inte 3 fails per attempt(då vi gör en read på temp, en på humidity och en på pressure)
        environment_sensor.increment_read_failure();
        return false;
    }

    // Todo - validation of SensorError code
    TemperatureReadingInC Ctemperature;
    Ctemperature.celcius = temperatur.celcius;

    HumidityReadingInC Chumidity;
    Chumidity.humidity = humidityReading.humidity;

    PressureReadingInC Cpressure;
    Cpressure.pressure = pressureReading.pressure;

    sensor->temperature = Ctemperature.celcius;
    sensor->humidity = Chumidity.humidity;
    sensor->pressure = Cpressure.pressure;
    // Todo - använda SensorError enum och koppla till valid?
    sensor->valid = true;
    // Todo - Låta read skicka temperatur?
    sensor->last_update_seconds = esp_timer_get_time() / 1000000ULL;


    publish_temperature_data(&Ctemperature);
    publish_humidity_data(&Chumidity);
    publish_pressure_data(&Cpressure);

    return true;
}

void Sensor_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;
    // Todo - ska default-config värden, om inget existerar i caches, 
    // skapas/initeras här(där respektive modul initieras de värden som är relevanta) eller i main?
    app->config_data.sensor_interval_ms = 1000; 
    uint32_t sensor_read_interval;

    Sensor_Init_v2(app);
    hal::BME280Sensor environment_sensor = hal::BME280Sensor();
    environment_sensor.bme280_sensor_init();
    while (1) {
        sensor_read_interval = app->config_data.sensor_interval_ms;
        Sensor_Read_v2(&app->sensor_data, environment_sensor);
        vTaskDelay(pdMS_TO_TICKS(sensor_read_interval));
    }
}
