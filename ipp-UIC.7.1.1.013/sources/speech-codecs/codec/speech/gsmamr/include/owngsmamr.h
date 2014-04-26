/*/////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2011 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives
//     USC - Unified Speech Codec interface library
//
// By downloading and installing USC codec, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// A speech coding standards promoted by ITU, ETSI, 3GPP and other
// organizations. Implementations of these standards, or the standard enabled
// platforms may require licenses from various entities, including
// Intel Corporation.
//
//
// Purpose: GSMAMR speech codec: internal header file.
//
*/

#ifndef __OWNGSMAMR_H__
#define __OWNGSMAMR_H__

#if defined( _WIN32_WCE)
#pragma warning( disable : 4505 )
#endif

#include "gsmamr.h"
#include "gsmamrapi.h"

#include "scratchmem.h"

#define ENC_KEY 0xecdaaa
#define DEC_KEY 0xdecaaa

/* GSMAMR constant definitions.
   Constant names are kept same as IETF GSM AMR */
#define SPEECH_BUF_SIZE          320
#define LP_WINDOW_SIZE           240
#define FRAME_SIZE_GSMAMR        160
#define SUBFR_SIZE_GSMAMR         40
#define LP_ORDER_SIZE             10
#define LP1_ORDER_SIZE     (LP_ORDER_SIZE+1)
#define LSF_GAP                  205
#define LP_ALL_FRAME       (4*LP1_ORDER_SIZE)
#define PITCH_MIN_LAG             20
#define PITCH_MAX_LAG            143
#define FLT_INTER_SIZE        (10+1)
#define PITCH_SHARP_MAX        13017
#define PITCH_SHARP_MIN            0

#define MAX_OFFSET   (PITCH_MAX_LAG +LP1_ORDER_SIZE)
#define MAX_NUM_PRM               57
#define PITCH_GAIN_CLIP        15565        /* Pitch gain clipping = 0.95               */
#define PG_NUM_FRAME               7
#define NUM_MEM_LTPG               5
#define NUM_SUBBANDS_VAD           9
#define DTX_HIST_SIZE              8
#define ENERGY_HIST_SIZE          60
#define CBGAIN_HIST_SIZE           7
#define NUM_PRED_TAPS              4
#define MIN_ENERGY            -14336         /* -14 dB  */
#define MIN_ENERGY_M122        -2381          /* -14 dB / (20*log10(2)) */
#define PSEUDO_NOISE_SEED 0x70816958L
#define INIT_BACKGROUND_NOISE    150
#define INIT_LOWPOW_SEGMENT    13106

/* Constants for pitch detection */
#define LOW_THRESHOLD              4
#define NUM_THRESHOLD              4
#define MAX_MED_SIZE               9
#define LTP_GAIN_LOG10_1        2721 /* 1.0 / (10*log10(2)) */
#define LTP_GAIN_LOG10_2        5443 /* 2.0 / (10*log10(2)) */
#define MAX_UPSAMPLING             6
#define LEN_INTERPOL_10    (FLT_INTER_SIZE-1)
#define LEN_FIRFLT     (MAX_UPSAMPLING*LEN_INTERPOL_10+1)
#define EXP_CONST_016           5243               /* 0.16 */
#define EXP_CONST_084          27525               /* 0.84 */
#define LTP_GAIN_MEM_SIZE          5
#define LTP_THRESH1             9830                /* 0.6  */
#define LTP_THRESH2            14746               /* 0.9  */
#define THRESH_ENERGY_LIMIT    17578         /* 150 */
#define LOW_NOISE_LIMIT           20            /* 5 */
#define UPP_NOISE_LIMIT         1953            /* 50 */
#define LEN_QNT_GAIN              32
#define LEN_QNT_PITCH             16
#define VEC_QNT_M475_SIZE        256
#define VEC_QNT_HIGHRATES_SIZE   128
#define VEC_QNT_LOWRATES_SIZE     64
#define ALPHA_09               29491                  /* 0.9 */
#define ONE_ALPHA               3277                      /* 1.0-ALPHA */
enum enDTXStateType {SPEECH = 0, DTX, DTX_MUTE, DTX_NODATA};
#define DTX_MAX_EMPTY_THRESH      50
#define DTX_ELAPSED_FRAMES_THRESH (24 + 7 -1)
#define DTX_HANG_PERIOD            7

