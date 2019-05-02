#include "HHDebugger.h"

HHDebugger::HHDebugger()
{
}

void HHDebugger::startDebugging()
{

    Serial.write(HHSerial::MODE_DEBUG);
    _isDebugging = true;

    uint8_t rcvd;
    while (Serial.readBytes(&rcvd, 1) && rcvd != HHSerial::DEBUG_END)
    {
        size_t n_bytes = Serial.readBytes(&rcvd, 1);

        switch (rcvd)
        {
        case HHSerial::DEVICE_READ:
            _sendDevice();
            break;
        case HHSerial::JOYSTICKS_READ:
            _sendJoysticks();
            break;
        default:
            Serial.write(HHSerial::ERROR);
            break;
        }
    }

    _isDebugging = false;
}

void HHDebugger::updateNeoPixels()
{
}

void HHDebugger::_sendDevice()
{
    uint8_t snd;
    for (int x = HHOffset::DEVICE; x < sizeof(device_t); x++)
    {
        snd = EEPROM.read(x);
        Serial.write(snd);
    }
}

void HHDebugger::_sendJoysticks()
{
    uint8_t snd;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < sizeof(joystick_t); y++)
        {
            snd = EEPROM.read(HHOffset::JOYSTICK0 + (x * sizeof(joystick_t) + y));
            Serial.write(snd);
        }
    }
}