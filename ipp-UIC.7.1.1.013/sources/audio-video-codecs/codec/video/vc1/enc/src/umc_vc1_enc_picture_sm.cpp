/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, simple profile picture level functionality
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include <new>

#include "umc_vc1_enc_picture_sm.h"
#include "umc_vc1_enc_common.h"
#include "umc_vc1_common_enc_tables.h"
#include "umc_vc1_common_zigzag_tbl.h"
#include "umc_vc1_enc_block.h"
#include "umc_vc1_enc_block_template.h"
#include "umc_vc1_enc_debug.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_enc_deblocking.h"
#include "umc_vc1_enc_statistic.h"
#include "umc_vc1_enc_pred.h"


namespace UMC_VC1_ENCODER
{
UMC::Status  VC1EncoderPictureSM::CheckParameters(vm_char* pLastError)
{
    if (!m_pSequenceHeader)
    {
        vm_string_sprintf(pLastError,VM_STRING("Error. pic. header parameter: seq. header is NULL\n"));
        return UMC::UMC_ERR_FAILED;
    }
    if (m_uiPictureType != VC1_ENC_I_FRAME &&
        m_uiPictureType != VC1_ENC_P_FRAME &&
        m_uiPictureType != VC1_ENC_P_FRAME_MIXED &&
       (!((m_uiPictureType == VC1_ENC_BI_FRAME||m_uiPictureType == VC1_ENC_B_FRAME)&& m_pSequenceHeader->IsBFrames())))
    {
        vm_string_sprintf(pLastError,VM_STRING("Error. pic. header parameter: picture type is incorrect\n"));
        return UMC::UMC_ERR_FAILED;
    }
    if (m_bFrameInterpolation && !m_pSequenceHeader->IsFrameInterpolation())
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: FrameInterpolation\n"));
        m_bFrameInterpolation = false;
    }
    if (m_bRangeRedution && !m_pSequenceHeader->IsRangeRedution())
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: RangeRedution\n"));
        m_bRangeRedution = false;
    }
    if (m_uiFrameCount >3 )
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: FrameCount\n"));
        m_uiFrameCount = 0;
    }
    if (m_uiResolution && m_pSequenceHeader->IsMultiRes() || m_uiResolution>3)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: Resolution\n"));
        m_uiResolution = 0;
    }
    if (m_uiDecTypeAC1>2)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: DecTypeAC1\n"));
        m_uiDecTypeAC1 = 0;
    }

    if (m_uiDecTypeDCIntra>1)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: DecTypeAC2\n"));
        m_uiDecTypeDCIntra = 0;
    }
    if (!m_uiQuantIndex || m_uiQuantIndex >31)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: m_uiQuantIndex\n"));
        m_uiQuantIndex = 1;
    }
    if (m_bHalfQuant && m_uiQuantIndex>8)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: m_bHalfQuant\n"));
        m_bHalfQuant = false;
    }

    if(m_uiMVMode == VC1_ENC_MIXED_QUARTER_BICUBIC && (m_uiPictureType == VC1_ENC_P_FRAME || m_uiPictureType == VC1_ENC_B_FRAME))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: m_uiMVMode\n"));
        m_uiMVMode = VC1_ENC_1MV_HALF_BILINEAR;
    }

    switch (m_pSequenceHeader->GetQuantType())
    {
    case VC1_ENC_QTYPE_IMPL:
        m_bUniformQuant = (m_uiQuantIndex>8)? false:true;
        m_uiQuant       = quantValue[m_uiQuantIndex];
        break;
    case VC1_ENC_QTYPE_UF:
        m_bUniformQuant = true;
        m_uiQuant       = m_uiQuantIndex;
        break;
    case VC1_ENC_QTYPE_NUF:
        m_bUniformQuant = false;
        m_uiQuant       = m_uiQuantIndex;
        break;
    }
    if (m_uiMVRangeIndex>3 || (!m_uiMVRangeIndex && m_pSequenceHeader->IsExtendedMV()))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: RangeIndex\n"));
        m_uiMVRangeIndex = 0;
    }
    if (m_uiPictureType == VC1_ENC_I_FRAME )
    {
        return CheckParametersI(pLastError);
    }
    return CheckParametersP(pLastError);
}
UMC::Status  VC1EncoderPictureSM::CheckParametersI(vm_char* pLastError)
{
    if (m_uiDecTypeAC2>2)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: DecTypeAC2\n"));
        m_uiDecTypeAC2 = 0;
    }
    if ((m_uiBFraction.denom < 2|| m_uiBFraction.denom >8)&& (m_uiBFraction.denom != 0xff))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: m_uiBFraction.denom\n"));
        m_uiBFraction.denom = 2;
    }
    if ((m_uiBFraction.num  < 1|| m_uiBFraction.num >= m_uiBFraction.denom)&& (m_uiBFraction.num != 0xff))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: m_uiBFraction.num\n"));
        m_uiBFraction.num = 1;
    }
    return UMC::UMC_OK;
}
UMC::Status  VC1EncoderPictureSM::CheckParametersP(vm_char* pLastError)
{

    if (m_bIntensity &&  (m_pSequenceHeader->GetProfile() == VC1_ENC_PROFILE_S))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: Intensity=0\n"));
        m_bIntensity = false;
    }
    if (m_bIntensity)
    {
        if (m_uiIntensityLumaScale>63)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: IntensityLumaScale=63\n"));
            m_uiIntensityLumaScale=63;
        }
        if (m_uiIntensityLumaShift>63)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: IntensityLumaShift=63\n"));
            m_uiIntensityLumaShift=63;
        }
    }
    else
    {
        if (m_uiIntensityLumaScale>0)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: IntensityLumaScale=0\n"));
            m_uiIntensityLumaScale=0;
        }
        if (m_uiIntensityLumaShift>0)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: IntensityLumaShift=0\n"));
            m_uiIntensityLumaShift=0;
        }
    }
    if (m_uiMVTab>4)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: MVTab=4\n"));
        m_uiMVTab=4;
    }
    if (m_uiCBPTab>4)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: CBPTab=4\n"));
        m_uiCBPTab=4;
    }
    if (m_uiAltPQuant>0 && m_pSequenceHeader->GetDQuant()==0)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: AltPQuant=0\n"));
        m_uiAltPQuant=0;
    }
    else if (m_pSequenceHeader->GetDQuant()!=0 && (m_uiAltPQuant==0||m_uiAltPQuant>31))
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: AltPQuant=Quant\n"));
        m_uiAltPQuant = m_uiQuant;
    }
    if ( m_QuantMode != VC1_ENC_QUANT_SINGLE && m_pSequenceHeader->GetDQuant()==0)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: QuantMode = VC1_ENC_QUANT_SINGLE\n"));
        m_QuantMode = VC1_ENC_QUANT_SINGLE;
    }
    if (m_bVSTransform && !m_pSequenceHeader->IsVSTransform())
    {
       vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: VSTransform = false\n"));
       m_bVSTransform = false;
    }
    if (!m_bVSTransform && m_uiTransformType!= VC1_ENC_8x8_TRANSFORM)
    {
       vm_string_sprintf(pLastError,VM_STRING("Warning. pic. header parameter: TransformType == VC1_ENC_8x8_TRANSFORM\n"));
       m_uiTransformType = VC1_ENC_8x8_TRANSFORM;
    }
    return UMC::UMC_OK;
}

UMC::Status VC1EncoderPictureSM::Init (VC1EncoderSequenceSM* SequenceHeader, VC1EncoderMBs* pMBs,  VC1EncoderCodedMB* pCodedMB)
{
    //UMC::Status err = UMC::UMC_OK;

    if (!SequenceHeader || !pMBs || !pCodedMB)
        return UMC::UMC_ERR_NULL_PTR;

    Close();

    m_pSequenceHeader = SequenceHeader;

    m_pMBs     = pMBs;
    m_pCodedMB = pCodedMB;

    return UMC::UMC_OK;
}

UMC::Status VC1EncoderPictureSM::Close()
{

   Reset();
   ResetPlanes();
   return UMC::UMC_OK;
}

UMC::Status     VC1EncoderPictureSM::SetPlaneParams  (Frame* pFrame, ePlaneType type)
{
    Ipp8u** pPlane;
    Ipp32u* pPlaneStep;
    switch(type)
    {
    case  VC1_ENC_CURR_PLANE:
        pPlane     = m_pPlane;
        pPlaneStep = m_uiPlaneStep;
        break;
    case  VC1_ENC_RAISED_PLANE:
        pPlane     = m_pRaisedPlane;
        pPlaneStep = m_uiRaisedPlaneStep;
        break;
    case  VC1_ENC_FORWARD_PLANE:
        pPlane     = m_pForwardPlane;
        pPlaneStep = m_uiForwardPlaneStep;
        break;
     case  VC1_ENC_BACKWARD_PLANE:
        pPlane     = m_pBackwardPlane;
        pPlaneStep = m_uiBackwardPlaneStep;
        break;
     default:
         return UMC::UMC_ERR_FAILED;
   }
   pPlane[0]       = pFrame->GetYPlane();
   pPlane[1]       = pFrame->GetUPlane();
   pPlane[2]       = pFrame->GetVPlane();
   pPlaneStep[0]   = pFrame->GetYStep();
   pPlaneStep[1]   = pFrame->GetUStep();
   pPlaneStep[2]   = pFrame->GetVStep();

   return UMC::UMC_OK;
}


UMC::Status VC1EncoderPictureSM::SetInitPictureParams(InitPictureParams* InitPicParam)
{
    m_uiMVMode           = InitPicParam->uiMVMode;
    m_uiBFraction.num    = InitPicParam->uiBFraction.num;
    m_uiBFraction.denom  = InitPicParam->uiBFraction.denom;
    m_uiMVRangeIndex     = InitPicParam->uiMVRangeIndex;

    SetPictureQuantParams(8, 0);

    return UMC::UMC_OK;
}

UMC::Status     VC1EncoderPictureSM::SetVLCTablesIndex(VLCTablesIndex*       VLCIndex)
{
    m_uiMVTab           = VLCIndex->uiMVTab;
    m_uiDecTypeAC1      = VLCIndex->uiDecTypeAC1;    //[0,2] - it's used to choose decoding table
    m_uiDecTypeDCIntra  = VLCIndex->uiDecTypeDCIntra; //[0,1] - it's used to choose decoding table

    return UMC::UMC_OK;
}

UMC::Status VC1EncoderPictureSM::SetPictureParams(ePType Type, Ipp16s* pSavedMV)
{
    m_uiPictureType     =  Type;

    m_pSavedMV  = pSavedMV;

    if (m_pSequenceHeader->IsFrameInterpolation())
        m_bFrameInterpolation   =  false;              // true or false
    else
        m_bFrameInterpolation   =  false;

    if (m_pSequenceHeader->IsRangeRedution())
        m_bRangeRedution      = false;
    else
        m_bRangeRedution      = false;                  // Should be equal to 0 in simple profile

    //if (m_pSequenceHeader->IsExtendedMV())
    //    m_uiMVRangeIndex = 1;                            // [0,3]
    //else
    //    m_uiMVRangeIndex = 0;

    switch (m_uiPictureType)
    {
    case VC1_ENC_I_FRAME:
        return SetPictureParamsI();
    case VC1_ENC_P_FRAME:
        return SetPictureParamsP();
    case VC1_ENC_P_FRAME_MIXED:
        return SetPictureParamsP();
    case VC1_ENC_B_FRAME:
        return SetPictureParamsB();
    case VC1_ENC_SKIP_FRAME:
        return UMC::UMC_OK;
    default:
        break;
    }
    return UMC::UMC_ERR_NOT_IMPLEMENTED;

}

UMC::Status VC1EncoderPictureSM::SetPictureQuantParams(Ipp8u uiQuantIndex, bool bHalfQuant)
{
    m_uiQuantIndex = uiQuantIndex;
    m_bHalfQuant = bHalfQuant;

     if (m_uiQuantIndex > 31)
         return UMC::UMC_ERR_FAILED;

     if (m_uiQuantIndex == 0)
         return UMC::UMC_ERR_FAILED;

    switch (m_pSequenceHeader->GetQuantType())
    {
    case VC1_ENC_QTYPE_IMPL:
        m_bUniformQuant = (m_uiQuantIndex>8)? false:true;
        m_uiQuant       = quantValue[m_uiQuantIndex];
        break;
    case VC1_ENC_QTYPE_EXPL:
        m_bUniformQuant = true;                         // can be true or false
        m_uiQuant       = m_uiQuantIndex;
        break;
    case VC1_ENC_QTYPE_UF:
        m_bUniformQuant = true;
        m_uiQuant       = m_uiQuantIndex;
        break;
    case VC1_ENC_QTYPE_NUF:
        m_bUniformQuant = false;
        m_uiQuant       = m_uiQuantIndex;
        break;
    default:
        return UMC::UMC_ERR_FAILED;
    }

    return UMC::UMC_OK;
}


UMC::Status VC1EncoderPictureSM::SetPictureParamsI()
{
    //m_uiBFraction.denom     = 2;                                // [2,8]
    //m_uiBFraction.num       = 1;                                // [1, m_uiBFraction.denom - 1]
    m_uiDecTypeAC2          = 0;                                // [0,2] - it's used to choose decoding table

    if (m_pSequenceHeader->IsMultiRes())
        m_uiResolution = VC1_ENC_RES_FULL_HOR | VC1_ENC_RES_FULL_VER;
                                                        /*..HALF_HOR|..HALF_VER, ..FULL_HOR |
                                                         ..HALF_VER, ..HALF_HOR | ..FULL_VER*/
    else
        m_uiResolution = VC1_ENC_RES_FULL_HOR | VC1_ENC_RES_FULL_VER;

    return UMC::UMC_OK;
}
UMC::Status VC1EncoderPictureSM::SetPictureParamsP()
{
    //m_uiMVMode = (bMixed)? VC1_ENC_MIXED_QUARTER_BICUBIC: VC1_ENC_1MV_HALF_BILINEAR;
    //                                                            //    VC1_ENC_1MV_HALF_BILINEAR
    //                                                            //    VC1_ENC_1MV_QUARTER_BICUBIC
    //                                                            //    VC1_ENC_1MV_HALF_BICUBIC
    //                                                            //    VC1_ENC_MIXED_QUARTER_BICUBIC
    if (m_pSequenceHeader->IsMultiRes())
        m_uiResolution = VC1_ENC_RES_FULL_HOR | VC1_ENC_RES_FULL_VER;
                                                        /*..HALF_HOR|..HALF_VER, ..FULL_HOR |
                                                         ..HALF_VER, ..HALF_HOR | ..FULL_VER*/
    else
        m_uiResolution = VC1_ENC_RES_FULL_HOR | VC1_ENC_RES_FULL_VER;

    if (m_pSequenceHeader->GetProfile() == VC1_ENC_PROFILE_S)
        m_bIntensity  = false;
    else
         m_bIntensity = false; //true or false

    if (m_bIntensity)
    {
        m_uiIntensityLumaScale=5;                               //[0,63]
        m_uiIntensityLumaShift=8;                               //[0,63]
    }
    else
    {
        m_uiIntensityLumaScale=0;
        m_uiIntensityLumaShift=0;
    }

    //m_uiCBPTab  =0;                                             //[0,4]

    if (m_pSequenceHeader->GetDQuant()==0)
    {
        m_uiAltPQuant=0;
    }
    else
    {
        m_uiAltPQuant = m_uiQuant;          // [1,31]
    }

    switch( m_pSequenceHeader->GetDQuant())
    {
    case 0:
        m_QuantMode = VC1_ENC_QUANT_SINGLE;
        break;
    case 1:
         m_QuantMode = VC1_ENC_QUANT_SINGLE;//        VC1_ENC_QUANT_SINGLE,
                                            //        VC1_ENC_QUANT_MB_ANY,
                                            //        VC1_ENC_QUANT_MB_PAIR,
                                            //        VC1_ENC_QUANT_EDGE_ALL,
                                            //        VC1_ENC_QUANT_EDGE_LEFT,
                                            //        VC1_ENC_QUANT_EDGE_TOP,
                                            //        VC1_ENC_QUANT_EDGE_RIGHT,
                                            //        VC1_ENC_QUANT_EDGE_BOTTOM,
                                            //        VC1_ENC_QUANT_EDGES_LEFT_TOP,
                                            //        VC1_ENC_QUANT_EDGES_TOP_RIGHT,
                                            //        VC1_ENC_QUANT_EDGES_RIGHT_BOTTOM,
                                            //        VC1_ENC_QUANT_EDGSE_BOTTOM_LEFT,
         break;
    case 2:
        m_QuantMode = VC1_ENC_QUANT_MB_PAIR;
        break;
    }
    if (!m_pSequenceHeader->IsVSTransform())
    {
       m_bVSTransform = false;
    }
    else
    {
        m_bVSTransform = false;                          // true & false

    }
    if (!m_bVSTransform)
    {
       m_uiTransformType = VC1_ENC_8x8_TRANSFORM;
    }
    else
    {
        m_uiTransformType = VC1_ENC_8x8_TRANSFORM;      //VC1_ENC_8x8_TRANSFORM
                                                        //VC1_ENC_8x4_TRANSFORM
                                                        //VC1_ENC_4x8_TRANSFORM
                                                        //VC1_ENC_4x4_TRANSFORM

    }
    return UMC::UMC_OK;


}
UMC::Status VC1EncoderPictureSM::SetPictureParamsB()
{
   //m_uiMVMode = VC1_ENC_1MV_HALF_BILINEAR;                      //    VC1_ENC_1MV_HALF_BILINEAR
   //                                                             //    VC1_ENC_1MV_QUARTER_BICUBIC

    //m_uiCBPTab  =0;                                             //[0,4]

    if (m_pSequenceHeader->GetDQuant()==0)
    {
        m_uiAltPQuant=0;
    }
    else
    {
        m_uiAltPQuant = m_uiQuant;          // [1,31]
    }

    switch( m_pSequenceHeader->GetDQuant())
    {
    case 0:
        m_QuantMode = VC1_ENC_QUANT_SINGLE;
        break;
    case 1:
         m_QuantMode = VC1_ENC_QUANT_SINGLE;//        VC1_ENC_QUANT_SINGLE,
                                            //        VC1_ENC_QUANT_MB_ANY,
                                            //        VC1_ENC_QUANT_MB_PAIR,
                                            //        VC1_ENC_QUANT_EDGE_ALL,
                                            //        VC1_ENC_QUANT_EDGE_LEFT,
                                            //        VC1_ENC_QUANT_EDGE_TOP,
                                            //        VC1_ENC_QUANT_EDGE_RIGHT,
                                            //        VC1_ENC_QUANT_EDGE_BOTTOM,
                                            //        VC1_ENC_QUANT_EDGES_LEFT_TOP,
                                            //        VC1_ENC_QUANT_EDGES_TOP_RIGHT,
                                            //        VC1_ENC_QUANT_EDGES_RIGHT_BOTTOM,
                                            //        VC1_ENC_QUANT_EDGSE_BOTTOM_LEFT,
         break;
    case 2:
        m_QuantMode = VC1_ENC_QUANT_MB_PAIR;
        break;
    }
    if (!m_pSequenceHeader->IsVSTransform())
    {
       m_bVSTransform = false;  // true & false
    }
    else
    {
        m_bVSTransform = false;

    }
    if (!m_bVSTransform)
    {
       m_uiTransformType = VC1_ENC_8x8_TRANSFORM;
    }
    else
    {
        m_uiTransformType = VC1_ENC_8x8_TRANSFORM;      //VC1_ENC_8x8_TRANSFORM
                                                        //VC1_ENC_8x4_TRANSFORM
                                                        //VC1_ENC_4x8_TRANSFORM
                                                        //VC1_ENC_4x4_TRANSFORM
    }
    return UMC::UMC_OK;
}

