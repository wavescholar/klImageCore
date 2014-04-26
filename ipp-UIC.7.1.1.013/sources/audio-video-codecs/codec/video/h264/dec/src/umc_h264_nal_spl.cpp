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
#include "umc_h264_nal_spl.h"
#include "umc_automatic_mutex.h"
#include "umc_h264_bitstream.h"/* DEBUG: extra header */
#include "umc_h264_slice_decoding.h"
#include "umc_h264_task_supplier.h"

#include <vector>

namespace UMC
{

inline
bool IsItAllowedCode(Ipp32s iCode)
{
    if (((NAL_UT_SLICE <= (iCode & NAL_UNITTYPE_BITS)) &&
        (NAL_UT_PPS >= (iCode & NAL_UNITTYPE_BITS)) &&
        (NAL_UT_SEI != (iCode & NAL_UNITTYPE_BITS))) ||
        (NAL_UT_SPS_EX == (iCode & NAL_UNITTYPE_BITS)) ||
        (NAL_UT_AUXILIARY == (iCode & NAL_UNITTYPE_BITS)) )
        return true;

    return false;

} // bool IsItAllowedCode(Ipp32s iCode)

inline
bool IsHeaderCode(Ipp32s iCode)
{
    return (NAL_UT_SPS == (iCode & NAL_UNITTYPE_BITS)) ||
           (NAL_UT_SPS_EX == (iCode & NAL_UNITTYPE_BITS)) ||
           (NAL_UT_PPS == (iCode & NAL_UNITTYPE_BITS));
}

inline
bool IsVLCCode(Ipp32s iCode)
{
    return ((NAL_UT_SLICE <= (iCode & NAL_UNITTYPE_BITS)) &&
           (NAL_UT_IDR_SLICE >= (iCode & NAL_UNITTYPE_BITS))) ||
           (NAL_UT_AUXILIARY == (iCode & NAL_UNITTYPE_BITS));
}

static Ipp32s FindStartCode(Ipp8u * (&pb), size_t &nSize)
{
    // there is no data
    if ((Ipp32s) nSize < 4)
        return 0;

    // find start code
    while ((4 <= nSize) && ((0 != pb[0]) ||
                            (0 != pb[1]) ||
                            (1 != pb[2])))
    {
        pb += 1;
        nSize -= 1;
    }

    if (4 <= nSize)
        return ((pb[0] << 24) | (pb[1] << 16) | (pb[2] << 8) | (pb[3]));

    return 0;

} // Ipp32s FindStartCode(Ipp8u * (&pb), size_t &nSize)

class StartCodeIterator : public StartCodeIteratorBase
{
public:

    virtual Ipp32s Init(MediaData * pSource)
    {
        StartCodeIteratorBase::Init(pSource);
        Ipp32s iCode = FindStartCode(m_pSource, m_nSourceSize);
        return iCode;
    }

    virtual Ipp32s GetNext()
    {
        m_pSource += 3;
        m_nSourceSize -= 3;
        Ipp32s iCode = FindStartCode(m_pSource, m_nSourceSize);

        return iCode;
    }

    virtual Ipp32s CheckNalUnitType(MediaData *pSource)
    {
        if (!pSource)
            return 0;

        return Init(pSource) & NAL_UNITTYPE_BITS; // find first start code
    }

    virtual Ipp32s GetNALUnit(MediaData * pSource, MediaData * pDst)
    {
        if (!pSource)
            return 0;

        StartCodeIteratorBase::Init(pSource);
        Ipp32s iCode = FindStartCode(m_pSource, m_nSourceSize);
        if (iCode == 0)
        {
            pSource->MoveDataPointer((Ipp32s)m_nSourceBaseSize);
            return 0;
        }

        Ipp32s firstOffset = GetCurrentOffset();
        Ipp32s iCodeNext = GetNext();

        if (iCodeNext)
            m_nSourceBaseSize = GetCurrentOffset() - firstOffset;
        else
            m_nSourceBaseSize -= firstOffset;

        pDst->SetBufferPointer(m_pSourceBase + firstOffset + 3, m_nSourceBaseSize);
        pDst->SetDataSize(m_nSourceBaseSize - 3);
        pDst->m_fPTSStart = pSource->m_fPTSStart;
        pSource->MoveDataPointer(m_nSourceBaseSize + firstOffset);
        return iCode & NAL_UNITTYPE_BITS;
    }
};

class StartCodeIterator1 : public StartCodeIteratorBase
{
public:

