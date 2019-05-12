#ifndef HHMCP23017_H_
#define HHMCP23017_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "HedgehogController.h"

class HHMCP23017
{
public:
    HHMCP23017(
        uint8_t addr,
        uint8_t intPinA,
        uint8_t intPinB,
        volatile state_t& state);
    static void ISRA();
    static void ISRB();
    uint8_t getPortA() const;
    uint8_t getPortB() const;
    int readIntPinA() const;
    int readIntPinB() const;
    uint8_t readIntCapA() const;
    uint8_t readIntCapB() const;
    void initialize();

private:
    enum _reg_addr : uint8_t
    {
        GPINTENA = 0x04,
        GPINTENB = 0x05,
        GPPUA = 0x0C,
        GPPUB = 0x0D,
        INTCAPA = 0x10,
        INTCAPB = 0x11,
        GPIOA = 0x12,
        GPIOB = 0x13,
    };

    static HHMCP23017* _instance;

    uint8_t _addr;
    uint8_t _intPinA;
    uint8_t _intPinB;
    volatile state_t& _state;

    uint8_t _readRegister(uint8_t reg_addr) const;
    void _writeRegister(uint8_t reg_addr, uint8_t value) const;
};

#endif