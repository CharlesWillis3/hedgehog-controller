#ifndef HH_DEBUG_H_
#define HH_DEBUG_H

#define DEBUG //enable/disable serial debug output

#ifdef DEBUG
    #define __DEBUG_PRINTLN_FMT(x, fmt) Serial.println(x, fmt)
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
    #define DEBUG_PRINTBIN(x) Serial.print(x, BIN)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTLN_DEC(x) __DEBUG_PRINTLN_FMT(x, DEC)
    #define DEBUG_PRINTLN_BIN(x) __DEBUG_PRINTLN_FMT(x, BIN)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTDEC(x)
    #define DEBUG_PRINTBIN(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTLN_DEC(x)
    #define DEBUG_PRINTLN_BIN(x)
#endif

#endif