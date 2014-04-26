/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FRAME_CONSTRUCTOR_H__
#define __UMC_FRAME_CONSTRUCTOR_H__

#include "umc_media_buffer.h"
#include "umc_linked_list.h"
#include "umc_splitter.h"
#include "umc_bitstream_reader.h"

#include "ipps.h"


namespace UMC
{
    // this macro checks 3 bytes for being '0x000001'
    #define IS_001(PTR) ((PTR)[0] == 0 && (PTR)[1] == 0 && (PTR)[2] == 1)
    // this macro checks 4 bytes for being video start code
    #define IS_CODE(PTR, CODE) (IS_001(PTR) && (PTR)[3] == (CODE))
    // this macro checks 4 bytes for being one of 2 video start codes
    #define IS_CODE_2(PTR, CODE1, CODE2) (IS_001(PTR) && ((PTR)[3] == (CODE1) || (PTR)[3] == (CODE2)))
    // this macro checks 4 bytes for being one of 3 video start codes
    #define IS_CODE_3(PTR, CODE1, CODE2, CODE3) (IS_001(PTR) && ((PTR)[3] == (CODE1) || (PTR)[3] == (CODE2) || (PTR)[3] == (CODE3)))
    // this macro checks 4 bytes for being one of 4 video start codes
    #define IS_CODE_4(PTR, CODE1, CODE2, CODE3, CODE4) (IS_001(PTR) && ((PTR)[3] == (CODE1) || (PTR)[3] == (CODE2) || (PTR)[3] == (CODE3) || (PTR)[3] == (CODE4)))
    // this macro checks 4 bytes for being one of video start codes from interval
    #define IS_CODE_INT(PTR, CODE_MIN, CODE_MAX) ((PTR)[0] == 0 && (PTR)[1] == 0 && (PTR)[2] == 1 && (PTR)[3] >= (CODE_MIN) && (PTR)[3] <= (CODE_MAX))
    // this macro checks 4 bytes for being valid h264 video start code
    #define IS_H264_CODE(PTR, CODE) ((PTR)[0] == 0 && (PTR)[1] == 0 && (PTR)[2] == 1 && ((PTR)[3] & 0x1f) == (CODE))

    // macro for mp3 header parsing
    #define MPEGA_HDR_VERSION(x)       ((x & 0x80000) >> 19)
    #define MPEGA_HDR_LAYER(x)         (4 - ((x & 0x60000) >> 17))
    #define MPEGA_HDR_ERRPROTECTION(x) ((x & 0x10000) >> 16)
    #define MPEGA_HDR_BITRADEINDEX(x)  ((x & 0x0f000) >> 12)
    #define MPEGA_HDR_SAMPLINGFREQ(x)  ((x & 0x00c00) >> 10)
    #define MPEGA_HDR_PADDING(x)       ((x & 0x00200) >> 9)
    #define MPEGA_HDR_EXTENSION(x)     ((x & 0x00100) >> 8)
    #define MPEGA_HDR_MODE(x)          ((x & 0x000c0) >> 6)
    #define MPEGA_HDR_MODEEXT(x)       ((x & 0x00030) >> 4)
    #define MPEGA_HDR_COPYRIGHT(x)     ((x & 0x00008) >> 3)
    #define MPEGA_HDR_ORIGINAL(x)      ((x & 0x00004) >> 2)
    #define MPEGA_HDR_EMPH(x)          ((x & 0x00003))

    inline
    Ipp32s CalcCurrentLevel(Ipp32s iStart, Ipp32s iEnd, Ipp32s iBufSize)
    {
        Ipp32s iCurrentLevel = iEnd - iStart;
        return iCurrentLevel + ((iCurrentLevel >= 0) ? 0 : iBufSize);
    }

    inline
    bool ArePTSEqual(Ipp64f dTime1, Ipp64f dTime2)
    {
        return 0 == (Ipp32s)(90000 * (dTime1 - dTime2));
    }

    // maps TrackType to AudioStreamType and VideoStreamType
    Ipp32u ConvertTrackType(TrackType type);
    // maps AudioStreamType to TrackType
    TrackType ConvertAudioType(Ipp32u type);
    // maps VideoStreamType to TrackType
    TrackType ConvertVideoType(Ipp32u type);

