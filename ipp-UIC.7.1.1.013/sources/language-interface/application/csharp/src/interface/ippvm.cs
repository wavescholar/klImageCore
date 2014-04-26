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

// generated automatically on Thu Feb  4 09:22:09 2010

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

unsafe public class vm {

   internal const string libname = "ippvm-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_32fc_A11 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_32fc_A21 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_32fc_A24 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_64fc_A26 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_64fc_A50 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAbs_64fc_A53 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcos_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAcosh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAdd_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAdd_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAdd_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAdd_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_32fc_A11 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_32fc_A21 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_32fc_A24 (  Ipp32fc *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_64fc_A26 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_64fc_A50 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsArg_64fc_A53 (  Ipp64fc *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsin_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAsinh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_32f_A11 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_32f_A21 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_64f_A26 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_64f_A50 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan2_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtan_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsAtanh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_32fc_A11 (  float *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_32fc_A21 (  float *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_32fc_A24 (  float *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_64fc_A26 (  double *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_64fc_A50 (  double *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCIS_64fc_A53 (  double *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCbrt_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNormInv_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCdfNorm_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCeil_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCeil_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsConj_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsConj_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCos_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsCosh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32f_A11 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32f_A21 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32fc_A11 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32fc_A21 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64f_A26 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64f_A50 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64fc_A26 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64fc_A50 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsDiv_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfInv_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErf_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfcInv_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsErfc_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExp_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsExpm1_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsFloor_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsFloor_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_32f_A11 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_32f_A21 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_64f_A26 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_64f_A50 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsHypot_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvCbrt_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInvSqrt_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsInv_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLn_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog10_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsLog1p_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsModf_32f (  float *a, float *r1, float *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsModf_64f (  double *a, double *r1, double *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_32fc_A11 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_32fc_A21 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_64fc_A26 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_64fc_A50 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMulByConj_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_32fc_A11 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_32fc_A21 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_64fc_A26 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_64fc_A50 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsMul_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsNearbyInt_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsNearbyInt_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow2o3_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow3o2_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32f_A11 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32f_A21 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32fc_A11 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32fc_A21 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64f_A26 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64f_A50 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64fc_A26 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64fc_A50 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPow_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32f_A11 (  float *a, float b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32f_A21 (  float *a, float b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32f_A24 (  float *a, float b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32fc_A11 (  Ipp32fc *a, Ipp32fc b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32fc_A21 (  Ipp32fc *a, Ipp32fc b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_32fc_A24 (  Ipp32fc *a, Ipp32fc b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64f_A26 (  double *a, double b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64f_A50 (  double *a, double b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64f_A53 (  double *a, double b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64fc_A26 (  Ipp64fc *a, Ipp64fc b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64fc_A50 (  Ipp64fc *a, Ipp64fc b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsPowx_64fc_A53 (  Ipp64fc *a, Ipp64fc b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsRint_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsRint_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsRound_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsRound_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_32f_A11 (  float *a, float *r1, float *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_32f_A21 (  float *a, float *r1, float *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_32f_A24 (  float *a, float *r1, float *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_64f_A26 (  double *a, double *r1, double *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_64f_A50 (  double *a, double *r1, double *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinCos_64f_A53 (  double *a, double *r1, double *r2, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSin_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSinh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqr_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqr_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSqrt_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSub_32f_A24 (  float *a, float *b, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSub_32fc_A24 (  Ipp32fc *a, Ipp32fc *b, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSub_64f_A53 (  double *a, double *b, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsSub_64fc_A53 (  Ipp64fc *a, Ipp64fc *b, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTan_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32f_A11 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32f_A21 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32f_A24 (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32fc_A11 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32fc_A21 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_32fc_A24 (  Ipp32fc *a, Ipp32fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64f_A26 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64f_A50 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64f_A53 (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64fc_A26 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64fc_A50 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTanh_64fc_A53 (  Ipp64fc *a, Ipp64fc *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTrunc_32f (  float *a, float *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname)] public static extern
IppStatus ippsTrunc_64f (  double *a, double *r, int n );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vm.libname,EntryPoint="ippvmGetLibVersion")] public static extern
int* xippvmGetLibVersion (    );
public static IppLibraryVersion ippvmGetLibVersion() {
   return new IppLibraryVersion( xippvmGetLibVersion() );
}
};
};
