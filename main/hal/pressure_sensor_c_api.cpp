#include "pressure_sensor_c_api.h"
#include "bme280_sensor.hpp"
#include "../app_types.h"

extern hal::BME280Sensor& pressure_sensor;

hal::SensorError sensor_read(hal::PressureReading& pressure) {
    hal::SensorError sensorResult;
    sensorResult = pressure_sensor.read(pressure);

    return sensorResult;
}


PressureReadingInC pressure_sensor_get_celcius(hal::PressureReading& reading) {
    PressureReadingInC CStruct;
    CStruct.pressure = reading.pressure;

    return CStruct;
}

bool pressure_sensor_is_available(void) {
    return pressure_sensor.is_present();
}