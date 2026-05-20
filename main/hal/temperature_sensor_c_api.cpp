#include "temperature_sensor_c_api.h"
#include "bme280_sensor.hpp"
#include "../app_types.h"

hal::BME280Sensor sensor;
hal::TemperatureReading temp;


/*
temp_read() {
    sensor.read(temp);
}
*/
hal::SensorError sensor_read(hal::TemperatureReading& temperature) {
    hal::SensorError sensorResult;
    sensorResult = sensor.read(temperature);
    return sensorResult;
}




TemperatureReadingInC temperature_sensor_get_celcius(hal::TemperatureReading& reading) {
    //sensor.read(temp); // fyller temp-strukten med temperatur

    TemperatureReadingInC CStruct;
    CStruct.celcius = reading.celcius;

    return CStruct;
}

bool temperature_sensor_is_available(void) {
    return sensor.is_present();
}