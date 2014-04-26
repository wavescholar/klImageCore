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

// generated automatically on Wed Feb  3 17:03:54 2010

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

unsafe public class mp {

   internal const string libname = "ippm-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mm_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_mm_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tata_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tm_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tm_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tt_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tt_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tt_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_tt_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_32f (  float *pSrc, int srcStride0, int srcStride2, float val, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_64f (  double *pSrc, int srcStride0, int srcStride2, double val, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vac_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_32f (  float *pSrc, int srcStride0, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_64f (  double *pSrc, int srcStride0, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vaca_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float *pSrc2, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double *pSrc2, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vc_32f (  float *pSrc, int srcStride2, float val, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vc_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vc_64f (  double *pSrc, int srcStride2, double val, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vc_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_32f (  float *pSrc, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_32f_L (  float *pSrc, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_32f_P (  float **ppSrc, int srcRoiShift, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_64f (  double *pSrc, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_64f_L (  double *pSrc, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vca_64f_P (  double **ppSrc, int srcRoiShift, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vv_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vv_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vv_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAdd_vv_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform3DH_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformCoord_m4v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformCoord_m4v2a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformCoord_m4v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformCoord_m4v3a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformNormal_m4v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformNormal_m4v2a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformNormal_m4v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransformNormal_m4v3a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v2a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v3a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmAffineTransform_m4v4a_32f (  float *pSrc1, float *pSrc2, int src2Stride0, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmBaryCentric_v2_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float f, float g, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmBaryCentric_v3_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float f, float g, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmBaryCentric_v4_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float f, float g, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mv_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mv_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyBackSubst_mva_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_m_32f_P (  float **ppSrc, int srcRoiShift, float **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_m_64f_P (  double **ppSrc, int srcRoiShift, double **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCholeskyDecomp_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_LL (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_LP (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_LS (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_PL (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_PP (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_PS (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_SL (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_SP (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_32f_SS (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_LL (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_LP (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_LS (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_PL (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_PP (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_PS (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_SL (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_SP (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_ma_64f_SS (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_LL (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_LP (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_LS (  float **ppSrc, int srcRoiShift, int srcStride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_PL (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_PP (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_PS (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_SL (  float *pSrc, int srcStride0, int srcStride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_SP (  float *pSrc, int srcStride0, int srcStride2, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_32f_SS (  float *pSrc, int srcStride0, int srcStride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_LL (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_LP (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_LS (  double **ppSrc, int srcRoiShift, int srcStride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_PL (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_PP (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_PS (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_SL (  double *pSrc, int srcStride0, int srcStride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_SP (  double *pSrc, int srcStride0, int srcStride2, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCopy_va_64f_SS (  double *pSrc, int srcStride0, int srcStride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_v4_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float *pSrc2, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double *pSrc2, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vv_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vv_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vv_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vv_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_32f_L (  float *pSrc1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_64f_L (  double *pSrc1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmCrossProduct_vva_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_m4_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_m_32f (  float *pSrc, int srcStride1, int srcStride2, int widthHeight, float *pBuffer, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_m_32f_P (  float **ppSrc, int srcRoiShift, int widthHeight, float *pBuffer, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_m_64f (  double *pSrc, int srcStride1, int srcStride2, int widthHeight, double *pBuffer, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_m_64f_P (  double **ppSrc, int srcRoiShift, int widthHeight, double *pBuffer, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, int widthHeight, float *pBuffer, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int widthHeight, float *pBuffer, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, int widthHeight, float *pBuffer, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, int widthHeight, double *pBuffer, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int widthHeight, double *pBuffer, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDet_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, int widthHeight, double *pBuffer, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vv_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vv_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vv_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmDotProduct_vv_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesGetBufSize_32f (  int widthHeight, int *pSizeBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesGetBufSize_64f (  int widthHeight, int *pSizeBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pBuffer, float *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_m_32f_P (  float **ppSrc, int srcRoiShift, float *pBuffer, float *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pBuffer, double *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_m_64f_P (  double **ppSrc, int srcRoiShift, double *pBuffer, double *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pBuffer, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pBuffer, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pBuffer, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pBuffer, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pBuffer, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesSym_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pBuffer, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsGetBufSize_32f (  int widthHeight, int *pSizeBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsGetBufSize_64f (  int widthHeight, int *pSizeBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDstVectorsLeft, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_m_32f_P (  float **ppSrc, int srcRoiShift, float **ppDstVectorsLeft, int dstLeftRoiShift, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDstVectorsLeft, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_m_64f_P (  double **ppSrc, int srcRoiShift, double **ppDstVectorsLeft, int dstLeftRoiShift, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDstVectorsLeft, int dstLeftStride0, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride0, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDstVectorsLeft, int dstLeftStride0, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsLeft_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride0, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDstVectorsRight, int dstRightStride1, int dstRightStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_m_32f_P (  float **ppSrc, int srcRoiShift, float **ppDstVectorsRight, int dstRightRoiShift, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDstVectorsRight, int dstRightStride1, int dstRightStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_m_64f_P (  double **ppSrc, int srcRoiShift, double **ppDstVectorsRight, int dstRightRoiShift, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDstVectorsRight, int dstRightStride0, int dstRightStride1, int dstRightStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride1, int dstRightStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride0, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDstVectorsRight, int dstRightStride0, int dstRightStride1, int dstRightStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride1, int dstRightStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsRight_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride0, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pBuffer, float *pDstVectors, int dstStride1, int dstStride2, float *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_m_32f_P (  float **ppSrc, int srcRoiShift, float *pBuffer, float **ppDstVectors, int dstRoiShift, float *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pBuffer, double *pDstVectors, int dstStride1, int dstStride2, double *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_m_64f_P (  double **ppSrc, int srcRoiShift, double *pBuffer, double **ppDstVectors, int dstRoiShift, double *pDstValues, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pBuffer, float *pDstVectors, int dstStride0, int dstStride1, int dstStride2, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pBuffer, float **ppDstVectors, int dstRoiShift, int dstStride1, int dstStride2, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pBuffer, float **ppDstVectors, int dstRoiShift, int dstStride0, float *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pBuffer, double *pDstVectors, int dstStride0, int dstStride1, int dstStride2, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pBuffer, double **ppDstVectors, int dstRoiShift, int dstStride1, int dstStride2, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectorsSym_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pBuffer, double **ppDstVectors, int dstRoiShift, int dstStride0, double *pDstValues, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDstVectorsRight, int dstRightStride1, int dstRightStride2, float *pDstVectorsLeft, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_m_32f_P (  float **ppSrc, int srcRoiShift, float **ppDstVectorsRight, int dstRightRoiShift, float **ppDstVectorsLeft, int dstLeftRoiShift, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDstVectorsRight, int dstRightStride1, int dstRightStride2, double *pDstVectorsLeft, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_m_64f_P (  double **ppSrc, int srcRoiShift, double **ppDstVectorsRight, int dstRightRoiShift, double **ppDstVectorsLeft, int dstLeftRoiShift, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDstVectorsRight, int dstRightStride0, int dstRightStride1, int dstRightStride2, float *pDstVectorsLeft, int dstLeftStride0, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride1, int dstRightStride2, float **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride1, int dstLeftStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride0, float **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride0, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDstVectorsRight, int dstRightStride0, int dstRightStride1, int dstRightStride2, double *pDstVectorsLeft, int dstLeftStride0, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride1, int dstRightStride2, double **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride1, int dstLeftStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValuesVectors_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double **ppDstVectorsRight, int dstRightRoiShift, int dstRightStride0, double **ppDstVectorsLeft, int dstLeftRoiShift, int dstLeftStride0, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_m_32f_P (  float **ppSrc, int srcRoiShift, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_m_64f_P (  double **ppSrc, int srcRoiShift, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDstValuesRe, float *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmEigenValues_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDstValuesRe, double *pDstValuesIm, int widthHeight, int count, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_m_32f_P (  float **ppSrc, int srcRoiShift, float *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_m_64f_P (  double **ppSrc, int srcRoiShift, double *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_t_32f (  float *pSrc, int srcStride1, int srcStride2, float *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_t_32f_P (  float **ppSrc, int srcRoiShift, float *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_t_64f (  double *pSrc, int srcStride1, int srcStride2, double *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_t_64f_P (  double **ppSrc, int srcRoiShift, double *pDst, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_ta_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_v_32f (  float *pSrc, int srcStride2, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_v_32f_P (  float **ppSrc, int srcRoiShift, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_v_64f (  double *pSrc, int srcStride2, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_v_64f_P (  double **ppSrc, int srcRoiShift, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_32f (  float *pSrc, int srcStride0, int srcStride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_64f (  double *pSrc, int srcStride0, int srcStride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmExtract_va_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_m_32f (  float *pSrc, int srcStride1, int srcStride2, int width, int height, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_m_32f_P (  float **ppSrc, int srcRoiShift, int width, int height, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_m_64f (  double *pSrc, int srcStride1, int srcStride2, int width, int height, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_m_64f_P (  double **ppSrc, int srcRoiShift, int width, int height, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, int width, int height, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int width, int height, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, int width, int height, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, int width, int height, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int width, int height, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmFrobNorm_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, int width, int height, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float *pSrc3, int src3Stride2, int src3Len, float *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mv_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Len, float **ppSrc3, int src3RoiShift, int src3Len, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double *pSrc3, int src3Stride2, int src3Len, double *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mv_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Len, double **ppSrc3, int src3RoiShift, int src3Len, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pSrc3, int src3Stride0, int src3Stride2, int src3Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float **ppSrc3, int src3RoiShift, int src3Stride2, int src3Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppSrc3, int src3RoiShift, int src3Stride0, int src3Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pSrc3, int src3Stride0, int src3Stride2, int src3Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double **ppSrc3, int src3RoiShift, int src3Stride2, int src3Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mva_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppSrc3, int src3RoiShift, int src3Stride0, int src3Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pSrc3, int src3Stride2, int src3Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float *pSrc3, int src3Stride2, int src3Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppSrc3, int src3RoiShift, int src3Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pSrc3, int src3Stride2, int src3Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double *pSrc3, int src3Stride2, int src3Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmGaxpy_mvav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppSrc3, int src3RoiShift, int src3Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname,EntryPoint="ippmGetLibVersion")] public static extern
int* xippmGetLibVersion (    );
public static IppLibraryVersion ippmGetLibVersion() {
   return new IppLibraryVersion( xippmGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmHermite_v2_32f (  float *pSrc1, float *pSrcT1, float *pSrc2, float *pSrcT2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmHermite_v3_32f (  float *pSrc1, float *pSrcT1, float *pSrc2, float *pSrcT2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmHermite_v4_32f (  float *pSrc1, float *pSrcT1, float *pSrc2, float *pSrcT2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmIdentity_m4_32f (  float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationCatmullRom_v2_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float *pSrc4, float s, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationCatmullRom_v3_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float *pSrc4, float s, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationCatmullRom_v4_32f (  float *pSrc1, float *pSrc2, float *pSrc3, float *pSrc4, float s, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationLinear_v2_32f (  float *pSrc1, float *pSrc2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationLinear_v3_32f (  float *pSrc1, float *pSrc2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInterpolationLinear_v4_32f (  float *pSrc1, float *pSrc2, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_m4_32f (  float *pSrc, float *pDstDet, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pBuffer, float *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_m_32f_P (  float **ppSrc, int srcRoiShift, float *pBuffer, float **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pBuffer, double *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_m_64f_P (  double **ppSrc, int srcRoiShift, double *pBuffer, double **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pBuffer, float *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pBuffer, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pBuffer, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pBuffer, double *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pBuffer, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmInvert_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pBuffer, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmIsIdentity_m4_32f (  float *pSrc, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_v_32f (  float *pSrc, int srcStride2, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_v_32f_P (  float **ppSrc, int srcRoiShift, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_v_64f (  double *pSrc, int srcStride2, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_v_64f_P (  double **ppSrc, int srcRoiShift, double *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_32f (  float *pSrc, int srcStride0, int srcStride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_64f (  double *pSrc, int srcStride0, int srcStride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmL2Norm_va_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pDst, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float scale1, float *pSrc2, int src2Stride2, float scale2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float scale1, float *pSrc2, int src2Stride2, float scale2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float scale1, float **ppSrc2, int src2RoiShift, float scale2, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double scale1, double *pSrc2, int src2Stride2, double scale2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double scale1, double *pSrc2, int src2Stride2, double scale2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double scale1, double **ppSrc2, int src2RoiShift, double scale2, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float scale1, float *pSrc2, int src2Stride0, int src2Stride2, float scale2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float scale1, float **ppSrc2, int src2RoiShift, int src2Stride2, float scale2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float scale1, float **ppSrc2, int src2RoiShift, int src2Stride0, float scale2, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double scale1, double *pSrc2, int src2Stride0, int src2Stride2, double scale2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double scale1, double **ppSrc2, int src2RoiShift, int src2Stride2, double scale2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double scale1, double **ppSrc2, int src2RoiShift, int src2Stride0, double scale2, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vv_32f (  float *pSrc1, int src1Stride2, float scale1, float *pSrc2, int src2Stride2, float scale2, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vv_32f_P (  float **ppSrc1, int src1RoiShift, float scale1, float **ppSrc2, int src2RoiShift, float scale2, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vv_64f (  double *pSrc1, int src1Stride2, double scale1, double *pSrc2, int src2Stride2, double scale2, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLComb_vv_64f_P (  double **ppSrc1, int src1RoiShift, double scale1, double **ppSrc2, int src2RoiShift, double scale2, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int *pSrcIndex, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int *pSrcIndex, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int *pSrcIndex, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int *pSrcIndex, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int *pSrcIndex, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int *pSrcIndex, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mv_32f_P (  float **ppSrc1, int src1RoiShift, int *pSrcIndex, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mv_64f_P (  double **ppSrc1, int src1RoiShift, int *pSrcIndex, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_32f_P (  float **ppSrc1, int src1RoiShift, int *pSrcIndex, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int *pSrcIndex, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUBackSubst_mva_64f_P (  double **ppSrc1, int src1RoiShift, int *pSrcIndex, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_m_32f (  float *pSrc, int srcStride1, int srcStride2, int *pDstIndex, float *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_m_32f_P (  float **ppSrc, int srcRoiShift, int *pDstIndex, float **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_m_64f (  double *pSrc, int srcStride1, int srcStride2, int *pDstIndex, double *pDst, int dstStride1, int dstStride2, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_m_64f_P (  double **ppSrc, int srcRoiShift, int *pDstIndex, double **ppDst, int dstRoiShift, int widthHeight );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, int *pDstIndex, float *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int *pDstIndex, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, int *pDstIndex, float **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, int *pDstIndex, double *pDst, int dstStride0, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int *pDstIndex, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLUDecomp_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, int *pDstIndex, double **ppDst, int dstRoiShift, int dstStride0, int widthHeight, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLenSqr_v2_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLenSqr_v3_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLenSqr_v4_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLen_v2_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLen_v3_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLen_v4_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_32f (  float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_32f_L (  float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_32f_P (  float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_64f (  double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_64f_L (  double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLoadIdentity_ma_64f_P (  double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLookAtLH_m4_32f (  float *pSrcEye, float *pSrcAt, float *pSrcUp, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmLookAtRH_m4_32f (  float *pSrcEye, float *pSrcAt, float *pSrcUp, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMax_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMax_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMax_v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMin_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMin_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMin_v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMulC_v2_32f (  float *pSrc, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMulC_v3_32f (  float *pSrc, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMulC_v4_32f (  float *pSrc, float scale, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMulTranspose_m4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float val, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double val, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mac_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mata_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mc_32f (  float *pSrc, int srcStride1, int srcStride2, float val, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mc_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mc_64f (  double *pSrc, int srcStride1, int srcStride2, double val, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mc_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mm4x4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mm_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mm_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mma_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mt_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mt_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mt_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mt_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mta_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mv_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Len, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mv_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Len, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_mva_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float val, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double val, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tac_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tata_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tc_32f (  float *pSrc, int srcStride1, int srcStride2, float val, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tc_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tc_64f (  double *pSrc, int srcStride1, int srcStride2, double val, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tc_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tm_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tm_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tma_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tt_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tt_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Width, int src2Height, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tt_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tt_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Width, int src2Height, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tta_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Width, int src2Height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride2, int src2Len, float *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tv_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Len, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride2, int src2Len, double *pDst, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tv_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Len, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float *pSrc2, int src2Stride0, int src2Stride2, int src2Len, float *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, float **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_32f_P (  float **ppSrc1, int src1RoiShift, int src1Width, int src1Height, float **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double *pSrc2, int src2Stride0, int src2Stride2, int src2Len, double *pDst, int dstStride0, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride2, int src2Len, double **ppDst, int dstRoiShift, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_tva_64f_P (  double **ppSrc1, int src1RoiShift, int src1Width, int src1Height, double **ppSrc2, int src2RoiShift, int src2Stride0, int src2Len, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_32f (  float *pSrc, int srcStride0, int srcStride2, float val, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_64f (  double *pSrc, int srcStride0, int srcStride2, double val, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vac_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_32f (  float *pSrc, int srcStride0, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_64f (  double *pSrc, int srcStride0, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vaca_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vc_32f (  float *pSrc, int srcStride2, float val, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vc_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vc_64f (  double *pSrc, int srcStride2, double val, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vc_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_32f (  float *pSrc, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_32f_L (  float *pSrc, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_32f_P (  float **ppSrc, int srcRoiShift, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_64f (  double *pSrc, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_64f_L (  double *pSrc, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmMul_vca_64f_P (  double **ppSrc, int srcRoiShift, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmNormalize_v2_32f (  float *pSrc1, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmNormalize_v3_32f (  float *pSrc1, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmNormalize_v4_32f (  float *pSrc1, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmOrthoLH_m4_32f (  float w, float h, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmOrthoOffCenterLH_m4_32f (  float left, float right, float bottom, float top, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmOrthoOffCenterRH_m4_32f (  float left, float right, float bottom, float top, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmOrthoRH_m4_32f (  float w, float h, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveFovLH_m4_32f (  float fovy, float aspect, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveFovRH_m4_32f (  float fovy, float aspect, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveLH_m4_32f (  float w, float h, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveOffCenterLH_m4_32f (  float left, float right, float bottom, float top, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveOffCenterRH_m4_32f (  float left, float right, float bottom, float top, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmPerspectiveRH_m4_32f (  float w, float h, float zNear, float zFar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmProject_m4v3_32f (  float *pWorld, float *pView, float *pProj, float *pSrc, float vpWidth, float vpHeight, float vpZnear, float vpZfar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmProject_m4v3a_32f (  float *pWorld, float *pView, float *pProj, float *pSrc, int srcStride0, float vpWidth, float vpHeight, float vpZnear, float vpZfar, float *pDst, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pBuffer, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pBuffer, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float *pBuffer, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pBuffer, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pBuffer, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double *pBuffer, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mv_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pBuffer, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mv_32f_P (  float **ppSrc1, int src1RoiShift, float *pBuffer, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mv_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pBuffer, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mv_64f_P (  double **ppSrc1, int src1RoiShift, double *pBuffer, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pBuffer, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float *pBuffer, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_32f_P (  float **ppSrc1, int src1RoiShift, float *pBuffer, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pBuffer, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double *pBuffer, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRBackSubst_mva_64f_P (  double **ppSrc1, int src1RoiShift, double *pBuffer, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_m_32f (  float *pSrc, int srcStride1, int srcStride2, float *pBuffer, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_m_32f_P (  float **ppSrc, int srcRoiShift, float *pBuffer, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_m_64f (  double *pSrc, int srcStride1, int srcStride2, double *pBuffer, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_m_64f_P (  double **ppSrc, int srcRoiShift, double *pBuffer, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, float *pBuffer, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, float *pBuffer, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pBuffer, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, double *pBuffer, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, double *pBuffer, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmQRDecomp_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pBuffer, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmRotationAxis_v3m4_32f (  float *pSrc, float angle, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmRotationX_m4_32f (  float angle, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmRotationY_m4_32f (  float angle, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmRotationYawPitchRoll_m4_32f (  float yaw, float pitch, float roll, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmRotationZ_m4_32f (  float angle, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float scale, float *pSrc2, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float scale, float *pSrc2, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float scale, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double scale, double *pSrc2, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double scale, double *pSrc2, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double scale, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float scale, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float scale, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float scale, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double scale, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double scale, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double scale, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vv_32f (  float *pSrc1, int src1Stride2, float scale, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vv_32f_P (  float **ppSrc1, int src1RoiShift, float scale, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vv_64f (  double *pSrc1, int src1Stride2, double scale, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vv_64f_P (  double **ppSrc1, int src1RoiShift, double scale, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_32f (  float *pSrc1, int src1Stride2, float scale, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_32f_L (  float *pSrc1, int src1Stride2, float scale, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_32f_P (  float **ppSrc1, int src1RoiShift, float scale, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_64f (  double *pSrc1, int src1Stride2, double scale, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_64f_L (  double *pSrc1, int src1Stride2, double scale, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSaxpy_vva_64f_P (  double **ppSrc1, int src1RoiShift, double scale, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmScaling_m4_32f (  float sx, float sy, float sz, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_32f (  float *pSrc, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_32f_L (  float *pSrc, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_32f_P (  float **ppSrc, int srcRoiShift, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_64f (  double *pSrc, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_64f_L (  double *pSrc, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cav_64f_P (  double **ppSrc, int srcRoiShift, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_32f (  float *pSrc, int srcStride0, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_64f (  double *pSrc, int srcStride0, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cava_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cv_32f (  float *pSrc, int srcStride2, float val, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cv_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cv_64f (  double *pSrc, int srcStride2, double val, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cv_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_32f (  float *pSrc, int srcStride0, int srcStride2, float val, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_64f (  double *pSrc, int srcStride0, int srcStride2, double val, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_cva_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mata_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mm_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mm_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mma_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mt_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mt_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mt_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mt_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_mta_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tam_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tama_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tat_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_32f (  float *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_64f (  double *pSrc1, int src1Stride0, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tata_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tm_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tm_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tm_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tm_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tma_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tt_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride1, int src2Stride2, float *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tt_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tt_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride1, int src2Stride2, double *pDst, int dstStride1, int dstStride2, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tt_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int width, int height );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_32f (  float *pSrc1, int src1Stride1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, float *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_32f_L (  float *pSrc1, int src1Stride1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_64f (  double *pSrc1, int src1Stride1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride1, int src2Stride2, double *pDst, int dstStride0, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_64f_L (  double *pSrc1, int src1Stride1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride1, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_tta_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int width, int height, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_v2_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_v3_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_v4_32f (  float *pSrc1, float *pSrc2, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_32f (  float *pSrc, int srcStride0, int srcStride2, float val, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float val, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float val, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_64f (  double *pSrc, int srcStride0, int srcStride2, double val, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double val, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vac_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double val, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_32f (  float *pSrc, int srcStride0, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_32f_L (  float **ppSrc, int srcRoiShift, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_64f (  double *pSrc, int srcStride0, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_64f_L (  double **ppSrc, int srcRoiShift, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vaca_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float *pSrc2, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double *pSrc2, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vav_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_32f (  float *pSrc1, int src1Stride0, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_32f_L (  float **ppSrc1, int src1RoiShift, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_32f_P (  float **ppSrc1, int src1RoiShift, int src1Stride0, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_64f (  double *pSrc1, int src1Stride0, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_64f_L (  double **ppSrc1, int src1RoiShift, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vava_64f_P (  double **ppSrc1, int src1RoiShift, int src1Stride0, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vc_32f (  float *pSrc, int srcStride2, float val, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vc_32f_P (  float **ppSrc, int srcRoiShift, float val, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vc_64f (  double *pSrc, int srcStride2, double val, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vc_64f_P (  double **ppSrc, int srcRoiShift, double val, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_32f (  float *pSrc, int srcStride2, float *pVal, int valStride0, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_32f_L (  float *pSrc, int srcStride2, float **ppVal, int valRoiShift, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_32f_P (  float **ppSrc, int srcRoiShift, float *pVal, int valStride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_64f (  double *pSrc, int srcStride2, double *pVal, int valStride0, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_64f_L (  double *pSrc, int srcStride2, double **ppVal, int valRoiShift, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vca_64f_P (  double **ppSrc, int srcRoiShift, double *pVal, int valStride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vv_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride2, float *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vv_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, float **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vv_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride2, double *pDst, int dstStride2, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vv_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, double **ppDst, int dstRoiShift, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_32f (  float *pSrc1, int src1Stride2, float *pSrc2, int src2Stride0, int src2Stride2, float *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_32f_L (  float *pSrc1, int src1Stride2, float **ppSrc2, int src2RoiShift, int src2Stride2, float **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_32f_P (  float **ppSrc1, int src1RoiShift, float **ppSrc2, int src2RoiShift, int src2Stride0, float **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_64f (  double *pSrc1, int src1Stride2, double *pSrc2, int src2Stride0, int src2Stride2, double *pDst, int dstStride0, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_64f_L (  double *pSrc1, int src1Stride2, double **ppSrc2, int src2RoiShift, int src2Stride2, double **ppDst, int dstRoiShift, int dstStride2, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmSub_vva_64f_P (  double **ppSrc1, int src1RoiShift, double **ppSrc2, int src2RoiShift, int src2Stride0, double **ppDst, int dstRoiShift, int dstStride0, int len, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_m_32f (  float *pSrc, int srcStride1, int srcStride2, int widthHeight, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_m_32f_P (  float **ppSrc, int srcRoiShift, int widthHeight, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_m_64f (  double *pSrc, int srcStride1, int srcStride2, int widthHeight, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_m_64f_P (  double **ppSrc, int srcRoiShift, int widthHeight, double *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, int widthHeight, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int widthHeight, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, int widthHeight, float *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, int widthHeight, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int widthHeight, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTrace_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, int widthHeight, double *pDst, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranslation_m4_32f (  float sx, float sy, float sz, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_m4_32f (  float *pSrc, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_m_32f (  float *pSrc, int srcStride1, int srcStride2, int width, int height, float *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_m_32f_P (  float **ppSrc, int srcRoiShift, int width, int height, float **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_m_64f (  double *pSrc, int srcStride1, int srcStride2, int width, int height, double *pDst, int dstStride1, int dstStride2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_m_64f_P (  double **ppSrc, int srcRoiShift, int width, int height, double **ppDst, int dstRoiShift );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_32f (  float *pSrc, int srcStride0, int srcStride1, int srcStride2, int width, int height, float *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_32f_L (  float **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int width, int height, float **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_32f_P (  float **ppSrc, int srcRoiShift, int srcStride0, int width, int height, float **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_64f (  double *pSrc, int srcStride0, int srcStride1, int srcStride2, int width, int height, double *pDst, int dstStride0, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_64f_L (  double **ppSrc, int srcRoiShift, int srcStride1, int srcStride2, int width, int height, double **ppDst, int dstRoiShift, int dstStride1, int dstStride2, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmTranspose_ma_64f_P (  double **ppSrc, int srcRoiShift, int srcStride0, int width, int height, double **ppDst, int dstRoiShift, int dstStride0, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmUnproject_m4v3_32f (  float *pWorld, float *pView, float *pProj, float *pSrc, float vpWidth, float vpHeight, float vpZnear, float vpZfar, float *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.mp.libname)] public static extern
IppStatus ippmUnproject_m4v3a_32f (  float *pWorld, float *pView, float *pProj, float *pSrc, int srcStride0, float vpWidth, float vpHeight, float vpZnear, float vpZfar, float *pDst, int dstStride0, int count );
};
};
