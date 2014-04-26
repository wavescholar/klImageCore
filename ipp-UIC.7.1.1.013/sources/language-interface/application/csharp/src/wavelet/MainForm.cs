/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.IO;
using ipp;

namespace wavelet
{
   public enum WtType
   {
      WT_HAAR,
      WT_LINSPLINE,
      WT_QUADSPLINE,
      WT_VAIDYANATHAN
   }

   public struct WtFilter
   {
      public float[] taps;
      public int anchor;
   }

   public struct WtBank
   {
      public WtType   type;
      public WtFilter filterDecLow;
      public WtFilter filterDecHigh;
      public WtFilter filterRecLow;
      public WtFilter filterRecHigh;
      public void Init(WtType type)
      {
         switch (type)
         {
            case WtType.WT_HAAR:
               type = WtType.WT_HAAR;
               filterDecLow.taps = new float[] { 0.5f, 0.5f };
               filterDecLow.anchor = 1;
               filterDecHigh.taps = new float[] { 0.5f, -0.5f };
               filterDecHigh.anchor = 1;
               filterRecLow.taps = new float[] { 1.0f, 1.0f };
               filterRecLow.anchor = 0;
               filterRecHigh.taps = new float[] { -1.0f, 1.0f };
               filterRecHigh.anchor = 0;
               break;
            case WtType.WT_LINSPLINE:
               type = WtType.WT_LINSPLINE;
               filterDecLow.taps = new float[] { 0.25f, 0.5f, 0.25f };
               filterDecLow.anchor = 1;
               filterDecHigh.taps = new float[] { -0.125f, -0.25f, 0.75f, -0.25f, -0.125f };
               filterDecHigh.anchor = 1;
               filterRecLow.taps = new float[] { -0.25f, 0.5f, 1.5f, 0.5f, -0.25f };
               filterRecLow.anchor = 2;
               filterRecHigh.taps = new float[] { -0.5f, 1.0f, -0.5f };
               filterRecHigh.anchor = 2;
               break;
            case WtType.WT_QUADSPLINE:
               type = WtType.WT_QUADSPLINE;
               filterDecLow.taps = new float[] { 0.125f, 0.375f, 0.375f, 0.125f };
               filterDecLow.anchor = 1;
               filterDecHigh.taps = new float[] {
                  4.68750000e-002f, 1.40625000e-001f, -1.09375000e-001f, -7.03125000e-001f,
                  7.03125000e-001f, 1.09375000e-001f, -1.40625000e-001f, -4.68750000e-002f
               };
               filterDecHigh.anchor = 3;
               filterRecLow.taps = new float[] {
                  9.37500000e-002f, -2.81250000e-001f, -2.18750000e-001f, 1.40625000e+000f,
                  1.40625000e+000f, -2.18750000e-001f, -2.81250000e-001f, 9.37500000e-002f
               };
               filterRecLow.anchor = 4;
               filterRecHigh.taps = new float[] { -0.25f, 0.75f, -0.75f, 0.25f };
               filterRecHigh.anchor = 2;
               break;
            case WtType.WT_VAIDYANATHAN:
               type = WtType.WT_VAIDYANATHAN;
               filterDecLow.taps = new float[] {
                  -4.44813429e-005f,  2.42984451e-004f, -3.20995805e-004f,
                  -6.68143175e-004f,  2.01089470e-003f,  5.00728833e-004f,
                  -6.25018999e-003f,  2.23010665e-003f,  1.39209633e-002f,
                  -1.05029739e-002f, -2.50813595e-002f,  2.73951689e-002f,
                   3.95219827e-002f, -5.49490921e-002f, -5.93466835e-002f,
                   9.33180571e-002f,  9.55189734e-002f, -1.37497248e-001f,
                  -1.86318962e-001f,  1.42561327e-001f,  4.49437821e-001f,
                   4.05029205e-001f,  1.76906895e-001f,  3.23850199e-002f
               };
               filterDecLow.anchor = 20;
               filterDecHigh.taps = new float[] {
                   3.23850199e-002f, -1.76906895e-001f,  4.05029205e-001f,
                  -4.49437821e-001f,  1.42561327e-001f,  1.86318962e-001f,
                  -1.37497248e-001f, -9.55189734e-002f,  9.33180571e-002f,
                   5.93466835e-002f, -5.49490921e-002f, -3.95219827e-002f,
                   2.73951689e-002f,  2.50813595e-002f, -1.05029739e-002f,
                  -1.39209633e-002f,  2.23010665e-003f,  6.25018999e-003f,
                   5.00728833e-004f, -2.01089470e-003f, -6.68143175e-004f,
                   3.20995805e-004f,  2.42984451e-004f,  4.44813429e-005f
               };
               filterDecHigh.anchor = 2;
               filterRecLow.taps = new float[] {
                   6.47700392e-002f,  3.53813788e-001f,  8.10058404e-001f,
                   8.98875635e-001f,  2.85122652e-001f, -3.72637922e-001f,
                  -2.74994493e-001f,  1.91037945e-001f,  1.86636113e-001f,
                  -1.18693366e-001f, -1.09898183e-001f,  7.90439647e-002f,
                   5.47903374e-002f, -5.01627186e-002f, -2.10059475e-002f,
                   2.78419264e-002f,  4.46021326e-003f, -1.25003799e-002f,
                   1.00145766e-003f,  4.02178937e-003f, -1.33628634e-003f,
                  -6.41991606e-004f,  4.85968898e-004f, -8.89626851e-005f
               };
               filterRecLow.anchor = 3;
               filterRecHigh.taps = new float[] {
                   8.89626851e-005f,  4.85968898e-004f,  6.41991606e-004f,
                  -1.33628634e-003f, -4.02178937e-003f,  1.00145766e-003f,
                   1.25003799e-002f,  4.46021326e-003f, -2.78419264e-002f,
                  -2.10059475e-002f,  5.01627186e-002f,  5.47903374e-002f,
                  -7.90439647e-002f, -1.09898183e-001f,  1.18693366e-001f,
                   1.86636113e-001f, -1.91037945e-001f, -2.74994493e-001f,
                   3.72637922e-001f,  2.85122652e-001f, -8.98875635e-001f,
                   8.10058404e-001f, -3.53813788e-001f,  6.47700392e-002f
               };
               filterRecHigh.anchor = 21;
               break;
            default:
               break;
         }
      }
   }

