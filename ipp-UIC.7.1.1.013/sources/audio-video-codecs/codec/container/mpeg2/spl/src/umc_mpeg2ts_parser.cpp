/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_SPLITTER

#include "umc_mpeg2ts_parser.h"

using namespace UMC;


#define PARSER_CHECK_INIT CHECK_OBJ_INIT(m_pDataReader)
#define SKIP_TS_PACKET() m_pDataReader->MovePosition((Ipp64u)m_iPacketSize)

Mpeg2TsParser::Mpeg2TsParser()
{
}

Status Mpeg2TsParser::Init(StreamParserParams &init)
{
    Status umcRes = Mpeg2PesParser::Init(init);
    if (UMC_OK != umcRes)
        return umcRes;

    m_iSysTimePid = -1;
    m_iOrig = -1;
    m_bPatWasChanged = false;
    m_bPmtWasChanged = false;
    m_bDetectPSIChanges = false;
    m_bPcrPresent = false;
    ippsSet_8u(0, (Ipp8u *)m_uiFirstPesPos, MAX_TRACK * sizeof(Ipp64u));
    ippsSet_8u(0, (Ipp8u *)m_uiLastPesPos, MAX_TRACK * sizeof(Ipp64u));
    ippsSet_8u(0, (Ipp8u *)m_dLastPesPts, MAX_TRACK * sizeof(Ipp64f));
    ippsSet_8u(0, (Ipp8u *)m_dLastPesDts, MAX_TRACK * sizeof(Ipp64f));

    if (DynamicCast<Mpeg2TsParserParams, StreamParserParams>(&init))
        m_bDetectPSIChanges = ((Mpeg2TsParserParams *)&init)->m_bDetectPSIChanges;

    // find start of first packet
    m_iPacketSize = (MPEG2_TRANSPORT_STREAM == init.m_SystemType) ? TS_PACKET_SIZE : TTS_PACKET_SIZE;
    m_Pat.Release();
    umcRes = ReSync();
    return umcRes;
}

Status Mpeg2TsParser::Close(void)
{
    PARSER_CHECK_INIT;
    m_Pat.Release();
    m_iOrig = -1;
    return Mpeg2PesParser::Close();
}

Status Mpeg2TsParser::SetPosition(Ipp64u pos)
{
    PARSER_CHECK_INIT;
    // reset TS variables
    m_pPacket->Reset();
    ippsSet_8u(0, (Ipp8u *)m_uiLastPesPos, MAX_TRACK * sizeof(Ipp64u));
    ippsSet_8u(0, (Ipp8u *)m_dLastPesPts, MAX_TRACK * sizeof(Ipp64f));
    ippsSet_8u(0, (Ipp8u *)m_dLastPesDts, MAX_TRACK * sizeof(Ipp64f));
    m_bPatWasChanged = false;
    m_bPmtWasChanged = false;

    Status umcRes = Mpeg2PesParser::SetPosition(pos);
    if (UMC_OK != umcRes)
        return umcRes;
    return ReSync();
}

