/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_SLICE_DECODING_H
#define __UMC_H264_SLICE_DECODING_H

#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_video_decoder.h"
#include "umc_h264_bitstream.h"
#include "umc_automatic_mutex.h"
#include "umc_event.h"

#include "umc_h264_heap.h"

namespace UMC
{
class H264DBPList;
class H264DecoderFrameList;

// Slice decoding constant
enum
{
    MINIMUM_NUMBER_OF_ROWS      = 4,
    MAXIMUM_NUMBER_OF_ROWS      = 8,
    MB_BUFFER_SIZE              = 480,
    NUMBER_OF_PIECES            = 8,
    NUMBER_OF_DEBLOCKERS        = 2
};

// Task ID enumerator
enum
{
    // whole slice is processed
    TASK_PROCESS                = 0,
    // piece of slice is decoded
    TASK_DEC,
    // piece of future frame's slice is decoded
    TASK_REC,
    // whole slice is deblocked
    TASK_DEB_SLICE,
    // piece of slice is deblocked
    TASK_DEB,
    // piece of slice is deblocked by several threads
    TASK_DEB_THREADED,
    // whole frame is deblocked (when there is the slice groups)
    TASK_DEB_FRAME,
    // whole frame is deblocked (when there is the slice groups)
    TASK_DEB_FRAME_THREADED,
    // //whole frame is deblocked (when there is the slice groups)
    TASK_DEC_REC
};

// forward declaration of internal types
struct H264RefListInfo;

typedef Ipp16s FactorArrayValue;

struct FactorArray
{
    FactorArrayValue values[MAX_NUM_REF_FRAMES][MAX_NUM_REF_FRAMES];
};

struct FactorArrayMV
{
    FactorArrayValue values[MAX_NUM_REF_FRAMES];
};

struct FactorArrayAFF
{
    FactorArrayValue values[MAX_NUM_REF_FRAMES][2][2][2][MAX_NUM_REF_FRAMES];
};

struct FactorArrayMVAFF
{
    FactorArrayValue values[2][2][2][MAX_NUM_REF_FRAMES];
};

class H264Task;
class H264MemoryPiece;
class H264DecoderFrameInfo;
class H264DecoderFrame;
class H264DecoderFrameInfo;

class H264Slice
{
    // It is OK. H264SliceStore is owner of H264Slice object.
    // He can do what he wants.
    friend class H264SegmentDecoderMultiThreaded;
    friend class TaskSupplier;
    friend class TaskBroker;
    friend class TaskBrokerTwoThread;
    friend class H264DecoderFrameInfo;
    friend void PrintInfoStatus(H264DecoderFrameInfo * info);

public:
    // Default constructor
    H264Slice(MemoryAllocator *pMemoryAllocator = 0);
    // Destructor
    virtual
    ~H264Slice(void);

    // Initialize slice
    bool Init(Ipp32s iConsumerNumber);
    // Set slice source data
    bool Reset(void *pSource, size_t nSourceSize, Ipp32s iConsumerNumber);

    // Set current slice number
    void SetSliceNumber(Ipp32s iSliceNumber);

    void InitializeContexts();

    Ipp32s RetrievePicParamSetNumber(void *pSource, size_t nSourceSize);

    //
    // method(s) to obtain slice specific information
    //

