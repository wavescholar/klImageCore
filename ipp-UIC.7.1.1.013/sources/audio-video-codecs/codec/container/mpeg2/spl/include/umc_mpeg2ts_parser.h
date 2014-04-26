/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2TS_PARSER_H__
#define __UMC_MPEG2TS_PARSER_H__

#include "umc_stream_parser.h"

#define TS_PACKET_SIZE 188
#define TTS_PACKET_SIZE 192

namespace UMC
{
    class Mpeg2TsParserParams : public StreamParserParams
    {
    public:
        DYNAMIC_CAST_DECL(Mpeg2TsParserParams, StreamParserParams)

        Mpeg2TsParserParams(void)
        {
            m_bDetectPSIChanges = false;
        }

        bool m_bDetectPSIChanges;
    };

    class Mpeg2TsParser : public Mpeg2PesParser
    {
    public:
        DYNAMIC_CAST_DECL(Mpeg2TsParser, Mpeg2PesParser)

        Mpeg2TsParser();
        virtual Status Init(StreamParserParams &init);
        virtual Status Close(void);
        virtual Status CheckNextData(MediaData* data, Ipp32u* pTrack);
        virtual Status SetPosition(Ipp64u pos);
        virtual Status GetSystemTime(CheckPoint &rCheckPoint, Ipp64u upToPos);
        virtual Status MoveToNextHeader(void);

    protected:
        Status ReSync(void);
        Status DispatchPacket(Packet &packet, Ipp32s &iPos, bool bExtractPcr);
        Status ParseTsPat(Mpeg2TsPat &pat, Ipp32s iPos, bool bDetectChanges);
        Status ParseTsPmt(Mpeg2TsPmt &pmt, Ipp32s iPos, bool bDetectChanges);
        Status DispatchPacketWithPid(Packet &packet, Ipp32s &iPos, bool bExtractPcr);

        // informs for PMT parser that PAT was changed
        // this flag is reseted at reposition, so problems are possible
        bool m_bPatWasChanged;
        bool m_bPmtWasChanged;
        bool m_bDetectPSIChanges;
        bool m_bPcrPresent;
        Ipp32s m_iPacketSize;
        Ipp32s m_iSysTimePid;

        // absolute position of first PES packet for every track
        Ipp64u m_uiFirstPesPos[MAX_TRACK];
        // PTS of last PES for every track
        Ipp64f m_dLastPesPts[MAX_TRACK];
        // DTS of last PES for every track
        Ipp64f m_dLastPesDts[MAX_TRACK];
        // absolute position of last PES for every track
        Ipp64u m_uiLastPesPos[MAX_TRACK];
        // in most cases it's 0, but for broken streams it could be more than 0
        // this value used for resync
        Ipp32s m_iOrig;
        // PAT from Mpeg2ts
        Mpeg2TsPat m_Pat;
    };
}

#endif /* __UMC_MPEG2TS_PARSER_H__ */
