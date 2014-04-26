/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, simple profile sequence header level functionality
//
*/

#ifndef _ENCODER_VC1_SEQUENCE_SM_H_
#define _ENCODER_VC1_SEQUENCE_SM_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_video_encoder.h"

namespace UMC_VC1_ENCODER
{

class VC1EncoderSequenceSM
{
private:
    Ipp32u                 m_uiPictureWidth;
    Ipp32u                 m_uiPictureHeight;

    Ipp32u                 m_nMBRow;
    Ipp32u                 m_nMBCol;

    Ipp8u                  m_uiProfile;         //VC1_ENC_PROFILE_S , VC1_ENC_PROFILE_M
    Ipp32u                 m_uiNumberOfFrames;  //Number of frames (0xffffff - if unknown)
    Ipp8u                  m_uiFrameRateQ;      // [0, 7]
    Ipp8u                  m_uiBitRateQ;        // [0,31]
    bool                   m_bLoopFilter;       // Should be equal to 0 in simple profile
    bool                   m_bMultiRes;         // Multi resolution coding
    bool                   m_bFastUVMC;         // Should be equal to 1 in simple profile
    bool                   m_bExtendedMV;       // Should be equal to 0 in simple profile
    Ipp8u                  m_uiDQuant;          // [0] - simple profile,
                                                // [0,1,2] - main profile (if m_bMultiRes then only 0)
    bool                   m_bVSTransform;      // variable-size transform
    bool                   m_bOverlap;
    bool                   m_bSyncMarkers;      // Should be equal to 0 in simple profile
    bool                   m_bRangeRedution;    // Should be equal to 0 in simple profile
    Ipp32u                 m_uiMaxBFrames;      // Number of B frames between I,P frames[0,7]
    eQuantType             m_uiQuantazer;       // [0,3] - quantizer specification
    bool                   m_bFrameInterpolation;

    Ipp8u                  m_uiLevel;           //VC1_ENC_LEVEL_S, VC1_ENC_LEVEL_M, VC1_ENC_LEVEL_H
    bool                   m_bConstBitRate;
    Ipp32u                 m_uiHRDBufferSize;   // buffersize in milliseconds [1,65536]
    Ipp32u                 m_uiHRDFrameRate;    // rate: bits per seconds [1,65536]
    Ipp32u                 m_uiFrameRate;       // 0xffffffff - if unknown
    Ipp8u                  m_uiRoundControl;    // 0,1;


public:
    VC1EncoderSequenceSM()
    {
        m_uiPictureWidth = 0;
        m_uiPictureHeight = 0;
        m_uiProfile  = 0;
        m_uiNumberOfFrames = 0;
        m_uiLevel = 0;
        m_uiFrameRateQ = 0;
        m_uiBitRateQ = 0;
        m_bLoopFilter = false;
        m_bMultiRes = false;
        m_bFastUVMC = false;
        m_bExtendedMV = false;
        m_uiDQuant = 0;
        m_bVSTransform = false;
        m_bOverlap = false;
        m_bSyncMarkers = false;
        m_bRangeRedution = false;
        m_uiMaxBFrames = 0;
        m_uiQuantazer = VC1_ENC_QTYPE_UF;
        m_bFrameInterpolation = 0;
        m_bConstBitRate  = false;
        m_uiHRDBufferSize = 0;
        m_uiHRDFrameRate = 0;
        m_uiFrameRate = 0;
        m_nMBRow = 0;
        m_nMBCol = 0;
        m_uiRoundControl = 0;
    }


    bool                IsFrameInterpolation()  {return m_bFrameInterpolation;}
    bool                IsRangeRedution()       {return m_bRangeRedution;}
    bool                IsFastUVMC()            {return m_bFastUVMC;}
    bool                IsMultiRes()            {return m_bMultiRes;}
    bool                IsBFrames()             {return m_uiMaxBFrames>0;}
    bool                IsExtendedMV()          {return m_bExtendedMV;}
    bool                IsLoopFilter()          {return m_bLoopFilter;}
    bool                IsVSTransform()         {return m_bVSTransform;}
    bool                IsOverlap()             {return m_bOverlap;}
    Ipp8u               GetDQuant()             {return m_uiDQuant;}

    Ipp32u              GetPictureWidth()        {return m_uiPictureWidth;}
    Ipp32u              GetPictureHeight()       {return m_uiPictureHeight;}
    Ipp32u              GetNumMBInRow()          {return m_nMBRow;}
    Ipp32u              GetNumMBInCol()          {return m_nMBCol;}

    eQuantType          GetQuantType()          {return m_uiQuantazer;}

    UMC::Status         WriteSeqHeader(VC1EncoderBitStreamSM* pCodedSH);
    UMC::Status         CheckParameters(vm_char* pLastError);
    UMC::Status         Init(UMC::VC1EncoderParams* pParams);
    Ipp8u               GetProfile()             {return m_uiProfile;}
    Ipp8u               GetLevel()               {return m_uiLevel;}
    void                SetLevel(Ipp8u level)    {m_uiLevel = level;}

    void        SetMaxBFrames(Ipp32u n)
    {
        assert(n<=7);
        m_uiMaxBFrames = n;
    }


};

}

#endif
