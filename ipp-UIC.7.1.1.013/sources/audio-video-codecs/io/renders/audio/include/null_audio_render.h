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
//    This file contains audio render doing nothing beside returning right time
//    based on system time. You can use it for pure video (without audio)
//    streams syncronization.
//

#ifndef __NULL_AUDIO_RENDER_H__
#define __NULL_AUDIO_RENDER_H__

#include "vm_time.h"
#include "umc_structures.h"
#include "basic_audio_render.h"

namespace UMC
{

typedef enum
{
    UMC_1X_FFMODE = 0,
    UMC_2X_FFMODE = 1,
    UMC_4X_FFMODE = 3,
    UMC_8X_FFMODE = 7,
    UMC_16X_FFMODE = 15
} UMC_FFMODE_MULTIPLIER;

class NULLAudioRenderParams: public AudioRenderParams
{
public:
    DYNAMIC_CAST_DECL(NULLAudioRenderParams, AudioRenderParams)

    NULLAudioRenderParams()
    {
        m_InitPTS = 0;
    }

    Ipp64f m_InitPTS;
};

class NULLAudioRender : public BasicAudioRender
{
public:
    DYNAMIC_CAST_DECL(NULLAudioRender, BasicAudioRender)

    NULLAudioRender(Ipp64f dfDelay = 0, UMC_FFMODE_MULTIPLIER mult = UMC_1X_FFMODE);
    virtual ~NULLAudioRender() {   Close();     }

    virtual Status  Init(MediaReceiverParams* pInit);
    virtual Status  SendFrame(MediaData* in);
    virtual Status  Pause(bool bPause);

    // Render manipulating
    // 0-silence 1-max, return previous volume
    virtual Ipp32f  SetVolume(Ipp32f /*volume*/);
    virtual Ipp32f  GetVolume();

    virtual Status  Close ();
    virtual Status  Reset ();

    virtual Status SetParams(MediaReceiverParams *params, Ipp32u  trickModes = UMC_TRICK_MODES_NO);

    virtual AudioRenderType GetRenderType(void) { return NULL_AUDIO_RENDER; };

protected:
    vm_tick         m_Freq;
    vm_tick         m_LastStartTime;
    vm_tick         m_PrePauseTime;
    bool            m_bPaused;
    Ipp64f          m_dInitialPTS;
    Ipp64f          m_uiFFModeMult;
    virtual Ipp64f  GetTimeTick();

};

} // namespace UMC

#endif // __NULL_AUDIO_RENDER_H__