UMC::Status VC1EncoderPictureSM::WriteSkipPictureHeader(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status err = UMC::UMC_OK;
    err = pCodedPicture->MakeBlankSegment(2);
    if (err!= UMC::UMC_OK)
        return err;
    err = pCodedPicture->PutBits(0,8);
    return err;

}
UMC::Status VC1EncoderPictureSM::WriteIPictureHeader(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status err      =   UMC::UMC_OK;
    bool   bBframes =   m_pSequenceHeader->IsBFrames();

    err = pCodedPicture->MakeBlankSegment(2);
    if (err!= UMC::UMC_OK)
        return err;

    if (m_pSequenceHeader->IsFrameInterpolation())
    {
        err =pCodedPicture->PutBits(m_bFrameInterpolation,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(m_uiFrameCount,2);
    if (err != UMC::UMC_OK)
        return err;
    if (m_pSequenceHeader->IsRangeRedution())
    {
        err = pCodedPicture->PutBits(m_bRangeRedution,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(frameTypeCodesVLC[bBframes][2*0],frameTypeCodesVLC[bBframes][2*0+1]);
    if (err!= UMC::UMC_OK)
        return err;

    err = pCodedPicture->PutBits(0,7);    /*buffer fullness - 7 bits*/
    if (err != UMC::UMC_OK)
        return err;

    err = pCodedPicture->PutBits(m_uiQuantIndex, 5);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuantIndex <= 8)
    {
        err = pCodedPicture->PutBits(m_bHalfQuant, 1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->GetQuantType()== VC1_ENC_QTYPE_EXPL)
    {
        err = pCodedPicture->PutBits(m_bUniformQuant,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->IsExtendedMV())
    {
        err = pCodedPicture->PutBits(MVRangeCodesVLC[m_uiMVRangeIndex*2],MVRangeCodesVLC[m_uiMVRangeIndex*2+1]);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->IsMultiRes())
    {
        err = pCodedPicture->PutBits(this->m_uiResolution,2);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(ACTableCodesVLC[2*m_uiDecTypeAC1],ACTableCodesVLC[2*m_uiDecTypeAC1+1]);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(ACTableCodesVLC[2*m_uiDecTypeAC2],ACTableCodesVLC[2*m_uiDecTypeAC2+1]);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiDecTypeDCIntra,1);

    return err;
}

UMC::Status VC1EncoderPictureSM::CompletePicture(VC1EncoderBitStreamSM* pCodedPicture, double dPTS, size_t  len)
{
    UMC::Status   err       = UMC::UMC_OK;
    Ipp32u   temp           = 0;
    Ipp32u   dataLen        = 0;
    Ipp32u   uTime          = (Ipp32u)(dPTS*1000000.0);




    dataLen = (Ipp32u)(pCodedPicture->GetDataLen() - len - 8);
//    assert(dataLen>=0);



    temp = ((m_uiPictureType == VC1_ENC_I_FRAME)<< 31) | (dataLen & 0x00FFFFFF);

    err = pCodedPicture->FillBlankSegment(temp);
    VC1_ENC_CHECK (err)

    err = pCodedPicture->FillBlankSegment(uTime);
    VC1_ENC_CHECK (err)

    pCodedPicture->DeleteBlankSegment();
    return err;
}

UMC::Status  VC1EncoderPictureSM::PAC_IFrame(UMC::MeParams* /*MEParams*/)
{
    UMC::Status                 err = UMC::UMC_OK;
    Ipp32u                      i=0, j=0, blk = 0;
    bool                        bRaiseFrame = (m_pRaisedPlane[0])&&(m_pRaisedPlane[1])&&(m_pRaisedPlane[2]);

    Ipp8u*                      pCurMBRow[3] =  {m_pPlane[0],m_pPlane[1],m_pPlane[2]};
    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();


    //forward transform quantization
    IntraTransformQuantFunction    TransformQuantACFunction = (m_bUniformQuant) ? IntraTransformQuantUniform :
                                                                        IntraTransformQuantNonUniform;
    //inverse transform quantization
    IntraInvTransformQuantFunction InvTransformQuantACFunction = (m_bUniformQuant) ? IntraInvTransformQuantUniform :
                                                                        IntraInvTransformQuantNonUniform;

    Ipp8u                       defDCPredCoeff  = (m_pSequenceHeader->IsOverlap()&& m_uiQuant>=9)? 0:1;
    eDirection                  direction[VC1_ENC_NUMBER_OF_BLOCKS];
    bool                        dACPrediction   = true;
    VC1EncoderMBData            TempBlock;
    Ipp16s                      tempBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];
    IppiSize                    blkSize     = {8,8};
    IppiSize                    blkSizeLuma = {16,16};

    Ipp8u deblkPattern = 0;//3 bits: right 1 bit - left/not left
                           //left 2 bits: 00 - top row, 01-middle row, 11 - bottom row
    Ipp8u deblkMask = (m_pSequenceHeader->IsLoopFilter()) ? 0xFE : 0;

    fDeblock_I_MB* pDeblk_I_MB = Deblk_I_MBFunction;

    Ipp8u                       doubleQuant     =  2*m_uiQuant + m_bHalfQuant;
    Ipp8u                       DCQuant         =  DCQuantValues[m_uiQuant];
    Ipp16s                      defPredictor    =  defDCPredCoeff*(1024 + (DCQuant>>1))/DCQuant;

    TempBlock.InitBlocks(tempBuffer);

    if (m_pSavedMV)
    {
        memset(m_pSavedMV,0,w*h*2*sizeof(Ipp16s));
    }
    err = m_pMBs->Reset();
    if (err != UMC::UMC_OK)
        return err;

#ifdef VC1_ENC_DEBUG_ON
        pDebug->SetDeblkFlag(m_pSequenceHeader->IsLoopFilter());
        pDebug->SetVTSFlag(false);
#endif

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            Ipp8u               MBPattern  = 0;
            Ipp8u               CBPCY      = 0;
            VC1EncoderMBInfo*   pCurMBInfo = 0;
            VC1EncoderMBData*   pCurMBData = 0;
            Ipp32s              xLuma      = VC1_ENC_LUMA_SIZE*j;
            Ipp32s              xChroma    = VC1_ENC_CHROMA_SIZE*j;
            NeighbouringMBsData MBs;

            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);

            pCurMBInfo  =   m_pMBs->GetCurrMBInfo();
            pCurMBData  =   m_pMBs->GetCurrMBData();

            MBs.LeftMB    = m_pMBs->GetLeftMBData();
            MBs.TopMB     = m_pMBs->GetTopMBData();
            MBs.TopLeftMB = m_pMBs->GetTopLeftMBData();

            pCompressedMB ->Init(VC1_ENC_I_MB);
            pCurMBInfo->Init(true);

            /*------------------- Compressing  ------------------------------------------------------*/

            pCurMBData->CopyMBProg(pCurMBRow[0]+ xLuma,   m_uiPlaneStep[0],
                                   pCurMBRow[1]+ xChroma, m_uiPlaneStep[1],
                                   pCurMBRow[2]+ xChroma, m_uiPlaneStep[2]);

            for (blk = 0; blk<6; blk++)
            {
                if (m_pSequenceHeader->IsOverlap())
                {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiSubC_16s_C1IRSfs(128, pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk],  blkSize, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                }
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                TransformQuantACFunction(pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk],
                                         DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
            }

            /* intra prediction Result is stored in temp block, becouse
               current block is saved for next prediction*/

STATISTICS_START_TIME(m_TStat->Intra_StartTime);
            dACPrediction = DCACPredictionIntraFrameSM( pCurMBData,&MBs,
                                                        &TempBlock, defPredictor,
                                                        direction);
STATISTICS_END_TIME(m_TStat->Intra_StartTime, m_TStat->Intra_EndTime, m_TStat->Intra_TotalTime);


            for (blk=0; blk<6; blk++)
            {
                pCompressedMB->SaveResidual(    TempBlock.m_pBlock[blk],
                                                TempBlock.m_uiBlockStep[blk],
                                                ZagTables_I[direction[blk]* dACPrediction],
                                                blk);
            }


            MBPattern = pCompressedMB->GetMBPattern();

            VC1EncoderMBInfo* t = m_pMBs->GetTopMBInfo();
            VC1EncoderMBInfo* l = m_pMBs->GetLeftMBInfo();
            VC1EncoderMBInfo* tl = m_pMBs->GetTopLeftMBInfo();

            CBPCY = Get_CBPCY(MBPattern, (t)? t->GetPattern():0, (l)? l->GetPattern():0, (tl)? tl->GetPattern():0);

            pCurMBInfo->SetMBPattern(MBPattern);

            pCompressedMB->SetACPrediction(dACPrediction);
            pCompressedMB->SetMBCBPCY(CBPCY);


#ifdef VC1_ENC_DEBUG_ON
            pDebug->SetMBType(VC1_ENC_I_MB);
            pDebug->SetCPB(MBPattern, CBPCY);
            pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
            pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
#endif
            /*--------------Reconstruction (if is needed)--------------------------------------------------*/
STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
            if (bRaiseFrame)
            {
                Ipp8u *pRFrameY = m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*(j+i*m_uiRaisedPlaneStep[0]);
                Ipp8u *pRFrameU = m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]);
                Ipp8u *pRFrameV = m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]);

                for (blk=0;blk<6; blk++)
                {
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                   InvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                               TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                               DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);
                   if (m_pSequenceHeader->IsOverlap())
                   {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                       ippiAddC_16s_C1IRSfs(128, TempBlock.m_pBlock[blk], TempBlock.m_uiBlockStep[blk],  blkSize, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                   }
                }
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippiConvert_16s8u_C1R(TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],
                                     pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                ippiConvert_16s8u_C1R(TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],
                                     pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                ippiConvert_16s8u_C1R(TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],
                                     pRFrameV,m_uiRaisedPlaneStep[2],blkSize);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

//deblocking
STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                    Ipp8u *DeblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};

                    pDeblk_I_MB[deblkPattern](DeblkPlanes, m_uiRaisedPlaneStep, m_uiQuant);

                    deblkPattern = deblkPattern | 0x1;
STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
            }

STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);
            /*------------------------------------------------------------------------------------------------*/
#ifdef VC1_ENC_DEBUG_ON
            pDebug->SetDblkHorEdgeLuma(0, 0, 15, 15);
            pDebug->SetDblkVerEdgeLuma(0, 0, 15, 15);
            pDebug->SetDblkHorEdgeU(0,15);
            pDebug->SetDblkHorEdgeV(0, 15);
            pDebug->SetDblkVerEdgeU(0, 15);
            pDebug->SetDblkVerEdgeV(0, 15);
#endif
            err = m_pMBs->NextMB();
            if (err != UMC::UMC_OK && j < w-1)
                return err;

#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
        }

        deblkPattern = (deblkPattern  | 0x2 | ( (! (Ipp8u)((i + 1 - (h -1)) >> 31)<<2 )))& deblkMask;

//Row deblocking
//STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
//        if (m_pSequenceHeader->IsLoopFilter()&& bRaiseFrame && i!=0 )
//        {
//STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
//            Ipp8u *planes[3] = {m_pRaisedPlane[0] + i*m_uiRaisedPlaneStep[0]*VC1_ENC_LUMA_SIZE,
//                                m_pRaisedPlane[1] + i*m_uiRaisedPlaneStep[1]*VC1_ENC_CHROMA_SIZE,
//                                m_pRaisedPlane[2] + i*m_uiRaisedPlaneStep[2]*VC1_ENC_CHROMA_SIZE};
//
//            if(i!=h-1)
//                Deblock_I_FrameRow(planes, m_uiRaisedPlaneStep, w, m_uiQuant);
//            else
//                Deblock_I_FrameBottomRow(planes, m_uiRaisedPlaneStep, w, m_uiQuant);
//STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
//        }
//STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

        err = m_pMBs->NextRow();
        if (err != UMC::UMC_OK)
            return err;

        pCurMBRow[0]+= m_uiPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pCurMBRow[1]+= m_uiPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pCurMBRow[2]+= m_uiPlaneStep[2]*VC1_ENC_CHROMA_SIZE;
    }



#ifdef VC1_ENC_DEBUG_ON
        if(bRaiseFrame)
            pDebug->PrintRestoredFrame(m_pRaisedPlane[0], m_uiRaisedPlaneStep[0],
                                    m_pRaisedPlane[1], m_uiRaisedPlaneStep[1],
                                    m_pRaisedPlane[2], m_uiRaisedPlaneStep[2], 0);
    //static FILE* f=0;
    //if (!f)
    //    f=fopen("1.yuv","wb");
    //for (i=0; i < h*16; i++)
    //{
    //    fwrite(m_pRaisedPlane[0]+i*m_uiRaisedPlaneStep[0],1,w*16,f);

    //}
    //for (i=0; i < h*8; i++)
    //{
    //    fwrite(m_pRaisedPlane[1]+i*m_uiRaisedPlaneStep[1],1,w*8,f);

    //}
    //for (i=0; i < h*8; i++)
    //{
    //    fwrite(m_pRaisedPlane[2]+i*m_uiRaisedPlaneStep[2],1,w*8,f);

    //}
    //fflush(f);
#endif

    return err;
}

UMC::Status  VC1EncoderPictureSM::VLC_IFrame(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status  err = UMC::UMC_OK;
    Ipp32u       i=0, j=0, blk = 0;

    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();

    eCodingSet   LumaCodingSet   = LumaCodingSetsIntra[m_uiQuantIndex>8][m_uiDecTypeAC2];
    eCodingSet   ChromaCodingSet = ChromaCodingSetsIntra[m_uiQuantIndex>8][m_uiDecTypeAC1];


    const sACTablesSet* pACTablesVLC[6] = {&ACTablesSet[LumaCodingSet],
                                           &ACTablesSet[LumaCodingSet],
                                           &ACTablesSet[LumaCodingSet],
                                           &ACTablesSet[LumaCodingSet],
                                           &ACTablesSet[ChromaCodingSet],
                                           &ACTablesSet[ChromaCodingSet]};

    const Ipp32u*        pDCTableVLC[6] = {DCTables[m_uiDecTypeDCIntra][0],
                                           DCTables[m_uiDecTypeDCIntra][0],
                                           DCTables[m_uiDecTypeDCIntra][0],
                                           DCTables[m_uiDecTypeDCIntra][0],
                                           DCTables[m_uiDecTypeDCIntra][1],
                                           DCTables[m_uiDecTypeDCIntra][1]};

    sACEscInfo   ACEscInfo = {(m_uiQuant<= 7 /*&& !VOPQuant*/)?
                               Mode3SizeConservativeVLC : Mode3SizeEfficientVLC,
                               0, 0};

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetCurrMBFirst();
#endif

    err = WriteIPictureHeader(pCodedPicture);
    if (err != UMC::UMC_OK)
        return err;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);

#ifdef VC1_ME_MB_STATICTICS
            m_MECurMbStat->MbType = UMC::ME_MbIntra;  //could be not correct
            m_MECurMbStat->whole = 0;
            memset(m_MECurMbStat->MVF, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->MVB, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->coeff, 0, 6*sizeof(Ipp16u));
            m_MECurMbStat->qp    = 2*m_uiQuant + m_bHalfQuant;

            pCompressedMB->SetMEFrStatPointer(m_MECurMbStat);
#endif

            err = pCompressedMB->WriteMBHeaderI_SM(pCodedPicture, m_bRawBitplanes);
            if (err != UMC::UMC_OK)
                return err;

            STATISTICS_START_TIME(m_TStat->AC_Coefs_StartTime);
            for (blk = 0; blk<6; blk++)
            {
                err = pCompressedMB->WriteBlockDC(pCodedPicture,pDCTableVLC[blk],m_uiQuant,blk);
                VC1_ENC_CHECK (err)

                    err = pCompressedMB->WriteBlockAC(pCodedPicture,pACTablesVLC[blk],&ACEscInfo,blk);
                VC1_ENC_CHECK (err)
            }//for
            STATISTICS_END_TIME(m_TStat->AC_Coefs_StartTime, m_TStat->AC_Coefs_EndTime, m_TStat->AC_Coefs_TotalTime);

#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
#ifdef VC1_ME_MB_STATICTICS
            m_MECurMbStat++;
#endif
        }
    }
    return err;
}

UMC::Status  VC1EncoderPictureSM::SetInterpolationParams(IppVCInterpolate_8u* pY,
                                                         IppVCInterpolate_8u* pU,
                                                         IppVCInterpolate_8u* pV,
                                                         Ipp8u* buffer,
                                                         bool bForward)
{
    UMC::Status ret = UMC::UMC_OK;
    Ipp8u **pPlane;
    Ipp32u *pStep;


    if (bForward)
    {
        pPlane = m_pForwardPlane;
        pStep  = m_uiForwardPlaneStep;
    }
    else
    {
        pPlane = m_pBackwardPlane;
        pStep  = m_uiBackwardPlaneStep;
    }

    pY->pSrc            = pPlane[0];
    pY->srcStep         = pStep[0];
    pY->pDst            = buffer;
    pY->dstStep         = VC1_ENC_LUMA_SIZE;
    pY->dx              = 0;
    pY->dy              = 0;
    pY->roundControl    = m_uiRoundControl;
    pY->roiSize.height  = VC1_ENC_LUMA_SIZE;
    pY->roiSize.width   = VC1_ENC_LUMA_SIZE;

    pU->pSrc            = pPlane[1];
    pU->srcStep         = pStep[1];
    pU->pDst            = buffer + VC1_ENC_LUMA_SIZE*VC1_ENC_LUMA_SIZE;
    pU->dstStep         = VC1_ENC_CHROMA_SIZE;
    pU->dx              = 0;
    pU->dy              = 0;
    pU->roundControl    = m_uiRoundControl;
    pU->roiSize.height  = VC1_ENC_CHROMA_SIZE;
    pU->roiSize.width   = VC1_ENC_CHROMA_SIZE;

    pV->pSrc            = pPlane[2];
    pV->srcStep         = pStep[2];
    pV->pDst            = buffer + VC1_ENC_LUMA_SIZE*VC1_ENC_LUMA_SIZE + VC1_ENC_CHROMA_SIZE*VC1_ENC_CHROMA_SIZE;
    pV->dstStep         = VC1_ENC_CHROMA_SIZE;
    pV->dx              = 0;
    pV->dy              = 0;
    pV->roundControl    = m_uiRoundControl;
    pV->roiSize.height  = VC1_ENC_CHROMA_SIZE;
    pV->roiSize.width   = VC1_ENC_CHROMA_SIZE;

    return ret;
}

//motion estimation
UMC::Status  VC1EncoderPictureSM::SetInterpolationParams4MV( IppVCInterpolate_8u* pY,
                                                              IppVCInterpolate_8u* pU,
                                                              IppVCInterpolate_8u* pV,
                                                              Ipp8u* buffer,
                                                              bool bForward)
{
    UMC::Status ret = UMC::UMC_OK;
    Ipp8u **pPlane;
    Ipp32u *pStep;


    if (bForward)
    {
        pPlane = m_pForwardPlane;
        pStep  = m_uiForwardPlaneStep;
    }
    else
    {
        pPlane = m_pBackwardPlane;
        pStep  = m_uiBackwardPlaneStep;
    }

    pY[0].pSrc            = pPlane[0];
    pY[0].srcStep         = pStep[0];
    pY[0].pDst            = buffer;
    pY[0].dstStep         = VC1_ENC_LUMA_SIZE;
    pY[0].dx              = 0;
    pY[0].dy              = 0;
    pY[0].roundControl    = m_uiRoundControl;
    pY[0].roiSize.height  = 8;
    pY[0].roiSize.width   = 8;

    pY[1].pSrc            = pPlane[0];
    pY[1].srcStep         = pStep[0];
    pY[1].pDst            = buffer+8;
    pY[1].dstStep         = VC1_ENC_LUMA_SIZE;
    pY[1].dx              = 0;
    pY[1].dy              = 0;
    pY[1].roundControl    = m_uiRoundControl;
    pY[1].roiSize.height  = 8;
    pY[1].roiSize.width   = 8;

    pY[2].pSrc            = pPlane[0];
    pY[2].srcStep         = pStep[0];
    pY[2].pDst            = buffer+8*VC1_ENC_LUMA_SIZE;
    pY[2].dstStep         = VC1_ENC_LUMA_SIZE;
    pY[2].dx              = 0;
    pY[2].dy              = 0;
    pY[2].roundControl    = m_uiRoundControl;
    pY[2].roiSize.height  = 8;
    pY[2].roiSize.width   = 8;

    pY[3].pSrc            = pPlane[0];
    pY[3].srcStep         = pStep[0];
    pY[3].pDst            = buffer+8*VC1_ENC_LUMA_SIZE+8;
    pY[3].dstStep         = VC1_ENC_LUMA_SIZE;
    pY[3].dx              = 0;
    pY[3].dy              = 0;
    pY[3].roundControl    = m_uiRoundControl;
    pY[3].roiSize.height  = 8;
    pY[3].roiSize.width   = 8;

    pU->pSrc            = pPlane[1];
    pU->srcStep         = pStep[1];
    pU->pDst            = buffer + VC1_ENC_LUMA_SIZE*VC1_ENC_LUMA_SIZE;
    pU->dstStep         = VC1_ENC_CHROMA_SIZE;
    pU->dx              = 0;
    pU->dy              = 0;
    pU->roundControl    = m_uiRoundControl;
    pU->roiSize.height  = VC1_ENC_CHROMA_SIZE;
    pU->roiSize.width   = VC1_ENC_CHROMA_SIZE;

    pV->pSrc            = pPlane[2];
    pV->srcStep         = pStep[2];
    pV->pDst            = buffer + VC1_ENC_LUMA_SIZE*VC1_ENC_LUMA_SIZE + VC1_ENC_CHROMA_SIZE*VC1_ENC_CHROMA_SIZE;
    pV->dstStep         = VC1_ENC_CHROMA_SIZE;
    pV->dx              = 0;
    pV->dy              = 0;
    pV->roundControl    = m_uiRoundControl;
    pV->roiSize.height  = VC1_ENC_CHROMA_SIZE;
    pV->roiSize.width   = VC1_ENC_CHROMA_SIZE;

    return ret;
}

UMC::Status VC1EncoderPictureSM::PAC_PFrame(UMC::MeParams* MEParams)
{
    UMC::Status  err = UMC::UMC_OK;
    Ipp32u                      i=0, j=0, blk = 0;
    bool                        bRaiseFrame = (m_pRaisedPlane[0])&&(m_pRaisedPlane[1])&&(m_pRaisedPlane[2]);

    Ipp8u*                      pCurMBRow[3] = {m_pPlane[0],        m_pPlane[1],        m_pPlane[2]};
    Ipp8u*                      pFMBRow  [3] = {m_pForwardPlane[0], m_pForwardPlane[1], m_pForwardPlane[2]};

    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();

    //forward transform quantization
    IntraTransformQuantFunction IntraTransformQuantACFunction = (m_bUniformQuant) ? IntraTransformQuantUniform :
                                                                        IntraTransformQuantNonUniform;
    InterTransformQuantFunction InterTransformQuantACFunction = (m_bUniformQuant) ? InterTransformQuantUniform :
                                                                        InterTransformQuantNonUniform;
    //inverse transform quantization
    IntraInvTransformQuantFunction IntraInvTransformQuantACFunction = (m_bUniformQuant) ? IntraInvTransformQuantUniform :
                                                                        IntraInvTransformQuantNonUniform;

    InterInvTransformQuantFunction InterInvTransformQuantACFunction = (m_bUniformQuant) ? InterInvTransformQuantUniform :
                                                                        InterInvTransformQuantNonUniform;

    CalculateChromaFunction     CalculateChroma      = (m_pSequenceHeader->IsFastUVMC())?
                                                        GetChromaMVFast:GetChromaMV;

    bool                        bIsBilinearInterpolation = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR);
    InterpolateFunction         InterpolateLumaFunction  = (bIsBilinearInterpolation)?
                                    ippiInterpolateQPBilinear_VC1_8u_C1R:
                                    ippiInterpolateQPBicubic_VC1_8u_C1R;
    InterpolateFunction         InterpolateChromaFunction  =    ippiInterpolateQPBilinear_VC1_8u_C1R;
    Ipp8u                       tempInterpolationBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];

    IppVCInterpolate_8u         sYInterpolation;
    IppVCInterpolate_8u         sUInterpolation;
    IppVCInterpolate_8u         sVInterpolation;
    VC1EncoderMBData            TempBlock;
    Ipp16s                      tempBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];

    eDirection                  direction[VC1_ENC_NUMBER_OF_BLOCKS];
    bool                        dACPrediction   = true;
    IppiSize                    blkSize     = {8,8};
    IppiSize                    blkSizeLuma = {16,16};


    Ipp8u                       doubleQuant     =  2*m_uiQuant + m_bHalfQuant;
    Ipp8u                       DCQuant         =  DCQuantValues[m_uiQuant];
    Ipp16s                      defPredictor    =  0;//defDCPredCoeff*(1024 + (DCQuant>>1))/DCQuant;
    Ipp16s*                     pSavedMV = m_pSavedMV;

    sCoordinate                 MVPredMBMin = {-60,-60};
    sCoordinate                 MVPredMBMax = {(Ipp16s)w*16*4-4, (Ipp16s)h*16*4-4};

    sCoordinate                 MVSavedMBMin = {-8,-8};
    sCoordinate                 MVSavedMBMax = {(Ipp16s)w*8, (Ipp16s)h*8};
    bool                        bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR ||
                                           m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

    bool                        bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);

    bool                        bSubBlkTS           = m_pSequenceHeader->IsVSTransform() && (!(m_bVSTransform &&m_uiTransformType==VC1_ENC_8x8_TRANSFORM));
    fGetExternalEdge            pGetExternalEdge    = GetExternalEdge_SM[m_uiMVMode==VC1_ENC_MIXED_QUARTER_BICUBIC][bSubBlkTS]; //4 MV, VTS
    fGetInternalEdge            pGetInternalEdge    = GetInternalEdge_SM[m_uiMVMode==VC1_ENC_MIXED_QUARTER_BICUBIC][bSubBlkTS]; //4 MV, VTS

    Ipp8u deblkPattern = 0;//4 bits: right 1 bit - 0 - left/1 - not left,
                           //left 2 bits: 00 - top row, 01-middle row, 11 - bottom row
                           //middle 1 bit - 1 - right/0 - not right
    Ipp8u deblkExceptoin = 0xFF; //if fist block intra equal to 0

    Ipp8u deblkMask = (m_pSequenceHeader->IsLoopFilter()) ? 0xFC : 0;
    fDeblock_P_MB*              pDeblk_P_MB = Deblk_P_MBFunction[bSubBlkTS];

    IppStatus                  ippSts  = ippStsNoErr;

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetInterpolType(bIsBilinearInterpolation);
    pDebug->SetRounControl(m_uiRoundControl);
    pDebug->SetDeblkFlag(m_pSequenceHeader->IsLoopFilter());
    pDebug->SetVTSFlag(bSubBlkTS);
