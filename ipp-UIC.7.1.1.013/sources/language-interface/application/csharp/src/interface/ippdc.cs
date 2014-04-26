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

// generated automatically on Wed Mar 31 16:13:13 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppLZ77HuffMode {
      IppLZ77UseFixed = 0,
      IppLZ77UseDynamic = 1,
      IppLZ77UseStored = 2,
   };
   public enum IppLZ77Flush {
      IppLZ77NoFlush = 0,
      IppLZ77SyncFlush = 1,
      IppLZ77FullFlush = 2,
      IppLZ77FinishFlush = 3,
   };
   public enum IppLZ77InflateStatus {
      IppLZ77InflateStatusInit = 0,
      IppLZ77InflateStatusHuffProcess = 1,
      IppLZ77InflateStatusLZ77Process = 2,
      IppLZ77InflateStatusFinal = 3,
   };
   public enum IppLZOMethod {
      IppLZO1XST = 0,
      IppLZO1XMT = 1,
   };
   public enum IppBWTSortAlgorithmHint {
      ippBWTItohTanakaLimSort = 0,
      ippBWTItohTanakaUnlimSort = 1,
      ippBWTSuffixSort = 2,
      ippBWTAutoSort = 3,
   };
   public enum IppGITStrategyHint {
      ippGITNoStrategy = 0,
      ippGITLeftReorder = 1,
      ippGITRightReorder = 2,
      ippGITFixedOrder = 3,
   };
   public enum IppLZ77Chcksm {
      IppLZ77NoChcksm = 0,
      IppLZ77Adler32 = 1,
      IppLZ77CRC32 = 2,
   };
   public enum IppLZ77DeflateStatus {
      IppLZ77StatusInit = 0,
      IppLZ77StatusLZ77Process = 1,
      IppLZ77StatusHuffProcess = 2,
      IppLZ77StatusFinal = 3,
   };
   public enum IppInflateMode {
      ippTYPE = 0,
      ippLEN = 1,
      ippLENEXT = 2,
   };
   public enum IppLZ77ComprLevel {
      IppLZ77FastCompr = 0,
      IppLZ77AverageCompr = 1,
      IppLZ77BestCompr = 2,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppDecodeHuffState_BZ2 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppEncodeHuffState_BZ2 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppGITState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppHuffState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppLZ77State_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppLZOState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppLZSSState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppMTFState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppRLEState_BZ2 {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppDeflateHuffCode {
      public ushort code;
      public ushort len;
   public IppDeflateHuffCode ( ushort code, ushort len ) {
      this.code = code;
      this.len = len;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppsVLCTable_32s {
      public int value;
      public int code;
      public int length;
   public IppsVLCTable_32s ( int value, int code, int length ) {
      this.value = value;
      this.code = code;
      this.length = length;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppLZ77Pair {
      public ushort length;
      public ushort offset;
   public IppLZ77Pair ( ushort length, ushort offset ) {
      this.length = length;
      this.offset = offset;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppInflateState {
      public byte* pWindow;
      public uint winSize;
      public uint tableType;
      public uint tableBufferSize;
   public IppInflateState ( byte* pWindow, uint winSize, uint tableType, uint tableBufferSize ) {
      this.pWindow = pWindow;
      this.winSize = winSize;
      this.tableType = tableType;
      this.tableBufferSize = tableBufferSize;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppDeflateFreqTable {
      public ushort freq;
      public ushort code;
   public IppDeflateFreqTable ( ushort freq, ushort code ) {
      this.freq = freq;
      this.code = code;
   }};

unsafe public class dc {

   internal const string libname = "ippdc-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname,EntryPoint="ippdcGetLibVersion")] public static extern
int* xippdcGetLibVersion (    );
public static IppLibraryVersion ippdcGetLibVersion() {
   return new IppLibraryVersion( xippdcGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsAdler32_8u (  byte *pSrc, int srcLen, uint *pAdler32 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTFwdGetBufSize_SelectSort_8u (  uint wndSize, uint *pBWTFwdBufSize, IppBWTSortAlgorithmHint sortAlgorithmHint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTFwdGetSize_8u (  int wndSize, int *pBWTFwdBuffSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTFwd_8u (  byte *pSrc, byte *pDst, int len, int *index, byte *pBWTFwdBuff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTFwd_SelectSort_8u (  byte *pSrc, byte *pDst, uint len, uint *index, byte *pBWTFwdBuf, IppBWTSortAlgorithmHint sortAlgorithmHint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTFwd_SmallBlock_8u (  byte *pSrc, byte *pDst, int len, int *index, byte *pBWTBuff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTGetSize_SmallBlock_8u (  int wndSize, int *pBuffSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTInvGetSize_8u (  int wndSize, int *pBWTInvBuffSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTInv_8u (  byte *pSrc, byte *pDst, int len, int index, byte *pBWTInvBuff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsBWTInv_SmallBlock_8u (  byte *pSrc, byte *pDst, int len, int index, byte *pBWTBuff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsCRC32C_8u (  byte *pSrc, uint srcLen, uint *pCRC32C );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsCRC32_8u (  byte *pSrc, int srcLen, uint *pCRC32 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsCRC32_BZ2_8u (  byte *pSrc, int srcLen, uint *pCRC32 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeBlockGetSize_BZ2_8u (  int blockSize, int *pBuffSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeBlock_BZ2_16u8u (  ushort *pSrc, int srcLen, byte *pDst, int *pDstLen, int index, int dictSize, byte *inUse, byte *pBuff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeGITGetSize_8u (  int maxSrcLen, int *pGITStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeGITInitAlloc_8u (  int maxSrcLen, int maxDstLen, IppGITState_8u **ppGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeGITInit_8u (  int maxDstLen, IppGITState_8u *pGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeGIT_8u (  byte *pSrc, int srcLen, byte *pDst, int *pDstLen, IppGITStrategyHint strategyHint, IppGITState_8u *pGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsDecodeHuffFree_BZ2_8u16u (  IppDecodeHuffState_BZ2 *pDecodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffGetSize_BZ2_8u16u (  int wndSize, int *pDecodeHuffStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffInitAlloc_8u (  int *codeLenTable, IppHuffState_8u **ppHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffInitAlloc_BZ2_8u16u (  int wndSize, int sizeDictionary, IppDecodeHuffState_BZ2 **ppDecodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffInit_8u (  int *codeLenTable, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffInit_BZ2_8u16u (  int sizeDictionary, IppDecodeHuffState_BZ2 *pDecodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuffOne_8u (  byte *pSrc, int srcOffsetBits, byte *pDst, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuff_8u (  byte *pSrc, int srcLen, byte *pDst, int *pDstLen, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeHuff_BZ2_8u16u (  uint *pCode, int *pCodeLenBits, byte **ppSrc, int *pSrcLen, ushort *pDst, int *pDstLen, IppDecodeHuffState_BZ2 *pDecodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77CopyState_8u (  IppLZ77State_8u *pLZ77StateSrc, IppLZ77State_8u *pLZ77StateDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77DynamicHuffFull_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77DynamicHuff_8u (  byte **ppSrc, int *pSrcLen, IppLZ77Pair **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77FixedHuffFull_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77FixedHuff_8u (  byte **ppSrc, int *pSrcLen, IppLZ77Pair **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77GetBlockType_8u (  byte **ppSrc, int *pSrcLen, IppLZ77HuffMode *pHuffMode, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77GetPairs_8u (  IppLZ77Pair **ppPairs, int *pPairsInd, int *pPairsLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77GetSize_8u (  int *pLZ77StateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77GetStatus_8u (  IppLZ77InflateStatus *pInflateStatus, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77InitAlloc_8u (  IppLZ77Chcksm checksum, IppLZ77State_8u **ppLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77Init_8u (  IppLZ77Chcksm checksum, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77Reset_8u (  IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77SetDictionary_8u (  byte *pDictionary, int dictLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77SetPairs_8u (  IppLZ77Pair *pPairs, int pairsInd, int pairsLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77SetStatus_8u (  IppLZ77InflateStatus inflateStatus, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77StoredBlock_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77StoredHuff_8u (  byte **ppSrc, int *pSrcLen, IppLZ77Pair **ppDst, int *pDstLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZ77_8u (  IppLZ77Pair **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZOSafe_8u (  byte *pSrc, uint srcLen, byte *pDst, uint *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZO_8u (  byte *pSrc, uint srcLen, byte *pDst, uint *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZSSInitAlloc_8u (  IppLZSSState_8u **ppLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZSSInit_8u (  IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeLZSS_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeRLEStateFlush_BZ2_8u (  IppRLEState_BZ2 *pRLEState, byte **ppDst, uint *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeRLEStateInit_BZ2_8u (  IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeRLEState_BZ2_8u (  byte **ppSrc, uint *pSrcLen, byte **ppDst, uint *pDstLen, IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeRLE_8u (  byte **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeRLE_BZ2_8u (  byte **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDecodeZ1Z2_BZ2_16u8u (  ushort **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDeflateDictionarySet_8u (  byte *pDictSrc, uint dictLen, int *pHashHeadDst, uint hashSize, int *pHashPrevDst, byte *pWindowDst, uint winSize, int comprLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDeflateHuff_8u (  byte *pLitSrc, ushort *pDistSrc, uint srcLen, ushort *pCode, uint *pCodeLenBits, IppDeflateHuffCode *pLitHuffCodes, IppDeflateHuffCode *pDistHuffCodes, byte *pDst, uint *pDstIdx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDeflateLZ77_8u (  byte **ppSrc, uint *pSrcLen, uint *pSrcIdx, byte *pWindow, uint winSize, int *pHashHead, int *pHashPrev, uint hashSize, IppDeflateFreqTable *pLitFreqTable, IppDeflateFreqTable *pDistFreqTable, byte *pLitDst, ushort *pDistDst, uint *pDstLen, int comprLevel, IppLZ77Flush flush );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsDeflateUpdateHash_8u (  byte *pSrc, uint srcIdx, uint srcLen, int *pHashHeadDst, uint hashSize, int *pHashPrevDst, uint winSize, int comprLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeGITGetSize_8u (  int maxSrcLen, int maxDstLen, int *pGITStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeGITInitAlloc_8u (  int maxSrcLen, int maxDstLen, IppGITState_8u **ppGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeGITInit_8u (  int maxSrcLen, int maxDstLen, IppGITState_8u *ppGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeGIT_8u (  byte *pSrc, int srcLen, byte *pDst, int *pDstLen, IppGITStrategyHint strategyHint, IppGITState_8u *pGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffFinal_8u (  byte *pDst, int *pDstLen, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsEncodeHuffFree_BZ2_16u8u (  IppEncodeHuffState_BZ2 *pEncodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffGetSize_BZ2_16u8u (  int wndSize, int *pEncodeHuffStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffInitAlloc_8u (  int *freqTable, IppHuffState_8u **ppHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffInitAlloc_BZ2_16u8u (  int wndSize, int sizeDictionary, int *freqTable, ushort *pSrc, int srcLen, IppEncodeHuffState_BZ2 **ppEncodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffInit_8u (  int *freqTable, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffInit_BZ2_16u8u (  int sizeDictionary, int *freqTable, ushort *pSrc, int srcLen, IppEncodeHuffState_BZ2 *pEncodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuffOne_8u (  byte src, byte *pDst, int dstOffsetBits, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuff_8u (  byte *pSrc, int srcLen, byte *pDst, int *pDstLen, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeHuff_BZ2_16u8u (  uint *pCode, int *pCodeLenBits, ushort **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen, IppEncodeHuffState_BZ2 *pEncodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77DynamicHuff_8u (  IppLZ77Pair **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77FixedHuff_8u (  IppLZ77Pair **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77Flush_8u (  byte **ppDst, int *pDstLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77GetPairs_8u (  IppLZ77Pair **ppPairs, int *pPairsInd, int *pPairsLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77GetSize_8u (  int *pLZ77StateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77GetStatus_8u (  IppLZ77DeflateStatus *pDeflateStatus, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77InitAlloc_8u (  IppLZ77ComprLevel comprLevel, IppLZ77Chcksm checksum, IppLZ77State_8u **ppLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77Init_8u (  IppLZ77ComprLevel comprLevel, IppLZ77Chcksm checksum, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77Reset_8u (  IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77SelectHuffMode_8u (  IppLZ77Pair *pSrc, int srcLen, IppLZ77HuffMode *pHuffMode, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77SetDictionary_8u (  byte *pDictionary, int dictLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77SetPairs_8u (  IppLZ77Pair *pPairs, int pairsInd, int pairsLen, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77SetStatus_8u (  IppLZ77DeflateStatus deflateStatus, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77StoredBlock_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZ77_8u (  byte **ppSrc, int *pSrcLen, IppLZ77Pair **ppDst, int *pDstLen, IppLZ77Flush flush, IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZOGetSize (  IppLZOMethod method, uint maxInputLen, uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZOInit_8u (  IppLZOMethod method, uint maxInputLen, IppLZOState_8u *pLZOState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZO_8u (  byte *pSrc, uint srcLen, byte *pDst, uint *pDstLen, IppLZOState_8u *pLZOState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZSSFlush_8u (  byte **ppDst, int *pDstLen, IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZSSInitAlloc_8u (  IppLZSSState_8u **ppLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZSSInit_8u (  IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeLZSS_8u (  byte **ppSrc, int *pSrcLen, byte **ppDst, int *pDstLen, IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeRLEFlush_BZ2_8u (  byte *pDst, int *pDstLen, IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeRLEInitAlloc_BZ2_8u (  IppRLEState_BZ2 **ppRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeRLEInit_BZ2_8u (  IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeRLE_8u (  byte **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeRLE_BZ2_8u (  byte **ppSrc, int *pSrcLen, byte *pDst, int *pDstLen, IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsEncodeZ1Z2_BZ2_8u16u (  byte **ppSrc, int *pSrcLen, ushort *pDst, int *pDstLen, int *freqTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsExpandDictionary_8u_I (  byte *inUse, byte *pSrcDst, int srcDstLen, int sizeDictionary );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsGITFree_8u (  IppGITState_8u *pGITState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsHuffFree_8u (  IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsHuffGetDstBuffSize_8u (  int *codeLenTable, int srcLen, int *pEncDstBuffSize, int *pDecDstBuffSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsHuffGetLenCodeTable_8u (  int *codeLenTable, IppHuffState_8u *pHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsHuffGetSize_8u (  int *pHuffStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsHuffLenCodeTablePack_8u (  int *codeLenTable, byte *pDst, int *pDstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsHuffLenCodeTableUnpack_8u (  byte *pSrc, int *pSrcLen, int *codeLenTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsInflateBuildHuffTable (  ushort *pCodeLens, uint nLitCodeLens, uint nDistCodeLens, IppInflateState *pIppInflateState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsInflate_8u (  byte **ppSrc, uint *pSrcLen, uint *pCode, uint *pCodeLenBits, uint winIdx, byte **ppDst, uint *pDstLen, uint dstIdx, IppInflateMode *pMode, IppInflateState *pIppInflateState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsLZ77Free_8u (  IppLZ77State_8u *pLZ77State );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsLZSSFree_8u (  IppLZSSState_8u *pLZSSState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsLZSSGetSize_8u (  int *pLZSSStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsMTFFree_8u (  IppMTFState_8u *pMTFState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsMTFFwd_8u (  byte *pSrc, byte *pDst, int len, IppMTFState_8u *pMTFState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsMTFGetSize_8u (  int *pMTFStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsMTFInitAlloc_8u (  IppMTFState_8u **ppMTFState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsMTFInit_8u (  IppMTFState_8u *pMTFState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsMTFInv_8u (  byte *pSrc, byte *pDst, int len, IppMTFState_8u *pMTFState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsPackHuffContext_BZ2_16u8u (  uint *pCode, int *pCodeLenBits, byte *pDst, int *pDstLen, IppEncodeHuffState_BZ2 *pEncodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsRLEFree_BZ2_8u (  IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsRLEGetInUseTable_8u (  byte *inUse, IppRLEState_BZ2 *pRLEState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsRLEGetSize_BZ2_8u (  int *pRLEStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsReduceDictionary_8u_I (  byte *inUse, byte *pSrcDst, int srcDstLen, int *pSizeDictionary );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsUnpackHuffContext_BZ2_8u16u (  uint *pCode, int *pCodeLenBits, byte **ppSrc, int *pSrcLen, IppDecodeHuffState_BZ2 *pDecodeHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCCountBits_16s32s (  short *pSrc, int srcLen, int *pCountBits, IppsVLCEncodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeBlock_1u16s (  byte **ppSrc, int *pSrcBitsOffset, short *pDst, int dstLen, IppsVLCDecodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsVLCDecodeFree_32s (  IppsVLCDecodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeGetSize_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeInitAlloc_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, IppsVLCDecodeSpec_32s **ppVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeInit_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, IppsVLCDecodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeOne_1u16s (  byte **ppSrc, int *pSrcBitsOffset, short *pDst, IppsVLCDecodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeUTupleBlock_1u16s (  byte **ppSrc, int *pSrcBitsOffset, short *pDst, int dstLen, IppsVLCDecodeUTupleSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsVLCDecodeUTupleFree_32s (  IppsVLCDecodeUTupleSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeUTupleGetSize_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, int numElements, int numValueBit, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeUTupleInitAlloc_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, int numElements, int numValueBit, IppsVLCDecodeUTupleSpec_32s **ppVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeUTupleInit_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSubTablesSizes, int numSubTables, int numElements, int numValueBit, IppsVLCDecodeUTupleSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCDecodeUTupleOne_1u16s (  byte **ppSrc, int *pSrcBitsOffset, short *pDst, IppsVLCDecodeUTupleSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCEncodeBlock_16s1u (  short *pSrc, int srcLen, byte **ppDst, int *pDstBitsOffset, IppsVLCEncodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
void ippsVLCEncodeFree_32s (  IppsVLCEncodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCEncodeGetSize_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCEncodeInitAlloc_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, IppsVLCEncodeSpec_32s **ppVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCEncodeInit_32s (  IppsVLCTable_32s *pInputTable, int inputTableSize, IppsVLCEncodeSpec_32s *pVLCSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.dc.libname)] public static extern
IppStatus ippsVLCEncodeOne_16s1u (  short src, byte **pDst, int *pDstBitsOffset, IppsVLCEncodeSpec_32s *pVLCSpec );
};
};
