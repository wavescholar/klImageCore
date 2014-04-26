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

#include "umc_mpeg2ps_parser.h"

using namespace UMC;


#define PARSER_CHECK_INIT CHECK_OBJ_INIT(m_pDataReader)
#define ON_ERR_RETURN_UMC_STATUS(res) {     \
    if (UMC_OK != res) {                    \
        if (UMC_ERR_END_OF_STREAM == res)   \
            m_ParserState = END_OF_STREAM;  \
        return res;                         \
    }                                       \
}

Status Mpeg2PsParser::Init(StreamParserParams &rInit)
{
    if (m_pDataReader)
        return UMC_ERR_FAILED;

    Status umcRes = Mpeg2PesParser::Init(rInit);
    m_SystemType = UNDEF_STREAM;
    m_Pmt.Release();
    return umcRes;
}

Status Mpeg2PsParser::Close(void)
{
    PARSER_CHECK_INIT;
    m_Pmt.Release();
    return Mpeg2PesParser::Close();
}

Status Mpeg2PsParser::CheckNextData(MediaData *pData, Ipp32u* pTrack)
{
    PARSER_CHECK_INIT;
    if (END_OF_STREAM == m_ParserState)
        return UMC_ERR_END_OF_STREAM;
    if (!pData || !pTrack)
        return UMC_ERR_NULL_PTR;

    Status umcRes = UMC_OK;
    if (HEADER == m_ParserState)
    {
        m_pPacket->SetFlag(FCSample::PES_START, 1);
        m_pPacket->uiSize = 0;
        for (; UMC_OK == umcRes;)
        {
            Ipp32s iPos = 0;
            Ipp8u packet[6];
            CACHE_N_BYTES(&packet[0], 6, 0);
            if (packet[0] != 0 || packet[1] != 0 || packet[2] != 1 || packet[3] < ID_PS_PACK)
            {
                umcRes = ReSync();
                ON_ERR_RETURN_UMC_STATUS(umcRes)
                CACHE_N_BYTES(&packet[0], 6, 0);
            }

            if(IS_PES_PACKET(packet[3]))
            { // pes packet found
                umcRes = ParsePesHeader(*m_pPacket, iPos, false);
            }
            else if (ID_PS_PACK == packet[3])
            { // pack found
                umcRes = ParsePsPack(iPos);
                m_pPacket->iBufOffset = iPos;
                m_pPacket->uiSize = 0;
            }
            else if (ID_PS_SYS == packet[3])
            { // skip padding bytes
                m_pPacket->iBufOffset = 6 + GET_16U(&packet[4]);
                m_pPacket->uiSize = 0;
            }
            else if (ID_PADDING == packet[3])
            { // skip padding bytes
                m_pPacket->iBufOffset = 6 + GET_16U(&packet[4]);
                m_pPacket->uiSize = 0;
            }
            else if (ID_PS_MAP == packet[3] && !m_uiTracks)
            { // we find PMT, parse it only once and only if we have allocated no tracks
                umcRes = ParsePsPmt(m_Pmt, iPos, false);
                if (UMC_OK == umcRes)
                    umcRes = ParsePmtInfo(m_Pmt);
                if (UMC_OK != umcRes)
                    m_Pmt.Release();
                m_pPacket->iBufOffset = iPos;
                m_pPacket->uiSize = 0;
            }
            else if (ID_PRIVATE_2 == packet[3])
            {
                m_pPacket->iPid = ID_PRIVATE_2;
                m_pPacket->iBufOffset = 6;
                m_pPacket->uiSize = GET_16U(&packet[4]);
                m_pPacket->dPTS = m_pPacket->dDTS = -1.0;
            }
            else
            {
                umcRes = m_pDataReader->MovePosition(1);
                continue;
            }

            if (UMC_ERR_INVALID_STREAM == umcRes)
            {
                umcRes = m_pDataReader->MovePosition(1);
                continue;
            }
            else
            {
                ON_ERR_RETURN_UMC_STATUS(umcRes);
            }

            Ipp32s iTrack = 0;
            if (m_pPacket->uiSize > 0)
            {
                bool isNew = false;
                m_pPacket->uiAbsPos = m_pDataReader->GetPosition();
                iTrack = GetTrackByPidOrCreateNew(m_pPacket->iPid, &isNew);
                if (iTrack < 0) // cannot allocate new track, skip packet
                {
                    m_pPacket->iBufOffset += m_pPacket->uiSize;
                    m_pPacket->uiSize = 0;
                }
            }

            // packet is OK, but it does not contain payload
            // go to next packet using pes_packet_length field
            if (!m_pPacket->uiSize)
            {
                umcRes = m_pDataReader->MovePosition(m_pPacket->iBufOffset);
                ON_ERR_RETURN_UMC_STATUS(umcRes);
                continue;
            }

            // Cut private_stream_1 headers
            if (TRACK_AC3 == m_pInfo[iTrack]->m_type || TRACK_DTS == m_pInfo[iTrack]->m_type)
            {
                m_pPacket->iBufOffset += IPP_MIN(m_pPacket->uiSize, 4);
                m_pPacket->uiSize -= IPP_MIN(m_pPacket->uiSize, 4);
            }
            else if (TRACK_LPCM == m_pInfo[iTrack]->m_type && m_pPacket->uiSize >= 7)
            {
                m_pPacket->iBufOffset += IPP_MIN(m_pPacket->uiSize, 7);
                m_pPacket->uiSize -= IPP_MIN(m_pPacket->uiSize, 7);
            }

            break;
        }
    }

    m_ParserState = PAYLOAD;
    pTrack[0] = GetTrackByPid(m_pPacket->iPid);
    PacketToMediaData(*m_pPacket, *pData);
    return umcRes;
}

