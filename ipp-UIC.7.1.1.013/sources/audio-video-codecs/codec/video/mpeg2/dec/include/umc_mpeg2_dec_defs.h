/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_DEC_DEFS_H__
#define __UMC_MPEG2_DEC_DEFS_H__

#include "umc_structures.h"
#include "umc_memory_allocator.h"
#include "umc_mpeg2_dec_bstream.h"

#include "ippvc.h"

namespace UMC
{
#define IPPVC_MC_FRAME     0x2
#define IPPVC_MC_FIELD     0x1
#define IPPVC_MC_16X8      0x2
#define IPPVC_MC_DP        0x3

#define IPPVC_DCT_FIELD    0x0
#define IPPVC_DCT_FRAME    0x1

#define IPPVC_MB_INTRA     0x1
#define IPPVC_MB_PATTERN   0x2
#define IPPVC_MB_BACKWARD  0x4
#define IPPVC_MB_FORWARD   0x8
#define IPPVC_MB_QUANT     0x10

//start/end codes
#define PICTURE_START_CODE       0x00000100
#define USER_DATA_START_CODE     0x000001B2
#define SEQUENCE_HEADER_CODE     0x000001B3
#define SEQUENCE_ERROR_CODE      0x000001B4
#define EXTENSION_START_CODE     0x000001B5
#define SEQUENCE_END_CODE        0x000001B7
#define GROUP_START_CODE         0x000001B8

#define SEQUENCE_EXTENSION_ID                  0x00000001
#define SEQUENCE_DISPLAY_EXTENSION_ID          0x00000002
#define QUANT_MATRIX_EXTENSION_ID              0x00000003
#define COPYRIGHT_EXTENSION_ID                 0x00000004
#define SEQUENCE_SCALABLE_EXTENSION_ID         0x00000005
#define PICTURE_DISPLAY_EXTENSION_ID           0x00000007
#define PICTURE_CODING_EXTENSION_ID            0x00000008
#define PICTURE_SPARTIAL_SCALABLE_EXTENSION_ID 0x00000009
#define PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID 0x0000000a

#define DATA_PARTITIONING        0x00000000
#define SPARTIAL_SCALABILITY     0x00000001
#define SNR_SCALABILITY          0x00000002
#define TEMPORAL_SCALABILITY     0x00000003



enum MPEG2FrameType
{
    MPEG2_I_PICTURE               = 1,
    MPEG2_P_PICTURE               = 2,
    MPEG2_B_PICTURE               = 3,
};

#define FRAME_PICTURE            3
#define TOP_FIELD                1
#define BOTTOM_FIELD             2

#define ROW_CHROMA_SHIFT_420 3
#define ROW_CHROMA_SHIFT_422 4
#define COPY_CHROMA_MB_420 ippiCopy8x8_8u_C1R
#define COPY_CHROMA_MB_422 ippiCopy8x16_8u_C1R

extern Ipp16s q_scale[2][32];
///////////////////////

extern Ipp16s reset_dc[4];

extern Ipp16s intra_dc_multi[4];


struct sVideoFrameBuffer
{
    Ipp8u*           Y_comp_data;
    Ipp8u*           U_comp_data;
    Ipp8u*           V_comp_data;
    FrameType        frame_type; // 1-I, 2-P, 3-B
    Ipp64f           frame_time;
    Ipp64f           duration;
    Ipp32s           va_index;
    Ipp32s           is_invalid;
#ifdef KEEP_HISTORY
    Ipp8u*           frame_history;
#endif
};


struct sSequenceHeader
{
    Ipp32s           mb_width; //the number of macroblocks in the row of the picture
    Ipp32s           mb_height;//the number of macroblocks in the column of the picture
    Ipp32s           numMB;    //the number of macroblocks in the picture

//sequence extension
    Ipp32u           profile;
    Ipp32u           level;
    Ipp32u           extension_start_code_ID;
    Ipp32u           scalable_mode;
    Ipp32u           progressive_sequence;

    Ipp32s           frame_rate_extension_d;
    Ipp32s           frame_rate_extension_n;
    Ipp64f           delta_frame_time;
    Ipp64f           stream_time;
    Ipp32s           stream_time_temporal_reference; // for current stream_time
    Ipp32s           first_p_occure;
    Ipp32s           first_i_occure;
    Ipp32s           num_of_skipped;
    Ipp32s           bdied;
    Ipp32s           b_curr_number;
    Ipp32s           is_skipped_b;
    Ipp32s           is_decoded;

