#ifndef HHDebugger_H_
#define HHDebugger_H_

#include <EEPROM.h>
#include <Arduino.h>
#include "HedgehogEnums.h"
#include "HedgehogData.h"

class HHDebugger
{
public:
  HHDebugger();
  void startDebugging();
  void updateNeoPixels();

private:
  bool _isDebugging;
  void _sendDevice();
  void _sendJoysticks();
};

#endif