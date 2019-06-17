#include <Arduino.h>
#include <EEPROM.h>
#include "HedgehogController.h"
#include "HedgehogConfig.h"
#include "HHProgrammer.h"
#include "HHDebugger.h"

state_t volatile globalState;

#if defined(__USE_BTNM_MUX)
    #include "HHMuxMonitor.h"
    HHMuxMonitor muxmon{globalState};
#endif

#if defined(__USE_LEDM_DEMUX)
    #include "HHLedDemux.h"
    HHLedDemux leddemux;
#endif

#define NP_LAST_UPDATE(x)
#define NP_CHANGE_COLOR(x, r, b, g)
#if defined(__USE_NEOPIXEL)
    #include <Adafruit_NeoPixel.h>
    #include "HHNeoPixel.h"
    HHNeoPixel neopixel;
    millis_t last_neopixel_update;
    #undef NP_CHANGE_COLOR
    #define NP_CHANGE_COLOR(x, r, b, g) neopixel.change_color(x, r, b, g)
    #undef NP_LAST_UPDATE
    #define NP_LAST_UPDATE(x) last_neopixel_update = x
#endif

HHProgrammer programmer{BTN_COUNT, 0, __NEOPX_PIXEL_COUNT};
// HHDebugger debugger{};

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

#if defined(DEBUG)
    while (Serial.read() != 9)
    {
    }
#endif

#if defined(__USE_BTNM_MUX)
    muxmon.initialize();
#endif

#if defined(__USE_NEOPIXEL)
    neopixel.initialize();
    neopixel.change_color(0, 0, 0, 255);
    neopixel.refresh();
#endif

#if defined(__USE_LEDM_DEMUX)
     leddemux.initialize();
     leddemux.refresh();
#endif

pinMode(A3, INPUT);
}

int sel = 0;
millis_t last_led_update;

void loop()
{
    if (Serial.available() && HHS_REQUIRE(HHSerial::HELLO))
    {
        NP_CHANGE_COLOR(0, 0, 0, 255);
        HHSM_HELLO;
        HHSM_READY;

        HHSerial s;
        if (HHS_WAIT_LONG(s))
        {
            switch (s)
            {
                case HHSerial::MODE_PROGRAMMING:
                    NP_CHANGE_COLOR(0, 0, 255, 0);
                    programmer.startProgramming();
                    break;
                // case HHSerial::MODE_DEBUG:
                //     neopixel.setPixelColor(0, 255, 0, 255);
                //     neopixel.show();
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

    // if (millis() - last_led_update >= 250)
    // {
    //     if (++sel >= 8) sel = 0;
    //     analogWrite(5, (sel % 3) == 0 ? 0 : analogRead(A3) / 4);
    //     bitWrite(__PORT(F), 5, bitRead(sel, 0));
    //     bitWrite(__PORT(F), 6, bitRead(sel, 1));
    //     bitWrite(__PORT(F), 7, bitRead(sel, 2));
    //     last_led_update = millis();
    // }


#if defined(__USE_BTNM_MUX)

    muxmon.update(millis());

    BYTE_LOOP(x, BTN_COUNT)
    {
        if (globalState.is_button_pressed(x))
        {
            DEBUG_PRINT(F("BUTTON PRESSED: "));
            DEBUG_PRINTLN(x);
            if (x < 16)
            {
                NP_CHANGE_COLOR(x % 8, 255, 0, 255);
                NP_LAST_UPDATE(millis());

            }
            else 
            {
                NP_CHANGE_COLOR(x % 8, 0, 255, 255);
                NP_LAST_UPDATE(millis());
            }
        }

        if (globalState.is_button_released(x))
        {
            DEBUG_PRINT(F("BUTTON RELEASED: "));
            DEBUG_PRINTLN(x);
            if (x < 16)
            {
                NP_CHANGE_COLOR(x % 8, 0, 255, 0);
                NP_LAST_UPDATE(millis());
            }
            else 
            {
                NP_CHANGE_COLOR(x % 8, 255, 0, 0);
                NP_LAST_UPDATE(millis());
            }
        }
    }
#endif

#if defined(__USE_NEOPIXEL)
    if (millis() - last_neopixel_update >= 2 * 1000)
    {
        BYTE_LOOP(x, 8)
        {
            neopixel.change_color(x, 255, 255, 255);
        }
    }

    neopixel.refresh();
#endif

#if defined(__USE_LEDM_DEMUX)
    BYTE_LOOP(x, 8)
    {
        int val = analogRead(A3) / 4;
        DEBUG_PRINT(x);
        DEBUG_PRINT(": ");
        DEBUG_PRINTLN(val);
        leddemux.change_duty_cycle(x, val);
    }

    leddemux.refresh();
#endif
}