    // Obtain pointer to slice header
    const H264SliceHeader *GetSliceHeader(void) const {return &m_SliceHeader;}
    H264SliceHeader *GetSliceHeader(void) {return &m_SliceHeader;}
    // Obtain bit stream object
    H264Bitstream *GetBitStream(void){return &m_BitStream;}
    // Obtain prediction weigth table
    const PredWeightTable *GetPredWeigthTable(Ipp32s iNum) const {return m_PredWeight[iNum & 1];}
    // Obtain first MB number
    Ipp32s GetFirstMBNumber(void) const {return m_iFirstMBFld;}
    Ipp32s GetStreamFirstMB(void) const {return m_iFirstMB;}
    void SetFirstMBNumber(Ipp32s x) {m_iFirstMB = x;}
    // Obtain MB width
    Ipp32s GetMBWidth(void) const {return m_iMBWidth;}
    // Obtain MB row width
    Ipp32s GetMBRowWidth(void) const {return (m_iMBWidth * (m_SliceHeader.MbaffFrameFlag + 1));}
    // Obtain MB height
    Ipp32s GetMBHeight(void) const {return m_iMBHeight;}
    // Obtain current picture parameter set number
    Ipp32s GetPicParamSet(void) const {return m_pPicParamSet->pic_parameter_set_id;}
    // Obtain current sequence parameter set number
    Ipp32s GetSeqParamSet(void) const {return m_pSeqParamSet->seq_parameter_set_id;}
    // Obtain current picture parameter set
    H264PicParamSet *GetPicParam(void){return m_pPicParamSet;}
    void SetPicParam(H264PicParamSet * pps) {m_pPicParamSet = pps;}
    // Obtain current sequence parameter set
    H264SeqParamSet *GetSeqParam(void){return m_pSeqParamSet;}
    void SetSeqParam(H264SeqParamSet * sps) {m_pSeqParamSet = sps;}

    // Obtain current sequence extension parameter set
    H264SeqParamSetExtension *GetSeqParamEx(void){return m_pSeqParamSetEx;}
    void SetSeqExParam(H264SeqParamSetExtension * spsex) {m_pSeqParamSetEx = spsex;}

    // Obtain current destination frame
    H264DecoderFrame *GetCurrentFrame(void){return m_pCurrentFrame;}
    void SetCurrentFrame(H264DecoderFrame * pFrame){m_pCurrentFrame = pFrame;}

    // Obtain slice number
    Ipp32s GetSliceNum(void) const {return m_iNumber;}
    // Obtain owning slice field index
    Ipp32s GetFieldIndex(void) const {return m_field_index;}
    // Need to check slice edges
    bool NeedToCheckSliceEdges(void) const {return m_bNeedToCheckMBSliceEdges;}
    // Do we can doing deblocking
    bool GetDeblockingCondition(void) const;
    // Obtain scale factor arrays
    FactorArray *GetDistScaleFactor(void){return &m_DistScaleFactor;}
    FactorArrayMV *GetDistScaleFactorMV(void){return &m_DistScaleFactorMV;}
    FactorArrayAFF *GetDistScaleFactorAFF(void){return m_DistScaleFactorAFF;}
    FactorArrayMVAFF *GetDistScaleFactorMVAFF(void){return m_DistScaleFactorMVAFF;}
    // Obtain maximum of macroblock
    Ipp32s GetMaxMB(void) const {return m_iMaxMB;}
    void SetMaxMB(Ipp32s x) {m_iMaxMB = x;}

    Ipp32s GetMBCount() const { return m_iMaxMB - m_iFirstMB;}
    // Obtain local MB information
    H264DecoderLocalMacroblockDescriptor &GetMBInfo(void){return *m_mbinfo;}
    // Obtain pointer to MB intra types
    IntraType *GetMBIntraTypes(void){return m_pMBIntraTypes;}

    // Check field slice
    bool IsField() const {return m_SliceHeader.field_pic_flag != 0;}
    // Check top field slice
    bool IsTopField() const {return m_SliceHeader.bottom_field_flag == 0;}
    // Check top field slice
    bool IsBottomField() const {return !IsTopField();}

    bool IsAuxiliary() const
    {
        return m_SliceHeader.is_auxiliary;
    }

    // Check slice organization
    bool IsSliceGroups(void) const {return (1 < m_pPicParamSet->num_slice_groups);};
    // Do we require to do deblocking through slice boundaries
    bool DeblockThroughBoundaries(void) const {return (DEBLOCK_FILTER_ON_NO_SLICE_EDGES != m_SliceHeader.disable_deblocking_filter_idc);};

    // Update reference list
    Status UpdateReferenceList(H264DBPList *pDecoderFrameList);

    //
    // Segment decoding mode's variables
    //

    // Obtain decoding state variables
    void GetStateVariables(Ipp32s &iMBSkipFlag,  Ipp32s &iQuantPrev, Ipp32s &iPrevDQuant);
    // Save decoding state variables
    void SetStateVariables(Ipp32s iMBSkipCount, Ipp32s iQuantPrev, Ipp32s iPrevDQuant);

