#ifndef HH_CONFIG_H
#define HH_CONFIG_H

//#define DEBUG //enable/disable serial debug output

#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
    #define DEBUG_PRINTBIN(x) Serial.print(x, BIN)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTDEC(x)
    #define DEBUG_PRINTBIN(x)
    #define DEBUG_PRINTLN(x)
#endif

/** LED MODES **/

#define USE_NEOPIXEL
#ifdef USE_NEOPIXEL
    #define NEO_PIXEL_COUNT 8
    #define NEO_PIXEL_PIN 7
    #define NEO_PIXEL_TYPE NEO_RGB
#endif

/** BUTTON MODES **/

#define __BTN_COUNT 0

#define BTNM_MUX_PORT_1_BANK_4 3

#define USE_BTN_MODE BTNM_MUX_PORT_1_BANK_4 
#if USE_BTN_MODE == BTNM_MUX_PORT_1_BANK_4
    #define BTNM_IN_PORT B
    #define BTNM_SEL_PORT C
    #define BTNM_SEL_BIT0 6 
    #define BTNM_SEL_BIT1 7

    #define __USE_BTNM_MUX
    #define __BTN_PORTS 1
    #define __BTN_BANKS 4

    #undef __BTN_COUNT
    #define __BTN_COUNT 32 // 8 bits in the port x 4 banks = 32 buttons 
#endif

#endif // HH_CONFIG_H