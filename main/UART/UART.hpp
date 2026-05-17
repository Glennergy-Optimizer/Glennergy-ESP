#ifndef UART_HPP
#define UART_HPP

#include <stdint.h> // c-version since main compiles this file
#include "freertos/FreeRTOS.h"
#include "../app_types.h"


// c++ exposed functions
#ifdef __cplusplus
bool UART_ReadByte(uint8_t* byte, TickType_t timeout);
#endif

#ifdef __cplusplus
    extern "C" {
#endif
// c exposed functions
void UART_Work(void* parameter);


#ifdef __cplusplus
    }
#endif

#endif
