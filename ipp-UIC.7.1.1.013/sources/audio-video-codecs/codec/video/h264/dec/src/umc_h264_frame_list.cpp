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

#include "umc_h264_frame_list.h"


namespace UMC
{

H264DecoderFrameList::H264DecoderFrameList(void)
{
    m_pHead = NULL;
    m_pTail = NULL;
} // H264DecoderFrameList::H264DecoderFrameList(void)

H264DecoderFrameList::~H264DecoderFrameList(void)
{
    Release();

} // H264DecoderFrameList::~H264DecoderFrameList(void)

void H264DecoderFrameList::Release(void)
{
    // destroy frame list
    while (m_pHead)
    {
        H264DecoderFrame *pNext = m_pHead->future();
        delete m_pHead;
        m_pHead = pNext;
    }

    m_pHead = NULL;
    m_pTail = NULL;

} // void H264DecoderFrameList::Release(void)

void H264DecoderFrameList::Reset()
{
    for (H264DecoderFrame *pFrame = head(); pFrame; pFrame = pFrame->future())
    {
        pFrame->m_NotifiersChain.Notify();
    }

    for (H264DecoderFrame *pFrame = head(); pFrame; pFrame = pFrame->future())
    {
        pFrame->Reset();
    }

    Release();
}

//////////////////////////////////////////////////////////////////////////////
// append
//   Appends a new decoded frame buffer to the "end" of the linked list
//////////////////////////////////////////////////////////////////////////////
void H264DecoderFrameList::append(H264DecoderFrame *pFrame)
{
    // Error check
    if (!pFrame)
    {
        // Sent in a NULL frame
        return;
    }

    // Has a list been constructed - is their a head?
    if (!m_pHead)
    {
        // Must be the first frame appended
        // Set the head to the current
        m_pHead = pFrame;
        m_pHead->setPrevious(0);
    }

    if (m_pTail)
    {
        // Set the old tail as the previous for the current
        pFrame->setPrevious(m_pTail);

        // Set the old tail's future to the current
        m_pTail->setFuture(pFrame);
    }
    else
    {
        // Must be the first frame appended
        // Set the tail to the current
        m_pTail = pFrame;
    }

    // The current is now the new tail
    m_pTail = pFrame;
    m_pTail->setFuture(0);
    //
}

void H264DecoderFrameList::swapFrames(H264DecoderFrame *pFrame1, H264DecoderFrame *pFrame2)
{
    H264DecoderFrame * temp1 = pFrame1->previous();
    H264DecoderFrame * temp2 = pFrame2->previous();

    if (temp1)
    {
        temp1->setFuture(pFrame2);
    }

    if (temp2)
    {
        temp2->setFuture(pFrame1);
    }

    pFrame1->setPrevious(temp2);
    pFrame2->setPrevious(temp1);

    temp1 = pFrame1->future();
    temp2 = pFrame2->future();

    if (temp1)
    {
        temp1->setPrevious(pFrame2);
    }

    if (temp2)
    {
        temp2->setPrevious(pFrame1);
    }

    pFrame1->setFuture(temp2);
    pFrame2->setFuture(temp1);


    if (m_pHead == pFrame1)
    {
        m_pHead = pFrame2;
    }
    else
    {
        if (m_pHead == pFrame2)
        {
            m_pHead = pFrame1;
        }
    }

    if (m_pTail == pFrame1)
    {
        m_pTail = pFrame2;
    }
    else
    {
        if (m_pTail == pFrame2)
        {
            m_pTail = pFrame1;
        }
    }
}

H264DBPList::H264DBPList()
    : m_dpbSize(0)
{
}

H264DecoderFrame * H264DBPList::GetOldestDisposable(void)
{
    H264DecoderFrame *pOldest = NULL;
    Ipp32s  SmallestPicOrderCnt = 0x7fffffff;    // very large positive
    Ipp32s  LargestRefPicListResetCount = 0;

    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->isDisposable())
        {
            if (pTmp->RefPicListResetCount(0,3) > LargestRefPicListResetCount)
            {
                pOldest = pTmp;
                SmallestPicOrderCnt = pTmp->PicOrderCnt(0,3);
                LargestRefPicListResetCount = pTmp->RefPicListResetCount(0,3);
            }
            else if ((pTmp->PicOrderCnt(0,3) < SmallestPicOrderCnt) &&
                     (pTmp->RefPicListResetCount(0,3) == LargestRefPicListResetCount))
            {
                pOldest = pTmp;
                SmallestPicOrderCnt = pTmp->PicOrderCnt(0,3);
            }
        }
    }

    return pOldest;
} // H264DecoderFrame *H264DBPList::GetDisposable(void)

