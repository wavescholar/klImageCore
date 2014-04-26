/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, VC1 encoder base class
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "vm_debug.h"
#include "vm_time.h"
#include "umc_video_data.h"
#include "umc_vc1_video_encoder.h"
#include "umc_vc1_enc_sm.h"
#include "umc_vc1_enc_adv.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_enc_statistic.h"

using namespace UMC;

VC1VideoEncoder::VC1VideoEncoder()
{
    m_pEncoderSM = 0;
    m_pEncoderADV = 0;
    m_bAdvance = false;
}


VC1VideoEncoder::~VC1VideoEncoder()
{
    Close();
}


Status VC1VideoEncoder::Reset()
{
    Status Sts = UMC_OK;

    if (m_pEncoderSM)
    {
        Sts = ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->Reset();
    }

    if (m_pEncoderADV)
    {
        Sts = ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->Reset();
    }

    return UMC_ERR_NOT_IMPLEMENTED;
}


Status VC1VideoEncoder::SetParams(BaseCodecParams* params)
{
    Status  err = UMC_OK;

    Init(params);

    return err;

}


Status VC1VideoEncoder::Init(BaseCodecParams* init)
{
    Status             err     =    UMC_OK;

    VC1EncoderParams   vc1Params;
    VC1EncoderParams*  params  =    DynamicCast<VC1EncoderParams, BaseCodecParams> (init);

    if (!params)
    {
        VideoEncoderParams*  videoParams  =    DynamicCast<VideoEncoderParams, BaseCodecParams> (init);
        if (!videoParams)
        {
            return UMC_ERR_NULL_PTR;
        }

        params = & vc1Params;
        params->m_info.videoInfo.m_iHeight = videoParams->m_info.videoInfo.m_iHeight;
        params->m_info.videoInfo.m_iWidth  = videoParams->m_info.videoInfo.m_iWidth;
        params->m_info.fFramerate        = videoParams->m_info.fFramerate;
        params->m_info.iBitrate          = videoParams->m_info.iBitrate;
        params->m_iThreads            = videoParams->m_iThreads;
        params->m_bInterlace          = false;
        params->m_info.iProfile       = UMC_VC1_ENCODER::VC1_ENC_PROFILE_A;
        params->m_uiGOPLength         = 12;
        params->m_uiBFrmLength        = 4;
        params->m_bMixed              = false;
    }


    Close();

    //MEMORY ALLOCATOR
    if (UMC_OK != BaseCodec::Init(init) )
    {
        Close();
        return UMC_ERR_INIT;
    }

    m_bAdvance = (params->m_info.iProfile == UMC_VC1_ENCODER::VC1_ENC_PROFILE_A);


    if (!m_bAdvance)
    {
        m_pEncoderSM = new UMC_VC1_ENCODER::VC1EncoderSM;
        if (!m_pEncoderSM)
            return UMC_ERR_ALLOC;
        ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->SetMemoryAllocator(m_pMemoryAllocator, m_bOwnAllocator);
        err = ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->Init(params);
    }
    else
    {
        m_pEncoderADV = new UMC_VC1_ENCODER::VC1EncoderADV;
        if (!m_pEncoderADV)
            return UMC_ERR_ALLOC;
        ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->SetMemoryAllocator(m_pMemoryAllocator, m_bOwnAllocator);
        err = ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->Init(params);
    }

    if (err != UMC_OK)
    {
        Close();
        return err;
    }

#ifdef _PROJECT_STATISTICS_
    TimeStatisticsStructureInitialization();
    if(params->m_pStreamName)
        vm_string_strcpy(m_TStat->streamName,params->m_pStreamName);
    m_TStat->bitrate = params->m_info.bitrate;
    m_TStat->GOPLen = params->m_uiGOPLength;
    m_TStat->BLen   = params->m_uiBFrmLength;
    m_TStat->meSpeedSearch = params->m_uiMESearchSpeed;
#endif

#ifdef _VC1_IPP_STATISTICS_
LibStatisticsStructureInitialization();
    if(params->m_pStreamName)
        vm_string_strcpy(m_LibStat->streamName,params->m_pStreamName);
    m_LibStat->bitrate = params->info.bitrate;
    m_LibStat->GOPLen = params->m_uiGOPLength;
    m_LibStat->BLen   = params->m_uiBFrmLength;
    m_LibStat->meSpeedSearch = params->m_uiMESearchSpeed;
#endif
    return UMC_OK;
}


Status VC1VideoEncoder::GetInfo(BaseCodecParams* info)
{
    Status err = UMC_OK;

    VC1EncoderParams*  params  =    DynamicCast<VC1EncoderParams, BaseCodecParams> (info);

    if(params)
    {
        if (m_pEncoderSM)
        {
            err = ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->GetInfo(params);
        }

        if (m_pEncoderADV)
        {
            err = ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->GetInfo(params);
        }
    }

    else
    {
        VC1EncoderParams   vc1Params;
        if (m_pEncoderSM)
        {
            err = ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->GetInfo(&vc1Params);
        }

        if (m_pEncoderADV)
        {
            err = ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->GetInfo(&vc1Params);
        }

        info->m_iThreads = vc1Params.m_iThreads;
        info->m_iSuggestedOutputSize = vc1Params.m_iSuggestedOutputSize;
        info->m_iSuggestedInputSize  = vc1Params.m_iSuggestedInputSize;
        info->m_pData = vc1Params.m_pData;
        info->m_lpMemoryAllocator = vc1Params.m_lpMemoryAllocator;
    }

    return err;
}