#define dVADState void

typedef struct {
   Ipp16s vSinceLastSid;
   Ipp16s vDTXHangCount;
   Ipp16s vExpireCount;
   Ipp16s vDataUpdated;

} sDecoderSidSync;

/* post filter memory */
typedef struct{
   Ipp16s a_MemSynPst[LP_ORDER_SIZE];
   Ipp16s vMemPrevRes;
   Ipp16s vPastGainScale;
   Ipp16s a_SynthBuf[LP_ORDER_SIZE];
} sPostFilterSt;

typedef struct {
  Ipp16s a_GainMemory[LTP_GAIN_MEM_SIZE];
  Ipp16s vPrevState;
  Ipp16s vPrevGain;
  Ipp16s vFlagLockGain;
  Ipp16s vOnSetGain;
} sPhaseDispSt;

typedef struct {
    Ipp16s vExpPredCBGain;
    Ipp16s vFracPredCBGain;
    Ipp16s vExpTargetEnergy;
    Ipp16s vFracTargetEnergy;
    Ipp16s a_ExpEnCoeff[5];
    Ipp16s a_FracEnCoeff[5];
    Ipp16s *pGainPtr;
    Ipp16s a_PastQntEnergy[4];
    Ipp16s a_PastQntEnergy_M122[4];
    Ipp16s a_PastUnQntEnergy[4];
    Ipp16s a_PastUnQntEnergy_M122[4];
    Ipp16s vOnSetQntGain;
    Ipp16s vPrevAdaptOut;
    Ipp16s vPrevGainCode;
    Ipp16s a_LTPHistoryGain[NUM_MEM_LTPG];

} sGainQuantSt;

typedef struct {
   Ipp16s a_LSPHistory[LP_ORDER_SIZE * DTX_HIST_SIZE];
   Ipp16s a_LogEnergyHistory[DTX_HIST_SIZE];
   Ipp16s vHistoryPtr;
   Ipp16s vLogEnergyIndex;
   Ipp16s vLSFQntIndex;
   Ipp16s a_LSPIndex[3];
   /* DTX handler */
   Ipp16s vDTXHangoverCt;
   Ipp16s vDecExpireCt;
} sDTXEncoderSt;

typedef struct {
   Ipp16s vLastSidFrame;
   Ipp16s vSidPeriodInv;
   Ipp16s vLogEnergy;
   Ipp16s vLogEnergyOld;
   Ipp32s vPerfSeedDTX_long;
   Ipp16s a_LSP[LP_ORDER_SIZE];
   Ipp16s a_LSP_Old[LP_ORDER_SIZE];
   Ipp16s a_LSFHistory[LP_ORDER_SIZE*DTX_HIST_SIZE];
   Ipp16s a_LSFHistoryMean[LP_ORDER_SIZE*DTX_HIST_SIZE];
   Ipp16s vLogMean;
   Ipp16s a_LogEnergyHistory[DTX_HIST_SIZE];
   Ipp16s vLogEnergyHistory;
   Ipp16s vLogEnergyCorrect;
   Ipp16s vDTXHangoverCt;
   Ipp16s vDecExpireCt;
   Ipp16s vFlagSidFrame;
   Ipp16s vFlagValidData;
   Ipp16s vDTXHangAdd;
   Ipp16s vSidUpdateCt;
   enum  enDTXStateType eDTXPrevState;
   Ipp16s vFlagDataUpdate;

   Ipp16s vSinceLastSid;
   Ipp16s vDTXHangCount;
   Ipp16s vExpireCount;
   Ipp16s vDataUpdated;

} sDTXDecoderSt;

/* Decoder part */
/********************************************************
*            array & table declarations
*********************************************************/
#define N_MODES     9

