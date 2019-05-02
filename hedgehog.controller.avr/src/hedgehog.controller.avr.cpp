#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "HedgehogController.h"
#include "HHProgrammer.h"
#include "HHDebugger.h"

#define DIGITALS  (uint8_t)25
#define ANALOGS   (uint8_t)0
#define NEOPIXELS (uint8_t)2

#define LED_PIN 1
#define LED_COUNT 1

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_RGB);
HHProgrammer programmer(DIGITALS, ANALOGS, NEOPIXELS);
HHDebugger debugger;

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

    led.begin();
    led.show();
}

void loop()
{
    if (Serial.available())
    {
        uint8_t rcvd;
        size_t n_bytes = Serial.readBytes(&rcvd, 1);

        if (n_bytes && rcvd == HHSerial::HELLO)
        {
            led.setPixelColor(0, 0, 0, 255);
            led.show();

            Serial.write(HHSerial::HELLO);

            n_bytes = Serial.readBytes(&rcvd, 1);

            if (n_bytes)
            {
                switch (rcvd)
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
                        break;
                    default:
                        Serial.write(HHSerial::ERROR);
                }
            } 
        }
        else
        {
            Serial.write(HHSerial::FAULT);
        }

        led.setPixelColor(0, 0, 255, 255);
        led.show();
        Serial.write(HHSerial::MODE_RUN);
    }
}