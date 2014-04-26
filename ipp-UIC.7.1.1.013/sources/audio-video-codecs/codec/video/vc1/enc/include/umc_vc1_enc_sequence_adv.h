/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, advance profile sequence header level functionality
//
*/

#ifndef _ENCODER_VC1_SEQUENCE_ADV_H_
#define _ENCODER_VC1_SEQUENCE_ADV_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_video_encoder.h"

namespace UMC_VC1_ENCODER
{

enum
{
    VC1_ENC_COND_OVERLAP_NO   = 0,
    VC1_ENC_COND_OVERLAP_SOME = 1,
    VC1_ENC_COND_OVERLAP_ALL  = 2,
};

class VC1EncoderSequenceADV
{
private:
    Ipp32u                 m_uiPictureWidth;
    Ipp32u                 m_uiPictureHeight;

    //------------------ for simple/main profiles:--------------------------------------------
    // Ipp32u                 m_uiNumberOfFrames;  //Number of frames (0xffffff - if unknown)
    // bool                   m_bMultiRes;         // Multi-resolution coding
    // bool                   m_bSyncMarkers;      // Should be equal to 0 in simple profile
    // bool                   m_bRangeRedution;    // Should be equal to 0 in simple profile
    // Ipp8u                  m_uiMaxBFrames;      // Number of B frames between I,P frames[0,7]

    bool                   m_bFrameInterpolation;

    Ipp8u                  m_uiFrameRateQ;      // [0, 7]
    Ipp8u                  m_uiBitRateQ;        // [0,31]
    bool                   m_bLoopFilter;       // Should be equal to 0 in simple profile
    bool                   m_bFastUVMC;         // Should be equal to 1 in simple profile
    bool                   m_bExtendedMV;       // Should be equal to 0 in simple profile
    Ipp8u                  m_uiDQuant;          // [0] - simple profile,
                                                // [0,1,2] - main profile (if m_bMultiRes then only 0)
    bool                   m_bVSTransform;      // variable-size transform
    bool                   m_bOverlap;

    eQuantType             m_uiQuantazer;       // [0,3] - quantizer specification


    Ipp8u                  m_uiLevel;           //VC1_ENC_LEVEL_S, VC1_ENC_LEVEL_M, VC1_ENC_LEVEL_H
    bool                   m_bConstBitRate;
    Ipp32u                 m_uiHRDBufferSize;   // buffer size in milliseconds [1,65536]
    Ipp32u                 m_uiHRDFrameRate;    // rate: bits per seconds [1,65536]
    Ipp32u                 m_uiFrameRate;       // 0xffffffff - if unknown

    //-------------------- for advance profile ------------------------------------------------------------

    bool                    m_bClosedEnryPoint ;
    bool                    m_bBrokenLink      ; // if !m_bClosedEnryPoint -> true or false
    bool                    m_bPanScan         ;
    bool                    m_bReferenceFrameDistance;
    bool                    m_bExtendedDMV     ;
    bool                    m_bSizeInEntryPoint;
    Ipp8s                   m_iRangeMapY       ;
    Ipp8s                   m_iRangeMapUV      ;

    bool                    m_bPullDown         ;
    bool                    m_bInterlace        ;
    bool                    m_bFrameCounter     ;
    bool                    m_bDisplayExtention ;
    bool                    m_bHRDParams        ;
    bool                    m_bPostProc         ;

public:
    VC1EncoderSequenceADV()
    {
        m_uiPictureWidth = 0;
        m_uiPictureHeight = 0;
        m_uiFrameRateQ = 0;
        m_uiLevel = 0;
        m_uiBitRateQ = 0,
        m_bLoopFilter = false;
        m_bFastUVMC = false;
        m_bExtendedMV = false;
        m_uiDQuant = 0;
        m_bVSTransform = false;
        m_bOverlap = false;
        m_uiQuantazer = VC1_ENC_QTYPE_UF;
        m_bFrameInterpolation = 0;
        m_bConstBitRate = false;
        m_uiHRDBufferSize = 0;
        m_uiHRDFrameRate = 0;
        m_uiFrameRate = 0;
        m_bClosedEnryPoint = true;
        m_bBrokenLink = false;
        m_bPanScan = false;
        m_bReferenceFrameDistance = false;
        m_bExtendedDMV = false;
        m_bSizeInEntryPoint = false;
        m_iRangeMapY = -1;
        m_iRangeMapUV = -1;
        m_bPullDown = false;
        m_bInterlace = false;
        m_bFrameCounter = false;
        m_bDisplayExtention = false;
        m_bHRDParams = false;
        m_bPostProc  = false;
    }

     bool                IsFrameInterpolation()   {return m_bFrameInterpolation;}
     bool                IsFastUVMC()             {return m_bFastUVMC;}
     bool                IsExtendedMV()           {return m_bExtendedMV;}
     bool                IsLoopFilter()           {return m_bLoopFilter;}
     bool                IsVSTransform()          {return m_bVSTransform;}
     bool                IsOverlap()              {return m_bOverlap;}
     bool                IsInterlace()            {return m_bInterlace;}
     bool                IsPullDown()             {return m_bPullDown;}
     bool                IsPostProc()             {return m_bPostProc;}
     bool                IsFrameCounter()         {return m_bFrameCounter;}
     bool                IsPanScan()              {return m_bPanScan;}
     bool                IsReferenceFrameDistance(){return m_bReferenceFrameDistance;}
     Ipp8u               GetDQuant()              {return m_uiDQuant;}
     bool                IsExtendedDMV()          {return m_bExtendedDMV;}

     Ipp32u              GetPictureWidth()        {return m_uiPictureWidth;}
     Ipp32u              GetPictureHeight()       {return m_uiPictureHeight;}

     eQuantType          GetQuantType()           {return m_uiQuantazer;}

    UMC::Status         WriteSeqHeader(VC1EncoderBitStreamAdv* pCodedSH);
    UMC::Status         CheckParameters(vm_char* pLastError);
    UMC::Status         Init(UMC::VC1EncoderParams* pParams);
    UMC::Status         WriteEntryPoint(VC1EncoderBitStreamAdv* pCodedSH);
    Ipp8u               GetProfile()             {return VC1_ENC_PROFILE_A;}
    Ipp8u               GetLevel()               {return m_uiLevel;}
    void                SetLevel(Ipp8u level)    {m_uiLevel = level;}

};

}

#endif
