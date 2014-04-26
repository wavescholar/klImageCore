/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_AVI_SPLITTER

#include <memory.h>
#include "vm_debug.h"
#include "umc_avi_spl_chunk.h"


#define MAP_SIZE (128)  //  Map section size in pages
#define AVI_FOURCC_FILE uimcFOURCC( 'F', 'I', 'L', 'E' )

using namespace UMC;


inline Ipp64u AlignTo2(const Ipp64u ullChunkSize)
{
    return (ullChunkSize + 1) & ~0x1;
}

AVIChunk::AVIChunk()
{
    m_uiPos = 0;
    m_ulStackPos = 0;
    m_ChnkStack[0].m_chnkName = AVI_FOURCC_FILE;
}

Status AVIChunk::Init(DataReader *pDataReader, Mutex *pMutex)
{
    m_uiPos = 0;
    m_ulStackPos = 0;
    //  Zero indexed m_ChnkStack entry corresponds file itself
    m_ChnkStack[0].m_chnkName = AVI_FOURCC_FILE;
    m_ChnkStack[0].m_stSize = 0;
    m_ChnkStack[0].m_stFilePos = 0;

    m_pReader = pDataReader;
    m_pReaderMutex = pMutex;
    m_ChnkStack[0].m_stSize = m_pReader->GetSize();
    return UMC_OK;
}

Status AVIChunk::DescendLIST(tFOURCC chnkName)
{
    return DescendChunkList(AVI_FOURCC_LIST, chnkName);
}

Status AVIChunk::DescendRIFF(tFOURCC chnkName)
{
    return DescendChunkList(AVI_FOURCC_RIFF, chnkName);
}

void AVIChunk::GoChunkHead()
{
    m_uiPos = m_ChnkStack[m_ulStackPos].m_stFilePos;
}

Ipp32u AVIChunk::GetChunkHead()
{
    return (Ipp32u)m_ChnkStack[m_ulStackPos].m_stFilePos;
}

tFOURCC AVIChunk::GetChunkFOURCC()
{
    return m_ChnkStack[m_ulStackPos].m_chnkName;
}

Ipp32u AVIChunk::GetChunkSize()
{
    Ipp64u stSize = m_ChnkStack[m_ulStackPos].m_stSize;
    VM_ASSERT(stSize == (Ipp32u)stSize);
    return (Ipp32u)stSize;
}

bool AVIChunk::CmpChunkNames(const tFOURCC chnkName1, const tFOURCC chnkName2)
{
    if (chnkName1 == chnkName2)
        return true;

    if((AVI_FOURCC_DB == (chnkName1 & 0xFFFF0000) || AVI_FOURCC_DC == (chnkName1 & 0xFFFF0000)) &&
       (AVI_FOURCC_DB == (chnkName2 & 0xFFFF0000) || AVI_FOURCC_DC == (chnkName2 & 0xFFFF0000)) &&
       ((chnkName1 & 0x0000FFFF) == (chnkName2 & 0x0000FFFF)))
        return true;

    return false;
}

Status AVIChunk::DescendChunk(tFOURCC chnkName)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(MAX_AVI_CHUNK_DEPTH - 1 > m_ulStackPos);

    CChnkInfo& rCurChunk = m_ChnkStack[m_ulStackPos];

    if (rCurChunk.m_stFilePos + AlignTo2(rCurChunk.m_stSize) == m_uiPos)
        return UMC_ERR_SYNC;

    //  Look for the chunk we need
    while (!CmpChunkNames(m_ChnkStack[m_ulStackPos + 1].m_chnkName, chnkName) &&
        m_uiPos < m_ChnkStack[m_ulStackPos].m_stFilePos + AlignTo2(m_ChnkStack[m_ulStackPos].m_stSize))
    {
        CChnkInfo& rNextChunk = m_ChnkStack[m_ulStackPos + 1];

        m_pReaderMutex->Lock();
        m_pReader->SetPosition(m_uiPos);
        umcRes = m_pReader->Get32uNoSwap(&rNextChunk.m_chnkName);
        if (UMC_OK != umcRes)
        {
            m_pReaderMutex->Unlock();
            return umcRes;
        }

        rNextChunk.m_stSize = 0;
        Ipp32u size;
        umcRes = m_pReader->Get32uNoSwap(&size);
        rNextChunk.m_stSize = size;
        rNextChunk.m_stFilePos = m_pReader->GetPosition();

        m_uiPos = m_pReader->GetPosition();
        m_pReaderMutex->Unlock();

        if (AVI_FOURCC_ANY_ == chnkName)
            break;
        else if (!CmpChunkNames(rNextChunk.m_chnkName, chnkName))
        {
            m_uiPos += AlignTo2(rNextChunk.m_stSize);
            umcRes = m_pReader->MovePosition(AlignTo2(rNextChunk.m_stSize));
        }
    }

    if (m_uiPos >= m_ChnkStack[m_ulStackPos].m_stFilePos + AlignTo2(m_ChnkStack[m_ulStackPos].m_stSize))
        return UMC_ERR_SYNC;
    else if (CmpChunkNames(m_ChnkStack[m_ulStackPos + 1].m_chnkName, chnkName) || AVI_FOURCC_ANY_== chnkName)
        m_ulStackPos++;

    return UMC_OK;
}