extern CONST Ipp16s TableHammingWindow[LP_WINDOW_SIZE];
extern CONST Ipp16s TablePastLSFQnt[80];
extern CONST Ipp16s TableMeanLSF_3[10];
extern CONST Ipp16s TableDecCode1LSF_3[256*3];
extern CONST Ipp16s TableDecCode2LSF_3[512*3];
extern CONST Ipp16s TableDecCode3LSF_3[512*4];
extern CONST Ipp16s TableMeanLSF_5[10];
extern CONST Ipp16s TableQuantGainPitch[16];
extern CONST Ipp16s TableLSPInitData[LP_ORDER_SIZE]; /* removed const for BE code */
extern CONST Ipp16s TableParamPerModes[N_MODES];
extern CONST Ipp16s *TableBitAllModes[N_MODES];
extern CONST IppSpchBitRate mode2rates[N_MODES];

/********************************************************
*               function declarations
*********************************************************/

void  ownVADPitchDetection_GSMAMR(IppGSMAMRVad1State *st, Ipp16s *pTimeVec, Ipp16s *vLagCountOld, Ipp16s *vLagOld);

void  ownUpdateLTPFlag_GSMAMR(IppSpchBitRate rate, Ipp32s L_Rmax, Ipp32s L_R0, Ipp16s *vFlagLTP);

void  ownLog2_GSMAMR_norm (Ipp32s inVal, Ipp16s exp, Ipp16s *expPart, Ipp16s *fracPart);

void  ownLog2_GSMAMR(Ipp32s inVal, Ipp16s *expPart, Ipp16s *fracPart);

Ipp32s   ownPow2_GSMAMR(Ipp16s expPart, Ipp16s fracPart);

Ipp32s   ownSqrt_Exp_GSMAMR(Ipp32s inVal, Ipp16s *exp);

void  ownReorderLSFVec_GSMAMR(Ipp16s *lsf, Ipp16s minDistance, Ipp16s len);

Ipp16s ownGetMedianElements_GSMAMR(Ipp16s *pPastGainVal, Ipp16s num);

Ipp16s ownCtrlDetectBackgroundNoise_GSMAMR (Ipp16s *excitation, Ipp16s excEnergy, Ipp16s *exEnergyHist,
                                           Ipp16s vVoiceHangover, Ipp16s prevBFI, Ipp16s carefulFlag);

Ipp16s ownComputeCodebookGain_GSMAMR(Ipp16s *pTargetVec2, Ipp16s *pFltVector);

void  ownGainAdaptAlpha_GSMAMR(Ipp16s *vOnSetQntGain, Ipp16s *vPrevAdaptOut, Ipp16s *vPrevGainCode,
                    Ipp16s *a_LTPHistoryGain, Ipp16s ltpg, Ipp16s gainCode, Ipp16s *alpha);

Ipp16s ownCBGainAverage_GSMAMR(Ipp16s *a_GainHistory, Ipp16s *vHgAverageVar, Ipp16s *vHgAverageCount, IppSpchBitRate rate,
                              Ipp16s gainCode, Ipp16s *lsp, Ipp16s *lspAver, Ipp16s badFrame, Ipp16s vPrevBadFr,
                              Ipp16s pdfi, Ipp16s vPrevDegBadFr, Ipp16s vBackgroundNoise, Ipp16s vVoiceHangover);

Ipp16s ownCheckLSPVec_GSMAMR(Ipp16s *count, Ipp16s *lsp);

Ipp32s   ownSubframePostProc_GSMAMR(Ipp16s *pSpeechVec, IppSpchBitRate rate, Ipp16s numSubfr,
      Ipp16s gainPitch, Ipp16s gainCode, Ipp16s *pAQnt, Ipp16s *pLocSynth, Ipp16s *pTargetVec,
      Ipp16s *code, Ipp16s *pAdaptCode, Ipp16s *pFltVector, Ipp16s *a_MemorySyn, Ipp16s *a_MemoryErr,
      Ipp16s *a_Memory_W0, Ipp16s *pLTPExc, Ipp16s *vFlagSharp);

void  ownPredExcMode3_6_GSMAMR (Ipp16s *pLTPExc, Ipp16s T0, Ipp16s frac, Ipp16s lenSubfr, Ipp16s flag3);

void  ownPhaseDispersion_GSMAMR (sPhaseDispSt *state, IppSpchBitRate rate, Ipp16s *x, Ipp16s cbGain,
                                Ipp16s ltpGain, Ipp16s *innovVec, Ipp16s pitchFactor, Ipp16s tmpShift);


