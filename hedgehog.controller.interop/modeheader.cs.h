#if !__LINE__
namespace hedgehog.controller.core.interop
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential, Pack=1)]
    public struct ModeHeader {
        public byte f_flags;
        public byte joystick_id;
        public byte joystick_btn;
        public byte neopixelid_red;
        public byte green_blue;
    }
}
#else
typedef struct __attribute__((packed, aligned(1))) ModeHeader {
    byte f_flags;
    byte joystick_id;
    byte joystick_btn;
    byte neopixelid_red;
    byte green_blue;
} modeHeader_t;
#endif