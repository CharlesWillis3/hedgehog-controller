#if !__LINE__
namespace hedgehog.controller.core.interop
{
    using System;
    using System.Runtime.InteropServices;
    
    [StructLayout(LayoutKind.Sequential, Pack=1)]
    public struct Joystick {
        public byte n_buttons;
        public byte n_hats;
        public AxesFlags f_axes;
        public byte d;
    } 
}
#else
typedef struct __attribute__((packed, aligned(1))) Joystick {
    byte n_buttons;
    byte n_hats;
    byte f_axes;
    byte d;
} joystick_t;
#endif