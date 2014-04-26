//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef __UMC_H264_ENC_CPB_H__
#define __UMC_H264_ENC_CPB_H__

#include "umc_memory_allocator.h"
#include "umc_h264_defs.h"
#include "vm_debug.h"

template<typename PIXTYPE>
struct EncoderRefPicList;

template<typename PIXTYPE>
struct H264EncoderFrame
{
    // These point to the previous and future reference frames
    // used to decode this frame.
    // These are also used to maintain a doubly-linked list of
    // reference frames in the H264EncoderFrameList class.  So, these
    // may be non-NULL even if the current frame is an I frame,
    // for example.  m_pPreviousFrame will always be valid when
    // decoding a P or B frame, and m_pFutureFrame will always
    // be valid when decoding a B frame.

//public:
    VideoData m_data;

    Ipp64f pts_start;
    Ipp64f pts_end;

    // L0 and L1 refer to RefList 0 and RefList 1 in JVT spec.
    // In this implementation, L0 is Forward MVs in B slices, and all MVs in P slices
    // L1 is Backward MVs in B slices and unused in P slices.
    H264GlobalMacroblocksDescriptor m_mbinfo;
    H264GlobalMacroblocksDescriptor m_mbinfo_alpha;
    H264GlobalMacroblocksDescriptor m_mbinfo_prim;

    Ipp32u uWidth;
    Ipp32u uHeight;

    H264EncoderFrame* m_pPreviousFrame;
    H264EncoderFrame* m_pFutureFrame;
    bool                  m_wasEncoded;

    //Description of picture, NULL if no picture
    PIXTYPE                *m_pYPlane;
    PIXTYPE                *m_pUPlane;
    PIXTYPE                *m_pVPlane;
    //Frame Data
    IppiSize        m_lumaSize;
    Ipp32u          m_pitchPixels;
    Ipp32u          m_pitchBytes;
#ifdef FRAME_INTERPOLATION
    Ipp32s          m_PlaneSize;
#endif
#ifdef FRAME_TYPE_DETECT_DS
    PIXTYPE         *m_pYPlane_DS;
#endif
    Ipp8u            m_PictureStructureForRef;
    Ipp8u            m_PictureStructureForDec;
    EnumPicCodType   m_PicCodType;
    Ipp32s           m_RefPic;
    Ipp32s           totalMBs;

    // For type 1 calculation of m_PicOrderCnt. m_FrameNum is needed to
    // be used as previous frame num.

    Ipp32s           m_PicNum[2];
    Ipp32s           m_LongTermPicNum[2];
    Ipp32s           m_FrameNum;
    Ipp32s           m_FrameNumWrap;
    Ipp32s           m_LongTermFrameIdx;
    Ipp32s           m_RefPicListResetCount[2];
    Ipp32s           m_PicOrderCnt[2];    // Display order picture count mod MAX_PIC_ORDER_CNT.
    Ipp32u           m_PicOrderCounterAccumulated; // Display order picture counter, but with compensation of IDR POC resets.
    Ipp32s           m_crop_left;
    Ipp32s           m_crop_right;
    Ipp32s           m_crop_top;
    Ipp32s           m_crop_bottom;
    Ipp8s            m_crop_flag;
    bool             m_isShortTermRef[2];
    bool             m_isLongTermRef[2];

    //Ipp32u              m_FrameNum;            // Decode order frame label, from slice header
    Ipp8u            m_PQUANT;            // Picture QP (from first slice header)
    Ipp8u            m_PQUANT_S;            // Picture QS (from first slice header)
    IppiSize         m_dimensions;
    Ipp8u            m_bottom_field_flag[2];

    IppiSize m_paddedParsedFrameDataSize;
    Ipp32s alpha_plane; // Is there alpha plane (>=0) and its number

#ifdef FRAME_QP_FROM_FILE
    int frame_qp;
#endif

    bool   m_bIsIDRPic;
    Ipp32s numMBs;
    Ipp32s numIntraMBs;
    // Read from slice NAL unit of current picture. True indicates the
    // picture contains only I or SI slice types.

