using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

namespace HynoFireComs
{
    public class HypnoFireComsInvoke
    {
        [DllImport("HypnoFireComs")]
        public static extern int createSerialSession([MarshalAs(UnmanagedType.LPStr)]string devicePath, uint baudRate, ref IntPtr container);
        [DllImport("libHypnoFireComs.so")]
        public static extern void destroySession(IntPtr container);
        [DllImport("libHypnoFireComs.so")]
        public static extern int sendPulse(IntPtr container);
    }
    
    class Program
    {
        static void Main(string[] args)
        {
            // while (!Debugger.IsAttached)
            // {
            //     Thread.Sleep(100);
            // }
            Environment.SetEnvironmentVariable("LD_DEBUG", "1");
            Environment.SetEnvironmentVariable("LD_LIBRARY_PATH", "/home/pknopf/git/hypno-fire/build-hypno-fire-coms-Desktop_Qt_6_2_1_GCC_64bit-Debug");
            
            IntPtr container = IntPtr.Zero;
            int result = HypnoFireComsInvoke.createSerialSession("/dev/ttyACM0", 9600, ref container);

            HypnoFireComsInvoke.sendPulse(container);
            HypnoFireComsInvoke.destroySession(container);
        }
    }
}
