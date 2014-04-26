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
#include "umc_automatic_mutex.h"

using namespace UMC;


enum
{
    ALIGN_VALUE                 = 128
};

LinearBuffer::LinearBuffer(void)
{
    // reset variables
    m_pbAllocatedBuffer = NULL;
    m_midAllocatedBuffer = MID_INVALID;
    m_lAllocatedBufferSize = 0;
    m_pbBuffer = NULL;
    m_lBufferSize = 0;
    m_pbFree = NULL;
    m_lFreeSize = 0;
    m_pbUsed = NULL;
    m_lUsedSize = 0;
    m_lDummySize = 0;
    m_pSamples = NULL;
    m_pFreeSampleInfo = NULL;

    m_bEndOfStream = false;
    m_bQuit = false;

    // reset time of dummy sample
    memset(&m_Dummy, 0, sizeof(m_Dummy));
    m_Dummy.m_dTime = -1.0;

} // LinearBuffer::LinearBuffer(void)

LinearBuffer::~LinearBuffer(void)
{
    Close();
} // LinearBuffer::~LinearBuffer(void)

Status LinearBuffer::Close(void)
{
    // stop all waiting(s)
    // actually, this method does nothing
    Stop();

    // delete buffer
    if (m_pbAllocatedBuffer)
        m_pMemoryAllocator->Unlock(m_midAllocatedBuffer);
    if (MID_INVALID != m_midAllocatedBuffer)
        m_pMemoryAllocator->Free(m_midAllocatedBuffer);
    m_midAllocatedBuffer = MID_INVALID;

    // delete sample list
    while (m_pSamples)
    {
        SampleInfo *pTemp = m_pSamples;
        m_pSamples = m_pSamples->m_pNext;
        delete pTemp;
    }

    // delete free sample list
    while (m_pFreeSampleInfo)
    {
        SampleInfo *pTemp = m_pFreeSampleInfo;
        m_pFreeSampleInfo = m_pFreeSampleInfo->m_pNext;
        delete pTemp;
    }

    // reset variables
    m_pbAllocatedBuffer = NULL;
    m_lAllocatedBufferSize = 0;
    m_pbBuffer = NULL;
    m_lBufferSize = 0;
    m_pbFree = NULL;
    m_lFreeSize = 0;
    m_pbUsed = NULL;
    m_lUsedSize = 0;
    m_lDummySize = 0;
    m_pSamples = NULL;
    m_pFreeSampleInfo = NULL;

    m_bEndOfStream = false;
    m_bQuit = false;

    // reset time of dummy sample
    m_Dummy.m_dTime = -1.0;

    // call the parent's method
    MediaBuffer::Close();

    return UMC_OK;

} // Status LinearBuffer::Close(void)

Status LinearBuffer::Init(MediaReceiverParams *init)
{
    size_t lAllocate, lMaxSampleSize;
    Ipp32u l, lFramesNumber;
    MediaBufferParams *pParams = DynamicCast<MediaBufferParams, MediaReceiverParams> (init);
    Status umcRes;

    // check error(s)
    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    if ((0 == pParams->m_numberOfFrames) ||
        (0 == pParams->m_prefInputBufferSize) ||
        (0 == pParams->m_prefOutputBufferSize))
        return UMC_ERR_INIT;

    // release the object before initialization
    Close();

    // call the parent's method
    umcRes = MediaBuffer::Init(init);
    if (UMC_OK != umcRes)
        return umcRes;

    m_Params = *pParams;

    // init mutex
    m_synchro.Reset();

    // allocate buffer (one more)
    lMaxSampleSize = IPP_MAX(pParams->m_prefInputBufferSize, pParams->m_prefOutputBufferSize);
    lAllocate = lMaxSampleSize * (IPP_MAX(pParams->m_numberOfFrames, 3) + 1);
    if (UMC_OK != m_pMemoryAllocator->Alloc(&m_midAllocatedBuffer, lAllocate + ALIGN_VALUE, UMC_ALLOC_PERSISTENT, 16))
        return UMC_ERR_ALLOC;
    m_pbAllocatedBuffer = (Ipp8u *) m_pMemoryAllocator->Lock(m_midAllocatedBuffer);
    if (NULL == m_pbAllocatedBuffer)
        return UMC_ERR_ALLOC;
    m_lAllocatedBufferSize = lAllocate + ALIGN_VALUE;

    // align buffer & reserve one sample
    m_pbBuffer = align_pointer<Ipp8u *> (m_pbAllocatedBuffer + lMaxSampleSize, ALIGN_VALUE);
    m_lBufferSize = lAllocate - lMaxSampleSize;

    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;
    m_pbUsed = m_pbBuffer;
    m_lUsedSize = 0;

    // allocate sample info
    lFramesNumber = (Ipp32u) ((IPP_MAX(pParams->m_prefInputBufferSize, pParams->m_prefOutputBufferSize) *
                               pParams->m_numberOfFrames) /
                              IPP_MIN(pParams->m_prefInputBufferSize, pParams->m_prefOutputBufferSize));
    for (l = 0; l < lFramesNumber; l++)
    {
        SampleInfo *pTemp;

        pTemp = new SampleInfo();
        if (NULL == pTemp)
            return UMC_ERR_ALLOC;

        pTemp->m_pNext = m_pFreeSampleInfo;
        m_pFreeSampleInfo = pTemp;
    }

    // save preferended size(s)
    m_lInputSize = pParams->m_prefInputBufferSize;
    m_lOutputSize = pParams->m_prefOutputBufferSize;

    return UMC_OK;

} // Status LinearBuffer::Init(MediaReceiverParams *init)