    // GOP info.
    Ipp32s           closed_gop;    // no ref to previous GOP
    Ipp32s           broken_link;   // ref to absent prev GOP
    Ipp32s           gop_second;    // second of first picture
    Ipp32s           gop_picture;   // starting picture in gop_second

    Ipp32s           frame_count;
};

// for prediction (forward and backward) and current frame;
struct sFrameBuffer
{
    sVideoFrameBuffer     frame_p_c_n[3];    // previous, current and next frames
    Ipp8u                 *ptr_context_data; // pointer to allocated data
    MemID                 mid_context_data;  // pointer to allocated data
    Ipp32u                Y_comp_height;
    Ipp32u                Y_comp_pitch;
    Ipp32u                U_comp_pitch;
    Ipp32u                V_comp_pitch;
    Ipp32u                pic_size;
    Ipp32s                prev_index; // 0 or 1; 0 initially
    Ipp32s                curr_index; // 0 or 1 or 2;  0 initially
    Ipp32s                next_index; // 0 or 1; 1 initially
    Ipp32s                retrieve;   // index of retrieved frame; -1 initially
    Ipp32s                ind_his_p;
    Ipp32s                ind_his_b;
    Ipp32s                ind_his_free;
    Ipp32s                ind_his_ref;
    Ipp32s                ind_his_curr;
    Ipp32s                field_buffer_index;
    Ipp32s                allocated_mb_width;
    Ipp32s                allocated_mb_height;
    ColorFormat           allocated_cformat;
    Ipp32s                allocated_size;
};


struct sPictureHeader
{
    MPEG2FrameType   picture_coding_type;
    Ipp32u           d_picture;

    Ipp32s           full_pel_forward_vector;
    Ipp32s           full_pel_backward_vector;

    //extensions
    Ipp32s           f_code[4];
    Ipp32s           r_size[4];
    Ipp32s           low_in_range[4];
    Ipp32s           high_in_range[4];
    Ipp32s           range[4];
    Ipp32u           picture_structure;
    Ipp32u           intra_dc_precision;
    Ipp32u           top_field_first;
    Ipp32u           frame_pred_frame_dct;
    Ipp32u           concealment_motion_vectors;
    Ipp32u           q_scale_type;
    Ipp32u           repeat_first_field;
    Ipp32u           progressive_frame;
    Ipp32s           temporal_reference;

    Ipp32s           curr_reset_dc;
    Ipp32s           intra_vlc_format;
    Ipp32s           alternate_scan;
    Ipp32s           curr_intra_dc_multi;
    Ipp32s           max_slice_vert_pos;
};

struct mp2_VLCTable
{
  Ipp32s max_bits;
  Ipp32s bits_table0;
  Ipp32s bits_table1;
  Ipp32u threshold_table0;
  Ipp16s *table0;
  Ipp16s *table1;
};

struct vlcStorageMPEG2
{
    Ipp32s *ippTableB5a;
    Ipp32s *ippTableB5b;
};

struct sliceInfo
{
  Ipp8u  *startPtr;
  Ipp8u  *endPtr;
  Ipp32s flag;
};

struct Mpeg2VideoContext
{
//Slice
    Ipp32s       slice_vertical_position;
    Ipp32u       m_bNewSlice;//l
    Ipp32s       cur_q_scale;

    Ipp32s       mb_row;
    Ipp32s       mb_col;

//Macroblock
    Ipp32u       macroblock_motion_forward;
    Ipp32u       macroblock_motion_backward;
    Ipp32s       prediction_type;

DECLALIGN(16) Ipp16s PMV[8];
DECLALIGN(16) Ipp16s vector[8];

    Ipp16s       dct_dc_past[3]; // y,u,v

    Ipp32s       mb_address_increment;//l
    Ipp32s       row_l, col_l, row_c, col_c;
    Ipp32s       offset_l, offset_c;

    Ipp8u        *blkCurrYUV[3];

//Block
DECLALIGN(16)
    DecodeIntraSpec_MPEG2 decodeIntraSpec;
DECLALIGN(16)
    DecodeInterSpec_MPEG2 decodeInterSpec;
DECLALIGN(16)
    DecodeIntraSpec_MPEG2 decodeIntraSpecChroma;
DECLALIGN(16)
    DecodeInterSpec_MPEG2 decodeInterSpecChroma;

//Bitstream
    Ipp8u*       bs_curr_ptr;
    Ipp32s       bs_bit_offset;
    Ipp8u*       bs_start_ptr;
    Ipp8u*       bs_end_ptr;
};

} // namespace UMC

