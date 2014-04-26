/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __WINMM_RENDER_H__
#define __WINMM_RENDER_H__

#include "umc_config.h"

#ifdef UMC_ENABLE_WINMM_AUDIO_RENDER

#include "vm_debug.h"
#include "umc_semaphore.h"
#include <mmsystem.h>
#include "basic_audio_render.h"

#define WINMM_BUF_NUM_MAX 10
#define WINMM_BUF_SIZE    1152*128

namespace UMC
{

class WinMMAudioRender :public BasicAudioRender
{
public:
    DYNAMIC_CAST_DECL(WinMMAudioRender, BasicAudioRender)

    WinMMAudioRender(void);
    virtual ~WinMMAudioRender(void);

    // Initialize the audio device. The playback is paused
    virtual Status Init(MediaReceiverParams* pInit);

    // Write sound data to sound buffer
    virtual Status SendFrame(MediaData* in);
    virtual Status UnLockInputBuffer(MediaData *in, Status StreamStatus = UMC_OK);

    // Toggle pause mode. Return current pts
    virtual Status Pause(bool pause);

    // Close the sound device
    virtual Status Close(void);

    // 0-silence 1-max, return previous volume */
    virtual Ipp32f SetVolume(Ipp32f volume) { return volume;}
    virtual Ipp32f GetVolume() { return 0.;}
    virtual Status Reset(void);

    void   Release(void);

    virtual AudioRenderType GetRenderType(void) { return WINMM_AUDIO_RENDER; };

protected:
    class BufArray
    {
    public:
        WAVEHDR m_pArray[WINMM_BUF_NUM_MAX];
        BufArray();
        ~BufArray();
        Status Init(Ipp32u uiBufSize);
        void   ZeroBuffers();
        inline Ipp32u GetHdrNum() { return m_uiHdrNum; }
    protected:
        Ipp32u m_uiBufSize;
        Ipp32u m_uiHdrNum;
    };

    BufArray   m_Hdrs;

    UMC::Semaphore m_sm_free_buffers;
    Ipp32s     m_iFreeBuf;
    Ipp32s     m_iDoneBuf;

    HWAVEOUT   m_hWO;
    WAVEFORMATEX m_WaveFmtX;

    Ipp64f     m_dfSampleNorm;
    Ipp64f     m_dfStartPTS;
    Ipp32s     m_iOffset;
    Ipp32u     m_dwVolume;
    Ipp32u     m_DataSize;
    bool       m_bHdrOK;    // header for all buffers created flag

    Ipp64f  GetTimeTick(void);
    Status DrainBuffer();
};

} // namespace UMC

#endif // UMC_ENABLE_WINMM_AUDIO_RENDER

#endif // __WINMM_RENDER_H__
