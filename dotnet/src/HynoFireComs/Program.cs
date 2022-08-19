using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace HynoFireComs
{
    public class HypnoFireComsInvoke
    {
        public enum LayerBrush : int
        {
            Add = 0,
            Subtract = 1,
            Overwrite = 2,
            Overlay = 3,
            Combine = 4,
            Mask = 5
        }
        [DllImport("HypnoFireComs")]
        public static extern int createSerialSession([MarshalAs(UnmanagedType.LPStr)]string devicePath, uint baudRate, ref IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern void destroySession(IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern int sendPrime(IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern int sendPulse(IntPtr container, int universe, int layer, int key, int color, int fadeInTicks, int durationTicks, int fadeOutTicks);
        [DllImport("HypnoFireComs")]
        public static extern int sendReset(IntPtr container);
        [DllImport("HypnoFireComs")]
        public static extern int sendLayerConfiguration(IntPtr container, int layer, LayerBrush layerBrush);
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
            
            for (int x = 0; x < 10000; x++)
            {
                HypnoFireComsInvoke.sendPrime(container);
                //HypnoFireComsInvoke.sendReset(container);
                HypnoFireComsInvoke.sendPulse(container, 0, 0, 0, DarkBlue, 0, 2, 0);
                Thread.Sleep(50);
                HypnoFireComsInvoke.sendPulse(container, 0, 0, 0, DarkBlue, 0, 2, 0);
                Thread.Sleep(50);
                HypnoFireComsInvoke.sendPulse(container, 0, 0, 0, DarkBlue, 0, 2, 0);
                Thread.Sleep(50);
                HypnoFireComsInvoke.sendPulse(container, 0, 0, 0, DarkBlue, 0, 2, 0);
                Thread.Sleep(1000);
            }
            
            return;
            
            HypnoFireComsInvoke.sendPrime(container);
            HypnoFireComsInvoke.sendReset(container);
            
            Console.WriteLine("Waiting for reset..");
            Thread.Sleep(2000);
            
            var flip = true;
            while (true)
            {
                flip = !flip;
                if (HypnoFireComsInvoke.sendPulse(container, 0, 0, 0, flip ? DarkBlue : DarkRed, 0, 3, 10) != 0)
                {
                    Console.WriteLine("Couldn't send pulse..");
                }
                else
                {
                    Console.WriteLine("Sent!");
                }
                Thread.Sleep(1000);
            }

            HypnoFireComsInvoke.destroySession(container);
        }
    }
}
