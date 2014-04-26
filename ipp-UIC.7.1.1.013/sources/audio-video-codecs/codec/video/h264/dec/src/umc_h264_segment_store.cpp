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

#include "umc_h264_dec_defs_dec.h"
#include "umc_structures.h"

namespace UMC
{

H264DecoderLocalMacroblockDescriptor::H264DecoderLocalMacroblockDescriptor(void)
{
    MVDeltas[0] =
    MVDeltas[1] = NULL;
    MacroblockCoeffsInfo = NULL;
    mbs = NULL;
    active_next_mb_table = NULL;

    m_pAllocated = NULL;
    m_nAllocatedSize = 0;

    m_midAllocated = 0;
    m_pMemoryAllocator = NULL;

    m_isBusy = false;
    m_pFrame = 0;

} // H264DecoderLocalMacroblockDescriptor::H264DecoderLocalMacroblockDescriptor(void)

H264DecoderLocalMacroblockDescriptor::~H264DecoderLocalMacroblockDescriptor(void)
{
    Release();

} // H264DecoderLocalMacroblockDescriptor::~H264DecoderLocalMacroblockDescriptor(void)

void H264DecoderLocalMacroblockDescriptor::Release(void)
{
    if (m_midAllocated)
    {
        m_pMemoryAllocator->Unlock(m_midAllocated);
        m_pMemoryAllocator->Free(m_midAllocated);
    }

    MVDeltas[0] =
    MVDeltas[1] = NULL;
    MacroblockCoeffsInfo = NULL;
    mbs = NULL;
    active_next_mb_table = NULL;

    m_pAllocated = NULL;
    m_nAllocatedSize = 0;

    m_midAllocated = NULL;
} // void H264DecoderLocalMacroblockDescriptor::Release(void)

bool H264DecoderLocalMacroblockDescriptor::Allocate(Ipp32s iMBCount, MemoryAllocator *pMemoryAllocator)
{
    // check error(s)
    if (NULL == pMemoryAllocator)
        return false;

    // allocate buffer
    size_t nSize = (sizeof(H264DecoderMacroblockMVs) +
                    sizeof(H264DecoderMacroblockMVs) +
                    sizeof(H264DecoderMacroblockCoeffsInfo) +
                    sizeof(H264DecoderMacroblockLocalInfo)) * iMBCount + 16 * 6;

    if ((NULL == m_pAllocated) ||
        (m_nAllocatedSize < nSize))
    {
        // release object before initialization
        Release();

        m_pMemoryAllocator = pMemoryAllocator;
        if (UMC_OK != m_pMemoryAllocator->Alloc(&m_midAllocated,
                                                nSize,
                                                UMC_ALLOC_PERSISTENT))
            return false;
        m_pAllocated = (Ipp8u *) m_pMemoryAllocator->Lock(m_midAllocated);

        ippsSet_8u(0, m_pAllocated, (Ipp32s)nSize);
        m_nAllocatedSize = nSize;
    }

    // reset pointer(s)
    MVDeltas[0] = align_pointer<H264DecoderMacroblockMVs *> (m_pAllocated, ALIGN_VALUE);
    MVDeltas[1] = align_pointer<H264DecoderMacroblockMVs *> (MVDeltas[0] + iMBCount, ALIGN_VALUE);
    MacroblockCoeffsInfo = align_pointer<H264DecoderMacroblockCoeffsInfo *> (MVDeltas[1] + iMBCount, ALIGN_VALUE);
    mbs = align_pointer<H264DecoderMacroblockLocalInfo *> (MacroblockCoeffsInfo + iMBCount, ALIGN_VALUE);

    return true;

} // bool H264DecoderLocalMacroblockDescriptor::Allocate(Ipp32s iMBCount)

H264DecoderLocalMacroblockDescriptor &H264DecoderLocalMacroblockDescriptor::operator = (H264DecoderLocalMacroblockDescriptor &Desc)
{
    MVDeltas[0] = Desc.MVDeltas[0];
    MVDeltas[1] = Desc.MVDeltas[1];
    MacroblockCoeffsInfo = Desc.MacroblockCoeffsInfo;
    mbs = Desc.mbs;
    active_next_mb_table = Desc.active_next_mb_table;

    return *this;

} // H264DecoderLocalMacroblockDescriptor &H264DecoderLocalMacroblockDescriptor::operator = (H264DecoderLocalMacroblockDescriptor &Dest)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
