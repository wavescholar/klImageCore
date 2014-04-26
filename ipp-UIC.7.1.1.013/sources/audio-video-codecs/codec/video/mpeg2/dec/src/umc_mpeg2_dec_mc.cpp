/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER

#include "umc_mpeg2_dec_base.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4244)
#endif

using namespace UMC;

/****************************************************************/

Ipp16s zero_memory[64*8] = {0};

#define MC_FORWARD0(H, PITCH_L, PITCH_C)                                                    \
  Ipp32s prev_index = frame_buffer.prev_index;                                              \
  Ipp32s curr_index = frame_buffer.curr_index;                                              \
  Ipp8u *ref_Y_data = frame_buffer.frame_p_c_n[prev_index].Y_comp_data;                     \
  Ipp8u *ref_U_data = frame_buffer.frame_p_c_n[prev_index].U_comp_data;                     \
  Ipp8u *ref_V_data = frame_buffer.frame_p_c_n[prev_index].V_comp_data;                     \
  Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[curr_index].Y_comp_data;                     \
  Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[curr_index].U_comp_data;                     \
  Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[curr_index].V_comp_data;                     \
  Ipp32s offset_l = video->offset_l;                                                        \
  Ipp32s offset_c = video->offset_c;                                                        \
  ippiCopy16x16_8u_C1R(ref_Y_data + offset_l, PITCH_L, cur_Y_data + offset_l, PITCH_L);     \
  ippiCopy8x##H##_8u_C1R(ref_U_data + offset_c, PITCH_C, cur_U_data + offset_c, PITCH_C);   \
  ippiCopy8x##H##_8u_C1R(ref_V_data + offset_c, PITCH_C, cur_V_data + offset_c, PITCH_C);

void MPEG2VideoDecoderBase::mc_frame_forward0_420(Mpeg2VideoContext *video)
{
    MC_FORWARD0(8, frame_buffer.Y_comp_pitch, frame_buffer.U_comp_pitch);
}

void MPEG2VideoDecoderBase::mc_frame_forward0_422(Mpeg2VideoContext *video)
{
    MC_FORWARD0(16, frame_buffer.Y_comp_pitch, frame_buffer.U_comp_pitch);
}

void MPEG2VideoDecoderBase::mc_field_forward0_420(Mpeg2VideoContext *video)
{
    MC_FORWARD0(8, 2 * frame_buffer.Y_comp_pitch, 2 * frame_buffer.U_comp_pitch);
}

void MPEG2VideoDecoderBase::mc_field_forward0_422(Mpeg2VideoContext *video)
{
    MC_FORWARD0(16, 2 * frame_buffer.Y_comp_pitch, 2 * frame_buffer.U_comp_pitch);
}

#define buffFRW  prev_index
#define buffBKW  next_index
#define indexFRW  0
#define indexBKW  2

