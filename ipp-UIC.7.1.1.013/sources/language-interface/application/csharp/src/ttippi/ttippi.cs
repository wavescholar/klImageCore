/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Using Intel(R) IPP 
//     in Microsoft* C# .NET for Windows* Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/

namespace ExampleIP
{
   using System;
   using System.IO;
   using System.Windows.Forms;
   using System.Drawing;
   using System.Drawing.Imaging;
   using System.Reflection;
   using System.Collections;
   using ipp;
   using System.Security;
   using System.Runtime.InteropServices;

   public class tip : System.Windows.Forms.Form 
   {

      private System.ComponentModel.Container components;
      private System.Drawing.Bitmap bmpsrc, bmpdst;
      private string libver, strSavePanelText;
      private StatusBarPanel sbpMenuHelp;
      private Assembly assembly;
      private object ippi;
      private Type ippiType;
      private Hashtable hash;
      private IppiSize roi;
      private ulong m_start, m_stop;
      private float m_sx, m_sy;
      private double[] m_cpx = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      private double m_mean, m_stddev;

      unsafe public tip(Bitmap bmp, string dir) 
      {
         assembly = Assembly.LoadFrom(dir+"\\ippi_cs.dll");
         ippi = assembly.CreateInstance("ipp.ip");
         ippiType = ippi.GetType();
         MethodInfo method = ippiType.GetMethod("ippiGetLibVersion");
         IppLibraryVersion lib = (IppLibraryVersion)method.Invoke(null, null);
         this.libver = lib.Name+" "+lib.Version;
         bmpsrc = bmp;
         InitializeComponent();
         bmpdst = new Bitmap( bmpsrc );
         roi = new IppiSize( bmpsrc.Width*3/4, bmpsrc.Height*3/4 );
      }

      protected override void OnPaint( PaintEventArgs e ) 
      {
         Graphics g = e.Graphics;
         g.ScaleTransform(m_sx, m_sy);
         g.DrawImage( bmpdst, 4, 4 );
      }

      private BitmapData getBmpData( Bitmap bmp ) 
      {
         return bmp.LockBits( new Rectangle(0,0,bmp.Width,bmp.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb );
      }

      private void msg( string fun, IppStatus st ) 
      {
         Console.WriteLine("-{0}: ({1}) {2}", fun, st,
            "sp.ippGetStatusString((IppStatus)st)" );
      }

      // Filtering Functions
      unsafe private void FilteringFunction(string func)
      {
         IppStatus st = IppStatus.ippStsNoErr;
         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );
         MethodInfo method = ippiType.GetMethod(func);
         const int ksize = 5, half = ksize/2;
         byte* pSrc = (byte*)bmpsrcdata.Scan0+(bmpsrcdata.Stride+3)*half,
               pDst = (byte*)bmpdstdata.Scan0+(bmpdstdata.Stride+3)*half;
         for (int i = 0; i < 10; i++)
         {
            m_start = ipp.core.ippGetCpuClocks();
            st = (ipp.IppStatus)method.Invoke(null, new object[]
            {(IntPtr)pSrc,bmpsrcdata.Stride, (IntPtr)pDst,bmpdstdata.Stride,
             roi, new IppiSize(ksize,ksize), new IppiPoint(half,half)});
            m_stop = ipp.core.ippGetCpuClocks();
            m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("{0}: {1,2:F}+-{2,2:F} cpx", func, m_mean, m_stddev);
         if (st != IppStatus.ippStsNoErr) msg(func, st);
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );
         Invalidate();
      }

      private void MenuFilteringOnClick(object sender,System.EventArgs e) 
      {
         FilteringFunction((string)hash[sender]);
      }

