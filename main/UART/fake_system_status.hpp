#ifndef FAKE_SYSTEM_STATUS_HPP
#define FAKE_SYSTEM_STATUS_HPP

#include "uart_diag_shell.hpp"

void fake_system_status_fill(system_status_t* system);

void fake_system_status_update(system_status_t* system);

#endif