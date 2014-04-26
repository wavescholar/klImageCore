/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_MUXER

#include "vm_debug.h"
#include "umc_mpeg2_muxer_chunk.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippdc.h"

using namespace UMC;


/*
 * MPEG2ChunkWriter
 */

MPEG2ChunkWriter::MPEG2ChunkWriter()
{
    m_pDataWriter = NULL;
} //MPEG2ChunkWriter::MPEG2ChunkWriter()

MPEG2ChunkWriter::~MPEG2ChunkWriter()
{
    Close();
} //MPEG2ChunkWriter::~MPEG2ChunkWriter()

Status MPEG2ChunkWriter::Close(void)
{
    MPEG2MuxerLinearBuffer::Close();

    m_pDataWriter = NULL;
    m_ESType = MPEG2MUX_ES_UNKNOWN;
    m_dReferenceClock = 0.0;
    m_uiMuxRate = 0;
    m_dFrameRate = 0.0;
    m_uiBitRate = 0;
    return UMC_OK;
} //Status MPEG2ChunkWriter::Close(void)

Status MPEG2ChunkWriter::Init(MediaReceiverParams *pInit)
{
    Status status;

    UMC_CHECK_FUNC(status, MPEG2MuxerLinearBuffer::Init(pInit));

    MPEG2ChunkWriterParams *pParams = DynamicCast<MPEG2ChunkWriterParams, MediaReceiverParams>(pInit);
    if (!pParams || !pParams->pDataWriter)
        return UMC_ERR_NULL_PTR;

    m_pDataWriter = pParams->pDataWriter;
    m_ESType = pParams->esType;
    m_dFrameRate = pParams->dFrameRate;
    m_uiBitRate = pParams->uiBitRate;
    return UMC_OK;
} //Status MPEG2ChunkWriter::Init(MediaReceiverParams *pInit)

void MPEG2ChunkWriter::SetReferenceClock(Ipp64f dReferenceClock)
{
    m_dReferenceClock = dReferenceClock;
} //void MPEG2ChunkWriter::SetReferenceClock(Ipp64f dReferenceClock)

void MPEG2ChunkWriter::SetMuxRate(Ipp32u uiMuxRate)
{
    m_uiMuxRate = uiMuxRate;
} //void MPEG2ChunkWriter::SetMuxRate(Ipp32u uiMuxRate)

Status MPEG2ChunkWriter::GetOutputTime(Ipp64f &dTime)
{
    if (m_uiNOfUsedSamples < 1)
        return m_bEndOfStream ? UMC_ERR_END_OF_STREAM : UMC_ERR_NOT_ENOUGH_DATA;

    dTime = m_pFirstUsedSample->dExactDTS;
    return UMC_OK;
} //Status MPEG2ChunkWriter::GetOutputTime(Ipp64f &dTime)

Status MPEG2ChunkWriter::LockInputBuffer(MediaData *pData)
{
    Status umcRes;
    MPEG2MuxerSample sample;

    umcRes = MPEG2MuxerLinearBuffer::LockInputBuffer(&sample);
    pData->SetBufferPointer(sample.pData, sample.uiSize);
    return umcRes;
} //Status MPEG2ChunkWriter::LockInputBuffer(MediaData *pData)

Status MPEG2ChunkWriter::UnLockInputBuffer(MediaData *pData, Status streamStatus)
{
    if (UMC_OK != streamStatus)
        return MPEG2MuxerLinearBuffer::UnLockInputBuffer((MPEG2MuxerSample *)NULL, streamStatus);

    MPEG2MuxerSample sample;
    sample.dPTS = pData->m_fPTSStart;
    sample.dDTS = pData->m_fPTSEnd;

    // DTS should not be more or equal PTS
    if (sample.dDTS >= sample.dPTS)
        sample.dDTS = -1.0;

    // DTS should not present for non-video streams
    if (!IsVideo(m_ESType))
        sample.dDTS = -1.0;

    // first sample should have valid time stamp and it should be not earlier than 0.3 sec
    Ipp64f time = sample.dDTS > 0.0 ? sample.dDTS : sample.dPTS;
    if (m_dNextDTS < 0.0 && time < 0.0 && m_ESType != MPEG2MUX_ES_UNKNOWN)
        return UMC_ERR_INVALID_PARAMS;

    sample.uiFrameType = TYPE_UNDEF; // should be assigned for video frames
    sample.pData = (Ipp8u *)pData->GetDataPointer();
    sample.uiSize = pData->GetDataSize();
    sample.uiTSOffset = 0; // should refer to picture start code for video frames

    // estimate duration of the sample
    if (IsVideo(m_ESType))
    {
        sample.dDuration = 1.0 / m_dFrameRate;
        SetFrameTypeAndOffset(&sample);
    }
    else
        sample.dDuration = (8.0 * sample.uiSize) / m_uiBitRate;

    return MPEG2MuxerLinearBuffer::UnLockInputBuffer(&sample, streamStatus);
} //Status MPEG2ChunkWriter::UnLockInputBuffer(MediaData *pData, Status streamStatus)

Status MPEG2ChunkWriter::WriteChunk(void)
{
    Status status;
    if (m_uiNOfUsedSamples == 0)
        return m_bEndOfStream ? UMC_ERR_END_OF_STREAM : UMC_ERR_NOT_ENOUGH_DATA;

    Ipp8u *pBuf = m_pFirstUsedSample->pData;
    size_t uiSize = m_pFirstUsedSample->uiSize;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(pBuf, uiSize));

    return MPEG2MuxerLinearBuffer::UnLockOutputBuffer(m_pFirstUsedSample);
} //Status MPEG2ChunkWriter::WriteChunk(void)

void MPEG2ChunkWriter::SetFrameTypeAndOffset(MPEG2MuxerSample *pData)
{
    Ipp8u *pBuf = pData->pData;
    size_t uiSize = pData->uiSize;
    Ipp32u uiPos = 0;

    if (MPEG2MUX_ES_MPEG1_VIDEO == m_ESType ||
        MPEG2MUX_ES_MPEG2_VIDEO == m_ESType)
    {
        for (uiPos = 0; uiPos + 5 < uiSize; uiPos++)
        {
            if (pBuf[uiPos + 0] == 0x00 &&
                pBuf[uiPos + 1] == 0x00 &&
                pBuf[uiPos + 2] == 0x01 &&
                pBuf[uiPos + 3] == 0x00)
            {
                FrameType type = (FrameType)((pBuf[uiPos + 5] >> 3) & 0x07);
                if (I_PICTURE == type)
                    pData->uiFrameType = TYPE_I_PIC;
                else if (P_PICTURE == type)
                    pData->uiFrameType = TYPE_P_PIC;
                else if (B_PICTURE == type)
                    pData->uiFrameType = TYPE_B_PIC;
                else
                    pData->uiFrameType = TYPE_UNDEF;

                pData->uiTSOffset = uiPos;
                break;
            }
        }
    }
    else if (MPEG2MUX_ES_MPEG4_VIDEO == m_ESType)
    {
        for (uiPos = 0; uiPos + 4 < uiSize; uiPos++)
        {
            if (pBuf[uiPos + 0] == 0x00 &&
                pBuf[uiPos + 1] == 0x00 &&
                pBuf[uiPos + 2] == 0x01 &&
                pBuf[uiPos + 3] == 0xb6)
            {
                FrameType type = (FrameType)((pBuf[uiPos + 4] >> 6) + 1);
                if (I_PICTURE == type)
                    pData->uiFrameType = TYPE_I_PIC;
                else if (P_PICTURE == type)
                    pData->uiFrameType = TYPE_P_PIC;
                else if (B_PICTURE == type)
                    pData->uiFrameType = TYPE_B_PIC;
                else
                    pData->uiFrameType = TYPE_UNDEF;

                pData->uiTSOffset = uiPos;
                break;
            }
        }
    }
    else if (MPEG2MUX_ES_H264_VIDEO == m_ESType)
    {
        for (uiPos = 0; uiPos + 4 < uiSize; uiPos++)
        {
            if (pBuf[uiPos + 0] == 0x00 &&
                pBuf[uiPos + 1] == 0x00 &&
                pBuf[uiPos + 2] == 0x00 &&
                pBuf[uiPos + 3] == 0x01 &&
                ((pBuf[uiPos + 4] & 0x1f) == 0x01 || (pBuf[uiPos + 4] & 0x1f) == 0x05))
            {
                if ((pBuf[uiPos + 4] & 0x1f) == 0x05)
                    pData->uiFrameType = TYPE_I_PIC;
                else
                    pData->uiFrameType = TYPE_UNDEF;

                pData->uiTSOffset = uiPos;
                break;
            }
        }
    }
} //void MPEG2ChunkWriter::SetFrameTypeAndOffset(MPEG2MuxerSample *pData)

