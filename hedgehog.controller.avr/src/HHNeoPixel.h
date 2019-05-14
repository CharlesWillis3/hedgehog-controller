#ifndef HH_NEOPIXEL_H
#define HH_NEOPIXEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <assert.h>
#include "HedgehogController.h"
#include "HedgehogConfig.h"

class HHNeoPixel
{
public:
    HHNeoPixel();
    void initialize();
    void changeColor(uint8_t pixel_id, uint8_t red, uint8_t green, uint8_t blue);
    void refresh();

private:
    bool _isDirty;
    Adafruit_NeoPixel _pixel;
    uint32_t _currentColor[NEO_PIXEL_COUNT];
};

#endif