/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER

#include "umc_ac3_decoder.h"
#include "ac3_dec.h"
#include "umc_audio_data.h"

using namespace UMC;


AC3Decoder::AC3Decoder()
{
    state      = NULL;
    stateMemId = 0;
}

AC3Decoder::~AC3Decoder()
{
    Close();
}

Status AC3Decoder::Init(BaseCodecParams* init)
{
    AC3Status result;
    Ipp32s size = 0;

    result = ac3decInit(NULL, &size);

    if(AC3_OK != result)
        return StatusAC3_2_UMC(result);

    // checks or create memory allocator;
    if(BaseCodec::Init(init) != UMC_OK)
        return UMC_ERR_ALLOC;

    if(m_pMemoryAllocator->Alloc(&stateMemId, size, UMC_ALLOC_PERSISTENT) != UMC_OK)
        return UMC_ERR_ALLOC;

    state = (AC3Dec*)m_pMemoryAllocator->Lock(stateMemId);
    if(!state)
        return UMC_ERR_ALLOC;

    result = ac3decInit(state, &size);
    MemUnlock();
    m_pts_prev = 0;
    if (AC3_OK == result)
    {
        if(NULL != init)
            SetParams(init);
        return UMC_OK;
    }
    else
        return StatusAC3_2_UMC(result);
}


Status AC3Decoder::Close()
{
    if (state == NULL)
        return UMC_OK;

    if (m_pMemoryAllocator == NULL)
        return UMC_OK;

    if (state)
    {
        m_pMemoryAllocator->Free(stateMemId);
        state = NULL;
    }

    BaseCodec::Close();
    state = NULL;
    return UMC_OK;
}

Status AC3Decoder::Reset()
{
    Status status;

    status = MemLock();
    if (status != UMC_OK)
        return status;

    if (state == NULL)
        return UMC_ERR_NOT_INITIALIZED;

    ac3decReset(state);

    MemUnlock();
    return UMC_OK;
}

Status AC3Decoder::GetFrame(MediaData* in, MediaData* out)
{
    AC3Status result;
    Ipp32s nChannelOut, nDecodedBytes, SampleRate;
    Ipp32s outBufferSize;
    Status status;

    if (!in || !out)
        return UMC_ERR_NULL_PTR;

    outBufferSize = (Ipp32s)out->GetBufferSize() - (Ipp32s)((Ipp8u *)out->GetDataPointer() - (Ipp8u *)out->GetBufferPointer());

    status = MemLock();
    if (status != UMC_OK)
        return status;

    if(state == NULL)
        return UMC_ERR_NOT_INITIALIZED;

    result = ac3decGetFrame((Ipp8u *)in->GetDataPointer(), (Ipp32s)in->GetDataSize(),
        &nDecodedBytes, (Ipp16s *)out->GetDataPointer(), outBufferSize, state);

    ac3decGetNumChannelOut(&nChannelOut, state);

    if (AC3_BAD_STREAM == result)
    {
        if (outBufferSize < (Ipp32s)(nChannelOut*256*6*sizeof(Ipp16s)))
        {
            MemUnlock();
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        }
    }

    in->MoveDataPointer(nDecodedBytes);
    ac3decGetSampleFrequency(&SampleRate, state);

    if (nDecodedBytes && (AC3_OK == result || AC3_BAD_STREAM == result))
 {
        Ipp64f pts_start = in->m_fPTSStart;
        Ipp64f pts_end;

        if (pts_start == -1.0)
            pts_start = m_pts_prev;

        m_pts_prev = pts_end = pts_start + (256*6)/((Ipp32f)SampleRate);
        in->m_fPTSStart = pts_end;

        if (AC3_BAD_STREAM == result) // fill with silent data
            memset(out->GetDataPointer(),0,nChannelOut*256*6*2);

        out->SetDataSize(nChannelOut*256*6*2);
        out->m_fPTSStart = pts_start;
        out->m_fPTSEnd   = pts_end;
    }

    if (AC3_OK == result)
    {
        AudioData* pAudio = DynamicCast<AudioData, MediaData>(out);

        if (pAudio)
        {
            pAudio->m_iBitPerSample    = 16;
            pAudio->m_iChannels        = nChannelOut;
            pAudio->m_iSampleFrequency = SampleRate;
        }
    }

    MemUnlock();
    return StatusAC3_2_UMC(result);
}

Status AC3Decoder::SetParams(BaseCodecParams * params)
{
    AC3DecoderParams *info = DynamicCast<AC3DecoderParams, BaseCodecParams>(params);

    if (info)
    {
        Status status;

        status = MemLock();
        if (status != UMC_OK)
            return status;

        if(state == NULL)
            return UMC_ERR_NOT_INITIALIZED;

        ac3decSetOutAcmod(info->out_acmod, state);
        ac3decSetOuLfeOn(info->outlfeon, state);
        ac3decSetDualMonoMode(info->dualmonomode, state);
        ac3decSetDrcScaleLow(info->drc_scaleLow, state);
        ac3decSetDrcScaleHigh(info->drc_scaleHigh, state);
        ac3decSetOutCompMod(info->out_compmod, state);
        ac3decSetKaraokeCapable(info->karaokeCapable, state);
        ac3decSetCrcMute(info->crc_mute, state);
        ac3decSetGainScale(info->gainScale, state);
    }
    else
        return UMC_ERR_NULL_PTR;

    return UMC_OK;
}