    struct TeletextStreamInfo : public StreamInfo
    {
        TeletextStreamInfo()
        : uiType(0), uiMagazineNumber(0), uiPageNumber(0)
        {
            memset(szLanguage, 0, sizeof(szLanguage));
        }

        Ipp8u szLanguage[4];
        Ipp8u uiType;
        Ipp8u uiMagazineNumber;
        Ipp8u uiPageNumber;
    };

    // extends TrackInfo for needs of mpeg2
    // adds some utilities
    class Mpeg2TrackInfo : public TrackInfo
    {
    public:
        DYNAMIC_CAST_DECL(Mpeg2TrackInfo, TrackInfo)

        // constructor
        Mpeg2TrackInfo();
        // copy track's info structure from another one including all pointers
        Status CopyFrom(Mpeg2TrackInfo *pSrc);
        // releases StreamInfo
        void ReleaseStreamInfo(void);
        // releases decoder specific info
        void ReleaseDecSpecInfo(void);
        // releases all pointers and resets fields
        void ReleaseAll(void);
        // allocates StreamInfo pointer, copies PID and stream type from parent object
        Status Alloc(void);
        // sets duration field (m_Type is requiered)
        void SetDuration(Ipp64f dDuration);

        // number of program that track belongs to
        // 0 if not applied
        Ipp32u m_uiProgNum;
        // number of frames is currently stored in track buffer
        // it decreaments after lock
        Ipp32u m_uiFramesReady;
        // number of track in order of its first frame ready
        // -1 means that first frame isn't ready yet
        Ipp32s m_iFirstFrameOrder;
    };

    class SplMediaData : public MediaData
    {
    public:
        DYNAMIC_CAST_DECL(SplMediaData, MediaData)

        // constructor
        SplMediaData();
        // sets absolute position
        void SetAbsPos(Ipp64u uiAbsPos);
        // returns absolute position
        Ipp64u GetAbsPos(void) const;
        // updates certain flag, returns its previous value
        bool SetFlag(Ipp32u mask, bool flag);
        // returns certain flag
        bool GetFlag(Ipp32u mask) const;
        // updates all flags, returns previous flagset
        Ipp32u SetFlags(Ipp32u flags);
        // returns current flagset
        Ipp32u GetFlags() const;
    protected:
        // absolute position of sample (specified by data reader)
        Ipp64u m_uiAbsPos;
        Ipp32u m_uiFlags;
    };

    struct FCSample
    {
        static const Ipp32u STAMPS_APPLIED  = 0x00000010;
        static const Ipp32u PES_START       = 0x00000020;
        static const Ipp32u ACCESS_POINT    = 0x00000040;
        static const Ipp32u ACCESS_UNIT     = 0x00000080;
        static const Ipp32u FIRST_SLICE     = 0x00000100;

        // constructor
        FCSample();
        // resets sample
        void Reset(void);
        // copy from MediaData
        void CopyFrom(MediaData &data, Ipp32s iOffset);
        // copy to MediaData
        void CopyTo(MediaData &data, Ipp8u *pBufBase);
        // checks if position is enclosed
        bool IsHit(Ipp32s iPos);
        // sets frame type, returns previous
        Ipp32u SetFrameType(Ipp32u uiType);
        // returns frame type
        Ipp32u GetFrameType(void);
        // sets certain flag, returns its previous value
        bool SetFlag(Ipp32u uiFlagMask, bool bNewFlag);
        // returns certain flag
        bool GetFlag(Ipp32u uiFlagMask);
        // updates all flags, returns previous flagset
        Ipp32u SetFlags(Ipp32u flags);
        // returns current flagset
        Ipp32u GetFlags();
        // moves forward data pointer
        void MovePointer(Ipp32u off);

        // PTS of media sample
        Ipp64f dPTS;
        // DTS or media sample
        Ipp64f dDTS;
        // media sample size
        Ipp32u uiSize;
        // bits 0...2 - picture type (only for video)
        // bit      3 - reserved
        // bit      4 - is set if sample's timestamps are used (for input samples)
        // bit      5 - indicates if access unit commence here
        // bit      6 - indicates if access point commence here
        Ipp32u uiFlags;
        // absolute position of sample in stream
        Ipp64u uiAbsPos;
        // offset in buffer of first byte of sample
        Ipp32s iBufOffset;
    };