H264DecoderFrame * H264DBPList::GetLastDisposable(void)
{
    H264DecoderFrame *pOldest = NULL;

    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->isDisposable())
        {
            pOldest = pTmp;
        }
    }
    return pOldest;
} // H264DecoderFrame *H264DBPList::GetDisposable(void)

void H264DBPList::MoveToTail(H264DecoderFrame * pFrame)
{
    if (m_pHead == m_pTail || pFrame == m_pTail)
    {
        return;
    }

    // move to tail
    if (pFrame == m_pHead)
    {
        m_pHead = pFrame->m_pFutureFrame;
        m_pHead->m_pPreviousFrame = 0;
    }
    else
    {
        pFrame->m_pPreviousFrame->m_pFutureFrame = pFrame->m_pFutureFrame;
        pFrame->m_pFutureFrame->m_pPreviousFrame = pFrame->m_pPreviousFrame;
    }

    m_pTail->m_pFutureFrame = pFrame;
    pFrame->m_pPreviousFrame = m_pTail;
    m_pTail = pFrame;
    pFrame->m_pFutureFrame = 0;
}

bool H264DBPList::IsDisposableExist()
{
    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->isDisposable())
        {
            return true;
        }
    }

    return false;
}

bool H264DBPList::IsAlmostDisposableExist()
{
    Ipp32s count = 0;
    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        count++;
        if (isAlmostDisposable(pTmp))
        {
            return true;
        }
    }

    return count < m_dpbSize;
}

H264DecoderFrame * H264DBPList::GetDisposable(void)
{
    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->isDisposable())
        {
            return pTmp;
        }
    }

    // We never found one
    return NULL;
} // H264DecoderFrame *H264DBPList::GetDisposable(void)


///////////////////////////////////////////////////////////////////////////////
// findOldestDisplayable
// Search through the list for the oldest displayable frame. It must be
// not disposable, not outputted, and have smallest PicOrderCnt.
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::findOldestDisplayable(Ipp32s /*dbpSize*/ )
{
    H264DecoderFrame *pCurr = m_pHead;
    H264DecoderFrame *pOldest = NULL;
    Ipp32s  SmallestPicOrderCnt = 0x7fffffff;    // very large positive
    Ipp32s  LargestRefPicListResetCount = 0;

    Ipp32s count = 0;
    while (pCurr)
    {
        if ((pCurr->isDisplayable() || (!pCurr->IsDecoded() && pCurr->IsFullFrame())) && !pCurr->wasOutputted())
        {
            // corresponding frame
            if (pCurr->RefPicListResetCount(0,3) > LargestRefPicListResetCount )
            {
                pOldest = pCurr;
                SmallestPicOrderCnt = pCurr->PicOrderCnt(0,3);
                LargestRefPicListResetCount = pCurr->RefPicListResetCount(0,3);
            }
            else if ((pCurr->PicOrderCnt(0,3) < SmallestPicOrderCnt ) &&
                     (pCurr->RefPicListResetCount(0,3) == LargestRefPicListResetCount ))
            {
                pOldest = pCurr;
                SmallestPicOrderCnt = pCurr->PicOrderCnt(0,3);
            }

            if (!pOldest->IsFrameExist() && pCurr->IsFrameExist())
            {
                if (pCurr->PicOrderCnt(0,3) == SmallestPicOrderCnt &&
                    pCurr->RefPicListResetCount(0,3) == LargestRefPicListResetCount)
                    pOldest = pCurr;
            }

            //if (count == dbpSize + 1)
              //  break;
            count++;
        }

        pCurr = pCurr->future();
    }

    // may be OK if NULL
    return pOldest;

}    // findOldestDisplayable