#endif

    if (m_pSavedMV)
    {
        memset(m_pSavedMV,0,w*h*2*sizeof(Ipp16s));
    }

   TempBlock.InitBlocks(tempBuffer);

   err = m_pMBs->Reset();
   if (err != UMC::UMC_OK)
        return err;

    SetInterpolationParams(&sYInterpolation,&sUInterpolation,&sVInterpolation,
                           tempInterpolationBuffer,true);

    if(MEParams->pSrc->MBs[0].MbType == UMC::ME_MbIntra)
        deblkExceptoin = 0;

#ifdef VC1_ENC_CHECK_MV
    err = CheckMEMV_P(MEParams,bMVHalf);
    assert(err == UMC::UMC_OK);
#endif

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            Ipp32s              xLuma       =  VC1_ENC_LUMA_SIZE*j;
            Ipp32s              xChroma     =  VC1_ENC_CHROMA_SIZE*j;

            sCoordinate         MVInt       = {0,0};
            sCoordinate         MVQuarter   = {0,0};
            sCoordinate         MV          = {0,0};
            Ipp8u               MBPattern   = 0;
            Ipp8u               CBPCY       = 0;

            VC1EncoderMBInfo  * pCurMBInfo  = m_pMBs->GetCurrMBInfo();
            VC1EncoderMBData  * pCurMBData  = m_pMBs->GetCurrMBData();

            VC1EncoderMBInfo* left          = m_pMBs->GetLeftMBInfo();
            VC1EncoderMBInfo* topLeft       = m_pMBs->GetTopLeftMBInfo();
            VC1EncoderMBInfo* top           = m_pMBs->GetTopMBInfo();
            VC1EncoderMBInfo* topRight      = m_pMBs->GetTopRightMBInfo();

            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);

            eMBType MBType;

            switch (MEParams->pSrc->MBs[j + i*w].MbType)
            {
            case UMC::ME_MbIntra:
                {
                NeighbouringMBsData MBs;

                MBs.LeftMB    = ((left)? left->isIntra():0)         ? m_pMBs->GetLeftMBData():0;
                MBs.TopMB     = ((top)? top->isIntra():0)           ? m_pMBs->GetTopMBData():0;
                MBs.TopLeftMB = ((topLeft)? topLeft->isIntra():0)   ? m_pMBs->GetTopLeftMBData():0;

                MV.x = 0;
                MV.y =0;

                MBType = VC1_ENC_P_MB_INTRA;
                pCompressedMB->Init(VC1_ENC_P_MB_INTRA);
                pCurMBInfo->Init(true);

                /*-------------------------- Compression ----------------------------------------------------------*/

                pCurMBData->CopyMBProg(pCurMBRow[0]+xLuma,  m_uiPlaneStep[0],
                                       pCurMBRow[1]+xChroma,m_uiPlaneStep[1],
                                       pCurMBRow[2]+xChroma,m_uiPlaneStep[2]);

                //only intra blocks:
                for (blk = 0; blk<6; blk++)
                {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiSubC_16s_C1IRSfs(128, pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk], blkSize, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    IntraTransformQuantACFunction(pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk],
                                            DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                }

STATISTICS_START_TIME(m_TStat->Intra_StartTime);
                dACPrediction = DCACPredictionFrame( pCurMBData,&MBs,
                                                     &TempBlock, defPredictor,direction);
STATISTICS_END_TIME(m_TStat->Intra_StartTime, m_TStat->Intra_EndTime, m_TStat->Intra_TotalTime);

                for (blk=0; blk<6; blk++)
                {
                    pCompressedMB->SaveResidual(    TempBlock.m_pBlock[blk],
                                                    TempBlock.m_uiBlockStep[blk],
                                                    VC1_Inter_8x8_Scan,
                                                    blk);
                }

                MBPattern = pCompressedMB->GetMBPattern();
                CBPCY = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetACPrediction(dACPrediction);
                pCompressedMB->SetMBCBPCY(CBPCY);
                pCurMBInfo->SetEdgesIntra(i==0, j==0);

#ifdef VC1_ENC_DEBUG_ON
            pDebug->SetMBType(VC1_ENC_P_MB_INTRA);
            pDebug->SetCPB(MBPattern, CBPCY);
            pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
            pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
#endif

                /*-------------------------- Reconstruction ----------------------------------------------------------*/
STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
                if (bRaiseFrame)
                {
                    Ipp8u *pRFrameY = m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*(j+i*m_uiRaisedPlaneStep[0]);
                    Ipp8u *pRFrameU = m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]);
                    Ipp8u *pRFrameV = m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]);

                    for (blk=0;blk<6; blk++)
                    {
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                        IntraInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                        TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                        DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                        ippiAddC_16s_C1IRSfs(128, TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],blkSize,0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                    }

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],pRFrameV,m_uiRaisedPlaneStep[2],blkSize);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

            //deblocking
            {
                STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                Ipp8u *pDblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};

                pDeblk_P_MB[deblkPattern](pDblkPlanes, m_uiRaisedPlaneStep, m_uiQuant, pCurMBInfo, top, topLeft, left);
                deblkPattern = deblkPattern | 0x1 | ((!(Ipp8u)((j + 1 - (w -1)) >> 31)<<1));
                STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
            }
        }
STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);
                /*------------------------------------------- ----------------------------------------------------------*/
                }
            break;
            case UMC::ME_MbFrw:
            case UMC::ME_MbFrwSkipped:
                {
                sCoordinate                 MVIntChroma     = {0,0};
                sCoordinate                 MVQuarterChroma = {0,0};
                sCoordinate                 MVChroma       =  {0,0};

                sCoordinate                 *mvC=0, *mvB=0, *mvA=0;
                sCoordinate                 mv1, mv2, mv3;
                sCoordinate                 mvPred;
                sCoordinate                 mvDiff;
                eTransformType              BlockTSTypes[6] = { m_uiTransformType, m_uiTransformType,
                                                                m_uiTransformType, m_uiTransformType,
                                                                m_uiTransformType, m_uiTransformType};

                Ipp8u                       hybrid = 0;

                MBType = (UMC::ME_MbFrw == MEParams->pSrc->MBs[j + i*w].MbType)?
                                                            VC1_ENC_P_MB_1MV:VC1_ENC_P_MB_SKIP_1MV;

                MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0]->x;
                MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0]->y;

                pCurMBInfo->Init(false);
                pCompressedMB->Init(MBType);

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                /*MV prediction */
                GetMVPredictionP(true)
                PredictMV(mvA,mvB,mvC, &mvPred);
                ScalePredict(&mvPred, j*16*4,i*16*4,MVPredMBMin,MVPredMBMax);
                hybrid = HybridPrediction(&mvPred,&MV,mvA,mvC);
                pCompressedMB->SetHybrid(hybrid);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                if (VC1_ENC_P_MB_SKIP_1MV == MBType)
                {
                    //correct ME results
                    VM_ASSERT(MV.x==mvPred.x && MV.y==mvPred.y);
                    MV.x=mvPred.x;
                    MV.y=mvPred.y;
                }
                pCurMBInfo->SetMV(MV);
                GetIntQuarterMV(MV,&MVInt, &MVQuarter);

                if (VC1_ENC_P_MB_SKIP_1MV != MBType || bRaiseFrame)
                {
                    /*interpolation*/
                    CalculateChroma(MV,&MVChroma);
                    pCurMBInfo->SetMVChroma(MVChroma);
                    GetIntQuarterMV(MVChroma,&MVIntChroma,&MVQuarterChroma);

STATISTICS_START_TIME(m_TStat->Interpolate_StartTime);
                    SetInterpolationParamsSrc(&sYInterpolation,pFMBRow[0]+ xLuma, m_uiForwardPlaneStep[0],   &MVInt,       &MVQuarter);
                    SetInterpolationParamsSrc(&sUInterpolation,pFMBRow[1]+ xChroma, m_uiForwardPlaneStep[1], &MVIntChroma, &MVQuarterChroma);
                    SetInterpolationParamsSrc(&sVInterpolation,pFMBRow[2]+ xChroma, m_uiForwardPlaneStep[2], &MVIntChroma, &MVQuarterChroma);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippSts = InterpolateLumaFunction(&sYInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
                    ippSts = InterpolateChromaFunction(&sUInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
                    ippSts = InterpolateChromaFunction(&sVInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
STATISTICS_END_TIME(m_TStat->Interpolate_StartTime, m_TStat->Interpolate_EndTime, m_TStat->Interpolate_TotalTime);
                } //interpolation

                if (VC1_ENC_P_MB_SKIP_1MV != MBType)
                {

                    if (bCalculateVSTransform)
                    {
                        GetTSType (MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                    }
                    pCompressedMB->SetTSType(BlockTSTypes);


                    mvDiff.x = MV.x - mvPred.x;
                    mvDiff.y = MV.y - mvPred.y;
                    pCompressedMB->SetdMV(mvDiff);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiGetDiff16x16_8u16s_C1(pCurMBRow[0]+xLuma,m_uiPlaneStep[0],
                                    sYInterpolation.pDst,sYInterpolation.dstStep,
                                    pCurMBData->m_pBlock[0],pCurMBData->m_uiBlockStep[0],
                                    NULL, 0, 0, 0);

                    ippiGetDiff8x8_8u16s_C1(pCurMBRow[1]+xChroma,m_uiPlaneStep[1],
                                sUInterpolation.pDst,sUInterpolation.dstStep,
                                pCurMBData->m_pBlock[4],pCurMBData->m_uiBlockStep[4],
                                NULL, 0, 0, 0);

                    ippiGetDiff8x8_8u16s_C1(pCurMBRow[2]+xChroma,m_uiPlaneStep[2],
                                sVInterpolation.pDst ,sVInterpolation.dstStep,
                                pCurMBData->m_pBlock[5],pCurMBData->m_uiBlockStep[5],
                                NULL, 0, 0, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

                    for (blk = 0; blk<6; blk++)
                    {
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                        InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                    BlockTSTypes[blk], doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                        pCompressedMB->SaveResidual(pCurMBData->m_pBlock[blk],
                                                    pCurMBData->m_uiBlockStep[blk],
                                                    ZagTables[BlockTSTypes[blk]],
                                                    blk);
                    }
                    MBPattern = pCompressedMB->GetMBPattern();
                    CBPCY = MBPattern;
                    pCurMBInfo->SetMBPattern(MBPattern);
                    pCompressedMB->SetMBCBPCY(CBPCY);
                    if (MBPattern==0 && mvDiff.x == 0 && mvDiff.y == 0)
                    {
                        pCompressedMB->ChangeType(VC1_ENC_P_MB_SKIP_1MV);
                        MBType = VC1_ENC_P_MB_SKIP_1MV;
                    }
                }//VC1_ENC_P_MB_SKIP_1MV != MBType

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(MBType);
                pDebug->SetMVInfo(&MV,  mvPred.x,mvPred.y, 0);
                pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 4);
                pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 5);
                 pDebug->SetQuant(m_uiQuant,m_bHalfQuant);

                if (pCompressedMB->isSkip() )
                {
                    pDebug->SetMBAsSkip();
                }
                else
                {
                    pDebug->SetCPB(MBPattern, CBPCY);
                    pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                }
#endif
                /*---------------------------Reconstruction ------------------------------------*/
STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
                if (bRaiseFrame)
                {
                    Ipp8u *pRFrameY = m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*(j+i*m_uiRaisedPlaneStep[0]);
                    Ipp8u *pRFrameU = m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]);
                    Ipp8u *pRFrameV = m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]);

                    if (MBPattern != 0)
                    {
                        TempBlock.Reset();
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                        for (blk=0;blk<6; blk++)
                        {
                            if (MBPattern & (1<<VC_ENC_PATTERN_POS(blk)))
                            {
                                InterInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                                doubleQuant,BlockTSTypes[blk]);
                            }
                        }
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                        ippiMC16x16_8u_C1(sYInterpolation.pDst,sYInterpolation.dstStep,
                                    TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],
                                    m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*(j+i*m_uiRaisedPlaneStep[0]),m_uiRaisedPlaneStep[0],
                                    0, 0);

                        ippiMC8x8_8u_C1(sUInterpolation.pDst,sUInterpolation.dstStep,
                                TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],
                                m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]),m_uiRaisedPlaneStep[1],
                                0, 0);

                        ippiMC8x8_8u_C1(sVInterpolation.pDst,sVInterpolation.dstStep,
                                TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],
                                m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]),m_uiRaisedPlaneStep[2],
                                0, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                    } //(MBPattern != 0)
                    else
                    {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                        ippiCopy_8u_C1R(sYInterpolation.pDst, sYInterpolation.dstStep, pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                        ippiCopy_8u_C1R(sUInterpolation.pDst, sUInterpolation.dstStep, pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                        ippiCopy_8u_C1R(sVInterpolation.pDst, sVInterpolation.dstStep, pRFrameV,m_uiRaisedPlaneStep[2],blkSize);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                    }

                    //deblocking
                    if (m_pSequenceHeader->IsLoopFilter())
                    {
                        Ipp8u YFlag0 = 0,YFlag1 = 0, YFlag2 = 0, YFlag3 = 0;
                        Ipp8u UFlag0 = 0,UFlag1 = 0;
                        Ipp8u VFlag0 = 0,VFlag1 = 0;

                        pCurMBInfo->SetBlocksPattern (pCompressedMB->GetBlocksPattern());
                        pCurMBInfo->SetVSTPattern(BlockTSTypes);

                        pGetExternalEdge (top,  pCurMBInfo, false, YFlag0,UFlag0,VFlag0);
                        YFlag0 = YFlag0&deblkExceptoin;
                        UFlag0 = UFlag0&deblkExceptoin;
                        VFlag0 = VFlag0&deblkExceptoin;

                        pCurMBInfo->SetExternalEdgeHor(YFlag0,UFlag0,VFlag0);

                        pGetExternalEdge(left, pCurMBInfo, true,  YFlag0,UFlag0,VFlag0);
                        YFlag0 = YFlag0&deblkExceptoin;
                        UFlag0 = UFlag0&deblkExceptoin;
                        VFlag0 = VFlag0&deblkExceptoin;
                        pCurMBInfo->SetExternalEdgeVer(YFlag0,UFlag0,VFlag0);

                        pGetInternalEdge (pCurMBInfo, YFlag0,YFlag1);
                        YFlag0 = YFlag0&deblkExceptoin;
                        YFlag1 = YFlag1&deblkExceptoin;
                        pCurMBInfo->SetInternalEdge(YFlag0,YFlag1);

                        GetInternalBlockEdge(pCurMBInfo, YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);//ver

                        pCurMBInfo->SetInternalBlockEdge(YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);// ver

                        //deblocking
                        {
                            STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                            Ipp8u *pDblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};
                            pDeblk_P_MB[deblkPattern](pDblkPlanes, m_uiRaisedPlaneStep, m_uiQuant, pCurMBInfo, top, topLeft, left);
                            deblkPattern = deblkPattern | 0x1 | ((!(Ipp8u)((j + 1 - (w -1)) >> 31)<<1));
                            STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
                        }
                    }
                    }
STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetInterpInfo(&sYInterpolation,  &sUInterpolation, &sVInterpolation, 0);
#endif
                }
                break;
            default:
                VM_ASSERT(0);
                return UMC::UMC_ERR_FAILED;
            }

            if (m_pSavedMV)
            {
                //pull back only simple/main profiles
                ScalePredict(&MVInt, j*8,i*8,MVSavedMBMin,MVSavedMBMax);
                *(pSavedMV ++) = (MVInt.x<<2) + MVQuarter.x;
                *(pSavedMV ++) = (MVInt.y<<2) + MVQuarter.y;
            }

#ifdef VC1_ENC_DEBUG_ON
            if(!bSubBlkTS)
            {
                pDebug->SetDblkHorEdgeLuma(pCurMBInfo->GetLumaExHorEdge(), pCurMBInfo->GetLumaInHorEdge(), 15, 15);
                pDebug->SetDblkVerEdgeLuma(pCurMBInfo->GetLumaExVerEdge(), pCurMBInfo->GetLumaInVerEdge(), 15, 15);
                pDebug->SetDblkHorEdgeU(pCurMBInfo->GetUExHorEdge(),15);
                pDebug->SetDblkHorEdgeV(pCurMBInfo->GetVExHorEdge(), 15);
                pDebug->SetDblkVerEdgeU(pCurMBInfo->GetUExVerEdge(), 15);
                pDebug->SetDblkVerEdgeV(pCurMBInfo->GetVExVerEdge(), 15);
            }
            else
            {
                pDebug->SetDblkHorEdgeLuma(pCurMBInfo->GetLumaExHorEdge(), pCurMBInfo->GetLumaInHorEdge(),
                                           pCurMBInfo->GetLumaAdUppEdge(), pCurMBInfo->GetLumaAdBotEdge() );
                pDebug->SetDblkVerEdgeLuma(pCurMBInfo->GetLumaExVerEdge(), pCurMBInfo->GetLumaInVerEdge(),
                                           pCurMBInfo->GetLumaAdLefEdge(), pCurMBInfo->GetLumaAdRigEdge());
                pDebug->SetDblkHorEdgeU(pCurMBInfo->GetUExHorEdge(), pCurMBInfo->GetUAdHorEdge());
                pDebug->SetDblkHorEdgeV(pCurMBInfo->GetVExHorEdge(), pCurMBInfo->GetVAdHorEdge());
                pDebug->SetDblkVerEdgeU(pCurMBInfo->GetUExVerEdge(), pCurMBInfo->GetUAdVerEdge());
                pDebug->SetDblkVerEdgeV(pCurMBInfo->GetVExVerEdge(), pCurMBInfo->GetVAdVerEdge());
            }
#endif

            err = m_pMBs->NextMB();
            if (err != UMC::UMC_OK && j < w-1)
                return err;

#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
        }

        deblkPattern = (deblkPattern | 0x4 | ( (! (Ipp8u)((i + 1 - (h -1)) >> 31)<<3))) & deblkMask;

////Row deblocking
//STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
//        if (m_pSequenceHeader->IsLoopFilter() && bRaiseFrame && i!=0)
//        {
//STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
//          Ipp8u *DeblkPlanes[3] = {m_pRaisedPlane[0] + i*m_uiRaisedPlaneStep[0]*VC1_ENC_LUMA_SIZE,
//                                   m_pRaisedPlane[1] + i*m_uiRaisedPlaneStep[1]*VC1_ENC_CHROMA_SIZE,
//                                   m_pRaisedPlane[2] + i*m_uiRaisedPlaneStep[2]*VC1_ENC_CHROMA_SIZE};
//            m_pMBs->StartRow();
//            if(bSubBlkTS)
//            {
//                if(i < h-1)
//                    Deblock_P_RowVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//                else
//                    Deblock_P_BottomRowVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//            }
//            else
//            {
//                if(i < h-1)
//                    Deblock_P_RowNoVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//                else
//                    Deblock_P_BottomRowNoVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//            }
//STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
//        }
//STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

        err = m_pMBs->NextRow();
        if (err != UMC::UMC_OK)
            return err;

        pCurMBRow[0]+= m_uiPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pCurMBRow[1]+= m_uiPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pCurMBRow[2]+= m_uiPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

        pFMBRow[0]+= m_uiForwardPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pFMBRow[1]+= m_uiForwardPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pFMBRow[2]+= m_uiForwardPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

    }

#ifdef VC1_ENC_DEBUG_ON
    if(bRaiseFrame)
    pDebug->PrintRestoredFrame(m_pRaisedPlane[0], m_uiRaisedPlaneStep[0],
                               m_pRaisedPlane[1], m_uiRaisedPlaneStep[1],
                               m_pRaisedPlane[2], m_uiRaisedPlaneStep[2], 0);
#endif

    return err;
}

UMC::Status VC1EncoderPictureSM::VLC_PFrame(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status  err = UMC::UMC_OK;
    Ipp32u       i=0, j=0, blk = 0;

    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();

    const Ipp16u* pCBPCYTable = VLCTableCBPCY_PB[m_uiCBPTab];

    eCodingSet    LumaCodingSetIntra   = LumaCodingSetsIntra  [m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet    ChromaCodingSetIntra = ChromaCodingSetsIntra[m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet    CodingSetInter       = CodingSetsInter      [m_uiQuantIndex>8][m_uiDecTypeAC1];

    const sACTablesSet* pACTablesSetIntra[6] = {&(ACTablesSet[LumaCodingSetIntra]),
                                                &(ACTablesSet[LumaCodingSetIntra]),
                                                &(ACTablesSet[LumaCodingSetIntra]),
                                                &(ACTablesSet[LumaCodingSetIntra]),
                                                &(ACTablesSet[ChromaCodingSetIntra]),
                                                &(ACTablesSet[ChromaCodingSetIntra])};

    const sACTablesSet*  pACTablesSetInter = &(ACTablesSet[CodingSetInter]);

    const Ipp32u*  pDCTableVLCIntra[6]  ={DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][1],
                                          DCTables[m_uiDecTypeDCIntra][1]};

    sACEscInfo  ACEscInfo = {(m_uiQuant<= 7 /*&& !VOPQuant*/)?
                              Mode3SizeConservativeVLC : Mode3SizeEfficientVLC,
                              0, 0};

    const Ipp16s    (*pTTMBVLC)[4][6] =  0;
    const Ipp8u     (* pTTBlkVLC)[6] = 0;
    const Ipp8u     *pSubPattern4x4VLC=0;

    bool  bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);
    bool  bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR || m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetCurrMBFirst();
#endif

    err = WritePPictureHeader(pCodedPicture);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuant<5)
    {
        pTTMBVLC            =  TTMBVLC_HighRate;
        pTTBlkVLC           =  TTBLKVLC_HighRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_HighRate;
    }
    else if (m_uiQuant<13)
    {
        pTTMBVLC            =  TTMBVLC_MediumRate;
        pTTBlkVLC           =  TTBLKVLC_MediumRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_MediumRate;

    }
    else
    {
        pTTMBVLC            =  TTMBVLC_LowRate;
        pTTBlkVLC           =  TTBLKVLC_LowRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_LowRate;
    }

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);