    EncoderRefPicList<PIXTYPE> *m_pRefPicList;

//private:
    MemoryAllocator* memAlloc;
    MemID            frameDataID;
    MemID            mbsDataID;
    MemID            refListDataID;
    Ipp8u           *frameData;
    Ipp8u           *mbsData;
    Ipp8u           *refListData;

    Ipp32s m_NumSlices;   // Number of slices.
    IppiSize m_macroBlockSize;
    Ipp32s m_isAuxiliary;    //Do we compress auxiliary channel?

};

// Struct containing list 0 and list 1 reference picture lists for one slice.
// Length is plus 1 to provide for null termination.
template<typename PIXTYPE>
struct EncoderRefPicListStruct
{
    H264EncoderFrame<PIXTYPE> *m_RefPicList[MAX_NUM_REF_FRAMES + 1];
    Ipp8s             m_Prediction[MAX_NUM_REF_FRAMES + 1];
    Ipp32s            m_POC[MAX_NUM_REF_FRAMES+1];
};

template<typename PIXTYPE>
struct EncoderRefPicList
{
    EncoderRefPicListStruct<PIXTYPE> m_RefPicListL0;
    EncoderRefPicListStruct<PIXTYPE> m_RefPicListL1;
};

// The above variables are used for management of reference frames
// on reference picture lists maintained in m_RefPicList. They are
// updated as reference picture management information is decoded
// from the bitstream. The picture and frame number variables determine
// reference picture ordering on the lists.
template<typename PIXTYPE>
void H264EncoderFrame_exchangeFrameYUVPointers(
    H264EncoderFrame<PIXTYPE>* frame1,
    H264EncoderFrame<PIXTYPE>* frame2);

template<typename PIXTYPE>
Status H264EncoderFrame_Create(
    H264EncoderFrame<PIXTYPE>* state,
    VideoData* in,
    MemoryAllocator *pMemAlloc
    , Ipp32s alpha
    , Ipp32s downScale/* = 0*/);

template<typename PIXTYPE>
void H264EncoderFrame_Destroy(
    H264EncoderFrame<PIXTYPE>* state);

template<typename PIXTYPE>
Status H264EncoderFrame_allocate(
    H264EncoderFrame<PIXTYPE>* state,
    const IppiSize& paddedSize,
    Ipp32s num_slices);

// A decoded frame can be "disposed" if it is not an active reference
// and it is not locked by the calling application and it has been
// output for display.
template<typename PIXTYPE>
inline
bool H264EncoderFrame_isDisposable(
    H264EncoderFrame<PIXTYPE>* state)
{
    return (!state->m_isShortTermRef[0] &&
            !state->m_isShortTermRef[1] &&
            !state->m_isLongTermRef[0] &&
            !state->m_isLongTermRef[1] &&
             state->m_wasEncoded);
}

template<typename PIXTYPE>
inline
bool H264EncoderFrame_isShortTermRef1(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE )
        return state->m_isShortTermRef[0] && state->m_isShortTermRef[1];
    else
        return state->m_isShortTermRef[WhichField];
}

template<typename PIXTYPE>
inline
Ipp8u H264EncoderFrame_isShortTermRef0(
    H264EncoderFrame<PIXTYPE>* state)
{
    return state->m_isShortTermRef[0] + state->m_isShortTermRef[1]*2;
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_PicNum(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s f,
    Ipp32s force/* = 0*/)
{
    if ((state->m_PictureStructureForRef>=FRM_STRUCTURE && force==0) || force==3)
    {
        return MIN(state->m_PicNum[0],state->m_PicNum[1]);
    }
    else if (force==2)
    {
        if (H264EncoderFrame_isShortTermRef1(state, 0) &&
            H264EncoderFrame_isShortTermRef1(state, 1))
            return MIN(state->m_PicNum[0],state->m_PicNum[1]);
        else if (H264EncoderFrame_isShortTermRef1(state, 0))
            return state->m_PicNum[0];
        else
            return state->m_PicNum[0];
    }

    return state->m_PicNum[f];
}

template<typename PIXTYPE>
inline
void H264EncoderFrame_setPicNum(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s PicNum,
    Ipp8u f)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE)
    {
        state->m_PicNum[0] = state->m_PicNum[1] = PicNum;
    }
    else
        state->m_PicNum[f] = PicNum;
}

