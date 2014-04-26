/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER

#include "umc_mpeg2_enc_defs.h"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Coding P picture ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

using namespace UMC;

#define DIST(aa,bb) ((aa)*(aa)+(bb)*(bb))
#define PDIST(p1,p2) DIST(p1.x-p2.x, p1.y-p2.y)
#define PMOD(p) DIST(p.x,p.y)

void MPEG2VideoEncoderBase::encodeP( Ipp32s numTh)
{
  const Ipp8u *pRef[2]; // parity same/opposite
  const Ipp8u *pRec[2]; // parity same/opposite
  Ipp16s      *pMBlock, *pDiff;
  Ipp32s      i, j, ic, jc, k, blk;
  Ipp32s      macroblock_address_increment;
  Ipp32s      Count[12], CodedBlockPattern;
  MpegMotionVector2 vector[3][1] = {0};
  // bounds in half pixels, border included
  Ipp32s      me_bound_left[1], me_bound_right[1];
  Ipp32s      me_bound_top[1], me_bound_bottom[1];
  Ipp32s      me_bound_2_top[1], me_bound_1_bottom[1];
  MB_prediction_info pred_info[2];
  MB_prediction_info *best = pred_info;
  MB_prediction_info *curr = pred_info + 1;
  Ipp32s      mean_frm[4], mean_fld[4];
  IppiSize    roi8x8 = {8,8};
  Ipp32s      slice_past_intra_address;
  Ipp32s      slice_macroblock_address;
  Ipp32s      dc_dct_pred[3];
  IppiSize    roi;
  Ipp32s      start_y;
  Ipp32s      start_uv;
  Ipp32s      stop_y;
  Ipp32s      skip_flag;
  Ipp32s      numIntra = 0;
  bitBuffer   bbStart;
  threadSpec[numTh].fieldCount = 0;

  CALC_START_STOP_ROWS

  best->pDiff = threadSpec[numTh].pDiff;
  curr->pDiff = threadSpec[numTh].pDiff1;

  if (!bQuantiserChanged) {
    Ipp32s mf; // *2 to support IP case
    mf = ipflag ? 1 : 2*P_distance;
    for(j=start_y; j < stop_y; j += 16)
    {
        for(i=0; i < (Ipp32s)encodeInfo.m_info.videoInfo.m_iWidth; i += 16, k++)
      {
        pMBInfo[k].MV[0][0].x = pMBInfo[k].MV_P[0].x*mf/512;
        pMBInfo[k].MV[0][0].y = pMBInfo[k].MV_P[0].y*mf/512;
        pMBInfo[k].MV[1][0].x = pMBInfo[k].MV_P[1].x*mf/512;
        pMBInfo[k].MV[1][0].y = pMBInfo[k].MV_P[1].y*mf/512;

      }
    }
    k = (threads) ? (start_y/16)*MBcountH : 0;
  }

  for(j=start_y, jc= start_uv; j < stop_y; j += 16, jc += BlkHeight_c)
  {
    PutSliceHeader( j >> 4, numTh);
    macroblock_address_increment = 1;
    slice_macroblock_address = 0;
    slice_past_intra_address = 0;

    // reset predictors at the start of slice
    dc_dct_pred[0] = dc_dct_pred[1] = dc_dct_pred[2] = ResetTbl[intra_dc_precision];

    ippsSet_8u(0, (Ipp8u*)threadSpec[numTh].PMV, sizeof(threadSpec[0].PMV));
    BOUNDS_V(0,j)
    BOUNDS_V_FIELD(0,j)

    for(i=ic=0; i < (Ipp32s)encodeInfo.m_info.videoInfo.m_iWidth; i += 16, ic += BlkWidth_c)
    {
      Ipp32s cur_offset   = i  + j  * YFrameHSize;
      Ipp32s cur_offset_c = ic + jc * UVFrameHSize;
      Ipp8u *YBlockRec = YRecFrame[curr_field][1] + cur_offset;
      Ipp8u *UBlockRec = URecFrame[curr_field][1] + cur_offset_c;
      Ipp8u *VBlockRec = VRecFrame[curr_field][1] + cur_offset_c;
      const Ipp8u *YBlock = Y_src + cur_offset;
      const Ipp8u *UBlock = U_src + cur_offset_c;
      const Ipp8u *VBlock = V_src + cur_offset_c;
//      Ipp32s size;
      Ipp32s goodpred = 0;

      pRef[0] = YRefFrame[curr_field][0] + cur_offset;   // same parity
      pRef[1] = YRefFrame[1-curr_field][0] + cur_offset; // opposite parity
      pRec[0] = YRecFrame[curr_field][0] + cur_offset;   // same parity
      pRec[1] = YRecFrame[1-curr_field][0] + cur_offset; // opposite parity
      slice_macroblock_address = i >> 4;

      memset(Count, 0, sizeof(Count));

      if (( bQuantiserChanged
          ) && pMBInfo[k].mb_type) {
        if(!bQuantiserChanged && pMBInfo[k].skipped)
          goto skip_macroblock;

        best->pred_type = pMBInfo[k].prediction_type;
        best->dct_type = pMBInfo[k].dct_type;
        best->mb_type = pMBInfo[k].mb_type;
        if (!(pMBInfo[k].mb_type & MB_INTRA)) {
          pDiff = best->pDiff;
          if (pMBInfo[k].prediction_type == MC_FRAME) {
            SET_MOTION_VECTOR((&vector[2][0]), pMBInfo[k].MV[0][0].x, pMBInfo[k].MV[0][0].y);
            GETDIFF_FRAME(Y, Y, l, pDiff, 0);
            VARMEAN_FRAME(pDiff, best->var, best->mean, best->var_sum);
          } else {
            if (picture_structure == MPS_PROGRESSIVE) {
              SET_FIELD_VECTOR((&vector[0][0]), pMBInfo[k].MV[0][0].x, pMBInfo[k].MV[0][0].y >> 1);
              SET_FIELD_VECTOR((&vector[1][0]), pMBInfo[k].MV[1][0].x, pMBInfo[k].MV[1][0].y >> 1);
            } else {
              SET_MOTION_VECTOR((&vector[0][0]), pMBInfo[k].MV[0][0].x, pMBInfo[k].MV[0][0].y);
              SET_MOTION_VECTOR((&vector[1][0]), pMBInfo[k].MV[1][0].x, pMBInfo[k].MV[1][0].y);
            }
            GETDIFF_FIELD(Y, Y, l, pDiff, 0);
            VARMEAN_FIELD(pDiff, best->var, best->mean, best->var_sum);
          }
        }
        goto encodeMB;
      }

      bbStart = threadSpec[numTh].bBuf; // save mb start position

      pMBInfo[k].mb_type = 0;
      VARMEAN_FRAME_Y(curr->var, mean_frm, curr->var_sum);
      // try to skip
      if (i != 0 && i != MBcountH*16 - 16 && !ipflag)
      {
        Ipp32s sad;
        Ipp32s var, mean;

        pDiff = threadSpec[numTh].pDiff;

        ippiGetDiff16x16_8u16s_C1(YBlock, YFrameHSize, pRef[0], YFrameHSize,
          pDiff, 32, 0, 0, 0, 0);

        skip_flag = 1;

        for (blk = 0; blk < 4; blk++) {
          ippiVarSum8x8_16s32s_C1R(pDiff + frm_diff_off[blk], 32, &var, &mean);
          if(var > varThreshold) {
            skip_flag = 0;
            break;
          }
          if(mean >= meanThreshold || mean <= -meanThreshold) {
            skip_flag = 0;
            break;
          }
        }
        if (skip_flag) {

          skip_flag = 0;
          ippiSAD8x8_8u32s_C1R(URefFrame[curr_field][0] + cur_offset_c, UVFrameHSize,
            UBlock, UVFrameHSize, &sad, 0);
          if(sad < sadThreshold) {
            ippiSAD8x8_8u32s_C1R(VRefFrame[curr_field][0] + cur_offset_c, UVFrameHSize,
              VBlock, UVFrameHSize, &sad, 0);
            if(sad < sadThreshold) {
              skip_flag = 1;
            }
          }
        }
        if (skip_flag) {
skip_macroblock:
          // skip this macroblock
          // no DCT coefficients and neither first nor last macroblock of slice and no motion
          ippsSet_8u(0, (Ipp8u*)threadSpec[numTh].PMV, sizeof(threadSpec[0].PMV)); // zero vectors
          ippsSet_8u(0, (Ipp8u*)pMBInfo[k].MV, sizeof(pMBInfo[k].MV)); // zero vectors
          pMBInfo[k].mb_type = 0; // skipped type

          roi.width = BlkWidth_l;
          roi.height = BlkHeight_l;
          ippiCopy_8u_C1R(YRecFrame[curr_field][0] + cur_offset, YFrameHSize, YBlockRec, YFrameHSize, roi);
          roi.width = BlkWidth_c;
          roi.height = BlkHeight_c;
          ippiCopy_8u_C1R(URecFrame[curr_field][0] + cur_offset_c, UVFrameHSize, UBlockRec, UVFrameHSize, roi);
          ippiCopy_8u_C1R(VRecFrame[curr_field][0] + cur_offset_c, UVFrameHSize, VBlockRec, UVFrameHSize, roi);

          macroblock_address_increment++;
          k++;
          continue;
        }
      }
      best->var_sum = (1 << 30);

      curr->mb_type = MB_INTRA;
      curr->dct_type = DCT_FRAME;
      curr->var_sum = SCALE_VAR_INTRA(curr->var_sum);
      if (curr->var_sum < best->var_sum)
      {
        SWAP_PTR(best, curr);
      }

      BOUNDS_H(0,i)
      ME_FRAME(0, curr->var_sum, curr->pDiff, curr->dct_type);
      curr->var_sum = SCALE_VAR(curr->var_sum, SC_VAR_1V);

      if(curr->var_sum < best->var_sum)
      {
        curr->mb_type = MB_FORWARD;
        curr->pred_type = MC_FRAME;
        SWAP_PTR(best, curr);
      }

      if (!curr_frame_pred ) {
        IF_GOOD_PRED(best->var, best->mean) {
          goodpred = 1;
          goto encodeMB;
        }
      }

      // Field type
      if (!curr_frame_dct) {
        VARMEAN_FIELD_Y(curr->var, mean_fld, curr->var_sum);
        curr->var_sum = SCALE_VAR_INTRA(curr->var_sum);
        if(curr->var_sum < best->var_sum)
        {
          curr->mb_type = MB_INTRA;
          curr->dct_type = DCT_FIELD;
          SWAP_PTR(best, curr);
        }
      }

      if (!curr_frame_pred && (vector[2][0].x | vector[2][0].y)) {

        ME_FIELD(0, curr->var_sum, curr->pDiff, curr->dct_type);
        curr->var_sum = SCALE_VAR(curr->var_sum, SC_VAR_2V);

        if(curr->var_sum < best->var_sum)
        {
          curr->mb_type = MB_FORWARD;
          curr->pred_type = MC_FIELD;
          SWAP_PTR(best, curr);
        }
      }

encodeMB:
      pMBInfo[k].prediction_type = best->pred_type;
      pMBInfo[k].dct_type = best->dct_type;
//enc_intra:
      pMBInfo[k].mb_type = best->mb_type;

      pDiff = best->pDiff;
      if (best->mb_type & MB_INTRA)
      { // intra
        ippsSet_8u(0, (Ipp8u*)threadSpec[numTh].PMV, sizeof(threadSpec[0].PMV));
        ippsSet_8u(0, (Ipp8u*)pMBInfo[k].MV, sizeof(pMBInfo[k].MV));

        PutAddrIncrement(macroblock_address_increment, numTh);
        macroblock_address_increment = 1;

        if(slice_macroblock_address - slice_past_intra_address > 1) {
          dc_dct_pred[0] = dc_dct_pred[1] = dc_dct_pred[2] = ResetTbl[intra_dc_precision];
        }

        slice_past_intra_address = (i >> 4);

        const Ipp8u *BlockSrc[3] = {YBlock, UBlock, VBlock};
        Ipp8u *BlockRec[3] = {YBlockRec, UBlockRec, VBlockRec};

        PutIntraMacroBlock(numTh, k, BlockSrc, BlockRec, dc_dct_pred);

        if (!curr_frame_dct)
          threadSpec[numTh].fieldCount += (pMBInfo[k].dct_type == DCT_FIELD)? 1 : -1;
        numIntra++;
        k++;
        continue;
      } //intra


      //non-intra
      if (pMBInfo[k].prediction_type == MC_FRAME)
      {
        if (curr_frame_dct) {
          GETDIFF_FRAME(Y, Y, l, pDiff, 0);
        }

        GETDIFF_FRAME(U, UV, c, pDiff, 0);
        GETDIFF_FRAME(V, UV, c, pDiff, 0);

        NonIntraMBCoeffs(numTh, best, Count, &CodedBlockPattern);

        if (!CodedBlockPattern) {
          if (!(vector[2][0].x | vector[2][0].y)) { // no motion
            if (i != 0 && i != MBcountH*16 - 16 &&
              (picture_structure == MPS_PROGRESSIVE || // probably first cond isn't needed
              pMBInfo[k].mv_field_sel[2][0] == curr_field)) // wrong when ipflag==1
            {
              goto skip_macroblock;
            }
          }
        }

        PutAddrIncrement(macroblock_address_increment, numTh);
        macroblock_address_increment = 1;

        if (!(vector[2][0].x | vector[2][0].y) && CodedBlockPattern &&
          ( picture_structure == MPS_PROGRESSIVE ||
          pMBInfo[k].mv_field_sel[2][0] == curr_field) ) { // no motion
          PUT_MB_MODE_NO_MV(MPEG2_P_PICTURE);
          PutMV_FRAME(numTh, k, &vector[2][0], 0);
        } else {
          PUT_MB_MODES(MPEG2_P_PICTURE, MB_FORWARD);
          PutMV_FRAME(numTh, k, &vector[2][0], MB_FORWARD);
        }
      }
      else
      {//FIELD prediction
        //!!! Cases with No motion and Skipped macroblocks are included in FRAME prediction!!!
        GETDIFF_FIELD(U, UV, c, pDiff, 0);
        GETDIFF_FIELD(V, UV, c, pDiff, 0);

        NonIntraMBCoeffs(numTh, best, Count, &CodedBlockPattern);

        PutAddrIncrement(macroblock_address_increment, numTh);
        macroblock_address_increment = 1;

        PUT_MB_MODES(MPEG2_P_PICTURE, MB_FORWARD);
        PutMV_FIELD(numTh, k, &vector[0][0], &vector[1][0], MB_FORWARD);
      }

      PUT_BLOCK_PATTERN(CodedBlockPattern);

      // Put blocks and decode
      const Ipp32s *dct_step, *diff_off;
      if (pMBInfo[k].dct_type == DCT_FIELD)
      {
        dct_step = fld_dct_step;
        diff_off = fld_diff_off;
      } else {
        dct_step = frm_dct_step;
        diff_off = frm_diff_off;
      }
      pMBlock = threadSpec[numTh].pMBlock;
      for (blk = 0; blk < block_count; blk++, pMBlock += 64) {
        pDiff = threadSpec[numTh].pDiff + diff_off[blk];
        if (!Count[blk]) {
          ippiSet_16s_C1R(0, pDiff, dct_step[blk], roi8x8);
          continue;
        }
        PutNonIntraBlock(pMBlock, Count[blk], numTh);
        ippiQuantInv_MPEG2_16s_C1I(pMBlock, quantiser_scale_value, NonIntraQMatrix);
        ippiDCT8x8Inv_16s_C1R(pMBlock, pDiff, dct_step[blk]);
      }

      // Motion compensation
      pDiff = threadSpec[numTh].pDiff;

      if(pMBInfo[k].prediction_type == MC_FRAME)
      {
        MC_FRAME_F(Y, Y, l, pDiff);
        MC_FRAME_F(U, UV, c, pDiff);
        MC_FRAME_F(V, UV, c, pDiff);
      } else {
        MC_FIELD_F(Y, Y, l, pDiff);
        MC_FIELD_F(U, UV, c, pDiff);
        MC_FIELD_F(V, UV, c, pDiff);
      }

      //pMBInfo[k].cbp = CodedBlockPattern;
      ippsCopy_8u((Ipp8u*)threadSpec[numTh].PMV, (Ipp8u*)pMBInfo[k].MV, sizeof(threadSpec[0].PMV));
      if (!curr_frame_dct)
        threadSpec[numTh].fieldCount += (pMBInfo[k].dct_type == DCT_FIELD)? 1 : -1;

      k++;
    } // for(i)
  } // for(j)

  threadSpec[numTh].numIntra = numIntra;
  Ipp32s mul = ipflag ? 0x200 : 0x100/P_distance; // double for IP case
  k = (threads) ? (start_y/16)*MBcountH : 0;
  for(j=start_y; j < stop_y; j += 16)
  {
      for(i=0; i < (Ipp32s)encodeInfo.m_info.videoInfo.m_iWidth; i += 16, k++)
      {
        pMBInfo[k].MV_P[0].x = pMBInfo[k].MV[0][0].x*mul;
        pMBInfo[k].MV_P[0].y = pMBInfo[k].MV[0][0].y*mul;
        pMBInfo[k].MV_P[1].x = pMBInfo[k].MV[1][0].x*mul;
        pMBInfo[k].MV_P[1].y = pMBInfo[k].MV[1][0].y*mul;
      }
  }
}

#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
