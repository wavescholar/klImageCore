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

#include "umc_filter_depth_conversion.h"
#include "umc_video_data.h"
#include "functions_common.h"

#include "ipps.h"

using namespace UMC;


static bool InvalidData(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_iWidth != pData2->m_iWidth || pData1->m_iHeight != pData2->m_iHeight)
        return true;
    return false;
}

VideoDepthConversion::VideoDepthConversion()
{
    m_pInterData        = NULL;
    m_size.width        = 0;
    m_size.height       = 0;
    m_srcFormat         = NONE;
    m_dstFormat         = NONE;
    m_bUseIntermediate  = false;
    m_bStateInitialized = false;
}

VideoDepthConversion::~VideoDepthConversion()
{
    Close();
}

Status VideoDepthConversion::Init(BaseCodecParams *pParams)
{
    VideoDepthConversionParams *params = DynamicCast<VideoDepthConversionParams, BaseCodecParams>(pParams);

    m_params = *params;

    m_pInterData    = new VideoData();

    return UMC_OK;
}

Status VideoDepthConversion::Close()
{
    UMC_DELETE(m_pInterData);

    return UMC_OK;
}

Status VideoDepthConversion::GetFrame(MediaData *input, MediaData *output)
{
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);
    bool   bConvert = false;
    Ipp32u i;

    UMC_CHECK(in,    UMC_ERR_NULL_PTR);
    UMC_CHECK(out,   UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);

    // nothing to do
    if(in == out)
        return UMC_OK;
    for(i = 0; i < in->GetPlanesNumber(); i++)
    {
        if(in->GetPlaneBitDepth(i) != out->GetPlaneBitDepth(i) || in->GetPlaneSampleSize(i) != out->GetPlaneSampleSize(i))
        {
            bConvert = true;
            break;
        }
    }
    if(!bConvert)
    {
        if(!out->GetPlaneDataPtr(0))
            return UMC_WRN_SKIP;
        else
            in->Copy(out);
        return UMC_OK;
    }

    // unsuported configurations
    if(in->GetMaxSampleSize() != 1 && in->GetMaxSampleSize() != 2)
        return UMC_ERR_INVALID_PARAMS;

    if(!out->GetDataPointer())
    {
        m_bStateInitialized = !InvalidData(in, inter);
        m_bUseIntermediate = true;
    }
    else
    {
        m_bUseIntermediate = false;
        if(InvalidData(in, out))
            return UMC_ERR_INVALID_PARAMS;
    }

    if(m_srcFormat != in->m_colorFormat || m_dstFormat != out->m_colorFormat)
        m_bStateInitialized = false;

    if(!m_bStateInitialized)
    {
        m_size.width  = in->m_iWidth;
        m_size.height = in->m_iHeight;
        m_srcFormat   = in->m_colorFormat;
        m_dstFormat   = out->m_colorFormat;

        if(m_bUseIntermediate)
        {
            inter->Close();
            inter->Init(in);
            for(Ipp32u i = 0; i < inter->GetPlanesNumber(); i++)
            {
                inter->SetPlaneSampleSize(1, i);
                inter->SetPlaneBitDepth(8, i);
            }
            inter->Alloc();
        }

        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    VideoData::PlaneData *pSrcPlane;
    VideoData::PlaneData *pDstPlane;
    IppiSize size;
    Ipp32u iPlanes;
    IppStatus sts = ippStsNoErr;

    iPlanes = in->GetPlanesNumber();
    if (iPlanes > out->GetPlanesNumber())
        iPlanes = out->GetPlanesNumber();

    for (i = 0; i < iPlanes; i++)
    {
        pSrcPlane = in->GetPtrToPlane(i);
        pDstPlane = out->GetPtrToPlane(i);

        size.width  = pSrcPlane->m_size.width * pSrcPlane->m_iSamples;
        size.height = pSrcPlane->m_size.height;

        if(pSrcPlane->m_iSampleSize == pDstPlane->m_iSampleSize)
        {
            size.width *= pSrcPlane->m_iSampleSize;
            sts = ippiCopy_8u_C1R(pSrcPlane->m_pPlane, (Ipp32s)pSrcPlane->m_iPitch, pDstPlane->m_pPlane, (Ipp32s)pDstPlane->m_iPitch, size);
        }
        else if (pSrcPlane->m_iSampleSize == 2 && pDstPlane->m_iSampleSize == 1)
            cppiConvert_16s8u_C1R((const Ipp16s*)pSrcPlane->m_pPlane, (Ipp32s)pSrcPlane->m_iPitch, pSrcPlane->m_iBitDepth, pDstPlane->m_pPlane, (Ipp32s)pDstPlane->m_iPitch, size);
        else if (pSrcPlane->m_iSampleSize == 1 && pDstPlane->m_iSampleSize == 2)
            cppiConvert_8u16s_C1R((const Ipp8u*)pSrcPlane->m_pPlane, (Ipp32s)pSrcPlane->m_iPitch, pDstPlane->m_iBitDepth, (Ipp16s*)pDstPlane->m_pPlane, (Ipp32s)pDstPlane->m_iPitch, size);
        else
            return UMC_ERR_UNSUPPORTED;
    }

    return (ippStsNoErr == sts) ? UMC_OK : UMC_ERR_FAILED;
}
