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

// generated automatically on Fri Jul  9 11:03:18 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppsNrMode {
      ippsNrNoUpdate = -1,
      ippsNrUpdate = 0,
      ippsNrUpdateAll = 1,
   };
   public enum IppSpchBitRate {
      IPP_SPCHBR_4750 = 0,
      IPP_SPCHBR_5150 = 1,
      IPP_SPCHBR_5300 = 2,
      IPP_SPCHBR_5900 = 3,
      IPP_SPCHBR_6300 = 4,
      IPP_SPCHBR_6600 = 5,
      IPP_SPCHBR_6700 = 6,
      IPP_SPCHBR_7400 = 7,
      IPP_SPCHBR_7950 = 8,
      IPP_SPCHBR_8850 = 9,
      IPP_SPCHBR_9600 = 10,
      IPP_SPCHBR_10200 = 11,
      IPP_SPCHBR_12200 = 12,
      IPP_SPCHBR_12650 = 13,
      IPP_SPCHBR_12800 = 14,
      IPP_SPCHBR_14250 = 15,
      IPP_SPCHBR_15850 = 16,
      IPP_SPCHBR_16000 = 17,
      IPP_SPCHBR_18250 = 18,
      IPP_SPCHBR_19850 = 19,
      IPP_SPCHBR_23050 = 20,
      IPP_SPCHBR_23850 = 21,
      IPP_SPCHBR_24000 = 22,
      IPP_SPCHBR_32000 = 23,
      IPP_SPCHBR_40000 = 24,
      IPP_SPCHBR_DTX = 25,
      IPP_SPCHBR_10400 = 26,
      IPP_SPCHBR_12000 = 27,
      IPP_SPCHBR_13600 = 28,
      IPP_SPCHBR_15200 = 29,
      IPP_SPCHBR_16800 = 30,
      IPP_SPCHBR_19200 = 31,
      IPP_SPCHBR_20800 = 32,
      IPP_SPCHBR_13330 = 33,
   };
   public enum IppsNRLevel {
      ippsNrNone = 0,
      ippsNrLow = 1,
      ippsNrMedium = 2,
      ippsNrNormal = 3,
      ippsNrHigh = 4,
      ippsNrAuto = 5,
   };
   public enum IppG722SBClass {
      IPP_G722_VOICED = 0,
      IPP_G722_UNVOICED = 1,
      IPP_G722_TRANSIENT = 2,
      IPP_G722_WEAKLY_VOICED = 3,
      IPP_G722_VUV_TRANSITION = 4,
   };
   public enum IppsNrSmoothMode {
      ippsNrSmoothDynamic = -1,
      ippsNrSmoothStatic = 0,
      ippsNrSmoothOff = 1,
   };
   public enum IppPCMLaw {
      IPP_PCM_MULAW = 0,
      IPP_PCM_ALAW = 1,
      IPP_PCM_LINEAR = 2,
   };
   public enum IppPCMFrequency {
      IPP_PCM_FREQ_8000 = 8000,
      IPP_PCM_FREQ_16000 = 16000,
      IPP_PCM_FREQ_22050 = 22050,
      IPP_PCM_FREQ_32000 = 32000,
      IPP_PCM_FREQ_11025 = 11025,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppsALCState_G169_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsAdaptiveCodebookDecodeState_AMRWB_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsCombinedFilterState_G728_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDecoderState_G722_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDecoderState_G726_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsEncoderState_G722_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsEncoderState_G726_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsFilterHighpassState_G7291_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsFilterNoiseState_EC_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsFilterNoiseState_RTA_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsFullbandControllerState_EC_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsFullbandControllerState_EC_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGenerateExcitationState_G7291_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHighPassFilterState_AMRWB_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsIIRState_G728_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPhaseDispersion_State_G729D {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPostFilterAdapterState_G728 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPostFilterState_G728_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPostFilterState_RTA_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsQMFState_G7291_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsQMFState_MSRTA_32s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsQMFState_RTA_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsShortTermPostFilterState_MSRTA_32s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSubbandControllerDTState_EC_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSubbandControllerState_EC_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSubbandControllerState_EC_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSubbandProcessState_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSubbandProcessState_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSynthesisFilterState_G728_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsToneDetectState_EC_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsToneDetectState_EC_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsVADState_AMRWB_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsWinHybridState_G728_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsWinHybridState_G729E_16s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsWinHybridState_G729E_32f {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct Ipp32s_EC_Sfs {
      public int val;
      public int sf;
   public Ipp32s_EC_Sfs ( int val, int sf ) {
      this.val = val;
      this.sf = sf;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppGSMAMRVad1State {
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
      public short[] pPrevSignalSublevel;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
      public short[] pPrevSignalLevel;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
      public short[] pPrevAverageLevel;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
      public short[] pBkgNoiseEstimate;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
      public short[] pFifthFltState;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
      public short[] pThirdFltState;
      public short burstCount;
      public short hangCount;
      public short statCount;
      public short vadReg;
      public short complexHigh;
      public short complexLow;
      public short complexHangTimer;
      public short complexHangCount;
      public short complexWarning;
      public short corrHp;
      public short pitchFlag;
   public IppGSMAMRVad1State(short burstCount, short hangCount, short statCount, short vadReg, short complexHigh, short complexLow, short complexHangTimer, short complexHangCount, short complexWarning, short corrHp, short pitchFlag ) {
      this.pPrevSignalSublevel = new short[9];
      this.pPrevSignalLevel = new short[9];
      this.pPrevAverageLevel = new short[9];
      this.pBkgNoiseEstimate = new short[9];
      this.pFifthFltState = new short[6];
      this.pThirdFltState = new short[5];
      this.burstCount = burstCount;
      this.hangCount = hangCount;
      this.statCount = statCount;
      this.vadReg = vadReg;
      this.complexHigh = complexHigh;
      this.complexLow = complexLow;
      this.complexHangTimer = complexHangTimer;
      this.complexHangCount = complexHangCount;
      this.complexWarning = complexWarning;
      this.corrHp = corrHp;
      this.pitchFlag = pitchFlag;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppGSMAMRVad2State {
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
      public int[] pEngyEstimate;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
      public int[] pNoiseEstimate;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
      public short[] pLongTermEngyDb;
      public short preEmphasisFactor;
      public short updateCount;
      public short lastUpdateCount;
      public short hysterCount;
      public short prevNormShift;
      public short shiftState;
      public short forcedUpdateFlag;
      public short ltpSnr;
      public short variabFactor;
      public short negSnrBias;
      public short burstCount;
      public short hangOverCount;
      public int frameCount;
   public IppGSMAMRVad2State(short preEmphasisFactor, short updateCount, short lastUpdateCount, short hysterCount, short prevNormShift, short shiftState, short forcedUpdateFlag, short ltpSnr, short variabFactor, short negSnrBias, short burstCount, short hangOverCount, int frameCount ) {
      this.pEngyEstimate = new int[16];
      this.pNoiseEstimate = new int[16];
      this.pLongTermEngyDb = new short[16];
      this.preEmphasisFactor = preEmphasisFactor;
      this.updateCount = updateCount;
      this.lastUpdateCount = lastUpdateCount;
      this.hysterCount = hysterCount;
      this.prevNormShift = prevNormShift;
      this.shiftState = shiftState;
      this.forcedUpdateFlag = forcedUpdateFlag;
      this.ltpSnr = ltpSnr;
      this.variabFactor = variabFactor;
      this.negSnrBias = negSnrBias;
      this.burstCount = burstCount;
      this.hangOverCount = hangOverCount;
      this.frameCount = frameCount;
   }};

unsafe public class sc {

   internal const string libname = "ippsc-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsACELPFixedCodebookSearch_G723_16s (  short *pSrcFixedCorr, short *pSrcMatrix, short *pDstFixedSign, short *pDstFixedPosition, short *pResultGrid, short *pDstFixedVector, short *pSearchTimes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsACELPFixedCodebookSearch_G723_32s16s (  short *pSrcFixedCorr, int *pSrcDstMatrix, short *pDstFixedSign, short *pDstFixedPosition, short *pResultGrid, short *pDstFixedVector, short *pSearchTimes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsALCGetStateSize_G169_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsALCInit_G169_16s (  IppsALCState_G169_16s *pALCMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsALCSetGain_G169_16s (  float maxGain, IppsALCState_G169_16s *pALCMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsALCSetLevel_G169_16s (  float targetLevel, float clipLevel, IppsALCState_G169_16s *pALCMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsALC_G169_16s (  short *pSrc, short *pDst, int len, IppsALCState_G169_16s *pALCMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookContribution_G729_16s (  short gain, short *pFltAdptVector, short *pSrcAdptTarget, short *pDstAdptTarget );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookContribution_G729_32f (  float gain, float *pFltAdptVector, float *pSrcAdptTarget, float *pDstAdptTarget );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecodeGetSize_AMRWB_16s (  int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecodeInit_AMRWB_16s (  IppsAdaptiveCodebookDecodeState_AMRWB_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecodeUpdate_AMRWB_16s (  int valPitchGain, int valIntPitchLag, IppsAdaptiveCodebookDecodeState_AMRWB_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecode_AMRWBE_16s (  int valAdptIndex, short *pSrcDstExcitation, short *pSrcDstPitchLag, short *pSrcDstFracPitchLag, short *pSrcDstPitchLagBounds, int subFrame, int bfi, short pitchOffset );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecode_AMRWB_16s (  int valAdptIndex, short *pFracPitchLag, short *pSrcDstExcitation, short *pResultIntPitchLag, short *pResultPrevIntPitchLagBounds, int subFrame, int bfi, int unusableFrame, IppSpchBitRate mode, IppsAdaptiveCodebookDecodeState_AMRWB_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookDecode_GSMAMR_16s (  short valAdptIndex, short *pValResultPrevIntPitchLag, short *pValResultLtpLag, short *pSrcDstExcitation, short *pResultIntPitchLag, short *pDstAdptVector, short subFrame, short bfi, short inBackgroundNoise, short voicedHangover, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGainCoeff_AMRWB_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, short *pDstGainCoeff, short *pResultAdptGain, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGainCoeffs_GSMAMR_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, short *pResultAdptGain, short *pResultAdptGainCoeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGain_G7291_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcAdptVector, short *pDstFltAdptVector, short *pResultAdptGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGain_G729A_16s (  short *pSrcAdptTarget, short *pSrcLPC, short *pSrcAdptVector, short *pDstFltAdptVector, short *pResultAdptGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGain_G729_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcAdptVector, short *pDstFltAdptVector, short *pResultAdptGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookGain_GSMAMR_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, short *pResultAdptGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_AMRWBE_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcOpenLoopLag, short *pPitchLag, short *pPitchLagBounds, short *pSrcDstExcitation, short *pFracPitchLag, short *pAdptIndex, int subFrame, IppSpchBitRate mode, short pitchOffset );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_AMRWB_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcOpenLoopLag, short *pPitchLag, short *pPitchLagBounds, short *pSrcDstExcitation, short *pFracPitchLag, short *pAdptIndex, int subFrame, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_G723 (  short valBaseDelay, short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcPrevExcitation, int *pSrcPrevError, short *pResultCloseLag, short *pResultAdptGainIndex, short subFrame, short sineDtct, IppSpchBitRate bitRate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_G7291_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcPrevExcitation, short *pDstDelay, short lagMin, short lagMax, int subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_G729A_16s (  short valOpenDelay, short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcDstPrevExcitation, short *pDstDelay, short *pDstAdptVector, short subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_G729D_16s (  short valOpenDelay, short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcDstPrevExcitation, short subFrame, short *pDstDelay );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_G729_16s (  short valOpenDelay, short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcDstPrevExcitation, short *pDstDelay, short *pDstAdptVector, short subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_GSMAMR_16s (  short *pSrcAdptTarget, short *pSrcImpulseResponse, short *pSrcOpenLoopLag, short *pValResultPrevIntPitchLag, short *pSrcDstExcitation, short *pResultFracPitchLag, short *pResultAdptIndex, short *pDstAdptVector, short subFrame, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAdaptiveCodebookSearch_RTA_32f (  float *pSrcAdptTarget, float *pSrcImpulseResponse, int subFrameSize, float *pSrcBoundary, float *pSrcDstExc, float *pSrcDstLag, int *pDstAdptIndex, float *pDstAdptContribution, int deltaSearchRange, int searchFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookDecode_AMRWB_16s (  short *pSrcIdxs, short *pDstFixedCode, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookSearchEX_GSMAMR_16s (  short valIntPitchLag, short valBoundQAdptGain, short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcDstImpulseResponse, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstEncPosSign, short subFrame, IppSpchBitRate mode, int *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookSearchL1_G7291_16s (  short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcFltAdptVector, short *pSrcDstImpulseResponse, short pitchLag, short pitchGain, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstCodebookIdx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookSearchL2_G7291_16s (  short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcImpulseResponse, short voiceFactor, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstCodebookIdx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookSearch_AMRWB_16s (  short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcDstImpulseResponse, short *pDstFixedVector, short *pDstFltFixedVector, IppSpchBitRate mode, short *pDstIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAlgebraicCodebookSearch_GSMAMR_16s (  short valIntPitchLag, short valBoundQAdptGain, short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcDstImpulseResponse, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstEncPosSign, short subFrame, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorrLagMax_32f (  float *pSrc, int len, int lowerLag, int upperLag, float *pMax, int *maxLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorrLagMax_Fwd_16s (  short *pSrc, int len, int lowerLag, int upperLag, int *pMax, int *maxLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorrLagMax_Inv_16s (  short *pSrc, int len, int lowerLag, int upperLag, int *pMax, int *maxLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_16s32s (  short *pSrc, int srcLen, int *pDst, int dstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_G723_16s (  short *pSrcSpch, short *pResultAutoCorrExp, short *pDstAutoCorr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_G729B (  short *pSrcSpch, short *pResultAutoCorrExp, int *pDstAutoCorr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_GSMAMR_16s32s (  short *pSrcSpch, int *pDstAutoCorr, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_NormE_16s32s (  short *pSrc, int len, int *pDst, int lenDst, int *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_NormE_G723_16s (  short *pSrc, short *pDst, int *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoCorr_NormE_NR_16s (  short *pSrc, int len, short *pDst, int lenDst, int *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoScale_16s (  short *pSrc, short *pDst, int Len, int *pScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsAutoScale_16s_I (  short *pSrcDst, int Len, int *pScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsBandJoinUpsample_AMRWBE_16s (  short *pSrcSigLF, short *pSrcSigHF, int lenSrc, short *pDstSig, int lenDst, short *pMem, short *pInterFracMem, int *pCountSamp, short upsampScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsBandJoin_AMRWBE_16s (  short *pSrcSig2k, short *pSrcSigHi, short *pDstSignal, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsBandPassFilter_RTA_32f_I (  float *pSrcDst, int len, float *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsBandSplitDownsample_AMRWBE_16s (  short *pSrcSig, int lenSrc, short *pDstSigLF, short *pDstSigHF, int lenDst, short *pMem, short *pInterFracMem, int *pCountSamp, short upsampScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsBandSplit_AMRWBE_16s (  short *pSrcSignal, short *pSrcDstSig2k, short *pDstSigHi, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsClassifyFrame_G722_16s_I (  short *pSrcDst, short valDelay, IppG722SBClass *pSrcDstClass, IppsDecoderState_G722_16s *pDecMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCodebookSearchTCQ_G728_16s8u (  short *pSrc, short *pSrcLPC, byte *pDst, short *pSrcDstWindow, short *pSrcDstResidual, int *pSrcDstBestNode, Ipp32s_EC_Sfs invGain, Ipp32s_EC_Sfs excGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCodebookSearch_G728_16s (  short *pSrcCorr, short *pSrcEnergy, int *pDstShapeIdx, int *pDstGainIdx, short *pDstCodebookIdx, IppSpchBitRate rate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCombinedFilterGetStateSize_G728_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCombinedFilterInit_G728_16s (  IppsCombinedFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCombinedFilterZeroInput_G728_16s (  short *pSyntCoeff, short *pWgtCoeff, short *pDstWgtZIR, IppsCombinedFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCombinedFilterZeroState_G728_16s (  short *pSyntCoeff, short *pWgtCoeff, short *pSrcDstExc, short excSfs, short *pDstSpeech, short *pSpeechSfs, IppsCombinedFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCompressEnvelopTime_G7291_16s (  short *pSrcEnvelopTime, short *pSrcDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsConvPartial_16s32s (  short *pSrc1, short *pSrc2, int *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsConvPartial_16s_Sfs (  short *pSrc1, short *pSrc2, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsConvPartial_NR_16s (  short *pSrc1, short *pSrc2, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsConvPartial_NR_Low_16s (  short *pSrc1, short *pSrc2, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorrLagMax_16s (  short *pSrc1, short *pSrc2, int len, int lag, int *pMax, int *pMaxLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorrLagMax_32f64f (  float *pSrc1, float *pSrc2, int len, int lag, double *pMax, int *pMaxLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorr_16s32s_Sfs (  short *pSrc1, short *pSrc2, int len, int *pDst, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorr_NR_16s (  short *pSrc1, short *pSrc2, int len, short *pDst, int lenDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorr_NR_16s32s (  short *pSrc1, short *pSrc2, int len, int *pDst, int lenDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorr_NR_16s_Sfs (  short *pSrc1, short *pSrc2, int len, short *pDst, int lenDst, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsCrossCorr_NormM_16s (  short *pSrc1, short *pSrc2, int len, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDCTFwd_G7221_16s (  short *pSrc, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDCTFwd_G722_16s (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDCTInv_G7221_16s (  short *pSrcMLT, short *pDstSmpl, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDCTInv_G722_16s (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecDTXBuffer_AMRWB_16s (  short *pSrcSpch, short *pSrcIsf, short *pUpdateIndex, short *pSrcDstIsfBuffer, short *pSrcDstLogEnergyBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecDTXBuffer_GSMAMR_16s (  short *pSrcSpch, short *pSrcLSF, short *pValResultUpdateIndex, short *pSrcDstLSFBuffer, short *pSrcDstLogEnergyBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeAdaptiveVector_G723_16s (  short valBaseDelay, short valCloseLag, short valAdptGainIndex, short *pSrcPrevExcitation, short *pDstAdptVector, IppSpchBitRate bitRate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeAdaptiveVector_G729_16s (  short *pSrcDelay, short *pSrcDstPrevExcitation, short *pDstAdptVector );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeAdaptiveVector_G729_16s_I (  short *pSrcDelay, short *pSrcDstPrevExcitation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeAdaptiveVector_G729_32f_I (  int *pSrcDelay, float *pSrcDstPrevExcitation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeDemux_AMRWBE_16s (  short *pSrcParams, int *pPacketSizes, int *pBFI, int nPackets, short *pDst, int nSubvectors );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeGain_AMRWB_16s (  int valQIndex, int valEnergy, short *pDstPitchGain, int *pDstCodeGain, int bfi, int prevBfi, short *pSrcDstPastEnergy, short *pPrevCodeGain, short *pSrcDstPastCodeGain, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeGain_G729I_16s (  int energy, short valGainAttenuation, short *pPastEnergy, short *quaIndex, short *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeGain_G729_16s (  int energy, short *pPastEnergy, short *quaIndex, short *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeGetStateSize_G726_8u16s (  uint *pDecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecodeInit_G726_8u16s (  IppsDecoderState_G726_16s *pDecMem, IppSpchBitRate rate, IppPCMLaw law );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecode_G726_8u16s (  IppsDecoderState_G726_16s *pDecMem, byte *pSrc, short *pDst, uint len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecomposeDCTToMLT_G7221_16s (  short *pSrcSpchDecomposed, short *pSrcDstSpchDecomposedOld, short *pDstSpch, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecomposeDCTToMLT_G722_16s (  short *pSrcSpchDecomposed, short *pSrcDstSpchDecomposedOld, short *pDstSpch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecomposeMLTToDCT_G7221_16s (  short *pSrcSpch, short *pSrcDstSpchOld, short *pDstSpchDecomposed, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDecomposeMLTToDCT_G722_16s (  short *pSrcSpch, short *pSrcDstSpchOld, short *pDstSpchDecomposed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDeemphasize_AMRWBE_NR_16s_I (  short gamma, int gammaScale, short *pSrcDstSignal, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDeemphasize_AMRWB_32s16s (  short gamma, int *pSrc, short *pDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDeemphasize_AMRWB_NR_16s_I (  short gamma, short *pSrcDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDeemphasize_GSMFR_16s_I (  short *pSrcDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDotProdAutoScale_16s32s_Sfs (  short *pSrc1, short *pSrc2, int len, int *pDp, int *pSfs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDotProd_G729A_16s32s (  short *pSrc1, short *pSrc2, int len, int *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDotProd_G729A_32f (  float *pSrc1, float *pSrc2, int len, float *pDp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDownsampleFilter_G722_16s (  short *pSrc, int lenSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsDownsample_AMRWBE_16s (  short *pSrcSignal, int lenSrc, short *pDstSignal, short *pMem, int bandIdx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncDTXBuffer_AMRWB_16s (  short *pSrcSpch, short *pSrcIsp, short *pUpdateIndex, short *pSrcDstIspBuffer, short *pSrcDstLogEnergyBuffer, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncDTXBuffer_GSMAMR_16s (  short *pSrcSpch, short *pSrcLsp, short *pValResultUpdateIndex, short *pSrcDstLspBuffer, short *pSrcDstLogEnergyBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncDTXHandler_GSMAMR_16s (  short *pValResultHangOverCount, short *pValResultDtxElapseCount, short *pValResultUsedMode, short *pResultSidFlag, short vadFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncDTXSID_GSMAMR_16s (  short *pSrcLspBuffer, short *pSrcLogEnergyBuffer, short *pValResultLogEnergyIndex, short *pValResultDtxLSFRefIndex, short *pSrcDstDtxQLSFIndex, short *pSrcDstPredQErr, short *pSrcDstPredQErrMR122, short sidFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncodeGetStateSize_G726_16s8u (  uint *pEncSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncodeInit_G726_16s8u (  IppsEncoderState_G726_16s *pEncMem, IppSpchBitRate rate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncodeMux_AMRWBE_16s (  short *pSrc, int nSubvectors, int *pPacketSizes, short *pDstParams, int nPackets );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEncode_G726_16s8u (  IppsEncoderState_G726_16s *pEncMem, short *pSrc, byte *pDst, uint len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEnvelopFrequency_G7291_16s (  short *pSrc, short *pDstEnvelopFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsEnvelopTime_G7291_16s (  short *pSrc, short nls, short *pDstEnvelopTime, int subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFFTFwd_RToPerm_AMRWBE_16s (  short *pSrc, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFFTFwd_RToPerm_GSMAMR_16s_I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFFTInv_PermToR_AMRWBE_16s (  short *pSrc, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRGenMidBand_AMRWBE_16s (  short *pSrcSignal, short *pSrcSideSignal, short *pTaps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubbandAPCoeffUpdate_EC_32fc_I (  double **ppSrcStepSize, float **ppSrcFilterInput, float **ppSrcError, float **ppSrcDstCoefs, uint numSegments, uint len, uint apOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubbandCoeffUpdate_EC_32fc_I (  double *pSrcStepSize, float **ppSrcFilterInput, Ipp32fc *pSrcError, float **ppSrcDstCoefs, int numSegments, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubbandCoeffUpdate_EC_32sc_I (  Ipp32s_EC_Sfs *pSrcStepSize, int **ppSrcFilterInput, Ipp32sc *pSrcError, int **ppSrcDstCoefsQ15, int numSegments, int len, int scaleFactorCoef );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubbandLowCoeffUpdate_EC_32sc_I (  int **ppSrcFilterInput, Ipp32sc *pSrcError, int **ppSrcDstCoefsQ15, int numSegments, Ipp32sc *pDstProdStepErrQ, Ipp32s_EC_Sfs *pSrcAdaptStepSize, int startSubband, int numSubbands, int scaleFactorCoef );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubbandLow_EC_32sc_Sfs (  int **ppSrcSignal, int **ppCoefs, int numSegments, Ipp32sc *pDstSignal, int startSubband, int numSubbands, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubband_EC_32fc (  float **ppSrcSignalIn, float **ppSrcCoefs, Ipp32fc *pDstSignalOut, int numSegments, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIRSubband_EC_32sc_Sfs (  int **ppSrcSignalIn, int **ppSrcCoefs, Ipp32sc *pDstSignalOut, int numSegments, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIR_EC_16s (  short *pSrcSpchRef, short *pSrcSpch, short *pDstSpch, int len, short *pSrcCoef, int tapLength );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFIR_EC_32f (  float *pSrcSpchRef, float *pSrcSpch, float *pDstSpch, int len, float *pSrcCoef, int tapLength );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterHighband_G722_16s_I (  short *pSrcDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterHighpassGetStateSize_G7291_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterHighpassInit_G7291_16s (  IppsFilterHighpassState_G7291_16s *pState, short *pCoeff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterHighpass_G7291_16s_ISfs (  short *pSrcDst, int len, IppsFilterHighpassState_G7291_16s *pState, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterLowpass_G7291_16s_I (  short *pSrcDst, int len, short *pMem, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseDetectModerate_EC_32f64f (  float *pSrc1, float *pSrc2, double *pNoisePwr, float *pMean, int *pDstFlag, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseDetect_EC_32f64f (  float *pSrc1, double *pNoisePwr, float *pMean, int *pDstFlag, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseGetStateSize_EC_32f (  IppPCMFrequency pcmFreq, int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseGetStateSize_RTA_32f (  IppPCMFrequency pcmFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseInit_EC_32f (  IppPCMFrequency pcmFreq, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseInit_RTA_32f (  IppPCMFrequency pcmFreq, IppsFilterNoiseState_RTA_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseLevel_EC_32f (  IppsNRLevel level, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseLevel_RTA_32f (  IppsNRLevel level, IppsFilterNoiseState_RTA_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoiseSetMode_EC_32f (  IppsNrSmoothMode mode, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoise_EC_32f (  float *pSrc, float *pDst, IppsNrMode noiseFlag, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoise_EC_32f_I (  float *pSrcDst, IppsNrMode noiseFlag, IppsFilterNoiseState_EC_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoise_RTA_32f (  float *pSrc, float *pDst, IppsFilterNoiseState_RTA_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilterNoise_RTA_32f_I (  float *pSrcDst, IppsFilterNoiseState_RTA_32f *pNRStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFilteredExcitation_G729_32f (  float *pSrcImpulseResponse, float *pSrcDstPrevExcitation, int len, float valExc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookDecode_GSMAMR_16s (  short *pSrcFixedIndex, short *pDstFixedVector, short subFrame, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearchBuffer_RTA_32f (  float *pSrcFixedTarget, float *pSrcImpulseResponse, float *pDstFixedVector, int subFrameSize, int *pDstFixedIndex, float *pDstGain, int numPulses, int complexity, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearchRandom_RTA_32f (  float *pSrcResidualTarget, float *pSrcImpulseResponse, float *pDstFixedVector, int subFrameSize, int *pDstFixedIndex, float *pDstGain, int codebookSize, int nStage, int gainLimit );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729A_16s (  short *pSrcFixedCorr, short *pSrcDstMatrix, short *pDstFixedVector, short *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729A_32f (  float *pSrcFixedCorr, float *pSrcDstMatrix, float *pDstFixedVector, int *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729A_32s16s (  short *pSrcFixedCorr, int *pSrcDstMatrix, short *pDstFixedVector, short *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729D_16s (  short *pSrcFixedCorr, short *pSrcImpulseResponse, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstSign, short *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729D_32f (  float *pSrcDstFixedCorr, float *pSrcDstMatrix, float *pSrcImpulseResponse, float *pDstFixedVector, float *pDstFltFixedVector, int *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729E_16s (  short valLpMode, short *pSrcFixedTarget, short *pSrcLtpResidual, short *pSrcImpulseResponse, short *pDstFixedVector, short *pDstFltFixedVector, short *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729E_32f (  int mode, float *pSrcDstFixedCorr, float *pSrcLtpResidual, float *pSrcImpulseResponse, float *pDstFixedVector, float *pDstFltFixedVector, int *pDstFixedIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729_16s (  short *pSrcFixedCorr, short *pSrcDstMatrix, short *pDstFixedVector, short *pDstFixedIndex, short *pSearchTimes, short subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729_32f (  float *pSrcFixedCorr, float *pSrcDstMatrix, float *pDstFixedVector, int *pDstFixedIndex, int *pSearchTimes, int subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_G729_32s16s (  short *pSrcFixedCorr, int *pSrcDstMatrix, short *pDstFixedVector, short *pDstFixedIndex, short *pSearchTimes, short subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_RTA_32f (  float *pSrcFixedTarget, float *pSrcImpulseResponse, float *pDstFixedVector, int subFrameSize, int *pDstFixedIndex, float *pDstGain, int numPulses, int complexity );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFixedCodebookSearch_RTA_GetBufferSize_32f (  int *pBuferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerGetSize_EC_16s (  int frameSize, int tapLen, IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerGetSize_EC_32f (  int frameSize, int tapLen, IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerInit_EC_16s (  IppsFullbandControllerState_EC_16s *pState, int frameSize, int tapLen, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerInit_EC_32f (  IppsFullbandControllerState_EC_32f *pState, int frameSize, int tapLen, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerReset_EC_16s (  IppsFullbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerReset_EC_32f (  IppsFullbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerUpdate_EC_16s (  short *pSrcRin, short *pSrcSin, int *pDstStepSize, IppsFullbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandControllerUpdate_EC_32f (  float *pSrcRin, float *pSrcSin, float *pDstStepSize, IppsFullbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandController_EC_16s (  short *pAdaptiveFilterErr, short *pFixedFilterErr, short *pAdaptiveCoefs, short *pFixedCoefs, short *pSGain, IppsFullbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsFullbandController_EC_32f (  float *pAdaptiveFilterErr, float *pFixedFilterErr, float *pAdaptiveCoefs, float *pFixedCoefs, float *pSGain, IppsFullbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainCodebookSearch_G729D_32f (  float *pSrcCorr, float valPredictedGain, int *pCand, int *pIdxs, int tameProcess );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainCodebookSearch_G729_32f (  float *pSrcCorr, float valPredictedGain, int *pCand, int *pIdxs, int tameProcess );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainControl_G723_16s_I (  int energy, short *pSrcDst, short *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainControl_G7291_16s_I (  short *pSrc, short *pSrcDst, short *pSrcDstGain, short gain, short valGainSwitching, int *pSrcDstSmoothLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainControl_G729A_16s_I (  short *pSrc1, short *pSrcDst, short *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainControl_G729_16s_I (  short *pSrc1, short *pSrcDst, short *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainControl_G729_32f_I (  float gainScalingVal, float gainFactor, float *pSrcDst, float *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainDecodeTCX_AMRWBE_16s (  short *pSrcQuantSignal, int lenSrc, short quantIdx, int bfi, short *pSrcDstRMSval, int *pGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuantTCX_AMRWBE_16s (  short *pSrcSignal, short srcScale, short *pSrcQuantSignal, int len, int quantFlag, int *pGain, short *pQuantIdx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_AMRWBE_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, int valFormat, short *pSrcFixedVector, short *pSrcFltFixedVector, int lenSrc, short *pSrcCorr, short meanEnergy, short *pSrcDstPitchGain, short *pDstCorrFactor, int *pDstCodeGain, short *pDstQGainIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_AMRWB_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, int valFormat, short *pSrcFixedVector, short *pSrcFltFixedVector, short *pSrcCorr, short *pSrcDstEnergyErr, short *pSrcDstPitchGain, int *pDstCodeGain, int valClipFlag, short *pDstQGainIndex, int lenSrc, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_G723_16s (  short *pImp, short *pSrc, short *pDstLoc, short *pDstAmp, int *pMaxErr, short *pGrid, short *pAmp, int Np, int *isBest );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_G7291_16s (  short *pSrcFixedTarget, short *pSrcFltFixedVector, short valSrcQGain, short *pDstQGainIndex, short *pDstQGain, int subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_G729D_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, short *pSrcFixedVector, short *pSrcFltFixedVector, short *pSrcDstEnergyErr, short *pDstQGain, short *pDstQGainIndex, short tameProcess );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGainQuant_G729_16s (  short *pSrcAdptTarget, short *pSrcFltAdptVector, short *pSrcFixedVector, short *pSrcFltFixedVector, short *pSrcDstEnergyErr, short *pDstQGain, short *pDstQGainIndex, short tameProcess );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGenerateExcitationGetStateSize_G7291_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGenerateExcitationInit_G7291_16s (  IppsGenerateExcitationState_G7291_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsGenerateExcitation_G7291_16s (  short *pIntPitchLag, short *pFracPitchLag, int *pLtpPower, int *pFixPower, short *pDst, IppsGenerateExcitationState_G7291_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHarmonicFilter_16s_I (  short val, int T, short *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHarmonicFilter_32f_I (  float val, int T, float *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHarmonicFilter_NR_16s (  short val, int T, short *pSrc, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHarmonicNoiseSubtract_G723_16s_I (  short val, int T, short *pSrc, short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHarmonicSearch_G723_16s (  short valOpenDelay, short *pSrcWgtSpch, short *pResultHarmonicDelay, short *pResultHarmonicGain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterGetDlyLine_AMRWB_16s (  IppsHighPassFilterState_AMRWB_16s *pState, short *pDlyLine, int order );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterGetSize_AMRWB_16s (  int order, int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterInit_AMRWB_16s (  short *pFilterCoeffA, short *pFilterCoeffB, int order, IppsHighPassFilterState_AMRWB_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterInit_G729 (  short *pCoeff, char *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterSetDlyLine_AMRWB_16s (  short *pDlyLine, IppsHighPassFilterState_AMRWB_16s *pState, int order );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilterSize_G729 (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_AMRWB_16s_ISfs (  short *pSrcDst, int len, IppsHighPassFilterState_AMRWB_16s *pState, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_AMRWB_16s_Sfs (  short *pSrc, short *pDst, int len, IppsHighPassFilterState_AMRWB_16s *pState, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_Direct_AMRWB_16s (  short *pSrcCoeff, short *pSrc, short *pDst, int len, int valBounds );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_G723_16s (  short *pSrc, short *pDst, int *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_G729_16s_ISfs (  short *pSrcDst, int len, int scaleFactor, char *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_GSMFR_16s (  short *pSrc, short *pDst, int len, int *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHighPassFilter_RTA_32f (  float *pSrc, float *pDst, int len, int hpfOrder, IppPCMFrequency frequency, float *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsHuffmanEncode_G722_16s32u (  int category, int qntAmpEnvIndex, short *pSrcMLTCoeffs, uint *pDstCode, int *pCodeLength );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16sGetStateSize_G728_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16sInit_G728_16s (  IppsIIRState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16sLow_G729_16s (  short *pCoeffs, short *pSrc, short *pDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16s_G723_16s32s (  short *pCoeffs, short *pSrc, int *pDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16s_G723_16s_I (  short *pCoeffs, short *pSrcDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16s_G723_32s16s_Sfs (  short *pCoeffs, int *pSrc, int sFs, short *pDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16s_G728_16s (  short *pCoeffs, short *pSrcQntSpeech, short *pDstWgtSpeech, int len, IppsIIRState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsIIR16s_G729_16s (  short *pCoeffs, short *pSrc, short *pDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantDTX_AMRWB_16s (  short *pSrcIsf, short *pDstQntIsf, short *pDstIdxs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantDecodeDTX_AMRWB_16s (  short *pSrcIdxs, short *pDstQntIsf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantDecodeHighBand_AMRWBE_16s (  short *pSrcQuantIdxs, short *pSrcDstPastQISF, short *pDstQISF, int bfi, int pitchAdjust );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantDecode_AMRWBE_16s (  short *pSrcIdxs, short *pDstQntIsf, short *pSrcDstResidual, short *pSrcPrevQntIsf, short *pSrcDstIsfMemory, int bfi, int bfiNext );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantDecode_AMRWB_16s (  short *pSrcIdxs, short *pDstQntIsf, short *pSrcDstResidual, short *pSrcPrevQntIsf, short *pSrcDstIsfMemory, int bfi, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuantHighBand_AMRWBE_16s (  short *pSrcISF, short *pSrcDstPastQISF, short *pDstQISF, short *pQuantIdxs, int pitchAdjust );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFQuant_AMRWB_16s (  short *pSrcIsf, short *pSrcDstResidual, short *pDstQIsf, short *pDstQIsfIndex, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISFToISP_AMRWB_16s (  short *pSrcIsf, short *pDstIsp, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISPToISF_Norm_AMRWB_16s (  short *pSrcIsp, short *pDstIsf, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsISPToLPC_AMRWB_16s (  short *pSrcIsp, short *pDstLpc, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsImpulseResponseEnergy_G728_16s (  short *pSrcImpResp, short *pDstEnergy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsImpulseResponseTarget_GSMAMR_16s (  short *pSrcSpch, short *pSrcWgtLPC1, short *pSrcWgtLPC2, short *pSrcQLPC, short *pSrcSynFltState, short *pSrcWgtFltState, short *pDstImpulseResponse, short *pDstLpResidual, short *pDstAdptTarget );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolateC_G729_16s_Sfs (  short *pSrc1, short val1, short *pSrc2, short val2, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolateC_G729_32f (  float *pSrc1, float val1, float *pSrc2, float val2, float *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolateC_NR_16s (  short *pSrc1, short val1, int val1ScaleFactor, short *pSrc2, short val2, int val2ScaleFactor, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolateC_NR_G729_16s_Sfs (  short *pSrc1, short val1, short *pSrc2, short val2, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolate_G729_16s (  short *pSrc1, short *pSrc2, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInterpolate_GSMAMR_16s (  short *pSrc1, short *pSrc2, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsInvSqrt_32s_I (  int *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCInverseFilter_G728_16s (  short *pSrcSpeech, short *pCoeffs, short *pDstResidual, IppsPostFilterAdapterState_G728 *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToISP_AMRWBE_16s (  short *pSrcLpc, short *pSrcPrevIsp, short *pDstIsp, int lpOrder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToISP_AMRWB_16s (  short *pSrcLpc, short *pDstIsp, short *pSrcPrevIsp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSF_G723_16s (  short *pSrcLPC, short *pSrcPrevLSF, short *pDstLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_G729A_16s (  short *pSrcLPC, short *pSrcPrevLsp, short *pDstLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_G729A_32f (  float *pSrcLPC, float *pSrcPrevLsp, float *pDstLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_G729_16s (  short *pSrcLPC, short *pSrcPrevLsp, short *pDstLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_G729_32f (  float *pSrcLPC, float *pSrcPrevLsp, float *pDstLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_GSMAMR_16s (  short *pSrcLPC, short *pSrcPrevLsp, short *pDstLsp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLPCToLSP_RTA_32f (  float *pSrcLPC, float *pDstLSP, int order );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecodeErased_G729_16s (  short maIndex, short *pQuantLSFTable, short *pLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecodeErased_G729_32f (  int maIndex, float *pQuantLSFTable, float *pSrcPrevLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G723_16s (  short *quantIndex, short *pPrevLSF, int erase, short *pLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G7291_16s (  short *pQuantIndex, short *pSrcDstPrevLSF, short *pSrcDstLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G729B_16s (  short *quantIndex, short *pQuantLSFTable, short *pLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G729B_32f (  int *quantIndex, float *pQuantLSFTable, float *pDstQLsp );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G729_16s (  short *quantIndex, short *pQuantLSPTable, short *pQuantLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFDecode_G729_32f (  int *quantIndex, float *pQuantLSFTable, float *pQuantLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFQuant_G723_16s32s (  short *pSrcLSF, short *pSrcPrevLSF, int *pResultQLSFIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFQuant_G729B_16s (  short *pLSF, short *pQuantLSFTable, short *pQuantLSF, short *quantIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFQuant_G729B_32f (  float *pLSF, float *pQuantLSFTable, float *pQuantLSF, int *quantIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFQuant_G729_16s (  short *pLSF, short *pQuantLSFTable, short *pQuantLSF, short *quantIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFToLPC_G723_16s (  short *pSrcLSF, short *pDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFToLPC_G723_16s_I (  short *pLSFLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFToLSP_G729_16s (  short *pLSF, short *pLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSFToLSP_GSMAMR_16s (  short *pLSF, short *pLSP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPQuant_G729E_16s (  short *pSrcLsp, short *pSrcPrevFreq, short *pDstQLsf, short *pDstQLsp, short *pDstQLspIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPQuant_G729E_32f (  float *pSrcLsp, float *pSrcDstPrevFreq, float *pDstQLsf, float *pDstQLsp, int *pDstQLspIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPQuant_G729_16s (  short *pSrcLsp, short *pSrcDstPrevFreq, short *pDstQLsp, short *pDstQLspIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPQuant_GSMAMR_16s (  short *pSrcLsp, short *pSrcDstPrevQLSFResidual, short *pDstQLsp, short *pDstQLspIndex, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPQuant_RTA_32f (  float *pSrcLSP, float *pSrcLPC, float *pDstLSP, int order, int *pDstLSPIndex, IppPCMFrequency frequency, int complexity );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLPC_G729_16s (  short *pSrcLSP, short *pDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLPC_G729_32f (  float *pSrcLSP, float *pDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLPC_GSMAMR_16s (  short *pSrcLsp, short *pDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLPC_RTA_32f (  float *pSrcLSP, float *pDstLPC, int order );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLSF_G729_16s (  short *pLSP, short *pLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLSPToLSF_Norm_G729_16s (  short *pLSP, short *pLSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLagWindow_G729_32s_I (  int *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G723_16s (  short *pSrcAutoCorr, short *pValResultSineDtct, short *pResultResidualEnergy, short *pDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G728_16s_ISfs (  short *pSrcAutoCorr, int numSrcLPC, int order, short *pSrcDstLPC, short *pSrcDstResidualEnergy, short *pSrcDstScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G728_16s_Sfs (  short *pSrcAutoCorr, int order, short *pDstLPC, short *pDstRC1, short *pDstResidualEnergy, short *pDstScaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G729B (  int *pSrcAutoCorr, short *pDstLPC, short *pDstRc, short *pResultResidualEnergy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G729_32f (  float *pSrcAutoCorr, int order, float *pDstLpc, float *pDstRc, float *pResultResidualEnergy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_G729_32s16s (  int *pSrcAutoCorr, int order, short *pDstLPC, short *pDstRc, short *pResultResidualEnergy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_GSMAMR_32s16s (  int *pSrcAutoCorr, short *pSrcDstLPC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLevinsonDurbin_RTA_32f (  float *pSrcAutoCorr, float *pDstLPC, float *pDstRC, int order );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLongTermPostFilter_G729A_16s (  short valDelay, short *pSrcSpch, short *pSrcLPC, short *pSrcDstResidual, short *pDstFltResidual );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLongTermPostFilter_G729B_16s (  short valDelay, short *pSrcSpch, short *pSrcLPC, short *pSrcDstResidual, short *pDstFltResidual, short *pResultVoice, short frameType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsLongTermPostFilter_G729_16s (  short gammaFactor, int valDelay, short *pSrcDstResidual, short *pDstFltResidual, short *pResultVoice );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMDCTFwd_G7291_16s (  short *pSrc, short *pSrcPrev, int *pDstNorm, short *pDstMDCTCoeffs, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMDCTInv_G7291_16s (  short *pMDCTCoeffs, short *pSrcDstMDCTPrevCoeffs, short *pDst, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMDCTPostProcess_G7291_16s (  short *pSrcDstMDCTCoeffs, int numBits );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMDCTQuantFwd_G7291_16s32u (  short *pMDCTCoeffs, short *pBitsNumber, uint *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMDCTQuantInv_G7291_32u16s (  uint *pSrc, short *pBitsNumber, short *pQuantSpecEnv, short *pDstMDCTCoeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMPMLQFixedCodebookSearch_G723 (  short valBaseDelay, short *pSrcImpulseResponse, short *pSrcResidualTarget, short *pDstFixedVector, short *pResultGrid, short *pResultTrainDirac, short *pResultAmpIndex, short *pResultAmplitude, int *pResultPosition, short subFrame );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMulC_NR_16s_ISfs (  short val, short *pSrcDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMulC_NR_16s_Sfs (  short *pSrc, short val, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMulPowerC_NR_16s_Sfs (  short *pSrc, short val, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMul_NR_16s_ISfs (  short *pSrc, short *pSrcDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsMul_NR_16s_Sfs (  short *pSrc1, short *pSrc2, short *pDst, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsNLMS_EC_16s (  short *pSrcSpchRef, short *pSrcSpch, int *pStepSize, short *pDstSpch, int len, short *pSrcDstCoef, int tapLength, short *pErr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsNLMS_EC_32f (  float *pSrcSpchRef, float *pSrcSpch, float *pStepSize, float *pDstSpch, int len, float *pSrcDstCoef, int tapLength, float *pErr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearchDTXVAD1_GSMAMR_16s (  short *pSrcWgtLPC1, short *pSrcWgtLPC2, short *pSrcSpch, short *pValResultToneFlag, short *pValResultPrevMidPitchLag, short *pValResultVvalue, short *pSrcDstPrevPitchLag, short *pSrcDstPrevWgtSpch, short *pResultMaxHpCorr, short *pDstOpenLoopLag, short *pDstOpenLoopGain, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearchDTXVAD2_GSMAMR_16s32s (  short *pSrcWgtLPC1, short *pSrcWgtLPC2, short *pSrcSpch, short *pValResultPrevMidPitchLag, short *pValResultVvalue, short *pSrcDstPrevPitchLag, short *pSrcDstPrevWgtSpch, int *pResultMaxCorr, int *pResultWgtEnergy, short *pDstOpenLoopLag, short *pDstOpenLoopGain, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearchNonDTX_GSMAMR_16s (  short *pSrcWgtLPC1, short *pSrcWgtLPC2, short *pSrcSpch, short *pValResultPrevMidPitchLag, short *pValResultVvalue, short *pSrcDstPrevPitchLag, short *pSrcDstPrevWgtSpch, short *pDstOpenLoopLag, short *pDstOpenLoopGain, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_AMRWBE_16s (  short *pSrcWgtSpch, short *pSrcFltWgtSpch, short *pPrevMedPitchLag, short *pAdaptiveParam, short *pDstOpenLoopLag, short *pToneFlag, short *pDstOpenLoopGain, short *pSrcDstPrevPitchLag, short *pSrcDstLagSwitcher, int len, short minPitchLag, short maxPitchLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_AMRWB_16s (  short *pSrcWgtSpch, short *pSrcFltWgtSpch, short *pPrevMidPitchLag, short *pAdaptiveParam, short *pDstOpenLoopLag, short *pToneFlag, short *pDstOpenLoopGain, short *pSrcDstPrevPitchLag, short *pSrcDstLagSwitcher, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_G723_16s (  short *pSrc, short *bestLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_G729A_16s (  short *pSrc, short *bestLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_G729A_32f (  float *pSrc, int *bestLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsOpenLoopPitchSearch_G729_16s (  short *pSrc, short *bestLag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPhaseDispersionGetStateSize_G729D_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPhaseDispersionInit_G729D_16s (  IppsPhaseDispersion_State_G729D *pPhDMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPhaseDispersionUpdate_G729D_16s (  short valPitchGain, short valCodebookGain, IppsPhaseDispersion_State_G729D *pPhDMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPhaseDispersion_G729D_16s (  short *pSrcExcSignal, short *pDstFltExcSignal, short valCodebookGain, short valPitchGain, short *pSrcDstInnovation, IppsPhaseDispersion_State_G729D *pPhDMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPitchPeriodExtraction_G728_16s (  short *pSrcResidual, int *pPitchPeriod, IppsPostFilterAdapterState_G728 *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPitchPostFilter_G723_16s (  short valBaseDelay, short *pSrcResidual, short *pResultDelay, short *pResultPitchGain, short *pResultScalingGain, short subFrame, IppSpchBitRate bitRate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterAdapterGetStateSize_G728 (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterAdapterStateInit_G728 (  IppsPostFilterAdapterState_G728 *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterGetStateSize_G728_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterGetStateSize_RTA_32f (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterInit_G728_16s (  IppsPostFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterInit_RTA_32f (  IppsPostFilterState_RTA_32f *pStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilterLowBand_AMRWBE_16s (  short *pSrcOldPitchLag, short *pSrcOldPitchGain, short *pSrcDstSignal, short *pOldSynth, short *pOldNoise, short *pFilterScale, int pitchAdjust );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilter_G728_16s (  short gl, short glb, short kp, short tiltz, short *pCoeffs, short *pSrc, short *pDst, IppsPostFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilter_GSMAMR_16s (  short *pSrcQLPC, short *pSrcSpch, short *pValResultPrevResidual, short *pValResultPrevScalingGain, short *pSrcDstFormantFIRState, short *pSrcDstFormantIIRState, short *pDstFltSpch, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPostFilter_RTA_32f_I (  float *pSrcLPC, int order, float *pSrcDstSpeech, int len, int pfType, float *pMem, IppsPostFilterState_RTA_32f *pStateMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_32f_I (  float gamma, float *pSrcDst, int len, float *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_AMRWB_16s_ISfs (  short gamma, short *pSrcDst, int len, int ScaleFactor, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_G729A_16s (  short gamma, short *pSrc, short *pDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_G729A_16s_I (  short gamma, short *pSrcDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_GSMAMR_16s (  short gamma, short *pSrc, short *pDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsPreemphasize_GSMFR_16s (  short *pSrc, short *pDst, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFDecode_G722_16s (  short *pSrc, short *pDst, int len, short *delay );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFDecode_G7291_16s (  short *pSrcLowBand, short *pSrcHighBand, short valGainSwitching, int len, short *pDst, IppsQMFState_G7291_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFDecode_RTA_32f (  float *pSrcLow, float *pSrcHigh, int len, float *pDst, IppsQMFState_RTA_32f *pQMFMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFEncode_G722_16s (  short *pSrc, short *pDst, int len, short *delay );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFEncode_G7291_16s (  short *pSrc, int len, short *pDstLowBand, short *pDstHighBand, IppsQMFState_G7291_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFEncode_RTA_32f (  float *pSrc, int len, float *pDstLow, float *pDstHigh, IppsQMFState_RTA_32f *pQMFMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFGetStateSize_G7291_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFGetStateSize_RTA_32f (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFInit_G7291_16s (  IppsQMFState_G7291_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQMFInit_RTA_32f (  IppsQMFState_RTA_32f *pQMFMem, int nTaps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQuantLSPDecode_GSMAMR_16s (  short *pSrcQLspIndex, short *pSrcDstPrevQLSFResidual, short *pSrcDstPrevQLSF, short *pSrcDstPrevQLsp, short *pDstQLsp, short bfi, IppSpchBitRate mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQuantParam_G7291_16s (  short *pSrcDst, short *pCdbkIndices );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsQuantTCX_AMRWBE_16s (  short *pSrc, short *pDst, int nSubvectors, int nBits, short *pDstNoiseFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsRPEQuantDecode_GSMFR_16s (  short *pSrc, short ampl, short amplSfs, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsRandomNoiseExcitation_G729B_16s (  short *pSeed, short *pExc, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsRandomNoiseExcitation_G729B_16s32f (  short *pSeed, float *pExc, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsResamplePolyphase_AMRWBE_16s (  short *pSrcSignal, int lenSrc, short upFactor, short downFactor, short *pInterpFracMem, short *pMem, short *pDstSignal, int lenDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsResidualFilter_AMRWB_16s_Sfs (  short *pSrcLpc, short valLPCOrder, short *pSrcSpeech, short *pDstResidualSignal, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsResidualFilter_G729E_16s (  short *pSrcLPC, short order, short *pSrc, short *pDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsResidualFilter_G729_16s (  short *pSrc, short *pSrcLPC, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsResidualFilter_Low_16s_Sfs (  short *pSrcLpc, short valLPCOrder, short *pSrcSpeech, short *pDstResidualSignal, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMDecodeInit_G722_16s (  IppsDecoderState_G722_16s *pDecMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMDecodeStateSize_G722_16s (  int *pDecMemSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMDecodeStateUpdate_G722_16s (  short *pCoeffs, short gainFactor, int resetDelayFlag, IppsDecoderState_G722_16s *pDecMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMDecode_G722_16s (  short *pSrc, short *pDst, int len, short mode, IppsDecoderState_G722_16s *pDecMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMEncodeInit_G722_16s (  IppsEncoderState_G722_16s *pEncMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMEncodeStateSize_G722_16s (  int *pEncMemSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSBADPCMEncode_G722_16s (  short *pSrc, short *pDst, int len, IppsEncoderState_G722_16s *pEncMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSNR_AMRWBE_16s (  short *pSrcSignal, short *pSrcEstimatedSignal, int lenSrc, int lenSeg, short *pDstSNR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSchur_GSMFR_32s16s (  int *pSrc, short *pDst, int dstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShapeEnvelopFrequency_G7291_16s (  short *pSrc, short *pSrcEnvelopFreq, short *pDst, short *pDstFilterCoeffs, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShapeEnvelopTime_G7291_16s (  short *pSrc, short *pSrcEnvelopTime, short *pSrcDstGain, short *pSrcDstNorm, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShortTermAnalysisFilter_GSMFR_16s_I (  short *pRC, short *pSrcDstSpch, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShortTermPostFilter_G729A_16s (  short *pSrcLPC, short *pSrcFltResidual, short *pSrcDstSpch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShortTermPostFilter_G729_16s (  short *pSrcLPC, short *pSrcFltResidual, short *pSrcDstSpch, short *pDstImpulseResponse );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsShortTermSynthesisFilter_GSMFR_16s (  short *pRC, short *pSrcResidual, short *pDstSpch, int len, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandAPControllerUpdate_EC_32f (  float **ppSrcRinSubbandsHistory, Ipp32fc *pSrcSinSubbands, double *pDstStepSize, float learningRate, IppsSubbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandAnalysis_16s32sc_Sfs (  short *pSignal, Ipp32sc *pSubbands, IppsSubbandProcessState_16s *pState, int scalefactor, byte *pBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandAnalysis_32f32fc (  float *pSignal, Ipp32fc *pSubbands, IppsSubbandProcessState_32f *pState, byte *pBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerDTGetSize_EC_16s (  int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerDTInit_EC_16s (  IppsSubbandControllerDTState_EC_16s *pState, int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerDTReset_EC_16s (  IppsSubbandControllerDTState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerDTUpdate_EC_16s (  int **ppSrcRinSubbandsHistory, Ipp32sc *pSrcSinSubbands, Ipp32sc *pSrcFilterErr, Ipp32s_EC_Sfs *pDstStepSize, int *pIsDT, int *pDisabledNLP, int startSubband, IppsSubbandControllerDTState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerDT_EC_16s (  Ipp32sc *pSrcAdaptiveFilterErr, Ipp32sc *pSrcFixedFilterErr, int **ppDstAdaptiveCoefs, int **ppDstFixedCoefs, long *pSrcDstFilterPwr, int *pDstStsAdapt, long pwrDelta, int filterUpdateEnabled, int adaptationEnabled, int startSubband, IppsSubbandControllerDTState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerGetSize_EC_16s (  int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerGetSize_EC_32f (  int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerInit_EC_16s (  IppsSubbandControllerState_EC_16s *pState, int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerInit_EC_32f (  IppsSubbandControllerState_EC_32f *pState, int numSubbands, int frameSize, int numSegments, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerReset_EC_16s (  IppsSubbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerReset_EC_32f (  IppsSubbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerUpdate_EC_16s (  short *pSrcRin, short *pSrcSin, int **ppSrcRinSubbandsHistory, Ipp32sc *pSrcSinSubbands, Ipp32s_EC_Sfs *pDstStepSize, IppsSubbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandControllerUpdate_EC_32f (  float *pSrcRin, float *pSrcSin, float **ppSrcRinSubbandsHistory, Ipp32fc *pSrcSinSubbands, double *pDstStepSize, IppsSubbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandController_EC_16s (  Ipp32sc *pSrcAdaptiveFilterErr, Ipp32sc *pSrcFixedFilterErr, int **ppDstAdaptiveCoefs, int **ppDstFixedCoefs, short *pDstSGain, IppsSubbandControllerState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandController_EC_32f (  Ipp32fc *pSrcAdaptiveFilterErr, Ipp32fc *pSrcFixedFilterErr, float **ppDstAdaptiveCoefs, float **ppDstFixedCoefs, float *pDstSGain, IppsSubbandControllerState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandProcessGetSize_16s (  int order, int windowLen, int *pSize, int *pInitBufSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandProcessGetSize_32f (  int order, int windowLen, int *pSize, int *pInitBufSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandProcessInit_16s (  IppsSubbandProcessState_16s *state, int order, int frameSize, int windowLen, short *pWindow, byte *pInitBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandProcessInit_32f (  IppsSubbandProcessState_32f *state, int order, int frameSize, int windowLen, float *pWindow, byte *pInitBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandSynthesis_32fc32f (  Ipp32fc *pSubbands, float *pSignal, IppsSubbandProcessState_32f *pState, byte *pBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSubbandSynthesis_32sc16s_Sfs (  Ipp32sc *pSubbands, short *pSignal, IppsSubbandProcessState_16s *pState, int scalefactor, byte *pBuf );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSyntesisFilterZeroInput_G728_16s (  short *pCoeffs, short *pSrcDstExc, short excSfs, short *pDstSpeech, short *pSpeechSfs, IppsSynthesisFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilterGetStateSize_G728_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilterInit_G728_16s (  IppsSynthesisFilterState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilterLow_NR_16s_ISfs (  short *pLPC, short *pSrcDst, int len, int scaleFactor, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilterZeroStateResponse_NR_16s (  short *pSrcLPC, short *pDstImp, int len, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_AMRWBE_16s32s_I (  short *pSrcLpc, int order, short *pSrcExc, int *pSrcDstSignal, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_AMRWB_16s32s_I (  short *pSrcLpc, int order, short *pSrcExc, int *pSrcDstSignal, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G723_16s (  short *pSrcLPC, short *pSrcResidual, short *pSrcDstIIRState, short *pDstSpch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G723_16s32s (  short *pQntLPC, short *pSrc, int *pDst, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G729E_16s (  short *pSrcLpc, short order, short *pSrcSignal, short *pDstFltSignal, int len, short *pSrcDstSynFltMemory );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G729E_16s_I (  short *pSrcLpc, short order, short *pSrcDstSignal, int len, short *pSrcDstSynFltMemory );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G729_16s (  short *pSrcResidual, short *pSrcLPC, short *pSrcDstSpch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_G729_32f (  float *pSrcLpc, int order, float *pSrcSignal, float *pDstFltSignal, int len, float *pSrcDstSynFltMemory );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_NR_16s_ISfs (  short *pLPC, short *pSrcDst, int len, int scaleFactor, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsSynthesisFilter_NR_16s_Sfs (  short *pLPC, short *pSrc, short *pDst, int len, int scaleFactor, short *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsTiltCompensation_G723_32s16s (  short val, int *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsTiltCompensation_G7291_16s (  short *pSrc, short *pDst, short gain );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsTiltCompensation_G729A_16s (  short *pSrcLPC, short *pSrcDstFltResidual );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsTiltCompensation_G729E_16s (  short val, short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsTiltCompensation_G729_16s (  short *pSrcImpulseResponse, short *pSrcDstSpch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G723_16s (  short *pSrcImpulseResponse, short *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G723_16s32s (  short *pSrcImpulseResponse, int *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G729D_32f (  float *pSrcImpulseResponse, float *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G729_16s (  short *pSrcImpulseResponse, short *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G729_16s32s (  short *pSrcImpulseResponse, int *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToeplizMatrix_G729_32f (  float *pSrcImpulseResponse, float *pDstMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetectGetStateSize_EC_16s (  IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetectGetStateSize_EC_32f (  IppPCMFrequency sampleFreq, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetectInit_EC_16s (  IppsToneDetectState_EC_16s *pState, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetectInit_EC_32f (  IppsToneDetectState_EC_32f *pState, IppPCMFrequency sampleFreq );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetect_EC_16s (  short *pSignal, int len, int *pResult, IppsToneDetectState_EC_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsToneDetect_EC_32f (  float *pSignal, int len, int *pResult, IppsToneDetectState_EC_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsUpsample_AMRWBE_16s (  short *pSrcSignal, short *pSrcDstSignal, int lenDst, short *pMem, int bandIdx, int addFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVAD1_GSMAMR_16s (  short *pSrcSpch, ref IppGSMAMRVad1State pSrcDstVad1State, short *pResultVadFlag, short maxHpCorr, short toneFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVAD2_GSMAMR_16s (  short *pSrcSpch, ref IppGSMAMRVad2State pSrcDstVad2State, short *pResultVadFlag, short ltpFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVADGetEnergyLevel_AMRWB_16s (  IppsVADState_AMRWB_16s *pState, short *pEnergyLevel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVADGetSize_AMRWB_16s (  int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVADInit_AMRWB_16s (  IppsVADState_AMRWB_16s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsVAD_AMRWB_16s (  short *pSrcSpch, IppsVADState_AMRWB_16s *pState, short *pToneFlag, short *pVadFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWeightingFilter_GSMFR_16s (  short *pSrc, short *pDst, int dstLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridBlock_G728_16s (  short bfi, short *pSrc, short *pSrcSfs, short *pDst, IppsWinHybridState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridGetStateSize_G728_16s (  int M, int L, int N, int DIM, int *pMemSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridGetStateSize_G729E_16s (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridGetStateSize_G729E_32f (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridInit_G728_16s (  short *pWinTab, int M, int L, int N, int DIM, short a2L, IppsWinHybridState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridInit_G729E_16s (  IppsWinHybridState_G729E_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybridInit_G729E_32f (  IppsWinHybridState_G729E_32f *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybrid_G728_16s (  short bfi, short *pSrc, short *pDst, IppsWinHybridState_G728_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybrid_G729E_16s32s (  short *pSrcSynthSpeech, int *pDstInvAutoCorr, IppsWinHybridState_G729E_16s *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname)] public static extern
IppStatus ippsWinHybrid_G729E_32f (  float *pSrcSynthSpeech, float *pDstInvAutoCorr, IppsWinHybridState_G729E_32f *pMem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.sc.libname,EntryPoint="ippscGetLibVersion")] public static extern
int* xippscGetLibVersion (    );
public static IppLibraryVersion ippscGetLibVersion() {
   return new IppLibraryVersion( xippscGetLibVersion() );
}
};
};