Status Mpeg2TsParser::CheckNextData(MediaData *pData, Ipp32u* pTrack)
{
    PARSER_CHECK_INIT;
    if (END_OF_STREAM == m_ParserState)
        return UMC_ERR_END_OF_STREAM;
    if (!pData || !pTrack)
         return UMC_ERR_NULL_PTR;

    if (m_bPatWasChanged || m_bPmtWasChanged)
    {
        // PSI change was detected at previous CheckNextData
        // Upper component is notified so we can destroy old track infos
        Ipp32u i;
        for (i = 0; i < m_uiTracks; i++)
        {
            m_pInfo[i]->ReleaseAll();
            delete m_pInfo[i];
            m_pInfo[i] = NULL;
        }

        if (m_bPatWasChanged)
            m_Pat.Release(); // release pat and all pmt
        if (m_bPmtWasChanged)
            for (i = 0; i < m_Pat.uiProgs; i++)
                m_Pat.pProgs[i].Release(); // release only all pmt

        m_uiTracks = 0;
        m_bPatWasChanged = m_bPmtWasChanged = false;
    }

    Status umcRes = UMC_OK;
    if (HEADER == m_ParserState)
    {
        m_pPacket->uiSize = 0;
        for (umcRes = UMC_OK; UMC_OK == umcRes; umcRes = SKIP_TS_PACKET())
        {
            Ipp32s iPos = 0;
            umcRes = DispatchPacket(*m_pPacket, iPos, false);
            if (UMC_ERR_INVALID_STREAM == umcRes)
                continue;
            else if (UMC_OK != umcRes)
                return umcRes;

            Ipp32s iProg;
            Ipp32s iTrack;
            if (ID_TS_PAT == m_pPacket->iPid)
            { // PAT found, parse it
                umcRes = ParseTsPat(m_Pat, iPos, m_bDetectPSIChanges);
            }
            else if ((iProg = m_Pat.GetProgIdx(m_pPacket->iPid)) >= 0)
            { // PMT found, parse it
                bool bFirstPmt = !m_Pat.pProgs[iProg].uiSecLen;
                umcRes = ParseTsPmt(m_Pat.pProgs[iProg], iPos, m_bDetectPSIChanges);
                if (UMC_OK == umcRes && bFirstPmt)
                    umcRes = ParsePmtInfo(m_Pat.pProgs[iProg]);
            }
            else if ((iTrack = GetTrackByPid(m_pPacket->iPid)) >= 0)
            { // track PID found
                if (m_pPacket->GetFlag(FCSample::PES_START))
                { // data follows PES header
                    if (0 == m_uiFirstPesPos[iTrack]) // save first PES header position
                        m_uiFirstPesPos[iTrack] = m_pPacket->uiAbsPos;
                    if (m_pPacket->uiAbsPos < m_uiFirstPesPos[iTrack])
                        continue; // this works after repositions to prevent 'early' samples
                    umcRes = ParsePesHeader(*m_pPacket, iPos, true);
                    m_pPacket->uiSize = (Ipp32u)(m_iPacketSize - iPos);
                }
                else if (m_uiLastPesPos[iTrack] > 0)
                { // this is continuation of PES packet
                    m_pPacket->iBufOffset = iPos;
                    m_pPacket->uiSize = (Ipp32u)(m_iPacketSize - iPos);
                }
                else
                { // have to wait for PES header
                    continue;
                }
            }
            else
            { // unregistered PID found
                continue;
            }

            if (UMC_ERR_INVALID_STREAM == umcRes)
                continue;
            else if (UMC_OK != umcRes)
                return umcRes;

            if (!m_pPacket->uiSize)
                continue;

            iTrack = GetTrackByPid(m_pPacket->iPid);
            if (iTrack < 0)
                continue;

            if (!m_pPacket->GetFlag(FCSample::PES_START))
            {
                m_pPacket->dPTS = m_dLastPesPts[iTrack];
                m_pPacket->dDTS = m_dLastPesDts[iTrack];
                m_pPacket->uiAbsPos = m_uiLastPesPos[iTrack];
            }
            else
            {
                m_dLastPesPts[iTrack] = m_pPacket->dPTS;
                m_dLastPesDts[iTrack] = m_pPacket->dDTS;
                m_uiLastPesPos[iTrack] = m_pPacket->uiAbsPos;
            }

            m_ParserState = PAYLOAD;
            break;
        }
    }

    if (UMC_ERR_END_OF_STREAM == umcRes)
        m_ParserState = END_OF_STREAM;

    *pTrack = GetTrackByPid(m_pPacket->iPid);
    PacketToMediaData(*m_pPacket, *pData);
    return umcRes;
}

Status Mpeg2TsParser::DispatchPacket(Packet &packet, Ipp32s &iPos, bool bExtractPcr)
{
    if (TTS_PACKET_SIZE == m_iPacketSize)
        iPos += 4;

    packet.uiAbsPos = m_pDataReader->GetPosition();
    Ipp8u tsHeader[12]; // that's enough for transport packet headers
    CACHE_N_BYTES(&tsHeader[0], 6, iPos);
    Ipp32s sync_byte = tsHeader[0];
    packet.SetFlag(FCSample::PES_START, (tsHeader[1] >> 6) & 0x01);
    packet.iPid = ((tsHeader[1] & 0x1f) << 8) + tsHeader[2];
    Ipp32s scrambling_ctrl = (tsHeader[3] >> 6) & 0x03;
    Ipp32s adapt_field_ctrl = (tsHeader[3] >> 4) & 0x03;
    Ipp32s adapt_field_len = tsHeader[4];

    // check forbidden values
    if (ID_TS_SYNC != sync_byte || scrambling_ctrl > 1 || ID_TS_NULL == packet.iPid ||
        ID_TS_CAT == packet.iPid || ID_TS_DT == packet.iPid || 0 == adapt_field_ctrl)
        return UMC_ERR_INVALID_STREAM;

    iPos += 4;
    if (bExtractPcr)
        packet.dPTS = -1.0;

    // skip adaptation field if present
    if (adapt_field_ctrl & 2)
    {
        if (adapt_field_len > 0)
        {
            if ((3 == adapt_field_ctrl && adapt_field_len > 182) || (2 == adapt_field_ctrl && adapt_field_len > 183))
                return UMC_ERR_INVALID_STREAM;
            if (bExtractPcr)
            {
                CACHE_N_BYTES(&tsHeader[6], 6, iPos + 2);
                if (tsHeader[5] & 0x10) // PCR_flag
                    packet.dPTS = GetMpeg2TsSysTime(&tsHeader[6]);
            }
        }
        iPos += 1 + adapt_field_len;
    }

    packet.uiAbsPos = m_pDataReader->GetPosition();
    return UMC_OK;
}

