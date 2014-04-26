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

// generated automatically on Fri Jul  9 10:50:19 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppiResizeFilterType {
      ippResizeFilterHann = 0,
      ippResizeFilterLanczos = 1,
   };
   public enum IppiFilterBilateralType {
      ippiFilterBilateralGauss = 100,
      ippiFilterBilateralGaussFast = 101,
   };
   public enum IppiAlphaType {
      ippAlphaOver = 0,
      ippAlphaIn = 1,
      ippAlphaOut = 2,
      ippAlphaATop = 3,
      ippAlphaXor = 4,
      ippAlphaPlus = 5,
      ippAlphaOverPremul = 6,
      ippAlphaInPremul = 7,
      ippAlphaOutPremul = 8,
      ippAlphaATopPremul = 9,
      ippAlphaXorPremul = 10,
      ippAlphaPlusPremul = 11,
   };
   public enum IppiAxis {
      ippAxsHorizontal = 0,
      ippAxsVertical = 1,
      ippAxsBoth = 2,
   };
   public enum IppiResampleType {
      ippResampleCatmullRom = 0,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppiDCTFwdSpec_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDCTInvSpec_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDFTSpec_C_32fc {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDFTSpec_R_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDFTSpec_R_32s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeconvFFTState_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeconvFFTState_32f_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeconvLR_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeconvLR_32f_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiFFTSpec_C_32fc {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiFFTSpec_R_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiFFTSpec_R_32s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiFilterBilateralSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMomentState_64f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMomentState_64s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiResampleRowSpec_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiResizeFilterState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiWTFwdSpec_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiWTFwdSpec_32f_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiWTInvSpec_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiWTInvSpec_32f_C3R {};

unsafe public class ip {

   internal const string libname = "ippi-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAbs_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_AC4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_AC4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C1IRSfs (  short value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C1RSfs (  short *pSrc, int srcStep, short value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C3IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C3RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16s_C4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_AC4IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_AC4RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_C1IRSfs (  Ipp16sc value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_C1RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_C3IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16sc_C3RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_AC4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C1IRSfs (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C3IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_16u_C4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_AC4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_AC4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C1IR (  float value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C1R (  float *pSrc, int srcStep, float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C3IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C3R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32f_C4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_AC4IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_AC4R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_C1IR (  Ipp32fc value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_C3IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32fc_C3R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_AC4IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_AC4RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_C1IRSfs (  Ipp32sc value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_C1RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_C3IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_32sc_C3RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_AC4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C1IRSfs (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C1RSfs (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C3IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C3RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddC_8u_C4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short mean, short stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short mean, short stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short mean, short stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short mean, short stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort mean, ushort stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort mean, ushort stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort mean, ushort stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort mean, ushort stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float mean, float stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float mean, float stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float mean, float stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float mean, float stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte mean, byte stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte mean, byte stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte mean, byte stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandGauss_Direct_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte mean, byte stdev, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short low, short high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short low, short high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short low, short high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short low, short high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort low, ushort high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort low, ushort high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort low, ushort high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort low, ushort high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float low, float high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float low, float high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float low, float high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float low, float high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte low, byte high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte low, byte high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte low, byte high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRandUniform_Direct_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte low, byte high, uint *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAddRotateShift (  double xCenter, double yCenter, double angle, double *xShift, double *yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_AC4IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_AC4RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_C1IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_C1RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_C3IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16sc_C3RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_AC4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_AC4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C1IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C1RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C3IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C3RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_16u_C4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_AC4IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_AC4R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_C1IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_C1R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_C3IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32fc_C3R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_AC4IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_AC4RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_C1IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_C1RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_C3IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_32sc_C3RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_AC4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_AC4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C1IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C1RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C3IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C3RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAdd_8u_C4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16s_C1R (  short *pSrc1, int src1Step, short alpha1, short *pSrc2, int src2Step, short alpha2, short *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16u_AC4R (  ushort *pSrc1, int src1Step, ushort alpha1, ushort *pSrc2, int src2Step, ushort alpha2, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16u_AP4R (  ushort **pSrc1, int src1Step, ushort alpha1, ushort **pSrc2, int src2Step, ushort alpha2, ushort **pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16u_C1R (  ushort *pSrc1, int src1Step, ushort alpha1, ushort *pSrc2, int src2Step, ushort alpha2, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16u_C3R (  ushort *pSrc1, int src1Step, ushort alpha1, ushort *pSrc2, int src2Step, ushort alpha2, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_16u_C4R (  ushort *pSrc1, int src1Step, ushort alpha1, ushort *pSrc2, int src2Step, ushort alpha2, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_32f_C1R (  float *pSrc1, int src1Step, float alpha1, float *pSrc2, int src2Step, float alpha2, float *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_32s_C1R (  int *pSrc1, int src1Step, int alpha1, int *pSrc2, int src2Step, int alpha2, int *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_32u_C1R (  uint *pSrc1, int src1Step, uint alpha1, uint *pSrc2, int src2Step, uint alpha2, uint *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8s_C1R (  sbyte *pSrc1, int src1Step, sbyte alpha1, sbyte *pSrc2, int src2Step, sbyte alpha2, sbyte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8u_AC4R (  byte *pSrc1, int src1Step, byte alpha1, byte *pSrc2, int src2Step, byte alpha2, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8u_AP4R (  byte **pSrc1, int src1Step, byte alpha1, byte **pSrc2, int src2Step, byte alpha2, byte **pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8u_C1R (  byte *pSrc1, int src1Step, byte alpha1, byte *pSrc2, int src2Step, byte alpha2, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8u_C3R (  byte *pSrc1, int src1Step, byte alpha1, byte *pSrc2, int src2Step, byte alpha2, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompC_8u_C4R (  byte *pSrc1, int src1Step, byte alpha1, byte *pSrc2, int src2Step, byte alpha2, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaCompColorKey_8u_AC4R (  byte *pSrc1, int src1Step, byte alpha1, byte *pSrc2, int src2Step, byte alpha2, byte *pDst, int dstStep, IppiSize roiSize, byte *colorKey, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_16s_AC1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_16u_AC1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_16u_AP4R (  ushort **pSrc1, int src1Step, ushort **pSrc2, int src2Step, ushort **pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32f_AC1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32s_AC1R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32s_AC4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32u_AC1R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, uint *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_32u_AC4R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, uint *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_8s_AC1R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, sbyte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_8s_AC4R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, sbyte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_8u_AC1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaComp_8u_AP4R (  byte **pSrc1, int src1Step, byte **pSrc2, int src2Step, byte **pDst, int dstStep, IppiSize roiSize, IppiAlphaType alphaType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_AC4IR (  ushort alpha, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_AC4R (  ushort *pSrc, int srcStep, ushort alpha, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_AP4IR (  ushort alpha, ushort **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_AP4R (  ushort **pSrc, int srcStep, ushort alpha, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C1IR (  ushort alpha, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C1R (  ushort *pSrc, int srcStep, ushort alpha, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C3IR (  ushort alpha, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C3R (  ushort *pSrc, int srcStep, ushort alpha, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C4IR (  ushort alpha, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_16u_C4R (  ushort *pSrc, int srcStep, ushort alpha, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_AC4IR (  byte alpha, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_AC4R (  byte *pSrc, int srcStep, byte alpha, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_AP4IR (  byte alpha, byte **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_AP4R (  byte **pSrc, int srcStep, byte alpha, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C1IR (  byte alpha, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C1R (  byte *pSrc, int srcStep, byte alpha, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C3IR (  byte alpha, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C3R (  byte *pSrc, int srcStep, byte alpha, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C4IR (  byte alpha, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremulC_8u_C4R (  byte *pSrc, int srcStep, byte alpha, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_16u_AP4IR (  ushort **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_16u_AP4R (  ushort **pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_8u_AP4IR (  byte **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAlphaPremul_8u_AP4R (  byte **pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_AC4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_AC4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C1IR (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C1R (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C3IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C3R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_16u_C4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_AC4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_AC4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C1IR (  int value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C1R (  int *pSrc, int srcStep, int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C3IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C3R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_32s_C4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_AC4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_AC4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C1IR (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C1R (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C3IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C3R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAndC_8u_C4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_AC4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_AC4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C1IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C1R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C3IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C3R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_32s_C4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiAnd_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, short colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, short *colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, short *colorkey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, ushort colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, ushort *colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, ushort *colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, byte colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, byte *colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompColorKey_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, byte *colorKey );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16s_AC4R (  short *pSrc, int srcStep, short *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16s_C1R (  short *pSrc, int srcStep, short value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16s_C3R (  short *pSrc, int srcStep, short *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16s_C4R (  short *pSrc, int srcStep, short *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16u_AC4R (  ushort *pSrc, int srcStep, ushort *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16u_C1R (  ushort *pSrc, int srcStep, ushort value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16u_C3R (  ushort *pSrc, int srcStep, ushort *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_16u_C4R (  ushort *pSrc, int srcStep, ushort *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_32f_AC4R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_32f_C1R (  float *pSrc, int srcStep, float value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_32f_C3R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_32f_C4R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_8u_AC4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_8u_C1R (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_8u_C3R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareC_8u_C4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEpsC_32f_AC4R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEpsC_32f_C1R (  float *pSrc, int srcStep, float value, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEpsC_32f_C3R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEpsC_32f_C4R (  float *pSrc, int srcStep, float *value, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEps_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEps_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEps_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompareEqualEps_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCompare_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiComplement_32s_C1IR (  int *pSrcDst, int srcdstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiComputeThreshold_Otsu_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pThreshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_16s_AC4R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_16s_C1R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_16s_C3R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_32f_AC4R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_32f_C1R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_32f_C3R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_8u_AC4R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_8u_C1R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvFull_8u_C3R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_16s_AC4R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_16s_C1R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_16s_C3R (  short *pSrc1, int src1Step, IppiSize src1Size, short *pSrc2, int src2Step, IppiSize src2Size, short *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_32f_AC4R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_32f_C1R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_32f_C3R (  float *pSrc1, int src1Step, IppiSize src1Size, float *pSrc2, int src2Step, IppiSize src2Size, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_8u_AC4R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_8u_C1R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvValid_8u_C3R (  byte *pSrc1, int src1Step, IppiSize src1Size, byte *pSrc2, int src2Step, IppiSize src2Size, byte *pDst, int dstStep, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s16u_C1Rs (  short *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32f_AC4R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32f_C1R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32f_C3R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32f_C4R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32s_AC4R (  short *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32s_C1R (  short *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32s_C3R (  short *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32s_C4R (  short *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s32u_C1Rs (  short *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s8s_C1RSfs (  short *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s8u_AC4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s8u_C1R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s8u_C3R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16s8u_C4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u16s_C1RSfs (  ushort *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32f_AC4R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32f_C1R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32f_C3R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32f_C4R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32s_AC4R (  ushort *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32s_C1R (  ushort *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32s_C3R (  ushort *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32s_C4R (  ushort *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u32u_C1R (  ushort *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u8s_C1RSfs (  ushort *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u8u_AC4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u8u_C1R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_16u8u_C4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_1u8u_C1R (  byte *pSrc, int srcStep, int srcBitOffset, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16s_AC4R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16s_C1R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16s_C1RSfs (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roi, IppRoundMode round, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16s_C3R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16s_C4R (  float *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16u_AC4R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16u_C1R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16u_C1RSfs (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roi, IppRoundMode round, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16u_C3R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f16u_C4R (  float *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f32s_C1RSfs (  float *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roi, IppRoundMode round, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f32u_C1IRSfs (  uint *pSrcDst, int srcDstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f32u_C1RSfs (  float *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8s_AC4R (  float *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8s_C1R (  float *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8s_C1RSfs (  float *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roi, IppRoundMode round, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8s_C3R (  float *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8s_C4R (  float *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8u_AC4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8u_C1R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8u_C1RSfs (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roi, IppRoundMode round, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8u_C3R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32f8u_C4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s16s_C1RSfs (  int *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s16u_C1RSfs (  int *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s32f_C1R (  int *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s32u_C1Rs (  int *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8s_AC4R (  int *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8s_C1R (  int *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8s_C3R (  int *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8s_C4R (  int *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8u_AC4R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8u_C1R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8u_C3R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32s8u_C4R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u16s_C1RSfs (  uint *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u16u_C1RSfs (  uint *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u32f_C1R (  uint *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u32s_C1RSfs (  uint *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u8s_C1RSfs (  uint *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_32u8u_C1RSfs (  uint *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s16u_C1Rs (  sbyte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32f_AC4R (  sbyte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32f_C1R (  sbyte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32f_C3R (  sbyte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32f_C4R (  sbyte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32s_AC4R (  sbyte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32s_C1R (  sbyte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32s_C3R (  sbyte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32s_C4R (  sbyte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s32u_C1Rs (  sbyte *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8s8u_C1Rs (  sbyte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16s_AC4R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16s_C3R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16s_C4R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16u_AC4R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16u_C1R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u16u_C4R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u1u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dstBitOffset, IppiSize roiSize, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32f_AC4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32f_C3R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32f_C4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32s_AC4R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32s_C3R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u32s_C4R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiConvert_8u8s_C1RSfs (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roi, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16s_AC4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16s_C1R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, short value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16s_C3R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16s_C4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, ushort value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16u_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_16u_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32s_AC4R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, int *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32s_C1R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, int value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32s_C3R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, int *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_32s_C4R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, int *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_8u_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, byte value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_8u_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyConstBorder_8u_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyManaged_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int flags );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_AC4IR (  short *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_AC4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C1IR (  short *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C1R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C3IR (  short *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C3R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C4IR (  short *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16s_C4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_AC4IR (  ushort *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C1IR (  ushort *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C3IR (  ushort *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C4IR (  ushort *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_16u_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_AC4IR (  float *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C1IR (  float *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C3IR (  float *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C4IR (  float *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_AC4IR (  int *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_AC4R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C1IR (  int *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C1R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C3IR (  int *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C3R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C4IR (  int *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_32s_C4R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_AC4IR (  byte *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C1IR (  byte *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C3IR (  byte *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C4IR (  byte *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyReplicateBorder_8u_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyWrapBorder_32f_C1IR (  float *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyWrapBorder_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyWrapBorder_32s_C1IR (  int *pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize, int topBorderHeight, int leftborderwidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopyWrapBorder_32s_C1R (  int *pSrc, int srcStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_AC4C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_AC4MR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C1C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C1C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C1MR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3CR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3MR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3P3R (  short *pSrc, int srcStep, short **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C4C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C4CR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C4MR (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C4P4R (  short *pSrc, int srcStep, short **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_P3C3R (  short **pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16s_P4C4R (  short **pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_AC4C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_AC4MR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C1C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C1C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C1MR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3CR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3MR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3P3R (  ushort *pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C4C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C4CR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C4MR (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C4P4R (  ushort *pSrc, int srcStep, ushort **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_P3C3R (  ushort **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_16u_P4C4R (  ushort **pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_AC4C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_AC4MR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C1C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C1C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C1MR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3CR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3MR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3P3R (  float *pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C4C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C4CR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C4MR (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C4P4R (  float *pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_P3C3R (  float **pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32f_P4C4R (  float **pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_AC4C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_AC4MR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_AC4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C1C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C1C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C1MR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3AC4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3CR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3MR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3P3R (  int *pSrc, int srcStep, int **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C4C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C4CR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C4MR (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C4P4R (  int *pSrc, int srcStep, int **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_P3C3R (  int **pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_32s_P4C4R (  int **pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_AC4C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_AC4MR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C1C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C1C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C1MR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3CR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3MR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3P3R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C4C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C4CR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C4MR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C4P4R (  byte *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_P3C3R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCopy_8u_P4C4R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, int *counts, float *lowerBound, float *upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, int *counts, float lowerBound, float upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, int *counts, float *lowerBound, float *upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, int *counts, byte *lowerBound, byte *upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, int *counts, byte lowerBound, byte upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCountInRange_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, int *counts, byte *lowerBound, byte *upperBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_16sc16s_C1R (  Ipp16sc *pSrc, int srcStep, IppiSize srcSize, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_16sc16s_C3R (  Ipp16sc *pSrc, int srcStep, IppiSize srcSize, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_32fc32f_C1R (  Ipp32fc *pSrc, int srcStep, IppiSize srcSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_32fc32f_C3R (  Ipp32fc *pSrc, int srcStep, IppiSize srcSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_32sc32s_C1R (  Ipp32sc *pSrc, int srcStep, IppiSize srcSize, int *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCplxExtendToPack_32sc32s_C3R (  Ipp32sc *pSrc, int srcStep, IppiSize srcSize, int *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_64f_C1R (  double *pSrc, int srcStep, IppiSize srcRoiSize, double *pTpl, int tplStep, IppiSize tplRoiSize, double *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_NormLevel_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrFull_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_NormLevel_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrSame_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_64f_C1R (  double *pSrc, int srcStep, IppiSize srcRoiSize, double *pTpl, int tplStep, IppiSize tplRoiSize, double *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_NormLevel_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiCrossCorrValid_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8FwdLS_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, short addVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_16s_C1R (  short *pSrc, int srcStep, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_32f_C1 (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_32f_C1I (  float *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Fwd_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8InvLSClip_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, short addVal, byte clipDown, byte clipUp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_16s_C1R (  short *pSrc, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_2x2_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_2x2_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_32f_C1 (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_32f_C1I (  float *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_4x4_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_4x4_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_A10_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8Inv_A10_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8To2x2Inv_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8To2x2Inv_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8To4x4Inv_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCT8x8To4x4Inv_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwdFree_32f (  IppiDCTFwdSpec_32f *pDCTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwdGetBufSize_32f (  IppiDCTFwdSpec_32f *pDCTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwdInitAlloc_32f (  IppiDCTFwdSpec_32f **pDCTSpec, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwd_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTFwdSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwd_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTFwdSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwd_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTFwdSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTFwd_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTFwdSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInvFree_32f (  IppiDCTInvSpec_32f *pDCTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInvGetBufSize_32f (  IppiDCTInvSpec_32f *pDCTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInvInitAlloc_32f (  IppiDCTInvSpec_32f **pDCTSpec, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInv_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTInvSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInv_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTInvSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInv_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTInvSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDCTInv_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDCTInvSpec_32f *pDCTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFree_C_32fc (  IppiDFTSpec_C_32fc *pDFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFree_R_32f (  IppiDFTSpec_R_32f *pDFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFree_R_32s (  IppiDFTSpec_R_32s *pFFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_CToC_32fc_C1IR (  Ipp32fc *pSrcDst, int srcDstStep, IppiDFTSpec_C_32fc *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_CToC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pDst, int dstStep, IppiDFTSpec_C_32fc *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_8u32s_AC4RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_8u32s_C1RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_8u32s_C3RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTFwd_RToPack_8u32s_C4RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTGetBufSize_C_32fc (  IppiDFTSpec_C_32fc *pDFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTGetBufSize_R_32f (  IppiDFTSpec_R_32f *pDFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTGetBufSize_R_32s (  IppiDFTSpec_R_32s *pDFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInitAlloc_C_32fc (  IppiDFTSpec_C_32fc **pDFTSpec, IppiSize roiSize, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInitAlloc_R_32f (  IppiDFTSpec_R_32f **pDFTSpec, IppiSize roiSize, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInitAlloc_R_32s (  IppiDFTSpec_R_32s **pDFTSpec, IppiSize roiSize, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_CToC_32fc_C1IR (  Ipp32fc *pSrcDst, int srcDstStep, IppiDFTSpec_C_32fc *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_CToC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pDst, int dstStep, IppiDFTSpec_C_32fc *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiDFTSpec_R_32f *pDFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32s8u_AC4RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32s8u_C1RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32s8u_C3RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDFTInv_PackToR_32s8u_C4RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiDFTSpec_R_32s *pDFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDecimateFilterColumn_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiFraction fraction );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDecimateFilterRow_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiFraction fraction );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFTFree_32f_C1R (  IppiDeconvFFTState_32f_C1R *pDeconvFFTState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFTFree_32f_C3R (  IppiDeconvFFTState_32f_C3R *pDeconvFFTState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFTInitAlloc_32f_C1R (  IppiDeconvFFTState_32f_C1R **pDeconvFFTState, float *pKernel, int kernelSize, int FFTorder, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFTInitAlloc_32f_C3R (  IppiDeconvFFTState_32f_C3R **pDeconvFFTState, float *pKernel, int kernelSize, int FFTorder, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFT_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi, IppiDeconvFFTState_32f_C1R *pDeconvFFTState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvFFT_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi, IppiDeconvFFTState_32f_C3R *pDeconvFFTState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLRFree_32f_C1R (  IppiDeconvLR_32f_C1R *pDeconvLR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLRFree_32f_C3R (  IppiDeconvLR_32f_C3R *pDeconvLR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLRInitAlloc_32f_C1R (  IppiDeconvLR_32f_C1R **pDeconvLR, float *pKernel, int kernelSize, IppiSize maxroi, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLRInitAlloc_32f_C3R (  IppiDeconvLR_32f_C3R **pDeconvLR, float *pKernel, int kernelSize, IppiSize maxroi, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLR_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi, int numiter, IppiDeconvLR_32f_C1R *pDeconvLR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeconvLR_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roi, int numiter, IppiDeconvLR_32f_C3R *pDeconvLR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDeinterlaceFilterCAVT_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, ushort threshold, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate3x3_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDilate_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_AC4IRSfs (  short *val, short *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_AC4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C1IRSfs (  short value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C1RSfs (  short *pSrc, int srcStep, short value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C3IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C3RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C4IRSfs (  short *val, short *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16s_C4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_AC4IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_AC4RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_C1IRSfs (  Ipp16sc value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_C1RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_C3IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16sc_C3RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_AC4IRSfs (  ushort *val, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C1IRSfs (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C3IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C4IRSfs (  ushort *val, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_16u_C4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_AC4IR (  float *val, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_AC4R (  float *pSrc, int srcStep, float *val, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C1IR (  float value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C1R (  float *pSrc, int srcStep, float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C3IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C3R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C4IR (  float *val, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32f_C4R (  float *pSrc, int srcStep, float *val, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_AC4IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_AC4R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_C1IR (  Ipp32fc value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_C3IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32fc_C3R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_AC4IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_AC4RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_C1IRSfs (  Ipp32sc value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_C1RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_C3IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_32sc_C3RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_AC4IRSfs (  byte *val, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C1IRSfs (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C1RSfs (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C3IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C3RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C4IRSfs (  byte *val, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDivC_8u_C4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_AC4IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_AC4RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_C1IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_C1RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_C3IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16sc_C3RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_AC4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_AC4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C1IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C1RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C3IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C3RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_16u_C4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_AC4IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_AC4R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_C1IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_C1R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_C3IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32fc_C3R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_AC4IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_AC4RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_C1IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_C1RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_C3IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_32sc_C3RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_AC4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_AC4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C1IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C1RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C3IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C3RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_8u_C4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_AC4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_AC4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C1IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C1RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C3IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C3RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_16u_C4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_AC4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_AC4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C1IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C1RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C3IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C3RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDiv_Round_8u_C4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, IppRoundMode rndMode, int ScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProdCol_32f_L2 (  float **ppSrcRow, float *pTaps, int tapsLen, float *pDst, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProdCol_64f_L2 (  double **ppSrcRow, double *pTaps, int tapsLen, double *pDst, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16s64f_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16s64f_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16s64f_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16s64f_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16u64f_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16u64f_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16u64f_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_16u64f_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32f64f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pDp, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32f64f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pDp, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32f64f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pDp, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32f64f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pDp, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32s64f_AC4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32s64f_C1R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32s64f_C3R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32s64f_C4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32u64f_AC4R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32u64f_C1R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32u64f_C3R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_32u64f_C4R (  uint *pSrc1, int src1Step, uint *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8s64f_AC4R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8s64f_C1R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8s64f_C3R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8s64f_C4R (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8u64f_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8u64f_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8u64f_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDotProd_8u64f_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDup_8u_C1C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiDup_8u_C1C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode3x3_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiErode_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16s_C1IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16s_C3IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16u_C1IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int sFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int sFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16u_C3IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int sFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int sFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_8u_C1IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_8u_C1RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_8u_C3IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiExp_8u_C3RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFree_C_32fc (  IppiFFTSpec_C_32fc *pFFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFree_R_32f (  IppiFFTSpec_R_32f *pFFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFree_R_32s (  IppiFFTSpec_R_32s *pFFTSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_CToC_32fc_C1IR (  Ipp32fc *pSrcDst, int srcDstStep, IppiFFTSpec_C_32fc *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_CToC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pDst, int dstStep, IppiFFTSpec_C_32fc *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_8u32s_AC4RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_8u32s_C1RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_8u32s_C3RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTFwd_RToPack_8u32s_C4RSfs (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTGetBufSize_C_32fc (  IppiFFTSpec_C_32fc *pFFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTGetBufSize_R_32f (  IppiFFTSpec_R_32f *pFFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTGetBufSize_R_32s (  IppiFFTSpec_R_32s *pFFTSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInitAlloc_C_32fc (  IppiFFTSpec_C_32fc **pFFTSpec, int orderX, int orderY, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInitAlloc_R_32f (  IppiFFTSpec_R_32f **pFFTSpec, int orderX, int orderY, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInitAlloc_R_32s (  IppiFFTSpec_R_32s **pFFTSpec, int orderX, int orderY, int flag, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_CToC_32fc_C1IR (  Ipp32fc *pSrcDst, int srcDstStep, IppiFFTSpec_C_32fc *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_CToC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pDst, int dstStep, IppiFFTSpec_C_32fc *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiFFTSpec_R_32f *pFFTSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32s8u_AC4RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32s8u_C1RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32s8u_C3RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFFTInv_PackToR_32s8u_C4RSfs (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiFFTSpec_R_32s *pFFTSpec, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_32s_C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_32s_C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_32s_C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s16s_C3R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s16s_C4R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s_C1R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s_C3R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8s_C4R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u16s_C3R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u16s_C4R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter32f_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBilateralGetBufSize_8u_C1R (  IppiFilterBilateralType filter, IppiSize maxDstRoiSize, IppiSize maxKernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBilateralInit_8u_C1R (  IppiFilterBilateralType filter, IppiSize maxKernelSize, float valSquareSigma, float posSquareSigma, int stepInKernel, IppiFilterBilateralSpec *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBilateral_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize kernelSize, IppiFilterBilateralSpec *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_AC4IR (  short *pSrc, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_AC4IR (  ushort *pSrc, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterBox_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn32f_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize dstRoiSize, double *pKernel, int kernelSize, int yAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterColumn_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int yAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGauss_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterGetBufSize_64f_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterHipass_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLaplace_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterLowpass_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMax_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianColor_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianCross_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianGetBufferSize_32f (  IppiSize dstRoiSize, IppiSize maskSize, uint nChannels, uint *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianGetBufferSize_64f (  IppiSize dstRoiSize, IppiSize maskSize, uint nChannels, uint *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianHoriz_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianVert_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedianWeightedCenter3x3_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int weight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMedian_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterMin_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittHoriz_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterPrewittVert_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsDown_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRobertsUp_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize16s_8u_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize16s_8u_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize16s_8u_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize16s_8u_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16s_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16s_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16s_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16s_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16u_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16u_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16u_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_16u_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_8u_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_8u_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_8u_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32f_8u_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16s_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16s_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16s_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16s_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16u_AC4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16u_C1R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16u_C3R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRoundGetBufSize32s_16u_C4R (  IppiSize kernelSize, int roiWidth, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow32f_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize dstRoiSize, double *pKernel, int kernelSize, int xAnchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterRow_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, int kernelSize, int xAnchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrHoriz_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrHoriz_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrHoriz_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrVert_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrVert_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterScharrVert_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSharpen_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelCross_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelCross_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelCross_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHorizMask_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHorizSecond_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHorizSecond_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHorizSecond_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelHoriz_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVertMask_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVertSecond_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVertSecond_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVertSecond_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8s16s_C1R (  sbyte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterSobelVert_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWienerGetBufferSize (  IppiSize dstRoiSize, IppiSize maskSize, int channels, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilterWiener_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, float *noise, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_64f_C1R (  double *pSrc, int srcStep, double *pDst, int dstStep, IppiSize dstRoiSize, double *pKernel, IppiSize kernelSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round16s_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, short *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round16s_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, short *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round16s_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, short *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round16s_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, short *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32f_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize dstRoiSize, float *pKernel, IppiSize kernelSize, IppiPoint anchor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiFilter_Round32s_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int *pKernel, IppiSize kernelSize, IppiPoint anchor, int divisor, IppRoundMode roundMode, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetAffineBound (  IppiRect srcROI, double *bound, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetAffineQuad (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetAffineTransform (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetBilinearBound (  IppiRect srcROI, double *bound, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetBilinearQuad (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetBilinearTransform (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetCentralMoment_64f (  IppiMomentState_64f *pState, int mOrd, int nOrd, int nChannel, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetCentralMoment_64s (  IppiMomentState_64s *pState, int mOrd, int nOrd, int nChannel, long *pValue, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetHuMoments_64f (  IppiMomentState_64f *pState, int nChannel, double* pHm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetHuMoments_64s (  IppiMomentState_64s *pState, int nChannel, long* pHm, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname,EntryPoint="ippiGetLibVersion")] public static extern
int* xippiGetLibVersion (    );
public static IppLibraryVersion ippiGetLibVersion() {
   return new IppLibraryVersion( xippiGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetNormalizedCentralMoment_64f (  IppiMomentState_64f *pState, int mOrd, int nOrd, int nChannel, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetNormalizedCentralMoment_64s (  IppiMomentState_64s *pState, int mOrd, int nOrd, int nChannel, long *pValue, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetNormalizedSpatialMoment_64f (  IppiMomentState_64f *pState, int mOrd, int nOrd, int nChannel, IppiPoint roiOffset, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetNormalizedSpatialMoment_64s (  IppiMomentState_64s *pState, int mOrd, int nOrd, int nChannel, IppiPoint roiOffset, long *pValue, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetPerspectiveBound (  IppiRect srcROI, double *bound, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetPerspectiveQuad (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetPerspectiveTransform (  IppiRect srcROI, double *quad, double *coeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetResizeFract (  IppiSize srcSize, IppiRect srcROI, double xFactor, double yFactor, double *xFr, double *yFr, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetResizeRect (  IppiRect srcROI, IppiRect *pDstRect, double xFactor, double yFactor, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetRotateBound (  IppiRect srcROI, double *bound, double angle, double xShift, double yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetRotateQuad (  IppiRect srcROI, double *quad, double angle, double xShift, double yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetRotateShift (  double xCenter, double yCenter, double angle, double *xShift, double *yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetShearBound (  IppiRect srcROI, double *bound, double xShear, double yShear, double xShift, double yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetShearQuad (  IppiRect srcROI, double *quad, double xShear, double yShear, double xShift, double yShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetSpatialMoment_64f (  IppiMomentState_64f *pState, int mOrd, int nOrd, int nChannel, IppiPoint roiOffset, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiGetSpatialMoment_64s (  IppiMomentState_64s *pState, int mOrd, int nOrd, int nChannel, IppiPoint roiOffset, long *pValue, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels, int lowerLevel, int upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels, int lowerLevel, int upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels, int lowerLevel, int upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramEven_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels, int *lowerLevel, int *upperLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, int **pHist, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, int *pHist, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, int **pHist, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, int **pHist, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, int *pHist, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiHistogramRange_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, int **pHist, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16s_AC4R (  short *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16s_C1R (  short *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16s_C3R (  short *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16s_C4R (  short *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16u_AC4R (  ushort *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16u_C1R (  ushort *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16u_C3R (  ushort *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_16u_C4R (  ushort *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32f_AC4R (  float *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32f_C1R (  float *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32f_C3R (  float *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32f_C4R (  float *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32s_AC4R (  int *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32s_C1R (  int *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32s_C3R (  int *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_32s_C4R (  int *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8s_AC4R (  sbyte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8s_C1R (  sbyte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8s_C3R (  sbyte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8s_C4R (  sbyte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8u_AC4R (  byte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8u_C1R (  byte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8u_C3R (  byte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageJaehne_8u_C4R (  byte *pDst, int DstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16s_AC4R (  short *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16s_C1R (  short *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16s_C3R (  short *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16s_C4R (  short *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16u_AC4R (  ushort *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16u_C1R (  ushort *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16u_C3R (  ushort *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_16u_C4R (  ushort *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32f_AC4R (  float *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32f_C1R (  float *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32f_C3R (  float *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32f_C4R (  float *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32s_AC4R (  int *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32s_C1R (  int *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32s_C3R (  int *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_32s_C4R (  int *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8s_AC4R (  sbyte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8s_C1R (  sbyte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8s_C3R (  sbyte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8s_C4R (  sbyte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8u_AC4R (  byte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8u_C1R (  byte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8u_C3R (  byte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiImageRamp_8u_C4R (  byte *pDst, int DstStep, IppiSize roiSize, float offset, float slope, IppiAxis axis );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_AC4IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_AC4R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C1IR (  uint value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C1R (  ushort *pSrc, int srcStep, uint value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C3IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C3R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C4IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_16u_C4R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_AC4IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_AC4R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C1IR (  uint value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C1R (  int *pSrc, int srcStep, uint value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C3IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C3R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C4IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_32s_C4R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_AC4IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_AC4R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C1IR (  uint value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C1R (  byte *pSrc, int srcStep, uint value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C3IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C3R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C4IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLShiftC_8u_C4R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPaletteSwap_16u_C3A0C4R (  ushort *pSrc, int srcStep, int alphaValue, ushort *pDst, int dstStep, IppiSize roiSize, ushort **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPaletteSwap_8u_C3A0C4R (  byte *pSrc, int srcStep, int alphaValue, byte *pDst, int dstStep, IppiSize roiSize, byte **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u24u_C1R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u32u_C1R (  ushort *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roiSize, uint *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u8u_C1R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u_C3A0C4R (  ushort *pSrc, int srcStep, int alphaValue, ushort *pDst, int dstStep, IppiSize roiSize, ushort **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u24u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u32u_C1R (  byte *pSrc, int srcStep, uint *pDst, int dstStep, IppiSize roiSize, uint *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u_C3A0C4R (  byte *pSrc, int srcStep, int alphaValue, byte *pDst, int dstStep, IppiSize roiSize, byte **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUTPalette_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte **pTable, int nBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Cubic_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *pValues, float *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float **pValues, float **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *pValues, int *pLevels, int nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLUT_Linear_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int **pValues, int **pLevels, int *nLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16s_C1IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16s_C3IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16u_C1IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16u_C3IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_8u_C1IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_8u_C1RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_8u_C3IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiLn_8u_C3RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_32s_C1RSfs (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitudePack_32s_C3RSfs (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_16sc16s_C1RSfs (  Ipp16sc *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_16sc16s_C3RSfs (  Ipp16sc *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_16uc16u_C1RSfs (  Ipp16uc *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_16uc16u_C3RSfs (  Ipp16uc *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_32fc32f_C1R (  Ipp32fc *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_32fc32f_C3R (  Ipp32fc *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_32sc32s_C1RSfs (  Ipp32sc *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMagnitude_32sc32s_C3RSfs (  Ipp32sc *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16s_AC4IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16s_C1IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16s_C3IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16s_C4IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxEvery_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, short *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, short *pMax, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, short *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, short *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pMax, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, float *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pMax, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, float *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pMax, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMaxIndx_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, short *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMax_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, double *mean, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pMean, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, double *mean, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, double *mean, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMean_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *mean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMedian_8u_P3C1R (  byte **pSrc, int srcStep, byte *pDst, int dstStep, IppiSize size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16s_AC4IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16s_C1IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16s_C3IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16s_C4IR (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinEvery_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, short *pMin, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pMin, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pMin, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pMin, int *pIndexX, int *pIndexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinIndx_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, int *indexX, int *indexY );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, short *pMin, short *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min, short *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pMin, ushort *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min, ushort *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pMin, float *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min, float *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pMin, byte *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMinMax_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min, byte *max );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, short *pMin );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, short *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, short *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pMin );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pMin );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, float *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pMin );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMin_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *min );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_AC4IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_AC4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C1IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C3IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C4IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_32s_C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMirror_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiAxis flip );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentFree_64f (  IppiMomentState_64f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentFree_64s (  IppiMomentState_64s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentGetStateSize_64s (  IppHintAlgorithm hint, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentInitAlloc_64f (  IppiMomentState_64f **pState, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentInitAlloc_64s (  IppiMomentState_64s **pState, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMomentInit_64s (  IppiMomentState_64s *pState, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64f_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64f *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMoments64s_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, IppiMomentState_64s *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_AC4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_AC4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C1IR (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C1R (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C3IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C3R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_16u_C4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_AC4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_AC4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C1IR (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C1R (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C3IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C3R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulCScale_8u_C4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_AC4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_AC4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C1IRSfs (  short value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C1RSfs (  short *pSrc, int srcStep, short value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C3IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C3RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16s_C4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_AC4IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_AC4RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_C1IRSfs (  Ipp16sc value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_C1RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_C3IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16sc_C3RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_AC4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C1IRSfs (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C3IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_16u_C4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_AC4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_AC4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C1IR (  float value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C1R (  float *pSrc, int srcStep, float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C3IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C3R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32f_C4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_AC4IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_AC4R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_C1IR (  Ipp32fc value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_C3IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32fc_C3R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_AC4IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_AC4RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_C1IRSfs (  Ipp32sc value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_C1RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_C3IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_32sc_C3RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_AC4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C1IRSfs (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C1RSfs (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C3IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C3RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulC_8u_C4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPackConj_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_AC4IRSfs (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_AC4RSfs (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C1IRSfs (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C1RSfs (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C3IRSfs (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C3RSfs (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C4IRSfs (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulPack_32s_C4RSfs (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMulScale_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_AC4IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_AC4RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_C1IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_C1RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_C3IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16sc_C3RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_AC4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_AC4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C1IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C1RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C3IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C3RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_16u_C4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_AC4IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_AC4R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_C1IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_C1R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_C3IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32fc_C3R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_AC4IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_AC4RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_C1IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_C1RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_C3IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_32sc_C3RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_AC4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_AC4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C1IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C1RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C3IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C3RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiMul_8u_C4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L1_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16s_AC4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16s_C1R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16s_C3R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16s_C4R (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNormRel_L2_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_32s_C1R (  int *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_Inf_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L1_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pValue, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, double *value, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNorm_L2_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiNot_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_AC4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_AC4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C1IR (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C1R (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C3IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C3R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_16u_C4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_AC4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_AC4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C1IR (  int value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C1R (  int *pSrc, int srcStep, int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C3IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C3R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_32s_C4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_AC4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_AC4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C1IR (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C1R (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C3IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C3R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOrC_8u_C4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_AC4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_AC4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C1IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C1R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C3IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C3R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_32s_C4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiOr_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPackToCplxExtend_32f32fc_C1R (  float *pSrc, IppiSize srcSize, int srcStep, Ipp32fc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPackToCplxExtend_32s32sc_C1R (  int *pSrc, IppiSize srcSize, int srcStep, Ipp32sc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_32s_C1RSfs (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhasePack_32s_C3RSfs (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize dstRoiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_16sc16s_C1RSfs (  Ipp16sc *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_16sc16s_C3RSfs (  Ipp16sc *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_16uc16u_C1RSfs (  Ipp16uc *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_16uc16u_C3RSfs (  Ipp16uc *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int ScalFact );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_32fc32f_C1R (  Ipp32fc *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_32fc32f_C3R (  Ipp32fc *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_32sc32s_C1RSfs (  Ipp32sc *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPhase_32sc32s_C3RSfs (  Ipp32sc *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_16sc_C1R (  short *pSrcMagn, short *pSrcPhase, int srcStep, int PhaseFixedPoint, IppiSize roiSize, Ipp16sc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_16sc_C3R (  short *pSrcMagn, short *pSrcPhase, int srcStep, int PhaseFixedPoint, IppiSize roiSize, Ipp16sc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_32f32fc_P2C1R (  float *pSrcMagn, float *pSrcPhase, int srcStep, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_32fc_C1R (  float *pSrcMagn, float *pSrcPhase, int srcStep, IppiSize roiSize, Ipp32fc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_32fc_C3R (  float *pSrcMagn, float *pSrcPhase, int srcStep, IppiSize roiSize, Ipp32fc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_32sc_C1R (  int *pSrcMagn, int *pSrcPhase, int srcStep, int PhaseFixedPoint, IppiSize roiSize, Ipp32sc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiPolarToCart_32sc_C3R (  int *pSrcMagn, int *pSrcPhase, int srcStep, int PhaseFixedPoint, IppiSize roiSize, Ipp32sc *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_16u32f_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_16u32f_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_16u32f_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_8u32f_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_8u32f_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiQualityIndex_8u32f_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, IppiSize roiSize, float *pQualityIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_AC4IR (  uint *value, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_AC4R (  short *pSrc, int srcStep, uint *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C1IR (  uint value, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C1R (  short *pSrc, int srcStep, uint value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C3IR (  uint *value, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C3R (  short *pSrc, int srcStep, uint *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C4IR (  uint *value, short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16s_C4R (  short *pSrc, int srcStep, uint *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_AC4IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_AC4R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C1IR (  uint value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C1R (  ushort *pSrc, int srcStep, uint value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C3IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C3R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C4IR (  uint *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_16u_C4R (  ushort *pSrc, int srcStep, uint *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_AC4IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_AC4R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C1IR (  uint value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C1R (  int *pSrc, int srcStep, uint value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C3IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C3R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C4IR (  uint *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_32s_C4R (  int *pSrc, int srcStep, uint *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_AC4IR (  uint *value, sbyte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_AC4R (  sbyte *pSrc, int srcStep, uint *value, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C1IR (  uint value, sbyte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C1R (  sbyte *pSrc, int srcStep, uint value, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C3IR (  uint *value, sbyte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C3R (  sbyte *pSrc, int srcStep, uint *value, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C4IR (  uint *value, sbyte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8s_C4R (  sbyte *pSrc, int srcStep, uint *value, sbyte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_AC4IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_AC4R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C1IR (  uint value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C1R (  byte *pSrc, int srcStep, uint value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C3IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C3R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C4IR (  uint *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRShiftC_8u_C4R (  byte *pSrc, int srcStep, uint *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_AC4R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_C1R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_C3R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_C4R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_P3R (  short **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16s_P4R (  short **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, short **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, ushort **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, float **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pxMap, int xMapStep, double *pyMap, int yMapStep, double **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte *pDst, int dstStep, IppiSize dstRoiSize, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRemap_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pxMap, int xMapStep, float *pyMap, int yMapStep, byte **pDst, int dstStep, IppiSize dstROI, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResampleRowGetBorderWidth_32f (  IppiResampleRowSpec_32f *pRowSpec, int *pLeftWidth, int *pRightWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResampleRowGetSize_32f (  int srcSampleWidth, int dstSampleWidth, int *pSpecSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResampleRowInit_32f (  IppiResampleRowSpec_32f *pRowSpec, IppiResampleType filterType, int srcSampleWidth, int dstSampleWidth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResampleRowReplicateBorder_32f_C1R (  float *pSrc, int srcLineStep, float *pDst, int dstLineStep, int xOriginRefGrid, IppiSize sizeRefGrid, IppiResampleRowSpec_32f *pRowSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResampleRow_32f_C1 (  float *pSrc, int srcLineStep, float *pDst, int dstLineStep, int xOriginRefGrid, IppiSize sizeRefGrid, IppiResampleRowSpec_32f *pRowSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeCenter_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeFilterGetSize_8u_C1R (  IppiSize srcRoiSize, IppiSize dstRoiSize, IppiResizeFilterType filter, uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeFilterInit_8u_C1R (  IppiResizeFilterState *pState, IppiSize srcRoiSize, IppiSize dstRoiSize, IppiResizeFilterType filter );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeFilter_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiResizeFilterState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeGetBufSize (  IppiRect srcROI, IppiRect dstROI, int nChannel, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeGetBufSize_64f (  IppiRect srcROI, IppiRect dstROI, int nChannel, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeShift_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFr, double yFr, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixelGetBufSize (  IppiSize dstSize, int nChannel, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_AC4R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_C1R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_C3R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_C4R (  short *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_P3R (  short **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16s_P4R (  short **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, short **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeSqrPixel_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double xFactor, double yFactor, double xShift, double yShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeYUV420GetBufSize (  IppiSize srcRoiSize, IppiSize dstRoiSize, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeYUV420_8u_P2R (  byte *pSrcY, int srcYStep, byte *pSrcUV, int srcUVStep, IppiSize srcRoiSize, byte *pDstY, int dstYStep, byte *pDstUV, int dstUVStep, IppiSize dstRoiSize, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResizeYUV422_8u_C2R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiResize_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiSize dstRoiSize, double xFactor, double yFactor, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotateCenter_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double angle, double xCenter, double yCenter, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiRotate_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double angle, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSSIM_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pSrc3, int src3Step, float *pSrc4, int src4Step, float *pSrc5, int src5Step, float *pDst, int dstStep, IppiSize roiSize, float C1, float C2, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16s8u_AC4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16s8u_C1R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16s8u_C3R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16s8u_C4R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16u8u_AC4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16u8u_C1R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16u8u_C3R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_16u8u_C4R (  ushort *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32f8u_AC4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32f8u_C1R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32f8u_C3R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32f8u_C4R (  float *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32s8u_AC4R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32s8u_C1R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32s8u_C3R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_32s8u_C4R (  int *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16s_AC4R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16s_C3R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16s_C4R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16u_AC4R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16u_C1R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16u_C3R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u16u_C4R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32f_AC4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32f_C3R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32f_C4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float vMin, float vMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32s_AC4R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32s_C3R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiScale_8u32s_C4R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_AC4MR (  short *value, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_AC4R (  short *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C1MR (  short value, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C1R (  short value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C3CR (  short value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C3MR (  short *value, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C3R (  short *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C4CR (  short value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C4MR (  short *value, short *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16s_C4R (  short *value, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_AC4MR (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_AC4R (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C1MR (  ushort value, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C1R (  ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C3CR (  ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C3MR (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C3R (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C4CR (  ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C4MR (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_16u_C4R (  ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_AC4MR (  float *value, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_AC4R (  float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C1MR (  float value, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C1R (  float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C3CR (  float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C3MR (  float *value, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C3R (  float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C4CR (  float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C4MR (  float *value, float *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32f_C4R (  float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_AC4MR (  int *value, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_AC4R (  int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C1MR (  int value, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C1R (  int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C3CR (  int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C3MR (  int *value, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C3R (  int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C4CR (  int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C4MR (  int *value, int *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_32s_C4R (  int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_AC4MR (  byte *value, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_AC4R (  byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C1MR (  byte value, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C1R (  byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C3CR (  byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C3MR (  byte *value, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C3R (  byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C4CR (  byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C4MR (  byte *value, byte *pDst, int dstStep, IppiSize roiSize, byte *pMask, int maskStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSet_8u_C4R (  byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiShear_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double xShear, double yShear, double xShift, double yShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceFull_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceSame_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_16u32f_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_16u32f_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_16u32f_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8s32f_AC4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8s32f_C1R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8s32f_C3R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8s32f_C4R (  sbyte *pSrc, int srcStep, IppiSize srcRoiSize, sbyte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u32f_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u32f_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u32f_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, float *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u_AC4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u_C1RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u_C3RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrDistanceValid_Norm_8u_C4RSfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pTpl, int tplStep, IppiSize tplRoiSize, byte *pDst, int dstStep, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_AC4IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_AC4RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C1IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C3IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C4IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16s_C4RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_AC4IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C1IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C3IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C4IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_16u_C4RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_AC4IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C1IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C1RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C3IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C3RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C4IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqr_8u_C4RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_AC4IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_AC4RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_C1IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_C1RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_C3IRSfs (  short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16s_C3RSfs (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_AC4IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_C1IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_C3IRSfs (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_AC4IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_C1IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_C1RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_C3IRSfs (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSqrt_8u_C3RSfs (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_AC4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_AC4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C1IRSfs (  short value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C1RSfs (  short *pSrc, int srcStep, short value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C3IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C3RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C4IRSfs (  short *value, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16s_C4RSfs (  short *pSrc, int srcStep, short *value, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_AC4IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_AC4RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_C1IRSfs (  Ipp16sc value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_C1RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_C3IRSfs (  Ipp16sc *value, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16sc_C3RSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *value, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_AC4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_AC4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C1IRSfs (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C1RSfs (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C3IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C3RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C4IRSfs (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_16u_C4RSfs (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_AC4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_AC4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C1IR (  float value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C1R (  float *pSrc, int srcStep, float value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C3IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C3R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C4IR (  float *value, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32f_C4R (  float *pSrc, int srcStep, float *value, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_AC4IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_AC4R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_C1IR (  Ipp32fc value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_C1R (  Ipp32fc *pSrc, int srcStep, Ipp32fc value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_C3IR (  Ipp32fc *value, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32fc_C3R (  Ipp32fc *pSrc, int srcStep, Ipp32fc *value, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_AC4IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_AC4RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_C1IRSfs (  Ipp32sc value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_C1RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_C3IRSfs (  Ipp32sc *value, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_32sc_C3RSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *value, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_AC4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_AC4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C1IRSfs (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C1RSfs (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C3IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C3RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C4IRSfs (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSubC_8u_C4RSfs (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_AC4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_AC4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C1IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C1RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C3IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C3RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C4IRSfs (  short *pSrc, int srcStep, short *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16s_C4RSfs (  short *pSrc1, int src1Step, short *pSrc2, int src2Step, short *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_AC4IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_AC4RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_C1IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_C1RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_C3IRSfs (  Ipp16sc *pSrc, int srcStep, Ipp16sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16sc_C3RSfs (  Ipp16sc *pSrc1, int src1Step, Ipp16sc *pSrc2, int src2Step, Ipp16sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_AC4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_AC4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C1IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C1RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C3IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C3RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C4IRSfs (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_16u_C4RSfs (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_AC4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_AC4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C3IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C3R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C4IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32f_C4R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_AC4IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_AC4R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_C1IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_C1R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_C3IR (  Ipp32fc *pSrc, int srcStep, Ipp32fc *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32fc_C3R (  Ipp32fc *pSrc1, int src1Step, Ipp32fc *pSrc2, int src2Step, Ipp32fc *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_AC4IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_AC4RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_C1IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_C1RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_C3IRSfs (  Ipp32sc *pSrc, int srcStep, Ipp32sc *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_32sc_C3RSfs (  Ipp32sc *pSrc1, int src1Step, Ipp32sc *pSrc2, int src2Step, Ipp32sc *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_AC4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_AC4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C1IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C1RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C3IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C3RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C4IRSfs (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSub_8u_C4RSfs (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16s32f_C1R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16s32f_C3R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16s32f_C4R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16u32f_C1R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16u32f_C3R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_16u32f_C4R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_8u32f_C3R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowColumn_8u32f_C4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16s32f_C1R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16s32f_C3R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16s32f_C4R (  short *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16u32f_C1R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16u32f_C3R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_16u32f_C4R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_8u32f_C3R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSumWindowRow_8u32f_C4R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize dstRoiSize, int maskSize, int anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16s_AC4R (  short *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16s_C1R (  short *pSrc, int srcStep, IppiSize roiSize, double *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16s_C3R (  short *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16s_C4R (  short *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_16u_C4R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_32f_AC4R (  float *pSrc, int srcStep, IppiSize roiSize, double *sum, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pSum, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, double *sum, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_32f_C4R (  float *pSrc, int srcStep, IppiSize roiSize, double *sum, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_8u_AC4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSum_8u_C4R (  byte *pSrc, int srcStep, IppiSize roiSize, double *sum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSamplingGetBufSize (  IppiSize srcRoiSize, IppiSize dstRoiSize, int nChannel, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_AC4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_C1R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_C3R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_C4R (  short *pSrc, int srcStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_P3R (  short **pSrc, int srcStep, IppiSize srcRoiSize, short **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16s_P4R (  short **pSrc, int srcStep, IppiSize srcRoiSize, short **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_AC4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_C4R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_P3R (  ushort **pSrc, int srcStep, IppiSize srcRoiSize, ushort **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_16u_P4R (  ushort **pSrc, int srcStep, IppiSize srcRoiSize, ushort **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_AC4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_C4R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_P3R (  float **pSrc, int srcStep, IppiSize srcRoiSize, float **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_32f_P4R (  float **pSrc, int srcStep, IppiSize srcRoiSize, float **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_AC4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_C4R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_P3R (  byte **pSrc, int srcStep, IppiSize srcRoiSize, byte **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSuperSampling_8u_P4R (  byte **pSrc, int srcStep, IppiSize srcRoiSize, byte **pDst, int dstStep, IppiSize dstRoiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16s_C3C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *dstOrder, short val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16s_C4C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16u_C3C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *dstOrder, ushort val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_16u_C4C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32f_C3C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, int *dstOrder, float val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32f_C4C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32s_AC4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32s_C3C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int *dstOrder, int val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32s_C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_32s_C4C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_C3C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *dstOrder, byte val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_C4C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiSwapChannels_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *dstOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold, short value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold, short value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold, ushort value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold, ushort value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold, byte value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold, byte value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GTVal_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_GT_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *thresholdLT, short *valueLT, short *thresholdGT, short *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *thresholdLT, short *valueLT, short *thresholdGT, short *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short thresholdLT, short valueLT, short thresholdGT, short valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short thresholdLT, short valueLT, short thresholdGT, short valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *thresholdLT, short *valueLT, short *thresholdGT, short *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *thresholdLT, short *valueLT, short *thresholdGT, short *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *thresholdLT, ushort *valueLT, ushort *thresholdGT, ushort *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *thresholdLT, ushort *valueLT, ushort *thresholdGT, ushort *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort thresholdLT, ushort valueLT, ushort thresholdGT, ushort valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort thresholdLT, ushort valueLT, ushort thresholdGT, ushort valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *thresholdLT, ushort *valueLT, ushort *thresholdGT, ushort *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *thresholdLT, ushort *valueLT, ushort *thresholdGT, ushort *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *thresholdLT, float *valueLT, float *thresholdGT, float *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *thresholdLT, float *valueLT, float *thresholdGT, float *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float thresholdLT, float valueLT, float thresholdGT, float valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float thresholdLT, float valueLT, float thresholdGT, float valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *thresholdLT, float *valueLT, float *thresholdGT, float *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *thresholdLT, float *valueLT, float *thresholdGT, float *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *thresholdLT, byte *valueLT, byte *thresholdGT, byte *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *thresholdLT, byte *valueLT, byte *thresholdGT, byte *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte thresholdLT, byte valueLT, byte thresholdGT, byte valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte thresholdLT, byte valueLT, byte thresholdGT, byte valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *thresholdLT, byte *valueLT, byte *thresholdGT, byte *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTValGTVal_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *thresholdLT, byte *valueLT, byte *thresholdGT, byte *valueGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold, short value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold, short value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold, ushort value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold, ushort value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold, byte value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold, byte value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LTVal_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_LT_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_AC4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_AC4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short threshold, short value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short threshold, short value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize, short *threshold, short *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, short *threshold, short *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_AC4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_AC4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort threshold, ushort value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort threshold, ushort value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize, ushort *threshold, ushort *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, ushort *threshold, ushort *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_AC4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_AC4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float threshold, float value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float threshold, float value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize, float *threshold, float *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *threshold, float *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_AC4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_AC4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte threshold, byte value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte threshold, byte value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *threshold, byte *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiThreshold_Val_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *threshold, byte *value, IppCmpOp ippCmpOp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C3IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C3R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C4IR (  short *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16s_C4R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C3IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C4IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_16u_C4R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C3IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C4IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C1IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C1R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C3IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C3R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C4IR (  int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_32s_C4R (  int *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C3IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C4IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiTranspose_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdFree_32f_C1R (  IppiWTFwdSpec_32f_C1R *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdFree_32f_C3R (  IppiWTFwdSpec_32f_C3R *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdGetBufSize_C1R (  IppiWTFwdSpec_32f_C1R *pSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdGetBufSize_C3R (  IppiWTFwdSpec_32f_C3R *pSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdInitAlloc_32f_C1R (  IppiWTFwdSpec_32f_C1R **pSpec, float *pTapsLow, int lenLow, int anchorLow, float *pTapsHigh, int lenHigh, int anchorHigh );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwdInitAlloc_32f_C3R (  IppiWTFwdSpec_32f_C3R **pSpec, float *pTapsLow, int lenLow, int anchorLow, float *pTapsHigh, int lenHigh, int anchorHigh );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwd_32f_C1R (  float *pSrc, int srcStep, float *pApproxDst, int approxStep, float *pDetailXDst, int detailXStep, float *pDetailYDst, int detailYStep, float *pDetailXYDst, int detailXYStep, IppiSize dstRoiSize, IppiWTFwdSpec_32f_C1R *pSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTFwd_32f_C3R (  float *pSrc, int srcStep, float *pApproxDst, int approxStep, float *pDetailXDst, int detailXStep, float *pDetailYDst, int detailYStep, float *pDetailXYDst, int detailXYStep, IppiSize dstRoiSize, IppiWTFwdSpec_32f_C3R *pSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvFree_32f_C1R (  IppiWTInvSpec_32f_C1R *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvFree_32f_C3R (  IppiWTInvSpec_32f_C3R *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvGetBufSize_C1R (  IppiWTInvSpec_32f_C1R *pSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvGetBufSize_C3R (  IppiWTInvSpec_32f_C3R *pSpec, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvInitAlloc_32f_C1R (  IppiWTInvSpec_32f_C1R **pSpec, float *pTapsLow, int lenLow, int anchorLow, float *pTapsHigh, int lenHigh, int anchorHigh );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInvInitAlloc_32f_C3R (  IppiWTInvSpec_32f_C3R **pSpec, float *pTapsLow, int lenLow, int anchorLow, float *pTapsHigh, int lenHigh, int anchorHigh );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInv_32f_C1R (  float *pApproxSrc, int approxStep, float *pDetailXSrc, int detailXStep, float *pDetailYSrc, int detailYStep, float *pDetailXYSrc, int detailXYStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiWTInvSpec_32f_C1R *pSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWTInv_32f_C3R (  float *pApproxSrc, int approxStep, float *pDetailXSrc, int detailXStep, float *pDetailYSrc, int detailYStep, float *pDetailXYSrc, int detailXYStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiWTInvSpec_32f_C3R *pSpec, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineBack_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffineQuad_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_AC4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_C1R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_C3R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_C4R (  double *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_P3R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_64f_P4R (  double **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpAffine_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearBack_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinearQuad_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpBilinear_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveBack_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, ushort **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, float **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte *pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspectiveQuad_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, double *srcQuad, byte **pDst, int dstStep, IppiRect dstROI, double *dstQuad, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_AC4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_C1R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_C3R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_C4R (  ushort *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_P3R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_16u_P4R (  ushort **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, ushort **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_AC4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_C1R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_C3R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_C4R (  float *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_P3R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_32f_P4R (  float **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, float **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_AC4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_C1R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_C3R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_C4R (  byte *pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte *pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_P3R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWarpPerspective_8u_P4R (  byte **pSrc, IppiSize srcSize, int srcStep, IppiRect srcROI, byte **pDst, int dstStep, IppiRect dstROI, double *coeffs, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlettSep_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinBartlett_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHammingSep_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_16u_C1IR (  ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_32f_C1IR (  float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiWinHamming_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_AC4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_AC4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C1IR (  ushort value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C1R (  ushort *pSrc, int srcStep, ushort value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C3IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C3R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C4IR (  ushort *value, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_16u_C4R (  ushort *pSrc, int srcStep, ushort *value, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_AC4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_AC4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C1IR (  int value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C1R (  int *pSrc, int srcStep, int value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C3IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C3R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C4IR (  int *value, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_32s_C4R (  int *pSrc, int srcStep, int *value, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_AC4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_AC4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C1IR (  byte value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C1R (  byte *pSrc, int srcStep, byte value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C3IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C3R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C4IR (  byte *value, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXorC_8u_C4R (  byte *pSrc, int srcStep, byte *value, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_AC4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_AC4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C3IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C3R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C4IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_16u_C4R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_AC4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_AC4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C1IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C1R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C3IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C3R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C4IR (  int *pSrc, int srcStep, int *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_32s_C4R (  int *pSrc1, int src1Step, int *pSrc2, int src2Step, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_AC4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_AC4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C3IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C4IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiXor_8u_C4R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiZigzagFwd8x8_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ip.libname)] public static extern
IppStatus ippiZigzagInv8x8_16s_C1 (  short *pSrc, short *pDst );
};
};
