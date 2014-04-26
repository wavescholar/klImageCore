//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef UMC_H264_CORE_ENC_H__
#define UMC_H264_CORE_ENC_H__

#include "umc_memory_allocator.h"
#include "umc_h264_defs.h"
#include "umc_h264_bs.h"
#include "umc_h264_enc_cpb.h"
#include "umc_h264_avbr.h"
#include "umc_h264_deblocking.h"

// These are used to index into Block_RLE below.
#define U_DC_RLE 48     // Used in Intra Prediciton Modes
#define V_DC_RLE 49     // Used in Intra Prediciton Modes
#define Y_DC_RLE 50     // Used in Intra 16x16 Prediciton Mode

#define ANALYSE_I_4x4               (1 << 0)
#define ANALYSE_I_8x8               (1 << 1)
#define ANALYSE_P_4x4               (1 << 2)
#define ANALYSE_P_8x8               (1 << 3)
#define ANALYSE_B_4x4               (1 << 4)
#define ANALYSE_B_8x8               (1 << 5)
#define ANALYSE_SAD                 (1 << 6)
#define ANALYSE_ME_EARLY_EXIT       (1 << 7)
#define ANALYSE_ME_ALL_REF          (1 << 8)
#define ANALYSE_ME_CHROMA           (1 << 9)
#define ANALYSE_ME_SUBPEL           (1 << 10)
#define ANALYSE_CBP_EMPTY           (1 << 11)
#define ANALYSE_RECODE_FRAME        (1 << 12)
#define ANALYSE_ME_AUTO_DIRECT      (1 << 13)
#define ANALYSE_FRAME_TYPE          (1 << 14)
#define ANALYSE_FLATNESS            (1 << 15)
#define ANALYSE_RD_MODE             (1 << 16)
#define ANALYSE_RD_OPT              (1 << 17)
#define ANALYSE_B_RD_OPT            (1 << 18)
#define ANALYSE_CHECK_SKIP_PREDICT  (1 << 19)
#define ANALYSE_CHECK_SKIP_INTPEL   (1 << 20)
#define ANALYSE_CHECK_SKIP_BESTCAND (1 << 21)
#define ANALYSE_CHECK_SKIP_SUBPEL   (1 << 22)
#define ANALYSE_SPLIT_SMALL_RANGE   (1 << 23)
#define ANALYSE_ME_EXT_CANDIDATES   (1 << 24)
#define ANALYSE_ME_SUBPEL_SAD       (1 << 25)
#define ANALYSE_INTRA_IN_ME         (1 << 26)
#define ANALYSE_ME_FAST_MULTIREF    (1 << 27)
#define ANALYSE_FAST_INTRA          (1 << 28)
#define ANALYSE_ME_PRESEARCH        (1 << 29)
#define ANALYSE_ME_CONTINUED_SEARCH (1 << 30)
#define ANALYSE_ME_BIDIR_REFINE     (1 << 31)

//Optimal quantization levels
#define OPT_QUANT_INTER_RD 2
#define OPT_QUANT_INTER 0
#define OPT_QUANT_INTRA16x16 0
#define OPT_QUANT_INTRA4x4 1
#define OPT_QUANT_INTRA8x8 1

#define CALC_16x16_INTRA_MB_TYPE(slice, mode, nc, ac) (1+IntraMBTypeOffset[slice]+mode+4*nc+12*ac)
#define CALC_4x4_INTRA_MB_TYPE(slice) (IntraMBTypeOffset[slice])
#define CALC_PCM_MB_TYPE(slice) (IntraMBTypeOffset[slice]+25);


inline Ipp8s GetReferenceField(Ipp8s *pFields, Ipp32s RefIndex)
{
    if (RefIndex < 0)
        return -1;
    else
    {
        VM_ASSERT(pFields[RefIndex] >= 0);
        return pFields[RefIndex];
    }
}

// forward declaration of internal types
typedef void (*H264CoreEncoder_DeblockingFunction)(void* state, Ipp32u nMBAddr);



//////////////////////////////////////////


template<typename COEFFSTYPE, typename PIXTYPE>
struct H264Slice
{
//public:
    EnumSliceType m_slice_type; // Type of the current slice.
    Ipp32s        m_slice_number; // Number of the current slice.
    Ipp32s        status;     //Return value from Compress_Slice function
    Ipp8s         m_iLastXmittedQP;
    Ipp32u        m_MB_Counter;
    Ipp32u        m_Intra_MB_Counter;
    Ipp32u        m_uSkipRun;
    Ipp32s        m_prev_dquant;