Status AC3Decoder::GetInfo(BaseCodecParams* info)
{
    Status status;

    if (!info)
        return UMC_ERR_NULL_PTR;

    info->m_iSuggestedInputSize  = 1920 * 2;
    info->m_iSuggestedOutputSize = 6 * (256 * 6) * sizeof(Ipp16s);

    AudioDecoderParams *p_info = DynamicCast<AudioDecoderParams, BaseCodecParams>(info);

    status = MemLock();
    if (status != UMC_OK)
        return status;

    ac3decGetInfo(&params, state);

    if (!p_info)
        return UMC_OK;

    p_info->m_info.iLevel               = AC3_LEVEL_MAIN;
    p_info->m_info.iProfile             = AC3_PROFILE_MAIN;

    if (p_info)
    {
        p_info->m_info.audioInfo.m_iChannels = params.m_info_out.channels;
        p_info->m_info.streamType = AC3_AUDIO;

        if (params.is_valid)
        {
            p_info->m_info.audioInfo.m_iBitPerSample     = params.m_info_out.bitPerSample;
            p_info->m_info.audioInfo.m_iSampleFrequency  = params.m_info_out.sample_frequency;
            p_info->m_info.iBitrate          = params.m_info_out.bitrate;
            p_info->m_info.audioInfo.m_iChannelMask      = 0;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_FRONT_CENTER)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_CENTER;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_FRONT_LEFT)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_LEFT;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_FRONT_RIGHT)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_FRONT_RIGHT;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_BACK_CENTER)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_CENTER;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_BACK_LEFT)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_LEFT;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_BACK_RIGHT)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_BACK_RIGHT;

            if (params.m_info_out.channel_mask & AC3_CHANNEL_LOW_FREQUENCY)
                p_info->m_info.audioInfo.m_iChannelMask |= CHANNEL_LOW_FREQUENCY;
        }
    }
    else
    {
        MemUnlock();
        return UMC_WRN_INFO_NOT_READY;
    }

    MemUnlock();
    return UMC_OK;
}

Status AC3Decoder::GetDuration(Ipp32f* p_duration)
{
    Status status;

    if (!p_duration)
        return UMC_ERR_NULL_PTR;

    status = MemLock();
    if (status != UMC_OK)
        return status;

    if (state == NULL)
        return UMC_ERR_NOT_INITIALIZED;

    ac3decGetDuration(p_duration, state);
    MemUnlock();
    return UMC_OK;
}

Status AC3Decoder::StatusAC3_2_UMC(AC3Status st)
{
    Status res;
    if (st == AC3_OK)
        res = UMC_OK;
    else if (st == AC3_NOT_ENOUGH_DATA)
        res = UMC_ERR_NOT_ENOUGH_DATA;
    else if (st == AC3_FLAGS_ERROR)
        res = UMC_ERR_INVALID_PARAMS;
    else if (st == AC3_ALLOC)
        res = UMC_ERR_ALLOC;
    else if (st == AC3_BAD_STREAM)
        res = UMC_ERR_INVALID_STREAM;
    else if (st == AC3_NULL_PTR)
        res = UMC_ERR_NULL_PTR;
    else if (st == AC3_NOT_FIND_SYNCWORD)
        res = UMC_ERR_SYNC;
    else if (st == AC3_NOT_ENOUGH_BUFFER)
        res = UMC_ERR_NOT_ENOUGH_BUFFER;
    else if (st == AC3_UNSUPPORTED)
        res = UMC_ERR_UNSUPPORTED;
    else
        res = UMC_ERR_UNSUPPORTED;

    return res;
}

Status AC3Decoder::MemLock()
{
    AC3Dec *pOldState = state;

    /* AYA: 07 june 07 failed if this check delete */
    if( m_pMemoryAllocator == NULL )
        return UMC_OK;

    state = (AC3Dec *)m_pMemoryAllocator->Lock(stateMemId);
    if(!state)
        return UMC_ERR_ALLOC;

    if (state != pOldState)
        ac3decUpdateMemMap(state, (Ipp32s)((Ipp8u *)state-(Ipp8u *)pOldState));

    return UMC_OK;
}

Status AC3Decoder::MemUnlock()
{
    if (stateMemId)
    {
        /* AYA: 07 june 07 failed if this check delete */
        if( m_pMemoryAllocator == NULL )
            return UMC_OK;

        if (m_pMemoryAllocator->Unlock(stateMemId) != UMC_OK)
            return UMC_ERR_ALLOC;
    }
    return UMC_OK;
}

#endif