      unsafe private void FixedFilteringFunction(string func)
      {
         IppStatus st = IppStatus.ippStsNoErr;
         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );
         MethodInfo method = ippiType.GetMethod(func);
         const int ksize = 3, half = ksize/2;      
         byte* pSrc = (byte*)bmpsrcdata.Scan0+(bmpsrcdata.Stride+3)*half,
            pDst = (byte*)bmpdstdata.Scan0+(bmpdstdata.Stride+3)*half;
         ParameterInfo[] parms = method.GetParameters();
         for (int i = 0; i < 10; i++)
         {
            m_start = ipp.core.ippGetCpuClocks();
            if (5 == parms.Length)
               st = (ipp.IppStatus)method.Invoke(null, new object[] { (IntPtr)pSrc, bmpsrcdata.Stride, (IntPtr)pDst, bmpdstdata.Stride, roi });
            if (6 == parms.Length)
               st = (ipp.IppStatus)method.Invoke(null, new object[] { (IntPtr)pSrc, bmpsrcdata.Stride, (IntPtr)pDst, bmpdstdata.Stride, roi, ipp.IppiMaskSize.ippMskSize3x3 });
            m_stop = ipp.core.ippGetCpuClocks();
            m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("{0}: {1,2:F}+-{2,2:F} cpx", func, m_mean, m_stddev);
         if (st != IppStatus.ippStsNoErr) msg(func, st);
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );
         Invalidate();
      }

      private void MenuFixFilteringOnClick(object sender,System.EventArgs e) 
      {
         FixedFilteringFunction((string)hash[sender]);
      }

      // Morphologogical Functions
      unsafe private void MorphologicalFunction(string func)
      {
         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );
         MethodInfo method = ippiType.GetMethod(func);
         const int msize = 5, half = msize/2;
         IppStatus st = IppStatus.ippStsNoErr;
         byte[] mask = {
                          0, 1, 1, 1, 0,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          0, 1, 1, 1, 0 };
         byte* pSrc = (byte*)bmpsrcdata.Scan0+(bmpsrcdata.Stride+3)*half,
               pDst = (byte*)bmpdstdata.Scan0+(bmpdstdata.Stride+3)*half;
         ParameterInfo[] parms = method.GetParameters();
         fixed( byte* pmask = mask ) 
         {
            for (int i = 0; i < 10; i++)
            {
               m_start = ipp.core.ippGetCpuClocks();
               if (8 == parms.Length)
               {
                  st = (ipp.IppStatus)method.Invoke(null, new object[]
                  {(IntPtr)pSrc,bmpsrcdata.Stride,(IntPtr)pDst,bmpdstdata.Stride,roi,(IntPtr)pmask,
                   new IppiSize(msize,msize), new IppiPoint(half,half)});
               }
               if (6 == parms.Length)
               {
                  st = (ipp.IppStatus)method.Invoke(null, new object[]
                  {(IntPtr)pDst,bmpdstdata.Stride,roi,(IntPtr)pmask,
                    new IppiSize(msize,msize), new IppiPoint(half,half)});
               }
               m_stop = ipp.core.ippGetCpuClocks();
               m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
            }
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("{0}: {1,2:F}+-{2,2:F} cpx", func, m_mean, m_stddev);
         if (st != IppStatus.ippStsNoErr) msg(func, st);
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );
         Invalidate();
      }

      private void MenuMorphoOnClick(object sender,System.EventArgs e) 
      {
         MorphologicalFunction((string)hash[sender]);
      }

      private void MenuCloseOnClick(object sender,System.EventArgs e) 
      {
         MorphologicalFunction("ippiDilate_8u_C3R");
         MorphologicalFunction("ippiErode_8u_C3IR");
      }

      private void MenuOpenMOnClick(object sender,System.EventArgs e) 
      {
         MorphologicalFunction("ippiErode_8u_C3R");
         MorphologicalFunction("ippiDilate_8u_C3IR");
      }

      // Geometric Functions
      unsafe private void MenuMirrorOnClick(object sender,System.EventArgs e) 
      {
//         m_start = ipp.core.ippGetCpuClocks();
//         bmpdst.RotateFlip(RotateFlipType.Rotate180FlipNone);

         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );

         IppiSize roi1 = new IppiSize(bmpsrc.Width, bmpsrc.Height);
         IppStatus st = IppStatus.ippStsNoErr;
         for (int i = 0; i < 10; i++)
         {
            m_start = ipp.core.ippGetCpuClocks();
            st = ip.ippiMirror_8u_C3R((byte*)bmpsrcdata.Scan0, bmpsrcdata.Stride,
               (byte*)bmpdstdata.Scan0, bmpdstdata.Stride, roi1, ipp.IppiAxis.ippAxsBoth);
            m_stop = ipp.core.ippGetCpuClocks();
            m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("ippiMirror_8u_C3R: {0,2:F}+-{1,2:F} cpx", m_mean, m_stddev);

         if( st != IppStatus.ippStsNoErr ) msg( "Mirror", st );
      
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );

