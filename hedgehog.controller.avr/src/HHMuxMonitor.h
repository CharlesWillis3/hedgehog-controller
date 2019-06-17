#ifndef HHMUXMONITOR_H_
#define HHMUXMONITOR_H_

#include <Arduino.h>
#include "HedgehogController.h"

class HHMuxMonitor
{
public:
    HHMuxMonitor(volatile state_t& state);
    void initialize();
    void update(millis_t curr_millis);

private:
    volatile state_t& _state;
    millis_t _last_update;
};

#endif