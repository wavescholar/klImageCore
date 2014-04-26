/* ///////////////////////////////////////////////////////////////////////////// */
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
#ifdef UMC_ENABLE_MJPEG_VIDEO_DECODER

#include <string.h>
#include "vm_debug.h"
#include "umc_video_data.h"
#include "umc_video_processing.h"
#include "umc_mjpeg_video_decoder.h"
#include "membuffin.h"
#include "jpegdec.h"

// VD: max num of padding bytes at the end of AVI frame
//     should be 1 by definition (padding up to Ipp16s boundary),
//     but in reality can be bigger
#define AVI_MAX_PADDING_BYTES 4

using namespace UMC;


struct JPEGInternalParams
{
    Ipp32s   iChannels;
    Ipp32s   iBitDepth;
    IppiSize size;
    JSS      sampling;
    JCOLOR   color;
    JDD      dctScaleFactor;
    JTMODE   threadingMode;
    int      iDctType;

    JPEGInternalParams()
    {
        size.width     = 0;
        size.height    = 0;
        iBitDepth      = 0;
        iChannels      = 0;
        iDctType       = 1;
        dctScaleFactor = JD_1_1;
        sampling       = JS_444;
        color          = JC_UNKNOWN;
        threadingMode  = JT_OLD;
    }
};

static bool invalidateFrame(JPEGInternalParams *pParams1, JPEGInternalParams *pParams2)
{
    if(pParams1->size.width != pParams2->size.width || pParams1->size.height != pParams2->size.height ||
        pParams1->iChannels != pParams2->iChannels || pParams1->color != pParams2->color ||
        pParams1->sampling != pParams2->sampling || pParams1->iBitDepth != pParams2->iBitDepth)
        return true;
    return false;
}

MJPEGVideoDecoder::MJPEGVideoDecoder(void)
{
    m_pDecoder      = NULL;
    m_pIntParams    = NULL;
    m_bField        = false;
    m_bFirstField   = false;
    m_frameMID      = 0;
}

MJPEGVideoDecoder::~MJPEGVideoDecoder(void)
{
    Close();
}

Status MJPEGVideoDecoder::Init(BaseCodecParams *pInit)
{
    VideoDecoderParams* pParams = DynamicCast<VideoDecoderParams, BaseCodecParams>(pInit);
    Status status;

    if(!pParams)
        return UMC_ERR_NULL_PTR;
    if(!pParams->m_pPostProcessor)
        return UMC_ERR_NULL_PTR;

    UMC_CHECK_FUNC(status, Close());
    UMC_CHECK_FUNC(status, VideoDecoder::Init(pInit));

    m_params  = *pParams;
    m_pPostProcessor = m_params.m_pPostProcessor;

    status = m_lastDecodedFrame.ReleaseImage();
    if(UMC_OK != status)
        return status;

    m_bField        = false;
    m_bFirstField   = true;

    if(!m_pIntParams)
    {
        m_pIntParams = new JPEGInternalParams;
        if(!m_pIntParams)
            return UMC_ERR_ALLOC;
    }

    if(!m_pDecoder)
    {
        m_pDecoder = new CJPEGDecoder;
        if(!m_pDecoder)
            return UMC_ERR_ALLOC;
    }

#ifdef _OPENMP
    if(pParams->m_iThreads == 0)
        pParams->m_iThreads = omp_get_max_threads();
#else
    pParams->m_iThreads = 1;
#endif

    set_num_threads(pParams->m_iThreads);

    return UMC_OK;
}

Status MJPEGVideoDecoder::Reset(void)
{
    if(m_frameMID)
    {
        m_pMemoryAllocator->Unlock(m_frameMID);
        m_pMemoryAllocator->Free(m_frameMID);
        m_frameMID = 0;
    }

    m_bField        = false;
    m_bFirstField   = true;

    m_pDecoder->Reset();

    return UMC_OK;
}

Status MJPEGVideoDecoder::Close(void)
{
    if(m_frameMID)
    {
        m_pMemoryAllocator->Unlock(m_frameMID);
        m_pMemoryAllocator->Free(m_frameMID);
        m_frameMID = 0;
    }

    m_bField        = false;
    m_bFirstField   = true;

    UMC_DELETE(m_pIntParams);
    UMC_DELETE(m_pDecoder);

    return VideoDecoder::Close();
}

