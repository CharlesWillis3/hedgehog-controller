#include "HHMCP23017.h"

HHMCP23017::HHMCP23017(uint8_t addr, volatile state_t &state)
    : _addr{addr}, _state{state}
{
}

void HHMCP23017::initialize()
{
    _writeRegister(_reg_addr::GPPUA, 0xFF);
    _writeRegister(_reg_addr::GPPUB, 0xFF);
}

void HHMCP23017::poll()
{
    _state.update_buttons_inv(getPortA(), getPortB());
    _state.pollCount += 1;
}

void HHMCP23017::update(uint64_t curr_millis)
{
    if (curr_millis - _prev_millis >= _poll_interval_ms)
    {
        poll();
    }
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