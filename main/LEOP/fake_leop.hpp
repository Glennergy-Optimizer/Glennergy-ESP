#ifndef FAKE_LEOP_HPP
#define FAKE_LEOP_HPP

#include "../app_types.h"

/**
 * @file fake_leop.hpp
 * @brief Public declarations for the fake LEOP data helpers.
 *
 * @ingroup LEOP
 */

/**
 * @defgroup LEOP LEOP
 * @brief LEOP data helpers and related types.
 *
 * Provides lightweight helpers for populating and updating LEOP test data.
 * These helpers are intended for local data preparation and do not change any
 * hardware state.
 * @{
 */

/**
 * @brief Fills a LEOPData instance with fake data.
 *
 * @param[in,out] leop Pointer to the LEOPData instance to populate.
 */
void fake_leop_fill(LEOPData* leop);

/** @} */

#endif
