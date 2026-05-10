#include "fake_system_status.hpp"


static int counter = 0;

void fake_system_status_fill(system_status_t* system) {
    system->wifi_connected = false;
    system->leop_connected = true;
    system->update_counter = 0;
    system->sensor_ok = true;
}

void fake_system_status_update(system_status_t* system) {
    counter += 1;
    system->update_counter += 1;
    if (counter % 15) {
        system->wifi_connected = !system->leop_connected;
    }

    if (counter % 100) {
        system->sensor_ok = !system->sensor_ok;
    }

}
