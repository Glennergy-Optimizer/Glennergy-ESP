/**
 * @file fake_system_status.hpp
 * @brief Fake system status helpers for test and simulation builds.
 *
 * Provides simple helpers that initialize and mutate a system status snapshot
 * used by the fake implementation.
 *
 * @ingroup FAKE
 */

#ifndef FAKE_SYSTEM_STATUS_HPP
#define FAKE_SYSTEM_STATUS_HPP

//#include "uart_diag_shell.hpp"
#include "../app_types.h"

/**
 * @brief Initializes a system status snapshot with fake values.
 *
 * @param[in,out] system Pointer to the status structure to populate.
 */
void fake_system_status_fill(system_status_t* system);

/**
 * @brief Updates a system status snapshot with fake values.
 *
 * @param[in,out] system Pointer to the status structure to update.
 */
void fake_system_status_update(system_status_t* system);

#endif
