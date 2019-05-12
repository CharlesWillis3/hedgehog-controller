// #include "HHDebugger.h"

// HHDebugger::HHDebugger()
// {
// }

// void HHDebugger::startDebugging()
// {
//     HHS_SEND_MSG(HHSerial::MODE_DEBUG);
//     _isDebugging = true;

//     HHSerial rcvd;
//     while ((rcvd = HHS_BLOCK) != HHSerial::DEBUG_END)
//     {
//         switch (rcvd)
//         {
//         case HHSerial::DEVICE_READ:
//             _sendDevice();
//             break;
//         case HHSerial::JOYSTICKS_READ:
//             _sendJoysticks();
//             break;
//         case HHSerial::BUTTONS_ID:
//             _reportButtons();
//             break;
//         default:
//             HHSM_ERROR;
//             break;
//         }
//     }

//     _isDebugging = false;
// }

// void HHDebugger::_sendDevice()
// {
//     uint8_t snd;
//     for (unsigned int x = HHOffset::DEVICE; x < sizeof(device_t); x++)
//     {
//         snd = EEPROM.read(x);
//         Serial.write(snd);
//     }
// }

// void HHDebugger::_sendJoysticks()
// {
//     uint8_t snd;
//     for (int x = 0; x < 4; x++)
//     {
//         for (unsigned int y = 0; y < sizeof(joystick_t); y++)
//         {
//             snd = EEPROM.read(HHOffset::JOYSTICK0 + (x * sizeof(joystick_t) + y));
//             Serial.write(snd);
//         }
//     }
// }

// void HHDebugger::reportButtons()
// {
//     _reportButtons();
// }

// void HHDebugger::_reportButtons()
// {
//     Serial.write(uint8_t(0));
//     // Serial.write(_momon.getPortA());
//     // Serial.write(_momon.getPortB());
//     // Serial.write(_momon.readIntCapA());
//     // Serial.write(_momon.readIntCapB());
//     // Serial.write(_momon.readIntPinA());
//     // Serial.write(_momon.readIntPinB());
// }