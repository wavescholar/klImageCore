/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_bitstream_inlines.h"
#include "umc_h264_segment_decoder_templates.h"

namespace UMC
{

SegmentDecoderHPBase *CreateSD_ManyBits(Ipp32s bit_depth_luma,
                                        Ipp32s bit_depth_chroma,
                                        bool is_field,
                                        Ipp32s color_format,
                                        bool is_high_profile)
{
    if (bit_depth_chroma > 8 || bit_depth_luma > 8)
    {
        if (is_field)
        {
            return CreateSegmentDecoderWrapper<Ipp32s, Ipp16u, Ipp16u, true>::CreateSoftSegmentDecoder(color_format, is_high_profile);
        } else {
            return CreateSegmentDecoderWrapper<Ipp32s, Ipp16u, Ipp16u, false>::CreateSoftSegmentDecoder(color_format, is_high_profile);
        }
    }
    else
    {
        // this function should be called from CreateSD
        VM_ASSERT(false);
    }

    return NULL;

} // SegmentDecoderHPBase *CreateSD(Ipp32s bit_depth_luma,

void InitializeSDCreator_ManyBits()
{
    CreateSegmentDecoderWrapper<Ipp32s, Ipp16u, Ipp16u, true>::CreateSoftSegmentDecoder(0, false);
    CreateSegmentDecoderWrapper<Ipp32s, Ipp16u, Ipp16u, false>::CreateSoftSegmentDecoder(0, false);
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
