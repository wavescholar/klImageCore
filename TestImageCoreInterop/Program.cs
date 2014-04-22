
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Threading;
using klBigTiff;
using klProcessImage;

using klDNT;

//using klCounters;

namespace TestklImageCoreV
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using klBigTiff;
    using System.IO;
    using System.Collections;   
    using log4net;
    using klCounters;

    class Program
    {
        static public ArrayList getTestFiles()
        {
            ArrayList symbols = new ArrayList();

            symbols.Add("d:\\klImaging\\test.tif");

            //symbols.Add("d:\\klImaging\\lena_std.tiff");
            //symbols.Add("d:\\klImaging\\l_hires.tif");
            
            return symbols;
        }
        static void Main(string[] args)
        {
            //We need security perms to set up perf counters
            klPerformanceCounter klpc = new klPerformanceCounter();
            klpc.SetupCategory();
            PerformanceCounter PC = null;
            PerformanceCounter BPC = null;
            klpc.CreateCounters(ref PC, ref BPC);
            ArrayList files = getTestFiles();
            int numFiles = files.Count;

            ArrayList ticsList = new ArrayList();

            for (int i = 0; i < numFiles; i++)
            {
                try
                {
                    klImageProcessor klip = new klImageProcessor();
                    klip.setStdOut();

                    klDOTNETTimer kldnt = new klDOTNETTimer();
                    kldnt.Start();

                    FileInfo fileInfo = new FileInfo(files[i].ToString());

                    klip.Process(fileInfo.Name);

                    kldnt.Stop();
                    double itime = 1;// kldnt.Duration;
                    ticsList.Add(itime);
                    
                    long ifreq = kldnt.Frequency;
                    double avg = 0;
                    for (int k = 0; k < ticsList.Count; k++)
                    {
                        avg += (double)ticsList[k];
                    }
                    avg = (long)Math.Floor(((decimal)avg / ticsList.Count));

                    PC.Increment();
                    BPC.RawValue = (long)itime;
                    //AITB.RawValue = ifreq;

                }
                catch (Exception e)
                {
                }
            }

           klpc.RemoveCategory("klPerformanceCounter");

        }

        static void ReadWSIWorkFlow(string[] args)
        {

            klPerformanceCounter klpc = new klPerformanceCounter();
            klpc.RemoveCategory("klPerformanceCounter");
            klpc.SetupCategory();
            PerformanceCounter PC = null;
            PerformanceCounter BPC = null;
            klpc.CreateCounters(ref PC, ref BPC);
            
            //Thread.Sleep(2000);

            //log4net.Config.XmlConfigurator.Configure();
            ArrayList files = getTestFiles();
            int numFiles = files.Count;

            ArrayList ticsList = new ArrayList();

            for (int i = 0; i < numFiles; i++)
            {
                try
                {
                    klBigTiffManagedReader bigTiffReader = new klBigTiffManagedReader();
                    klDOTNETTimer kldnt = new klDOTNETTimer();
                    kldnt.Start();
                    FileInfo fileInfo = new FileInfo(files[i].ToString());
                    String ofileName = fileInfo.Name;
                    StringBuilder sb = new StringBuilder(ofileName);
                    sb.Replace(".ndpi", "");
                    sb.Replace(".jp2", "");
                    sb.Replace(".tif", "");
                    sb.Replace(".svs", "");
                    sb.Replace(" ", "");
                    
                    String outfile = sb.ToString();
                    if (bigTiffReader.isklBigTiffImage(fileInfo.ToString()))
                    {
                        String exp = bigTiffReader.GetExposure(fileInfo.ToString());

                        String fnp = outfile;
                        bigTiffReader.ImportImage(fileInfo.ToString(), fnp);
                    }
                    

                    kldnt.Stop();
                    double itime = 1;// kldnt.Duration;
                    
                    ticsList.Add(itime);

                    long ifreq = kldnt.Frequency;
                    double avg = 0;
                    for (int k = 0; k < ticsList.Count; k++)
                    {
                        avg += (double)ticsList[k];

                    }
                    avg = (long)Math.Floor(((decimal)avg / ticsList.Count));

                    PC.Increment();
                    BPC.RawValue = (long)itime;
                    //AITB.RawValue = ifreq;

                }
                catch (Exception e)
                {
                }
            }


        }
    }
}