extern Ipp16s zero_memory[64*8];

#define ippiCopy8x16_8u_C1R(pSrc, srcStep, pDst, dstStep)                   \
  ippiCopy8x8_8u_C1R(pSrc, srcStep, pDst, dstStep);                         \
  ippiCopy8x8_8u_C1R(pSrc + 8*(srcStep), srcStep, pDst + 8*(dstStep), dstStep);

#define ippiCopy8x16HP_8u_C1R(pSrc, srcStep, pDst, dstStep, acc, rounding)  \
  ippiCopy8x8HP_8u_C1R(pSrc, srcStep, pDst, dstStep, acc, rounding);        \
  ippiCopy8x8HP_8u_C1R(pSrc + 8 * srcStep, srcStep, pDst + 8 * dstStep, dstStep, acc, rounding);

#define ippiInterpolateAverage8x16_8u_C1IR(pSrc, srcStep, pDst, dstStep, acc, rounding)  \
  ippiInterpolateAverage8x8_8u_C1IR(pSrc, srcStep, pDst, dstStep, acc, rounding);        \
  ippiInterpolateAverage8x8_8u_C1IR(pSrc + 8 * srcStep, srcStep, pDst + 8 * dstStep, dstStep, acc, rounding);

#define FUNC_COPY_HP(W, H, pSrc, srcStep, pDst, dstStep, acc, rounding) \
  ippiCopy##W##x##H##HP_8u_C1R(pSrc, srcStep, pDst, dstStep, acc, rounding)

#define FUNC_AVE_HP(W, H, pSrc, srcStep, pDst, dstStep, mcType, roundControl) \
  ippiInterpolateAverage##W##x##H##_8u_C1IR(pSrc, srcStep, pDst, dstStep, mcType, roundControl);

#define FUNC_AVE_HP_B(W, H, pSrcRefF, srcStepF, mcTypeF, pSrcRefB, srcStepB, mcTypeB, \
                      pDst, dstStep, roundControl) \
  FUNC_COPY_HP(W, H, pSrcRefF, srcStepF, pDst, dstStep, mcTypeF, roundControl); \
  FUNC_AVE_HP(W, H, pSrcRefB, srcStepB, pDst, dstStep, mcTypeB, roundControl)

/*******************************************************/

#define HP_FLAG_MC(flag, x, y) \
  flag = ((x + x) & 2) | (y & 1); \
  flag = (flag << 2)

#define HP_FLAG_CP(flag, x, y) \
  flag = (x & 1) | ((y + y) & 2)

#define HP_FLAG_AV  HP_FLAG_CP

#define CHECK_OFFSET_L(offs, hhalf, pitch, hh) \
  if ((offs) < 0 || (offs)+(hhalf)+(hh-1)*(pitch)+15 > frame_buffer.pic_size) \
{ \
  return UMC_ERR_INVALID_STREAM; \
}

#define CALC_OFFSETS_FRAME_420(offs_l, offs_c, flag_l, flag_c, xl, yl, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl/2; \
\
  offs_l = video->offset_l + (yl >> 1)*pitch_l + (xl >> 1); \
  offs_c = video->offset_c + (yc >> 1)*pitch_c + (xc >> 1); \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define CALC_OFFSETS_FULLPEL(offs_l, offs_c, xl, yl, pitch_l, pitch_c) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl/2; \
\
  offs_l = video->offset_l + (yl >> 1)*pitch_l + (xl >> 1); \
  offs_c = video->offset_c + (yc >> 1)*pitch_c + (xc >> 1); \
}

