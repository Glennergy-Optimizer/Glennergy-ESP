#include "temperature_sensor_c_api.h"
#include "bme280_sensor.hpp"
#include "../app_types.h"
//#include "temperature_sensor.hpp"


hal::BME280Sensor temperature_sensor;
hal::TemperatureReading temp;


/*
temp_read() {
    sensor.read(temp);
}
*/
hal::SensorError sensor_read(hal::TemperatureReading& temperature) {
    hal::SensorError sensorResult;
    sensorResult = temperature_sensor.read(temperature);
    return sensorResult;
}




TemperatureReadingInC temperature_sensor_get_celcius(hal::TemperatureReading& reading) {
    //sensor.read(temp); // fyller temp-strukten med temperatur

    TemperatureReadingInC CStruct;
    CStruct.celcius = reading.celcius;

    return CStruct;
}

bool temperature_sensor_is_available(void) {
    return temperature_sensor.is_present();
}