/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/
#include "ipps.h"
#include "umc_media_data.h"

namespace UMC
{

MediaData::MediaData(size_t length)
{
    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_nBufferSize      = 0;
    m_nDataSize        = 0;
    m_pts_start        = -1;
    m_pts_end          = 0;
    m_FrameType        = NONE_PICTURE;
    m_isInvalid        = 0;

    m_bMemoryAllocated = 0;

    if (length)
    {
        m_pBufferPointer = new Ipp8u[length];
        if (m_pBufferPointer)
        {
            m_pDataPointer     = m_pBufferPointer;
            m_nBufferSize      = length;
            m_bMemoryAllocated = 1;
        }
    }
} // MediaData::MediaData(size_t length) :

MediaData::~MediaData()
{
    Close();

} // MediaData::~MediaData()

Status MediaData::Alloc(size_t length)
{
    Close();

    if (length)
    {
        m_pBufferPointer = new Ipp8u[length];
        if (!m_pBufferPointer) {
            return UMC_ERR_ALLOC;
        }
        m_pDataPointer     = m_pBufferPointer;
        m_nBufferSize      = length;
        m_bMemoryAllocated = 1;
    }

    return UMC_OK;
}

Status MediaData::Close(void)
{
    if (m_bMemoryAllocated)
    {
        if (m_pBufferPointer)
            delete[] m_pBufferPointer;
    }

    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_nBufferSize      = 0;
    m_nDataSize        = 0;
    m_FrameType        = NONE_PICTURE;

    m_bMemoryAllocated = 0;

    return UMC_OK;

} // Status MediaData::Close(void)

Status MediaData::SetDataSize(size_t bytes)
{
    if (!m_pBufferPointer)
        return UMC_ERR_NULL_PTR;

    if (bytes > (m_nBufferSize - (m_pDataPointer - m_pBufferPointer)))
        return UMC_ERR_FAILED;

    m_nDataSize = bytes;

    return UMC_OK;

} // Status MediaData::SetDataSize(size_t bytes)

// Set the pointer to a buffer allocated by the user
// bytes define the size of buffer
// size of data is equal to buffer size after this call
Status MediaData::SetBufferPointer(Ipp8u *ptr, size_t size)
{
    // release object
    MediaData::Close();

    // set new value(s)
    m_pBufferPointer  = ptr;
    m_pDataPointer    = ptr;
    m_nBufferSize     = size;
    m_nDataSize       = 0;

    return UMC_OK;

} // Status MediaData::SetBufferPointer(Ipp8u *ptr, size_t size)

Status MediaData::SetTime(Ipp64f start, Ipp64f end)
{
 //   if (start < 0  && start != -1.0)
 //       return UMC_ERR_FAILED;

    m_pts_start = start;
    m_pts_end = end;

    return UMC_OK;

} // Status MediaData::SetTime(Ipp64f start, Ipp64f end)

Status MediaData::GetTime(Ipp64f& start, Ipp64f& end)
{
    start = m_pts_start;
    end = m_pts_end;

    return UMC_OK;

} // Status MediaData::GetTime(Ipp64f& start, Ipp64f& end)

Status MediaData::MoveDataPointer(Ipp32s bytes)
{
    if (bytes >= 0 && m_nDataSize >= (size_t)bytes) {
        m_pDataPointer   += bytes;
        m_nDataSize      -= bytes;

        return UMC_OK;
    } else if (bytes < 0 && (size_t)(m_pDataPointer - m_pBufferPointer) >= (size_t)(-bytes)) {
        m_pDataPointer   += bytes;
        m_nDataSize      -= bytes;

        return UMC_OK;
    }

    return UMC_ERR_FAILED;
} // Status MediaData::MovePointer(Ipp32s bytes)

Status MediaData::Reset()
{
    m_pDataPointer = m_pBufferPointer;
    m_nDataSize = 0;
    m_pts_start = -1.0;
    m_pts_end = -1.0;
    m_FrameType = NONE_PICTURE;
    m_isInvalid = 0;
    return UMC_OK;
}

MediaData& MediaData::operator = (MediaData& src)
{
    MediaData::Close();

    m_pts_start        = src.m_pts_start;
    m_pts_end          = src.m_pts_end;
    m_nBufferSize      = src.m_nBufferSize;
    m_nDataSize        = src.m_nDataSize;
    m_FrameType        = src.m_FrameType;
    m_isInvalid        = src.m_isInvalid;

    m_pDataPointer     = src.m_pDataPointer;
    m_pBufferPointer   = src.m_pBufferPointer;
    m_bMemoryAllocated = false;

    return *this;
} // MediaData& MediaData::operator = (MediaData& src)

Status MediaData::MoveDataTo(MediaData* dst)
{
    MediaData *src;
    Ipp8u *pDataEnd;
    Ipp8u *pBufferEnd;
    size_t size;

    // check error(s)
    if (NULL == m_pDataPointer)
    {
        return UMC_ERR_NOT_INITIALIZED;
    }
    if ((NULL == dst) ||
        (NULL == dst->m_pDataPointer))
    {
        return UMC_ERR_NULL_PTR;
    }

    // get parameters
    src = this;
    pDataEnd = dst->m_pDataPointer + dst->m_nDataSize;
    pBufferEnd = dst->m_pBufferPointer + dst->m_nBufferSize;
    size = IPP_MIN(src->m_nDataSize, (size_t) (pBufferEnd - pDataEnd));

    if (size)
    {
        ippsCopy_8u(src->m_pDataPointer, pDataEnd, size);
    }

    dst->m_nDataSize += size;
    src->MoveDataPointer(size);

    dst->m_pts_start = src->m_pts_start;
    dst->m_pts_end   = src->m_pts_end;
    dst->m_FrameType = src->m_FrameType;
    dst->m_isInvalid = src->m_isInvalid;

    return UMC_OK;

} // MediaData::MoveDataTo(MediaData& src)

} // namespace UMC