    Ipp32s      m_is_cur_mb_field;
    bool        m_is_cur_mb_bottom_field;
    Ipp32s      m_first_mb_in_slice;
    Ipp8s       m_slice_qp_delta;            // delta between slice QP and picture QP
    Ipp8u       m_cabac_init_idc;            // CABAC initialization table index (0..2)
    bool        m_use_transform_for_intra_decision;

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> m_cur_mb;

    H264BsBase* m_pbitstream; // Where this slice is encoded to.
    H264BsFake* fakeBitstream;
    H264BsFake* fBitstreams[9]; //For INTRA mode selection

    Ipp8u       m_disable_deblocking_filter_idc; // deblock filter control, 0=filter all edges
    Ipp8s       m_slice_alpha_c0_offset;         // deblock filter c0, alpha table offset
    Ipp8s       m_slice_beta_offset;             // deblock filter beta table offset
    Ipp32s     *m_InitialOffset;
    Ipp32s      m_NumRefsInL0List;
    Ipp32s      m_NumRefsInL1List;
    Ipp32s      m_NumRefsInLTList;
    Ipp8u       num_ref_idx_active_override_flag;   // nonzero: use ref_idx_active from slice header
    Ipp32s      num_ref_idx_l0_active;              // num of ref pics in list 0 used to decode the slice,
    Ipp32s      num_ref_idx_l1_active;              // num of ref pics in list 1 used to decode the slice

    // MB work buffer, allocated buffer pointer for freeing
    Ipp8u*      m_pAllocatedMBEncodeBuffer;
    // m_pAllocatedMBEncodeBuffer is mapped onto the following pointers.
    PIXTYPE*    m_pPred4DirectB;      // the 16x16 MB prediction for direct B mode
    PIXTYPE*    m_pPred4BiPred;       // the 16x16 MB prediction for BiPredicted B Mode
    PIXTYPE*    m_pTempBuff4DirectB;  // 16x16 working buffer for direct B
    PIXTYPE*    m_pTempBuff4BiPred;  // 16x16 working buffer for BiPred B
    PIXTYPE*    m_pTempChromaPred;  // 16x16 working buffer for chroma pred B
    PIXTYPE*    m_pMBEncodeBuffer;    // temp work buffer

    // Buffers for CABAC.
    T_RLE_Data<COEFFSTYPE> Block_RLE[51];       // [0-15] Luma, [16-31] Chroma U/Luma1, [32-47] Chroma V/Luma2, [48] Chroma U DC/Luma1 DC, [49] Chroma V DC/Luma2 DC, [50] Luma DC
    T_Block_CABAC_Data<COEFFSTYPE> Block_CABAC[51];  // [0-15] Luma, [16-31] Chroma U/Luma1, [32-47] Chroma V/Luma2, [48] Chroma U DC/Luma1 DC, [49] Chroma V DC/Luma2 DC, [50] Luma DC

    EncoderRefPicList<PIXTYPE> m_TempRefPicList[2][2];

    Ipp32s      MapColMBToList0[MAX_NUM_REF_FRAMES][2];
    Ipp32s      DistScaleFactor[MAX_NUM_REF_FRAMES][MAX_NUM_REF_FRAMES];
    Ipp32s      DistScaleFactorMV[MAX_NUM_REF_FRAMES][MAX_NUM_REF_FRAMES];
    Ipp32s      DistScaleFactorAFF[2][2][2][MAX_NUM_REF_FRAMES]; // [curmb field],[ref1field],[ref0field]
    Ipp32s      DistScaleFactorMVAFF[2][2][2][MAX_NUM_REF_FRAMES]; // [curmb field],[ref1field],[ref0field]

#ifdef SLICE_THREADING_LOAD_BALANCING
    Ipp64s m_ticks_per_slice; // Total time used to encode all macroblocks in the slice
#endif // SLICE_THREADING_LOAD_BALANCING
};

template<typename COEFFSTYPE, typename PIXTYPE>
struct H264CoreEncoder
{
//public:
//protected:
    //f
    Ipp32s                        m_Analyse, m_SubME_Algo;
    Ipp32s                        profile_frequency;
    H264LocalMacroblockDescriptor m_mbinfo;

