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
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_structures.h"
#include "umc_h264_heap.h"
#include "umc_h264_dec_defs_dec.h"

namespace UMC
{
Ipp32s lock_failed = 0;

enum
{
    COEFFS_BUFFER_ALIGN_VALUE                 = 128
};

H264CoeffsBuffer::H264CoeffsBuffer(void)
{
    // reset variables
    m_pbAllocatedBuffer = 0;
    m_lAllocatedBufferSize = 0;

    m_pbBuffer = 0;
    m_lBufferSize = 0;
    m_pbFree = 0;
    m_lFreeSize = 0;
    m_pBuffers = 0;

} // H264CoeffsBuffer::H264CoeffsBuffer(void)

H264CoeffsBuffer::~H264CoeffsBuffer(void)
{
    Close();
} // H264CoeffsBuffer::~H264CoeffsBuffer(void)

void H264CoeffsBuffer::Lock()
{
    mutex.Lock();
}

void H264CoeffsBuffer::Unlock()
{
    mutex.Unlock();
}

void H264CoeffsBuffer::Close(void)
{
    // delete buffer
    if (m_pbAllocatedBuffer)
        delete[] m_pbAllocatedBuffer;

    // reset variables
    m_pbAllocatedBuffer = 0;
    m_lAllocatedBufferSize = 0;
    m_pbBuffer = 0;
    m_lBufferSize = 0;
    m_pbFree = 0;
    m_lFreeSize = 0;
    m_pBuffers = 0;
} // void H264CoeffsBuffer::Close(void)

Status H264CoeffsBuffer::Init(Ipp32s numberOfItems, Ipp32s sizeOfItem)
{
    Ipp32s lAllocate, lMaxSampleSize;

    Close();

    // allocate buffer
    lMaxSampleSize = sizeOfItem + COEFFS_BUFFER_ALIGN_VALUE + (Ipp32s)sizeof(BufferInfo);
    lAllocate = lMaxSampleSize * numberOfItems;
    m_pbAllocatedBuffer = h264_new_array_throw<Ipp8u>(lAllocate + COEFFS_BUFFER_ALIGN_VALUE);
    m_lAllocatedBufferSize = lAllocate + COEFFS_BUFFER_ALIGN_VALUE;

    // align buffer
    m_pbBuffer = align_pointer<Ipp8u *> (m_pbAllocatedBuffer, COEFFS_BUFFER_ALIGN_VALUE);
    m_lBufferSize = lAllocate;

    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;

    // save preferred size(s)
    m_lItemSize = sizeOfItem;
    return UMC_OK;

} // Status H264CoeffsBuffer::Init(MediaReceiverParams *init)

bool H264CoeffsBuffer::IsInputAvailable() const
{
    size_t lFreeSize;

    // check error(s)
    if (NULL == m_pbFree)
        return false;

    // get free size
    if (m_pbFree >= m_pbBuffer + (m_lBufferSize - m_lFreeSize))
    {
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
    }
    else
    {
        lFreeSize = m_lFreeSize;
        if (lFreeSize < m_lItemSize + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo))
            return false;
    }

    // check free size
    if (lFreeSize < m_lItemSize + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo))
    {
        // when used data is present,
        // concatenate dummy bytes to last buffer info
        if (m_pBuffers)
        {
            return (m_lFreeSize - lFreeSize > m_lItemSize + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo));
        }
        // when no used data,
        // simple move pointer(s)
        else
        {
            return (m_lFreeSize == m_lBufferSize);
        }
    }

    return true;
} // bool H264CoeffsBuffer::IsInputAvailable() const

