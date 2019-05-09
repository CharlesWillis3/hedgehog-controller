#ifndef HHDebugger_H_
#define HHDebugger_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "HedgehogController.h"
#include "HHMomentaryMonitor.h"

class HHDebugger
{
public:
  HHDebugger(const HHMomentaryMonitor& momon);
  void startDebugging();
  void reportButtons();

private:
  const HHMomentaryMonitor& _momon;
  bool _isDebugging;
  void _sendDevice();
  void _sendJoysticks();
  void _reportButtons();
};

#endif