Ipp16s ownSourceChDetectorUpdate_GSMAMR (Ipp16s *a_EnergyHistVector, Ipp16s *vCountHangover, Ipp16s *ltpGainHist, Ipp16s *pSpeechVec, Ipp16s *vVoiceHangover);

void  ownCalcUnFiltEnergy_GSMAMR(Ipp16s *pLPResVec, Ipp16s *exc, Ipp16s *code, Ipp16s gainPitch, Ipp16s lenSubfr, Ipp16s *fracEnergyCoeff,
                          Ipp16s *expEnergyCoeff, Ipp16s *ltpg);

void  ownCalcFiltEnergy_GSMAMR(IppSpchBitRate rate, Ipp16s *pTargetVec, Ipp16s *pTargetVec2, Ipp16s *pAdaptCode, Ipp16s *pFltVector, Ipp16s *fracEnergyCoeff,
                        Ipp16s *expEnergyCoeff, Ipp16s *optFracCodeGain, Ipp16s *optExpCodeGain);
void  ownCalcTargetEnergy_GSMAMR(Ipp16s *pTargetVec, Ipp16s *optExpCodeGain, Ipp16s *optFracCodeGain);

void  ownConvertDirectCoeffToReflCoeff_GSMAMR(Ipp16s *pDirectCoeff, Ipp16s *pReflCoeff);

void  ownScaleExcitation_GSMAMR(Ipp16s *pInputSignal, Ipp16s *pOutputSignal);

void  ownPredEnergyMA_GSMAMR(Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122, IppSpchBitRate rate, Ipp16s *code,
        Ipp16s *expGainCodeCB, Ipp16s *fracGainCodeCB, Ipp16s *fracEnergyCoeff, Ipp16s *frac_en);

Ipp16s ownQntGainCodebook_GSMAMR(IppSpchBitRate rate, Ipp16s expGainCodeCB, Ipp16s fracGainCodeCB, Ipp16s *gain,
                                Ipp16s *pQntEnergyErr_M122, Ipp16s *pQntEnergyErr);
Ipp16s ownQntGainPitch_M7950_GSMAMR(Ipp16s gainPitchLimit, Ipp16s *gain, Ipp16s *pGainCand, Ipp16s *pGainCind);

Ipp16s ownQntGainPitch_M122_GSMAMR(Ipp16s gainPitchLimit, Ipp16s gain);

void  ownUpdateUnQntPred_M475(Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122,
      Ipp16s expGainCodeCB, Ipp16s fracGainCodeCB, Ipp16s optExpCodeGain, Ipp16s optFracCodeGain);

Ipp16s ownGainQnt_M475(Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122, Ipp16s vExpPredCBGain,
      Ipp16s vFracPredCBGain, Ipp16s *a_ExpEnCoeff, Ipp16s *a_FracEnCoeff, Ipp16s vExpTargetEnergy,
      Ipp16s vFracTargetEnergy, Ipp16s *codeNoSharpSF1, Ipp16s expGainCodeSF1, Ipp16s fracGainCodeSF1,
      Ipp16s *expCoeffSF1, Ipp16s *fracCoeffSF1, Ipp16s expTargetEnergySF1, Ipp16s fracTargetEnergySF1,
      Ipp16s gainPitchLimit, Ipp16s *gainPitSF0, Ipp16s *gainCodeSF0, Ipp16s *gainPitSF1, Ipp16s *gainCodeSF1);

void  ownGainQuant_M795_GSMAMR(Ipp16s *vOnSetQntGain, Ipp16s *vPrevAdaptOut, Ipp16s *vPrevGainCode, Ipp16s *a_LTPHistoryGain, Ipp16s *pLTPRes,
      Ipp16s *pLTPExc, Ipp16s *code, Ipp16s *fracEnergyCoeff, Ipp16s *expEnergyCoeff, Ipp16s expCodeEnergy, Ipp16s fracCodeEnergy,
      Ipp16s expGainCodeCB, Ipp16s fracGainCodeCB, Ipp16s lenSubfr, Ipp16s optFracCodeGain, Ipp16s optExpCodeGain, Ipp16s gainPitchLimit,
      Ipp16s *gainPitch, Ipp16s *gainCode, Ipp16s *pQntEnergyErr_M122, Ipp16s *pQntEnergyErr, Ipp16s **ppAnalysisParam);

