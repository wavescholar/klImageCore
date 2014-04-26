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

#include "umc_filter_deinterlace.h"
#include "umc_video_data.h"
#include "ippvc.h"
#include "ippi.h"
#include "ipps.h"

using namespace UMC;


static bool InvalidData(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_iWidth != pData2->m_iWidth || pData1->m_iHeight != pData2->m_iHeight ||
        pData1->m_colorFormat != pData2->m_colorFormat || pData1->GetPlanesNumber() != pData2->GetPlanesNumber())
        return true;
    return false;
}

static bool InvalidData2(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_iWidth != pData2->m_iWidth || pData1->m_iHeight*2 != pData2->m_iHeight ||
        pData1->m_colorFormat != pData2->m_colorFormat || pData1->GetPlanesNumber() != pData2->GetPlanesNumber())
        return true;
    return false;
}

VideoDeinterlace::VideoDeinterlace()
{
    m_prevStructure     = PS_PROGRESSIVE;
    m_pairStructure     = PS_PROGRESSIVE;
    m_bUseIntermediate  = false;
    m_bStateInitialized = false;
    m_bHaveField        = false;
    m_pInterData        = NULL;
    m_pInDataPair       = NULL;
}

VideoDeinterlace::~VideoDeinterlace()
{
    Close();
}

Status VideoDeinterlace::Init(BaseCodecParams *pParams)
{
    VideoDeinterlaceParams *params = DynamicCast<VideoDeinterlaceParams, BaseCodecParams>(pParams);
    Ipp16u iThreshold = 0;

    if (NULL == params)
        return UMC_ERR_NULL_PTR;

    m_params = *params;

    if(m_params.m_deinterlacingType == DEINTERLACING_MEDIAN_THRESHOLD)
        iThreshold = 6;
    if(m_params.m_deinterlacingType == DEINTERLACING_CAVT)
        iThreshold = 17;

    if(!m_params.m_iThreshold)
        m_params.m_iThreshold = iThreshold;

    m_pInterData  = new VideoData();
    m_pInDataPair = new VideoData();

    return UMC_OK;
}

Status VideoDeinterlace::Close()
{
    UMC_DELETE(m_pInterData);
    UMC_DELETE(m_pInDataPair);

    return UMC_OK; 
}

