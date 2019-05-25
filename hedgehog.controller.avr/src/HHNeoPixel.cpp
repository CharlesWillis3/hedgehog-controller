#include "HHNeoPixel.h"

HHNeoPixel::HHNeoPixel()
    : _pixel{Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_PIXEL_TYPE)}
{
}

void HHNeoPixel::initialize()
{
    _pixel.begin();
    _pixel.show();
}

void HHNeoPixel::changeColor(uint8_t pixel_id, uint8_t red, uint8_t green, uint8_t blue)
{
    assert(pixel_id < NEO_PIXEL_COUNT);
    uint32_t new_color = _pixel.Color(red / 4, green / 4, blue / 4);
    if (_currentColor[pixel_id] == new_color)
    {
        DEBUG_PRINTLN("Color already set");
        return;
    }

    DEBUG_PRINTLN("Changing color");
    _currentColor[pixel_id] = new_color;
    _pixel.setPixelColor(pixel_id, new_color);
    _isDirty = true;
}

void HHNeoPixel::refresh()
{
    if (_isDirty)
    {
        BYTE_LOOP(x, NEO_PIXEL_COUNT)
        {
            _pixel.setPixelColor(x, _currentColor[x]);
        }

        _isDirty = false;
        _pixel.show();
    }
}
