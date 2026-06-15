/**
 * @file fake_leop.cpp
 * @brief Implementation of the fake LEOP data helpers.
 *
 * @ingroup LEOP
 */

#include "fake_leop.hpp"

// Simple fill just to get some dummy data
/**
 * @brief Implementation of fake_leop_fill.
 *
 * See header for full contract documentation.
 */
void fake_leop_fill(LEOPData* leop) {
    // leop->id = 1;
    // leop->entry_count = 3;
    // leop->entries[0] = {"2000-01-01 10:45", 0.15, recommendation_type_t(1)}; //buy
    // leop->entries[1] = {"2000-01-01 10:45", 0.85, recommendation_type_t(2)}; //sell
    // leop->entries[2] = {"2000-01-01 10:45", 0.50, recommendation_type_t(3)}; //hold
}

/**
 * @brief Implementation of fake_leop_update.
 *
 * See header for full contract documentation.
 */
void fake_leop_update(LEOPData* leop) {
    
}
