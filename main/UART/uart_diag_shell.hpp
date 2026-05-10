#ifndef UART_DIAG_SHELL_HPP
#define UART_DIAG_SHELL_HPP

#include <string>
#include "../app_types.h"

// So we need a function which reads the app state.
void handle_input(const std::string& input, app_state_t* state);

#endif
