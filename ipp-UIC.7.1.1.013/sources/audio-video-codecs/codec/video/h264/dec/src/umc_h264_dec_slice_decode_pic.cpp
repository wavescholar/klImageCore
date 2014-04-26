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

#include "umc_h264_slice_decoding.h"
#include "umc_h264_video_decoder.h"
#include "umc_h264_frame_list.h"

namespace UMC
{
struct H264RefListInfo
{
    Ipp32s m_iNumShortEntriesInList;
    Ipp32s m_iNumLongEntriesInList;
    Ipp32s m_iNumFramesInL0List;
    Ipp32s m_iNumFramesInL1List;
    Ipp32s m_iNumFramesInLTList;

    H264RefListInfo()
        : m_iNumShortEntriesInList(0)
        , m_iNumLongEntriesInList(0)
        , m_iNumFramesInL0List(0)
        , m_iNumFramesInL1List(0)
        , m_iNumFramesInLTList(0)
    {
    }
};

static H264DecoderFrame *FindLastValidReference(H264DecoderFrame **pList, Ipp32s iLength)
{
    Ipp32s i;
    H264DecoderFrame *pLast = NULL;

    for (i = 0; i < iLength; i += 1)
    {
        if (pList[i])
            pLast = pList[i];
    }

    return pLast;

} // H264DecoderFrame *FindLastValidReference(H264DecoderFrame **pList,


FactorArrayAFF H264Slice::m_StaticFactorArrayAFF;

Status H264Slice::UpdateReferenceList(H264DBPList *pDecoderFrameList)
{
    Status ps = UMC_OK;
    RefPicListReorderInfo *pReorderInfo_L0 = &ReorderInfoL0;
    RefPicListReorderInfo *pReorderInfo_L1 = &ReorderInfoL1;
    H264SeqParamSet *sps = GetSeqParam();
    Ipp32u uMaxFrameNum;
    Ipp32u uMaxPicNum;
    H264DecoderFrame *pFrm;
    H264DecoderFrame *pHead = pDecoderFrameList->head();
    //Ipp32u i;
    H264DecoderFrame **pRefPicList0;
    H264DecoderFrame **pRefPicList1;
    ReferenceFlags *pFields0;
    ReferenceFlags *pFields1;
    Ipp32u NumShortTermRefs, NumLongTermRefs;
    H264RefListInfo rli;
    H264DecoderFrame *(pLastInList[2]) = {NULL, NULL};

    if (m_SliceHeader.is_auxiliary)
    {
        pHead = GetAuxiliaryFrame(pHead);
    }

    VM_ASSERT(m_pCurrentFrame);

    pRefPicList0 = m_pCurrentFrame->GetRefPicList(m_iNumber, 0)->m_RefPicList;
    pRefPicList1 = m_pCurrentFrame->GetRefPicList(m_iNumber, 1)->m_RefPicList;
    pFields0 = m_pCurrentFrame->GetRefPicList(m_iNumber, 0)->m_Flags;
    pFields1 = m_pCurrentFrame->GetRefPicList(m_iNumber, 1)->m_Flags;

    // Spec reference: 8.2.4, "Decoding process for reference picture lists
    // construction"

    // get pointers to the picture and sequence parameter sets currently in use
    uMaxFrameNum = (1<<sps->log2_max_frame_num);
    uMaxPicNum = (m_SliceHeader.field_pic_flag == 0) ? uMaxFrameNum : uMaxFrameNum<<1;

    for (pFrm = pHead; pFrm; pFrm = pFrm->future())
    {
        // update FrameNumWrap and picNum if frame number wrap occurred,
        // for short-term frames
        // TBD: modify for fields
        pFrm->UpdateFrameNumWrap((Ipp32s)m_SliceHeader.frame_num,
            uMaxFrameNum,
            m_pCurrentFrame->m_PictureStructureForDec +
            m_SliceHeader.bottom_field_flag);

        // For long-term references, update LongTermPicNum. Note this
        // could be done when LongTermFrameIdx is set, but this would
        // only work for frames, not fields.
        // TBD: modify for fields
        pFrm->UpdateLongTermPicNum(m_pCurrentFrame->m_PictureStructureForDec +
                                   m_SliceHeader.bottom_field_flag);
    }

    pRefPicList0[0] = 0;
    pRefPicList1[0] = 0;

    if ((m_SliceHeader.slice_type != INTRASLICE) && (m_SliceHeader.slice_type != S_INTRASLICE))
    {
        for (Ipp32s number = 0; number <= MAX_NUM_REF_FRAMES + 1; number++)
        {
            pRefPicList0[number] = 0;
            pRefPicList1[number] = 0;
            pFields0[number].field = 0;
            pFields0[number].isShortReference = 0;
            pFields1[number].field = 0;
            pFields1[number].isShortReference = 0;
        }

        // Detect and report no available reference frames
        pDecoderFrameList->countActiveRefs(NumShortTermRefs, NumLongTermRefs);
        if ((NumShortTermRefs + NumLongTermRefs) == 0)
        {
            VM_ASSERT(0);
            ps = UMC_ERR_INVALID_STREAM;
        }

        if (ps == UMC_OK)
        {
            /*static int kkkk = 0;
            kkkk++;*/

            // Initialize the reference picture lists
            // Note the slice header get function always fills num_ref_idx_lx_active
            // fields with a valid value; either the override from the slice
            // header in the bitstream or the values from the pic param set when
            // there is no override.
            if ((m_SliceHeader.slice_type == PREDSLICE) ||
                (m_SliceHeader.slice_type == S_PREDSLICE))
            {
                InitPSliceRefPicList(m_SliceHeader.field_pic_flag != 0,
                                     m_SliceHeader.num_ref_idx_l0_active,
                                     pRefPicList0,
                                     pDecoderFrameList);

                pLastInList[0] = FindLastValidReference(pRefPicList0,
                                                        m_SliceHeader.num_ref_idx_l0_active);
            }
            else
            {
                //pRefPicList1 = m_pCurrentFrame->GetRefPicList(m_iNumber, 1)->m_RefPicList;
                InitBSliceRefPicLists(m_SliceHeader.field_pic_flag != 0, m_SliceHeader.num_ref_idx_l0_active,
                                      m_SliceHeader.num_ref_idx_l1_active,
                                      pRefPicList0, pRefPicList1,
                                      pDecoderFrameList,
                                      rli);

                pLastInList[0] = FindLastValidReference(pRefPicList0,
                                                        m_SliceHeader.num_ref_idx_l0_active);
                pLastInList[1] = FindLastValidReference(pRefPicList1,
                                                        m_SliceHeader.num_ref_idx_l1_active);
            }

            // Reorder the reference picture lists
            if (m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE)
            {
                rli.m_iNumFramesInL0List = AdjustRefPicListForFields(pRefPicList0, pFields0, rli);
            }

            if (BPREDSLICE == m_SliceHeader.slice_type)
            {
                if (m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE)
                {
                    rli.m_iNumFramesInL1List = AdjustRefPicListForFields(pRefPicList1, pFields1, rli);
                }

                if ((rli.m_iNumFramesInL0List == rli.m_iNumFramesInL1List) &&
                    (rli.m_iNumFramesInL0List > 1))
                {
                    bool isNeedSwap = true;
                    for (Ipp32s i = 0; i < rli.m_iNumFramesInL0List; i++)
                    {
                        if (pRefPicList1[i] != pRefPicList0[i] ||
                            pFields1[i].field != pFields0[i].field)
                        {
                            isNeedSwap = false;
                            break;
                        }
                    }

                    if (isNeedSwap)
                    {
                        swapValues(pRefPicList1[0], pRefPicList1[1]);
                        swapValues(pFields1[0], pFields1[1]);
                    }
                }
            }

            pRefPicList0[m_SliceHeader.num_ref_idx_l0_active] = 0;


            /*{
            FILE  * fl = fopen("d:/ipp.ref", "a+");
            fprintf(fl, "init - l0 - %d\n", kkkk);
            for (Ipp32s k1 = 0; k1 < m_SliceHeader.num_ref_idx_l0_active; k1++)
            {
                if (!pRefPicList0[k1])
                    break;
                fprintf(fl, "i - %d, field - %d, poc %d \n", k1, pFields0[k1].field, pRefPicList0[k1]->PicOrderCnt(pRefPicList0[k1]->GetNumberByParity(pFields0[k1].field)));
            }

            fprintf(fl, "l1 - %d\n", kkkk);
            for (Ipp32s  k1 = 0; k1 < m_SliceHeader.num_ref_idx_l1_active; k1++)
            {
                if (!pRefPicList1[k1])
                    break;
                fprintf(fl, "i - %d, field - %d, poc %d \n", k1, pFields1[k1].field, pRefPicList1[k1]->PicOrderCnt(pRefPicList1[k1]->GetNumberByParity(pFields1[k1].field)));
            }

            fclose(fl);
        }*/

            if (pReorderInfo_L0->num_entries > 0)
                ReOrderRefPicList(m_SliceHeader.field_pic_flag != 0, pRefPicList0, pFields0, pReorderInfo_L0, uMaxPicNum, m_SliceHeader.num_ref_idx_l0_active, pDecoderFrameList);

            if (BPREDSLICE == m_SliceHeader.slice_type)
            {
                pRefPicList1[m_SliceHeader.num_ref_idx_l1_active] = 0;

                if (pReorderInfo_L1->num_entries > 0)
                    ReOrderRefPicList(m_SliceHeader.field_pic_flag != 0, pRefPicList1, pFields1, pReorderInfo_L1, uMaxPicNum, m_SliceHeader.num_ref_idx_l1_active, pDecoderFrameList);
            }

            /*{kkkk++;
            FILE  * fl = fopen("d:/ipp.ref", "a+");
            fprintf(fl, "reorder - l0 - %d\n", kkkk);
            for (Ipp32s k1 = 0; k1 < m_SliceHeader.num_ref_idx_l0_active; k1++)
            {
                fprintf(fl, "i - %d, field - %d, poc %d \n", k1, pFields0[k1].field, pRefPicList0[k1]->PicOrderCnt(pRefPicList0[k1]->GetNumberByParity(pFields0[k1].field)));
            }

            fprintf(fl, "l1 - %d\n", kkkk);
            for (Ipp32s  k1 = 0; k1 < m_SliceHeader.num_ref_idx_l1_active; k1++)
            {
                fprintf(fl, "i - %d, field - %d, poc %d \n", k1, pFields1[k1].field, pRefPicList1[k1]->PicOrderCnt(pRefPicList1[k1]->GetNumberByParity(pFields1[k1].field)));
            }

            fclose(fl);
            }*/

            // set absent references
            {
                Ipp32s i;
                Ipp32s iCurField = 1;

                for (i = 0; i < m_SliceHeader.num_ref_idx_l0_active; i += 1)
                {
                    if (NULL == pRefPicList0[i])
                    {
                        pRefPicList0[i] = pLastInList[0];
                        pFields0[i].field = (Ipp8s) (iCurField ^= 1);
                        pFields0[i].isShortReference = 1;
                        m_pCurrentFrame->SetError(1);
                    }
                    else
                    {
                        m_pCurrentFrame->SetError(pRefPicList0[i]->IsFrameExist() ? 0 : 1);
                        pFields0[i].isShortReference =
                            pRefPicList0[i]->isShortTermRef(pRefPicList0[i]->GetNumberByParity(pFields0[i].field));
                    }
                }

                if (BPREDSLICE == m_SliceHeader.slice_type)
                {
                    iCurField = 1;

                    for (i = 0; i < m_SliceHeader.num_ref_idx_l1_active; i += 1)
                    {
                        if (NULL == pRefPicList1[i])
                        {
                            pRefPicList1[i] = pLastInList[1];
                            pFields1[i].field = (Ipp8s) (iCurField ^= 1);
                            pFields1[i].isShortReference = 1;
                            m_pCurrentFrame->SetError(1);
                        }
                        else
                        {
                            m_pCurrentFrame->SetError(pRefPicList1[i]->IsFrameExist() ? 0 : 1);
                            pFields1[i].isShortReference =
                                pRefPicList1[i]->isShortTermRef(pRefPicList1[i]->GetNumberByParity(pFields1[i].field));
                        }

                    }
                }
            }

            // If B slice, init scaling factors array
            if ((BPREDSLICE == m_SliceHeader.slice_type) && (pRefPicList1[0] != NULL))
                InitDistScaleFactor(
                        m_SliceHeader.num_ref_idx_l0_active,
                        m_SliceHeader.num_ref_idx_l1_active,
                        pRefPicList0, pRefPicList1, pFields0, pFields1);
        }
    }

    return ps;

} // Status H264Slice::UpdateRefPicList(H264DecoderFrameList *pDecoderFrameList)

void H264Slice::InitPSliceRefPicList(bool bIsFieldSlice,
                                     Ipp32s /*NumL0RefActive*/,
                                     H264DecoderFrame **pRefPicList,
                                     H264DecoderFrameList *pDecoderFrameList)
{
    Ipp32s j, k;
    Ipp32s NumFramesInList;
    H264DecoderFrame *pHead = pDecoderFrameList->head();
    H264DecoderFrame *pFrm;
    Ipp32s picNum;
    bool bError = false;

    VM_ASSERT(pRefPicList);

    if (m_SliceHeader.is_auxiliary)
    {
        pHead = GetAuxiliaryFrame(pHead);
    }

    NumFramesInList = 0;

    if (!bIsFieldSlice)
    {
        // Frame. Ref pic list ordering: Short term largest pic num to
        // smallest, followed by long term, largest long term pic num to
        // smallest. Note that ref pic list has one extra slot to assist
        // with re-ordering.
        for (pFrm = pHead; pFrm; pFrm = pFrm->future())
        {
            if (pFrm->isShortTermRef()==3)
            {
                // add to ordered list
                picNum = pFrm->PicNum(0);

                // find insertion point
                j=0;
                while (j<NumFramesInList &&
                        pRefPicList[j]->isShortTermRef() &&
                        pRefPicList[j]->PicNum(0) > picNum)
                    j++;

                // make room if needed
                if (pRefPicList[j])
                {
                    for (k=NumFramesInList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        pRefPicList[k] = pRefPicList[k-1];
                    }
                }

                // add the short-term reference
                pRefPicList[j] = pFrm;
                NumFramesInList++;
            }
            else if (pFrm->isLongTermRef()==3)
            {
                // add to ordered list
                picNum = pFrm->LongTermPicNum(0,3);

                // find insertion point
                j=0;
                // Skip past short-term refs and long term refs with smaller
                // long term pic num
                while (j<NumFramesInList &&
                        (pRefPicList[j]->isShortTermRef() ||
                        (pRefPicList[j]->isLongTermRef() &&
                        pRefPicList[j]->LongTermPicNum(0,2) < picNum)))
                    j++;

                // make room if needed
                if (pRefPicList[j])
                {
                    for (k=NumFramesInList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        pRefPicList[k] = pRefPicList[k-1];
                    }
                }

                // add the long-term reference
                pRefPicList[j] = pFrm;
                NumFramesInList++;
            }
            if (bError) break;
        }
    }
    else
    {
        // TBD: field
        for (pFrm = pHead; pFrm; pFrm = pFrm->future())
        {
            if (pFrm->isShortTermRef())
            {
                // add to ordered list
                picNum = pFrm->FrameNumWrap();

                // find insertion point
                j=0;
                while (j<NumFramesInList &&
                    pRefPicList[j]->isShortTermRef() &&
                    pRefPicList[j]->FrameNumWrap() > picNum)
                    j++;

                // make room if needed
                if (pRefPicList[j])
                {
                    for (k=NumFramesInList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        pRefPicList[k] = pRefPicList[k-1];
                    }
                }

                // add the short-term reference
                pRefPicList[j] = pFrm;
                NumFramesInList++;
            }
            else if (pFrm->isLongTermRef())
            {
                // long term reference
                picNum = pFrm->LongTermPicNum(0,2);

                // find insertion point
                j=0;
                // Skip past short-term refs and long term refs with smaller
                // long term pic num
                while (j<NumFramesInList &&
                    (pRefPicList[j]->isShortTermRef() ||
                    (pRefPicList[j]->isLongTermRef() &&
                    pRefPicList[j]->LongTermPicNum(0,2) < picNum)))
                    j++;

                // make room if needed
                if (pRefPicList[j])
                {
                    for (k=NumFramesInList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        pRefPicList[k] = pRefPicList[k-1];
                    }
                }

                // add the long-term reference
                pRefPicList[j] = pFrm;
                NumFramesInList++;
            }
            if (bError) break;
        }

    }

    // If the number of reference pictures on the L0 list is greater than the
    // number of active references, discard the "extras".
    //I realy don't know why...
    /*if (NumFramesInList > NumL0RefActive)
    {
        for (i=NumFramesInList-1; i>=NumL0RefActive; i--)
            pRefPicList[i] = NULL;
    }*/

} // void H264Slice::InitPSliceRefPicList(bool bIsFieldSlice,

void H264Slice::InitBSliceRefPicLists(bool bIsFieldSlice,
                                      Ipp32s /*NumL0RefActive*/,
                                      Ipp32s /*NumL1RefActive*/,
                                      H264DecoderFrame **pRefPicList0,
                                      H264DecoderFrame **pRefPicList1,
                                      H264DecoderFrameList *pDecoderFrameList,
                                      H264RefListInfo &rli)
{
    Ipp32s i, j, k;
    Ipp32s NumFramesInL0List;
    Ipp32s NumFramesInL1List;
    Ipp32s NumFramesInLTList;
    H264DecoderFrame *pHead = pDecoderFrameList->head();
    H264DecoderFrame *pFrm;
    Ipp32s FrmPicOrderCnt;
    H264DecoderFrame *LTRefPicList[MAX_NUM_REF_FRAMES];    // temp storage for long-term ordered list
    Ipp32s LongTermPicNum;
    bool bError = false;

    if (m_SliceHeader.is_auxiliary)
    {
        pHead = GetAuxiliaryFrame(pHead);
    }

    for (i=0; i<MAX_NUM_REF_FRAMES; i++)
    {
        LTRefPicList[i] = 0;
    }

    NumFramesInL0List = 0;
    NumFramesInL1List = 0;
    NumFramesInLTList = 0;

    if (!bIsFieldSlice)
    {
        Ipp32s CurrPicOrderCnt = m_pCurrentFrame->PicOrderCnt(0);
        // Short term references:
        // Need L0 and L1 lists. Both contain 2 sets of reference frames ordered
        // by PicOrderCnt. The "previous" set contains the reference frames with
        // a PicOrderCnt < current frame. The "future" set contains the reference
        // frames with a PicOrderCnt > current frame. In both cases the ordering
        // is from closest to current frame to farthest. L0 has the previous set
        // followed by the future set; L1 has the future set followed by the previous set.
        // Accomplish this by one pass through the decoded frames list creating
        // the ordered previous list in the L0 array and the ordered future list
        // in the L1 array. Then copy from both to the other for the second set.

        // Long term references:
        // The ordered list is the same for L0 and L1, is ordered by ascending
        // LongTermPicNum. The ordered list is created using local temp then
        // appended to the L0 and L1 lists after the short-term references.

        for (pFrm = pHead; pFrm; pFrm = pFrm->future())
        {
            if (pFrm->isShortTermRef()==3)
            {
                // add to ordered list
                FrmPicOrderCnt = pFrm->PicOrderCnt(0,3);

                if (FrmPicOrderCnt < CurrPicOrderCnt)
                {
                    // Previous reference to L0, order large to small
                    j=0;
                    while (j < NumFramesInL0List &&
                        (pRefPicList0[j]->PicOrderCnt(0,3) > FrmPicOrderCnt))
                        j++;

                    // make room if needed
                    if (pRefPicList0[j])
                    {
                        for (k = NumFramesInL0List; k > j; k--)
                        {
                            // Avoid writing beyond end of list
                            if (k > MAX_NUM_REF_FRAMES-1)
                            {
                                VM_ASSERT(0);
                                bError = true;
                                break;
                            }
                            pRefPicList0[k] = pRefPicList0[k-1];
                        }
                    }

                    // add the short-term reference
                    pRefPicList0[j] = pFrm;
                    NumFramesInL0List++;
                }
                else
                {
                    // Future reference to L1, order small to large
                    j=0;
                    while (j<NumFramesInL1List &&
                            pRefPicList1[j]->PicOrderCnt(0,3) < FrmPicOrderCnt)
                        j++;

                    // make room if needed
                    if (pRefPicList1[j])
                    {
                        for (k=NumFramesInL1List; k>j; k--)
                        {
                            // Avoid writing beyond end of list
                            if (k > MAX_NUM_REF_FRAMES-1)
                            {
                                VM_ASSERT(0);
                                bError = true;
                                break;
                            }
                            pRefPicList1[k] = pRefPicList1[k-1];
                        }
                    }

                    // add the short-term reference
                    pRefPicList1[j] = pFrm;
                    NumFramesInL1List++;
                }
            }    // short-term B
            else if (pFrm->isLongTermRef()==3)
            {
                // long term reference
                LongTermPicNum = pFrm->LongTermPicNum(0,3);

                // order smallest to largest
                j=0;
                while (j<NumFramesInLTList &&
                        LTRefPicList[j]->LongTermPicNum(0) < LongTermPicNum)
                    j++;

                // make room if needed
                if (LTRefPicList[j])
                {
                    for (k=NumFramesInLTList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        LTRefPicList[k] = LTRefPicList[k-1];
                    }
                }

                // add the long-term reference
                LTRefPicList[j] = pFrm;
                NumFramesInLTList++;

            }    // long term reference

            if (bError) break;

        }    // for pFrm

        if ((NumFramesInL0List + NumFramesInL1List + NumFramesInLTList) < MAX_NUM_REF_FRAMES)
        {
            // Complete L0 and L1 lists
            // Add future short-term references to L0 list, after previous
            for (i=0; i<NumFramesInL1List; i++)
                pRefPicList0[NumFramesInL0List+i] = pRefPicList1[i];

            // Add previous short-term references to L1 list, after future
            for (i=0; i<NumFramesInL0List; i++)
                pRefPicList1[NumFramesInL1List+i] = pRefPicList0[i];

            // Add long term list to both L0 and L1
            for (i=0; i<NumFramesInLTList; i++)
            {
                pRefPicList0[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
                pRefPicList1[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
            }

            // Special rule: When L1 has more than one entry and L0 == L1, all entries,
            // swap the first two entries of L1.
            // They can be equal only if there are no future or no previous short-term
            // references.
            if ((NumFramesInL0List == 0 || NumFramesInL1List == 0) &&
                ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) > 1))
            {
                pRefPicList1[0] = pRefPicList0[1];
                pRefPicList1[1] = pRefPicList0[0];
            }
        }
        else
        {
            // too many reference frames
            VM_ASSERT(0);
        }

    }    // not field slice
    else
    {
        Ipp32s CurrPicOrderCnt = m_pCurrentFrame->PicOrderCnt(m_pCurrentFrame->GetNumberByParity(m_SliceHeader.bottom_field_flag));
        // Short term references:
        // Need L0 and L1 lists. Both contain 2 sets of reference frames ordered
        // by PicOrderCnt. The "previous" set contains the reference frames with
        // a PicOrderCnt < current frame. The "future" set contains the reference
        // frames with a PicOrderCnt > current frame. In both cases the ordering
        // is from closest to current frame to farthest. L0 has the previous set
        // followed by the future set; L1 has the future set followed by the previous set.
        // Accomplish this by one pass through the decoded frames list creating
        // the ordered previous list in the L0 array and the ordered future list
        // in the L1 array. Then copy from both to the other for the second set.

        // Long term references:
        // The ordered list is the same for L0 and L1, is ordered by ascending
        // LongTermPicNum. The ordered list is created using local temp then
        // appended to the L0 and L1 lists after the short-term references.

        for (pFrm = pHead; pFrm; pFrm = pFrm->future())
        {
            if (pFrm->isShortTermRef())
            {
                // add to ordered list
                FrmPicOrderCnt = pFrm->PicOrderCnt(0,2);//returns POC of reference field (or min if both are reference)

                if (FrmPicOrderCnt <= CurrPicOrderCnt)
                {
                    // Previous reference to L0, order large to small
                    j=0;
                    while (j < NumFramesInL0List &&
                        (pRefPicList0[j]->PicOrderCnt(0,2) > FrmPicOrderCnt))
                        j++;

                    // make room if needed
                    if (pRefPicList0[j])
                    {
                        for (k = NumFramesInL0List; k > j; k--)
                        {
                            // Avoid writing beyond end of list
                            if (k > MAX_NUM_REF_FRAMES-1)
                            {
                                VM_ASSERT(0);
                                bError = true;
                                break;
                            }
                            pRefPicList0[k] = pRefPicList0[k-1];
                        }
                    }
                    // add the short-term reference
                    pRefPicList0[j] = pFrm;
                    NumFramesInL0List++;
                }
                else
                {
                    // Future reference to L1, order small to large
                    j=0;
                    while (j<NumFramesInL1List &&
                        pRefPicList1[j]->PicOrderCnt(0,2) < FrmPicOrderCnt)
                        j++;

                    // make room if needed
                    if (pRefPicList1[j])
                    {
                        for (k=NumFramesInL1List; k>j; k--)
                        {
                            // Avoid writing beyond end of list
                            if (k > MAX_NUM_REF_FRAMES-1)
                            {
                                VM_ASSERT(0);
                                bError = true;
                                break;
                            }
                            pRefPicList1[k] = pRefPicList1[k-1];
                        }
                    }

                    // add the short-term reference
                    pRefPicList1[j] = pFrm;
                    NumFramesInL1List++;
                }
            }    // short-term B
            else if (pFrm->isLongTermRef())
            {
                // long term reference
                LongTermPicNum = pFrm->LongTermPicNum(0,2);

                // order smallest to largest
                j=0;
                while (j < NumFramesInLTList &&
                    LTRefPicList[j]->LongTermPicNum(0,2) < LongTermPicNum)
                    j++;

                // make room if needed
                if (LTRefPicList[j])
                {
                    for (k=NumFramesInLTList; k>j; k--)
                    {
                        // Avoid writing beyond end of list
                        if (k > MAX_NUM_REF_FRAMES-1)
                        {
                            VM_ASSERT(0);
                            bError = true;
                            break;
                        }
                        LTRefPicList[k] = LTRefPicList[k-1];
                    }
                }

                // add the long-term reference
                LTRefPicList[j] = pFrm;
                NumFramesInLTList++;

            }    // long term reference

            if (bError) break;
        }    // for pFrm

        if ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) < MAX_NUM_REF_FRAMES)
        {
            // Complete L0 and L1 lists
            // Add future short-term references to L0 list, after previous
            for (i=0; i<NumFramesInL1List; i++)
                pRefPicList0[NumFramesInL0List+i] = pRefPicList1[i];


            // Add previous short-term references to L1 list, after future
            for (i=0; i<NumFramesInL0List; i++)
                pRefPicList1[NumFramesInL1List+i] = pRefPicList0[i];


            // Add long term list to both L0 and L1
            for (i=0; i<NumFramesInLTList; i++)
            {
                pRefPicList0[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
                pRefPicList1[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
            }

            // Special rule: When L1 has more than one entry and L0 == L1, all entries,
            // swap the first two entries of L1.
            // They can be equal only if there are no future or no previous short-term
            // references. For fields will be done later.
            /*
            if ((NumFramesInL0List == 0 || NumFramesInL1List == 0) &&
                ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) > 1))
            {
                pRefPicList1[0] = pRefPicList0[1];
                pRefPicList1[1] = pRefPicList0[0];
            }*/
        }
        else
        {
            // too many reference frames
            VM_ASSERT(0);
        }

    }

    rli.m_iNumFramesInL0List = NumFramesInL0List;
    rli.m_iNumFramesInL1List = NumFramesInL1List;
    rli.m_iNumFramesInLTList = NumFramesInLTList;

} // void H264Slice::InitBSliceRefPicLists(bool bIsFieldSlice,

#define CalculateDSF(index, value, value_mv)                                                     \
        /* compute scaling ratio for temporal direct and implicit weighting*/   \
        tb = picCntCur - picCntRef0;    /* distance from previous */            \
        td = picCntRef1 - picCntRef0;    /* distance between ref0 and ref1 */   \
                                                                                \
        /* special rule: if td is 0 or if L0 is long-term reference, use */     \
        /* L0 motion vectors and equal weighting.*/                             \
        if (td == 0 || pRefPicList0[index]->isLongTermRef(pRefPicList0[index]->GetNumberByParity(RefFieldTop)))  \
        {                                                                       \
            /* These values can be used directly in scaling calculations */     \
            /* to get back L0 or can use conditional test to choose L0.    */   \
            value = 128;    /* for equal weighting    */    \
            value_mv = 256;                                  \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            tb = IPP_MAX(-128,tb);                                              \
            tb = IPP_MIN(127,tb);                                               \
            td = IPP_MAX(-128,td);                                              \
            td = IPP_MIN(127,td);                                               \
                                                                                \
            VM_ASSERT(td != 0);                                                 \
                                                                                \
            tx = (16384 + abs(td/2))/td;                                        \
                                                                                \
            DistScaleFactor = (tb*tx + 32)>>6;                                  \
            DistScaleFactor = IPP_MAX(-1024, DistScaleFactor);                  \
            DistScaleFactor = IPP_MIN(1023, DistScaleFactor);                   \
                                                                                \
            if (isL1LongTerm || DistScaleFactor < -256 || DistScaleFactor > 512)                \
                value = 128;    /* equal weighting     */   \
            else                                                                \
                value = (FactorArrayValue)DistScaleFactor;                    \
                                                                                \
            value_mv = (FactorArrayValue)DistScaleFactor;                      \
        }

void H264Slice::InitDistScaleFactor(Ipp32s NumL0RefActive,
                                    Ipp32s NumL1RefActive,
                                    H264DecoderFrame **pRefPicList0,
                                    H264DecoderFrame **pRefPicList1,
                                    ReferenceFlags *pFields0,
                                    ReferenceFlags *pFields1)

{
    Ipp32s L0Index, L1Index;
    Ipp32s picCntRef0;
    Ipp32s picCntRef1;
    Ipp32s picCntCur;
    Ipp32s DistScaleFactor;
    FactorArrayValue *pDistScaleFactor;
    FactorArrayValue *pDistScaleFactorMV;

    Ipp32s tb;
    Ipp32s td;
    Ipp32s tx;

    VM_ASSERT(NumL0RefActive <= MAX_NUM_REF_FRAMES);
    VM_ASSERT(pRefPicList1[0]);

    bool isL1LongTerm = false;

    picCntCur = m_pCurrentFrame->PicOrderCnt(m_pCurrentFrame->GetNumberByParity(m_SliceHeader.bottom_field_flag));

    bool isNeedScale = (m_pPicParamSet->weighted_bipred_idc == 2);

    if (!isNeedScale)
        NumL1RefActive = 1;

    for (L1Index = NumL1RefActive - 1; L1Index >=0 ; L1Index--)
    {
        pDistScaleFactor = m_DistScaleFactor.values[L1Index];        //frames or fields
        pDistScaleFactorMV = m_DistScaleFactorMV.values;  //frames or fields

        Ipp32s RefField = m_pCurrentFrame->m_PictureStructureForDec >= FRM_STRUCTURE ?
            0 : GetReferenceField(pFields1, L1Index);

        picCntRef1 = pRefPicList1[L1Index]->PicOrderCnt(pRefPicList1[L1Index]->GetNumberByParity(RefField));
        isL1LongTerm = pRefPicList1[L1Index]->isLongTermRef(pRefPicList1[L1Index]->GetNumberByParity(RefField));

        for (L0Index = 0; L0Index < NumL0RefActive; L0Index++)
        {
            Ipp32s RefFieldTop = (m_pCurrentFrame->m_PictureStructureForDec >= FRM_STRUCTURE) ?
                0 : GetReferenceField(pFields0, L0Index);
            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(RefFieldTop));

            CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
        }
    }

