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

#include "umc_filter_color_conversion.h"
#include "umc_video_data.h"
#include "functions_common.h"

#include "ipps.h"
#include "ippcc.h"

using namespace UMC;


static bool InvalidData(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_iWidth != pData2->m_iWidth || pData1->m_iHeight != pData2->m_iHeight)
        return true;
    return false;
}

template <class T> inline
void SwapValues(T &one, T& two)
{
  T tmp;
  tmp = one;
  one = two;
  two = tmp;
}

static Status CopyImage(VideoData *pSrc, VideoData *pDst, int flag, int bSwapUV)
{
    VideoData::PlaneData *pSrcPlane;
    VideoData::PlaneData *pDstPlane;
    IppiSize size;
    Ipp32u iPlanes;
    Ipp32u iDstPlane;
    IppStatus sts = ippStsNoErr;

    flag = flag;
    iPlanes = pSrc->GetPlanesNumber();
    if (iPlanes > pDst->GetPlanesNumber())
        iPlanes = pDst->GetPlanesNumber();

    for (iDstPlane = 0; iDstPlane < iPlanes; iDstPlane++)
    {
        int iSrcPlane = iDstPlane;
        if (bSwapUV)
        {
            if (iDstPlane == 1)
                iSrcPlane = 2;
            else if (iDstPlane == 2)
                iSrcPlane = 1;
        }

        pSrcPlane = pSrc->GetPtrToPlane(iSrcPlane);
        pDstPlane = pDst->GetPtrToPlane(iDstPlane);

        size.width  = pSrcPlane->m_size.width * pSrcPlane->m_iSamples;
        size.height = pSrcPlane->m_size.height;

        size.width *= pSrcPlane->m_iSampleSize;
        sts = ippiCopy_8u_C1R(pSrcPlane->m_pPlane, (Ipp32s)pSrcPlane->m_iPitch, pDstPlane->m_pPlane, (Ipp32s)pDstPlane->m_iPitch, size);
    }

    return (ippStsNoErr == sts) ? UMC_OK : UMC_ERR_FAILED;
}


VideoColorConversion::VideoColorConversion()
{
    m_pInterData        = NULL;
    m_pInterConvert     = NULL;
    m_iCopyMode         = 0;
    m_size.width        = 0;
    m_size.height       = 0;
    m_srcFormat         = NONE;
    m_dstFormat         = NONE;
    m_bSrcSwapUV        = false;
    m_bDstSwapUV        = false;
    m_bUseIntermediate  = false;
    m_bStateInitialized = false;
}

VideoColorConversion::~VideoColorConversion()
{
    Close();
}

Status VideoColorConversion::Init(BaseCodecParams *pParams)
{
    VideoColorConversionParams *params = DynamicCast<VideoColorConversionParams, BaseCodecParams>(pParams);

    m_params = *params;

    m_pInterData    = new VideoData();
    m_pInterConvert = new VideoData();

    return UMC_OK;
}

Status VideoColorConversion::Close()
{
    UMC_DELETE(m_pInterData);
    UMC_DELETE(m_pInterConvert);

    return UMC_OK;
}