    inline
    void UpdateInputSample(FCSample &sample, Ipp32s iFrom, Ipp32s iTo, bool isPure)
    {
        if (isPure && (iTo > sample.iBufOffset))
            sample.uiAbsPos += IPP_MIN(iTo - IPP_MAX(iFrom, sample.iBufOffset), (Ipp32s)sample.uiSize);

        Ipp32s iEndOffset = sample.iBufOffset + sample.uiSize;
        if (sample.iBufOffset > iFrom)
            sample.iBufOffset -= IPP_MIN(sample.iBufOffset, iTo) - iFrom;
        if (iEndOffset > iFrom)
            iEndOffset -= IPP_MIN(iEndOffset, iTo) - iFrom;
        sample.uiSize = iEndOffset - sample.iBufOffset;
    }

    inline
    void CutInterval(FCSample &sample1, FCSample &sample2, Ipp8u *pBuf, Ipp32s iFrom, Ipp32s iTo, Ipp32s iLastByte, bool isPure = false)
    {
        Ipp8u *pFrom = pBuf + iTo;
        Ipp8u *pTo = pBuf + iFrom;
        ippsCopy_8u(pFrom, pTo, iLastByte - iTo);
        UpdateInputSample(sample1, iFrom, iTo, isPure);
        UpdateInputSample(sample2, iFrom, iTo, isPure);
    }

    struct InnerListElement
    {
        InnerListElement(void);
        InnerListElement(FCSample &rData);
        FCSample m_data;
        InnerListElement *pNext;
        InnerListElement *pPrev;
    };

#if defined (__ICL)
    //function "UMC::MediaData::Alloc(size_t={unsigned int})" is hidden by "UMC::VideoData::Alloc" -- virtual function override intended?
#pragma warning(disable:1125)
#endif
    // used for reordering at backward
    class ReorderQueue : public LinkedList<InnerListElement>
    {
    public:
        ReorderQueue(void);
        Status Add(FCSample &rSample);
        Status Add(FCSample &rSample, Ipp32s idx);
        Status Remove(void);
        Status Remove(Ipp32s idx);
        Status First(FCSample &rSample);
        Status Last(FCSample &rSample);
        Status Next(FCSample &rSample);
        Status Prev(FCSample &rSample);
        Status Get(FCSample &rSample, Ipp32s idx);

        const FCSample *FirstBO(void) const;
        const FCSample *LastBO(void) const;

    protected:
        void AddToSuperList(InnerListElement *pAddedElem);
        void RemoveFromSuperList(void);
        InnerListElement *m_pSuperFirst;
        InnerListElement *m_pSuperLast;
    };

    class FrameConstructorParams : public MediaReceiverParams
    {
    public:
        DYNAMIC_CAST_DECL(FrameConstructorParams, MediaReceiverParams)

        // constructor-initializer
        FrameConstructorParams();

        // pointer to heritable ESInfo (may be NULL if inheritance isn't needed)
        Mpeg2TrackInfo *m_pInfo;
        // size of buffer to allocate
        size_t m_lBufferSize;
        // initial number of samples in buffer (might be dynamically allocated if needed)
        Ipp32u m_nOfFrames;
        // indicates if stop when frame is constructed or parses whole input chunk
        bool m_bStopAtFrame;
        // this option affect absPos value assignment
        // for pure stream it is byte-accurate
        // otherwise it is packet-accurate
        bool m_bPureStream;
        // pointer to memory allocator object (if NULL it will be created internally)
        MemoryAllocator *m_pMemoryAllocator;
    };

    // base class for all frame constructors
    // it implements most of functionality
    // that can be extended by by derived classes or used directly
    // the most important fuction to implement is GetFrame()
    // that actually performs frame constructing
    class FrameConstructor : public MediaBuffer
    {
    public:
        DYNAMIC_CAST_DECL(FrameConstructor, MediaBuffer)

        // constructor
        FrameConstructor();
        // destructor
        ~FrameConstructor();

        // inherited from base MediaBuffer
        virtual Status Init(MediaReceiverParams *pInit);
        virtual Status Close();
        virtual Status Reset();
        virtual Status Stop();
        virtual Status LockInputBuffer(MediaData *in);
        virtual Status UnLockInputBuffer(MediaData *in, Status streamStatus = UMC_OK);
        virtual Status LockOutputBuffer(MediaData *out);
        virtual Status UnLockOutputBuffer(MediaData *out);

