#include "HHNeoPixel.h"

HHNeoPixel::HHNeoPixel()
    : _pixel{Adafruit_NeoPixel(__NEOPX_PIXEL_COUNT, __NEOPX_PIN, __NEOPX_TYPE)}
{
}

void HHNeoPixel::initialize()
{
    _pixel.begin();
    _pixel.setBrightness(255 / __NEOPX_BRT_DIV);
    _pixel.show();
}

void HHNeoPixel::change_color(uint8_t pixel_id, uint8_t red, uint8_t green, uint8_t blue)
{
    assert(pixel_id < __NEOPX_PIXEL_COUNT);
    uint32_t new_color = _pixel.Color(red, green, blue);
    if (_currentColor[pixel_id] == new_color)
    {
        return;
    }

    _currentColor[pixel_id] = new_color;
    _pixel.setPixelColor(pixel_id, new_color);
    _isDirty = true;
}

void HHNeoPixel::refresh()
{
    if (_isDirty)
    {
        BYTE_LOOP(x, __NEOPX_PIXEL_COUNT)
        {
            _pixel.setPixelColor(x, _currentColor[x]);
        }

        _isDirty = false;
        _pixel.show();
    }
}