Status LinearBuffer::LockInputBuffer(MediaData* in)
{
    AutomaticMutex guard(m_synchro);
    size_t lFreeSize;
    bool bAtEnd = false;

    // check error(s)
    if (NULL == in)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbFree)
        return UMC_ERR_NOT_INITIALIZED;

    // get free size
    if (m_pbFree >= m_pbBuffer + (m_lBufferSize - m_lFreeSize))
    {
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
        bAtEnd = true;
    }
    else
        lFreeSize = m_lFreeSize;

    // check free size
    if (lFreeSize < m_lInputSize)
    {
        if (false == bAtEnd)
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        // free space at end is too small
        else
        {
            // when used data is present,
            // concatenate dummy bytes to last sample info
            if (m_pSamples)
            {
                SampleInfo *pTemp;

                // find last sample info
                pTemp = m_pSamples;
                while (pTemp->m_pNext)
                    pTemp = pTemp->m_pNext;
                pTemp->m_lBufferSize += lFreeSize;

                // update variable(s)
                m_pbFree = m_pbBuffer;
                m_lFreeSize -= lFreeSize;
                m_lDummySize = lFreeSize;
                m_lUsedSize += lFreeSize;

                // need to Unlock to avoid double locking of
                // the mutex
                guard.Unlock();
                // and call again to lock space at the
                // beginning of the buffer
                return LockInputBuffer(in);
            }
            // when there is no used data,
            // simply move pointer(s)
            else
            {
                m_pbFree = m_pbBuffer;
                m_pbUsed = m_pbBuffer;
                lFreeSize = m_lFreeSize;
            }
        }
    }

    // check free sample info
    if (NULL == m_pFreeSampleInfo)
    {
        // when needed - allocate one more
        if (m_lUsedSize - m_lDummySize < m_lOutputSize)
        {
            m_pFreeSampleInfo = new SampleInfo();
            if (m_pFreeSampleInfo)
                m_pFreeSampleInfo->m_pNext = NULL;
            else
                return UMC_ERR_NOT_ENOUGH_BUFFER;
        }
        else
            return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    // set free pointer
    in->SetBufferPointer(m_pbFree, lFreeSize);
    in->SetDataSize(0);
    return UMC_OK;

} // Status LinearBuffer::LockInputBuffer(MediaData* in)

