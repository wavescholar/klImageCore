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

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_sequence_sm.h"
namespace UMC_VC1_ENCODER
{
UMC::Status VC1EncoderSequenceSM::WriteSeqHeader(VC1EncoderBitStreamSM* pCodedSH)
{
    UMC::Status     ret     =   UMC::UMC_OK;
    //Ipp32u          temp    =   0;

    if (!pCodedSH)
        return UMC::UMC_ERR_NULL_PTR;


    ret = pCodedSH->PutBitsHeader(0xC5,                   8);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBitsHeader(m_uiNumberOfFrames ,   24);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBitsHeader(0x00000004,            32);
    if (ret != UMC::UMC_OK) return ret;


    /* STRUCT_C */

    ret = pCodedSH->PutBits(m_uiProfile           , 4);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_uiFrameRateQ        , 3);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_uiBitRateQ          , 5);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bLoopFilter         , 1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(0                     , 1);  /* reserved */
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bMultiRes           , 1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(1                      ,1); /* reserved */
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bFastUVMC            ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bExtendedMV          ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_uiDQuant             ,2);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bVSTransform         ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(0                      ,1);  /* reserved */
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bOverlap             ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bSyncMarkers         ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bRangeRedution       ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_uiMaxBFrames         ,3);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_uiQuantazer          ,2);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(m_bFrameInterpolation  ,1);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBits(1                      ,1); /* reserved */
    if (ret != UMC::UMC_OK) return ret;

    /* STRUCT_A */

    ret = pCodedSH->PutBitsHeader(m_uiPictureHeight  ,     32);
    if (ret != UMC::UMC_OK) return ret;

    ret = pCodedSH->PutBitsHeader(m_uiPictureWidth  ,     32);
    if (ret != UMC::UMC_OK) return ret;

    /* ... */

    ret = pCodedSH->PutBitsHeader(0x0000000C,            32);
    if (ret != UMC::UMC_OK) return ret;

    /* STRUCT_B */

    ret = pCodedSH->PutBits(m_uiLevel,             3);
    if (ret != UMC::UMC_OK) return ret;
    ret = pCodedSH->PutBits(m_bConstBitRate,       1);
    if (ret != UMC::UMC_OK) return ret;
    ret = pCodedSH->PutBits(0,                     4);
    if (ret != UMC::UMC_OK) return ret;
    ret = pCodedSH->PutBits(m_uiHRDBufferSize,    24);
    if (ret != UMC::UMC_OK) return ret;
    ret = pCodedSH->PutBits(m_uiHRDFrameRate,     32);
    if (ret != UMC::UMC_OK) return ret;
    ret = pCodedSH->PutBits(m_uiFrameRate,        32);
    if (ret != UMC::UMC_OK) return ret;

    return ret;
}