   public partial class MainForm : Form
   {
      private WtBank bank;

      public MainForm()
      {
         InitializeComponent();
         imgSrc.Image = new Bitmap(150, 100, PixelFormat.Format24bppRgb);
         imgDst.Image = new Bitmap(150, 100, PixelFormat.Format24bppRgb);
         imgA.Image = new Bitmap(75, 50, PixelFormat.Format24bppRgb);
         imgD.Image = new Bitmap(75, 50, PixelFormat.Format24bppRgb);
         imgH.Image = new Bitmap(75, 50, PixelFormat.Format24bppRgb);
         imgV.Image = new Bitmap(75, 50, PixelFormat.Format24bppRgb);
         bank.Init(WtType.WT_HAAR);
         OpenFile("..\\application\\wavelet\\data\\501.jpg");
         cbxType.SelectedIndex = 0;
      }

      protected override void OnPaint(PaintEventArgs e)
      {
         Graphics g = e.Graphics;
         Pen pen1, pen2;
         pen1 = new Pen(SystemColors.ButtonHighlight);
         pen2 = new Pen(SystemColors.ButtonShadow);
         int x1 = panel1.Left + panel1.Width / 2;
         int y1 = panel3.Top + imgA.Top + imgA.Height / 2;
         int x2 = panel3.Left;
         int y2 = panel1.Top;
         int y3 = panel1.Bottom;
         int y4 = panel3.Top + imgD.Top + imgD.Height / 2;
         int x3 = panel3.Right;
         int x4 = panel2.Left + panel2.Width / 2;
         int x5 = panel1.Right;
         int x6 = panel2.Left;
         int y5 = panel3.Top + imgH.Top + imgH.Height / 2;
         int y6 = panel3.Top + imgV.Top + imgV.Height / 2;

         g.DrawLine(pen1, new Point(x1, y1), new Point(x1, y2));
         g.DrawLine(pen2, new Point(x1 + 1, y1), new Point(x1 + 1, y2));

         g.DrawLine(pen1, new Point(x1, y1), new Point(x2, y1));
         g.DrawLine(pen2, new Point(x1, y1 + 1), new Point(x2, y1 + 1));

         g.DrawLine(pen1, new Point(x1, y3), new Point(x1, y4));
         g.DrawLine(pen2, new Point(x1 + 1, y3), new Point(x1 + 1, y4));

         g.DrawLine(pen1, new Point(x1, y4), new Point(x2, y4));
         g.DrawLine(pen2, new Point(x1, y4 + 1), new Point(x2, y4 + 1));

         g.DrawLine(pen1, new Point(x3, y4), new Point(x4, y4));
         g.DrawLine(pen2, new Point(x3, y4 + 1), new Point(x4, y4 + 1));

         g.DrawLine(pen1, new Point(x4, y4), new Point(x4, y3));
         g.DrawLine(pen2, new Point(x4 + 1, y4 + 1), new Point(x4 + 1, y3));

         g.DrawLine(pen1, new Point(x4, y1), new Point(x4, y2));
         g.DrawLine(pen2, new Point(x4 + 1, y1), new Point(x4 + 1, y2));

         g.DrawLine(pen1, new Point(x3, y1), new Point(x4, y1));
         g.DrawLine(pen2, new Point(x3, y1 + 1), new Point(x4, y1 + 1));

         g.DrawLine(pen1, new Point(x5, y5), new Point(x2, y5));
         g.DrawLine(pen2, new Point(x5, y5 + 1), new Point(x2, y5 + 1));

         g.DrawLine(pen1, new Point(x5, y6), new Point(x2, y6));
         g.DrawLine(pen2, new Point(x5, y6 + 1), new Point(x2, y6 + 1));

         g.DrawLine(pen1, new Point(x6, y5), new Point(x3, y5));
         g.DrawLine(pen2, new Point(x6, y5 + 1), new Point(x3, y5 + 1));

         g.DrawLine(pen1, new Point(x6, y6), new Point(x3, y6));
         g.DrawLine(pen2, new Point(x6, y6 + 1), new Point(x3, y6 + 1));
      }