#define CALC_OFFSETS_FIELD_420(offs_l, offs_c, flag_l, flag_c, xl, yl, field_sel, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl/2; \
\
  offs_l = video->offset_l + ((yl &~ 1) + field_sel)*pitch_l + (xl >> 1); \
  offs_c = video->offset_c + ((yc &~ 1) + field_sel)*pitch_c + (xc >> 1); \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define CALC_OFFSETS_FIELDX_420(offs_l, offs_c, flag_l, flag_c, xl, yl, field_sel, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl/2; \
  offs_l = ((yl &~ 1) + 2*video->row_l + field_sel)*pitch_l + (xl >> 1) + video->col_l; \
  offs_c = ((yc &~ 1) + 2*video->row_c + field_sel)*pitch_c + (xc >> 1) + video->col_c; \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define CALC_OFFSETS_FRAME_422(offs_l, offs_c, flag_l, flag_c, xl, yl, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl; \
\
  offs_l = video->offset_l + (yl >> 1)*pitch_l + (xl >> 1); \
  offs_c = video->offset_c + (yc >> 1)*pitch_c + (xc >> 1); \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define CALC_OFFSETS_FIELD_422(offs_l, offs_c, flag_l, flag_c, xl, yl, field_sel, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl; \
\
  offs_l = video->offset_l + ((yl &~ 1) + field_sel)*pitch_l + (xl >> 1); \
  offs_c = video->offset_c + ((yc &~ 1) + field_sel)*pitch_c + (xc >> 1); \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define CALC_OFFSETS_FIELDX_422(offs_l, offs_c, flag_l, flag_c, xl, yl, field_sel, HP_FLAG) \
{ \
  Ipp32s xc = xl/2; \
  Ipp32s yc = yl; \
  offs_l = ((yl &~ 1) + 2*video->row_l + field_sel)*pitch_l + (xl >> 1) + video->col_l; \
  offs_c = ((yc &~ 1) + 2*video->row_c + field_sel)*pitch_c + (xc >> 1) + video->col_c; \
  HP_FLAG(flag_l, xl, yl); \
  HP_FLAG(flag_c, xc, yc); \
}

#define MCZERO_FRAME(ADD, ref_Y_data1, ref_U_data1, ref_V_data1, flag1, flag2) \
  FUNC_##ADD##_HP(16, 16, ref_Y_data1,pitch_l,     \
                          cur_Y_data,                   \
                          pitch_l, flag1, 0);           \
  FUNC_##ADD##_HP(8, 8, ref_U_data1,pitch_c,     \
                              cur_U_data,               \
                              pitch_c, flag2, 0);      \
  FUNC_##ADD##_HP(8, 8, ref_V_data1,pitch_c,     \
                              cur_V_data,               \
                              pitch_c, flag2, 0);

#define MCZERO_FRAME_422(ADD, ref_Y_data1, ref_U_data1, ref_V_data1, flag1, flag2) \
  FUNC_##ADD##_HP(16, 16, ref_Y_data1,pitch_l,     \
                          cur_Y_data,                   \
                          pitch_l, flag1, 0);           \
  FUNC_##ADD##_HP(8, 16, ref_U_data1,pitch_c,     \
                              cur_U_data,               \
                              pitch_c, flag2, 0);      \
  FUNC_##ADD##_HP(8, 16, ref_V_data1,pitch_c,     \
                              cur_V_data,               \
                              pitch_c, flag2, 0);

#define MCZERO_FIELD0(ADD, ref_Y_data1, ref_U_data1, ref_V_data1, flag1, flag2) \
  FUNC_##ADD##_HP(16, 8, ref_Y_data1, pitch_l2,      \
                      cur_Y_data, pitch_l2,            \
                      flag1, 0);                        \
  FUNC_##ADD##_HP(8, 4, ref_U_data1, pitch_c2,     \
                      cur_U_data, pitch_c2,       \
                      flag2, 0);                    \
  FUNC_##ADD##_HP(8, 4, ref_V_data1, pitch_c2,     \
                      cur_V_data, pitch_c2,       \
                      flag2, 0);

#define MCZERO_FIELD0_422(ADD, ref_Y_data1, ref_U_data1, ref_V_data1, flag1, flag2) \
  FUNC_##ADD##_HP(16, 8, ref_Y_data1, pitch_l2,      \
                      cur_Y_data, pitch_l2,            \
                      flag1, 0);                        \
  FUNC_##ADD##_HP(8, 8, ref_U_data1, pitch_c2,     \
                      cur_U_data, pitch_c2,       \
                      flag2, 0);                    \
  FUNC_##ADD##_HP(8, 8, ref_V_data1, pitch_c2,     \
                      cur_V_data, pitch_c2,       \
                      flag2, 0);

