/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, advance profile picture level functionality
//
*/

#ifndef _ENCODER_VC1_PICTURE_ADV_H_
#define _ENCODER_VC1_PICTURE_ADV_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_enc_sequence_adv.h"
#include "umc_vc1_enc_mb.h"
#include "umc_me.h"
#include "umc_vme.h"
#include "umc_vc1_enc_planes.h"
#include "umc_vc1_enc_bit_rate_control.h"


namespace UMC_VC1_ENCODER
{

class VC1EncoderPictureADV
{
private:

/*All picture types*/
    VC1EncoderSequenceADV*      m_pSequenceHeader;
    ePType                      m_uiPictureType;
    bool                        m_bFrameInterpolation;
    Ipp8u                       m_uiQuantIndex;                 // [1, 31]
    bool                        m_bHalfQuant;                   // can be true if  m_uiQuantIndex<8
    bool                        m_bUniformQuant;
    //Ipp8u                       m_uiResolution;               //[0,3] - full, half (horizontal, vertical scale)
    Ipp8u                       m_uiDecTypeAC1;                 //[0,2] - it's used to choose decoding table
    Ipp8u                       m_uiDecTypeDCIntra;             //[0,1] - it's used to choose decoding table

    /* ------------------------------I picture ---------------- */

    Ipp8u                       m_uiDecTypeAC2;                 //[0,2] - it's used to choose decoding table

    /* ------------------------------P picture----------------- */

    eMVModes                    m_uiMVMode;                     //    VC1_ENC_1MV_HALF_BILINEAR
                                                                //    VC1_ENC_1MV_QUARTER_BICUBIC
                                                                //    VC1_ENC_1MV_HALF_BICUBIC
                                                                //    VC1_ENC_MIXED_QUARTER_BICUBIC

    bool                        m_bIntensity;                   // only for progressive
    Ipp8u                       m_uiIntensityLumaScale;         // prog: [0,63], if m_bIntensity
                                                                // field: for TOP field[0,63] , if  m_uiFieldIntensityType != VC1_ENC_NONE
    Ipp8u                       m_uiIntensityLumaShift;         // prog: [0,63], if m_bIntensity
                                                                // field: for TOP field[0,63] , if  m_uiFieldIntensityType != VC1_ENC_NONE
    Ipp8u                       m_uiMVTab;                      // prog: [0,4] - table for MV coding
                                                                // field:[0,3] - if m_uiReferenceFieldType != VC1_ENC_BOTH_FIELD
                                                                // field:[0,7] - if m_uiReferenceFieldType == VC1_ENC_BOTH_FIELD
    Ipp8u                       m_uiCBPTab;                     // prog:  [0,4] - table for CBP coding
                                                                // field: [0,7]
    Ipp8u                       m_uiAltPQuant;                  // if picture DQuant!=0
    eQuantMode                  m_QuantMode;                    // VC1_ENC_QUANT_SINGLE, if   picture DQuant==0
    bool                        m_bVSTransform;                 // if sequence(m_bVSTransform) variable-size transform
    eTransformType              m_uiTransformType;              // VC1_ENC_8x8_TRANSFORM, if  m_bVSTransform == false

    /*----------------- All frames- adv--------------------------- */
    /*--------------------I frame - adv--------------------------- */

    Ipp8u                       m_uiRoundControl;                // 0,1;
    Ipp8u                       m_uiCondOverlap;                 //VC1_ENC_COND_OVERLAP_NO, VC1_ENC_COND_OVERLAP_SOME, VC1_ENC_COND_OVERLAP_ALL

    /*--------------------P frame - adv--------------------------- */
    Ipp8u                       m_uiMVRangeIndex;                //progr:  [0,3] - 64x32, 128x64, 512x128,  1024x256
                                                                 //fields half: [0,3] - 128x64,256x128,1024x256, 248x512
    /*--------------------B frame - adv--------------------------- */
    sFraction                   m_uiBFraction;                  // 0xff/oxff - BI frame

