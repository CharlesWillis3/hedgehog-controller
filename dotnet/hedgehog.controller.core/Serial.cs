using System;
using System.IO.Ports;
using hedgehog.controller.core.interop;

namespace hedgehog.controller.core
{
    public class Serial : IDisposable
    {
        public const int DefaultReadTimeoutMs = 2000;

        private readonly string portName;

        private bool disposedValue = false;

        public Serial(string serialPortName)
        {
            this.portName = serialPortName ?? throw new ArgumentNullException(nameof(serialPortName));
            this.SerialPort = new SerialPort(serialPortName)
            {
                BaudRate = 9600,
                DiscardNull = false,
                Parity = Parity.None,
                StopBits = StopBits.One,
                DataBits = 8,
                Handshake = Handshake.RequestToSend,
            };
        }

        protected static byte[] WriteBuffer { get; } = new byte[4];

        public SerialPort SerialPort { get; private set; }

        public static string[] GetPorts()
        {
            return SerialPort.GetPortNames();
        }

        public void Reset()
        {
            if (this.SerialPort.IsOpen)
            {
                Console.WriteLine("Closing open port");
                this.SerialPort.Close();
            }

            this.SerialPort = new SerialPort(this.portName)
            {
                BaudRate = 1200,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One
            };

            Console.WriteLine("Attempting reset...");
            this.SerialPort.Open();
            this.SerialPort.Close();
            Console.WriteLine("Done.");
        }

        public void Dispose()
        {
            Dispose(true);
        }

        protected bool OpenAndSayHello()
        {
            this.SerialPort.Open();

            Console.WriteLine("Tickling the Hedgehog...");

            this.Send(HHSerial.HELLO);
            if (!this.WaitForMessage(HHSerial.HELLO))
            {
                Console.WriteLine($"Failed waiting for: {HHSerial.HELLO}");
                return false;
            }
            else
            {
                Console.WriteLine("The Hedgehog likes it :-)");
            }

            return true;
        }

        protected void Send(HHSerial message)
        {
            Serial.WriteBuffer[0] = (byte)message;
            this.SerialPort.Write(Serial.WriteBuffer, 0, 1);
        }

        
        protected void SendWriteBuffer(int startIndex, int length)
        {
            this.SerialPort.Write(Serial.WriteBuffer, startIndex, length);
        }

        protected bool WaitForBytes(int nBytes, out byte[] result)
        {
            this.SerialPort.ReadTimeout = Serial.DefaultReadTimeoutMs;
            result = new byte[nBytes];
            for (int x = 0; x < nBytes; x++)
            {
                // read = this.serialPort.Read(result, 0, 3);
                result[x] = (byte)this.SerialPort.ReadByte();
                Console.WriteLine($"Read Byte[{x}]: {result[x]}");
            }

            return true;
        }

        protected bool WaitForMessage(HHSerial waitFor, int timeoutMs = Serial.DefaultReadTimeoutMs)
        {
            this.SerialPort.ReadTimeout = timeoutMs;
            HHSerial reply = HHSerial.UNKNOWN;

            try
            {
                reply = (HHSerial)this.SerialPort.ReadByte();
            }
            catch (TimeoutException)
            {
                throw;
            }

            if (reply == waitFor)
            {
                return true;
            }

            switch (reply)
            {
                case HHSerial.ERROR:
                    return false;
                case HHSerial.FAULT:
                    throw new ApplicationException("The device has faulted. Cannot continue.");
                default:
                    throw new InvalidOperationException($"An unexpected reply was received. Expected: 0x{waitFor:X}. Actual: 0x{reply:X}");
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    this.SerialPort.Dispose();
                }

                disposedValue = true;
            }
        }
    }
}