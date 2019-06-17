#include "HHLedDemux.h"

HHLedDemux::HHLedDemux()
{
}

void HHLedDemux::initialize()
{
    // bitSet(__DDR(__LED_SEL_PORT), __LED_PWM_PIN);
    __DDR(__LED_SEL_PORT) |= (1 << __LED_PWM_PIN);
    __DDR(__LED_SEL_PORT) &= ~(0B111 << __LED_SEL_BIT_0);
    // bitClear(__DDR(__LED_SEL_PORT), __LED_SEL_BIT_0);
    // bitClear(__DDR(__LED_SEL_PORT), __LED_SEL_BIT_1);
    // bitClear(__DDR(__LED_SEL_PORT), __LED_SEL_BIT_2);
}

//        (maxo-mino)(x - mini)
// f(x) = --------------  + mino
//           maxi - mini
void HHLedDemux::change_duty_cycle(uint8_t led_id, int duty_cycle)
{
    assert(led_id < LED_COUNT);
    _currentDutyCycle[led_id] = duty_cycle;
}

void HHLedDemux::refresh()
{
    if (_id >= LED_COUNT)
        _id = 0;

#define _SELB(x) __CAT(__LED_SEL_BIT_, x)

    for (int x = 0; x < LED_COUNT; x++)
    {
        __PORT(__LED_SEL_PORT) = (__PORT(__LED_SEL_PORT) & ~( 0B111 << _SELB(0))) | (x << _SELB(0));
        // analogWrite(__LED_PWM_PIN, _currentDutyCycle[x]);
        digitalWrite(__LED_PWM_PIN, 1);
    }
}