#define MCZERO_FIELD1(ADD, ref_Y_data2, ref_U_data2, ref_V_data2, flag1, flag2) \
  FUNC_##ADD##_HP(16, 8, ref_Y_data2, pitch_l2,       \
                      cur_Y_data + pitch_l, pitch_l2,   \
                      flag1, 0);                         \
  FUNC_##ADD##_HP(8, 4, ref_U_data2, pitch_c2,          \
                      cur_U_data + pitch_c, pitch_c2, \
                      flag2, 0);                         \
  FUNC_##ADD##_HP(8, 4, ref_V_data2, pitch_c2,          \
                      cur_V_data + pitch_c, pitch_c2, \
                      flag2, 0);

#define MCZERO_FIELD1_422(ADD, ref_Y_data2, ref_U_data2, ref_V_data2, flag1, flag2) \
  FUNC_##ADD##_HP(16, 8, ref_Y_data2, pitch_l2,       \
                      cur_Y_data + pitch_l, pitch_l2,   \
                      flag1, 0);                         \
  FUNC_##ADD##_HP(8, 8, ref_U_data2, pitch_c2,          \
                      cur_U_data + pitch_c, pitch_c2, \
                      flag2, 0);                         \
  FUNC_##ADD##_HP(8, 8, ref_V_data2, pitch_c2,          \
                      cur_V_data + pitch_c, pitch_c2, \
                      flag2, 0);

#define SWAP(TYPE, _val0, _val1) { \
  TYPE _tmp = _val0; \
  _val0 = _val1; \
  _val1 = _tmp; \
}

#define RESET_PMV(array) {                         \
  Ipp32u nn;                                       \
  for(nn=0; nn<sizeof(array)/sizeof(Ipp32s); nn++) \
    ((Ipp32s*)array)[nn] = 0;                      \
}

#define COPY_PMV(adst,asrc) {                      \
  Ipp32u nn;                                       \
  for(nn=0; nn<sizeof(adst)/sizeof(Ipp32s); nn++)  \
    ((Ipp32s*)adst)[nn] = ((Ipp32s*)asrc)[nn];     \
}

#define RECONSTRUCT_INTRA_MB(BITSTREAM, NUM_BLK, DCT_TYPE)               \
{                                                                        \
  Ipp32s *pitch = blkPitches[DCT_TYPE];                                  \
  Ipp32s *offsets = blkOffsets[DCT_TYPE];                                \
  Ipp32s curr_index = frame_buffer.curr_index;                           \
  Ipp8u* yuv[3] = {                                                      \
    frame_buffer.frame_p_c_n[curr_index].Y_comp_data + video->offset_l,  \
    frame_buffer.frame_p_c_n[curr_index].U_comp_data + video->offset_c,  \
    frame_buffer.frame_p_c_n[curr_index].V_comp_data + video->offset_c   \
  };                                                                     \
  Ipp32s blk;                                                            \
  DecodeIntraSpec_MPEG2 *intraSpec = &video->decodeIntraSpec;        \
                                                                         \
  for (blk = 0; blk < NUM_BLK; blk++) {                                  \
    IppStatus sts;                                                       \
    Ipp32s chromaFlag, cc;                                               \
    chromaFlag = blk >> 2;                                               \
    cc = chromaFlag + (blk & chromaFlag);                                \
    CHR_SPECINTRA_##NUM_BLK                                              \
                                                                         \
    sts = ownDecodeIntra8x8IDCT_MPEG2_1u8u(                             \
      &BITSTREAM##_curr_ptr,                                             \
      &BITSTREAM##_bit_offset,                                           \
      intraSpec,                                                         \
      video->cur_q_scale,                                                \
      chromaFlag,                                                        \
      &video->dct_dc_past[cc],                                           \
      yuv[cc] + offsets[blk],                                            \
      pitch[chromaFlag]);                                                \
    if(sts != ippStsOk)                                                  \
      return sts;                                                        \
  }                                                                      \
}

#define CHR_SPECINTRA_6
#define CHR_SPECINTRA_8 intraSpec = chromaFlag ? &video->decodeIntraSpecChroma : &video->decodeIntraSpec; \

#define RECONSTRUCT_INTRA_MB_420(BITSTREAM, DCT_TYPE) \
  RECONSTRUCT_INTRA_MB(BITSTREAM, 6, DCT_TYPE)

#define RECONSTRUCT_INTRA_MB_422(BITSTREAM, DCT_TYPE) \
  RECONSTRUCT_INTRA_MB(BITSTREAM, 8, DCT_TYPE)


