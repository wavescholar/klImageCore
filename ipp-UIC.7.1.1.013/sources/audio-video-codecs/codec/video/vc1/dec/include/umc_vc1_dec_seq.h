/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Coded block pattern tables
//
*/

#ifndef __UMC_VC1_DEC_SEQ_H__
#define __UMC_VC1_DEC_SEQ_H__

#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_task.h"

#include "ippi.h"

Ipp32u          DecodeBegin                           (VC1Context* pContext,
                                                       Ipp32u streamType);

//sequence layer
VC1Status SequenceLayer                               (VC1Context* pContext);
bool InitTables                                     (VC1Context* pContext);
Ipp32s InitCommonTables                               (VC1Context* pContext);
Ipp32s InitInterlacedTables                           (VC1Context* pContext);

void FreeTables                                       (VC1Context* pContext);


//picture layer
//Simple/main
VC1Status GetNextPicHeader                            (VC1Context* pContext, bool isExtHeader);
VC1Status DecodePictureHeader                         (VC1Context* pContext,  bool isExtHeader);
//VC1Status SetDisplayIndex                             (VC1Context* pContext,VC1FrameBuffer*        pfrmBuff);
VC1Status Decode_PictureLayer                         (VC1Context* pContext);
VC1Status DecodeFrame                                 (VC1Context* pContext);

//Advanced
VC1Status DecodePictureHeader_Adv                     (VC1Context* pContext);
VC1Status GetNextPicHeader_Adv                        (VC1Context* pContext);


//for threading
void PrepareForNextFrame                         (VC1Context*pContext);

//I,P,B headers
VC1Status DecodePicHeader                             (VC1Context* pContext);

VC1Status Decode_FieldPictureLayer_Adv                (VC1Context* pContext);

VC1Status DecodePictHeaderParams_InterlaceFieldPicture_Adv (VC1Context* pContext);
VC1Status Decode_FirstField_Adv                        (VC1Context* pContext);
VC1Status Decode_SecondField_Adv                       (VC1Context* pContext);
VC1Status DecodeSkippicture                       (VC1Context* pContext);

void      ChooseDCTable                               (VC1Context* pContext,
                                                       Ipp32s transDCtableIndex);

void      ChooseACTable                               (VC1Context* pContext,
                                                       Ipp32s transACtableIndex1,
                                                       Ipp32s transACtableIndex2);

void   ChooseTTMB_TTBLK_SBP                          (VC1Context* pContext);

void ChooseMBModeInterlaceFrame                       (VC1Context* pContext,
                                                       Ipp32u MV4SWITCH,
                                                       Ipp32u MBMODETAB);
void ChooseMBModeInterlaceField                       (VC1Context* pContext,
                                                       Ipp32s MBMODETAB);
void ChoosePredScaleValuePPictbl                      (VC1PictureLayerHeader* picLayerHeader);
void ChoosePredScaleValueBPictbl                      (VC1PictureLayerHeader* picLayerHeader);
Ipp8u GetTTBLK                                        (VC1Context* pContext,
                                                       Ipp32s blk_num);
// Simple/Main
VC1Status DecodePictureLayer_ProgressiveIpicture            (VC1Context* pContext);
// Advanced
VC1Status DecodePictHeaderParams_ProgressiveIpicture_Adv    (VC1Context* pContext);
VC1Status DecodePictHeaderParams_InterlaceIpicture_Adv      (VC1Context* pContext);
VC1Status DecodeFieldHeaderParams_InterlaceFieldIpicture_Adv (VC1Context* pContext);
VC1Status Decode_InterlaceFieldIpicture_Adv                 (VC1Context* pContext);

// Simple/Main
VC1Status DecodePictureLayer_ProgressivePpicture            (VC1Context* pContext);
// Advanced
VC1Status DecodePictHeaderParams_ProgressivePpicture_Adv    (VC1Context* pContext);
VC1Status DecodePictHeaderParams_InterlacePpicture_Adv      (VC1Context* pContext);
VC1Status DecodeFieldHeaderParams_InterlaceFieldPpicture_Adv (VC1Context* pContext);
VC1Status Decode_InterlaceFieldPpicture_Adv                 (VC1Context* pContext);

// Simple/Main
VC1Status DecodePictureLayer_ProgressiveBpicture            (VC1Context* pContext);
// Advanced
VC1Status DecodePictHeaderParams_ProgressiveBpicture_Adv    (VC1Context* pContext);
VC1Status DecodePictHeaderParams_InterlaceBpicture_Adv      (VC1Context* pContext);
VC1Status Decode_InterlaceFieldBpicture_Adv                 (VC1Context* pContext);
VC1Status DecodeFieldHeaderParams_InterlaceFieldBpicture_Adv(VC1Context* pContext);