Status LinearBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)
{
    AutomaticMutex guard(m_synchro);
    size_t lFreeSize;
    SampleInfo *pTemp;

    // check END OF STREAM
    if (UMC_OK != StreamStatus)
        m_bEndOfStream = true;

    // check error(s)
    if (NULL == in)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbFree)
        return UMC_ERR_NOT_INITIALIZED;

    // when no data is given
    if (0 == in->GetDataSize())
        return UMC_OK;

    // get free size
    if (m_pbFree + m_lFreeSize >= m_pbBuffer + m_lBufferSize)
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
    else
        lFreeSize = m_lFreeSize;

    // check free size
    if ((lFreeSize < m_lInputSize) ||
        (NULL == m_pFreeSampleInfo))
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    // check used data
    if (in->GetDataSize() > lFreeSize)
        return UMC_ERR_FAILED;

    // get new sample info
    pTemp = m_pFreeSampleInfo;
    m_pFreeSampleInfo = m_pFreeSampleInfo->m_pNext;

    //
    // fill sample info
    //

    // handle data gaps
    if ((m_pSamples) || (in->m_fPTSStart > m_Dummy.m_dTime))
        pTemp->m_dTime = in->m_fPTSStart;
    else
        pTemp->m_dTime = m_Dummy.m_dTime;
    pTemp->m_lBufferSize =
    pTemp->m_lDataSize = in->GetDataSize();
    pTemp->m_pNext = NULL;

    // add sample to end of queue
    if (m_pSamples)
    {
        SampleInfo *pWork = m_pSamples;

        while (pWork->m_pNext)
            pWork = pWork->m_pNext;

        pWork->m_pNext = pTemp;
    }
    else
        m_pSamples = pTemp;

    // update variable(s)
    m_pbFree += pTemp->m_lBufferSize;
    if (m_pbBuffer + m_lBufferSize == m_pbFree)
        m_pbFree = m_pbBuffer;
    m_lFreeSize -= pTemp->m_lBufferSize;
    m_lUsedSize += pTemp->m_lBufferSize;
    return UMC_OK;

} // Status LinearBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)

Status LinearBuffer::LockOutputBuffer(MediaData* out)
{
    AutomaticMutex guard(m_synchro);
    size_t lUsedSize;

    // check error(s)
    if (NULL == out)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbUsed)
        return UMC_ERR_NOT_INITIALIZED;

    // when END OF STREAM
    if (m_bEndOfStream)
    {
        // time to exit
        if ((m_bQuit) || (0 == m_lUsedSize - m_lDummySize))
        {
            // add sample info(s) to list of free sample info(s)
            if (m_pFreeSampleInfo)
            {
                SampleInfo *pTemp = m_pFreeSampleInfo;

                while (pTemp->m_pNext)
                    pTemp = pTemp->m_pNext;

                pTemp->m_pNext = m_pSamples;
                m_pSamples = NULL;
            }
            else
            {
                m_pFreeSampleInfo = m_pSamples;
                m_pSamples = NULL;
            }

            // reset variables
            m_pbFree = m_pbBuffer;
            m_lFreeSize = m_lBufferSize;
            m_pbUsed = m_pbBuffer;
            m_lUsedSize = 0;
            m_lDummySize = 0;

            return UMC_ERR_END_OF_STREAM;
        }
        // set the last "lock output" request
        else
            m_bQuit = true;
    }

    // when used pointer is wrapped around
    if (m_pbBuffer + m_lBufferSize == m_pbUsed)
        m_pbUsed = m_pbBuffer;

    // when used space at end is small (move it to beginning)
    if ((m_pbUsed + m_lUsedSize >= m_pbBuffer + m_lBufferSize) &&
        (m_pbBuffer + m_lBufferSize < m_pbUsed + m_lOutputSize + m_lDummySize))
    {
        size_t lBytesAtEnd;

        lBytesAtEnd = m_pbBuffer + m_lBufferSize - m_pbUsed - m_lDummySize;
        // copy remain byte(s) before the buffer
        memcpy(m_pbBuffer - lBytesAtEnd, m_pbUsed, lBytesAtEnd);
        // update variable(s)
        m_pbUsed = m_pbBuffer - lBytesAtEnd;
        m_lFreeSize += lBytesAtEnd + m_lDummySize;
        m_lUsedSize -= m_lDummySize;
        m_lDummySize = 0;
        // remove dummy bytes from sample info
        {
            SampleInfo *pTemp = m_pSamples;

            while (pTemp)
            {
                if (pTemp->m_lBufferSize != pTemp->m_lDataSize)
                {
                    pTemp->m_lBufferSize = pTemp->m_lDataSize;
                    break;
                }
                pTemp = pTemp->m_pNext;
            }
        }
    }

    // get used size
    if (m_pbUsed >= m_pbBuffer + (m_lBufferSize - m_lUsedSize))
        lUsedSize = m_pbBuffer + m_lBufferSize - m_pbUsed - m_lDummySize;
    else
        lUsedSize = m_lUsedSize;

    // check used size
    if ((lUsedSize < m_lOutputSize) && (false == m_bEndOfStream))
        return UMC_ERR_NOT_ENOUGH_DATA;

    // set used pointer
    out->SetBufferPointer(m_pbUsed, lUsedSize);
    out->SetDataSize(lUsedSize);
    out->m_fPTSStart = m_pSamples->m_dTime;
    return UMC_OK;

} // Status LinearBuffer::LockOutputBuffer(MediaData* out)

