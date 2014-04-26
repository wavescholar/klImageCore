/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __BASIC_AUDIO_RENDER_H__
#define __BASIC_AUDIO_RENDER_H__

#include "umc_audio_render.h"
#include "umc_linear_buffer.h"
#include "umc_thread.h"
#include "umc_event.h"

namespace UMC
{

class BasicAudioRender: public AudioRender
{
public:
    DYNAMIC_CAST_DECL(BasicAudioRender, AudioRender)

    BasicAudioRender();
    virtual ~BasicAudioRender();

    virtual Status Init(MediaReceiverParams* pInit);
    virtual Status Close();
    /* to feed the renderer with audio data */
    virtual Status LockInputBuffer(MediaData *in);
    virtual Status UnLockInputBuffer(MediaData *in, Status StreamStatus = UMC_OK);
    /* to reset internal buffer with audio data */
    virtual Status Reset();
    /* to stop rendering (to kill internal thread) */
    virtual Status Stop();
    /* to render the rest of audio data be kept in buffer */
    virtual Status Pause(bool pause);

    virtual Ipp64f GetTime();
    virtual Ipp64f GetDelay();

    void ThreadProc();

    virtual Status SetParams(MediaReceiverParams *params,
                                Ipp32u trickModes = UMC_TRICK_MODES_NO);

protected:
    LinearBuffer m_DataBuffer;
    virtual Ipp64f GetTimeTick() = 0;
    virtual Status GetTimeTick(Ipp64f pts);
    /* to render the rest of audio data be kept in buffer */
    virtual Status DrainBuffer() { return UMC_OK; }

    Ipp64f  m_dfNorm;
    vm_tick m_tStartTime;
    vm_tick m_tStopTime;
    vm_tick m_tFreq;
    Thread  m_Thread;
    bool    m_bStop;
    bool    m_bPause;

    UMC::Event m_eventSyncPoint;
    UMC::Event m_eventSyncPoint1;

    bool    m_RendererIsStopped;
    Ipp32u  m_wInitedChannels;
    Ipp32u  m_dwInitedFrequency;
    Ipp64f  m_dDynamicChannelPTS;
    Ipp32s  m_wDynamicChannels;

}; // class BasicAudioRender

} // namespace UMC

#endif // __BASIC_AUDIO_RENDER_H__