    AdaptiveMarkingInfo * GetAdaptiveMarkingInfo();

    void InitDistScaleFactor(Ipp32s NumL0RefActive, Ipp32s NumL1RefActive,
        H264DecoderFrame **pRefPicList0, H264DecoderFrame **pRefPicList1, ReferenceFlags *pFields0, ReferenceFlags *pFields1);

    bool IsError() const {return m_bError;}

    bool IsReference() const {return m_SliceHeader.nal_ref_idc != 0;}

    void SetHeap(H264_Heap_Objects  *pObjHeap)
    {
        m_pObjHeap = pObjHeap;
    }

public:

    H264MemoryPiece *m_pSource;                                 // (H264MemoryPiece *) pointer to owning memory piece
    Ipp64f m_dTime;                                             // (Ipp64f) slice's time stamp

public:  // DEBUG !!!! should remove dependence

    void Reset();

    void Release();

    // Decode slice header
    bool DecodeSliceHeader(bool bFullInitialization = true);

    // Reference list(s) management functions & tools
    void InitPSliceRefPicList(bool bIsFieldSlice, Ipp32s /*NumL0RefActive*/, H264DecoderFrame **pRefPicList, H264DecoderFrameList *pDecoderFrameList);
    void InitBSliceRefPicLists(bool bIsFieldSlice, Ipp32s /*NumL0RefActive*/, Ipp32s /*NumL1RefActive*/, H264DecoderFrame **pRefPicList0, H264DecoderFrame **pRefPicList1, H264DecoderFrameList *pDecoderFrameList, H264RefListInfo &rli);
    Ipp32s AdjustRefPicListForFields(H264DecoderFrame **pRefPicList, ReferenceFlags *pFields, H264RefListInfo &rli);
    void ReOrderRefPicList(bool bIsFieldSlice, H264DecoderFrame **pRefPicList, ReferenceFlags *pFields, RefPicListReorderInfo *pReorderInfo, Ipp32s MaxPicNum, Ipp32s NumRefActive, H264DBPList *pDecoderFrameList);

    RefPicListReorderInfo ReorderInfoL0;                        // (RefPicListReorderInfo) reference list 0 info
    RefPicListReorderInfo ReorderInfoL1;                        // (RefPicListReorderInfo) reference list 1 info

    H264SliceHeader m_SliceHeader;                              // (H264SliceHeader) slice header
    H264Bitstream m_BitStream;                                  // (H264Bitstream) slice bit stream

    PredWeightTable m_PredWeight[2][MAX_NUM_REF_FRAMES];        // (PredWeightTable []) prediction weight table

    FactorArray     m_DistScaleFactor;
    FactorArrayMV   m_DistScaleFactorMV;
    FactorArrayAFF  *m_DistScaleFactorAFF;                      // [curmb field],[ref1field],[ref0field]
    FactorArrayMVAFF *m_DistScaleFactorMVAFF;                   // [curmb field],[ref1field],[ref0field]

    H264PicParamSet* m_pPicParamSet;                            // (H264PicParamSet *) pointer to array of picture parameters sets
    H264SeqParamSet* m_pSeqParamSet;                            // (H264SeqParamSet *) pointer to array of sequence parameters sets
    H264SeqParamSetExtension* m_pSeqParamSetEx;                 // (H264SeqParamSet *) pointer to array of sequence parameters sets

    H264DecoderFrame *m_pCurrentFrame;        // (H264DecoderFrame *) pointer to destination frame
    H264DecoderLocalMacroblockDescriptor *m_mbinfo;             // (H264DecoderLocalMacroblockDescriptor*) current frame MB information
    IntraType *m_pMBIntraTypes;                                 // (IntraType *) array of macroblock intra types

    Ipp32s m_iMBWidth;                                          // (Ipp32s) width in macroblock units
    Ipp32s m_iMBHeight;                                         // (Ipp32s) height in macroblock units
    Ipp32s m_CurrentPicParamSet;                                // (Ipp32s) current picture parameter set
    Ipp32s m_CurrentSeqParamSet;                                // (Ipp32s) current sequence parameter set

