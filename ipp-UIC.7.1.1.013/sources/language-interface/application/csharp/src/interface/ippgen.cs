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

// generated automatically on Wed Feb  3 16:36:40 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct ippgDCT4Spec_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct ippgDCT4Spec_64f {};

unsafe public class gen {

   internal const string libname = "ippgen-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4Free_32f (  ippgDCT4Spec_32f *spec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4Free_64f (  ippgDCT4Spec_64f *spec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4GetSize_32f (  int len, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4GetSize_64f (  int len, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4InitAlloc_32f (  ippgDCT4Spec_32f **pSpec, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4InitAlloc_64f (  ippgDCT4Spec_64f **pSpec, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4Init_32f (  ippgDCT4Spec_32f **pSpec, int len, byte *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4Init_64f (  ippgDCT4Spec_64f **pSpec, int len, byte *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4_32f (  float *pSrc, float *pDst, ippgDCT4Spec_32f *spec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDCT4_64f (  double *pSrc, double *pDst, ippgDCT4Spec_64f *spec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_10_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_10_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_11_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_11_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_12_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_12_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_13_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_13_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_14_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_14_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_15_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_15_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_16_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_16_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_17_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_17_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_18_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_18_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_19_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_19_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_20_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_20_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_21_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_21_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_22_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_22_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_23_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_23_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_24_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_24_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_25_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_25_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_26_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_26_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_27_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_27_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_28_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_28_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_29_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_29_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_2_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_2_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_30_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_30_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_31_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_31_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_32_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_32_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_33_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_33_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_34_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_34_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_35_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_35_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_36_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_36_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_37_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_37_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_38_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_38_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_39_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_39_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_3_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_3_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_40_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_40_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_41_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_41_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_42_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_42_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_43_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_43_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_44_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_44_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_45_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_45_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_46_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_46_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_47_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_47_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_48_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_48_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_49_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_49_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_4_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_4_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_50_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_50_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_51_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_51_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_52_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_52_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_53_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_53_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_54_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_54_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_55_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_55_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_56_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_56_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_57_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_57_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_58_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_58_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_59_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_59_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_5_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_5_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_60_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_60_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_61_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_61_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_62_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_62_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_63_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_63_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_64_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_64_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_6_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_6_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_7_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_7_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_8_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_8_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_9_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_CToC_9_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToCCS_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPack_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTFwd_RToPerm_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CCSToR_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_10_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_10_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_11_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_11_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_12_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_12_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_13_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_13_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_14_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_14_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_15_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_15_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_16_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_16_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_17_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_17_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_18_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_18_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_19_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_19_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_20_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_20_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_21_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_21_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_22_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_22_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_23_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_23_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_24_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_24_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_25_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_25_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_26_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_26_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_27_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_27_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_28_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_28_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_29_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_29_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_2_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_2_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_30_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_30_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_31_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_31_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_32_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_32_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_33_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_33_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_34_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_34_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_35_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_35_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_36_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_36_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_37_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_37_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_38_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_38_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_39_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_39_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_3_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_3_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_40_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_40_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_41_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_41_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_42_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_42_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_43_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_43_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_44_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_44_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_45_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_45_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_46_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_46_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_47_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_47_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_48_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_48_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_49_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_49_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_4_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_4_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_50_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_50_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_51_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_51_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_52_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_52_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_53_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_53_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_54_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_54_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_55_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_55_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_56_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_56_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_57_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_57_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_58_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_58_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_59_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_59_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_5_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_5_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_60_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_60_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_61_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_61_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_62_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_62_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_63_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_63_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_64_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_64_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_6_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_6_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_7_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_7_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_8_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_8_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_9_32fc (  Ipp32fc *pSrc, Ipp32fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_CToC_9_64fc (  Ipp64fc *pSrc, Ipp64fc *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PackToR_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgDFTInv_PermToR_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_10_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_10_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_11_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_11_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_12_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_12_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_13_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_13_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_14_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_14_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_15_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_15_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_16_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_16_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_17_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_17_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_18_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_18_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_19_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_19_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_20_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_20_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_21_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_21_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_22_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_22_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_23_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_23_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_24_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_24_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_25_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_25_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_26_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_26_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_27_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_27_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_28_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_28_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_29_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_29_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_2_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_2_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_30_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_30_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_31_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_31_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_32_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_32_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_32f (  float *pSrc, float *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_33_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_33_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_34_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_34_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_35_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_35_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_36_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_36_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_37_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_37_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_38_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_38_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_39_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_39_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_3_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_3_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_40_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_40_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_41_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_41_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_42_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_42_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_43_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_43_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_44_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_44_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_45_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_45_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_46_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_46_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_47_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_47_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_48_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_48_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_49_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_49_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_4_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_4_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_50_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_50_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_51_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_51_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_52_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_52_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_53_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_53_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_54_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_54_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_55_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_55_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_56_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_56_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_57_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_57_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_58_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_58_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_59_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_59_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_5_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_5_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_60_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_60_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_61_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_61_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_62_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_62_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_63_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_63_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_64_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_64_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_64f (  double *pSrc, double *pDst, int length, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_6_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_6_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_7_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_7_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_8_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_8_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_9_32f (  float *pSrc, float *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgHartley_9_64f (  double *pSrc, double *pDst, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHTGetBufferSize_32f (  int order, uint *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHTGetBufferSize_64f (  int order, uint *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_10_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_10_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_11_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_11_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_12_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_12_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_13_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_13_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_1_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_1_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_2_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_2_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_32f (  float *pSrc, float *pDst, int order, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_3_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_3_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_4_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_4_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_5_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_5_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_64f (  double *pSrc, double *pDst, int order, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_6_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_6_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_7_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_7_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_8_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_8_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_9_32f (  float *pSrc, float *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname)] public static extern
IppStatus ippgWHT_9_64f (  double *pSrc, double *pDst, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.gen.libname,EntryPoint="ippgenGetLibVersion")] public static extern
int* xippgenGetLibVersion (    );
public static IppLibraryVersion ippgenGetLibVersion() {
   return new IppLibraryVersion( xippgenGetLibVersion() );
}
};
};
