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
#include "umc_sample_buffer.h"
#include "umc_automatic_mutex.h"

using namespace UMC;


enum
{
    ALIGN_VALUE                 = 128
};

SampleBuffer::SampleBuffer(void)
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
    m_pSamples = NULL;

    m_bEndOfStream = false;
    m_bQuit = false;
} // SampleBuffer::SampleBuffer(void)

SampleBuffer::~SampleBuffer(void)
{
    Close();
} // SampleBuffer::~SampleBuffer(void)

Status SampleBuffer::Close(void)
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

    // reset variables
    m_pbAllocatedBuffer = NULL;
    m_lAllocatedBufferSize = 0;
    m_pbBuffer = NULL;
    m_lBufferSize = 0;
    m_pbFree = NULL;
    m_lFreeSize = 0;
    m_pbUsed = NULL;
    m_lUsedSize = 0;
    m_pSamples = NULL;

    m_bEndOfStream = false;
    m_bQuit = false;

    // call the parent's method
    MediaBuffer::Close();

    return UMC_OK;

} // Status SampleBuffer::Close(void)

Status SampleBuffer::Init(MediaReceiverParams *init)
{
    size_t lAllocate, lMaxSampleSize;
    MediaBufferParams *pParams = DynamicCast<MediaBufferParams, MediaReceiverParams> (init);
    Status umcRes;

    // check error(s)
    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    if ((0 == pParams->m_numberOfFrames) ||
        (0 == pParams->m_prefInputBufferSize))
        return UMC_ERR_INIT;

    // release the object before initialization
    Close();

    // call the parent's method
    umcRes = MediaBuffer::Init(init);
    if (UMC_OK != umcRes)
        return umcRes;

    m_Params = *pParams;
    // init the mutex
    m_synchro.Reset();

    // allocate buffer
    lMaxSampleSize = IPP_MAX(pParams->m_prefInputBufferSize, pParams->m_prefOutputBufferSize) +
                     ALIGN_VALUE + sizeof(SampleInfo);
    lAllocate = lMaxSampleSize * IPP_MAX(pParams->m_numberOfFrames, 3);
    if (UMC_OK != m_pMemoryAllocator->Alloc(&m_midAllocatedBuffer, lAllocate + ALIGN_VALUE, UMC_ALLOC_PERSISTENT, 16))
        return UMC_ERR_ALLOC;
    m_pbAllocatedBuffer = (Ipp8u *) m_pMemoryAllocator->Lock(m_midAllocatedBuffer);
    if (NULL == m_pbAllocatedBuffer)
        return UMC_ERR_ALLOC;
    m_lAllocatedBufferSize = lAllocate + ALIGN_VALUE;

    // align buffer
    m_pbBuffer = align_pointer<Ipp8u *> (m_pbAllocatedBuffer, ALIGN_VALUE);
    m_lBufferSize = lAllocate;

    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;
    m_pbUsed = m_pbBuffer;
    m_lUsedSize = 0;

    // save preferred size(s)
    m_lInputSize = pParams->m_prefInputBufferSize;
    return UMC_OK;

} // Status SampleBuffer::Init(MediaReceiverParams *init)

Status SampleBuffer::LockInputBuffer(MediaData* in)
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
    if (lFreeSize < m_lInputSize + ALIGN_VALUE * 2 + sizeof(SampleInfo))
    {
        if (false == bAtEnd)
            return UMC_ERR_NOT_ENOUGH_BUFFER;
        // free space at end is too small
        else
        {
            // when used data is present,
            // concatenate dummy bytes to the last sample info
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

                // need to call Unlock to avoid double locking of
                // the mutex
                guard.Unlock();
                // and call again to lock space at the
                // beginning of the buffer
                return LockInputBuffer(in);
            }
            // when no used data,
            // simply move pointer(s)
            else
            {
                m_pbFree = m_pbBuffer;
                m_pbUsed = m_pbBuffer;
                lFreeSize = m_lFreeSize;
            }
        }
    }

    // set free pointer
    in->SetBufferPointer(m_pbFree, lFreeSize - ALIGN_VALUE - sizeof(SampleInfo));
    in->SetDataSize(0);
    return UMC_OK;

} // Status SampleBuffer::LockInputBuffer(MediaData* in)

