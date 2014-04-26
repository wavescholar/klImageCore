/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_TASK_SUPPLIER_H
#define __UMC_H264_TASK_SUPPLIER_H

#include <vector>
#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_heap.h"
#include "umc_h264_slice_decoding.h"
#include "umc_h264_frame_info.h"

#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_headers.h"

#include "umc_frame_allocator.h"

#include "umc_h264_au_splitter.h"

namespace UMC
{
class TaskBroker;

class H264DBPList;
class H264DecoderFrame;
class MediaData;

class BaseCodecParams;
class H264SegmentDecoderMultiThreaded;

class MemoryAllocator;
struct H264IntraTypesProp;

/****************************************************************************************************/
// Skipping class routine
/****************************************************************************************************/
class Skipping
{
public:
    Skipping();
    virtual
    ~Skipping();

    void PermanentDisableDeblocking(bool disable);
    bool IsShouldSkipDeblocking(H264DecoderFrame * pFrame, Ipp32s field);
    bool IsShouldSkipFrame(H264DecoderFrame * pFrame, Ipp32s field);
    void ChangeVideoDecodingSpeed(Ipp32s& num);
    void Reset();

    H264VideoDecoder::SkipInfo GetSkipInfo() const;

private:

    Ipp32s m_VideoDecodingSpeed;
    Ipp32s m_SkipCycle;
    Ipp32s m_ModSkipCycle;
    Ipp32s m_PermanentTurnOffDeblocking;
    Ipp32s m_SkipFlag;

    Ipp32s m_NumberOfSkippedFrames;
};

/****************************************************************************************************/
// POCDecoder
/****************************************************************************************************/
class POCDecoder
{
public:

    POCDecoder();

    virtual ~POCDecoder();
    void DecodePictureOrderCount(H264Slice *slice, Ipp32s frame_num);

    Ipp32s DetectFrameNumGap(H264Slice *slice);

    void Reset(Ipp32s IDRFrameNum = 0);

protected:
    Ipp32s                     m_PrevFrameRefNum;
    Ipp32s                     m_FrameNum;
    Ipp32s                     m_PicOrderCnt;
    Ipp32s                     m_PicOrderCntMsb;
    Ipp32s                     m_PicOrderCntLsb;
    Ipp32s                     m_FrameNumOffset;
    Ipp32u                     m_TopFieldPOC;
    Ipp32u                     m_BottomFieldPOC;
};

/****************************************************************************************************/
// Resources
/****************************************************************************************************/
class LocalResources
{
public:

    LocalResources();
    virtual ~LocalResources();

    Status Init(Ipp32s numberOfBuffers, MemoryAllocator *pMemoryAllocator);

    void Reset();
    void Close();

    H264DecoderLocalMacroblockDescriptor & GetMBInfo(Ipp32s number);
    IntraType * GetIntraTypes(Ipp32s number);

    void AllocateMBInfo(Ipp32s number, Ipp32u iMBCount);
    bool AllocateMBIntraTypes(Ipp32s iIndex, Ipp32s iMBNumber);

    Status AllocateBuffers(H264SeqParamSet* sps, bool exactSizeRequested);

    bool LockFrameResource(H264DecoderFrame * frame);
    void UnlockFrameResource(H264DecoderFrame * frame);

    H264DecoderFrame * IsBusyByFrame(Ipp32s number);

    Ipp32u GetCurrentResourceIndex();

protected:
    Ipp8u         *m_pMBMap;
    H264DecoderMBAddr **next_mb_tables;//0 linear scan, 1,.. - bitstream defined scan (slice groups)

private:
    IntraType *(*m_ppMBIntraTypes);
    H264IntraTypesProp *m_piMBIntraProp;

    H264DecoderLocalMacroblockDescriptor *m_pMBInfo;

    Ipp32s m_numberOfBuffers;
    MemoryAllocator *m_pMemoryAllocator;

    Ipp32s          m_parsedDataLength;
    IppiSize        m_paddedParsedDataSize;
    Ipp8u          *m_pParsedData;
    MemID           m_midParsedData;       // (MemID) mem id for allocated parsed data

    Ipp32u          m_currentResourceIndex;

    void DeallocateBuffers();
private:
    const LocalResources& operator=(const LocalResources&) { return *this; }
    LocalResources(const LocalResources&) {}
};

/****************************************************************************************************/
// TaskSupplier
/****************************************************************************************************/
class SEI_Storer
{
public:

