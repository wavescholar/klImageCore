/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_MUXER_CHUNK_H
#define __UMC_MPEG2_MUXER_CHUNK_H

#include "umc_data_writer.h"
#include "umc_mpeg2_muxer_defs.h"
#include "umc_mpeg2_muxer_bitstream.h"
#include "umc_mpeg2_muxer_linbuf.h"

namespace UMC
{
    class MPEG2ChunkWriterParams : public MPEG2MuxerLinearBufferParams
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2ChunkWriterParams, MPEG2MuxerLinearBufferParams)

        MPEG2ChunkWriterParams()
        {
            pDataWriter = NULL;
            esType = MPEG2MUX_ES_UNKNOWN;
            dFrameRate = 0.0;
            uiBitRate = 0;
        };

        DataWriter *pDataWriter; // pointer to data writer
        MPEG2MuxerESType esType; // type of elementary stream
        Ipp64f dFrameRate;
        Ipp32u uiBitRate;
    };

    class MPEG2ChunkWriter : public MediaBuffer, protected MPEG2MuxerLinearBuffer
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2ChunkWriter, MediaBuffer)

        MPEG2ChunkWriter(void);
        ~MPEG2ChunkWriter(void);
        Status Init(MediaReceiverParams *pInit);
        Status Close(void);
        Status LockInputBuffer(MediaData *pData);
        Status UnLockInputBuffer(MediaData *pData, Status streamStatus = UMC_OK);
        Status LockOutputBuffer(MediaData *pData) { pData = pData; return UMC_OK; }
        Status UnLockOutputBuffer(MediaData *pData) { pData = pData; return UMC_OK; }
        Status Stop(void) { return UMC_OK; }

        // Provides time of first output sample if exists
        // Returns UMC_ERR_NOT_ENOUGH_DATA when buffer is empty
        // Returns UMC_ERR_END_OF_STREAM when buffer is empty and EOS was received
        Status GetOutputTime(Ipp64f &dTime);

        // updates reference clock field
        virtual void SetReferenceClock(Ipp64f dReferenceClock);

        // updates mux rate field
        void SetMuxRate(Ipp32u uiMuxRate);

        // writes chunk into stream and returns remain data
        virtual Status WriteChunk(void);

        // parse video frames to get picture type and offset for timestamps
        void SetFrameTypeAndOffset(MPEG2MuxerSample *pData);

    protected:
        MPEG2MuxerESType m_ESType; // type of elementary stream
        DataWriter *m_pDataWriter; // pointer to data writer

        Ipp64f m_dReferenceClock; // system clock reference
        Ipp32u m_uiMuxRate; // mux rate (MPEG2PS)

        Ipp64f m_dFrameRate;
        Ipp32u m_uiBitRate;
    };

    class MPEG2PSChunkWriterParams : public MPEG2ChunkWriterParams
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2PSChunkWriterParams, MPEG2ChunkWriterParams)

        MPEG2PSChunkWriterParams(void)
        {
            uiPacketSize = 0;
            bPackHeaderPresent = false;
            uiStreamID = 0;
            uiSubStreamID = 0;
        };

        Ipp16u uiPacketSize; // maximum size of chunk
        bool bPackHeaderPresent; // used only for MPEG2PS

        Ipp32u uiStreamID; // stream ID
        Ipp32u uiSubStreamID; // used only if uiStreamID is PRIVATE_STREAM_1
    };

    class SystemHeaderParams
    {
    public:
        SystemHeaderParams(void)
        {
            uiSystemRateBound = 0;
            uiSystemAudioBound = 0;
            uiSystemVideoBound = 0;
            uiSystemNumberOfStreams = 0;
            pSystemStreamID = NULL;
            pSystemSizeBound = NULL;
        }

        Ipp32u uiSystemRateBound; // maximum mux rate
        Ipp32u uiSystemAudioBound; // maximum number of MPEG1/2 audio streams
        Ipp32u uiSystemVideoBound; // maximum number of MPEG1/2 video streams
        Ipp32u uiSystemNumberOfStreams; // number of streams mentioned in system header
        Ipp32u *pSystemStreamID; // array of stream IDs
        Ipp32u *pSystemSizeBound; // array of size bounds
    };

    class MPEG2PSChunkWriter : public MPEG2ChunkWriter
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2PSChunkWriter, MPEG2ChunkWriter)

        MPEG2PSChunkWriter(void);
        ~MPEG2PSChunkWriter(void);
        Status Init(MediaReceiverParams *pInit);
        Status Close(void);
        Status WriteChunk(void);

        Status SetSystemHeaderParams(SystemHeaderParams *pSysHeaderParams);

        // turns on system header, after writing it will be turned off
        void ToggleSystemHeader(void);

    protected:
        // locks payload, analyzes its properties, calculates length fields
        // and check if PTS/DTS will actually present
        virtual Status PrepareChunk(void);

        // writes non-PES headers (pack, system) into header buffer
        virtual void GenerateNonPESHeaders(void);

        // writes PES header into header buffer
        void GeneratePESHeader(void);

        // writes special headers into header buffer
        virtual void GenerateSpecialHeader(void);

        // writes specified number of padding bytes
        Status WritePadding(Ipp32s iNOfPaddingBytes);

    protected:
        MPEG2MuxerSample payload; // prepared payload
        Ipp16u m_uiPacketSize; // aggregate size of PS chunk including payload and all headers
        Ipp8u m_uiSpecialHeaderSize; // size of special header (AC3, LPCM)
        Ipp8u *m_pHeaderBuf; // pointer to header buffer
        Ipp32u m_uiHeaderBufSize; // number of bytes used in header buffer

        // Pack header section
        Ipp32u m_uiPackHeaderSize; // 14 bytes (if present)

        // System header section
        Ipp32u m_uiSystemHeaderSize;
        Ipp32u m_uiSystemHeaderLength; // is assigned itself
        Ipp32u m_uiSystemRateBound; // is assigned once at initializing
        Ipp32u m_uiSystemAudioBound; // is assigned once at initializing
        Ipp32u m_uiSystemVideoBound; // is assigned once at initializing
        Ipp32u m_uiSystemNumberOfStreams; // is assigned once at initializing
        Ipp32u *m_pSystemStreamID; // is assigned once at initializing
        Ipp32u *m_pSystemBoundScale; // is calculated at initializing
        Ipp32u *m_pSystemSizeBound; // is assigned once at initializing

        // PES header section
        Ipp32u m_uiStreamID;

        // special header fields
        Ipp32u m_uiSubStreamID;
        Ipp32u m_uiOffset; // offset of first frame in current packet
        Ipp32u m_uiNextOffset; // offset of first frame in next packet
        Ipp32u m_uiNOfFrames; // number of frames in packet
        Ipp32u m_uiFrameNumber; // number of first frame [0...19]

        // PES fields
        Ipp32u m_uiPTSDTSFlags; // field PTS_DTS_flags
        Ipp32u m_uiPESPacketLength; // field PES_packet_length
        Ipp32u m_uiPESHeaderDataLength; // field PES_header_data_length
        Ipp32u m_uiNOfStuffingBytes; // number of stuffing bytes in PES packet
        Ipp32u m_uiMaxPayloadSize; // maximum available space for payload
        Ipp32u m_uiPayloadSize; // available space for payload taking into account properties of payload (time, size)
        Ipp32u m_uiNOfPaddingBytes; // number of padding bytes is needed after this packet
    };

    class MPEG2TSChunkWriterParams : public MPEG2PSChunkWriterParams
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2TSChunkWriterParams, MPEG2PSChunkWriterParams)

        MPEG2TSChunkWriterParams(void)
        {
            uiPID = 0;
            bIsPCRPID = false;
        };

        Ipp32u uiPID;
        bool bIsPCRPID;
    };

    class MPEG2TSChunkWriter : public MPEG2PSChunkWriter
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2TSChunkWriter, MPEG2PSChunkWriter)

        MPEG2TSChunkWriter(void);
        ~MPEG2TSChunkWriter(void);
        Status Init(MediaReceiverParams *pInit);
        Status Close(void);
        Status WriteChunk(void);

    protected:
        Status UnLockOutputBuffer(MPEG2MuxerSample *pData);
        Status PrepareChunk(void);
        void GenerateNonPESHeaders(void);
        void GenerateSpecialHeader(void);

    protected:
        Ipp32u m_uiPID;
        bool m_bIsPCRPID;

        Ipp64f m_dPCR;
        Ipp8u m_uiPCRFlag;
        Ipp8u m_uiStartIndicator;
        Ipp8u m_uiCounter;
        Ipp8u m_uiAdaptFieldCtrl;
        Ipp8u m_uiAdaptFieldLen;
        Ipp8u m_uiNOfTSStuffingBytes;
    };

    struct TsProgramNumbers
    {
        TsProgramNumbers() : pProgramPid(0), pProgramNum(0) {}
        Ipp16u pProgramPid;
        Ipp16u pProgramNum;
    };

    class PATTableWriterParams : public MPEG2ChunkWriterParams
    {
    public:
        DYNAMIC_CAST_DECL(PATTableWriterParams, MPEG2ChunkWriterParams)

        PATTableWriterParams()
        {
            iNOfPrograms = 0;
            pProgramNumbers = NULL;
            transportStreamId = MPEG2MUX_TS_ID;
        }

        Ipp32s iNOfPrograms;
        TsProgramNumbers* pProgramNumbers;
        Ipp16u transportStreamId;
    };

    class PATTableWriter : public MPEG2ChunkWriter
    {
    public:
        DYNAMIC_CAST_DECL(PATTableWriter, MPEG2ChunkWriter)

        PATTableWriter(void);
        ~PATTableWriter(void);
        Status Init(MediaReceiverParams *pInit);
        Status Close(void);
        Status WriteChunk(void);

    protected:
        Ipp8u m_uiCounter;
        Ipp8u m_pHeaderBuf[MPEG2MUX_TS_PACKET_LENGTH];
    };

    class PMTTableWriterParams : public MPEG2ChunkWriterParams
    {
    public:
        DYNAMIC_CAST_DECL(PMTTableWriterParams, MPEG2ChunkWriterParams)

        PMTTableWriterParams()
        {
            uiProgramPID = 0;
            uiProgramNumber = 0;
            uiNOfStreams = 0;
            pStreamType = NULL;
            pPID = NULL;
            uiPCRPID = 0;
        }

        Ipp32u uiProgramPID;
        Ipp32u uiProgramNumber;
        Ipp32u uiNOfStreams;
        MPEG2MuxerESType *pStreamType;
        Ipp32u *pPID;
        Ipp32u uiPCRPID;
    };

    class PMTTableWriter : public MPEG2ChunkWriter
    {
    public:
        DYNAMIC_CAST_DECL(PMTTableWriter, MPEG2ChunkWriter)

        PMTTableWriter(void);
        ~PMTTableWriter(void);
        Status Init(MediaReceiverParams *pInit);
        Status Close(void);
        Status WriteChunk(void);

    protected:
        Ipp32u m_uiProgramPID;
        Ipp32u m_uiProgramNumber;
        Ipp32u m_uiNOfStreams;
        MPEG2MuxerESType *m_pStreamType;
        Ipp32u *m_pPID;
        Ipp32u m_uiPCRPID;

        Ipp8u m_uiCounter;
        Ipp8u *m_pHeaderBuf;
    };

    class MPEG2TTSChunkWriter : public MPEG2TSChunkWriter
    {
    public:
        DYNAMIC_CAST_DECL(MPEG2TTSChunkWriter, MPEG2TSChunkWriter)

        MPEG2TTSChunkWriter(bool zeroTs) : MPEG2TSChunkWriter(), m_zeroTs(zeroTs) {}
        Status WriteChunk(void);
    private:
        bool m_zeroTs;
    };

    class TTSPATTableWriter : public PATTableWriter
    {
    public:
        DYNAMIC_CAST_DECL(TTSPATTableWriter, PATTableWriter)

        TTSPATTableWriter(bool zeroTs) : PATTableWriter(), m_zeroTs(zeroTs) {}
        Status WriteChunk(void);
    private:
        bool m_zeroTs;
    };

    class TTSPMTTableWriter : public PMTTableWriter
    {
    public:
        DYNAMIC_CAST_DECL(TTSPMTTableWriter, PMTTableWriter)

        TTSPMTTableWriter(bool zeroTs) : PMTTableWriter(), m_zeroTs(zeroTs) {}
        Status WriteChunk(void);
    private:
        bool m_zeroTs;
    };

    class IdBank
    {
    public:
        enum Type { TS_PID = 0, AUD_ES_ID, VID_ES_ID, AC3_SUB_ID, LPCM_SUB_ID };
        static const Ipp32u NO_ID;

        IdBank();
        bool IsSet(Type type, Ipp32u id) const;
        bool IsValid(Type type, Ipp32u id) const;
        void Set(Type type, Ipp32u id);
        Ipp32u GetUnusedId(Type type) const;

        void Reset(Type type, Ipp32u id);
        void Reset(Type type);
        void Reset();

    private:
        struct Constrait
        {
            Ipp32u min;
            Ipp32u max;
        };

        static const Constrait CONSTRAINTS[5];

        Ipp32u m_tsPid    [(0x1FFE - 0x0010 + 1 + 31) / 32];
        Ipp32u m_audEsId  [(0x00DF - 0x00C0 + 1 + 31) / 32];
        Ipp32u m_vidEsId  [(0x00EF - 0x00E0 + 1 + 31) / 32];
        Ipp32u m_ac3SubId [(0x0087 - 0x0080 + 1 + 31) / 32];
        Ipp32u m_lpcmSubId[(0x00A7 - 0x00A0 + 1 + 31) / 32];
        Ipp32u* m_tabs[5];
        Ipp32u m_unused[5];
    };

}; // end namespace UMC

#endif //__UMC_MPEG2_MUXER_CHUNK_H