    StartCodeIterator1()
        : m_code(0)
        , m_pts(-1)
    {
        Reset();
    }

    virtual void Reset()
    {
        m_code = 0;
        m_pts = -1;
        m_prev.clear();
    }

    virtual Ipp32s Init(MediaData * pSource)
    {
        Reset();
        StartCodeIteratorBase::Init(pSource);
        Ipp32s iCode = UMC::FindStartCode(m_pSource, m_nSourceSize);
        return iCode;
    }

    virtual Ipp32s GetNext()
    {
        m_pSource += 3;
        m_nSourceSize -= 3;
        Ipp32s iCode = UMC::FindStartCode(m_pSource, m_nSourceSize);

        return iCode;
    }

    virtual Ipp32s CheckNalUnitType(MediaData * pSource)
    {
        if (!pSource)
            return 0;

        if (!m_code)
            m_prev.clear();

        Ipp8u * source = (Ipp8u *)pSource->GetDataPointer();
        size_t  size = pSource->GetDataSize();

        Ipp32s startCodeSize;
        Ipp32s iCodeNext = FindStartCode(source, size, startCodeSize);
        return iCodeNext;
    }

    virtual Ipp32s GetNALUnit(MediaData * pSource, MediaData * pDst)
    {
        if (!pSource)
            return EndOfStream(pDst);

        if (!m_code)
            m_prev.clear();

        Ipp8u * source = (Ipp8u *)pSource->GetDataPointer();
        size_t  size = pSource->GetDataSize();

        if (!size)
            return 0;

        Ipp32s startCodeSize;

        Ipp32s iCodeNext = FindStartCode(source, size, startCodeSize);

        if (m_prev.size())
        {
            if (!iCodeNext)
            {
                size_t sz = source - (Ipp8u *)pSource->GetDataPointer();
                if (m_prev.size() + sz >  m_suggestedSize)
                {
                    m_prev.clear();
                    sz = IPP_MIN(sz, m_suggestedSize);
                }

                m_prev.insert(m_prev.end(), (Ipp8u *)pSource->GetDataPointer(), (Ipp8u *)pSource->GetDataPointer() + sz);
                pSource->MoveDataPointer((Ipp32s)sz);
                return 0;
            }

            source -= startCodeSize;
            m_prev.insert(m_prev.end(), (Ipp8u *)pSource->GetDataPointer(), source);
            pSource->MoveDataPointer((Ipp32s)(source - (Ipp8u *)pSource->GetDataPointer()));

            pDst->SetBufferPointer(&(m_prev[0]), m_prev.size());
            pDst->SetDataSize(m_prev.size());
            pDst->m_fPTSStart = m_pts;
            Ipp32s code = m_code;
            m_code = 0;
            m_pts = -1;

            return code;
        }

        if (!iCodeNext)
        {
            pSource->MoveDataPointer((Ipp32s)(source - (Ipp8u *)pSource->GetDataPointer()));
            return 0;
        }

        m_pts = pSource->m_fPTSStart;
        m_code = iCodeNext;

        // move before start code
        pSource->MoveDataPointer((Ipp32s)(source - (Ipp8u *)pSource->GetDataPointer() - startCodeSize));

        Ipp32s startCodeSize1;
        iCodeNext = FindStartCode(source, size, startCodeSize1);

        pSource->MoveDataPointer(startCodeSize);

        Ipp32u flags = pSource->m_iFlags;

        if (!iCodeNext && !(flags & MDF_NOT_FULL_UNIT))
        {
            iCodeNext = 1;
            startCodeSize1 = 0;
        }

        if (!iCodeNext)
        {
            if (m_code == NAL_UT_SPS)
            {
                pSource->MoveDataPointer(-startCodeSize); // leave start code for SPS
                return 0;
            }

            VM_ASSERT(!m_prev.size());
            size_t sz = source - (Ipp8u *)pSource->GetDataPointer();
            if (sz >  m_suggestedSize)
            {
                sz = m_suggestedSize;
            }

            m_prev.insert(m_prev.end(), (Ipp8u *)pSource->GetDataPointer(), (Ipp8u *)pSource->GetDataPointer() + sz);
            pSource->MoveDataPointer((Ipp32s)sz);
            return 0;
        }

        // fill
        size_t nal_size = source - (Ipp8u *)pSource->GetDataPointer() - startCodeSize1;
        pDst->SetBufferPointer((Ipp8u*)pSource->GetDataPointer(), nal_size);
        pDst->SetDataSize(nal_size);
        pSource->MoveDataPointer((Ipp32s)nal_size);

        Ipp32s code = m_code;
        m_code = 0;

        pDst->m_fPTSStart = m_pts;
        m_pts = -1;
        return code;
    }