      private void eToolStripMenuItem_Click(object sender, EventArgs e)
      {
         Close();
      }

      // Not implemented in IPP yet
      unsafe IppStatus ippiCopyWrapBorder_32f_C3IR(float* pSrc, int srcDstStep,
         IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth)
      {
         float[] Tmp = new float[dstRoiSize.width * dstRoiSize.height];
         int srcOffset = topBorderHeight * (srcDstStep / sizeof(float)) + leftBorderWidth * 3;
         int tmpStep = dstRoiSize.width * sizeof(float);
         int tmpOffset = topBorderHeight * (tmpStep / sizeof(float)) + leftBorderWidth;
         fixed (float* pTmp = Tmp)
         {
            float* p = pSrc - srcOffset;
            ipp.ip.ippiCopy_32f_C3C1R(p, srcDstStep, pTmp, tmpStep, dstRoiSize);
            ipp.ip.ippiCopyWrapBorder_32f_C1IR(pTmp + tmpOffset, tmpStep, srcRoiSize, dstRoiSize, topBorderHeight, leftBorderWidth);
            ipp.ip.ippiCopy_32f_C1C3R(pTmp, tmpStep, p, srcDstStep, dstRoiSize);
            ipp.ip.ippiCopy_32f_C3C1R(p+1, srcDstStep, pTmp, tmpStep, dstRoiSize);
            ipp.ip.ippiCopyWrapBorder_32f_C1IR(pTmp + tmpOffset, tmpStep, srcRoiSize, dstRoiSize, topBorderHeight, leftBorderWidth);
            ipp.ip.ippiCopy_32f_C1C3R(pTmp, tmpStep, p+1, srcDstStep, dstRoiSize);
            ipp.ip.ippiCopy_32f_C3C1R(p+2, srcDstStep, pTmp, tmpStep, dstRoiSize);
            ipp.ip.ippiCopyWrapBorder_32f_C1IR(pTmp + tmpOffset, tmpStep, srcRoiSize, dstRoiSize, topBorderHeight, leftBorderWidth);
            ipp.ip.ippiCopy_32f_C1C3R(pTmp, tmpStep, p+2, srcDstStep, dstRoiSize);
         }
         return IppStatus.ippStsNoErr;
      }