    if (m_pCurrentFrame->m_PictureStructureForDec == AFRM_STRUCTURE)
    {
        if (!m_DistScaleFactorAFF && isNeedScale)
            m_DistScaleFactorAFF = (FactorArrayAFF*)m_pObjHeap->Allocate((FactorArrayAFF*)0);

        if (!m_DistScaleFactorMVAFF)
            m_DistScaleFactorMVAFF = (FactorArrayMVAFF*)m_pObjHeap->Allocate((FactorArrayMVAFF*)0);

        if (!isNeedScale)
        {
            m_DistScaleFactorAFF = &m_StaticFactorArrayAFF;
        }

        for (L1Index = NumL1RefActive - 1; L1Index >=0 ; L1Index--)
        {
            // [curmb field],[ref1field],[ref0field]
            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][0][0][0];      //complementary field pairs, cf=top r1=top,r0=top
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[0][0][0];  //complementary field pairs, cf=top r1=top,r0=top

            picCntCur = m_pCurrentFrame->PicOrderCnt(m_pCurrentFrame->GetNumberByParity(0), 1);
            picCntRef1 = pRefPicList1[L1Index]->PicOrderCnt(pRefPicList1[L1Index]->GetNumberByParity(0), 1);
            isL1LongTerm = pRefPicList1[L1Index]->isLongTermRef(pRefPicList1[L1Index]->GetNumberByParity(0));

            for (L0Index=0; L0Index<NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 0;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(0), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }

            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][0][1][0];        //complementary field pairs, cf=top r1=top,r0=bottom
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[0][1][0];  //complementary field pairs, cf=top r1=top,r0=bottom