Ipp16s ownGainQntInward_GSMAMR(IppSpchBitRate rate, Ipp16s expGainCodeCB, Ipp16s fracGainCodeCB, Ipp16s *fracEnergyCoeff, Ipp16s *expEnergyCoeff,
      Ipp16s gainPitchLimit, Ipp16s *gainPitch, Ipp16s *gainCode, Ipp16s *pQntEnergyErr_M122, Ipp16s *pQntEnergyErr);

Ipp32s   ownGainQuant_GSMAMR(sGainQuantSt *st, IppSpchBitRate rate, Ipp16s *pLTPRes, Ipp16s *pLTPExc, Ipp16s *code, Ipp16s *pTargetVec,
      Ipp16s *pTargetVec2, Ipp16s *pAdaptCode, Ipp16s *pFltVector, Ipp16s even_subframe, Ipp16s gainPitchLimit, Ipp16s *gainPitSF0,
      Ipp16s *gainCodeSF0, Ipp16s *gainPitch, Ipp16s *gainCode, Ipp16s **ppAnalysisParam);

void  ownDecodeFixedCodebookGain_GSMAMR(Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122, IppSpchBitRate rate,
                                       Ipp16s index, Ipp16s *code, Ipp16s *gainCode);

void  ownDecodeCodebookGains_GSMAMR(Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122, IppSpchBitRate rate, Ipp16s index,
                                   Ipp16s *code, Ipp16s evenSubfr, Ipp16s * gainPitch, Ipp16s * gain_cod);

void  ownConcealCodebookGain_GSMAMR(Ipp16s *a_GainBuffer, Ipp16s vPastGainCode, Ipp16s *a_PastQntEnergy,
                                   Ipp16s *a_PastQntEnergy_M122, Ipp16s state, Ipp16s *gainCode);

void  ownConcealCodebookGainUpdate_GSMAMR(Ipp16s *a_GainBuffer, Ipp16s *vPastGainCode, Ipp16s *vPrevGainCode,   /* i   : flag: frame is bad               */
                                         Ipp16s badFrame, Ipp16s vPrevBadFr, Ipp16s *gainCode);

void  ownConcealGainPitch_GSMAMR(Ipp16s *a_LSFBuffer, Ipp16s vPastGainZero, Ipp16s state, Ipp16s *pGainPitch);

void  ownConcealGainPitchUpdate_GSMAMR(Ipp16s *a_LSFBuffer, Ipp16s *vPastGainZero, Ipp16s *vPrevGainZero,      /* i   : flag: frame is bad                */
                                      Ipp16s badFrame, Ipp16s vPrevBadFr, Ipp16s *gain_pitch);

Ipp32s ownCloseLoopFracPitchSearch_GSMAMR(Ipp16s *vTimePrevSubframe, Ipp16s *a_GainHistory, IppSpchBitRate rate, Ipp16s frameOffset,
      Ipp16s *pLoopPitchLags, Ipp16s *pImpResVec, Ipp16s *pLTPExc, Ipp16s *pPredRes, Ipp16s *pTargetVec, Ipp16s lspFlag, Ipp16s *pTargetVec2,
      Ipp16s *pAdaptCode, Ipp16s *pExpPitchDel, Ipp16s *pFracPitchDel, Ipp16s *gainPitch, Ipp16s **ppAnalysisParam, Ipp16s *gainPitchLimit);

Ipp16s ownGenNoise_GSMAMR(Ipp32s *pShiftReg, Ipp16s numBits);

void   ownBuildCNCode_GSMAMR(Ipp32s *seed, Ipp16s *pCNVec);

void   ownBuildCNParam_GSMAMR(Ipp16s *seed, const Ipp16s numParam, const Ipp16s *pTableSizeParam, Ipp16s *pCNParam);

void   ownDecLSPQuantDTX_GSMAMR(Ipp16s *a_PastQntPredErr, Ipp16s *a_PastLSFQnt, Ipp16s BadFrInd, Ipp16s *indice, Ipp16s *lsp1_q);

