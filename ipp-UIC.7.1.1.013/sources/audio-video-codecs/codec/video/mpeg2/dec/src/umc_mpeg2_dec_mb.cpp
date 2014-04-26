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

Status MPEG2VideoDecoderBase::DecodeSlice_FrameI_420(Mpeg2VideoContext *video)
{
  Ipp32s dct_type = 0;
  Ipp32s pitch_l = frame_buffer.Y_comp_pitch;
  Ipp32s pitch_c = frame_buffer.U_comp_pitch;
  Ipp32s macroblock_type, macroblock_address_increment;
  Ipp32s load_dct_type;

  video->dct_dc_past[0] =
  video->dct_dc_past[1] =
  video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

  video->mb_row = video->slice_vertical_position - 1;
  video->mb_col = -1;

  if (PictureHeader.picture_structure == FRAME_PICTURE) {
    dct_type = 0;
    load_dct_type = !PictureHeader.frame_pred_frame_dct;
    video->offset_l = (video->mb_col << 4) + pitch_l * (video->mb_row << 4);
    video->offset_c = (video->mb_col << 3) + pitch_c * (video->mb_row << ROW_CHROMA_SHIFT_420);
  } else {
    dct_type = 2;
    load_dct_type = 0;
    video->offset_l = (video->mb_col << 4) + 2 * pitch_l * (video->mb_row << 4);
    video->offset_c = (video->mb_col << 3) + 2 * pitch_c * (video->mb_row << ROW_CHROMA_SHIFT_420);
    if (PictureHeader.picture_structure == BOTTOM_FIELD) {
      video->offset_l += pitch_l;
      video->offset_c += pitch_c;
    }
  }

  for (;;) {
    int remained = GET_REMAINED_BYTES(video->bs);
    if (remained == 0) {
      return UMC_OK;
    } else if (remained < 0) {
      return UMC_ERR_INVALID_STREAM;
    }

    video->mb_col++;
    video->offset_l += 16;
    video->offset_c += 8;
    if (IS_NEXTBIT1(video->bs)) {
      SKIP_BITS(video->bs, 1)
    } else {
      DECODE_MB_INCREMENT(video->bs, macroblock_address_increment);
      video->mb_col += macroblock_address_increment;
      video->offset_l += macroblock_address_increment << 4;
      video->offset_c += macroblock_address_increment << 3;
    }
    if (video->mb_col >= sequenceHeader.mb_width) {
      return UMC_ERR_INVALID_STREAM;
    }

    DECODE_VLC(macroblock_type, video->bs, vlcMBType[0]);

    if (load_dct_type) {
      GET_1BIT(video->bs, dct_type);
    }

    if (macroblock_type & IPPVC_MB_QUANT)
    {
      DECODE_QUANTIZER_SCALE(video->bs, video->cur_q_scale);
    }

    if (PictureHeader.concealment_motion_vectors)
    {
      if (PictureHeader.picture_structure != FRAME_PICTURE) {
        SKIP_BITS(video->bs, 1);
      }
      mv_decode(0, 0, video);
      SKIP_BITS(video->bs, 1);
    }

    RECONSTRUCT_INTRA_MB_420(video->bs, dct_type);
  }
}//DecodeSlice_FrameI_420