#define DECODE_MBPATTERN_6(code, BITSTREAM, vlcMBPattern)                \
  DECODE_VLC(code, BITSTREAM, vlcMBPattern)

#define DECODE_MBPATTERN_8(code, BITSTREAM, vlcMBPattern)                \
{                                                                        \
  Ipp32s cbp_1;                                                          \
  DECODE_VLC(code, BITSTREAM, vlcMBPattern);                             \
  GET_TO9BITS(video->bs, 2, cbp_1);                                      \
  code = (code << 2) | cbp_1;                                            \
}

#define RECONSTRUCT_INTER_MB(BITSTREAM, NUM_BLK, DCT_TYPE)               \
{                                                                        \
  DecodeInterSpec_MPEG2 *interSpec = &video->decodeInterSpec;        \
  Ipp32s cur_q_scale = video->cur_q_scale;                               \
  Ipp32s *pitch = blkPitches[DCT_TYPE];                                  \
  Ipp32s *offsets = blkOffsets[DCT_TYPE];                                \
  Ipp32s mask = 1 << (NUM_BLK - 1);                                      \
  Ipp32s code;                                                           \
  Ipp32s blk;                                                            \
                                                                         \
  DECODE_MBPATTERN_##NUM_BLK(code, BITSTREAM, vlcMBPattern);             \
                                                                         \
  for (blk = 0; blk < NUM_BLK; blk++) {                                  \
    if (code & mask) {                                                   \
      IppStatus sts;                                                     \
      Ipp32s chromaFlag = blk >> 2;                                      \
      Ipp32s cc = chromaFlag + (blk & chromaFlag);                       \
      CHR_SPECINTER_##NUM_BLK                                                 \
                                                                         \
      sts = ownDecodeInter8x8IDCTAdd_MPEG2_1u8u(                        \
        &BITSTREAM##_curr_ptr,                                           \
        &BITSTREAM##_bit_offset,                                         \
        interSpec,                                                       \
        cur_q_scale,                                                     \
        video->blkCurrYUV[cc] + offsets[blk],                            \
        pitch[chromaFlag]);                                              \
      if(sts != ippStsOk)                                                \
        return sts;                                                      \
    }                                                                    \
    code += code;                                                        \
  }                                                                      \
}

#define CHR_SPECINTER_6
#define CHR_SPECINTER_8 interSpec = chromaFlag ? &video->decodeInterSpecChroma : &video->decodeInterSpec;

#define RECONSTRUCT_INTER_MB_420(BITSTREAM, DCT_TYPE) \
  RECONSTRUCT_INTER_MB(BITSTREAM, 6, DCT_TYPE)

#define RECONSTRUCT_INTER_MB_422(BITSTREAM, DCT_TYPE) \
  RECONSTRUCT_INTER_MB(BITSTREAM, 8, DCT_TYPE)

#define UPDATE_MV(val, mcode, S) \
  if(PictureHeader.r_size[S]) { \
    GET_TO9BITS(video->bs,PictureHeader.r_size[S], residual); \
    if(mcode < 0) { \
        val += ((mcode + 1) << PictureHeader.r_size[S]) - (residual + 1); \
        if(val < PictureHeader.low_in_range[S]) \
          val += PictureHeader.range[S]; \
    } else { \
        val += ((mcode - 1) << PictureHeader.r_size[S]) + (residual + 1); \
        if(val > PictureHeader.high_in_range[S]) \
          val -= PictureHeader.range[S]; \
    } \
  } else { \
    val += mcode; \
    if(val < PictureHeader.low_in_range[S]) \
      val += PictureHeader.range[S]; \
    else if(val > PictureHeader.high_in_range[S]) \
      val -= PictureHeader.range[S]; \
  }

#define DECODE_MV(BS, R, S, vectorX, vectorY)           \
  /* R = 2*(2*r + s); S = 2*s */                        \
  /* Decode x vector */                                 \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&video->PMV[R], S, video);                \
  }                                                     \
  vectorX = video->PMV[R];                              \
                                                        \
  /* Decode y vector */                                 \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&video->PMV[R + 1], S + 1, video);        \
  }                                                     \
  vectorY = video->PMV[R + 1]