      unsafe private void Recalc()
      {
         int leftBorderLow = bank.filterDecLow.taps.Length - 1 - bank.filterDecLow.anchor;
         if (leftBorderLow < 0) leftBorderLow = 0;
         int leftBorderHigh = bank.filterDecHigh.taps.Length - 1 - bank.filterDecHigh.anchor;
         if (leftBorderHigh < 0) leftBorderHigh = 0;
         int rightBorderLow = bank.filterDecLow.taps.Length - 2 - leftBorderLow;
         if (rightBorderLow < 0) rightBorderLow = 0;
         int rightBorderHigh = bank.filterDecHigh.taps.Length - 2 - leftBorderHigh;
         if (rightBorderHigh < 0) rightBorderHigh = 0;
         int leftTopBorder = Math.Max(leftBorderLow, leftBorderHigh);
         int rightBottomBorder = Math.Max(rightBorderLow, rightBorderHigh);
         int width = imgSrc.Image.Width + leftTopBorder + rightBottomBorder;
         int height = imgSrc.Image.Height + leftTopBorder + rightBottomBorder;
         // Create the C3 source image area extended by border pixels
         float[] extSrcImg = new float[(width * 3) * height];
         int extSrcStep = width * 3 * sizeof(float);
         int offset = leftTopBorder * (extSrcStep / sizeof(float))+ leftTopBorder * 3;
         ipp.IppiSize roiSize = new ipp.IppiSize(imgSrc.Image.Width, imgSrc.Image.Height);
         ipp.IppiSize roiExtSize = new ipp.IppiSize(width, height);
         BitmapData bmpsrcdata = ((Bitmap)(imgSrc.Image)).LockBits(new Rectangle(0, 0, imgSrc.Image.Width, imgSrc.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         byte* pSrc = (byte*)bmpsrcdata.Scan0;
         fixed (float* pExtSrc = extSrcImg)
         {
            ipp.ip.ippiConvert_8u32f_C3R(pSrc, bmpsrcdata.Stride, pExtSrc + offset, extSrcStep, roiSize);
            ippiCopyWrapBorder_32f_C3IR(pExtSrc + offset, extSrcStep, roiSize, roiExtSize, leftTopBorder, leftTopBorder);
         }
         ((Bitmap)(imgSrc.Image)).UnlockBits(bmpsrcdata);

         leftBorderLow = (bank.filterRecLow.taps.Length - 1 - bank.filterRecLow.anchor) / 2;
         if (leftBorderLow < 0) leftBorderLow = 0;
         leftBorderHigh = (bank.filterRecHigh.taps.Length - 1 - bank.filterRecHigh.anchor) / 2;
         if (leftBorderHigh < 0) leftBorderHigh = 0;
         rightBorderLow = (bank.filterRecLow.anchor + 1) / 2;
         rightBorderHigh = (bank.filterRecHigh.anchor + 1) / 2;
         int aLeftBorder = leftBorderLow;
         int aRightBorder = rightBorderLow;
         int aTopBorder = leftBorderLow;
         int aBottomBorder = rightBorderLow;
         int hLeftBorder = leftBorderLow;
         int hRightBorder = rightBorderLow;
         int hTopBorder = leftBorderHigh;
         int hBottomBorder = rightBorderHigh;
         int vLeftBorder = leftBorderHigh;
         int vRightBorder = rightBorderHigh;
         int vTopBorder = leftBorderLow;
         int vBottomBorder = rightBorderLow;
         int dLeftBorder = leftBorderHigh;
         int dRightBorder = rightBorderHigh;
         int dTopBorder = leftBorderHigh;
         int dBottomBorder = rightBorderHigh;

         // Create the C3 approximation image area extended by border pixels
         width = imgA.Image.Width + aLeftBorder + aRightBorder + 1;
         height = imgA.Image.Height + aTopBorder + aBottomBorder + 1;
         float[] aImg = new float[(width * 3) * height];
         int aStep = width * 3 * sizeof(float);
         int aOffset = aTopBorder * (aStep / sizeof(float)) + aLeftBorder * 3;
         ipp.IppiSize aRoi = new ipp.IppiSize(width, height);
         // Create the C3 horizontal detail image area extended by border pixels
         width = imgH.Image.Width + hLeftBorder + hRightBorder + 1;
         height = imgH.Image.Height + hTopBorder + hBottomBorder + 1;
         float[] hImg = new float[(width * 3) * height];
         int hStep = width * 3 * sizeof(float);
         int hOffset = hTopBorder * (hStep / sizeof(float)) + hLeftBorder * 3;
         ipp.IppiSize hRoi = new ipp.IppiSize(width, height);
         // Create the C3 vertical detail image area extended by border pixels
         width = imgV.Image.Width + vLeftBorder + vRightBorder + 1;
         height = imgV.Image.Height + vTopBorder + vBottomBorder + 1;
         float[] vImg = new float[(width * 3) * height];
         int vStep = width * 3 * sizeof(float);
         int vOffset = vTopBorder * (vStep / sizeof(float)) + vLeftBorder * 3;
         ipp.IppiSize vRoi = new ipp.IppiSize(width, height);
         // Create the C3 diagonal detail image area extended by border pixels
         width = imgD.Image.Width + dLeftBorder + dRightBorder + 1;
         height = imgD.Image.Height + dTopBorder + dBottomBorder + 1;
         float[] dImg = new float[(width * 3) * height];
         int dStep = width * 3 * sizeof(float);
         int dOffset = dTopBorder * (dStep / sizeof(float)) + dLeftBorder * 3;
         ipp.IppiSize dRoi = new ipp.IppiSize(width, height);

         // Wavelet decomposition
         ipp.IppiWTFwdSpec_32f_C3R* pSpecFwd = null;
         fixed (float* pDecLow = bank.filterDecLow.taps, pDecHigh = bank.filterDecHigh.taps)
         {
            ipp.ip.ippiWTFwdInitAlloc_32f_C3R(&pSpecFwd, pDecLow, bank.filterDecLow.taps.Length, bank.filterDecLow.anchor,
               pDecHigh, bank.filterDecHigh.taps.Length, bank.filterDecHigh.anchor);
         }
         int bufSize;
         ipp.ip.ippiWTFwdGetBufSize_C3R(pSpecFwd, &bufSize);
         byte[] BufFwd = new byte[bufSize];
         ipp.IppiSize dstRoiSize = new ipp.IppiSize(imgA.Image.Width, imgA.Image.Height);
         fixed (float* pExtSrc = extSrcImg, pAImg = aImg, pHImg = hImg, pVImg = vImg, pDImg = dImg)
         {
            fixed (byte* pBufFwd = BufFwd)
            {
               ipp.ip.ippiWTFwd_32f_C3R(pExtSrc + offset, extSrcStep,
                  pAImg + aOffset, aStep, pHImg + hOffset, hStep, pVImg + vOffset, vStep, pDImg + dOffset, dStep,
                  dstRoiSize, pSpecFwd, pBufFwd);
            }
         }
         ipp.ip.ippiWTFwdFree_32f_C3R(pSpecFwd);

         float[] zero = { 0, 0, 0 };
         BitmapData bmpData = ((Bitmap)(imgA.Image)).LockBits(new Rectangle(0, 0, imgA.Image.Width, imgA.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         byte* pImg = (byte*)bmpData.Scan0;
         fixed (float* pAImg = aImg, pZero = zero)
         {
            ipp.ip.ippiConvert_32f8u_C3R(pAImg + aOffset, aStep, pImg, bmpData.Stride, dstRoiSize, ipp.IppRoundMode.ippRndNear);
            if (!cbA.Checked)
            {
               ipp.ip.ippiSet_32f_C3R(pZero, pAImg, aStep, aRoi);
            }
            else
            {
               ippiCopyWrapBorder_32f_C3IR(pAImg + aOffset, aStep, dstRoiSize, aRoi, aTopBorder, aLeftBorder);
            }
         }
         ((Bitmap)(imgA.Image)).UnlockBits(bmpData);

         float[] min = { 0, 0, 0 };
         float[] max = { 0, 0, 0 };
         bmpData = ((Bitmap)(imgH.Image)).LockBits(new Rectangle(0, 0, imgH.Image.Width, imgH.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         pImg = (byte*)bmpData.Scan0;
         fixed (float* pHImg = hImg, pMin = min, pMax = max, pZero = zero)
         {
            ipp.ip.ippiMinMax_32f_C3R(pHImg + hOffset, hStep, dstRoiSize, pMin, pMax);
            ipp.ip.ippiScale_32f8u_C3R(pHImg + hOffset, hStep, pImg, bmpData.Stride, dstRoiSize,
               Math.Max(min[0], Math.Min(min[1], min[2])), Math.Max(max[0], Math.Max(max[1], max[2])));
            if (!cbH.Checked)
            {
               ipp.ip.ippiSet_32f_C3R(pZero, pHImg, hStep, hRoi);
            }
            else
            {
               ippiCopyWrapBorder_32f_C3IR(pHImg + hOffset, hStep, dstRoiSize, hRoi, hTopBorder, hLeftBorder);
            }
         }
         ((Bitmap)(imgH.Image)).UnlockBits(bmpData);

         bmpData = ((Bitmap)(imgV.Image)).LockBits(new Rectangle(0, 0, imgV.Image.Width, imgV.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         pImg = (byte*)bmpData.Scan0;
         fixed (float* pVImg = vImg, pMin = min, pMax = max, pZero = zero)
         {
            ipp.ip.ippiMinMax_32f_C3R(pVImg + vOffset, vStep, dstRoiSize, pMin, pMax);
            ipp.ip.ippiScale_32f8u_C3R(pVImg + vOffset, vStep, pImg, bmpData.Stride, dstRoiSize,
               Math.Max(min[0], Math.Min(min[1], min[2])), Math.Max(max[0], Math.Max(max[1], max[2])));
            if (!cbV.Checked)
            {
               ipp.ip.ippiSet_32f_C3R(pZero, pVImg, vStep, vRoi);
            }
            else
            {
               ippiCopyWrapBorder_32f_C3IR(pVImg + vOffset, vStep, dstRoiSize, vRoi, vTopBorder, vLeftBorder);
            }
         }
         ((Bitmap)(imgV.Image)).UnlockBits(bmpData);

         bmpData = ((Bitmap)(imgD.Image)).LockBits(new Rectangle(0, 0, imgD.Image.Width, imgD.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         pImg = (byte*)bmpData.Scan0;
         fixed (float* pDImg = dImg, pMin = min, pMax = max, pZero = zero)
         {
            ipp.ip.ippiMinMax_32f_C3R(pDImg + dOffset, dStep, dstRoiSize, pMin, pMax);
            ipp.ip.ippiScale_32f8u_C3R(pDImg + dOffset, dStep, pImg, bmpData.Stride, dstRoiSize,
               Math.Max(min[0], Math.Min(min[1], min[2])), Math.Max(max[0], Math.Max(max[1], max[2])));
            if (!cbD.Checked)
            {
               ipp.ip.ippiSet_32f_C3R(pZero, pDImg, dStep, dRoi);
            }
            else
            {
               ippiCopyWrapBorder_32f_C3IR(pDImg + dOffset, dStep, dstRoiSize, dRoi, dTopBorder, dLeftBorder);
            }
         }
         ((Bitmap)(imgD.Image)).UnlockBits(bmpData);

         // one-level wavelet reconstruction
         ipp.IppiWTInvSpec_32f_C3R* pSpecInv = null;
         fixed (float* pRecLow = bank.filterRecLow.taps, pRecHigh = bank.filterRecHigh.taps)
         {
            ipp.ip.ippiWTInvInitAlloc_32f_C3R(&pSpecInv, pRecLow, bank.filterRecLow.taps.Length, bank.filterRecLow.anchor,
               pRecHigh, bank.filterRecHigh.taps.Length, bank.filterRecHigh.anchor);
         }
         ipp.ip.ippiWTInvGetBufSize_C3R(pSpecInv, &bufSize);
         byte[] BufInv = new byte[bufSize];
         float[] Dst = new float[(imgSrc.Image.Width * 3) * imgSrc.Image.Height];
         int dstStep = (imgSrc.Image.Width * 3) * sizeof(float);

         fixed (float* pDst = Dst, pAImg = aImg, pHImg = hImg, pVImg = vImg, pDImg = dImg)
         {
            fixed (byte* pBufInv = BufInv)
            {
               ipp.ip.ippiWTInv_32f_C3R(
                  pAImg + aOffset, aStep,
                  pHImg + hOffset, hStep,
                  pVImg + vOffset, vStep,
                  pDImg + dOffset, dStep,
                  dstRoiSize, pDst, dstStep, pSpecInv, pBufInv);
            }
         }
         ipp.ip.ippiWTInvFree_32f_C3R(pSpecInv);

         bmpData = ((Bitmap)(imgDst.Image)).LockBits(new Rectangle(0, 0, imgDst.Image.Width, imgDst.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         pImg = (byte*)bmpData.Scan0;
         fixed (float* pDst = Dst)
         {
            ipp.ip.ippiConvert_32f8u_C3R(pDst, dstStep, pImg, bmpData.Stride, roiSize, ipp.IppRoundMode.ippRndNear);
         }
         ((Bitmap)(imgDst.Image)).UnlockBits(bmpData);

         // calculate norms
         BitmapData srcData = ((Bitmap)(imgSrc.Image)).LockBits(new Rectangle(0, 0, imgSrc.Image.Width, imgSrc.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         byte* pS = (byte*)srcData.Scan0;
         BitmapData dstData = ((Bitmap)(imgDst.Image)).LockBits(new Rectangle(0, 0, imgDst.Image.Width, imgDst.Image.Height),
            ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
         byte* pD = (byte*)dstData.Scan0;
         double[] Norm = { 0, 0, 0 };
         fixed (double* pNorm = Norm)
         {
            ipp.ip.ippiNormDiff_Inf_8u_C3R(pS, srcData.Stride, pD, dstData.Stride, roiSize, pNorm);
            lblCB.Text = Norm[0].ToString();
            lblCG.Text = Norm[1].ToString();
            lblCR.Text = Norm[2].ToString();
            ipp.ip.ippiNormDiff_L1_8u_C3R(pS, srcData.Stride, pD, dstData.Stride, roiSize, pNorm);
            lblL1B.Text = Norm[0].ToString();
            lblL1G.Text = Norm[1].ToString();
            lblL1R.Text = Norm[2].ToString();
            ipp.ip.ippiNormDiff_L2_8u_C3R(pS, srcData.Stride, pD, dstData.Stride, roiSize, pNorm);
            lblL2B.Text = Norm[0].ToString("f2");
            lblL2G.Text = Norm[1].ToString("f2");
            lblL2R.Text = Norm[2].ToString("f2");
            ipp.ip.ippiNormRel_Inf_8u_C3R(pD, dstData.Stride, pS, srcData.Stride, roiSize, pNorm);
            lblRCB.Text = Norm[0].ToString("f2");
            lblRCG.Text = Norm[1].ToString("f2");
            lblRCR.Text = Norm[2].ToString("f2");
            ipp.ip.ippiNormRel_L1_8u_C3R(pD, dstData.Stride, pS, srcData.Stride, roiSize, pNorm);
            lblRL1B.Text = Norm[0].ToString("f2");
            lblRL1G.Text = Norm[1].ToString("f2");
            lblRL1R.Text = Norm[2].ToString("f2");
            ipp.ip.ippiNormRel_L2_8u_C3R(pD, dstData.Stride, pS, srcData.Stride, roiSize, pNorm);
            lblRL2B.Text = Norm[0].ToString("f2");
            lblRL2G.Text = Norm[1].ToString("f2");
            lblRL2R.Text = Norm[2].ToString("f2");
         }
         ((Bitmap)(imgSrc.Image)).UnlockBits(srcData);
         ((Bitmap)(imgDst.Image)).UnlockBits(dstData);
      }

      private void openToolStripMenuItem_Click(object sender, EventArgs e)
      {
         if (DialogResult.OK == openFileDialog.ShowDialog())
         {
            openFileDialog.InitialDirectory = Path.GetDirectoryName(openFileDialog.FileName);
            OpenFile(openFileDialog.FileName);
            Recalc();
            Invalidate(true);
         }
      }

      unsafe private void OpenFile(string filename)
      {
         try
         {
            Bitmap bigImage = new Bitmap(filename);
            if (imgSrc.Image.Width == bigImage.Width && imgSrc.Image.Height == bigImage.Height)
            {
               imgSrc.Image = bigImage;
            }
            else
            {
               BitmapData bmpsrcdata = bigImage.LockBits(new Rectangle(0, 0, bigImage.Width, bigImage.Height),
                  ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
               if (null == imgSrc.Image)
               {
                  imgSrc.Image = new Bitmap(150, 100, PixelFormat.Format24bppRgb);
               }
               BitmapData bmpdstdata = ((Bitmap)(imgSrc.Image)).LockBits(new Rectangle(0, 0, imgSrc.Image.Width, imgSrc.Image.Height),
                  ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
               byte* pSrc = (byte*)bmpsrcdata.Scan0;
               ipp.IppiSize srcSize = new ipp.IppiSize(bmpsrcdata.Width, bmpsrcdata.Height);
               ipp.IppiRect srcROI = new ipp.IppiRect(0, 0, bmpsrcdata.Width, bmpsrcdata.Height);
               byte* pDst = (byte*)bmpdstdata.Scan0;
               ipp.IppiSize dstRoiSize = new ipp.IppiSize(bmpdstdata.Width, bmpdstdata.Height);
               ipp.IppiRect dstROI = new ipp.IppiRect(0, 0, bmpdstdata.Width, bmpdstdata.Height);
               int bufSize;
               ipp.ip.ippiResizeGetBufSize(srcROI, dstROI, 3, 4, &bufSize);
               byte[] buffer = new byte[bufSize];
               fixed (byte* pBuffer = buffer)
               {
                   ipp.ip.ippiResizeSqrPixel_8u_C3R(pSrc, srcSize, bmpsrcdata.Stride, srcROI, pDst, bmpdstdata.Stride, dstROI,
                       (float)imgSrc.Image.Width / bigImage.Width, (float)imgSrc.Image.Height / bigImage.Height, 0, 0, 4, pBuffer);
               }
               ((Bitmap)(imgSrc.Image)).UnlockBits(bmpdstdata);
               bigImage.UnlockBits(bmpsrcdata);
            }
         }
         catch
         {
         }
      }

      private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
      {
         AboutForm about = new AboutForm();
         about.ShowDialog();
      }

      private void cb_CheckedChanged(object sender, EventArgs e)
      {
         Recalc();
         Invalidate(true);
      }

      private void cbxType_SelectedIndexChanged(object sender, EventArgs e)
      {
         bank.Init((WtType)(cbxType.SelectedIndex));
         Recalc();
         Invalidate(true);
      }

   }
}