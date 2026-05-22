#ifndef I2C_HPP
#define I2C_HPP

#include "esp_err.h"
#include "i2c_bus.h"

class I2CBus {
    public:
        virtual esp_err_t readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t* data, size_t len) = 0;
}


#endif