/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_SPLITTER_H__
#define __UMC_SPLITTER_H__

#include "umc_structures.h"
#include "umc_data_reader.h"
#include "umc_dynamic_cast.h"
#include "umc_memory_allocator.h"
#include "umc_media_data.h"

namespace UMC
{

enum // splitter flags
{
    //invalid value
    UNDEF_SPLITTER             = 0x00000000,
    //audio splitting required in any present in stream
    AUDIO_SPLITTER             = 0x00000001,
    //video splitting required in any present in stream
    VIDEO_SPLITTER             = 0x00000002,
    //example if setup VIDEO_SPLITTER && !set AUDIO_SPLITTER, splitter will ignore
    //any audio elementary stream, only video data request will be valid

    //audio and video splitting required if any present in stream
    AV_SPLITTER                = AUDIO_SPLITTER|VIDEO_SPLITTER,

    //main video header (sequence header) is required to return from Init
    //splitter function, application is responsible to pass it to decoder
    //as a regular video data for properly decoding consequent data
    FLAG_VSPL_VIDEO_HEADER_REQ = 0x00000010,

    //the first video frame is required to return from Init
    //splitter function, application is responsible to pass it to decoder
    //as a regular video data for properly decoding consequent data.
    //The first frame will follow main video header. This flag expands
    //splitter behavior for FLAG_VSPL_VIDEO_HEADER_REQ case
    FLAG_VSPL_VIDEO_FRAME_REQ  = 0x00000020,
    FLAG_VSPL_AUDIO_INFO_REQ   = 0x00000040,
    FLAG_VSPL_VIDEO_INFO_REQ   = 0x00000080,

    //next flag describes endian related properties of input data
    //when set, means that coded data should be accessed by 4-reading operations
    //for little-endian systems it means that each 4 bytes are swapped
    //i.e [0]<->[3], [1]<->[2]
    //for big-endian systems swapping is not required
    FLAG_VSPL_4BYTE_ACCESS     = 0x00000100,

    ////traditional, not UMC specific behavior
    ////original byte order, headers before data, return bytes consumed
    //FLAG_VSPL_COMPATIBLE       = 0x00001000,

    //some splitters may have a behavior to run internal
    //to prohibit asynchronous splitting use this flag
    FLAG_VSPL_NO_INTERNAL_THREAD= 0x00002000,
    // if reposition is not supported

    FLAG_SPL_REPOSITION_DISABLED= 0x00004000
};

enum TrackType
{
    /* video types 0x0000XXXX */
    TRACK_MPEG1V                = 0x00000001,
    TRACK_MPEG2V                = 0x00000002,
    TRACK_MPEG4V                = 0x00000004,
    TRACK_H261                  = 0x00000008,
    TRACK_H263                  = 0x00000010,
    TRACK_H264                  = 0x00000020,
    TRACK_DVSD                  = 0x00000040,
    TRACK_DV50                  = 0x00000080,
    TRACK_DVHD                  = 0x00000100,
    TRACK_DVSL                  = 0x00000200,
    TRACK_VC1                   = 0x00000400,
    TRACK_WMV                   = 0x00000800,
    TRACK_MJPEG                 = 0x00001000,
    TRACK_YUV                   = 0x00002000,
    TRACK_AVS                   = 0x00004000,
    TRACK_VP8                   = 0x00008000,
    TRACK_ANY_VIDEO             = 0x0000FFFF,

    /* audio types 0x0XXX0000 */
    TRACK_PCM                   = 0x00010000,
    TRACK_LPCM                  = 0x00020000,
    TRACK_AC3                   = 0x00040000,
    TRACK_AAC                   = 0x00080000,
    TRACK_MPEGA                 = 0x00100000,
    TRACK_TWINVQ                = 0x00200000,
    TRACK_DTS                   = 0x00400000,
    TRACK_VORBIS                = 0x00800000,
    TRACK_AMR                   = 0x01000000,
    TRACK_ANY_AUDIO             = 0x0FFF0000,

    /* special types 0xX0000000 */
    TRACK_SUB_PIC               = 0x10000000,
    TRACK_DVD_NAV               = 0x20000000,
    TRACK_ANY_DVD               = 0x30000000,
    TRACK_VBI_TXT               = 0x40000000,
    TRACK_VBI_SPEC              = 0x80000000,
    TRACK_ANY_VBI               = 0xC0000000,
    TRACK_ANY_SPECIAL           = 0xF0000000,

