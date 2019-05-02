#ifndef HHProgrammer_H_
#define HHProgrammer_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "HedgehogEnums.h"
#include "HedgehogData.h"

class HHProgrammer
{
public:
  HHProgrammer(uint8_t digitals, uint8_t analogs, uint8_t neopixels);
  void startProgramming();
  void updateNeoPixels();

private:
  uint8_t _digitals;
  uint8_t _analogs;
  uint8_t _neopixels;
  bool _isProgramming;
  void _receiveJoystickData();
  void _receiveDeviceData();
  void _sendHardwareInfo();
};

#endif