/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __FW_AUDIO_RENDER_H__
#define __FW_AUDIO_RENDER_H__

#ifdef UMC_ENABLE_FW_AUDIO_RENDER

#include "umc_structures.h"
#include "basic_audio_render.h"
#include "umc_wave_writer.h"
#include "umc_file_writer.h"

namespace UMC
{

class FWAudioRenderParams: public AudioRenderParams
{
public:
    DYNAMIC_CAST_DECL(FWAudioRenderParams, AudioRenderParams)

    FWAudioRenderParams() {}

    DString sFileName;
};

class FWAudioRender :public BasicAudioRender
{
public:
    DYNAMIC_CAST_DECL(FWAudioRender, BasicAudioRender)

    FWAudioRender();
    virtual ~FWAudioRender();

    virtual Status Init(MediaReceiverParams* pInit);
    virtual Status SendFrame(MediaData* in);
    virtual Status Pause(bool pause);
    virtual Status Close (void);
    virtual Status Reset (void);

    // Render manipulating
    // 0-silence 1-max, return previous volume
    virtual Ipp32f SetVolume(Ipp32f volume);
    virtual Ipp32f GetVolume();

    virtual AudioRenderType GetRenderType(void) { return FW_AUDIO_RENDER; };

protected:
    virtual Ipp64f GetTimeTick(void);

    WAVEWriter     m_wav_writer;
    FileWriter     m_fileWriter;    // file writer
    Ipp64f         m_dfStartPTS;
    vm_tick        m_tickStartTime;
    vm_tick        m_tickPrePauseTime;
    DString        m_pOutFileName;
};

};  //  namespace UMC

#endif

#endif // __FW_AUDIO_RENDER_H__
