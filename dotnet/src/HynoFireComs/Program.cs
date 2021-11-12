using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace HynoFireComs
{
    public class HypnoFireComsInvoke
    {
        [DllImport("HypnoFireComs")]
        public static extern int createSerialSession([MarshalAs(UnmanagedType.LPStr)]string devicePath, uint baudRate, ref IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern void destroySession(IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern int sendPulse(IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern int sendSolidColor(IntPtr container, int layer, int key, int color, int durationTicks, int fadInTicks, int fadOutTicks);
    }
    
    class Program
    {
        private const int DarkBlue = 0x00008B;
        private const int DarkRed = 0x8B0000;
        
        static void Main(string[] args)
        {
            // while (!Debugger.IsAttached)
            // {
            //     Thread.Sleep(100);
            // }
            Environment.SetEnvironmentVariable("LD_LIBRARY_PATH", "/home/pknopf/git/hypno-fire/build-hypno-fire-coms-Desktop_Qt_6_2_1_GCC_64bit-Debug");
    
            
            IntPtr container = IntPtr.Zero;
            if (HypnoFireComsInvoke.createSerialSession("/dev/ttyACM0", 9600, ref container) != 0)
            {
                Console.WriteLine("Couldn't open device...");
                return;
            }

            var flip = true;
            while (true)
            {
                flip = !flip;
                HypnoFireComsInvoke.sendSolidColor(container, 0, 0, flip ? DarkBlue : DarkRed, 20, 0, 0);
                Thread.Sleep(1000);
            }

            HypnoFireComsInvoke.destroySession(container);
        }
    }
}
