/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
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

// generated automatically on Fri Jul  9 10:31:55 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppiDitherType {
      ippDitherNone = 0,
      ippDitherFS = 1,
      ippDitherJJN = 2,
      ippDitherStucki = 3,
      ippDitherBayer = 4,
   };
   public enum IppiBayerGrid {
      ippiBayerBGGR = 0,
      ippiBayerRGGB = 1,
      ippiBayerGBRG = 2,
      ippiBayerGRBG = 3,
   };
//
// hidden or own structures
//

unsafe public class cc {

   internal const string libname = "ippcc-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname,EntryPoint="ippccGetLibVersion")] public static extern
int* xippccGetLibVersion (    );
public static IppLibraryVersion ippccGetLibVersion() {
   return new IppLibraryVersion( xippccGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYCbCr411_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYCbCr420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYCbCr422_16u8u_C3C2R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYCbCr422_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYCrCb420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR555ToYUV420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToBGR_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYCbCr411_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYCbCr420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYCbCr422_16u8u_C3C2R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYCbCr422_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYCrCb420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGR565ToYUV420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToBGR565_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToCbYCr422_709HDTV_8u_AC4C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToCbYCr422_709HDTV_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToCbYCr422_8u_AC4C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_AC4P4R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_AP4C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_AP4R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToHLS_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToLab_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToLab_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr411_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr411_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_709CSC_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_709CSC_8u_C3P2R (  byte *pSrc, int srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_709CSC_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_709HDTV_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr420_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr422_8u_AC4C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr422_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr422_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCbCr422_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCoCg_8u16s_C3P3R (  byte *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCoCg_8u16s_C4P3R (  byte *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCoCg_Rev_8u16s_C3P3R (  byte *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCoCg_Rev_8u16s_C4P3R (  byte *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCrCb420_709CSC_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCrCb420_709CSC_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCrCb420_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYCrCb420_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiBGRToYUV420_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCFAToRGB_16u_C1C3R (  ushort *pSrc, IppiRect srcRoi, IppiSize srcSize, int srcStep, ushort *pDst, int dstStep, IppiBayerGrid grid, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCFAToRGB_8u_C1C3R (  byte *pSrc, IppiRect srcRoi, IppiSize srcSize, int srcStep, byte *pDst, int dstStep, IppiBayerGrid grid, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToBGR_709HDTV_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToBGR_709HDTV_8u_C2C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToBGR_8u_C2C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToRGB_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr411_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr420_8u_C2P2R (  byte *pSrc, int srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr420_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr420_Interlace_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr422_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr422_8u_C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiCbYCr422ToYCrCb420_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_16s_AC4C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_16s_C3C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_16u_AC4C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_16u_C3C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_32f_AC4C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_32f_C3C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_64f_AC4C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize roiSize, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_64f_C3C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize roiSize, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_8u_AC4C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorToGray_8u_C3C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_IP3R (  short **pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16s_P3R (  short **pSrc, int srcStep, short **pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_IP3R (  ushort **pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_16u_P3R (  ushort **pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_AC4IR (  sbyte *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_AC4R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_C3IR (  sbyte *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_C3R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_IP3R (  sbyte **pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8s_P3R (  sbyte **pSrc, int srcStep, sbyte **pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_IP3R (  byte **pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist32f_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_IP3R (  float **pSrcDst, int srcDstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiColorTwist_32f_P3R (  float **pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize, float *twist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiDemosaicAHD_16u_C1C3R (  ushort *pSrc, IppiRect srcRoi, IppiSize srcSize, int srcStep, ushort *pDst, int dstStep, IppiBayerGrid grid, ushort *pTmp, int tmpStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiDemosaicAHD_8u_C1C3R (  byte *pSrc, IppiRect srcRoi, IppiSize srcSize, int srcStep, byte *pDst, int dstStep, IppiBayerGrid grid, byte *pTmp, int tmpStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_IP3R (  ushort **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_16u_P3R (  ushort **pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_IP3R (  float **pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_32f_P3R (  float **pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_IP3R (  byte **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaFwd_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_IP3R (  ushort **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_16u_P3R (  ushort **pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_IP3R (  float **pSrcDst, int srcDstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_32f_P3R (  float **pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_IP3R (  byte **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiGammaInv_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_AC4P4R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_AP4C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_AP4R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToBGR_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHLSToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHSVToRGB_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHSVToRGB_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHSVToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiHSVToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLUVToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLabToBGR_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiLabToBGR_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGB565ToRGB_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGB565ToYUV420_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGB565ToYUV422_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToCbYCr422Gamma_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToCbYCr422_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_16s_AC4C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_16s_C3C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_16u_AC4C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_16u_C3C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_32f_AC4C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_32f_C3C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_8u_AC4C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToGray_8u_C3C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHLS_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHSV_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHSV_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHSV_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToHSV_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToLUV_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToRGB565_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToXYZ_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCC_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr420_8u_C3P2R (  byte *pRGB, int rgbStep, byte *pY, int YStep, byte *pCbCr, int CbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr420_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr420_8u_C4P2R (  byte *pRGB, int rgbStep, byte *pY, int YStep, byte *pCbCr, int CbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr422_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr422_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr422_8u_P3C2R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCbCr_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCoCg_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCrCb420_8u_AC4P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCrCb422_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYCrCb422_8u_P3C2R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV420_8u_C3P3 (  byte *pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV420_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV420_8u_P3 (  byte **pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV420_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV422_8u_C3C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV422_8u_C3P3 (  byte *pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV422_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV422_8u_P3 (  byte **pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV422_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV_8u_AC4P4R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiRGBToYUV_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s8u_AC4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s8u_C1R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s8u_C3R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s8u_C4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u8u_AC4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u8u_C1R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u8u_C4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16s_AC4R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16s_C1R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16s_C3R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16s_C4R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16u_AC4R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16u_C1R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16u_C3R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f16u_C4R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f8u_AC4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f8u_C1R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f8u_C3R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_32f8u_C4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_8u1u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dstBitOffset, IppiSize roiSize, int noise, int seed, IppiDitherType dtype, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiReduceBits_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int noise, IppiDitherType dtype, int levels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_16s32s_C3P3R (  short *pBGR, int bgrStep, int **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_16s32s_C4P3R (  short *pBGR, int bgrStep, int **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_16s_C3P3R (  short *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_16s_C4P3R (  short *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_Rev_16s32s_C3P3R (  short *pBGR, int bgrStep, int **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_Rev_16s32s_C4P3R (  short *pBGR, int bgrStep, int **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_Rev_16s_C3P3R (  short *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiSBGRToYCoCg_Rev_16s_C4P3R (  short *pBGR, int bgrStep, short **pYCC, int yccStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiToneMapLinear_32f8u_C1R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiToneMapMean_32f8u_C1R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiXYZToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCCToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr1620To420_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411To420_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToBGR555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToBGR565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToBGR_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToBGR_8u_P3C4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr420_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr420_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr420_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr422_8u_P2C2R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr422_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCbCr422_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCrCb420_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCrCb422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411ToYCrCb422_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr411_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420To1620_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420To411_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420To422_Interlace_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR_709CSC_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR_709HDTV_8u_P3C4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToBGR_8u_P3C4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToCbYCr422_8u_P2C2R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToCbYCr422_Interlace_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB_8u_P2C3R (  byte *pY, int pYStep, byte *pCbCr, int CbCrStep, byte *pRGB, int rgbStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB_8u_P2C4R (  byte *pY, int pYStep, byte *pCbCr, int CbCrStep, byte *pRGB, int rgbStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToRGB_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr411_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr411_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_8u_P2C2R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_Filter_8u_P2C2R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte *pDst, int dstStep, IppiSize roiSize, int layout );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_Filter_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCbCr422_Filter_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCrCb420_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420ToYCrCb420_Filter_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize, int layout );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420_8u_P2P3R (  byte *pSrcY, int srcYStep, byte *pSrcCbCr, int srcCbCrStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr420_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422To420_Interlace_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR444Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR444_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR555Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR555_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR565Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR565_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR_8u_C2C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToBGR_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToCbYCr422_8u_C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToCbYCr422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB444Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB444_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB555Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB555_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB565Dither_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB565_8u16u_C2C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr411_8u_C2P2R (  byte *pSrc, int srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr411_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr411_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr411_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr420_8u_C2P2R (  byte *pSrc, int srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr420_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr420_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCbCr420_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCrCb420_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCrCb422_8u_C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422ToYCrCb422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCr422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR444Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR444Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR444_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR444_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR555Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR555Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR555_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR555_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR565Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR565Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR565_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR565_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR_709CSC_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR_709CSC_8u_P3C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToBGR_8u_P3C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB444Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB444Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB444_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB444_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB555Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB555Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB555_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB555_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB565Dither_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB565Dither_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB565_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB565_8u16u_P3C3R (  byte **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB_8u_P3C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCbCrToRGB_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToBGR_16s8u_P3C3R (  short **pYCC, int yccStep, byte *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToBGR_16s8u_P3C4R (  short **pYCC, int yccStep, byte *pBGR, int bgrStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToBGR_Rev_16s8u_P3C3R (  short **pYCC, int yccStep, byte *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToBGR_Rev_16s8u_P3C4R (  short **pYCC, int yccStep, byte *pBGR, int bgrStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToRGB_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_16s_P3C3R (  short **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_16s_P3C4R (  short **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize, short aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_32s16s_P3C3R (  int **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_32s16s_P3C4R (  int **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize, short aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_Rev_16s_P3C3R (  short **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_Rev_16s_P3C4R (  short **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize, short aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_Rev_32s16s_P3C3R (  int **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCoCgToSBGR_Rev_32s16s_P3C4R (  int **pYCC, int yccStep, short *pBGR, int bgrStep, IppiSize roiSize, short aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToBGR_Filter_8u_P3C4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToCbYCr422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToRGB_8u_P3C4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr411_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr420_8u_P3P2R (  byte **pSrc, int *srcStep, byte *pDstY, int dstYStep, byte *pDstCbCr, int dstCbCrStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_8u_P3C2R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_Filter_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb422ToRGB_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb422ToRGB_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr411_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr420_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_8u_C2P3R (  byte *pSrc, int srcStep, byte **pDst, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToBGR_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB444Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB444_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB555Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB555_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB565Dither_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB565_8u16u_P3C3R (  byte **pSrc, int *srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB_8u_P3 (  byte **pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB_8u_P3AC4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB_8u_P3C3 (  byte **pSrc, byte *pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV420ToRGB_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_P3 (  byte **pSrc, byte **pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_P3AC4R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_P3C3 (  byte **pSrc, byte *pDst, IppiSize imgSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_P3C3R (  byte **pSrc, int *srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUV422ToRGB_8u_P3R (  byte **pSrc, int *srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUVToRGB_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUVToRGB_8u_C3C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUVToRGB_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUVToRGB_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cc.libname)] public static extern
IppStatus ippiYUVToRGB_8u_P3R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );
};
};
