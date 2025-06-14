using System.Runtime.InteropServices;

namespace Core.Net
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SockAddrSz
    {
        unsafe public fixed byte a[@sockaddrsz@];
    }
}