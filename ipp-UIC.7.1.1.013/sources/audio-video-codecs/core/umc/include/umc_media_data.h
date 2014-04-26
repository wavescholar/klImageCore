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

#include "umc_defs.h"
#include "umc_dynamic_cast.h"

namespace UMC
{

enum MediaDataFlags
{
    MDF_NOT_FULL_FRAME = 0x0001,
    MDF_NOT_FULL_UNIT  = 0x0002,
    MDF_WAS_DISPLAYED  = 0x0004,
};

class MediaData
{
public:
    DYNAMIC_CAST_DECL_BASE(MediaData)

    MediaData();
    virtual ~MediaData();

    // Allocate buffer
    Status Alloc(size_t iLength);

    // Release object
    virtual Status Close(void);

    // Reset data position and information
    virtual Status Reset(void);

    // Get an address of the beginning of the buffer.
    // This pointer could not be equal to the beginning of valid data.
    virtual void* GetBufferPointer(void) { return m_pBufferPointer; }

    // Get an address of the beginning of valid data.
    // This pointer could not be equal to the beginning of the buffer.
    virtual void* GetDataPointer(void)   { return m_pDataPointer; }

    // Return size of the buffer
    virtual size_t GetBufferSize(void) const   { return m_iBufferSize; }

    // Return size of valid data in the buffer
    virtual size_t GetDataSize(void) const     { return m_iDataSize; }

    // Set the pointer to a buffer allocated by an user.
    // The bytes variable defines the size of the buffer.
    // Size of valid data is set to zero
    virtual Status SetBufferPointer(Ipp8u* pBuffer, size_t iSize);

    // Set size of valid data in the buffer.
    // Valid data is supposed to be placed from the beginning of the buffer.
    virtual Status SetDataSize(size_t iSize);

    // Move data pointer inside and decrease or increase data size
    virtual Status MoveDataPointer(Ipp64s iBytes);

    // Returns data pointer to buffer pointer and increases data size accordingly
    virtual Status ResetDataPointer();

    // Move data to another MediaData
    virtual Status MoveDataTo(MediaData* dst);

    MediaData& operator=(const MediaData&);

public:
    FrameType  m_frameType;     // type of the frame
    Ipp64f     m_fPTSStart;     // start media PTS
    Ipp64f     m_fPTSEnd;       // finish media PTS
    Ipp32u     m_iFlags;
    Ipp32s     m_iInvalid;      // data is invalid when set

protected:
    Ipp8u* m_pBufferPointer;
    Ipp8u* m_pDataPointer;
    size_t m_iBufferSize;      // size of buffer
    size_t m_iDataSize;        // quantity of data in buffer
    bool   m_bMemoryAllocated; // is memory owned by object
};

class MediaDataEx : public MediaData
{
public:
    DYNAMIC_CAST_DECL(MediaDataEx, MediaData)

    class _MediaDataEx
    {
    public:
        DYNAMIC_CAST_DECL_BASE(_MediaDataEx)

        Ipp32u count;
        Ipp32u index;
        Ipp64u bstrm_pos;
        Ipp32u *offsets;
        Ipp32u *values;
        Ipp32u limit;

        _MediaDataEx()
        {
            count = 0;
            index = 0;
            bstrm_pos = 0;
            limit   = 2000;
            offsets = (Ipp32u*)malloc(sizeof(Ipp32u)*limit);
            values  = (Ipp32u*)malloc(sizeof(Ipp32u)*limit);
        }

        virtual ~_MediaDataEx()
        {
            if(offsets)
            {
                free(offsets);
                offsets = 0;
            }
            if(values)
            {
                free(values);
                values = 0;
            }
            limit   = 0;
        }
    };

    MediaDataEx(void)
    {
        m_exData = NULL;
    };

    _MediaDataEx* GetExData(void)
    {
        return m_exData;
    };

    void SetExData(_MediaDataEx* pDataEx)
    {
        m_exData = pDataEx;
    };

protected:
    _MediaDataEx* m_exData;
};

} // namespace UMC

#endif /* __UMC_MEDIA_DATA_H__ */