#ifdef VC1_ME_MB_STATICTICS
            {
                m_MECurMbStat->whole = 0;
                memset(m_MECurMbStat->MVF, 0,   4*sizeof(Ipp16u));
                memset(m_MECurMbStat->MVB, 0,   4*sizeof(Ipp16u));
                memset(m_MECurMbStat->coeff, 0, 6*sizeof(Ipp16u));
                m_MECurMbStat->qp    = 2*m_uiQuant + m_bHalfQuant;

                pCompressedMB->SetMEFrStatPointer(m_MECurMbStat);
            }
#endif
            switch (pCompressedMB->GetMBType())
            {
            case VC1_ENC_P_MB_INTRA:
                {
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->MbType = UMC::ME_MbIntra;
                    Ipp32u MBStart = pCodedPicture->GetCurrBit();
#endif
               err = pCompressedMB->WriteMBHeaderP_INTRA    ( pCodedPicture,
                                                              m_bRawBitplanes,
                                                              MVDiffTablesVLC[m_uiMVTab],
                                                              pCBPCYTable);
               VC1_ENC_CHECK (err)

STATISTICS_START_TIME(m_TStat->AC_Coefs_StartTime);
                //Blocks coding
                for (blk = 0; blk<6; blk++)
                {
                    err = pCompressedMB->WriteBlockDC(pCodedPicture,pDCTableVLCIntra[blk],m_uiQuant,blk);
                    VC1_ENC_CHECK (err)
                    err = pCompressedMB->WriteBlockAC(pCodedPicture,pACTablesSetIntra[blk],&ACEscInfo,blk);
                    VC1_ENC_CHECK (err)
                }//for
STATISTICS_END_TIME(m_TStat->AC_Coefs_StartTime, m_TStat->AC_Coefs_EndTime, m_TStat->AC_Coefs_TotalTime);
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->whole = (Ipp16u)(pCodedPicture->GetCurrBit()- MBStart);
#endif
                break;
                }
            case VC1_ENC_P_MB_1MV:
                {
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->MbType = UMC::ME_MbFrw;
#endif
                err = pCompressedMB->WritePMB1MV(pCodedPicture,
                                                m_bRawBitplanes,
                                                m_uiMVTab,
                                                m_uiMVRangeIndex,
                                                pCBPCYTable,
                                                bCalculateVSTransform,
                                                bMVHalf,
                                                pTTMBVLC,
                                                pTTBlkVLC,
                                                pSubPattern4x4VLC,
                                                pACTablesSetInter,
                                                &ACEscInfo);



                VC1_ENC_CHECK (err)
                break;
                }
            case VC1_ENC_P_MB_SKIP_1MV:
                {
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbFrwSkipped;
#endif
                err = pCompressedMB->WritePMB_SKIP(pCodedPicture, m_bRawBitplanes);
                VC1_ENC_CHECK (err)
                }
                break;
            }
#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
#ifdef VC1_ME_MB_STATICTICS
           m_MECurMbStat++;
#endif
        }
    }
    return err;
}

UMC::Status VC1EncoderPictureSM::PAC_PFrameMixed(UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

    Ipp32u                      i=0, j=0, blk = 0;
    bool                        bRaiseFrame = (m_pRaisedPlane[0])&&(m_pRaisedPlane[1])&&(m_pRaisedPlane[2]);

    Ipp8u*                      pCurMBRow[3] = {m_pPlane[0],        m_pPlane[1],        m_pPlane[2]};
    Ipp8u*                      pFMBRow  [3] = {m_pForwardPlane[0], m_pForwardPlane[1], m_pForwardPlane[2]};

    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();

    //forward transform quantization
    IntraTransformQuantFunction IntraTransformQuantACFunction = (m_bUniformQuant) ? IntraTransformQuantUniform :
                                                                        IntraTransformQuantNonUniform;
    InterTransformQuantFunction InterTransformQuantACFunction = (m_bUniformQuant) ? InterTransformQuantUniform :
                                                                        InterTransformQuantNonUniform;
    //inverse transform quantization
    IntraInvTransformQuantFunction IntraInvTransformQuantACFunction = (m_bUniformQuant) ? IntraInvTransformQuantUniform :
                                                                        IntraInvTransformQuantNonUniform;

    InterInvTransformQuantFunction InterInvTransformQuantACFunction = (m_bUniformQuant) ? InterInvTransformQuantUniform :
                                                                        InterInvTransformQuantNonUniform;

    CalculateChromaFunction     CalculateChroma      = (m_pSequenceHeader->IsFastUVMC())?
                                                        GetChromaMVFast:GetChromaMV;

    bool                        bIsBilinearInterpolation = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR);
   InterpolateFunction         InterpolateLumaFunction  = (bIsBilinearInterpolation)?
                                    ippiInterpolateQPBilinear_VC1_8u_C1R:
                                    ippiInterpolateQPBicubic_VC1_8u_C1R;
    InterpolateFunction         InterpolateChromaFunction  =    ippiInterpolateQPBilinear_VC1_8u_C1R;

    Ipp8u                       tempInterpolationBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];

    IppVCInterpolate_8u         sYInterpolation = {0};
    IppVCInterpolate_8u         sUInterpolation = {0};
    IppVCInterpolate_8u         sVInterpolation = {0};
    IppVCInterpolate_8u         sYInterpolationBlk[4]={0};

    VC1EncoderMBData            TempBlock;
    Ipp16s                      tempBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];

    eDirection                  direction[VC1_ENC_NUMBER_OF_BLOCKS];
    Ipp8s                       dACPrediction   = 1;

    IppiSize                    blkSize     = {8,8};
    IppiSize                    blkSizeLuma = {16,16};

    Ipp8u                       doubleQuant     =  2*m_uiQuant + m_bHalfQuant;
    Ipp8u                       DCQuant         =  DCQuantValues[m_uiQuant];
    Ipp16s                      defPredictor    =  0;
    Ipp16s*                     pSavedMV = m_pSavedMV;

    sCoordinate                 MVPredMBMin = {-60,-60};
    sCoordinate                 MVPredMBMax = {(Ipp16s)w*16*4-4, (Ipp16s)h*16*4-4};

    sCoordinate                 MVPredMBMinB = {-28,-28};
    sCoordinate                 MVPredMBMaxB = {(Ipp16s)w*16*4-4, (Ipp16s)h*16*4-4};

    sCoordinate                 MVSavedMBMin = {-8,-8};
    sCoordinate                 MVSavedMBMax = {(Ipp16s)w*8, (Ipp16s)h*8};

    bool                        bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR ||
                                           m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

    bool                        bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);

    bool                        bSubBlkTS           = m_pSequenceHeader->IsVSTransform() && (!(m_bVSTransform &&m_uiTransformType==VC1_ENC_8x8_TRANSFORM));
    fGetExternalEdge            pGetExternalEdge    = GetExternalEdge_SM[m_uiMVMode==VC1_ENC_MIXED_QUARTER_BICUBIC][bSubBlkTS]; //4 MV, VTS
    fGetInternalEdge            pGetInternalEdge    = GetInternalEdge_SM[m_uiMVMode==VC1_ENC_MIXED_QUARTER_BICUBIC][bSubBlkTS]; //4 MV, VTS

    Ipp8u deblkPattern = 0;//4 bits: right 1 bit - 0 - left/1 - not left,
                           //left 2 bits: 00 - top row, 01-middle row, 11 - bottom row
                           //middle 1 bit - 1 - right/0 - not right
    Ipp8u deblkExceptoin = 0xFF; //if fist block intra equal to 0

    Ipp8u deblkMask = (m_pSequenceHeader->IsLoopFilter()) ? 0xFC : 0;
    fDeblock_P_MB*              pDeblk_P_MB = Deblk_P_MBFunction[bSubBlkTS];

    IppStatus                  ippSts  = ippStsNoErr;

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetInterpolType(bIsBilinearInterpolation);
    pDebug->SetRounControl(m_uiRoundControl);
    pDebug->SetDeblkFlag(m_pSequenceHeader->IsLoopFilter());
    pDebug->SetVTSFlag(bSubBlkTS);
#endif

    if (m_pSavedMV)
    {
        memset(m_pSavedMV,0,w*h*2*sizeof(Ipp16s));
    }

   TempBlock.InitBlocks(tempBuffer);

   err = m_pMBs->Reset();
   if (err != UMC::UMC_OK)
        return err;

    SetInterpolationParams(&sYInterpolation,&sUInterpolation,&sVInterpolation,
                           tempInterpolationBuffer,true);
    SetInterpolationParams4MV(sYInterpolationBlk,&sUInterpolation,&sVInterpolation,
                           tempInterpolationBuffer,true);

    if(MEParams->pSrc->MBs[0].MbType == UMC::ME_MbIntra ||
        (MEParams->pSrc->MBs[0].MbType == UMC::ME_MbMixed
            && MEParams->pSrc->MBs[0].BlockType[0] == UMC::ME_MbIntra))
        deblkExceptoin = 0;

#ifdef VC1_ENC_CHECK_MV
    err = CheckMEMV_P_MIXED(MEParams, bMVHalf);
    assert(err == UMC::UMC_OK);
#endif

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            //bool                bIntra      = false;
            Ipp32s              xLuma        =  VC1_ENC_LUMA_SIZE*j;
            Ipp32s              xChroma      =  VC1_ENC_CHROMA_SIZE*j;

            //Ipp32s              posY        =  VC1_ENC_LUMA_SIZE*i;
            Ipp8u               MBPattern   = 0;
            Ipp8u               CBPCY       = 0;

            VC1EncoderMBInfo  * pCurMBInfo  = m_pMBs->GetCurrMBInfo();
            VC1EncoderMBData  * pCurMBData  = m_pMBs->GetCurrMBData();

            VC1EncoderMBInfo* left          = m_pMBs->GetLeftMBInfo();
            VC1EncoderMBInfo* topLeft       = m_pMBs->GetTopLeftMBInfo();
            VC1EncoderMBInfo* top           = m_pMBs->GetTopMBInfo();
            VC1EncoderMBInfo* topRight      = m_pMBs->GetTopRightMBInfo();
            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);
            Ipp8u          intraPattern       = 0; //from UMC_ME: position 1<<VC_ENC_PATTERN_POS(blk) (if not skiped)

            Ipp8u *pRFrameY = 0;
            Ipp8u *pRFrameU = 0;
            Ipp8u *pRFrameV = 0;

            eTransformType  BlockTSTypes[6] = { m_uiTransformType, m_uiTransformType,
                                                m_uiTransformType, m_uiTransformType,
                                                m_uiTransformType, m_uiTransformType};



            eMBType MBType;

            switch (MEParams->pSrc->MBs[j + i*w].MbType)
            {
                case UMC::ME_MbIntra:
                    MBType = VC1_ENC_P_MB_INTRA;
                    break;
                case UMC::ME_MbFrw:
                    MBType = (MEParams->pSrc->MBs[j + i*w].MbPart == UMC::ME_Mb8x8)? VC1_ENC_P_MB_4MV:VC1_ENC_P_MB_1MV;
                    break;
                case UMC::ME_MbMixed:
                    MBType = VC1_ENC_P_MB_4MV;
                    intraPattern = (Ipp8u)(((UMC::ME_MbIntra==MEParams->pSrc->MBs[j + i*w].BlockType[0])<<VC_ENC_PATTERN_POS(0)|
                                    (UMC::ME_MbIntra==MEParams->pSrc->MBs[j + i*w].BlockType[1])<<VC_ENC_PATTERN_POS(1)|
                                    (UMC::ME_MbIntra==MEParams->pSrc->MBs[j + i*w].BlockType[2])<<VC_ENC_PATTERN_POS(2)|
                                    (UMC::ME_MbIntra==MEParams->pSrc->MBs[j + i*w].BlockType[3])<<VC_ENC_PATTERN_POS(3)));
                    break;
                case UMC::ME_MbFrwSkipped:
                    MBType = (MEParams->pSrc->MBs[j + i*w].MbPart == UMC::ME_Mb8x8)? VC1_ENC_P_MB_SKIP_4MV:VC1_ENC_P_MB_SKIP_1MV;
                    break;
                default:
                    assert(0);
                    return UMC::UMC_ERR_FAILED;
            }

            if (bRaiseFrame)
            {
               pRFrameY = m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*  (j+i*m_uiRaisedPlaneStep[0]);
               pRFrameU = m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]);
               pRFrameV = m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]);
            }
            pCompressedMB->Init(MBType);

            switch  (MBType)
            {
            case VC1_ENC_P_MB_INTRA:
            {
                NeighbouringMBsData MBs;
                NeighbouringMBsIntraPattern     MBsIntraPattern;

                MBs.LeftMB    = m_pMBs->GetLeftMBData();
                MBs.TopMB     = m_pMBs->GetTopMBData();
                MBs.TopLeftMB = m_pMBs->GetTopLeftMBData();

                MBsIntraPattern.LeftMB      = (left)?       left->GetIntraPattern():0;
                MBsIntraPattern.TopMB       = (top)?        top->GetIntraPattern():0;
                MBsIntraPattern.TopLeftMB   = (topLeft)?    topLeft->GetIntraPattern():0;



                pCurMBInfo->Init(true);
                pCurMBData->CopyMBProg(pCurMBRow[0]+xLuma,  m_uiPlaneStep[0],
                                       pCurMBRow[1]+xChroma,m_uiPlaneStep[1],
                                       pCurMBRow[2]+xChroma,m_uiPlaneStep[2]);

                //only intra blocks:
                for (blk = 0; blk<6; blk++)
                {

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiSubC_16s_C1IRSfs(128, pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk], blkSize, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                        IntraTransformQuantACFunction(pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk],
                                            DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                }

STATISTICS_START_TIME(m_TStat->Intra_StartTime);
                dACPrediction = DCACPredictionFrame4MVIntraMB( pCurMBData,&MBs,&MBsIntraPattern,&TempBlock, defPredictor,direction);
STATISTICS_END_TIME(m_TStat->Intra_StartTime, m_TStat->Intra_EndTime, m_TStat->Intra_TotalTime);


                for (blk=0; blk<6; blk++)
                {
                    pCompressedMB->SaveResidual(    TempBlock.m_pBlock[blk],
                                                    TempBlock.m_uiBlockStep[blk],
                                                    VC1_Inter_8x8_Scan,
                                                    blk);
                }

                MBPattern = pCompressedMB->GetMBPattern();
                CBPCY = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetACPrediction(dACPrediction);
                pCompressedMB->SetMBCBPCY(CBPCY);
                pCurMBInfo->SetEdgesIntra(i==0, j==0);

STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
                if (bRaiseFrame)
                {
                    for (blk=0;blk<6; blk++)
                    {
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                        IntraInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                        TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                        DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                        ippiAddC_16s_C1IRSfs(128, TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],blkSize,0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
                    }
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                    ippiConvert_16s8u_C1R(TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],pRFrameV,m_uiRaisedPlaneStep[2],blkSize);

                    //deblocking
                    {
                        STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                        Ipp8u *pDblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};

                        pDeblk_P_MB[deblkPattern](pDblkPlanes, m_uiRaisedPlaneStep, m_uiQuant, pCurMBInfo, top, topLeft, left);
                        deblkPattern = deblkPattern | 0x1 | ((!(Ipp8u)((j + 1 - (w -1)) >> 31)<<1));
                        STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
                    }

                }
STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

                if (m_pSavedMV)
                {
                    *(pSavedMV ++) = 0;
                    *(pSavedMV ++) = 0;
                }
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(VC1_ENC_P_MB_INTRA);
                pDebug->SetCPB(MBPattern, CBPCY);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
#endif
            }
            break;
            case VC1_ENC_P_MB_1MV:
            case VC1_ENC_P_MB_SKIP_1MV:
            {

                sCoordinate    MVIntChroma     = {0,0};
                sCoordinate    MVQuarterChroma = {0,0};
                sCoordinate    MVChroma        = {0,0};
                sCoordinate    MVInt           = {0,0};
                sCoordinate    MVQuarter       = {0,0};
                sCoordinate    MV              = { MEParams->pSrc->MBs[j + i*w].MV[0]->x,MEParams->pSrc->MBs[j + i*w].MV[0]->y};

                sCoordinate    *mvC=0, *mvB=0, *mvA=0;
                sCoordinate     mv1,    mv2,    mv3;
                sCoordinate     mvPred;
                sCoordinate     mvDiff;
                Ipp8u           hybrid = 0;

                pCurMBInfo->Init(false);

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                /*MV prediction */
                GetMVPredictionP(true)
                PredictMV(mvA,mvB,mvC, &mvPred);
                ScalePredict(&mvPred, j*16*4,i*16*4,MVPredMBMin,MVPredMBMax);
                hybrid = HybridPrediction(&mvPred,&MV,mvA,mvC);
                pCompressedMB->SetHybrid(hybrid);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                if (VC1_ENC_P_MB_SKIP_1MV == MBType)
                {
                    //correct ME results
                    VM_ASSERT(MV.x==mvPred.x && MV.y==mvPred.y);
                    MV.x=mvPred.x;
                    MV.y=mvPred.y;
                }

                pCurMBInfo->SetMV(MV);
                GetIntQuarterMV(MV,&MVInt, &MVQuarter);


                if (MBType != VC1_ENC_P_MB_SKIP_1MV || bRaiseFrame)
                {
                    CalculateChroma(MV,&MVChroma);
                    pCurMBInfo->SetMVChroma(MVChroma);
                    GetIntQuarterMV(MVChroma,&MVIntChroma,&MVQuarterChroma);

STATISTICS_START_TIME(m_TStat->Interpolate_StartTime);
                    SetInterpolationParamsSrc(&sYInterpolation,pFMBRow[0]+ xLuma, m_uiForwardPlaneStep[0],   &MVInt,       &MVQuarter);
                    SetInterpolationParamsSrc(&sUInterpolation,pFMBRow[1]+ xChroma, m_uiForwardPlaneStep[1], &MVIntChroma, &MVQuarterChroma);
                    SetInterpolationParamsSrc(&sVInterpolation,pFMBRow[2]+ xChroma, m_uiForwardPlaneStep[2], &MVIntChroma, &MVQuarterChroma);
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippSts = InterpolateLumaFunction(&sYInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
                    ippSts = InterpolateChromaFunction(&sUInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
                    ippSts = InterpolateChromaFunction(&sVInterpolation);
                    if (ippSts != ippStsNoErr)
                        return UMC::UMC_ERR_OPEN_FAILED;
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
STATISTICS_END_TIME(m_TStat->Interpolate_StartTime, m_TStat->Interpolate_EndTime, m_TStat->Interpolate_TotalTime);
                } //interpolation

                if (VC1_ENC_P_MB_SKIP_1MV != MBType)
                {
                    if (bCalculateVSTransform)
                    {
                        GetTSType (MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                    }
                    pCompressedMB->SetTSType(BlockTSTypes);

                    mvDiff.x = MV.x - mvPred.x;
                    mvDiff.y = MV.y - mvPred.y;
                    pCompressedMB->SetdMV(mvDiff);



LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiGetDiff16x16_8u16s_C1(pCurMBRow[0]+xLuma,m_uiPlaneStep[0],
                                    sYInterpolation.pDst,sYInterpolation.dstStep,
                                    pCurMBData->m_pBlock[0],pCurMBData->m_uiBlockStep[0],
                                    NULL, 0, 0, 0);

                    ippiGetDiff8x8_8u16s_C1(pCurMBRow[1]+xChroma,m_uiPlaneStep[1],
                                sUInterpolation.pDst,sUInterpolation.dstStep,
                                pCurMBData->m_pBlock[4],pCurMBData->m_uiBlockStep[4],
                                NULL, 0, 0, 0);

                    ippiGetDiff8x8_8u16s_C1(pCurMBRow[2]+xChroma,m_uiPlaneStep[2],
                                sVInterpolation.pDst ,sVInterpolation.dstStep,
                                pCurMBData->m_pBlock[5],pCurMBData->m_uiBlockStep[5],
                                NULL, 0, 0, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

                    for (blk = 0; blk<6; blk++)
                    {
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                        InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                    BlockTSTypes[blk], doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                        pCompressedMB->SaveResidual(pCurMBData->m_pBlock[blk],
                                                    pCurMBData->m_uiBlockStep[blk],
                                                    ZagTables[BlockTSTypes[blk]],
                                                    blk);
                    }

                    MBPattern = pCompressedMB->GetMBPattern();
                    CBPCY = MBPattern;
                    pCurMBInfo->SetMBPattern(MBPattern);
                    pCompressedMB->SetMBCBPCY(CBPCY);
                    if (MBPattern==0 && mvDiff.x == 0 && mvDiff.y == 0)
                    {
                        pCompressedMB->ChangeType(VC1_ENC_P_MB_SKIP_1MV);
                        MBType = VC1_ENC_P_MB_SKIP_1MV;
                    }
                }// not skipped



STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
                if (bRaiseFrame)
                {
                    if (MBPattern!=0)
                    {
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                        for (blk = 0; blk <6 ; blk++)
                        {
                            InterInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                            TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                            doubleQuant,BlockTSTypes[blk]);
                        }
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);

                        ippiMC16x16_8u_C1(sYInterpolation.pDst,sYInterpolation.dstStep,
                                    TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],
                                    m_pRaisedPlane[0]+VC1_ENC_LUMA_SIZE*(j+i*m_uiRaisedPlaneStep[0]),m_uiRaisedPlaneStep[0],
                                    0, 0);

                        ippiMC8x8_8u_C1(sUInterpolation.pDst,sUInterpolation.dstStep,
                                TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],
                                m_pRaisedPlane[1]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[1]),m_uiRaisedPlaneStep[1],
                                0, 0);

                        ippiMC8x8_8u_C1(sVInterpolation.pDst,sVInterpolation.dstStep,
                                TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],
                                m_pRaisedPlane[2]+VC1_ENC_CHROMA_SIZE*(j+i*m_uiRaisedPlaneStep[2]),m_uiRaisedPlaneStep[2],
                                0, 0);

                    }
                    else
                    {
                        ippiCopy_8u_C1R(sYInterpolation.pDst, sYInterpolation.dstStep, pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                        ippiCopy_8u_C1R(sUInterpolation.pDst, sUInterpolation.dstStep, pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                        ippiCopy_8u_C1R(sVInterpolation.pDst, sVInterpolation.dstStep, pRFrameV,m_uiRaisedPlaneStep[2],blkSize);
                    }

                    if (m_pSequenceHeader->IsLoopFilter())
                    {
                        Ipp8u YFlag0 = 0,YFlag1 = 0, YFlag2 = 0, YFlag3 = 0;
                        Ipp8u UFlag0 = 0,UFlag1 = 0;
                        Ipp8u VFlag0 = 0,VFlag1 = 0;

                        pCurMBInfo->SetBlocksPattern (pCompressedMB->GetBlocksPattern());
                        pCurMBInfo->SetVSTPattern(BlockTSTypes);

                        pGetExternalEdge (top,  pCurMBInfo, false, YFlag0,UFlag0,VFlag0);
                        //exception #1
                        YFlag0 = YFlag0 & deblkExceptoin;
                        UFlag0 = UFlag0 & deblkExceptoin;
                        VFlag0 = VFlag0 & deblkExceptoin;

                        pCurMBInfo->SetExternalEdgeHor(YFlag0,UFlag0,VFlag0);

                        pGetExternalEdge(left, pCurMBInfo, true,  YFlag0,UFlag0,VFlag0);
                        //exception #1
                        YFlag0 = YFlag0 & deblkExceptoin;
                        UFlag0 = UFlag0 & deblkExceptoin;
                        VFlag0 = VFlag0 & deblkExceptoin;

                        pCurMBInfo->SetExternalEdgeVer(YFlag0,UFlag0,VFlag0);

                        pGetInternalEdge (pCurMBInfo, YFlag0,YFlag1);
                        //exception #1
                        YFlag0 = YFlag0 & deblkExceptoin;
                        YFlag1 = YFlag1 & deblkExceptoin;

                        pCurMBInfo->SetInternalEdge(YFlag0,YFlag1);

                        GetInternalBlockEdge(pCurMBInfo, YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);//ver
                        //exception #1
                        YFlag0 = YFlag0 & deblkExceptoin;
                        YFlag1 = YFlag1 & deblkExceptoin;

                        pCurMBInfo->SetInternalBlockEdge(YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);// ver

                        //deblocking
                        {
                            STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                            Ipp8u *pDblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};

                            pDeblk_P_MB[deblkPattern](pDblkPlanes, m_uiRaisedPlaneStep, m_uiQuant, pCurMBInfo, top, topLeft, left);
                            deblkPattern = deblkPattern | 0x1 | ((!(Ipp8u)((j + 1 - (w -1)) >> 31)<<1));
                            STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
                        }

                    }
                }
STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

                if (m_pSavedMV)
                {
                    //pull back only simple/main profiles
                    ScalePredict(&MVInt, j*8,i*8,MVSavedMBMin,MVSavedMBMax);
                    *(pSavedMV ++) = (MVInt.x<<2) + MVQuarter.x;
                    *(pSavedMV ++) = (MVInt.y<<2) + MVQuarter.y;
                 }
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(MBType);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetMVInfo(&MV, mvPred.x, mvPred.y, 0);
                pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 4);
                pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 5);
                if (MBType != VC1_ENC_P_MB_SKIP_1MV)
                {
                    pDebug->SetCPB(MBPattern, CBPCY);
                    pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                }
                else
                {
                    pDebug->SetMBAsSkip();
                }

                pDebug->SetInterpInfo(&sYInterpolation,  &sUInterpolation, &sVInterpolation, 0);
#endif

            }
            break;

            case VC1_ENC_P_MB_4MV:
            case VC1_ENC_P_MB_SKIP_4MV:
            {

                sCoordinate    MVInt[4]           = {0};
                sCoordinate    MVQuarter[4]       = {0};
                sCoordinate    MV[4]              = {{MEParams->pSrc->MBs[j + i*w].MV[0][0].x,MEParams->pSrc->MBs[j + i*w].MV[0][0].y},
                                                     {MEParams->pSrc->MBs[j + i*w].MV[0][1].x,MEParams->pSrc->MBs[j + i*w].MV[0][1].y},
                                                     {MEParams->pSrc->MBs[j + i*w].MV[0][2].x,MEParams->pSrc->MBs[j + i*w].MV[0][2].y},
                                                     {MEParams->pSrc->MBs[j + i*w].MV[0][3].x,MEParams->pSrc->MBs[j + i*w].MV[0][3].y}};
                //sCoordinate    MVPred[4]       = {0};
                sCoordinate    MVLuma          = {0,0};
                sCoordinate    MVChroma        = {0,0};
                sCoordinate    MVIntChroma     = {0,0};
                sCoordinate    MVQuarterChroma = {0,0};

                sCoordinate    *mvC=0, *mvB=0, *mvA=0;
                sCoordinate     mv1,    mv2,    mv3;
                sCoordinate     mvPred[4]={0};
                sCoordinate     mvDiff;
                Ipp8u           hybrid = 0;
                //bool            bInterpolation  = false;
                bool            bNullMV         = true;
                NeighbouringMBsData             MBs;
                NeighbouringMBsIntraPattern     MBsIntraPattern;

                MBs.LeftMB    = m_pMBs->GetLeftMBData();
                MBs.TopMB     = m_pMBs->GetTopMBData();
                MBs.TopLeftMB = m_pMBs->GetTopLeftMBData();

                MBsIntraPattern.LeftMB      = (left)?       left->GetIntraPattern():0;
                MBsIntraPattern.TopMB       = (top)?        top->GetIntraPattern():0;
                MBsIntraPattern.TopLeftMB   = (topLeft)?    topLeft->GetIntraPattern():0;

                pCurMBInfo->Init(false);

#ifdef VC1_ENC_DEBUG_ON
                if (VC1_ENC_P_MB_SKIP_4MV == MBType  )
                {
                    pDebug->SetMBType(VC1_ENC_P_MB_SKIP_4MV);
                    pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                    pDebug->SetMBAsSkip();
                }
                else
                {
                    pDebug->SetMBType(VC1_ENC_P_MB_4MV);
                    pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                }
#endif
                // --- MV Prediction ---- //
                if (VC1_ENC_P_MB_SKIP_4MV != MBType )
                {
                    if (bCalculateVSTransform)
                    {
                        GetTSType (MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                    }
                    pCompressedMB->SetTSType(BlockTSTypes);

                    if (0 != intraPattern )
                        memset (tempInterpolationBuffer,128,VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS*sizeof(Ipp8u));

                    for (blk = 0; blk<4; blk++)
                    {
                        if ((intraPattern&(1<<VC_ENC_PATTERN_POS(blk))) == 0)
                        {
                            Ipp32u blkPosX = (blk&0x1)<<3;
                            Ipp32u blkPosY = (blk/2)<<3;

                            pCurMBInfo->SetMV(MV[blk],blk, true);

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                            GetMVPredictionPBlk(blk);
                            PredictMV(mvA,mvB,mvC, &mvPred[blk]);

                            ScalePredict(&mvPred[blk], ((j<<4) + blkPosX)<<2,((i<<4) + blkPosY)<<2,MVPredMBMinB,MVPredMBMaxB);
                            hybrid = HybridPrediction(&mvPred[blk],&MV[blk],mvA,mvC);
                            pCompressedMB->SetHybrid(hybrid,blk);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                            mvDiff.x = MV[blk].x - mvPred[blk].x;
                            mvDiff.y = MV[blk].y - mvPred[blk].y;

                            bNullMV = ((mvDiff.x!=0)||(mvDiff.y!=0))? false: bNullMV;
                            pCompressedMB->SetBlockdMV(mvDiff,blk);
#ifdef VC1_ENC_DEBUG_ON
                            pDebug->SetMVInfo(&MV[blk], mvPred[blk].x,mvPred[blk].y, 0, blk);
                            pDebug->SetMVDiff(mvDiff.x, mvDiff.y, 0, blk);
#endif
                        }
                        else
                        {
                            pCurMBInfo->SetIntraBlock(blk);
                            pCompressedMB->SetIntraBlock(blk);

    #ifdef VC1_ENC_DEBUG_ON
                            pDebug->SetBlockAsIntra(blk);
    #endif
                        }
                    } // for
                    if (!pCurMBInfo->GetLumaMV(&MVLuma))
                    {   //chroma intra type
                        intraPattern = intraPattern | (1<<VC_ENC_PATTERN_POS(4))|(1<<VC_ENC_PATTERN_POS(5));
                        pCurMBInfo->SetIntraBlock(4);
                        pCurMBInfo->SetIntraBlock(5);
                        pCompressedMB->SetIntraBlock(4);
                        pCompressedMB->SetIntraBlock(5);
#ifdef VC1_ENC_DEBUG_ON
                        pDebug->SetBlockAsIntra(4);
                        pDebug->SetBlockAsIntra(5);
#endif
                    }
                }
                else  // prediction for skip MB
                {
                    for (blk = 0; blk<4; blk++)
                    {
                         Ipp32u blkPosX = (blk&0x1)<<3;
                         Ipp32u blkPosY = (blk/2)<<3;

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                         GetMVPredictionPBlk(blk);
                         PredictMV(mvA,mvB,mvC, &mvPred[blk]);
                         ScalePredict(&mvPred[blk], ((j<<4) + blkPosX)<<2,((i<<4) + blkPosY)<<2,MVPredMBMinB,MVPredMBMaxB);
                         hybrid = HybridPrediction(&mvPred[blk],&MV[blk],mvA,mvC);
                         pCompressedMB->SetHybrid(hybrid,blk);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                        //check MV from ME
                        assert( mvPred[blk].x == MV[blk].x && mvPred[blk].y == MV[blk].y);
                        MV[blk].x = mvPred[blk].x;
                        MV[blk].y = mvPred[blk].y;
                        pCurMBInfo->SetMV(MV[blk],blk, true);
                        GetIntQuarterMV(MV[blk],&MVInt[blk], &MVQuarter[blk]);
#ifdef VC1_ENC_DEBUG_ON
                            pDebug->SetMVInfo(&MV[blk], mvPred[blk].x,mvPred[blk].y, 0, blk);
#endif

                    }
                    pCurMBInfo->GetLumaMV(&MVLuma);
                    intraPattern = 0;
                }

                // --- Interpolation --- //
                if (VC1_ENC_P_MB_SKIP_4MV != MBType   || bRaiseFrame)
                {
                   for (blk = 0; blk<4; blk++)
                   {
                        Ipp32u blkPosX = (blk&0x1)<<3;
                        Ipp32u blkPosY = (blk/2)<<3;


                       // Luma blocks
                       if ((intraPattern&(1<<VC_ENC_PATTERN_POS(blk))) == 0)
                       {
                            GetIntQuarterMV(MV[blk],&MVInt[blk], &MVQuarter[blk]);
                            SetInterpolationParamsSrc(&sYInterpolationBlk[blk],pFMBRow[0]+ xLuma + blkPosX + blkPosY*m_uiForwardPlaneStep[0], m_uiForwardPlaneStep[0],   &MVInt[blk], &MVQuarter[blk]);
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                            ippSts = InterpolateLumaFunction(&sYInterpolationBlk[blk]);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

#ifdef VC1_ENC_DEBUG_ON
                            //interpolation
                            pDebug->SetInterpInfo(sYInterpolationBlk[blk].pDst, sYInterpolationBlk[blk].dstStep, blk, 0,
                                                  sYInterpolationBlk[blk].pSrc, sYInterpolationBlk[blk].srcStep);
#endif
                       }//if
                   }//for blk

                   if ((intraPattern&(1<<VC_ENC_PATTERN_POS(4))) == 0)
                   {
                       CalculateChroma(MVLuma,&MVChroma);
                       pCurMBInfo->SetMVChroma(MVChroma);
                       GetIntQuarterMV(MVChroma,&MVIntChroma,&MVQuarterChroma);
                       SetInterpolationParamsSrc(&sUInterpolation,pFMBRow[1]+ xChroma, m_uiForwardPlaneStep[1], &MVIntChroma, &MVQuarterChroma);
                       SetInterpolationParamsSrc(&sVInterpolation,pFMBRow[2]+ xChroma, m_uiForwardPlaneStep[2], &MVIntChroma, &MVQuarterChroma);
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                       ippSts = InterpolateChromaFunction(&sUInterpolation);
                       VC1_ENC_IPP_CHECK(ippSts)
                       ippSts = InterpolateChromaFunction(&sVInterpolation);
                       VC1_ENC_IPP_CHECK(ippSts)
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

#ifdef VC1_ENC_DEBUG_ON
                       pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 4);
                       pDebug->SetMVInfo(&MVChroma,  0, 0, 0, 5);
                       pDebug->SetInterpInfo(sUInterpolation.pDst, sUInterpolation.dstStep, 4, 0,
                            sUInterpolation.pSrc, sUInterpolation.srcStep);
                       pDebug->SetInterpInfo(sVInterpolation.pDst, sVInterpolation.dstStep, 5, 0,
                            sVInterpolation.pSrc, sVInterpolation.srcStep);
#endif
                   } // chroma
                }//if interpolation
                //Chroma blocks

                // --- Compressing --- //
                if (VC1_ENC_P_MB_SKIP_4MV != MBType)
                {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippSts = ippiGetDiff16x16_8u16s_C1( pCurMBRow[0]+xLuma,m_uiPlaneStep[0],
                                                        sYInterpolation.pDst,sYInterpolation.dstStep,
                                                        pCurMBData->m_pBlock[0],pCurMBData->m_uiBlockStep[0],
                                                        0,0,0,0);

                    VC1_ENC_IPP_CHECK(ippSts)
                    ippSts = ippiGetDiff8x8_8u16s_C1 ( pCurMBRow[1]+xChroma,m_uiPlaneStep[1],
                                                        sUInterpolation.pDst,sUInterpolation.dstStep,
                                                        pCurMBData->m_pBlock[4],pCurMBData->m_uiBlockStep[4],
                                                        0,0,0,0);
                    VC1_ENC_IPP_CHECK(ippSts)
                    ippSts = ippiGetDiff8x8_8u16s_C1 ( pCurMBRow[2]+xChroma,m_uiPlaneStep[2],
                                                        sVInterpolation.pDst,sVInterpolation.dstStep,
                                                        pCurMBData->m_pBlock[5],pCurMBData->m_uiBlockStep[5],
                                                        0,0,0,0);
                    VC1_ENC_IPP_CHECK(ippSts)
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    for (blk = 0; blk<6; blk++)
                    {
                        if (!pCurMBInfo->isIntra(blk))
                        {
                            InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                            BlockTSTypes[blk], doubleQuant);
                        }
                        else
                        {
                            IntraTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                            DCQuant, doubleQuant);
                        }
                    }
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);


                    dACPrediction = DCACPredictionFrame4MVBlockMixed   (pCurMBData,
                                                                            pCurMBInfo->GetIntraPattern(),
                                                                            &MBs,
                                                                            &MBsIntraPattern,
                                                                            &TempBlock,
                                                                            defPredictor,
                                                                            direction);

                    for (blk = 0; blk<6; blk++)
                    {
                            pCompressedMB->SaveResidual(TempBlock.m_pBlock[blk],
                                                        TempBlock.m_uiBlockStep[blk],
                                                        ZagTables[BlockTSTypes[blk]],
                                                        blk);
                    }
                    MBPattern = pCompressedMB->GetMBPattern();
                    CBPCY = MBPattern;
                    pCurMBInfo->SetMBPattern(MBPattern);
                    pCompressedMB->SetMBCBPCY(CBPCY);
                    pCompressedMB->SetACPrediction(dACPrediction);
                    if (MBPattern==0 && intraPattern==0 && bNullMV)
                    {
                        pCompressedMB->ChangeType(VC1_ENC_P_MB_SKIP_4MV);
                        MBType = VC1_ENC_P_MB_SKIP_4MV;
#ifdef VC1_ENC_DEBUG_ON
                        pDebug->SetMBType(VC1_ENC_P_MB_SKIP_4MV);
                        pDebug->SetMBAsSkip();
#endif
                    }
#ifdef VC1_ENC_DEBUG_ON
                    if (MBType != VC1_ENC_P_MB_SKIP_4MV)
                    {
                        pDebug->SetCPB(MBPattern, CBPCY);
                        pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                    }
#endif
                } // end compressing

#ifdef VC1_ENC_DEBUG_ON
                if (MBType != VC1_ENC_P_MB_SKIP_4MV)
                {
                    pDebug->SetCPB(MBPattern, CBPCY);
                    pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                }
#endif

STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
                if (bRaiseFrame)
                {
                    if (MBPattern!=0 || intraPattern!=0)
                     {
STATISTICS_START_TIME(m_TStat->InvQT_StartTime);
                        for (blk = 0; blk <6 ; blk++)
                        {
                            if (!pCurMBInfo->isIntra(blk))
                            {
                                 InterInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                    TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                                    doubleQuant,BlockTSTypes[blk]);
                            }
                            else
                            {
                                IntraInvTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                    TempBlock.m_pBlock[blk],TempBlock.m_uiBlockStep[blk],
                                                    DCQuant,doubleQuant);
                            }
                        } // for
STATISTICS_END_TIME(m_TStat->InvQT_StartTime, m_TStat->InvQT_EndTime, m_TStat->InvQT_TotalTime);

                        ippiMC16x16_8u_C1(sYInterpolation.pDst,sYInterpolation.dstStep,
                                    TempBlock.m_pBlock[0],TempBlock.m_uiBlockStep[0],
                                    pRFrameY,m_uiRaisedPlaneStep[0],
                                    0, 0);

                        ippiMC8x8_8u_C1(sUInterpolation.pDst,sUInterpolation.dstStep,
                                TempBlock.m_pBlock[4],TempBlock.m_uiBlockStep[4],
                                pRFrameU, m_uiRaisedPlaneStep[1],
                                0, 0);

                        ippiMC8x8_8u_C1(sVInterpolation.pDst,sVInterpolation.dstStep,
                                TempBlock.m_pBlock[5],TempBlock.m_uiBlockStep[5],
                                pRFrameV, m_uiRaisedPlaneStep[2],
                                0, 0);

                    }
                    else
                    {
                        ippiCopy_8u_C1R(sYInterpolation.pDst, sYInterpolation.dstStep, pRFrameY,m_uiRaisedPlaneStep[0],blkSizeLuma);
                        ippiCopy_8u_C1R(sUInterpolation.pDst, sUInterpolation.dstStep, pRFrameU,m_uiRaisedPlaneStep[1],blkSize);
                        ippiCopy_8u_C1R(sVInterpolation.pDst, sVInterpolation.dstStep, pRFrameV,m_uiRaisedPlaneStep[2],blkSize);
                    }

                    if (m_pSequenceHeader->IsLoopFilter())
                    {
                        Ipp8u YFlag0 = 0,YFlag1 = 0, YFlag2 = 0, YFlag3 = 0;
                        Ipp8u UFlag0 = 0,UFlag1 = 0;
                        Ipp8u VFlag0 = 0,VFlag1 = 0;

                        pCurMBInfo->SetBlocksPattern (pCompressedMB->GetBlocksPattern());
                        pCurMBInfo->SetVSTPattern(BlockTSTypes);

                        pGetExternalEdge (top,  pCurMBInfo, false, YFlag0,UFlag0,VFlag0);
                        pCurMBInfo->SetExternalEdgeHor(YFlag0,UFlag0,VFlag0);

                        pGetExternalEdge(left, pCurMBInfo, true,  YFlag0,UFlag0,VFlag0);
                        pCurMBInfo->SetExternalEdgeVer(YFlag0,UFlag0,VFlag0);

                        pGetInternalEdge (pCurMBInfo, YFlag0,YFlag1);
                        pCurMBInfo->SetInternalEdge(YFlag0,YFlag1);

                        GetInternalBlockEdge(pCurMBInfo, YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);//ver

                        pCurMBInfo->SetInternalBlockEdge(YFlag0,YFlag1, UFlag0,VFlag0, //hor
                                                         YFlag2,YFlag3, UFlag1,VFlag1);// ver

                        //deblocking
                        {
                            STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
                            Ipp8u *pDblkPlanes[3] = {pRFrameY, pRFrameU, pRFrameV};

                            pDeblk_P_MB[deblkPattern](pDblkPlanes, m_uiRaisedPlaneStep, m_uiQuant, pCurMBInfo, top, topLeft, left);
                            deblkPattern = deblkPattern | 0x1 | ((!(Ipp8u)((j + 1 - (w -1)) >> 31)<<1));
                            STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
                        }

                    }
                }
STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime);

                if (m_pSavedMV)
                {
                    sCoordinate MVIntLuma    ={0};
                    sCoordinate MVQuarterLuma={0};
                    //pull back only simple/main profiles
                    GetIntQuarterMV(MVLuma,&MVIntLuma,&MVQuarterLuma);
                    ScalePredict(&MVIntLuma, j*8,i*8,MVSavedMBMin,MVSavedMBMax);
                    *(pSavedMV ++) = (MVIntLuma.x<<2) + MVQuarterLuma.x;
                    *(pSavedMV ++) = (MVIntLuma.y<<2) + MVQuarterLuma.y;

                }
            }
            break;

            }
#ifdef VC1_ENC_DEBUG_ON
            if(!bSubBlkTS)
            {
                pDebug->SetDblkHorEdgeLuma(pCurMBInfo->GetLumaExHorEdge(), pCurMBInfo->GetLumaInHorEdge(), 15, 15);
                pDebug->SetDblkVerEdgeLuma(pCurMBInfo->GetLumaExVerEdge(), pCurMBInfo->GetLumaInVerEdge(), 15, 15);
                pDebug->SetDblkHorEdgeU(pCurMBInfo->GetUExHorEdge(),15);
                pDebug->SetDblkHorEdgeV(pCurMBInfo->GetVExHorEdge(), 15);
                pDebug->SetDblkVerEdgeU(pCurMBInfo->GetUExVerEdge(), 15);
                pDebug->SetDblkVerEdgeV(pCurMBInfo->GetVExVerEdge(), 15);
            }
            else
            {
                pDebug->SetDblkHorEdgeLuma(pCurMBInfo->GetLumaExHorEdge(), pCurMBInfo->GetLumaInHorEdge(),
                                           pCurMBInfo->GetLumaAdUppEdge(), pCurMBInfo->GetLumaAdBotEdge() );
                pDebug->SetDblkVerEdgeLuma(pCurMBInfo->GetLumaExVerEdge(), pCurMBInfo->GetLumaInVerEdge(),
                                           pCurMBInfo->GetLumaAdLefEdge(), pCurMBInfo->GetLumaAdRigEdge());
                pDebug->SetDblkHorEdgeU(pCurMBInfo->GetUExHorEdge(), pCurMBInfo->GetUAdHorEdge());
                pDebug->SetDblkHorEdgeV(pCurMBInfo->GetVExHorEdge(), pCurMBInfo->GetVAdHorEdge());
                pDebug->SetDblkVerEdgeU(pCurMBInfo->GetUExVerEdge(), pCurMBInfo->GetUAdVerEdge());
                pDebug->SetDblkVerEdgeV(pCurMBInfo->GetVExVerEdge(), pCurMBInfo->GetVAdVerEdge());
            }
#endif
            err = m_pMBs->NextMB();
            if (err != UMC::UMC_OK && j < w-1)
                return err;

#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
        }

        deblkPattern = (deblkPattern | 0x4 | ( (! (Ipp8u)((i + 1 - (h -1)) >> 31)<<3))) & deblkMask;

