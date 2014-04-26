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

#include "umc_linear_buffer.h"
#include "umc_video_buffer.h"
#include "umc_video_data.h"
#include "umc_automatic_mutex.h"

using namespace UMC;


VideoBuffer::VideoBuffer(void)
{
    m_lFrameNumber = 0;
    m_lIPDistance = 0;
    m_lGOPSize = 0;
    m_pEncPattern = NULL;
    m_nImageSize = 0;

} // VideoBuffer::VideoBuffer(void)

VideoBuffer::~VideoBuffer(void)
{
    Close();

} // VideoBuffer::~VideoBuffer(void)

bool VideoBuffer::BuildPattern(void)
{
    Ipp32u i;

    // error checking
    if ((0 == m_lGOPSize) || (m_lIPDistance > m_lGOPSize))
        return false;

    // allocate new pattern
    m_pEncPattern = new FrameType[m_lGOPSize + 1];
    if (NULL == m_pEncPattern)
        return false;

    // first frame in GOP is always I
    m_pEncPattern[0] = I_PICTURE;
    // fill encoding pattern
    for (i = 1; i < m_lGOPSize + 1; i++)
    {
        // all predicted frames (except last) are always P
        if (0 == ((i - 1) % m_lIPDistance))
        {
            if (m_lGOPSize - m_lIPDistance < i)
                m_pEncPattern[i] = I_PICTURE;
            else
                m_pEncPattern[i] = P_PICTURE;
        }
        // others frames are always B
        else
            m_pEncPattern[i] = B_PICTURE;
    }
    return true;

} // bool VideoBuffer::BuildPattern(void)

Status VideoBuffer::Init(MediaReceiverParams* init)
{
    VideoBufferParams* pParams = DynamicCast<VideoBufferParams, MediaReceiverParams> (init);
    Status umcRes;

    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    // check error(s)
    if (0 >= pParams->m_lGOPSize)
        return UMC_ERR_INVALID_STREAM;
    // release buffer
    Close();

    pParams->m_numberOfFrames = IPP_MAX(pParams->m_lIPDistance + 2, pParams->m_numberOfFrames);

    // initalize buffer (call to parent)
    umcRes = SampleBuffer::Init(pParams);
    if (UMC_OK != umcRes)
        return umcRes;

    // save parameter(s)
    m_lGOPSize = pParams->m_lGOPSize;
    m_lIPDistance = pParams->m_lIPDistance;

    // build pattern
    if (false == BuildPattern())
        return UMC_ERR_ALLOC;
    return UMC_OK;

} // Status VideoBuffer::Init(MediaReceiverParams* init)

Status VideoBuffer::Close(void)
{
    // stop all waiting(s)
    Stop();

    SampleBuffer::Close();

    // delete pattern
    if (m_pEncPattern)
        delete m_pEncPattern;

    m_lFrameNumber = 0;
    m_lIPDistance = 0;
    m_lGOPSize = 0;
    m_pEncPattern = NULL;
    m_nImageSize = 0;
    return UMC_OK;

} // Status VideoBuffer::Close(void)

Status VideoBuffer::LockInputBuffer(MediaData *in)
{
    VideoData *pData = DynamicCast<VideoData, MediaData> (in);
    Status umcRes;

    // check error(s)
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    // allocate buffer
    umcRes = SampleBuffer::LockInputBuffer(in);
    if (UMC_OK != umcRes)
        return umcRes;

    pData->m_frameType = I_PICTURE;
    return UMC_OK;

} // Status VideoBuffer::LockInputBuffer(MediaData *in)

Status VideoBuffer::UnLockInputBuffer(MediaData *in, Status StreamStatus)
{
    VideoData *pData = DynamicCast<VideoData, MediaData> (in);
    Status umcRes;

    // check error(s)
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    // just for ensurance set image size
    if (UMC_OK == StreamStatus)
        pData->SetDataSize(m_lInputSize);
    else
        pData->SetDataSize(0);

    // save frame in queue
    umcRes = SampleBuffer::UnLockInputBuffer(in, StreamStatus);
    if (UMC_OK != umcRes)
        return umcRes;
    return UMC_OK;

} // Status VideoBuffer::UnLockInputBuffer(MediaData *in, Status StreamStatus))

