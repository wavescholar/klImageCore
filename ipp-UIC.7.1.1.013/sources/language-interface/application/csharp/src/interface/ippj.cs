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

// generated automatically on Wed Mar 31 16:16:17 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppiWTSubband {
      ippWTSubbandLxLy = 0,
      ippWTSubbandLxHy = 1,
      ippWTSubbandHxLy = 2,
      ippWTSubbandHxHy = 3,
   };
   public enum IppiVLCScanType {
      ippVLCScanVert = 0,
      ippVLCScanHoriz = 1,
      ippVLCScanRaster = 2,
   };
   public enum IppiVLCAdaptType {
      ippVLCAdaptLowpass = 0,
      ippVLCAdaptHighpass = 1,
   };
   public enum IppiWTFilterFirst {
      ippWTFilterFirstLow = 0,
      ippWTFilterFirstHigh = 1,
   };
   public enum IppiMQRateAppr {
      ippMQRateApprGood = 0,
   };
   public enum IppiMQTermination {
      ippMQTermSimple = 0,
      ippMQTermNearOptimal = 1,
      ippMQTermPredictable = 2,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppiDecodeCBProgrState_JPEG2K {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDecodeHuffmanSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDecodeHuffmanState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiEncodeHuffmanSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiEncodeHuffmanState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiEncodeState_JPEG2K {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiVLCState_JPEGXR {};

unsafe public class jp {

   internal const string libname = "ippj-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiAdd128_JPEG_16s8u_C1R (  short *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR555ToYCbCr_JPEG_16u8u_C3P3R (  ushort *pSrcBGR, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R (  ushort *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGR565ToYCbCr_JPEG_16u8u_C3P3R (  ushort *pSrc, int srcStep, byte **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr411LS_MCU_8u16s_C3P3R (  byte *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr411_JPEG_8u_C3P3R (  byte *pBGR, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr411_JPEG_8u_C4P3R (  byte *pBGRA, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr422LS_MCU_8u16s_C3P3R (  byte *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr422_JPEG_8u_C3P3R (  byte *pBGR, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr422_JPEG_8u_C4P3R (  byte *pBGRA, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr444LS_MCU_8u16s_C3P3R (  byte *pSrcBGR, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr_JPEG_8u_C3P3R (  byte *pSrcBGR, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToYCbCr_JPEG_8u_C4P3R (  byte *pBGRA, int srcStep, byte **pYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiBGRToY_JPEG_8u_C3C1R (  byte *pSrcBGR, int srcStep, byte *pDstY, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiCMYKToYCCK411LS_MCU_8u16s_C4P4R (  byte *pSrcCMYK, int srcStep, short **pDstYCCK );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiCMYKToYCCK422LS_MCU_8u16s_C4P4R (  byte *pSrcCMYK, int srcStep, short **pDstYCCK );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiCMYKToYCCK444LS_MCU_8u16s_C4P4R (  byte *pSrcCMYK, int srcStep, short **pDstYCCK );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiCMYKToYCCK_JPEG_8u_C4P4R (  byte *pSrcCMYK, int srcStep, byte **pDstYCCK, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiCMYKToYCCK_JPEG_8u_P4R (  byte **pSrcCMYK, int srcStep, byte **pDstYCCK, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantFwd8x8LS_JPEG_16u16s_C1R (  ushort *pSrc, int srcStep, short *pDst, float *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantFwd8x8LS_JPEG_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, ushort *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantFwd8x8_JPEG_16s_C1 (  short *pSrc, short *pDst, ushort *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantFwd8x8_JPEG_16s_C1I (  short *pSrcDst, ushort *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8LS_1x1_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8LS_2x2_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8LS_4x4_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8LS_JPEG_16s16u_C1R (  short *pSrc, ushort *pDst, int dstStep, float *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8To2x2LS_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8To4x4LS_JPEG_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8_JPEG_16s_C1 (  short *pSrc, short *pDst, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDCTQuantInv8x8_JPEG_16s_C1I (  short *pSrcDst, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrAttach_JPEG2K_32s_C1R (  int *pDst, int dstStep, IppiSize codeBlockSize, IppiDecodeCBProgrState_JPEG2K *pState, IppiWTSubband subband, int sfBits, int codeStyleFlags );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrFree_JPEG2K (  IppiDecodeCBProgrState_JPEG2K *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrGetCurBitPlaneNum_JPEG2K (  IppiDecodeCBProgrState_JPEG2K *pState, int *pBitPlaneNum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrGetPassCounter_JPEG2K (  IppiDecodeCBProgrState_JPEG2K *state, int *pNOfResidualPasses );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrGetStateSize_JPEG2K (  IppiSize codeBlockMaxSize, int *pStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrInitAlloc_JPEG2K (  IppiDecodeCBProgrState_JPEG2K **pState, IppiSize codeBlockMaxSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrInit_JPEG2K (  IppiDecodeCBProgrState_JPEG2K *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrSetPassCounter_JPEG2K (  int nOfPasses, IppiDecodeCBProgrState_JPEG2K *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCBProgrStep_JPEG2K (  byte *pSrc, int srcLen, IppiDecodeCBProgrState_JPEG2K *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeCodeBlock_JPEG2K_1u32s_C1R (  byte *pSrc, int *pDst, int dstStep, IppiSize codeBlockSize, IppiWTSubband subband, int sfBits, int nOfPasses, int *pTermPassLen, int nOfTermPasses, int codeStyleFlags, int *pErrorBitPlane, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeGetBufSize_JPEG2K (  IppiSize codeBlockMaxSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1 (  byte *pSrc, int srcLenBytes, int *pSrcCurrPos, short *pDst, int *pMarker, int Ss, int Se, int Al, IppiDecodeHuffmanSpec *pAcTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1 (  byte *pSrc, int srcLenBytes, int *pSrcCurrPos, short *pDst, int *pMarker, int Ss, int Se, int Al, IppiDecodeHuffmanSpec *pAcTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1 (  byte *pSrc, int srcLenBytes, int *pSrcCurrPos, short *pDst, short *pLastDC, int *pMarker, int Al, IppiDecodeHuffmanSpec *pDcTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1 (  byte *pSrc, int srcLenBytes, int *pSrcCurrPos, short *pDst, int *pMarker, int Al, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_Direct_JPEG_1u16s_C1 (  byte *pSrc, int *pSrcBitsLen, short *pDst, short *pLastDC, int *pMarker, uint *pPrefetchedBits, int *pNumValidPrefetchedBits, IppiDecodeHuffmanSpec *pDcTable, IppiDecodeHuffmanSpec *pAcTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffman8x8_JPEG_1u16s_C1 (  byte *pSrc, int srcLenBytes, int *pSrcCurrPos, short *pDst, short *pLastDC, int *pMarker, IppiDecodeHuffmanSpec *pDcTable, IppiDecodeHuffmanSpec *pAcTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanOne_JPEG_1u16s_C1 (  byte *pSrc, int nSrcLenBytes, int *pSrcCurrPos, short *pDst, int *pMarker, IppiDecodeHuffmanSpec *pDecHuffTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanRow_JPEG_1u16s_C1P4 (  byte *pSrc, int nSrcLenBytes, int *pSrcCurrPos, short **pDst, int nDstLen, int nDstRows, int *pMarker, IppiDecodeHuffmanSpec **pDecHuffTable, IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanSpecFree_JPEG_8u (  IppiDecodeHuffmanSpec *pDecHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanSpecGetBufSize_JPEG_8u (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanSpecInitAlloc_JPEG_8u (  byte *pListBits, byte *pListVals, IppiDecodeHuffmanSpec **ppDecHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanSpecInit_JPEG_8u (  byte *pListBits, byte *pListVals, IppiDecodeHuffmanSpec *pDecHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanStateFree_JPEG_8u (  IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanStateGetBufSize_JPEG_8u (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanStateInitAlloc_JPEG_8u (  IppiDecodeHuffmanState **ppDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDecodeHuffmanStateInit_JPEG_8u (  IppiDecodeHuffmanState *pDecHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDiffPredFirstRow_JPEG_16s_C1 (  short *pSrc, short *pDst, int width, int P, int Pt );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiDiffPredRow_JPEG_16s_C1 (  short *pSrc, short *pPrevRow, short *pDst, int width, int predictor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeFree_JPEG2K (  IppiEncodeState_JPEG2K *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeGetDist_JPEG2K (  IppiEncodeState_JPEG2K *pState, int passNumber, double *pDist );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeGetRate_JPEG2K (  IppiEncodeState_JPEG2K *pState, int passNumber, int *pRate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeGetTermPassLen_JPEG2K (  IppiEncodeState_JPEG2K *pState, int passNumber, int *pPassLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int dstLenBytes, int *pDstCurrPos, int Ss, int Se, int Al, IppiEncodeHuffmanSpec *pAcTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int dstLenBytes, int *pDstCurrPos, int Ss, int Se, int Al, IppiEncodeHuffmanSpec *pAcTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int dstLenBytes, int *pDstCurrPos, short *pLastDC, int Al, IppiEncodeHuffmanSpec *pDcTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int dstLenBytes, int *pDstCurrPos, int Al, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_Direct_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int *pDstBitsLen, short *pLastDC, IppiEncodeHuffmanSpec *pDcTable, IppiEncodeHuffmanSpec *pAcTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffman8x8_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int dstLenBytes, int *pDstCurrPos, short *pLastDC, IppiEncodeHuffmanSpec *pDcTable, IppiEncodeHuffmanSpec *pAcTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanOne_JPEG_16s1u_C1 (  short *pSrc, byte *pDst, int nDstLenBytes, int *pDstCurrPos, IppiEncodeHuffmanSpec *pEncHuffTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanRawTableInit_JPEG_8u (  int *pStatistics, byte *pListBits, byte *pListVals );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanRow_JPEG_16s1u_P4C1 (  short **pSrc, int nSrcLen, int nSrcRows, byte *pDst, int nDstLenBytes, int *pDstCurrPos, IppiEncodeHuffmanSpec **pEncHuffTable, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanSpecFree_JPEG_8u (  IppiEncodeHuffmanSpec *pEncHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanSpecGetBufSize_JPEG_8u (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanSpecInitAlloc_JPEG_8u (  byte *pListBits, byte *pListVals, IppiEncodeHuffmanSpec **ppEncHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanSpecInit_JPEG_8u (  byte *pListBits, byte *pListVals, IppiEncodeHuffmanSpec *pEncHuffSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanStateFree_JPEG_8u (  IppiEncodeHuffmanState *pEncHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanStateGetBufSize_JPEG_8u (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanStateInitAlloc_JPEG_8u (  IppiEncodeHuffmanState **ppEncHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeHuffmanStateInit_JPEG_8u (  IppiEncodeHuffmanState *pEncHuffState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeInitAlloc_JPEG2K (  IppiEncodeState_JPEG2K **pState, IppiSize codeBlockMaxSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeLoadCodeBlock_JPEG2K_32s_C1R (  int *pSrc, int srcStep, IppiSize codeBlockSize, IppiEncodeState_JPEG2K *pState, IppiWTSubband subband, int magnBits, IppiMQTermination mqTermType, IppiMQRateAppr mqRateAppr, int codeStyleFlags, int *pSfBits, int *pNOfPasses, int *pNOfTermPasses );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiEncodeStoreBits_JPEG2K_1u (  byte *pDst, int *pDstLen, IppiEncodeState_JPEG2K *pState, int *pIsNotFinish );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiFLCDecode4x4_JPEGXR_1u16s_C1IR (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, short *pSrcDst, uint srcDstStep, uint numFlexBits, short trimMult, IppBool acFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiFLCDecode4x4_JPEGXR_1u32s_C1IR (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, int *pSrcDst, uint srcDstStep, uint numFlexBits, int trimMult, IppBool acFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiFLCEncode4x4_JPEGXR_16s1u_C1R (  short *pSrc, uint srcStep, int *pSrcResidual, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint numFlexBits, uint numTrimBits, IppBool acFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiFLCEncode4x4_JPEGXR_32s1u_C1R (  int *pSrc, uint srcStep, int *pSrcResidual, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint numFlexBits, uint numTrimBits, IppBool acFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiGetHuffmanStatistics8x8_ACFirst_JPEG_16s_C1 (  short *pSrc, int *pAcStatistics, int Ss, int Se, int Al, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiGetHuffmanStatistics8x8_ACRefine_JPEG_16s_C1 (  short *pSrc, int *pAcStatistics, int Ss, int Se, int Al, IppiEncodeHuffmanState *pEncHuffState, int bFlushState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiGetHuffmanStatistics8x8_DCFirst_JPEG_16s_C1 (  short *pSrc, int *pDcStatistics, short *pLastDC, int Al );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiGetHuffmanStatistics8x8_JPEG_16s_C1 (  short *pSrc, int *pDcStatistics, int *pAcStatistics, short *pLastDC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiGetHuffmanStatisticsOne_JPEG_16s_C1 (  short *pSrc, int *pHuffStatistics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTFwd_JPEG2K_16s_P3IR (  short **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTFwd_JPEG2K_32f_C3P3R (  float *pSrc, int srcStep, float **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTFwd_JPEG2K_32f_P3IR (  float **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTFwd_JPEG2K_32s_P3IR (  int **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTInv_JPEG2K_16s_P3IR (  short **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTInv_JPEG2K_32f_P3C3R (  float **pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTInv_JPEG2K_32f_P3IR (  float **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiICTInv_JPEG2K_32s_P3IR (  int **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiJoin422LS_MCU_16s8u_P3C2R (  short **pSrcMCU, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiJoinRow_TIFF_8u16u_C1 (  byte **pSrc, ushort *pDst, int dstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd16x16_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd16x16_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd16x16_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd16x16_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x16_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x16_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x16_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x16_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x8_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x8_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x8_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd8x8_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTFwd_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv16x16_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv16x16_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv16x16_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv16x16_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x16_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x16_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x16_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x16_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x8_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x8_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x8_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv8x8_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv_HDP_32s_C1IR (  int *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv_JPEGXR_16s_C1IR (  short *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv_JPEGXR_32f_C1IR (  float *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPCTInv_JPEGXR_32s_C1IR (  int *pSrcDst, uint srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiPackBitsRow_TIFF_8u_C1 (  byte *pSrc, int srcLenBytes, byte *pDst, int *pDstCurrPos, int dstLenBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiQuantFwd8x8_JPEG_16s_C1I (  short *pSrcDst, ushort *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiQuantFwdRawTableInit_JPEG_8u (  byte *pQuantRawTable, int qualityFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiQuantFwdTableInit_JPEG_8u16u (  byte *pQuantRawTable, ushort *pQuantFwdTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiQuantInv8x8_JPEG_16s_C1I (  short *pSrcDst, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiQuantInvTableInit_JPEG_8u16u (  byte *pQuantRawTable, ushort *pQuantInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTFwd_JPEG2K_16s_C3P3R (  short *pSrc, int srcStep, short **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTFwd_JPEG2K_16s_P3IR (  short **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTFwd_JPEG2K_32s_C3P3R (  int *pSrc, int srcStep, int **pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTFwd_JPEG2K_32s_P3IR (  int **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTInv_JPEG2K_16s_P3C3R (  short **pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTInv_JPEG2K_16s_P3IR (  short **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTInv_JPEG2K_32s_P3C3R (  int **pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRCTInv_JPEG2K_32s_P3IR (  int **pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGB555ToYCbCr_JPEG_16u8u_C3P3R (  ushort *pSrcRGB, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGB565ToYCbCr_JPEG_16u8u_C3P3R (  ushort *pSrcRGB, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr411LS_MCU_8u16s_C3P3R (  byte *pSrcRGB, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr411_JPEG_8u_C3P3R (  byte *pRGB, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr411_JPEG_8u_C4P3R (  byte *pRGBA, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr422LS_MCU_8u16s_C3P3R (  byte *pSrcRGB, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr422_JPEG_8u_C3P3R (  byte *pRGB, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr422_JPEG_8u_C4P3R (  byte *pRGBA, int srcStep, byte **pYCbCr, int *dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr444LS_MCU_8u16s_C3P3R (  byte *pSrcRGB, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr_JPEG_8u_C3P3R (  byte *pSrcRGB, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr_JPEG_8u_C4P3R (  byte *pRGBA, int srcStep, byte **pYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToYCbCr_JPEG_8u_P3R (  byte **pSrcRGB, int srcStep, byte **pDstYCbCr, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToY_JPEG_8u_C3C1R (  byte *pSrcRGB, int srcStep, byte *pDstY, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiRGBToY_JPEG_8u_P3C1R (  byte **pSrcRGB, int srcStep, byte *pDstY, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiReconstructPredFirstRow_JPEG_16s_C1 (  short *pSrc, short *pDst, int width, int P, int Pt );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiReconstructPredRow_JPEG_16s_C1 (  short *pSrc, short *pPrevRow, short *pDst, int width, int predictor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDown411LS_MCU_8u16s_C3P3R (  byte *pSrc, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDown422LS_MCU_8u16s_C3P3R (  byte *pSrc, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDown444LS_MCU_8u16s_C3P3R (  byte *pSrc, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDownH2V1_JPEG_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDownH2V2_JPEG_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDownRowH2V1_Box_JPEG_8u_C1 (  byte *pSrc, int srcWidth, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleDownRowH2V2_Box_JPEG_8u_C1 (  byte *pSrc1, byte *pSrc2, int srcWidth, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUp411LS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUp422LS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUp444LS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUpH2V1_JPEG_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUpH2V2_JPEG_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUpRowH2V1_Triangle_JPEG_8u_C1 (  byte *pSrc, int srcWidth, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSampleUpRowH2V2_Triangle_JPEG_8u_C1 (  byte *pSrc1, byte *pSrc2, int srcWidth, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSplit422LS_MCU_8u16s_C2P3R (  byte *pSrc, int srcStep, short **pDstMCU );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSplitRow_TIFF_16u8u_C1 (  ushort *pSrc, byte **pDst, int dstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiSub128_JPEG_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGBA_BC7_8u_C1C4R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGBA_DXT1_8u_C1C4R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGBA_DXT3_8u_C1C4R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGBA_DXT5_8u_C1C4R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGB_BC6_16s_C1C3R (  byte *pSrc, short *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRGB_BC6_16u_C1C3R (  byte *pSrc, ushort *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlockToRG_BC5_8u_C1C2R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureDecodeBlock_BC4_8u_C1R (  byte *pSrc, byte *pDst, uint dstStep, IppiSize dstRoi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlock4x4FromRGBA_BC7_8u_C4C1R (  byte *pSrc, uint srcStep, byte *pDst, uint *pMSE, uint mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlockFromRGBA_DXT1_8u_C4C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlockFromRGBA_DXT3_8u_C4C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlockFromRGBA_DXT5_8u_C4C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlockFromRG_BC5_8u_C2C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlockFromYCoCg_DXT5_8u_C3C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiTextureEncodeBlock_BC4_8u_C1R (  byte *pSrc, uint srcStep, IppiSize srcRoi, byte *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiUnpackBitsRow_TIFF_8u_C1 (  byte *pSrc, int *pSrcCurrPos, int srcLenBytes, byte *pDst, int dstLenBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCAdapt_JPEGXR (  IppiVLCState_JPEGXR *pVLCState, IppiVLCAdaptType adaptType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCDecode4x4_JPEGXR_1u16s_C1R (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, short dequantMult, short *pDst, uint dstStep, IppBool chroma, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCDecode4x4_JPEGXR_1u32s_C1R (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, int dequantMult, int *pDst, uint dstStep, IppBool chroma, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCDecodeDC420_JPEGXR_1u32s (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, int *pDstCoeffsScan, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCDecodeDC422_JPEGXR_1u32s (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, int *pDstCoeffsScan, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCDecodeDC444_JPEGXR_1u32s (  byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, int *pDstCoeffsScan, IppBool chroma, uint *pNumNonzero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncode4x4Flex_JPEGXR_16s1u_C1R (  short *pSrc, uint srcStep, IppBool chroma, uint numModelBits, uint numTrimBits, int *pDstResidual, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncode4x4Flex_JPEGXR_32s1u_C1R (  int *pSrc, uint srcStep, IppBool chroma, uint numModelBits, uint numTrimBits, int *pDstResidual, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncode4x4_JPEGXR_16s1u_C1R (  short *pSrc, uint srcStep, IppBool chroma, uint numModelBits, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncode4x4_JPEGXR_32s1u_C1R (  int *pSrc, uint srcStep, IppBool chroma, uint numModelBits, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncodeDC420_JPEGXR_32s1u (  int *pSrcCoeffsScan, uint numCoeffs, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncodeDC422_JPEGXR_32s1u (  int *pSrcCoeffsScan, uint numCoeffs, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCEncodeDC444_JPEGXR_32s1u (  int *pSrcCoeffsScan, int numCoeffs, IppBool chroma, byte **ppBitStream, uint *pBitsBuf, uint *pNumBits, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCGetStateSize_JPEGXR (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCInit_JPEGXR (  IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCScan4x4DC_JPEGXR_32s (  int *pSrcCoeffs, uint numModelBits, int *pDstResidual, int *pDstCoeffsScan, uint *pNumNonZero, IppiVLCState_JPEGXR *pVLCState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCScanReset_JPEGXR (  IppiVLCState_JPEGXR *pVLCState, IppiVLCScanType scanType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiVLCScanSet_JPEGXR (  IppiVLCState_JPEGXR *pVLCState, IppiVLCScanType scanType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdColLift_B53_JPEG2K_16s_C1 (  short *pSrc0, short *pSrc1, short *pSrc2, short *pDstLow0, short *pSrcHigh0, short *pDstHigh1, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdColLift_B53_JPEG2K_32s_C1 (  int *pSrc0, int *pSrc1, int *pSrc2, int *pDstLow0, int *pSrcHigh0, int *pDstHigh1, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdColLift_D97_JPEG2K_16s_C1 (  short *pSrc0, short *pSrc1, short *pSrc2, short *pSrcDstLow0, short *pDstLow1, short *pSrcDstHigh0, short *pSrcDstHigh1, short *pDstHigh2, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdColLift_D97_JPEG2K_32f_C1 (  float *pSrc0, float *pSrc1, float *pSrc2, float *pSrcDstLow0, float *pDstLow1, float *pSrcDstHigh0, float *pSrcDstHigh1, float *pDstHigh2, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdColLift_D97_JPEG2K_32s_C1 (  int *pSrc0, int *pSrc1, int *pSrc2, int *pSrcDstLow0, int *pDstLow1, int *pSrcDstHigh0, int *pSrcDstHigh1, int *pDstHigh2, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdCol_B53_JPEG2K_16s_C1R (  short *pSrc, int srcStep, short *pDstLow, int dstLowStep, short *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdCol_B53_JPEG2K_32s_C1R (  int *pSrc, int srcStep, int *pDstLow, int dstLowStep, int *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdCol_D97_JPEG2K_32f_C1R (  float *pSrc, int srcStep, float *pDstLow, int dstLowStep, float *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdRow_B53_JPEG2K_16s_C1R (  short *pSrc, int srcStep, short *pDstLow, int dstLowStep, short *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdRow_B53_JPEG2K_32s_C1R (  int *pSrc, int srcStep, int *pDstLow, int dstLowStep, int *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdRow_D97_JPEG2K_16s_C1R (  short *pSrc, int srcStep, short *pDstLow, int dstLowStep, short *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdRow_D97_JPEG2K_32f_C1R (  float *pSrc, int srcStep, float *pDstLow, int dstLowStep, float *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwdRow_D97_JPEG2K_32s_C1R (  int *pSrc, int srcStep, int *pDstLow, int dstLowStep, int *pDstHigh, int dstHighStep, IppiSize dstRoiSize, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_B53_JPEG2K_16s_C1IR (  short *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_B53_JPEG2K_16s_C1R (  short *pSrc, int srcStep, IppiRect *pTileRect, short **pDst, int *dstStep, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_B53_JPEG2K_32s_C1IR (  int *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_B53_JPEG2K_32s_C1R (  int *pSrc, int srcStep, IppiRect *pTileRect, int **pDst, int *dstStep, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_D97_JPEG2K_16s_C1IR (  short *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_D97_JPEG2K_16s_C1R (  short *pSrc, int srcStep, IppiRect *pTileRect, short **pDst, int *dstStep, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_D97_JPEG2K_32s_C1IR (  int *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTFwd_D97_JPEG2K_32s_C1R (  int *pSrc, int srcStep, IppiRect *pTileRect, int **pDst, int *dstStep, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_B53_JPEG2K_16s_C1IR (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_B53_JPEG2K_16s_C1R (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_B53_JPEG2K_32s_C1IR (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_B53_JPEG2K_32s_C1R (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_D97_JPEG2K_16s_C1IR (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_D97_JPEG2K_16s_C1R (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_D97_JPEG2K_32s_C1IR (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTGetBufSize_D97_JPEG2K_32s_C1R (  IppiRect *pTileRect, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvColLift_B53_JPEG2K_16s_C1 (  short *pSrcLow0, short *pSrcHigh0, short *pSrcHigh1, short *pSrc0, short *pDst1, short *pDst2, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvColLift_B53_JPEG2K_32s_C1 (  int *pSrcLow0, int *pSrcHigh0, int *pSrcHigh1, int *pSrc0, int *pDst1, int *pDst2, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvColLift_D97_JPEG2K_16s_C1 (  short *pSrcLow0, short *pSrcHigh0, short *pSrcHigh1, short *pSrc0, short *pSrcDst1, short *pSrcDst2, short *pDst3, short *pDst4, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvColLift_D97_JPEG2K_32f_C1 (  float *pSrcLow0, float *pSrcHigh0, float *pSrcHigh1, float *pSrc0, float *pSrcDst1, float *pSrcDst2, float *pDst3, float *pDst4, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvColLift_D97_JPEG2K_32s_C1 (  int *pSrcLow0, int *pSrcHigh0, int *pSrcHigh1, int *pSrc0, int *pSrcDst1, int *pSrcDst2, int *pDst3, int *pDst4, int width );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvCol_B53_JPEG2K_16s_C1R (  short *pSrcLow, int srcLowStep, short *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvCol_B53_JPEG2K_32s_C1R (  int *pSrcLow, int srcLowStep, int *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvCol_D97_JPEG2K_32f_C1R (  float *pSrcLow, int srcLowStep, float *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvRow_B53_JPEG2K_16s_C1R (  short *pSrcLow, int srcLowStep, short *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvRow_B53_JPEG2K_32s_C1R (  int *pSrcLow, int srcLowStep, int *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvRow_D97_JPEG2K_16s_C1R (  short *pSrcLow, int srcLowStep, short *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, short *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvRow_D97_JPEG2K_32f_C1R (  float *pSrcLow, int srcLowStep, float *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInvRow_D97_JPEG2K_32s_C1R (  int *pSrcLow, int srcLowStep, int *pSrcHigh, int srcHighStep, IppiSize srcRoiSize, int *pDst, int dstStep, IppiWTFilterFirst phase );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_B53_JPEG2K_16s_C1IR (  short *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_B53_JPEG2K_16s_C1R (  short **pSrc, int *srcStep, short *pDst, int dstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_B53_JPEG2K_32s_C1IR (  int *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_B53_JPEG2K_32s_C1R (  int **pSrc, int *srcStep, int *pDst, int dstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_D97_JPEG2K_16s_C1IR (  short *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_D97_JPEG2K_16s_C1R (  short **pSrc, int *srcStep, short *pDst, int dstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_D97_JPEG2K_32s_C1IR (  int *pSrcDstTile, int srcDstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiWTInv_D97_JPEG2K_32s_C1R (  int **pSrc, int *srcStep, int *pDst, int dstStep, IppiRect *pTileRect, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCCK411ToCMYKLS_MCU_16s8u_P4C4R (  short **pSrcMCU, byte *pDstCMYK, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCCK422ToCMYKLS_MCU_16s8u_P4C4R (  short **pSrcMCU, byte *pDstCMYK, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCCK444ToCMYKLS_MCU_16s8u_P4C4R (  short **pSrcMCU, byte *pDstCMYK, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCCKToCMYK_JPEG_8u_P4C4R (  byte **pSrcYCCK, int srcStep, byte *pDstCMYK, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCCKToCMYK_JPEG_8u_P4R (  byte **pSrcYCCK, int srcStep, byte **pDstCMYK, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToBGRLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToBGR_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int *srcStep, byte *pDstBGR, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToBGR_JPEG_8u_P3C4R (  byte **pYCbCr, int *srcStep, byte *pBGR, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToRGBLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstRGB, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToRGB_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int *srcStep, byte *pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr411ToRGB_JPEG_8u_P3C4R (  byte **pYCbCr, int *srcStep, byte *pRGB, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToBGRLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToBGR_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int *srcStep, byte *pDstBGR, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToBGR_JPEG_8u_P3C4R (  byte **pYCbCr, int *srcStep, byte *pBGR, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToRGBLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstRGB, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_JPEG_8u_C2C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int *srcStep, byte *pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr422ToRGB_JPEG_8u_P3C4R (  byte **pYCbCr, int *srcStep, byte *pRGB, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R (  short **pSrcMCU, ushort *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr444ToBGRLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstBGR, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCr444ToRGBLS_MCU_16s8u_P3C3R (  short **pSrcMCU, byte *pDstRGB, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToBGR555_JPEG_8u16u_P3C3R (  byte **pSrcYCbCr, int srcStep, ushort *pDstBGR, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToBGR565_JPEG_8u16u_P3C3R (  byte **pSrcYCbCr, int srcStep, ushort *pDstBGR, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToBGR_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int srcStep, byte *pDstBGR, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToBGR_JPEG_8u_P3C4R (  byte **pYCbCr, int srcStep, byte *pBGR, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToRGB555_JPEG_8u16u_P3C3R (  byte **pSrcYCbCr, int srcStep, ushort *pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToRGB565_JPEG_8u16u_P3C3R (  byte **pSrcYCbCr, int srcStep, ushort *pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToRGB_JPEG_8u_P3C3R (  byte **pSrcYCbCr, int srcStep, byte *pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToRGB_JPEG_8u_P3C4R (  byte **pYCbCr, int srcStep, byte *pRGB, int dstStep, IppiSize roiSize, byte aval );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname)] public static extern
IppStatus ippiYCbCrToRGB_JPEG_8u_P3R (  byte **pSrcYCbCr, int srcStep, byte **pDstRGB, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.jp.libname,EntryPoint="ippjGetLibVersion")] public static extern
int* xippjGetLibVersion (    );
public static IppLibraryVersion ippjGetLibVersion() {
   return new IppLibraryVersion( xippjGetLibVersion() );
}
};
};
