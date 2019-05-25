#ifndef HEDGEHOG_CONTROLLER_H_
#define HEDGEHOG_CONTROLLER_H_

#define __ASSERT_USE_STDERR

#include <assert.h>
#include <Arduino.h>
#include <TimerOne.h>

#include "HedgehogEnums.h"
#include "HedgehogData.h"
#include "HedgehogSerial.h"
#include "HedgehogConfig.h"

#define __STR_EXPAND(tok) #tok
#define __STR(tok) __STR_EXPAND(tok)
#define __CAT(a, b) a ## b
#define __PORT(x) __CAT(PORT, x)
#define __DDR(x) __CAT(DDR, x)
#define __PIN(x) __CAT(PIN, x) 
#define BYTE_LOOP(x, n) for (uint8_t x = 0; x < n; ++x)

typedef uint8_t pin_t;
typedef uint64_t millis_t;

typedef struct State
{
    uint8_t momentaryButtonHistory[__BTN_COUNT] = {0};

    void update_button_history_inv(uint8_t bank, uint8_t cap) volatile
    {
        assert(bank < __BTN_BANKS);
        cap = ~cap;
        BYTE_LOOP(x, 8)
        {
            uint8_t idx = x + (bank * 8);
            momentaryButtonHistory[idx] = momentaryButtonHistory[idx] << 1;
            momentaryButtonHistory[idx] |= bitRead(cap, x);
        }
    }

    #define MASK 0b11000111
    #define mask_bits(x) (x & MASK)

    uint8_t is_button_pressed(uint8_t id) volatile
    {
        assert(id < __BTN_COUNT);

        uint8_t pressed = 0;
        if (mask_bits(momentaryButtonHistory[id]) == 0b0000001){ 
            pressed = 1;
            momentaryButtonHistory[id] = 0b11111111;
        }
        return pressed;
    }

    uint8_t is_button_released(uint8_t id) volatile
    {
        assert(id < __BTN_COUNT);

        uint8_t released = 0;   
        if (mask_bits(momentaryButtonHistory[id]) == 0b11000000){ 
            released = 1;
            momentaryButtonHistory[id] = 0b00000000;
        }
        return released;
    }

} state_t;

inline bool process_update(volatile bool& flag)
{
    if (flag)
    {
        flag = !flag;
        return true;
    }

    return false;
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
        interrupts();
    }

    ~HHTimerOneInterruptLock()
    {
        Timer1.attachInterrupt(_isr);
    }

private:
    void (*const _isr)();
    HHTimerOneInterruptLock(const HHTimerOneInterruptLock&);
};

class HHVolatileReadInterruptLock
{
public:
    HHVolatileReadInterruptLock()
    {
        noInterrupts();
    }

    ~HHVolatileReadInterruptLock()
    {
        interrupts();
    }
};

#endif