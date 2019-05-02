#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    using System;
    
    [Flags]
    public 
#endif

// shared enum for both C, C++ and C#
    enum DeviceFlags : byte 
    { 
        unknown  = 0x00,
        keyboard = 0x01,
        mouse    = 0x02,
        joystick = 0x04
    };

#if !__LINE__
}
#endif