////Row deblocking
//STATISTICS_START_TIME(m_TStat->Reconst_StartTime);
//        if (m_pSequenceHeader->IsLoopFilter() && bRaiseFrame && i!=0)
//        {
//STATISTICS_START_TIME(m_TStat->Deblk_StartTime);
//          Ipp8u *DeblkPlanes[3] = {m_pRaisedPlane[0] + i*m_uiRaisedPlaneStep[0]*VC1_ENC_LUMA_SIZE,
//                            m_pRaisedPlane[1] + i*m_uiRaisedPlaneStep[1]*VC1_ENC_CHROMA_SIZE,
//                            m_pRaisedPlane[2] + i*m_uiRaisedPlaneStep[2]*VC1_ENC_CHROMA_SIZE};
//            m_pMBs->StartRow();
//
//            if(bSubBlkTS)
//            {
//                if(i < h-1)
//                    Deblock_P_RowVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//                else
//                    Deblock_P_BottomRowVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//            }
//            else
//            {
//                if(i < h-1)
//                    Deblock_P_RowNoVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//                else
//                    Deblock_P_BottomRowNoVts(DeblkPlanes, m_uiRaisedPlaneStep, w, m_uiQuant, m_pMBs);
//            }
//STATISTICS_END_TIME(m_TStat->Deblk_StartTime, m_TStat->Deblk_EndTime, m_TStat->Deblk_TotalTime);
//        }
//STATISTICS_END_TIME(m_TStat->Reconst_StartTime, m_TStat->Reconst_EndTime, m_TStat->Reconst_TotalTime)
//

        err = m_pMBs->NextRow();
        if (err != UMC::UMC_OK)
            return err;

        pCurMBRow[0]+= m_uiPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pCurMBRow[1]+= m_uiPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pCurMBRow[2]+= m_uiPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

        pFMBRow[0]+= m_uiForwardPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pFMBRow[1]+= m_uiForwardPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pFMBRow[2]+= m_uiForwardPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

    }

#ifdef VC1_ENC_DEBUG_ON
    if(bRaiseFrame)
    pDebug->PrintRestoredFrame(m_pRaisedPlane[0], m_uiRaisedPlaneStep[0],
                               m_pRaisedPlane[1], m_uiRaisedPlaneStep[1],
                               m_pRaisedPlane[2], m_uiRaisedPlaneStep[2], 0);
#endif

    return err;
}

UMC::Status VC1EncoderPictureSM::VLC_PFrameMixed(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status     err = UMC::UMC_OK;
    Ipp32u          i=0, j=0;

    Ipp32u                      h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u                      w = m_pSequenceHeader->GetNumMBInRow();

    const Ipp16u*   pCBPCYTable = VLCTableCBPCY_PB[m_uiCBPTab];

    eCodingSet      LumaCodingSetIntra   = LumaCodingSetsIntra  [m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet      ChromaCodingSetIntra = ChromaCodingSetsIntra[m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet       CodingSetInter       = CodingSetsInter      [m_uiQuantIndex>8][m_uiDecTypeAC1];

    const sACTablesSet*  pACTablesSetIntra[6] = {&(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[ChromaCodingSetIntra]),
                                                 &(ACTablesSet[ChromaCodingSetIntra])};

    const sACTablesSet* pACTablesSetInter = &(ACTablesSet[CodingSetInter]);

    const Ipp32u*  pDCTableVLCIntra[6]  ={DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][0],
                                          DCTables[m_uiDecTypeDCIntra][1],
                                          DCTables[m_uiDecTypeDCIntra][1]};

    sACEscInfo   ACEscInfo = {(m_uiQuant<= 7 /*&& !VOPQuant*/)?
                               Mode3SizeConservativeVLC : Mode3SizeEfficientVLC,
                               0, 0};

    bool bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);
    bool bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR || m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

    const Ipp16s   (*pTTMBVLC)[4][6]   = 0;
    const Ipp8u    (* pTTBlkVLC)[6]    = 0;
    const Ipp8u     *pSubPattern4x4VLC = 0;

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetCurrMBFirst();
#endif

    err = WritePPictureHeader(pCodedPicture);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuant<5)
    {
        pTTMBVLC            =  TTMBVLC_HighRate;
        pTTBlkVLC           =  TTBLKVLC_HighRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_HighRate;
    }
    else if (m_uiQuant<13)
    {
        pTTMBVLC            =  TTMBVLC_MediumRate;
        pTTBlkVLC           =  TTBLKVLC_MediumRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_MediumRate;
    }
    else
    {
        pTTMBVLC            =  TTMBVLC_LowRate;
        pTTBlkVLC           =  TTBLKVLC_LowRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_LowRate;
    }

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);
#ifdef VC1_ME_MB_STATICTICS
            m_MECurMbStat->whole = 0;
            memset(m_MECurMbStat->MVF, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->MVB, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->coeff, 0, 6*sizeof(Ipp16u));
            m_MECurMbStat->qp    = 2*m_uiQuant + m_bHalfQuant;

            pCompressedMB->SetMEFrStatPointer(m_MECurMbStat);
#endif
            switch  (pCompressedMB->GetMBType())
            {
            case VC1_ENC_P_MB_INTRA:
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->MbType = UMC::ME_MbIntra;
#endif
                err = pCompressedMB->WritePMBMixed_INTRA (pCodedPicture,m_bRawBitplanes,m_uiQuant,
                                                          MVDiffTablesVLC[m_uiMVTab],
                                                          pCBPCYTable,
                                                          pDCTableVLCIntra,
                                                          pACTablesSetIntra,
                                                          &ACEscInfo);
                VC1_ENC_CHECK (err)


                    break;

            case VC1_ENC_P_MB_1MV:
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->MbType = UMC::ME_MbFrw;
#endif
                err = pCompressedMB->WritePMB1MVMixed(pCodedPicture, m_bRawBitplanes,m_uiMVTab,
                                                      m_uiMVRangeIndex, pCBPCYTable,
                                                      bCalculateVSTransform,
                                                      bMVHalf,
                                                      pTTMBVLC,
                                                      pTTBlkVLC,
                                                      pSubPattern4x4VLC,
                                                      pACTablesSetInter,
                                                      &ACEscInfo);
                VC1_ENC_CHECK (err)
                    break;

            case VC1_ENC_P_MB_SKIP_1MV:
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbFrwSkipped;
#endif
                err = pCompressedMB->WritePMB1MVSkipMixed(pCodedPicture, m_bRawBitplanes);
                VC1_ENC_CHECK (err)
                    break;
            case VC1_ENC_P_MB_SKIP_4MV:
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbFrwSkipped;
#endif
                err = pCompressedMB->WritePMB4MVSkipMixed(pCodedPicture, m_bRawBitplanes);
                VC1_ENC_CHECK (err)
                    break;

            case VC1_ENC_P_MB_4MV:
#ifdef VC1_ME_MB_STATICTICS
                m_MECurMbStat->MbType = UMC::ME_MbMixed;
#endif
                err = pCompressedMB->WritePMB4MVMixed(pCodedPicture,
                                                m_bRawBitplanes,   m_uiQuant,
                                                m_uiMVTab,         m_uiMVRangeIndex,
                                                pCBPCYTable,       MVDiffTablesVLC[m_uiMVTab],
                                                bCalculateVSTransform, bMVHalf,
                                                pTTMBVLC,          pTTBlkVLC,
                                                pSubPattern4x4VLC, pDCTableVLCIntra,
                                                pACTablesSetIntra, pACTablesSetInter,
                                                &ACEscInfo);
                VC1_ENC_CHECK (err)
                    break;
            }
#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
#ifdef VC1_ME_MB_STATICTICS
           m_MECurMbStat++;
#endif
        }
    }

    return err;
}

UMC::Status VC1EncoderPictureSM::PAC_BFrame(UMC::MeParams* MEParams)
{
    UMC::Status err           =   UMC::UMC_OK;
    Ipp32u   i = 0, j = 0, blk = 0;
    Ipp8u*   pCurMBRow[3],* pFMB[3], *pBMB[3];
    Ipp32u   h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u   w = m_pSequenceHeader->GetNumMBInRow();

    IntraTransformQuantFunction IntraTransformQuantACFunction = (m_bUniformQuant) ? IntraTransformQuantUniform :
                                                                        IntraTransformQuantNonUniform;
    InterTransformQuantFunction InterTransformQuantACFunction = (m_bUniformQuant) ? InterTransformQuantUniform :
                                                                        InterTransformQuantNonUniform;

    CalculateChromaFunction     CalculateChroma      = (m_pSequenceHeader->IsFastUVMC())?
                                                        GetChromaMVFast:GetChromaMV;
    // ------------------------------------------------------------------------------------------------ //

    bool                        bIsBilinearInterpolation = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR);
    InterpolateFunction         InterpolateLumaFunction  = (bIsBilinearInterpolation)?
                                    ippiInterpolateQPBilinear_VC1_8u_C1R:
                                    ippiInterpolateQPBicubic_VC1_8u_C1R;
    InterpolateFunction         InterpolateChromaFunction  =    ippiInterpolateQPBilinear_VC1_8u_C1R;

    IppVCInterpolate_8u         sYInterpolationF;
    IppVCInterpolate_8u         sUInterpolationF;
    IppVCInterpolate_8u         sVInterpolationF;
    IppVCInterpolate_8u         sYInterpolationB;
    IppVCInterpolate_8u         sUInterpolationB;
    IppVCInterpolate_8u         sVInterpolationB;

    Ipp8u                       tempInterpolationBufferF[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];
    Ipp8u                       tempInterpolationBufferB[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];
    IppStatus                   ippSts = ippStsNoErr;

    eDirection                  direction[VC1_ENC_NUMBER_OF_BLOCKS];

    VC1EncoderMBData            TempBlock;
    Ipp16s                      tempBuffer[VC1_ENC_BLOCK_SIZE*VC1_ENC_NUMBER_OF_BLOCKS];
    IppiSize                    blkSize     = {8,8};

    sCoordinate                 MVPredMin = {-28,-28};
    sCoordinate                 MVPredMax = {((Ipp16s)w*8-1)*4, ((Ipp16s)h*8-1)*4};

    Ipp8u                       doubleQuant     =  2*m_uiQuant + m_bHalfQuant;
    Ipp8u                       DCQuant         =  DCQuantValues[m_uiQuant];
    Ipp16s                      defPredictor    =  0;

    bool  bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR ||
                     m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

    bool  bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);

 #ifdef VC1_ENC_DEBUG_ON
    pDebug->SetHalfCoef(bMVHalf);
    pDebug->SetDeblkFlag(false);
    pDebug->SetVTSFlag(false);
#endif

    TempBlock.InitBlocks(tempBuffer);

    pCurMBRow[0]      =   m_pPlane[0];        //Luma
    pCurMBRow[1]      =   m_pPlane[1];        //Cb
    pCurMBRow[2]      =   m_pPlane[2];        //Cr

    pFMB[0]      =   m_pForwardPlane[0];        //Luma
    pFMB[1]      =   m_pForwardPlane[1];        //Cb
    pFMB[2]      =   m_pForwardPlane[2];        //Cr

    pBMB[0]      =   m_pBackwardPlane[0];        //Luma
    pBMB[1]      =   m_pBackwardPlane[1];        //Cb
    pBMB[2]      =   m_pBackwardPlane[2];        //Cr

    err = m_pMBs->Reset();
    if (err != UMC::UMC_OK)
        return err;

    SetInterpolationParams(&sYInterpolationF,&sUInterpolationF,&sVInterpolationF,
                           tempInterpolationBufferF,true);
    SetInterpolationParams(&sYInterpolationB,&sUInterpolationB,&sVInterpolationB,
                           tempInterpolationBufferB,false);

#ifdef VC1_ENC_CHECK_MV
    err = CheckMEMV_B(MEParams, bMVHalf);
    assert(err == UMC::UMC_OK);
