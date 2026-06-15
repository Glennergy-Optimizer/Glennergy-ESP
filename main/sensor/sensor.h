#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "../app_types.h"

// sensor.h needs to be a normal header since our main.c/program is in C.
// So sensor.cpp includes this, but Extern C tells compiler to use C linkage instead of C++ linkage
#ifdef __cplusplus
    extern "C" {
#endif

//void Sensor_Init(app_state_t* app);
//bool Sensor_Read(sensor_data_t* out);

void Sensor_Work(void* parameter);

#ifdef __cplusplus
    }
#endif


#endif