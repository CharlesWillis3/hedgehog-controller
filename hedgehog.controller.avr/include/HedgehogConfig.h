#ifndef HH_CONFIG_H
#define HH_CONFIG_H

/** BUTTON MODES **/

#define BTN_COUNT 0

#define BTNM_NONE 0
#define BTNM_MUX_PORT_1_BANK_4 1

#define USE_BTN_MODE BTNM_MUX_PORT_1_BANK_4 
#if USE_BTN_MODE == BTNM_MUX_PORT_1_BANK_4
    #define __BTN_IN_PORT B
    #define __BTN_SEL_PORT D
    #define __BTN_SEL_BIT_0 0 
    #define __BTN_SEL_BIT_1 1

    #define __USE_BTNM_MUX
    #define __BTN_UPDATE_INT_MS millis_t(5)
    #define __BTN_PORTS 1
    #define __BTN_BANKS 4

    #undef BTN_COUNT
    #define BTN_COUNT 32 // 8 bits in the port x 4 banks = 32 buttons 
#endif

/** ANALOG MODES **/

#define ANALOG_COUNT 1

/** LED MODES **/

#define LED_COUNT 0

#define LEDM_NONE 0
#define LEDM_DEMUX_4051 1

#define USE_LED_MODE LEDM_DEMUX_4051
#if USE_LED_MODE == LEDM_DEMUX_4051
    #define __LED_SEL_PORT F
    #define __LED_SEL_BIT_0 5
    #define __LED_SEL_BIT_1 6
    #define __LED_SEL_BIT_2 7
    #define __LED_PWM_PIN pin_t(5)

    #define __USE_LEDM_DEMUX
    #define __LED_UPDATE_INT_MS millis_t(5)

    #undef LED_COUNT
    #define LED_COUNT 8
#endif

#define NEOPXM_NONE 0
#define NEOPXM_STICK_RGB 1

#define USE_NEOPIXEL_MODE NEOPXM_STICK_RGB
#if USE_NEOPIXEL_MODE == NEOPXM_STICK_RGB
    #define __NEOPX_PIXEL_COUNT 8
    #define __NEOPX_PIN pin_t(7)
    #define __NEOPX_TYPE NEO_RGB
    #define __NEOPX_BRT_DIV 30
    #define __USE_NEOPIXEL
#endif

#endif // HH_CONFIG_H