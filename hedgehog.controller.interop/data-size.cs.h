#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    public 
#endif

// shared enum for both C, C++ and C#
    enum HHDataSize : byte
    { 
        S_DEVICE     = 5,
        S_JOYSTICK   = 4,
        S_MODEHEADER = 5,
        S_ACTION     = 3
    };

#if !__LINE__
}
#endif