Status SampleBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)
{
    AutomaticMutex guard(m_synchro);
    size_t lFreeSize;
    SampleInfo *pTemp;
    Ipp8u *pb;

    // check END OF STREAM
    if (UMC_OK != StreamStatus)
    {
        m_bEndOfStream = true;
    }

    // check error(s)
    if (NULL == in)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbFree)
        return UMC_ERR_NOT_INITIALIZED;

    // get free size
    if (m_pbFree >= m_pbBuffer + (m_lBufferSize - m_lFreeSize))
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
    else
        lFreeSize = m_lFreeSize;

    // check free size
    if (lFreeSize < m_lInputSize)
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    // check used data
    if (in->GetDataSize() + ALIGN_VALUE * 2 + sizeof(SampleInfo) > lFreeSize)
        return UMC_ERR_FAILED;

    // get new sample info
    pb = align_pointer<Ipp8u *> (m_pbFree + in->GetDataSize(), ALIGN_VALUE);
    pTemp = reinterpret_cast<SampleInfo *> (pb);

    // fill sample info
    pTemp->m_dTime     = in->m_fPTSStart;
    pTemp->m_dTimeAux  = in->m_fPTSEnd;
    pTemp->m_FrameType = in->m_frameType;
    pTemp->m_lBufferSize = align_value<size_t> (pb + sizeof(SampleInfo) - m_pbFree, ALIGN_VALUE);
    pTemp->m_lDataSize = in->GetDataSize();
    pTemp->m_pbData = m_pbFree;
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
    m_lUsedSize += pTemp->m_lDataSize;

    return UMC_OK;

} // Status SampleBuffer::UnLockInputBuffer(MediaData* in, Status StreamStatus)

Status SampleBuffer::LockOutputBuffer(MediaData* out)
{
    AutomaticMutex guard(m_synchro);

    // check error(s)
    if (NULL == out)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbUsed)
        return UMC_ERR_NOT_INITIALIZED;

    // when END OF STREAM
    if (m_bEndOfStream)
    {
        // time to exit
        if ((m_bQuit) || (0 == m_lUsedSize))
        {
            // reset variables
            m_pbFree = m_pbBuffer;
            m_lFreeSize = m_lBufferSize;
            m_pbUsed = m_pbBuffer;
            m_lUsedSize = 0;
            m_pSamples = NULL;

            return UMC_ERR_END_OF_STREAM;
        }
        // set the last "lock output" request
        else
            m_bQuit = true;
    }

    if (NULL == m_pSamples)
        return UMC_ERR_NOT_ENOUGH_DATA;

    // set used pointer
    out->SetBufferPointer(m_pbUsed, m_pSamples->m_lDataSize);
    out->SetDataSize(m_pSamples->m_lDataSize);
    out->m_fPTSStart = m_pSamples->m_dTime;
    out->m_fPTSEnd   = m_pSamples->m_dTimeAux;
    out->m_frameType = m_pSamples->m_FrameType;
    return UMC_OK;

} // Status SampleBuffer::LockOutputBuffer(MediaData* out)

Status SampleBuffer::UnLockOutputBuffer(MediaData* out)
{
    AutomaticMutex guard(m_synchro);
    size_t lToSkip;

    // check error(s)
    if (NULL == out)
        return UMC_ERR_NULL_PTR;
    if (NULL == m_pbUsed)
        return UMC_ERR_NOT_INITIALIZED;

    // when END OF STREAM
    if (m_bEndOfStream)
        m_bQuit = false;

    // no filled data is present
    if (NULL == m_pSamples)
        return UMC_ERR_FAILED;

    // when 0 bytes to unlock
    lToSkip = out->GetBufferSize() - out->GetDataSize();
    if ((0 == lToSkip) &&
        (m_pSamples->m_lDataSize))
        return UMC_OK;

    // error occurs
    if (lToSkip > m_pSamples->m_lDataSize)
        return UMC_ERR_FAILED;

    // skip part of sample
    if (lToSkip < m_pSamples->m_lDataSize)
    {
        // update variable(s)
        m_lFreeSize += lToSkip;
        m_pbUsed += lToSkip;
        m_lUsedSize -= lToSkip;

        m_pSamples->m_lBufferSize -= lToSkip;
        m_pSamples->m_lDataSize -= lToSkip;
        m_pSamples->m_dTime = out->m_fPTSStart;
    }
    // skip whole sample
    else
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

} // Status SampleBuffer::UnLockOutputBuffer(MediaData* out)

Status SampleBuffer::Stop(void)
{
    return UMC_OK;

} // Status SampleBuffer::Stop(void)

Status SampleBuffer::Reset(void)
{
    AutomaticMutex guard(m_synchro);

    // reset variables
    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;
    m_pbUsed = m_pbBuffer;
    m_lUsedSize = 0;

    m_pSamples = NULL;

    m_bEndOfStream = false;
    m_bQuit = false;

    return UMC_OK;

} //Status SampleBuffer::Reset(void)
