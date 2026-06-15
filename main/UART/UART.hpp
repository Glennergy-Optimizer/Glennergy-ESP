#ifndef UART_HPP
#define UART_HPP

/**
 * @file UART.hpp
 * @brief Public declarations for the UART task and helper functions.
 *
 * Provides the C and C++ entry points used by the UART console worker.
 *
 * @ingroup UART
 */

#include <stdint.h> // c-version since main compiles this file
#include "freertos/FreeRTOS.h"
#include "../app_types.h"


/**
 * @brief Reads one byte from the UART console.
 *
 * @param[out] byte Destination for the received byte.
 * @param[in] timeout UART read timeout in FreeRTOS ticks.
 *
 * @return `true` if a byte was read, otherwise `false`.
 */
 // c++ exposed functions
#ifdef __cplusplus
bool UART_ReadByte(uint8_t* byte, TickType_t timeout);
#endif

#ifdef __cplusplus
    extern "C" {
#endif
/**
 * @brief UART worker task entry point.
 *
 * @param parameter Pointer to the application state passed to the task.
 *
 * @note Runs in task context and blocks while waiting for UART input.
 */
 // c exposed functions
void UART_Work(void* parameter);


#ifdef __cplusplus
    }
#endif

#endif
