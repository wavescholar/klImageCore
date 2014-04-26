/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_MUXER_H__
#define __UMC_MPEG2_MUXER_H__

#include "ippdefs.h"
#include "umc_muxer.h"

namespace UMC
{
    class MPEG2MuxerParams;
    class MPEG2ChunkWriter;
    class PATTableWriterParams;
    class PATTableWriter;
    class PMTTableWriterParams;
    class PMTTableWriter;

    enum
    {
        MPEG2MUX_ALIGN_I        = 0x01,
        MPEG2MUX_ALIGN_P        = 0x02,
        MPEG2MUX_ALIGN_B        = 0x04,
        MPEG2MUX_ALIGN_UNDEF    = 0x08,

        MPEG2MUX_ALIGN_IP       = MPEG2MUX_ALIGN_I | MPEG2MUX_ALIGN_P,
        MPEG2MUX_ALIGN_IPB      = MPEG2MUX_ALIGN_IP | MPEG2MUX_ALIGN_B,
        MPEG2MUX_ALIGN_ALL      = MPEG2MUX_ALIGN_IPB | MPEG2MUX_ALIGN_UNDEF
    };

    class MPEG2MuxerParams : public MuxerParams
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2MuxerParams, MuxerParams)

        MPEG2MuxerParams()
        {
            m_uiAlignmentFlags = 0;
            m_dSystemTimeDelay = 0.3;
            m_uiChunkSizeLimit = 2048;
            m_uiProgramNum = 1;
            m_uiProgramPid = 0x0010;
            m_uiTransportStreamId = 0x0000;
        }

        Ipp8u m_uiAlignmentFlags; // alignment for samples
        Ipp64f m_dSystemTimeDelay; // delay of system time relatively to tracks PTSs
        Ipp16u m_uiChunkSizeLimit; // max size of data chunk
        Ipp16u m_uiProgramNum; // only one program is currently supported
        Ipp16u m_uiProgramPid; // only one program is currently supported
        Ipp16u m_uiTransportStreamId; // Mpeg2 Transport Stream element
    };

    class MPEG2Muxer : public Muxer
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2Muxer, Muxer)

        MPEG2Muxer();
        virtual ~MPEG2Muxer();
        virtual Status Init(MuxerParams *lpInit);
        virtual Status Close(void);

        virtual Status PutEndOfStream(Ipp32s iTrack);
        virtual Status UnlockBuffer(MediaData *pData, Ipp32s iTrack);
        virtual Status Flush(void);

    protected:
        virtual Status GetOutputTime(Ipp32s nStreamNumber, Ipp64f &dTime);

        // Writes chunks into stream
        // Flush mode means discarding empty buffers unless all buffers are empty
        Status WriteInterleavedFramesIntoStream(bool bFlushMode);

    protected:
        MPEG2MuxerParams *m_pMPEG2MuxerParams;
        Mutex m_synchro; // synchro object
        bool m_bQuit; // stop flag

        // rate variables
        Ipp32s m_uiTotalRate; // total bitrate of all elementary streams

        // system clock approximation
        Ipp64f m_dReferenceClock;
        Ipp64f m_dReferenceClockOfPrevSI;

        // PSI tables writers and params
        PATTableWriterParams *m_pPATTableWriterParams;
        PATTableWriter *m_pPATTableWriter;
        PMTTableWriterParams *m_pPMTTableWriterParams;
        PMTTableWriter **m_ppPMTTableWriter;

        Ipp32s max_video_buff;
        Ipp32s max_audio_buff;
    };

}; // end namespace UMC

#endif // __UMC_MPEG2_MUXER_H__
