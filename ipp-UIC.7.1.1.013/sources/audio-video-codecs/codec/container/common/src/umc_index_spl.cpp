/*////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/


#include "vm_semaphore.h"
#include "vm_thread.h"
#include "vm_types.h"
#include "vm_time.h"
#include "bstream.h"
#include "umc_media_data.h"
#include "umc_sample_buffer.h"

#include "umc_index_spl.h"

enum
{
#ifndef _WIN32_WCE
    TIME_TO_SLEEP = 5,
#else
    TIME_TO_SLEEP = 0,
#endif
};

using namespace UMC;

struct ReadESThreadParam
{
    Ipp32s        uiPin;
    IndexSplitter* pThis;
};


IndexSplitter::IndexSplitter()
{
    m_bFlagStop         = true;
    m_bNewPortion       = true;
    m_AVC_NALUlen_fld   = 0;
    m_pReadESThread     = NULL;
    m_pReader           = NULL;
    m_pTrackIndex       = NULL;
    m_ppMediaBuffer     = NULL;
    m_ppLockedFrame     = NULL;
    m_pIsLocked         = NULL;
    m_pInfo             = NULL;
    m_pDataReader       = NULL;
}

IndexSplitter::~IndexSplitter()
{
    Close();
}

Status IndexSplitter::Init(SplitterParams *pInitParams)
{
    if(!pInitParams)
        return UMC_ERR_NULL_PTR;

    m_pReader = pInitParams->m_pDataReader;
    return UMC_OK;
}

Status IndexSplitter::Close()
{
    StopSplitter();
    m_pReader = NULL;
    m_ReaderMutex.Reset();
    return UMC_OK;
}

Status IndexSplitter::StopSplitter()
{
    if (m_bFlagStop)
        return UMC_OK;

    if (m_pReader == NULL)
        return UMC_ERR_NOT_INITIALIZED;

    Status umcRes;
    IndexEntry entry;
    Ipp32u i;

    m_bFlagStop = true;
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (vm_thread_is_valid(&m_pReadESThread[i]))
        {
            vm_thread_wait(&m_pReadESThread[i]);
            vm_thread_close(&m_pReadESThread[i]);
            vm_thread_set_invalid(&m_pReadESThread[i]);
        }
        if (m_ppMediaBuffer[i])
        {
            umcRes = m_ppMediaBuffer[i]->Reset();
            UMC_CHECK_STATUS(umcRes)
        }
        if (m_ppLockedFrame[i])
        {
            umcRes = m_ppLockedFrame[i]->Reset();
            UMC_CHECK_STATUS(umcRes)
            m_pIsLocked[i] = 0;
        }
    }

    return UMC_OK;
}

Status IndexSplitter::GetInfo(SplitterInfo** ppInfo)
{
    ppInfo[0] = m_pInfo;
    return UMC_OK;
}

void IndexSplitter::ReadES(Ipp32u uiPin)
{
    Status umcRes;
    TrackIndex *pIndex = &m_pTrackIndex[uiPin];
    IndexEntry m_Frame;
    MediaData in;
    Ipp32s i;
    Ipp64u sourceSize = m_pReader->GetSize();

    if (m_pInfo->m_fRate == 0)
        return;

    umcRes = pIndex->Get(m_Frame);
    while (umcRes == UMC_OK && !m_bFlagStop)
    {
        if(!m_ppMediaBuffer[uiPin]) // empty track
            return;

        umcRes = m_ppMediaBuffer[uiPin]->LockInputBuffer(&in);
        while (umcRes == UMC_ERR_NOT_ENOUGH_BUFFER)
        {
            if (m_bFlagStop)
                return;

            vm_time_sleep(TIME_TO_SLEEP);
            umcRes = m_ppMediaBuffer[uiPin]->LockInputBuffer(&in);
        }

        if ((Ipp64u)m_Frame.stPosition + m_Frame.uiSize > sourceSize)
        {
            umcRes = UMC_ERR_END_OF_STREAM;
            break;
        }

        if ((m_pInfo->m_systemType == MP4_ATOM_STREAM) && (m_pInfo->m_ppTrackInfo[uiPin]->m_type == TRACK_H264))
        {
            if(m_bNewPortion)
            {
                Ipp8u *phead = (Ipp8u *)m_pInfo->m_ppTrackInfo[uiPin]->m_pHeader->GetDataPointer();
                Ipp32u headlen = (Ipp32u)m_pInfo->m_ppTrackInfo[uiPin]->m_pHeader->GetDataSize();
                memcpy(in.GetDataPointer(), phead, headlen);
                in.SetDataSize(headlen);
                m_bNewPortion = false;
            }

            size_t buffer_size;
            size_t fr_size = (Ipp32u)in.GetDataSize();
            Ipp8u *pDst = (Ipp8u *)in.GetDataPointer() + fr_size;
            size_t curPos = 0;

            m_ReaderMutex.Lock();
            m_pReader->SetPosition((Ipp64u)m_Frame.stPosition);
            while(curPos < m_Frame.uiSize)
            {
                size_t NALUlen = 0;
                switch(m_AVC_NALUlen_fld)
                {
                    case 1:
                    {
                        Ipp8u val8 = 0;
                        m_pReader->Get8u(&val8);
                        NALUlen = val8;
                        break;
                    }
                    case 2:
                    {
                        Ipp16u val16 = 0;
                        m_pReader->Get16uSwap(&val16);
                        NALUlen = val16;
                        break;
                    }
                    case 3:
                    {
                        Ipp8u val8 = 0;
                        m_pReader->Get8u(&val8);
                        NALUlen = val8;
                        m_pReader->Get8u(&val8);
                        NALUlen = (NALUlen << 8) | val8;
                        m_pReader->Get8u(&val8);
                        NALUlen = (NALUlen << 8) | val8;
                        break;
                    }
                    case 4:
                    {
                        Ipp32u val32 = 0;
                        m_pReader->Get32uSwap(&val32);
                        NALUlen = val32;
                        break;
                    }
                }
                pDst[0] = 0; pDst[1] = 0; pDst[2] = 1;
                pDst += 3;
                buffer_size = in.GetBufferSize() - (pDst - (Ipp8u*)in.GetBufferPointer());
                if(buffer_size < NALUlen) // check nal size vs buffer size
                    NALUlen = buffer_size;
                m_pReader->GetData(pDst, NALUlen);
                fr_size += NALUlen + 3;
                pDst += NALUlen;
                curPos += m_AVC_NALUlen_fld + NALUlen;
            }
            m_ReaderMutex.Unlock();
            in.SetDataSize(fr_size);
        }
        else
        {
            m_ReaderMutex.Lock();
            m_pReader->SetPosition((Ipp64u)m_Frame.stPosition);
            m_pReader->GetData(in.GetDataPointer(), m_Frame.uiSize);
            m_ReaderMutex.Unlock();
            in.SetDataSize(m_Frame.uiSize);
        }

        in.m_fPTSStart = m_Frame.dPts;
        in.m_fPTSEnd   = m_Frame.dDts;
        in.m_frameType = NONE_PICTURE;
        if (TRACK_ANY_VIDEO & m_pInfo->m_ppTrackInfo[uiPin]->m_type)
        {
            in.m_frameType = (FrameType)m_Frame.uiFlags;
            /*** update AVS start code ***/
            if (TRACK_AVS & m_pInfo->m_ppTrackInfo[uiPin]->m_type)
            {
                Ipp8u *pbSrc = (Ipp8u *) in.GetDataPointer();
                Ipp8u *pbSrcEnd = pbSrc + m_Frame.uiSize;
                while (pbSrcEnd > pbSrc + 5)
                {
                    Ipp32u unitSize;
                    unitSize = (pbSrc[0] << 24) | (pbSrc[1] << 16) | (pbSrc[2] << 8) | (pbSrc[3]);
                    pbSrc[0] = 0x00;
                    pbSrc[1] = 0x00;
                    pbSrc[2] = 0x00;
                    pbSrc[3] = 0x00;
                    pbSrc[4] = 0x01;
                    pbSrc += unitSize + 4;
                }
            }
        }

        m_ppMediaBuffer[uiPin]->UnLockInputBuffer(&in);
        vm_time_sleep(0);

        if (m_pInfo->m_fRate == 1)
            umcRes = pIndex->Next(m_Frame);
        else if (m_pInfo->m_fRate > 0)
        {
            for (i = 0; i < m_pInfo->m_fRate; i++)
            {
                umcRes = pIndex->Next(m_Frame);
                if(umcRes != UMC_OK)
                    break;
            }
            if(umcRes == UMC_OK && m_Frame.uiFlags != I_PICTURE)
                umcRes = pIndex->NextKey(m_Frame);
        }
        else
        {  /*** m_pInfo->m_dRate < 0 ***/
            for (i = 0; i < -m_pInfo->m_fRate; i++)
            {
                umcRes = pIndex->Prev(m_Frame);
                if (umcRes != UMC_OK)
                    break;
            }
            if (umcRes == UMC_OK && m_Frame.uiFlags != I_PICTURE)
                umcRes = pIndex->PrevKey(m_Frame);
        }
    }

    if (umcRes != UMC_OK)
        m_ppMediaBuffer[uiPin]->UnLockInputBuffer(NULL, UMC_ERR_END_OF_STREAM);
}

