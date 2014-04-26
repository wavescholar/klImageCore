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

// generated automatically on Wed Mar 31 15:58:50 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppRegExpFormat {
      ippFmtASCII = 0,
      ippFmtUTF8 = 1,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppRegExpMultiState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppRegExpReplaceState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppRegExpState {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppRegExpMultiFind {
      public uint regexpDoneFlag;
      public uint regexpID;
      public int numMultiFind;
      public IppStatus status;
      public void* pFind;
   public IppRegExpMultiFind ( uint regexpDoneFlag, uint regexpID, int numMultiFind, IppStatus status, void* pFind ) {
      this.regexpDoneFlag = regexpDoneFlag;
      this.regexpID = regexpID;
      this.numMultiFind = numMultiFind;
      this.status = status;
      this.pFind = pFind;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppRegExpFind {
      public void* pFind;
      public int lenFind;
   public IppRegExpFind ( void* pFind, int lenFind ) {
      this.pFind = pFind;
      this.lenFind = lenFind;
   }};

unsafe public class ch {

   internal const string libname = "ippch-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname,EntryPoint="ippchGetLibVersion")] public static extern
int* xippchGetLibVersion (    );
public static IppLibraryVersion ippchGetLibVersion() {
   return new IppLibraryVersion( xippchGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsCompareIgnoreCaseLatin_16u (  ushort *pSrc1, ushort *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsCompareIgnoreCaseLatin_8u (  byte *pSrc1, byte *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsCompareIgnoreCase_16u (  ushort *pSrc1, ushort *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsCompare_16u (  ushort *pSrc1, ushort *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsCompare_8u (  byte *pSrc1, byte *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcatC_16u_D2L (  ushort **pSrc, int *srcLen, int numSrc, ushort delim, ushort *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcatC_8u_D2L (  byte **pSrc, int *srcLen, int numSrc, byte delim, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcat_16u (  ushort *pSrc1, int len1, ushort *pSrc2, int len2, ushort *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcat_16u_D2L (  ushort **pSrc, int *srcLen, int numSrc, ushort *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcat_8u (  byte *pSrc1, int len1, byte *pSrc2, int len2, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConcat_8u_D2L (  byte **pSrc, int *srcLen, int numSrc, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConvertUTF_16u8u (  ushort *pSrc, uint *pSrcLen, byte *pDst, uint *pDstLen, int BEFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsConvertUTF_8u16u (  byte *pSrc, uint *pSrcLen, ushort *pDst, uint *pDstLen, int BEFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsEqual_16u (  ushort *pSrc1, ushort *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsEqual_8u (  byte *pSrc1, byte *pSrc2, int len, int *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindCAny_16u (  ushort *pSrc, int len, ushort *pAnyOf, int lenAnyOf, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindCAny_8u (  byte *pSrc, int len, byte *pAnyOf, int lenAnyOf, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindC_16u (  ushort *pSrc, int len, ushort valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindC_8u (  byte *pSrc, int len, byte valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindC_Z_16u (  ushort *pSrcZ, ushort valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindC_Z_8u (  byte *pSrcZ, byte valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRevCAny_16u (  ushort *pSrc, int len, ushort *pAnyOf, int lenAnyOf, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRevCAny_8u (  byte *pSrc, int len, byte *pAnyOf, int lenAnyOf, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRevC_16u (  ushort *pSrc, int len, ushort valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRevC_8u (  byte *pSrc, int len, byte valFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRev_16u (  ushort *pSrc, int len, ushort *pFind, int lenFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFindRev_8u (  byte *pSrc, int len, byte *pFind, int lenFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFind_16u (  ushort *pSrc, int len, ushort *pFind, int lenFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFind_8u (  byte *pSrc, int len, byte *pFind, int lenFind, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFind_Z_16u (  ushort *pSrcZ, ushort *pFindZ, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsFind_Z_8u (  byte *pSrcZ, byte *pFindZ, int *pIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHashMSCS_16u32u (  ushort *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHashMSCS_8u32u (  byte *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHashSJ2_16u32u (  ushort *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHashSJ2_8u32u (  byte *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHash_16u32u (  ushort *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsHash_8u32u (  byte *pSrc, int len, uint *pHashVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsInsert_16u (  ushort *pSrc, int srcLen, ushort *pInsert, int insertLen, ushort *pDst, int startIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsInsert_16u_I (  ushort *pInsert, int insertLen, ushort *pSrcDst, int *pSrcDstLen, int startIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsInsert_8u (  byte *pSrc, int srcLen, byte *pInsert, int insertLen, byte *pDst, int startIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsInsert_8u_I (  byte *pInsert, int insertLen, byte *pSrcDst, int *pSrcDstLen, int startIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercaseLatin_16u (  ushort *pSrc, ushort *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercaseLatin_16u_I (  ushort *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercaseLatin_8u (  byte *pSrc, byte *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercaseLatin_8u_I (  byte *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercase_16u (  ushort *pSrc, ushort *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsLowercase_16u_I (  ushort *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpFind_8u (  byte *pSrc, int srcLen, IppRegExpState *pRegExpState, IppRegExpFind *pFind, int *pNumFind );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
void ippsRegExpFree (  IppRegExpState *pRegExpState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpGetSize (  char *pPattern, int *pRegExpStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpInit (  char *pPattern, char *pOptions, IppRegExpState *pRegExpState, int *pErrOffset );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpInitAlloc (  char *pPattern, char *pOptions, IppRegExpState **ppRegExpState, int *pErrOffset );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiAdd (  IppRegExpState *pRegExpState, uint regexpID, IppRegExpMultiState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiDelete (  uint regexpID, IppRegExpMultiState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiFind_8u (  byte *pSrc, int srcLen, IppRegExpMultiFind *pDstMultiFind, IppRegExpMultiState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
void ippsRegExpMultiFree (  IppRegExpMultiState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiGetSize (  uint maxPatterns, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiInit (  IppRegExpMultiState *pState, uint maxPatterns );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiInitAlloc (  IppRegExpMultiState **ppState, uint maxPatterns );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpMultiModify (  IppRegExpState *pRegExpState, uint regexpID, IppRegExpMultiState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpReplaceGetSize (  byte *pSrcReplacement, uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpReplaceInit (  byte *pSrcReplacement, IppRegExpReplaceState *pReplaceState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpReplace_8u (  byte *pSrc, int *pSrcLenOffset, byte *pDst, int *pDstLen, IppRegExpFind *pFind, int *pNumFind, IppRegExpState *pRegExpState, IppRegExpReplaceState *pReplaceState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpSetFormat (  IppRegExpFormat fmt, IppRegExpState *pRegExpState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRegExpSetMatchLimit (  int matchLimit, IppRegExpState *pRegExpState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRemove_16u (  ushort *pSrc, int srcLen, ushort *pDst, int startIndex, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRemove_16u_I (  ushort *pSrcDst, int *pSrcDstLen, int startIndex, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRemove_8u (  byte *pSrc, int srcLen, byte *pDst, int startIndex, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsRemove_8u_I (  byte *pSrcDst, int *pSrcDstLen, int startIndex, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsReplaceC_16u (  ushort *pSrc, ushort *pDst, int len, ushort oldVal, ushort newVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsReplaceC_8u (  byte *pSrc, byte *pDst, int len, byte oldVal, byte newVal );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsSplitC_16u_D2L (  ushort *pSrc, int srcLen, ushort delim, ushort **pDst, int *dstLen, int *pNumDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsSplitC_8u_D2L (  byte *pSrc, int srcLen, byte delim, byte **pDst, int *dstLen, int *pNumDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimCAny_16u (  ushort *pSrc, int srcLen, ushort *pTrim, int trimLen, ushort *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimCAny_8u (  byte *pSrc, int srcLen, byte *pTrim, int trimLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimC_16u (  ushort *pSrc, int srcLen, ushort odd, ushort *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimC_16u_I (  ushort *pSrcDst, int *pLen, ushort odd );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimC_8u (  byte *pSrc, int srcLen, byte odd, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimC_8u_I (  byte *pSrcDst, int *pLen, byte odd );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimEndCAny_16u (  ushort *pSrc, int srcLen, ushort *pTrim, int trimLen, ushort *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimEndCAny_8u (  byte *pSrc, int srcLen, byte *pTrim, int trimLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimStartCAny_16u (  ushort *pSrc, int srcLen, ushort *pTrim, int trimLen, ushort *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsTrimStartCAny_8u (  byte *pSrc, int srcLen, byte *pTrim, int trimLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercaseLatin_16u (  ushort *pSrc, ushort *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercaseLatin_16u_I (  ushort *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercaseLatin_8u (  byte *pSrc, byte *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercaseLatin_8u_I (  byte *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercase_16u (  ushort *pSrc, ushort *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.ch.libname)] public static extern
IppStatus ippsUppercase_16u_I (  ushort *pSrcDst, int len );
};
};