// Updates m_FrameNumWrap and m_PicNum if the frame is a short-term
// reference and a frame number wrap has occurred.
template<typename PIXTYPE>
void H264EncoderFrame_UpdateFrameNumWrap(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s CurrFrameNum,
    Ipp32s MaxFrameNum,
    Ipp32s CurrPicStruct);

template<typename PIXTYPE>
inline
void H264EncoderFrame_SetisShortTermRef(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE)
        state->m_isShortTermRef[0] = state->m_isShortTermRef[1] = true;
    else
        state->m_isShortTermRef[WhichField] = true;
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_PicOrderCnt(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s index,
    Ipp32s force/* = 0*/)
{
    if ((state->m_PictureStructureForRef>=FRM_STRUCTURE && force==0) || force==3)
    {
        return MIN(state->m_PicOrderCnt[0], state->m_PicOrderCnt[1]);
    }
    else if (force==2)
    {
        if (H264EncoderFrame_isShortTermRef1(state, 0) &&
            H264EncoderFrame_isShortTermRef1(state, 1))
            return MIN(state->m_PicOrderCnt[0], state->m_PicOrderCnt[1]);
        else if (H264EncoderFrame_isShortTermRef1(state, 0))
            return state->m_PicOrderCnt[0];
        else
            return state->m_PicOrderCnt[1];
    }
    return state->m_PicOrderCnt[index];
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_DeblockPicID(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s index)
{
#if 0
    //the constants are subject to change
    return PicOrderCnt(index,force)*2+FrameNumWrap()*534+FrameNum()*878+PicNum(index,force)*14
        +RefPicListResetCount(index,force);
#else
    size_t ret = (size_t)state;
    return (Ipp32s)(ret + index);

#endif
}

template<typename PIXTYPE>
inline
bool H264EncoderFrame_isLongTermRef1(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE)
        return state->m_isLongTermRef[0] && state->m_isLongTermRef[1];
    else
        return state->m_isLongTermRef[WhichField];
}

template<typename PIXTYPE>
inline
Ipp8u H264EncoderFrame_isLongTermRef0(
    H264EncoderFrame<PIXTYPE>* state)
{
    return state->m_isLongTermRef[0] + state->m_isLongTermRef[1]*2;
}

template<typename PIXTYPE>
inline
void H264EncoderFrame_SetisLongTermRef(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE)
        state->m_isLongTermRef[0] = state->m_isLongTermRef[1] = true;
    else
        state->m_isLongTermRef[WhichField] = true;
}

template<typename PIXTYPE>
inline
void H264EncoderFrame_unSetisShortTermRef(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE){
        state->m_isShortTermRef[0] = state->m_isShortTermRef[1] = false;
    }else
        state->m_isShortTermRef[WhichField] = false;
}

