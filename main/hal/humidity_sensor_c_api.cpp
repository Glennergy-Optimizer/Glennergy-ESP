#include "bme280_sensor.hpp"
#include "humidity_sensor.hpp"
#include "humidity_sensor_c_api.h"



// TODO - Detta bör vara en pekare?
// På så vis kan man skapa sensorer, 1 eller flera, beroende på vilka man behöver.
// I vårt fall blir det bara 1 sensor, men att modulers lokala kod pekar på en humidity_sensor, en temperature_sensor osv.
// Men i vårt fall kommer dessa pekare vara på en singulär 280 sensor?
//extern hal::BME280Sensor original_sensor_object;
//hal::BME280Sensor& humidity_sensor = original_sensor_object;
//hal::BME280Sensor& humidity_sensor = original_sensor_object;
extern hal::BME280Sensor& humidity_sensor;

hal::SensorError sensor_read(hal::HumidityReading& humidity) {
    hal::SensorError sensorResult;
    sensorResult = humidity_sensor.read(humidity);
    return sensorResult;
}

HumidityReadingInC humidity_sensor_get_celcius(hal::HumidityReading& reading) {
    HumidityReadingInC CStruct;
    CStruct.humidity = reading.humidity;

    return CStruct;
}

// Todo - Ärligt talat, börjat det bli för mycket abstraktion?
// Vi vet att vi bara kommer köra bme280?
// Aja, testprojekt, vi testar en översiktlig struktur så har vi testat det en gång.
bool humidity_sensor_is_available(void) {
    return humidity_sensor.is_present();
}