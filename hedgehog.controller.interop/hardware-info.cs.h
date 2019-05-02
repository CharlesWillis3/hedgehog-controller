#if !__LINE__    
namespace hedgehog.controller.core.interop
{
    public 
#endif

// shared enum for both C, C++ and C#
    enum HHHardwareInfo : byte 
    { 
        NUM_DIGITALS   = 0x00,
        NUM_ANALOGS    = 0x01,
        NUM_NEOPIXELS  = 0x02
    };

#if !__LINE__
}
#endif