Status VideoColorConversion::GetFrame(MediaData *input, MediaData *output)
{
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);
    VideoData *cvt   = DynamicCast<VideoData, MediaData>(m_pInterConvert);
    Ipp32s iOrder[4] = {2,1,0,3};

    UMC_CHECK(in,    UMC_ERR_NULL_PTR);
    UMC_CHECK(out,   UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);

    // nothing to do
    if(in == out)
        return UMC_OK;
    if(in->m_colorFormat == out->m_colorFormat)
    {
        if(!out->GetPlaneDataPtr(0))
            return UMC_WRN_SKIP;
        else
            in->Copy(out);
        return UMC_OK;
    }

    // unsuported configurations
    if(in->GetMaxSampleSize() != 1)
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

        m_bSrcSwapUV = 0;
        if(m_srcFormat == YV12) // process YV12 as YUV420
            m_bSrcSwapUV = 1;

        m_bDstSwapUV = 0;
        if(m_dstFormat == YV12) // process YV12 as YUV420
            m_bDstSwapUV = 1;

        m_iCopyMode  = 0;
        if(m_srcFormat == YUV420 && m_dstFormat == YV12)
            m_iCopyMode = 1;
        if(m_srcFormat == YV12 && m_dstFormat == YUV420)
            m_iCopyMode = 1;
        if(m_srcFormat == m_dstFormat)
            m_iCopyMode = 1;
        if(YUV420A == m_srcFormat && YUV420 == m_dstFormat)
            m_iCopyMode = 3;
        if(GRAYA == m_srcFormat && GRAY == m_dstFormat)
            m_iCopyMode = 4;

        if(m_bUseIntermediate)
        {
            inter->Close();
            inter->Init(in);
            inter->m_colorFormat = m_dstFormat;
            inter->Alloc();
        }

        // reset buffer
        cvt->Close();

        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    if(m_iCopyMode)
        return CopyImage(in, out, m_iCopyMode, m_bSrcSwapUV ^ m_bDstSwapUV);

    const Ipp8u *(pSrc[4]) = {(Ipp8u*)in->GetPlaneDataPtr(0), (Ipp8u*)in->GetPlaneDataPtr(1), (Ipp8u*)in->GetPlaneDataPtr(2), (Ipp8u*)in->GetPlaneDataPtr(3)};
    Ipp32s pSrcStep[4]     = {(Ipp32s)in->GetPlanePitch(0), (Ipp32s)in->GetPlanePitch(1), (Ipp32s)in->GetPlanePitch(2), (Ipp32s)in->GetPlanePitch(3)};
    Ipp8u *(pDst[4])       = {(Ipp8u*)out->GetPlaneDataPtr(0), (Ipp8u*)out->GetPlaneDataPtr(1), (Ipp8u*)out->GetPlaneDataPtr(2), (Ipp8u*)out->GetPlaneDataPtr(3)};
    Ipp32s pDstStep[4]     = {(Ipp32s)out->GetPlanePitch(0), (Ipp32s)out->GetPlanePitch(1), (Ipp32s)out->GetPlanePitch(2), (Ipp32s)out->GetPlanePitch(3)};

    if(m_bSrcSwapUV)
    {
        SwapValues(pSrc[1], pSrc[2]);
        SwapValues(pSrcStep[1], pSrcStep[2]);
    }
    if(m_bDstSwapUV)
    {
        SwapValues(pDst[1], pDst[2]);
        SwapValues(pDstStep[1], pDstStep[2]);
    }

    const Ipp8u *(pYVU[4]) = {pSrc[0], pSrc[2], pSrc[1], pSrc[3]};
    Ipp32s pYVUStep[4]     = {pSrcStep[0], pSrcStep[2], pSrcStep[1], pSrcStep[3]};
    Ipp8u *(pDstYVU[4])    = {pDst[0], pDst[2], pDst[1]};
    Ipp32s pDstStepYVU[4]  = {pDstStep[0], pDstStep[2], pDstStep[1], pDstStep[3]};
    IppStatus status;

    switch (m_srcFormat)
    {
    case GRAY:
        switch (m_dstFormat)
        {
        case YUV420:
            status = cppiGrayToYCbCr420_8u_P1P3R(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = cppiGrayToYCbCr422_8u_P1P3R(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case YUV444:
            status = cppiGrayToYCbCr444_8u_P1P3R(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case RGB:
        case BGR:
            status = cppiGrayToBGR_8u_P1C3R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case NV12:
        switch (m_dstFormat)
        {
        case YV12:
            status = ippiYCbCr420ToYCrCb420_8u_P2P3R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst, pDstStep, m_size);
            break;
        case YUV411:
            status = ippiYCbCr420ToYCbCr411_8u_P2P3R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst, pDstStep, m_size);
            break;
        case YUV420:
            status = ippiYCbCr420_8u_P2P3R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = ippiYCbCr420ToYCbCr422_8u_P2P3R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst, pDstStep, m_size);
            break;
        case YUY2:
            status = ippiYCbCr420ToYCbCr422_8u_P2C2R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst[0], pDstStep[0], m_size);
            break;
        case UYVY:
            status = ippiYCbCr420ToCbYCr422_8u_P2C2R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], pDst[0], pDstStep[0], m_size);
            break;
        case BGR: // temporary for MediaSDK
            {
                if(!cvt->GetPlaneDataPtr(0))
                {
                    cvt->Init(m_size.width, m_size.height, YUV420);
                    cvt->Alloc();
                }
                const Ipp8u *(pCvt[3]) = {(Ipp8u*)cvt->GetPlaneDataPtr(0), (Ipp8u*)cvt->GetPlaneDataPtr(1), (Ipp8u*)cvt->GetPlaneDataPtr(2)};
                Ipp32s pCvtStep[3]     = {(Ipp32s)cvt->GetPlanePitch(0), (Ipp32s)cvt->GetPlanePitch(1), (Ipp32s)cvt->GetPlanePitch(2)};

                status = ippiYCbCr420_8u_P2P3R(pSrc[0], pSrcStep[0], pSrc[1], pSrcStep[1], (Ipp8u**)pCvt, pCvtStep, m_size);
                status = ippiYCbCr420ToBGR_8u_P3C3R(pCvt, pCvtStep, pDst[0], pDstStep[0], m_size);
            }
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case YUV411:
        switch (m_dstFormat)
        {
        case NV12:
            status = ippiYCbCr411ToYCbCr420_8u_P3P2R(pSrc, pSrcStep, pDst[0], pDstStep[0], pDst[1], pDstStep[1], m_size);
            break;
        case YUV420:
            status = ippiYCbCr411ToYCbCr420_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = ippiYCbCr411ToYCbCr422_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUY2:
            status = ippiYCbCr411ToYCbCr422_8u_P3C2R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case UYVY:
            status = ippiYCbCr411ToYCbCr422_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case BGR:
            status = ippiYCbCr411ToBGR_8u_P3C3R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case BGRA:
            status = ippiYCbCr411ToBGR_8u_P3C4R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size, 0);
            break;
        case BGR555:
            status = ippiYCbCr411ToBGR555_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR565:
            status = ippiYCbCr411ToBGR565_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case YUV420:
    case YUV420A:
        switch (m_dstFormat)
        {
        case GRAY:
            status = cppiYCbCrToGray_8u_P3P1R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case YUV411:
            status = ippiYCbCr420To411_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = ippiYCbCr420ToYCbCr422_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUV444:
            status = cppiYCbCr420ToYCbCr444_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case Y41P:
            status = cppiYCbCr420To41P_8u_P3P1(pYVU, pYVUStep, pDst[0], pDstStep[0], m_size);
            break;
        case NV12:
            status = ippiYCbCr420_8u_P3P2R(pSrc, pSrcStep, pDst[0], pDstStep[0], pDst[1], pDstStep[1], m_size);
            break;
        case YUY2:
            status = ippiYCrCb420ToYCbCr422_8u_P3C2R(pYVU, pYVUStep, pDst[0], pDstStep[0], m_size);
            break;
        case UYVY:
            status = ippiYCrCb420ToCbYCr422_8u_P3C2R(pYVU, pYVUStep, pDst[0], pDstStep[0], m_size);
            break;
        case RGB:
            status = ippiYCbCr420ToRGB_8u_P3C3R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case RGBA:
            status = ippiYCbCr420ToBGR_8u_P3C4R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size, 0);
            cppiSwapChannes_C4_I(pDst[0], pDstStep[0], m_size);
            break;
        case RGB565:
            status = ippiYCbCr420ToRGB565_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case RGB555:
            status = ippiYCbCr420ToRGB555_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case RGB444:
            status = ippiYCbCr420ToRGB444_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = ippiYCbCr420ToBGR_8u_P3C3R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case BGRA:
            status = ippiYCbCr420ToBGR_8u_P3C4R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size, 0);
            break;
        case BGR565:
            status = ippiYCbCr420ToBGR565_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR555:
            status = ippiYCbCr420ToBGR555_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR444:
            status = ippiYCbCr420ToBGR444_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case YUV422:
    case YUV422A:
        switch (m_dstFormat)
        {
        case GRAY:
            status = cppiYCbCrToGray_8u_P3P1R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case YUV420:
            status = ippiYCbCr422ToYCbCr420_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUV444:
            status = cppiYCbCr422ToYCbCr444_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUY2:
            status = ippiYCbCr422_8u_P3C2R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case UYVY:
            status = ippiYCbCr422ToCbYCr422_8u_P3C2R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case NV12:
            status = ippiYCbCr422ToYCbCr420_8u_P3P2R(pSrc, pSrcStep, pDst[0], pDstStep[0], pDst[1], pDstStep[1], m_size);
            break;
        case RGB:
            status = ippiYCbCr422ToRGB_8u_P3C3R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case RGB565:
            status = ippiYCbCr422ToRGB565_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case RGB555:
            status = ippiYCbCr422ToRGB555_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case RGB444:
            status = ippiYCbCr422ToRGB444_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = ippiYCbCr422ToBGR_8u_P3C3R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case BGR565:
            status = ippiYCbCr422ToBGR565_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR555:
            status = ippiYCbCr422ToBGR555_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR444:
            status = ippiYCbCr422ToBGR444_8u16u_P3C3R(pSrc, pSrcStep, (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case YUV444:
    case YUV444A:
        switch (m_dstFormat)
        {
        case GRAY:
            status = cppiYCbCrToGray_8u_P3P1R(pSrc, pSrcStep, pDst[0], pDstStep[0], m_size);
            break;
        case YUV420:
            status = cppiYCbCr444ToYCbCr420_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = cppiYCbCr444ToYCbCr422_8u_P3R(pSrc, pSrcStep, pDst, pDstStep, m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case YUY2:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiYCbCr422ToYCbCr420_8u_C2P3R(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case YUV422:
            status = ippiYCbCr422_8u_C2P3R( pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case NV12:
            status = ippiYCbCr422ToYCbCr420_8u_C2P2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], pDst[1], pDstStep[1], m_size);
            break;
        case YV12:
            status = ippiYCbCr422ToYCrCb420_8u_C2P3R(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case UYVY:
            status = ippiYCbCr422ToCbYCr422_8u_C2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = ippiYCbCr422ToBGR_8u_C2C3R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGRA:
            status = ippiYCbCr422ToBGR_8u_C2C4R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size, 0);
            break;
        case BGR565:
            status = ippiYCbCr422ToBGR565_8u16u_C2C3R(pSrc[0], pSrcStep[0], (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR555:
            status = ippiYCbCr422ToBGR555_8u16u_C2C3R(pSrc[0], pSrcStep[0], (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case BGR444:
            status = ippiYCbCr422ToBGR444_8u16u_C2C3R(pSrc[0], pSrcStep[0], (Ipp16u*)pDst[0], pDstStep[0], m_size);
            break;
        case RGB:
            status = ippiYCbCr422ToBGR_8u_C2C3R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;

        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case UYVY:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiCbYCr422ToYCrCb420_8u_C2P3R(pSrc[0], pSrcStep[0], pDstYVU, pDstStepYVU, m_size);
            break;
        case YUV422:
            status = ippiCbYCr422ToYCbCr422_8u_C2P3R( pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        case NV12:
            status = ippiCbYCr422ToYCbCr420_8u_C2P2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], pDst[1], pDstStep[1], m_size);
            break;
        case YUY2:
            status = ippiCbYCr422ToYCbCr422_8u_C2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGRA:
            status = ippiCbYCr422ToBGR_8u_C2C4R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size, 0);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case RGB:
        if(m_dstFormat == BGR)
        {
            status = ippiSwapChannels_8u_C3R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size, iOrder);
            break;
        }
        cppiSwapChannes_C3_I((Ipp8u*)pSrc[0], pSrcStep[0], m_size);
    case BGR:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiBGRToYCrCb420_8u_C3P3R(pSrc[0], pSrcStep[0], pDstYVU, pDstStepYVU, m_size);
            break;
        case YUY2:
            status = ippiBGRToYCbCr422_8u_C3C2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGRA:
            status = cppiBGR_8u_C3AC4(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case RGBA:
        if(m_dstFormat == BGRA)
        {
            status = ippiSwapChannels_8u_AC4R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size, iOrder);
            break;
        }
        cppiSwapChannes_C4_I((Ipp8u*)pSrc[0], pSrcStep[0], m_size);
    case BGRA:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiBGRToYCrCb420_8u_AC4P3R(pSrc[0], pSrcStep[0], pDstYVU, pDstStepYVU, m_size);
            break;
        case YUY2:
            status = ippiBGRToYCbCr422_8u_AC4C2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case UYVY:
            status = ippiBGRToCbYCr422_8u_AC4C2R(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = cppiBGR_8u_AC4C3(pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case BGR555:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiBGR555ToYCrCb420_16u8u_C3P3R((const Ipp16u*)pSrc[0], pSrcStep[0], pDstYVU, pDstStepYVU, m_size);
            break;
        case YUY2:
            status = ippiBGR555ToYCbCr422_16u8u_C3C2R((const Ipp16u*)pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = cppiBGR555ToBGR_16u8u_C3((const Ipp16u*)pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case BGR565:
        switch (m_dstFormat)
        {
        case YUV420:
            status = ippiBGR565ToYCrCb420_16u8u_C3P3R((const Ipp16u*)pSrc[0], pSrcStep[0], pDstYVU, pDstStepYVU, m_size);
            break;
        case YUY2:
            status = ippiBGR565ToYCbCr422_16u8u_C3C2R((const Ipp16u*)pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        case BGR:
            status = cppiBGR565ToBGR_16u8u_C3((const Ipp16u*)pSrc[0], pSrcStep[0], pDst[0], pDstStep[0], m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    case Y41P:
        switch (m_dstFormat)
        {
        case YUV420:
            status = cppiYCbCr41PTo420_8u_P1P3(pSrc[0], pSrcStep[0], pDst, pDstStep, m_size);
            break;
        default:
            return UMC_ERR_NOT_IMPLEMENTED;
        }
        break;
    default:
        return UMC_ERR_NOT_IMPLEMENTED;
    }

    return (status >= ippStsNoErr) ? UMC_OK : UMC_ERR_INVALID_PARAMS;
}

////////////////////////////////////////////////////////////////////////

namespace UMC
{

Status FillBlockWithColor(VideoData *pData, int y, int u, int v)
{
    IppiSize ySize = {16, 16};
    IppiSize lSize = {8, 8};
    Ipp8u yuyv[4] = {(Ipp8u)y, (Ipp8u)u, (Ipp8u)y, (Ipp8u)v};
    Ipp16s uv = (Ipp16s)(u | (v << 8));

    switch (pData->m_colorFormat)
    {
    case YV12:
    case YUV420:
        ippiSet_8u_C1R(yuyv[0], (Ipp8u*)pData->GetPlaneDataPtr(0), (int)pData->GetPlanePitch(0), ySize);
        ippiSet_8u_C1R(yuyv[1], (Ipp8u*)pData->GetPlaneDataPtr(1), (int)pData->GetPlanePitch(1), lSize);
        ippiSet_8u_C1R(yuyv[3], (Ipp8u*)pData->GetPlaneDataPtr(2), (int)pData->GetPlanePitch(2), lSize);
        return UMC_OK;
    case YUY2:
    case UYVY:
        ippiSet_8u_C4R(yuyv, (Ipp8u*)pData->GetPlaneDataPtr(0), (int)pData->GetPlanePitch(0), ySize);
        return UMC_OK;
    case NV12:
        ippiSet_8u_C1R(yuyv[0], (Ipp8u*)pData->GetPlaneDataPtr(0), (int)pData->GetPlanePitch(0), ySize);
        ippiSet_16s_C1R(uv, (Ipp16s*)pData->GetPlaneDataPtr(1), (int)pData->GetPlanePitch(1), lSize);
        return UMC_OK;
    default:
        return UMC_ERR_NOT_IMPLEMENTED;
    }
}

} // namespace UMC
