#ifndef LEOP_H
#define LEOP_H

#include "../app_types.h"

/**
 * @file leop.h
 * @brief Public API for the LEOP module.
 *
 * Provides initialization, data access, and worker entry points for the LEOP
 * data path.
 *
 * @defgroup LEOP LEOP
 * @brief LEOP data handling module.
 *
 * The module initializes LEOP state, exposes the latest data snapshot, and
 * provides a worker entry point used by the application task setup.
 * @{
 */

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Initializes the LEOP module state.
 *
 * @param[in] app Application state that contains LEOP storage.
 */
void Leop_Init(app_state_t* app);

/**
 * @brief Retrieves the current LEOP data snapshot.
 *
 * @param[in,out] leop Destination for the LEOP data.
 *
 * @return `true` when data was provided, otherwise `false`.
 */
bool Leop_GetData(LEOPData leop);

/**
 * @brief Worker entry point for LEOP processing.
 *
 * @param[in] parameter Pointer to the application state passed to the worker.
 */
void Leop_Work(void* parameter);

#ifdef __cplusplus
    }
#endif

/** @} */

#endif
