/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, common declarations
//
*/

#ifndef _ENCODER_VC1_COMMON
#define _ENCODER_VC1_COMMON

#include "umc_vc1_common_defs.h"
#include "umc_vc1_enc_statistic.h"
#include "umc_me_vc1.h"


#define GetMVPredictionB                \
    if (left)                           \
    {                                   \
        mvC_F = &mv1;                   \
        mvC_B = &mv4;                   \
        left->GetMV(mvC_F,1,true);      \
        left->GetMV(mvC_B,1,false);     \
    }                                   \
    if (top)                            \
    {                                   \
        mvA_F = &mv2;                   \
        mvA_B = &mv5;                   \
        top->GetMV(mvA_F,2,true);       \
        top->GetMV(mvA_B,2,false);      \
    }                                   \
    if (topRight)                       \
    {                                   \
        mvB_F = &mv3;                   \
        mvB_B = &mv6;                   \
        topRight->GetMV(mvB_F,2,true);  \
        topRight->GetMV(mvB_B,2,false); \
    }                                   \
    if (!topRight && topLeft)           \
    {                                   \
        mvB_F = &mv3;                   \
        mvB_B = &mv6;                   \
        topLeft->GetMV(mvB_F,3,true);   \
        topLeft->GetMV(mvB_B,3,false);  \
    }
 #define    GetMVPredictionP(forward)   \
    if (left)                           \
    {                                   \
        left->GetMV(&mv1,1,forward);    \
        mvC = &mv1;                     \
    }                                   \
    if (top)                            \
    {                                   \
        top->GetMV(&mv2,2,forward);     \
        mvA = &mv2;                     \
    }                                   \
    if (topRight)                       \
    {                                   \
        topRight->GetMV(&mv3,2,forward);\
        mvB = &mv3;                     \
    }                                   \
    else if(topLeft)                    \
    {                                   \
        topLeft->GetMV(&mv3,3,forward); \
        mvB = &mv3;                     \
    }
 #define    GetMVPredictionPField(forward)  \
    if (left)                               \
    {                                       \
        if (!left->isIntra(1))              \
        {                                   \
            left->GetMV_F(&mv1,1,forward);    \
            mvC =&mv1;                      \
        }                                   \
    }                                       \
    if (top)                                \
    {                                       \
        if (!top->isIntra(2))               \
        {                                   \
            top->GetMV_F(&mv2,2,forward);     \
            mvA = &mv2;                     \
        }                                   \
    }                                       \
    if (topRight)                           \
    {                                       \
        if (!topRight->isIntra(2))          \
        {                                   \
            topRight->GetMV_F(&mv3,2,forward);\
            mvB =&mv3;                      \
        }                                   \
    }                                       \
    else if(topLeft)                        \
    {                                       \
        if (!topLeft->isIntra(2))           \
        {                                   \
            topLeft->GetMV_F(&mv3,2,forward); \
            mvB =&mv3;                      \
        }                                   \
    }
#define    GetMVPredictionP_0(forward)  \
    if (left)                           \
    {                                   \
        left->GetMV(&mv1,1,forward);    \
        mvC = &mv1;                     \
    }                                   \
    if (top)                            \
    {                                   \
        top->GetMV(&mv2,2,forward);     \
        mvA = &mv2;                     \
    }                                   \
    if (topLeft)                        \
    {                                   \
        topLeft->GetMV(&mv3,3,forward); \
        mvB = &mv3;                     \
    }                                   \
    else if(top)                        \
    {                                   \
        top->GetMV(&mv3,3,forward);     \
        mvB = &mv3;                     \
    }

#define    GetMVPredictionP_1(forward)  \
    pCurMBInfo->GetMV(&mv1,0,forward);  \
    mvC = &mv1;                         \
    if (top)                            \
    {                                   \
        top->GetMV(&mv2,3,forward);     \
        mvA = &mv2;                     \
    }                                   \
    if (topRight)                       \
    {                                   \
        topRight->GetMV(&mv3,2,forward);\
        mvB = &mv3;                     \
    }                                   \
    else if(top)                        \
    {                                   \
        top->GetMV(&mv3,2,forward);     \
        mvB = &mv3;                     \
    }