/*
 * MPEG2PSChunkWriter
 */

MPEG2PSChunkWriter::MPEG2PSChunkWriter(void)
{
    m_pHeaderBuf = NULL;
    m_pSystemStreamID = NULL;
    m_pSystemBoundScale = NULL;
    m_pSystemSizeBound = NULL;
} //MPEG2PSChunkWriter::MPEG2PSChunkWriter(void)

MPEG2PSChunkWriter::~MPEG2PSChunkWriter(void)
{
    Close();
} //MPEG2PSChunkWriter::~MPEG2PSChunkWriter(void)

Status MPEG2PSChunkWriter::Close(void)
{
    MPEG2ChunkWriter::Close();

    m_uiPacketSize = 0;
    m_uiSpecialHeaderSize = 0;
    m_uiPackHeaderSize = 0;

    // System header section
    m_uiSystemHeaderSize = 0;
    m_uiSystemHeaderLength = 0;
    m_uiSystemRateBound = 0;
    m_uiSystemAudioBound = 0;
    m_uiSystemVideoBound = 0;
    m_uiSystemNumberOfStreams = 0;

    UMC_FREE(m_pHeaderBuf);
    UMC_FREE(m_pSystemStreamID);
    UMC_FREE(m_pSystemBoundScale);
    UMC_FREE(m_pSystemSizeBound);

    m_uiStreamID = 0;
    m_uiSubStreamID = 0;
    m_uiOffset = 0;
    m_uiNextOffset = 0;
    m_uiNOfFrames = 0;
    m_uiFrameNumber = 0;

    return UMC_OK;
} //Status MPEG2PSChunkWriter::Close(void)

Status MPEG2PSChunkWriter::Init(MediaReceiverParams *pInit)
{
    Status status;

    UMC_CHECK_FUNC(status, MPEG2ChunkWriter::Init(pInit));

    MPEG2PSChunkWriterParams *pParams = DynamicCast<MPEG2PSChunkWriterParams, MediaReceiverParams>(pInit);
    UMC_CHECK_PTR(pParams);

    m_uiSpecialHeaderSize = 0;
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
        m_uiSpecialHeaderSize = 7;
    else if (MPEG2MUX_ES_AC3_AUDIO == m_ESType)
        m_uiSpecialHeaderSize = 4;

    m_uiPacketSize = IPP_MAX(pParams->uiPacketSize, MPEG2MUX_CHUNK_HEADER_SIZE);

    UMC_ALLOC_ARR(m_pHeaderBuf, Ipp8u, MPEG2MUX_CHUNK_HEADER_SIZE);
    m_uiHeaderBufSize = 0;

    // Pack header section
    m_uiPackHeaderSize = pParams->bPackHeaderPresent ? 14 : 0;
    m_dReferenceClock = 0.0;
    m_uiMuxRate = 0;

    // System header section
    m_uiSystemHeaderSize = 0;
    m_uiSystemHeaderLength = 0;

    // PES header section
    m_uiStreamID = pParams->uiStreamID;
    m_uiSubStreamID = pParams->uiSubStreamID;
    return UMC_OK;
} //Status MPEG2PSChunkWriter::Init(MediaReceiverParams *pInit)

Status MPEG2PSChunkWriter::SetSystemHeaderParams(SystemHeaderParams *pSysHeaderParams)
{
    UMC_CHECK_PTR(pSysHeaderParams);
    UMC_CHECK_PTR(pSysHeaderParams->pSystemStreamID);
    UMC_CHECK_PTR(pSysHeaderParams->pSystemSizeBound);

    // System header section
    m_uiSystemHeaderSize = 0;
    m_uiSystemHeaderLength = 0;
    m_uiSystemRateBound = pSysHeaderParams->uiSystemRateBound;
    m_uiSystemAudioBound = pSysHeaderParams->uiSystemAudioBound;
    m_uiSystemVideoBound = pSysHeaderParams->uiSystemVideoBound;
    m_uiSystemNumberOfStreams = pSysHeaderParams->uiSystemNumberOfStreams;
    m_uiSystemHeaderLength = 6 + 3 * m_uiSystemNumberOfStreams;

    UMC_ALLOC_ARR(m_pSystemStreamID, Ipp32u, m_uiSystemNumberOfStreams);
    UMC_ALLOC_ARR(m_pSystemSizeBound, Ipp32u, m_uiSystemNumberOfStreams);
    UMC_ALLOC_ARR(m_pSystemBoundScale, Ipp32u, m_uiSystemNumberOfStreams);

    ippsCopy_8u((Ipp8u*)pSysHeaderParams->pSystemStreamID, (Ipp8u*)m_pSystemStreamID, m_uiSystemNumberOfStreams*sizeof(Ipp32s));
    ippsCopy_8u((Ipp8u*)pSysHeaderParams->pSystemSizeBound, (Ipp8u*)m_pSystemSizeBound, m_uiSystemNumberOfStreams*sizeof(Ipp32s));

    Ipp32u i;
    for (i = 0; i < m_uiSystemNumberOfStreams; i++)
    {
        m_pSystemBoundScale[i] = 1; // means video
        if (m_pSystemStreamID[i] == 0xb8 ||
            m_pSystemStreamID[i] == 0xbf ||
            (m_pSystemStreamID[i] >= 0xc0 && m_pSystemStreamID[i] <= 0xdf))
            m_pSystemBoundScale[i] = 0; // means audio
        m_pSystemSizeBound[i] /= m_pSystemBoundScale[i] ? 1024 : 128;
    }

    return UMC_OK;
} //Status MPEG2PSChunkWriter::SetSystemHeaderParams(SystemHeaderParams *pSysHeaderParams)

void MPEG2PSChunkWriter::ToggleSystemHeader(void)
{
    m_uiSystemHeaderSize = 6 + m_uiSystemHeaderLength;
} //void MPEG2PSChunkWriter::ToggleSystemHeader(void)