//         m_stop = ipp.core.ippGetCpuClocks();
         //sbpMenuHelp.Text = string.Format("{0,2:F} cpx", (double)(m_stop-m_start)/(3.0*roi.width*roi.height));
         Invalidate();
      }

      unsafe private void MenuRotateOnClick(object sender,System.EventArgs e) 
      {

         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );
         IppStatus st = IppStatus.ippStsNoErr;

         for (int i = 0; i < 10; i++)
         {
            m_start = ipp.core.ippGetCpuClocks();
            st = ip.ippiRotateCenter_8u_C3R((byte*)bmpsrcdata.Scan0,
               new IppiSize(bmpsrc.Width, bmpsrc.Height), bmpsrcdata.Stride,
               new IppiRect(0, 0, roi.width, roi.height),
               (byte*)bmpdstdata.Scan0, bmpdstdata.Stride, new IppiRect(0, 0, roi.width, roi.height),
               10.0, 0.5 * roi.width, 0.5 * roi.height, 1);
            m_stop = ipp.core.ippGetCpuClocks();
            m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("ippiRotateCenter_8u_C3R: {0,2:F}+-{1,2:F} cpx", m_mean, m_stddev);
         if( st != IppStatus.ippStsNoErr ) msg( "RotateCenter", st );
      
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );
         Invalidate();
      }

      unsafe private void MenuWarpOnClick(object sender,System.EventArgs e) 
      {
         BitmapData bmpsrcdata = getBmpData( bmpsrc );
         BitmapData bmpdstdata = getBmpData( bmpdst );
         IppStatus st = IppStatus.ippStsNoErr;
         double[,] coeff = { {0.9, 0.1, 0}, {0.1, 0.9, 0} };
         fixed (double* pcoeff = coeff)
         {
            for (int i = 0; i < 10; i++)
            {
               m_start = ipp.core.ippGetCpuClocks();
               st = ip.ippiWarpAffine_8u_C3R((byte*)bmpsrcdata.Scan0, new IppiSize(bmpsrc.Width,
                  bmpdst.Height), bmpsrcdata.Stride, new IppiRect(0, 0, bmpsrc.Width, bmpdst.Height),
                  (byte*)bmpdstdata.Scan0,
                  bmpdstdata.Stride, new IppiRect(0, 0, bmpdst.Width, bmpdst.Height), pcoeff, 1);
               m_stop = ipp.core.ippGetCpuClocks();
               m_cpx[i] = (double)(m_stop - m_start) / (3.0 * roi.width * roi.height);
            }
         }
         fixed (double* p = m_cpx, mean = &m_mean, stddev = &m_stddev)
         {
            ipp.sp.ippsSortAscend_64f_I(p, 10);
            ipp.sp.ippsMeanStdDev_64f(p, 8, mean, stddev);
         }
         sbpMenuHelp.Text = string.Format("ippiWarpAffine_8u_C3R: {0,2:F}+-{1,2:F} cpx", m_mean, m_stddev);
         if (st != IppStatus.ippStsNoErr) msg("WarpAffine", st);
         bmpsrc.UnlockBits( bmpsrcdata );
         bmpdst.UnlockBits( bmpdstdata );
         Invalidate();
      }

      void AdjustDrawingSize()
      {
         int width = bmpsrc.Width, height = bmpsrc.Height;
         if( (width > 632) || (height > 452) )
         {
            m_sx = 632.0f/(float)width;
            m_sy = 452.0f/(float)height;
            if( m_sx < m_sy )
               m_sy = m_sx;
            else
               m_sx = m_sy;
            width = (int)(width * m_sx + 0.5f);
            height = (int)(height * m_sy + 0.5f);
         } 
         else 
         {
            m_sx = 1.0f; m_sy = 1.0f;
         }
         this.ClientSize = new System.Drawing.Size( width+8, height+28 );
      }

      unsafe void RGBA_FPX_to_BGRA(byte* data, int width, int height, int step)
      {
         int i, j;
         byte r, g, b, a;
         byte* ptr;

         ptr = data;
         for( i = 0; i < height; i++ )
         {
            ptr = data + step*i;
            for( j = 0; j < width; j++ )
            {
               r = ptr[0];
               g = ptr[1];
               b = ptr[2];
               a = ptr[3];
               ptr[2] = (byte)( (r*a+1) >> 8 );
               ptr[1] = (byte)( (g*a+1) >> 8 );
               ptr[0] = (byte)( (b*a+1) >> 8 );
               ptr += 4;
            }
         }
      }

      unsafe void BGRA_to_RGBA(byte* data, int width, int height, int step)
      {
         int   i, j;
         byte  r, g, b;
         byte* ptr;

         ptr = data;
         for( i = 0; i < height; i++ )
         {
            ptr = data + step*i;
            for( j = 0; j < width; j++ )
            {
               b = ptr[0];
               g = ptr[1];
               r = ptr[2];
               ptr[0] = r;
               ptr[1] = g;
               ptr[2] = b;
               ptr += 4;
            }
         }
         return;
      }

      int width=0, height=0, nChannels=0, precision=0;
      JCOLOR color=0, imageColor=0;

      unsafe Bitmap OpenJPEG(string filename) 
      {
         FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
         long JPEGSize = fs.Length;
         BinaryReader br = new BinaryReader(fs);
         byte[] pJPEG = br.ReadBytes((int)JPEGSize);
         br.Close();
         fs.Close();
         JPEGDecoder decoder = new JPEGDecoder();
         decoder.SetSource(ref pJPEG, (int)JPEGSize);
         JSS sampling=0;
         decoder.ReadHeader(ref width, ref height, ref nChannels, ref precision,
            ref color, ref sampling);
         PixelFormat form;
         switch( nChannels )
         {
            case 1:
               imageColor = JCOLOR.JC_GRAY;
               form = PixelFormat.Format8bppIndexed;
               break;
            case 3:
               imageColor = JCOLOR.JC_BGR;
               form = PixelFormat.Format24bppRgb;
               break;
            case 4:
               imageColor = JCOLOR.JC_CMYK;
               form = PixelFormat.Format32bppArgb;
               break;
            default:
               form = PixelFormat.Format24bppRgb;
               break;
         }

         Bitmap dst = new Bitmap(width, height, form);
         BitmapData dstdata = getBmpData(dst);
         decoder.SetDestination((byte*)dstdata.Scan0, dstdata.Stride,
            new IppiSize(width, height), nChannels, imageColor, 8);
         decoder.ReadData();
         if( imageColor == JCOLOR.JC_CMYK )
         {
            RGBA_FPX_to_BGRA((byte*)dstdata.Scan0,width,height,dstdata.Stride);
         }
         dst.UnlockBits(dstdata);
         return dst;
      }
      void MenuOpenOnClick(object sender, EventArgs ea)
      {
         OpenFileDialog dlg = new OpenFileDialog();
         dlg.Filter = "JPEG File Interchange Format (*.jpg)|*.jpg";
         if( DialogResult.OK == dlg.ShowDialog() )
         {
            Bitmap bmp = OpenJPEG( dlg.FileName );
            //Bitmap bmp = new Bitmap( dlg.FileName );
            bmpsrc = bmp;
            bmpdst = new Bitmap( bmpsrc );
            roi = new IppiSize( bmpsrc.Width*3/4, bmpsrc.Height*3/4 );
            AdjustDrawingSize();
            Invalidate();
         }
      }

      unsafe void MenuSaveOnClick(object sender, EventArgs ea)
      {
         SaveFileDialog dlg = new SaveFileDialog();
         dlg.Filter = "JPEG File Interchange Format (*.jpg)|*.jpg";
         if( DialogResult.OK == dlg.ShowDialog() )
         {
            IppiSize     roi;
            JSS          sampling  = JSS.JS_411;
            JCOLOR       in_color = JCOLOR.JC_BGR;
            JCOLOR       out_color = JCOLOR.JC_YCBCR;
            int          JPEGSize;
            byte[]       pJPEG;
            JERRCODE     jerr;
            JPEGEncoder  encoder = new JPEGEncoder();

            roi = new IppiSize(bmpdst.Width, bmpdst.Height);
            JPEGSize = roi.width * roi.height * nChannels;
            // just for too small images
            if( JPEGSize < 1024 )
            {
               JPEGSize = 4096;
            }
            pJPEG = new byte[JPEGSize];
            switch( nChannels )
            {
               case 1:
                  in_color  = JCOLOR.JC_GRAY;
                  out_color = JCOLOR.JC_GRAY;
                  sampling  = JSS.JS_444;
                  break;
               case 3:
                  in_color  = JCOLOR.JC_BGR;
                  out_color = JCOLOR.JC_YCBCR;
                  break;
               case 4:
                  in_color  = JCOLOR.JC_CMYK;
                  out_color = JCOLOR.JC_YCCK;
                  break;
               default:
                  break;
            }
            BitmapData dstdata = getBmpData(bmpdst);
            encoder.SetSource((byte*)dstdata.Scan0, dstdata.Stride, roi, nChannels,in_color);
            encoder.SetDestination(ref pJPEG, JPEGSize, 75, sampling, out_color, JMODE.JPEG_BASELINE, 0);
            //encoder.SetDestination(ref pJPEG, JPEGSize, 75, sampling, out_color, JMODE.JPEG_PROGRESSIVE, 0);
            if( in_color == JCOLOR.JC_CMYK )
            {
               BGRA_to_RGBA((byte*)dstdata.Scan0, width, height, dstdata.Stride);
            }
            int size = JPEGSize;
            jerr = encoder.WriteImageBaseline(ref size);
            //jerr = encoder.WriteImageProgressive(ref size);
            if( JERRCODE.JPEG_OK == jerr )
            {
               FileStream fs = new FileStream(dlg.FileName, FileMode.Create, FileAccess.Write);
               BinaryWriter bw = new BinaryWriter(fs);
               bw.Write(pJPEG, 0, size);
               bw.Close();
               fs.Close();
               bmpdst.UnlockBits(dstdata);
            }
         }
      }

      void MenuResetOnClick(object sender, EventArgs ea)
      {
         bmpdst = new Bitmap( bmpsrc );
         Invalidate();
      }

      void MenuExitOnClick(object sender, EventArgs ea)
      {
         Close();
      }

      protected override void OnMenuStart(EventArgs ea)
      {
         strSavePanelText = sbpMenuHelp.Text;
      }

      protected override void OnMenuComplete(EventArgs ea)
      {
         sbpMenuHelp.Text = strSavePanelText;
      }

      void MenuOnSelect(object obj, EventArgs ea)
      {
         MenuItem mi = (MenuItem)obj;
         string str;
         switch( mi.Text )
         {
            case "Open...":
               str = "Open the JPG-file"; break;
            case "Save as...":
               str = "Save the image as JPG-file"; break;
            case "Exit":
               str = "Close the program"; break;
            case "Blur":
               str = "Smoothing or blurring"; break;
            case "Min Filter":
               str = "Decreasing the contrast"; break;
            case "Max Filter":
               str = "Increasing the contrast"; break;
            case "Median Filter":
               str = "Removing the isolated intensity spikes"; break;
            case "Prewitt Horizontal":
               str = "Enhancing horizontal edges"; break;
            case "Prewitt Vertical":
               str = "Enhancing vertical edges"; break;
            case "Sobel Horizontal":
               str = "Enhancing and smoothing horizontal edges"; break;
            case "Sobel Vertical":
               str = "Enhancing and smoothing vertical edges"; break;
            case "Roberts Horizontal":
               str = "Horizontal Roberts operator"; break;
            case "Roberts Vertical":
               str = "Vertical Roberts operator"; break;
            case "Sharpen":
               str = "Enhances high-frequency"; break;
            case "Laplacian":
               str = "Locate zero crossings"; break;
            case "Gaussian":
               str = "Gaussian distribution"; break;
            case "Highpass":
               str = "Attenuates low-frequency"; break;
            case "Lowpass":
               str = "Blurs"; break;
            case "Erode":
               str = "Erosion"; break;
            case "Dilate":
               str = "Dilation"; break;
            case "Open":
               str = "Erosion and dilation"; break;
            case "Close":
               str = "Dilation and erosion"; break;
            case "Mirror":
               str = "Mirrors about a horizontal and vertical axis"; break;
            case "Rotate":
               str = "Rotates the ROI"; break;
            case "Warp":
               str = "Affine warp transforms"; break;
            default:
               str = "";
               break;
         }
         sbpMenuHelp.Text = str;
      }

      void CreateMenu()
      {
         hash = new Hashtable();
         EventHandler ehSelect = new EventHandler(MenuOnSelect);

         MenuItem miDash0 = new MenuItem("-");
         miDash0.Select += ehSelect;
         MenuItem miOpen  = new MenuItem("Open...", new EventHandler(MenuOpenOnClick), Shortcut.CtrlO);
         miOpen.Select += ehSelect;
         MenuItem miSave  = new MenuItem("Save as...", new EventHandler(MenuSaveOnClick), Shortcut.CtrlS);
         miSave.Select += ehSelect;
         MenuItem miExit  = new MenuItem("Exit", new EventHandler(MenuExitOnClick), Shortcut.AltF4);
         miExit.Select += ehSelect;
         MenuItem miFile  = new MenuItem("File", new MenuItem[] {miOpen, miSave, miDash0, miExit});
         miFile.Select += ehSelect;

         // Filtering
         MenuItem miDash1 = new MenuItem("-");
         miDash1.Select += ehSelect;
         MenuItem miBlur   = new MenuItem("Blur", new EventHandler(MenuFilteringOnClick));
         hash.Add(miBlur, "ippiFilterBox_8u_C3R");
         miBlur.Select += ehSelect;
         MenuItem miMin    = new MenuItem("Min Filter", new EventHandler(MenuFilteringOnClick));
         hash.Add(miMin, "ippiFilterMin_8u_C3R");
         miMin.Select += ehSelect;
         MenuItem miMax    = new MenuItem("Max Filter", new EventHandler(MenuFilteringOnClick));
         hash.Add(miMax, "ippiFilterMax_8u_C3R");
         miMax.Select += ehSelect;
         MenuItem miMedian = new MenuItem("Median Filter", new EventHandler(MenuFilteringOnClick));
         hash.Add(miMedian, "ippiFilterMedian_8u_C3R");
         miMedian.Select += ehSelect;
         MenuItem miPrewittH = new MenuItem("Prewitt Horizontal", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miPrewittH, "ippiFilterPrewittHoriz_8u_C3R");
         miPrewittH.Select += ehSelect;
         MenuItem miPrewittV = new MenuItem("Prewitt Vertical", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miPrewittV, "ippiFilterPrewittVert_8u_C3R");
         miPrewittV.Select += ehSelect;
         MenuItem miSobelH   = new MenuItem("Sobel Horizontal", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miSobelH, "ippiFilterSobelHoriz_8u_C3R");
         miSobelH.Select += ehSelect;
         MenuItem miSobelV   = new MenuItem("Sobel Vertical", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miSobelV, "ippiFilterSobelVert_8u_C3R");
         miSobelV.Select += ehSelect;
         MenuItem miRobertsH = new MenuItem("Roberts Horizontal", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miRobertsH, "ippiFilterRobertsDown_8u_C3R");
         miRobertsH.Select += ehSelect;
         MenuItem miRobertsV = new MenuItem("Roberts Vertical", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miRobertsV, "ippiFilterRobertsUp_8u_C3R");
         miRobertsV.Select += ehSelect;
         MenuItem miSharpen  = new MenuItem("Sharpen", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miSharpen, "ippiFilterSharpen_8u_C3R");
         miSharpen.Select += ehSelect;
         MenuItem miLaplace  = new MenuItem("Laplacian", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miLaplace, "ippiFilterLaplace_8u_C3R");
         miLaplace.Select += ehSelect;
         MenuItem miGauss    = new MenuItem("Gaussian", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miGauss, "ippiFilterGauss_8u_C3R");
         miGauss.Select += ehSelect;
         MenuItem miHipass   = new MenuItem("Highpass", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miHipass, "ippiFilterHipass_8u_C3R");
         miHipass.Select += ehSelect;
         MenuItem miLowpass  = new MenuItem("Lowpass", new EventHandler(MenuFixFilteringOnClick));
         hash.Add(miLowpass, "ippiFilterLowpass_8u_C3R");
         miLowpass.Select += ehSelect;
         MenuItem miFixed  = new MenuItem("Fixed Filter", new MenuItem[]
           {miPrewittH,miPrewittV,miSobelH,miSobelV,miRobertsH,miRobertsV,miSharpen,miLaplace,miGauss,miHipass,miLowpass});
         miFixed.Select += ehSelect;
         MenuItem miFilter = new MenuItem("Filter", new MenuItem[] {miBlur,miMin,miMax,miMedian,miDash1,miFixed});
         miFilter.Select += ehSelect;

         // Morphological
         MenuItem miErode  = new MenuItem("Erode", new EventHandler(MenuMorphoOnClick));
         hash.Add(miErode, "ippiErode_8u_C3R");
         miErode.Select += ehSelect;
         MenuItem miDilate = new MenuItem("Dilate", new EventHandler(MenuMorphoOnClick));
         hash.Add(miDilate, "ippiDilate_8u_C3R");
         miDilate.Select += ehSelect;
         MenuItem miClose  = new MenuItem("Close", new EventHandler(MenuCloseOnClick));
         miClose.Select += ehSelect;
         MenuItem miOpenM  = new MenuItem("Open", new EventHandler(MenuOpenMOnClick));
         miOpenM.Select += ehSelect;
         MenuItem miMorpho = new MenuItem("Morphological", new MenuItem[] {miErode,miDilate,miClose,miOpenM});
         miMorpho.Select += ehSelect;

         // Geometric
         MenuItem miMirror = new MenuItem("Mirror", new EventHandler(MenuMirrorOnClick));
         miMirror.Select += ehSelect;
         MenuItem miRotate = new MenuItem("Rotate", new EventHandler(MenuRotateOnClick));
         miRotate.Select += ehSelect;
         MenuItem miWarp   = new MenuItem("Warp", new EventHandler(MenuWarpOnClick));
         miWarp.Select += ehSelect;
         MenuItem miGeometr = new MenuItem("Geometric", new MenuItem[] {miMirror,miRotate,miWarp});
         miGeometr.Select += ehSelect;

         MenuItem miReset  = new MenuItem("Reset", new EventHandler(MenuResetOnClick));
         miReset.Select += ehSelect;

         MenuItem miDash2 = new MenuItem("-");
         miDash2.Select += ehSelect;
         MenuItem miProc = new MenuItem("&Process", new MenuItem[] {miFilter, miMorpho, miGeometr, miDash2, miReset});
         miProc.Select += ehSelect;
         Menu = new MainMenu(new MenuItem[] {miFile, miProc});
      }

      void InitializeComponent() 
      {
         StatusBar sb = new StatusBar();
         sb.Parent = this;
         sb.ShowPanels = true;
         sbpMenuHelp = new StatusBarPanel();
         sbpMenuHelp.Text = "";
         sbpMenuHelp.AutoSize = StatusBarPanelAutoSize.Spring;
         sb.Panels.Add(sbpMenuHelp);

         CreateMenu();

         this.components = new System.ComponentModel.Container();
         AdjustDrawingSize();
         this.Text = this.libver;
      }

      [STAThread] public static void Main() 
      {
         string dir = Application.StartupPath;
         try 
         {
            //OpenFileDialog dlg = new OpenFileDialog();
            //dlg.Filter = "JPEG File Interchange Format (*.jpg)|*.jpg";
            //if( DialogResult.OK == dlg.ShowDialog() )
            //{
               //Bitmap bmp = new Bitmap( dlg.FileName );
               Bitmap bmp = new Bitmap( 200, 100 );
               Application.Run( new tip( bmp, dir ) ); 
            //} 
         } 
         catch( Exception e ) 
         {
            Console.WriteLine( " problem to run:" );
            Console.WriteLine( e.Message );
         }
      }
   }
}
