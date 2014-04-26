/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MUXER_H__
#define __UMC_MUXER_H__

#include "umc_media_buffer.h"
#include "umc_data_writer.h"

namespace UMC
{

enum MuxerTrackType
{
    UNDEF_TRACK = 0,
    AUDIO_TRACK = 1,
    VIDEO_TRACK = 2,
    VBI_TRACK   = 4,
};

enum MuxerFlags
{
    FLAG_MUXER_ENABLE_THREADING     = 0x00000010,
    FLAG_DATA_FROM_SPLITTER         = 0x00000020,
    FLAG_FRAGMENTED_AT_I_PICTURES   = 0x00000040, // for mpeg4 muxer
    FLAG_FRAGMENTED_BY_HEADER_SIZE  = 0x00000080, // for mpeg4 muxer
    FLAG_START_WITH_HEADER          = 0x00000100, // for mpeg4 muxer
};

class TrackParams
{
public:
    TrackParams(void)
    {
        type = UNDEF_TRACK;
        info.undef = NULL;
        info.audio = NULL;
        info.video = NULL;
    }

    union
    {
        AudioStreamInfo* audio;
        VideoStreamInfo* video;
        void*            undef;
    } info;

    MuxerTrackType      type;
    MediaBufferParams   bufferParams;
};

class MuxerParams
{
public:
    DYNAMIC_CAST_DECL_BASE(MuxerParams)

    MuxerParams(void)
    {
      m_SystemType      = UNDEF_STREAM;
      m_lFlags          = 0;
      m_nNumberOfTracks = 0;
      m_pTrackParams    = NULL;
      m_lpDataWriter    = NULL;
      m_bAllocated      = false;
    }

    virtual ~MuxerParams(void)
    {
        MuxerParams::Close();
    }

    Status operator=(MuxerParams &p);

    SystemStreamType m_SystemType;       // type of media stream
    Ipp32s           m_lFlags;           // muxer flag(s)
    Ipp32s           m_nNumberOfTracks;  // number of tracks
    TrackParams*     m_pTrackParams;     // track parameters
    DataWriter*      m_lpDataWriter;     // pointer to data writer

protected:
    virtual Status Close(void);

    bool m_bAllocated; // (bool) TrackParams was allocated in operator=
};

/******************************************************************************/

class Muxer
{
public:
    DYNAMIC_CAST_DECL_BASE(Muxer)

    Muxer(void);
    virtual ~Muxer(void);

    // Flushes buffers and release all resources
    virtual Status Close(void);

    // Initialize muxer
    virtual Status Init(MuxerParams* lpInit) = 0;

    // Locks input buffer
    virtual Status LockBuffer(MediaData* lpData, Ipp32s iTrack);

    // Unlocks input buffer
    virtual Status UnlockBuffer(MediaData* lpData, Ipp32s iTrack);

    // Try to lock input buffer, copies user data into it and unlocks
    virtual Status PutData(MediaData* lpData, Ipp32s iTrack);

    // Deliver EOS
    virtual Status PutEndOfStream(Ipp32s iTrack);

    // Copy video sample to input buffer
    virtual Status PutVideoData(MediaData* lpData, Ipp32s iVideoIndex = 0);

    // Copy audio sample to input buffer
    virtual Status PutAudioData(MediaData* lpData, Ipp32s iAudioIndex = 0);

    // Flushes all data from buffers to output stream
    virtual Status Flush(void) = 0;

    // Get index of track of specified type
    virtual Ipp32s GetTrackIndex(MuxerTrackType type, Ipp32s index = 0);

protected:
    // Copy input parameters to m_pParams, m_uiTotalNumStreams, m_pTrackParams
    // and alloc m_ppBuffers array
    virtual Status CopyMuxerParams(MuxerParams* lpInit);

    // Provides time of first output sample
    // Returns UMC_ERR_NOT_ENOUGH_DATA when buffer is empty
    // Returns UMC_ERR_END_OF_STREAM when buffer is empty and EOS was received
    virtual Status GetOutputTime(Ipp32s iTrack, Ipp64f& dTime) = 0;

    // Find stream with minimum time of first output sample
    // In flush mode it disregards empty buffers (if at least one buffer is not empty)
    // In non-flush mode it returns UMC_ERR_NOT_ENOUGH_DATA if one of buffer is empty
    Status GetStreamToWrite(Ipp32s& rStreamNumber, bool bFlushMode);

protected:
    MuxerParams*  m_pParams;            // pointer to params
    Ipp32u        m_uiTotalNumStreams;  // number of tracks
    TrackParams*  m_pTrackParams;       // pointer to track params [m_uiTotalNumStreams]
    MediaBuffer** m_ppBuffers;          // array of pointers [m_uiTotalNumStreams] to media buffers
};

}

#endif