void MPEG2PSChunkWriter::GenerateNonPESHeaders(void)
{
    AdvancedBitStream bs;

    if (m_uiPackHeaderSize)
    {
        bs.Reset(m_pHeaderBuf, MPEG2MUX_CHUNK_HEADER_SIZE);

        Ipp32u scrExt = (Ipp32u)(((Ipp64u)(m_dReferenceClock * MPEG2MUX_SYS_CLOCK_FREQ)) % 300);
        Ipp64u scrBase = ((Ipp64u)(m_dReferenceClock * MPEG2MUX_SYS_CLOCK_FREQ)) / 300;

        bs.AddBits(0x000001BA, 32);                         // pack_start_code
        bs.AddBits(0x01, 2);                                // 01
        bs.AddBits((Ipp32u)((scrBase >> 30) & 0x7), 3);     // system_clock_reference_base [32..30]
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits((Ipp32u)((scrBase >> 15) & 0x7FFF), 15); // system_clock_reference_base [29..15]
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits((Ipp32u)(scrBase & 0x7FFF), 15);         // system_clock_reference_base [14..0]
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits(scrExt & 0x1FF, 9);                      // system_clock_reference_extension
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits(m_uiMuxRate / 50, 22);                   // program_mux_rate
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits(0x01, 1);                                // marker_bit
        bs.AddBits(0x1f, 5);                                // reserved
        bs.AddBits(0x00, 3);                                // pack_stuffing_length

        m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - m_pHeaderBuf);
    }

    if (m_uiSystemHeaderSize > 0)
    {
        bs.Reset(&m_pHeaderBuf[m_uiHeaderBufSize], MPEG2MUX_CHUNK_HEADER_SIZE - m_uiHeaderBufSize);

        bs.AddBits(0x000001BB, 32);               // pack_start_code
        bs.AddBits(m_uiSystemHeaderLength, 16);   // header_length
        bs.AddBits(0x01, 1);                      // marker_bit
        bs.AddBits(m_uiSystemRateBound / 50, 22); // rate_bound
        bs.AddBits(0x01, 1);                      // marker_bit
        bs.AddBits(m_uiSystemAudioBound, 6);      // audio_bound
        bs.AddBits(0x00, 1);                      // fixed_flag
        bs.AddBits(0x00, 1);                      // CSPS_flag
        bs.AddBits(0x00, 1);                      // system_audio_lock_flag
        bs.AddBits(0x00, 1);                      // system_video_lock_flag
        bs.AddBits(0x01, 1);                      // marker_bit
        bs.AddBits(m_uiSystemVideoBound, 5);      // video_bound
        bs.AddBits(0x00, 1);                      // packet_rate_restriction_flag
        bs.AddBits(0x7f, 7);                      // reserved_byte

        Ipp32u i;
        for (i = 0; i < m_uiSystemNumberOfStreams; i++)
        {
            bs.AddBits(m_pSystemStreamID[i], 8);   // stream_id
            bs.AddBits(0x3, 2);                    // '11'
            bs.AddBits(m_pSystemBoundScale[i], 1); // P-STD_buffer_bound_scale
            bs.AddBits(m_pSystemSizeBound[i], 13); // P-STD_buffer_size_bound
        }

        m_uiSystemHeaderSize = 0;
        m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - m_pHeaderBuf);
    }
} //void MPEG2PSChunkWriter::GenerateNonPESHeaders(void)

void MPEG2PSChunkWriter::GeneratePESHeader(void)
{
    AdvancedBitStream bs;
    bs.Reset(&m_pHeaderBuf[m_uiHeaderBufSize], MPEG2MUX_CHUNK_HEADER_SIZE - m_uiHeaderBufSize);

    bs.AddBits(0x000001, 24);               // PES start code
    bs.AddBits(m_uiStreamID, 8);            // stream_id
    bs.AddBits(m_uiPESPacketLength, 16);    // PES_packet_length
    bs.AddBits(0x02, 2);                    // '10'
    bs.AddBits(0x00, 2);                    // PES_scrambling_control
    bs.AddBits(0x00, 1);                    // PES_priority
    bs.AddBits(0x00, 1);                    // data_alignment_indicator
    bs.AddBits(0x00, 1);                    // copyright
    bs.AddBits(0x00, 1);                    // original_or_copy
    bs.AddBits(m_uiPTSDTSFlags, 2);         // PTS_DTS_flags
    bs.AddBits(0x00, 1);                    // ESCR_flag
    bs.AddBits(0x00, 1);                    // ES_rate_flag
    bs.AddBits(0x00, 1);                    // DSM_trick_mode_flag
    bs.AddBits(0x00, 1);                    // additional_copy_info_flag
    bs.AddBits(0x00, 1);                    // PES_CRC_flag
    bs.AddBits(0x00, 1);                    // PES_extension_flag
    bs.AddBits(m_uiPESHeaderDataLength, 8); // PES_header_data_length

    if (m_uiPTSDTSFlags & 0x02)
    {
        Ipp64u pts = (Ipp64u)(payload.dPTS * 90000.0 + 0.5);
        bs.AddBits(m_uiPTSDTSFlags & 3, 4);             // '0010' or '0011'
        bs.AddBits((Ipp32s)((pts >> 30) & 0x07), 3);    // PTS [32..30]
        bs.AddBits(0x01, 1);                            // marker_bit
        bs.AddBits((Ipp32s)((pts >> 15) & 0x7fff), 15); // PTS [29..15]
        bs.AddBits(0x01, 1);                            // marker_bit
        bs.AddBits((Ipp32s)(pts & 0x7fff), 15);         // PTS [14..0]
        bs.AddBits(0x01, 1);                            // marker_bit
    }
    if (0x03 == m_uiPTSDTSFlags)
    {
        Ipp64u dts = (Ipp64u)(payload.dDTS * 90000.0 + 0.5);
        bs.AddBits(0x01, 4);                            // '0001'
        bs.AddBits((Ipp32s)((dts >> 30) & 0x07), 3);    // DTS [32..30]
        bs.AddBits(0x01, 1);                            // marker_bit
        bs.AddBits((Ipp32s)((dts >> 15) & 0x7fff), 15); // DTS [29..15]
        bs.AddBits(0x01, 1);                            // marker_bit
        bs.AddBits((Ipp32s)(dts & 0x7fff), 15);         // DTS [14..0]
        bs.AddBits(0x01, 1);                            // marker_bit
    }

    Ipp32u i;
    for (i = 0; i < m_uiNOfStuffingBytes; i++)
        bs.AddBits(0xff, 8); // stuffing_byte

    m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - m_pHeaderBuf);
} //void MPEG2PSChunkWriter::GeneratePESHeader(void)