#define FUNC_MC_MBLOCK_420(DIR, METH, FLG)                                               \
  Ipp8u *refY = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].Y_comp_data;            \
  Ipp8u *refU = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].U_comp_data;            \
  Ipp8u *refV = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].V_comp_data;            \
  Ipp8u *curY = video->blkCurrYUV[0];                                                    \
  Ipp8u *curU = video->blkCurrYUV[1];                                                    \
  Ipp8u *curV = video->blkCurrYUV[2];                                                    \
  Ipp32u pitch_l = frame_buffer.Y_comp_pitch;                                            \
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;                                            \
  Ipp32s flag_l, flag_c;                                                                 \
  Ipp32s offs_l, offs_c;                                                                 \
  Ipp16s vec_x, vec_y;                                                                   \
                                                                                         \
  if(video->prediction_type == IPPVC_MC_FRAME)                                           \
  {                                                                                      \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x, vec_y);                          \
    video->PMV[index##DIR + 4] = video->PMV[index##DIR];                                 \
    video->PMV[index##DIR + 5] = video->PMV[index##DIR + 1];                             \
                                                                                         \
    CALC_OFFSETS_FRAME_420(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, HP_FLAG_##FLG)  \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l, 16+(vec_y&1))                              \
                                                                                         \
    FUNC_##METH##_HP(16, 16, refY + offs_l, pitch_l, curY, pitch_l, flag_l, 0);          \
    FUNC_##METH##_HP(8,  8,  refU + offs_c, pitch_c, curU, pitch_c, flag_c, 0);          \
    FUNC_##METH##_HP(8,  8,  refV + offs_c, pitch_c, curV, pitch_c, flag_c, 0);          \
  }                                                                                      \
  else                                                                                   \
  {                                                                                      \
    Ipp32u pitch_l2 = pitch_l + pitch_l;                                                 \
    Ipp32u pitch_c2 = pitch_c + pitch_c;                                                 \
    Ipp32s field_sel0, field_sel1;                                                       \
    Ipp16s vec1_x, vec1_y;                                                               \
                                                                                         \
    GET_1BIT(video->bs, field_sel0);                                                     \
    DECODE_MV_FIELD(video->bs, index##DIR, index##DIR, vec_x, vec_y);                    \
    GET_1BIT(video->bs, field_sel1);                                                     \
    DECODE_MV_FIELD(video->bs, index##DIR + 4, index##DIR, vec1_x, vec1_y);              \
                                                                                                  \
    CALC_OFFSETS_FIELD_420(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, field_sel0, HP_FLAG_##FLG)   \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l2, 8+(vec_y&1))                                       \
                                                                                                  \
    FUNC_##METH##_HP(16, 8, refY + offs_l, pitch_l2, curY, pitch_l2, flag_l, 0);                  \
    FUNC_##METH##_HP(8,  4, refU + offs_c, pitch_c2, curU, pitch_c2, flag_c, 0);                  \
    FUNC_##METH##_HP(8,  4, refV + offs_c, pitch_c2, curV, pitch_c2, flag_c, 0);                  \
                                                                                                  \
    CALC_OFFSETS_FIELD_420(offs_l, offs_c, flag_l, flag_c, vec1_x, vec1_y, field_sel1, HP_FLAG_##FLG) \
    CHECK_OFFSET_L(offs_l, (vec1_x&1), pitch_l2, 8+(vec1_y&1))                                     \
                                                                                                  \
    FUNC_##METH##_HP(16, 8, refY + offs_l, pitch_l2, curY + pitch_l, pitch_l2, flag_l, 0);        \
    FUNC_##METH##_HP(8,  4, refU + offs_c, pitch_c2, curU + pitch_c, pitch_c2, flag_c, 0);        \
    FUNC_##METH##_HP(8,  4, refV + offs_c, pitch_c2, curV + pitch_c, pitch_c2, flag_c, 0);        \
  }                                                                                               \
  return UMC_OK

Status MPEG2VideoDecoderBase::mc_frame_forward_420(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_420(FRW, COPY, CP);
}


Status MPEG2VideoDecoderBase::mc_frame_backward_420(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_420(BKW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_frame_backward_add_420(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_420(BKW, AVE, AV);
}

#define FUNC_MC_MBLOCK_422(DIR, METH, FLG)                                               \
  Ipp8u *refY = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].Y_comp_data;            \
  Ipp8u *refU = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].U_comp_data;            \
  Ipp8u *refV = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].V_comp_data;            \
  Ipp8u *curY = video->blkCurrYUV[0];                                                    \
  Ipp8u *curU = video->blkCurrYUV[1];                                                    \
  Ipp8u *curV = video->blkCurrYUV[2];                                                    \
  Ipp32u pitch_l = frame_buffer.Y_comp_pitch;                                            \
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;                                            \
  Ipp32s flag_l, flag_c;                                                                 \
  Ipp32s offs_l, offs_c;                                                                 \
  Ipp16s vec_x, vec_y;                                                                   \
                                                                                         \
  if(video->prediction_type == IPPVC_MC_FRAME)                                           \
  {                                                                                      \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x, vec_y);                          \
    video->PMV[index##DIR + 4] = video->PMV[index##DIR];                                 \
    video->PMV[index##DIR + 5] = video->PMV[index##DIR + 1];                             \
                                                                                         \
    CALC_OFFSETS_FRAME_422(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, HP_FLAG_##FLG)  \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l, 16+(vec_y&1))                              \
                                                                                         \
    FUNC_##METH##_HP(16, 16, refY + offs_l, pitch_l, curY, pitch_l, flag_l, 0);          \
    FUNC_##METH##_HP(8,  16, refU + offs_c, pitch_c, curU, pitch_c, flag_c, 0);          \
    FUNC_##METH##_HP(8,  16, refV + offs_c, pitch_c, curV, pitch_c, flag_c, 0);          \
  }                                                                                      \
  else                                                                                   \
  {                                                                                      \
    Ipp32u pitch_l2 = pitch_l + pitch_l;                                                 \
    Ipp32u pitch_c2 = pitch_c + pitch_c;                                                 \
    Ipp32s field_sel0, field_sel1;                                                       \
    Ipp16s vec1_x, vec1_y;                                                               \
                                                                                         \
    GET_1BIT(video->bs, field_sel0);                                                     \
    DECODE_MV_FIELD(video->bs, index##DIR, index##DIR, vec_x, vec_y);                    \
    GET_1BIT(video->bs, field_sel1);                                                     \
    DECODE_MV_FIELD(video->bs, index##DIR + 4, index##DIR, vec1_x, vec1_y);              \
                                                                                                        \
    CALC_OFFSETS_FIELD_422(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, field_sel0, HP_FLAG_##FLG)     \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l2, 8+(vec_y&1))                                             \
                                                                                                        \
    FUNC_##METH##_HP(16, 8, refY + offs_l, pitch_l2, curY, pitch_l2, flag_l, 0);                        \
    FUNC_##METH##_HP(8,  8, refU + offs_c, pitch_c2, curU, pitch_c2, flag_c, 0);                        \
    FUNC_##METH##_HP(8,  8, refV + offs_c, pitch_c2, curV, pitch_c2, flag_c, 0);                        \
                                                                                                        \
    CALC_OFFSETS_FIELD_422(offs_l, offs_c, flag_l, flag_c, vec1_x, vec1_y, field_sel1, HP_FLAG_##FLG)   \
    CHECK_OFFSET_L(offs_l, (vec1_x&1), pitch_l2, 8+(vec1_y&1))                                           \
                                                                                                        \
    FUNC_##METH##_HP(16, 8, refY + offs_l, pitch_l2, curY + pitch_l, pitch_l2, flag_l, 0);              \
    FUNC_##METH##_HP(8,  8, refU + offs_c, pitch_c2, curU + pitch_c, pitch_c2, flag_c, 0);              \
    FUNC_##METH##_HP(8,  8, refV + offs_c, pitch_c2, curV + pitch_c, pitch_c2, flag_c, 0);              \
  }                                                                                                     \
  return UMC_OK

Status MPEG2VideoDecoderBase::mc_frame_forward_422(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_422(FRW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_frame_backward_422(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_422(BKW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_frame_backward_add_422(Mpeg2VideoContext *video)
{
  FUNC_MC_MBLOCK_422(BKW, AVE, AV);
}

#define FUNC_MC_FULLPEL(DIR, METH, FLG)                                       \
  Ipp8u *refY = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].Y_comp_data; \
  Ipp8u *refU = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].U_comp_data; \
  Ipp8u *refV = frame_buffer.frame_p_c_n[frame_buffer.buff##DIR].V_comp_data; \
  Ipp8u *curY = video->blkCurrYUV[0];                                         \
  Ipp8u *curU = video->blkCurrYUV[1];                                         \
  Ipp8u *curV = video->blkCurrYUV[2];                                         \
  Ipp32u pitch_l = frame_buffer.Y_comp_pitch;                                 \
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;                                 \
  Ipp32s offs_l, offs_c;                                                      \
  Ipp16s vec_x, vec_y;                                                        \
                                                                              \
  DECODE_MV_FULLPEL(video->bs, index##DIR, index##DIR, vec_x, vec_y);         \
                                                                              \
  CALC_OFFSETS_FULLPEL(offs_l, offs_c, vec_x, vec_y, pitch_l, pitch_c)        \
  CHECK_OFFSET_L(offs_l, 0, pitch_l, 16)                                         \
                                                                              \
  FUNC_##METH##_HP(16, 16, refY + offs_l, pitch_l, curY, pitch_l, 0, 0);      \
  FUNC_##METH##_HP(8,  8,  refU + offs_c, pitch_c, curU, pitch_c, 0, 0);      \
  FUNC_##METH##_HP(8,  8,  refV + offs_c, pitch_c, curV, pitch_c, 0, 0);      \
                                                                              \
  return UMC_OK

Status MPEG2VideoDecoderBase::mc_fullpel_forward(Mpeg2VideoContext *video)
{
  FUNC_MC_FULLPEL(FRW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_fullpel_backward(Mpeg2VideoContext *video)
{
  FUNC_MC_FULLPEL(BKW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_fullpel_backward_add(Mpeg2VideoContext *video)
{
  FUNC_MC_FULLPEL(BKW, AVE, AV);
}

#define FIELD_BUFF_IND(field_sel) \
  ((frame_buffer.field_buffer_index && PictureHeader.picture_structure + field_sel == BOTTOM_FIELD) \
   ? frame_buffer.curr_index : frame_buffer.prev_index)

#define FUNC_MC_FIELD_420(DIR, METH, FLG)                                                           \
  Ipp8u *cur_Y = video->blkCurrYUV[0];                                                              \
  Ipp8u *cur_U = video->blkCurrYUV[1];                                                              \
  Ipp8u *cur_V = video->blkCurrYUV[2];                                                              \
  Ipp32u pitch_l = frame_buffer.Y_comp_pitch;                                                       \
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;                                                       \
                                                                                                    \
  if(video->prediction_type == IPPVC_MC_FIELD)                                                      \
  {                                                                                                 \
    Ipp8u *ref_Y;                                                                                   \
    Ipp8u *ref_U;                                                                                   \
    Ipp8u *ref_V;                                                                                   \
    Ipp32s offs_l, offs_c;                                                                          \
    Ipp32s flag_l, flag_c;                                                                          \
    Ipp16s vec_x, vec_y;                                                                            \
    Ipp32s field_sel;                                                                               \
                                                                                                    \
    GET_1BIT(video->bs, field_sel);                                                                 \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x, vec_y);                                     \
    video->PMV[index##DIR + 4] = video->PMV[index##DIR + 0];                                        \
    video->PMV[index##DIR + 5] = video->PMV[index##DIR + 1];                                        \
                                                                                                    \
    Ipp32s ind0 = FIELD_BUFF_IND(field_sel);                                                        \
    if (PictureHeader.picture_coding_type == MPEG2_B_PICTURE)                                             \
      ind0 = frame_buffer.buff##DIR;                                                                \
                                                                                                    \
    ref_Y = frame_buffer.frame_p_c_n[ind0].Y_comp_data;                                             \
    ref_U = frame_buffer.frame_p_c_n[ind0].U_comp_data;                                             \
    ref_V = frame_buffer.frame_p_c_n[ind0].V_comp_data;                                             \
                                                                                                    \
    CALC_OFFSETS_FIELDX_420(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, field_sel, HP_FLAG_##FLG) \
    pitch_l *= 2;                                                                                   \
    pitch_c *= 2;                                                                                   \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l, 16+(vec_y&1))                                         \
    FUNC_##METH##_HP(16, 16, ref_Y + offs_l, pitch_l, cur_Y, pitch_l, flag_l, 0);                   \
    FUNC_##METH##_HP(8,  8,  ref_U + offs_c, pitch_c, cur_U, pitch_c, flag_c, 0);                   \
    FUNC_##METH##_HP(8,  8,  ref_V + offs_c, pitch_c, cur_V, pitch_c, flag_c, 0);                   \
  }                                                                                                 \
  else /* 16x8 */                                                                                   \
  {                                                                                                 \
    Ipp16s vec_x0, vec_y0, vec_x1, vec_y1;                                                          \
    Ipp32s field_sel0, field_sel1;                                                                  \
                                                                                                    \
    GET_1BIT(video->bs, field_sel0);                                                                \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x0, vec_y0);                                   \
                                                                                                    \
    GET_1BIT(video->bs, field_sel1);                                                                \
    DECODE_MV(video->bs, index##DIR + 4, index##DIR, vec_x1, vec_y1);                               \
                                                                                                    \
    Ipp32s ind0 = FIELD_BUFF_IND(field_sel0);                                                       \
    Ipp32s ind1 = FIELD_BUFF_IND(field_sel1);                                                       \
    if (PictureHeader.picture_coding_type == MPEG2_B_PICTURE)                                             \
      ind0 = ind1 = frame_buffer.buff##DIR;                                                         \
                                                                                                    \
    Ipp8u *ref_Y0 = frame_buffer.frame_p_c_n[ind0].Y_comp_data;                                     \
    Ipp8u *ref_U0 = frame_buffer.frame_p_c_n[ind0].U_comp_data;                                     \
    Ipp8u *ref_V0 = frame_buffer.frame_p_c_n[ind0].V_comp_data;                                     \
    Ipp8u *ref_Y1 = frame_buffer.frame_p_c_n[ind1].Y_comp_data;                                     \
    Ipp8u *ref_U1 = frame_buffer.frame_p_c_n[ind1].U_comp_data;                                     \
    Ipp8u *ref_V1 = frame_buffer.frame_p_c_n[ind1].V_comp_data;                                     \
    Ipp32s offs_l0, offs_c0, flag_l0, flag_c0;                                                      \
    Ipp32s offs_l1, offs_c1, flag_l1, flag_c1;                                                      \
                                                                                                    \
    CALC_OFFSETS_FIELDX_420(offs_l0, offs_c0, flag_l0, flag_c0, vec_x0, vec_y0, field_sel0, HP_FLAG_##FLG) \
    CALC_OFFSETS_FIELDX_420(offs_l1, offs_c1, flag_l1, flag_c1, vec_x1, vec_y1, field_sel1, HP_FLAG_##FLG) \
    pitch_l *= 2;                                                                                   \
    pitch_c *= 2;                                                                                   \
    offs_l1 += 8*pitch_l;                                                                           \
    offs_c1 += 4*pitch_c;                                                                           \
    CHECK_OFFSET_L(offs_l0, (vec_x0&1), pitch_l, 8+(vec_y0&1))                                       \
    CHECK_OFFSET_L(offs_l1, (vec_x1&1), pitch_l, 8+(vec_y1&1))                                       \
    FUNC_##METH##_HP(16, 8, ref_Y0 + offs_l0, pitch_l, cur_Y, pitch_l, flag_l0, 0);                 \
    FUNC_##METH##_HP(16, 8, ref_Y1 + offs_l1, pitch_l, cur_Y + 8*pitch_l, pitch_l, flag_l1, 0);     \
    FUNC_##METH##_HP( 8, 4, ref_U0 + offs_c0, pitch_c, cur_U, pitch_c, flag_c0, 0);                 \
    FUNC_##METH##_HP( 8, 4, ref_U1 + offs_c1, pitch_c, cur_U + 4*pitch_c, pitch_c, flag_c1, 0);     \
    FUNC_##METH##_HP( 8, 4, ref_V0 + offs_c0, pitch_c, cur_V, pitch_c, flag_c0, 0);                 \
    FUNC_##METH##_HP( 8, 4, ref_V1 + offs_c1, pitch_c, cur_V + 4*pitch_c, pitch_c, flag_c1, 0);     \
  }                                                                                                 \
  return UMC_OK

Status MPEG2VideoDecoderBase::mc_field_forward_420(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_420(FRW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_field_backward_420(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_420(BKW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_field_backward_add_420(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_420(BKW, AVE, AV);
}

#define FUNC_MC_FIELD_422(DIR, METH, FLG)                                                           \
  Ipp8u *cur_Y = video->blkCurrYUV[0];                                                              \
  Ipp8u *cur_U = video->blkCurrYUV[1];                                                              \
  Ipp8u *cur_V = video->blkCurrYUV[2];                                                              \
  Ipp32u pitch_l = frame_buffer.Y_comp_pitch;                                                       \
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;                                                       \
                                                                                                    \
  if(video->prediction_type == IPPVC_MC_FIELD)                                                      \
  {                                                                                                 \
    Ipp8u *ref_Y;                                                                                   \
    Ipp8u *ref_U;                                                                                   \
    Ipp8u *ref_V;                                                                                   \
    Ipp32s offs_l, offs_c;                                                                          \
    Ipp32s flag_l, flag_c;                                                                          \
    Ipp16s vec_x, vec_y;                                                                            \
    Ipp32s field_sel;                                                                               \
                                                                                                    \
    GET_1BIT(video->bs, field_sel);                                                                 \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x, vec_y);                                     \
    video->PMV[index##DIR + 4] = video->PMV[index##DIR + 0];                                        \
    video->PMV[index##DIR + 5] = video->PMV[index##DIR + 1];                                        \
                                                                                                    \
    Ipp32s ind0 = FIELD_BUFF_IND(field_sel);                                                        \
    if (PictureHeader.picture_coding_type == MPEG2_B_PICTURE)                                             \
      ind0 = frame_buffer.buff##DIR;                                                                \
                                                                                                    \
    ref_Y = frame_buffer.frame_p_c_n[ind0].Y_comp_data;                                             \
    ref_U = frame_buffer.frame_p_c_n[ind0].U_comp_data;                                             \
    ref_V = frame_buffer.frame_p_c_n[ind0].V_comp_data;                                             \
                                                                                                    \
    CALC_OFFSETS_FIELDX_422(offs_l, offs_c, flag_l, flag_c, vec_x, vec_y, field_sel, HP_FLAG_##FLG) \
    pitch_l *= 2;                                                                                   \
    pitch_c *= 2;                                                                                   \
    CHECK_OFFSET_L(offs_l, (vec_x&1), pitch_l, 16+(vec_y&1))                                         \
    FUNC_##METH##_HP(16, 16, ref_Y + offs_l, pitch_l, cur_Y, pitch_l, flag_l, 0);                   \
    FUNC_##METH##_HP(8,  16, ref_U + offs_c, pitch_c, cur_U, pitch_c, flag_c, 0);                   \
    FUNC_##METH##_HP(8,  16, ref_V + offs_c, pitch_c, cur_V, pitch_c, flag_c, 0);                   \
  }                                                                                                 \
  else /* 16x8 */                                                                                   \
  {                                                                                                 \
    Ipp16s vec_x0, vec_y0, vec_x1, vec_y1;                                                          \
    Ipp32s field_sel0, field_sel1;                                                                  \
                                                                                                    \
    GET_1BIT(video->bs, field_sel0);                                                                \
    DECODE_MV(video->bs, index##DIR, index##DIR, vec_x0, vec_y0);                                   \
                                                                                                    \
    GET_1BIT(video->bs, field_sel1);                                                                \
    DECODE_MV(video->bs, index##DIR + 4, index##DIR, vec_x1, vec_y1);                               \
                                                                                                    \
    Ipp32s ind0 = FIELD_BUFF_IND(field_sel0);                                                       \
    Ipp32s ind1 = FIELD_BUFF_IND(field_sel1);                                                       \
    if (PictureHeader.picture_coding_type == MPEG2_B_PICTURE)                                             \
      ind0 = ind1 = frame_buffer.buff##DIR;                                                         \
                                                                                                    \
    Ipp8u *ref_Y0 = frame_buffer.frame_p_c_n[ind0].Y_comp_data;                                     \
    Ipp8u *ref_U0 = frame_buffer.frame_p_c_n[ind0].U_comp_data;                                     \
    Ipp8u *ref_V0 = frame_buffer.frame_p_c_n[ind0].V_comp_data;                                     \
    Ipp8u *ref_Y1 = frame_buffer.frame_p_c_n[ind1].Y_comp_data;                                     \
    Ipp8u *ref_U1 = frame_buffer.frame_p_c_n[ind1].U_comp_data;                                     \
    Ipp8u *ref_V1 = frame_buffer.frame_p_c_n[ind1].V_comp_data;                                     \
    Ipp32s offs_l0, offs_c0, flag_l0, flag_c0;                                                      \
    Ipp32s offs_l1, offs_c1, flag_l1, flag_c1;                                                      \
                                                                                                    \
    CALC_OFFSETS_FIELDX_422(offs_l0, offs_c0, flag_l0, flag_c0, vec_x0, vec_y0, field_sel0, HP_FLAG_##FLG) \
    CALC_OFFSETS_FIELDX_422(offs_l1, offs_c1, flag_l1, flag_c1, vec_x1, vec_y1, field_sel1, HP_FLAG_##FLG) \
    pitch_l *= 2;                                                                                   \
    pitch_c *= 2;                                                                                   \
    offs_l1 += 8*pitch_l;                                                                           \
    offs_c1 += 8*pitch_c;                                                                           \
    CHECK_OFFSET_L(offs_l0, (vec_x0&1), pitch_l, 8+(vec_y0&1))                                       \
    CHECK_OFFSET_L(offs_l1, (vec_x1&1), pitch_l, 8+(vec_y1&1))                                       \
    FUNC_##METH##_HP(16, 8, ref_Y0 + offs_l0, pitch_l, cur_Y, pitch_l, flag_l0, 0);                 \
    FUNC_##METH##_HP(16, 8, ref_Y1 + offs_l1, pitch_l, cur_Y + 8*pitch_l, pitch_l, flag_l1, 0);     \
    FUNC_##METH##_HP( 8, 8, ref_U0 + offs_c0, pitch_c, cur_U, pitch_c, flag_c0, 0);                 \
    FUNC_##METH##_HP( 8, 8, ref_U1 + offs_c1, pitch_c, cur_U + 8*pitch_c, pitch_c, flag_c1, 0);     \
    FUNC_##METH##_HP( 8, 8, ref_V0 + offs_c0, pitch_c, cur_V, pitch_c, flag_c0, 0);                 \
    FUNC_##METH##_HP( 8, 8, ref_V1 + offs_c1, pitch_c, cur_V + 8*pitch_c, pitch_c, flag_c1, 0);     \
  }                                                                                                 \
  return UMC_OK

Status MPEG2VideoDecoderBase::mc_field_forward_422(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_422(FRW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_field_backward_422(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_422(BKW, COPY, CP);
}

Status MPEG2VideoDecoderBase::mc_field_backward_add_422(Mpeg2VideoContext *video)
{
  FUNC_MC_FIELD_422(BKW, AVE, AV);
}

Status MPEG2VideoDecoderBase::mc_mp2_420_skip(Mpeg2VideoContext *video)
{
  Ipp32s ref_index = (video->macroblock_motion_backward) ? frame_buffer.next_index : frame_buffer.prev_index;
  Ipp8u *ref_Y_data = frame_buffer.frame_p_c_n[ref_index].Y_comp_data;
  Ipp8u *ref_U_data = frame_buffer.frame_p_c_n[ref_index].U_comp_data;
  Ipp8u *ref_V_data = frame_buffer.frame_p_c_n[ref_index].V_comp_data;
  Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
  Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
  Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  Ipp32u pitch_l  = frame_buffer.Y_comp_pitch;
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;
  Ipp32u flag1, flag2;
  Ipp32s offs_uv1, offs_y1;
  Ipp16s *vector = video->vector;
  IppiSize roi = {16*(video->mb_address_increment - 1), 16};
  Ipp32s i;

  cur_Y_data += video->offset_l;
  cur_U_data += video->offset_c;
  cur_V_data += video->offset_c;

  if (video->macroblock_motion_backward) vector += 2;

  if (PictureHeader.picture_coding_type == MPEG2_P_PICTURE) {
    ref_Y_data += video->offset_l;
    ref_U_data += video->offset_c;
    ref_V_data += video->offset_c;
    if(PictureHeader.picture_structure != FRAME_PICTURE) {
      if(PictureHeader.picture_structure == BOTTOM_FIELD) {
        cur_Y_data += pitch_l;
        cur_U_data += pitch_c;
        cur_V_data += pitch_c;
      }
      pitch_l <<= 1;
      pitch_c <<= 1;
    }
    ippiCopy_8u_C1R( ref_Y_data, pitch_l, cur_Y_data, pitch_l, roi );
    roi.height >>= 1;
    roi.width  >>= 1;
    ippiCopy_8u_C1R(ref_U_data, pitch_c, cur_U_data, pitch_c, roi);
    ippiCopy_8u_C1R(ref_V_data, pitch_c, cur_V_data, pitch_c, roi);
    return UMC_OK;
  }

  if(PictureHeader.picture_structure == FRAME_PICTURE) {
    CALC_OFFSETS_FRAME_420(offs_y1, offs_uv1, flag1, flag2, vector[0], vector[1], HP_FLAG_CP)
  } else {
    CALC_OFFSETS_FIELD_420(offs_y1, offs_uv1, flag1, flag2, vector[0], vector[1], 0, HP_FLAG_CP)

    pitch_l <<= 1;
    pitch_c <<= 1;
  }

  for (i = 0; i < video->mb_address_increment - 1; i++) {
    CHECK_OFFSET_L(offs_y1, (vector[0]&1), pitch_l, 16+(vector[1]&1))
    MCZERO_FRAME(COPY, ref_Y_data + offs_y1, ref_U_data + offs_uv1, ref_V_data + offs_uv1, flag1, flag2)
    cur_Y_data += 16;
    cur_U_data += 8;
    cur_V_data += 8;
    offs_y1 += 16;
    offs_uv1 += 8;
  }

  return UMC_OK;
} //mc_mp2_420_skip

Status MPEG2VideoDecoderBase::mc_mp2_422_skip(Mpeg2VideoContext *video)
{
  Ipp32s ref_index = (video->macroblock_motion_backward) ? frame_buffer.next_index : frame_buffer.prev_index;
  Ipp8u *ref_Y_data = frame_buffer.frame_p_c_n[ref_index].Y_comp_data;
  Ipp8u *ref_U_data = frame_buffer.frame_p_c_n[ref_index].U_comp_data;
  Ipp8u *ref_V_data = frame_buffer.frame_p_c_n[ref_index].V_comp_data;
  Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
  Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
  Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  Ipp32u pitch_l  = frame_buffer.Y_comp_pitch;
  Ipp32u pitch_c = frame_buffer.U_comp_pitch;
  Ipp32u flag1, flag2;
  Ipp32s offs_uv1, offs_y1;
  Ipp16s *vector = video->vector;
  IppiSize roi = {16*(video->mb_address_increment - 1), 16};
  Ipp32s i;

  cur_Y_data += video->offset_l;
  cur_U_data += video->offset_c;
  cur_V_data += video->offset_c;

  if (video->macroblock_motion_backward) vector += 2;

  if (PictureHeader.picture_coding_type == MPEG2_P_PICTURE) {
    ref_Y_data += video->offset_l;
    ref_U_data += video->offset_c;
    ref_V_data += video->offset_c;
    if(PictureHeader.picture_structure != FRAME_PICTURE) {
      if(PictureHeader.picture_structure == BOTTOM_FIELD) {
        cur_Y_data += pitch_l;
        cur_U_data += pitch_c;
        cur_V_data += pitch_c;
      }
      pitch_l <<= 1;
      pitch_c <<= 1;
    }
    ippiCopy_8u_C1R( ref_Y_data, pitch_l, cur_Y_data, pitch_l, roi );
    roi.width  >>= 1;
    ippiCopy_8u_C1R(ref_U_data, pitch_c, cur_U_data, pitch_c, roi);
    ippiCopy_8u_C1R(ref_V_data, pitch_c, cur_V_data, pitch_c, roi);
    return UMC_OK;
  }

  if(PictureHeader.picture_structure == FRAME_PICTURE) {
    CALC_OFFSETS_FRAME_422(offs_y1, offs_uv1, flag1, flag2, vector[0], vector[1], HP_FLAG_CP)
  } else {
    CALC_OFFSETS_FIELD_422(offs_y1, offs_uv1, flag1, flag2, vector[0], vector[1], 0, HP_FLAG_CP)

    pitch_l <<= 1;
    pitch_c <<= 1;
  }

  for (i = 0; i < video->mb_address_increment - 1; i++) {
    CHECK_OFFSET_L(offs_y1, (vector[0]&1), pitch_l, 16+(vector[1]&1))
    MCZERO_FRAME_422(COPY, ref_Y_data + offs_y1, ref_U_data + offs_uv1, ref_V_data + offs_uv1, flag1, flag2)
    cur_Y_data += 16;
    cur_U_data += 8;
    cur_V_data += 8;
    offs_y1 += 16;
    offs_uv1 += 8;
  }

  return UMC_OK;
} //mc_mp2_422_skip

Status MPEG2VideoDecoderBase::mc_mp2_420b_skip(Mpeg2VideoContext *video)
{
  Ipp8u *ref_Y_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  Ipp8u *ref_U_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  Ipp8u *ref_V_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;
  Ipp8u *ref_Y_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].Y_comp_data;
  Ipp8u *ref_U_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].U_comp_data;
  Ipp8u *ref_V_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].V_comp_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32u flag1, flag2, flag3, flag4;
  Ipp32s offs_uv1, offs_uv2, offs_y1, offs_y2;
  Ipp32u pitch_l, pitch_c;
  Ipp32u pitch_l2, pitch_c2;
  Ipp32s i;

  pitch_l2 = pitch_l =  frame_buffer.Y_comp_pitch;
  pitch_c2 = pitch_c =  frame_buffer.U_comp_pitch;

  cur_Y_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
  cur_U_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
  cur_V_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  cur_Y_data += video->offset_l;
  cur_U_data += video->offset_c;
  cur_V_data += video->offset_c;

  if (PictureHeader.picture_structure != FRAME_PICTURE) {
    pitch_l2 =  pitch_l + pitch_l;
    pitch_c2 =  pitch_c + pitch_c;
    CALC_OFFSETS_FIELD_420(offs_y1, offs_uv1, flag1, flag2, video->vector[0], video->vector[1], 0, HP_FLAG_AV)
    CALC_OFFSETS_FIELD_420(offs_y2, offs_uv2, flag3, flag4, video->vector[2], video->vector[3], 0, HP_FLAG_AV)
  } else {
    CALC_OFFSETS_FRAME_420(offs_y1, offs_uv1, flag1, flag2, video->vector[0], video->vector[1], HP_FLAG_AV)
    CALC_OFFSETS_FRAME_420(offs_y2, offs_uv2, flag3, flag4, video->vector[2], video->vector[3], HP_FLAG_AV)
  }

  for (i = 0; i <= video->mb_address_increment - 3; i += 2) {
    CHECK_OFFSET_L(offs_y1, 16+(video->vector[0]&1), pitch_l2, 16+(video->vector[1]&1))
    CHECK_OFFSET_L(offs_y2, 16+(video->vector[2]&1), pitch_l2, 16+(video->vector[3]&1))
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1, pitch_l2, flag1,
      ref_Y_data2 + offs_y2, pitch_l2, flag3,
      cur_Y_data, pitch_l2, 0);
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1 + 16, pitch_l2, flag1,
      ref_Y_data2 + offs_y2 + 16, pitch_l2, flag3,
      cur_Y_data + 16, pitch_l2, 0);

    FUNC_AVE_HP_B(16, 8, ref_U_data1 + offs_uv1, pitch_c2, flag2,
      ref_U_data2 + offs_uv2, pitch_c2, flag4,
      cur_U_data, pitch_c2, 0);

    FUNC_AVE_HP_B(16, 8, ref_V_data1 + offs_uv1, pitch_c2, flag2,
      ref_V_data2 + offs_uv2, pitch_c2, flag4,
      cur_V_data, pitch_c2, 0);

    cur_Y_data += 32;
    offs_y1 += 32;
    offs_y2 += 32;
    cur_U_data += 16;
    cur_V_data += 16;
    offs_uv1 += 16;
    offs_uv2 += 16;
  }
  if (i < video->mb_address_increment - 1) {
    CHECK_OFFSET_L(offs_y1, (video->vector[0]&1), pitch_l2, 16+(video->vector[1]&1))
    CHECK_OFFSET_L(offs_y2, (video->vector[2]&1), pitch_l2, 16+(video->vector[3]&1))
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1, pitch_l2, flag1,
      ref_Y_data2 + offs_y2, pitch_l2, flag3,
      cur_Y_data, pitch_l2, 0);

    FUNC_AVE_HP_B(8, 8, ref_U_data1 + offs_uv1, pitch_c2, flag2,
      ref_U_data2 + offs_uv2, pitch_c2, flag4,
      cur_U_data, pitch_c2, 0);

    FUNC_AVE_HP_B(8, 8, ref_V_data1 + offs_uv1, pitch_c2, flag2,
      ref_V_data2 + offs_uv2, pitch_c2, flag4,
      cur_V_data, pitch_c2, 0);
  }

  return UMC_OK;
} //mc_mp2_420b_skip

Status MPEG2VideoDecoderBase::mc_mp2_422b_skip(Mpeg2VideoContext *video)
{
  Ipp8u *ref_Y_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  Ipp8u *ref_U_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  Ipp8u *ref_V_data1 = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;
  Ipp8u *ref_Y_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].Y_comp_data;
  Ipp8u *ref_U_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].U_comp_data;
  Ipp8u *ref_V_data2 = frame_buffer.frame_p_c_n[frame_buffer.next_index].V_comp_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32u flag1, flag2, flag3, flag4;
  Ipp32s offs_uv1, offs_uv2, offs_y1, offs_y2;
  Ipp32u pitch_l, pitch_c;
  Ipp32u pitch_l2, pitch_c2;
  Ipp32s i;

  pitch_l2 = pitch_l =  frame_buffer.Y_comp_pitch;
  pitch_c2 = pitch_c =  frame_buffer.U_comp_pitch;

  cur_Y_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
  cur_U_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
  cur_V_data = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  cur_Y_data += video->offset_l;
  cur_U_data += video->offset_c;
  cur_V_data += video->offset_c;

  if (PictureHeader.picture_structure != FRAME_PICTURE) {
    pitch_l2 =  pitch_l + pitch_l;
    pitch_c2 =  pitch_c + pitch_c;
    CALC_OFFSETS_FIELD_422(offs_y1, offs_uv1, flag1, flag2, video->vector[0], video->vector[1], 0, HP_FLAG_AV)
    CALC_OFFSETS_FIELD_422(offs_y2, offs_uv2, flag3, flag4, video->vector[2], video->vector[3], 0, HP_FLAG_AV)
  } else {
    CALC_OFFSETS_FRAME_422(offs_y1, offs_uv1, flag1, flag2, video->vector[0], video->vector[1], HP_FLAG_AV)
    CALC_OFFSETS_FRAME_422(offs_y2, offs_uv2, flag3, flag4, video->vector[2], video->vector[3], HP_FLAG_AV)
  }

  for (i = 0; i <= video->mb_address_increment - 3; i += 2) {
    CHECK_OFFSET_L(offs_y1, (video->vector[0]&1), pitch_l2, 16+(video->vector[1]&1))
    CHECK_OFFSET_L(offs_y2, (video->vector[2]&1), pitch_l2, 16+(video->vector[3]&1))
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1, pitch_l2, flag1,
      ref_Y_data2 + offs_y2, pitch_l2, flag3,
      cur_Y_data, pitch_l2, 0);
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1 + 16, pitch_l2, flag1,
      ref_Y_data2 + offs_y2 + 16, pitch_l2, flag3,
      cur_Y_data + 16, pitch_l2, 0);

    FUNC_AVE_HP_B(16, 16, ref_U_data1 + offs_uv1, pitch_c2, flag2,
      ref_U_data2 + offs_uv2, pitch_c2, flag4,
      cur_U_data, pitch_c2, 0);

    FUNC_AVE_HP_B(16, 16, ref_V_data1 + offs_uv1, pitch_c2, flag2,
      ref_V_data2 + offs_uv2, pitch_c2, flag4,
      cur_V_data, pitch_c2, 0);

    cur_Y_data += 32;
    offs_y1 += 32;
    offs_y2 += 32;
    cur_U_data += 16;
    cur_V_data += 16;
    offs_uv1 += 16;
    offs_uv2 += 16;
  }
  if (i < video->mb_address_increment - 1) {
    CHECK_OFFSET_L(offs_y1, (video->vector[0]&1), pitch_l2, 16+(video->vector[1]&1))
    CHECK_OFFSET_L(offs_y2, (video->vector[2]&1), pitch_l2, 16+(video->vector[3]&1))
    FUNC_AVE_HP_B(16, 16, ref_Y_data1 + offs_y1, pitch_l2, flag1,
      ref_Y_data2 + offs_y2, pitch_l2, flag3,
      cur_Y_data, pitch_l2, 0);

    FUNC_AVE_HP_B(8, 16, ref_U_data1 + offs_uv1, pitch_c2, flag2,
      ref_U_data2 + offs_uv2, pitch_c2, flag4,
      cur_U_data, pitch_c2, 0);

    FUNC_AVE_HP_B(8, 16, ref_V_data1 + offs_uv1, pitch_c2, flag2,
      ref_V_data2 + offs_uv2, pitch_c2, flag4,
      cur_V_data, pitch_c2, 0);
  }

  return UMC_OK;
} //mc_mp2_422b_skip

Status MPEG2VideoDecoderBase::mc_dualprime_frame_420(Mpeg2VideoContext *video)
{
  Ipp32u pitch_l, pitch_c;
  Ipp8u *ref_Y_data, *ref_U_data, *ref_V_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32s nMCType,  nMCTypeUV;
  Ipp32s nMCType0, nMCTypeUV0;
  Ipp32s nMCType1, nMCTypeUV1;
  Ipp32s off_l, off_c;
  Ipp32s off0_l, off0_c;
  Ipp32s off1_l, off1_c;

  mv_decode_dp(video);

  pitch_l   = frame_buffer.Y_comp_pitch;
  pitch_c   = frame_buffer.U_comp_pitch;

  ref_Y_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  ref_U_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  ref_V_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;

  // create first vector for block
  CALC_OFFSETS_FIELD_420(off_l,  off_c,  nMCType,  nMCTypeUV,  video->vector[0], video->vector[1], 0, HP_FLAG_AV)
  // create second vector for upper halfblock
  CALC_OFFSETS_FIELD_420(off0_l, off0_c, nMCType0, nMCTypeUV0, video->vector[4], video->vector[5], 0, HP_FLAG_AV)
  // create second vector for lower halfblock
  CALC_OFFSETS_FIELD_420(off1_l, off1_c, nMCType1, nMCTypeUV1, video->vector[6], video->vector[7], 0, HP_FLAG_AV)
  CHECK_OFFSET_L(off_l+pitch_l, (video->vector[0]&1), pitch_l*2, 8+(video->vector[1]&1))
  CHECK_OFFSET_L(off0_l+pitch_l, (video->vector[4]&1), pitch_l*2, 8+(video->vector[5]&1))
  CHECK_OFFSET_L(off1_l, (video->vector[6]&1), pitch_l*2, 8+(video->vector[7]&1)) 

  FUNC_AVE_HP_B(16, 8, ref_Y_data+off_l, pitch_l * 2, nMCType,
    ref_Y_data+off0_l + pitch_l, pitch_l * 2, nMCType0,
    cur_Y_data, pitch_l * 2,
    0);
  FUNC_AVE_HP_B(16, 8, ref_Y_data+off_l+pitch_l, pitch_l * 2, nMCType,
    ref_Y_data+off1_l, pitch_l * 2, nMCType1,
    cur_Y_data + pitch_l, pitch_l * 2,
    0);

  FUNC_AVE_HP_B(8, 4, ref_U_data+off_c, pitch_c * 2, nMCTypeUV,
    ref_U_data+off0_c + pitch_c, pitch_c * 2, nMCTypeUV0,
    cur_U_data, pitch_c * 2,
    0);
  FUNC_AVE_HP_B(8, 4, ref_U_data+off_c + pitch_c, pitch_c * 2, nMCTypeUV,
    ref_U_data+off1_c, pitch_c * 2, nMCTypeUV1,
    cur_U_data + pitch_c, pitch_c * 2,
    0);

  FUNC_AVE_HP_B(8, 4, ref_V_data+off_c, pitch_c * 2, nMCTypeUV,
    ref_V_data+off0_c + pitch_c, pitch_c * 2, nMCTypeUV0,
    cur_V_data, pitch_c * 2,
    0);
  FUNC_AVE_HP_B(8, 4, ref_V_data+off_c + pitch_c, pitch_c * 2, nMCTypeUV,
    ref_V_data+off1_c, pitch_c * 2, nMCTypeUV1,
    cur_V_data + pitch_c, pitch_c * 2,
    0);

  return UMC_OK;
} //mc_dualprime_frame

Status MPEG2VideoDecoderBase::mc_dualprime_field_420(Mpeg2VideoContext *video)
{
  Ipp32u pitch_l, pitch_c;
  Ipp8u *ref_Y_data, *ref_U_data, *ref_V_data;
  Ipp8u *prev_Y_data, *prev_U_data, *prev_V_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32s nMCType,  nMCTypeUV, nMCType2, nMCTypeUV2;
  Ipp32s off_l, off_c, off2_l, off2_c;
  Ipp32s off_fld, off_fldUV;

  mv_decode_dp(video);

  pitch_l   = frame_buffer.Y_comp_pitch;
  pitch_c   = frame_buffer.U_comp_pitch;

  off_fld = 0;
  off_fldUV = 0;
  if(PictureHeader.picture_structure == BOTTOM_FIELD) {
    off_fld = pitch_l;
    off_fldUV = pitch_c;
  }

  prev_Y_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  prev_U_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  prev_V_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;

  // for opposite parity
  if(frame_buffer.field_buffer_index) { // second field
    ref_Y_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
    ref_U_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
    ref_V_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  } else {
    ref_Y_data = prev_Y_data;
    ref_U_data = prev_U_data;
    ref_V_data = prev_V_data;
  }

  // create first vector for block (same parity)
  CALC_OFFSETS_FIELD_420(off_l,  off_c,  nMCType,  nMCTypeUV,  video->vector[0], video->vector[1], 0, HP_FLAG_AV)
  // create second vector for block (opposite parity)
  CALC_OFFSETS_FIELD_420(off2_l, off2_c, nMCType2, nMCTypeUV2, video->vector[4], video->vector[5], 0, HP_FLAG_AV)
  CHECK_OFFSET_L(off_l, (video->vector[0]&1), pitch_l*2, 16+(video->vector[1]&1))
  CHECK_OFFSET_L(off2_l+pitch_l -2*off_fld, (video->vector[4]&1), pitch_l*2, 16+(video->vector[5]&1))

  FUNC_AVE_HP_B(16, 16, prev_Y_data + off_l, pitch_l * 2, nMCType,
    ref_Y_data + off2_l + pitch_l - 2 * off_fld, pitch_l * 2, nMCType2,
    cur_Y_data, pitch_l * 2,
    0);

  FUNC_AVE_HP_B(8, 8, prev_U_data + off_c, pitch_c * 2, nMCTypeUV,
    ref_U_data + off2_c + pitch_c - 2 * off_fldUV, pitch_c * 2, nMCTypeUV2,
    cur_U_data, pitch_c * 2,
    0);

  FUNC_AVE_HP_B(8, 8, prev_V_data + off_c, pitch_c * 2, nMCTypeUV,
    ref_V_data + off2_c + pitch_c - 2 * off_fldUV, pitch_c * 2, nMCTypeUV2,
    cur_V_data, pitch_c * 2,
    0);

  return UMC_OK;
} //mc_dualprime_field_420

Status MPEG2VideoDecoderBase::mc_dualprime_frame_422(Mpeg2VideoContext *video)
{
  Ipp32u pitch_l, pitch_c;
  Ipp8u *ref_Y_data, *ref_U_data, *ref_V_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32s nMCType,  nMCTypeUV;
  Ipp32s nMCType0, nMCTypeUV0;
  Ipp32s nMCType1, nMCTypeUV1;
  Ipp32s off_l, off_c;
  Ipp32s off0_l, off0_c;
  Ipp32s off1_l, off1_c;

  mv_decode_dp(video);

  pitch_l   = frame_buffer.Y_comp_pitch;
  pitch_c   = frame_buffer.U_comp_pitch;

  ref_Y_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  ref_U_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  ref_V_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;

  // create first vector for block
  CALC_OFFSETS_FIELD_422(off_l,  off_c,  nMCType,  nMCTypeUV,  video->vector[0], video->vector[1], 0, HP_FLAG_AV)
  // create second vector for upper halfblock
  CALC_OFFSETS_FIELD_422(off0_l, off0_c, nMCType0, nMCTypeUV0, video->vector[4], video->vector[5], 0, HP_FLAG_AV)
  // create second vector for lower halfblock
  CALC_OFFSETS_FIELD_422(off1_l, off1_c, nMCType1, nMCTypeUV1, video->vector[6], video->vector[7], 0, HP_FLAG_AV)
  CHECK_OFFSET_L(off_l, pitch_l+(video->vector[0]&1), pitch_l*2, 8+(video->vector[1]&1))
  CHECK_OFFSET_L(off0_l+pitch_l, (video->vector[4]&1), pitch_l*2, 8+(video->vector[5]&1))
  CHECK_OFFSET_L(off1_l, (video->vector[6]&1), pitch_l*2, 8+(video->vector[7]&1))

  FUNC_AVE_HP_B(16, 8, ref_Y_data+off_l, pitch_l * 2, nMCType,
    ref_Y_data+off0_l + pitch_l, pitch_l * 2, nMCType0,
    cur_Y_data, pitch_l * 2,
    0);
  FUNC_AVE_HP_B(16, 8, ref_Y_data+off_l+pitch_l, pitch_l * 2, nMCType,
    ref_Y_data+off1_l, pitch_l * 2, nMCType1,
    cur_Y_data + pitch_l, pitch_l * 2,
    0);

  FUNC_AVE_HP_B(8, 8, ref_U_data+off_c, pitch_c * 2, nMCTypeUV,
    ref_U_data+off0_c + pitch_c, pitch_c * 2, nMCTypeUV0,
    cur_U_data, pitch_c * 2,
    0);
  FUNC_AVE_HP_B(8, 8, ref_U_data+off_c + pitch_c, pitch_c * 2, nMCTypeUV,
    ref_U_data+off1_c, pitch_c * 2, nMCTypeUV1,
    cur_U_data + pitch_c, pitch_c * 2,
    0);

  FUNC_AVE_HP_B(8, 8, ref_V_data+off_c, pitch_c * 2, nMCTypeUV,
    ref_V_data+off0_c + pitch_c, pitch_c * 2, nMCTypeUV0,
    cur_V_data, pitch_c * 2,
    0);
  FUNC_AVE_HP_B(8, 8, ref_V_data+off_c + pitch_c, pitch_c * 2, nMCTypeUV,
    ref_V_data+off1_c, pitch_c * 2, nMCTypeUV1,
    cur_V_data + pitch_c, pitch_c * 2,
    0);

  return UMC_OK;
} //mc_dualprime_frame_422

Status MPEG2VideoDecoderBase::mc_dualprime_field_422(Mpeg2VideoContext *video)
{
  Ipp32u pitch_l, pitch_c;
  Ipp8u *ref_Y_data, *ref_U_data, *ref_V_data;
  Ipp8u *prev_Y_data, *prev_U_data, *prev_V_data;
  Ipp8u *cur_Y_data = video->blkCurrYUV[0];
  Ipp8u *cur_U_data = video->blkCurrYUV[1];
  Ipp8u *cur_V_data = video->blkCurrYUV[2];
  Ipp32s nMCType,  nMCTypeUV, nMCType2, nMCTypeUV2;
  Ipp32s off_l, off_c, off2_l, off2_c;
  Ipp32s off_fld, off_fldUV;

  mv_decode_dp(video);

  pitch_l   = frame_buffer.Y_comp_pitch;
  pitch_c   = frame_buffer.U_comp_pitch;

  off_fld = 0;
  off_fldUV = 0;
  if(PictureHeader.picture_structure == BOTTOM_FIELD) {
    off_fld = pitch_l;
    off_fldUV = pitch_c;
  }

  prev_Y_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].Y_comp_data;
  prev_U_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].U_comp_data;
  prev_V_data = frame_buffer.frame_p_c_n[frame_buffer.prev_index].V_comp_data;

  // for opposite parity
  if(frame_buffer.field_buffer_index) { // second field
    ref_Y_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].Y_comp_data;
    ref_U_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].U_comp_data;
    ref_V_data  = frame_buffer.frame_p_c_n[frame_buffer.curr_index].V_comp_data;
  } else {
    ref_Y_data = prev_Y_data;
    ref_U_data = prev_U_data;
    ref_V_data = prev_V_data;
  }

  // create first vector for block (same parity)
  CALC_OFFSETS_FIELD_422(off_l,  off_c,  nMCType,  nMCTypeUV,  video->vector[0], video->vector[1], 0, HP_FLAG_AV)
  // create second vector for block (opposite parity)
  CALC_OFFSETS_FIELD_422(off2_l, off2_c, nMCType2, nMCTypeUV2, video->vector[4], video->vector[5], 0, HP_FLAG_AV)
  CHECK_OFFSET_L(off_l, (video->vector[0]&1), pitch_l*2, 16+(video->vector[1]&1))
  CHECK_OFFSET_L(off2_l+pitch_l -2*off_fld, (video->vector[4]&1), pitch_l*2, 16+(video->vector[5]&1))

  FUNC_AVE_HP_B(16, 16, prev_Y_data + off_l, pitch_l * 2, nMCType,
    ref_Y_data + off2_l + pitch_l - 2 * off_fld, pitch_l * 2, nMCType2,
    cur_Y_data, pitch_l * 2,
    0);

  FUNC_AVE_HP_B(8, 16, prev_U_data + off_c, pitch_c * 2, nMCTypeUV,
    ref_U_data + off2_c + pitch_c - 2 * off_fldUV, pitch_c * 2, nMCTypeUV2,
    cur_U_data, pitch_c * 2,
    0);

  FUNC_AVE_HP_B(8, 16, prev_V_data + off_c, pitch_c * 2, nMCTypeUV,
    ref_V_data + off2_c + pitch_c - 2 * off_fldUV, pitch_c * 2, nMCTypeUV2,
    cur_V_data, pitch_c * 2,
    0);

  return UMC_OK;
} //mc_dualprime_field_422

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
