#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    public 
#endif

// shared enum for both C, C++ and C#
    enum HHSerial : byte
    { 
        UNKNOWN           = 0x00,
        HELLO             = 0x01,
        RCVD              = 0x02,

        MODE_PROGRAMMING  = 0x10,
        DEVICE_START      = 0x11,
        JOYSTICK_START    = 0x12,
        REQUEST_HARDWARE  = 0x13,

        MODE_DEBUG        = 0x50,
        DEVICE_READ       = 0x51,
        JOYSTICKS_READ    = 0x52,
        BUTTONS_ID        = 0x53,
        DEBUG_END         = 0x69,

        MODE_RUN          = 0x80,

        READY             = 0x90,
        BAD_PROTOCOL      = 0x96,
        TIMEOUT           = 0x97,
        ERROR             = 0x98,
        FAULT             = 0x99
    };

#if !__LINE__
}
#endif