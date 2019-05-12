#ifndef HEDGEHOG_CONTROLLER_H_
#define HEDGEHOG_CONTROLLER_H_

#define __ASSERT_USE_STDERR

#include <assert.h>
#include <Arduino.h>
#include <TimerOne.h>

#include "HedgehogEnums.h"
#include "HedgehogData.h"
#include "HedgehogSerial.h"

#define USTOS(x) 1000UL * 1000UL * x
#define BYTE_LOOP(x, n) for (uint8_t x = 0; x < n; ++x)

enum InBtnState : uint8_t
{
    IB_UP = 0x00,
    IB_DOWN = 0x01,
    IB_RISING = 0x10,
    IB_FALLING = 0x20
};

inline bool processUpdate(volatile bool& flag);
inline void updateInBtn(volatile InBtnState& state, InBtnState nextState);

typedef struct State
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool updateLed;
    bool resetLed;

    InBtnState portA[8] = {IB_UP};
    bool hasUpdate_portA;
    InBtnState portB[8] = {IB_UP};
    bool hasUpdate_portB;

    State copyAndReset() volatile
    {
        State n;
        n.red = this->red;
        n.green = this->green;
        n.blue = this->blue;
        memcpy(n.portA, const_cast<InBtnState*>(this->portA), 8);
        memcpy(n.portB, const_cast<InBtnState*>(this->portB), 8);

        this->updateLed = false;
        this->resetLed = false;
        this->hasUpdate_portA = false;
        this->hasUpdate_portB = false;

        return n;
    }

    void updatePortA(uint8_t inBtnId, InBtnState inBtnState) volatile
    {
        assert(inBtnId < 8);
        updateInBtn(portA[inBtnId], inBtnState);
        hasUpdate_portA = true;
    }

    void updatePortB(uint8_t inBtnId, InBtnState inBtnState) volatile
    {
        assert(inBtnId < 8);
        updateInBtn(portB[inBtnId], inBtnState);
        hasUpdate_portB = true;
    }

    void updateLeds(uint8_t red, uint8_t green, uint8_t blue) volatile
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->updateLed = true;
    }
} state_t;

inline bool processUpdate(volatile bool& flag)
{
    if (flag)
    {
        flag = !flag;
        return true;
    }

    return false;
}

inline void updateInBtn(volatile InBtnState& state, InBtnState nextState)
{
    assert(nextState == IB_UP || nextState == IB_DOWN);

    switch (state)
    {
        case IB_UP:
        case IB_RISING:
            state = nextState == IB_UP ? IB_UP : IB_FALLING;
            break;
        case IB_DOWN:
        case IB_FALLING:
            state = nextState == IB_UP ? IB_RISING : IB_DOWN;
            break;
    }
}

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

class HHTimerOneInterruptLock
{
public:
    HHTimerOneInterruptLock(void (*const isr)())
        : _isr{isr}
    {
        noInterrupts();
        Timer1.detachInterrupt();
    }

    ~HHTimerOneInterruptLock()
    {
        interrupts();
        Timer1.attachInterrupt(_isr);
    }

private:
    void (*const _isr)();
    HHTimerOneInterruptLock(const HHTimerOneInterruptLock&);
};

#endif