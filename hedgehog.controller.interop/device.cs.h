#if !__LINE__
namespace hedgehog.controller.core.interop
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential, Pack=1)]
    public struct Device {
        public DeviceFlags f_flags;
        public byte n_joysticks;
        public byte n_modes;
        public UInt16 n_blocks;
    }
}
#else
typedef struct __attribute__((packed, aligned(1))) Device {
    byte f_flags;
    byte n_joysticks;
    byte n_modes;
    uint16_t n_blocks;
} device_t;
#endif