Status VideoDeinterlace::CombineFields(MediaData *input, MediaData *output)
{
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);

    UMC_CHECK(in,    UMC_ERR_NULL_PTR);
    UMC_CHECK(out,   UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);

    // nothing to do
    if(in == out)
        return UMC_OK;
    if(in->m_picStructure != PS_TOP_FIELD && in->m_picStructure != PS_BOTTOM_FIELD)
    {
        if(!out->GetPlaneDataPtr(0))
            return UMC_WRN_SKIP;
        else
            in->Copy(out);
        return UMC_OK;
    }

    if(!out->GetDataPointer())
    {
        m_bStateInitialized = !InvalidData2(in, inter);
        m_bUseIntermediate = true;
    }
    else
    {
        m_bUseIntermediate = false;
        if(InvalidData(in, out))
            return UMC_ERR_INVALID_PARAMS;
    }

    if(!m_bStateInitialized)
    {
        if(m_bUseIntermediate)
        {
            inter->Close();
            inter->Init(in);
            inter->m_iHeight      = in->m_iHeight*2;
            inter->m_picStructure = out->m_picStructure;
            inter->Alloc();
        }
        m_bHaveField = false;

        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    // first field
    if(!m_bHaveField)
    {
        m_prevStructure = in->m_picStructure;
        m_pairStructure = (in->m_picStructure == PS_TOP_FIELD)?PS_TOP_FIELD_FIRST:PS_BOTTOM_FIELD_FIRST;
    }
    else
    {
        if(m_params.m_fieldsMode == FIELDS_COMBINE_SOFT)
        {
            if(m_prevStructure == PS_TOP_FIELD)
                in->m_picStructure = PS_BOTTOM_FIELD;
            else if(m_prevStructure == PS_BOTTOM_FIELD)
                in->m_picStructure = PS_TOP_FIELD;
        }
    }

    IppStatus ippStatus;
    VideoData::PlaneData *pSrcPlane;
    Ipp8u   *pSrc, *pDst;
    Ipp32s   iSrcStep, iDstStep;
    IppiSize size;

    for(Ipp32u i = 0; i < in->GetPlanesNumber(); i++)
    {
        pSrcPlane = in->GetPtrToPlane(i);
        pSrc      = (Ipp8u*)in->GetPlaneDataPtr(i);
        iSrcStep  = (Ipp32s)in->GetPlanePitch(i);
        pDst      = (Ipp8u*)out->GetPlaneDataPtr(i);
        iDstStep  = (Ipp32s)out->GetPlanePitch(i);

        size.width  = pSrcPlane->m_size.width * pSrcPlane->m_iSamples * pSrcPlane->m_iSampleSize;
        size.height = pSrcPlane->m_size.height;

        if(m_params.m_fieldsMode == FIELDS_EXPAND)
        {
            ippStatus = ippiCopy_8u_C1R(pSrc, iSrcStep, pDst, iDstStep*2, size);
            if(ippStatus != ippStsNoErr)
                return UMC_ERR_FAILED;

            ippStatus = ippiCopy_8u_C1R(pSrc, iSrcStep, pDst + iDstStep, iDstStep*2, size);
            if(ippStatus != ippStsNoErr)
                return UMC_ERR_FAILED;
        }
        else
        {
            if(in->m_picStructure == PS_BOTTOM_FIELD)
                pDst += iDstStep;

            ippStatus = ippiCopy_8u_C1R(pSrc, iSrcStep, pDst, iDstStep*2, size);
            if(ippStatus != ippStsNoErr)
                return UMC_ERR_FAILED;
        }
    }

    if(m_params.m_fieldsMode == FIELDS_COMBINE_SOFT || m_params.m_fieldsMode == FIELDS_COMBINE_STRICT)
    {
        if(m_bHaveField)
        {
            // don't switch fields on duplicate field
            if(m_prevStructure != in->m_picStructure)
                m_bHaveField = false;
        }
        else
            m_bHaveField = true;

        if(m_bHaveField)
            return UMC_ERR_NOT_ENOUGH_DATA;

            out->m_picStructure = m_pairStructure;
    }
    else
        out->m_picStructure = in->m_picStructure;

    return UMC_OK;
}

