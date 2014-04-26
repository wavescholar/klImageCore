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
#ifdef UMC_ENABLE_MP3_AUDIO_ENCODER

#include "umc_mp3_encoder.h"
#include "mp3enc.h"

using namespace UMC;


MP3Encoder::MP3Encoder()
{
    state                 = NULL;
    m_freq                = 0;
    m_bitrate             = 0;
    m_stereo              = 0;
    m_id                  = 0;
    m_frame_size          = 0;
    m_layer               = 0;
    m_stereo_mode         = 0;
    m_ns_mode             = 0;
    m_force_mpeg1         = 0;
    m_mc_matrix_procedure = 0;
    stateMemId            = 0;
    m_initialized         = 0;
}

MP3Encoder::~MP3Encoder()
{
    Close();
}

bool MP3Encoder::CheckBitRate(Ipp32s sample_rate, Ipp32s layer, Ipp32s bitrate, Ipp32s stereo)
{
    Ipp32s ind;
    Ipp32s id;

    if(sample_rate == 44100 || sample_rate == 48000 || sample_rate == 32000)
        id = 1;
    else if(sample_rate == 22050 || sample_rate == 24000 || sample_rate == 16000)
        id = 0;
    else
        return false;

    if (mp3enc_checkBitRate(id, layer, stereo, bitrate / 1000, &ind))
        return true;

    return false;
}

Status MP3Encoder::Init(BaseCodecParams *init)
{
    Ipp32s size;
    Ipp32s st, br, fr, chm, channel_mask;
    Ipp32s stereo_mode;
    MP3Status res;
    MP3EncoderParams* pMP3EncoderParams;

    // checks or create memory allocator;
    if (BaseCodec::Init(init) != UMC_OK)
        return UMC_ERR_ALLOC;

    AudioEncoderParams* pAudioCodecInit = DynamicCast<AudioEncoderParams, BaseCodecParams>(init);

    if (!pAudioCodecInit)
        return UMC_ERR_NULL_PTR;

    fr  = pAudioCodecInit->m_info.audioInfo.m_iSampleFrequency;
    st  = pAudioCodecInit->m_info.audioInfo.m_iChannels;
    chm = pAudioCodecInit->m_info.audioInfo.m_iChannelMask;
    br  = pAudioCodecInit->m_info.iBitrate / 1000;

    m_frame_num = 0;
    m_pts_prev  = 0;

    pMP3EncoderParams = DynamicCast<MP3EncoderParams, BaseCodecParams>(init);
    if (pMP3EncoderParams)
    {
        m_stereo_mode         = pMP3EncoderParams->stereo_mode;
        m_ns_mode             = pMP3EncoderParams->ns_mode;
        m_layer               = pMP3EncoderParams->layer;
        m_force_mpeg1         = pMP3EncoderParams->force_mpeg1;
        m_mc_matrix_procedure = pMP3EncoderParams->mc_matrix_procedure;
        m_mc_lfe_filter_off   = pMP3EncoderParams->mc_lfe_filter_off;
        m_br_mode             = pMP3EncoderParams->mode;
    }
    else
    {
        m_stereo_mode         = UMC_MPA_LR_STEREO;
        m_ns_mode             = 0;
        m_layer               = 3;
        m_force_mpeg1         = 0;
        m_mc_matrix_procedure = 0;
        m_mc_lfe_filter_off   = 0;
        m_br_mode             = MPAENC_CBR;
    }

    if (st == 1)
        m_stereo_mode = UMC_MPA_MONO;

    switch (m_stereo_mode)
    {
    case UMC_MPA_MONO:
        stereo_mode = MPA_MONO;
        break;
    case UMC_MPA_LR_STEREO:
        stereo_mode = MPA_LR_STEREO;
        break;
    case UMC_MPA_MS_STEREO:
        stereo_mode = MPA_MS_STEREO;
        break;
    case UMC_MPA_JOINT_STEREO:
        stereo_mode = MPA_JOINT_STEREO;
        break;
    default:
        stereo_mode = MPA_LR_STEREO;
        break;
    }

    channel_mask = 0;

    if (chm & (CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT))
        channel_mask |= MP3_CHANNEL_STEREO;

    if (chm & CHANNEL_FRONT_CENTER)
        channel_mask  |= MP3_CHANNEL_CENTER;

    if (chm & CHANNEL_LOW_FREQUENCY)
        channel_mask  |= MP3_CHANNEL_LOW_FREQUENCY;

    if (chm & CHANNEL_BACK_CENTER)
        channel_mask  |= MP3_CHANNEL_SURROUND_MONO;

    if (chm & (CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT))
        channel_mask |= MP3_CHANNEL_SURROUND_STEREO;

    /*    if (chm & (CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT))
    channel_mask |= MP3_CHANNEL_SURROUND_STEREO_P2;*/

    res = mp3encInit(NULL, fr, st, channel_mask, m_layer, br, m_br_mode, stereo_mode, m_ns_mode,
        m_force_mpeg1, m_mc_matrix_procedure, m_mc_lfe_filter_off, &size);
    if (res != MP3_OK)
        return StatusMP3_2_UMC(res);

    if (m_pMemoryAllocator->Alloc(&stateMemId, size, UMC_ALLOC_PERSISTENT, 32) != UMC_OK)
        return UMC_ERR_ALLOC;

    state = (MP3Enc *)m_pMemoryAllocator->Lock(stateMemId);
    if(!state)
        return UMC_ERR_ALLOC;

    res = mp3encInit(state, fr, st, channel_mask, m_layer, br, m_br_mode, stereo_mode, m_ns_mode,
        m_force_mpeg1, m_mc_matrix_procedure, m_mc_lfe_filter_off, &size);

    if (res == MP3_OK)
    {
        m_stereo  = st;
        m_freq    = fr;
        m_bitrate = br;
        mp3encGetMpegId(state, &m_id);
        mp3encGetUpsample(state, &m_upsample);
        mp3encGetFrameSize(&m_frame_size, m_id, m_layer, m_upsample);
        m_initialized = 1;
    }

    MemUnlock();

    return StatusMP3_2_UMC(res);
}

