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

using namespace UMC;

void MPEG2VideoEncoderBase::NonIntraMBCoeffs(Ipp32s numTh, MB_prediction_info *prediction_info, Ipp32s *Count, Ipp32s *pCodedBlockPattern)
{
  Ipp16s *pDiff = prediction_info->pDiff;
  Ipp32s *var = prediction_info->var;
  Ipp32s *mean = prediction_info->mean;
  Ipp16s *pMBlock = threadSpec[numTh].pMBlock;
  const Ipp32s *dct_step, *diff_off;
  Ipp32s loop;
  Ipp32s CodedBlockPattern = 0;

  if (prediction_info->dct_type == DCT_FRAME) {
    dct_step = frm_dct_step;
    diff_off = frm_diff_off;
  } else {
    dct_step = fld_dct_step;
    diff_off = fld_diff_off;
  }

  for (loop = 0; loop < block_count; loop++) {
    CodedBlockPattern = (CodedBlockPattern << 1);
    if (loop < 4 )
      ippiVarSum8x8_16s32s_C1R(pDiff + diff_off[loop], dct_step[loop], &var[loop], &mean[loop]);
    if (loop < 4 && var[loop] < varThreshold && abs(mean[loop]) < meanThreshold) {
      Count[loop] = 0;
    } else {
      ippiDCT8x8Fwd_16s_C1R(pDiff + diff_off[loop], dct_step[loop], pMBlock);

      ippiQuant_MPEG2_16s_C1I(pMBlock, quantiser_scale_value, InvNonIntraQMatrix, &Count[loop]);
      if (Count[loop] != 0) CodedBlockPattern++;
    }
    pMBlock += 64;
  }

  *pCodedBlockPattern = CodedBlockPattern;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Coding current picture ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MPEG2VideoEncoderBase::encodePicture(Ipp32s numTh)
{
    switch (picture_coding_type) {
    case MPEG2_I_PICTURE:
      encodeI(numTh);
      break;
    case MPEG2_P_PICTURE:
      encodeP(numTh);
      break;
    case MPEG2_B_PICTURE:
      encodeB(numTh);
      break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Coding I picture ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MPEG2VideoEncoderBase::encodeI(Ipp32s numTh)
{
  Ipp32s i, j, ic, jc, k;
  Ipp32s start_y;
  Ipp32s start_uv;
  Ipp32s stop_y;
  threadSpec[numTh].fieldCount = 0;

  CALC_START_STOP_ROWS

  for(j=start_y, jc= start_uv; j < stop_y; j += 16, jc += BlkHeight_c)
  {
    Ipp32s dc_init = ResetTbl[intra_dc_precision];
    Ipp32s dc_dct_pred[3] = {dc_init, dc_init, dc_init};
    //Ipp32s tmp_dc_dct_pred[2][3] = {{dc_init, dc_init, dc_init}, {dc_init, dc_init, dc_init}};

    PutSliceHeader(j >> 4, numTh);

    for(i=ic=0; i < (Ipp32s)encodeInfo.m_info.videoInfo.m_iWidth; i += 16, ic += BlkWidth_c)
    {
      const Ipp8u *BlockSrc[3];
      Ipp8u       *BlockRec[3];
      Ipp32s cur_offset   = i  + j  * YFrameHSize;
      Ipp32s cur_offset_c = ic + jc * UVFrameHSize;

      BlockSrc[0] = Y_src + cur_offset;
      BlockSrc[1] = U_src + cur_offset_c;
      BlockSrc[2] = V_src + cur_offset_c;
      BlockRec[0] = YRecFrame[curr_field][1] + cur_offset;
      BlockRec[1] = URecFrame[curr_field][1] + cur_offset_c;
      BlockRec[2] = VRecFrame[curr_field][1] + cur_offset_c;

      pMBInfo[k].prediction_type = MC_FRAME;
      pMBInfo[k].mb_type         = MB_INTRA;
      pMBInfo[k].dct_type        = DCT_FRAME;


      if (!curr_frame_dct) {
        Ipp32s var_fld = 0, var = 0;
        ippiFrameFieldSAD16x16_8u32s_C1R(BlockSrc[0], YFrameHSize, &var, &var_fld);

        if (var_fld < var) {
          pMBInfo[k].dct_type = DCT_FIELD;
          threadSpec[numTh].fieldCount ++;
        } else {
          threadSpec[numTh].fieldCount --;
        }
      }


      // macroblock in I picture cannot be skipped
      PutAddrIncrement(1 , numTh);

      PutIntraMacroBlock(numTh, k, BlockSrc, BlockRec, dc_dct_pred);

      k++;
    } //for(i)
  } //for(j)
}


#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