        // resets tailing bytes after last constracted sample
        virtual Status SoftReset();
        // sets playback rate, affects only video frame constructors
        virtual void SetRate(Ipp64f dRate);
        // unlocks input buffer and tries to construct frame
        // but constructed frame is uncommited (can be rolled back)
        virtual Status PreUnLockInputBuffer(MediaData *in, Status streamStatus = UMC_OK);
        // returns last constructed frame
        virtual Status GetLastFrame(MediaData *data);
        // returns pointer to ESInfo
        virtual Mpeg2TrackInfo *GetInfo(void);

    protected:
        virtual Status GetFrame(SplMediaData *frame);
        virtual Status GetSampleFromQueue(FCSample *pSample);
        void AssignAbsPos(Ipp32s iPos);
        void AssignTimeStamps(Ipp32s iPos);

        // synchro object
        Mutex m_synchro;

        // pointer to allocated buffer
        Ipp8u *m_pBuf;
        // memory ID of allocated buffer
        MemID m_midAllocatedBuffer;

        // frame being constructed
        FCSample m_CurFrame;
        // queue of output samples
        ReorderQueue m_OutputQueue;
        // index of first-to-decode sample
        Ipp32s m_iFirstInDecOrderIdx;
        // last constructed frame
        SplMediaData m_LastFrame;
        // previous input sample
        FCSample m_PrevSample;
        // last input sample
        FCSample m_LastSample;

        // indicates end of stream
        bool m_bEndOfStream;
        // is output buffer locked
        bool m_bIsOutputBufferLocked;
        // is info filled, this is needed to prevent info changing
        bool m_bInfoFilled;
        // indicates if stop when frame is constructed or parses whole input chunk
        bool m_bStopAtFrame;
        bool m_bPureStream;

        // size of allocated buffer
        Ipp32s m_lBufferSize;
        // position in buffer of byte after last data byte
        Ipp32s m_lLastBytePos;
        // position in buffer of first data byte
        Ipp32s m_lFirstBytePos;
        // position of end sequence code in buffer (to speed up search for start codes)
        Ipp32s m_lSeqEndPos;
        // current position in buffer
        Ipp32s m_lCurPos;
        // position in buffer of picture start code of frame being constuncted
        Ipp32s m_lCurPicStart;

        // playback rate
        Ipp64f m_dRate;
        // number of commited frames
        Ipp32u m_uiCommitedFrames;
        // total number of frames in buffer
        Ipp32u m_uiTotalFrames;
        // actual size of user data
        Ipp32s m_iCurrentLevel;
        // pointer to info structure
        Mpeg2TrackInfo *m_pInfo;
    };

    class AudioFrameConstructor : public FrameConstructor
    {
    public:
        DYNAMIC_CAST_DECL(AudioFrameConstructor, FrameConstructor)

        static const AudioStreamType MpegAStreamType[2][3];
        static const Ipp32s MpegAFrequency[3][4];
        static const Ipp32s MpegABitrate[2][3][15];
        static const Ipp32s MpegAChannels[4];
        static const Ipp32s AC3Frequency[3];
        static const Ipp32s AC3FrequencyExt[8];
        static const Ipp32s AC3BitRate[19];
        static const Ipp32s AC3NumChannels[];
        static const Ipp32f MpegAFramesize[2][4];
        static const Ipp32s AACFrequency[16];
        static const Ipp32u AACChannels[16];
        static const Ipp32s DTSChannels[16];
        static const Ipp32s DTSFrequency[16];
        static const Ipp32s DTSBitrate[32];
        static const Ipp32s LPCMChannels[16];
        Status Init(MediaReceiverParams *pInit);
        Status Reset(void) {
          FrameConstructor::Reset();
          m_bHeaderParsed = false;
          return UMC_OK;
        }
    protected:
        // parses headers of all supported audio
        Status ParseHeader();
        // indicate if header parsed
        bool m_bHeaderParsed;
    };

    class TimeStampedAudioFrameConstructor : public AudioFrameConstructor
    {
    public:
        Status GetFrame(SplMediaData *frame);
    };

    class BufferedAudioFrameConstructor : public AudioFrameConstructor
    {
    public:
        explicit BufferedAudioFrameConstructor(Ipp64f dToBuf = 0.05);
        Status GetFrame(SplMediaData *frame);
    protected:
        Ipp64f m_dToBuf;
    };

