
using System;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Threading;
namespace klDNT
{
    public class klDOTNETTimer
    {
        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceCounter(out long lpPerformanceCount);
        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceFrequency(out long lpFrequency);
        private long startTime;
        private long stopTime;
        private long freq;

        public klDOTNETTimer()
        {
            startTime = 0;
            stopTime = 0;
            freq = 0;
            if (QueryPerformanceFrequency(out freq) == false)
            {
                throw new Win32Exception(); 
            }
        }

        public long Start()
        {
            QueryPerformanceCounter(out startTime);
            return startTime;
        }

        public long Stop()
        {
            QueryPerformanceCounter(out stopTime);
            return stopTime;
        }

        public double Duration
        {
            get
            {
                return (double)(stopTime - startTime) / (double)freq;
            }
        }

        public double DurationTics
        {
            get
            {
                return (double) (stopTime - startTime);
            }
        }

        public long Frequency
        {
            get
            {
                QueryPerformanceFrequency(out freq);
                return freq;
            }
        }
    }
}