Status Mpeg2TsParser::DispatchPacketWithPid(Packet &packet, Ipp32s &iPos, bool bExtractPcr)
{
    Status umcRes = UMC_OK;
    Ipp64u upToPos = packet.uiAbsPos;
    Ipp32s iPid = packet.iPid;
    packet.uiAbsPos = 0;

    for (; m_pDataReader->GetPosition() < upToPos && UMC_OK == umcRes; umcRes = SKIP_TS_PACKET())
    {
        iPos = 0;
        umcRes = DispatchPacket(packet, iPos, bExtractPcr);
        if (UMC_ERR_INVALID_STREAM == umcRes)
            continue;
        else if (UMC_OK != umcRes)
            return umcRes;
        if (packet.iPid == iPid)
            break;
    }

    return m_pDataReader->GetPosition() < upToPos ? umcRes : UMC_ERR_FAILED;
}

Status Mpeg2TsParser::ParseTsPat(Mpeg2TsPat &pat, Ipp32s iPos, bool bDetectChanges)
{
    if (pat.uiSecLen > 0 && !bDetectChanges)
        return UMC_OK;

    // read rest of packet
    Ipp8u tsHeader[TTS_PACKET_SIZE];
    CACHE_N_BYTES(&tsHeader[iPos], TTS_PACKET_SIZE - iPos, iPos);

    Ipp32u pointer_field = tsHeader[iPos];
    iPos += 1 + pointer_field;
    if (iPos + 3 >= m_iPacketSize)
        return UMC_ERR_INVALID_STREAM;

    Ipp32u table_id = tsHeader[iPos++];
    if (0 != table_id)
        return UMC_ERR_INVALID_STREAM;

    Ipp32s section_length = GET_16U(&tsHeader[iPos]) & 0x0fff;
    if (section_length > m_iPacketSize - iPos - 2 || section_length < 9)
        return UMC_ERR_INVALID_STREAM;

    Ipp16u transport_stream_id = GET_16U(&tsHeader[iPos + 2]);
    Ipp8u version_number = (tsHeader[iPos + 4] >> 1) & 0x1f;
    Ipp32s section_number = tsHeader[iPos + 5];
    Ipp32u last_section_number = tsHeader[iPos + 6];
    if (0 != section_number || 0 != last_section_number)
        return UMC_ERR_INVALID_STREAM;

    iPos += 7;
    Ipp32s i;
    Ipp32u uiPrograms = 0;
    // first count programs
    for (i = 0; i < (section_length - 9) / 4; i++)
    {
        if (GET_16U(&tsHeader[iPos + 4 * i]) > 0)
            uiPrograms++;
    }

    if (!pat.uiSecLen && uiPrograms > 0)
    { // allocate Mpeg2TsPmts only ones
        pat.pProgs = new Mpeg2TsPmt[uiPrograms];
        if (!pat.pProgs)
            return UMC_ERR_ALLOC;
    }
    else if (bDetectChanges)
    { // check some PAT fields for changes
        if (pat.uiSecLen != section_length || pat.uiTsId != transport_stream_id ||
            pat.uiVer != version_number || pat.uiProgs != uiPrograms)
        {
            m_bPatWasChanged = true;
            return UMC_WRN_INVALID_STREAM;
        }
    }

    // parse program PID's checking chasnges
    for (i = 0; i < (Ipp32s)uiPrograms; iPos += 4)
    {
        Ipp16u program_number = GET_16U(&tsHeader[iPos + 0]);
        Ipp16u program_PID = GET_16U(&tsHeader[iPos + 2]) & 0x1fff;
        if (0 == program_number)
            continue;

        if (!pat.uiSecLen)
        { // fill program params
            pat.pProgs[i].uiProgInd = (Ipp16s)i;
            pat.pProgs[i].uiProgNum = program_number;
            pat.pProgs[i].uiProgPid = program_PID;
        }
        else if (m_bDetectPSIChanges)
        { // check changes in programs's numbers/PIDs
            if (pat.pProgs[i].uiProgNum != program_number || pat.pProgs[i].uiProgPid != program_PID)
            {
                m_bPatWasChanged = true;
                return UMC_WRN_INVALID_STREAM;
            }
        }
        i++;
    }

    if (!pat.uiSecLen)
    { // fill PAT fields only once
        pat.uiSecLen = (Ipp16u)section_length;
        pat.uiTsId = transport_stream_id;
        pat.uiVer = version_number;
        pat.uiProgs = uiPrograms;
    }

    return UMC_OK;
}