#define    GetMVPredictionP_2(forward)  \
    if (left)                           \
    {                                   \
        left->GetMV(&mv1,3,forward);    \
        mvC = &mv1;                     \
    }                                   \
    pCurMBInfo->GetMV(&mv2,0,forward);  \
    mvA = &mv2;                         \
    pCurMBInfo->GetMV(&mv3,1,forward);  \
    mvB = &mv3;                         \

#define    GetMVPredictionP_3(forward)  \
    pCurMBInfo->GetMV(&mv1,2,forward);  \
    mvC = &mv1;                         \
    pCurMBInfo->GetMV(&mv2,1,forward);  \
    mvA = &mv2;                         \
    pCurMBInfo->GetMV(&mv3,0,forward);  \
    mvB = &mv3;                         \

#define GetMVPredictionPBlk(blk)            \
    mvA = mvB =  mvC = 0;                   \
    switch (blk)                            \
    {                                       \
    case 0:                                 \
        GetMVPredictionP_0(true);  break;   \
    case 1:                                 \
        GetMVPredictionP_1(true);  break;   \
    case 2:                                 \
        GetMVPredictionP_2(true);  break;   \
    case 3:                                 \
        GetMVPredictionP_3(true);  break;   \
    };

namespace UMC_VC1_ENCODER
{
UMC::Status     Enc_IFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_I_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);

UMC::Status     Enc_PFrame(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_PFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);

UMC::Status     Enc_P_Field(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_P_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);

UMC::Status     Enc_PFrameMixed(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_PFrameUpdateMixed(UMC::MeBase* pME, UMC::MeParams* MEParams);

UMC::Status     Enc_BFrame(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_BFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);

UMC::Status     Enc_B_Field(UMC::MeBase* pME, UMC::MeParams* MEParams);
UMC::Status     Enc_B_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams);

bool    IsBottomField(bool bTopFieldFirst, bool bSecondField);
UMC::Status   SetFieldPlane(Ipp8u* pFieldPlane[3], Ipp8u* pPlane[3], Ipp32u planeStep[3], bool bBottom);
UMC::Status   SetFieldPlane(Ipp8u** pFieldPlane, Ipp8u* pPlane, Ipp32u planeStep, bool bBottom);

UMC::Status   SetFieldStep(Ipp32u  fieldStep[3], Ipp32u planeStep[3]);
UMC::Status   SetFieldStep(Ipp32s* fieldStep, Ipp32u* planeStep);

UMC::Status   Set1RefFrwFieldPlane(Ipp8u* pPlane[3],       Ipp32u planeStep[3],
                                   Ipp8u* pFrwPlane[3],    Ipp32u frwStep[3],
                                   Ipp8u* pRaisedPlane[3], Ipp32u raisedStep[3],
                                   eReferenceFieldType     uiReferenceFieldType,
                                   bool bSecondField,      bool bBottom);

UMC::Status   Set1RefFrwFieldPlane(Ipp8u** pPlane,       Ipp32s *planeStep,
                                   Ipp8u* pFrwPlane,    Ipp32u frwStep,
                                   Ipp8u* pRaisedPlane, Ipp32u raisedStep,
                                   eReferenceFieldType     uiReferenceFieldType,
                                   bool bSecondField,      bool bBottom);

UMC::Status   Set2RefFrwFieldPlane(Ipp8u* pPlane[2][3], Ipp32u planeStep[2][3],
                                   Ipp8u* pFrwPlane[3], Ipp32u frwStep[3],
                                   Ipp8u* pRaisedPlane[3], Ipp32u raisedStep[3],
                                   bool bSecondField, bool bBottom);