Ipp8u* H264CoeffsBuffer::LockInputBuffer()
{
    size_t lFreeSize;
    //size_t size = m_lItemSize;

    // check error(s)
    if (NULL == m_pbFree)
        return 0;

    if (m_pBuffers == 0)
    {  // We could do it, because only one thread could get input buffer
        m_pbFree = m_pbBuffer;
        m_lFreeSize = m_lBufferSize;
    }

    // get free size
    if (m_pbFree >= m_pbBuffer + (m_lBufferSize - m_lFreeSize))
    {
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
    }
    else
    {
        lFreeSize = m_lFreeSize;
        if (lFreeSize < m_lItemSize + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo))
            return 0;
    }

    // check free size
    if (lFreeSize < m_lItemSize + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo))
    {
        // when used data is present,
        // concatenate dummy bytes to last buffer info
        if (m_pBuffers)
        {
            BufferInfo *pTemp;

            // find last sample info
            pTemp = m_pBuffers;
            while (pTemp->m_pNext)
                pTemp = pTemp->m_pNext;
            pTemp->m_Size += lFreeSize;

            // update variable(s)
            m_pbFree = m_pbBuffer;
            m_lFreeSize -= lFreeSize;

            return LockInputBuffer();
        }
        // when no used data,
        // simple move pointer(s)
        else
        {
            if (m_lFreeSize == m_lBufferSize)
            {
                m_pbFree = m_pbBuffer;
                return m_pbFree;
            }
            return 0;
        }
    }

    // set free pointer
    //pointer = m_pbFree;
    //size = lFreeSize - COEFFS_BUFFER_ALIGN_VALUE - sizeof(BufferInfo);
    return m_pbFree;
} // bool H264CoeffsBuffer::LockInputBuffer(MediaData* in)

bool H264CoeffsBuffer::UnLockInputBuffer(size_t size)
{
    size_t lFreeSize;
    BufferInfo *pTemp;
    Ipp8u *pb;

    // check error(s)
    if (NULL == m_pbFree)
        return false;

    // when no data given
    //if (0 == size) // even for size = 0 we should add buffer
      //  return true;

    // get free size
    if (m_pbFree >= m_pbBuffer + (m_lBufferSize - m_lFreeSize))
        lFreeSize = m_pbBuffer + m_lBufferSize - m_pbFree;
    else
        lFreeSize = m_lFreeSize;

    // check free size
    if (lFreeSize < m_lItemSize)
        return false;

    // check used data
    if (size + COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo) > lFreeSize) // DEBUG : should not be !!!
    {
        VM_ASSERT(false);
        return false;
    }

    // get new buffer info
    pb = align_pointer<Ipp8u *> (m_pbFree + size, COEFFS_BUFFER_ALIGN_VALUE);
    pTemp = reinterpret_cast<BufferInfo *> (pb);

    size += COEFFS_BUFFER_ALIGN_VALUE + sizeof(BufferInfo);

    // fill buffer info
    pTemp->m_Size = size;
    pTemp->m_pPointer = m_pbFree;
    pTemp->m_pNext = 0;

    // add sample to end of queue
    if (m_pBuffers)
    {
        BufferInfo *pWork = m_pBuffers;

        while (pWork->m_pNext)
            pWork = pWork->m_pNext;

        pWork->m_pNext = pTemp;
    }
    else
        m_pBuffers = pTemp;

    // update variable(s)
    m_pbFree += size;
    if (m_pbBuffer + m_lBufferSize == m_pbFree)
        m_pbFree = m_pbBuffer;
    m_lFreeSize -= size;

    return true;
} // bool H264CoeffsBuffer::UnLockInputBuffer(size_t size)

bool H264CoeffsBuffer::IsOutputAvailable() const
{
    return (0 != m_pBuffers);
} // bool H264CoeffsBuffer::IsOutputAvailable() const

bool H264CoeffsBuffer::LockOutputBuffer(Ipp8u* &pointer, size_t &size)
{
    // check error(s)
    if (0 == m_pBuffers)
        return false;

    // set used pointer
    pointer = m_pBuffers->m_pPointer;
    size = m_pBuffers->m_Size;
    return true;
} // bool H264CoeffsBuffer::LockOutputBuffer(Ipp8u* &pointer, size_t &size)

bool H264CoeffsBuffer::UnLockOutputBuffer()
{
    // no filled data is present
    if (0 == m_pBuffers)
        return false;

    // update variable(s)
    m_lFreeSize += m_pBuffers->m_Size;
    m_pBuffers = m_pBuffers->m_pNext;

    return true;
} // bool H264CoeffsBuffer::UnLockOutputBuffer()

void H264CoeffsBuffer::Reset()
{
    // align buffer
    m_pbBuffer = align_pointer<Ipp8u *> (m_pbAllocatedBuffer, COEFFS_BUFFER_ALIGN_VALUE);

    m_pBuffers = 0;

    m_pbFree = m_pbBuffer;
    m_lFreeSize = m_lBufferSize;
} //void H264CoeffsBuffer::Reset()

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
