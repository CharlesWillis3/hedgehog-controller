#include "HHMCP23017.h"

HHMCP23017::HHMCP23017(
        uint8_t addr,
        uint8_t intPinA,
        uint8_t intPinB,
        volatile state_t& state)
    : _addr{addr}, _intPinA{intPinA}, _intPinB{intPinB}, _state{state}
{
    pinMode(_intPinA, INPUT_PULLUP);
    pinMode(_intPinB, INPUT_PULLUP);

    _instance = this;
}

/** Statics */

void HHMCP23017::ISRA()
{
    uint8_t cap;
    {
        HHInterruptLock lock{_instance->_intPinA, ISRA};
        cap = _instance->readIntCapA();
    }

    BYTE_LOOP(x, 8)
    {
        _instance->_state.updatePortA(x, bitRead(cap, x) ? IB_DOWN : IB_UP);
    }
}

void HHMCP23017::ISRB()
{
    uint8_t cap;
    {
        HHInterruptLock lock{_instance->_intPinB, ISRB};
        cap = _instance->readIntCapB();
    }

    BYTE_LOOP(x, 8)
    {
        _instance->_state.updatePortB(x, bitRead(cap, x) ? IB_DOWN : IB_UP);
    }
}

HHMCP23017* HHMCP23017::_instance = nullptr;

/* end Statics **/

void HHMCP23017::initialize()
{
    _writeRegister(_reg_addr::GPPUA, 0xFF);
    _writeRegister(_reg_addr::GPPUB, 0xFF);
    _writeRegister(_reg_addr::GPINTENA, 0xFF);
    _writeRegister(_reg_addr::GPINTENB, 0xFF);
    HH_ATTACH_INT(_intPinA, ISRA);
    HH_ATTACH_INT(_intPinB, ISRB);
}

int HHMCP23017::readIntPinA() const
{
    return digitalRead(_intPinA);
}

int HHMCP23017::readIntPinB() const
{
    return digitalRead(_intPinB);
}

uint8_t HHMCP23017::readIntCapA() const
{
    return _readRegister(_reg_addr::INTCAPA);
}

uint8_t HHMCP23017::readIntCapB() const
{
    return _readRegister(_reg_addr::INTCAPB);
}

uint8_t HHMCP23017::getPortA() const
{
    return _readRegister(_reg_addr::GPIOA);
}

uint8_t HHMCP23017::getPortB() const
{
    return _readRegister(_reg_addr::GPIOB);
}

void HHMCP23017::_writeRegister(uint8_t reg_addr, uint8_t value) const
{
    Wire.beginTransmission(_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t HHMCP23017::_readRegister(uint8_t reg_addr) const
{
    Wire.beginTransmission(_addr);
    Wire.write(reg_addr);
    Wire.endTransmission();
    Wire.requestFrom(_addr, uint8_t(1));
    return Wire.read();
}