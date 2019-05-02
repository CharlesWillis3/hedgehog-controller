#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    public 
#endif

// shared enum for both C, C++ and C#
    enum HHOffset
    { 
        DEVICE    = 0x000,
        JOYSTICK0 = 0x004,
        JOYSTICK1 = 0x008,
        JOYSTICK2 = 0x00C,
        JOYSTICK3 = 0x010,
        MODE0     = 0x014,
        LAST      = 0x400
    };

#if !__LINE__
}
#endif