/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_dual_thread_codec.h"
#include "vm_debug.h"
#include "vm_time.h"

namespace UMC
{
Status PutDataToBuffer(MediaBuffer *pBuffer, MediaData* in, Status StreamStatus)
{
    Status status;
    MediaData tmp[1];
    *tmp = *in;
    UMC_CHECK_FUNC(status, pBuffer->LockInputBuffer(tmp));
    UMC_CHECK_FUNC(status, in->MoveDataTo(tmp));
    //UMC_CHECK_FUNC(status,  !!!
    pBuffer->UnLockInputBuffer(tmp, StreamStatus);
    in->m_fPTSStart = in->m_fPTSEnd = -1; // timestamps become incorrect after MoveDataTo()
    return UMC_OK;
}

Status GetDataFromBuffer(MediaBuffer *pBuffer, MediaData* out)
{
    Status status;
    MediaData tmp[1];
    UMC_CHECK_FUNC(status, pBuffer->LockOutputBuffer(tmp));
    UMC_CHECK_FUNC(status, tmp->MoveDataTo(out));
    UMC_CHECK_FUNC(status, pBuffer->UnLockOutputBuffer(tmp));
    return UMC_OK;
}
}

using namespace UMC;


DualThreadedCodec::DualThreadedCodec(void)
{
    m_pCodec                  = NULL;
    m_pInputBuffer            = NULL;
    m_pOutputBuffer           = NULL;
    m_pPostponedParams        = NULL;
    m_bDeleteResourcesOnClose = false;
    m_bGetFrameLoop           = false;
}

DualThreadedCodec::~DualThreadedCodec(void)
{
    DualThreadedCodec::Close();
}

Status DualThreadedCodec::InitCodec(DualThreadCodecParams* init)
{
    BaseCodecParams    CodecParams;
    Status umcRes;

    // release codec
    Close();

    // check parameter(s)
    if ((NULL == init) ||
        (NULL == init->m_pCodec) ||
        (NULL == init->m_pMediaBuffer) ||
        (NULL == init->m_pCodecInitParams) || // really can be NULL for MP3 decoder
        (NULL == init->m_pMediaBufferParams)) // must keep at least number of frames
        return UMC_ERR_NULL_PTR;

    // init audio codec
    umcRes = init->m_pCodec->Init(init->m_pCodecInitParams);
    if (umcRes != UMC_OK)
        return umcRes;

    // get SuggestedInputSize
    umcRes = init->m_pCodec->GetInfo(&CodecParams);
    if (umcRes != UMC_OK)
        return umcRes;

    // initialize media buffer

    // to have the possibility to increase InputSize from the app
    if (CodecParams.m_iSuggestedInputSize > init->m_pMediaBufferParams->m_prefInputBufferSize)
        init->m_pMediaBufferParams->m_prefInputBufferSize = CodecParams.m_iSuggestedInputSize;

    init->m_pMediaBufferParams->m_prefOutputBufferSize = CodecParams.m_iSuggestedInputSize;

    umcRes = init->m_pMediaBuffer->Init(init->m_pMediaBufferParams);
    if (umcRes != UMC_OK)
        return umcRes;

    // save pointer(s)
    m_pCodec = init->m_pCodec;
    m_pInputBuffer = init->m_pMediaBuffer;

    return umcRes;
} // Status DualThreadedCodec::InitCodec(DualThreadCodecParams* init)

Status DualThreadedCodec::Init(BaseCodec   *pCodec,
                               MediaBuffer *pInputBuffer,
                               MediaBuffer *pOutputBuffer)
{
    UMC_CHECK_PTR(pCodec);

    m_pCodec = pCodec;
    m_pInputBuffer = pInputBuffer;
    m_pOutputBuffer = pOutputBuffer;

    m_bDeleteResourcesOnClose = true; //!!!
    m_bGetFrameLoop = true; //!!!

    if (pInputBuffer == NULL) m_bGetFrameLoop = false;

    return UMC_OK;
}

static MediaData** GetPointerToData(BaseCodecParams *pCodecParams)
{
    if (NULL != pCodecParams)
    {
       return &pCodecParams->m_pData;
    }
    return NULL;
}

Status DualThreadedCodec::Init(BaseCodecParams *pCodecParams)
{
    MediaData **ppData;
    Status status;
    UMC_CHECK(m_pCodec, UMC_ERR_NOT_INITIALIZED);

    ppData = GetPointerToData(pCodecParams);

    if (m_pInputBuffer && ppData && *ppData) // use postponed codec init
    {
        UMC_CHECK_FUNC(status, PutDataToBuffer(m_pInputBuffer, *ppData));
        *ppData = NULL;
        m_pPostponedParams = pCodecParams; // save pCodecParams for postponed Init()
        return PostponedInit();
    }
    else
    {
        return m_pCodec->Init(pCodecParams);
    }
}

Status DualThreadedCodec::PostponedInit()
{
    MediaData tmp[1];
    Status umcRes = UMC_OK;

    UMC_CHECK(m_pPostponedParams, UMC_OK);

    umcRes = m_pInputBuffer->LockOutputBuffer(tmp);
    if (UMC_ERR_NOT_ENOUGH_DATA == umcRes)
    {
        return UMC_OK;
    }
    UMC_CHECK_STATUS(umcRes);

    MediaData **ppData = GetPointerToData(m_pPostponedParams);
    UMC_CHECK_PTR(ppData);
    *ppData = tmp;

    UMC_CHECK_FUNC(umcRes, m_pCodec->Init(m_pPostponedParams));
    *ppData = NULL;

    //UMC_CHECK_FUNC(status, 
    m_pCodec->GetInfo(m_pPostponedParams);

    // Here means succesful initialiazation
    UMC_CHECK_FUNC(umcRes, m_pInputBuffer->UnLockOutputBuffer(tmp));
    m_pPostponedParams = NULL;

    return UMC_OK;
}

