/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#include "umc_video_processing.h"

using namespace UMC;


VideoProcessing::VideoProcessing()
{
    m_iNumFilters = 0;
}

VideoProcessing::~VideoProcessing()
{
    Close();
}

Status VideoProcessing::Init(BaseCodecParams *pParams)
{
    VideoProcessingParams *params = DynamicCast<VideoProcessingParams, BaseCodecParams>(pParams);

    m_params = *params;

    return UMC_OK;
}

Status VideoProcessing::Close()
{
    for(List<BaseCodec*>::Iterator filter = m_filtersChain.ItrFront(); filter != m_filtersChain.ItrBackBound(); ++filter)
        delete (*(BaseCodec**)filter);

    return UMC_OK;
}

Status VideoProcessing::AddFilter(VideoFilter iFilter, BaseCodecParams *filterParams)
{
    BaseCodec *pFilter;

    switch(iFilter)
    {
    case FILTER_DEPTH_CONVERTER:
        pFilter = new VideoDepthConversion();
        break;
    case FILTER_DEINTERLACER:
        pFilter = new VideoDeinterlace();
        break;
    case FILTER_COLOR_CONVERTER:
        pFilter = new VideoColorConversion();
        break;
    case FILTER_RESIZER:
        pFilter = new VideoResize();
        break;
    case FILTER_ANAGLYPHER:
        pFilter = new VideoAnaglyph();
        break;
    default:
        return UMC_ERR_NOT_IMPLEMENTED;
    }

    pFilter->Init(filterParams);

    m_filtersChain.PushBack();
    m_filtersChain.Back() = pFilter;

    m_iNumFilters++;

    return UMC_OK;
}

Status VideoProcessing::GetFrame(MediaData* input, MediaData* output)
{
    VideoData *pInData  = DynamicCast<VideoData, MediaData>(input);
    VideoData *pOutData = DynamicCast<VideoData, MediaData>(output);
    VideoData  interData[2]; // swap chain buffers
    VideoData *pSrc;
    VideoData *pDst;
    Ipp32u     iFiltersCount = 0;
    Status     res;

    Ipp32s iFrameSize;

    UMC_CHECK(pInData,  UMC_ERR_NULL_PTR);
    UMC_CHECK(pOutData, UMC_ERR_NULL_PTR);

    if(pOutData->GetPlaneDataPtr(0) == NULL)
        return UMC_ERR_NULL_PTR;

    iFrameSize = (Ipp32s)pOutData->GetMappingSize();
    if(iFrameSize <= 0)
        iFrameSize = (Ipp32s)pInData->GetMappingSize();

    if(iFrameSize <= 0)
        iFrameSize = 1;

    pSrc = pInData;
    pDst = &interData[0];
    for(List<BaseCodec*>::Iterator filter = m_filtersChain.ItrFront(); filter != m_filtersChain.ItrBackBound(); ++filter)
    {
        iFiltersCount++;
        if(iFiltersCount == m_iNumFilters)
            pDst = pOutData;
        else
            pDst->Init(pOutData);

        res = (*(BaseCodec**)filter)->GetFrame(pSrc, pDst);
        if(res == UMC_ERR_NOT_ENOUGH_DATA) // filter requires more than one frame
            return res;
        if(res == UMC_WRN_SKIP) // empty filter
            continue;
        if(res != UMC_OK)
        {
            vm_string_printf(VM_STRING("Video processing: chain filtration failure\n"));
            return res;
        }

        pSrc = pDst;
        pDst = ((pDst == &interData[0])?(&interData[1]):(&interData[0])); // swap buffer
    }
    if(m_iNumFilters == 0)
    {
        res = pInData->Copy(pOutData);
        if(res != UMC_OK)
        {
            vm_string_printf(VM_STRING("Video processing: final data formats do not match\n"));
            return res;
        }
    }

    pOutData->m_frameType = pInData->m_frameType;
    pOutData->m_fPTSStart = pInData->m_fPTSStart;
    pOutData->m_fPTSEnd   = pInData->m_fPTSEnd;
    pInData->SetDataSize(0);
    pOutData->SetDataSize(iFrameSize);

    return UMC_OK;
}
