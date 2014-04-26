/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AUDIO_RENDER_H__
#define __UMC_AUDIO_RENDER_H__

#include "vm_time.h"
#include "umc_structures.h"
#include "umc_media_data.h"
#include "umc_media_receiver.h"
#include "umc_system_context.h"

/*
//  Class:       AudioRender
//
//  Notes:       Base abstract class of audio render. Class describes
//               the high level interface of abstract audio render.
//               All system specific (Windows (DirectSound, WinMM), Linux(SDL, etc)) must be implemented in
//               derived classes.
*/

namespace UMC
{

enum // audio renderer flags
{
    FLAG_AREN_HIDDEN          = 0x00000001
};

class AudioRenderParams : public MediaReceiverParams
{
public:
    DYNAMIC_CAST_DECL(AudioRenderParams, MediaReceiverParams)

    AudioRenderParams(void)
    {
        m_pSysContext = NULL;
    }

    AudioStreamInfo   m_info;            // common fields which necessary to initialize decoder
    SysRenderContext *m_pSysContext;  // platform dependent context
};

class AudioRender : public MediaReceiver
{
public:
    DYNAMIC_CAST_DECL(AudioRender, MediaReceiver)

    AudioRender(void) {}
    virtual ~AudioRender(void) {}

    // Send new portion of data to render
    virtual Status SendFrame(MediaData* pDataIn) = 0;

    // Pause(Resume) playing of soundSend new portion of data to render
    virtual Status Pause(bool bPause) = 0;

    // Volume manipulating
    virtual Ipp32f SetVolume(Ipp32f fVolume) = 0;
    virtual Ipp32f GetVolume(void) = 0;

    // Audio Reference Clock
    virtual Ipp64f GetTime(void) = 0;

    // Estimated value of device latency
    virtual Ipp64f GetDelay(void) = 0;

    virtual Status SetParams(MediaReceiverParams* pMedia, Ipp32u  trickModes = UMC_TRICK_MODES_NO)
    {
        pMedia     = pMedia; // avoid unreference parameter warning
        trickModes = trickModes;

        return UMC_ERR_NOT_IMPLEMENTED;
    }

    virtual AudioRenderType GetRenderType(void) = 0;
};

}

#endif