            for (L0Index=0; L0Index < NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);
                Ipp32s RefFieldTop = 1;

                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(1), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }

            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][0][0][1];        //complementary field pairs, cf=top r1=top,r0=top
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[0][0][1];  //complementary field pairs, cf=top r1=top,r0=top

            picCntRef1 = pRefPicList1[L1Index]->PicOrderCnt(pRefPicList1[L1Index]->GetNumberByParity(1), 1);
            isL1LongTerm = pRefPicList1[L1Index]->isLongTermRef(pRefPicList1[L1Index]->GetNumberByParity(1));

            for (L0Index=0; L0Index<NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 0;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(0), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }

            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][0][1][1];        //complementary field pairs, cf=top r1=top,r0=bottom
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[0][1][1];  //complementary field pairs, cf=top r1=top,r0=bottom

            for (L0Index=0; L0Index < NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);
                Ipp32s RefFieldTop = 1;

                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(1),1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }

            /*--------------------------------------------------------------------*/
            picCntCur = m_pCurrentFrame->PicOrderCnt(m_pCurrentFrame->GetNumberByParity(1), 1);
            picCntRef1 = pRefPicList1[L1Index]->PicOrderCnt(pRefPicList1[L1Index]->GetNumberByParity(0), 1);
            isL1LongTerm = pRefPicList1[L1Index]->isLongTermRef(pRefPicList1[L1Index]->GetNumberByParity(0));

            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][1][0][0];        //complementary field pairs, cf=bottom r1=bottom,r0=top
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[1][0][0];  //complementary field pairs, cf=bottom r1=bottom,r0=top

            for (L0Index=0; L0Index<NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 0;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(0), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }
            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][1][1][0];       //complementary field pairs, cf=bottom r1=bottom,r0=bottom
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[1][1][0];   //complementary field pairs, cf=bottom r1=bottom,r0=bottom

            for (L0Index=0; L0Index < NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 1;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(1), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }

            picCntRef1 = pRefPicList1[L1Index]->PicOrderCnt(pRefPicList1[L1Index]->GetNumberByParity(1), 1);
            isL1LongTerm = pRefPicList1[L1Index]->isLongTermRef(pRefPicList1[L1Index]->GetNumberByParity(1));

            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][1][0][1];        //complementary field pairs, cf=bottom r1=bottom,r0=top
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[1][0][1];  //complementary field pairs, cf=bottom r1=bottom,r0=top

            for (L0Index=0; L0Index<NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 0;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(0), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }
            pDistScaleFactor = m_DistScaleFactorAFF->values[L1Index][1][1][1];       //complementary field pairs, cf=bottom r1=bottom,r0=bottom
            pDistScaleFactorMV = m_DistScaleFactorMVAFF->values[1][1][1];   //complementary field pairs, cf=bottom r1=bottom,r0=bottom

            for (L0Index=0; L0Index < NumL0RefActive; L0Index++)
            {
                VM_ASSERT(pRefPicList0[L0Index]);

                Ipp32s RefFieldTop = 1;
                picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(pRefPicList0[L0Index]->GetNumberByParity(1), 1);
                CalculateDSF(L0Index, pDistScaleFactor[L0Index], pDistScaleFactorMV[L0Index]);
            }
        }

        if (!isNeedScale)
        {
            m_DistScaleFactorAFF = 0;
        }
    }
} // void H264Slice::InitDistScaleFactor(Ipp32s NumL0RefActive,