    Ipp32s EndOfStream(MediaData * pDst)
    {
        if (!m_code)
        {
            m_prev.clear();
            return 0;
        }

        if (m_prev.size())
        {
            pDst->SetBufferPointer(&(m_prev[0]), m_prev.size());
            pDst->SetDataSize(m_prev.size());
            pDst->m_fPTSStart = m_pts;
            Ipp32s code = m_code;
            m_code = 0;
            m_pts = -1;
            return code;
        }

        m_code = 0;
        return 0;
    }

private:
    std::vector<Ipp8u>  m_prev;
    Ipp32s   m_code;
    Ipp64f   m_pts;

    Ipp32s FindStartCode(Ipp8u * (&pb), size_t & size, Ipp32s & startCodeSize)
    {
        Ipp32u zeroCount = 0;

        for (Ipp32u i = 0 ; i < (Ipp32u)size; i++, pb++)
        {
            switch(pb[0])
            {
            case 0x00:
                zeroCount++;
                break;
            case 0x01:
                if (zeroCount >= 2)
                {
                    startCodeSize = IPP_MIN(zeroCount + 1, 4);
                    size -= i + 1;
                    pb++; // remove 0x01 symbol
                    zeroCount = 0;
                    if (size >= 1)
                    {
                        return pb[0] & NAL_UNITTYPE_BITS;
                    }
                    else
                    {
                        pb -= startCodeSize;
                        size += startCodeSize;
                        startCodeSize = 0;
                        return 0;
                    }
                }
                zeroCount = 0;
                break;
            default:
                zeroCount = 0;
                break;
            }
        }

        zeroCount = IPP_MIN(zeroCount, 3);
        pb -= zeroCount;
        size += zeroCount;
        zeroCount = 0;
        startCodeSize = 0;
        return 0;
    }
};

class Swapper : public SwapperBase
{
public:

    virtual void SwapMemory(Ipp8u *pDestination, size_t &nDstSize, Ipp8u *pSource, size_t nSrcSize)
    {
        SwapMemoryAndRemovePreventingBytes(pDestination, nDstSize, pSource, nSrcSize);
    }

    virtual void SwapMemory(H264MemoryPiece * pMemDst, H264MemoryPiece * pMemSrc)
    {
        size_t dstSize = pMemSrc->GetDataSize();
        /*if (IsVLCCode(pMediaDataEx->values[0]))
        {
            size_t small_size = IPP_MIN(1024, pMemSrc->GetDataSize());
            swapper->SwapMemory(pMemDst->GetPointer(),
                        dstSize,
                        (Ipp8u*)pMemSrc->GetPointer(),
                        small_size);
        }
        else*/
        {
            SwapMemory(pMemDst->GetPointer(),
                        dstSize,
                        pMemSrc->GetPointer(),
                        pMemSrc->GetDataSize());

            VM_ASSERT(pMemDst->GetSize() >= dstSize);
            size_t tail_size = IPP_MIN(pMemDst->GetSize() - dstSize, 8);
            memset(pMemDst->GetPointer() + dstSize, 0, tail_size);
            pMemDst->SetDataSize(dstSize);
            pMemDst->SetTime(pMemSrc->GetTime());
        }
    }