Status MJPEGVideoDecoder::GetInfo(BaseCodecParams* pInfo)
{
    VideoDecoderParams *pParams = DynamicCast<VideoDecoderParams, BaseCodecParams>(pInfo);

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    pParams->m_info = m_params.m_info;

    return UMC_OK;
}

Status MJPEGVideoDecoder::GetFrame(MediaData *pDataIn, MediaData *pDataOut)
{
    CMemBuffInput      memReader;
    JPEGInternalParams params;
    Status   status;
    JERRCODE jStatus;

    if(!m_pDecoder)
        return UMC_ERR_NOT_INITIALIZED;

    if(!pDataOut)
        return UMC_ERR_NULL_PTR;

    if(!pDataIn)
        return UMC_ERR_NOT_ENOUGH_DATA;

    memReader.Open((Ipp8u*)pDataIn->GetDataPointer(), (Ipp32s)pDataIn->GetDataSize());

    jStatus = m_pDecoder->SetSource(&memReader);
    if(JPEG_OK != jStatus)
        return UMC_ERR_FAILED;

    jStatus = m_pDecoder->ReadHeader(&params.size.width, &params.size.height, &params.iChannels, &params.color, &params.sampling, &params.iBitDepth);
    if(JPEG_ERR_BUFF == jStatus)
        return UMC_ERR_NOT_ENOUGH_DATA;
    else if(JPEG_OK != jStatus)
        return UMC_ERR_FAILED;

    m_pDecoder->SetThreadingMode(JT_RSTI);

    // override unsupported source formats
    if(params.sampling == JS_244 || params.sampling == JS_OTHER)
        params.sampling = JS_444;

    // invalidate frame data
    if(invalidateFrame(&params, m_pIntParams))
    {
        // treat frame as interlaced if clip height is twice more than JPEG frame height
        if(params.size.height == (Ipp32s)((m_params.m_info.videoInfo.m_iHeight >> 1)))
            m_bField = true;

        m_pIntParams->size      = params.size;
        m_pIntParams->color     = params.color;
        m_pIntParams->sampling  = params.sampling;
        m_pIntParams->iChannels = params.iChannels;
        m_pIntParams->iBitDepth = params.iBitDepth;

        ColorFormat colorFormat;
        Ipp32u iMemAlloc = 0;

        // use planar output for YUV
        if(params.color == JC_YCBCR || params.color == JC_YCCK)
        {
            m_iDstStep[0] = params.size.width*((m_pIntParams->iBitDepth + 7)/8);
            iMemAlloc = m_iDstStep[0]*params.size.height;

            if(params.sampling == JS_444)
            {
                m_iDstStep[1] = m_iDstStep[2] = m_iDstStep[0];
                iMemAlloc += ((m_iDstStep[1] + m_iDstStep[2])*params.size.height);
            }
            else if(params.sampling == JS_422 || params.sampling == JS_411)
            {
                m_iDstStep[1] = m_iDstStep[2] = (m_iDstStep[0] + 1)/2;
                iMemAlloc += ((m_iDstStep[1] + m_iDstStep[2])*params.size.height);
            }

            if(params.color == JC_YCCK)
            {
                m_iDstStep[3] = m_iDstStep[0];
                iMemAlloc += (m_iDstStep[3]*params.size.height);
            }
        }
        else
        {
            m_iDstStep[0] = params.size.width*params.iChannels*((m_pIntParams->iBitDepth + 7)/8);
            iMemAlloc = m_iDstStep[0]*params.size.height;
        }

        if(!m_pMemoryAllocator)
            return UMC_ERR_ALLOC;

        status = m_pMemoryAllocator->Alloc(&m_frameMID, iMemAlloc, UMC_ALLOC_PERSISTENT, 16);
        if(UMC_OK != status)
            return UMC_ERR_ALLOC;

        m_pDst[0] = (Ipp8u*)m_pMemoryAllocator->Lock(m_frameMID);
        if(!m_pDst[0])
            return UMC_ERR_ALLOC;

        if(params.color == JC_YCBCR || params.color == JC_YCCK)
        {
            for(Ipp32s i = 1; i < params.iChannels; i++)
                m_pDst[i] = m_pDst[i-1] + m_iDstStep[i-1]*params.size.height;

            if(params.iBitDepth <= 8)
                m_pDecoder->SetDestination(m_pDst, m_iDstStep, params.size, params.iChannels, params.color, params.sampling, 8);
            else
                m_pDecoder->SetDestination((Ipp16s**)m_pDst, m_iDstStep, params.size, params.iChannels, params.color, params.sampling, 16);
        }
        else
        {
            if(params.iBitDepth <= 8)
                m_pDecoder->SetDestination(m_pDst[0], m_iDstStep[0], params.size, params.iChannels, params.color, JS_444, 8);
            else
                m_pDecoder->SetDestination((Ipp16s*)m_pDst[0], m_iDstStep[0], params.size, params.iChannels, params.color, JS_444, 16);
        }

        switch(params.color)
        {
        case JC_GRAY:  colorFormat = GRAY; break;
        case JC_RGB:   colorFormat = RGB; break;
        case JC_BGR:   colorFormat = BGR; break;
        case JC_CMYK:  colorFormat = CMYK; break;
        case JC_BGRA:  colorFormat = BGRA; break;
        case JC_RGBA:  colorFormat = RGBA; break;
        case JC_YCBCR:
            if(params.sampling == JS_444)
                colorFormat = YUV444;
            else if(params.sampling == JS_422)
                colorFormat = YUV422;
            else if(params.sampling == JS_411)
                colorFormat = YUV420;
            else
                return UMC_ERR_UNSUPPORTED;
            break;
        case JC_YCCK:
            if(params.sampling == JS_444)
                colorFormat = YUV444A;
            else if(params.sampling == JS_422)
                colorFormat = YUV422A;
            else if(params.sampling == JS_411)
                colorFormat = YUV420A;
            else
                return UMC_ERR_UNSUPPORTED;
            break;
        default:
            return UMC_ERR_UNSUPPORTED;
        }

        m_lastDecodedFrame.Init(params.size.width, params.size.height, colorFormat, params.iBitDepth);
        if(params.color == JC_YCBCR)
        {
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[0], 0);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[0], 0);
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[1], 1);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[1], 1);
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[2], 2);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[2], 2);
        }
        else if(params.color == JC_YCCK)
        {
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[0], 0);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[0], 0);
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[1], 1);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[1], 1);
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[2], 2);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[2], 2);
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[3], 3);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[3], 3);
        }
        else
        {
            m_lastDecodedFrame.SetPlaneDataPtr(m_pDst[0], 0);
            m_lastDecodedFrame.SetPlanePitch(m_iDstStep[0], 0);
        }
        m_lastDecodedFrame.m_frameType = I_PICTURE;
    }

    m_lastDecodedFrame.m_fPTSStart = pDataIn->m_fPTSStart;
    if(m_bField)
        m_lastDecodedFrame.m_picStructure = (m_bFirstField)?PS_TOP_FIELD:PS_BOTTOM_FIELD;

    // check AVI1 header for interlace info
    if(m_pDecoder->IsAVI1App0Detected())
    {
        if(m_pDecoder->GetAVI1App0Polarity() == 0)
        {
            m_bField = false;
            m_lastDecodedFrame.m_picStructure = PS_PROGRESSIVE;
        }
        else if(m_pDecoder->GetAVI1App0Polarity() == 1)
        {
            m_bField = true;
            m_lastDecodedFrame.m_picStructure = PS_TOP_FIELD;
        }
        else if(m_pDecoder->GetAVI1App0Polarity() == 2)
        {
            m_bField = true;
            m_lastDecodedFrame.m_picStructure = PS_BOTTOM_FIELD;
        }
    }

    jStatus = m_pDecoder->ReadData();
    if(JPEG_ERR_BUFF == jStatus)
        return UMC_ERR_NOT_ENOUGH_DATA;
    else if(JPEG_OK != jStatus)
        return UMC_ERR_FAILED;

    pDataIn->MoveDataPointer(m_pDecoder->GetNumDecodedBytes());
    m_pDecoder->Reset();

    m_bFirstField = !m_bFirstField; // switch field

    status = m_pPostProcessor->GetFrame(&m_lastDecodedFrame, pDataOut);
    if(UMC_ERR_NOT_ENOUGH_DATA == status && pDataIn->GetDataSize() > 4) // second field is already in buffer, force additional loop with old data
        return UMC_ERR_SYNC;
    else if(UMC_OK != status)
        return status;

    m_params.m_iFramesCounter++;

    return UMC_OK;
}

#endif