Ipp32u IndexSplitter::ReadESThreadCallback(void* ptr)
{
    VM_ASSERT(NULL != ptr);
    ReadESThreadParam* m_pParam = (ReadESThreadParam*)ptr;

    m_pParam->pThis->ReadES(m_pParam->uiPin);
    delete (ReadESThreadParam*)ptr;
    return 0;
}

Status IndexSplitter::Run()
{
    if (!m_bFlagStop)
        return UMC_OK;

    Ipp32u i;
    int res;

    m_bFlagStop = false;
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (((m_pInfo->m_fRate == 1.0) ||
            (m_pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)) &&
            (m_pInfo->m_ppTrackInfo[i]->m_bEnabled))
        {
            ReadESThreadParam* m_pESParam = new ReadESThreadParam;
            m_pESParam->uiPin = i;
            m_pESParam->pThis = this;

            res = vm_thread_create(&m_pReadESThread[i], (vm_thread_callback)ReadESThreadCallback, (void *)m_pESParam);
            if (res != 1)
                return UMC_ERR_FAILED;
        }
        else if (m_ppMediaBuffer[i])
            m_ppMediaBuffer[i]->UnLockInputBuffer(NULL, UMC_ERR_END_OF_STREAM);
    }

    return UMC_OK;
}

Status IndexSplitter::GetNextData(MediaData* data, Ipp32u nTrack)
{
    Status umcRes;

    if (nTrack > m_pInfo->m_iTracks)
        return UMC_ERR_FAILED;

    if (!m_ppMediaBuffer[nTrack] || !m_pInfo->m_ppTrackInfo[nTrack]->m_bEnabled)
        return UMC_ERR_FAILED;

    if (m_pIsLocked[nTrack] != 0)
    {
        m_ppLockedFrame[nTrack]->SetDataSize(0);
        m_ppMediaBuffer[nTrack]->UnLockOutputBuffer(m_ppLockedFrame[nTrack]);
        m_pIsLocked[nTrack] = 0;
    }

    umcRes = m_ppMediaBuffer[nTrack]->LockOutputBuffer(data);
    if (umcRes == UMC_OK)
    {
        *(m_ppLockedFrame[nTrack]) = *data;
        m_pIsLocked[nTrack] = 1;
    }

    return umcRes;
}

