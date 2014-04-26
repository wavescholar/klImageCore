/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_INDEX_H__
#define __UMC_INDEX_H__

#include "umc_linked_list.h"
#include "umc_mutex.h"

namespace UMC
{

struct IndexEntry
{
    // Constructor
    IndexEntry()
    {
        stPosition = 0;
        dPts = -1.0;
        dDts = -1.0;
        uiSize = 0;
        uiFlags = 0;
    }

    // Returns DTS if present otherwise PTS
    inline Ipp64f GetTimeStamp(void)
    {
        return dDts < 0.0 ? dPts : dDts;
    }

    // Aboslute position of the sample
    Ipp64u stPosition;

    // Presentation time stamp in seconds
    Ipp64f dPts;

    // Decoding time stamp in seconds
    Ipp64f dDts;

    // Sample size in byte
    size_t uiSize;

    // Flags (frame type for video samples)
    Ipp32u uiFlags;
};

struct IndexFragment
{
    // Constructor
    IndexFragment()
    {
        pEntryArray = NULL;
        iNOfEntries = 0;
    }

    // The pointer to array of entries
    IndexEntry *pEntryArray;

    // number of entries in the array
    Ipp32s iNOfEntries;

};

class TrackIndex
{
public:

    TrackIndex();
    ~TrackIndex();

    // Returns number of entries in ALL fragments
    Ipp32u NOfEntries(void);

    // Provides FIRST entry from the FIRST fragment
    Status First(IndexEntry &entry);

    // Provides LAST entry from the LAST fragment
    Status Last(IndexEntry &entry);

    // Provides next entry
    // If last returned entry is the last in the fragment,
    // first entry from the NEXT fragment will be returned
    Status Next(IndexEntry &entry);

    // Provides previous entry
    // If last returned entry is the first in the fragment,
    // last entry from the PREVIOUS fragment will be returned
    Status Prev(IndexEntry &entry);

    // Provides next key entry
    // If last returned entry is the last in the fragment,
    // first entry from the NEXT fragment will be returned
    Status NextKey(IndexEntry &entry);

    // Provides previous key entry
    // If last returned entry is the first in the fragment,
    // last entry from the PREVIOUS fragment will be returned
    Status PrevKey(IndexEntry &entry);

    // Provides last returned entry
    Status Get(IndexEntry &entry);

    // Provides entry at the specified position (through ALL fragments)
    Status Get(IndexEntry &entry, Ipp32s pos);

    // Provides entry with timestamp is less or equal to specified (through ALL fragments)
    Status Get(IndexEntry &entry, Ipp64f time);

    // Add whole fragment to the end of index
    Status Add(IndexFragment &newFrag);

    // Removes last fragment
    Status Remove(void);

protected:

    // Returns pointer to next entry
    // If last returned entry is the last in the fragment,
    // first entry from the NEXT fragment will be returned
    // Input parameter and current state are not checked
    // State variables will be modified
    IndexEntry* NextEntry(void);

    // Returns pointer to previous entry
    // If last returned entry is the first in the fragment,
    // last entry from the PREVIOUS fragment will be returned
    // Input parameter and current state are not checked
    // State variables will be modified
    IndexEntry* PrevEntry(void);

    // Returns element at a specified position
    // Input parameter and current state are not checked
    // State variables will be modified
    IndexEntry* GetEntry(Ipp32s pos);

    // Returns element with timestamp is less or equal to specified
    // Input parameter and current state are not checked
    // State variables will be modified
    IndexEntry* GetEntry(Ipp64f time);

    // Linked list of index fragments
    LinkedList<IndexFragment> m_FragmentList;

    // Total number of entries in all fragments
    Ipp32u m_uiTotalEntries;

    // Copy of fragment which contains last returned entry
    IndexFragment m_ActiveFrag;

    // absolute position in index of the first entry of active fragment
    Ipp32s m_iFirstEntryPos;

    // absolute position in index of the last entry of active fragment
    Ipp32s m_iLastEntryPos;

    // relative position inside active fragment of the last returned entry
    Ipp32s m_iLastReturned;

    // synchro object
    Mutex m_mutex;

}; // class TrackIndex

}; // namespace UMC

#endif // __UMC_INDEX_H__
