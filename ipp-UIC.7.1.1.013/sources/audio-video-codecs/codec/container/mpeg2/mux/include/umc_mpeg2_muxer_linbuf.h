/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_MUXER_LINBUF_H__
#define __UMC_MPEG2_MUXER_LINBUF_H__

#include "vm_mutex.h"
#include "umc_media_buffer.h"
#include "umc_mpeg2_muxer_defs.h"

namespace UMC
{
    enum
    {
        TYPE_I_PIC = 0x01,
        TYPE_P_PIC = 0x02,
        TYPE_B_PIC = 0x04,
        TYPE_UNDEF = 0x08
    };

    struct MPEG2MuxerSample
    {
        MPEG2MuxerSample()
        {
            dPTS = -1.0;
            dDTS = -1.0;
            dDuration = 0.0;
            dExactDTS = 0.0;
            uiSize = 0;
            uiTSOffset = 0;
            pData = NULL;
            uiFrameType = TYPE_UNDEF;
            pNext = NULL;
            pPrev = NULL;
        }

        Ipp64f dPTS; // PTS of sample
        Ipp64f dDTS; // DTS of sample
        Ipp64f dDuration; // duration of sample
        Ipp64f dExactDTS; // it equals dDTS if exists and is approximated if dDTS is absence
        size_t uiSize; // size of sample
        size_t uiTSOffset; // offset of byte assocciated with time stamps
        Ipp8u *pData; // pointer to data
        Ipp8u uiFrameType; // type of frame (only for video)
        MPEG2MuxerSample *pNext; // pointer to next sample
        MPEG2MuxerSample *pPrev; // pointer to prev sample
    };

    class MPEG2MuxerLinearBufferParams : public MediaReceiverParams
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2MuxerLinearBufferParams, MediaReceiverParams)

        MPEG2MuxerLinearBufferParams()
        {
            uiBufferSize = 0;
            uiInputSize = 0;
            uiOfFrames = 0;
            uiAlignmentFlags = 0;
        }

        size_t uiBufferSize; // size of buffer to allocate
        Ipp32u uiInputSize; // guaranteed size of input buffers
        Ipp32u uiOfFrames; // number of sample will be initially allocated
        Ipp8u uiAlignmentFlags;
    };

    class MPEG2MuxerLinearBuffer
    {
    public:
        MPEG2MuxerLinearBuffer();
        virtual ~MPEG2MuxerLinearBuffer();
        virtual Status Init(MediaReceiverParams *pInit);
        virtual Status Close(void);
        virtual Status Reset(void);
        Status LockInputBuffer(MPEG2MuxerSample *pData);
        Status UnLockInputBuffer(MPEG2MuxerSample *pData, Status streamStatus = UMC_OK);
        Status LockOutputBuffer(MPEG2MuxerSample *pData);
        virtual Status UnLockOutputBuffer(MPEG2MuxerSample *pData);

    protected:
        MPEG2MuxerSample *m_pFirstUsedSample; // pointer to first used sample
        MPEG2MuxerSample *m_pFirstFreeSample; // pointer to first free sample
        MPEG2MuxerSample *m_pLastUsedSample; // pointer to last used sample
        MPEG2MuxerSample *m_pLastFreeSample; // pointer to last free sample
        Ipp8u m_uiAlignmentFlags;

        Ipp8u *m_pAllocBuf; // pointer to allocated buffer
        Ipp8u *m_pBuf; // pointer to main part of buffer
        Ipp32s m_uiInputSize; // guaranteed size of input buffers
        Ipp32s m_iBufferSize; // size of allocated buffer
        Ipp32s m_iFirstFreeByte; // number of byte next to last used byte
        Ipp32s m_iFirstUsedByte; // number of first data byte
        Ipp64f m_dNextDTS;

        Ipp32s m_iUnusedSizeAtTheEnd; // number of unused bytes at the end of buffer (when data is wrapped)
        Ipp32s m_iUsedSize; // space is occupied by samples
        Ipp32u m_uiNOfUsedSamples; // number of samples in buffer
        Ipp32u m_uiNOfAllocatedSamples; // number of allocated sample structures
        bool m_bEndOfStream; // indicates end of stream

        Mutex m_synchro; // synchro object
        static const Ipp32s m_iMaxOutputSize = 0xFFFF;
    };

}; // end namespace UMC

#endif // __UMC_MPEG2_MUXER_LINBUF_H__
