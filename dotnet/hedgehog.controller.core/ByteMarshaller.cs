// https://codereview.stackexchange.com/a/136011

using System;
using System.Runtime.InteropServices;

namespace hedgehog.controller.core.interop
{
    public sealed class ByteMarshaller<T> : IDisposable where T : struct
    {
        private readonly IntPtr handle;

        private bool isDisposed = false;

        public int StructureSize {get;}

        public ByteMarshaller()
        {
            this.StructureSize = Marshal.SizeOf(typeof(T));
            this.handle = Marshal.AllocHGlobal(this.StructureSize);
        }

        public byte[] ToByteArray(T data)
        {
            var buffer = new byte[this.StructureSize];
            this.CopyBytes(data, buffer, 0);
            return buffer;
        }

        public int CopyBytes(T data, byte[] buffer, int startIndex = 0)
        {
            int copyLength = this.StructureSize;
            int availableLength = buffer.Length - startIndex;

            if (availableLength <= 0)
            {
                return 0;
            }

            if (availableLength < copyLength)
            {
                copyLength = availableLength;
            }

            Marshal.StructureToPtr(data, this.handle, true);
            Marshal.Copy(this.handle, buffer, startIndex, copyLength);

            return copyLength;
        }

        public T ToStructure(byte[] byteArray)
        {
            var packet = new T();

            Marshal.Copy(byteArray, 0, this.handle, this.StructureSize);

            return Marshal.PtrToStructure<T>(this.handle);
        }

        public void Dispose()
        {
            if (!isDisposed)
            {
                Marshal.FreeHGlobal(this.handle);

                isDisposed = true;
            }

            GC.SuppressFinalize(this);
        }

        ~ByteMarshaller()
        {
            Dispose();
        }
    }
}