Status MP3Encoder::Close()
{
    Status res = MemLock();
    if (res != UMC_OK)
        return UMC_OK;

    mp3encClose(state);
    MemUnlock();

    if(state)
    {
        m_pMemoryAllocator->Free(stateMemId);
        state = NULL;
    }

    BaseCodec::Close();

    return UMC_OK;
}

Status  MP3Encoder::GetFrame(MediaData * in, MediaData * out)
{
    Ipp32s inSamples;
    Ipp32s bytes;
    MP3Status res;
    Ipp32s out_size, slot_size;
    Status status;
    Ipp64f pts_start, pts_end;
    Ipp32s nullInput = 0;

    if (!out || !out->GetDataPointer())
        return UMC_ERR_NULL_PTR;

    if (!m_initialized)
        return UMC_ERR_NOT_INITIALIZED;

    if (!in || !in->GetDataPointer())
        nullInput = 1;

    if (!nullInput)
    {
        inSamples = (Ipp32s)(in->GetDataSize() / sizeof(Ipp16s));

        if (inSamples < m_stereo * m_frame_size)
            return UMC_ERR_NOT_ENOUGH_DATA;
    }

    inSamples = m_stereo * m_frame_size;

    out_size = (Ipp32s)(out->GetBufferSize() - ((Ipp8u*)out->GetDataPointer() - (Ipp8u*)out->GetBufferPointer()));

    status = MemLock();
    if (status != UMC_OK)
        return status;

    mp3encGetSlotSize(state, &slot_size);
    if (out_size < slot_size + (m_layer == 3 ? 1024 : 0))
    {
        MemUnlock();
        return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    if (!nullInput)
        res = mp3encGetFrame((Ipp16s *)in->GetDataPointer(), &bytes, (Ipp8u *)out->GetDataPointer(), state);
    else
        res = mp3encGetFrame(NULL, &bytes, (Ipp8u *)out->GetDataPointer(), state);

    if (res == MP3_OK)
    {
        if (!nullInput)
            pts_start = in->m_fPTSStart;
        else
            pts_start = -1;

        if (pts_start < 0)
            pts_start = m_pts_prev;

        m_pts_prev = pts_end = pts_start+(Ipp32f)(inSamples/m_stereo)/(Ipp32f)(m_freq);

        out->m_fPTSStart = pts_start;
        out->m_fPTSEnd   = pts_end;
        out->SetDataSize(bytes);

        if (!nullInput)
        {
            in->MoveDataPointer(inSamples*sizeof(Ipp16s));
            in->m_fPTSStart = pts_end;
            m_frame_num++;
        }
    }

    MemUnlock();
    return StatusMP3_2_UMC(res);
}

Status MP3Encoder::GetInfo(BaseCodecParams * info)
{
    if (!info)
        return UMC_ERR_NULL_PTR;

    info->m_iSuggestedInputSize = m_stereo * sizeof(Ipp16s) * m_frame_size;
    info->m_iSuggestedOutputSize = m_stereo * sizeof(Ipp16s) * m_frame_size;
    if (m_layer == 3)
        info->m_iSuggestedOutputSize += 1024;

    AudioEncoderParams *pAudioCodecInfo = DynamicCast<AudioEncoderParams, BaseCodecParams>(info);

    if (!pAudioCodecInfo)
        return UMC_OK;

    pAudioCodecInfo->m_info.audioInfo.m_iBitPerSample    = 16;
    pAudioCodecInfo->m_info.iBitrate        = 1000*m_bitrate;
    pAudioCodecInfo->m_info.audioInfo.m_iChannels        = m_stereo;
    pAudioCodecInfo->m_info.audioInfo.m_iSampleFrequency  = m_freq;
    pAudioCodecInfo->m_info.streamType = UNDEF_AUDIO;

    if (m_layer == 3)
    {
        if (m_id == 1)
            pAudioCodecInfo->m_info.streamType = MP1L3_AUDIO;
        else if (m_id == 0)
            pAudioCodecInfo->m_info.streamType = MP2L3_AUDIO;
    }
    else if (m_layer == 2)
    {
        if (m_id == 1)
            pAudioCodecInfo->m_info.streamType = MP1L2_AUDIO;
        else if (m_id == 0)
            pAudioCodecInfo->m_info.streamType = MP2L2_AUDIO;
    }
    else if (m_layer == 1)
    {
        if (m_id == 1)
            pAudioCodecInfo->m_info.streamType = MP1L1_AUDIO;
        else if (m_id == 0)
            pAudioCodecInfo->m_info.streamType = MP2L1_AUDIO;
    }

    return UMC_OK;
}

Status MP3Encoder::GetDuration(Ipp32f *p_duration)
{
    Ipp32f duration;

    if(!p_duration)
        return UMC_ERR_NULL_PTR;

    if(!m_initialized)
        return UMC_ERR_NOT_INITIALIZED;

    duration  = (Ipp32f)m_frame_num * m_frame_size;
    duration /= (Ipp32f)(m_freq);

    p_duration[0] = duration;

    return UMC_OK;
}

Status MP3Encoder::SetParams(BaseCodecParams * /* params */)
{
    return UMC_ERR_NOT_IMPLEMENTED;
}

Status MP3Encoder::Reset()
{
    return UMC_ERR_NOT_IMPLEMENTED;
}

Status MP3Encoder::StatusMP3_2_UMC(MP3Status st)
{
    Status res;

    if (st == MP3_OK)
        res = UMC_OK;
    else if (st == MP3_NOT_ENOUGH_DATA)
        res = UMC_ERR_NOT_ENOUGH_DATA;
    else if (st == MP3_BAD_FORMAT)
        res = UMC_ERR_INVALID_STREAM;
    else if (st == MP3_ALLOC)
        res = UMC_ERR_ALLOC;
    else if (st == MP3_BAD_STREAM)
        res = UMC_ERR_INVALID_STREAM;
    else if (st == MP3_NULL_PTR)
        res = UMC_ERR_NULL_PTR;
    else if (st == MP3_NOT_FIND_SYNCWORD)
        res = UMC_ERR_SYNC;
    else if (st == MP3_NOT_ENOUGH_BUFFER)
        res = UMC_ERR_NOT_ENOUGH_BUFFER;
    else if (st == MP3_UNSUPPORTED)
        res = UMC_ERR_UNSUPPORTED;
    else if (st == MP3_FAILED_TO_INITIALIZE)
        res = UMC_ERR_INIT;
    else if (st == MP3_END_OF_STREAM)
        res = UMC_ERR_END_OF_STREAM;
    else
        res = UMC_ERR_UNSUPPORTED;

    return res;
}

Status MP3Encoder::MemLock()
{
    MP3Enc *pOldState = state;

    if (!m_pMemoryAllocator)
        return UMC_ERR_ALLOC;

    state = (MP3Enc *)m_pMemoryAllocator->Lock(stateMemId);
    if(!state)
        return UMC_ERR_ALLOC;

    if (state != pOldState)
        mp3encUpdateMemMap(state, (Ipp32s)((Ipp8u *)state-(Ipp8u *)pOldState));

    return UMC_OK;
}

Status MP3Encoder::MemUnlock()
{
    if (stateMemId)
    {
        if (m_pMemoryAllocator->Unlock(stateMemId) != UMC_OK)
            return UMC_ERR_ALLOC;
    }
    return UMC_OK;
}

#endif