void MPEG2PSChunkWriter::GenerateSpecialHeader(void)
{
    AdvancedBitStream bs;
    bs.Reset(&m_pHeaderBuf[m_uiHeaderBufSize], MPEG2MUX_CHUNK_HEADER_SIZE - m_uiHeaderBufSize);

    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
    {
        bs.AddBits(m_uiSubStreamID, 8); // LPCM_ID (0xA0...0xA7)
        bs.AddBits(m_uiNOfFrames, 8);   // number_of_frames
        bs.AddBits(m_uiOffset + 4, 16); // first_access_unit_pointer (4)
        bs.AddBits(0x00, 1);            // emphasis_flag (off)
        bs.AddBits(0x00, 1);            // mute_flag (off)
        bs.AddBits(0x00, 1);            // reserved
        bs.AddBits(m_uiFrameNumber, 5); // number_of_frame
        bs.AddBits(0x00, 2);            // sample_size_code (16bit)
        bs.AddBits(0x00, 2);            // sample_rate_code (48k)
        bs.AddBits(0x09, 4);            // number_of_channels_code (dual_mono)
        bs.AddBits(0x80, 8);            // dynamic_range_control

        // update special header fields
        m_uiFrameNumber = (m_uiFrameNumber + m_uiNOfFrames) % 20;
        m_uiOffset = m_uiNextOffset;
        m_uiNOfFrames = 0;
    }
    else if (MPEG2MUX_ES_AC3_AUDIO == m_ESType)
    {
        bs.AddBits(m_uiSubStreamID, 8); // AC3_ID (0x80...0x87)
        bs.AddBits(0x00, 1);            // emphasis_flag (off)
        bs.AddBits(0x00, 1);            // mute_flag (off)
        bs.AddBits(0x00, 1);            // reserved
        bs.AddBits(m_uiNOfFrames, 5);   // number_of_frames
        bs.AddBits(m_uiOffset, 16);     // offset of first access unit

        // update special header fields
        m_uiNOfFrames = 0;
        m_uiOffset = m_uiNextOffset;
    }

    m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - m_pHeaderBuf);
} //void MPEG2PSChunkWriter::GenerateSpecialHeader(void)

Status MPEG2PSChunkWriter::PrepareChunk(void)
{
    Status status;

    m_uiPESPacketLength = 0;
    m_uiPESHeaderDataLength = 0;
    m_uiNOfStuffingBytes = 0;
    m_uiNOfPaddingBytes = 0;
    m_uiPayloadSize = 0;
    m_uiPTSDTSFlags = 0x00;

    // calculate PES_packet_length of PES packet
    m_uiPESPacketLength = m_uiPacketSize;
    m_uiPESPacketLength -= m_uiPackHeaderSize; // pack header if exist
    m_uiPESPacketLength -= m_uiSystemHeaderSize; // system header if exist
    m_uiPESPacketLength -= 6; // PES_start_code and PES_header_length

    // calculate maximum space available for payload
    m_uiPayloadSize = m_uiPESPacketLength;
    m_uiPayloadSize -= 3; // PES_flags and PES_header_data_length field
    m_uiPayloadSize -= m_uiSpecialHeaderSize; // special header (AC3 or LPCM)

    // try to get payload
    payload.uiSize = m_uiPayloadSize;
    if (MPEG2MUX_ES_AC3_AUDIO == m_ESType)
        payload.uiSize += 4; // it is to be sure that whole AC3 header will be available

    UMC_CHECK_FUNC(status, MPEG2MuxerLinearBuffer::LockOutputBuffer(&payload));

    // analize time stamps
    if (payload.dPTS > 0.0)
    {
        m_uiPESHeaderDataLength += 5;
        m_uiPayloadSize -= 5;
        m_uiPTSDTSFlags = 0x02;
        if (payload.dDTS > 0.0)
        {
            m_uiPESHeaderDataLength += 5;
            m_uiPayloadSize -= 5;
            m_uiPTSDTSFlags = 0x03;
        }
    }

    //analize size of payload
    if (payload.uiSize < m_uiPayloadSize)
    {
        if (m_uiPayloadSize - payload.uiSize > 6)
        { // size of payload is much smaller, padding packet is needed
            m_uiNOfPaddingBytes = m_uiPayloadSize - (Ipp32u)payload.uiSize;
            m_uiPESPacketLength -= m_uiNOfPaddingBytes;
        }
        else
        { // size of payload is not much smaller, add stuffing bytes right to PES header
            m_uiNOfStuffingBytes = m_uiPayloadSize - (Ipp32u)payload.uiSize;
            m_uiPESHeaderDataLength += m_uiNOfStuffingBytes;
        }

        m_uiPayloadSize = (Ipp32u)payload.uiSize;
    }
    else
    {
        payload.uiSize = m_uiPayloadSize;
        if (payload.uiTSOffset >= m_uiPayloadSize)
        { // time stamps is proved to be out of this packet
            if (m_uiPTSDTSFlags & 0x02)
            { // add stuffing bytes intead of time stamps
                m_uiNOfStuffingBytes += 5;
                if (m_uiPTSDTSFlags == 0x03)
                    m_uiNOfStuffingBytes += 5;
            }

            m_uiPTSDTSFlags = 0x00;
        }
    }

    // prepare special header fields
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
    {
        if (payload.uiSize & 1)
        { // even number of byte of LPCM should be written
            m_uiPayloadSize--;
            payload.uiSize--;
            m_uiNOfStuffingBytes++;
            m_uiPESHeaderDataLength++;
        }

        m_uiNOfFrames = ((Ipp32u)payload.uiSize - m_uiOffset + MPEG2MUX_LPCM_FRAME_SIZE - 1) / MPEG2MUX_LPCM_FRAME_SIZE;
        m_uiNextOffset = m_uiOffset + m_uiNOfFrames * MPEG2MUX_LPCM_FRAME_SIZE - (Ipp32u)payload.uiSize;
    }
    else if (MPEG2MUX_ES_AC3_AUDIO == m_ESType)
    {
        Ipp8u *pBuf = payload.pData;
        Ipp32u uiPos = 0;
        m_uiFrameNumber = 0;

        if (m_uiOffset >= m_uiPayloadSize)
        { // AC3 syncword is out of this packet, update offset
            m_uiOffset -= m_uiPayloadSize;
            return UMC_OK;
        }

        if (pBuf[uiPos + m_uiOffset] != 0x0b || pBuf[uiPos + m_uiOffset + 1] != 0x77)
        { // wrong offset, look for AC3 syncword
            for (; uiPos < m_uiPayloadSize; uiPos++)
                if (pBuf[uiPos] == 0x0b && pBuf[uiPos + 1] == 0x77)
                    break;

            // offset is not found in this packet
            if (uiPos >= m_uiPayloadSize)
                return UMC_OK;

            m_uiOffset = uiPos; // correct offset is found
        }
        else
        { // correct offset, go to it
            uiPos = m_uiOffset;
        }

        m_uiFrameNumber++; // one AC3 syncword is found

        // evaluate next offset
        Ipp8u fscod = pBuf[uiPos + 4] >> 6;
        Ipp8u frmsizecod = pBuf[uiPos + 4] & 0x3F;
        if(frmsizecod > 37)
            frmsizecod = 37;
        m_uiNextOffset = 2 * AC3FrameSize[3 * frmsizecod + fscod];

        // count AC3 frames
        while (uiPos + m_uiNextOffset < m_uiPayloadSize)
        {
            if (pBuf[uiPos + m_uiNextOffset] != 0x0b || pBuf[uiPos + m_uiNextOffset + 1] != 0x77)
            { // wrong offset, look for AC3 syncword
                m_uiNextOffset = 0;
                for (; uiPos < m_uiPayloadSize; uiPos++)
                    if (pBuf[uiPos] == 0x0b && pBuf[uiPos + 1] == 0x77)
                        break;

                // no more AC3 syncwords in this packet
                if (uiPos >= m_uiPayloadSize)
                    break;

                m_uiNextOffset = uiPos; // correct offset is found
            }
            else
                uiPos += m_uiNextOffset;

            m_uiFrameNumber++;
            fscod = pBuf[uiPos + 4] >> 6;
            frmsizecod = pBuf[uiPos + 4] & 0x3F;
            m_uiNextOffset = 2 * AC3FrameSize[3 * frmsizecod + fscod];
        }

        m_uiNextOffset -= m_uiPayloadSize - uiPos;
    }

    return UMC_OK;
} //Status MPEG2PSChunkWriter::PrepareChunk(void)