Ipp32s ownDTXDecoder_GSMAMR(sDTXDecoderSt *st, Ipp16s *a_MemorySyn, Ipp16s *a_PastQntPredErr, Ipp16s *a_PastLSFQnt,
      Ipp16s *a_PastQntEnergy, Ipp16s *a_PastQntEnergy_M122, Ipp16s *vHgAverageVar, Ipp16s *vHgAverageCount,
      enum enDTXStateType newState, GSMAMR_Rate_t rate, Ipp16s *pParmVec, Ipp16s *pSynthSpeechVec,
      Ipp16s *pA_LP);

enum   enDTXStateType ownRX_DTX_Handler_GSMAMR(GSMAMRDecoder_Obj* decoderObj, RXFrameType frame_type);

Ipp32s ownDecSidSyncReset_GSMAMR(sDTXDecoderSt *st);
enum   enDTXStateType ownDecSidSync(sDTXDecoderSt *st, RXFrameType frame_type );

void   ownPrm2Bits_GSMAMR(const Ipp16s* prm, Ipp8u *Vout, GSMAMR_Rate_t rate );

void   ownBits2Prm_GSMAMR( const Ipp8u *bitstream, Ipp16s* prm , GSMAMR_Rate_t rate );

Ipp32s ownEncDetectSize_GSMAMR(Ipp32s* pEncSize);

Ipp32s ownEncoderInit_GSMAMR(GSMAMREncoder_Obj* st);

Ipp32s ownDtxEncoderInit_GSMAMR(sDTXEncoderSt* st);

Ipp32s ownGainQuantInit_GSMAMR(sGainQuantSt *state);

Ipp32s ownVAD1Init_GSMAMR(IppGSMAMRVad1State *state);

Ipp32s ownVAD2Init_GSMAMR(IppGSMAMRVad2State *state);

/********************************************************
*               structs declarations
*********************************************************/

typedef struct _GSMAMRCoder_Obj{
   Ipp32s              objSize;
   Ipp32s              key;
   Ipp32s              mode;          /* encoder mode's */
   GSMAMRCodec_Type    codecType;
}GSMAMRCoder_Obj;

typedef struct {
   Ipp16s a_SpeechVecOld[SPEECH_BUF_SIZE];
   Ipp16s *pSpeechPtr, *pWindowPtr, *pWindowPtr_M122;
   Ipp16s *pSpeechPtrNew;
   Ipp16s a_WeightSpeechVecOld[FRAME_SIZE_GSMAMR + PITCH_MAX_LAG];
   Ipp16s *pWeightSpeechVec;
   Ipp16s a_LTPStateOld[5];
   Ipp16s a_GainFlg[2];
   Ipp16s a_ExcVecOld[FRAME_SIZE_GSMAMR + PITCH_MAX_LAG + FLT_INTER_SIZE];
   Ipp16s *pExcVec;
   Ipp16s a_ZeroVec[SUBFR_SIZE_GSMAMR + LP1_ORDER_SIZE];
   Ipp16s *pZeroVec;
   Ipp16s *pImpResVec;
   Ipp16s a_ImpResVec[SUBFR_SIZE_GSMAMR];
   Ipp16s a_SubState[LP_ORDER_SIZE + 1];
   Ipp16s a_LSP_Old[LP_ORDER_SIZE];
   Ipp16s a_LSPQnt_Old[LP_ORDER_SIZE];
   Ipp16s a_PastQntPredErr[LP_ORDER_SIZE];
   Ipp16s vTimePrevSubframe;
   sGainQuantSt stGainQntSt;
   Ipp16s vTimeMedOld;
   Ipp16s vFlagVADState;
   Ipp16s vCount;
   Ipp16s vFlagTone;
   Ipp16s a_GainHistory[PG_NUM_FRAME];
   dVADState *pVAD1St;
   dVADState *pVAD2St;
   Ipp32s vFlagDTX;
   sDTXEncoderSt stDTXEncState;
   Ipp16s a_MemorySyn[LP_ORDER_SIZE], a_Memory_W0[LP_ORDER_SIZE];
   Ipp16s a_MemoryErr[LP_ORDER_SIZE + SUBFR_SIZE_GSMAMR], *pErrorPtr;

   Ipp16s vFlagSharp;
   Ipp16s vFlagLTP;
   Ipp16s vLagCountOld, vLagOld;
   Ipp16s vBestHpCorr;

} sEncoderState_GSMAMR;