    /*--------------------Interlace--------------------------------*/
    bool                        m_bUVSamplingInterlace;           // if false - progressive
    Ipp8u                       m_nReferenceFrameDist;            // is used only in I/I, I/P, P/I, P/P
    eReferenceFieldType         m_uiReferenceFieldType;           // VC1_ENC_REF_FIELD_FIRST
                                                                  // VC1_ENC_REF_FIELD_SECOND
                                                                  // VC1_ENC_REF_FIELD_BOTH


    eFieldType                  m_uiFieldIntensityType;          // intensity compensation for field picture
                                                                 // VC1_ENC_FIELD_NONE
                                                                 // VC1_ENC_TOP_FIELD
                                                                 // VC1_ENC_BOTTOM_FIELD
                                                                 // VC1_ENC_BOTH_FIELD

    Ipp8u                       m_uiIntensityLumaScaleB;         // [0,63], for bottom field
    Ipp8u                       m_uiIntensityLumaShiftB;         // [0,63], for bottom field

    Ipp8u                       m_uiDMVRangeIndex;               // [0,3] - 64x32, 128x64, 512x128, 1024x256
    Ipp8u                       m_uiMBModeTab;                   // [0,7]
    Ipp8u                       m_ui4MVCBPTab;                   // [0,3]

    bool                        m_bTopFieldFirst;
    bool                        m_bRepeateFirstField;
private:
    Ipp8u*                      m_pPlane[3];
    Ipp32u                      m_uiPlaneStep[3];

    Ipp8u*                      m_pRaisedPlane[3];
    Ipp32u                      m_uiRaisedPlaneStep[3];

    Ipp8u*                      m_pForwardPlane[3];
    Ipp32u                      m_uiForwardPlaneStep[3];

    Ipp8u*                      m_pBackwardPlane[3];
    Ipp32u                      m_uiBackwardPlaneStep[3];

    // those parameters are used for direct MB reconstruction.
    // array of nMB

    Ipp16s*                     m_pSavedMV;
    Ipp8u*                      m_pRefType; //0 - intra, 1 - First field, 2 - Second field