Status Mpeg2TsParser::ParseTsPmt(Mpeg2TsPmt &pmt, Ipp32s iPos, bool bDetectChanges)
{
    if (pmt.uiSecLen > 0 && !bDetectChanges)
        return UMC_OK;

    // read rest of packet
    Ipp8u tsHeader[TTS_PACKET_SIZE];
    CACHE_N_BYTES(&tsHeader[iPos], TTS_PACKET_SIZE - iPos, iPos);

    Ipp32u pointer_field = tsHeader[iPos];
    iPos += 1 + pointer_field;
    if (iPos + 3 >= m_iPacketSize)
        return UMC_ERR_INVALID_STREAM;

    Ipp32u table_id = tsHeader[iPos++];
    if (2 != table_id)
        return UMC_ERR_INVALID_STREAM;

    Ipp32s section_length = GET_16U(&tsHeader[iPos]) & 0xfff;
    Ipp32s section_end = (Ipp16u)(iPos + 2 + section_length);
    if (section_end > m_iPacketSize)
        return UMC_ERR_INVALID_STREAM;

    Ipp8u version_number = (tsHeader[iPos + 4] >> 1) & 0x1f;
    Ipp32u section_number = tsHeader[iPos + 5];
    Ipp32u last_section_number = tsHeader[iPos + 6];
    if (0 != section_number || 0 != last_section_number)
        return UMC_ERR_INVALID_STREAM; // multi-section tables are not supported

    Ipp16u PCR_PID = GET_16U(&tsHeader[iPos + 7]) & 0x1fff;
    Ipp32s program_info_length = GET_16U(&tsHeader[iPos + 9]) & 0x0fff;
    if (iPos + 11 + program_info_length + 4 > section_end)
        return UMC_ERR_INVALID_STREAM;

    iPos += 11; // points program info
    Ipp8u *pProgInfo = &tsHeader[iPos];
    iPos += program_info_length;

    //first count ESs
    Ipp32u uiESs = 0;
    Ipp32s iTmpPos = iPos;
    Ipp32s iBytesLeft = section_length - 13 - program_info_length;
    for (uiESs = 0; iTmpPos + 5 < section_end; uiESs++)
    {
        Ipp32u ES_info_length = GET_16U(&tsHeader[iTmpPos + 3]) & 0x0fff;
        if (ES_info_length >= 1023) return UMC_ERR_INVALID_STREAM;
        iTmpPos += 5 + ES_info_length;
        iBytesLeft -= 5 + ES_info_length;
    }

    if (!pmt.uiSecLen && uiESs > 0)
    { // allocate Mpeg2TsPmts only ones
        pmt.pESs = new Mpeg2Es[uiESs];
        if (!pmt.pESs)
        {
            pmt.Release();
            return UMC_ERR_ALLOC;
        }
        pmt.uiProgInfoLen = (Ipp16u)program_info_length;
        if (program_info_length > 0)
            pmt.SetInfo(pProgInfo, (Ipp16u)program_info_length);
    }
    else if (bDetectChanges)
    { // check some PMT fields for changes
        if (pmt.uiSecLen != section_length || pmt.uiVer != version_number ||
            pmt.uiProgInfoLen != program_info_length || pmt.uiESs != uiESs)
        {
            m_bPmtWasChanged = true;
            return UMC_WRN_INVALID_STREAM;
        }
    }

    // parse ESs PIDs/types checking changes
    Ipp32u i;
    Ipp32s ES_info_length = 0;
    iBytesLeft = section_length - 13 - program_info_length;
    for (i = 0; i < uiESs; i++, iPos += 5 + ES_info_length)
    {
        Ipp8u stream_type = tsHeader[iPos];
        Ipp16u elementary_PID = GET_16U(&tsHeader[iPos + 1]) & 0x1fff;
        ES_info_length = GET_16U(&tsHeader[iPos + 3]) & 0x0fff;

        if (!pmt.uiSecLen)
        { // first PMT
            pmt.pESs[i].uiType = stream_type;
            pmt.pESs[i].uiPid = elementary_PID;
            pmt.pESs[i].uiEsInfoLen = ES_info_length;
            if (ES_info_length > 0)
                pmt.pESs[i].SetInfo(&tsHeader[iPos + 5], (Ipp16u)ES_info_length);
            pmt.uiESs++;
        }
        else if (bDetectChanges)
        { // check ES description
            if (pmt.pESs[i].uiPid != elementary_PID || pmt.pESs[i].uiType != stream_type ||
                pmt.pESs[i].uiEsInfoLen != (Ipp16u)ES_info_length)
            {
                m_bPmtWasChanged = true;
                return UMC_WRN_INVALID_STREAM;
            }
        }
    }

    if (!pmt.uiSecLen)
    { // firsr PMT, fill fields
        pmt.uiPcrPid = PCR_PID;
        pmt.uiSecLen = (Ipp16u)section_length;
        pmt.uiVer = version_number;
    }

    iPos += 4;
    return UMC_OK;
}

