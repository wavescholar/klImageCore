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

#include "umc_filter_resize.h"
#include "umc_video_data.h"
#include "ippcore.h"
#include "ippi.h"

using namespace UMC;


typedef enum
{
    RF_RGB = 0,
    RF_YUY2,
    RF_NV12
} ResizeFunctions;

static bool InvalidData(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_colorFormat != pData2->m_colorFormat || pData1->GetPlanesNumber() != pData2->GetPlanesNumber())
        return true;
    return false;
}

VideoResize::VideoResize()
{
    m_plane             = NULL;
    m_iPlanes           = 0;
    m_srcSize.width     = 0;
    m_srcSize.height    = 0;
    m_dstSize.width     = 0;
    m_dstSize.height    = 0;
    m_iFunctions        = RF_RGB;
    m_bStateInitialized = false;
    m_bUseIntermediate  = false;
    m_bAllocated        = false;
    m_pInterData        = NULL;
}

VideoResize::~VideoResize()
{
    Close();
}

Status VideoResize::Init(BaseCodecParams *pParams)
{
    VideoResizeParams *params = DynamicCast<VideoResizeParams, BaseCodecParams>(pParams);

    if (NULL == params)
        return UMC_ERR_NULL_PTR;

    m_params = *params;

    m_pInterData = new VideoData();

    return UMC_OK;
}

Status VideoResize::InitInternal(MediaData *input, MediaData *output)
{
    VideoData *in = DynamicCast<VideoData, MediaData>(input);
    VideoData *out = DynamicCast<VideoData, MediaData>(output);
    VideoData::PlaneData *pSrcPlane;
    VideoData::PlaneData *pDstPlane;

    Ipp32s    iExtBufferSize = 0;
    IppStatus ippSts = ippStsNoErr;

    CloseInternal();

    m_iFillValue8  = (Ipp8u)m_params.m_iFillValue;
    m_iFillValue16 = (Ipp16u)m_params.m_iFillValue;

    m_fXRatio = (Ipp64f)(out->m_iWidth)/(Ipp64f)(in->m_iWidth);
    m_fYRatio = (Ipp64f)(out->m_iHeight)/(Ipp64f)(in->m_iHeight);

    m_plane = new PlaneParams[m_iPlanes];
    memset(m_plane, 0, sizeof(PlaneParams)*m_iPlanes);

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        m_plane[i].dstOffset.x = m_plane[i].dstOffset.y = 0;

        pSrcPlane = in->GetPtrToPlane(i);
        pDstPlane = out->GetPtrToPlane(i);

        UMC_CHECK(pSrcPlane->m_iSampleSize == pDstPlane->m_iSampleSize, UMC_ERR_INVALID_PARAMS);
        UMC_CHECK(pSrcPlane->m_iSamples == pDstPlane->m_iSamples, UMC_ERR_INVALID_PARAMS);

        m_plane[i].rectSrc.x = m_plane[i].rectSrc.y = 0;
        m_plane[i].rectSrc.width = pSrcPlane->m_size.width;
        m_plane[i].rectSrc.height = pSrcPlane->m_size.height;

        ippiGetResizeRect(m_plane[i].rectSrc, &m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType);

        ippSts = ippiResizeGetBufSize(m_plane[i].rectSrc, m_plane[i].rectDst, pSrcPlane->m_iSamples, m_params.m_interpolationType, &iExtBufferSize);
        if(ippSts != ippStsNoErr)
            return UMC_ERR_UNSUPPORTED;

        m_plane[i].pExtBuffer = (Ipp8u*)ippMalloc(iExtBufferSize);
        if(m_plane[i].pExtBuffer == NULL)
          return UMC_ERR_NULL_PTR;
    }

    m_bAllocated = true;

    return UMC_OK;
}

Status VideoResize::Close()
{
    CloseInternal();

    UMC_DELETE(m_pInterData);

    return UMC_OK; 
}

Status VideoResize::CloseInternal()
{
    if(m_bAllocated)
    {
        for(Ipp32u i = 0; i < m_iPlanes; i++)
            ippFree(m_plane[i].pExtBuffer);

        delete[] m_plane;
        m_bAllocated = false;
    }

    return UMC_OK; 
}