    class PureAudioFrameConstructor : public AudioFrameConstructor
    {
    public:
        Status GetFrame(SplMediaData *frame);
    };

    class VideoFrameConstructor : public FrameConstructor
    {
    public:
        DYNAMIC_CAST_DECL(VideoFrameConstructor, FrameConstructor)

        VideoFrameConstructor(void);
        virtual Status Init(MediaReceiverParams *pInit);
        virtual Status Reset(void);
        virtual Status SoftReset(void);
        virtual void SetRate(Ipp64f dRate);
        virtual bool IsFrameStartFound(void);

    protected:
        // find next sample to output, performs reordering at backward
        virtual Status GetSampleFromQueue(FCSample *pSample);
        // returns true when sample such sample is allowed at playback rate
        bool IsSampleComplyWithTmPolicy(FCSample &sample, Ipp64f dRate);
        // is sequence header found and parsed
        bool m_bSeqSCFound;
        // is picture header found and parsed
        bool m_bPicSCFound;
        // is start of frame found
        bool m_bFrameBegFound;
        // FrameConstructor generate 4-zero sequence at the end of last video frame
        bool m_bIsFinalizeSequenceSent;
    };

    class Mpeg2FrameConstructor : public VideoFrameConstructor
    {
    public:
        // parses sequence header and fills info if passed
        static Status ParseSequenceHeader(Ipp8u *buf, Ipp32s iLen, Mpeg2TrackInfo *pInfo);
        // parses picture header and sets interlace_type field if info struct passed
        static Status ParsePictureHeader(Ipp8u *buf, Ipp32s iLen, Mpeg2TrackInfo *pInfo);
        // mpeg1/2 frame rate values
        static const Ipp64f FrameRate[9];
        // mpeg1 aspect ratio values
        static const Ipp32u AspectRatioMp1[15][2];
        // mpeg2 aspect ratio values
        static const Ipp32s AspectRatioMp2[5][2];
        // mpeg2 color format values
        static const ColorFormat ColorFormats[4];
    protected:
        virtual Status GetFrame(SplMediaData *frame);
    };

    class Mpeg4FrameConstructor : public VideoFrameConstructor
    {
    public:
        // parses video object layer header and fills info if passed
        static Status ParseVideoObjectLayer(Ipp8u *buf, Ipp32s iLen, Mpeg2TrackInfo *pInfo);
        // mpeg4 aspect ratio values
        static const Ipp32s AspectRatio[6][2];
    protected:
        virtual Status GetFrame(SplMediaData *frame);
    };

    class H261FrameConstructor : public VideoFrameConstructor
    {
    public:
        // parses h261 header and fills info if passed
        static Status ParseHeader(Ipp8u *buf, Ipp32s iLen, Mpeg2TrackInfo *pInfo);
    protected:
        virtual Status GetFrame(SplMediaData *frame);
    };

    class H263FrameConstructor : public VideoFrameConstructor
    {
    public:
        // parses h263 header and fills info if passed
        static Status ParseHeader(Ipp8u *buf, Ipp32s iLen, Mpeg2TrackInfo *pInfo);
        // valid frame sizes for h263 video
        static const Ipp16s PictureSize[6][2];
    protected:
        virtual Status GetFrame(SplMediaData *frame);
    };

    enum
    {
        NALU_SLICE      = 1,
        NALU_IDR_SLICE  = 5,
        NALU_SEI        = 6,
        NALU_SPS        = 7,
        NALU_PPS        = 8,
        NALU_DELIMITER  = 9,
        NALU_SPS_EX     = 13
    };

    class PesFrameConstructor : public FrameConstructor
    {
    public:
        virtual Status Init(MediaReceiverParams *pInit);
        virtual Status Reset(void);
        virtual Status SoftReset(void);

    protected:
        virtual Status GetFrame(SplMediaData *frame);

    protected:
        bool m_bIsFirst;
    };

    class H264ParsingCore
    {
    public:
        enum Result { Ok, OkPic, ErrNeedSync, ErrNeedData, ErrInvalid };

        H264ParsingCore();
        Result Sync(MediaData& data, bool eos = false);
        Result Construct(MediaData& data, bool eos = false);
        void GetInfo(VideoStreamInfo& info);
        void Reset();