Status DualThreadedCodec::GetFrame(MediaData *in, MediaData *out)
{
    MediaData *pInputData = in;
    MediaData *pOutputData = out;
    Status umcRes = UMC_OK;

    UMC_CHECK(m_pCodec, UMC_ERR_NOT_INITIALIZED);

    // Put data to InputBuffer
    if (m_pInputBuffer && in && in->GetDataSize())
    {
        umcRes = PutDataToBuffer(m_pInputBuffer, in);
    }

    if (m_pPostponedParams)
    {
        UMC_CHECK_FUNC(umcRes, PostponedInit());
        if (m_pPostponedParams) return UMC_ERR_NOT_INITIALIZED; // not inilized yet
        return UMC_ERR_NOT_ENOUGH_DATA; // codec was initialized, waiting for data
    }

    // Lock output data
    if (m_pOutputBuffer && UMC_OK == umcRes)
    {
        umcRes = m_pOutputBuffer->LockInputBuffer(m_pTmpOutput);
        if (UMC_OK == umcRes) {
            pOutputData = m_pTmpOutput;
        }
    }

    // GetFrame
    if (UMC_OK == umcRes)
    {
        double dStartTime = -1;
        int out_size = 0;

        for (;;)
        {
            bool bLocked = false;
            umcRes = UMC_OK;

            // Lock input data
            if (m_pInputBuffer)
            {
                pInputData = m_pTmpInput;
                umcRes = m_pInputBuffer->LockOutputBuffer(m_pTmpInput);
                bLocked = (UMC_OK == umcRes);
            }

            if (!pInputData->GetDataSize() && m_bGetFrameLoop)
            {
                umcRes = UMC_ERR_NOT_ENOUGH_DATA;
            }

            if (UMC_OK == umcRes)
            {
                umcRes = m_pCodec->GetFrame(pInputData, pOutputData);
            }

            // Unlock input data
            if (bLocked) // if input buffer was locked
            {
                if ((Ipp8u*)pInputData->GetDataPointer() != (Ipp8u*)pInputData->GetBufferPointer())
                {
                    m_pInputBuffer->UnLockOutputBuffer(pInputData);
                }
#if 0
                else if (pInputData->GetDataSize())
                {
                    umcRes = UMC_ERR_NOT_ENOUGH_DATA;
                }
#endif
            }

            if (UMC_OK != umcRes)
            {
                break;
            }

            if (!out_size)
            {
                dStartTime = pOutputData->m_fPTSStart;
            }

            int curr_size = (int) pOutputData->GetDataSize();
            out_size += curr_size;
            pOutputData->MoveDataPointer(curr_size);

            if (!m_bGetFrameLoop)
            {
                break;
            }
        }

        if (out_size)
        {
            umcRes = UMC_OK;
            pOutputData->MoveDataPointer(-out_size);
            // correct start time
            pOutputData->m_fPTSStart = dStartTime;
        }
    }

    // Unlock output data
    if (pOutputData == m_pTmpOutput) // if output buffer was locked
    {
        m_pOutputBuffer->UnLockInputBuffer(pOutputData);
    }

    // Get data from OutputBuffer
    if (m_pOutputBuffer && out && UMC_OK == umcRes)
    {
        umcRes = GetDataFromBuffer(m_pOutputBuffer, out);
    }

    return umcRes;
}

Status DualThreadedCodec::GetInfo(BaseCodecParams *info)
{
    UMC_CHECK(m_pCodec, UMC_ERR_NOT_INITIALIZED);
    UMC_CHECK(NULL == m_pPostponedParams, UMC_ERR_NOT_INITIALIZED);
    return m_pCodec->GetInfo(info);
}

Status DualThreadedCodec::Close(void)
{
    if (m_bDeleteResourcesOnClose)
    {
        UMC_DELETE(m_pCodec);
        UMC_DELETE(m_pInputBuffer);
        UMC_DELETE(m_pOutputBuffer);
    }
    return UMC_OK;
}

Status DualThreadedCodec::Reset(void)
{
    UMC_CHECK(m_pCodec, UMC_ERR_NOT_INITIALIZED);
    if (m_pInputBuffer)
    {
        m_pInputBuffer->Reset();
    }
    if (m_pOutputBuffer)
    {
        m_pOutputBuffer->Reset();
    }
    return m_pCodec->Reset();
}

Status DualThreadedCodec::SetParams(BaseCodecParams *params)
{
    UMC_CHECK(m_pCodec, UMC_ERR_NOT_INITIALIZED);
    return m_pCodec->SetParams(params);
}

Status DualThreadedCodec::LockInputBuffer(MediaData* in)
{
    UMC_CHECK(m_pInputBuffer, UMC_ERR_NOT_INITIALIZED);
    return m_pInputBuffer->LockInputBuffer(in);
}

Status DualThreadedCodec::UnLockInputBuffer(MediaData* in, Status StreamStatus)
{
    UMC_CHECK(m_pInputBuffer, UMC_ERR_NOT_INITIALIZED);
    return m_pInputBuffer->UnLockInputBuffer(in, StreamStatus);
}

Status DualThreadedCodec::GetFrame(MediaData* out)
{
    return GetFrame(NULL, out);
}
