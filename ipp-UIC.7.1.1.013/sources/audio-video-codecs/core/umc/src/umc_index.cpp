/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_index.h"
#include "umc_automatic_mutex.h"

using namespace UMC;

TrackIndex::TrackIndex()
{
    m_uiTotalEntries = 0;
    m_iFirstEntryPos = 0;
    m_iLastEntryPos = 0;
    m_iLastReturned = -1;
} // TrackIndex::TrackIndex()

TrackIndex::~TrackIndex()
{
    m_iLastReturned = -1;
    m_iFirstEntryPos = 0;
    m_iLastEntryPos = 0;
    m_uiTotalEntries = 0;

    // delete memory allocated for arrays of entries
    IndexFragment frag;
    Status umcRes = m_FragmentList.First(frag);
    while (UMC_OK == umcRes)
    {
        frag.iNOfEntries = 0;
        delete[] frag.pEntryArray;
        umcRes = m_FragmentList.Next(frag);
    }

} // TrackIndex::~TrackIndex()

Ipp32u TrackIndex::NOfEntries(void)
{
    AutomaticMutex guard(m_mutex);
    return m_uiTotalEntries;
} // Ipp32u TrackIndex::NOfEntries(void)

Status TrackIndex::First(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    Status umcRes = m_FragmentList.First(m_ActiveFrag);
    if (UMC_OK != umcRes)
        return umcRes;

    m_iLastReturned = 0;
    m_iFirstEntryPos = 0;
    m_iLastEntryPos = m_ActiveFrag.iNOfEntries - 1;
    entry = m_ActiveFrag.pEntryArray[m_iLastReturned];
    return UMC_OK;
} // Status TrackIndex::First(IndexEntry &entry)

Status TrackIndex::Last(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    Status umcRes = m_FragmentList.Last(m_ActiveFrag);
    if (UMC_OK != umcRes)
        return umcRes;

    m_iLastReturned = m_ActiveFrag.iNOfEntries - 1;
    m_iFirstEntryPos = m_uiTotalEntries - m_ActiveFrag.iNOfEntries;
    m_iLastEntryPos = m_uiTotalEntries - 1;
    entry = m_ActiveFrag.pEntryArray[m_iLastReturned];
    return UMC_OK;
} // Status TrackIndex::Last(IndexEntry &entry)

Status TrackIndex::Next(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    if (m_iLastReturned < 0)
        return UMC_ERR_FAILED;

    IndexEntry *pNextEntry = NextEntry();
    if (NULL == pNextEntry)
        return UMC_ERR_NOT_ENOUGH_DATA;

    entry = *pNextEntry;
    return UMC_OK;
} // Status TrackIndex::Next(IndexEntry &entry)

Status TrackIndex::Prev(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    if (m_iLastReturned < 0)
        return UMC_ERR_FAILED;

    IndexEntry *pPrevEntry = PrevEntry();
    if (NULL == pPrevEntry)
        return UMC_ERR_NOT_ENOUGH_DATA;

    entry = *pPrevEntry;
    return UMC_OK;
} // Status TrackIndex::Prev(IndexEntry &entry)

Status TrackIndex::NextKey(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    if (m_iLastReturned < 0)
        return UMC_ERR_FAILED;

    IndexEntry *pNextEntry;

    do {
        pNextEntry = NextEntry();
    } while (NULL != pNextEntry && I_PICTURE != pNextEntry->uiFlags);

    if (NULL == pNextEntry)
        return UMC_ERR_NOT_ENOUGH_DATA;

    entry = *pNextEntry;
    return UMC_OK;
} // Status TrackIndex::NextKey(IndexEntry &entry)