Status IndexSplitter::CheckNextData(MediaData* data, Ipp32u nTrack)
{
    Status umcRes = UMC_OK;

    if (nTrack > m_pInfo->m_iTracks)
        return UMC_ERR_FAILED;

    if (!m_ppMediaBuffer[nTrack] || !m_pInfo->m_ppTrackInfo[nTrack]->m_bEnabled)
        return UMC_ERR_FAILED;

    if (m_pIsLocked[nTrack] != 0)
    {
        m_ppLockedFrame[nTrack]->SetDataSize(0);
        m_ppMediaBuffer[nTrack]->UnLockOutputBuffer(m_ppLockedFrame[nTrack]);
        m_pIsLocked[nTrack] = 0;
    }

    umcRes = m_ppMediaBuffer[nTrack]->LockOutputBuffer(data);
    if (umcRes == UMC_OK) /*** not to get EOS after 2 CheckNextData ***/
        m_ppMediaBuffer[nTrack]->UnLockOutputBuffer(data);

    return umcRes;
}

Status IndexSplitter::SetTimePosition(Ipp64f position)
{
    IndexEntry entry;
    Ipp32u i, j;

    StopSplitter(); /* close read threads and reset buffers */
    m_bNewPortion = true;

    // tune up time position using first selected video track
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (m_pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO && m_pInfo->m_ppTrackInfo[i]->m_bEnabled)
        {
            m_pTrackIndex[i].First(entry);
            // add origin time stamp
            position += entry.GetTimeStamp();
            Status umcRes = m_pTrackIndex[i].Get(entry, position);
            if (UMC_OK == umcRes && I_PICTURE != entry.uiFlags)
            {
                // save time stamp of nearest previous key frame
                umcRes = m_pTrackIndex[i].PrevKey(entry);
                if (UMC_OK == umcRes)
                    position = entry.GetTimeStamp();
            }

            // align position of all tracks to one time position
            for (j = 0; j < m_pInfo->m_iTracks; j++)
            {
                if (m_pInfo->m_ppTrackInfo[j]->m_bEnabled && j != i)
                    m_pTrackIndex[j].Get(entry, position);
            }
            return UMC_OK;
        }
    }

    // if there are no video tracks
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (m_pInfo->m_ppTrackInfo[i]->m_bEnabled)
            m_pTrackIndex[i].Get(entry, position);
    }

    return UMC_OK;
}