    virtual void CopyBitStream(Ipp8u *pDestination, Ipp8u *pSource, size_t &nSrcSize)
    {
        memcpy(pDestination, pSource, nSrcSize);
    }
};

/*************************************************************************************/
// MP4 stuff
/*************************************************************************************/

static inline Ipp32s GetValue16(Ipp8u * buf)
{
    return ((*buf) << 8) | *(buf + 1);
}

static inline Ipp32s GetValue24(Ipp8u * buf)
{
    return ((*buf) << 16) | (*(buf + 1) << 8) | *(buf + 2);
}

static inline Ipp32s GetValue32(Ipp8u * buf)
{
    return ((*buf) << 24) | (*(buf + 1) << 16) | (*(buf + 2) << 8) | *(buf + 3);
}

static inline Ipp32s GetLenght(Ipp32s len_bytes_count, Ipp8u * buf)
{
    Ipp32s length = 0;

    switch (len_bytes_count)
    {
    case 1: // 1 byte
        length = *buf;
        break;
    case 2: // 2 bytes
        length = GetValue16(buf);
        break;
    case 3: // 3 bytes
        length = GetValue24(buf);
        break;
    case 4:  // 4 bytes
        length = GetValue32(buf);
        break;
    }

    return length;

} // Ipp32s GetLenght(Ipp32s len_bytes_count, Ipp8u * buf)


NALUnitSplitter::NALUnitSplitter(H264_Heap * heap)
    : m_pSupplier(0)
    , m_bWaitForIDR(true)
    , m_pHeap(heap)
    , m_pSwapper(0)
    , m_pStartCodeIter(0)
{
    m_MediaData.SetExData(&m_MediaDataEx);
}

NALUnitSplitter::~NALUnitSplitter()
{
    Release();
}

void NALUnitSplitter::Init()
{
    Release();

    m_bWaitForIDR = true;

    m_pSwapper = new Swapper();
    m_pStartCodeIter = new StartCodeIterator1();
}

void NALUnitSplitter::Reset()
{
    m_bWaitForIDR = true;
    if (m_pStartCodeIter)
    {
        m_pStartCodeIter->Reset();
    }
}

void NALUnitSplitter::Release()
{
    delete m_pSwapper;
    m_pSwapper = 0;
    delete m_pStartCodeIter;
    m_pStartCodeIter = 0;
}

Ipp32s NALUnitSplitter::CheckNalUnitType(MediaData * pSource)
{
    return m_pStartCodeIter->CheckNalUnitType(pSource); // find first start code
}

MediaDataEx * NALUnitSplitter::GetNalUnits(MediaData * pSource)
{
    MediaDataEx * out = &m_MediaData;
    MediaDataEx::_MediaDataEx* pMediaDataEx = &m_MediaDataEx;

    Ipp32s iCode = m_pStartCodeIter->GetNALUnit(pSource, out);

    if (iCode == 0)
    {
        pMediaDataEx->count = 0;
        return 0;
    }

    pMediaDataEx->values[0] = iCode;

    pMediaDataEx->offsets[0] = 0;
    pMediaDataEx->offsets[1] = (Ipp32s)out->GetDataSize();
    pMediaDataEx->count = 1;
    pMediaDataEx->index = 0;
    return out;
}

size_t BuildNALUnit(MediaDataEx * mediaData, Ipp8u * buf, Ipp32s lengthSize)
{
    MediaDataEx::_MediaDataEx* pMediaEx = mediaData->GetExData();

    size_t len = GetLenght(lengthSize, buf);
    buf += lengthSize;

    Ipp32u dstSize = (Ipp32u)len;// + NALUnitSplitterMP4::D_START_CODE_LENGHT;

    if (mediaData->GetBufferSize() < dstSize)
    {
        mediaData->Alloc(dstSize);
    }

    Ipp8u * write_buf = (Ipp8u*)mediaData->GetDataPointer() + mediaData->GetDataSize();

    memcpy(write_buf, buf, len);

    pMediaEx->values[pMediaEx->count] = (*write_buf) & NAL_UNITTYPE_BITS;
    pMediaEx->count++;
    pMediaEx->offsets[pMediaEx->count] = pMediaEx->offsets[pMediaEx->count - 1] + dstSize;
    mediaData->SetDataSize(mediaData->GetDataSize() + dstSize);
    return (len + lengthSize);
}

NALUnitSplitterMP4::NALUnitSplitterMP4(H264_Heap * heap)
    : NALUnitSplitter(heap)
    , m_isHeaderReaded(false)
{
}

void NALUnitSplitterMP4::Init()
{
    Release();
    m_bWaitForIDR = true;
    m_isHeaderReaded = false;
    m_pSwapper = new Swapper();
}

void NALUnitSplitterMP4::Reset()
{
    NALUnitSplitter::Reset();
}

Ipp32s NALUnitSplitterMP4::CheckNalUnitType(MediaData * pSource)
{
    if (!pSource)
        return 0;

    if (m_isHeaderReaded)
    {
        return *((Ipp8u*)pSource->GetDataPointer() + (avcRecord.lengthSizeMinusOne + 1)) & NAL_UNITTYPE_BITS;
    }

    return NAL_UT_SPS;
}

void NALUnitSplitterMP4::ReadHeader(MediaData * pSource)
{
    Ipp8u *p = (Ipp8u *) pSource->GetDataPointer();
    avcRecord.configurationVersion = *p;

    //VM_ASSERT(avc_record.configurationVersion != 1);

    p++;
    avcRecord.AVCProfileIndication = *p;
    p++;
    avcRecord.profile_compatibility = *p;
    p++;
    avcRecord.AVCLevelIndication = *p;
    p++;
    avcRecord.lengthSizeMinusOne = *p;
    avcRecord.lengthSizeMinusOne <<= 6;
    avcRecord.lengthSizeMinusOne >>= 6; // remove reserved bits
    p++;

    if (avcRecord.lengthSizeMinusOne > 4)
    {
        throw h264_exception(UMC_ERR_INVALID_STREAM);
    }

    avcRecord.numOfSequenceParameterSets = *p;
    avcRecord.numOfSequenceParameterSets <<= 3;
    avcRecord.numOfSequenceParameterSets >>= 3;// remove reserved bits
    p++;

    // calculate length of memory
    // read sequence par sets
    Ipp8u * temp = p;
    Ipp32s i;
    size_t result_length = 0;

    for (i = 0; i < avcRecord.numOfSequenceParameterSets; i++)
    {
        Ipp32s length = GetValue16(temp);
        temp += length + D_BYTES_FOR_HEADER_LENGHT;
        result_length += length + D_START_CODE_LENGHT + 3; // +3 - for ALIGNMENT
    }

    if (result_length > pSource->GetDataSize() + avcRecord.numOfSequenceParameterSets*3 + 3)
    {
        pSource->SetDataSize(0);
        throw h264_exception(UMC_ERR_INVALID_STREAM);
    }

    avcRecord.numOfPictureParameterSets = *temp;
    temp++;

    for (i = 0; i < avcRecord.numOfPictureParameterSets; i++)
    {
        Ipp32s length = GetValue16(temp);
        temp += length + D_BYTES_FOR_HEADER_LENGHT;
        result_length += length + D_START_CODE_LENGHT + 3; // +3 - for ALIGNMENT
    }

    result_length += 3; // +3 - for ALIGNMENT

    if (!result_length && (result_length > pSource->GetDataSize() +
        (avcRecord.numOfSequenceParameterSets + avcRecord.numOfPictureParameterSets + 3)*3))
    {
        pSource->SetDataSize(0);
        throw h264_exception(UMC_ERR_INVALID_STREAM);
    }

    if (m_MediaData.GetBufferSize() < result_length)
    {
        m_MediaData.Alloc(result_length);
    }

    // read sequence par sets
    for (i = 0; i < avcRecord.numOfSequenceParameterSets; i++)
    {
        size_t length = BuildNALUnit(&m_MediaData, p, D_BYTES_FOR_HEADER_LENGHT);
        p += length;
    }

    avcRecord.numOfPictureParameterSets = *p;
    p++;

    // read picture par sets
    for (i = 0; i < avcRecord.numOfPictureParameterSets; i++)
    {
        size_t length = BuildNALUnit(&m_MediaData, p, D_BYTES_FOR_HEADER_LENGHT);
        p += length;
    }

    pSource->SetDataSize(0);

    m_isHeaderReaded = true;
}

MediaDataEx * NALUnitSplitterMP4::GetNalUnits(MediaData * pSource)
{
    MediaDataEx * out = &m_MediaData;
    MediaDataEx::_MediaDataEx* pMediaDataEx = &m_MediaDataEx;

    m_MediaData.SetDataSize(0);
    m_MediaDataEx.count = 0;
    pMediaDataEx->offsets[0] = 0;
    pMediaDataEx->values[0] = 0;
    pMediaDataEx->index = 0;

    pMediaDataEx->count = 0;
    pMediaDataEx->index = 0;
    out->SetDataSize(0);

    if (!pSource)
        return 0;

    if (!m_isHeaderReaded)
    {
        ReadHeader(pSource);
        return &m_MediaData;
    }

    if (pSource->GetDataSize() < (size_t)(avcRecord.lengthSizeMinusOne + 1))
    {
        return 0;
    }

    size_t length = BuildNALUnit(&m_MediaData, (Ipp8u*)pSource->GetDataPointer(), avcRecord.lengthSizeMinusOne + 1);
    pSource->MoveDataPointer((Ipp32s)length);
    return &m_MediaData;
}

/* temporal class definition */
class H264DwordPointer_
{
public:
    // Default constructor
    H264DwordPointer_(void)
    {
        m_pDest = NULL;
        m_nByteNum = 0;
    }

