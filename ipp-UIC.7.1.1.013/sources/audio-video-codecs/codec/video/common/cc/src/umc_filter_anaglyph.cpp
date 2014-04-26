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

#include "umc_filter_anaglyph.h"
#include "umc_video_data.h"

#include "ipps.h"

using namespace UMC;


static bool InvalidData(VideoData *pData1, VideoData *pData2)
{
    if(pData1->m_iWidth != pData2->m_iWidth || pData1->m_iHeight != pData2->m_iHeight || pData1->m_colorFormat != pData2->m_colorFormat)
        return true;
    return false;
}

VideoAnaglyph::VideoAnaglyph()
{
    m_pInterData        = NULL;
    m_pInDataPair       = NULL;
    m_bPairReady        = false;
    m_bUseIntermediate  = false;
    m_bStateInitialized = false;
}

VideoAnaglyph::~VideoAnaglyph()
{
    Close();
}

Status VideoAnaglyph::Init(BaseCodecParams *pParams)
{
    VideoAnaglyphParams *params = DynamicCast<VideoAnaglyphParams, BaseCodecParams>(pParams);

    m_params = *params;

    m_pInterData  = new VideoData();
    m_pInDataPair = new VideoData();

    return UMC_OK;
}

Status VideoAnaglyph::Close()
{
    UMC_DELETE(m_pInterData);
    UMC_DELETE(m_pInDataPair);

    return UMC_OK;
}

Status VideoAnaglyph::GetFrame(MediaData *input, MediaData *output)
{
    VideoData *in    = DynamicCast<VideoData, MediaData>(input);
    VideoData *out   = DynamicCast<VideoData, MediaData>(output);
    VideoData *inter = DynamicCast<VideoData, MediaData>(m_pInterData);
    VideoData *pair  = DynamicCast<VideoData, MediaData>(m_pInDataPair);
    Status     status;

    UMC_CHECK(in,    UMC_ERR_NULL_PTR);
    UMC_CHECK(out,   UMC_ERR_NULL_PTR);
    UMC_CHECK(inter, UMC_ERR_NOT_INITIALIZED);
    UMC_CHECK(pair,  UMC_ERR_NOT_INITIALIZED);

    // nothing to do
    if(in == out)
        return UMC_OK;
    if(!m_params.m_bEnabled || (in->m_colorFormat != RGB && in->m_colorFormat != BGR))
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
            inter->Alloc();
        }

        pair->Close();
        pair->Init(in);
        pair->Alloc();

        m_bPairReady        = false;
        m_bStateInitialized = true;
    }

    if(m_bUseIntermediate)
        *out = *inter;

    // anaglyph builder
    if(m_bPairReady)
    {
        if(in->m_colorFormat == BGR)
        {
            Ipp8u *pInPix     = (Ipp8u*)in->GetDataPointer();
            Ipp8u *pInPixPair = (Ipp8u*)pair->GetDataPointer() + 2;
            Ipp8u *pOutPix    = (Ipp8u*)out->GetDataPointer();
            Ipp32u i;

            for(i = 0; i < in->GetBufferSize(); i+=3)
            {
                pOutPix[i]     = pInPix[i];
                pOutPix[i + 1] = pInPix[i + 1];
                pOutPix[i + 2] = pInPixPair[i];
            }
        }
        else if(in->m_colorFormat == RGB)
        {
            Ipp8u *pInPix     = (Ipp8u*)in->GetDataPointer();
            Ipp8u *pInPixPair = (Ipp8u*)pair->GetDataPointer();
            Ipp8u *pOutPix    = (Ipp8u*)out->GetDataPointer();
            Ipp32u i;

            for(i = 0; i < in->GetBufferSize(); i+=3)
            {
                pOutPix[i]     = pInPixPair[i];
                pOutPix[i + 1] = pInPix[i + 1];
                pOutPix[i + 2] = pInPix[i + 2];
            }
        }
        else
            return UMC_WRN_SKIP;

        m_bPairReady = false;
    }
    else
    {   // Collect frame and go to next
        status = in->Copy(pair);
        if(status != UMC_OK)
            return status;

        m_bPairReady = true;
        return UMC_ERR_NOT_ENOUGH_DATA;
    }

    return UMC_OK;
}