Status AVIChunk::DescendChunkList(tFOURCC chnkName, tFOURCC listName)
{
    Status umcRes = UMC_OK;
    VM_ASSERT(MAX_AVI_CHUNK_DEPTH - 1 > m_ulStackPos);

    for (;;)
    {
        umcRes = DescendChunk(chnkName);
        if (UMC_OK != umcRes)
            return umcRes;

        m_pReaderMutex->Lock();
        m_pReader->SetPosition(m_uiPos);

        tFOURCC fcData = 0;
        umcRes = m_pReader->Get32uNoSwap(&fcData);
        if (UMC_OK != umcRes)
        {
            m_pReaderMutex->Unlock();
            return umcRes;
        }

        m_uiPos = m_pReader->GetPosition();
        m_pReaderMutex->Unlock();

        if (CmpChunkNames(fcData, listName))
        { // we've found the list chunk we was looking for
            m_ChnkStack[m_ulStackPos].m_stFilePos += sizeof(tFOURCC);
            m_ChnkStack[m_ulStackPos].m_stSize -= sizeof(tFOURCC);
            break;
        }
        else
        {
            umcRes = Ascend();
            if (UMC_OK != umcRes)
                return umcRes;
        }
    }

    return UMC_OK;
}

Status AVIChunk::Ascend()
{
    VM_ASSERT(0 < m_ulStackPos);

    if (0 == m_ulStackPos)
        return UMC_ERR_FAILED;

    CChnkInfo& rCurChunk = m_ChnkStack[m_ulStackPos];
    m_pReader->SetPosition(rCurChunk.m_stFilePos + AlignTo2(rCurChunk.m_stSize));
    m_uiPos = rCurChunk.m_stFilePos + AlignTo2(rCurChunk.m_stSize);
    rCurChunk.m_chnkName = AVI_FOURCC_WRONG;
    m_ulStackPos--;
    return UMC_OK;
}

Status AVIChunk::GetData(Ipp8u* pbBuffer, size_t uiBufSize)
{
    Status umcRes = UMC_OK;
    CChnkInfo& rCurChunk = m_ChnkStack[m_ulStackPos];

    VM_ASSERT(rCurChunk.m_stFilePos + AlignTo2(rCurChunk.m_stSize) >= m_uiPos);

    //  Check if we have enough data
#if 0
    // There are some strange AVIs, where 'strh' chunk size does not contain rcFrame data.
    // Insufficient data comes with 'strf' chunks
    if (rCurChunk.m_stSize - (m_uiPos - rCurChunk.m_stFilePos) < uiBufSize)
#else
    uiBufSize = uiBufSize;
    if (rCurChunk.m_stSize - (m_uiPos - rCurChunk.m_stFilePos) < rCurChunk.m_stSize)
#endif
        return UMC_ERR_NOT_ENOUGH_DATA;

    m_pReaderMutex->Lock();
    m_pReader->SetPosition(m_uiPos);

#if 0
    umcRes = m_pReader->GetData(pbBuffer, uiBufSize);
#else
    size_t chunkSize = (size_t)rCurChunk.m_stSize;
    umcRes = m_pReader->GetData(pbBuffer, chunkSize);
#endif
    if (UMC_OK != umcRes)
    {
        m_pReaderMutex->Unlock();
        return umcRes;
    }

    m_uiPos = m_pReader->GetPosition();
    m_pReaderMutex->Unlock();

    return UMC_OK;
}

Status AVIChunk::GetData(Ipp64u uiOffset, Ipp8u* pbBuffer, size_t uiBufSize)
{
    if (uiOffset + uiBufSize > m_ChnkStack[0].m_stSize)
        return UMC_ERR_NOT_ENOUGH_DATA;

    m_pReaderMutex->Lock();
    m_pReader->SetPosition(uiOffset);
    m_pReader->GetData(pbBuffer, uiBufSize);
    m_pReaderMutex->Unlock();
    return UMC_OK;
}

Status AVIChunk::JumpToFilePos(const Ipp64u uiFilePos)
{
    Status umcRes = UMC_OK;
    while ((uiFilePos < m_ChnkStack[m_ulStackPos].m_stFilePos ||
           uiFilePos > m_ChnkStack[m_ulStackPos].m_stFilePos + m_ChnkStack[m_ulStackPos].m_stSize) &&
           0 < m_ulStackPos)
        m_ulStackPos--;

    if (0 == m_ulStackPos && uiFilePos > m_ChnkStack[m_ulStackPos].m_stFilePos + m_ChnkStack[m_ulStackPos].m_stSize)
        return UMC_ERR_END_OF_STREAM;

    umcRes = m_pReader->SetPosition(uiFilePos);
    return umcRes;
}

#endif