Status Mpeg2PsParser::SetPosition(Ipp64u pos)
{
    PARSER_CHECK_INIT;
    Status umcRes = Mpeg2PesParser::SetPosition(pos);
    if (UMC_OK != umcRes)
        return umcRes;
    ReSync();
    return umcRes;
}

Status Mpeg2PsParser::ReSync(void)
{
    // need to any chunk
    // startcodes could be easily emulated
    // so we have to be sure that found chunk is followed by another valid chunk

    // for most of chunks 6 bytes is enough to identify it and to determine its size
    // but for pack we need 14 bytes to determine its size
    Ipp8u data[14];

    Ipp64u uiOffset;
    Ipp64u uiMaxOffset = 0x10000;
    for (uiOffset = 0; uiOffset < uiMaxOffset; uiOffset++)
    {
        CACHE_N_BYTES(&data[0], 14, uiOffset);
        if (0 != data[0] || 0 != data[1] || 1 != data[2] || data[3] < ID_PS_PACK)
            continue;

        Ipp64u uiNextChunk;
        if (ID_PS_PACK == data[3])
        {
            if (0x40 == (data[4] & 0xc0) && MPEG1_SYSTEM_STREAM != m_SystemType)
            {
                m_SystemType = MPEG2_PROGRAMM_STREAM;
                uiNextChunk = 14 + (data[13] & 7);
            }
            else if (0x20 == (data[4] & 0xf0) && MPEG2_PROGRAMM_STREAM != m_SystemType)
            {
                m_SystemType = MPEG1_PROGRAMM_STREAM;
                uiNextChunk = 12;
            }
            else // seems it was pack start code emulation
                continue;
        }
        else
        {
            uiNextChunk = GET_16U(&data[4]); // pes_packet_length
            if (0 == uiNextChunk)
                continue; // seems it was pes packet start code emulation
            uiNextChunk += 6;
        }

        // check following chunk
        CACHE_N_BYTES(&data[0], 4, uiOffset + uiNextChunk);
        if (0 == data[0] && 0 == data[1] && 1 == data[2] && data[3] >= ID_PS_PACK)
            break;
    }

    if (uiOffset < uiMaxOffset)
        m_pDataReader->MovePosition(uiOffset);
    return uiOffset < uiMaxOffset ? UMC_OK : UMC_ERR_END_OF_STREAM;
}