#define DECODE_MV_FIELD(BS, R, S, vectorX, vectorY)     \
  /* R = 2*(2*r + s); S = 2*s */                        \
  /* Decode x vector */                                 \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&video->PMV[R], S, video);                \
  }                                                     \
  vectorX = video->PMV[R];                              \
                                                        \
  /* Decode y vector */                                 \
  vectorY = video->PMV[R + 1] >> 1;                     \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&vectorY, S + 1, video);                  \
  }                                                     \
  video->PMV[R + 1] = vectorY << 1

#define DECODE_MV_FULLPEL(BS, R, S, vectorX, vectorY)   \
  /* R = 2*(2*r + s); S = 2*s */                        \
  /* Decode x vector */                                 \
  vectorX = video->PMV[R] >> 1;                         \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&vectorX, S, video);                      \
  }                                                     \
  video->PMV[R] = vectorX << 1;                         \
                                                        \
  /* Decode y vector */                                 \
  vectorY = video->PMV[R + 1] >> 1;                     \
  if (IS_NEXTBIT1(BS)) {                                \
    SKIP_BITS(BS, 1)                                    \
  } else {                                              \
    update_mv(&vectorY, S + 1, video);                  \
  }                                                     \
  video->PMV[R + 1] = vectorY << 1

#define DECODE_QUANTIZER_SCALE(BS, Q_SCALE) \
{                                           \
  Ipp32s _q_scale;                             \
  GET_TO9BITS(video->bs, 5, _q_scale)       \
  if (_q_scale < 1) {                       \
    return UMC_ERR_INVALID_STREAM;                  \
  }                                         \
  Q_SCALE = q_scale[PictureHeader.q_scale_type][_q_scale]; \
}

#define DECODE_MB_INCREMENT(BS, macroblock_address_increment)         \
{                                                                     \
  Ipp32s code;                                                        \
  SHOW_BITS(BS, 11, code)                                             \
                                                                      \
  if(code == 0) {                                                     \
    return UMC_OK; /* end of slice or bad code. Anyway, stop slice */ \
  }                                                                   \
                                                                      \
  macroblock_address_increment = 0;                                   \
  while(code == 8)                                                    \
  {                                                                   \
    macroblock_address_increment += 33; /* macroblock_escape */       \
    GET_BITS(BS, 11, code);                                           \
    SHOW_BITS(BS, 11, code)                                           \
  }                                                                   \
  DECODE_VLC(code, BS, vlcMBAdressing);                               \
  macroblock_address_increment += code;                               \
  macroblock_address_increment--;                                     \
  if (macroblock_address_increment > (sequenceHeader.mb_width - video->mb_col)) { \
    macroblock_address_increment = sequenceHeader.mb_width - video->mb_col;       \
  }                                                                               \
}

////////////////////////////////////////////////////////////////////

#define APPLY_SIGN(val, sign)  \
  val += sign;                 \
  if (val > 2047) val = 2047; /* with saturation */ \
  val ^= sign

#define SAT(val) \
  if (val > 2047) val = 2047;   \
  if (val < -2048) val = -2048;

#ifdef LOCAL_BUFFERS
#define DEF_BLOCK(NAME) \
  Ipp16s NAME[64];
#else
#define DEF_BLOCK(NAME) \
  Ipp16s *NAME = pQuantSpec->NAME;
#endif

#define MP2_FUNC(type, name, arg)  type name arg

#define FUNC_DCT8x8      ippiDCT8x8Inv_16s_C1
#define FUNC_DCT4x4      ippiDCT8x8Inv_4x4_16s_C1
#define FUNC_DCT2x2      ippiDCT8x8Inv_2x2_16s_C1
#define FUNC_DCT8x8Intra ippiDCT8x8Inv_16s8u_C1R
#define FUNC_ADD8x8      ippiAdd8x8_16s8u_C1IRS

void IDCTAdd_1x1to8x8(Ipp32s val, Ipp8u* y, Ipp32s step);
void IDCTAdd_1x4to8x8(const Ipp16s* x, Ipp8u* y, Ipp32s step);
void Pack8x8(Ipp16s* x, Ipp8u* y, Ipp32s step);


#ifdef MPEG2_USE_REF_IDCT

extern "C" {
void Reference_IDCT(Ipp16s *block, Ipp16s *out, Ipp32s step);
//#define Reference_IDCT(in, out, step) ippiDCT8x8Inv_16s_C1R(in, out, 2*(step))
} /* extern "C" */

