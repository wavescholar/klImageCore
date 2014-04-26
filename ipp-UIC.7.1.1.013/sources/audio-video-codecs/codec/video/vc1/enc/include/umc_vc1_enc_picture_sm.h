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

#ifndef _ENCODER_VC1_PICTURE_SM_H_
#define _ENCODER_VC1_PICTURE_SM_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_enc_sequence_sm.h"
#include "umc_vc1_enc_mb.h"
#include "umc_me.h"
#include "umc_vme.h"
#include "umc_vc1_enc_planes.h"
#include "umc_vc1_enc_bit_rate_control.h"


namespace UMC_VC1_ENCODER
{

class VC1EncoderPictureSM
{
private:

/*All picture types*/
    VC1EncoderSequenceSM*       m_pSequenceHeader;
    ePType                      m_uiPictureType;
    bool                        m_bFrameInterpolation;
    bool                        m_bRangeRedution;
    Ipp8u                       m_uiFrameCount;                 // [0-3] in the coded mode
    Ipp8u                       m_uiQuantIndex;                 // [1, 31]
    bool                        m_bHalfQuant;                   // can be true if  m_uiQuantIndex<8
    bool                        m_bUniformQuant;
    Ipp8u                       m_uiMVRangeIndex;               //[0,3] - 64x32, 128x64, 512x128, 1024x256
    Ipp8u                       m_uiResolution;                 //[0,3] - full, half (horizontal, vertical scale)
    Ipp8u                       m_uiDecTypeAC1;                 //[0,2] - it's used to choose decoding table
    Ipp8u                       m_uiDecTypeDCIntra;             //[0,1] - it's used to choose decoding table

    /* ------------------------------I picture ---------------- */

    Ipp8u                       m_uiDecTypeAC2;                 //[0,2] - it's used to choose decoding table
    sFraction                   m_uiBFraction;                  // 0xff/oxff - BI frame

    /* ------------------------------P picture----------------- */

    eMVModes                    m_uiMVMode;                     //    VC1_ENC_1MV_HALF_BILINEAR
                                                                //    VC1_ENC_1MV_QUARTER_BICUBIC
                                                                //    VC1_ENC_1MV_HALF_BICUBIC
                                                                //    VC1_ENC_MIXED_QUARTER_BICUBIC

    bool                        m_bIntensity;                   // only for main profile
    Ipp8u                       m_uiIntensityLumaScale;         // [0,63], if m_bIntensity
    Ipp8u                       m_uiIntensityLumaShift;         // [0,63], if m_bIntensity
    Ipp8u                       m_uiMVTab;                      // [0,4] - table for MV coding
    Ipp8u                       m_uiCBPTab;                     // [0,4] - table for CBP coding
    Ipp8u                       m_uiAltPQuant;                  // if picture DQuant!=0
    eQuantMode                  m_QuantMode;                    // VC1_ENC_QUANT_SINGLE, if   picture DQuant==0
    bool                        m_bVSTransform;                 // if sequence(m_bVSTransform) variable-size transform
    eTransformType              m_uiTransformType;              // VC1_ENC_8x8_TRANSFORM, if  m_bVSTransform == false

private:
    Ipp8u*                      m_pPlane[3];
    Ipp32u                      m_uiPlaneStep[3];

    Ipp8u*                      m_pRaisedPlane[3];
    Ipp32u                      m_uiRaisedPlaneStep[3];

    Ipp8u*                      m_pForwardPlane[3];
    Ipp32u                      m_uiForwardPlaneStep[3];

    Ipp8u*                      m_pBackwardPlane[3];
    Ipp32u                      m_uiBackwardPlaneStep[3];

    Ipp16s*                     m_pSavedMV;

    VC1EncoderMBs*              m_pMBs;
    VC1EncoderCodedMB*          m_pCodedMB;