Status TrackIndex::PrevKey(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    if (m_iLastReturned < 0)
        return UMC_ERR_FAILED;

    IndexEntry *pPrevEntry;

    do {
        pPrevEntry = PrevEntry();
    } while (NULL != pPrevEntry && I_PICTURE != pPrevEntry->uiFlags);

    if (NULL == pPrevEntry)
        return UMC_ERR_NOT_ENOUGH_DATA;

    entry = *pPrevEntry;
    return UMC_OK;
} // Status TrackIndex::PrevKey(IndexEntry &entry)

Status TrackIndex::Get(IndexEntry &entry)
{
    AutomaticMutex guard(m_mutex);

    if (m_iLastReturned < 0)
        return UMC_ERR_FAILED;

    entry = m_ActiveFrag.pEntryArray[m_iLastReturned];
    return UMC_OK;
} // Status TrackIndex::Get(IndexEntry &entry)

Status TrackIndex::Get(IndexEntry &entry, Ipp32s pos)
{
    AutomaticMutex guard(m_mutex);

    if (pos < 0 || pos >= (Ipp32s)m_uiTotalEntries)
        return UMC_ERR_FAILED;

    // init search session if not inited
    if (m_iLastReturned < 0)
    {
        m_FragmentList.First(m_ActiveFrag);
        m_iFirstEntryPos = 0;
        m_iLastEntryPos = m_ActiveFrag.iNOfEntries - 1;
    }

    IndexEntry *pEntryToGet = GetEntry(pos);

    // something goes wrong
    if (NULL == pEntryToGet)
    {
        m_iLastReturned = -1;
        return UMC_ERR_FAILED;
    }

    entry = *pEntryToGet;
    return UMC_OK;
} // Status TrackIndex::Get(IndexEntry &entry, Ipp32s pos)

Status TrackIndex::Get(IndexEntry &entry, Ipp64f time)
{
    AutomaticMutex guard(m_mutex);

    if (time < 0)
        return UMC_ERR_FAILED;

    // init search session if not inited
    if (m_iLastReturned < 0)
    {
        m_FragmentList.First(m_ActiveFrag);
        m_iFirstEntryPos = 0;
        m_iLastEntryPos = m_ActiveFrag.iNOfEntries - 1;
    }

    IndexEntry *pEntryToGet = GetEntry(time);

    // something goes wrong
    if (NULL == pEntryToGet)
    {
        m_iLastReturned = -1;
        return UMC_ERR_FAILED;
    }

    entry = *pEntryToGet;
    return UMC_OK;
} // Status TrackIndex::Get(IndexEntry &entry, Ipp64f time)

Status TrackIndex::Add(IndexFragment &newFrag)
{
    AutomaticMutex guard(m_mutex);

    if (0 == newFrag.iNOfEntries || NULL == newFrag.pEntryArray)
        return UMC_ERR_FAILED;

    Status umcRes = m_FragmentList.Add(newFrag);
    if (UMC_OK != umcRes)
        return umcRes;

    m_uiTotalEntries += newFrag.iNOfEntries;

    return UMC_OK;
} // Status TrackIndex::Add(IndexFragment &newFrag)

Status TrackIndex::Remove(void)
{
    AutomaticMutex guard(m_mutex);

    IndexFragment frag;
    Status umcRes = m_FragmentList.Last(frag);
    if (UMC_OK != umcRes)
        return umcRes;

    // reset state
    m_iLastReturned = -1;
    m_iFirstEntryPos = 0;
    m_iLastEntryPos = 0;

    // decrease entries counter
    m_uiTotalEntries -= frag.iNOfEntries;

    m_FragmentList.Remove();
    return UMC_OK;
} // Status TrackIndex::Remove(void)