Status MPEG2PSChunkWriter::WriteChunk(void)
{
    Status status;

    UMC_CHECK_FUNC(status, PrepareChunk());

    GenerateNonPESHeaders();
    GeneratePESHeader();
    if (m_uiSpecialHeaderSize > 0)
        GenerateSpecialHeader();

    // write headers
    size_t iToWrite = m_uiHeaderBufSize;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(m_pHeaderBuf, iToWrite));

#ifndef _BIG_ENDIAN_
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
        ippsSwapBytes_16u((Ipp16u *)payload.pData, (Ipp16u *)payload.pData, (Ipp32u)payload.uiSize / 2);
#endif

    // write payload data
    iToWrite = payload.uiSize;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(payload.pData, iToWrite));

    // check if packet is had to be padded
    if (m_uiNOfPaddingBytes > 0)
    {
        UMC_CHECK_FUNC(status, WritePadding(m_uiNOfPaddingBytes));
    }

    return MPEG2MuxerLinearBuffer::UnLockOutputBuffer(&payload);
} //Status MPEG2PSChunkWriter::WriteChunk(void)

static const Ipp8u paddingArray[2048] = {0x00};
Status MPEG2PSChunkWriter::WritePadding(Ipp32s iNOfPaddingBytes)
{
    Status status;
    size_t iToWrite;
    Ipp32s i, size = iNOfPaddingBytes - 6;
    if (size < 0)
        size += m_uiPacketSize;

    Ipp8u pPaddingHeader[6];
    pPaddingHeader[0] = 0x00;
    pPaddingHeader[1] = 0x00;
    pPaddingHeader[2] = 0x01;
    pPaddingHeader[3] = MPEG2MUX_PES_ID_PADDING;
    pPaddingHeader[4] = (Ipp8u)(size >> 8);
    pPaddingHeader[5] = (Ipp8u)(size & 0xFF);

    iToWrite = 6;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(pPaddingHeader, iToWrite));

    iToWrite = 2048;
    for (i = 0; i < size / 2048; i++)
        UMC_CHECK_FUNC(status, m_pDataWriter->PutData((void *)paddingArray, iToWrite));

    iToWrite = size % 2048;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData((void *)paddingArray, iToWrite));

    return UMC_OK;
} //Status MPEG2PSChunkWriter::WritePadding(Ipp32s iNOfPaddingBytes)


/*
 * MPEG2TSChunkWriter
 */

MPEG2TSChunkWriter::MPEG2TSChunkWriter()
{
} //MPEG2TSChunkWriter::MPEG2TSChunkWriter()

MPEG2TSChunkWriter::~MPEG2TSChunkWriter()
{
    Close();
} //MPEG2TSChunkWriter::MPEG2TSChunkWriter()

Status MPEG2TSChunkWriter::Close(void)
{
    MPEG2PSChunkWriter::Close();

    m_uiPID = 0;
    m_bIsPCRPID = false;

    m_dPCR = 0.0;
    m_uiPCRFlag = 0;
    m_uiStartIndicator = 0;
    m_uiCounter = 0;
    m_uiAdaptFieldCtrl = 0;
    m_uiAdaptFieldLen = 0;
    m_uiNOfTSStuffingBytes = 0;
    return UMC_OK;
} //Status MPEG2TSChunkWriter::Close(void)

Status MPEG2TSChunkWriter::Init(MediaReceiverParams *pInit)
{
    Status status;

    UMC_CHECK_FUNC(status, MPEG2PSChunkWriter::Init(pInit));

    MPEG2TSChunkWriterParams *pParams = DynamicCast<MPEG2TSChunkWriterParams, MediaReceiverParams>(pInit);
    if (!pParams)
        return UMC_ERR_NULL_PTR;

    m_uiPID = pParams->uiPID;
    m_bIsPCRPID = pParams->bIsPCRPID;

    m_uiStartIndicator = 1;
    m_uiPackHeaderSize = 0;
    m_uiSystemHeaderSize = 0;
    m_uiSpecialHeaderSize = 0;
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
        m_uiSpecialHeaderSize = 6;

    return UMC_OK;
} //Status MPEG2TSChunkWriter::Init(MediaReceiverParams *pInit)

Status MPEG2TSChunkWriter::UnLockOutputBuffer(MPEG2MuxerSample *pData)
{
    if (pData->uiSize == m_pFirstUsedSample->uiSize)
        m_uiStartIndicator = 1;

    return MPEG2MuxerLinearBuffer::UnLockOutputBuffer(pData);
} //Status MPEG2TSChunkWriter::UnLockOutputBuffer(MPEG2MuxerSample *pData)

