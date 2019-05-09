#ifndef HEDGEHOG_CONTROLLER_H_
#define HEDGEHOG_CONTROLLER_H_

#include <Arduino.h>
#include "HedgehogEnums.h"
#include "HedgehogData.h"
#include "HedgehogSerial.h"

typedef struct State
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool updateLed;

    uint8_t intCapA;
    uint8_t intCapB;
    bool updateIntCap;

    void updateLeds(uint8_t red, uint8_t green, uint8_t blue) volatile
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->updateLed = true;
    }

    void updateIntCapA(uint8_t intCapA) volatile
    {
        this->intCapA = intCapA;
        this->updateIntCap = true;
    }

    void updateIntCapB(uint8_t intCapB) volatile
    {
        this->intCapB = intCapB;
        this->updateIntCap = true;
    }
} state_t;

#define HH_ATTACH_INT(x,y) attachInterrupt(digitalPinToInterrupt(x), y, FALLING);
#define HH_DETACH_INT(x) detachInterrupt(digitalPinToInterrupt(x));

class HHInterruptLock
{
public:
    HHInterruptLock(uint8_t pin, void (*const isr)())
        : _pin{pin}, _isr{isr}
    {
        noInterrupts();
        HH_DETACH_INT(_pin);
        interrupts();
    }

    ~HHInterruptLock()
    {
        HH_ATTACH_INT(_pin, _isr);
    }

private:
    uint8_t _pin;
    void (*const _isr)();
    HHInterruptLock(const HHInterruptLock&);
};

#endif