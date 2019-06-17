#include "HHMuxMonitor.h"

HHMuxMonitor::HHMuxMonitor(volatile state_t& state)
    : _state(state)
{
}

void HHMuxMonitor::initialize()
{
#if (__BTN_PORTS == 1)
    // Set the port as input with pullups
    __DDR(__BTN_IN_PORT) = 0x00;
    __PORT(__BTN_IN_PORT) = 0xFF;
#endif

#if (__BTN_BANKS == 4)
    // Set the selection pins as output
    bitSet(__DDR(__BTN_SEL_PORT), __BTN_SEL_BIT_0);
    bitSet(__DDR(__BTN_SEL_PORT), __BTN_SEL_BIT_1);
#endif
}

void HHMuxMonitor::update(millis_t curr_millis)
{
    if (curr_millis - _last_update >= __BTN_UPDATE_INT_MS)
    {
#if (__BTN_PORTS == 1) && (__BTN_BANKS == 4)
        uint8_t sel_bank = 0;
        BYTE_LOOP(i, 2)
        {
            BYTE_LOOP(j, 2)
            {
                bitWrite(__PORT(__BTN_SEL_PORT), __BTN_SEL_BIT_0, i);
                bitWrite(__PORT(__BTN_SEL_PORT), __BTN_SEL_BIT_1, j);

                _state.update_button_history_inv(sel_bank++, __PIN(__BTN_IN_PORT));
            }
        }
#endif
        _last_update = curr_millis;
    }
}