/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER

#ifdef M2_USE_CME

//#include "ipps.h"
#include "umc_mpeg2_enc_defs.h"

#include "umc_me_m2.h"

using namespace UMC;

Status MPEG2VideoEncoderBase::externME()
{
//  return UMC_ERR_FAILED;
  Status ret = UMC_OK;
  Ipp32s fwd=0, bwd=1;
  Ipp32s i, j, k;

  MeParams MEParams;
  //MeFrame MEFrame;
  if(m_pME == 0) {
    m_pME = new MeMPEG2;
    if(m_pME == 0)
      return UMC_ERR_ALLOC;

    MeInitParams MEInitParams;
    ippsSet_8u(0,(Ipp8u*)&MEInitParams,sizeof(MEInitParams));
    MEInitParams.width = encodeInfo.info.videoInfo.m_iWidth;
    MEInitParams.height = encodeInfo.info.videoInfo.m_iWidth;
    /// not clear
    MEInitParams.refPadding = 0; //at least 32 bits on each side
    /// nameless structure is used - warning
    /// if interlaced 16x8?
    MEInitParams.MbPart = ME_Mb16x16;
    MEInitParams.SearchDirection = ME_BidirSearch;

    /// why returns bool
    if(m_pME->Init(&MEInitParams) != true)
      return UMC_ERR_INIT;
  }

  ippsSet_8u(0,(Ipp8u*)&MEParams,sizeof(MEParams));

  MeFrame frames[3];
  MeFrame *ref_ptr[2];
  ippsSet_8u(0,(Ipp8u*)&frames,sizeof(frames));
  for(j=0; j<3; j++) {
    frames[j].HeightMB = MBcountV;
    frames[j].WidthMB = MBcountH;
    frames[j].index = 0;
    frames[j].step[0] = YFrameHSize;
    frames[j].step[1] = UVFrameHSize;
    frames[j].step[2] = UVFrameHSize;
  }

  frames[0].type = picture_coding_type==B_PICTURE ? ME_FrmBidir : ME_FrmFrw;
  Ipp32s ref_ind[2] = {0,1};
  if(B_count*2 > encodeInfo.IPDistance) {
    ref_ind[0] = 1; // start from closest
    ref_ind[1] = 0;
  }
  frames[0].RefIndx = ref_ind;
  frames[0].ptr[0] = Y_src;
  frames[0].ptr[1] = U_src;
  frames[0].ptr[2] = V_src;
  MEParams.pSrc = &frames[0];

  MEParams.FRefFramesNum = 1;
  ref_ptr[ref_ind[0]] = &frames[1];
  MEParams.pRefF = &ref_ptr[0];
  if(picture_coding_type==B_PICTURE) {
    MEParams.BRefFramesNum = 1;
    ref_ptr[ref_ind[1]] = &frames[2];
    MEParams.pRefB = &ref_ptr[1];
  } else {
    MEParams.BRefFramesNum = 0;
    ref_ptr[ref_ind[1]] = 0;
    MEParams.pRefB = 0;
  }
  for(j=1; j<(2 + MEParams.BRefFramesNum); j++) {
    for(i=0; i<3; i++) {
      frames[j].ptr[i] = (Ipp8u*)(pRotFrames[j-1][0]->GetPlanePointer(i));
    }
  }


  MEParams.Interpolation   = ME_VC1_Bilinear; // ?? bilinear, bicubic, ...
  MEParams.SearchDirection = picture_coding_type==B_PICTURE ? ME_BidirSearch : ME_ForwardSearch;
  /// no HalfPixel?
  MEParams.PixelType       = ME_HalfPixel;
  MEParams.MbPart          = ME_Mb16x16;
  MEParams.SetSearchSpeed(64); //0-127, 0 - maximum quality, 127 - maximum speed, -1 use specified parameters
  MEParams.PicRange.top_left.x = 0;//-32;
  MEParams.PicRange.top_left.y = 0;//-32;
  MEParams.PicRange.bottom_right.x = encodeInfo.info.videoInfo.m_iWidth + 0;//32;
  MEParams.PicRange.bottom_right.y = encodeInfo.info.videoInfo.m_iHeight + 0;//32;
  /// can't specify different F/B ranges
  MEParams.SearchRange.x = pMotionData[B_count].searchRange[0][0]; //in integer pixel
  MEParams.SearchRange.y = pMotionData[B_count].searchRange[0][1];
  //MEParams.SearchRange.x = 64; //in integer pixel
  //MEParams.SearchRange.y = 32;

  MEParams.Quant = quantiser_scale_value;

  //ippsSet_8u(0,(Ipp8u*)&MEFrame,sizeof(MEFrame));
  ///// what is Index?
  //MEFrame.Index    = 0;
  //MEFrame.Type     = picture_coding_type==B_PICTURE ? FrmBidir : FrmFrw;
  //MEFrame.NumOfMVs = picture_coding_type==B_PICTURE ? 2 : 1;
//printf("before ME %d\n", encodeInfo.numEncodedFrames); fflush(stdout);
  if(m_pME->EstimateFrame(&MEParams) != true)
    return UMC_ERR_INVALID_PARAMS;
//printf(" after ME %d\n", encodeInfo.numEncodedFrames); fflush(stdout);

#define HISTSZ 10
#define FC_TRESH 16

#define ABS_MV(vec) ((vec) ^ ((vec) >> 31))

#define VEC_TO_F_CODE(vec,fcode) {    \
  Ipp32s val = ABS_MV(vec); \
  RANGE_TO_F_CODE(val, (fcode))       \
}

  Ipp32s countI = 0, countH[2] = {0,0};
  Ipp32s histMV[2][HISTSZ][2] = {0,};
  Ipp32s decrease[2][2] = {0,};

  // complete review is needed
  for(k=0, j=0; j<MBcountV; j++) {
    for(i=0; i<MBcountH; i++, k++) {
      pMBInfo[k].dct_type = DCT_FRAME;
      pMBInfo[k].prediction_type = MC_FRAME;
      //need to analyze skipping
      //pMBInfo[k].skipped = MEFrame.MBs[k].skipped;

      // to check if in half pixels
      // to check positions of B in BW
      pMBInfo[k].MV[0][fwd].x = frames[0].MBs[k].MV[0][0].x>>0;
      pMBInfo[k].MV[0][fwd].y = frames[0].MBs[k].MV[0][0].y>>0;
      pMBInfo[k].MV[0][bwd].x = frames[0].MBs[k].MV[1][0].x>>0;
      pMBInfo[k].MV[0][bwd].y = frames[0].MBs[k].MV[1][0].y>>0;

      switch(frames[0].MBs[k].MbType) {
        case ME_MbIntra: pMBInfo[k].mb_type = MB_INTRA;
          pMBInfo[k].prediction_type = 0;
          countI ++;
          break;
        case ME_MbFrw:
          pMBInfo[k].mb_type = (fwd==0)?MB_FORWARD:MB_BACKWARD;
          break;
        case ME_MbBkw:
          pMBInfo[k].mb_type = (bwd==1)?MB_BACKWARD:MB_FORWARD;
// both MVs are stored in their positions!!!
          break;
        case ME_MbBidir:
          pMBInfo[k].mb_type = MB_BACKWARD | MB_FORWARD;
          break;
        default:
          return UMC_ERR_FAILED;
      }
      //if (i>0 && i<MBcountH-1 && pMBInfo[k].mb_type != MB_INTRA) { // try to skip
      //  if (picture_coding_type==P_PICTURE && !ipflag &&
      //      MEFrame.MBs[k].MbCosts[0] < varThreshold &&
      //      pMBInfo[k].MV[0][fwd].x == 0 &&
      //      pMBInfo[k].MV[0][fwd].y == 0 )
      //    pMBInfo[k].mb_type = 0;
      //  //if (picture_coding_type==B_PICTURE &&
      //  //  !(pMBInfo[k-1].mb_type & MB_INTRA)
      //  //  && (!(pMBInfo[k - 1].mb_type & MB_FORWARD)
      //  //  || i+(threadSpec[numTh].PMV[0][0].x>>1)+16 < srcYFrameHSize)
      //  //  && (!(pMBInfo[k - 1].mb_type & MB_BACKWARD)
      //  //  || i+(threadSpec[numTh].PMV[0][1].x>>1)+16 < srcYFrameHSize) )

      //}

      // fcode histogram to select it
      Ipp32s fcode;
      if(pMBInfo[k].mb_type & MB_FORWARD) {
        VEC_TO_F_CODE(pMBInfo[k].MV[0][0].x, fcode);
        histMV[0][fcode][0]++;
        VEC_TO_F_CODE(pMBInfo[k].MV[0][0].y, fcode);
        histMV[0][fcode][1]++;
        countH[0]++;
      }
      if(pMBInfo[k].mb_type & MB_BACKWARD) {
        VEC_TO_F_CODE(pMBInfo[k].MV[0][1].x, fcode);
        histMV[1][fcode][0]++;
        VEC_TO_F_CODE(pMBInfo[k].MV[0][1].y, fcode);
        histMV[1][fcode][1]++;
        countH[1]++;
      }
    }
  }

  if(picture_coding_type == P_PICTURE && countI*2 > MBcount) {
    return 21;
  }

  for(j=0; j<2; j++) {
    if(j==1 && picture_coding_type == P_PICTURE)
      break;
    for(i=0; i<2; i++) {
      Ipp32s remain = countH[j];
      for(k=0; k<HISTSZ; k++) {
        remain -= histMV[j][k][i];
        if(remain*FC_TRESH <= countH[j]) {
          //while(k>1 && histMV[j][k][i]*FC_TRESH*2 < countH[j] )
          //  k--;
          pMotionData[B_count].searchRange[j][i] = 4 << k;
          pMotionData[B_count].f_code[j][i] = k;
          decrease[j][i] = remain;
          break;
        }
      }
    }
  }
  for(k=0; k<MBcountV*MBcountH; k++) {
    if(pMBInfo[k].mb_type & MB_FORWARD) {
      if(ABS_MV(pMBInfo[k].MV[0][0].x) >= pMotionData[B_count].searchRange[0][0]) {
        pMBInfo[k].mb_type &= ~MB_FORWARD;
      }
      if(ABS_MV(pMBInfo[k].MV[0][0].y) >= pMotionData[B_count].searchRange[0][1]) {
        pMBInfo[k].mb_type &= ~MB_FORWARD;
      }
    }
    if(pMBInfo[k].mb_type & MB_BACKWARD) {
      if(ABS_MV(pMBInfo[k].MV[0][1].x) >= pMotionData[B_count].searchRange[1][0]) {
        pMBInfo[k].mb_type &= ~MB_BACKWARD;
      }
      if(ABS_MV(pMBInfo[k].MV[0][1].y) >= pMotionData[B_count].searchRange[1][1]) {
        pMBInfo[k].mb_type &= ~MB_BACKWARD;
      }
    }
  }

  return ret;
}
#endif // M2_USE_CME
#endif
