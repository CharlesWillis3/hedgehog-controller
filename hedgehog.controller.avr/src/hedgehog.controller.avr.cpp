#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "HedgehogController.h"
#include "HHMCP23017.h"
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
HHMCP23017 momon{0x20, INTA_PIN, INTB_PIN, state};
HHProgrammer programmer{DIGITALS, ANALOGS, NEOPIXELS};
// HHDebugger debugger{};

void resetLedColor();

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

    Wire.begin();

    led.begin();
    led.show();

    momon.initialize();

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

    if (state.hasUpdate_portA)
    {
        Serial.println("--PORT A--");
        BYTE_LOOP(x, 8)
        {
            Serial.print(x);
            Serial.print(": ");
            Serial.println(state.portA[x], HEX);
        }

        if (state.portA[0] == IB_FALLING)
        {
            Serial.println("A0 FALLING");
            state.updateLeds(0, 255, 0);
        }
        else if (state.portA[0] == IB_RISING)
        {
            Serial.println("A0 RISING");
            state.updateLeds(0, 255, 255);
        }

        state.hasUpdate_portA = false;
    }
    
    if (state.hasUpdate_portB)
    {
        Serial.println("++PORT B++");
        BYTE_LOOP(x, 8)
        {
            Serial.print(x);
            Serial.print(": ");
            Serial.println(state.portB[x], HEX);
        }

        if (state.portB[0] == IB_FALLING)
        {
            Serial.println("B0 FALLING");
            state.updateLeds(255, 255, 0);
        }
        else if (state.portB[0] == IB_RISING)
        {
            Serial.println("B1 RISING");
            state.updateLeds(0, 0, 255);
        }

        state.hasUpdate_portB = false;
    }

    if (state.updateLed)
    {
        led.setPixelColor(0, state.red, state.green, state.blue);
        led.show();
        state.updateLed = false;
    }
}