H264DecoderFrame * H264DBPList::findFirstDisplayable()
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if ((pCurr->isDisplayable() || !pCurr->IsDecoded()) && !pCurr->wasOutputted())
        {
            return pCurr;
        }

        pCurr = pCurr->future();
    }

    return 0;
}    // findFirstDisplayable

Ipp32s H264DBPList::countAllFrames()
{
    H264DecoderFrame *pCurr = head();
    Ipp32u count = 0;

    while (pCurr)
    {
        count++;
        pCurr = pCurr->future();
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
// countNumDisplayable
//  Return number of displayable frames.
///////////////////////////////////////////////////////////////////////////////
Ipp32s H264DBPList::countNumDisplayable()
{
    H264DecoderFrame *pCurr = head();
    Ipp32u NumDisplayable = 0;

    while (pCurr)
    {
        if (((pCurr->isShortTermRef() || pCurr->isLongTermRef()) && pCurr->IsFullFrame()) || ((pCurr->isDisplayable() || (!pCurr->IsDecoded() && pCurr->IsFullFrame())) && !pCurr->wasOutputted()))
            NumDisplayable++;
        pCurr = pCurr->future();
    }

    return NumDisplayable;
}    // countNumDisplayable

///////////////////////////////////////////////////////////////////////////////
// countActiveRefs
//  Return number of active Ipp16s and long term reference frames.
///////////////////////////////////////////////////////////////////////////////
void H264DBPList::countActiveRefs(Ipp32u &NumShortTerm, Ipp32u &NumLongTerm)
{
    H264DecoderFrame *pCurr = m_pHead;
    NumShortTerm = 0;
    NumLongTerm = 0;

    while (pCurr)
    {
        if (pCurr->isShortTermRef())
            NumShortTerm++;
        else if (pCurr->isLongTermRef())
            NumLongTerm++;
        pCurr = pCurr->future();
    }

}    // countActiveRefs

///////////////////////////////////////////////////////////////////////////////
// removeAllRef
// Marks all frames as not used as reference frames.
///////////////////////////////////////////////////////////////////////////////
void H264DBPList::removeAllRef(NotifiersChain * notify)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr->isShortTermRef() || pCurr->isLongTermRef())
        {
            pCurr->unSetisLongTermRef(0);
            pCurr->unSetisLongTermRef(1);
            pCurr->unSetisShortTermRef(0);
            pCurr->unSetisShortTermRef(1);

            OnSlideWindow(pCurr, notify);
        }

        pCurr = pCurr->future();
    }

}    // removeAllRef

void H264DBPList::IncreaseRefPicListResetCount(H264DecoderFrame *ExcludeFrame)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr!=ExcludeFrame)
        {
            pCurr->IncreaseRefPicListResetCount(0);
            pCurr->IncreaseRefPicListResetCount(1);
        }
        pCurr = pCurr->future();
    }

}    // IncreaseRefPicListResetCount

///////////////////////////////////////////////////////////////////////////////
// freeOldestShortTermRef
// Marks the oldest (having smallest FrameNumWrap) short-term reference frame
// as not used as reference frame.
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::freeOldestShortTermRef()
{
    H264DecoderFrame *pOldest = findOldestShortTermRef();
    VM_ASSERT(pOldest != 0);    // Should not have been called if no short-term refs

    if (pOldest)
    {
        pOldest->unSetisShortTermRef(0);
        pOldest->unSetisShortTermRef(1);
    }

    return pOldest;
}    // freeOldestShortTermRef