Status MPEG2TSChunkWriter::PrepareChunk(void)
{
    // PES section
    m_uiPESPacketLength = 0;
    m_uiPTSDTSFlags = 0x00;
    m_uiPESHeaderDataLength = 0;
    m_uiNOfStuffingBytes = 0;
    m_uiPayloadSize = 0;
    m_uiNOfPaddingBytes = 0;

    // TS section
    Ipp32u uiMaxPayloadSize = MPEG2MUX_TS_PACKET_LENGTH;
    m_uiAdaptFieldCtrl = 0x01;
    m_uiAdaptFieldLen = 0;
    m_uiPCRFlag = 0;
    m_uiNOfTSStuffingBytes = 0;

    uiMaxPayloadSize -= 4; // obligatory TS packet fields

    payload = *m_pFirstUsedSample;
    if (m_uiStartIndicator && m_ESType != MPEG2MUX_ES_VBI_DATA)
    {
        if (m_bIsPCRPID)
        {
            m_uiPCRFlag = 1;
            m_uiAdaptFieldCtrl = 0x03;
            m_uiAdaptFieldLen = 7;
            uiMaxPayloadSize -= 8;

            if (m_dPCR < payload.dExactDTS - 0.3)
                m_dPCR = payload.dExactDTS - 0.3;
            else
                m_dPCR += 1.0 / MPEG2MUX_SYS_CLOCK_FREQ;

        }

        if (payload.dPTS > 0.0)
        {
            m_uiPTSDTSFlags = 0x02;
            m_uiPESHeaderDataLength += 5;
            if (payload.dDTS > 0.0)
            {
                m_uiPTSDTSFlags = 0x03;
                m_uiPESHeaderDataLength += 5;
            }
        }

        m_uiPESPacketLength = (Ipp32u)payload.uiSize;
        m_uiPESPacketLength += m_uiSpecialHeaderSize; // special header is needed for LPCM
        m_uiPESPacketLength += m_uiPESHeaderDataLength; // PTS/DTS
        m_uiPESPacketLength += 3; // PES flags and PES_header_data_length field
        if (m_uiPESPacketLength > 0xFFFF)
            m_uiPESPacketLength = 0;

        uiMaxPayloadSize -= m_uiSpecialHeaderSize;
        uiMaxPayloadSize -= m_uiPESHeaderDataLength; // PTS/DTS
        uiMaxPayloadSize -= 3; // PES flags and PES_header_data_length field
        uiMaxPayloadSize -= 6; // PES_start_code and PES_packet_length

        // prepare special header fields
        if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
        {
            m_uiNOfFrames = ((Ipp32u)payload.uiSize - m_uiOffset + MPEG2MUX_LPCM_FRAME_SIZE - 1) / MPEG2MUX_LPCM_FRAME_SIZE;
            m_uiNextOffset = m_uiOffset + m_uiNOfFrames * MPEG2MUX_LPCM_FRAME_SIZE - (Ipp32u)payload.uiSize;
        }
    }

    if (payload.uiSize < uiMaxPayloadSize)
    { // frame (or rest of frame) is smaller than available space
        if (0x03 == m_uiAdaptFieldCtrl)
        { // adapration field already exists, just add stuffing bytes
            m_uiNOfTSStuffingBytes = (Ipp8u)(m_uiNOfTSStuffingBytes + uiMaxPayloadSize - payload.uiSize);
            m_uiAdaptFieldLen = (Ipp8u)(m_uiAdaptFieldLen + m_uiNOfTSStuffingBytes);
        }
        else
        { // adapration field is needed to add stuffing bytes
            m_uiAdaptFieldCtrl = 0x03;
            m_uiAdaptFieldLen = (Ipp8u)(uiMaxPayloadSize - payload.uiSize - 1);
            if (m_uiAdaptFieldLen > 0)
                m_uiNOfTSStuffingBytes = m_uiAdaptFieldLen - 1;
        }
    }
    else
    {
        payload.uiSize = uiMaxPayloadSize;
    }

    if ((MPEG2MUX_ES_LPCM_AUDIO == m_ESType) && (payload.uiSize & 1))
    { // even number of byte of LPCM should be written
        payload.uiSize--;
        m_uiNOfStuffingBytes++;
        m_uiPESHeaderDataLength++;
    }

    return UMC_OK;
} //Status MPEG2TSChunkWriter::PrepareChunk(void)

void MPEG2TSChunkWriter::GenerateNonPESHeaders(void)
{
    AdvancedBitStream bs;
    bs.Reset(m_pHeaderBuf, MPEG2MUX_CHUNK_HEADER_SIZE);

    bs.AddBits(0x47, 8);                // sync_byte
    bs.AddBits(0x00, 1);                // transport_error_indicator
    bs.AddBits(m_uiStartIndicator, 1);  // payload_unit_start_indicator
    bs.AddBits(0x00, 1);                // transport_priority
    bs.AddBits(m_uiPID, 13);            // PID
    bs.AddBits(0x00, 2);                // transport_scrambling_control
    bs.AddBits(m_uiAdaptFieldCtrl, 2);  // adaptation_field_control
    bs.AddBits(m_uiCounter, 4);         // continuity_counter

    if (0x03 == m_uiAdaptFieldCtrl)
    {
        bs.AddBits(m_uiAdaptFieldLen, 8);   // adaptation_field_length

        if (m_uiAdaptFieldLen > 0)
        {
            bs.AddBits(0x00, 1);            // discontinuity_indicator
            bs.AddBits(0x00, 1);            // random_access_indicator
            bs.AddBits(0x00, 1);            // elementary_stream_priority_indicator
            bs.AddBits(m_uiPCRFlag, 1);     // PCR_flag
            bs.AddBits(0x00, 1);            // OPCR_flag
            bs.AddBits(0x00, 1);            // splicing_point_flag
            bs.AddBits(0x00, 1);            // transport_private_data_flag
            bs.AddBits(0x00, 1);            // adaptation_field_extension_flag

            if (m_uiPCRFlag)
            {
                Ipp64u pcr = (Ipp64u)(m_dPCR * 27000000 + 0.5);
                Ipp64u pcr_base = (pcr / 300) & CONST_LL(0x1FFFFFFFF);
                Ipp32u pcr_ext = (Ipp32u)((pcr % 300) & 0x1FF);

                bs.AddBits((Ipp32s)(pcr_base >> 32), 1);         // program_clock_reference_base[32..32]
                bs.AddBits((Ipp32s)(pcr_base & 0xFFFFFFFF), 32); // program_clock_reference_base[31..0]
                bs.AddBits(0x3F, 6);                             // reserved
                bs.AddBits((pcr_ext), 9);                        // program_clock_reference_extension
            }

            Ipp32u i;
            for (i = 0; i < m_uiNOfTSStuffingBytes; i++)
                bs.AddBits(0xFF, 8);        // stuffing byte
        }
    }

    m_uiCounter = (m_uiCounter + 1) & 0x0f;
    m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - bs.m_lpbBuffer);
} //void MPEG2TSChunkWriter::GenerateNonPESHeaders(void)

void MPEG2TSChunkWriter::GenerateSpecialHeader(void)
{
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
    {
        AdvancedBitStream bs;
        bs.Reset(&m_pHeaderBuf[m_uiHeaderBufSize], MPEG2MUX_CHUNK_HEADER_SIZE - m_uiHeaderBufSize);

        bs.AddBits(m_uiNOfFrames, 8);   // number_of_frames
        bs.AddBits(m_uiOffset + 4, 16); // first_access_unit_pointer (4)
        bs.AddBits(0x00, 1);            // emphasis_flag (off)
        bs.AddBits(0x00, 1);            // mute_flag (off)
        bs.AddBits(0x00, 1);            // reserved
        bs.AddBits(m_uiFrameNumber, 5); // number_of_frame
        bs.AddBits(0x00, 2);            // sample_size_code (16bit)
        bs.AddBits(0x00, 2);            // sample_rate_code (48k)
        bs.AddBits(0x09, 4);            // number_of_channels_code (dual_mono)
        bs.AddBits(0x80, 8);            // dynamic_range_control

        // update special header fields
        m_uiFrameNumber = (m_uiFrameNumber + m_uiNOfFrames) % 20;
        m_uiOffset = m_uiNextOffset;
        m_uiNOfFrames = 0;
        m_uiHeaderBufSize = (Ipp32u)(bs.m_lpb - m_pHeaderBuf);
    }
} //void MPEG2TSChunkWriter::GenerateSpecialHeader(void)

Status MPEG2TSChunkWriter::WriteChunk(void)
{
    Status status;

    UMC_CHECK_FUNC(status, PrepareChunk());

    GenerateNonPESHeaders();
    if (m_uiStartIndicator && m_ESType != MPEG2MUX_ES_VBI_DATA)
    {
        GeneratePESHeader();
        if (m_uiSpecialHeaderSize > 0)
            GenerateSpecialHeader();
    }

    // write headers
    size_t iToWrite = (Ipp32s)m_uiHeaderBufSize;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(m_pHeaderBuf, iToWrite));

#ifndef _BIG_ENDIAN_
    if (MPEG2MUX_ES_LPCM_AUDIO == m_ESType)
        ippsSwapBytes_16u((Ipp16u *)payload.pData, (Ipp16u *)payload.pData, (Ipp32u)payload.uiSize / 2);
