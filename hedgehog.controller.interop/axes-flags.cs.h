#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    using System;
    
    [Flags]
    public 
#endif

// shared enum for both C, C++ and C#
    enum AxesFlags : byte 
    { 
        none     = 0x00,
        x        = 0x01,
        y        = 0x02,
        z        = 0x04,
        xyz      = 0x07,
        rX       = 0x08,
        rY       = 0x10,
        rZ       = 0x20,
        rXYZ     = 0x38,
        throttle = 0x40,
        rudder   = 0x80,
        all      = 0xFF
    };

#if !__LINE__
}
#endif