H264DecoderFrame * H264DBPList::findOldestShortTermRef()
{
    H264DecoderFrame *pCurr = m_pHead;
    H264DecoderFrame *pOldest = 0;
    Ipp32s  SmallestFrameNumWrap = 0x0fffffff;    // very large positive

    while (pCurr)
    {
        if (pCurr->isShortTermRef() && (pCurr->FrameNumWrap() < SmallestFrameNumWrap))
        {
            pOldest = pCurr;
            SmallestFrameNumWrap = pCurr->FrameNumWrap();
        }
        pCurr = pCurr->future();
    }

    return pOldest;
}    // findOldestShortTermRef

H264DecoderFrame * H264DBPList::findOldestLongTermRef()
{
    H264DecoderFrame *pCurr = m_pHead;
    H264DecoderFrame *pOldest = 0;
    Ipp32s  SmallestFrameNumWrap = 0x0fffffff;    // very large positive

    while (pCurr)
    {
        if (pCurr->isLongTermRef() && (pCurr->FrameNumWrap() < SmallestFrameNumWrap))
        {
            pOldest = pCurr;
            SmallestFrameNumWrap = pCurr->FrameNumWrap();
        }
        pCurr = pCurr->future();
    }

    return pOldest;
}    // findOldestLongTermRef

///////////////////////////////////////////////////////////////////////////////
// freeShortTermRef
// Mark the short-term reference frame with specified picNum as not used
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::freeShortTermRef(Ipp32s  picNum)
{
    H264DecoderFrame *pCurr = m_pHead;
    bool found = false;
    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef >= FRM_STRUCTURE)
        {
            if (pCurr->isShortTermRef() && (pCurr->PicNum(0) == picNum))
            {
                pCurr->unSetisShortTermRef(0);
                return pCurr;
            }
        }
        else
        {
            if (pCurr->isShortTermRef(0) && (pCurr->PicNum(0) == picNum))
            {
                pCurr->unSetisShortTermRef(0);
                found = true;
            }

            if (pCurr->isShortTermRef(1) && (pCurr->PicNum(1) == picNum))
            {
                pCurr->unSetisShortTermRef(1);
                found = true;
            }

            if (found)
                return pCurr;
        }

        pCurr = pCurr->future();
    }

    //VM_ASSERT(false);
    return 0;
}    // freeShortTermRef

///////////////////////////////////////////////////////////////////////////////
// freeLongTermRef
// Mark the long-term reference frame with specified LongTermPicNum as not used
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::freeLongTermRef(Ipp32s  LongTermPicNum)
{
    H264DecoderFrame *pCurr = m_pHead;
    bool found = false;

    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef>=FRM_STRUCTURE)
        {
            if (pCurr->isLongTermRef() && (pCurr->LongTermPicNum(0) == LongTermPicNum))
            {
                pCurr->unSetisLongTermRef(0);
                return pCurr;
            }
        }
        else
        {
            if (pCurr->isLongTermRef(0) && (pCurr->LongTermPicNum(0) == LongTermPicNum))
            {
                pCurr->unSetisLongTermRef(0);
                found = true;
            }

            if (pCurr->isLongTermRef(1) && (pCurr->LongTermPicNum(1) == LongTermPicNum))
            {
                pCurr->unSetisLongTermRef(1);
                found = true;
            }

            if (found)
                return pCurr;
        }

        pCurr = pCurr->future();
    }


    //VM_ASSERT(false);    // No match found, should not happen.
    return 0;
}    // freeLongTermRef