#endif

    // write payload data
    iToWrite = (Ipp32s)payload.uiSize;
    UMC_CHECK_FUNC(status, m_pDataWriter->PutData(payload.pData, iToWrite));

    m_uiStartIndicator = 0;
    return UnLockOutputBuffer(&payload);
} //Status MPEG2TSChunkWriter::WriteChunk(void)


/*
 * PATTableWriter
 */

PATTableWriter::PATTableWriter()
{
    m_uiCounter = 0;
} //PATTableWriter::PATTableWriter()

PATTableWriter::~PATTableWriter()
{
    Close();
} //PATTableWriter::~PATTableWriter()

Status PATTableWriter::Close(void)
{
    MPEG2ChunkWriter::Close();
    m_uiCounter = 0;
    return UMC_OK;
} //Status PATTableWriter::Close(void)

Status PATTableWriter::Init(MediaReceiverParams *pInit)
{
    //DO NOT initialize linear buffer here
    Close();

    PATTableWriterParams *pParams = DynamicCast<PATTableWriterParams, MediaReceiverParams>(pInit);
    UMC_CHECK_PTR(pParams);
    UMC_CHECK_PTR(pParams->pDataWriter);
    UMC_CHECK_PTR(pParams->pProgramNumbers);

    m_pDataWriter = pParams->pDataWriter;
    UMC_CHECK(pParams->iNOfPrograms > 0, UMC_ERR_INVALID_PARAMS);

    ippsSet_8u(0xFF, m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    AdvancedBitStream bs;
    bs.Reset(m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    // write Program Association Table
    bs.AddBits(0x47, 8);                        // sync_byte
    bs.AddBits(0x00, 1);                        // transport_error_indicator
    bs.AddBits(0x01, 1);                        // payload_unit_start_indicator
    bs.AddBits(0x00, 1);                        // transport_priority
    bs.AddBits(0x00, 13);                       // PID
    bs.AddBits(0x00, 2);                        // transport_scrambling_control
    bs.AddBits(0x01, 2);                        // adaptation_field_control
    bs.AddBits(m_uiCounter, 4);                 // continuity_counter
    bs.AddBits(0x00, 8);                        // pointer_field
    bs.AddBits(0x00, 8);                        // table_id
    bs.AddBits(0x01, 1);                        // section_syntax_indicator
    bs.AddBits(0x00, 1);                        // '0'
    bs.AddBits(0x03, 2);                        // reserved
    bs.AddBits(4 * pParams->iNOfPrograms + 9, 12);  // section_length
    bs.AddBits(pParams->transportStreamId, 16); // transport_stream_id
    bs.AddBits(0x03, 2);                        // reserved
    bs.AddBits(0x00, 5);                        // version_number
    bs.AddBits(0x01, 1);                        // current_next_indicator
    bs.AddBits(0x00, 8);                        // section_number
    bs.AddBits(0x00, 8);                        // last_section_number

    Ipp32s i;
    for (i = 0; i < pParams->iNOfPrograms; i++)
    {
        bs.AddBits(pParams->pProgramNumbers[i].pProgramNum, 16);    // program_number
        bs.AddBits(0x07, 3);                                        // reserved
        bs.AddBits(pParams->pProgramNumbers[i].pProgramPid, 13);    // program_map_PID
    }

    Ipp32u uiCRCDataLen = 4 * pParams->iNOfPrograms + 8;
    Ipp32u uiCRC32 = 0;
    ippsCRC32_8u(bs.m_lpb - uiCRCDataLen, uiCRCDataLen, &uiCRC32);
    uiCRC32 ^= 0xFFFFFFFF;
    bs.AddBits(uiCRC32, 32);                    // CRC_32

    return UMC_OK;
} //Status PATTableWriter::Init(MediaReceiverParams *pInit)

Status PATTableWriter::WriteChunk(void)
{
    size_t iToWrite = MPEG2MUX_TS_PACKET_LENGTH;
    Status umcRes = m_pDataWriter->PutData(m_pHeaderBuf, iToWrite);

    AdvancedBitStream bs;
    bs.Reset(m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    // update continuity_counter
    m_uiCounter = (m_uiCounter + 1) & 0x0f;
    bs.SkipBits(8 + 1 + 1 + 1 + 13 + 2 + 2);
    bs.AddBits(m_uiCounter, 4);
    return umcRes;
} //Status PATTableWriter::WriteChunk(void)

/*
 * PMTTableWriter
 */

PMTTableWriter::PMTTableWriter()
{
    m_pStreamType = NULL;
    m_pPID = NULL;
    m_pHeaderBuf = NULL;
} //PMTTableWriter::PMTTableWriter()

PMTTableWriter::~PMTTableWriter()
{
    Close();
} //PMTTableWriter::~PMTTableWriter()

Status PMTTableWriter::Close(void)
{
    MPEG2ChunkWriter::Close();

    UMC_FREE(m_pHeaderBuf);
    UMC_FREE(m_pStreamType);
    UMC_FREE(m_pPID);

    m_uiProgramPID = 0;
    m_uiProgramNumber = 0;
    m_uiNOfStreams = 0;
    m_uiPCRPID = 0;
    m_uiCounter = 0;
    return UMC_OK;
} //Status PMTTableWriter::Close(void)

Status PMTTableWriter::Init(MediaReceiverParams *pInit)
{
    //DO NOT initialize linear buffer here
    Close();

    PMTTableWriterParams *pParams = DynamicCast<PMTTableWriterParams, MediaReceiverParams>(pInit);

    UMC_CHECK_PTR(pParams);
    UMC_CHECK_PTR(pParams->pDataWriter);
    UMC_CHECK_PTR(pParams->pStreamType);
    UMC_CHECK_PTR(pParams->pPID);
    UMC_CHECK(pParams->uiNOfStreams > 0, UMC_ERR_INVALID_PARAMS);

    m_pDataWriter = pParams->pDataWriter;
    m_uiNOfStreams = pParams->uiNOfStreams;
    m_uiProgramNumber = pParams->uiProgramNumber;
    m_uiProgramPID = pParams->uiProgramPID;
    m_uiPCRPID = pParams->uiPCRPID;

    UMC_ALLOC_ARR(m_pStreamType, MPEG2MuxerESType, m_uiNOfStreams);
    UMC_ALLOC_ARR(m_pPID, Ipp32u, m_uiNOfStreams);
    UMC_ALLOC_ARR(m_pHeaderBuf, Ipp8u, MPEG2MUX_TS_PACKET_LENGTH);

    ippsCopy_8u((Ipp8u*)pParams->pStreamType, (Ipp8u*)m_pStreamType, m_uiNOfStreams * sizeof(MPEG2MuxerESType));
    ippsCopy_8u((Ipp8u*)pParams->pPID, (Ipp8u*)m_pPID, m_uiNOfStreams * sizeof(Ipp32s));
    ippsSet_8u(0xFF, m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    AdvancedBitStream bs;
    bs.Reset(m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    // write Program Map Table
    bs.AddBits(0x47, 8);                        // sync_byte
    bs.AddBits(0x00, 1);                        // transport_error_indicator
    bs.AddBits(0x01, 1);                        // payload_unit_start_indicator
    bs.AddBits(0x00, 1);                        // transport_priority
    bs.AddBits(m_uiProgramPID, 13);             // PID
    bs.AddBits(0x00, 2);                        // transport_scrambling_control
    bs.AddBits(0x01, 2);                        // adaptation_field_control
    bs.AddBits(m_uiCounter, 4);                 // continuity_counter
    bs.AddBits(0x00, 8);                        // pointer_field
    bs.AddBits(0x02, 8);                        // table_id
    bs.AddBits(0x01, 1);                        // section_syntax_indicator
    bs.AddBits(0x00, 1);                        // '0'
    bs.AddBits(0x03, 2);                        // reserved
    bs.AddBits(13 + 5 * m_uiNOfStreams, 12);    // section_length
    bs.AddBits(m_uiProgramNumber, 16);          // program_number
    bs.AddBits(0x03, 2);                        // reserved
    bs.AddBits(0x00, 5);                        // version_number
    bs.AddBits(0x01, 1);                        // current_next_indicator
    bs.AddBits(0x00, 8);                        // section_number
    bs.AddBits(0x00, 8);                        // last_section_number
    bs.AddBits(0x07, 3);                        // reserved
    bs.AddBits(m_uiPCRPID, 13);                 // PRC_PID
    bs.AddBits(0x0F, 4);                        // reserved
    bs.AddBits(0x00, 12);                       // program_info_length

    Ipp32u uiNum;
    for (uiNum = 0; uiNum < m_uiNOfStreams; uiNum++)
    {
        bs.AddBits(m_pStreamType[uiNum],  8);   // stream_type
        bs.AddBits(0x07,  3);                   // reserved
        bs.AddBits(m_pPID[uiNum], 13);          // elementary_PID
        bs.AddBits(0x0F,  4);                   // reserved
        bs.AddBits(0x00, 12);                   // ES_info_length
    }

    Ipp32u uiCRC32 = 0;
    Ipp32u uiCRCDataLen = 12 + 5 * m_uiNOfStreams;
    ippsCRC32_8u(bs.m_lpb - uiCRCDataLen, uiCRCDataLen, &uiCRC32);
    uiCRC32 ^= 0xFFFFFFFF;
    bs.AddBits(uiCRC32, 32);                    // CRC_32

    return UMC_OK;
} //Status PMTTableWriter::Init(MediaReceiverParams *pInit)

Status PMTTableWriter::WriteChunk(void)
{
    size_t iToWrite = MPEG2MUX_TS_PACKET_LENGTH;
    Status umcRes = m_pDataWriter->PutData(m_pHeaderBuf, iToWrite);

    AdvancedBitStream bs;
    bs.Reset(m_pHeaderBuf, MPEG2MUX_TS_PACKET_LENGTH);

    // update continuity_counter
    m_uiCounter = (m_uiCounter + 1) & 0x0f;
    bs.SkipBits(8 + 1 + 1 + 1 + 13 + 2 + 2);
    bs.AddBits(m_uiCounter, 4);
    return umcRes;
} //Status PMTTableWriter::WriteChunk(void)


/*
 * MPEG2TTS
 */

Status MPEG2TTSChunkWriter::WriteChunk(void)
{
    size_t iToWrite = 4;
    Ipp32u uiTimeStamp = m_zeroTs ? 0 : LITTLE_ENDIAN_SWAP32((Ipp32u)(m_dReferenceClock * MPEG2MUX_SYS_CLOCK_FREQ));
    m_pDataWriter->PutData(&uiTimeStamp, iToWrite);
    return MPEG2TSChunkWriter::WriteChunk();
} //Status MPEG2TTSChunkWriter::WriteChunk(void)

Status TTSPATTableWriter::WriteChunk(void)
{
    size_t iToWrite = 4;
    Ipp32u uiTimeStamp = m_zeroTs ? 0 : LITTLE_ENDIAN_SWAP32((Ipp32u)(m_dReferenceClock * MPEG2MUX_SYS_CLOCK_FREQ));
    m_pDataWriter->PutData(&uiTimeStamp, iToWrite);
    return PATTableWriter::WriteChunk();
} //Status TTSPATTableWriter::WriteChunk(void)

Status TTSPMTTableWriter::WriteChunk(void)
{
    size_t iToWrite = 4;
    Ipp32u uiTimeStamp = m_zeroTs ? 0 : LITTLE_ENDIAN_SWAP32((Ipp32u)(m_dReferenceClock * MPEG2MUX_SYS_CLOCK_FREQ));
    m_pDataWriter->PutData(&uiTimeStamp, iToWrite);
    return PMTTableWriter::WriteChunk();
} //Status TTSPMTTableWriter::WriteChunk(void)

const IdBank::Constrait IdBank::CONSTRAINTS[5] = {
    { 0x0010, 0x1FFE },
    { 0x00C0, 0x00DF },
    { 0x00E0, 0x00EF },
    { 0x0080, 0x0087 },
    { 0x00A0, 0x00A7 }
};

const Ipp32u IdBank::NO_ID = 0;

IdBank::IdBank()
{
    m_tabs[TS_PID] = &m_tsPid[0];
    m_tabs[AUD_ES_ID] = m_audEsId;
    m_tabs[VID_ES_ID] = m_vidEsId;
    m_tabs[AC3_SUB_ID] = m_ac3SubId;
    m_tabs[LPCM_SUB_ID] = m_lpcmSubId;
    Reset();
}

void IdBank::Reset(Type type, Ipp32u id)
{
    if (id < m_unused[type])
        m_unused[type] = IPP_MIN(id, m_unused[type]);

    id -= IdBank::CONSTRAINTS[type].min;
    m_tabs[type][id / 32] &= ~(1 << (31 - (id & 0x1f)));
}

void IdBank::Reset(Type type)
{
    ippsSet_8u(0, (Ipp8u *)m_tabs[type], 4 * ((IdBank::CONSTRAINTS[type].max - IdBank::CONSTRAINTS[type].min + 1 + 31) / 32));
    m_unused[type] = IdBank::CONSTRAINTS[type].min;
}

void IdBank::Reset()
{
    Reset(TS_PID);
    Reset(AUD_ES_ID);
    Reset(VID_ES_ID);
    Reset(AC3_SUB_ID);
    Reset(LPCM_SUB_ID);
}

bool IdBank::IsValid(Type type, Ipp32u id) const
{
    return IdBank::CONSTRAINTS[type].min <= id && id <= IdBank::CONSTRAINTS[type].max;
}

bool IdBank::IsSet(Type type, Ipp32u id) const
{
    id -= IdBank::CONSTRAINTS[type].min;
    return ((m_tabs[type][id / 32] >> (31 - (id & 0x1f))) & 0x1) ? true : false;
}

void IdBank::Set(Type type, Ipp32u id)
{
    if (id == m_unused[type])
        for (m_unused[type]++; m_unused[type] <= IdBank::CONSTRAINTS[type].max; m_unused[type]++)
            if (!IsSet(type, m_unused[type]))
                break;

    id -= IdBank::CONSTRAINTS[type].min;
    m_tabs[type][id / 32] |= 1 << (31 - (id & 0x1f));
}

Ipp32u IdBank::GetUnusedId(Type type) const
{
    return m_unused[type] > IdBank::CONSTRAINTS[type].max ? IdBank::NO_ID : m_unused[type];
}

#endif