    H264DwordPointer_ operator = (void *pDest)
    {
        m_pDest = (Ipp32u *) pDest;
        m_nByteNum = 0;
        m_iCur = 0;

        return *this;
    }

    // Increment operator
    H264DwordPointer_ &operator ++ (void)
    {
        if (4 == ++m_nByteNum)
        {
            *m_pDest = m_iCur;
            m_pDest += 1;
            m_nByteNum = 0;
            m_iCur = 0;
        }
        else
            m_iCur <<= 8;

        return *this;
    }

    Ipp8u operator = (Ipp8u nByte)
    {
        m_iCur = (m_iCur & ~0x0ff) | ((Ipp32u) nByte);

        return nByte;
    }

protected:
    Ipp32u *m_pDest;                                            // (Ipp32u *) pointer to destination buffer
    Ipp32u m_nByteNum;                                          // (Ipp32u) number of current byte in dword
    Ipp32u m_iCur;                                              // (Ipp32u) current dword
};

class H264SourcePointer_
{
public:
    // Default constructor
    H264SourcePointer_(void)
    {
        m_pSource = NULL;
    }

    H264SourcePointer_ &operator = (void *pSource)
    {
        m_pSource = (Ipp8u *) pSource;

        m_nZeros = 0;
        m_nRemovedBytes = 0;

        return *this;
    }

