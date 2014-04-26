/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DEMUXER_H__
#define __UMC_DEMUXER_H__

#include "umc_event.h"
#include "umc_stream_parser.h"
#include "umc_frame_constructor.h"
#include "umc_splitter.h"
#include "umc_event.h"

namespace UMC
{
// This object is used for backward playback
// It parses forward few frames (stream segment) from reference video track
// then jumps backward and parses preceded segment and so on
// Initially object should be initialized through method RestartSegmentation()
// This method should be called also after all repositions of stream parser
class Segmentator
{
public:
    // constructor
    Segmentator(void);
    // should be call before pulling segments and after parser's repositions
    void RestartSegmentation(StreamParser *pParser, Ipp32u uiTrack, VideoFrameConstructor *pVFC);
    // forces Segmentator to make additional backward jumps between segments
    // it may be called after pulling segment
    // this operation slightly differ from RestartSegmentation
    void AdditionalJump(Ipp32s iJumpSize);
    // pulls one segment from parser and put it into frame constructor
    Status PullSegment(SplMediaData *pData);

protected:
    // invokes stream parser to find packet from certain track
    Status CheckPacketFromTrack(SplMediaData *pData);
    // pointer to stream parser
    StreamParser *m_pParser;
    // video track that is used for backward
    Ipp32u m_uiTrack;
    // pointer to frame constructor related with video track
    VideoFrameConstructor *m_pVFC;
    // number of frames pulled from current segment
    Ipp32s m_iSegFrames;
    // absolute position of GOP start
    Ipp64u m_uiSegStart;
    // absolute position of GOP end
    Ipp64u m_uiSegEnd;
    // current size of segment, can be changed adaptively
    Ipp32u m_uiSegSize;
    // indicates that segment finished and reposition is needed
    bool m_bNewSegment;
    // idicates that currect segment is the last one
    bool m_bLastSegment;
    // EOS flag
    bool m_bEndOfStream;
};

// This object is used for discontinuity detection and operates through attached parser
// Demuxer uses StreamParser directly to get data and info
// When demuxer estimates duration and seeks time position it uses TimeStampCorrector
// to collect info about discontinuity and correct time stamps accodingly
class TimeStampCorrector
{
public:
    TimeStampCorrector();
    bool IsAttachedWithParser() const;
    void AttachParser(StreamParser *pParser);
    // calls GetSystemTime of attached Mpeg2PesParser and tries to correct system time
    // if it detects gap it will provide gap
    Status GetSystemTime(CheckPoint &rCheckPoint, bool *pIsGapDetectHere = NULL);
    // Does the same as GetSystemTime(), but searches system time from current pos until
    // specified position and returns UMC_ERR_SYNC if is unable to find
    Status GetSystemTimeUpTo(CheckPoint &rCheckPoint, Ipp64u upToPos, bool *pIsGapDetectHere = NULL);
    // resets m_dPrevValidSystemTime
    void ResetAfterReposition(void);

protected:
    bool CorrectTime(CheckPoint &rCheckPoint);
    // pointer to attached parser
    StreamParser* m_pParser;
    // previous valid PTS, used to detect discontinuity
    Ipp64f m_dPrevValidSystemTime;
    // container to store information about discontinuities
    LinkedList<CheckPoint> m_ListOfGaps;
};

enum
{
    // enforces demuxer to treat stream as MPEG2TS and not to parse payload of PES packets
    FLAG_VSPL_ENCRYPTED_DATA  = 0x00008000,
    // enforses internal thread to stop after init finished
    FLAG_VSPL_STOP_AFTER_INIT = 0x00010000,
    // init exits immediately after internal thread running
    FLAG_VSPL_FAST_INIT       = 0x00020000,
    // indicates that new track was enabled
    FLAG_VSPL_NEW_TRACK       = 0x00040000,
    // indicates that PAT or/and PMT table was changed
    // applied for Mpeg2 Transport Stream only
    FLAG_VSPL_PSI_CHANGED     = 0x00080000,

    // there is set of predefined initialization rules
    // they could be useful for simple applications those don't want to go into details
    // if one of presets is chosen all manually defined rules will be ignored
    // up to 15 presets can be assigned, 0 means using of manually defined rules
    FLAG_VSPL_PRESET_MASK     = 0x0000000F,

    // forses to select only 1 audio track (if present)
    // ignoring everything after initialization
    // actually this is the same as usual AUDIO_SPLITTER
    FLAG_VSPL_PRESET_1A_0V    = 0x00000001,

    // forses to select only 1 video track (if present)
    // ignoring everything after initialization
    // actually this is the same as usual VIDEO_SPLITTER
    FLAG_VSPL_PRESET_0A_1V    = 0x00000002,

    // forses to select 1 audio and video tracks (if present)
    // ignoring everything after initialization
    // actually this is the same as usual AV_SPLITTER
    FLAG_VSPL_PRESET_1A_1V    = 0x00000003,

    // forses to select 2 audio and video tracks (if present)
    // ignoring everything after initialization
    FLAG_VSPL_PRESET_2A_1V    = 0x00000004,

    // forses to select all audio and video tracks (if present)
    // ignoring everything after initialization
    FLAG_VSPL_PRESET_AA_1V    = 0x00000005,