#endif

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            VC1EncoderMBInfo  *         pCurMBInfo     = 0;
            VC1EncoderMBData  *         pCurMBData     = 0;
            VC1EncoderCodedMB*          pCompressedMB  = &(m_pCodedMB[w*i+j]);
            Ipp32s                      posX           =  VC1_ENC_LUMA_SIZE*j;
            Ipp32s                      posXChroma     =  VC1_ENC_CHROMA_SIZE*j;
            Ipp8u                       MBPattern       = 0;
            Ipp8u                       CBPCY           = 0;
            eMBType                     mbType         =  VC1_ENC_B_MB_F; //From ME

            //Ipp16s          posY        =  VC1_ENC_LUMA_SIZE*i;

            sCoordinate MVF         ={0,0};
            sCoordinate MVB         ={0,0};

            pCurMBInfo  =   m_pMBs->GetCurrMBInfo();
            pCurMBData  =   m_pMBs->GetCurrMBData();

            VC1EncoderMBInfo* left        = m_pMBs->GetLeftMBInfo();
            VC1EncoderMBInfo* topLeft     = m_pMBs->GetTopLeftMBInfo();
            VC1EncoderMBInfo* top         = m_pMBs->GetTopMBInfo();
            VC1EncoderMBInfo* topRight    = m_pMBs->GetTopRightMBInfo();

            MVF.x  = MEParams->pSrc->MBs[j + i*w].MV[0][0].x;
            MVF.y  = MEParams->pSrc->MBs[j + i*w].MV[0][0].y;
            MVB.x  = MEParams->pSrc->MBs[j + i*w].MV[1][0].x;
            MVB.y  = MEParams->pSrc->MBs[j + i*w].MV[1][0].y;

            switch (MEParams->pSrc->MBs[(j + i*w)].MbType)
            {
            case UMC::ME_MbIntra:
                  MVF.x = MVF.y = MVB.x = MVB.y = 0;
                  mbType = VC1_ENC_B_MB_INTRA;
                  break;

            case UMC::ME_MbFrw:

                mbType = VC1_ENC_B_MB_F;
                break;

            case UMC::ME_MbFrwSkipped:

                mbType = VC1_ENC_B_MB_SKIP_F;
                break;

            case UMC::ME_MbBkw:

                mbType = VC1_ENC_B_MB_B;
                break;
            case UMC::ME_MbBkwSkipped:

                mbType = VC1_ENC_B_MB_SKIP_B;
                break;
            case UMC::ME_MbBidir:

                mbType = VC1_ENC_B_MB_FB;
                break;

            case UMC::ME_MbBidirSkipped:

                mbType = VC1_ENC_B_MB_SKIP_FB;

                break;

            case UMC::ME_MbDirect:

                mbType = VC1_ENC_B_MB_DIRECT;

                break;
            case UMC::ME_MbDirectSkipped:

                mbType = VC1_ENC_B_MB_SKIP_DIRECT;

                break;
            default:
                assert(0);
                return UMC::UMC_ERR_FAILED;


            }
            switch(mbType)
            {
            case VC1_ENC_B_MB_INTRA:
                {
                bool                dACPrediction   = true;
                NeighbouringMBsData MBs;

                MBs.LeftMB    = ((left)? left->isIntra():0)         ? m_pMBs->GetLeftMBData():0;
                MBs.TopMB     = ((top)? top->isIntra():0)           ? m_pMBs->GetTopMBData():0;
                MBs.TopLeftMB = ((topLeft)? topLeft->isIntra():0)   ? m_pMBs->GetTopLeftMBData():0;

                pCompressedMB->Init(VC1_ENC_B_MB_INTRA);
                pCurMBInfo->Init(true);

                pCurMBData->CopyMBProg( pCurMBRow[0]+posX,  m_uiPlaneStep[0],
                                        pCurMBRow[1]+posXChroma,m_uiPlaneStep[1],
                                        pCurMBRow[2]+posXChroma,m_uiPlaneStep[2]);
                //only intra blocks:
                for (blk = 0; blk<6; blk++)
                {
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                    ippiSubC_16s_C1IRSfs(128, pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk], blkSize, 0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    IntraTransformQuantACFunction(pCurMBData->m_pBlock[blk], pCurMBData->m_uiBlockStep[blk],
                                         DCQuant, doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                }
STATISTICS_START_TIME(m_TStat->Intra_StartTime);
                    // should be changed on DCACPredictionPFrameSM
                    dACPrediction = DCACPredictionFrame     ( pCurMBData,&MBs,
                                                            &TempBlock, defPredictor,direction);
STATISTICS_END_TIME(m_TStat->Intra_StartTime, m_TStat->Intra_EndTime, m_TStat->Intra_TotalTime);

                for (blk=0; blk<6; blk++)
                {
                    pCompressedMB->SaveResidual(    TempBlock.m_pBlock[blk],
                                                    TempBlock.m_uiBlockStep[blk],
                                                    VC1_Inter_8x8_Scan,
                                                    blk);
                }
                MBPattern = pCompressedMB->GetMBPattern();
                CBPCY = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetACPrediction(dACPrediction);
                pCompressedMB->SetMBCBPCY(CBPCY);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(VC1_ENC_B_MB_INTRA);
                pDebug->SetCPB(MBPattern, CBPCY);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
#endif
                break;
                }
            case VC1_ENC_B_MB_SKIP_F:
                {
                sCoordinate  *mvC=0,   *mvB=0,   *mvA=0;
                sCoordinate  mv1={0,0}, mv2={0,0}, mv3={0,0};
                sCoordinate  mvPred   = {0,0};

                pCompressedMB->Init(mbType);

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                GetMVPredictionP(true)
                PredictMV(mvA,mvB,mvC, &mvPred);
                ScalePredict(&mvPred, j*8*4,i*8*4,MVPredMin,MVPredMax);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                assert (mvPred.x == MVF.x && mvPred.y == MVF.y);

                MVF.x = mvPred.x;  MVF.y = mvPred.y;

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetMBAsSkip();
                pDebug->SetMVInfo(&MVF, mvPred.x, mvPred.y, 0);
                pDebug->SetMVInfo(&MVB, mvPred.x, mvPred.y, 1);
#endif
                break;
                }
            case VC1_ENC_B_MB_SKIP_B:
                {
                sCoordinate  *mvC=0,   *mvB=0,   *mvA=0;
                sCoordinate  mv1={0,0}, mv2={0,0}, mv3={0,0};
                sCoordinate  mvPred   = {0,0};

                pCompressedMB->Init(mbType);

STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                GetMVPredictionP(false)
                PredictMV(mvA,mvB,mvC, &mvPred);
                ScalePredict(&mvPred, j*8*4,i*8*4,MVPredMin,MVPredMax);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                assert (mvPred.x == MVB.x && mvPred.y == MVB.y);

                MVB.x = mvPred.x;  MVB.y = mvPred.y;

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetMBAsSkip();
                pDebug->SetMVInfo(&MVF, mvPred.x, mvPred.y, 0);
                pDebug->SetMVInfo(&MVB, mvPred.x, mvPred.y, 1);

#endif
                break;
                }
            case VC1_ENC_B_MB_F:
            case VC1_ENC_B_MB_B:
                {
                sCoordinate  *mvC=0,   *mvB=0,   *mvA=0;
                sCoordinate  mv1={0,0}, mv2={0,0}, mv3={0,0};
                sCoordinate  mvPred   = {0,0};
                sCoordinate  mvDiff   = {0,0};
                bool         bForward = (mbType == VC1_ENC_B_MB_F);

                IppVCInterpolate_8u*         pYInterpolation= &sYInterpolationF;
                IppVCInterpolate_8u*         pUInterpolation= &sUInterpolationF;
                IppVCInterpolate_8u*         pVInterpolation= &sVInterpolationF;

                Ipp8u**                      pPredMB   = pFMB;
                Ipp32u*                      pPredStep = m_uiForwardPlaneStep;

                sCoordinate*                 pMV                = &MVF;

                sCoordinate                  MVInt             = {0,0};
                sCoordinate                  MVQuarter         = {0,0};

                sCoordinate                  MVChroma          = {0,0};
                sCoordinate                  MVIntChroma       = {0,0};
                sCoordinate                  MVQuarterChroma   = {0,0};


                eTransformType  BlockTSTypes[6] = { m_uiTransformType, m_uiTransformType,
                                                    m_uiTransformType, m_uiTransformType,
                                                    m_uiTransformType, m_uiTransformType};


                if (bCalculateVSTransform)
                {
                    GetTSType (MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                }
                if (!bForward)
                {
                    pYInterpolation= &sYInterpolationB;
                    pUInterpolation= &sUInterpolationB;
                    pVInterpolation= &sVInterpolationB;
                    pPredMB   = pBMB;
                    pPredStep = m_uiBackwardPlaneStep;
                    pMV       = &MVB;
                }

                pCompressedMB->Init(mbType);
                pCompressedMB->SetTSType(BlockTSTypes);

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

 STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                GetMVPredictionP(bForward)
                PredictMV(mvA,mvB,mvC, &mvPred);
                ScalePredict(&mvPred, j*8*4,i*8*4,MVPredMin,MVPredMax);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                mvDiff.x = (pMV->x - mvPred.x);
                mvDiff.y = (pMV->y - mvPred.y);
                pCompressedMB->SetdMV(mvDiff,bForward);

                GetIntQuarterMV(*pMV,&MVInt,&MVQuarter);
                CalculateChroma(*pMV,&MVChroma);
                GetIntQuarterMV(MVChroma,&MVIntChroma,&MVQuarterChroma);

STATISTICS_START_TIME(m_TStat->Interpolate_StartTime);

                SetInterpolationParamsSrc(pYInterpolation,pPredMB[0]+ posX,       pPredStep[0],   &MVInt,       &MVQuarter);
                SetInterpolationParamsSrc(pUInterpolation,pPredMB[1]+ posXChroma, pPredStep[1],   &MVIntChroma, &MVQuarterChroma);
                SetInterpolationParamsSrc(pVInterpolation,pPredMB[2]+ posXChroma, pPredStep[2],   &MVIntChroma, &MVQuarterChroma);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippSts = InterpolateLumaFunction(pYInterpolation);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(pUInterpolation);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(pVInterpolation);
                VC1_ENC_IPP_CHECK(ippSts)
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
STATISTICS_END_TIME(m_TStat->Interpolate_StartTime, m_TStat->Interpolate_EndTime, m_TStat->Interpolate_TotalTime);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippiGetDiff16x16_8u16s_C1(pCurMBRow[0]+ posX,            m_uiPlaneStep[0],
                                     pYInterpolation->pDst,   pYInterpolation->dstStep,
                                     pCurMBData->m_pBlock[0], pCurMBData->m_uiBlockStep[0],
                                     0,0,0,0);
                ippiGetDiff8x8_8u16s_C1  (pCurMBRow[1]+ posXChroma,      m_uiPlaneStep[1],
                                     pUInterpolation->pDst,   pUInterpolation->dstStep,
                                     pCurMBData->m_pBlock[4], pCurMBData->m_uiBlockStep[4],
                                     0,0,0,0);
                ippiGetDiff8x8_8u16s_C1  (pCurMBRow[2]+ posXChroma,      m_uiPlaneStep[2],
                                     pVInterpolation->pDst,   pVInterpolation->dstStep,
                                     pCurMBData->m_pBlock[5], pCurMBData->m_uiBlockStep[5],
                                     0,0,0,0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

                MBPattern = 0;
                for (blk = 0; blk<6; blk++)
                {
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                  BlockTSTypes[blk], doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                    pCompressedMB->SaveResidual(pCurMBData->m_pBlock[blk],
                                                 pCurMBData->m_uiBlockStep[blk],
                                                 ZagTables[BlockTSTypes[blk]],
                                                 blk);
                }
                MBPattern   = pCompressedMB->GetMBPattern();
                CBPCY       = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetMBCBPCY(CBPCY);
                if (MBPattern==0 && mvDiff.x == 0 && mvDiff.y == 0)
                {
                    if (bForward)
                    {
                        pCompressedMB->ChangeType(VC1_ENC_B_MB_SKIP_F);
                        mbType = VC1_ENC_B_MB_SKIP_F;
                    }
                    else
                    {
                        pCompressedMB->ChangeType(VC1_ENC_B_MB_SKIP_B);
                        mbType = VC1_ENC_B_MB_SKIP_B;
                    }
#ifdef VC1_ENC_DEBUG_ON
                    pDebug->SetMBAsSkip();
#endif
                }

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetMVInfo(&MVF,mvPred.x, mvPred.y, 0);
                pDebug->SetMVInfo(&MVB, mvPred.x, mvPred.y, 1);

                pDebug->SetMVInfo(&MVChroma,  0, 0, !bForward, 4);
                pDebug->SetMVInfo(&MVChroma,  0, 0, !bForward, 5);
                pDebug->SetCPB(MBPattern, CBPCY);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);

                pDebug->SetInterpInfo(pYInterpolation,  pUInterpolation, pVInterpolation, !bForward);
#endif
                break;
                }
            case VC1_ENC_B_MB_FB:
                {
                sCoordinate  *mvC_F=0,   *mvB_F=0,   *mvA_F=0;
                sCoordinate  *mvC_B=0,   *mvB_B=0,   *mvA_B=0;
                sCoordinate  mv1={0,0}, mv2={0,0}, mv3={0,0};
                sCoordinate  mv4={0,0}, mv5={0,0}, mv6={0,0};
                sCoordinate  mvPredF    = {0,0};
                sCoordinate  mvPredB    = {0,0};
                sCoordinate  mvDiffF    = {0,0};
                sCoordinate  mvDiffB    = {0,0};
                sCoordinate  MVIntF     = {0,0};
                sCoordinate  MVQuarterF = {0,0};
                sCoordinate  MVIntB     = {0,0};
                sCoordinate  MVQuarterB = {0,0};

                sCoordinate  MVChromaF       = {0,0};
                sCoordinate  MVIntChromaF    = {0,0};
                sCoordinate  MVQuarterChromaF= {0,0};
                sCoordinate  MVChromaB       = {0,0};
                sCoordinate  MVIntChromaB    = {0,0};
                sCoordinate  MVQuarterChromaB= {0,0};


                eTransformType  BlockTSTypes[6] = { m_uiTransformType, m_uiTransformType,
                                                    m_uiTransformType, m_uiTransformType,
                                                    m_uiTransformType, m_uiTransformType};
                if (bCalculateVSTransform)
                {
                    GetTSType (MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                }

                pCompressedMB->Init(mbType);
                pCompressedMB->SetTSType(BlockTSTypes);

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

 STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                GetMVPredictionB
                PredictMV(mvA_F,mvB_F,mvC_F, &mvPredF);
                PredictMV(mvA_B,mvB_B,mvC_B, &mvPredB);
                ScalePredict(&mvPredF, j*8*4,i*8*4,MVPredMin,MVPredMax);
                ScalePredict(&mvPredB, j*8*4,i*8*4,MVPredMin,MVPredMax);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                mvDiffF.x = (MVF.x - mvPredF.x);
                mvDiffF.y = (MVF.y - mvPredF.y);
                pCompressedMB->SetdMV(mvDiffF,true);

                mvDiffB.x = (MVB.x - mvPredB.x);
                mvDiffB.y = (MVB.y - mvPredB.y);
                pCompressedMB->SetdMV(mvDiffB,false);

                GetIntQuarterMV(MVF,&MVIntF,&MVQuarterF);
                CalculateChroma(MVF,&MVChromaF);
                GetIntQuarterMV(MVChromaF,&MVIntChromaF,&MVQuarterChromaF);

                GetIntQuarterMV(MVB,&MVIntB,&MVQuarterB);
                CalculateChroma(MVB,&MVChromaB);
                GetIntQuarterMV(MVChromaB,&MVIntChromaB,&MVQuarterChromaB);

STATISTICS_START_TIME(m_TStat->Interpolate_StartTime);

                SetInterpolationParamsSrc(&sYInterpolationF,pFMB[0]+ posX,  m_uiForwardPlaneStep[0],  &MVIntF,       &MVQuarterF);
                SetInterpolationParamsSrc(&sUInterpolationF,pFMB[1]+ posXChroma, m_uiForwardPlaneStep[1], &MVIntChromaF, &MVQuarterChromaF);
                SetInterpolationParamsSrc(&sVInterpolationF,pFMB[2]+ posXChroma, m_uiForwardPlaneStep[2], &MVIntChromaF, &MVQuarterChromaF);

                SetInterpolationParamsSrc(&sYInterpolationB,pBMB[0]+ posX,  m_uiBackwardPlaneStep[0],  &MVIntB,       &MVQuarterB);
                SetInterpolationParamsSrc(&sUInterpolationB,pBMB[1]+ posXChroma, m_uiBackwardPlaneStep[1], &MVIntChromaB, &MVQuarterChromaB);
                SetInterpolationParamsSrc(&sVInterpolationB,pBMB[2]+ posXChroma, m_uiBackwardPlaneStep[2], &MVIntChromaB, &MVQuarterChromaB);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippSts = InterpolateLumaFunction(&sYInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sUInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sVInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)

                ippSts = InterpolateLumaFunction(&sYInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sUInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sVInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

STATISTICS_END_TIME(m_TStat->Interpolate_StartTime, m_TStat->Interpolate_EndTime, m_TStat->Interpolate_TotalTime);
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippiGetDiff16x16B_8u16s_C1(pCurMBRow[0]+ posX,            m_uiPlaneStep[0],
                                           sYInterpolationF.pDst,   sYInterpolationF.dstStep, 0,
                                           sYInterpolationB.pDst,   sYInterpolationB.dstStep, 0,
                                           pCurMBData->m_pBlock[0], pCurMBData->m_uiBlockStep[0],0);
                ippiGetDiff8x8B_8u16s_C1  (pCurMBRow[1]+ posXChroma,      m_uiPlaneStep[1],
                                           sUInterpolationF.pDst,   sUInterpolationF.dstStep, 0,
                                           sUInterpolationB.pDst,   sUInterpolationB.dstStep, 0,
                                           pCurMBData->m_pBlock[4], pCurMBData->m_uiBlockStep[4],0);
                ippiGetDiff8x8B_8u16s_C1  (pCurMBRow[2]+ posXChroma,      m_uiPlaneStep[2],
                                           sVInterpolationF.pDst,   sVInterpolationF.dstStep, 0,
                                           sVInterpolationB.pDst,   sVInterpolationB.dstStep, 0,
                                           pCurMBData->m_pBlock[5], pCurMBData->m_uiBlockStep[5],0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

                for (blk = 0; blk<6; blk++)
                {
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                  BlockTSTypes[blk], doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                    pCompressedMB->SaveResidual(pCurMBData->m_pBlock[blk],
                                                 pCurMBData->m_uiBlockStep[blk],
                                                 ZagTables[BlockTSTypes[blk]],
                                                 blk);
                }
                MBPattern   = pCompressedMB->GetMBPattern();
                CBPCY       = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetMBCBPCY(CBPCY);
                if (MBPattern==0 && mvDiffF.x == 0 && mvDiffF.y == 0 && mvDiffB.x == 0 && mvDiffB.y == 0)
                {
                     pCompressedMB->ChangeType(VC1_ENC_B_MB_SKIP_FB);
                     mbType = VC1_ENC_B_MB_SKIP_FB;
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBAsSkip();
#endif
                }

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetMVInfo(&MVF, mvPredF.x, mvPredF.y, 0);
                pDebug->SetMVInfo(&MVB, mvPredB.x, mvPredB.y, 1);
                pDebug->SetMVInfo(&MVChromaF,  0, 0, 0, 4);
                pDebug->SetMVInfo(&MVChromaF,  0, 0, 0, 5);
                pDebug->SetMVInfo(&MVChromaB,  0, 0, 1, 4);
                pDebug->SetMVInfo(&MVChromaB,  0, 0, 1, 5);
                pDebug->SetCPB(MBPattern, CBPCY);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                pDebug->SetInterpInfo(&sYInterpolationF,  &sUInterpolationF, &sVInterpolationF, 0);
                pDebug->SetInterpInfo(&sYInterpolationB,  &sUInterpolationB, &sVInterpolationB, 1);
#endif
                break;
                }
            case VC1_ENC_B_MB_SKIP_FB:
                {
                sCoordinate  *mvC_F=0,   *mvB_F=0,   *mvA_F=0;
                sCoordinate  *mvC_B=0,   *mvB_B=0,   *mvA_B=0;
                sCoordinate  mv1={0,0}, mv2={0,0}, mv3={0,0};
                sCoordinate  mv4={0,0}, mv5={0,0}, mv6={0,0};
                sCoordinate  mvPredF    = {0,0};
                sCoordinate  mvPredB    = {0,0};

                pCompressedMB->Init(mbType);

  STATISTICS_START_TIME(m_TStat->Inter_StartTime);
                GetMVPredictionB
                PredictMV(mvA_F,mvB_F,mvC_F, &mvPredF);
                PredictMV(mvA_B,mvB_B,mvC_B, &mvPredB);
                ScalePredict(&mvPredF, j*8*4,i*8*4,MVPredMin,MVPredMax);
                ScalePredict(&mvPredB, j*8*4,i*8*4,MVPredMin,MVPredMax);
STATISTICS_END_TIME(m_TStat->Inter_StartTime, m_TStat->Inter_EndTime, m_TStat->Inter_TotalTime);

                assert (MVF.x == mvPredF.x && MVF.y == mvPredF.y &&
                        MVB.x == mvPredB.x && MVB.y == mvPredB.y);

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(mvPredF,true);
                pCurMBInfo->SetMV(mvPredB,false);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetMBAsSkip();

                pDebug->SetMVInfo(&MVF, mvPredF.x, mvPredF.y, 0);
                pDebug->SetMVInfo(&MVB, mvPredB.x, mvPredB.y, 1);

                //pDebug->SetMVInfo(MVChroma.x, MVChroma.y,  0, 0, !bForward, 4);
                //pDebug->SetMVInfo(MVChroma.x, MVChroma.y,  0, 0, !bForward, 5);

#endif
                break;
                }
            case VC1_ENC_B_MB_DIRECT:
                {
                //sCoordinate  mvPredF    = {0,0};
                //sCoordinate  mvPredB    = {0,0};
                sCoordinate  MVIntF     = {0,0};
                sCoordinate  MVQuarterF = {0,0};
                sCoordinate  MVIntB     = {0,0};
                sCoordinate  MVQuarterB = {0,0};

                sCoordinate  MVChromaF       = {0,0};
                sCoordinate  MVIntChromaF    = {0,0};
                sCoordinate  MVQuarterChromaF= {0,0};
                sCoordinate  MVChromaB       = {0,0};
                sCoordinate  MVIntChromaB    = {0,0};
                sCoordinate  MVQuarterChromaB= {0,0};

                eTransformType  BlockTSTypes[6] = { m_uiTransformType,
                                                    m_uiTransformType,
                                                    m_uiTransformType,
                                                    m_uiTransformType,
                                                    m_uiTransformType,
                                                    m_uiTransformType};

               if (bCalculateVSTransform)
                {
                    GetTSType(MEParams->pSrc->MBs[j + i*w].BlockTrans, BlockTSTypes);
                }
                //direct
                pCompressedMB->Init(mbType);
                pCompressedMB->SetTSType(BlockTSTypes);

                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

                GetIntQuarterMV(MVF,&MVIntF,&MVQuarterF);
                CalculateChroma(MVF,&MVChromaF);
                GetIntQuarterMV(MVChromaF,&MVIntChromaF,&MVQuarterChromaF);

                GetIntQuarterMV(MVB,&MVIntB,&MVQuarterB);
                CalculateChroma(MVB,&MVChromaB);
                GetIntQuarterMV(MVChromaB,&MVIntChromaB,&MVQuarterChromaB);

STATISTICS_START_TIME(m_TStat->Interpolate_StartTime);
                SetInterpolationParamsSrc(&sYInterpolationF,pFMB[0]+ posX,  m_uiForwardPlaneStep[0],  &MVIntF,       &MVQuarterF);
                SetInterpolationParamsSrc(&sUInterpolationF,pFMB[1]+ posXChroma, m_uiForwardPlaneStep[1], &MVIntChromaF, &MVQuarterChromaF);
                SetInterpolationParamsSrc(&sVInterpolationF,pFMB[2]+ posXChroma, m_uiForwardPlaneStep[2], &MVIntChromaF, &MVQuarterChromaF);

                SetInterpolationParamsSrc(&sYInterpolationB,pBMB[0]+ posX,  m_uiBackwardPlaneStep[0],  &MVIntB,       &MVQuarterB);
                SetInterpolationParamsSrc(&sUInterpolationB,pBMB[1]+ posXChroma, m_uiBackwardPlaneStep[1], &MVIntChromaB, &MVQuarterChromaB);
                SetInterpolationParamsSrc(&sVInterpolationB,pBMB[2]+ posXChroma, m_uiBackwardPlaneStep[2], &MVIntChromaB, &MVQuarterChromaB);

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippSts = InterpolateLumaFunction(&sYInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sUInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sVInterpolationF);
                VC1_ENC_IPP_CHECK(ippSts)

                ippSts = InterpolateLumaFunction(&sYInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sUInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
                ippSts = InterpolateChromaFunction(&sVInterpolationB);
                VC1_ENC_IPP_CHECK(ippSts)
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

STATISTICS_END_TIME(m_TStat->Interpolate_StartTime, m_TStat->Interpolate_EndTime, m_TStat->Interpolate_TotalTime);
LIB_STAT_START_TIME(m_LibStat->LibStartTime);
                ippiGetDiff16x16B_8u16s_C1(pCurMBRow[0]+ posX,            m_uiPlaneStep[0],
                                            sYInterpolationF.pDst,   sYInterpolationF.dstStep, 0,
                                            sYInterpolationB.pDst,   sYInterpolationB.dstStep, 0,
                                            pCurMBData->m_pBlock[0], pCurMBData->m_uiBlockStep[0],0);
                ippiGetDiff8x8B_8u16s_C1  (pCurMBRow[1]+ posXChroma,      m_uiPlaneStep[1],
                                            sUInterpolationF.pDst,   sUInterpolationF.dstStep, 0,
                                            sUInterpolationB.pDst,   sUInterpolationB.dstStep, 0,
                                            pCurMBData->m_pBlock[4], pCurMBData->m_uiBlockStep[4],0);
                ippiGetDiff8x8B_8u16s_C1  (pCurMBRow[2]+ posXChroma,      m_uiPlaneStep[2],
                                            sVInterpolationF.pDst,   sVInterpolationF.dstStep, 0,
                                            sVInterpolationB.pDst,   sVInterpolationB.dstStep, 0,
                                            pCurMBData->m_pBlock[5], pCurMBData->m_uiBlockStep[5],0);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);


                for (blk = 0; blk<6; blk++)
                {
STATISTICS_START_TIME(m_TStat->FwdQT_StartTime);
                    InterTransformQuantACFunction(pCurMBData->m_pBlock[blk],pCurMBData->m_uiBlockStep[blk],
                                                  BlockTSTypes[blk], doubleQuant);
STATISTICS_END_TIME(m_TStat->FwdQT_StartTime, m_TStat->FwdQT_EndTime, m_TStat->FwdQT_TotalTime);
                    pCompressedMB->SaveResidual(pCurMBData->m_pBlock[blk],
                                                 pCurMBData->m_uiBlockStep[blk],
                                                 ZagTables[BlockTSTypes[blk]],
                                                 blk);
                }
                MBPattern   = pCompressedMB->GetMBPattern();
                CBPCY       = MBPattern;
                pCurMBInfo->SetMBPattern(MBPattern);
                pCompressedMB->SetMBCBPCY(CBPCY);


                if (MBPattern==0)
                {
                     pCompressedMB->ChangeType(VC1_ENC_B_MB_SKIP_DIRECT);
                     mbType = VC1_ENC_B_MB_SKIP_DIRECT;
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBAsSkip();
#endif
                }

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetCPB(MBPattern, CBPCY);
                pDebug->SetMBType(mbType);
                pDebug->SetMVInfo(&MVF, 0, 0, 0);
                pDebug->SetMVInfo(&MVB, 0, 0, 1);
                pDebug->SetMVInfo(&MVChromaF,  0, 0, 0, 4);
                pDebug->SetMVInfo(&MVChromaF,  0, 0, 0, 5);
                pDebug->SetMVInfo(&MVChromaB,  0, 0, 1, 4);
                pDebug->SetMVInfo(&MVChromaB,  0, 0, 1, 5);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetBlockDifference(pCurMBData->m_pBlock, pCurMBData->m_uiBlockStep);
                pDebug->SetInterpInfo(&sYInterpolationF,  &sUInterpolationF, &sVInterpolationF, 0);
                pDebug->SetInterpInfo(&sYInterpolationB,  &sUInterpolationB, &sVInterpolationB, 1);
#endif
                break;
                }
            case VC1_ENC_B_MB_SKIP_DIRECT:
                {
                pCompressedMB->Init(mbType);
                pCurMBInfo->Init(false);
                pCurMBInfo->SetMV(MVF,true);
                pCurMBInfo->SetMV(MVB,false);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetMBType(mbType);
                pDebug->SetQuant(m_uiQuant,m_bHalfQuant);
                pDebug->SetMBAsSkip();
#endif
                break;
                }
            default:
                return UMC::UMC_ERR_FAILED;
            }

            err = m_pMBs->NextMB();
            if (err != UMC::UMC_OK && j < w-1)
                return err;

#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
        }
        err = m_pMBs->NextRow();
        if (err != UMC::UMC_OK)
            return err;

        pCurMBRow[0]+= m_uiPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pCurMBRow[1]+= m_uiPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pCurMBRow[2]+= m_uiPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

        pFMB[0]+= m_uiForwardPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pFMB[1]+= m_uiForwardPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pFMB[2]+= m_uiForwardPlaneStep[2]*VC1_ENC_CHROMA_SIZE;

        pBMB[0]+= m_uiBackwardPlaneStep[0]*VC1_ENC_LUMA_SIZE;
        pBMB[1]+= m_uiBackwardPlaneStep[1]*VC1_ENC_CHROMA_SIZE;
        pBMB[2]+= m_uiBackwardPlaneStep[2]*VC1_ENC_CHROMA_SIZE;
    }

    return err;
}

UMC::Status VC1EncoderPictureSM::VLC_BFrame(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status    err  = UMC::UMC_OK;
    Ipp32u         i = 0, j = 0, blk = 0;

    Ipp32u         h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u         w = m_pSequenceHeader->GetNumMBInRow();

    const Ipp16u*  pCBPCYTable = VLCTableCBPCY_PB[m_uiCBPTab];

    eCodingSet     LumaCodingSetIntra   = LumaCodingSetsIntra  [m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet     ChromaCodingSetIntra = ChromaCodingSetsIntra[m_uiQuantIndex>8][m_uiDecTypeAC1];
    eCodingSet     CodingSetInter       = CodingSetsInter      [m_uiQuantIndex>8][m_uiDecTypeAC1];

    const sACTablesSet*  pACTablesSetIntra[6] = {&(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[LumaCodingSetIntra]),
                                                 &(ACTablesSet[ChromaCodingSetIntra]),
                                                 &(ACTablesSet[ChromaCodingSetIntra])};

    const sACTablesSet* pACTablesSetInter = &(ACTablesSet[CodingSetInter]);


    const Ipp32u*       pDCTableVLCIntra[6] = {DCTables[m_uiDecTypeDCIntra][0],
                                               DCTables[m_uiDecTypeDCIntra][0],
                                               DCTables[m_uiDecTypeDCIntra][0],
                                               DCTables[m_uiDecTypeDCIntra][0],
                                               DCTables[m_uiDecTypeDCIntra][1],
                                               DCTables[m_uiDecTypeDCIntra][1]};

    sACEscInfo    ACEscInfo = {(m_uiQuant<= 7 /*&& !VOPQuant*/)?
                                Mode3SizeConservativeVLC : Mode3SizeEfficientVLC,
                                0, 0};

    bool bCalculateVSTransform = (m_pSequenceHeader->IsVSTransform())&&(!m_bVSTransform);
    bool bMVHalf = (m_uiMVMode == VC1_ENC_1MV_HALF_BILINEAR || m_uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? true: false;

    const Ipp16s  (*pTTMBVLC)[4][6]   = 0;
    const Ipp8u   (* pTTBlkVLC)[6]    = 0;
    const Ipp8u    *pSubPattern4x4VLC = 0;

#ifdef VC1_ENC_DEBUG_ON
    pDebug->SetCurrMBFirst();
#endif

    err = WriteBPictureHeader(pCodedPicture);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuant<5)
    {
        pTTMBVLC            =  TTMBVLC_HighRate;
        pTTBlkVLC           =  TTBLKVLC_HighRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_HighRate;
    }
    else if (m_uiQuant<13)
    {
        pTTMBVLC            =  TTMBVLC_MediumRate;
        pTTBlkVLC           =  TTBLKVLC_MediumRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_MediumRate;

    }
    else
    {
        pTTMBVLC            =  TTMBVLC_LowRate;
        pTTBlkVLC           =  TTBLKVLC_LowRate;
        pSubPattern4x4VLC   =  SubPattern4x4VLC_LowRate;
    }

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            VC1EncoderCodedMB*  pCompressedMB = &(m_pCodedMB[w*i+j]);

            switch(pCompressedMB->GetMBType())
            {
#ifdef VC1_ME_MB_STATICTICS
/*            m_MECurMbStat->whole = 0;
            memset(m_MECurMbStat->MVF, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->MVB, 0,   4*sizeof(Ipp16u));
            memset(m_MECurMbStat->coeff, 0, 6*sizeof(Ipp16u));
            m_MECurMbStat->qp    = 2*m_uiQuant + m_bHalfQuant;

            pCompressedMB->SetMEFrStatPointer(m_MECurMbStat);
*/
#endif
            case VC1_ENC_B_MB_INTRA:
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbIntra;
#endif

                err = pCompressedMB->WriteMBHeaderB_INTRA  (pCodedPicture,
                                                            m_bRawBitplanes,
                                                            MVDiffTablesVLC[m_uiMVTab],
                                                            pCBPCYTable);
                VC1_ENC_CHECK (err)

STATISTICS_START_TIME(m_TStat->AC_Coefs_StartTime);
                for (blk = 0; blk<6; blk++)
                {
                   err = pCompressedMB->WriteBlockDC(pCodedPicture,pDCTableVLCIntra[blk],m_uiQuant,blk);
                   VC1_ENC_CHECK (err)
                   err = pCompressedMB->WriteBlockAC(pCodedPicture,pACTablesSetIntra[blk],&ACEscInfo,blk);
                   VC1_ENC_CHECK (err)
                }
STATISTICS_END_TIME(m_TStat->AC_Coefs_StartTime, m_TStat->AC_Coefs_EndTime, m_TStat->AC_Coefs_TotalTime);
                break;
            case VC1_ENC_B_MB_SKIP_F:
            case VC1_ENC_B_MB_SKIP_B:
            case VC1_ENC_B_MB_SKIP_FB:
#ifdef VC1_ME_MB_STATICTICS
                if(pCompressedMB->GetMBType() == VC1_ENC_B_MB_SKIP_F)
                    m_MECurMbStat->MbType = UMC::ME_MbFrwSkipped;
                else if(pCompressedMB->GetMBType() == VC1_ENC_B_MB_SKIP_F)
                    m_MECurMbStat->MbType = UMC::ME_MbBkwSkipped;
                else
                    m_MECurMbStat->MbType = UMC::ME_MbBidirSkipped;
#endif

                err = pCompressedMB->WriteBMB_SKIP_NONDIRECT(  pCodedPicture,
                                                               m_bRawBitplanes,
                                                               2*m_uiBFraction.num < m_uiBFraction.denom);
                VC1_ENC_CHECK (err)
                break;
            case VC1_ENC_B_MB_F:
            case VC1_ENC_B_MB_B:
            case VC1_ENC_B_MB_FB:
#ifdef VC1_ME_MB_STATICTICS
                if(pCompressedMB->GetMBType() == VC1_ENC_B_MB_F)
                    m_MECurMbStat->MbType = UMC::ME_MbFrw;
                else if(pCompressedMB->GetMBType() == VC1_ENC_B_MB_B)
                    m_MECurMbStat->MbType = UMC::ME_MbBkw;
                else
                    m_MECurMbStat->MbType = UMC::ME_MbBidir;
#endif
                err = pCompressedMB->WriteBMB  (pCodedPicture,
                                                m_bRawBitplanes,
                                                m_uiMVTab,
                                                m_uiMVRangeIndex,
                                                pCBPCYTable,
                                                bCalculateVSTransform,
                                                bMVHalf,
                                                pTTMBVLC,
                                                pTTBlkVLC,
                                                pSubPattern4x4VLC,
                                                pACTablesSetInter,
                                                &ACEscInfo,
                                                2*m_uiBFraction.num < m_uiBFraction.denom);
               VC1_ENC_CHECK (err)
                break;
            case VC1_ENC_B_MB_DIRECT:
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbDirect;
#endif
               err = pCompressedMB->WriteBMB_DIRECT  (
                            pCodedPicture,
                            m_bRawBitplanes,
                            pCBPCYTable,
                            bCalculateVSTransform,
                            pTTMBVLC,
                            pTTBlkVLC,
                            pSubPattern4x4VLC,
                            pACTablesSetInter,
                            &ACEscInfo);
               VC1_ENC_CHECK (err)
               break;
            case VC1_ENC_B_MB_SKIP_DIRECT:
#ifdef VC1_ME_MB_STATICTICS
                    m_MECurMbStat->MbType = UMC::ME_MbDirectSkipped;
#endif
                err = pCompressedMB->WriteBMB_SKIP_DIRECT(  pCodedPicture,
                                                            m_bRawBitplanes);
                VC1_ENC_CHECK (err)

                break;
            }
#ifdef VC1_ENC_DEBUG_ON
            pDebug->NextMB();
#endif
#ifdef VC1_ME_MB_STATICTICS
           m_MECurMbStat++;
#endif
        }
    }

    return err;
}

UMC::Status   VC1EncoderPictureSM::WritePPictureHeader(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status     err           =   UMC::UMC_OK;
    bool            bBframes      =   m_pSequenceHeader->IsBFrames();
    Ipp8s           diff          =   m_uiAltPQuant -  m_uiQuant - 1;

    err = pCodedPicture->MakeBlankSegment(2);
    if (err!= UMC::UMC_OK)
        return err;
    if (m_pSequenceHeader->IsFrameInterpolation())
    {
        err =pCodedPicture->PutBits(m_bFrameInterpolation,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(m_uiFrameCount,2);
    if (err != UMC::UMC_OK)
        return err;
    if (m_pSequenceHeader->IsRangeRedution())
    {
        err = pCodedPicture->PutBits(m_bRangeRedution,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(frameTypeCodesVLC[bBframes][2*1],frameTypeCodesVLC[bBframes][2*1+1]);
    if (err!= UMC::UMC_OK)
        return err;

    err = pCodedPicture->PutBits(m_uiQuantIndex, 5);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuantIndex <= 8)
    {
        err = pCodedPicture->PutBits(m_bHalfQuant, 1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->GetQuantType()== VC1_ENC_QTYPE_EXPL)
    {
        err = pCodedPicture->PutBits(m_bUniformQuant,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->IsExtendedMV())
    {
        err = pCodedPicture->PutBits(MVRangeCodesVLC[m_uiMVRangeIndex*2],MVRangeCodesVLC[m_uiMVRangeIndex*2+1]);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->IsMultiRes())
    {
        err = pCodedPicture->PutBits(m_uiResolution,2);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(MVModeP[m_bIntensity*2 + (m_uiQuant <= 12)][2*m_uiMVMode],MVModeP[m_bIntensity*2 + (m_uiQuant <= 12)][2*m_uiMVMode+1]);
    if (err != UMC::UMC_OK)
       return err;

    if (m_bIntensity)
    {
        err = pCodedPicture->PutBits(m_uiIntensityLumaScale,6);
        if (err != UMC::UMC_OK)
            return err;
        err = pCodedPicture->PutBits(m_uiIntensityLumaShift,6);
        if (err != UMC::UMC_OK)
            return err;
    }
    assert( m_bRawBitplanes == true);
    if (m_uiMVMode == VC1_ENC_MIXED_QUARTER_BICUBIC)
    {
        //raw bitplane for MB type
       err = pCodedPicture->PutBits(0,5);
       if (err != UMC::UMC_OK)
          return err;
    }
    // raw bitplane for skip MB
    err = pCodedPicture->PutBits(0,5);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiMVTab,2);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiCBPTab,2);
    if (err != UMC::UMC_OK)
       return err;

    switch (m_pSequenceHeader->GetDQuant())
    {
    case 2:
        if (diff>=0 && diff<7)
        {
            err = pCodedPicture->PutBits(diff,3);
        }
        else
        {
            err = pCodedPicture->PutBits((7<<5)+ m_uiAltPQuant,3+5);
        }
        if (err != UMC::UMC_OK)
            return err;
        break;
    case 1:
        err = pCodedPicture->PutBits( m_QuantMode != VC1_ENC_QUANT_SINGLE, 1);
        if (err != UMC::UMC_OK)
           return err;
        if (m_QuantMode != VC1_ENC_QUANT_SINGLE)
        {
           err =  pCodedPicture->PutBits(QuantProfileTableVLC[2*m_QuantMode], QuantProfileTableVLC[2*m_QuantMode+1]);
           if (err != UMC::UMC_OK)
                return err;
           if (m_QuantMode != VC1_ENC_QUANT_MB_ANY)
           {
                if (diff>=0 && diff<7)
                {
                    err = pCodedPicture->PutBits(diff,3);
                }
                else
                {
                    err = pCodedPicture->PutBits((7<<5)+ m_uiAltPQuant,3+5);
                }
                if (err != UMC::UMC_OK)
                    return err;
           }
        }
        break;
    default:
        break;
    }


    if (m_pSequenceHeader->IsVSTransform())
    {
        err = pCodedPicture->PutBits(m_bVSTransform,1);
        if (err != UMC::UMC_OK)
            return err;
        if (m_bVSTransform)
        {
            err = pCodedPicture->PutBits(m_uiTransformType,2);
            if (err != UMC::UMC_OK)
                return err;
        }
    }
    err = pCodedPicture->PutBits(ACTableCodesVLC[2*m_uiDecTypeAC1],ACTableCodesVLC[2*m_uiDecTypeAC1+1]);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiDecTypeDCIntra,1);
    return err;
}
UMC::Status   VC1EncoderPictureSM::WriteBPictureHeader(VC1EncoderBitStreamSM* pCodedPicture)
{
    UMC::Status     err           =   UMC::UMC_OK;
    Ipp8s           diff          =   m_uiAltPQuant -  m_uiQuant - 1;

    err = pCodedPicture->MakeBlankSegment(2);
    if (err!= UMC::UMC_OK)
        return err;
   if (m_pSequenceHeader->IsFrameInterpolation())
    {
        err =pCodedPicture->PutBits(m_bFrameInterpolation,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(m_uiFrameCount,2);
    if (err != UMC::UMC_OK)
        return err;
    if (m_pSequenceHeader->IsRangeRedution())
    {
        err = pCodedPicture->PutBits(m_bRangeRedution,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    err = pCodedPicture->PutBits(frameTypeCodesVLC[1][2*3],frameTypeCodesVLC[1][2*3+1]);
    if (err!= UMC::UMC_OK)
        return err;

    err = pCodedPicture->PutBits(BFractionVLC[m_uiBFraction.denom ][2*m_uiBFraction.num],BFractionVLC[m_uiBFraction.denom ][2*m_uiBFraction.num+1]);
    if (err!= UMC::UMC_OK)
        return err;

    err = pCodedPicture->PutBits(m_uiQuantIndex, 5);
    if (err != UMC::UMC_OK)
        return err;

    if (m_uiQuantIndex <= 8)
    {
        err = pCodedPicture->PutBits(m_bHalfQuant, 1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->GetQuantType()== VC1_ENC_QTYPE_EXPL)
    {
        err = pCodedPicture->PutBits(m_bUniformQuant,1);
        if (err != UMC::UMC_OK)
            return err;
    }
    if (m_pSequenceHeader->IsExtendedMV())
    {
        err = pCodedPicture->PutBits(MVRangeCodesVLC[m_uiMVRangeIndex*2],MVRangeCodesVLC[m_uiMVRangeIndex*2+1]);
        if (err != UMC::UMC_OK)
            return err;
    }

    err = pCodedPicture->PutBits(m_uiMVMode == VC1_ENC_1MV_QUARTER_BICUBIC,1);
    if (err != UMC::UMC_OK)
       return err;

    assert( m_bRawBitplanes == true);

    //raw bitplane for DIRECT MB type
    err = pCodedPicture->PutBits(0,5);
    if (err != UMC::UMC_OK)
       return err;

    // raw bitplane for skip MB
    err = pCodedPicture->PutBits(0,5);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiMVTab,2);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiCBPTab,2);
    if (err != UMC::UMC_OK)
       return err;

    switch (m_pSequenceHeader->GetDQuant())
    {
    case 2:
        if (diff>=0 && diff<7)
        {
            err = pCodedPicture->PutBits(diff,3);
        }
        else
        {
            err = pCodedPicture->PutBits((7<<5)+ m_uiAltPQuant,3+5);
        }
        if (err != UMC::UMC_OK)
            return err;
        break;
    case 1:
        err = pCodedPicture->PutBits( m_QuantMode != VC1_ENC_QUANT_SINGLE, 1);
        if (err != UMC::UMC_OK)
           return err;
        if (m_QuantMode != VC1_ENC_QUANT_SINGLE)
        {
           err =  pCodedPicture->PutBits(QuantProfileTableVLC[2*m_QuantMode], QuantProfileTableVLC[2*m_QuantMode+1]);
           if (err != UMC::UMC_OK)
                return err;
           if (m_QuantMode != VC1_ENC_QUANT_MB_ANY)
           {
                if (diff>=0 && diff<7)
                {
                    err = pCodedPicture->PutBits(diff,3);
                }
                else
                {
                    err = pCodedPicture->PutBits((7<<5)+ m_uiAltPQuant,3+5);
                }
                if (err != UMC::UMC_OK)
                    return err;
           }
        }
        break;
    default:
        break;
    }


    if (m_pSequenceHeader->IsVSTransform())
    {
        err = pCodedPicture->PutBits(m_bVSTransform,1);
        if (err != UMC::UMC_OK)
            return err;
        if (m_bVSTransform)
        {
            err = pCodedPicture->PutBits(m_uiTransformType,2);
            if (err != UMC::UMC_OK)
                return err;
        }
    }
    err = pCodedPicture->PutBits(ACTableCodesVLC[2*m_uiDecTypeAC1],ACTableCodesVLC[2*m_uiDecTypeAC1+1]);
    if (err != UMC::UMC_OK)
       return err;

    err = pCodedPicture->PutBits(m_uiDecTypeDCIntra,1);
    return err;
}


UMC::Status   VC1EncoderPictureSM::WriteMBQuantParameter(VC1EncoderBitStreamSM* pCodedPicture, Ipp8u Quant)
{
   UMC::Status err = UMC::UMC_OK;

   if (m_QuantMode == VC1_ENC_QUANT_MB_ANY)
   {
       Ipp16s diff = Quant - m_uiQuant;
       if (diff < 7 && diff>=0)
       {
            err = pCodedPicture->PutBits(diff,3);
       }
       else
       {
            err = pCodedPicture->PutBits((7<<5)+ Quant,3+5);
       }

   }
   else if (m_QuantMode ==VC1_ENC_QUANT_MB_PAIR)
   {
       err = pCodedPicture->PutBits(m_uiAltPQuant == Quant,1);

   }
   return err;
}

UMC::Status VC1EncoderPictureSM::CheckMEMV_P(UMC::MeParams* MEParams, bool bMVHalf)
{
    UMC::Status UmcSts = UMC::UMC_OK;
    Ipp32u i = 0;
    Ipp32u j = 0;
    Ipp32u h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u w = m_pSequenceHeader->GetNumMBInRow();

    sCoordinate         MV          = {0,0};

    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            if(MEParams->pSrc->MBs[j + i*w].MbType != UMC::ME_MbIntra)
            {
                MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0]->x;
                MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0]->y;

                if(bMVHalf)
                {
                    if((MV.x>>1)<<1 != MV.x)
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                    if((MV.y >>1)<<1!= MV.y)
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.x)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.y)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                ////correction MV
                //MV.x  = (bMVHalf)? (MEParams.pSrc->MBs[j + i*w].MVF->x>>1)<<1:MEFrame->MBs[j + i*w].MVF->x;
                //MV.y  = (bMVHalf)? (MEParams.pSrc->MBs[j + i*w].MVF->y>>1)<<1:MEFrame->MBs[j + i*w].MVF->y;

                //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) > (Ipp32s)(w*VC1_ENC_LUMA_SIZE))
                //{
                //    MV.x = (w - j)*VC1_ENC_LUMA_SIZE + (MV.x & 0x03);
                //}
                //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) > (Ipp32s)(h*VC1_ENC_LUMA_SIZE))
                //{
                //    MV.y = (h - i)*VC1_ENC_LUMA_SIZE + (MV.y & 0x03);
                //}
                //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < -16)
                //{
                //    MV.x = (1 - (Ipp32s)j)*VC1_ENC_LUMA_SIZE ;
                //}
                //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < -16)
                //{
                //    MV.y = (1 - (Ipp32s)i)*VC1_ENC_LUMA_SIZE ;
                //}
                if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }
                if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

            }
        }
    }

    return UmcSts;
}

UMC::Status VC1EncoderPictureSM::CheckMEMV_P_MIXED(UMC::MeParams* MEParams, bool bMVHalf)
{
    UMC::Status UmcSts = UMC::UMC_OK;
    Ipp32u i = 0;
    Ipp32u j = 0;
    Ipp32u h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u w = m_pSequenceHeader->GetNumMBInRow();
    Ipp32u blk = 0;

    sCoordinate         MV          = {0,0};

    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            switch (MEParams->pSrc->MBs[j + i*w].MbType)
            {
            case UMC::ME_MbIntra:
                break;
            case UMC::ME_MbFrw:
            case UMC::ME_MbFrwSkipped:

                MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0]->x;
                MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0]->y;

                if(bMVHalf)
                {
                    if((MV.x>>1)<<1 != MV.x)
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                    if((MV.y >>1)<<1!= MV.y)
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }
                if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_CHROMA_SIZE > MEParams->PicRange.bottom_right.x)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_CHROMA_SIZE > MEParams->PicRange.bottom_right.y)
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                //MV correction
                //MV.x  = (bMVHalf)? (MV.x>>1)<<1:MV.x;
                //MV.y  = (bMVHalf)? (MV.y>>1)<<1:MV.y;

                //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) > (Ipp32s)(w*VC1_ENC_LUMA_SIZE))
                //{
                //    MV.x = (w - j)*VC1_ENC_LUMA_SIZE + (MV.x & 0x03);
                //}
                //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) > (Ipp32s)(h*VC1_ENC_LUMA_SIZE))
                //{
                //    MV.y = (h - i)*VC1_ENC_LUMA_SIZE + (MV.y & 0x03);
                //}
                //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < -8)
                //{
                //    MV.x = (1 - (Ipp32s)j)*VC1_ENC_LUMA_SIZE ;
                //}
                //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < -8)
                //{
                //    MV.y = (1 - (Ipp32s)i)*VC1_ENC_LUMA_SIZE ;
                //}
                if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }
                if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                {
                    assert(0);
                    UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                }

                break;
            case UMC::ME_MbMixed:
                for (blk = 0; blk<4; blk++)
                {
                    MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0][blk].x;
                    MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0][blk].y;

                    if(bMVHalf)
                    {
                        if((MV.x>>1)<<1 != MV.x)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                        if((MV.y >>1)<<1!= MV.y)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_CHROMA_SIZE > MEParams->PicRange.bottom_right.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_CHROMA_SIZE > MEParams->PicRange.bottom_right.y)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    //MV correction
                    //MV.x  = (bMVHalf)? (MV.x>>1)<<1:MV.x;
                    //MV.y  = (bMVHalf)? (MV.y>>1)<<1:MV.y;

                    //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) > (Ipp32s)(w*VC1_ENC_LUMA_SIZE))
                    //{
                    //    MV.x = (w - j)*VC1_ENC_LUMA_SIZE + (MV.x & 0x03);
                    //}
                    //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) > (Ipp32s)(h*VC1_ENC_LUMA_SIZE))
                    //{
                    //    MV.y = (h - i)*VC1_ENC_LUMA_SIZE + (MV.y & 0x03);
                    //}
                    //if ((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < -8)
                    //{
                    //    MV.x = (1 - (Ipp32s)j)*VC1_ENC_LUMA_SIZE ;
                    //}
                    //if ((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < -8)
                    //{
                    //    MV.y = (1 - (Ipp32s)i)*VC1_ENC_LUMA_SIZE ;
                    //}
                    if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                    if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                }
                break;
            default:
                assert(0);
                return UMC::UMC_ERR_FAILED;
            }

        }
    }
