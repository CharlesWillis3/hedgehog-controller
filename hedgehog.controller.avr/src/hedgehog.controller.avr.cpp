#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "HedgehogController.h"
#include "HedgehogConfig.h"
#include "HHProgrammer.h"
#include "HHDebugger.h"
#include "HHNeoPixel.h"

state_t volatile globalState;

#if defined(__USE_BTNM_MUX)
    #include "HHMuxMonitor.h"
    HHMuxMonitor muxmon{globalState};
#endif

HHProgrammer programmer{__BTN_COUNT, 0, NEO_PIXEL_COUNT};
// HHDebugger debugger{};

HHNeoPixel led;

millis_t last_led_update;

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

    led.initialize();
    led.changeColor(0, 0, 0, 255);
    led.refresh();

#if defined(DEBUG)
    while (Serial.read() != 9)
    {
    }
#endif

#if defined(__USE_BTNM_MUX)
    muxmon.initialize();
#endif

}

void loop()
{
    if (Serial.available() && HHS_REQUIRE(HHSerial::HELLO))
    {
        led.changeColor(0, 0, 0, 255);

        HHSM_HELLO;
        HHSM_READY;

        HHSerial s;
        if (HHS_WAIT_LONG(s))
        {
            switch (s)
            {
                case HHSerial::MODE_PROGRAMMING:
                    led.changeColor(0, 0, 255, 0);

                    programmer.startProgramming();
                    break;
                // case HHSerial::MODE_DEBUG:
                //     led.setPixelColor(0, 255, 0, 255);
                //     led.show();
                //     debugger.startDebugging();
                //     break;
                case HHSerial::MODE_RUN:
                    Serial.write(HHSerial::MODE_RUN);
                    break;
                default:
                    HHSM_ERROR;
            }
        }

        Serial.write(HHSerial::MODE_RUN);
    }

#if defined(__USE_BTNM_MUX)

    muxmon.update(millis());

    BYTE_LOOP(x, __BTN_COUNT)
    {
        if (globalState.is_button_pressed(x))
        {
            DEBUG_PRINT("BUTTON PRESSED: ");
            DEBUG_PRINTLN(x);
            if (x < 16)
            {
                led.changeColor(x % 8, 255, 0, 255);
                last_led_update = millis();

            }
            else 
            {
                led.changeColor(x % 8, 0, 255, 255);
                last_led_update = millis();
            }
        }

        if (globalState.is_button_released(x))
        {
            DEBUG_PRINT("BUTTON RELEASED: ");
            DEBUG_PRINTLN(x);
            if (x < 16)
            {
                led.changeColor(x % 8, 0, 255, 0);
                last_led_update = millis();

            }
            else 
            {
                led.changeColor(x % 8, 255, 0, 0);
                last_led_update = millis();
            }
        }
    }
#endif

    if (millis() - last_led_update >= 2 * 1000)
    {
        BYTE_LOOP(x, 8)
        {
            led.changeColor(x, 0, 0, 255);
        }
    }

    led.refresh();
}