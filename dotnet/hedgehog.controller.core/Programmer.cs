using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Threading.Tasks;
using hedgehog.controller.core.interop;
using hedgehog.controller.core.model;

namespace hedgehog.controller.core
{
    public class Programmer :  Serial
    {
        public byte NumDigitals { get; private set; }
        public byte NumAnalogs { get; private set; }
        public byte NumNeoPixels { get; private set; }

        public Programmer(string serialPortName) : base(serialPortName)
        {
        }

        public bool Start(Configuration config)
        {
            if (!this.OpenAndSayHello())
            {
                return false;
            }

            this.Send(HHSerial.MODE_PROGRAMMING);
            if (!this.WaitForMessage(HHSerial.MODE_PROGRAMMING))
            {
                Console.WriteLine($"Failed waiting for: {HHSerial.READY}");
            }

            this.Send(HHSerial.REQUEST_HARDWARE);
            if (!this.WaitForBytes(3, out byte[] result))
            {
                Console.WriteLine($"Failed waiting for hardware description. Received: {result[0]}:{result[1]}:{result[2]}");
            }

            this.NumDigitals = result[(int)HHHardwareInfo.NUM_DIGITALS];
            this.NumAnalogs = result[(int)HHHardwareInfo.NUM_ANALOGS];
            this.NumNeoPixels = result[(int)HHHardwareInfo.NUM_NEOPIXELS];

            this.SendHeader(config.Controller);

            this.Send(HHSerial.MODE_RUN);
            if (this.WaitForMessage(HHSerial.MODE_RUN))
            {
                return true;
            }

            return false;
        }

        private void SendHeader(HedgehogController controller)
        {
            this.Send(HHSerial.DEVICE_START);
            this.WaitForMessage(HHSerial.READY);

            using (var m = new ByteMarshaller<Device>())
            {
                Console.WriteLine($"Size of {nameof(Device)}: {m.StructureSize}");
                m.CopyBytes(controller.Device, Serial.WriteBuffer);
                this.SendWriteBuffer(0, 4);
            }

            this.WaitForMessage(HHSerial.READY);

            this.Send(HHSerial.JOYSTICK_START);
            this.WaitForMessage(HHSerial.READY);

            using (var m = new ByteMarshaller<Joystick>())
            {
                Console.WriteLine($"Size of {nameof(Joystick)}: {m.StructureSize}");

                for (int x = 0; x < 4; x++)
                {
                    var j = controller.Joysticks.ElementAtOrDefault(x);
                    m.CopyBytes(j, Programmer.WriteBuffer);
                    this.SendWriteBuffer(0, m.StructureSize);
                }
            }

            this.WaitForMessage(HHSerial.READY);
        }
    }
}