UMC::Status VC1EncoderSequenceSM::CheckParameters(vm_char* pLastError)
{
    UMC::Status ret = UMC::UMC_OK;
    if (m_uiProfile != VC1_ENC_PROFILE_S && m_uiProfile != VC1_ENC_PROFILE_M)
    {
        vm_string_sprintf(pLastError,VM_STRING("Error: seq. header parameter: profile\n"));
        return UMC::UMC_ERR_NOT_INITIALIZED;
    }
    if (!m_uiPictureWidth || !m_uiPictureHeight)
    {
        vm_string_sprintf(pLastError,VM_STRING("Error: seq. header parameter: %d x %d\n"), m_uiPictureWidth, m_uiPictureHeight);
        return UMC::UMC_ERR_NOT_INITIALIZED;
    }
    if (!m_uiNumberOfFrames || m_uiNumberOfFrames > 16777214)
    {
        m_uiNumberOfFrames = 0x00ffffff;
    }
    if (m_uiFrameRateQ >7)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: FrameRateQ (%d)> 7\n"),m_uiFrameRateQ);
        m_uiFrameRateQ = 7;
    }
    if (m_uiBitRateQ >31)
    {
        vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: BitRateQ  (%d)> 31\n"),m_uiBitRateQ);
        m_uiBitRateQ = 31;
    }
    if (m_uiProfile == VC1_ENC_PROFILE_S )
    {
        if (m_bLoopFilter == true)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: loop filter should be equal to 0 in the simple profile \n"));
            m_bLoopFilter = false;
        }
        if (m_bFastUVMC == false)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: fast UV MC should be equal to 1 in the simple profile \n"));
            m_bFastUVMC = true;
        }
        if (m_bExtendedMV == true)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: extended MV should be equal to 0 in the simple profile \n"));
            m_bExtendedMV = false;
        }
        if (m_uiDQuant!=0)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: DQuant should be equal to 0 in the simple profile \n"));
            m_uiDQuant = 0;
        }
        if (m_bSyncMarkers == true)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: SyncMarker should be equal to 0 in the simple profile \n"));
            m_bSyncMarkers = false;
        }
        if (m_bRangeRedution == true)
        {
            vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: RangeRedution should be equal to 0 in the simple profile \n"));
            m_bRangeRedution= false;
        }
    }
    if (m_uiDQuant>2)
    {
          vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: m_uiDQuant (%d) >2 \n"),m_uiDQuant);
          m_uiDQuant = 2;
    }
    if (m_uiDQuant>0 && m_bMultiRes)
    {
          vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: m_uiDQuant should be equal to 0 in the case of multi resolution \n"));
          m_uiDQuant= 0;
    }
    if (m_uiMaxBFrames > 7)
    {
       vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: MaxBFrames (%d) > 7\n"),m_uiMaxBFrames);
       m_uiMaxBFrames = 7;
    }
    if (m_uiQuantazer!= VC1_ENC_QTYPE_IMPL &&
        m_uiQuantazer!= VC1_ENC_QTYPE_EXPL &&
        m_uiQuantazer!= VC1_ENC_QTYPE_UF   &&
        m_uiQuantazer!= VC1_ENC_QTYPE_NUF)
    {
       vm_string_sprintf(pLastError,VM_STRING("Warning: seq. header parameter: Quantazer (%d) > 7\n"),m_uiQuantazer);
       m_uiQuantazer = VC1_ENC_QTYPE_UF;
    }
    if (m_uiLevel !=VC1_ENC_LEVEL_S && m_uiLevel !=VC1_ENC_LEVEL_M && m_uiLevel !=VC1_ENC_LEVEL_H)
    {
        vm_string_sprintf(pLastError,VM_STRING("Error: seq. header parameter: level\n"));
        return UMC::UMC_ERR_NOT_INITIALIZED;
    }
    if ( m_uiPictureWidth %2 != 0 || m_uiPictureHeight%2 != 0 )
    {
        vm_string_sprintf(pLastError,VM_STRING("Error: seq. header parameter: picture size\n"));
        return UMC::UMC_ERR_NOT_INITIALIZED;
    }
    return ret;

}
UMC::Status  VC1EncoderSequenceSM::Init(UMC::VC1EncoderParams* pParams)
{
    if (!pParams)
        return UMC::UMC_ERR_NULL_PTR;

    m_uiPictureWidth    = pParams->m_info.videoInfo.m_iWidth;
    m_uiPictureHeight   = pParams->m_info.videoInfo.m_iHeight;
    m_uiNumberOfFrames  = pParams->m_iFramesCounter;

    m_bVSTransform      =   pParams->m_bVSTransform;        // variable-size transform


    m_nMBRow = (m_uiPictureWidth  +15)/16;
    m_nMBCol = (m_uiPictureHeight +15)/16;


    m_uiProfile         =   (Ipp8u)pParams->m_info.iProfile;
    m_bLoopFilter       =   pParams->m_bDeblocking;         // Should be equal to 0 in simple profile


    m_uiFrameRateQ      =   0;                  // [0, 7]
    m_uiBitRateQ        =   0;                  // [0,31]
    m_bMultiRes         =   false;              // Multi resolution coding
    m_bOverlap          =   false;
    m_uiMaxBFrames      =   pParams->m_uiBFrmLength;// Number of B frames between I,P frames[0,7]

    switch(pParams->m_uiQuantType)
    {
        case 0:
            m_uiQuantazer = VC1_ENC_QTYPE_IMPL;
            break;
        case 1:
            m_uiQuantazer = VC1_ENC_QTYPE_EXPL;
            break;
        case 2:
            m_uiQuantazer = VC1_ENC_QTYPE_NUF;
            break;
        case 3:
            m_uiQuantazer =   VC1_ENC_QTYPE_UF; // [0,3] - quantizer specification
            break;
        default:
            assert(0);
            break;
    }

    m_bFrameInterpolation=  false;
    m_uiLevel           =   (Ipp8u)pParams->m_info.iLevel;    //VC1_ENC_LEVEL_S, VC1_ENC_LEVEL_M, VC1_ENC_LEVEL_H
    m_bConstBitRate     =   false;
    m_uiHRDBufferSize   =   1;                  // buffersize in milliseconds [1,65536]
    m_uiHRDFrameRate    =   1;                  // rate: bits per seconds [1,65536]
    m_uiFrameRate       =   30;                 // 0xffffffff - if unknown


    m_bFastUVMC         =   pParams->m_bFastUVMC;// Should be equal to 1 in simple profile
    m_bExtendedMV       =   false;              // Should be equal to 0 in simple profile
    m_uiDQuant          =   0;                  // [0] - simple profile,
                                                // [0,1,2] - main profile (if m_bMultiRes then only 0)
    m_bSyncMarkers      =   false;              // Should be equal to 0 in simple profile
    m_bRangeRedution    =   false;              // Should be equal to 0 in simple profile

    return UMC::UMC_OK;
}

}

#endif //defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