///////////////////////////////////////////////////////////////////////////////
// freeLongTermRef
// Mark the long-term reference frame with specified LongTermFrameIdx
// as not used
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::freeLongTermRefIdx(Ipp32s  LongTermFrameIdx, H264DecoderFrame * pCurrentFrame)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef >= FRM_STRUCTURE)
        {
            if (pCurr->isLongTermRef() && (pCurr->LongTermFrameIdx() == LongTermFrameIdx ))
            {
                if (pCurrentFrame == pCurr)
                    return 0;
                pCurr->unSetisLongTermRef(0);
                return pCurr;
            }
        }
        else
        {
            if (pCurr->isLongTermRef(0) && (pCurr->LongTermFrameIdx() == LongTermFrameIdx ))
            {
                if (pCurrentFrame == pCurr)
                    return 0;
                pCurr->unSetisLongTermRef(0);
                pCurr->unSetisLongTermRef(1);
                return pCurr;
            }

            if (pCurr->isLongTermRef(1) && (pCurr->LongTermFrameIdx() == LongTermFrameIdx ))
            {
                if (pCurrentFrame == pCurr)
                    return 0;
                pCurr->unSetisLongTermRef(0);
                pCurr->unSetisLongTermRef(1);
                return pCurr;
            }
        }

        pCurr = pCurr->future();
    }

    //VM_ASSERT(false);
    return 0;
}    // freeLongTermRefIdx

///////////////////////////////////////////////////////////////////////////////
// freeOldLongTermRef
// Mark any long-term reference frame with LongTermFrameIdx greater
// than MaxLongTermFrameIdx as not used. When MaxLongTermFrameIdx is -1, this
// indicates no long-term frame indices and all long-term reference
// frames should be freed.
///////////////////////////////////////////////////////////////////////////////
H264DecoderFrame * H264DBPList::freeOldLongTermRef(Ipp32s  MaxLongTermFrameIdx, NotifiersChain * notify)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr->isLongTermRef(0) && (pCurr->LongTermFrameIdx() > MaxLongTermFrameIdx))
        {
            pCurr->unSetisLongTermRef(0);
            pCurr->unSetisLongTermRef(1);

            OnSlideWindow(pCurr, notify);
        }

        pCurr = pCurr->future();
    }

    return 0;
    // OK to not find any to free

}    // freeOldLongTermRef

///////////////////////////////////////////////////////////////////////////////
// changeSTtoLTRef
//    Mark the short-term reference frame with specified PicNum as long-term
//  with specified long term idx.
///////////////////////////////////////////////////////////////////////////////
void H264DBPList::changeSTtoLTRef(Ipp32s  picNum, Ipp32s  longTermFrameIdx)
{
    H264DecoderFrame *pCurr = m_pHead;
    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef >= FRM_STRUCTURE)
        {
            if (pCurr->isShortTermRef() && (pCurr->PicNum(0) == picNum))
            {
                pCurr->unSetisShortTermRef(0);
                pCurr->setLongTermFrameIdx(longTermFrameIdx);
                pCurr->SetisLongTermRef(0);
                pCurr->UpdateLongTermPicNum(2);
                return;
            }
        }
        else
        {
            if (pCurr->isShortTermRef(0) && (pCurr->PicNum(0) == picNum))
            {
                pCurr->unSetisShortTermRef(0);
                pCurr->setLongTermFrameIdx(longTermFrameIdx);
                pCurr->SetisLongTermRef(0);
                pCurr->UpdateLongTermPicNum(pCurr->m_bottom_field_flag[0]);
                return;
            }

            if (pCurr->isShortTermRef(1) && (pCurr->PicNum(1) == picNum))
            {
                pCurr->unSetisShortTermRef(1);
                pCurr->setLongTermFrameIdx(longTermFrameIdx);
                pCurr->SetisLongTermRef(1);
                pCurr->UpdateLongTermPicNum(pCurr->m_bottom_field_flag[1]);
                return;
            }
        }
        pCurr = pCurr->future();
    }

    //VM_ASSERT(false);    // No match found, should not happen.
    return;
}    // changeSTtoLTRef

