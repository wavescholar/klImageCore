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

#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_video_decoder.h"
#include "umc_h264_segment_decoder_templates.h"

namespace UMC
{

Status H264SegmentDecoderMultiThreaded::DecodeMacroBlockCABAC(Ipp32u nCurMBNumber,
                                                              Ipp32u &nMaxMBNumber)
{
    Status status = m_SD->DecodeSegmentCABAC(nCurMBNumber, nMaxMBNumber, this);
    return status;

} // Status H264SegmentDecoderMultiThreaded::DecodeMacroBlockCABAC(Ipp32u nCurMBNumber,

Status H264SegmentDecoderMultiThreaded::ReconstructMacroBlockCABAC(Ipp32u nCurMBNumber,
                                                                   Ipp32u nMaxMBNumber)
{
    Status status = m_SD->ReconstructSegment(nCurMBNumber, nMaxMBNumber, this);
    return status;
} // Status H264SegmentDecoderMultiThreaded::ReconstructMacroBlockCABAC(Ipp32u nCurMBNumber,

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