VC1Status MVRangeDecode                                (VC1Context* pContext);
VC1Status DMVRangeDecode                               (VC1Context* pContext);

//I picture MB Layer
VC1Status MBLayer_ProgressiveIpicture                  (VC1Context* pContext);
VC1Status MBLayer_IntraField_InterlasedIpicture        (VC1Context* pContext);
VC1Status MBLayer_ProgressiveIpicture_Adv              (VC1Context* pContext);
VC1Status MBLayer_Frame_InterlaceIpicture              (VC1Context* pContext);
VC1Status MBLayer_Field_InterlaceIpicture              (VC1Context* pContext);

//P picture MB Layer
VC1Status MBLayer_ProgressivePpicture                  (VC1Context* pContext);
VC1Status MBLayer_Frame_InterlacedPpicture             (VC1Context* pContext);
VC1Status MBLayer_Field_InterlacedPpicture              (VC1Context* pContext);
VC1Status MBLayer_ProgressivePpicture_Adv              (VC1Context* pContext);

//B picture MB Layer
VC1Status MBLayer_ProgressiveBpicture                  (VC1Context* pContext);
VC1Status MBLayer_Frame_InterlacedBpicture             (VC1Context* pContext);
VC1Status MBLayer_Field_IntrlacedBpicture              (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_Adv              (VC1Context* pContext);
VC1Status MBLayer_Field_InterlacedBpicture             (VC1Context* pContext);

void GetIntraDCPredictors                              (VC1Context* pContext);
void GetPDCPredictors                                  (VC1Context* pContext);
void GetIntraScaleDCPredictors                         (VC1Context* pContext);
void GetPScaleDCPredictors                             (VC1Context* pContext);
Ipp32s DecodeSymbol                                    (Ipp32u** pbs,
                                                        Ipp32s* bitOffset,
                                                        Ipp16s* run,
                                                        Ipp16s* level,
                                                        Ipp32s* last_flag,
                                                        const VC1ACDecodeSet * decodeSet,
                                                        VC1EscInfo* EscInfo);

IppStatus DecodeBlockACIntra_VC1                        (VC1Bitstream* pBitstream,
                                                         Ipp16s* pDst,
                                                         const  Ipp8u* pZigzagTbl,
                                                         const VC1ACDecodeSet * pDecodeSet,
                                                         VC1EscInfo* pEscInfo);
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif
// interfaces should be tha same
IppStatus DecodeBlockInter8x8_VC1                       (VC1Bitstream* pBitstream,
                                                         Ipp16s* pDst,
                                                         const  Ipp8u* pZigzagTbl,
                                                         const VC1ACDecodeSet * pDecodeSet,
                                                         VC1EscInfo* pEscInfo,
                                                         Ipp32s subBlockPattern);

IppStatus DecodeBlockInter4x8_VC1                       (VC1Bitstream* pBitstream,
                                                         Ipp16s* pDst,
                                                         const  Ipp8u* pZigzagTbl,
                                                         const VC1ACDecodeSet * pDecodeSet,
                                                         VC1EscInfo* pEscInfo,
                                                         Ipp32s subBlockPattern);

IppStatus DecodeBlockInter8x4_VC1                       (VC1Bitstream* pBitstream,
                                                         Ipp16s* pDst,
                                                         const  Ipp8u* pZigzagTbl,
                                                         const VC1ACDecodeSet * pDecodeSet,
                                                         VC1EscInfo* pEscInfo,
                                                         Ipp32s subBlockPattern);

IppStatus DecodeBlockInter4x4_VC1                       (VC1Bitstream* pBitstream,
                                                         Ipp16s* pDst,
                                                         const  Ipp8u* pZigzagTbl,
                                                         const VC1ACDecodeSet * pDecodeSet,
                                                         VC1EscInfo* pEscInfo,
                                                         Ipp32s subBlockPattern);

inline
Ipp32s CalculateLeftTopRightPositionFlag (VC1SingletonMB* sMB)
{
    Ipp32s LeftTopRightPositionFlag = VC1_COMMON_MB;
    //Top position
    //macroblock is on the first column
    //#0 and #1 blocks have top border
    if(sMB->slice_currMBYpos == 0)
    {
        LeftTopRightPositionFlag |= VC1_TOP_MB;
    }
    //Left position
    //macroblock is on the first row
    //#0 and #2 blocks have left border
    if(sMB->m_currMBXpos == 0)
    {
        LeftTopRightPositionFlag |= VC1_LEFT_MB;
    }
    else if (sMB->m_currMBXpos == sMB->widthMB - 1)
    {
        LeftTopRightPositionFlag |= VC1_RIGHT_MB;
    }
    return LeftTopRightPositionFlag;
}
void CalculateIntraFlag                                (VC1Context* pContext);


//Block layer
VC1Status BLKLayer_Intra_Luma                          (VC1Context* pContext,
                                                        Ipp32s blk_num,
                                                        Ipp32u bias,
                                                        Ipp32u ACPRED);
VC1Status BLKLayer_Intra_Chroma                        (VC1Context* pContext,
                                                        Ipp32s blk_num,
                                                        Ipp32u bias,
                                                        Ipp32u ACPRED);
VC1Status BLKLayer_Inter_Luma                          (VC1Context* pContext,
                                                        Ipp32s blk_num);
VC1Status BLKLayer_Inter_Chroma                        (VC1Context* pContext,
                                                        Ipp32s blk_num);

VC1Status BLKLayer_Intra_Luma_Adv                     (VC1Context* pContext,
                                                       Ipp32s blk_num,
                                                       Ipp32u ACPRED);
VC1Status BLKLayer_Intra_Chroma_Adv                   (VC1Context* pContext,
                                                       Ipp32s blk_num,
                                                       Ipp32u ACPRED);
VC1Status BLKLayer_Inter_Luma_Adv                     (VC1Context* pContext,
                                                       Ipp32s blk_num);
VC1Status BLKLayer_Inter_Chroma_Adv                   (VC1Context* pContext,
                                                       Ipp32s blk_num);

Ipp32s CalculateCBP                                    (VC1MB* pCurrMB,
                                                        Ipp32u decoded_cbpy,
                                                        Ipp32s width);
Ipp32s CalculateDC                                     (VC1Context* pContext,
                                                        Ipp32s DCDiff, Ipp32s blk_num);

//DeQuantization
VC1Status VOPDQuant            (VC1Context* pContext);
VC1Status CalculatePQuant      (VC1Context* pContext);

//Bitplane decoding
void  DecodeBitplane        (VC1Context* pContext, VC1Bitplane* pBitplane, Ipp32s rowMB, Ipp32s colMB, Ipp32s offset);


Ipp16u DecodeMVDiff                      (VC1Context* pContext,Ipp32s hpelfl,
                                          Ipp16s *dmv_x, Ipp16s *dmv_y);

void DecodeMVDiff_Adv                    (VC1Context* pContext,Ipp16s* pdmv_x, Ipp16s* pdmv_y);
Ipp8u DecodeMVDiff_TwoReferenceField_Adv  (VC1Context* pContext,
                                           Ipp16s* pdmv_x,
                                           Ipp16s* pdmv_y);

void Progressive1MVPrediction                               (VC1Context* pContext);
void Progressive4MVPrediction                               (VC1Context* pContext);
void Field1MVPrediction                                     (VC1Context* pContext);
void Field4MVPrediction                                     (VC1Context* pContext);

inline void ApplyMVPrediction  ( VC1Context* pContext,
                                 Ipp32s blk_num,
                                 Ipp16s* pMVx, Ipp16s* pMVy,
                                 Ipp16s dmv_x, Ipp16s dmv_y,
                                 Ipp32s Backwards)
{
    const VC1MVRange *pMVRange = pContext->m_picLayerHeader->m_pCurrMVRangetbl;
    Ipp16u RangeX, RangeY, YBias=0;
    Ipp32s Count;
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp16s MVx, MVy;
    RangeX = pMVRange->r_x;
    RangeY = pMVRange->r_y;

    dmv_x = dmv_x + *pMVx;
    dmv_y = dmv_y + *pMVy;

    // (dmv_x + predictor_x) smod range_x
    //MVx = ((DMV_X + RangeX) & (2 * RangeX - 1)) - RangeX;
    MVx = ((dmv_x + RangeX) & ((RangeX << 1) - 1)) - RangeX;

    // (dmv_y + predictor_y) smod range_y
    //MVy = ((DMV_Y + RangeY - YBias) & (2 * RangeY - 1)) - RangeY + YBias;
    MVy = ((dmv_y + RangeY - YBias) & ( (RangeY <<1) - 1)) - RangeY + YBias;

    if((pMB->mbType&0x03) == VC1_MB_1MV_INTER)
    {
        for(Count = 0; Count < 4; Count++)
        {
            pMB->m_pBlocks[Count].mv[Backwards][0] = (Ipp16s)MVx;
            pMB->m_pBlocks[Count].mv[Backwards][1] = (Ipp16s)MVy;
        }
    }
    else if((pMB->mbType&0x03) == VC1_MB_2MV_INTER)
    {
        for(Count = 0; Count < 2; Count++)
        {
            pMB->m_pBlocks[Count+blk_num].mv[Backwards][0] = (Ipp16s)MVx;
            pMB->m_pBlocks[Count+blk_num].mv[Backwards][1] = (Ipp16s)MVy;
        }
    }
    else    /* 4MV */
    {
        pMB->m_pBlocks[blk_num].mv[Backwards][0] = (Ipp16s)MVx;
        pMB->m_pBlocks[blk_num].mv[Backwards][1] = (Ipp16s)MVy;
    }
    *pMVx = MVx;
    *pMVy = MVy;

}

/* Apply MVPrediction need to lead to ApplyMVPredictionCalculate*/
void ApplyMVPredictionCalculate (VC1Context* pContext,
                                       Ipp16s* pMVx,
                                       Ipp16s* pMVy,
                                       Ipp32s dmv_x,
                                       Ipp32s dmv_y);
void ApplyMVPredictionCalculateOneReference( VC1PictureLayerHeader* picLayerHeader,
                                             Ipp16s* pMVx,
                                             Ipp16s* pMVy,
                                             Ipp32s dmv_x,
                                             Ipp32s dmv_y,
                                             Ipp8u same_polatity);
void ApplyMVPredictionCalculateTwoReference( VC1PictureLayerHeader* picLayerHeader,
                                             Ipp16s* pMVx,
                                             Ipp16s* pMVy,
                                             Ipp32s dmv_x,
                                             Ipp32s dmv_y,
                                             Ipp8u same_polatity);

void Decode_BMVTYPE                (VC1Context* pContext);
void Decode_InterlaceFrame_BMVTYPE (VC1Context* pContext);
void Decode_InterlaceField_BMVTYPE (VC1Context* pContext);
void CalculateMV              (Ipp16s x[],Ipp16s y[], Ipp16s *X, Ipp16s* Y);

void CalculateMV_Interlace    (Ipp16s x[],Ipp16s y[],
                               Ipp16s x_bottom[],Ipp16s y_bottom[],
                               Ipp16s *Xt, Ipp16s* Yt,Ipp16s *Xb, Ipp16s* Yb );

void CalculateMV_InterlaceField  (VC1Context* pContext, Ipp16s *X, Ipp16s* Y);

void Scale_Direct_MV          (VC1PictureLayerHeader* picHeader, Ipp16s X, Ipp16s Y,
                               Ipp16s* Xf, Ipp16s* Yf,Ipp16s* Xb, Ipp16s* Yb);

void Scale_Direct_MV_Interlace(VC1PictureLayerHeader* picHeader, Ipp16s X, Ipp16s Y,
                               Ipp16s* Xf, Ipp16s* Yf,Ipp16s* Xb, Ipp16s* Yb);

void PullBack_PPred(VC1Context* pContext, Ipp16s *pMVx, Ipp16s* pMVy, Ipp32s blk_num);
void PullBack_PPred4MV(VC1SingletonMB* sMB, Ipp16s *pMVx, Ipp16s* pMVy, Ipp32s blk_num);

void AssignCodedBlockPattern                          (VC1MB * pMB,VC1SingletonMB* sMB);

Ipp8u GetSubBlockPattern_8x4_4x8                  (VC1Context* pContext,Ipp32s num_blk);
Ipp8u GetSubBlockPattern_4x4                      (VC1Context* pContext,Ipp32s num_blk);

Ipp32u GetDCStepSize                                (Ipp32s MQUANT);

inline Ipp16s median3(Ipp16s* pSrc)
{
   if(pSrc[0] > pSrc[1])
    {
        if(pSrc[1]>pSrc[2])
            return pSrc[1];

        if(pSrc[0]>pSrc[2])
            return pSrc[2];
        else
            return pSrc[0];
    }
    else
    {
        if(pSrc[0]>pSrc[2])
            return pSrc[0];

        if(pSrc[1]>pSrc[2])
            return pSrc[2];
        else
            return pSrc[1];
    }
}

inline Ipp16s median4(Ipp16s* pSrc)
{
    Ipp16s max, min;
    Ipp16s med;
    static IppiSize size = {1,4};
    ippiMinMax_16s_C1R(pSrc, 2, size, &min, &max);
    med = (pSrc[0]+ pSrc[1] + pSrc[2] + pSrc[3] - max - min) / 2;
    return med;
}

inline
Ipp16u vc1_abs_16s(Ipp16s pSrc)
{
    Ipp16u S;
    S = pSrc >> 15;
    S = (pSrc + S)^S;
    return S;
}
void ExpandMBRowSM(VC1Context* pContext, Ipp32s StartMBPos, Ipp32s EndMBPos);

void DecodeTransformInfo    (VC1Context* pContext);
VC1Status GetTTMB           (VC1Context* pContext);

//MV prediction
//common
void DeriveSecondStageChromaMV                        (VC1Context* pContext,
                                                       Ipp16s* xMV,
                                                       Ipp16s* yMV);
void DeriveSecondStageChromaMV_Interlace              (VC1Context* pContext,
                                                       Ipp16s* xMV,
                                                       Ipp16s* yMV);


//void save_MV                                          (VC1Context* pContext);
//void save_MV_InterlaceField                           (VC1Context* pContext);
//void save_MV_InterlaceFrame                           (VC1Context* pContext);
void PackDirectMVProgressive(VC1MB* pCurrMB, Ipp16s* pSavedMV);
void PackDirectMVIField(VC1MB* pCurrMB, Ipp16s* pSavedMV, bool isBottom, Ipp8u* bRefField);
void PackDirectMVIFrame(VC1MB* pCurrMB, Ipp16s* pSavedMV);
void PackDirectMVs(VC1MB*  pCurrMB,
                   Ipp16s* pSavedMV,
                   Ipp8u   isBottom,
                   Ipp8u*  bRefField,
                   Ipp32u  FCM);

inline void Derive4MV(VC1SingletonMB* sMB, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32u _MVcount = sMB->MVcount;
    Ipp16s* xLuMV = sMB->xLuMV;
    Ipp16s* yLuMV = sMB->yLuMV;
    switch(_MVcount)
    {
    case 0:
    case 1:
        {
            *xMV = VC1_MVINTRA;
            *yMV = VC1_MVINTRA;
            return;
        }
    case 2:
        {
            *xMV = (xLuMV[0] + xLuMV[1]) / 2;
            *yMV = (yLuMV[0] + yLuMV[1]) / 2;
            return;
        }
    case 3:
        {
            *xMV = median3(xLuMV);
            *yMV = median3(yLuMV);
            return;
        }
    case 4:
        {
            *xMV = median4(xLuMV);
            *yMV = median4(yLuMV);
            return;
        }
    }
}

void Derive4MV_Field                                  (Ipp32u _MVcount,
                                                       Ipp16s* xMV, Ipp16s* yMV,
                                                       Ipp16s* xLuMV, Ipp16s* yLuMV);

void GetPredictProgressiveMV                          (VC1Block *pA,
                                                       VC1Block *pB,
                                                       VC1Block *pC,
                                                       Ipp16s *pX,
                                                       Ipp16s *pY,
                                                       Ipp32s Back);

void HybridMV                                         (VC1Context* pContext,
                                                       VC1Block *pA,
                                                       VC1Block *pC,
                                                       Ipp16s *pPredMVx,
                                                       Ipp16s *pPredMVy,
                                                       Ipp32s Back);


void CalculateMV                                     (Ipp16s x[],
                                                      Ipp16s y[],
                                                      Ipp16s *X,
                                                      Ipp16s* Y);

void Decode_BMVTYPE                                  (VC1Context* pContext);
void Decode_InterlaceFrame_BMVTYPE                   (VC1Context* pContext);

VC1Status MVRangeDecode                              (VC1Context* pContext);



VC1Status PredictBlock_P                            (VC1Context* pContext);
VC1Status PredictBlock_B                            (VC1Context* pContext);

VC1Status PredictBlock_InterlacePPicture            (VC1Context* pContext);
VC1Status PredictBlock_InterlaceBPicture            (VC1Context* pContext);

VC1Status PredictBlock_InterlaceFieldPPicture       (VC1Context* pContext);
VC1Status PredictBlock_InterlaceFieldBPicture       (VC1Context* pContext);


void CropLumaPullBack                                 (VC1Context* pContext,
                                                       Ipp16s* xMV, Ipp16s* yMV);
void CropChromaPullBack                               (VC1Context* pContext,
                                                       Ipp16s* xMV, Ipp16s* yMV);
#define PullBack_BDirect CropChromaPullBack // same algorithm

void ExpandFrame                                      (Frame* pFrame,
                                                       IppiSize roiSize);

void ExpandFrame_Adv                                  (VC1Context* pContext,
                                                       Frame* pFrame);

void ExpandFrame_Interlace                            (VC1Context* pContext,
                                                       Frame* pFrame);

void ExpandFrame_Field                                (VC1Context* pContext,
                                                       Frame* pFrame,
                                                       Ipp32u bottom_field);

VC1Status EntryPointLayer                              (VC1Context* m_pContext);


VC1Status FillTablesForIntensityCompensation           (VC1Context* pContext,
                                                        Ipp32u scale,
                                                        Ipp32u shift);

VC1Status FillTablesForIntensityCompensation_Adv        (VC1Context* pContext,
                                                         Ipp32u scale,
                                                         Ipp32u shift,
                                                         Ipp32u bottom_field,
                                                         Ipp32s index);

void SZTables                                           (VC1Context* pContext);
void CreateComplexICTablesForFields                     (VC1Context* pContext);
void CreateComplexICTablesForFrame                      (VC1Context* pContext);
void UpdateICTablesForSecondField                       (VC1Context* pContext);


VC1Status Set_MQuant                                  (VC1Context* pContext);
VC1Status Set_MQuant_Field                            (VC1Context* pContext);
VC1Status Set_Alt_MQUANT                              (VC1Context* pContext);
void GetMQUANT                                        (VC1Context* pContext);

//advanced
void CropLumaPullBack_Adv                             (VC1Context* pContext,
                                                       Ipp16s* xMV,
                                                       Ipp16s* yMV);
void CropLumaPullBackField_Adv                        (VC1Context* pContext,
                                                       Ipp16s* xMV,
                                                       Ipp16s* yMV);

void CropChromaPullBack_Adv                           (VC1Context* pContext,
                                                       Ipp16s* xMV,
                                                       Ipp16s* yMV);

void CalculateProgressive1MV                          (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy);
void CalculateProgressive4MV                           (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy,
                                                        Ipp32s blk_num);

void CalculateProgressive4MV_Adv                        (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                                        Ipp32s blk_num);

void CalculateProgressive1MV_B                          (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,
                                                         Ipp16s *pPredMVy,
                                                         Ipp32s Back);
void CalculateProgressive1MV_B_Adv                      (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,
                                                         Ipp16s *pPredMVy,
                                                         Ipp32s Back);

void CalculateInterlaceFrame1MV_P                       (VC1MVPredictors* MVPredictors,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy);

void CalculateInterlaceFrame1MV_B                       (VC1MVPredictors* MVPredictors,
                                                           Ipp16s *f_x,Ipp16s *f_y,
                                                           Ipp16s *b_x,Ipp16s *b_y,
                                                           Ipp32u back);

void CalculateInterlaceFrame1MV_B_Interpolate           (VC1MVPredictors* MVPredictors,
                                                         Ipp16s *f_x,Ipp16s *f_y,
                                                         Ipp16s *b_x,Ipp16s *b_y);


void Calculate4MVFrame_Adv                              (VC1MVPredictors* MVPredictors,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy,
                                                        Ipp32u blk_num);

void CalculateInterlace4MV_TopField_Adv                 (VC1MVPredictors* MVPredictors,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy,
                                                        Ipp32u blk_num);

void CalculateInterlace4MV_BottomField_Adv              (VC1MVPredictors* MVPredictors,
                                                        Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                                        Ipp32u blk_num);

void CalculateField1MVOneReferencePPic                  (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy);

void CalculateField1MVTwoReferencePPic                  (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,
                                                         Ipp16s *pPredMVy,
                                                         Ipp8u* PredFlag);
void CalculateField4MVOneReferencePPic                  (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,
                                                         Ipp16s *pPredMVy,
                                                         Ipp32s blk_num);

void CalculateField4MVTwoReferencePPic                  (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy,
                                                        Ipp32s blk_num,
                                                        Ipp8u* PredFlag);

void CalculateField1MVTwoReferenceBPic                  (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,
                                                         Ipp16s *pPredMVy,
                                                         Ipp32s Back,
                                                         Ipp8u* PredFlag);

void CalculateField4MVTwoReferenceBPic                  (VC1Context* pContext,
                                                         Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                                         Ipp32s blk_num,Ipp32s Back,
                                                         Ipp8u* PredFlag);

void PredictInterlaceFrame1MV                          (VC1Context* pContext);

void PredictInterlace4MVFrame_Adv                       (VC1Context* pContext);

void PredictInterlace4MVField_Adv                       (VC1Context* pContext);

void PredictInterlace2MV_Field_Adv                     (VC1MB* pCurrMB,
                                                        Ipp16s pPredMVx[2],
                                                        Ipp16s pPredMVy[2],
                                                        Ipp16s backTop,
                                                        Ipp16s backBottom,
                                                        Ipp32u widthMB);

void ScaleOppositePredPPic                             (VC1PictureLayerHeader* picLayerHeader,
                                                       Ipp16s *x,
                                                       Ipp16s *y);
void ScaleSamePredPPic                                (VC1PictureLayerHeader* picLayerHeader,
                                                       Ipp16s *x,
                                                       Ipp16s *y,
                                                       Ipp32s dominant,
                                                       Ipp32s fieldFlag);

void ScaleOppositePredBPic                             (VC1PictureLayerHeader* picLayerHeader,
                                                        Ipp16s *x,
                                                        Ipp16s *y,
                                                        Ipp32s dominant,
                                                        Ipp32s fieldFlag,
                                                        Ipp32s back);

void ScaleSamePredBPic                                (VC1PictureLayerHeader* picLayerHeader,
                                                       Ipp16s *x,
                                                       Ipp16s *y,
                                                       Ipp32s dominant,
                                                       Ipp32s fieldFlag,
                                                       Ipp32s back);

void HybridFieldMV                                     (VC1Context* pContext,
                                                        Ipp16s *pPredMVx,
                                                        Ipp16s *pPredMVy,
                                                        Ipp16s MV_px[3],
                                                        Ipp16s MV_py[3]);

VC1Status MBLayer_ProgressiveBpicture_NONDIRECT_Prediction          (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_DIRECT_Prediction             (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_SKIP_NONDIRECT_Prediction     (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_SKIP_DIRECT_Prediction        (VC1Context* pContext);

VC1Status MBLayer_ProgressiveBpicture_SKIP_NONDIRECT_AdvPrediction  (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_SKIP_DIRECT_AdvPrediction     (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_NONDIRECT_AdvPrediction       (VC1Context* pContext);
VC1Status MBLayer_ProgressiveBpicture_DIRECT_AdvPrediction          (VC1Context* pContext);

VC1Status MBLayer_InterlaceFrameBpicture_SKIP_NONDIRECT_Prediction  (VC1Context* pContext);
VC1Status MBLayer_InterlaceFrameBpicture_SKIP_DIRECT_Prediction     (VC1Context* pContext);
VC1Status MBLayer_InterlaceFrameBpicture_NONDIRECT_Prediction       (VC1Context* pContext);
VC1Status MBLayer_InterlaceFrameBpicture_DIRECT_Prediction          (VC1Context* pContext);

VC1Status MBLayer_InterlaceFieldBpicture_NONDIRECT_Predicition      (VC1Context* pContext);
VC1Status MBLayer_InterlaceFieldBpicture_DIRECT_Prediction          (VC1Context* pContext);

//Smoothing
void Smoothing_I(VC1Context* pContext, Ipp32s Height);
void Smoothing_P(VC1Context* pContext, Ipp32s Height);
void Smoothing_I_Adv(VC1Context* pContext, Ipp32s Height);
void Smoothing_P_Adv(VC1Context* pContext, Ipp32s Height);

IppStatus _ownReconstructIntraUniform_VC1_16s_C1IR    (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant);
IppStatus _ownReconstructIntraNonuniform_VC1_16s_C1IR (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant);
IppStatus _ownReconstructInterUniform_VC1_16s_C1IR    (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant,Ipp32u BlkType);
IppStatus _ownReconstructInterNonuniform_VC1_16s_C1IR (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant,Ipp32u BlkType);

inline Ipp32s SubBlockPattern(VC1Block* _pBlk, VC1SingletonBlock* _sBlk)
{
    Ipp32s subbpattern = 0;
    if ((_sBlk->Coded ==0)&&(_pBlk->blkType < VC1_BLK_INTRA_TOP))
    {
        _pBlk->blkType = VC1_BLK_INTER8X8;
    }
    if (_sBlk->Coded ==0)
    {
        return 0;
    }

    switch (_pBlk->blkType)
    {
    case VC1_BLK_INTER4X4:
        subbpattern =  _sBlk->numCoef;
        break;

    case VC1_BLK_INTER8X4:
        if (_sBlk->numCoef & VC1_SBP_0)
        {
            subbpattern |= 0xC;
        }

        if (_sBlk->numCoef & VC1_SBP_1)
        {
            subbpattern |= 0x3;
        }
        break;

    case VC1_BLK_INTER4X8:
        if (_sBlk->numCoef & VC1_SBP_0)
        {
            subbpattern |= 0xA;
        }

        if (_sBlk->numCoef & VC1_SBP_1)
        {
            subbpattern |= 0x5;
        }
        break;

    default:
        subbpattern = 0xF;
    }
    return subbpattern;
}

void HorizontalDeblockingBlkI                         (Ipp8u* pUUpBlock,
                                                       Ipp32u Pquant,
                                                       Ipp32s Pitch);

void VerticalDeblockingBlkI                           (Ipp8u* pUUpLBlock,
                                                       Ipp32u Pquant,
                                                       Ipp32s Pitch);
//Deblocking simple/main
void Deblocking_ProgressiveIpicture                   (VC1Context* pContext);

void Deblocking_ProgressivePpicture                   (VC1Context* pContext);
//Deblocking advanced
void Deblocking_ProgressiveIpicture_Adv               (VC1Context* pContext);

void Deblocking_ProgressivePpicture_Adv               (VC1Context* pContext);
void Deblocking_InterlaceFieldBpicture_Adv            (VC1Context* pContext);

void Deblocking_InterlaceFrameIpicture_Adv            (VC1Context* pContext);
void Deblocking_InterlaceFramePpicture_Adv            (VC1Context* pContext);

void HorizontalDeblockingBlkInterlaceI                (Ipp8u* pUUpBlock,
                                                       Ipp32u Pquant,
                                                       Ipp32s Pitch,
                                                       Ipp32u foffset_1);

void VerticalDeblockingBlkInterlaceI                  (Ipp8u*pUUpLBlock,
                                                       Ipp32u Pquant,
                                                       Ipp32s Pitch,
                                                       Ipp32s foffset_1);

void RangeDown                                        (Ipp8u* pSrc,
                                                       Ipp32s srcStep,
                                                       Ipp8u* pDst,
                                                       Ipp32s dstStep,
                                                       Ipp32s width,
                                                       Ipp32s height);

void RangeRefFrame                                  (VC1Context* pContext);

//Advanced

void write_Intraluma_to_interlace_frame_Adv          (VC1MB * pCurrMB,
                                                      Ipp16s* pBlock);

void write_Interluma_to_interlace_frame_Adv          (VC1MB * pCurrMB,
                                                      Ipp8u* pPred,
                                                      Ipp16s* pBlock);
void write_Interluma_to_interlace_frame_MC_Adv       (VC1MB * pCurrMB,
                                                      const Ipp8u* pDst,
                                                      Ipp32u dstStep,
                                                      Ipp16s* pBlock);
void write_Interluma_to_interlace_frame_MC_Adv_Copy   (VC1MB * pCurrMB,
                                                      Ipp16s* pBlock);

void write_Interluma_to_interlace_B_frame_MC_Adv     (VC1MB * pCurrMB,
                                                      const Ipp8u* pDst1,
                                                      Ipp32u dstStep1,
                                                      const Ipp8u* pDst2,
                                                      Ipp32u dstStep2,
                                                      Ipp16s* pBlock);
VC1Status VC1ProcessDiffIntra                         (VC1Context* pContext,Ipp32s blk_num);
VC1Status VC1ProcessDiffInter                         (VC1Context* pContext,Ipp32s blk_num);
VC1Status VC1ProcessDiffSpeedUpIntra                  (VC1Context* pContext,Ipp32s blk_num);
VC1Status VC1ProcessDiffSpeedUpInter                  (VC1Context* pContext,Ipp32s blk_num);

inline Ipp16s PullBack_PredMV(Ipp16s *pMV,Ipp32s pos,
                            Ipp32s Min, Ipp32s Max)
{
    Ipp16s MV = *pMV;
    Ipp32s IMV = pos + MV;

    if (IMV < Min)
    {
        MV = (Ipp16s)(Min - pos);
    }

    else if (IMV > Max)
    {
        MV = (Ipp16s)(Max - pos);
    }

    return MV;
}

#endif //__umc_vc1_dec_seq_H__