    Ipp32s                        m_HeightInMBs;
    Ipp32s                        m_WidthInMBs;

    MemoryAllocator*              memAlloc;
    Ipp8u                         *m_pParsedDataNew;
    T_EncodeMBOffsets             *m_pMBOffsets;
    EnumPicCodType                *eFrameType;
    H264EncoderFrame<PIXTYPE>          **eFrameSeq;
    H264BsReal*               m_bs1; // Pointer to the main bitstream.
    IppiSize                      m_PaddedSize;
    Ipp8u                        m_FieldStruct;

//public:
    H264EncoderParams               m_params;
    H264_Encoder_Compression_Flags  cflags;
    H264_Encoder_Compression_Notes  cnotes;
    H264EncoderFrameList<PIXTYPE>        m_cpb;
    H264EncoderFrameList<PIXTYPE>        m_dpb;
    Ipp32u                          m_dpbSize;
    H264EncoderFrame<PIXTYPE>*      m_pCurrentFrame;
    H264EncoderFrame<PIXTYPE>*      m_pLastFrame;     // ptr to last frame
    H264EncoderFrame<PIXTYPE>*      m_pReconstructFrame;

    // Table to obtain value to advance the 4x4 block offset for the next block.
    Ipp32s                          m_EncBlockOffsetInc[2][48];
    Ipp32s                          m_is_cur_pic_afrm;
    bool                            m_is_mb_data_initialized;

    bool                            m_NeedToCheckMBSliceEdges;
    Ipp32s                          m_field_index;
    Ipp32s                          m_NumShortEntriesInList;
    Ipp32s                          m_NumLongEntriesInList;
    AdaptiveMarkingInfo             m_AdaptiveMarkingInfo;
    RefPicListReorderInfo           m_ReorderInfoL0;
    RefPicListReorderInfo           m_ReorderInfoL1;

    Ipp32s                          m_InitialOffsets[2][2];

    Ipp32s                          m_MaxLongTermFrameIdx;

//protected:
    Ipp8u*                          m_pAllocEncoderInst;

    // flags read by DetermineFrameType while sequencing the profile:
    bool                            m_bMakeNextFrameKey;
    bool                            m_bMakeNextFrameIDR;
    Ipp32s                          m_uIntraFrameInterval;
    Ipp32s                          m_uIDRFrameInterval;
    Ipp32s                          m_l1_cnt_to_start_B;

    H264Slice<COEFFSTYPE, PIXTYPE> *m_Slices; // thread independent slice information.
#ifdef DEBLOCK_THREADING
    volatile Ipp8u      *mbs_deblock_ready;
#endif // _OPENMP

    Ipp32s m_iProfileIndex;
    Ipp8u* m_pBitStream;   // Pointer to bitstream buffer.
    Ipp32u m_uTRWrapAround;  // Wrap around for TR
    Ipp32u m_uFrames_Num;  // Decode order frame number.
    Ipp32u m_uFrameCounter;
    Ipp32s m_Pitch;
    Ipp64s m_total_bits_encoded; //Bit counter for all stream

//private:
    ///////////////////////////////////////////////////////////////////////
    // Data
    ///////////////////////////////////////////////////////////////////////

    H264BsReal**    m_pbitstreams; // Array of bitstreams for threads.

    // Which CPU-specific flavor of algorithm to use.
    EnumPicClass            m_PicClass;
    H264SliceHeader         m_SliceHeader;
    H264SeqParamSet         m_SeqParamSet;
    H264PicParamSet         m_PicParamSet;
    EnumPicCodType          m_PicType;
    Ipp32u                  m_FrameNum;
    Ipp32s                  m_FrameNumGap;
    Ipp32s                  m_PicOrderCnt_Accu; // Accumulator to compensate POC resets on IDR frames.
    Ipp32u                  m_PicOrderCnt;
    Ipp32u                  m_PicOrderCntMsb;
    Ipp32u                  m_PicOrderCntLsb;
    Ipp32s                  m_FrameNumOffset;
    Ipp32u                  m_TopFieldPOC;
    Ipp32u                  m_BottomFieldPOC;

    bool use_implicit_weighted_bipred;

    Ipp16u uNumSlices;                  // Number of Slices in the Frame

    Ipp32u m_slice_length;                // Number of Macroblocks in each slice
    // Except the last slice which may have
    // more macroblocks.

