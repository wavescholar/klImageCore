/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "pts_buf.h"

using namespace UMC;


void BufPTSArrayM::Normalize()
{
    Ipp32u uiPos = 0;
    Ipp32u uiSize = m_PtsArray.GetSize();
    sPtsEntry* pts_entry = m_PtsArray.GetArray();
    sPtsEntry* pts_entry_next = pts_entry + 1;

    if (1.0 != m_dfNorm)
    {
        while (uiPos + 1 < uiSize)
        {
            if (pts_entry->uiStart + pts_entry->uiLen >
                                                pts_entry_next->uiStart)
            {
                if (pts_entry->uiStart + pts_entry->uiLen <
                    pts_entry_next->uiStart + pts_entry_next->uiLen)
                {
                    Ipp32u uiDiff = pts_entry->uiStart +
                        pts_entry->uiLen - pts_entry_next->uiStart;

                    pts_entry_next->dfPts += uiDiff * m_dfNorm;
                    pts_entry_next->uiStart += uiDiff;
                    pts_entry_next->uiLen -= uiDiff;

                    pts_entry = pts_entry_next;
                    pts_entry_next++;
                    uiPos++;
                }
                else
                {
                    m_PtsArray.DeleteAt(uiPos + 1);
                    uiSize--;
                }
            }
            else
            {
                uiPos++;
                pts_entry = pts_entry_next;
                pts_entry_next++;
            }
        }
    }
}

Status BufPTSArrayM::Init(const Ipp32u /*uiArraySize*/,
                                    const Ipp64f fdNorm,
                                    bool doNormalize)
{
    m_doNormalize = doNormalize;
    m_PtsArray.DeleteAll();
    m_dfNorm = fdNorm;
    m_Mutex.Reset();
    return UMC_OK;
}

Status BufPTSArrayM::SetFramePTS(const Ipp32u uiBufPos,
                                           const Ipp32u uiLen,
                                           const Ipp64f dfPTS)
{
    Status umcRes = UMC_OK;

    m_Mutex.Lock();
    Ipp32u uiPos = 0;
    Ipp32u size = m_PtsArray.GetSize();
    sPtsEntry* pts_entry = m_PtsArray.GetArray();

    if (uiBufPos == 0)
        uiPos = 0;

    while (uiPos < size)
    {
      if (pts_entry->uiStart > uiBufPos)
          break;

      pts_entry++;
      uiPos++;
    }

    if (size == uiPos ||
        pts_entry->uiStart > uiBufPos ||
        pts_entry->dfPts != dfPTS)
    {
        if (!m_PtsArray.Insert(uiPos, sPtsEntry()))
        {   umcRes = UMC_ERR_ALLOC; }

        pts_entry = m_PtsArray.GetArray();
        pts_entry += uiPos;

    }

    if (UMC_OK == umcRes) {

        pts_entry->dfPts = dfPTS;
        pts_entry->uiStart = uiBufPos;
        pts_entry->uiLen = uiLen;

        if (m_doNormalize)
            Normalize();
    }
    m_Mutex.Unlock();
    return umcRes;
}

Status BufPTSArrayM::UpdateFramePTS(const Ipp32u uiOldBufPos,
                                              const Ipp32u uiNewBufPos,
                                              const Ipp32u /*uiNewLen*/,
                                              const Ipp64f dfNewPTS)
{
    m_Mutex.Lock();

    Ipp32u uiPos = 0;
    Ipp32u size;
    sPtsEntry* pts_entry = m_PtsArray.GetArray();

    size = m_PtsArray.GetSize();

    while (uiPos < size &&  pts_entry->uiStart != uiOldBufPos)
    {
      uiPos++;
      pts_entry++;
    }

    if (uiOldBufPos > uiNewBufPos)
    {
        while (uiPos < size)
        {
            m_PtsArray.DeleteAt(uiPos);
            size--;
        }

        uiPos = 0;
        pts_entry = m_PtsArray.GetArray();
    }

    while (uiPos < size &&  pts_entry->uiStart + pts_entry->uiLen <= uiNewBufPos)
    {
        m_PtsArray.DeleteAt(uiPos);
        size--;
    }

    if ((uiPos) < size) {
        pts_entry->uiLen   = pts_entry->uiLen - (uiNewBufPos - pts_entry->uiStart);
        if (pts_entry->uiStart != uiNewBufPos) {
            pts_entry->uiStart = uiNewBufPos;
            pts_entry->dfPts   = dfNewPTS;
        }
    }

    pts_entry = m_PtsArray.GetArray();

    uiPos = 0;
    while (uiPos < size)
    {
      pts_entry++;
      uiPos++;
    }

    m_Mutex.Unlock();
    return UMC_OK;
}

