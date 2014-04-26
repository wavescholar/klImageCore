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
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "memory"

#include "umc_h264_au_splitter.h"

#include "umc_h264_nal_spl.h"
#include "umc_h264_bitstream.h"
#include "umc_h264_dec_defs_dec.h"
#include "umc_structures.h"

#include "umc_frame_data.h"


namespace UMC
{

AU_Splitter::AU_Splitter(H264_Heap *heap, H264_Heap_Objects *objectHeap)
    : m_Headers(objectHeap), m_objHeap(objectHeap), m_heap(heap)
{
}

AU_Splitter::~AU_Splitter()
{
    Close();
}

void AU_Splitter::Init(VideoDecoderParams *init)
{
    Close();

    if (init->m_info.streamSubtype == AVC1_VIDEO)
    {
        m_pNALSplitter.reset(new NALUnitSplitterMP4(m_heap));
    }
    else
    {
        m_pNALSplitter.reset(new NALUnitSplitter(m_heap));
    }

    m_pNALSplitter->Init();
}

void AU_Splitter::Close()
{
    m_pNALSplitter.reset(0);
}

void AU_Splitter::Reset()
{
    if (m_pNALSplitter.get())
        m_pNALSplitter->Reset();

    m_Headers.Reset();
}

void AU_Splitter::SetSplitterListner(SplitterListner * listner)
{
    m_listner = listner;
}

Ipp32s AU_Splitter::CheckNalUnitType(MediaData * src)
{
    return m_pNALSplitter->CheckNalUnitType(src);
}

MediaDataEx * AU_Splitter::GetNalUnit(MediaData * src)
{
    return m_pNALSplitter->GetNalUnits(src);
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