Status VideoResize::GetFrame(MediaData *input, MediaData *output)
{
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);

    VideoData::PlaneData *pSrcPlane;
    VideoData::PlaneData *pDstPlane;
    Status sts = UMC_OK;
    IppStatus ippSts = ippStsNoErr;

    UMC_CHECK(in,  UMC_ERR_NULL_PTR);
    UMC_CHECK(out, UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);

    // nothing to do
    if(in == out)
        return UMC_OK;
    if(in->m_iWidth == out->m_iWidth && in->m_iHeight == out->m_iHeight)
    {
        if(!out->GetPlaneDataPtr(0))
            return UMC_WRN_SKIP;
        else
            in->Copy(out);
        return UMC_OK;
    }

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

    if(((Ipp32u)m_dstSize.width != out->m_iWidth || (Ipp32u)m_dstSize.height != out->m_iHeight ||
        (Ipp32u)m_srcSize.width != in->m_iWidth || (Ipp32u)m_srcSize.height != in->m_iHeight))
        m_bStateInitialized = false;

    if(!m_bStateInitialized)
    {
        m_srcSize.width  = in->m_iWidth;
        m_srcSize.height = in->m_iHeight;
        m_dstSize.width  = out->m_iWidth;
        m_dstSize.height = out->m_iHeight;
        m_iPlanes        = in->GetPlanesNumber();

        if(m_bUseIntermediate)
        {
            inter->Close();
            inter->Init(in);
            inter->m_iWidth  = out->m_iWidth;
            inter->m_iHeight = out->m_iHeight;
            inter->Alloc();
            sts = InitInternal(in, inter);
            if(UMC_OK != sts)
                return sts;
        }
        else
        {
            sts = InitInternal(in, out);
            if(UMC_OK != sts)
                return sts;
        }

        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    for(Ipp32u i = 0; i < m_iPlanes; i++)
    {
        pSrcPlane = in->GetPtrToPlane(i);
        pDstPlane = out->GetPtrToPlane(i);

        UMC_CHECK(pSrcPlane->m_iSampleSize == pDstPlane->m_iSampleSize, UMC_ERR_INVALID_PARAMS);
        UMC_CHECK(pSrcPlane->m_iSamples == pDstPlane->m_iSamples, UMC_ERR_INVALID_PARAMS);

        if(pSrcPlane->m_iSampleSize != sizeof(Ipp8u) && pSrcPlane->m_iSampleSize != sizeof(Ipp16u))
            return UMC_ERR_UNSUPPORTED;

        if(pSrcPlane->m_iSampleSize == sizeof(Ipp8u))
        {
            if(pSrcPlane->m_iSamples == 1)
                ippSts = ippiResizeSqrPixel_8u_C1R((const Ipp8u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp8u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else if(pSrcPlane->m_iSamples == 3)
                ippSts = ippiResizeSqrPixel_8u_C3R((const Ipp8u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp8u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else if(pSrcPlane->m_iSamples == 4)
                ippSts = ippiResizeSqrPixel_8u_C4R((const Ipp8u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp8u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else
                return UMC_ERR_UNSUPPORTED;
        }
        else
        {
            if(pSrcPlane->m_iSamples == 1)
                ippSts = ippiResizeSqrPixel_16u_C1R((const Ipp16u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp16u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else if(pSrcPlane->m_iSamples == 3)
                ippSts = ippiResizeSqrPixel_16u_C3R((const Ipp16u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp16u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else if(pSrcPlane->m_iSamples == 4)
                ippSts = ippiResizeSqrPixel_16u_C4R((const Ipp16u*)pSrcPlane->m_pPlane, pSrcPlane->m_size, (int)pSrcPlane->m_iPitch, m_plane[i].rectSrc, (Ipp16u*)pDstPlane->m_pPlane, (int)pDstPlane->m_iPitch, m_plane[i].rectDst, m_fXRatio, m_fYRatio, 0, 0, m_params.m_interpolationType, m_plane[i].pExtBuffer);
            else
                return UMC_ERR_UNSUPPORTED;
        }
        if(ippSts != ippStsNoErr)
            return UMC_ERR_FAILED;
        if(sts != UMC_OK)
            return sts;
    }

    return UMC_OK;
}
