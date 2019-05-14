#ifndef HEDGEHOG_CONTROLLER_H_
#define HEDGEHOG_CONTROLLER_H_

#define __ASSERT_USE_STDERR

#include <assert.h>
#include <Arduino.h>
#include <TimerOne.h>

#include "HedgehogEnums.h"
#include "HedgehogData.h"
#include "HedgehogSerial.h"

#define BYTE_LOOP(x, n) for (uint8_t x = 0; x < n; ++x)

enum InBtnState : uint8_t
{
    IB_UP = 0x00,
    IB_DOWN = 0x01,
    IB_RISING = 0x10,
    IB_FALLING = 0x20
};

inline bool processUpdate(volatile bool& flag);

typedef struct State
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool has_update_led;

    uint32_t pollCount = 0;

    uint8_t momentaryButtonHistory[16] = {0};

    State copy_reset() volatile
    {
        State n;
        n.red = this->red;
        n.green = this->green;
        n.blue = this->blue;
        n.has_update_led = this->has_update_led;
        n.pollCount = this->pollCount;
        memcpy(n.momentaryButtonHistory, const_cast<uint8_t*>(this->momentaryButtonHistory), 16);

        this->has_update_led = false;
        return n;
    }

    void update_button(uint8_t id, uint8_t next) volatile
    {
        assert(id < 16);
        volatile uint8_t *button_history = &momentaryButtonHistory[id];
        *button_history = *button_history << 1;
        *button_history |= next;
    }

    void update_buttons_inv(uint8_t low, uint8_t high) volatile
    {
        // volatile uint8_t *button_history;
        BYTE_LOOP(i, 8)
        {
            // button_history = &momentaryButtonHistory[i];
            // *button_history = *button_history << 1;
            // *button_history |= bitRead(low, i);

            momentaryButtonHistory[i] = momentaryButtonHistory[i] << 1;
            momentaryButtonHistory[i] |= bitRead(low, i) == 0 ? 1 : 0;
        }

        BYTE_LOOP(j, 8)
        {
            // button_history = &momentaryButtonHistory[j + 8];
            // *button_history = *button_history << 1;
            // *button_history |= bitRead(high, j);

            momentaryButtonHistory[j + 8] = momentaryButtonHistory[j + 8] << 1;
            momentaryButtonHistory[j + 8] |= bitRead(high, j) == 0 ? 1 : 0;
        }
    }

    #define MASK 0b11000111

    uint8_t is_button_pressed(uint8_t id) volatile
    {
        assert(id < 16);
        uint8_t pressed = 0;
        if ((momentaryButtonHistory[id] & MASK) == 0b00000111){ 
            pressed = 1;
            momentaryButtonHistory[id] = 0b11111111;
        }
        return pressed;
    }

    void update_leds(uint8_t red, uint8_t green, uint8_t blue) volatile
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->has_update_led = true;
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