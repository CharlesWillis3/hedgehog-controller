#include "HHProgrammer.h"

HHProgrammer::HHProgrammer(uint8_t digitals, uint8_t analogs, uint8_t neopixels)
{
    _digitals  = digitals;
    _analogs   = analogs;
    _neopixels = neopixels;
}

void HHProgrammer::startProgramming()
{
    Serial.write(HHSerial::MODE_PROGRAMMING);

    _isProgramming = true;
    uint8_t rcvd;

    while (Serial.readBytes(&rcvd, 1) && rcvd != HHSerial::MODE_RUN)
    {
        switch (rcvd)
        {
        case HHSerial::REQUEST_HARDWARE:
            _sendHardwareInfo();
            break;
        case HHSerial::DEVICE_START:
            _receiveDeviceData();
            break;
        case HHSerial::JOYSTICK_START:
            _receiveJoystickData();
            break;
        default:
            Serial.write(HHSerial::ERROR);
            break;
        }
    }

    _isProgramming = false;
}

void HHProgrammer::_receiveDeviceData()
{
    uint8_t raw[4];
    Serial.write(HHSerial::READY);
    Serial.readBytes(raw, 4);
    device_t device;
    memcpy(&device, raw, 4);
    EEPROM.put(0, device);
    Serial.write(HHSerial::READY);
}

void HHProgrammer::_receiveJoystickData()
{
    uint8_t raw[4];
    joystick_t joystick;
    Serial.write(HHSerial::READY);
    for (int x = 0; x < 4; x++)
    {
        Serial.readBytes(raw, 4);
        memcpy(&joystick, raw, 4);
        EEPROM.put(HHOffset::JOYSTICK0 + (x * sizeof(joystick_t)), joystick);
    }
    Serial.write(HHSerial::READY);
}

void HHProgrammer::_sendHardwareInfo()
{
    uint8_t info[3];
    info[HHHardwareInfo::NUM_DIGITALS]  = _digitals;
    info[HHHardwareInfo::NUM_ANALOGS]   = _analogs;
    info[HHHardwareInfo::NUM_NEOPIXELS] = _neopixels;
    Serial.write(info, 3);
}