Status MPEG2VideoDecoderBase::DecodeSlice_FramePB_420(Mpeg2VideoContext *video)
{
  Ipp32s dct_type = 0;
  Ipp32s macroblock_type;
  Ipp32s macroblock_address_increment;

  video->prediction_type = IPPVC_MC_FRAME;

  video->dct_dc_past[0] =
  video->dct_dc_past[1] =
  video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

  video->mb_row = video->slice_vertical_position - 1;
  video->mb_col = -1;
  video->offset_l = (video->mb_col << 4) + frame_buffer.Y_comp_pitch * (video->mb_row << 4);
  video->offset_c = (video->mb_col << 3) + frame_buffer.U_comp_pitch * (video->mb_row << ROW_CHROMA_SHIFT_420);

  for (;;) {
    int remained = GET_REMAINED_BYTES(video->bs);
    if (remained == 0) {
      return UMC_OK;
    } else if (remained < 0) {
      return UMC_ERR_INVALID_STREAM;
    }

    video->mb_col++;
    video->offset_l += 16;
    video->offset_c += 8;
    if (IS_NEXTBIT1(video->bs)) { // increment=1
      SKIP_BITS(video->bs, 1)
    } else {
      //COPY_BITSTREAM(_video->bs, video->bs)
      DECODE_MB_INCREMENT(video->bs, macroblock_address_increment);

      video->dct_dc_past[0] =
      video->dct_dc_past[1] =
      video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

      // skipped macroblocks
      if (video->mb_col > 0)
      {
        Ipp32s pitch_l = frame_buffer.Y_comp_pitch;
        Ipp32s pitch_c = frame_buffer.U_comp_pitch;
        Ipp32s offset_l = video->offset_l;
        Ipp32s offset_c = video->offset_c;
        Ipp32s id_his_old, id_his_new;
        Ipp32s prev_index = frame_buffer.prev_index;

        if (PictureHeader.picture_coding_type == MPEG2_P_PICTURE) {
          RESET_PMV(video->PMV)
          id_his_old = id_his_new = 1;
        } else {
          id_his_new = 0;
          video->prediction_type = IPPVC_MC_FRAME;
          COPY_PMV(video->vector, video->PMV);
          if (!video->macroblock_motion_backward) {
            if (!video->PMV[0] && !video->PMV[1]) {
              id_his_new = 2;
            }
          } else if (!video->macroblock_motion_forward) {
            if (!video->PMV[2] && !video->PMV[3]) {
              id_his_new = 3;
              prev_index = frame_buffer.next_index;
            }
          }
          id_his_old = id_his_new;
          if (sequenceHeader.b_curr_number == 1) id_his_old = -1; // first B frame
        }
        if (id_his_new) {
          Ipp32s curr_index = frame_buffer.curr_index;
          Ipp8u *ref_Y_data = frame_buffer.frame_p_c_n[prev_index].Y_comp_data;
          Ipp8u *ref_U_data = frame_buffer.frame_p_c_n[prev_index].U_comp_data;
          Ipp8u *ref_V_data = frame_buffer.frame_p_c_n[prev_index].V_comp_data;
          Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[curr_index].Y_comp_data;
          Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[curr_index].U_comp_data;
          Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[curr_index].V_comp_data;
#ifdef KEEP_HISTORY
          Ipp8u *ref_history = frame_buffer.frame_p_c_n[frame_buffer.ind_his_ref].frame_history;
          Ipp8u *curr_history = frame_buffer.frame_p_c_n[frame_buffer.ind_his_curr].frame_history;
          Ipp32s offset_his = video->mb_row*sequenceHeader.mb_width + video->mb_col;
          Ipp32s i;
          for (i = 0; i < macroblock_address_increment; i++) {
            if (ref_history[offset_his] != id_his_old)
            {
              ippiCopy16x16_8u_C1R(ref_Y_data + offset_l, pitch_l, cur_Y_data + offset_l, pitch_l);
              COPY_CHROMA_MB_420(ref_U_data + offset_c, pitch_c, cur_U_data + offset_c, pitch_c);
              COPY_CHROMA_MB_420(ref_V_data + offset_c, pitch_c, cur_V_data + offset_c, pitch_c);
            }
            curr_history[offset_his] = id_his_new;
            offset_l += 16;
            offset_c += 8;
            offset_his++;
          }
#else
          IppiSize roi = {16*macroblock_address_increment, 16};
          ippiCopy_8u_C1R(ref_Y_data + offset_l, pitch_l, cur_Y_data + offset_l, pitch_l, roi);
          roi.height = 1 << ROW_CHROMA_SHIFT_420;
          roi.width >>= 1;
          ippiCopy_8u_C1R(ref_U_data + offset_c, pitch_c, cur_U_data + offset_c, pitch_c, roi);
          ippiCopy_8u_C1R(ref_V_data + offset_c, pitch_c, cur_V_data + offset_c, pitch_c, roi);
#endif
        } else {
          video->mb_address_increment = macroblock_address_increment + 1;
          if (video->macroblock_motion_forward && video->macroblock_motion_backward) {
            mc_mp2_420b_skip(video);
          } else {
            mc_mp2_420_skip(video);
          }
        }
      } // skipped macroblocks
      video->mb_col += macroblock_address_increment;
      video->offset_l += macroblock_address_increment << 4;
      video->offset_c += macroblock_address_increment << 3;
    }
    if (video->mb_col >= sequenceHeader.mb_width) {
      return UMC_ERR_INVALID_STREAM;
    }

    DECODE_VLC(macroblock_type, video->bs, vlcMBType[PictureHeader.picture_coding_type - 1]);

    video->macroblock_motion_forward = macroblock_type & IPPVC_MB_FORWARD;
    video->macroblock_motion_backward= macroblock_type & IPPVC_MB_BACKWARD;

    if(macroblock_type & IPPVC_MB_INTRA)
    {
      if(!PictureHeader.frame_pred_frame_dct) {
        GET_1BIT(video->bs, dct_type);
      }

      if(macroblock_type & IPPVC_MB_QUANT)
      {
        DECODE_QUANTIZER_SCALE(video->bs, video->cur_q_scale);
      }

      if(!PictureHeader.concealment_motion_vectors)
      {
        RESET_PMV(video->PMV)
      }
      else
      {
        Ipp32s code;

        video->prediction_type = IPPVC_MC_FRAME;

        mv_decode(0, 0, video);
        video->PMV[4] = video->PMV[0];
        video->PMV[5] = video->PMV[1];

        GET_1BIT(video->bs, code);
      }

      RECONSTRUCT_INTRA_MB_420(video->bs, dct_type);
      continue;
    }//intra

    video->dct_dc_past[0] =
    video->dct_dc_past[1] =
    video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

    if(!PictureHeader.frame_pred_frame_dct) {
      if (video->macroblock_motion_forward || video->macroblock_motion_backward) {
        GET_TO9BITS(video->bs, 2, video->prediction_type);
      }
      if(macroblock_type & IPPVC_MB_PATTERN) {
        GET_1BIT(video->bs, dct_type);
      }
    }

    if(macroblock_type & IPPVC_MB_QUANT) {
      DECODE_QUANTIZER_SCALE(video->bs, video->cur_q_scale);
    }

    Ipp32s curr_index = frame_buffer.curr_index;
    Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[curr_index].Y_comp_data;
    Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[curr_index].U_comp_data;
    Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[curr_index].V_comp_data;
    video->blkCurrYUV[0] = cur_Y_data + video->offset_l;
    video->blkCurrYUV[1] = cur_U_data + video->offset_c;
    video->blkCurrYUV[2] = cur_V_data + video->offset_c;

    if (video->macroblock_motion_forward) {
      if (video->prediction_type == IPPVC_MC_DP) {
        mc_dualprime_frame_420(video);
      } else {
        mc_frame_forward_420(video);
        if (video->macroblock_motion_backward) {
          mc_frame_backward_add_420(video);
        }
      }
    } else {
      if (video->macroblock_motion_backward) {
        mc_frame_backward_420(video);
      } else {
        RESET_PMV(video->PMV)
        mc_frame_forward0_420(video);
      }
    }

    if (macroblock_type & IPPVC_MB_PATTERN) {
      RECONSTRUCT_INTER_MB_420(video->bs, dct_type);
    }
  }

  //return UMC_OK;
}//DecodeSlice_FramePB_420