template<typename PIXTYPE>
inline
void H264EncoderFrame_unSetisLongTermRef(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s WhichField)
{
    if (state->m_PictureStructureForRef>=FRM_STRUCTURE){
        state->m_isLongTermRef[0] = state->m_isLongTermRef[1] = false;
    }else
        state->m_isLongTermRef[WhichField] = false;
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_LongTermPicNum(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s f,
    Ipp32s force/* = 0*/)
{
    if ((state->m_PictureStructureForRef>=FRM_STRUCTURE && force==0) || force==3){
        return MIN(state->m_LongTermPicNum[0], state->m_LongTermPicNum[1]);
    }else if (force==2){
        if (H264EncoderFrame_isLongTermRef1(state, 0) &&
            H264EncoderFrame_isLongTermRef1(state, 1))
            return MIN(state->m_LongTermPicNum[0], state->m_LongTermPicNum[1]);
        else if (H264EncoderFrame_isShortTermRef1(state, 0))
            return state->m_LongTermPicNum[0];
        else
            return state->m_LongTermPicNum[0];
    }
    return state->m_LongTermPicNum[f];
}

template<typename PIXTYPE>
void H264EncoderFrame_UpdateLongTermPicNum(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s CurrPicStruct);

template<typename PIXTYPE>
inline
void H264EncoderFrame_IncreaseRefPicListResetCount(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s f)
{
    /*if (state->m_PictureStructureForRef>=FRM_STRUCTURE)
    {
    state->m_RefPicListResetCount[0]++;
    state->m_RefPicListResetCount[1]++;
    }
    else*/
    state->m_RefPicListResetCount[f]++;
}

template<typename PIXTYPE>
inline
void H264EncoderFrame_InitRefPicListResetCount(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s f)
{
    if (state->m_PictureStructureForRef >= FRM_STRUCTURE) {
        // Should reconsider keeping this branch.
        state->m_RefPicListResetCount[0] = state->m_RefPicListResetCount[1] = 0;
    }
    else
        state->m_RefPicListResetCount[f] = 0;
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_RefPicListResetCount(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s f,
    Ipp32s force/* = 0*/)
{
    if ((state->m_PictureStructureForRef>=FRM_STRUCTURE && force==0)|| force==3)
        return MAX(state->m_RefPicListResetCount[0], state->m_RefPicListResetCount[1]);
    else
        return state->m_RefPicListResetCount[f];
}

template<typename PIXTYPE>
inline
Ipp32s H264EncoderFrame_GetNumberByParity(
    H264EncoderFrame<PIXTYPE>* state,
    Ipp32s parity)
{
    if (parity == -1)
        return -1;
    if (state->m_bottom_field_flag[0] == parity)
        return 0;
    if (state->m_bottom_field_flag[1] == parity)
        return 1;

    VM_ASSERT(state->m_PictureStructureForRef >= FRM_STRUCTURE);
    return 0;
}

// Returns pointer to start of specified ref pic list.
template<typename PIXTYPE>
inline
EncoderRefPicListStruct<PIXTYPE>* H264EncoderFrame_GetRefPicList(H264EncoderFrame<PIXTYPE>* state, Ipp32s SliceNum, Ipp32s List, Ipp32u iMaxSliceSize)
{
    EncoderRefPicListStruct<PIXTYPE> *pList;

    if(iMaxSliceSize)
        SliceNum = 0;

    if (List == LIST_0)
        pList = &state->m_pRefPicList[SliceNum].m_RefPicListL0;
    else
        pList = &state->m_pRefPicList[SliceNum].m_RefPicListL1;

    return pList;

}

template<typename PIXTYPE>
Status H264EncoderFrame_allocateParsedFrameData(
    H264EncoderFrame<PIXTYPE>* state,
    const IppiSize&,
    Ipp32s);

template<typename PIXTYPE>
void H264EncoderFrame_deallocateParsedFrameData(
    H264EncoderFrame<PIXTYPE>* state);

template<typename PIXTYPE>
void H264EncoderFrame_useAux(
    H264EncoderFrame<PIXTYPE>* state);

template<typename PIXTYPE>
void H264EncoderFrame_usePrimary(
    H264EncoderFrame<PIXTYPE>* state);

template<typename PIXTYPE>
struct H264EncoderFrameList
{
//private:
    // m_pHead points to the first element in the list, and m_pTail
    // points to the last.  m_pHead->previous() and m_pTail->future()
    // are both NULL.
    H264EncoderFrame<PIXTYPE> *m_pHead;
    H264EncoderFrame<PIXTYPE> *m_pTail;
    H264EncoderFrame<PIXTYPE> *m_pCurrent;
    MemoryAllocator* memAlloc;

    unsigned int test;
    Ipp32s curPpoc;
};

template<typename PIXTYPE>
Status H264EncoderFrameList_Create(
    H264EncoderFrameList<PIXTYPE>* state);

template<typename PIXTYPE>
Status H264EncoderFrameList_Create(
    H264EncoderFrameList<PIXTYPE>* state,
    MemoryAllocator *pMemAlloc);

template<typename PIXTYPE>
void H264EncoderFrameList_Destroy(
    H264EncoderFrameList<PIXTYPE>* state);

template<typename PIXTYPE>
void H264EncoderFrameList_clearFrameList(
    H264EncoderFrameList<PIXTYPE>* state);

// Detach the first frame and return a pointer to it
template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE>* H264EncoderFrameList_detachHead(
    H264EncoderFrameList<PIXTYPE>* state);

// Removes selected frame from list
template<typename PIXTYPE>
void H264EncoderFrameList_RemoveFrame(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrame<PIXTYPE>*);

// Append the given frame to our tail
template<typename PIXTYPE>
void H264EncoderFrameList_append(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrame<PIXTYPE> *pFrame);

template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE>* H264EncoderFrameList_InsertFrame(
    H264EncoderFrameList<PIXTYPE>* state,
    VideoData* rFrame,
    EnumPicCodType ePictureType,
    Ipp32s isRef,
    Ipp32s num_slices,
    const IppiSize& padded_size
    , Ipp32s alpha
);

// Move the given list to the beginning of our list.
template<typename PIXTYPE>
void H264EncoderFrameList_insertList(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrameList<PIXTYPE>& src);

// Search through the list for the next disposable frame to decode into
template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE> *H264EncoderFrameList_findNextDisposable(
    H264EncoderFrameList<PIXTYPE>* state);

// Search through the list for the oldest disposable frame to decode into
template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE> *H264EncoderFrameList_findOldestDisposable(
    H264EncoderFrameList<PIXTYPE>* state);

// Inserts a frame immediately after the position pointed to by m_pCurrent
template<typename PIXTYPE>
void H264EncoderFrameList_insertAtCurrent(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrame<PIXTYPE> *pFrame);

// Mark all frames as not used as reference frames.
template<typename PIXTYPE>
void H264EncoderFrameList_removeAllRef(
    H264EncoderFrameList<PIXTYPE>* state);

// Mark all frames as not used as reference frames.
template<typename PIXTYPE>
void H264EncoderFrameList_IncreaseRefPicListResetCount(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrame<PIXTYPE> *ExcludeFrame);

// Mark the oldest short-term reference frame as not used.
template<typename PIXTYPE>
void H264EncoderFrameList_freeOldestShortTermRef(
    H264EncoderFrameList<PIXTYPE>* state);

// Mark the short-term reference frame with specified PicNum as not used
template<typename PIXTYPE>
void H264EncoderFrameList_freeShortTermRef(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32s PicNum);

// Mark the long-term reference frame with specified LongTermPicNum  as not used
template<typename PIXTYPE>
void H264EncoderFrameList_freeLongTermRef(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32s LongTermPicNum);

// Mark the long-term reference frame with specified LongTermFrameIdx as not used
template<typename PIXTYPE>
void H264EncoderFrameList_freeLongTermRefIdx(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32s LongTermFrameIdx);

// Mark any long-term reference frame with LongTermFrameIdx greater than MaxLongTermFrameIdx as not used.
template<typename PIXTYPE>
void H264EncoderFrameList_freeOldLongTermRef(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32s MaxLongTermFrameIdx);

// Mark the short-term reference frame with specified PicNum as long-term with specified long term idx.
template<typename PIXTYPE>
void H264EncoderFrameList_changeSTtoLTRef(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32s PicNum,
    Ipp32s LongTermFrameIdx);

template<typename PIXTYPE>
void H264EncoderFrameList_countActiveRefs(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32u &NumShortTerm,
    Ipp32u &NumLongTerm);

// Return number of active short and long term reference frames.
template<typename PIXTYPE>
void H264EncoderFrameList_countL1Refs(
    H264EncoderFrameList<PIXTYPE>* state,
    Ipp32u &NumRefs,
    Ipp32s curPOC);

// Search through the list for the oldest displayable frame.
template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE>* H264EncoderFrameList_findOldestToEncode(
    H264EncoderFrameList<PIXTYPE>* state,
    H264EncoderFrameList<PIXTYPE>* dpb,
    Ipp32u min_L1_refs,
    Ipp32s use_B_as_refs);

// Search through the list for the newest displayable frame.
template<typename PIXTYPE>
H264EncoderFrame<PIXTYPE>* H264EncoderFrameList_findNewestToEncode(
    H264EncoderFrameList<PIXTYPE>* state);

template<typename PIXTYPE>
Ipp32s H264EncoderFrameList_countNumToEncode(
    H264EncoderFrameList<PIXTYPE>* state);

template<typename PIXTYPE>
void H264EncoderFrameList_switchToPrimary(
    H264EncoderFrameList<PIXTYPE>* state);

template<typename PIXTYPE>
void H264EncoderFrameList_switchToAuxiliary(
    H264EncoderFrameList<PIXTYPE>* state);

#endif // __UMC_H264_ENC_CPB_H__