    Ipp8u                       m_uiQuant;
    bool                        m_bRawBitplanes;
    Ipp8u                       m_uiRoundControl;                // 0,1;

#ifdef VC1_ME_MB_STATICTICS
    UMC::MeMbStat* m_MECurMbStat;
#endif

protected:
    void        Reset()
    {
        m_pSequenceHeader       = 0;
        m_uiPictureType         = VC1_ENC_I_FRAME;
        m_bFrameInterpolation   = 0;
        m_bRangeRedution        = 0;
        m_uiFrameCount          = 0;
        m_uiQuantIndex          = 0;
        m_bHalfQuant            = 0;
        m_bUniformQuant         = 0;
        m_uiMVRangeIndex        = 0;
        m_uiResolution          = 0;
        m_uiDecTypeAC1          = 0;
        m_uiDecTypeAC2          = 0;
        m_uiDecTypeDCIntra      = 0;
        m_uiQuant               = 0;
        m_pMBs                  = 0;
        m_uiBFraction.num       = 1;
        m_uiBFraction.denom     = 2;
        m_uiMVMode              = VC1_ENC_1MV_HALF_BILINEAR;
        m_bIntensity            = false;
        m_uiIntensityLumaScale  = 0;
        m_uiIntensityLumaShift  = 0;
        m_uiMVTab               = 0;
        m_uiCBPTab              = 0;
        m_uiAltPQuant           = 0;
        m_QuantMode             = VC1_ENC_QUANT_SINGLE;
        m_bVSTransform          = false;
        m_uiTransformType       = VC1_ENC_8x8_TRANSFORM;
        m_bRawBitplanes         = true;
        m_pSavedMV              = 0;
        m_uiRoundControl        = 0;
        m_pCodedMB              = 0;

#ifdef VC1_ME_MB_STATICTICS
        m_MECurMbStat           = NULL;
#endif
    }
    void        ResetPlanes()
    {
       m_pPlane[0]                 = 0;
       m_pPlane[1]                 = 0;
       m_pPlane[2]                 = 0;
       m_uiPlaneStep[0]            = 0;
       m_uiPlaneStep[1]            = 0;
       m_uiPlaneStep[2]            = 0;

       m_pRaisedPlane[0]           = 0;
       m_pRaisedPlane[1]           = 0;
       m_pRaisedPlane[2]           = 0;
       m_uiRaisedPlaneStep[0]      = 0;
       m_uiRaisedPlaneStep[1]      = 0;
       m_uiRaisedPlaneStep[2]      = 0;

       m_pForwardPlane[0]          = 0;
       m_pForwardPlane[1]          = 0;
       m_pForwardPlane[2]          = 0;
       m_uiForwardPlaneStep[0]     = 0;
       m_uiForwardPlaneStep[1]     = 0;
       m_uiForwardPlaneStep[2]     = 0;

       m_pBackwardPlane[0]          = 0;
       m_pBackwardPlane[1]          = 0;
       m_pBackwardPlane[2]          = 0;
       m_uiBackwardPlaneStep[0]     = 0;
       m_uiBackwardPlaneStep[1]     = 0;
       m_uiBackwardPlaneStep[2]     = 0;
    }

    UMC::Status     CheckParametersI(vm_char* pLastError);
    UMC::Status     CheckParametersP(vm_char* pLastError);
    UMC::Status     SetPictureParamsI();
    UMC::Status     SetPictureParamsP();
    UMC::Status     SetPictureParamsB();

    UMC::Status     WriteIPictureHeader(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     WritePPictureHeader(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     WriteBPictureHeader(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     WriteSkipPictureHeader(VC1EncoderBitStreamSM* pCodedPicture);

    UMC::Status     WriteMBQuantParameter(VC1EncoderBitStreamSM* pCodedPicture, Ipp8u Quant);

    UMC::Status     SetInterpolationParams(IppVCInterpolate_8u* pY,
                                           IppVCInterpolate_8u* pU,
                                           IppVCInterpolate_8u* pv,
                                           Ipp8u* buffer,
                                           bool bForward);

    UMC::Status     CheckMEMV_P(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_P_MIXED(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_B(UMC::MeParams* MEParams, bool bMVHalf);


    UMC::Status     SetInterpolationParams4MV(  IppVCInterpolate_8u* pY,
                                                IppVCInterpolate_8u* pU,
                                                IppVCInterpolate_8u* pv,
                                                Ipp8u* buffer,
                                                bool bForward);

public:
    VC1EncoderPictureSM()
    {
        Reset();
        ResetPlanes();
    }

    UMC::Status     SetInitPictureParams(InitPictureParams* InitPicParam);
    UMC::Status     SetVLCTablesIndex(VLCTablesIndex*       VLCIndex);

    UMC::Status     SetPictureParams( ePType pType, Ipp16s* pSavedMV);
    UMC::Status     SetPlaneParams  ( Frame* pFrame, ePlaneType type);

    UMC::Status     Init(VC1EncoderSequenceSM * SequenceHeader, VC1EncoderMBs* pMBs, VC1EncoderCodedMB* pCodedMB);

    UMC::Status     Close();


    UMC::Status     CompletePicture(VC1EncoderBitStreamSM* pCodedPicture, double dPTS, size_t len);
    UMC::Status     CheckParameters(vm_char* pLastError);

    void SetRoundControl(Ipp8u roundControl)
    {
        m_uiRoundControl = roundControl;
    }

    UMC::Status     VLC_IFrame(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     PAC_IFrame(UMC::MeParams* /*MEParams*/);

    UMC::Status     VLC_PFrame(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     PAC_PFrame(UMC::MeParams* MEParams);

    UMC::Status     VLC_PFrameMixed(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     PAC_PFrameMixed(UMC::MeParams* MEParams);

    UMC::Status     VLC_BFrame(VC1EncoderBitStreamSM* pCodedPicture);
    UMC::Status     PAC_BFrame(UMC::MeParams* MEParams);


    UMC::Status     VLC_SkipFrame(VC1EncoderBitStreamSM* pCodedPicture)
    {
        return WriteSkipPictureHeader(pCodedPicture);
    };

    UMC::Status SetPictureQuantParams(Ipp8u uiQuantIndex, bool bHalfQuant);
#ifdef VC1_ME_MB_STATICTICS
    void SetMEStat(UMC::MeMbStat*   stat);
#endif
};

}

#endif