H264DecoderFrame *H264DBPList::findShortTermPic(Ipp32s  picNum, Ipp32s * field)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef >= FRM_STRUCTURE)
        {
            if ((pCurr->isShortTermRef() == 3) && (pCurr->PicNum(0) == picNum))
            {
                return pCurr;
            }
        }
        else
        {
            if (pCurr->isShortTermRef(0) && (pCurr->PicNum(0) == picNum))
            {
                if (field)
                    *field = 0;
                return pCurr;
            }

            if (pCurr->isShortTermRef(1) && (pCurr->PicNum(1) == picNum))
            {
                if (field)
                    *field = 1;
                return pCurr;
            }
        }

        pCurr = pCurr->future();
    }

    //VM_ASSERT(false);    // No match found, should not happen.
    return 0;
}    // findShortTermPic

H264DecoderFrame *H264DBPList::findLongTermPic(Ipp32s  picNum, Ipp32s * field)
{
    H264DecoderFrame *pCurr = m_pHead;

    while (pCurr)
    {
        if (pCurr->m_PictureStructureForRef >= FRM_STRUCTURE)
        {
            if ((pCurr->isLongTermRef() == 3) && (pCurr->LongTermPicNum(0) == picNum))
            {
                return pCurr;
            }
        }
        else
        {
            if (pCurr->isLongTermRef(0) && (pCurr->LongTermPicNum(0) == picNum))
            {
                *field = 0;
                return pCurr;
            }

            if (pCurr->isLongTermRef(1) && (pCurr->LongTermPicNum(1) == picNum))
            {
                *field = 1;
                return pCurr;
            }
        }

        pCurr = pCurr->future();
    }

    //VM_ASSERT(false);    // No match found, should not happen.
    return 0;
}    // findLongTermPic

H264DecoderFrame * H264DBPList::FindClosest(H264DecoderFrame * pFrame)
{
    Ipp32s originalPOC = pFrame->PicOrderCnt(0, 3);
    Ipp32s originalResetCount = pFrame->RefPicListResetCount(0,3);

    H264DecoderFrame * pOldest = 0;

    Ipp32s  SmallestPicOrderCnt = 0;    // very large positive
    Ipp32s  SmallestRefPicListResetCount = 0x7fffffff;

    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->IsSkipped() || pTmp == pFrame || pTmp->GetBusyState() >= 2)
            continue;

        if (pTmp->m_chroma_format != pFrame->m_chroma_format ||
            pTmp->lumaSize().width != pFrame->lumaSize().width ||
            pTmp->lumaSize().height != pFrame->lumaSize().height)
            continue;

        if (pTmp->RefPicListResetCount(0,3) < SmallestRefPicListResetCount)
        {
            pOldest = pTmp;
            SmallestPicOrderCnt = pTmp->PicOrderCnt(0,3);
            SmallestRefPicListResetCount = pTmp->RefPicListResetCount(0,3);
        }
        else if (pTmp->RefPicListResetCount(0,3) == SmallestRefPicListResetCount)
        {
            if (pTmp->RefPicListResetCount(0,3) == originalResetCount)
            {
                if (SmallestRefPicListResetCount != originalResetCount)
                {
                    SmallestPicOrderCnt = 0x7fff;
                }

                if (abs(pTmp->PicOrderCnt(0,3) - originalPOC) < SmallestPicOrderCnt)
                {
                    pOldest = pTmp;
                    SmallestPicOrderCnt = pTmp->PicOrderCnt(0,3);
                    SmallestRefPicListResetCount = pTmp->RefPicListResetCount(0,3);
                }
            }
            else
            {
                if (pTmp->PicOrderCnt(0,3) > SmallestPicOrderCnt)
                {
                    pOldest = pTmp;
                    SmallestPicOrderCnt = pTmp->PicOrderCnt(0,3);
                    SmallestRefPicListResetCount = pTmp->RefPicListResetCount(0,3);
                }
            }
        }
    }

    return pOldest;
}

H264DecoderFrame * H264DBPList::FindByIndex(Ipp32s index)
{
    for (H264DecoderFrame * pTmp = m_pHead; pTmp; pTmp = pTmp->future())
    {
        if (pTmp->m_index == index)
            return pTmp;
    }

    return 0;
}

} // end namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
