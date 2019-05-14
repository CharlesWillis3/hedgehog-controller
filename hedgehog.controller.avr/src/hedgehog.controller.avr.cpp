#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <TimerOne.h>
#include "HedgehogController.h"
#include "HedgehogConfig.h"
#include "HHMCP23017.h"
#include "HHProgrammer.h"
#include "HHDebugger.h"
#include "HHNeoPixel.h"

#define DIGITALS  (uint8_t)25
#define ANALOGS   (uint8_t)0

state_t volatile globalState;

HHMCP23017 momon{0x20, globalState};
HHProgrammer programmer{DIGITALS, ANALOGS, NEO_PIXEL_COUNT};
// HHDebugger debugger{};

HHNeoPixel led;

void pollButtons();

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    Serial.setTimeout(2000);
    #endif

    // OCR0A = 0xAF;
    // TIMSK0 |= _BV(OCIE0A);

    // Timer1.attachInterrupt(pollButtons);
    // Timer1.initialize(1000UL * 10);

    Wire.begin();

    momon.initialize();

    led.initialize();
    led.changeColor(0, 0, 0, 255);
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

    // Serial.print("Poll Count: ");
    // Serial.println(state.pollCount, DEC);

    momon.update(millis());

    BYTE_LOOP(x, 16)
    {
        if (globalState.is_button_pressed(x))
        {
            DEBUG_PRINT("BUTTON PRESSED: ");
            DEBUG_PRINTLN(x);
            if (x < 8)
            {
                globalState.update_leds(255, 0, 255);

            }
            else 
            {
                globalState.update_leds(0, 255, 255);
            }
        }
    }

    if (process_update(globalState.has_update_led))
    {
        uint8_t red, green, blue;
        // {
        //     HHVolatileReadInterruptLock lock;
        //     red = globalState.red;
        //     green = globalState.green;
        //     blue = globalState.blue;
        // }

        red = globalState.red;
        green = globalState.green;
        blue = globalState.blue;

        led.changeColor(0, red, green, blue);
        DEBUG_PRINTDEC(red);
        DEBUG_PRINTDEC(green);
        DEBUG_PRINTDEC(blue);
    }

    led.refresh();
}

void pollButtons()
{
    HHTimerOneInterruptLock lock{pollButtons};
    momon.poll();
}

// SIGNAL(TIMER0_COMPA_vect)
// {
//     //momon.poll(curr_millis);
//     globalState.pollCount += 1;
// }