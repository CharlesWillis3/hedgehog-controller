#ifndef HEDGEHOGSERIAL_H_
#define HEDGEHOGSERIAL_H_

#include <Arduino.h>
#include "HedgehogEnums.h"

#define HHS_RCVD(x) hh_serial_rcvd(x)
#define HHS_REQUIRE(x) hh_serial_require(x)
#define HHS_RCVD_GET(x,y) hh_serial_rcvd(x, y)
#define HHS_GET(x) hh_serial_get(x)
#define HHS_BLOCK hh_serial_block()
#define HHS_WAIT_LONG(x) hh_serial_wait(30 * 1000, x)

#define HHS_SEND_MSG(x) Serial.write(x)
#define HHSM_HELLO HHS_SEND_MSG(HHSerial::HELLO)
#define HHSM_READY HHS_SEND_MSG(HHSerial::READY)
#define HHSM_TIMEOUT HHS_SEND_MSG(HHSerial::TIMEOUT)
#define HHSM_BAD_PROTOCOL HHS_SEND_MSG(HHSerial::BAD_PROTOCOL)
#define HHSM_ERROR HHS_SEND_MSG(HHSerial::ERROR)
#define HHSM_FAULT HHS_SEND_MSG(HHSerial::FAULT)

inline bool hh_serial_rcvd(HHSerial expected)
{
    uint8_t rcvd;
    size_t n_bytes = Serial.readBytes(&rcvd, 1);
    return (n_bytes && (rcvd == expected));
}

inline bool hh_serial_rcvd(HHSerial expected, HHSerial& actual)
{
    uint8_t rcvd;
    size_t n_bytes = Serial.readBytes(&rcvd, 1);
    HHSerial msg = HHSerial(rcvd);
    actual = msg;
    return (n_bytes && (msg == expected));
}

inline bool hh_serial_require(HHSerial expected)
{
    uint8_t rcvd;
    size_t n_bytes = Serial.readBytes(&rcvd, 1);
    if (n_bytes && (rcvd == expected))
    {
        return true;
    }

    HHSM_BAD_PROTOCOL;
    return false;
}

inline bool hh_serial_get(HHSerial& result)
{
    uint8_t rcvd;
    size_t n_bytes = Serial.readBytes(&rcvd, 1);
    result = HHSerial(rcvd);
    return n_bytes == 1;
}

inline HHSerial hh_serial_block()
{
    while (!Serial.available())
    {}

    HHSerial result;
    if (hh_serial_get(result))
    {
        return result;
    }
    else
    {
        return HHSerial::UNKNOWN;
    }
}

inline bool hh_serial_wait(unsigned long timeout, HHSerial& result)
{
    unsigned long t = Serial.getTimeout();
    Serial.setTimeout(timeout);
    bool ret = hh_serial_get(result);
    Serial.setTimeout(t);

    if (!ret)
    {
        HHSM_TIMEOUT;
    }

    return ret;
}

#endif