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

// generated automatically on Wed Feb  3 16:33:28 2010

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
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFSpec_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPoly_GF8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRSDecodeSpec_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRSEncodeSpec_8u {};

unsafe public class di {

   internal const string libname = "ippdi-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname,EntryPoint="ippdiGetLibVersion")] public static extern
int* xippdiGetLibVersion (    );
public static IppLibraryVersion ippdiGetLibVersion() {
   return new IppLibraryVersion( xippdiGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFAdd_8u (  byte srcA, byte srcB, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFDiv_8u (  byte srcA, byte srcB, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFExpAlpha_8u (  byte srcPwr, byte *pDdstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFGetSize_8u (  int gfDegree, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFInit_8u (  int gfDegree, byte *pPolynomial, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFInv_8u (  byte srcA, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFLogAlpha_8u (  byte srcA, byte *pDstPwr, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFMul_8u (  byte srcA, byte srcB, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFNeg_8u (  byte srcA, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFPow_8u (  byte srcA, int srcPwr, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsGFSub_8u (  byte srcA, byte srcB, byte *pDstR, IppsGFSpec_8u *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFAdd_8u (  IppsPoly_GF8u *pSrcA, IppsPoly_GF8u *pSrcB, IppsPoly_GF8u *pDstR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFCopy_8u (  IppsPoly_GF8u *pPolyA, IppsPoly_GF8u *pPolyB );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFDerive_8u (  IppsPoly_GF8u *pSrc, IppsPoly_GF8u *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFDiv_8u (  IppsPoly_GF8u *pSrcDividend, IppsPoly_GF8u *pSrcDivisor, IppsPoly_GF8u *pDstQuotient, IppsPoly_GF8u *pDstReminder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFGCD_8u (  IppsPoly_GF8u *pSrcA, IppsPoly_GF8u *pSrcB, IppsPoly_GF8u *pDstGCD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFGetRef_8u (  byte **pDstCoeff, int *pDstDegree, IppsGFSpec_8u **pDstGF, IppsPoly_GF8u *pPoly );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFGetSize_8u (  int maxDegree, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFInit_8u (  IppsGFSpec_8u *pGF, int maxDegree, IppsPoly_GF8u *pPoly );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFIrreducible_8u (  IppsPoly_GF8u *pSrc, IppBool *pIsIrreducible );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFMod_8u (  IppsPoly_GF8u *pSrcA, IppsPoly_GF8u *pSrcB, IppsPoly_GF8u *pDstR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFMul_8u (  IppsPoly_GF8u *pSrcA, IppsPoly_GF8u *pSrcB, IppsPoly_GF8u *pDstR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFPrimitive_8u (  IppsPoly_GF8u *pSrc, IppBool isIrreducible, IppBool *pIsPrimitive );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFRoots_8u (  IppsPoly_GF8u *pSrc, byte *pRoot, int *nRoots );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFSetCoeffs_8u (  byte *pCoeff, int degree, IppsPoly_GF8u *pPoly );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFSetDegree_8u (  int degree, IppsPoly_GF8u *pPoly );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFShlC_8u (  IppsPoly_GF8u *pSrc, int nShift, IppsPoly_GF8u *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFShrC_8u (  IppsPoly_GF8u *pSrc, int nShift, IppsPoly_GF8u *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFSub_8u (  IppsPoly_GF8u *pSrcA, IppsPoly_GF8u *pSrcB, IppsPoly_GF8u *pSrcR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsPolyGFValue_8u (  IppsPoly_GF8u *pSrc, byte srcE, byte *pDstValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeBMGetBufferSize_8u (  IppsRSDecodeSpec_8u *pRS, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeBM_8u (  int *pErasureList, int erasureListLen, byte *pSrcDstCodeWord, IppsRSDecodeSpec_8u *pRS, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeEEGetBufferSize_8u (  IppsRSDecodeSpec_8u *pRS, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeEE_8u (  int *pErasureList, int erasureListLen, byte *pSrcDstCodeWord, IppsRSDecodeSpec_8u *pRS, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeGetSize_8u (  int codeLen, int dataLen, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSDecodeInit_8u (  int codeLen, int dataLen, IppsGFSpec_8u *pGF, byte root, IppsRSDecodeSpec_8u *pRS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSEncodeGetBufferSize_8u (  IppsRSEncodeSpec_8u *pRS, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSEncodeGetSize_8u (  int codeLen, int dataLen, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSEncodeInit_8u (  int codeLen, int dataLen, IppsGFSpec_8u *pGF, byte root, IppsRSEncodeSpec_8u *pRS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.di.libname)] public static extern
IppStatus ippsRSEncode_8u (  byte *pSrc, byte *pDst, IppsRSEncodeSpec_8u *pRS, byte *pBuffer );
};
};