#define IDCT_INTER(SRC, NUM, BUFF, DST, STEP)        \
  Reference_IDCT(SRC, BUFF, 8);                      \
  FUNC_ADD8x8(BUFF, 16, DST, STEP)

#define IDCT_INTER1(val, idct, pSrcDst, srcDstStep)  \
  SAT(val);                                          \
  for (k = 0; k < 64; k++) pDstBlock[k] = 0;         \
  pDstBlock[0] = val;                                \
  mask = 1 ^ val;                                    \
  pDstBlock[63] ^= mask & 1;                         \
  IDCT_INTER(pDstBlock, 0, idct, pSrcDst, srcDstStep)

#define IDCT_INTRA(SRC, NUM, BUFF, DST, STEP)       \
  Ipp32s ii, jj;                                    \
  SRC[0] -= 1024;                                   \
  Reference_IDCT(SRC, BUFF, 8);                     \
  for(ii = 0; ii < 8; ii++) {                       \
    for(jj = 0; jj < 8; jj++) {                     \
      BUFF[ii * 8 + jj] += 128;                     \
    }                                               \
  }                                                 \
  Pack8x8(BUFF, DST, STEP)

#define IDCT_INTRA1(val, idct, pSrcDst, srcDstStep)  \
  SAT(val);                                          \
  for (k = 0; k < 64; k++) pDstBlock[k] = 0;         \
  pDstBlock[0] = val;                                \
  mask = 1 ^ val;                                    \
  pDstBlock[63] ^= mask & 1;                         \
  IDCT_INTRA(pDstBlock, 0, idct, pSrcDst, srcDstStep)

#else /* MPEG2_USE_REF_IDCT */

#ifdef USE_INTRINSICS
#define IDCT_INTER_1x4(SRC, NUM, DST, STEP)        \
  if (NUM < 4 && !SRC[1]) {                        \
    IDCTAdd_1x4to8x8(SRC, DST, STEP);              \
  } else
#else
#define IDCT_INTER_1x4(SRC, NUM, DST, STEP)
#endif

#define IDCT_INTER(SRC, NUM, BUFF, DST, STEP)      \
  if (NUM < 10) {                                  \
    if (!NUM) {                                    \
      IDCTAdd_1x1to8x8(SRC[0], DST, STEP);         \
    } else                                         \
    IDCT_INTER_1x4(SRC, NUM, DST, STEP)            \
    {                                              \
      FUNC_DCT4x4(SRC, BUFF);                      \
      FUNC_ADD8x8(BUFF, 16, DST, STEP);            \
    }                                              \
  } else {                                         \
    FUNC_DCT8x8(SRC, BUFF);                        \
    FUNC_ADD8x8(BUFF, 16, DST, STEP);              \
  }

#define IDCT_INTER1(val, idct, pSrcDst, srcDstStep) \
  ippiAddC8x8_16s8u_C1IR((Ipp16s)((val + 4) >> 3), pSrcDst, srcDstStep)

#ifdef USE_INTRINSICS

#define IDCT_INTRA(SRC, NUM, BUFF, DST, STEP)                   \
  if (NUM < 10) {                                               \
    if (!NUM) {                                                 \
      ippiDCT8x8Inv_AANTransposed_16s8u_C1R(SRC, DST, STEP, 0); \
    } else {                                                    \
      FUNC_DCT4x4(SRC, BUFF);                                   \
      Pack8x8(BUFF, DST, STEP);                                 \
    }                                                           \
  } else {                                                      \
    FUNC_DCT8x8Intra(SRC, DST, STEP);                           \
  }

#else

#define IDCT_INTRA(SRC, NUM, BUFF, DST, STEP)                   \
  if (!NUM) {                                                   \
    ippiDCT8x8Inv_AANTransposed_16s8u_C1R(SRC, DST, STEP, 0);   \
  } else {                                                      \
    FUNC_DCT8x8Intra(SRC, DST, STEP);                           \
  }

#endif

#define IDCT_INTRA1(val, idct, pSrcDst, srcDstStep) \
  pDstBlock[0] = (Ipp16s)val; \
  ippiDCT8x8Inv_AANTransposed_16s8u_C1R(pDstBlock, pSrcDst, srcDstStep, 0)

#endif // MPEG2_USE_REF_IDCT
#endif // __UMC_MPEG2_DEC_DEFS_H