Status VideoDeinterlace::GetFrame(MediaData *input, MediaData *output)
{
    Status     status;
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);
    VideoData *pair  = DynamicCast<VideoData, MediaData>(m_pInDataPair);

    UMC_CHECK(in,    UMC_ERR_NULL_PTR);
    UMC_CHECK(out,   UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);
    UMC_CHECK(pair,  UMC_ERR_NOT_INITIALIZED);

    // if fields are separate try to combine them
    if(m_params.m_fieldsMode != FIELDS_LEAVE)
    {
        *pair = *out;
        status = CombineFields(in, pair);
        UMC_CHECK(status == UMC_OK || status == UMC_WRN_SKIP, status);
        if(status != UMC_WRN_SKIP)
        {
            if(!m_params.m_bEnabled && m_bUseIntermediate)
            {
                *out = *inter;
                return UMC_OK;
            }
            in = pair;
        }
    }

    // nothing to do
    if(in == out)
        return UMC_OK;
    if(!m_params.m_bEnabled || in->m_picStructure == out->m_picStructure)
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

    if(!m_bStateInitialized)
    {
        if(m_bUseIntermediate)
        {
            inter->Close();
            inter->Init(in);
            inter->m_picStructure = out->m_picStructure;
            inter->Alloc();
        }

        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    IppStatus ippStatus;
    VideoData::PlaneData *pSrcPlane;
    Ipp8u   *pSrc, *pDst;
    Ipp32s   iSrcStep, iDstStep;
    IppiSize size;
    Ipp32u   iTFF = (in->m_picStructure & PS_TOP_FIELD_FIRST) ? 1 : 0;
    Ipp32u   i;

    for(i = 0; i < in->GetPlanesNumber(); i++)
    {
        pSrcPlane = in->GetPtrToPlane(i);
        pSrc     = (Ipp8u*)in->GetPlaneDataPtr(i);
        iSrcStep = (Ipp32s)in->GetPlanePitch(i);
        pDst     = (Ipp8u*)out->GetPlaneDataPtr(i);
        iDstStep = (Ipp32s)out->GetPlanePitch(i);

        size.width  = pSrcPlane->m_size.width * pSrcPlane->m_iSamples * pSrcPlane->m_iSampleSize;
        size.height = pSrcPlane->m_size.height;

        switch(m_params.m_deinterlacingType)
        {
            case DEINTERLACING_BLEND:
                ippStatus = ippiDeinterlaceFilterTriangle_8u_C1R(pSrc, iSrcStep, pDst, iDstStep, size, 128, IPP_LOWER | IPP_UPPER | IPP_CENTER);
                if(ippStatus != ippStsNoErr)
                    return UMC_ERR_FAILED;
                break;

            case DEINTERLACING_EDGE_DETECT:
                ippStatus = ippiDeinterlaceEdgeDetect_8u_C1R(pSrc, iSrcStep, pDst, iDstStep, size, iTFF, 1);
                if(ippStatus != ippStsNoErr)
                    return UMC_ERR_FAILED;
                break;

            case DEINTERLACING_MEDIAN_THRESHOLD:
                ippStatus = ippiDeinterlaceMedianThreshold_8u_C1R(pSrc, iSrcStep, pDst, iDstStep, size, m_params.m_iThreshold, iTFF, 1);
                if(ippStatus != ippStsNoErr)
                    return UMC_ERR_FAILED;
                break;

            case DEINTERLACING_CAVT:
                {
                    Ipp8u *pS, *pD;
                    Ipp32s srcStep, dstStep;

                    if(iTFF)
                    {
                        pS = pSrc;
                        pD = pDst;
                        srcStep = iSrcStep;
                        dstStep = iDstStep;
                    }
                    else
                    {
                        pS = pSrc + (size.height - 1)*iSrcStep;
                        pD = pDst + (size.height - 1)*iDstStep;
                        srcStep = -iSrcStep;
                        dstStep = -iDstStep;
                    }
                    ippStatus = ippiDeinterlaceFilterCAVT_8u_C1R(pS, srcStep, pD, dstStep, m_params.m_iThreshold, size);
                    if(ippStatus != ippStsNoErr)
                        return UMC_ERR_FAILED;
                }
                break;

            case DEINTERLACING_MEDIAN:
                {
                    IppiSize roi = {size.width, size.height >> 1};
                    const Ipp8u *pS[3];
                    Ipp8u *pD;

                    if (iTFF == 1)
                    {
                        ippiCopy_8u_C1R(pSrc, 2*iSrcStep, pDst, 2*iDstStep, roi);
                        ippsCopy_8u(pSrc + (size.height - 1) * iSrcStep, pDst + (size.height - 1) * iDstStep, size.width);
                        pS[0] = pSrc;
                        pS[1] = pSrc + iSrcStep;
                        pS[2] = pSrc + 2*iSrcStep;
                        pD    = pDst + iDstStep;
                    }
                    else
                    {
                        ippsCopy_8u(pSrc, pDst, size.width);
                        ippiCopy_8u_C1R(pSrc + iSrcStep, 2 * iSrcStep, pDst + iDstStep, 2 * iDstStep, roi);
                        pS[0] = pSrc + iSrcStep;
                        pS[1] = pSrc + 2*iSrcStep;
                        pS[2] = pSrc + 3*iSrcStep;
                        pD    = pDst + 2*iDstStep;
                    }
                    roi.height--;
                    ippStatus = ippiMedian_8u_P3C1R(pS, 2*iSrcStep, pD, 2*iDstStep, roi);
                    if(ippStatus != ippStsNoErr)
                        return UMC_ERR_FAILED;
                }
                break;

            case DEINTERLACING_DUPLICATE:
                {
                    pSrc  = pSrc + (iTFF?0:iSrcStep);
                    Ipp8u *pDst2 = pDst + iDstStep;
                    iSrcStep *= 2;
                    iDstStep *= 2;
                    size.height /= 2;
                    ippStatus = ippiCopy_8u_C1R(pSrc, iSrcStep, pDst,  iDstStep, size);
                    if(ippStatus != ippStsNoErr)
                        return UMC_ERR_FAILED;
                    ippStatus = ippiCopy_8u_C1R(pSrc, iSrcStep, pDst2, iDstStep, size);
                    if(ippStatus != ippStsNoErr)
                        return UMC_ERR_FAILED;
                }
                break;

            default:
                return UMC_WRN_SKIP;
        }
    }

    return UMC_OK;
}