    VC1EncoderMBs*              m_pMBs;
    VC1EncoderCodedMB*          m_pCodedMB;
    Ipp8u                       m_uiQuant;
    bool                        m_bRawBitplanes;


#ifdef VC1_ME_MB_STATICTICS
    UMC::MeMbStat* m_MECurMbStat;
#endif

protected:
    void        Reset()
    {
        m_pSequenceHeader       = 0;
        m_uiPictureType         = VC1_ENC_I_FRAME;
        m_bFrameInterpolation   = 0;
        //m_bRangeRedution        = 0;
        //m_uiFrameCount          = 0;
        m_uiQuantIndex          = 0;
        m_bHalfQuant            = 0;
        m_bUniformQuant         = 0;
        m_uiMVRangeIndex        = 0;
        m_uiDMVRangeIndex       = 0;
        //m_uiResolution          = 0;
        m_uiDecTypeAC1          = 0;
        m_uiDecTypeAC2          = 0;
        m_uiDecTypeDCIntra      = 0;
        m_uiQuant               = 0;
        //m_pMBs                  = 0;
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
        m_uiCondOverlap         = VC1_ENC_COND_OVERLAP_NO;
        m_pCodedMB              = 0;
        m_bUVSamplingInterlace  = 0;
        m_nReferenceFrameDist   = 0;
        m_uiReferenceFieldType  = VC1_ENC_REF_FIELD_FIRST;
        m_uiFieldIntensityType  = VC1_ENC_FIELD_NONE;     // intensity compensation for second reference
                                         // used only if eReferenceFieldType = VC1_ENC_REF_FIELD_BOTH
        m_uiIntensityLumaScaleB = 0;     // [0,63], if m_bIntensity
        m_uiIntensityLumaShiftB = 0;

        m_uiMBModeTab           = 0;
        m_ui4MVCBPTab           = 0;
        m_bTopFieldFirst        = 1;
        m_pRefType              = 0;
        m_bRepeateFirstField    = false;

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
    UMC::Status     CheckParametersIField(vm_char* pLastError);
    UMC::Status     CheckParametersP(vm_char* pLastError);
    UMC::Status     CheckParametersPField(vm_char* pLastError);
    UMC::Status     CheckParametersB(vm_char* pLastError);
    UMC::Status     CheckParametersBField(vm_char* pLastError);

    UMC::Status     SetPictureParamsI();
    UMC::Status     SetPictureParamsP();
    UMC::Status     SetPictureParamsB();
    UMC::Status     SetPictureParamsIField();
    UMC::Status     SetPictureParamsPField();
    UMC::Status     SetPictureParamsBField();

    inline
    UMC::Status     SetReferenceFrameDist(Ipp8u distance)
    {
        m_nReferenceFrameDist = distance;
        return UMC::UMC_OK;
    }
    inline
    UMC::Status     SetUVSamplingInterlace(bool bUVSamplingInterlace)
    {
        m_bUVSamplingInterlace = bUVSamplingInterlace;
        return UMC::UMC_OK;
    }

    UMC::Status     WriteIPictureHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WritePPictureHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WriteBPictureHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WriteSkipPictureHeader(VC1EncoderBitStreamAdv* pCodedPicture);

    UMC::Status     WriteFieldPictureHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WriteIFieldHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WritePFieldHeader(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     WriteBFieldHeader(VC1EncoderBitStreamAdv* pCodedPicture);

    UMC::Status     WriteMBQuantParameter(VC1EncoderBitStreamAdv* pCodedPicture, Ipp8u Quant);

    UMC::Status     SetInterpolationParams(IppVCInterpolate_8u* pY,
                                           IppVCInterpolate_8u* pU,
                                           IppVCInterpolate_8u* pv,
                                           Ipp8u* buffer,
                                           bool bForward,
                                           bool bField = false);


    UMC::Status     CheckMEMV_P(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_P_MIXED(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_B(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_PField(UMC::MeParams* MEParams, bool bMVHalf);
    UMC::Status     CheckMEMV_BField(UMC::MeParams* MEParams, bool bMVHalf);


    UMC::Status     SetInterpolationParams4MV(  IppVCInterpolate_8u* pY,
                                                IppVCInterpolate_8u* pU,
                                                IppVCInterpolate_8u* pv,
                                                Ipp8u* buffer,
                                                bool bForward);

public:
    VC1EncoderPictureADV()
    {
        Reset();
        ResetPlanes();
    }

    UMC::Status     SetInitPictureParams(InitPictureParams* InitPicParam);
    UMC::Status     SetVLCTablesIndex(VLCTablesIndex*       VLCIndex);

    UMC::Status     SetPictureParams( ePType pType, Ipp16s* pSavedMV, Ipp8u* pRefType, bool bSecondField);
    UMC::Status     SetPlaneParams  ( Frame* pFrame, ePlaneType type );

    UMC::Status     Init(VC1EncoderSequenceADV* SequenceHeader, VC1EncoderMBs* pMBs, VC1EncoderCodedMB* pCodedMB);

    UMC::Status     Close();


    UMC::Status     CompletePicture(VC1EncoderBitStreamAdv* pCodedPicture, double dPTS, size_t len);
    UMC::Status     CheckParameters(vm_char* pLastError, bool bSecondField);

    UMC::Status     PACIField(UMC::MeParams* /*MEParams*/,bool bSecondField);

    UMC::Status     VLC_IFrame(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     PAC_IFrame(UMC::MeParams* /*MEParams*/);

    UMC::Status     VLCIField(VC1EncoderBitStreamAdv* pCodedPicture, bool bSecondField);
    UMC::Status     VLC_PFrame(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     PAC_PFrame(UMC::MeParams* MEParams);

    UMC::Status     VLC_PFrameMixed(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     PAC_PFrameMixed(UMC::MeParams* MEParams);

    UMC::Status     VLC_BFrame(VC1EncoderBitStreamAdv* pCodedPicture);
    UMC::Status     PAC_BFrame(UMC::MeParams* MEParams);
    UMC::Status     VLC_I_FieldPic(VC1EncoderBitStreamAdv* pCodedPicture, bool bSecondField)
    {
        UMC::Status err = UMC::UMC_OK;

        if(!bSecondField)
        {
            err = WriteFieldPictureHeader(pCodedPicture);
            if (err != UMC::UMC_OK) return err;
            err = WriteIFieldHeader(pCodedPicture);
            if (err != UMC::UMC_OK) return err;
            err = VLCIField(pCodedPicture, false);
        }
        else
        {
            err = pCodedPicture->PutStartCode(0x0000010C);
            if (err != UMC::UMC_OK) return err;
            err = WriteIFieldHeader(pCodedPicture);
            if (err != UMC::UMC_OK) return err;
            err = VLCIField(pCodedPicture, true);
        }

        return err;
    }

    UMC::Status     VLC_P_FieldPic(VC1EncoderBitStreamAdv* pCodedPicture,
                                   bool bSecondField)
    {
        UMC::Status err = UMC::UMC_OK;

        if(!bSecondField)
        {
            err = WriteFieldPictureHeader(pCodedPicture);
            if (err != UMC::UMC_OK) return err;

            if(m_uiReferenceFieldType != VC1_ENC_REF_FIELD_BOTH)
                err = VLC_PField1Ref(pCodedPicture, bSecondField);
            else
                err = VLC_PField2Ref(pCodedPicture, bSecondField);
        }
        else
        {
            err = pCodedPicture->PutStartCode(0x0000010C);
            if (err != UMC::UMC_OK) return err;
            if(m_uiReferenceFieldType != VC1_ENC_REF_FIELD_BOTH)
                err = VLC_PField1Ref(pCodedPicture, bSecondField);
            else
                err = VLC_PField2Ref(pCodedPicture, bSecondField);
        }

        return err;
    }

    UMC::Status     VLC_B_FieldPic(VC1EncoderBitStreamAdv* pCodedPicture,
                                   bool bSecondField)
    {
        UMC::Status err = UMC::UMC_OK;

        if(!bSecondField)
        {
            err = WriteFieldPictureHeader(pCodedPicture);
            if (err != UMC::UMC_OK) return err;

            err = VLC_BField(pCodedPicture, bSecondField);
        }
        else
        {
            err = pCodedPicture->PutStartCode(0x0000010C);
            if (err != UMC::UMC_OK) return err;

            err = VLC_BField(pCodedPicture, bSecondField);
        }

        return err;
    }

    UMC::Status     VLC_SkipFrame(VC1EncoderBitStreamAdv* pCodedPicture)
    {
        return WriteSkipPictureHeader(pCodedPicture);
    };

    UMC::Status     PACPField(UMC::MeParams* MEParams, bool bSecondField)
    {
        UMC::Status err = UMC::UMC_OK;
        if(m_uiReferenceFieldType != VC1_ENC_REF_FIELD_BOTH)
            err = PAC_PField1Ref(MEParams,bSecondField);
        else
            err = PAC_PField2Ref(MEParams,bSecondField);

        return err;
    }

    UMC::Status     PACBField(UMC::MeParams* MEParams, bool bSecondField)
    {
        UMC::Status err = UMC::UMC_OK;
        err = PAC_BField(MEParams,bSecondField);

        return err;
    }

    UMC::Status     PAC_PField1Ref(UMC::MeParams* MEParams,bool bSecondField);
    UMC::Status     PAC_PField2Ref(UMC::MeParams* MEParams,bool bSecondField);
    UMC::Status     VLC_PField1Ref(VC1EncoderBitStreamAdv* pCodedPicture,bool bSecondField);
    UMC::Status     VLC_PField2Ref(VC1EncoderBitStreamAdv* pCodedPicture,bool bSecondField);
    UMC::Status     PAC_BField(UMC::MeParams* MEParams, bool bSecondField);
    UMC::Status     SetPictureQuantParams(Ipp8u uiQuantIndex, bool bHalfQuant);
    UMC::Status     VLC_BField(VC1EncoderBitStreamAdv* pCodedPicture,bool bSecondField);

#ifdef VC1_ME_MB_STATICTICS
    void SetMEStat(UMC::MeMbStat*   stat, bool bSecondField);
#endif
};
}

#endif