    Ipp32u m_uSliceRemainder;         // Number of extra macroblocks in last slice.

    Ipp32u* m_EmptyThreshold; // empty block threshold table.
    Ipp32u* m_DirectBSkipMEThres;
    Ipp32u* m_PSkipMEThres;
    Ipp32s* m_BestOf5EarlyExitThres;

    // ***************************************************************************
    // Rate control related fields.
    // ***************************************************************************

    Ipp64f  FrameRate;
    Ipp32s  BitRate;
    //Ipp32s  qscale[3];             // qscale codes for 3 frame types (Ipp32s!)
    Ipp32s  m_DirectTypeStat[2];
    //f
    H264_AVBR avbr;
    Ipp32u  m_MaxSliceSize;

#ifdef SLICE_THREADING_LOAD_BALANCING
    // Load balancing for slice level multithreading
    Ipp64s *m_B_ticks_per_macroblock; // Array of timings for every P macroblock of the previous frame
    Ipp64s *m_P_ticks_per_macroblock; // Array of timings for every B macroblock of the previous frame
    Ipp64s m_B_ticks_per_frame; // Total time used to encode all macroblocks in the frame
    Ipp64s m_P_ticks_per_frame; // Total time used to encode all macroblocks in the frame
    Ipp8u m_B_ticks_data_available;
    Ipp8u m_P_ticks_data_available;
#endif // SLICE_THREADING_LOAD_BALANCING
};