Status MPEG2VideoDecoderBase::DecodeSlice_FieldPB_420(Mpeg2VideoContext *video)
{
  Ipp32s macroblock_type;
  Ipp32s macroblock_address_increment;
  Ipp32s pitch_l = frame_buffer.Y_comp_pitch;
  Ipp32s pitch_c = frame_buffer.U_comp_pitch;

  video->prediction_type = IPPVC_MC_FIELD;

  video->dct_dc_past[0] =
  video->dct_dc_past[1] =
  video->dct_dc_past[2] =
    PictureHeader.curr_reset_dc;

  video->mb_row = video->slice_vertical_position - 1;
  video->mb_col = -1;

  video->row_l  = video->mb_row << 4;
  video->col_l  = video->mb_col << 4;

  video->row_c  = video->mb_row << ROW_CHROMA_SHIFT_420;
  video->col_c  = video->mb_col << 3;

  video->offset_l = 2 * video->row_l * pitch_l + video->col_l;
  video->offset_c = 2 * video->row_c * pitch_c + video->col_c;

  //odd_pitch = pitch for bottom field (odd strings)
  if(PictureHeader.picture_structure == BOTTOM_FIELD)
  {
    video->offset_l += pitch_l;
    video->offset_c += pitch_c;
  }

  for (;;) {
    int remained = GET_REMAINED_BYTES(video->bs);
    if (remained == 0) {
      return UMC_OK;
    } else if (remained < 0) {
      return UMC_ERR_INVALID_STREAM;
    }

    video->mb_col++;
    video->offset_l += 16;
    video->offset_c += 8;
    video->col_l += 16;
    video->col_c += 8;

    if (IS_NEXTBIT1(video->bs)) { // increment=1
      SKIP_BITS(video->bs, 1)
    } else {
      DECODE_MB_INCREMENT(video->bs, macroblock_address_increment);

      video->dct_dc_past[0] =
      video->dct_dc_past[1] =
      video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

      // skipped macroblocks
      if (video->mb_col > 0)
      {
        Ipp32s pitch_l = frame_buffer.Y_comp_pitch * 2;
        Ipp32s pitch_c = frame_buffer.U_comp_pitch * 2;
        Ipp32s offset_l = video->offset_l;
        Ipp32s offset_c = video->offset_c;
        Ipp32s id_his_old, id_his_new;
        Ipp32s prev_index = frame_buffer.prev_index;

        if (PictureHeader.picture_coding_type == MPEG2_P_PICTURE) {
          RESET_PMV(video->PMV)
          id_his_old = id_his_new = 1;
        } else {
          id_his_new = 0;
          video->prediction_type = IPPVC_MC_FIELD;
          COPY_PMV(video->vector, video->PMV);
          if (!video->macroblock_motion_backward) {
            if (!video->PMV[0] && !video->PMV[1]) {
              id_his_new = 4;
            }
          } else if (!video->macroblock_motion_forward) {
            if (!video->PMV[2] && !video->PMV[3]) {
              id_his_new = 5;
              prev_index = frame_buffer.next_index;
            }
          }
          id_his_old = id_his_new;
          if (sequenceHeader.b_curr_number == 1) id_his_old = -1; // first B frame
        }

        if (id_his_new) {
          Ipp32s curr_index = frame_buffer.curr_index;
          Ipp8u *ref_Y_data = frame_buffer.frame_p_c_n[prev_index].Y_comp_data;
          Ipp8u *ref_U_data = frame_buffer.frame_p_c_n[prev_index].U_comp_data;
          Ipp8u *ref_V_data = frame_buffer.frame_p_c_n[prev_index].V_comp_data;
          Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[curr_index].Y_comp_data;
          Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[curr_index].U_comp_data;
          Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[curr_index].V_comp_data;
#ifdef KEEP_HISTORY
          Ipp8u *ref_history = frame_buffer.frame_p_c_n[frame_buffer.ind_his_ref].frame_history;
          Ipp8u *curr_history = frame_buffer.frame_p_c_n[frame_buffer.ind_his_curr].frame_history;
          Ipp32s offset_his = video->mb_row*sequenceHeader.mb_width + video->mb_col;
          Ipp32s i;
          if (PictureHeader.picture_structure == BOTTOM_FIELD) {
            offset_his += (sequenceHeader.numMB >> 1);
          }
          for (i = 0; i < macroblock_address_increment; i++) {
            if (ref_history[offset_his] != id_his_old)
            {
              ippiCopy16x16_8u_C1R(ref_Y_data + offset_l, pitch_l, cur_Y_data + offset_l, pitch_l);
              COPY_CHROMA_MB_420(ref_U_data + offset_c, pitch_c, cur_U_data + offset_c, pitch_c);
              COPY_CHROMA_MB_420(ref_V_data + offset_c, pitch_c, cur_V_data + offset_c, pitch_c);
            }
            curr_history[offset_his] = id_his_new;
            offset_l += 16;
            offset_c += 8;
            offset_his++;
          }
#else
          IppiSize roi = {16*macroblock_address_increment, 16};
          ippiCopy_8u_C1R(ref_Y_data + offset_l, pitch_l, cur_Y_data + offset_l, pitch_l, roi);
          roi.height = 1 << ROW_CHROMA_SHIFT_420;
          roi.width >>= 1;
          ippiCopy_8u_C1R(ref_U_data + offset_c, pitch_c, cur_U_data + offset_c, pitch_c, roi);
          ippiCopy_8u_C1R(ref_V_data + offset_c, pitch_c, cur_V_data + offset_c, pitch_c, roi);
#endif
        } else {
          video->mb_address_increment = macroblock_address_increment + 1;
          if (video->macroblock_motion_forward && video->macroblock_motion_backward) {
            mc_mp2_420b_skip(video);
          } else {
            mc_mp2_420_skip(video);
          }
        }
      } // skipped macroblocks
      video->mb_col += macroblock_address_increment;
      video->col_l = video->mb_col << 4;
      video->col_c = video->mb_col << 3;
      video->offset_l += macroblock_address_increment << 4;
      video->offset_c += macroblock_address_increment << 3;
    }
    if (video->mb_col >= sequenceHeader.mb_width) {
      return UMC_ERR_INVALID_STREAM;
    }

    DECODE_VLC(macroblock_type, video->bs, vlcMBType[PictureHeader.picture_coding_type - 1]);

    video->macroblock_motion_forward = macroblock_type & IPPVC_MB_FORWARD;
    video->macroblock_motion_backward= macroblock_type & IPPVC_MB_BACKWARD;

    if(macroblock_type & IPPVC_MB_INTRA)
    {
      if(macroblock_type & IPPVC_MB_QUANT)
      {
        DECODE_QUANTIZER_SCALE(video->bs, video->cur_q_scale);
      }

      if(!PictureHeader.concealment_motion_vectors)
      {
        RESET_PMV(video->PMV)
      }
      else
      {
        Ipp32s field_sel;
        Ipp32s code;

        video->prediction_type = IPPVC_MC_FIELD;
        GET_1BIT(video->bs, field_sel);

        mv_decode(0, 0, video);
        video->PMV[4] = video->PMV[0];
        video->PMV[5] = video->PMV[1];

        GET_1BIT(video->bs, code);
      }

      RECONSTRUCT_INTRA_MB_420(video->bs, 2);
      continue;
    }//intra

    video->dct_dc_past[0] =
    video->dct_dc_past[1] =
    video->dct_dc_past[2] = PictureHeader.curr_reset_dc;

    if (video->macroblock_motion_forward || video->macroblock_motion_backward) {
      GET_TO9BITS(video->bs, 2, video->prediction_type);
    }

    if(macroblock_type & IPPVC_MB_QUANT) {
      DECODE_QUANTIZER_SCALE(video->bs, video->cur_q_scale);
    }

    Ipp32s curr_index = frame_buffer.curr_index;
    Ipp8u *cur_Y_data = frame_buffer.frame_p_c_n[curr_index].Y_comp_data;
    Ipp8u *cur_U_data = frame_buffer.frame_p_c_n[curr_index].U_comp_data;
    Ipp8u *cur_V_data = frame_buffer.frame_p_c_n[curr_index].V_comp_data;
    video->blkCurrYUV[0] = cur_Y_data + video->offset_l;
    video->blkCurrYUV[1] = cur_U_data + video->offset_c;
    video->blkCurrYUV[2] = cur_V_data + video->offset_c;

    if (video->macroblock_motion_forward) {
      if (video->prediction_type == IPPVC_MC_DP) {
        mc_dualprime_field_420(video);
      } else {
        mc_field_forward_420(video);
        if (video->macroblock_motion_backward) {
          mc_field_backward_add_420(video);
        }
      }
    } else {
      if (video->macroblock_motion_backward) {
        mc_field_backward_420(video);
      } else {
        RESET_PMV(video->PMV)
        mc_field_forward0_420(video);
      }
    }

    if (macroblock_type & IPPVC_MB_PATTERN) {
      RECONSTRUCT_INTER_MB_420(video->bs, 2);
    }
  }

  //return UMC_OK;
}//DecodeSlice_FieldPB_420

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
