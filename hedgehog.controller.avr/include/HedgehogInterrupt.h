#ifndef HH_INTERRUPTLOCK_H_
#define HH_INTERRUPTLOCK_H_

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