Ipp32s H264Slice::AdjustRefPicListForFields(H264DecoderFrame **pRefPicList,
                                          ReferenceFlags *pFields,
                                          H264RefListInfo &rli)
{
    H264DecoderFrame *TempList[MAX_NUM_REF_FRAMES+1];
    Ipp8u TempFields[MAX_NUM_REF_FRAMES+1];
    //walk through list and set correct indices
    Ipp32s i=0,j=0,numSTR=0,numLTR=0;
    Ipp32s num_same_parity = 0, num_opposite_parity = 0;
    Ipp8s current_parity = m_SliceHeader.bottom_field_flag;
    //first scan the list to determine number of shortterm and longterm reference frames
    while ((numSTR < 16) && pRefPicList[numSTR] && pRefPicList[numSTR]->isShortTermRef()) numSTR++;
    while ((numSTR + numLTR < 16) && pRefPicList[numSTR+numLTR] && pRefPicList[numSTR+numLTR]->isLongTermRef()) numLTR++;

    while (num_same_parity < numSTR ||  num_opposite_parity < numSTR)
    {
        //try to fill shorttermref fields with the same parity first
        if (num_same_parity < numSTR)
        {
            while (num_same_parity < numSTR)
            {
                Ipp32s ref_field = pRefPicList[num_same_parity]->GetNumberByParity(current_parity);
                if (pRefPicList[num_same_parity]->isShortTermRef(ref_field))
                    break;

                num_same_parity++;

            }

            if (num_same_parity<numSTR)
            {
                TempList[i] = pRefPicList[num_same_parity];
                TempFields[i] = current_parity;
                i++;
                num_same_parity++;
            }
        }
        //now process opposite parity
        if (num_opposite_parity < numSTR)
        {
            while (num_opposite_parity < numSTR)
            {
                Ipp32s ref_field = pRefPicList[num_opposite_parity]->GetNumberByParity(!current_parity);
                if (pRefPicList[num_opposite_parity]->isShortTermRef(ref_field))
                    break;
                num_opposite_parity++;
            }

            if (num_opposite_parity<numSTR) //selected field is reference
            {
                TempList[i] = pRefPicList[num_opposite_parity];
                TempFields[i] = !current_parity;
                i++;
                num_opposite_parity++;
            }
        }
    }

    rli.m_iNumShortEntriesInList = (Ipp8u) i;
    num_same_parity = num_opposite_parity = 0;
    //now processing LongTermRef
    while(num_same_parity<numLTR ||  num_opposite_parity<numLTR)
    {
        //try to fill longtermref fields with the same parity first
        if (num_same_parity < numLTR)
        {
            while (num_same_parity < numLTR)
            {
                Ipp32s ref_field = pRefPicList[num_same_parity+numSTR]->GetNumberByParity(current_parity);
                if (pRefPicList[num_same_parity+numSTR]->isLongTermRef(ref_field))
                    break;
                num_same_parity++;
            }
            if (num_same_parity<numLTR)
            {
                TempList[i] = pRefPicList[num_same_parity+numSTR];
                TempFields[i] = current_parity;
                i++;
                num_same_parity++;
            }
        }
        //now process opposite parity
        if (num_opposite_parity<numLTR)
        {
            while (num_opposite_parity < numLTR)
            {
                Ipp32s ref_field = pRefPicList[num_opposite_parity+numSTR]->GetNumberByParity(!current_parity);

                if (pRefPicList[num_opposite_parity+numSTR]->isLongTermRef(ref_field))
                    break;
                num_opposite_parity++;
            }

            if (num_opposite_parity<numLTR) //selected field is reference
            {
                TempList[i] = pRefPicList[num_opposite_parity+numSTR];
                TempFields[i] = !current_parity;
                i++;
                num_opposite_parity++;
            }
        }
    }

    rli.m_iNumLongEntriesInList = (Ipp8u) (i - rli.m_iNumShortEntriesInList);
    j = 0;
    while(j < i)//copy data back to list
    {
        pRefPicList[j] = TempList[j];
        pFields[j].field = TempFields[j];
        pFields[j].isShortReference = (unsigned char) (pRefPicList[j]->isShortTermRef(pRefPicList[j]->GetNumberByParity(TempFields[j])) ? 1 : 0);
        j++;
    }

    while(j < MAX_NUM_REF_FRAMES)//fill remaining entries
    {
        pRefPicList[j] = NULL;
        pFields[j].field = 0;
        pFields[j].isShortReference = 0;
        j++;
    }

    return i;
} // Ipp32s H264Slice::AdjustRefPicListForFields(H264DecoderFrame **pRefPicList, Ipp8s *pFields)