Ipp32s Mpeg2PsParser::GetTrackByPidOrCreateNew(Ipp32s iPid, bool *pIsNew)
{
    Ipp32s iTrack = StreamParser::GetTrackByPidOrCreateNew(iPid, pIsNew);
    if (iTrack >= 0 && pIsNew && pIsNew[0] && !m_Pmt.uiSecLen)
    { // new track and we have no PMT, use stream_id to determine track type
        if (IS_ID_OF(ID_VIDEO, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_MPEG2V;
        else if (IS_ID_OF(ID_AUDIO, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_MPEGA;
        else if (IS_ID_OF(SUB_ID_AC3, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_AC3;
        else if (IS_ID_OF(SUB_ID_DTS, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_DTS;
        else if (IS_ID_OF(SUB_ID_LPCM, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_LPCM;
        else if (IS_ID_OF(SUB_ID_SUBPIC, m_pPacket->iPid))
            m_pInfo[iTrack]->m_type = TRACK_SUB_PIC;
        else if (ID_PRIVATE_2 == m_pPacket->iPid)
            m_pInfo[iTrack]->m_type = TRACK_DVD_NAV;
        else
        { // oops, unsupported track type, rollback
            m_uiTracks--;
            iTrack = -1;
        }

        if (iTrack >= 0)
        { // allocate StreamInfo
            if (!m_pInfo[iTrack]->m_pStreamInfo)
                m_pInfo[iTrack]->Alloc();

            // for LPCM audio track we have to extract channel info from 7-byte header
            if (TRACK_LPCM == m_pInfo[iTrack]->m_type && m_pInfo[iTrack]->m_pStreamInfo && m_pPacket->uiSize >= 7)
            {
                Ipp8u chan = 0;
                m_pDataReader->Check8u(&chan, m_pPacket->iBufOffset + 5);
                ((AudioStreamInfo *)m_pInfo[iTrack]->m_pStreamInfo)->audioInfo.m_iChannels =
                    AudioFrameConstructor::LPCMChannels[chan & 0x0F];
            }
        }
    }

    return iTrack;
}

Status Mpeg2PsParser::ParsePsPack(Ipp32s &iPos)
{
    Ipp32s iInnerPos = iPos;
    Ipp8u packet[14]; // this is enough for both mpeg2 and mpeg1 pack
    CACHE_N_BYTES(&packet[0], 14, iPos);
    if (packet[0] != 0 || packet[1] != 0 || packet[2] != 1 || packet[3] != ID_PS_PACK)
        return UMC_ERR_INVALID_STREAM;

    if (0x40 == (packet[4] & 0xc0))
    {
        if (UNDEF_STREAM == m_SystemType)
            m_SystemType = MPEG2_PROGRAMM_STREAM;
        else if (MPEG2_PROGRAMM_STREAM != m_SystemType)
            return UMC_ERR_INVALID_STREAM;
        iInnerPos += 14 + (packet[13] & 7);
    }
    else if (0x20 == (packet[4] & 0xf0))
    {
        if (UNDEF_STREAM == m_SystemType)
            m_SystemType = MPEG1_PROGRAMM_STREAM;
        else if (MPEG1_PROGRAMM_STREAM != m_SystemType)
            return UMC_ERR_INVALID_STREAM;
        iInnerPos += 12;
    }

    CACHE_N_BYTES(&packet[0], 6, iInnerPos);
    if (0 == packet[0] && 0 == packet[1] && 1 == packet[2] && ID_PS_SYS == packet[3])
        iInnerPos += 6 + GET_16U(&packet[4]);

    iPos += iInnerPos;
    return UMC_OK;
}

Status Mpeg2PsParser::ParsePsPmt(Mpeg2TsPmt &pmt, Ipp32s &iPos, bool bDetectChanges)
{
    if (pmt.uiSecLen > 0 && !bDetectChanges)
        return UMC_OK;

    Ipp32s iInnerPos = 0;
    Ipp8u packet[1024]; // this is maximum length of Program Stream map
    CACHE_N_BYTES(&packet[0], 6, iPos);
    if (packet[0] != 0 || packet[1] != 0 || packet[2] != 1 || packet[3] != ID_PS_MAP)
        return UMC_ERR_INVALID_STREAM;

    Ipp32s map_length = GET_16U(&packet[4]);
    if (map_length < 10 || map_length > 1018)
        return UMC_ERR_INVALID_STREAM;

    CACHE_N_BYTES(&packet[6], map_length, iPos + 6);
    Ipp8u version_number = packet[iInnerPos + 6] & 0x1f;
    Ipp32s prog_info_len = GET_16U(&packet[8]);
    Ipp8u *pProgInfo = &packet[10];
    if (prog_info_len + 10 > map_length)
        return UMC_ERR_INVALID_STREAM;

    iInnerPos += 10 + prog_info_len;
    Ipp32s es_map_len = GET_16U(&packet[iInnerPos]);
    if (iInnerPos + 2 + es_map_len > map_length + 6)
        return UMC_ERR_INVALID_STREAM;

    // first count elementary streams
    iInnerPos += 2;
    Ipp32s iEsMapStart = iInnerPos;
    Ipp32s iEsCount = 0;
    while (iInnerPos - iEsMapStart + 4 <= es_map_len)
    {
        iEsCount++;
        iInnerPos += 4 + GET_16U(&packet[iInnerPos + 2]);
    }

    // return to start and parse
    iInnerPos = iEsMapStart;
    if (!pmt.uiProgInfoLen)
    {
        pmt.pESs = new Mpeg2Es[iEsCount];
        if (!pmt.pESs)
        {
            pmt.Release();
            return UMC_ERR_ALLOC;
        }
        pmt.uiProgInfoLen = (Ipp16u)prog_info_len;
        if (prog_info_len > 0)
            pmt.SetInfo(pProgInfo, (Ipp16u)prog_info_len);
    }

    Ipp32s i;
    for (i = 0; i < iEsCount; i++)
    {
        pmt.pESs[i].uiType = packet[iInnerPos];
        pmt.pESs[i].uiPid = packet[iInnerPos + 1];
        pmt.pESs[i].uiEsInfoLen = GET_16U(&packet[iInnerPos + 2]);
        if (pmt.pESs[i].uiEsInfoLen > 0)
            pmt.pESs[i].SetInfo(&packet[iInnerPos + 4], (Ipp16u)pmt.pESs[i].uiEsInfoLen);
        iInnerPos += 4 + pmt.pESs[i].uiEsInfoLen;
    }

    if (!pmt.uiProgInfoLen)
    {
        pmt.uiESs = (Ipp32u)iEsCount;
        pmt.uiSecLen = (Ipp16u)map_length;
        pmt.uiVer = version_number;
    }

    iPos += iInnerPos + 4;
    return UMC_OK;
}

Status Mpeg2PsParser::GetSystemTime(CheckPoint &rCheckPoint, Ipp64u upToPos)
{
    PARSER_CHECK_INIT;
    Ipp8u packet[14];
    for (;; m_pDataReader->MovePosition(GET_16U(&packet[4])))
    {
        Ipp64u pos = m_pDataReader->GetPosition();
        if (pos >= upToPos)
            return UMC_ERR_SYNC;

        Status umcRes = ReSync();
        if (UMC_OK != umcRes)
            return umcRes;

        CACHE_N_BYTES(&packet[0], 14, 0);
        if (ID_PS_PACK != packet[3])
            continue;

        Ipp32u uiNextChunk;
        if (MPEG2_PROGRAMM_STREAM == m_SystemType)
        {
            rCheckPoint.dTime = GetMpeg2PsSysTime(&packet[4]);
            uiNextChunk = 14 + (packet[13] & 7);
        }
        else
        {
            rCheckPoint.dTime = GetMpeg1PsSysTime(&packet[4]);
            uiNextChunk = 12;
        }
        rCheckPoint.uiPos = pos;
        umcRes = m_pDataReader->MovePosition(uiNextChunk);
        if (UMC_OK != umcRes)
            return umcRes;
        break;
    }

    return UMC_OK;
}

#endif
