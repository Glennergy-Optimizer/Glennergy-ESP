#ifndef LEOP_H
#define LEOP_H

#include "../app_types.h"

#ifdef __cplusplus
    extern "C" {
#endif

void Leop_Init(app_state_t* app);
bool Leop_GetData(leop_data_t* leop);

void Leop_Work(void* parameter);

#ifdef __cplusplus
    }
#endif


#endif