    // forses to select all audio and all video tracks (if present)
    // ignoring everything after initialization
    FLAG_VSPL_PRESET_AA_AV    = 0x00000006
};

class DemuxerParams : public SplitterParams
{
public:
    DYNAMIC_CAST_DECL(DemuxerParams, SplitterParams)

    DemuxerParams()
    {
        m_uiRules = 0;
        m_pRules = NULL;
        m_dAudToBuf = 0.05;
        m_pOnPSIChangeEvent = NULL;
    }
    // number of rules pointed by m_pRules
    Ipp32u m_uiRules;
    // array of rules
    TrackRule *m_pRules;
    // minimal duration of audio samples
    // this option allows buffering of audio data
    // does not work for pure audio
    Ipp64f m_dAudToBuf;
    // pointer to event for notification about PSI changes
    // ThreadedDemuxer sets this event to notify application about PMT or PAT change
    // this event is actually needed only for threaded model, because internal thread is
    // unable to notify application
    Event *m_pOnPSIChangeEvent;
};

// This is core object for demultiplexing
class Demuxer : public Splitter
{
public:
    DYNAMIC_CAST_DECL(Demuxer, Splitter)

    Demuxer(void);
    virtual ~Demuxer(void);
    virtual Status Init(SplitterParams *pParams);
    virtual Status Close(void);
    virtual Status Run(void);
    virtual Status Stop(void);
    virtual Status SetTrackState(Ipp32u nTrack, TrackState iState);

    // getting data
    virtual Status CheckNextData(MediaData *data, Ipp32u uiTrack);
    virtual Status GetNextData(MediaData *data, Ipp32u uiTrack);

    // trick-modes
    virtual Status SetRate(Ipp64f rate);
    virtual Status SetPosition(Ipp64u pos);
    virtual Status SetTimePosition(Ipp64f timePos);
    virtual Status GetTimePosition(Ipp64f& timePos);

    // getting info
    virtual Status GetInfo(SplitterInfo** ppInfo);

    // mpeg2 very specific functions
    virtual Ipp32s GetTrackByPid(Ipp32u PID);
    virtual Status CheckNextData(MediaData *data, Ipp32u *pTrack);
    virtual Status GetNextData(MediaData *data, Ipp32u *pTrack);
    virtual void AlterQuality(Ipp64f time);

protected:
    void TerminateInit();
    virtual SystemStreamType DetectSystem(DataReader* pDataReader);
    virtual Status CreateAndInitParser(SplitterParams *pParams);
    virtual Status CreateFrameConstructor(Ipp32s iTrack);
    virtual Status CheckNextDataBackward(SplMediaData* data, Ipp32u* pTrack);
    Status CheckNextDataForward(SplMediaData* data, Ipp32u* pTrack);
    Ipp64f GetDuration(void);
    Ipp64f GetExactDuration(void);
    void AssignRefTrack(void);
    bool IsSeekable(void);
    bool IsPure(void);
    bool TryToDetectDiscontinuity(CheckPoint start, CheckPoint &rEnd, Ipp32s nOfTries);
#ifdef DUMP_SOMETHING
    void DumpSample(Ipp32u uiTrack);
#endif

    // helping objects
    SplitterInfo m_SplInfo;
    MemoryAllocator *m_pMemoryAllocator;
    StreamParser *m_pParser;
    TimeStampCorrector m_Corrector;
    Segmentator m_Segmentator;
    FrameConstructor *m_pFC[MAX_TRACK];
    MediaData *m_pPrevData[MAX_TRACK];
    TrackInfo *m_pInfoStack[MAX_TRACK];

    // number of tracks for those at least one frame has been constructed
    Ipp16s m_uiTracksReady;
    // number of tracks detected by StreamParser
    Ipp32u m_uiTracks;
    // track that last sample was received from
    Ipp32s m_iCurTrack;
    // reference track, it is used for trick-mode operations
    // demuxer tries to use video tracks as reference
    Ipp32s m_iRefTrack;

    // true if last StreamParser::CheckNextData() returned UMC_WRN_INVALID_STREAM
    // next calls of Demuxer::CheckNextData() will re-init frame constructors
    bool m_bPSIWasChanged;
    // demuxer creates special frame constructors preventing access to encrypted packets payload
    bool m_bEncryptedData;
    // is memory allocator object created inside demuxer
    bool m_bInnerMemAllocator;
    // inidicates that one of buffers was once filled
    bool m_bBufFilled;
    bool m_bEndOfStream;
    Ipp64f m_dRate;
    Ipp64f m_dDuration;
    Ipp64u m_uiSourceSize;
    Ipp64f m_dAudToBuf;

    // bytes per second ration for whole stream
    Ipp64f m_dBytesPerSec;
    // first check point (valid time stamp and position of packet contains it)
    CheckPoint m_Start;
    // last check point (valid time stamp and position of packet contains it)
    CheckPoint m_End;
    // number of parsed frames from reference track, used for estimation of duration of pure video streams
    Ipp32u m_uiNOfFrames;
    // total size of parsed frames from reference track, used for estimation of duration of pure video streams
    Ipp64u m_uiTotalSize;

    // adaptive factor for QoS maintaining, this is used at playback rates of absolute values >4
    Ipp32u m_uiAdaptiveFactor;
    // time gap from previous quality report, this is used at playback rates of absolute values >4
    Ipp64f m_dPrevTimeGap;
};

}

#endif /* __UMC_DEMUXER_H__ */