Status LinearBuffer::UnLockOutputBuffer(MediaData* out)
{
    AutomaticMutex guard(m_synchro);
    size_t lUsedSize, lToSkip;
    SampleInfo *pTemp;
    bool bSingleMemoryPiece = true;

    // check error(s)
    if (NULL == out)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbUsed)
        return UMC_ERR_NOT_INITIALIZED;

    // drop the "last lock output" event,
    // because data was used
    if (m_bEndOfStream)
        m_bQuit = false;

    // when 0 bytes to unlock
    if (0 == (out->GetBufferSize() - out->GetDataSize()))
        return UMC_OK;

    // get used size
    if (m_pbUsed >= m_pbBuffer + (m_lBufferSize - m_lUsedSize))
        lUsedSize = m_pbBuffer + m_lBufferSize - m_pbUsed - m_lDummySize;
    else
        lUsedSize = m_lUsedSize;

    // check used size
    if ((lUsedSize < m_lOutputSize) && (false == m_bEndOfStream))
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    // check used data
    lToSkip = out->GetBufferSize() - out->GetDataSize();
    if (lToSkip > lUsedSize)
        return UMC_ERR_FAILED;

    // skip data
    while (lToSkip)
    {
        size_t lFreeSizeInc;

        pTemp = m_pSamples;

        // when skipped data is placed before the buffer
        if (m_pbUsed < m_pbBuffer)
        {
            if (lToSkip < pTemp->m_lDataSize)
                lFreeSizeInc = 0;
            else
                lFreeSizeInc = IPP_MAX(m_pbUsed + pTemp->m_lBufferSize - m_pbBuffer, 0);
        }
        // when skipped data is placed in middle of buffer
        else
        {
            if (lToSkip < pTemp->m_lDataSize)
                lFreeSizeInc = lToSkip;
            else
                lFreeSizeInc = pTemp->m_lBufferSize;
        }

        // when data to skip is smaller then the current data potion
        if (lToSkip < pTemp->m_lDataSize)
        {
            // update timestamp. We keep the timestamp only for the first
            // frame in memory piece.
            if (bSingleMemoryPiece)
                pTemp->m_dTime = -1.0;

            m_pbUsed += lToSkip;
            m_lUsedSize -= lToSkip;
            m_lFreeSize += lFreeSizeInc;
            pTemp->m_lBufferSize -= lToSkip;
            pTemp->m_lDataSize -= lToSkip;

            lToSkip = 0;
        }
        // when data to skip is greater then the current data potion
        else
        {
            if (pTemp->m_lBufferSize != pTemp->m_lDataSize)
                m_lDummySize = 0;

            m_pbUsed += pTemp->m_lBufferSize;
            m_lUsedSize -= pTemp->m_lBufferSize;
            m_lFreeSize += lFreeSizeInc;
            lToSkip -= pTemp->m_lDataSize;

            // update sample queue(s)
            m_pSamples = m_pSamples->m_pNext;
            pTemp->m_pNext = m_pFreeSampleInfo;
            m_pFreeSampleInfo = pTemp;
            pTemp = m_pSamples;

            // we skip several memory pieces
            bSingleMemoryPiece = false;
        }
    }
    return UMC_OK;

} // Status LinearBuffer::UnLockOutputBuffer(MediaData* out)

Status LinearBuffer::Stop(void)
{
    return UMC_OK;

} // Status LinearBuffer::Stop(void)

Status LinearBuffer::Reset(void)
{
    AutomaticMutex guard(m_synchro);

    // reset variables
    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;
    m_pbUsed = m_pbBuffer;
    m_lUsedSize = 0;

    // move all samples to the free list
    while (m_pSamples)
    {
        SampleInfo *pTemp = m_pSamples;
        m_pSamples = m_pSamples->m_pNext;
        pTemp->m_pNext = m_pFreeSampleInfo;
        m_pFreeSampleInfo = pTemp;
    }

    m_bEndOfStream = false;
    m_bQuit = false;

    return UMC_OK;

} // Status LinearBuffer::Reset(void)
