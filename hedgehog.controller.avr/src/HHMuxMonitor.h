#ifndef HHMUXMONITOR_H_
#define HHMUXMONITOR_H_

#include <Arduino.h>
#include "HedgehogController.h"

class HHMuxMonitor
{
public:
    HHMuxMonitor(volatile state_t& state);
    void update(millis_t curr_millis);
    void initialize();

private:
    const millis_t _update_interval = 5;

    volatile state_t& _state;
    millis_t _last_update;
};

#endif