    H264SourcePointer_ &operator ++ (void)
    {
        Ipp8u bCurByte = m_pSource[0];

        if (0 == bCurByte)
            m_nZeros += 1;
        else
        {
            if ((3 == bCurByte) && (2 <= m_nZeros))
                m_nRemovedBytes += 1;
            m_nZeros = 0;
        }

        m_pSource += 1;

        return *this;
    }

    bool IsPrevent(void)
    {
        if ((3 == m_pSource[0]) && (2 <= m_nZeros))
            return true;
        else
            return false;
    }

    operator Ipp8u (void)
    {
        return m_pSource[0];
    }

    Ipp32u GetRemovedBytes(void)
    {
        return m_nRemovedBytes;
    }

protected:
    Ipp8u *m_pSource;                                           // (Ipp8u *) pointer to destination buffer
    Ipp32u m_nZeros;                                            // (Ipp32u) number of preceding zeros
    Ipp32u m_nRemovedBytes;                                     // (Ipp32u) number of removed bytes
};

void SwapMemoryAndRemovePreventingBytes(void *pDestination, size_t &nDstSize, void *pSource, size_t nSrcSize)
{
    H264DwordPointer_ pDst;
    H264SourcePointer_ pSrc;
    size_t i;

    // DwordPointer object is swapping written bytes
    // H264SourcePointer_ removes preventing start-code bytes

    // reset pointer(s)
    pSrc = pSource;
    pDst = pDestination;

    // first two bytes
    i = 0;
    while (i < (Ipp32u) IPP_MIN(2, nSrcSize))
    {
        pDst = (Ipp8u) pSrc;
        ++pDst;
        ++pSrc;
        ++i;
    }

    // do swapping
    while (i < (Ipp32u) nSrcSize)
    {
        if (false == pSrc.IsPrevent())
        {
            pDst = (Ipp8u) pSrc;
            ++pDst;
        }
        ++pSrc;
        ++i;
    }

    // write padding bytes
    nDstSize = nSrcSize - pSrc.GetRemovedBytes();
    while (nDstSize & 3)
    {
        pDst = (Ipp8u) (0);
        ++nDstSize;
        ++pDst;
    }

} // void SwapMemoryAndRemovePreventingBytes(void *pDst, size_t &nDstSize, void *pSrc, size_t nSrcSize)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