Status Mpeg2TsParser::ReSync(void)
{
    if (m_iOrig < 0)
    {
        for (m_iOrig = 0; m_iOrig < m_iPacketSize; m_iOrig++)
        {
            Status umcRes;
            Ipp8u byte1, byte2, byte3;
            umcRes = m_pDataReader->Check8u(&byte1, m_iOrig + 1 * m_iPacketSize - TS_PACKET_SIZE);
            umcRes = m_pDataReader->Check8u(&byte2, m_iOrig + 2 * m_iPacketSize - TS_PACKET_SIZE);
            umcRes = m_pDataReader->Check8u(&byte3, m_iOrig + 3 * m_iPacketSize - TS_PACKET_SIZE);
            if (UMC_OK != umcRes)
                return UMC_ERR_FAILED;
            if (ID_TS_SYNC == byte1 && ID_TS_SYNC == byte2 && ID_TS_SYNC == byte3)
                break;
        }
        if (m_iOrig >= m_iPacketSize)
        {
            m_iOrig = -1;
            return UMC_ERR_FAILED;
        }
    }

    Ipp64u uiCurPos = m_pDataReader->GetPosition();
    Ipp64u uiPos = IPP_MAX(uiCurPos, (Ipp64u)m_iOrig);
    uiPos = ((uiPos - m_iOrig + m_iPacketSize - 1) / m_iPacketSize) * m_iPacketSize + m_iOrig;
    return uiPos > uiCurPos ? m_pDataReader->MovePosition(uiPos - uiCurPos) : UMC_OK;
}

// convert stream type from mpeg2ts pmt into internal TrackType
TrackType PmtStreamTypeToTrackType(Ipp8u uiStreamType)
{
    switch (uiStreamType)
    {
    case 0x01: return TRACK_MPEG1V;
    case 0x02: return TRACK_MPEG2V;
    case 0x03: return TRACK_MPEGA;
    case 0x04: return TRACK_MPEGA;
    case 0x0F: return TRACK_AAC;
    case 0x10: return TRACK_MPEG4V;
    case 0x11: return TRACK_AAC;
    case 0x1A: return TRACK_H264;
    case 0x1B: return TRACK_H264;
    case 0x81: return TRACK_AC3;
    case 0x83: return TRACK_LPCM;
    default:   return TRACK_UNKNOWN;
    }
}

