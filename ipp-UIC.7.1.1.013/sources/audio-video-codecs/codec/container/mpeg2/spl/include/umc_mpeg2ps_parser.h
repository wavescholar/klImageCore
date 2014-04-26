/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2PS_PARSER_H__
#define __UMC_MPEG2PS_PARSER_H__

#include "umc_stream_parser.h"

namespace UMC
{
    class Mpeg2PsParser : public Mpeg2PesParser
    {
    public:
        DYNAMIC_CAST_DECL(Mpeg2PsParser, Mpeg2PesParser)

        virtual Status Init(StreamParserParams &rInit);
        virtual Status Close(void);
        virtual Status CheckNextData(MediaData* data, Ipp32u* pTrack);
        virtual Status SetPosition(Ipp64u bytePos);
        virtual Status GetSystemTime(CheckPoint &rCheckPoint, Ipp64u upToPos);

    protected:
        Status ReSync(void);
        Ipp32s GetTrackByPidOrCreateNew(Ipp32s iPid, bool *pIsNew);
        Status ParsePsPmt(Mpeg2TsPmt &pmt, Ipp32s &iPos, bool bDetectChanges);
        Status ParsePsPack(Ipp32s &iPos);

        // used if Program Stream map presents
        Mpeg2TsPmt m_Pmt;
    };
}

#endif /* __UMC_MPEG2PS_PARSER_H__ */
