/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MEDIA_DATA_H__
#define __UMC_MEDIA_DATA_H__

#include "umc_structures.h"
#include "umc_dynamic_cast.h"

namespace UMC
{

class MediaData
{
    DYNAMIC_CAST_DECL_BASE(MediaData)

public:

    // Default constructor
    MediaData(size_t length = 0);

    // Destructor
    virtual ~MediaData();

    // Release object
    virtual Status Close(void);

    // Allocate buffer
    virtual Status Alloc(size_t length);

    // Get an address of the beginning of the buffer.
    // This pointer could not be equal to the beginning of valid data.
    virtual void* GetBufferPointer(void) { return m_pBufferPointer; }

    // Get an address of the beginning of valid data.
    // This pointer could not be equal to the beginning of the buffer.
    virtual void* GetDataPointer(void)   { return m_pDataPointer; }

    // Return size of the buffer
    virtual size_t GetBufferSize(void)   { return m_nBufferSize; }

    // Return size of valid data in the buffer
    virtual size_t GetDataSize(void)     { return m_nDataSize; }

    // Set the pointer to a buffer allocated by an user.
    // The bytes variable defines the size of the buffer.
    // Size of valid data is set to zero
    virtual Status SetBufferPointer(Ipp8u* ptr, size_t bytes);

    // Set size of valid data in the buffer.
    // Valid data is supposed to be placed from the beginning of the buffer.
    virtual Status SetDataSize(size_t bytes);

    //  Move data pointer inside and decrease or increase data size
    virtual Status MoveDataPointer(Ipp32s bytes);

    // return time stamp of media data
    virtual Ipp64f GetTime(void)         { return m_pts_start; }

    // return time stamp of media data, start and end
    virtual Status GetTime(Ipp64f& start, Ipp64f& end);

    //  Set time stamp of media data block;
    virtual Status SetTime(Ipp64f start, Ipp64f end = 0);

    // Set frame type
    inline Status SetFrameType(FrameType ft);
    // Get frame type
    inline FrameType GetFrameType(void);

    // Set Invalid state
    inline void SetInvalid(Ipp32s isInvalid) { m_isInvalid = isInvalid; }
    // Get Invalid state
    inline Ipp32s GetInvalid(void)           { return m_isInvalid; }

    //  Set data pointer to beginning of buffer and data size to zero
    virtual Status Reset();

    MediaData& operator=(MediaData&);

    //  Move data to another MediaData
    virtual Status MoveDataTo(MediaData* dst);

protected:

    Ipp64f m_pts_start;        // (Ipp64f) start media PTS
    Ipp64f m_pts_end;          // (Ipp64f) finish media PTS
    size_t m_nBufferSize;      // (size_t) size of buffer
    size_t m_nDataSize;        // (size_t) quantity of data in buffer

    Ipp8u* m_pBufferPointer;
    Ipp8u* m_pDataPointer;

    FrameType m_FrameType;     // type of the frame
    Ipp32s    m_isInvalid;     // data is invalid when set

    // Actually this variable should has type bool.
    // But some compilers generate poor executable code.
    // On count of this, we use type Ipp32u.
    Ipp32u m_bMemoryAllocated; // (Ipp32u) is memory owned by object
};


inline Status MediaData::SetFrameType(FrameType ft)
{
    // check error(s)
    if ((ft < NONE_PICTURE) || (ft > D_PICTURE))
        return UMC_ERR_INVALID_STREAM;

    m_FrameType = ft;

    return UMC_OK;
} // VideoData::SetFrameType()


inline FrameType MediaData::GetFrameType(void)
{
    return m_FrameType;
} // VideoData::GetFrameType()

} // namespace UMC

#endif /* __UMC_MEDIA_DATA_H__ */