Ipp64f BufPTSArrayM::GetTime(const Ipp32u uiCurPos)
{
    Ipp32u uiPos = 0;
    Ipp64f dfPts = 0;
    Ipp32u size;
    sPtsEntry* pts_entry;

    m_Mutex.Lock();

    pts_entry = m_PtsArray.GetArray();
    size = m_PtsArray.GetSize();

    if (size > 0) {
        while (uiPos + 1 < size && (pts_entry+1)->uiStart <= uiCurPos)
        {
            uiPos++;
            pts_entry++;
        }

        dfPts = pts_entry->dfPts;

        if (1 != m_dfNorm) {
            dfPts += (uiCurPos - pts_entry->uiStart) * m_dfNorm;
        }
    }

    m_Mutex.Unlock();
    return dfPts;
}

void BufPTSArrayM::Reset()
{   m_PtsArray.DeleteAll(); }

void BufPTSArray::Normalize()
{
    Ipp32u uiPos = 0;
    while (uiPos + 1 < m_PtsArray.GetSize())
    {
        if (m_PtsArray[uiPos].uiStart + m_PtsArray[uiPos].uiLen >
            m_PtsArray[uiPos+1].uiStart)
        {
            if (m_PtsArray[uiPos].uiStart + m_PtsArray[uiPos].uiLen <
                m_PtsArray[uiPos+1].uiStart + m_PtsArray[uiPos+1].uiLen)
            {
                Ipp32u uiDiff = m_PtsArray[uiPos].uiStart +
                    m_PtsArray[uiPos].uiLen - m_PtsArray[uiPos+1].uiStart;

                m_PtsArray[uiPos+1].dfPts += uiDiff * m_dfNorm;
                m_PtsArray[uiPos+1].uiStart += uiDiff;
                m_PtsArray[uiPos+1].uiLen -= uiDiff;
                uiPos++;
            }
            else
            {   m_PtsArray.DeleteAt(uiPos + 1); }
        }
        else
        {   uiPos++;    }
    }
}

Status BufPTSArray::Init(const Ipp32u /*uiArraySize*/,
                                   const Ipp64f fdNorm)
{
    m_PtsArray.DeleteAll();
    m_dfNorm = fdNorm;
    m_Mutex.Reset();
    return UMC_OK;
}

Ipp64f BufPTSArray::DynamicSetParams(const Ipp64f fdNorm)
{
    Ipp64f ret = m_dfNorm;
    m_Mutex.Lock();
    m_dfNorm = fdNorm;
    m_Mutex.Unlock();
    return ret;
}

Status BufPTSArray::SetFramePTS(const Ipp32u uiBufPos,
                                          const Ipp32u uiLen,
                                          const Ipp64f dfPTS)
{
    Status umcRes = UMC_OK;
    //if (1 == m_dfNorm) {  umcRes = UMC_ERR_NOT_INITIALIZED;   }

    m_Mutex.Lock();
    Ipp32u uiPos = 0;
    while (UMC_OK == umcRes && uiPos < m_PtsArray.GetSize() &&
            m_PtsArray[uiPos].uiStart < uiBufPos)
    { uiPos++; }

    if (m_PtsArray.GetSize() == uiPos ||
        m_PtsArray[uiPos].uiStart > uiBufPos ||
        m_PtsArray[uiPos].dfPts != dfPTS)
    {
        if (!m_PtsArray.Insert(uiPos, sPtsEntry()))
        {   umcRes = UMC_ERR_ALLOC; }
    }

    if (UMC_OK == umcRes)
    {
        m_PtsArray[uiPos].dfPts = dfPTS;
        m_PtsArray[uiPos].uiStart = uiBufPos;
        m_PtsArray[uiPos].uiLen = uiLen;
        Normalize();
    }

    m_Mutex.Unlock();
    return umcRes;
}

Ipp64f BufPTSArray::GetTime(const Ipp32u uiCurPos)
{
    Ipp64f dfPts = -1.0;

    m_Mutex.Lock();
    if (m_PtsArray.GetSize() > 0)
    {
        Ipp32u uiPos = 0;
        while (uiPos + 1 < m_PtsArray.GetSize() &&
                m_PtsArray[uiPos + 1].uiStart < uiCurPos)
        {   uiPos++; }

        dfPts = m_PtsArray[uiPos].dfPts;

        if (1 != m_dfNorm &&
            m_PtsArray[uiPos].uiStart + m_PtsArray[uiPos].uiLen > uiCurPos)
        {   dfPts += (uiCurPos - m_PtsArray[uiPos].uiStart) * m_dfNorm; }
    }
    m_Mutex.Unlock();
    return dfPts;
}

void BufPTSArray::Reset() {   m_PtsArray.DeleteAll(); }