    struct SEI_Message
    {
        H264DecoderFrame * frame;
        SEI_TYPE type;
        Ipp32s  isUsed;
        size_t  msg_size;
        size_t  offset;
        Ipp64f  timestamp;
        Ipp8u*  data;

        SEI_Message()
            : frame(0)
            , type(SEI_RESERVED)
            , isUsed(0)
            , msg_size(0)
            , offset(0)
            , timestamp(0)
            , data(0)
        {
        }

    };

    SEI_Storer();

    virtual ~SEI_Storer();

    void Init();

    void Close();

    void Reset();

    void SetTimestamp(H264DecoderFrame * frame);

    SEI_Message* AddMessage(UMC::MediaDataEx *nalUnit, SEI_TYPE type);

    const SEI_Message * GetPayloadMessage();

    void SetFrame(H264DecoderFrame * frame);

private:

    enum
    {
        MAX_BUFFERED_SIZE = 16 * 1024, // 16 kb
        START_ELEMENTS = 10,
        MAX_ELEMENTS = 128
    };

    std::vector<Ipp8u>  m_data;
    std::vector<SEI_Message> m_payloads;

    size_t m_offset;
    Ipp32s m_lastUsed;

    //std::list<> ;
};


/****************************************************************************************************/
// TaskSupplier
/****************************************************************************************************/
class TaskSupplier : public Skipping, private POCDecoder, protected LocalResources, protected AU_Splitter, public SplitterListner
{
    friend class TaskBroker;

public:

    TaskSupplier();
    virtual ~TaskSupplier();

    virtual Status Init(BaseCodecParams *pInit);

    virtual Status PreInit(BaseCodecParams *pInit);

    virtual void Reset();
    virtual void Close();

    Status GetInfo(VideoDecoderParams *lpInfo);

    Status GetInfoFromData(BaseCodecParams* params);

    virtual Status AddSource(MediaData * pSource, MediaData *dst);

    Status GetFrame(MediaData * pSource, MediaData *dst);

    void SetMemoryAllocator(MemoryAllocator *pMemoryAllocator)
    {
        m_pMemoryAllocator = pMemoryAllocator;
    }

    void SetFrameAllocator(FrameAllocator *pFrameAllocator)
    {
        m_pFrameAllocator = pFrameAllocator;
    }

    H264DecoderFrame *GetFrameToDisplayInternal(bool force);
    virtual bool GetFrameToDisplay(MediaData *dst, bool force);
    Status  SetParams(BaseCodecParams* params);

    Status GetUserData(MediaData * pUD);

    bool IsWantToShowFrame(bool force = false);

    H264DBPList  * GetDPBList()
    {
        return m_pDecodedFramesList;
    }

    TaskBroker * GetTaskBroker()
    {
        return m_pTaskBroker;
    }

    virtual Status RunDecoding(bool force, H264DecoderFrame ** decoded = 0);
    virtual Status IsNeedRunDecoding(H264DecoderFrame ** decoded);
    virtual Status RunDecoding_1();
    virtual H264DecoderFrame * FindSurface(FrameMemID id);

    void PostProcessDisplayFrame(MediaData *dst, H264DecoderFrame *pFrame);

    virtual void AfterErrorRestore();

    SEI_Storer * GetSEIStorer() const { return m_sei_messages;}

    H264DecoderFrame * GetLastFullFrame();

    bool IsShouldSuspendDisplay();

    const Headers * GetHeaders() const { return &m_Headers;}

    virtual void OnSequenceHeader(const H264SeqParamSet * sps);
    virtual void OnPictureHeader(const H264PicParamSet * pps);

protected:

    void InitColorConverter(H264DecoderFrame *source, VideoData * videoData, Ipp8u force_field);

    void SlideWindow(H264Slice * pSlice, Ipp32s field_index);
    void AddSliceToFrame(H264DecoderFrame *pFrame, H264Slice *pSlice);
    virtual H264DecoderFrame * AddFrame(H264Slice *pSlice);
    Status AddSlice(H264Slice * pSlice, MediaData * pSource);
    virtual void InitFrame(H264DecoderFrame * pFrame, H264Slice *pSlice);
    virtual void CompleteFrame(H264DecoderFrame * pFrame, Ipp32s m_field_index);
    virtual void OnFullFrame(H264DecoderFrame * pFrame);
    virtual bool ProcessNonPairedField(H264DecoderFrame * pFrame);
    virtual Status InitFreeFrame(H264DecoderFrame * pFrame, H264Slice *pSlice);