void H264Slice::ReOrderRefPicList(bool bIsFieldSlice,
                                  H264DecoderFrame **pRefPicList,
                                  ReferenceFlags *pFields,
                                  RefPicListReorderInfo *pReorderInfo,
                                  Ipp32s MaxPicNum,
                                  Ipp32s NumRefActive,
                                  H264DBPList *pDecoderFrameList)
{
    Ipp32u i;
    Ipp32s picNumNoWrap;
    Ipp32s picNum;
    Ipp32s picNumPred;
    Ipp32s picNumCurr;

    // Reference: Reordering process for reference picture lists, 8.2.4.3
    if (!bIsFieldSlice)
    {
        picNumCurr = m_pCurrentFrame->PicNum(0,3);
        picNumPred = picNumCurr;

        for (i=0; i<pReorderInfo->num_entries; i++)
        {
            if (pReorderInfo->reordering_of_pic_nums_idc[i] < 2)
            {
                // short-term reorder
                if (pReorderInfo->reordering_of_pic_nums_idc[i] == 0)
                {
                    picNumNoWrap = picNumPred - pReorderInfo->reorder_value[i];
                    if (picNumNoWrap < 0)
                        picNumNoWrap += MaxPicNum;
                }
                else
                {
                    picNumNoWrap = picNumPred + pReorderInfo->reorder_value[i];
                    if (picNumNoWrap >= MaxPicNum)
                        picNumNoWrap -= MaxPicNum;
                }
                picNumPred = picNumNoWrap;

                picNum = picNumNoWrap;
                if (picNum > picNumCurr)
                    picNum -= MaxPicNum;

                H264DecoderFrame* pFrame = pDecoderFrameList->findShortTermPic(picNum, 0);

                for (Ipp32u k = NumRefActive; k > i; k--)
                {
                    pRefPicList[k] = pRefPicList[k - 1];
                    pFields[k] = pFields[k - 1];
                }

                // Place picture with picNum on list, shifting pictures
                // down by one while removing any duplication of picture with picNum.
                pRefPicList[i] = pFrame;
                pFields[i].field = 0;
                pFields[i].isShortReference = 1;
                Ipp32s refIdxLX = i + 1;

                for(Ipp32s kk = i + 1; kk <= NumRefActive; kk++)
                {
                    if (pRefPicList[kk])
                    {
                        if (!(pRefPicList[kk]->isShortTermRef(pRefPicList[kk]->GetNumberByParity(pFields[kk].field)) &&
                            pRefPicList[kk]->PicNum(pRefPicList[kk]->GetNumberByParity(pFields[kk].field), 1) == picNum))
                        {
                            pRefPicList[refIdxLX] = pRefPicList[kk];
                            pFields[refIdxLX] = pFields[kk];
                            refIdxLX++;
                        }
                    }
                }
            }    // short-term reorder
            else
            {
                // long term reorder
                picNum = pReorderInfo->reorder_value[i];

                H264DecoderFrame* pFrame = pDecoderFrameList->findLongTermPic(picNum, 0);

                for (Ipp32u k = NumRefActive; k > i; k--)
                {
                    pRefPicList[k] = pRefPicList[k - 1];
                    pFields[k] = pFields[k - 1];
                }

                // Place picture with picNum on list, shifting pictures
                // down by one while removing any duplication of picture with picNum.
                pRefPicList[i] = pFrame;
                pFields[i].field = 0;
                pFields[i].isShortReference = 0;
                Ipp32s refIdxLX = i + 1;

                for(Ipp32s kk = i + 1; kk <= NumRefActive; kk++)
                {
                    if (pRefPicList[kk])
                    {
                        if (!(pRefPicList[kk]->isLongTermRef(pRefPicList[kk]->GetNumberByParity(pFields[kk].field)) &&
                            pRefPicList[kk]->LongTermPicNum(pRefPicList[kk]->GetNumberByParity(pFields[kk].field), 1) == picNum))
                        {
                            pRefPicList[refIdxLX] = pRefPicList[kk];
                            pFields[refIdxLX] = pFields[kk];
                            refIdxLX++;
                        }
                    }
                }
            }    // long term reorder
        }    // for i
    }
    else
    {
        picNumCurr = m_pCurrentFrame->PicNum(m_pCurrentFrame->GetNumberByParity(m_SliceHeader.bottom_field_flag));
        picNumPred = picNumCurr;

        for (i=0; i<pReorderInfo->num_entries; i++)
        {
            if (pReorderInfo->reordering_of_pic_nums_idc[i] < 2)
            {
                // short-term reorder
                if (pReorderInfo->reordering_of_pic_nums_idc[i] == 0)
                {
                    picNumNoWrap = picNumPred - pReorderInfo->reorder_value[i];
                    if (picNumNoWrap < 0)
                        picNumNoWrap += MaxPicNum;
                }
                else
                {
                    picNumNoWrap = picNumPred + pReorderInfo->reorder_value[i];
                    if (picNumNoWrap >= MaxPicNum)
                        picNumNoWrap -= MaxPicNum;
                }
                picNumPred = picNumNoWrap;

                picNum = picNumNoWrap;
                if (picNum > picNumCurr)
                    picNum -= MaxPicNum;

                Ipp32s frameField;
                H264DecoderFrame* pFrame = pDecoderFrameList->findShortTermPic(picNum, &frameField);

                for (Ipp32u k = NumRefActive; k > i; k--)
                {
                    pRefPicList[k] = pRefPicList[k - 1];
                    pFields[k] = pFields[k - 1];
                }

                // Place picture with picNum on list, shifting pictures
                // down by one while removing any duplication of picture with picNum.
                pRefPicList[i] = pFrame;
                pFields[i].field = (char) (pFrame ? pFrame->m_bottom_field_flag[frameField] : 0);
                pFields[i].isShortReference = 1;
                Ipp32s refIdxLX = i + 1;

                for(Ipp32s kk = i + 1; kk <= NumRefActive; kk++)
                {
                    if (pRefPicList[kk])
                    {
                        if (!(pRefPicList[kk]->isShortTermRef(pRefPicList[kk]->GetNumberByParity(pFields[kk].field)) &&
                            pRefPicList[kk]->PicNum(pRefPicList[kk]->GetNumberByParity(pFields[kk].field), 1) == picNum))
                        {
                            pRefPicList[refIdxLX] = pRefPicList[kk];
                            pFields[refIdxLX] = pFields[kk];
                            refIdxLX++;
                        }
                    }
                }
            }    // short term reorder
            else
            {
                // long term reorder
                picNum = pReorderInfo->reorder_value[i];

                Ipp32s frameField;
                H264DecoderFrame* pFrame = pDecoderFrameList->findLongTermPic(picNum, &frameField);

                for (Ipp32u k = NumRefActive; k > i; k--)
                {
                    pRefPicList[k] = pRefPicList[k - 1];
                    pFields[k] = pFields[k - 1];
                }

                // Place picture with picNum on list, shifting pictures
                // down by one while removing any duplication of picture with picNum.
                pRefPicList[i] = pFrame;
                pFields[i].field = (char) (pFrame ? pFrame->m_bottom_field_flag[frameField] : 0);
                pFields[i].isShortReference = 0;
                Ipp32s refIdxLX = i + 1;

                for(Ipp32s kk = i + 1; kk <= NumRefActive; kk++)
                {
                    if (pRefPicList[kk])
                    {
                        if (!(pRefPicList[kk]->isLongTermRef(pRefPicList[kk]->GetNumberByParity(pFields[kk].field)) &&
                            pRefPicList[kk]->LongTermPicNum(pRefPicList[kk]->GetNumberByParity(pFields[kk].field), 1) == picNum))
                        {
                            pRefPicList[refIdxLX] = pRefPicList[kk];
                            pFields[refIdxLX] = pFields[kk];
                            refIdxLX++;
                        }
                    }
                }
            }    // long term reorder
        }    // for i
    }
} // void H264Slice::ReOrderRefPicList(bool bIsFieldSlice,

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