UMC::Status   Set2RefFrwFieldPlane(Ipp8u** pPlane1, Ipp32s* planeStep1,
                                   Ipp8u** pPlane2, Ipp32s* planeStep2,
                                   Ipp8u* pFrwPlane, Ipp32u frwStep,
                                   Ipp8u* pRaisedPlane, Ipp32u raisedStep,
                                   bool bSecondField, bool bBottom);

UMC::Status   SetBkwFieldPlane(Ipp8u* pPlane[2][3], Ipp32u planeStep[2][3],
                               Ipp8u* pBkwPlane[3], Ipp32u bkwStep[3],
                               bool bBottom);

UMC::Status   SetBkwFieldPlane(Ipp8u** pPlane1, Ipp32s* planeStep1,
                               Ipp8u** pPlane2, Ipp32s* planeStep2,
                               Ipp8u* pBkwPlane, Ipp32u bkwStep,
                               bool bBottom);

bool IsFieldPicture(ePType PicType);

typedef struct
{
    eMVModes   uiMVMode;  //    VC1_ENC_1MV_HALF_BILINEAR
                          //    VC1_ENC_1MV_QUARTER_BICUBIC
                          //    VC1_ENC_1MV_HALF_BICUBIC
                          //    VC1_ENC_MIXED_QUARTER_BICUBIC

    Ipp8u               uiMVRangeIndex;
    Ipp8u               nReferenceFrameDist;
    eReferenceFieldType uiReferenceFieldType;
    sFraction           uiBFraction;     // 0xff/0xff - BI frame
}InitPictureParams;