Status VideoBuffer::LockOutputBuffer(MediaData *out)
{
    VideoData *pData = DynamicCast<VideoData, MediaData> (out);
    AutomaticMutex guard(m_synchro);
    SampleInfo *pTemp = NULL;
    Ipp32u lOffset;

    // check error(s)
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    // calc offset in ready frames list
    // when it's first I frame - offset always 0
    if ((I_PICTURE == m_pEncPattern[m_lFrameNumber]) && (0 == m_lFrameNumber))
        lOffset = 0;
    // when it's B frame - offset always 0
    else if (B_PICTURE == m_pEncPattern[m_lFrameNumber])
        lOffset = 0;
    // when it's I frame at end of pattern (very complex to understand)
    else if (I_PICTURE == m_pEncPattern[m_lFrameNumber])
        lOffset = m_lGOPSize - m_lFrameNumber;
    // when P_PICTURE to encode
    else
        lOffset = m_lIPDistance - 1;

    // get needed buffer
    pTemp = m_pSamples;
    while ((pTemp) && (pTemp->m_pNext) && (lOffset))
    {
        pTemp = pTemp->m_pNext;
        lOffset -= 1;
    }
    if (pTemp && lOffset>0) { // cant reach next P/I frame, use last
        if (!m_bEndOfStream) return UMC_ERR_NOT_ENOUGH_DATA;
    }

    if (NULL == pTemp)
    {
        // handle end of stream
        if (m_bEndOfStream)
        {
            // time to exit
            if ((m_bQuit) || (0 == m_lUsedSize))
            {
                // reset variables
                m_lFrameNumber = 0;
                m_pbFree = m_pbBuffer;
                m_lFreeSize = m_lBufferSize;
                m_pbUsed = m_pbBuffer;
                m_lUsedSize = 0;
                m_pSamples = NULL;

                return UMC_ERR_END_OF_STREAM;
            }
            // last "lock output" request
            else
                m_bQuit = true;
        }
        return UMC_ERR_NOT_ENOUGH_DATA;
    }

    // set used pointer
    out->SetBufferPointer(pTemp->m_pbData, pTemp->m_lDataSize);
    out->SetDataSize(pTemp->m_lDataSize);
    out->m_fPTSStart = pTemp->m_dTime;

    pData->m_frameType = m_pEncPattern[m_lFrameNumber];
    return UMC_OK;

} // Status VideoBuffer::LockOutputBuffer(MediaData *out)

Status VideoBuffer::UnLockOutputBuffer(MediaData* out)
{
    AutomaticMutex guard(m_synchro);
    SampleInfo *pTemp = NULL;
    Ipp32u lOffset;

    // check error(s)
    if ((NULL == out) ||
        (NULL == m_pbUsed))
        return UMC_ERR_NULL_PTR;

    // when END OF STREAM
    if (m_bEndOfStream)
        m_bQuit = false;

    // calc offset in ready frames list
    // when it's first I frame - offset always 0
    if ((I_PICTURE == m_pEncPattern[m_lFrameNumber]) && (0 == m_lFrameNumber))
        lOffset = 0;
    // when it's B frame - offset always 0
    else if (B_PICTURE == m_pEncPattern[m_lFrameNumber])
        lOffset = 0;
    // when it's I frame at end of pattern (very complex to understand)
    else if (I_PICTURE == m_pEncPattern[m_lFrameNumber])
        lOffset = m_lGOPSize - m_lFrameNumber;
    // when P_PICTURE to encode
    else
        lOffset = m_lIPDistance - 1;

    // get needed buffer
    pTemp = m_pSamples;
    while ((pTemp) && (pTemp->m_pNext) && (lOffset))
    {
        pTemp = pTemp->m_pNext;
        lOffset -= 1;
    }

    if (NULL == pTemp)
        return UMC_ERR_FAILED;

    // increment frame number
    m_lFrameNumber += 1;

    // reset frame number (TO 1 EXACT)
    if (m_lFrameNumber > m_lGOPSize)
        m_lFrameNumber = 1;

    // skip sample
    pTemp->m_pbData = NULL;

    // free byte(s)
    while ( (m_pSamples) && (NULL == m_pSamples->m_pbData))
    {
        // update variable(s)
        m_lFreeSize += m_pSamples->m_lBufferSize;
        m_pbUsed += m_pSamples->m_lBufferSize;
        if (m_pbBuffer + m_lBufferSize == m_pbUsed)
            m_pbUsed = m_pbBuffer;
        m_lUsedSize -= m_pSamples->m_lDataSize;

        m_pSamples = m_pSamples->m_pNext;

    }
    return UMC_OK;

} // Status VideoBuffer::UnLockOutputBuffer(MediaData* out)