    Ipp32s m_iNumber;                                           // (Ipp32s) current slice number
    Ipp32s m_iFirstMB;                                          // (Ipp32s) first MB number in slice
    Ipp32s m_iMaxMB;                                            // (Ipp32s) last unavailable  MB number in slice

    Ipp32s m_iFirstMBFld;                                       // (Ipp32s) first MB number in slice

    Ipp32s m_iAvailableMB;                                      // (Ipp32s) available number of macroblocks (used in "unknown mode")

    Ipp32s m_iCurMBToDec;                                       // (Ipp32s) current MB number to decode
    Ipp32s m_iCurMBToRec;                                       // (Ipp32s) current MB number to reconstruct
    Ipp32s m_iCurMBToDeb;                                       // (Ipp32s *) current MB number to de-blocking

    bool m_bInProcess;                                          // (bool) slice is under whole decoding
    Ipp32s m_bDecVacant;                                        // (Ipp32s) decoding is vacant
    Ipp32s m_bRecVacant;                                        // (Ipp32s) reconstruct is vacant
    Ipp32s m_bDebVacant;                                        // (Ipp32s) de-blocking is vacant
    bool m_bFirstDebThreadedCall;                               // (bool) "first threaded deblocking call" flag
    bool m_bPermanentTurnOffDeblocking;                         // (bool) "disable deblocking" flag
    bool m_bError;                                              // (bool) there is an error in decoding
    bool m_isInitialized;

    Ipp32s m_MVsDistortion;

    AdaptiveMarkingInfo     m_AdaptiveMarkingInfo;
    
    Ipp32s m_iAllocatedMB;                                      // (Ipp32s) size of allocated buffer in macroblock

    H264CoeffsBuffer   m_CoeffsBuffers;

    // through-decoding variable(s)
    Ipp32s m_nMBSkipCount;                                      // (Ipp32u) current count of skipped macro blocks
    Ipp32s m_nQuantPrev;                                        // (Ipp32u) quantize value of previous macro block
    Ipp32s m_prev_dquant;
    Ipp32s m_field_index;                                       // (Ipp32s) current field index
    bool m_bNeedToCheckMBSliceEdges;                            // (bool) need to check inter-slice boundaries

    bool m_bDecoded;                                            // (bool) "slice has been decoded" flag
    bool m_bPrevDeblocked;                                      // (bool) "previous slice has been deblocked" flag
    bool m_bDeblocked;                                          // (bool) "slice has been deblocked" flag

    H264Slice *m_pNext;                                         // (H264Slice *) pointer to next slice in list

    // memory management tools
    MemoryAllocator *m_pMemoryAllocator;                        // (MemoryAllocator *) pointer to memory allocation tool

    H264_Heap_Objects           *m_pObjHeap;

