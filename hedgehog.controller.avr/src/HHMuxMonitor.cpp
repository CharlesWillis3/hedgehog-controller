#include "HHMuxMonitor.h"

HHMuxMonitor::HHMuxMonitor(volatile state_t& state)
    : _state(state)
{
}

void HHMuxMonitor::initialize()
{
#if (__BTN_PORTS == 1) && (__BTN_BANKS == 4)
    // Set the port as input with pullups
    __DDR(BTNM_IN_PORT) = 0x00;
    __PORT(BTNM_IN_PORT) = 0xFF;

    // Set the selection pins as output
    bitSet(__DDR(BTNM_SEL_PORT), BTNM_SEL_BIT0);
    bitSet(__DDR(BTNM_SEL_PORT), BTNM_SEL_BIT1);
#endif
}

void HHMuxMonitor::update(millis_t curr_millis)
{
    if (curr_millis - _last_update >= _update_interval)
    {
#if (__BTN_PORTS == 1) && (__BTN_BANKS == 4)
        uint8_t sel_bank = 0;
        BYTE_LOOP(i, 2)
        {
            BYTE_LOOP(j, 2)
            {
                bitWrite(__PORT(BTNM_SEL_PORT), BTNM_SEL_BIT0, i);
                bitWrite(__PORT(BTNM_SEL_PORT), BTNM_SEL_BIT1, j);

                uint8_t cap = __PIN(BTNM_IN_PORT);
                _state.update_button_history_inv(sel_bank++, cap);
            }
        }
#endif
        _last_update = curr_millis;
    }
}