Status IndexSplitter::GetTimePosition(Ipp64f& position)
{
    IndexEntry entry;
    Ipp32u i;

    /* get TimePos of the first selected video track */
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (m_pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO && m_pInfo->m_ppTrackInfo[i]->m_bEnabled)
        {
            m_pTrackIndex[i].Get(entry);
            break;
        }
    }

    /* if no video get TimePos of the first selected track */
    if (i == m_pInfo->m_iTracks)
    {
        for (i = 0; i < m_pInfo->m_iTracks; i++)
        {
            if (m_pInfo->m_ppTrackInfo[i]->m_bEnabled)
            {
                m_pTrackIndex[i].Get(entry);
                break;
            }
        }
    }

    position = entry.GetTimeStamp();
    return UMC_OK;
}

Status IndexSplitter::SetRate(Ipp64f rate)
{
    if (m_pReader == NULL)
        return UMC_ERR_NOT_INITIALIZED;

    m_pInfo->m_fRate = (Ipp32s)rate;
    StopSplitter();
    return UMC_OK;
}

Status IndexSplitter::SetTrackState(Ipp32u nTrack, TrackState state)
{
    Status umcRes;

    if(nTrack > m_pInfo->m_iTracks)
        return UMC_ERR_FAILED;

    if (!(m_pInfo->m_ppTrackInfo[nTrack]->m_type & TRACK_ANY_VIDEO) && !(m_pInfo->m_ppTrackInfo[nTrack]->m_type & TRACK_ANY_AUDIO))
        return UMC_ERR_FAILED;

    if(state == TRACK_ENABLED)
    {
        if(m_pInfo->m_ppTrackInfo[nTrack]->m_bEnabled)
            return UMC_OK;

        if ((m_pInfo->m_ppTrackInfo[nTrack]->m_type & TRACK_ANY_AUDIO) && (m_pInfo->m_fRate != 1.0))
            return UMC_OK;

        int res = 0;
        IndexEntry entry;
        Ipp64f curPos;

        umcRes = GetTimePosition(curPos);
        UMC_CHECK_STATUS(umcRes)

        m_pTrackIndex[nTrack].First(entry);
        // add origin time stamp
        curPos += entry.GetTimeStamp();
        m_pTrackIndex[nTrack].Get(entry, curPos);
        if (m_pInfo->m_ppTrackInfo[nTrack]->m_type & TRACK_ANY_VIDEO)
        {
            if (I_PICTURE != entry.uiFlags)
            m_pTrackIndex[nTrack].PrevKey(entry);
        }

        ReadESThreadParam* m_pESParam = new ReadESThreadParam;
        m_pESParam->uiPin = nTrack;
        m_pESParam->pThis = this;

        res = vm_thread_create(&m_pReadESThread[nTrack], (vm_thread_callback)ReadESThreadCallback, (void *)m_pESParam);
        if (res != 1)
            return UMC_ERR_FAILED;

        m_pInfo->m_ppTrackInfo[nTrack]->m_bEnabled = true;
    }
    else if(state == TRACK_DISABLED)
    {
        if (vm_thread_is_valid(&m_pReadESThread[nTrack]))
        {
            vm_thread_close(&m_pReadESThread[nTrack]);
            vm_thread_set_invalid(&m_pReadESThread[nTrack]);
        }
        if (m_ppMediaBuffer[nTrack])
        {
            umcRes = m_ppMediaBuffer[nTrack]->Reset();
            UMC_CHECK_STATUS(umcRes)
        }
        if (m_ppLockedFrame[nTrack])
        {
            umcRes = m_ppLockedFrame[nTrack]->Reset();
            UMC_CHECK_STATUS(umcRes)
            m_pIsLocked[nTrack] = 0;
        }
        m_pInfo->m_ppTrackInfo[nTrack]->m_bEnabled = false;
    }
    else
        return UMC_ERR_UNSUPPORTED; // unsupported state

    return UMC_OK;
}
