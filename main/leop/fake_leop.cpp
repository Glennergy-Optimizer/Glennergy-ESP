#include "fake_leop.hpp"

// Simple fill just to get some dummy data
void fake_leop_fill(leop_data_t* leop) {
    leop->id = 1;
    leop->entry_count = 3;
    leop->entries[0] = {"2000-01-01 10:45", 0.15, recommendation_type_t(1)}; //buy
    leop->entries[1] = {"2000-01-01 10:45", 0.85, recommendation_type_t(2)}; //sell
    leop->entries[2] = {"2000-01-01 10:45", 0.50, recommendation_type_t(3)}; //hold
}

void fake_leop_update(leop_data_t* leop) {
    
}
