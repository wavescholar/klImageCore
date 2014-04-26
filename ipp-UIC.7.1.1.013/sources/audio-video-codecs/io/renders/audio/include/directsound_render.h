/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __DIRECTSOUND_RENDER_H__
#define __DIRECTSOUND_RENDER_H__

#include "umc_config.h"

#ifdef UMC_ENABLE_DSOUND_AUDIO_RENDER

#include <dsound.h>
#include "umc_structures.h"
#include "basic_audio_render.h"
#include "pts_buf.h"
#include "dsound_buf.h"

namespace UMC
{

class DSoundAudioRender: public BasicAudioRender
{
public:
    DYNAMIC_CAST_DECL(DSoundAudioRender, BasicAudioRender)

    DSoundAudioRender();
    virtual ~DSoundAudioRender(void);

    virtual Status  Init(MediaReceiverParams* pInit);
    virtual Status  SendFrame(MediaData* in);
    virtual Status  Pause(bool);

    // Render manipulating
    virtual Ipp32f  SetVolume(Ipp32f volume);   /* 0-silence 1-max, return previous volume */
    virtual Ipp32f  GetVolume();

    virtual Status  Close (void);
    virtual Status  Reset (void);

    virtual Status  SetParams(MediaReceiverParams *params, Ipp32u  trickModes = UMC_TRICK_MODES_NO);

    virtual AudioRenderType GetRenderType(void) { return DSOUND_AUDIO_RENDER; };

protected:
    static const Ipp32u m_cuiNumOfFrames;
    virtual Status  DrainBuffer();

    HANDLE          m_hTimer;
    BufPTSArray     m_AudioPtsArray;
    DSBuffer        m_DSBuffer;
    Ipp32u          m_dwSampleAlign;

    virtual Ipp64f  GetTimeTick(void);
};  //class DirectSoundRender

};  //  namespace UMC

#endif //  __DIRECTSOUND_RENDER_H__

#endif