return UmcSts;
}
UMC::Status VC1EncoderPictureSM::CheckMEMV_B(UMC::MeParams* MEParams, bool bMVHalf)
{
    UMC::Status UmcSts = UMC::UMC_OK;
    Ipp32u i = 0;
    Ipp32u j = 0;
    Ipp32u h = m_pSequenceHeader->GetNumMBInCol();
    Ipp32u w = m_pSequenceHeader->GetNumMBInRow();

    sCoordinate         MV          = {0,0};

    //sCoordinate                 MVMin = {-16*4,-16*4};
    //sCoordinate                 MVMax = {w*16*4,h*16*4};

    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            switch (MEParams->pSrc->MBs[j + i*w].MbType)
            {
                case UMC::ME_MbIntra:
                case UMC::ME_MbDirect:
                case UMC::ME_MbDirectSkipped:
                    break;
                case UMC::ME_MbFrw:
                case UMC::ME_MbFrwSkipped:
                    MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0]->x;
                    MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0]->y;

                    if(bMVHalf)
                    {
                        if((MV.x>>1)<<1 != MV.x)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                        if((MV.y >>1)<<1!= MV.y)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.y)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                   //correction MV
                    //ScalePredict(&MV, j*16*4,i*16*4,MVMin,MVMax);
                    if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                    if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    break;
                case UMC::ME_MbBkw:
                case UMC::ME_MbBkwSkipped:
                    MV.x  = MEParams->pSrc->MBs[j + i*w].MV[1]->x;
                    MV.y  = MEParams->pSrc->MBs[j + i*w].MV[1]->y;

                    if(bMVHalf)
                    {
                        if((MV.x>>1)<<1 != MV.x)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                        if((MV.y >>1)<<1!= MV.y)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.y)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                   //correction MV
                    //ScalePredict(&MV, j*16*4,i*16*4,MVMin,MVMax);
                    if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                    if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    break;
                case UMC::ME_MbBidir:
                case UMC::ME_MbBidirSkipped:
                    //forward
                    MV.x  = MEParams->pSrc->MBs[j + i*w].MV[0]->x;
                    MV.y  = MEParams->pSrc->MBs[j + i*w].MV[0]->y;

                    if(bMVHalf)
                    {
                        if((MV.x>>1)<<1 != MV.x)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                        if((MV.y >>1)<<1!= MV.y)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.y)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    //correction MV
                    //ScalePredict(&MV, j*16*4,i*16*4,MVMin,MVMax);

                    //backward
                    if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                    if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    MV.x  = MEParams->pSrc->MBs[j + i*w].MV[1]->x;
                    MV.y  = MEParams->pSrc->MBs[j + i*w].MV[1]->y;

                    if(bMVHalf)
                    {
                        if((MV.x>>1)<<1 != MV.x)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;

                        if((MV.y >>1)<<1!= MV.y)
                            UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) < MEParams->PicRange.top_left.y )
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.x>>2)+(Ipp32s)(j*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.x)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    if((MV.y>>2)+(Ipp32s)(i*VC1_ENC_LUMA_SIZE) + VC1_ENC_LUMA_SIZE > MEParams->PicRange.bottom_right.y)
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    //correction MV
                    //ScalePredict(&MV, j*16*4,i*16*4,MVMin,MVMax);
                    if (MV.x >(MEParams->SearchRange.x<<2) || MV.x <(-(MEParams->SearchRange.x<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }
                    if (MV.y >(MEParams->SearchRange.y<<2) || MV.y <(-(MEParams->SearchRange.y<<2)))
                    {
                        assert(0);
                        UmcSts = UMC::UMC_ERR_INVALID_PARAMS;
                    }

                    break;
                default:
                    assert(0);
                    break;

            }
        }
    }

    return UmcSts;
}

#ifdef VC1_ME_MB_STATICTICS
void VC1EncoderPictureSM::SetMEStat(UMC::MeMbStat*   stat)
{
    m_MECurMbStat = stat;
}
#endif
}

#endif //defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
