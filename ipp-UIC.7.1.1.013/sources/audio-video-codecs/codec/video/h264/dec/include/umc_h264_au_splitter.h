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

#ifndef __UMC_H264_AU_SPLITTER_H
#define __UMC_H264_AU_SPLITTER_H

#include <vector>

#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_heap.h"
#include "umc_h264_slice_decoding.h"
#include "umc_h264_frame_info.h"

#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_headers.h"

#include "umc_frame_allocator.h"

namespace UMC
{

class H264Task;
class TaskBroker;

class H264DBPList;
class H264DecoderFrame;
struct H264SliceHeader;
class MediaData;
class NALUnitSplitter;
class H264Slice;

class BaseCodecParams;
class H264SegmentDecoderMultiThreaded;

class MemoryAllocator;
struct H264IntraTypesProp;

class SplitterListner
{
public:

    virtual ~SplitterListner() {}

    virtual void OnSequenceHeader(const H264SeqParamSet * sps) = 0;
    virtual void OnPictureHeader(const H264PicParamSet * pps) = 0;

};

class AU_Splitter
{
public:
    AU_Splitter(H264_Heap *heap, H264_Heap_Objects *objectHeap);
    virtual ~AU_Splitter();

    void Init(VideoDecoderParams *init);
    void Close();

    void Reset();

    void SetSplitterListner(SplitterListner * listner);

    Ipp32s CheckNalUnitType(MediaData * pSource);

    MediaDataEx * GetNalUnit(MediaData * src);

protected:

    Headers     m_Headers;
    H264_Heap_Objects   *m_objHeap;
    H264_Heap      *m_heap;

protected:

    //Status DecodeHeaders(MediaDataEx *nalUnit);

    SplitterListner * m_listner;
    std::auto_ptr<NALUnitSplitter> m_pNALSplitter;
};

} // namespace UMC

#endif // __UMC_H264_AU_SPLITTER_H