IndexEntry *TrackIndex::NextEntry(void)
{
    if (m_iLastReturned + 1 >= m_ActiveFrag.iNOfEntries)
    { // go to next fragment
        Status umcRes = m_FragmentList.Next(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        m_iLastReturned = 0;
        m_iFirstEntryPos = m_iLastEntryPos + 1;
        m_iLastEntryPos += m_ActiveFrag.iNOfEntries;
    }
    else
        m_iLastReturned++;

    return &m_ActiveFrag.pEntryArray[m_iLastReturned];
} // IndexEntry *TrackIndex::NextEntry(void)

IndexEntry *TrackIndex::PrevEntry(void)
{
    if (m_iLastReturned - 1 < 0)
    { // go to previous fragment
        Status umcRes = m_FragmentList.Prev(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        m_iLastEntryPos = m_iFirstEntryPos - 1;
        m_iFirstEntryPos -= m_ActiveFrag.iNOfEntries;
        m_iLastReturned = m_ActiveFrag.iNOfEntries - 1;
    }
    else
        m_iLastReturned--;

    return &m_ActiveFrag.pEntryArray[m_iLastReturned];
} // IndexEntry *TrackIndex::PrevEntry(void)

IndexEntry *TrackIndex::GetEntry(Ipp32s pos)
{
    // if needed entry is located before active fragment
    while (pos < m_iFirstEntryPos)
    {
        Status umcRes = m_FragmentList.Prev(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        m_iLastEntryPos = m_iFirstEntryPos - 1;
        m_iFirstEntryPos -= m_ActiveFrag.iNOfEntries;
    }

    // needed entry is located after active fragment
    while (pos > m_iLastEntryPos)
    {
        Status umcRes = m_FragmentList.Next(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        m_iFirstEntryPos = m_iLastEntryPos + 1;
        m_iLastEntryPos += m_ActiveFrag.iNOfEntries;
    }

    // requested fragment have been found
    m_iLastReturned = pos - m_iFirstEntryPos;
    return &m_ActiveFrag.pEntryArray[m_iLastReturned];
} // IndexEntry *TrackIndex::GetEntry(Ipp32s pos)

IndexEntry *TrackIndex::GetEntry(Ipp64f time)
{
    // if needed entry is located before active fragment
    while (time < m_ActiveFrag.pEntryArray[0].GetTimeStamp())
    {
        Status umcRes = m_FragmentList.Prev(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        m_iLastEntryPos = m_iFirstEntryPos - 1;
        m_iFirstEntryPos -= m_ActiveFrag.iNOfEntries;
    }

    // needed entry is located after active fragment
    while (time > m_ActiveFrag.pEntryArray[m_ActiveFrag.iNOfEntries - 1].GetTimeStamp())
    {
        Status umcRes = m_FragmentList.Next(m_ActiveFrag);
        if (UMC_OK != umcRes)
            return NULL;

        if (time < m_ActiveFrag.pEntryArray[0].GetTimeStamp())
        {
            m_FragmentList.Prev(m_ActiveFrag);
            break;
        }

        m_iFirstEntryPos = m_iLastEntryPos + 1;
        m_iLastEntryPos += m_ActiveFrag.iNOfEntries;
    }

    // requested fragment have been found
    Ipp32s nOfEntries = m_ActiveFrag.iNOfEntries;
    IndexEntry *pEntryArray = m_ActiveFrag.pEntryArray;
    Ipp64f dStartTime = pEntryArray[0].GetTimeStamp();
    Ipp64f dEndTime = pEntryArray[nOfEntries - 1].GetTimeStamp();

    m_iLastReturned = 0;

    if (nOfEntries > 1)
    {
      // approximate position of requested entry
      m_iLastReturned = (Ipp32s)(nOfEntries * (time - dStartTime) / (dEndTime - dStartTime));

      if (pEntryArray[m_iLastReturned].GetTimeStamp() < time)
          while (m_iLastReturned + 1 < nOfEntries && pEntryArray[m_iLastReturned + 1].GetTimeStamp() < time)
              m_iLastReturned++;
      else
          while (m_iLastReturned >= 0 && pEntryArray[m_iLastReturned].GetTimeStamp() > time)
              m_iLastReturned--;
    }

    return &m_ActiveFrag.pEntryArray[m_iLastReturned];
} // IndexEntry *TrackIndex::GetEntry(Ipp64f time)
