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

#include "umc_h264_slice_decoding.h"
#include "umc_h264_segment_decoder_mt.h"
#include "umc_h264_heap.h"


namespace UMC
{


H264Slice::H264Slice(MemoryAllocator *pMemoryAllocator)
    : m_pMemoryAllocator(pMemoryAllocator)
{
    Reset();
} // H264Slice::H264Slice()

H264Slice::~H264Slice()
{
    Release();

} // H264Slice::~H264Slice(void)

void H264Slice::Reset()
{
    // DEBUG tools: set following variable to turn of deblocking
    m_bPermanentTurnOffDeblocking = false;
    // end of DEBUG tools

    m_pPicParamSet = 0;
    m_pSeqParamSet = 0;
    m_pSeqParamSetEx = 0;

    m_iMBWidth = -1;
    m_iMBHeight = -1;
    m_CurrentPicParamSet = -1;
    m_CurrentSeqParamSet = -1;

    m_iAllocatedMB = 0;

    m_pNext = 0;
    m_pCurrentFrame = 0;

    m_DistScaleFactorAFF = 0;
    m_DistScaleFactorMVAFF = 0;
    m_isInitialized = false;
}

void H264Slice::Release()
{
    m_CoeffsBuffers.Reset();

    m_pObjHeap->FreeObject<FactorArrayAFF>(m_DistScaleFactorAFF);
    m_pObjHeap->FreeObject<FactorArrayMVAFF>(m_DistScaleFactorMVAFF);

    Reset();

} // void H264Slice::Release(void)

bool H264Slice::Init(Ipp32s )
{
    // release object before initialization
    Release();

    return true;

} // bool H264Slice::Init(Ipp32s iConsumerNumber)

Ipp32s H264Slice::RetrievePicParamSetNumber(void *pSource, size_t nSourceSize)
{
    if (!nSourceSize)
        return -1;

    m_BitStream.Reset((Ipp8u *) pSource, (Ipp32u) nSourceSize);

    Status umcRes = UMC_OK;

    try
    {
        NAL_Unit_Type NALUType;
        Ipp8u nal_ref_idc;

        umcRes = m_BitStream.GetNALUnitType(NALUType, nal_ref_idc);
        if (UMC_OK != umcRes)
            return false;

        m_SliceHeader.nal_unit_type = NALUType;

        // decode first part of slice header
        umcRes = m_BitStream.GetSliceHeaderPart1(&m_SliceHeader);
        if (UMC_OK != umcRes)
            return -1;
    }
    catch(...)
    {
        return -1;
    }

    return m_SliceHeader.pic_parameter_set_id;
}

bool H264Slice::Reset(void *pSource, size_t nSourceSize, Ipp32s iConsumerNumber)
{
    Ipp32s iMBInFrame;
    Ipp32s iFieldIndex;

    m_BitStream.Reset((Ipp8u *) pSource, (Ipp32u) nSourceSize);

    // decode slice header
    if (nSourceSize && false == DecodeSliceHeader())
        return false;

    m_iMBWidth  = GetSeqParam()->frame_width_in_mbs;
    m_iMBHeight = GetSeqParam()->frame_height_in_mbs;

    iMBInFrame = (m_iMBWidth * m_iMBHeight) / ((m_SliceHeader.field_pic_flag) ? (2) : (1));
    iFieldIndex = (m_SliceHeader.field_pic_flag && m_SliceHeader.bottom_field_flag) ? (1) : (0);

    // set slice variables
    m_iFirstMB = m_SliceHeader.first_mb_in_slice;
    m_iMaxMB = iMBInFrame;

    m_iFirstMBFld = m_SliceHeader.first_mb_in_slice + iMBInFrame * iFieldIndex;

    m_iAvailableMB = iMBInFrame;

    if (m_iFirstMB >= m_iAvailableMB)
        return false;

    // reset all internal variables
    m_iCurMBToDec = m_iFirstMB;
    m_iCurMBToRec = m_iFirstMB;
    m_iCurMBToDeb = m_iFirstMB;

    m_bInProcess = false;
    m_bDecVacant = 1;
    m_bRecVacant = 1;
    m_bDebVacant = 1;
    m_bFirstDebThreadedCall = true;
    m_bError = false;

    m_MVsDistortion = 0;

    // reallocate internal buffer
    if (false == IsSliceGroups() && iConsumerNumber > 1)
    {
        Ipp32s iMBRowSize = GetMBRowWidth();
        Ipp32s iMBRowBuffers;
        Ipp32s bit_depth_luma, bit_depth_chroma;
        if (m_SliceHeader.is_auxiliary)
        {
            bit_depth_luma = GetSeqParamEx()->bit_depth_aux;
            bit_depth_chroma = 8;
        } else {
            bit_depth_luma = GetSeqParam()->bit_depth_luma;
            bit_depth_chroma = GetSeqParam()->bit_depth_chroma;
        }

        Ipp32s isU16Mode = (bit_depth_luma > 8 || bit_depth_chroma > 8) ? 2 : 1;

        // decide number of buffers
        iMBRowBuffers = IPP_MAX(MINIMUM_NUMBER_OF_ROWS, MB_BUFFER_SIZE / iMBRowSize);
        iMBRowBuffers = IPP_MIN(MAXIMUM_NUMBER_OF_ROWS, iMBRowBuffers);

        m_CoeffsBuffers.Init(iMBRowBuffers, (Ipp32s)sizeof(Ipp16s) * isU16Mode * (iMBRowSize * COEFFICIENTS_BUFFER_SIZE + DEFAULT_ALIGN_VALUE));
    }

    m_CoeffsBuffers.Reset();

    // reset through-decoding variables
    m_nMBSkipCount = 0;
    m_nQuantPrev = m_pPicParamSet->pic_init_qp +
                   m_SliceHeader.slice_qp_delta;
    m_prev_dquant = 0;
    m_field_index = iFieldIndex;

    if (IsSliceGroups())
        m_bNeedToCheckMBSliceEdges = true;
    else
        m_bNeedToCheckMBSliceEdges = (0 == m_SliceHeader.first_mb_in_slice) ? (false) : (true);

    // set conditional flags
    m_bDecoded = false;
    m_bPrevDeblocked = false;
    m_bDeblocked = (m_SliceHeader.disable_deblocking_filter_idc == DEBLOCK_FILTER_OFF);

    if (m_bDeblocked)
    {
        m_bDebVacant = 0;
        m_iCurMBToDeb = m_iMaxMB;
    }

    // frame is not associated yet
    m_pCurrentFrame = NULL;

    return true;

} // bool H264Slice::Reset(void *pSource, size_t nSourceSize, Ipp32s iNumber)

void H264Slice::SetSliceNumber(Ipp32s iSliceNumber)
{
    m_iNumber = iSliceNumber;

} // void H264Slice::SetSliceNumber(Ipp32s iSliceNumber)

AdaptiveMarkingInfo * H264Slice::GetAdaptiveMarkingInfo()
{
    return &m_AdaptiveMarkingInfo;
}

bool H264Slice::DecodeSliceHeader(bool bFullInitialization)
{
    Status umcRes = UMC_OK;
    // Locals for additional slice data to be read into, the data
    // was read and saved from the first slice header of the picture,
    // is not supposed to change within the picture, so can be
    // discarded when read again here.
    bool bIsIDRPic = false;
    NAL_Unit_Type NALUType;
    Ipp8u nal_ref_idc;
    Ipp32s iSQUANT;

    try
    {
        memset(&m_SliceHeader, 0, sizeof(m_SliceHeader));

        umcRes = m_BitStream.GetNALUnitType(NALUType, nal_ref_idc);
        if (UMC_OK != umcRes)
            return false;

        if ((NALUType != NAL_UT_SLICE) && (NALUType != NAL_UT_IDR_SLICE) &&
            (NALUType != NAL_UT_AUXILIARY))
        {
            VM_ASSERT((NALUType == NAL_UT_SLICE) || (NALUType == NAL_UT_IDR_SLICE) || (NALUType == NAL_UT_AUXILIARY));
            return false;
        }

        m_SliceHeader.nal_unit_type = NALUType;
        bIsIDRPic = (NALUType == NAL_UT_IDR_SLICE);

        if (NALUType == NAL_UT_AUXILIARY)
        {
            if (!m_pCurrentFrame || !GetSeqParamEx())
                return false;

            bIsIDRPic = m_pCurrentFrame->m_bIDRFlag;
            NALUType = bIsIDRPic ? NAL_UT_IDR_SLICE : NAL_UT_SLICE;
            m_SliceHeader.nal_unit_type = NALUType;
            m_SliceHeader.is_auxiliary = true;
        }

        // decode first part of slice header
        umcRes = m_BitStream.GetSliceHeaderPart1(&m_SliceHeader);
        if (UMC_OK != umcRes)
            return false;

        m_CurrentPicParamSet = m_SliceHeader.pic_parameter_set_id;
        m_CurrentSeqParamSet = m_pPicParamSet->seq_parameter_set_id;

        // decode second part of slice header
        umcRes = m_BitStream.GetSliceHeaderPart2(&m_SliceHeader,
                                                 m_pPicParamSet,
                                                 bIsIDRPic,
                                                 m_pSeqParamSet,
                                                 nal_ref_idc);
        if (UMC_OK != umcRes)
            return false;

        // when we require only slice header
        if (false == bFullInitialization)
            return true;

        // decode second part of slice header
        umcRes = m_BitStream.GetSliceHeaderPart3(&m_SliceHeader,
                                                 m_PredWeight[0],
                                                 m_PredWeight[1],
                                                 &ReorderInfoL0,
                                                 &ReorderInfoL1,
                                                 &m_AdaptiveMarkingInfo,
                                                 m_pPicParamSet,
                                                 m_pSeqParamSet,
                                                 nal_ref_idc);
        if (UMC_OK != umcRes)
            return false;

        m_iMBWidth = m_pSeqParamSet->frame_width_in_mbs;
        m_iMBHeight = m_pSeqParamSet->frame_height_in_mbs;

        if (m_bPermanentTurnOffDeblocking)
            m_SliceHeader.disable_deblocking_filter_idc = DEBLOCK_FILTER_OFF;

        // redundant slice, discard
        if (m_SliceHeader.redundant_pic_cnt)
            return false;

        // Set next MB.
        if (m_SliceHeader.first_mb_in_slice >= (Ipp32s) (m_iMBWidth * m_iMBHeight))
        {
            return false;
        }

        Ipp32s bit_depth_luma = m_SliceHeader.is_auxiliary ?
            GetSeqParamEx()->bit_depth_aux : GetSeqParam()->bit_depth_luma;

        iSQUANT = m_pPicParamSet->pic_init_qp +
                  m_SliceHeader.slice_qp_delta;
        if (iSQUANT < QP_MIN - 6*(bit_depth_luma - 8)
            || iSQUANT > QP_MAX)
        {
            return false;
        }

        if (m_pPicParamSet->entropy_coding_mode)
            m_BitStream.AlignPointerRight();
    }
    catch(const h264_exception &)
    {
        return false;
    }
    catch(...)
    {
        return false;
    }

    return (UMC_OK == umcRes);

} // bool H264Slice::DecodeSliceHeader(bool bFullInitialization)

void H264Slice::InitializeContexts()
{
    if (!m_isInitialized && m_pPicParamSet->entropy_coding_mode)
    {
        // reset CABAC engine
        m_BitStream.InitializeDecodingEngine_CABAC();
        if (INTRASLICE == m_SliceHeader.slice_type)
        {
            m_BitStream.InitializeContextVariablesIntra_CABAC(m_pPicParamSet->pic_init_qp +
                                                              m_SliceHeader.slice_qp_delta);
        }
        else
        {
            m_BitStream.InitializeContextVariablesInter_CABAC(m_pPicParamSet->pic_init_qp +
                                                              m_SliceHeader.slice_qp_delta,
                                                              m_SliceHeader.cabac_init_idc);
        }
    }

    m_isInitialized = true;
}

bool H264Slice::GetDeblockingCondition(void) const
{
    // there is no deblocking
    if (DEBLOCK_FILTER_OFF == m_SliceHeader.disable_deblocking_filter_idc)
        return false;

    // no filtering edges of this slice
    if ((DEBLOCK_FILTER_ON_NO_SLICE_EDGES == m_SliceHeader.disable_deblocking_filter_idc) ||
        (m_bPrevDeblocked))
    {
        if (false == IsSliceGroups())
            return true;
    }

    return false;

} // bool H264Slice::GetDeblockingCondition(void)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