    static FactorArrayAFF m_StaticFactorArrayAFF;
};

inline
void H264Slice::GetStateVariables(Ipp32s &iMBSkipFlag, Ipp32s &iQuantPrev, Ipp32s &iPrevDQuant)
{
    iMBSkipFlag = m_nMBSkipCount;
    iQuantPrev = m_nQuantPrev;
    iPrevDQuant = m_prev_dquant;

} // void H264Slice::GetStateVariables(Ipp32s &iMBSkipFlag, Ipp32s &iQuantPrev, bool &bSkipNextFDF, Ipp32s &iPrevDQuant)

inline
void H264Slice::SetStateVariables(Ipp32s iMBSkipFlag, Ipp32s iQuantPrev, Ipp32s iPrevDQuant)
{
    m_nMBSkipCount = iMBSkipFlag;
    m_nQuantPrev = iQuantPrev;
    m_prev_dquant = iPrevDQuant;

} // void H264Slice::SetStateVariables(Ipp32s iMBSkipFlag, Ipp32s iQuantPrev, bool bSkipNextFDF, Ipp32s iPrevDQuant)

inline
bool IsPictureTheSame(H264Slice *pSliceOne, H264Slice *pSliceTwo)
{
    if (!pSliceOne)
        return true;

    const H264SliceHeader *pOne = pSliceOne->GetSliceHeader();
    const H264SliceHeader *pTwo = pSliceTwo->GetSliceHeader();

    // this function checks two slices are from same picture or not
    // 7.4.1.2.4 part of h264 standart

    if ((pOne->frame_num != pTwo->frame_num) ||
        (pOne->first_mb_in_slice == pTwo->first_mb_in_slice) ||
        (pOne->pic_parameter_set_id != pTwo->pic_parameter_set_id) ||
        (pOne->field_pic_flag != pTwo->field_pic_flag) ||
        (pOne->bottom_field_flag != pTwo->bottom_field_flag))
        return false;

    if ((pOne->nal_ref_idc != pTwo->nal_ref_idc) &&
        (0 == IPP_MIN(pOne->nal_ref_idc, pTwo->nal_ref_idc)))
        return false;

    if (0 == pSliceOne->GetSeqParam()->pic_order_cnt_type)
    {
        if ((pOne->pic_order_cnt_lsb != pTwo->pic_order_cnt_lsb) ||
            (pOne->delta_pic_order_cnt_bottom != pTwo->delta_pic_order_cnt_bottom))
            return false;
    }
    else
    {
        if ((pOne->delta_pic_order_cnt[0] != pTwo->delta_pic_order_cnt[0]) ||
            (pOne->delta_pic_order_cnt[1] != pTwo->delta_pic_order_cnt[1]))
            return false;
    }

    if (pOne->nal_unit_type != pTwo->nal_unit_type)
    {
        if ((NAL_UT_IDR_SLICE == pOne->nal_unit_type) ||
            (NAL_UT_IDR_SLICE == pTwo->nal_unit_type))
            return false;
    }
    else if (NAL_UT_IDR_SLICE == pOne->nal_unit_type)
    {
        if (pOne->idr_pic_id != pTwo->idr_pic_id)
            return false;
    }

    return true;

} // bool IsPictureTheSame(H264SliceHeader *pOne, H264SliceHeader *pTwo)

// Declaration of internal class(es)
class H264SegmentDecoder;
class H264SegmentDecoderMultiThreaded;

class H264Task
{
public:
    // Default constructor
    H264Task(Ipp32s iThreadNumber)
        : m_iThreadNumber(iThreadNumber)
    {
        m_pSlice = 0;

        pFunction = 0;
        m_pBuffer = 0;
        m_WrittenSize = 0;

        m_iFirstMB = -1;
        m_iMaxMB = -1;
        m_iMBToProcess = -1;
        m_iTaskID = 0;
        m_bDone = false;
        m_bError = false;
        m_mvsDistortion = 0;
        m_taskPreparingGuard = 0;
    }

    Status (H264SegmentDecoderMultiThreaded::*pFunction)(Ipp32s nCurMBNumber, Ipp32s &nMaxMBNumber); // (Status (*) (Ipp32s, Ipp32s &)) pointer to working function

    CoeffsPtrCommon m_pBuffer;                                  // (Ipp16s *) pointer to working buffer
    size_t          m_WrittenSize;

    H264Slice *m_pSlice;                                        // (H264Slice *) pointer to owning slice
    H264DecoderFrameInfo * m_pSlicesInfo;
    AutomaticMutex    * m_taskPreparingGuard;

    Ipp32s m_mvsDistortion;
    Ipp32s m_iThreadNumber;                                     // (Ipp32s) owning thread number
    Ipp32s m_iFirstMB;                                          // (Ipp32s) first MB in slice
    Ipp32s m_iMaxMB;                                            // (Ipp32s) maximum MB number in owning slice
    Ipp32s m_iMBToProcess;                                      // (Ipp32s) number of MB to processing
    Ipp32s m_iTaskID;                                           // (Ipp32s) task identificator
    bool m_bDone;                                               // (bool) task was done
    bool m_bError;                                              // (bool) there is a error
};

// Declare function to swapping memory
extern
void SwapMemoryAndRemovePreventingBytes(void *pDst, size_t &nDstSize, void *pSrc, size_t nSrcSize);

} // namespace UMC

#endif // __UMC_H264_SLICE_DECODING_H
