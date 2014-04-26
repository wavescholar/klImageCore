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

#ifndef __UMC_H264_FRAME_LIST_H__
#define __UMC_H264_FRAME_LIST_H__

#include "umc_h264_frame.h"

namespace UMC
{

class H264DecoderFrameList
{
public:
    // Default constructor
    H264DecoderFrameList(void);
    // Destructor
    virtual
    ~H264DecoderFrameList(void);

    H264DecoderFrame   *head() { return m_pHead; }
    H264DecoderFrame   *tail() { return m_pTail; }

    const H264DecoderFrame   *head() const { return m_pHead; }
    const H264DecoderFrame   *tail() const { return m_pTail; }

    bool isEmpty() { return !m_pHead; }

    void swapFrames(H264DecoderFrame *pFrame1, H264DecoderFrame *pFrame2);

    void append(H264DecoderFrame *pFrame);
    // Append the given frame to our tail

    Ipp32s GetFreeIndex()
    {
        for(Ipp32s i = 0; i < 128; i++)
        {
            H264DecoderFrame *pFrm;

            for (pFrm = head(); pFrm && pFrm->m_index != i; pFrm = pFrm->future())
            {}

            if(pFrm == NULL)
            {
                return i;
            }
        }

        VM_ASSERT(false);
        return -1;
    };

    void Reset();

protected:

    // Release object
    void Release(void);

    H264DecoderFrame *m_pHead;                          // (H264DecoderFrame *) pointer to first frame in list
    H264DecoderFrame *m_pTail;                          // (H264DecoderFrame *) pointer to last frame in list
};

class H264DBPList : public H264DecoderFrameList
{
public:

    H264DBPList();

    H264DecoderFrame * GetOldestDisposable();

    H264DecoderFrame * GetLastDisposable();

    bool IsDisposableExist();

    bool IsAlmostDisposableExist();

    H264DecoderFrame *GetDisposable(void);
    // Search through the list for the disposable frame to decode into
    // Move disposable frame to tail

    void removeAllRef(NotifiersChain * notify);
    // Mark all frames as not used as reference frames.

    void IncreaseRefPicListResetCount(H264DecoderFrame *excludeFrame);
    // Mark all frames as not used as reference frames.

    H264DecoderFrame * freeOldestShortTermRef();
    // Mark the oldest short-term reference frame as not used.

    H264DecoderFrame *  freeShortTermRef(Ipp32s picNum);
    // Mark the short-term reference frame with specified picNum
    // as not used

    H264DecoderFrame *  freeLongTermRef(Ipp32s longTermPicNum);
    // Mark the long-term reference frame with specified LongTermPicNum
    // as not used

    H264DecoderFrame *  freeLongTermRefIdx(Ipp32s longTermFrameIdx, H264DecoderFrame *pCurrentFrame);
    // Mark the long-term reference frame with specified LongTermFrameIdx
    // as not used

    H264DecoderFrame *  freeOldLongTermRef(Ipp32s maxLongTermFrameIdx, NotifiersChain * notify);
    // Mark any long-term reference frame with LongTermFrameIdx greater
    // than MaxLongTermFrameIdx as not used.

    void changeSTtoLTRef(Ipp32s picNum, Ipp32s longTermFrameIdx);
    // Mark the short-term reference frame with
    // specified picNum as long-term with specified long term idx.

    H264DecoderFrame * findOldestShortTermRef();

    H264DecoderFrame * findOldestLongTermRef();

    H264DecoderFrame *findShortTermPic(Ipp32s  picNum, Ipp32s * field);

    H264DecoderFrame *findLongTermPic(Ipp32s  picNum, Ipp32s * field);

    Ipp32s countAllFrames();

    void countActiveRefs(Ipp32u &numShortTerm, Ipp32u &numLongTerm);
    // Return number of active Ipp16s and long term reference frames.

    H264DecoderFrame * findFirstDisplayable();

    H264DecoderFrame *findOldestDisplayable(Ipp32s dbpSize);
    // Search through the list for the oldest displayable frame.

    Ipp32s countNumDisplayable();
    // Return number of displayable frames.

    void MoveToTail(H264DecoderFrame * pFrame);

    H264DecoderFrame * FindClosest(H264DecoderFrame * pFrame);

    H264DecoderFrame * FindByIndex(Ipp32s index);

    Ipp32s GetDPBSize() const
    {
        return m_dpbSize;
    }

    void SetDPBSize(Ipp32s dpbSize)
    {
        m_dpbSize = dpbSize;
    }

protected:
    Ipp32s m_dpbSize;
};

extern
void OnSlideWindow(H264DecoderFrame *pRefFrame, NotifiersChain * notify);

} // end namespace UMC

#endif // __UMC_H264_FRAME_LIST_H__
