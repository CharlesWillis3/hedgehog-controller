#ifndef HHMCP23017_H_
#define HHMCP23017_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "HedgehogController.h"

class HHMCP23017
{
public:
    HHMCP23017(uint8_t addr, volatile state_t& state);

    uint8_t getPortA() const;
    uint8_t getPortB() const;
    int readIntPinA() const;
    int readIntPinB() const;
    void initialize();
    void poll();
    void update(uint64_t curr_millis);

private:
    enum _reg_addr : uint8_t
    {
        GPINTENA = 0x04,
        GPINTENB = 0x05,
        GPPUA = 0x0C,
        GPPUB = 0x0D,
        GPIOA = 0x12,
        GPIOB = 0x13,
    }; 

    const uint64_t _poll_interval_ms = 5;
    uint64_t _prev_millis;

    uint8_t _addr;
    volatile state_t& _state;

    uint8_t _readRegister(uint8_t reg_addr) const;
    void _writeRegister(uint8_t reg_addr, uint8_t value) const;
};

#endif