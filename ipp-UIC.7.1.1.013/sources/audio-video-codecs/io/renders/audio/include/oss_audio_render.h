/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

//
//    This file contains audio render based on Open Sound System for UNIX and
//    UNIX-like systems (Particulary for Linux - we have no opportunity to test
//    other OSes) supporting /dev/dsp and /dev/mixer devices.
//

#ifndef __OSS_AUDIO_RENDER_H__
#define __OSS_AUDIO_RENDER_H__

#include "umc_config.h"

#ifdef UMC_ENABLE_OSS_AUDIO_RENDER

#include "umc_structures.h"
#include "basic_audio_render.h"
#include "oss_dev.h"

namespace UMC
{

class OSSAudioRender :public BasicAudioRender
{
public:
    DYNAMIC_CAST_DECL(OSSAudioRender, BasicAudioRender)

    OSSAudioRender();
    virtual ~OSSAudioRender() {   Close();     }

    virtual Status Init(MediaReceiverParams* pInit);

    virtual Status SendFrame(MediaData* in);

    virtual Status Pause(bool pause);

    // Render manipulating
    // 0-silence 1-max, return previous volume
    virtual Ipp32f SetVolume(Ipp32f volume);
    virtual Ipp32f GetVolume();

    virtual Status Close (void);
    virtual Status Reset (void);

    virtual Status SetParams(MediaReceiverParams *params,
                                Ipp32u trickModes = UMC_TRICK_MODES_NO);

    virtual AudioRenderType GetRenderType(void) { return OSS_AUDIO_RENDER; };

protected:
    virtual Ipp64f GetTimeTick(void);

    OSSDev         m_Dev;
    Ipp64f         m_dfStartPTS;
    vm_tick        m_tickStartTime;
    vm_tick        m_tickPrePauseTime;
};

};  //  namespace UMC

#endif // UMC_ENABLE_OSS_AUDIO_RENDER

#endif // __OSS_AUDIO_RENDER_H__