    protected:
        static const Ipp32u BytesForSliceReq = 30;
        static const Ipp32u MaxNumSps = 32;
        static const Ipp32u MaxNumPps = 256;
        static const FrameType SliceType[10];
        static const Ipp16u AspectRatio[17][2];
        static const ColorFormat ColorFormats[4];
        static const ColorFormat ColorFormatsA[4];
        struct Slice;

        Status ParseSps(Ipp8u* buf, Ipp32s len);
        Status ParseSpsEx(Ipp8u* buf, Ipp32s len);
        Status ParsePps(Ipp8u* buf, Ipp32s len);
        Status ParseSh(Ipp8u* buf, Ipp32s len);
        bool IsNewPicture(const Slice& prev, const Slice& last);

        struct BaseNaluHeader
        {
            BaseNaluHeader() : m_ready(false) {}
            void SetReady(bool ready) { m_ready = ready; }
            bool IsReady() const { return m_ready; }
        private:
            bool m_ready;
        };

        struct Sps : public BaseNaluHeader
        {
            Sps() { Reset(); }
            void Reset();
            Ipp32u time_scale; // 30
            Ipp32u num_units_in_tick; // 1
            Ipp16u sar_width; // 1
            Ipp16u sar_height; // 1
            Ipp8u profile_idc;
            Ipp8u level_idc;
            Ipp8u chroma_format_idc; // 1
            Ipp8u bit_depth_luma_minus8;
            Ipp8u bit_depth_chroma_minus8;
            Ipp8u log2_max_pic_order_cnt_lsb;
            Ipp8u log2_max_frame_num;
            Ipp8u frame_mbs_only_flag;
            Ipp8u pic_order_cnt_type;
            Ipp8u delta_pic_order_always_zero_flag;
            Ipp8u frame_width_in_mbs;
            Ipp8u frame_height_in_mbs;
            Ipp8u frame_cropping_rect_left_offset;
            Ipp8u frame_cropping_rect_right_offset;
            Ipp8u frame_cropping_rect_top_offset;
            Ipp8u frame_cropping_rect_bottom_offset;
        };

        struct SpsEx : public BaseNaluHeader
        {
            SpsEx() { Reset(); }
            void Reset();
            Ipp32u aux_format_idc;
            Ipp32u bit_depth_aux_minus8;
            Ipp32u alpha_incr_flag;
            Ipp32u alpha_opaque_value;
            Ipp32u alpha_transparent_value;
            Ipp32u additional_extension_flag;
        };

        struct Pps : public BaseNaluHeader
        {
            Pps() { Reset(); }
            void Reset();
            void SetSps(const Sps& sps) { m_sps = &sps; }
            const Sps& GetSps() const { return *m_sps; }
            Ipp8u pic_order_present_flag;
        private:
            const Sps* m_sps;
        };

        struct Slice : public BaseNaluHeader
        {
            Slice() { Reset(); }
            void Reset();
            void SetPps(const Pps& pps) { m_pps = &pps; }
            const Pps& GetPps() const { return *m_pps; }

            Ipp16u pic_parameter_set_id;
            Ipp8u field_pic_flag;
            Ipp8u bottom_field_flag;
            Ipp32u frame_num;
            Ipp32s slice_type;
            Ipp32u pic_order_cnt_lsb;
            Ipp32s delta_pic_order_cnt_bottom;
            Ipp32s delta_pic_order_cnt[2];
            Ipp8u nal_ref_idc;
            Ipp8u idr_flag;
            Ipp32u idr_pic_id;
        private:
            const Pps* m_pps;
        };

        Sps   m_sps[MaxNumSps];
        SpsEx m_sps_ex[MaxNumSps];
        Pps   m_pps[MaxNumPps];
        Slice m_prev;
        Slice m_last;
        bool m_synced;
        Ipp32u m_skip;
        FrameType m_type;
        bool m_spsParsed;
    };

    class H264FrameConstructor : public VideoFrameConstructor
    {
    public:
        H264FrameConstructor();
        virtual Status Init(MediaReceiverParams *pInit);
        virtual Status Reset();
        virtual Status SoftReset();

    protected:
        virtual Status GetFrame(SplMediaData *frame);
        void InternBuf2MediaData(MediaData& data);
        void MediaData2InternBuf(MediaData& data);
        H264ParsingCore m_core;
    };
}

#endif /* __UMC_FRAME_CONSTRUCTOR_H__ */
