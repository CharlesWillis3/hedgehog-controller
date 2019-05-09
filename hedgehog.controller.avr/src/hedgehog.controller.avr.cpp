#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <TimerOne.h>
#include "HedgehogController.h"
#include "HHMomentaryMonitor.h"
#include "HHProgrammer.h"
#include "HHDebugger.h"

#define DIGITALS  (uint8_t)25
#define ANALOGS   (uint8_t)0
#define NEOPIXELS (uint8_t)2

#define LED_PIN 4
#define LED_COUNT 1
#define INTA_PIN 0
#define INTB_PIN 1

state_t volatile state;

Adafruit_NeoPixel led{LED_COUNT, LED_PIN, NEO_RGB};
HHMomentaryMonitor momon{0x20, INTA_PIN, INTB_PIN, state};
HHProgrammer programmer{DIGITALS, ANALOGS, NEOPIXELS};
HHDebugger debugger{momon};

void stopTimerResetLed();
void resetLedColor();

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

    Wire.begin();

    led.begin();
    led.show();

    momon.initialize();

    Timer1.initialize(1000UL * 1000UL * 2);
    Timer1.attachInterrupt(resetLedColor);

    led.setPixelColor(0, 0, 0, 255);
    led.show();
}

void loop()
{
    if (Serial.available() && HHS_REQUIRE(HHSerial::HELLO))
    {
        led.setPixelColor(0, 0, 0, 255);
        led.show();

        HHSM_HELLO;
        HHSM_READY;

        HHSerial s;
        if (HHS_WAIT_LONG(s))
        {
            switch (s)
            {
                case HHSerial::MODE_PROGRAMMING:
                    led.setPixelColor(0, 0, 255, 0);
                    led.show();
                    programmer.startProgramming();
                    break;
                case HHSerial::MODE_DEBUG:
                    led.setPixelColor(0, 255, 0, 255);
                    led.show();
                    debugger.startDebugging();
                    break;
                case HHSerial::MODE_RUN:
                    Serial.write(HHSerial::MODE_RUN);
                    break;
                default:
                    HHSM_ERROR;
            }
        }

        Serial.write(HHSerial::MODE_RUN);
    }

    if (state.updateLed)
    {
        Timer1.restart();
        led.setPixelColor(0, state.red, state.green, state.blue);
        led.show();
        state.updateLed = false;
    }

    if (state.updateIntCap)
    {
        Serial.println();
        Serial.print("INTCAPA: ");
        Serial.println(state.intCapA, BIN);
        Serial.print("INTCAPB: ");
        Serial.println(state.intCapB, BIN);
        state.updateIntCap = false;
    }
}

void resetLedColor()
{
    state.updateLeds(0, 0, 255);
}