template<typename COEFFSTYPE, typename PIXTYPE>
inline EncoderRefPicListStruct<PIXTYPE>* GetRefPicList(
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    Ipp32u List,
    Ipp32s mb_cod_type,
    Ipp32s is_bottom_mb)
{
    EncoderRefPicListStruct<PIXTYPE> *pList;
    if (List == LIST_0)
        pList = &curr_slice->m_TempRefPicList[mb_cod_type][is_bottom_mb].m_RefPicListL0;
    else
        pList = &curr_slice->m_TempRefPicList[mb_cod_type][is_bottom_mb].m_RefPicListL1;
    return pList;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264Slice_Create(
    void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264Slice_Init(
    void* state,
    H264EncoderParams& info); // Must be called once as parameters are available.

template<typename COEFFSTYPE, typename PIXTYPE>
void H264Slice_Destroy(
    void* state);

/* public: */
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Create(
    void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Destroy(
    void* state);

// Initialize codec with specified parameter(s)
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Init(
    void* state,
    BaseCodecParams *init,
    MemoryAllocator *pMemAlloc);

// Compress (decompress) next frame
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_GetFrame(
    void* state,
    MediaData *in,
    MediaData *out);

// Get codec working (initialization) parameter(s)
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_GetInfo(
    void* state,
    BaseCodecParams *info);

template<typename COEFFSTYPE, typename PIXTYPE>
const H264PicParamSet* H264CoreEncoder_GetPicParamSet(void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
const H264SeqParamSet* H264CoreEncoder_GetSeqParamSet(void* state);

// Close all codec resources
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Close(
    void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Reset(
    void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_SetParams(
    void* state,
    BaseCodecParams* params);

template<typename COEFFSTYPE, typename PIXTYPE>
VideoData* H264CoreEncoder_GetReconstructedFrame(
    void* state);

template<typename COEFFSTYPE>
void H264CoreEncoder_ScanSignificant_CABAC( //stateless
    COEFFSTYPE coeff[],
    Ipp32s ctxBlockCat,
    Ipp32s numcoeff,
    const Ipp32s* dec_single_scan,
    T_Block_CABAC_Data<COEFFSTYPE>* c_data);

template<typename COEFFSTYPE>
void H264CoreEncoder_MakeSignificantLists_CABAC( //stateless
    COEFFSTYPE* coeff,
    const Ipp32s* dec_single_scan,
    T_Block_CABAC_Data<COEFFSTYPE>* c_data);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_SetSliceHeaderCommon(
    void* state,
    H264EncoderFrame<PIXTYPE>*);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Start_Picture(
    void* state,
    const EnumPicClass* pic_class,
    EnumPicCodType pic_type);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_UpdateCurrentMBInfo(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Intra16x16SelectAndPredict(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32u *puAIMBSAD,      // return total MB SAD here
    PIXTYPE *pPredBuf);   // return predictor pels here

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBChromaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBLumaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBChromaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopRightLocationForCurrentMBLumaNonMBAFF( //stateless
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block,
    Ipp32s AdditionalDecrement/* = 0*/);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBChromaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block,
    bool is_deblock_calls);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBChromaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopRightLocationForCurrentMBLumaMBAFF(
    void* state,
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block);

template<typename PIXTYPE>
void ExpandPlane(
    PIXTYPE* StartPtr,
    Ipp32s   frameWidth,
    Ipp32s   frameHeight,
    Ipp32s   pitchPixels,
    Ipp32s   pels);
/*
template<typename PIXTYPE>
void ExpandPlane_NV12(
    PIXTYPE* StartPtr,
    Ipp32s   frameWidth,
    Ipp32s   frameHeight,
    Ipp32s   pitchPixels,
    Ipp32s   pels);

template<typename PIXTYPE>
void PlanarPredictLuma(
    PIXTYPE* pBlock,
    Ipp32u uPitch,
    PIXTYPE* pPredBuf,
    Ipp32s bitDepth);

template<typename PIXTYPE>
void PlanarPredictChroma(
    PIXTYPE* pBlock,
    Ipp32u uPitch,
    PIXTYPE* pPredBuf,
    Ipp32s bitDepth,
    Ipp32s idc);
*/

// umc_h264_pack.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MBHeader_Fake(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MBLuma_Fake(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MBChroma_Fake(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_PackSubBlockLuma_Fake(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32u uBlock);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MB_Real(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MBHeader_Real(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Put_MB_Fake(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

// umc_h264_ermb.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_TransQuantIntra_RD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantChromaIntra_RD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantIntra16x16_RD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Encode4x4IntraBlock(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32s block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Encode8x8IntraBlock(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32s block);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_CEncAndRecMB(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_TransQuantInter_RD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantChromaInter_RD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

// umc_h264_deblocking.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockSlice(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    Ipp32u uFirstMB,
    Ipp32u uNumMBs );

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockLuma(
    void* state,
    Ipp32u dir,
    DeblockingParameters<PIXTYPE>* pParams);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockChroma(
    void* state,
    Ipp32u dir,
    DeblockingParameters<PIXTYPE>* pParams);

template<typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersISlice(
    void* state,
    DeblockingParameters<PIXTYPE> *pParams);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice(
    void* state,
    DeblockingParameters<PIXTYPE> *pParams);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice(
    void* state,
    DeblockingParameters<PIXTYPE>* pParams);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4(
    void* state,
    Ipp32u dir,
    DeblockingParameters<PIXTYPE>* pParams);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice4(
    void* state,
    Ipp32u dir,
    DeblockingParameters<PIXTYPE>* pParams);

// umc_h264_deblocking_mbaff.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockISliceMBAFF(
    void* state,
    Ipp32u MBAddr);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockPSliceMBAFF(
    void* state,
    Ipp32u MBAddr);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockBSliceMBAFF(
    void* state,
    Ipp32u MBAddr);

// umc_h264_me.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_FrameTypeDetect(
    void* state);

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32s H264CoreEncoder_ME_P(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32s H264CoreEncoder_ME_B(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Skip_MV_Predicted(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    H264MotionVector* pMVPredicted,
    H264MotionVector* pMVOut);  // Returns Skip MV if not NULL

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Calc_One_MV_Predictor(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    Ipp32u uBlock,              // which 4x4 Block (UL Corner, Raster Order)
    Ipp32u uList,               // 0 or 1 for L0 or L1
    Ipp32u uBlocksWide,         // 1, 2, or 4
    Ipp32u uBlocksHigh,         // 1, 2, or 4 (4x16 and 16x4 not permitted)
    H264MotionVector* pMVPred,  // resulting MV predictor
    H264MotionVector* pMVDelta, // resulting MV delta
    bool updateDMV/* = true*/);

// umc_h264_mc.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_MCOneMBLuma(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    const H264MotionVector* pMVFwd,   // motion vectors in subpel units
    const H264MotionVector* pMVBwd,   // motion vectors in subpel units
    PIXTYPE* pDst);                   // put the resulting block here

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_MCOneMBChroma(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    PIXTYPE* pDst);

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_CDirectBOneMB_Interp_Cr(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE>* curr_slice,
    const H264MotionVector* pMVL0,// Fwd motion vectors in subpel units
    const H264MotionVector* pMVL1,// Bwd motion vectors in subpel units
    Ipp8s* pFields0,              //
    Ipp8s* pFields1,              //
    PIXTYPE* pDst,                // put the resulting block here with pitch of 16
    Ipp32s offset,
    IppiSize size);

// umc_h264_aic.cpp
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32u uBestSAD,        //Best previous SAD
    Ipp32u *puAIMBSAD);     // return total MB SAD here

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_Intra16x16SelectRD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE  *pSrc,           // pointer to upper left pel of source MB
    PIXTYPE  *pRef,           // pointer to same MB in reference picture
    Ipp32s      pitchPixels,    // of source and ref data
    T_AIMode   *pMode,          // selected mode goes here
    PIXTYPE  *pPredBuf);      // predictor pels for selected mode goes here

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_IntraSelectChromaRD(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pUSrc,           // pointer to upper left pel of U source MB
    PIXTYPE* pURef,           // pointer to same MB in U reference picture
    PIXTYPE* pVSrc,           // pointer to upper left pel of V source MB
    PIXTYPE* pVRef,           // pointer to same MB in V reference picture
    Ipp32u   uPitch,            // of source and ref data
    Ipp8u*   pMode,             // selected mode goes here
    PIXTYPE *pUPredBuf,       // U predictor pels for selected mode go here
    PIXTYPE *pVPredBuf);

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneMB_16x16(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrc,          // pointer to upper left pel of source MB
    PIXTYPE* pRef,          // pointer to same MB in reference picture
    Ipp32s   pitchPixels,   // of source and ref data in pixels
    T_AIMode *pMode,        // selected mode goes here
    PIXTYPE *pPredBuf);     // predictor pels for selected mode goes here

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectChromaMBs_8x8(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pUSrc,         // pointer to upper left pel of U source MB
    PIXTYPE* pURef,         // pointer to same MB in U reference picture
    PIXTYPE* pVSrc,         // pointer to upper left pel of V source MB
    PIXTYPE* pVRef,         // pointer to same MB in V reference picture
    Ipp32u uPitch,          // of source and ref data
    Ipp8u *pMode,           // selected mode goes here
    PIXTYPE *pUPredBuf,     // U predictor pels for selected mode go here
    PIXTYPE *pVPredBuf);    // V predictor pels for selected mode go here

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    Ipp32u uBestSAD,    // Best previous SAD
    Ipp32u *puAIMBSAD); // return total MB SAD here

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneMB_8x8(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrc,            // pointer to upper left pel of source MB
    PIXTYPE* pRef,            // pointer to same MB in reference picture
    Ipp32s uBlock,
    T_AIMode* pMode,            // selected mode goes here
    PIXTYPE* pPredBuf);       // predictor pels for selected mode goes here

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneBlock(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrcBlock,     // pointer to upper left pel of source block
    PIXTYPE* pReconBlock,   // pointer to same block in reconstructed picture
    Ipp32u uBlock,          // which 4x4 of the MB (0..15)
    T_AIMode *intra_types,  // selected mode goes here
    PIXTYPE *pPred);        // predictor pels for selected mode goes here

template<typename PIXTYPE>
void H264CoreEncoder_Filter8x8Pels( //stateless
    PIXTYPE* pred_pels,
    Ipp32u pred_pels_mask);

template<typename PIXTYPE>
void H264CoreEncoder_GetPredBlock( //stateless
    Ipp32u uMode,           // advanced intra mode of the block
    PIXTYPE *pPredBuf,
    PIXTYPE* PredPel);      // predictor pels

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetBlockPredPels(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pLeftRefBlock,       // pointer to block in reference picture
    Ipp32u uLeftPitch,              // of source data. Pitch in pixels.
    PIXTYPE* pAboveRefBlock,      // pointer to block in reference picture
    Ipp32u uAbovePitch,             // of source data. Pitch in pixels.
    PIXTYPE* pAboveLeftRefBlock,  // pointer to block in reference picture
    Ipp32u uAboveLeftPitch,         // of source data. Pitch in pixels.
    Ipp32u uBlock,                  // 0..15 for luma blocks only
    PIXTYPE* PredPel);              // result here

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetPrediction8x8(
    void* state,
    T_AIMode mode,
    PIXTYPE* pred_pels,
    Ipp32u pred_pels_mask,
    PIXTYPE* pels);

#endif