struct _GSMAMREncoder_Obj {
   GSMAMRCoder_Obj       objPrm;
/* preprocess state */
   Ipp8s                 *preProc;      /* High pass pre processing filter memory */
   sEncoderState_GSMAMR  stEncState;
   GSMAMR_Rate_t         rate;          /* encode rate */
};


typedef struct{
   Ipp16s a_ExcVecOld[SUBFR_SIZE_GSMAMR + PITCH_MAX_LAG + FLT_INTER_SIZE];
   Ipp16s *pExcVec;
   Ipp16s a_LSP_Old[LP_ORDER_SIZE];
   Ipp16s a_MemorySyn[LP_ORDER_SIZE];
   Ipp16s vFlagSharp;
   Ipp16s vPrevPitchLag;
   Ipp16s vPrevBadFr;
   Ipp16s vPrevDegBadFr;
   Ipp16s vStateMachine;
   Ipp16s a_EnergyHistSubFr[9];
   Ipp16s vLTPLag;
   Ipp16s vBackgroundNoise;
   Ipp16s vVoiceHangover;
   Ipp16s a_LTPGainHistory[9];
   Ipp16s a_EnergyHistVector[ENERGY_HIST_SIZE];
   Ipp16s vCountHangover;
   Ipp16s vCNGen;
   Ipp16s a_GainHistory[CBGAIN_HIST_SIZE];
   Ipp16s vHgAverageVar;
   Ipp16s vHgAverageCount;
   Ipp16s a_LSPAveraged[LP_ORDER_SIZE];
   Ipp16s a_PastQntPredErr[LP_ORDER_SIZE];
   Ipp16s a_PastLSFQnt[LP_ORDER_SIZE];
   Ipp16s a_LSFBuffer[5];
   Ipp16s vPastGainZero;
   Ipp16s vPrevGainZero;
   Ipp16s a_GainBuffer[5];
   Ipp16s vPastGainCode;
   Ipp16s vPrevGainCode;
   Ipp16s a_PastQntEnergy_M122[4];
   Ipp16s a_PastQntEnergy[4];
   sPhaseDispSt stPhDispState;
   sDTXDecoderSt dtxDecoderState;
} sDecoderState_GSMAMR;

struct _GSMAMRDecoder_Obj {
/* post process state */
   GSMAMRCoder_Obj       objPrm;
   Ipp8s                 *postProc;     /* High pass post processing filter memory */
   sDecoderState_GSMAMR  stDecState;
   sPostFilterSt         stPFiltState;
   GSMAMR_Rate_t         rate;           /* decode rate */
};

Ipp32s ownEncode_GSMAMR(sEncoderState_GSMAMR *st, GSMAMR_Rate_t rate, Ipp16s ana[], Ipp32s *pVad, Ipp16s synth[]);

Ipp32s ownDecoderInit_GSMAMR(sDecoderState_GSMAMR* state, GSMAMR_Rate_t rate);

Ipp32s ownDtxDecoderInit_GSMAMR(sDTXDecoderSt* st);

Ipp32s ownPhDispInit_GSMAMR(sPhaseDispSt* state);

Ipp32s ownPostFilterInit_GSMAMR(sPostFilterSt *state);

#define   GSMAMR_CODECFUN(type,name,arg)                extern type name arg
/********************************************************
*      auxiliary inline functions declarations
*********************************************************/

#include "aux_fnxs.h"

__INLINE Ipp32s AddProduct_32s (Ipp32s x, Ipp16s hi1, Ipp16s lo1, Ipp16s hi2, Ipp16s lo2)
{
    x += 2*(hi1*hi2) + 2*((hi1*lo2)>>15) + 2*((lo1*hi2)>>15);
    return x;
}

__INLINE Ipp32s AddProduct16s_32s (Ipp32s x, Ipp16s hi, Ipp16s lo, Ipp16s n)
{
    x += 2*(hi*n) + 2*((lo*n)>>15);
    return x;
}

#endif /* __OWNGSMAMR_H__ */