    void DBPUpdate(H264DecoderFrame * pFrame, Ipp32s field);
    Status UpdateRefPicMarking(H264DecoderFrame * pFrame, H264Slice * pSlice, Ipp32s field_index);

    virtual H264Slice * DecodeSliceHeader(MediaDataEx *nalUnit);
    virtual void AddFakeReferenceFrame(H264Slice * pSlice);

    Status AddOneFrame(MediaData * pSource, MediaData *dst);

    virtual Status AllocateFrameData(H264DecoderFrame * pFrame, IppiSize dimensions, Ipp32s bit_depth_luma, Ipp32s bit_depth_chroma, ColorFormat color_format);

    virtual Status DecodeHeaders(MediaDataEx *nalUnit);
    virtual Status DecodeSEI(MediaDataEx *nalUnit);

    Status ProcessFrameNumGap(H264Slice *slice, Ipp32s field);

    // Obtain free frame from queue
    virtual H264DecoderFrame *GetFreeFrame();

    Status CompleteDecodedFrames(H264DecoderFrame ** decoded);

    void SetDPBSize();

    H264DecoderFrame *GetAnyFrameToDisplay(bool force);

    // Initialize MB ordering for the picture using slice groups as
    // defined in the picture parameter set.
    void SetMBMap(H264DecoderFrame *frame);

    void PreventDPBFullness();

    H264_Heap_Objects           m_ObjHeap;

    H264SegmentDecoderMultiThreaded **m_pSegmentDecoder;
    Ipp32s m_iThreadNum;

    H264ThreadGroup  m_threadGroup;

    H264_Heap      m_Heap;

    Ipp32s      m_field_index;

    Ipp64f      m_local_frame_time;
    Ipp64f      m_local_delta_frame_time;
    bool        m_use_external_framerate;

    H264DBPList  *m_pDecodedFramesList;

    H264Slice * m_pLastSlice;

    H264DecoderFrame *m_pCurrentFrame;
    H264DecoderFrame *m_pLastFullFrame;
    H264DecoderFrame *m_pLastDisplayed;

    MemoryAllocator *m_pMemoryAllocator;
    FrameAllocator  *m_pFrameAllocator;

    // Keep track of which parameter set is in use.
    bool              m_WaitForIDR;

    Ipp32s            m_dpbSize;
    Ipp32s            m_maxDecFrameBuffering;
    Ipp32s            m_DPBSizeEx;
    Ipp32s            m_TrickModeSpeed;

    TaskBroker * m_pTaskBroker;

    VideoData              m_LastNonCropDecodedFrame;
    BaseCodec              *m_pPostProcessing;

    NotifiersChain          m_DefaultNotifyChain;

    Ipp32s m_UIDFrameCounter;

    List<H264SEIPayLoad> m_UserData;
    SEI_Storer *m_sei_messages;

    bool m_isInitialized;

    Mutex m_mGuard;

private:
    TaskSupplier & operator = (TaskSupplier &)
    {
        return *this;

    } // TaskSupplier & operator = (TaskSupplier &)

};

inline Ipp32s CalculateDPBSize(Ipp32s level_idc, Ipp32s iWidthMBs, Ipp32s iHeightMBs)
{
    Ipp32u iMaxDPB;

    // MaxDPB, per Table A-1, Level Limits
    switch (level_idc)
    {
    case 10:
        iMaxDPB = 396;
        break;
    case 11:
        iMaxDPB = 900;
        break;
    case 12:
    case 13:
    case 20:
        iMaxDPB = 2376;
        break;
    case 21:
        iMaxDPB = 4752;
        break;
    case 22:
    case 30:
        iMaxDPB = 8100;
        break;
    case 31:
        iMaxDPB = 18000;
        break;
    case 32:
        iMaxDPB = 20480;
        break;
    case 40:
    case 41:
        iMaxDPB = 32768;
        break;
    case 42:
        iMaxDPB = 34816;
        break;
    case 50:
        iMaxDPB = 110400;
        break;
    case 51:
        iMaxDPB = 184320;
        break;
    default:
        iMaxDPB = 184320; //get as much as we may
    }

    return IPP_MIN(16, iMaxDPB/(iWidthMBs*iHeightMBs));
}

} // namespace UMC

#endif // __UMC_H264_TASK_SUPPLIER_H
