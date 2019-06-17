#ifndef HHLEDDEMUX_H_
#define HHLEDDEMUX_H_

#include <Arduino.h>
#include "HedgehogController.h"

class HHLedDemux
{
public:
    HHLedDemux();
    void initialize();
    void change_duty_cycle(uint8_t led_id, int duty_cycle);
    void refresh();

private:
    int _id;
    int _currentDutyCycle[LED_COUNT];
};

#endif