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

#ifndef __UMC_H264_NAL_SPL_H
#define __UMC_H264_NAL_SPL_H

#include <vector>
#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_heap.h"

namespace UMC
{

class MediaData;
class TaskSupplier;

class SwapperBase
{
public:
    virtual ~SwapperBase() {}

    virtual void SwapMemory(Ipp8u *pDestination, size_t &nDstSize, Ipp8u *pSource, size_t nSrcSize) = 0;
    virtual void SwapMemory(H264MemoryPiece * pMemDst, H264MemoryPiece * pMemSrc) = 0;

    virtual void CopyBitStream(Ipp8u *pDestination, Ipp8u *pSource, size_t &nSrcSize) = 0;
};

class StartCodeIteratorBase
{
public:

    StartCodeIteratorBase()
        : m_pSource(0)
        , m_nSourceSize(0)
        , m_pSourceBase(0)
        , m_nSourceBaseSize(0)
        , m_suggestedSize(10 * 1024)
    {
    }

    virtual ~StartCodeIteratorBase() 
    {
    }

    virtual Ipp32s Init(MediaData * pSource)
    {
        m_pSourceBase = m_pSource = (Ipp8u *) pSource->GetDataPointer();
        m_nSourceBaseSize = m_nSourceSize = pSource->GetDataSize();
        return 0;
    }

    Ipp32s GetCurrentOffset()
    {
        return (Ipp32s)(m_pSource - m_pSourceBase);
    }

    virtual void SetSuggestedSize(size_t size)
    {
        if (size > m_suggestedSize)
            m_suggestedSize = size;
    }

    virtual Ipp32s GetNext() = 0;

    virtual Ipp32s CheckNalUnitType(MediaData * pSource) = 0;
    virtual Ipp32s GetNALUnit(MediaData * pSource, MediaData * pDst) = 0;

    virtual void Reset() = 0;

protected:
    Ipp8u * m_pSource;
    size_t  m_nSourceSize;

    Ipp8u * m_pSourceBase;
    size_t  m_nSourceBaseSize;

    size_t  m_suggestedSize;
};

class NALUnitSplitter
{
public:

    NALUnitSplitter(H264_Heap * heap);

    virtual ~NALUnitSplitter();

    virtual void Init();
    virtual void Release();

    virtual Ipp32s CheckNalUnitType(MediaData * pSource);
    virtual MediaDataEx * GetNalUnits(MediaData * in);

    virtual void Reset();

    virtual void SetSuggestedSize(size_t size)
    {
        if (!m_pStartCodeIter)
            return;

        m_pStartCodeIter->SetSuggestedSize(size);
    }

    SwapperBase * GetSwapper()
    {
        return m_pSwapper;
    }

protected:

    TaskSupplier *  m_pSupplier;
    bool m_bWaitForIDR;
    H264_Heap   *   m_pHeap;
    SwapperBase *   m_pSwapper;
    StartCodeIteratorBase * m_pStartCodeIter;

    MediaDataEx m_MediaData;
    MediaDataEx::_MediaDataEx m_MediaDataEx;
};

size_t BuildNALUnit(MediaDataEx * , Ipp8u * , Ipp32s lengthSize);

class NALUnitSplitterMP4 : public NALUnitSplitter
{
public:
    NALUnitSplitterMP4(H264_Heap * heap);

    virtual void Init();

    virtual void Reset();

    virtual MediaDataEx * GetNalUnits(MediaData * in);

    virtual Ipp32s CheckNalUnitType(MediaData * pSource);

private:
    void ReadHeader(MediaData * pSource);

    struct AVCRecord
    {
        AVCRecord()
        {
            configurationVersion = 1;
            lengthSizeMinusOne = 1;
            numOfSequenceParameterSets = 0;
            numOfPictureParameterSets = 0;
        }

        Ipp8u configurationVersion;
        Ipp8u AVCProfileIndication;
        Ipp8u profile_compatibility;
        Ipp8u AVCLevelIndication;
        Ipp8u lengthSizeMinusOne;
        Ipp8u numOfSequenceParameterSets;
        Ipp8u numOfPictureParameterSets;

    };

    enum
    {
        D_START_CODE_LENGHT = 4,
        D_BYTES_FOR_HEADER_LENGHT = 2
    };

    AVCRecord avcRecord;
    bool m_isHeaderReaded;

    friend size_t UMC::BuildNALUnit(MediaDataEx *, Ipp8u *, Ipp32s );
};

} // namespace UMC

#endif // __UMC_H264_NAL_SPL_H
