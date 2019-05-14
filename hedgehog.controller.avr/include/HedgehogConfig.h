#ifndef HH_CONFIG_H
#define HH_CONFIG_H

//#define DEBUG //enable/disable serial debug output

#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTDEC(x)
    #define DEBUG_PRINTLN(x)
#endif

#define USE_NEOPIXEL
#ifdef USE_NEOPIXEL
    #define NEO_PIXEL_COUNT 1
    #define NEO_PIXEL_PIN 4
    #define NEO_PIXEL_TYPE NEO_RGB
#endif

#endif