Status Mpeg2TsParser::GetSystemTime(CheckPoint &rCheckPoint, Ipp64u upToPos)
{
    Status umcRes = UMC_OK;
    Packet packet;

    // if pid is unspecified find out it from PAT/PMT
    for (; m_iSysTimePid < 0;)
    {
        // find PAT table
        Mpeg2TsPat pat;
        packet.iPid = ID_TS_PAT;
        for (umcRes = UMC_OK; !pat.uiProgs && UMC_OK == umcRes; umcRes = SKIP_TS_PACKET())
        {
            Ipp32s iPos = 0;
            packet.uiAbsPos = upToPos;
            umcRes = DispatchPacketWithPid(packet, iPos, false);
            if (UMC_ERR_INVALID_STREAM == umcRes)
                continue;
            if (UMC_OK == umcRes)
                umcRes = ParseTsPat(pat, iPos, false);
        }
        if (UMC_OK != umcRes)
            return umcRes;

        // find any PMT table with valid PCR_PID
        Ipp32s prog = 0;
        Ipp32u uiProgsChecked = 0;
        for (; packet.uiAbsPos < upToPos && UMC_OK == umcRes; umcRes = SKIP_TS_PACKET())
        {
            Ipp32s iPos = 0;
            umcRes = DispatchPacket(packet, iPos, false);
            if (UMC_OK == umcRes && (prog = pat.GetProgIdx(packet.iPid)) >= 0)
            {
                if (!pat.pProgs[prog].uiSecLen)
                    uiProgsChecked++;
                umcRes = ParseTsPmt(pat.pProgs[prog], iPos, false);
                if (UMC_OK != umcRes)
                    uiProgsChecked--;
                else if (pat.pProgs[prog].uiPcrPid != ID_TS_NULL || uiProgsChecked >= pat.uiProgs)
                    break; // exit if 'good' pcr_pid is found or if all programs are checked
            }
        }
        if (packet.uiAbsPos >= upToPos)
            return UMC_ERR_FAILED;
        else if (UMC_OK != umcRes)
            return umcRes;

        if (ID_TS_NULL == pat.pProgs[prog].uiPcrPid)
        { // PCR is absent in the stream, try to use PTS/DTS of one of tracks
            m_bPcrPresent = false;
            Ipp32s iVideoPid = -1;
            for (prog = 0; prog < (Ipp32s)pat.uiProgs && m_iSysTimePid < 0; prog++)
            {
                Ipp32u es;
                for (es = 0; es < pat.pProgs[prog].uiESs && m_iSysTimePid < 0; es++)
                {
                    if (PmtStreamTypeToTrackType(pat.pProgs[prog].pESs[es].uiType) & TRACK_ANY_AUDIO)
                        m_iSysTimePid = (Ipp32s)pat.pProgs[prog].pESs[es].uiPid;
                    else if (PmtStreamTypeToTrackType(pat.pProgs[prog].pESs[es].uiType) & TRACK_ANY_VIDEO)
                        iVideoPid = iVideoPid < 0 ? (Ipp32s)pat.pProgs[prog].pESs[es].uiPid : iVideoPid;
                }
            }
            if (m_iSysTimePid < 0)
                m_iSysTimePid = iVideoPid;
            if (m_iSysTimePid < 0) // critical situation, stream with nor audio neither video
                return UMC_ERR_FAILED;
        }
        else
        { // PCR presents in the stream
            m_bPcrPresent = true;
            m_iSysTimePid = pat.pProgs[prog].uiPcrPid;
        }
    }

    packet.iPid = m_iSysTimePid;
    for (; UMC_OK == umcRes; umcRes = SKIP_TS_PACKET())
    {
        Ipp32s iPos = 0;
        packet.uiAbsPos = upToPos;
        packet.dPTS = packet.dDTS = -1.0;
        umcRes = DispatchPacketWithPid(packet, iPos, m_bPcrPresent);
        if (UMC_OK != umcRes)
            return umcRes;

        if (!m_bPcrPresent)
        { // if pcr isn't present in the stream, we have to get pes packet timestamps
            if (!packet.GetFlag(FCSample::PES_START))
                continue;
            umcRes = ParsePesHeader(packet, iPos, true);
            if (UMC_ERR_INVALID_STREAM == umcRes)
                continue;
        }

        if (packet.dPTS < 0.0)
            continue;
        rCheckPoint.uiPos = packet.uiAbsPos;
        rCheckPoint.dTime = packet.dDTS < 0.0 ? packet.dPTS : packet.dDTS;
        umcRes = SKIP_TS_PACKET();
        break;
    }

    return umcRes;
}

Status Mpeg2TsParser::MoveToNextHeader(void)
{
    PARSER_CHECK_INIT;
    if (END_OF_STREAM == m_ParserState)
        return UMC_ERR_END_OF_STREAM;
    else if (HEADER == m_ParserState)
        return UMC_OK;
    m_ParserState = HEADER;
    Status umcRes = m_pDataReader->MovePosition(m_iPacketSize);
    Reset();
    return umcRes;
}

#endif