Status VC1VideoEncoder::Close()
{
    if (m_pEncoderSM)
    {
      ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->Close();
       delete ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM);
        m_pEncoderSM = 0;
    }

    if (m_pEncoderADV)
    {
      ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->Close();
       delete ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV);
       m_pEncoderADV = 0;
    }

    BaseCodec::Close(); // delete internal allocator if exists

#ifdef _PROJECT_STATISTICS_
    DeleteStatistics();
#endif

#ifdef _VC1_IPP_STATISTICS_
    DeleteLibStatistics();
#endif

    return UMC_OK;
}


Status VC1VideoEncoder::GetFrame(MediaData* pIn, MediaData* pOut)
{
    static int i = -1;
    i++;
    Status Sts = UMC_ERR_NOT_INITIALIZED;

STATISTICS_START_TIME(m_TStat->startTime);
LIB_STAT_START_TIME(m_LibStat->startTime);
    if (m_pEncoderSM)
        Sts = ((UMC_VC1_ENCODER::VC1EncoderSM*)m_pEncoderSM)->GetFrame(pIn,pOut);
    else if (m_pEncoderADV)
        Sts = ((UMC_VC1_ENCODER::VC1EncoderADV*)m_pEncoderADV)->GetFrame(pIn,pOut);
STATISTICS_END_TIME(m_TStat->startTime, m_TStat->endTime, m_TStat->totalTime);
LIB_STAT_END_TIME(m_LibStat->startTime, m_LibStat->endTime, m_LibStat->totalTime);

#ifdef _PROJECT_STATISTICS_
if(Sts == UMC_ERR_END_OF_STREAM)
    WriteStatisticResults();
else
    m_TStat->frameCount++;
#endif

#ifdef _VC1_IPP_STATISTICS_
if(Sts == UMC_ERR_END_OF_STREAM)
    WriteLibStatisticResults();
else
    m_LibStat->frameCount++;
#endif

   return Sts;
}

Status VC1EncoderParams::ReadParams(ParserCfg *par)
{
    DString sProfile;
    Ipp32u    iTemp;

    UMC_CHECK(par, UMC_ERR_NULL_PTR);
    if(!par->GetNumOfkeys())
        return UMC_ERR_NOT_INITIALIZED;

    par->GetParam(VM_STRING("sProfile"), &sProfile, 1);
    if(!vm_string_stricmp(sProfile, VM_STRING("simple")))
        m_info.iProfile = UMC_VC1_ENCODER::VC1_ENC_PROFILE_S;
    else if(!vm_string_stricmp(sProfile, VM_STRING("main")))
        m_info.iProfile = UMC_VC1_ENCODER::VC1_ENC_PROFILE_M;
    else if(!vm_string_stricmp(sProfile, VM_STRING("advance")))
        m_info.iProfile = UMC_VC1_ENCODER::VC1_ENC_PROFILE_A;

    par->GetParam(VM_STRING("iGOPLength"),               &m_uiGOPLength, 1);
    par->GetParam(VM_STRING("iBFramesLength"),           &m_uiBFrmLength, 1);
    par->GetParam(VM_STRING("iBFramesLength"),           &m_uiBFrmLength, 1);
    par->GetParam(VM_STRING("bVSTtransform"),            &m_bVSTransform, 1);
    par->GetParam(VM_STRING("bDeblocking"),              &m_bDeblocking, 1);
    if(par->GetParam(VM_STRING("iConstQuantization"),       &iTemp, 1))
        m_iConstQuant = (Ipp8u)iTemp;
    par->GetParam(VM_STRING("iMESearchSpeed"),           &m_uiMESearchSpeed, 1);
    par->GetParam(VM_STRING("iHRDBufferSize"),           &m_uiHRDBufferSize, 1);
    par->GetParam(VM_STRING("iHRDInitFullness"),         &m_uiHRDBufferInitFullness, 1);
    par->GetParam(VM_STRING("bFrameRecoding"),           &m_bFrameRecoding, 1);
    par->GetParam(VM_STRING("bMixedMV"),                 &m_bMixed, 1);
    par->GetParam(VM_STRING("bOriginalPredictedFrame"),  &m_bOrigFramePred, 1);
    par->GetParam(VM_STRING("bInterlaceField"),          &m_bInterlace, 1);
    par->GetParam(VM_STRING("iRefField"),                &m_uiReferenceFieldType, 1);
    par->GetParam(VM_STRING("bSceneAnalyzer"),           &m_bSceneAnalyzer, 1);
    par->GetParam(VM_STRING("bUseFeedback"),             &m_bUseMeFeedback, 1);
    par->GetParam(VM_STRING("bUpdateFeedback"),          &m_bUseUpdateMeFeedback, 1);
    par->GetParam(VM_STRING("bUseFastFeedback"),         &m_bUseFastMeFeedback, 1);
    par->GetParam(VM_STRING("iQuantType"),               &m_uiQuantType, 1);
    par->GetParam(VM_STRING("bSelectVLCTables"),         &m_bSelectVLCTables, 1);
    par->GetParam(VM_STRING("bChangeInterpPixelType"),   &m_bChangeInterpPixelType, 1);
    par->GetParam(VM_STRING("bUseTreillisQuantization"), &m_bUseTreillisQuantization, 1);

    m_uiNumFrames = 0;

    return UMC_OK;
}

#endif //defined (UMC_ENABLE_VC1_VIDEO_ENCODER)

