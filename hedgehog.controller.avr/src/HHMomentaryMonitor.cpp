#include "HHMomentaryMonitor.h"

HHMomentaryMonitor::HHMomentaryMonitor(uint8_t addr, uint8_t intPinA, uint8_t intPinB, volatile state_t& state)
    : _addr{addr}, _intPinA{intPinA}, _intPinB{intPinB}, _state{state}
{
    pinMode(_intPinA, INPUT_PULLUP);
    pinMode(_intPinB, INPUT_PULLUP);

    _instance = this;
}

/** Statics */

void HHMomentaryMonitor::ISRA()
{
    uint8_t cap;
    {
        HHInterruptLock lock{_instance->_intPinA, ISRA};
        cap = _instance->readIntCapA();
    }

    if (bitRead(cap, 0) == 0)
    {
        _instance->_state.updateLeds(0, 255, 0);
    }
    _instance->_state.updateIntCapA(cap);
}

void HHMomentaryMonitor::ISRB()
{
    uint8_t cap;
    {
        HHInterruptLock lock{_instance->_intPinB, ISRB};
        cap = _instance->readIntCapB();
    }

    if (bitRead(cap, 0) == 0)
    {
        _instance->_state.updateLeds(255, 0, 0);
    }
    _instance->_state.updateIntCapB(cap);
}

HHMomentaryMonitor* HHMomentaryMonitor::_instance = nullptr;

/* end Statics **/

void HHMomentaryMonitor::initialize()
{
    _writeRegister(_reg_addr::GPPUA, 0xFF);
    _writeRegister(_reg_addr::GPPUB, 0xFF);
    _writeRegister(_reg_addr::GPINTENA, 0xFF);
    _writeRegister(_reg_addr::GPINTENB, 0xFF);
    HH_ATTACH_INT(_intPinA, ISRA);
    HH_ATTACH_INT(_intPinB, ISRB);
}

int HHMomentaryMonitor::readIntPinA() const
{
    return digitalRead(_intPinA);
}

int HHMomentaryMonitor::readIntPinB() const
{
    return digitalRead(_intPinB);
}

uint8_t HHMomentaryMonitor::readIntCapA() const
{
    return _readRegister(_reg_addr::INTCAPA);
}

uint8_t HHMomentaryMonitor::readIntCapB() const
{
    return _readRegister(_reg_addr::INTCAPB);
}

uint8_t HHMomentaryMonitor::getPortA() const
{
    return _readRegister(_reg_addr::GPIOA);
}

uint8_t HHMomentaryMonitor::getPortB() const
{
    return _readRegister(_reg_addr::GPIOB);
}

void HHMomentaryMonitor::_writeRegister(uint8_t reg_addr, uint8_t value) const
{
    Wire.beginTransmission(_addr);
    Wire.write(reg_addr);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t HHMomentaryMonitor::_readRegister(uint8_t reg_addr) const
{
    Wire.beginTransmission(_addr);
    Wire.write(reg_addr);
    Wire.endTransmission();
    Wire.requestFrom(_addr, uint8_t(1));
    return Wire.read();
}