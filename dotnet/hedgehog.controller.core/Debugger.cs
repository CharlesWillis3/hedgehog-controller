using System;
using hedgehog.controller.core.interop;

namespace hedgehog.controller.core
{
    public class Debugger : Serial
    {
        public Debugger(string serialPortName) : base(serialPortName)
        {
        }

        public bool Start()
        {
            if (!this.OpenAndSayHello())
            {
                Console.WriteLine("Failed waiting for HELLO");
                return false;
            }

            this.Send(HHSerial.MODE_DEBUG);
            if (!this.WaitForMessage(HHSerial.MODE_DEBUG))
            {
                Console.WriteLine("Failed waiting for MODE_DEBUG");
                return false;
            }

            return true;
        }

        public void ReadJoysticks()
        {
            this.Send(HHSerial.JOYSTICKS_READ);

            using (var m = new ByteMarshaller<Joystick>())
            {
                for (int x = 0; x < 4; x++)
                {
                    if (this.WaitForBytes(4, out byte[] result2))
                    {
                        Joystick j = m.ToStructure(result2);
                        Console.WriteLine("Recevied Joystick " + x);
                        j.PrettyPrint();
                    }
                }
            }
        }

        public void ReadDevice()
        {
            Device device = default;

            this.Send(HHSerial.DEVICE_READ);
            if (this.WaitForBytes(4, out byte[] result))
            {
                using (var m = new ByteMarshaller<Device>())
                {
                    device = m.ToStructure(result);
                }

                Console.WriteLine("Recevied device:");
                device.PrettyPrint();
            }
            else
            {
                Console.WriteLine("Failed waiting for Device bytes");
            }
        }

        public void ReportButtons()
        {
            Console.WriteLine();
            Console.WriteLine("Press any key to stop");
            this.Send(HHSerial.BUTTONS_ID);
            
            while (!Console.KeyAvailable)
            {
                var rcvd = this.SerialPort.ReadLine();
                Console.WriteLine(rcvd);
            }

            Console.ReadKey(true);

            this.Send(HHSerial.READY);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.Send(HHSerial.DEBUG_END);
                this.WaitForMessage(HHSerial.MODE_RUN);
            }
        }
    }
}