    TRACK_UNKNOWN               = 0x00000000
};

enum TrackState
{
    TRACK_DISABLED = 0x0,
    TRACK_ENABLED  = 0x1
};

class TrackInfo
{
public:
    DYNAMIC_CAST_DECL_BASE(TrackInfo)

    TrackInfo()
    {
        m_type         = TRACK_UNKNOWN;
        m_iPID         = 0;
        m_bEnabled     = false;
        m_pHeader      = NULL;
        m_pHeaderExt   = NULL;
        m_pStreamInfo  = NULL;
    }

    virtual ~TrackInfo() {}

    TrackType    m_type;          // common type (all audio/video/other in one enum)
    Ipp32u       m_iPID;
    bool         m_bEnabled;      // if Track is on or off
    MediaData*   m_pHeader;       // Keeps Header and its length
    MediaData*   m_pHeaderExt;    // Keeps extension Header and its length
    StreamInfo*  m_pStreamInfo;   // Base for AudioStreamInfo, VideoStreamInfo, etc
};

class SplitterInfo
{
public:
    DYNAMIC_CAST_DECL_BASE(SplitterInfo)

    SplitterInfo()
    {
        m_iFlags      = 0;
        m_systemType  = UNDEF_STREAM;
        m_iTracks     = 0;
        m_fRate       = 1;
        m_fDuration   = -1.0;
        m_ppTrackInfo = NULL;
    }
    virtual ~SplitterInfo() {}

    // common fields
    Ipp32u            m_iFlags;
    SystemStreamType  m_systemType;       // system type (MPEG4, MPEG2, AVI, pure)
    Ipp32u            m_iTracks;          // number of tracks detected
    Ipp64f            m_fRate;            // current playback rate
    Ipp64f            m_fDuration;        // duration of stream
    TrackInfo**       m_ppTrackInfo;      // array of pointers to TrackInfo(s)
};

class SplitterParams
{
public:
    DYNAMIC_CAST_DECL_BASE(SplitterParams)

    SplitterParams(void)
    {
        m_iFlags            = 0;
        m_pDataReader       = NULL;
        m_iSelectedVideoPID = SELECT_ANY_VIDEO_PID;
        m_iSelectedAudioPID = SELECT_ANY_AUDIO_PID;
        m_pMemoryAllocator  = NULL;
    }
    virtual ~SplitterParams(void) {}

    Ipp32u           m_iFlags;              // splitter's flags
    DataReader*      m_pDataReader;         // pointer to data reader
    Ipp32u           m_iSelectedVideoPID;   // ID for video stream chosen by user
    Ipp32u           m_iSelectedAudioPID;   // ID for audio stream chosen by user
    MemoryAllocator* m_pMemoryAllocator;    // pointer to memory allocator object
};

/*
//  Class:       Splitter
//
//  Notes:       Base abstract class of splitter. Class describes the high level interface of abstract splitter of media stream.
//               All specific ( avi, mpeg2, mpeg4 etc ) must be implemented in derived classes.
//               Splitter uses this class to obtain data
//
*/
class Splitter
{
public:
    DYNAMIC_CAST_DECL_BASE(Splitter)

    Splitter(void);
    virtual ~Splitter(void) {}

    // Get media data type
    static SystemStreamType GetStreamType(DataReader *pReader);

    virtual Status Init(SplitterParams *pInit) = 0;
    virtual Status Close(void) = 0;

    // Get next data, unlocks previously returned
    virtual Status GetNextData(MediaData *pData, Ipp32u iTrack) = 0;

    // Get next data without moving DataReader
    virtual Status CheckNextData(MediaData *pData, Ipp32u iTrack) = 0;

    // Set time position
    virtual Status SetTimePosition(Ipp64f fPos) = 0;

    // Get time position
    virtual Status GetTimePosition(Ipp64f &fPos) = 0;

    // Get splitter info
    virtual Status GetInfo(SplitterInfo **ppInfo) = 0;

    // Set playback rate
    virtual Status SetRate(Ipp64f fRate)
    {
        fRate = fRate;
        return UMC_ERR_NOT_IMPLEMENTED;
    }

    // changes state of track
    virtual Status SetTrackState(Ipp32u iTrack, TrackState state) = 0;

    // Runs reading threads
    virtual Status Run(void) = 0;

    // Stops reading threads
    virtual Status Stop(void) = 0;

protected:
    DataReader*  m_pDataReader;  // pointer to data reader
    SplitterInfo m_info;         // splitter info
};

}

#endif
