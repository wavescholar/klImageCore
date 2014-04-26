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

#include "umc_h264_segment_decoder.h"
#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_bitstream_inlines.h"
#include "umc_h264_segment_decoder_templates.h"
#include "vm_event.h"
#include "vm_thread.h"

namespace UMC
{

H264SegmentDecoder::H264SegmentDecoder(TaskBroker * pTaskBroker)
{
    m_pCoefficientsBuffer = NULL;
    m_nAllocatedCoefficientsBuffer = 0;
    m_pTaskBroker = pTaskBroker;

    m_pSlice = 0;

    // set pointer to backward prediction buffer
    m_pPredictionBuffer = align_pointer<UMC::PlanePtrYCommon>(m_BufferForBackwardPrediction, DEFAULT_ALIGN_VALUE);

    bit_depth_luma = 8;
    bit_depth_chroma = 8;
} // H264SegmentDecoder::H264SegmentDecoder(H264SliceStore &Store)

H264SegmentDecoder::~H264SegmentDecoder(void)
{
    Release();

} // H264SegmentDecoder::~H264SegmentDecoder(void)

void H264SegmentDecoder::Release(void)
{
    if (m_pCoefficientsBuffer)
        delete [] (Ipp32s*)m_pCoefficientsBuffer;

    m_pCoefficientsBuffer = NULL;
    m_nAllocatedCoefficientsBuffer = 0;
} // void H264SegmentDecoder::Release(void)

Status H264SegmentDecoder::Init(Ipp32s iNumber)
{
    // release object before initialization
    Release();

    // save ordinal number
    m_iNumber = iNumber;

    // ADB
    m_pCoefficientsBuffer = (UMC::CoeffsCommon*)(new Ipp32s[COEFFICIENTS_BUFFER_SIZE + DEFAULT_ALIGN_VALUE]);
    m_nAllocatedCoefficientsBuffer = 1;

    return UMC_OK;

} // Status H264SegmentDecoder::Init(Ipp32s sNumber)

UMC::CoeffsPtrCommon H264SegmentDecoder::GetCoefficientsBuffer(Ipp32u nNum)
{
    return align_pointer<UMC::CoeffsPtrCommon> (m_pCoefficientsBuffer +
                                     COEFFICIENTS_BUFFER_SIZE * nNum, DEFAULT_ALIGN_VALUE);

} // Ipp16s *H264SegmentDecoder::GetCoefficientsBuffer(Ipp32u nNum)

SegmentDecoderHPBase *CreateSD(Ipp32s bit_depth_luma,
                               Ipp32s bit_depth_chroma,
                               bool is_field,
                               Ipp32s color_format,
                               bool is_high_profile)
{
    if (bit_depth_chroma > 8 || bit_depth_luma > 8)
    {
        return CreateSD_ManyBits(bit_depth_luma,
                                 bit_depth_chroma,
                                 is_field,
                                 color_format,
                                 is_high_profile);
    }
    else
    {
        if (is_field)
        {
            return CreateSegmentDecoderWrapper<Ipp16s, Ipp8u, Ipp8u, true>::CreateSoftSegmentDecoder(color_format, is_high_profile);
        } else {
            return CreateSegmentDecoderWrapper<Ipp16s, Ipp8u, Ipp8u, false>::CreateSoftSegmentDecoder(color_format, is_high_profile);
        }
    }

} // SegmentDecoderHPBase *CreateSD(Ipp32s bit_depth_luma,

static
void InitializeSDCreator()
{
    CreateSegmentDecoderWrapper<Ipp16s, Ipp8u, Ipp8u, true>::CreateSoftSegmentDecoder(0, false);
    CreateSegmentDecoderWrapper<Ipp16s, Ipp8u, Ipp8u, false>::CreateSoftSegmentDecoder(0, false);
}

class SDInitializer
{
public:
    SDInitializer()
    {
        InitializeSDCreator();
        InitializeSDCreator_ManyBits();
    }
};

static SDInitializer tableInitializer;


} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
