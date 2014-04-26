/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_STREAM_PARSER_H__
#define __UMC_STREAM_PARSER_H__

#include "umc_data_reader.h"
#include "umc_demuxer_defs.h"
#include "umc_frame_constructor.h"
#include "umc_index.h"

#define IS_ID_OF(ID0, ID) (((ID) & ~((ID0##_MAX) - 1)) == ID0)

#define GET_16U(PTR) (256 * (PTR)[0] + (PTR)[1])

#define CACHE_N_BYTES_FROM_READER(READER, PTR, N, OFFSET) {                     \
    size_t size = (N);                                                          \
    Status umcRes = READER->CacheData((void *)(PTR), size, (OFFSET));  \
    if (UMC_OK != umcRes || size != (size_t)(N))                                \
    {                                                                           \
        m_ParserState = END_OF_STREAM;                                          \
        return UMC_ERR_END_OF_STREAM;                                           \
    }                                                                           \
}

#define CACHE_N_BYTES(PTR, N, OFFSET) CACHE_N_BYTES_FROM_READER(m_pDataReader, PTR, N, OFFSET)

#define IS_PES_PACKET(ID) (IS_ID_OF(ID_AUDIO, ID) || IS_ID_OF(ID_VIDEO, ID) ||  \
    ID == ID_PRIVATE_1 || ID == ID_ISO_14496_SL || ID == ID_ISO_14496_FLEX ||   \
    ID == ID_RESERVED_0xFC || ID == ID_RESERVED_0xFD || ID == ID_RESERVED_0xFE)

namespace UMC
{
    class StreamParserParams
    {
    public:
        DYNAMIC_CAST_DECL_BASE(StreamParserParams)

        StreamParserParams(void);
        virtual ~StreamParserParams(void);

        SystemStreamType  m_SystemType;     // (SystemStreamType) system stream type
        DataReader       *m_pDataReader;    // (DataReader *) pointer to data reader
    };

    class StreamParser
    {
    public:
        DYNAMIC_CAST_DECL_BASE(StreamParser)

        // Constructor
        StreamParser(void);
        // Destructor
        virtual ~StreamParser(void);
        // Initialize parser
        virtual Status Init(StreamParserParams &rInit);
        // Close parser and free all resources
        virtual Status Close(void);
        // Check next data
        virtual Status CheckNextData(MediaData* data, Ipp32u* pTrack);
        // Get next data
        virtual Status GetNextData(MediaData* data, Ipp32u* pTrack);
        // Skip current payload and move to the next header
        virtual Status MoveToNextHeader(void);
        // Get number of elementary streams contained in system stream
        virtual Ipp32u GetNumberOfTracks(void);
        // Get info about specified ES (unsafe function)
        virtual Mpeg2TrackInfo *GetTrackInfo(Ipp32u uiTrack);
        // returns system type of media stream
        virtual SystemStreamType GetSystemStreamType(void);
        // Set position
        virtual Status SetPosition(Ipp64u bytePos);
        // Reset current state of parser
        virtual void Reset(void);
        // Get current parser position
        virtual Ipp64u GetPosition(void);
        // Get byte size of stream if provided by data reader
        virtual Ipp64u GetSize(void);
        // Get duration of stream in seconds if provided by stream headers
        virtual Ipp64f GetDuration(void);
        // Get track of elementary stream with specified PID
        virtual Ipp32s GetTrackByPid(Ipp32u PID);
        // parses stream from current unless next system time is encountered
        virtual Status GetSystemTime(CheckPoint&, Ipp64u)
        { return UMC_ERR_FAILED; }

    protected:
        enum ParserState
        {
            HEADER, // parser at packet header
            PAYLOAD, // parser at packet payload
            END_OF_STREAM // parser at eos
        } m_ParserState;

        Status EstimateMPEGAudioDuration(void);
        Ipp32s GetTrackByPidOrCreateNew(Ipp32s iPid, bool *pIsNew);
        virtual void PacketToMediaData(FCSample &packet, MediaData &data);

        // unchangeable features of stream are determined at initialization
        Ipp64u m_uiSourceSize; // 0 means transcoded, network or DVD streams
        DataReader *m_pDataReader; // pointer to data reader
        SystemStreamType m_SystemType; // type of system stream
        Ipp64f m_dDuration; // duration if estimated

        // accumulated info about stream, is changed while parsing
        Ipp32u m_uiTracks; // num of encountered elementary streams
        Mpeg2TrackInfo *m_pInfo[MAX_TRACK]; // array of ESinfo structures

        struct Packet : public FCSample
        {
            // constructor
            Packet(void) { Reset(); }
            // resets all fields
            void Reset(void) { FCSample::Reset(); iPid = 0; }
            // PID of packet
            Ipp32s iPid;
        } *m_pPacket;
    };

    class WaveParser : public StreamParser
    {
    public:
        DYNAMIC_CAST_DECL(WaveParser, StreamParser)

        virtual Status CheckNextData(MediaData* data, Ipp32u* pTrack);
    };

    class Mpeg2PesParser : public StreamParser
    {
    public:
        DYNAMIC_CAST_DECL(Mpeg2PesParser, StreamParser)

    protected:
        Status ParsePesHeader(Packet &packet, Ipp32s &iPos, bool bWrapped);
        Status ParsePmtInfo(Mpeg2TsPmt &pmt);
    };

    class DescriptorNavigator
    {
        DescriptorNavigator(void) {}
    public:
        DescriptorNavigator(Ipp8u *pPtr, Ipp32u uiLen);
        Ipp8u *GetNextDescriptor(Ipp32u *pTag, Ipp32u *pLen);
    protected:
        Ipp8u *m_pPtr;
        Ipp32u m_uiLen;
    };
}

#endif /* __UMC_STREAM_PARSER_H__ */
