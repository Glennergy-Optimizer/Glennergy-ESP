/**
 * @file sensor.cpp
 * @brief Implementation of the sensor worker and queue handoff logic.
 *
 * @ingroup SENSOR
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensor.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "../app_queues.h"
#include "../hal/bme280_sensor.hpp"

/**
 * @brief Module tag used for ESP-IDF logging.
 */
static constexpr char* TAG = "Sensor.cpp";
static constexpr time_t MIN_VALID_UNIX_TIME = 1704067200; // 2024-01-01 00:00:00 UTC
static bool fake_mode = false;

/**
 * @brief Queue used to publish the latest sensor snapshot.
 *
 * The queue holds a single `sensor_data_t` item and is overwritten with the
 * latest reading.
 */
QueueHandle_t Sensor_Queue = NULL;

/**
 * @brief Initializes sensor state and the latest-value queue.
 *
 * @param[in,out] app Application state that receives the initial sensor values.
 */
void Sensor_Init_v2(app_state_t* app) 
{
    app->sensor_data.valid = false;
    app->sensor_data.last_update_seconds = 0;
    app->sensor_data.last_unix_time = 0;
    app->sensor_data.wall_time_valid = false;
    app->sensor_data.temperature = 0;
    app->sensor_data.pressure = 0;
    app->sensor_data.humidity = 0;

    Sensor_Queue = xQueueCreate(1, sizeof(sensor_data_t));

    if (Sensor_Queue == NULL)
    {
        ESP_LOGW(TAG, "Failed to create sensor queue!");
    }

}

// Todo as of 2026-05-25 - Let's not overabstract since we only have a single bme280 sensor for now.
// 
/**
 * @brief Reads the BME280 sensor and updates the application snapshot.
 *
 * On failure, the current snapshot is marked invalid and the sensor read
 * failure counter is incremented once for the attempted sample.
 *
 * @param[in,out] sensor Destination for the latest sensor values.
 * @param[in,out] environment_sensor BME280 driver instance used for reading.
 *
 * @return `true` when all readings succeed; otherwise `false`.
 */
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
        // UIn ցույց shows the latest values, with (TODO-fix this) a timestamp and some kind of error note, so the user can easily see that for example 15:37:02 was the latest OK sensor read
        sensor->valid = false;
        // Flyttat ut adderingen av last_reconnect_attempt_ms här, så den bara körs 1 gång om något failar, inte 3 fails per attempt(då vi gör en read på temp, en på humidity och en på pressure)
        environment_sensor.increment_read_failure();
        return false;
    }

    sensor->temperature = temperatur.celcius;
    sensor->humidity = humidityReading.humidity;
    sensor->pressure = pressureReading.pressure;
    // Todo - använda SensorError enum och koppla till valid?
    sensor->valid = true;
    // Todo - Låta read skicka temperatur?
    sensor->last_update_seconds = esp_timer_get_time() / 1000000ULL;
    sensor->last_unix_time = temperatur.unix_timestamp;
    sensor->wall_time_valid = sensor->last_unix_time >= MIN_VALID_UNIX_TIME;

    sensor_data_t sensor_snapshot = *sensor;
    xQueueOverwrite(Sensor_Queue, &sensor_snapshot);

    return true;
}

/**
 * @brief Background task that initializes the BME280 and performs periodic reads.
 *
 * The task runs in FreeRTOS task context, delays during startup, then polls the
 * configured interval and publishes each sample through the shared sensor state.
 *
 * @param[in] parameter Pointer to `app_state_t`.
 */
void Sensor_Work(void* parameter) {
    app_state_t* app = (app_state_t*)parameter;
    // Todo - ska default-config värden, om inget existerar i caches, 
    // skapas/initeras här(där respektive modul initieras de värden som är relevanta) eller i main?
    //app->config_data.sensor_interval_ms = 1000; 
    uint32_t sensor_read_interval;

    Sensor_Init_v2(app);
    vTaskDelay(pdMS_TO_TICKS(3000));
    hal::BME280Sensor environment_sensor = hal::BME280Sensor();
    environment_sensor.bme280_sensor_init();
    while (1) {
        sensor_read_interval = app->config_data.sensor_interval_ms;
        Sensor_Read_v2(&app->sensor_data, environment_sensor);
        vTaskDelay(pdMS_TO_TICKS(sensor_read_interval));
    }
}