typedef struct
{
    Ipp8u               uiDecTypeAC1;    //[0,2] - it's used to choose decoding table
    Ipp8u               uiDecTypeDCIntra;//[0,1] - it's used to choose decoding table

    Ipp8u uiMVTab;
    Ipp8u m_uiCBPTab;

}VLCTablesIndex;

 //-------------FORWARD-TRANSFORM-QUANTIZATION---------------------------

    inline IppStatus IntraTransformQuantUniform(Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                       Ipp32s DCQuant, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        Sts = ippiTransform8x8Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
        VC1_ENC_IPP_CHECK(Sts);

        pSrcDst[0] = pSrcDst[0]/(Ipp16s)DCQuant;
        Sts =  ippiQuantIntraUniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

        return Sts;
    }

    inline IppStatus IntraTransformQuantNonUniform(Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                          Ipp32s DCQuant, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        Sts = ippiTransform8x8Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
        VC1_ENC_IPP_CHECK(Sts);

        pSrcDst[0] = pSrcDst[0]/(Ipp16s)DCQuant;
        Sts =  ippiQuantIntraNonuniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

        return Sts;
    }

    inline IppStatus InterTransformQuantUniform(Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                Ipp32s TransformType, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize roiSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        switch (TransformType)
        {
            case VC1_ENC_8x8_TRANSFORM:
                Sts = ippiTransform8x8Fwd_VC1_16s_C1IR(pSrcDst, srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                break;
            case VC1_ENC_4x8_TRANSFORM:
                roiSize.width  = 4;
                roiSize.height = 8;

                Sts =  ippiTransform4x8Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts =  ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts =  ippiTransform4x8Fwd_VC1_16s_C1IR(pSrcDst+4,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts =  ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst + 4, srcDstStep, doubleQuant, roiSize);
                break;
            case VC1_ENC_8x4_TRANSFORM:
                roiSize.width  = 8;
                roiSize.height = 4;

                Sts = ippiTransform8x4Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform8x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                                             srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep), srcDstStep, doubleQuant, roiSize);

                break;
            case VC1_ENC_4x4_TRANSFORM:
                roiSize.height = 4;
                roiSize.width  = 4;

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR(pSrcDst + 4,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR(pSrcDst + 4, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                                            srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                        srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep)+4,
                                                                                                srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterUniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep)+4, srcDstStep, doubleQuant, roiSize);
                break;
            default:
                return ippStsErr;
                break;
        }
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

        return Sts;
    }

    inline IppStatus InterTransformQuantNonUniform(Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                   Ipp32s TransformType, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize roiSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        switch (TransformType)
        {
            case VC1_ENC_8x8_TRANSFORM:
                Sts = ippiTransform8x8Fwd_VC1_16s_C1IR(pSrcDst, srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                break;
            case VC1_ENC_4x8_TRANSFORM:
                roiSize.width  = 4;
                roiSize.height = 8;

                Sts = ippiTransform4x8Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts =  ippiTransform4x8Fwd_VC1_16s_C1IR(pSrcDst+4,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst + 4, srcDstStep, doubleQuant, roiSize);

                break;
            case VC1_ENC_8x4_TRANSFORM:
                roiSize.width  = 8;
                roiSize.height = 4;

                Sts = ippiTransform8x4Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform8x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                                            srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                        srcDstStep, doubleQuant, roiSize);
                break;
            case VC1_ENC_4x4_TRANSFORM:
                roiSize.height = 4;
                roiSize.width  = 4;

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR(pSrcDst,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR(pSrcDst + 4,srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR(pSrcDst + 4, srcDstStep, doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep),
                                                                                             srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep), srcDstStep,
                                                                                    doubleQuant, roiSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiTransform4x4Fwd_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep)+4,
                                                                                               srcDstStep);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiQuantInterNonuniform_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pSrcDst+4*srcDstStep)+4, srcDstStep, doubleQuant, roiSize);
                break;
            default:
                return ippStsErr;
                break;
        }
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

        return Sts;
    }

    typedef IppStatus (*IntraTransformQuantFunction)  (Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                       Ipp32s DCQuant, Ipp32s doubleQuant);
    typedef IppStatus (*InterTransformQuantFunction)  (Ipp16s* pSrcDst, Ipp32s srcDstStep,
                                                       Ipp32s TransformType, Ipp32s doubleQuant);

    inline IppStatus IntraInvTransformQuantUniform(Ipp16s* pSrc, Ipp32s srcStep,
                                                   Ipp16s* pDst, Ipp32s dstStep,
                                                   Ipp32s DCQuant, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize DstSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        Sts = ippiQuantInvIntraUniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                                                                doubleQuant, &DstSize);
        pDst[0] = pSrc[0]*(Ipp16s)DCQuant;
        VC1_ENC_IPP_CHECK(Sts);
        Sts = ippiTransform8x8Inv_VC1_16s_C1IR(pDst,dstStep,DstSize);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

         return Sts;
    }

    inline IppStatus IntraInvTransformQuantNonUniform(Ipp16s* pSrc, Ipp32s srcStep,
                                                      Ipp16s* pDst, Ipp32s dstStep,
                                                      Ipp32s DCQuant, Ipp32s doubleQuant)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize DstSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        Sts = ippiQuantInvIntraNonuniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                                                                    doubleQuant, &DstSize);
        pDst[0] = pSrc[0]*(Ipp16s)DCQuant;
        VC1_ENC_IPP_CHECK(Sts);
        Sts = ippiTransform8x8Inv_VC1_16s_C1IR(pDst,dstStep,DstSize);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

         return Sts;
    }

    inline IppStatus InterInvTransformQuantUniform(Ipp16s* pSrc, Ipp32s srcStep,
                                                   Ipp16s* pDst, Ipp32s dstStep,
                                                   Ipp32s doubleQuant, Ipp32s TransformType)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize roiSize = {8, 8};
        IppiSize dstSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        switch (TransformType)
        {
            case VC1_ENC_8x8_TRANSFORM:
                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                                                                  doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x8Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                break;
            case VC1_ENC_4x8_TRANSFORM:
                roiSize.width  = 4;
                roiSize.height = 8;

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                                                                doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x8Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc + 4, srcStep, pDst + 4, dstStep,
                                                                        doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x8Inv_VC1_16s_C1IR(pDst + 4, dstStep, dstSize);

                break;
            case VC1_ENC_8x4_TRANSFORM:
                roiSize.width  = 8;
                roiSize.height = 4;

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc, srcStep,
                                        pDst, dstStep,  doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x4Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts =ippiQuantInvInterUniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc + 4*srcStep), srcStep,
                    (Ipp16s*)((Ipp8u*)pDst + 4*dstStep) , dstStep,  doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep),dstStep,dstSize);
                break;
            case VC1_ENC_4x4_TRANSFORM:
                roiSize.height = 4;
                roiSize.width  = 4;

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc, srcStep,
                                                            pDst, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R(pSrc + 4, srcStep,
                                                        pDst + 4, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR(pDst+4,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc+4*srcStep), srcStep,
                    (Ipp16s*)((Ipp8u*)pDst+4*dstStep), dstStep,  doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep),dstStep,
                                                                                            dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterUniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc + 4*srcStep)+4, srcStep,
                    (Ipp16s*)((Ipp8u*)pDst + 4*dstStep)+4, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep)+4,dstStep, dstSize);
                break;
            default:
                return ippStsErr;
                break;
        }
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

        return Sts;
    }

    inline IppStatus InterInvTransformQuantNonUniform(Ipp16s* pSrc, Ipp32s srcStep,
                                                      Ipp16s* pDst, Ipp32s dstStep,
                                                      Ipp32s doubleQuant, Ipp32s TransformType)
    {
        IppStatus Sts = ippStsNoErr;
        IppiSize roiSize = {8, 8};
        IppiSize dstSize = {8, 8};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
        switch (TransformType)
        {
            case VC1_ENC_8x8_TRANSFORM:
                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                    doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x8Inv_VC1_16s_C1IR(pDst, dstStep, dstSize);
                break;
            case VC1_ENC_4x8_TRANSFORM:
                roiSize.width  = 4;
                roiSize.height = 8;

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                    doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x8Inv_VC1_16s_C1IR(pDst, dstStep, dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts =ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc + 4, srcStep,
                    pDst + 4, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x8Inv_VC1_16s_C1IR(pDst + 4, dstStep, dstSize);
                break;
            case VC1_ENC_8x4_TRANSFORM:
                roiSize.width  = 8;
                roiSize.height = 4;

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc, srcStep, pDst, dstStep,
                    doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x4Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc + 4*srcStep), srcStep,
                    (Ipp16s*)((Ipp8u*)pDst + 4*dstStep) , dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform8x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep),dstStep,dstSize);
                break;
            case VC1_ENC_4x4_TRANSFORM:
                roiSize.height = 4;
                roiSize.width  = 4;

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc, srcStep,
                    pDst, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR(pDst,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R(pSrc + 4, srcStep,
                    pDst + 4, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR(pDst+4,dstStep,dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc+4*srcStep), srcStep,
                    (Ipp16s*)((Ipp8u*)pDst+4*dstStep), dstStep,  doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep),dstStep, dstSize);
                VC1_ENC_IPP_CHECK(Sts);

                Sts = ippiQuantInvInterNonuniform_VC1_16s_C1R((Ipp16s*)((Ipp8u*)pSrc + 4*srcStep)+4, srcStep,
                    (Ipp16s*)((Ipp8u*)pDst + 4*dstStep)+4, dstStep, doubleQuant, roiSize, &dstSize);
                VC1_ENC_IPP_CHECK(Sts);
                Sts = ippiTransform4x4Inv_VC1_16s_C1IR((Ipp16s*)((Ipp8u*)pDst + 4*dstStep)+4,dstStep, dstSize);
                break;
            default:
                return ippStsErr;
                break;
        }

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
        return Sts;
    }

    typedef IppStatus (*IntraInvTransformQuantFunction)  (Ipp16s* pSrc, Ipp32s srcStep,
                                                       Ipp16s* pDst, Ipp32s dstStep,
                                                       Ipp32s DCQuant, Ipp32s doubleQuant);
    typedef IppStatus (*InterInvTransformQuantFunction)  (Ipp16s* pSrc, Ipp32s srcStep,
                                                       Ipp16s* pDst, Ipp32s dstStep,
                                                       Ipp32s doubleQuant, Ipp32s TransformType);
    ///////////////////

    typedef void        (*CalculateChromaFunction)(sCoordinate LumaMV, sCoordinate * pChroma);

    Ipp8u               Get_CBPCY(Ipp32u MBPatternCur, Ipp32u CBPCYTop, Ipp32u CBPCYLeft, Ipp32u CBPCYULeft);
    //eDirection          GetDCPredDirectionSM(Ipp16s dcA, Ipp16s dcB, Ipp16s dcC);
    //void                Diff_1x7_16s(Ipp16s* pSrcDst, Ipp16s step, const Ipp16s* pDst);
    //void                Diff_7x1_16s(Ipp16s* pSrc, const Ipp16s* pDst);
    Ipp32s              SumSqDiff_1x7_16s(Ipp16s* pSrc, Ipp32u step, Ipp16s* pPred);
    Ipp32s              SumSqDiff_7x1_16s(Ipp16s* pSrc, Ipp16s* pPred);
    //Ipp32u              GetNumZeros(Ipp16s* pSrc, Ipp32u srcStep, bool bIntra);
    //Ipp32u              GetNumZeros(Ipp16s* pSrc, Ipp32u srcStep);

    Ipp8u               GetMode( Ipp8u &run,  Ipp16s &level, const Ipp8u *pTableDR, const Ipp8u *pTableDL, bool& sign);
    //bool                GetRLCode(Ipp32s run, Ipp32s level, IppVCHuffmanSpec_32s *pTable, Ipp32s &code, Ipp32s &len);
    //Ipp8u               GetLength_16s(Ipp16s value);
    //Ipp8u               Zigzag( Ipp16s*                pBlock,
    //                        Ipp32u                 blockStep,
    //                        bool                   bIntra,
    //                        const Ipp8u*           pScanMatrix,
    //                        Ipp8u*                 pRuns,
    //                        Ipp16s*                pLevels);

    void                Diff8x8 (const Ipp16s* pSrc1, Ipp32s src1Step,
                            const Ipp16s* pSrc2, Ipp32s src2Step,
                            const Ipp8u*  pCoeff,Ipp32s coefStep,
                            Ipp16s* pDst, Ipp32s dstStep);

    void                Copy8x8_16s(Ipp16s*  pSrc, Ipp32u   srcStep,
                            Ipp16s*  pDst, Ipp32u   dstStep);
    //void                Copy16x16_16s( Ipp16s*  pSrc, Ipp32u   srcStep,
    //                        Ipp16s*  pDst, Ipp32u   dstStep);

    Ipp16s median3(Ipp16s a, Ipp16s b, Ipp16s c);
    Ipp16s median4(Ipp16s a, Ipp16s b, Ipp16s c, Ipp16s d);

    void PredictMV(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC, sCoordinate* res);

    inline Ipp16s VC1abs(Ipp16s value);
    void ScalePredict(sCoordinate * MV, Ipp32s x,Ipp32s y,sCoordinate MVPredMin, sCoordinate MVPredMax);
    //Ipp32s FullSearch(const Ipp8u* pSrc, Ipp32u srcStep, const Ipp8u* pPred, Ipp32u predStep, sCoordinate Min, sCoordinate Max,sCoordinate * MV);
    //Ipp32s SumBlock16x16(const Ipp8u* pSrc, Ipp32u srcStep);
    //Ipp32s SumBlockDiff16x16(const Ipp8u* pSrc1, Ipp32u srcStep1,const Ipp8u* pSrc2, Ipp32u srcStep2);
    //Ipp32s SumBlockDiffBPred16x16(const Ipp8u* pSrc, Ipp32u srcStep,const Ipp8u* pPred1, Ipp32u predStep1,
    //                                                            const Ipp8u* pPred2, Ipp32u predStep2);
    void  GetMVDirectHalf  (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void  GetMVDirectQuarter(Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void  GetChromaMV   (sCoordinate LumaMV, sCoordinate * pChroma);
    void  GetChromaMVFast(sCoordinate LumaMV, sCoordinate * pChroma);

    void GetIntQuarterMV(sCoordinate MV,sCoordinate *pMVInt, sCoordinate *pMVIntQuarter);
    //void GetQMV(sCoordinate *pMV,sCoordinate MVInt, sCoordinate MVIntQuarter);

    //void GetBlockType(Ipp16s* pBlock, Ipp32s step, Ipp8u Quant, eTransformType& type);
    //bool GetMBTSType(Ipp16s** ppBlock, Ipp32u* pStep, Ipp8u Quant , eTransformType* pTypes);

    //Ipp8u GetBlockPattern(Ipp16s* pSrc, Ipp32u srcStep);

    Ipp8u HybridPrediction(     sCoordinate * mvPred,
                                const sCoordinate * MV,
                                const sCoordinate * mvA,
                                const sCoordinate * mvC);

//--Begin----------Interpolation---------------

    typedef IppStatus  ( __STDCALL *InterpolateFunction)  (const IppVCInterpolate_8u* pParam);

    inline void SetInterpolationParamsSrc(IppVCInterpolate_8u* pInterpolation, Ipp8u* pSrc, Ipp32u step, const sCoordinate* mvInt, const sCoordinate* mvQuarter)
    {
        pInterpolation->pSrc = pSrc + mvInt->x + mvInt->y*(Ipp32s)step;
        pInterpolation->dx   = mvQuarter -> x;
        pInterpolation->dy   = mvQuarter -> y;
    }
//--End------------Interpolation---------------

    inline Ipp16u vc1_abs_16s(Ipp16s pSrc)
    {
        Ipp16u S;
        S = pSrc >> 15;
        S = (pSrc + S)^S;
        return S;
    }
    inline Ipp32u vc1_abs_32s(Ipp32s pSrc)
    {
        Ipp32u S;
        S = pSrc >> 31;
        S = (pSrc + S)^S;
        return S;
    }
    bool MaxAbsDiff(Ipp8u* pSrc1, Ipp32u step1,Ipp8u* pSrc2, Ipp32u step2,IppiSize roiSize,Ipp32u max);
    void InitScaleInfo(sScaleInfo* pInfo, bool bCurSecondField ,bool bBottom,
                        Ipp8u ReferenceFrameDist, Ipp8u MVRangeIndex);
    bool PredictMVField2(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC,
                          sCoordinate* res, sScaleInfo* pInfo, bool bSecondField,
                          sCoordinate* predAEx,sCoordinate* predCEx, bool bBackward, bool bHalf);
    Ipp8u isHybridPredictionFields(     sCoordinate * mvPred,
                                        const sCoordinate * mvA,
                                        const sCoordinate * mvC,
                                        Ipp32u        th);
    void PredictMVField1(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC, sCoordinate* res);
    void initMVFieldInfo(bool extenedX, bool extendedY, const Ipp32u* pVLCTable ,sMVFieldInfo* MVFieldInfo);

    void  GetMVDirectCurrHalfBackHalf      (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void  GetMVDirectCurrQuarterBackHalf   (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void  GetMVDirectCurrHalfBackQuarter   (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void  GetMVDirectCurrQuarterBackQuarter(Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB);
    void InitScaleInfoBackward (sScaleInfo* pInfo, bool bCurSecondField ,bool bBottom,  Ipp8u ReferenceFrameDist, Ipp8u MVRangeIndex);
}

#endif
