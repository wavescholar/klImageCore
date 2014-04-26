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
#include "vm_time.h"
#include "vm_sys_info.h"

using namespace UMC;

static const Ipp64f ratetab[8]=
  {24000.0/1001.0,24.0,25.0,30000.0/1001.0,30.0,50.0,60000.0/1001.0,60.0};

// converts display aspect ratio to pixel AR
// or vise versa with exchanged width and height
static Status DARtoPAR(Ipp32u width, Ipp32u height, Ipp32u dar_h, Ipp32u dar_v,
                Ipp32u *par_h, Ipp32u *par_v)
{
  // (width*par_h) / (height*par_v) == dar_h/dar_v =>
  // par_h / par_v == dar_h * height / (dar_v * width)
  Ipp32s simple_tab[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
  Ipp32s denom;
  Ipp32u i;

  // suppose no overflow of 32s
  Ipp32s h = dar_h * height;
  Ipp32s v = dar_v * width;
  // remove common multipliers
  while( ((h|v)&1) == 0 ) {
    h>>=1;
    v>>=1;
  }

  for(i=0;i<sizeof(simple_tab)/sizeof(simple_tab[0]);i++) {
    denom = simple_tab[i];
    while(h%denom==0 && v%denom==0) {
      v /= denom;
      h /= denom;
    }
    if(v<=denom || h<=denom)
      break;
  }
  *par_h = h;
  *par_v = v;
  // can don't reach minimum, no problem
  if(i < sizeof(simple_tab)/sizeof(simple_tab[0]))
    return UMC_WRN_INVALID_STREAM;
  return UMC_OK;
};

MPEG2VideoEncoderBase::MPEG2VideoEncoderBase ()
{
  m_Inited = 0;
  picture_structure = MPS_PROGRESSIVE;
  top_field_first = 0; // 1 when !progressive
  repeat_first_field = 0;
  vbv_delay   = 19887;
  onlyIFrames = 0;
  encodeInfo.lFlags = FLAG_VENC_REORDER;
  frame_loader = NULL;
  m_PTS = -1;
  m_DTS = -1;

  pMBInfo = 0;
  threadsAllocated = 0;
  threadSpec = NULL;
  threads    = NULL;
  vlcTableB15 = 0;
  vlcTableB5c_e = 0;

  quantiser_scale_value = -1;
  intra_dc_precision = 0; // 8 bit
  varThreshold = 0;
  prqscale[0] = prqscale[1] = prqscale[2] = 0;
  prsize[0]   = prsize[1]   = prsize[2]   = 0;

  pMotionData = NULL;
  MotionDataCount = 0;
}

MPEG2VideoEncoderBase::~MPEG2VideoEncoderBase ()
{
  Close();
}

static Ipp32u VM_THREAD_CALLCONVENTION ThreadWorkingRoutine(void* ptr)
{
  threadInfo* th = (threadInfo*)ptr;

  for (;;) {
    // wait for start
    if (VM_OK != vm_event_wait(&th->start_event)) {
      //vm_debug_trace(VM_DEBUG_PROGRESS, VM_STRING("ThreadWorkingRoutine result wait start_event\n"));
    }

    if (VM_TIMEOUT != vm_event_timed_wait(&th->quit_event, 0)) {
      break;
    }

    th->m_lpOwner->encodePicture(th->numTh);

    if (VM_OK != vm_event_signal(&th->stop_event)) {
      //vm_debug_trace(VM_DEBUG_PROGRESS, VM_STRING("ThreadWorkingRoutine result signal stop_event\n"));
    }

    //vm_time_sleep(0);
  }

  return 0;
}

Status MPEG2VideoEncoderBase::Init(BaseCodecParams *params)
{
  Ipp32s i, j;
  Ipp32s sizeChanged = !m_Inited;

  VideoEncoderParams *VideoParams = DynamicCast<VideoEncoderParams, BaseCodecParams>(params);
  if(VideoParams != 0) { // at least VideoEncoder parameters
    MPEG2EncoderParams *info = DynamicCast<MPEG2EncoderParams, BaseCodecParams>(params);
    if (VideoParams->m_info.videoInfo.m_iWidth <= 0 || VideoParams->m_info.videoInfo.m_iHeight <= 0) {
      return UMC_ERR_INVALID_PARAMS;
    }
    // trying to support size change
    sizeChanged = sizeChanged ||
      (VideoParams->m_info.videoInfo.m_iWidth != encodeInfo.m_info.videoInfo.m_iWidth) ||
      (VideoParams->m_info.videoInfo.m_iHeight != encodeInfo.m_info.videoInfo.m_iHeight);
    // good to have more checks before overwriting
    if(NULL != info) { // Mpeg2 encoder parameters
      // trying to support chroma_format change
      sizeChanged = sizeChanged ||
          (info->m_info.videoInfo.m_colorFormat != encodeInfo.m_info.videoInfo.m_colorFormat);
      encodeInfo = *info;
    } else
      *(VideoEncoderParams*) &encodeInfo = *VideoParams;
  } else if (NULL != params) {
    *(BaseCodecParams*) &encodeInfo = *params;
  } else if (!m_Inited) { // 0 when initialized means Reset
    return UMC_ERR_NULL_PTR;
  }

  if(params == NULL) { // Reset or SetParams(0)
    m_FirstFrame = 1;
    B_count      = 0;
    GOP_count    = 0;
    closed_gop = 1;
    return UMC_OK;
  }

  BaseCodec::Init(params); // to prepare allocator
  SetAspectRatio(encodeInfo.m_info.videoInfo.m_iSAWidth, encodeInfo.m_info.videoInfo.m_iSAHeight);

  // FrameRate to code and extensions and back to exact value
  if(UMC_OK != SetFrameRate(encodeInfo.m_info.fFramerate, encodeInfo.mpeg1))
  {
    frame_rate_code = 5;
    frame_rate_extension_n = 0;
    frame_rate_extension_d = 0;
    encodeInfo.m_info.fFramerate = 30;
  }
  encodeInfo.m_info.fFramerate = ratetab[frame_rate_code - 1]*(frame_rate_extension_n+1)/(frame_rate_extension_d+1);
  encodeInfo.Profile_and_Level_Checks();
  encodeInfo.RelationChecks();

  // allocate ranges, compute search ranges and f_code
  if (pMotionData && MotionDataCount < encodeInfo.IPDistance) {
    MP2_FREE(pMotionData);
    pMotionData = 0;
    MotionDataCount = 0;
  }
  if (pMotionData == 0) {
    pMotionData = MP2_ALLOC(MotionData, encodeInfo.IPDistance);
    if( !pMotionData )
      return UMC_ERR_ALLOC;
    MotionDataCount = encodeInfo.IPDistance;
  }
  for (i = 0; i < encodeInfo.IPDistance; i++) {
    for (j = 0; j < 2; j++) { // x/y
      if (i==0) { // P
        pMotionData[i].searchRange[0][j] = encodeInfo.rangeP[j];
        RANGE_TO_F_CODE(pMotionData[i].searchRange[0][j], pMotionData[i].f_code[0][j]);
        pMotionData[i].searchRange[0][j] = 4 << pMotionData[i].f_code[0][j];
        pMotionData[i].f_code[1][j] = 15;
        pMotionData[i].searchRange[1][j] = 0;
      } else { // B
        pMotionData[i].searchRange[0][j] =
          (encodeInfo.rangeB[0][0]*(encodeInfo.IPDistance-i) + encodeInfo.rangeB[1][0]*(i-1))/(encodeInfo.IPDistance-1);
        pMotionData[i].searchRange[1][j] =
          (encodeInfo.rangeB[1][0]*(encodeInfo.IPDistance-i) + encodeInfo.rangeB[0][0]*(i-1))/(encodeInfo.IPDistance-1);
        RANGE_TO_F_CODE(pMotionData[i].searchRange[0][j], pMotionData[i].f_code[0][j]);
        pMotionData[i].searchRange[0][j] = 4 << pMotionData[i].f_code[0][j];
        RANGE_TO_F_CODE(pMotionData[i].searchRange[1][j], pMotionData[i].f_code[1][j]);
        pMotionData[i].searchRange[1][j] = 4 << pMotionData[i].f_code[1][j];
      }
    }
  }

  if (encodeInfo.m_info.videoInfo.m_picStructure == PS_PROGRESSIVE ||
      encodeInfo.m_info.videoInfo.m_picStructure == PS_BOTTOM_FIELD_FIRST)
    top_field_first = 0;
  else
    top_field_first = 1;

  if(!m_Inited) { // only first call
    DC_Tbl[0] = Y_DC_Tbl;
    DC_Tbl[1] = Cr_DC_Tbl;
    DC_Tbl[2] = Cr_DC_Tbl;

    ippiCreateRLEncodeTable(Table15, &vlcTableB15);
    ippiCreateRLEncodeTable(dct_coeff_next_RL, &vlcTableB5c_e);
  }

  // chroma format dependent
  BlkWidth_c = (encodeInfo.m_info.videoInfo.m_colorFormat != YUV444) ? 8 : 16;
  BlkStride_c = 16;//BlkWidth_c;
  BlkHeight_c = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV420) ? 8 : 16;
  chroma_fld_flag = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV420) ? 0 : 1;

  switch (encodeInfo.m_info.videoInfo.m_colorFormat)
  {
  case YUV420:
    block_count = 6;
    func_getdiff_frame_c = ippiGetDiff8x8_8u16s_C1;
    func_getdiff_field_c = ippiGetDiff8x4_8u16s_C1;
    func_getdiffB_frame_c = ippiGetDiff8x8B_8u16s_C1;
    func_getdiffB_field_c = ippiGetDiff8x4B_8u16s_C1;
    func_mc_frame_c = ippiMC8x8_8u_C1;
    func_mc_field_c = ippiMC8x4_8u_C1;
    break;

  case YUV422:
    block_count = 8;
    func_getdiff_frame_c = ippiGetDiff8x16_8u16s_C1;
    func_getdiff_field_c = ippiGetDiff8x8_8u16s_C1;
    func_getdiffB_frame_c = ippiGetDiff8x16B_8u16s_C1;
    func_getdiffB_field_c = ippiGetDiff8x8B_8u16s_C1;
    func_mc_frame_c = ippiMC8x16_8u_C1;
    func_mc_field_c = ippiMC8x8_8u_C1;
    break;

  case YUV444:
    block_count = 12;
    func_getdiff_frame_c = ippiGetDiff16x16_8u16s_C1;
    func_getdiff_field_c = ippiGetDiff16x8_8u16s_C1;
    func_getdiffB_frame_c = ippiGetDiff16x16B_8u16s_C1;
    func_getdiffB_field_c = ippiGetDiff16x8B_8u16s_C1;
    func_mc_frame_c = ippiMC16x16_8u_C1;
    func_mc_field_c = ippiMC16x8_8u_C1;
    break;
  }

  for (i = 0; i < 4; i++) {
    frm_dct_step[i] = BlkStride_l*sizeof(Ipp16s);
    fld_dct_step[i] = 2*BlkStride_l*sizeof(Ipp16s);
  }
  for (i = 4; i < block_count; i++) {
    frm_dct_step[i] = BlkStride_c*sizeof(Ipp16s);
    fld_dct_step[i] = BlkStride_c*sizeof(Ipp16s) << chroma_fld_flag;
  }

  frm_diff_off[0] = 0;
  frm_diff_off[1] = 8;
  frm_diff_off[2] = BlkStride_l*8;
  frm_diff_off[3] = BlkStride_l*8 + 8;
  frm_diff_off[4] = OFF_U;
  frm_diff_off[5] = OFF_V;
  frm_diff_off[6] = OFF_U + BlkStride_c*8;
  frm_diff_off[7] = OFF_V + BlkStride_c*8;
  frm_diff_off[8] = OFF_U + 8;
  frm_diff_off[9] = OFF_V + 8;
  frm_diff_off[10] = OFF_U + BlkStride_c*8 + 8;
  frm_diff_off[11] = OFF_V + BlkStride_c*8 + 8;

  fld_diff_off[0] = 0;
  fld_diff_off[1] = 8;
  fld_diff_off[2] = BlkStride_l;
  fld_diff_off[3] = BlkStride_l + 8;
  fld_diff_off[4] = OFF_U;
  fld_diff_off[5] = OFF_V;
  fld_diff_off[6] = OFF_U + BlkStride_c;
  fld_diff_off[7] = OFF_V + BlkStride_c;
  fld_diff_off[8] = OFF_U + 8;
  fld_diff_off[9] = OFF_V + 8;
  fld_diff_off[10] = OFF_U + BlkStride_c + 8;
  fld_diff_off[11] = OFF_V + BlkStride_c + 8;

  // frame size dependent
  // macroblock aligned size
  MBcountH = (encodeInfo.m_info.videoInfo.m_iWidth + 15)/16;
  MBcountV = (encodeInfo.m_info.videoInfo.m_iHeight + 15)/16;
  if(encodeInfo.m_info.videoInfo.m_picStructure != PS_PROGRESSIVE) {
    MBcountV = (MBcountV + 1) & ~1;
  }
  MBcount = MBcountH * MBcountV;

  YFrameHSize = MBcountH * 16;
  YFrameVSize = MBcountV * 16;
  UVFrameHSize = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV444) ? YFrameHSize : (YFrameHSize >> 1);
  UVFrameVSize = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV420) ? (YFrameVSize >> 1) : YFrameVSize;
  YFrameSize = YFrameHSize*YFrameVSize;
  UVFrameSize = UVFrameHSize*UVFrameVSize;
  YUVFrameSize = YFrameSize + 2*UVFrameSize;
  encodeInfo.m_iSuggestedInputSize = YUVFrameSize;

  block_offset_frm[0]  = 0;
  block_offset_frm[1]  = 8;
  block_offset_frm[2]  = 8*YFrameHSize;
  block_offset_frm[3]  = 8*YFrameHSize + 8;
  block_offset_frm[4]  = 0;
  block_offset_frm[5]  = 0;
  block_offset_frm[6]  = 8*UVFrameHSize;
  block_offset_frm[7]  = 8*UVFrameHSize;
  block_offset_frm[8]  = 8;
  block_offset_frm[9]  = 8;
  block_offset_frm[10] = 8*UVFrameHSize + 8;
  block_offset_frm[11] = 8*UVFrameHSize + 8;

  block_offset_fld[0]  = 0;
  block_offset_fld[1]  = 8;
  block_offset_fld[2]  = YFrameHSize;
  block_offset_fld[3]  = YFrameHSize + 8;
  block_offset_fld[4]  = 0;
  block_offset_fld[5]  = 0;
  block_offset_fld[6]  = UVFrameHSize;
  block_offset_fld[7]  = UVFrameHSize;
  block_offset_fld[8]  = 8;
  block_offset_fld[9]  = 8;
  block_offset_fld[10] = UVFrameHSize + 8;
  block_offset_fld[11] = UVFrameHSize + 8;

  // verify bitrate
  {
      Ipp32s BitRate = encodeInfo.m_info.iBitrate;
      Ipp64f pixrate = encodeInfo.m_info.fFramerate * YUVFrameSize * 8;
    if((Ipp64f)encodeInfo.m_info.iBitrate > pixrate)
      BitRate = (Ipp32s)(pixrate); // too high
    if(BitRate != 0 && (Ipp64f)encodeInfo.m_info.iBitrate < pixrate/500)
      BitRate = (Ipp32s)(pixrate/500); // too low
    if ((Ipp32s)encodeInfo.m_info.iBitrate != BitRate) {
      //vm_debug_trace1(VM_DEBUG_WARNING, VM_STRING("BitRate value fixed to %d\n"), (BitRate+399)/400*400);
    }
    encodeInfo.m_info.iBitrate = (BitRate+399)/400*400; // 400 bps step
  }

  if(sizeChanged) {
    if (pMBInfo != 0)
      MP2_FREE(pMBInfo);
    pMBInfo = MP2_ALLOC(MBInfo, MBcount);
    if (pMBInfo == 0) {
      return UMC_ERR_ALLOC;
    }
    ippsSet_8u(0, (Ipp8u*)pMBInfo, sizeof(MBInfo)*MBcount);

    if(UMC_OK != CreateFrameBuffer()) // deletes if neccessary
      return UMC_ERR_ALLOC;

  }

  // threads preparation
  if (encodeInfo.m_iThreads != 1) {
    if (encodeInfo.m_iThreads == 0)
      encodeInfo.m_iThreads = vm_sys_info_get_cpu_num();

    Ipp32u max_threads = MBcountV;
    if (encodeInfo.m_info.videoInfo.m_picStructure != PS_PROGRESSIVE)
      max_threads = MBcountV/2;
    if (encodeInfo.m_iThreads > max_threads)
      encodeInfo.m_iThreads = max_threads;
  }

  // add threads if was fewer
  if(threadsAllocated < (Ipp32s)encodeInfo.m_iThreads) {
    threadSpecificData* cur_threadSpec = threadSpec;
    threadSpec = MP2_ALLOC(threadSpecificData, encodeInfo.m_iThreads);
    // copy existing
    if(threadsAllocated) {
      ippsCopy_8u((Ipp8u*)cur_threadSpec, (Ipp8u*)threadSpec,
        threadsAllocated*sizeof(threadSpecificData));
      MP2_FREE(cur_threadSpec);
    }

    if(encodeInfo.m_iThreads > 1) {
      threadInfo** cur_threads = threads;
      threads = MP2_ALLOC(threadInfo*, encodeInfo.m_iThreads - 1);
      if(threadsAllocated > 1) {
        for (i = 0; i < threadsAllocated - 1; i++)
          threads[i] = cur_threads[i];
        MP2_FREE(cur_threads);
      }
    }

    // init newly allocated
    for (i = threadsAllocated; i < (Ipp32s)encodeInfo.m_iThreads; i++) {
      threadSpec[i].pDiff   = MP2_ALLOC(Ipp16s, 3*256);
      threadSpec[i].pDiff1  = MP2_ALLOC(Ipp16s, 3*256);
      threadSpec[i].pMBlock = MP2_ALLOC(Ipp16s, 3*256);
      threadSpec[i].me_matrix_size = 0;
      threadSpec[i].me_matrix_buff = NULL;
      threadSpec[i].me_matrix_id   = 0;

      if(i>0) {
        threads[i-1] = MP2_ALLOC(threadInfo, 1);
        vm_event_set_invalid(&threads[i-1]->start_event);
        if (VM_OK != vm_event_init(&threads[i-1]->start_event, 0, 0))
          return UMC_ERR_ALLOC;
        vm_event_set_invalid(&threads[i-1]->stop_event);
        if (VM_OK != vm_event_init(&threads[i-1]->stop_event, 0, 0))
          return UMC_ERR_ALLOC;
        vm_event_set_invalid(&threads[i-1]->quit_event);
        if (VM_OK != vm_event_init(&threads[i-1]->quit_event, 0, 0))
          return UMC_ERR_ALLOC;
        vm_thread_set_invalid(&threads[i-1]->thread);
        if (0 == vm_thread_create(&threads[i-1]->thread,
                                  ThreadWorkingRoutine,
                                  threads[i-1])) {
          return UMC_ERR_ALLOC;
        }
        threads[i-1]->numTh = i;
        threads[i-1]->m_lpOwner = this;
      }
    }
    threadsAllocated = encodeInfo.m_iThreads;
  }

  Ipp32s mb_shift = (encodeInfo.m_info.videoInfo.m_picStructure == PS_PROGRESSIVE) ? 4 : 5;
  for (i = 0; i < (Ipp32s)encodeInfo.m_iThreads; i++) {
    threadSpec[i].start_row = ((16*MBcountV>>mb_shift)*i/encodeInfo.m_iThreads) << mb_shift;
  }

  // quantizer matrices
  IntraQMatrix = encodeInfo.IntraQMatrix;
  NonIntraQMatrix = (encodeInfo.CustomNonIntraQMatrix) ? encodeInfo.NonIntraQMatrix : NULL;

  if (encodeInfo.CustomIntraQMatrix) {
    InvIntraQMatrix = _InvIntraQMatrix;
    for (i = 0; i < 64; i++) {
      InvIntraQMatrix[i] = 1.f / (Ipp32f)IntraQMatrix[i];
    }
  } else {
    InvIntraQMatrix = NULL;
  }

  if (encodeInfo.CustomNonIntraQMatrix) {
    InvNonIntraQMatrix = _InvNonIntraQMatrix;
    for (i = 0; i < 64; i++) {
      InvNonIntraQMatrix[i] = 1.f / (Ipp32f)NonIntraQMatrix[i];
    }
  } else {
    InvNonIntraQMatrix = NULL;
  }

  onlyIFrames = 0;
  // field pictures use IP frames, so reconstruction is required for them
  if (encodeInfo.gopSize == 1 && encodeInfo.IPDistance == 1 && !encodeInfo.FieldPicture) {
    onlyIFrames = 1;
  }

  // initialize rate control
  InitRateControl(encodeInfo.m_info.iBitrate);

  // frames list
  m_GOP_Start  = 0;
  P_distance   = encodeInfo.IPDistance;
  B_count      = 0;
  GOP_count    = 0;
  m_FirstFrame = 1;
  closed_gop   = 1;
  m_Inited = 1;

  cpu_freq = 0;
  motion_estimation_time = 1e-7;
#ifdef MPEG2_DEBUG_CODE
  cpu_freq = (Ipp64f)(Ipp64s)(GET_FREQUENCY);
  motion_estimation_time = 0;
#endif

  return UMC_OK;
}

Status MPEG2VideoEncoderBase::GetInfo(BaseCodecParams *Params)
{
  MPEG2EncoderParams *MPEG2Params = DynamicCast<MPEG2EncoderParams, BaseCodecParams>(Params);
  encodeInfo.m_iQuality = 100 - (qscale[0]+qscale[1]+qscale[2])*100/(3*112); // rough
  encodeInfo.m_info.streamType = MPEG2_VIDEO;
  if (NULL != MPEG2Params) {
    *MPEG2Params = encodeInfo;
  } else {
    VideoEncoderParams *VideoParams = DynamicCast<VideoEncoderParams, BaseCodecParams>(Params);
    if (NULL != VideoParams) {
      *VideoParams = *(VideoEncoderParams*)&encodeInfo;
    } else if (NULL != Params){
      *Params = *(BaseCodecParams*)&encodeInfo;
    } else
      return UMC_ERR_NULL_PTR;
  }
  return UMC_OK;
}

Status MPEG2VideoEncoderBase::Close()
{
  Ipp32s i;

  if (!threadSpec) {
    return UMC_ERR_NULL_PTR;
  }

  if (threadsAllocated) {
    // let all threads to exit
    if (threadsAllocated > 1) {
      for (i = 0; i < threadsAllocated - 1; i++) {
        vm_event_signal(&threads[i]->quit_event);
        vm_event_signal(&threads[i]->start_event);
      }

      for (i = 0; i < threadsAllocated - 1; i++)
      {
        if (vm_thread_is_valid(&threads[i]->thread)) {
          vm_thread_close(&threads[i]->thread);
          vm_thread_set_invalid(&threads[i]->thread);
        }
        if (vm_event_is_valid(&threads[i]->start_event)) {
          vm_event_destroy(&threads[i]->start_event);
          vm_event_set_invalid(&threads[i]->start_event);
        }
        if (vm_event_is_valid(&threads[i]->stop_event)) {
          vm_event_destroy(&threads[i]->stop_event);
          vm_event_set_invalid(&threads[i]->stop_event);
        }
        if (vm_event_is_valid(&threads[i]->quit_event)) {
          vm_event_destroy(&threads[i]->quit_event);
          vm_event_set_invalid(&threads[i]->quit_event);
        }
        MP2_FREE(threads[i]);
      }

      MP2_FREE(threads);
    }

    for(i = 0; i < threadsAllocated; i++)
    {
      if(threadSpec[i].pDiff)
      {
        MP2_FREE(threadSpec[i].pDiff);
        threadSpec[i].pDiff = NULL;
      }
      if(threadSpec[i].pDiff1)
      {
        MP2_FREE(threadSpec[i].pDiff1);
        threadSpec[i].pDiff1 = NULL;
      }
      if(threadSpec[i].pMBlock)
      {
        MP2_FREE(threadSpec[i].pMBlock);
        threadSpec[i].pMBlock = NULL;
      }
      if (threadSpec[i].me_matrix_buff) {
        MP2_FREE(threadSpec[i].me_matrix_buff);
        threadSpec[i].me_matrix_buff = NULL;
      }
    }

    MP2_FREE(threadSpec);
    threadsAllocated = 0;
  }

  if(pMBInfo)
  {
    MP2_FREE(pMBInfo);
    pMBInfo = NULL;
  }
  if (pMotionData) {
    MP2_FREE(pMotionData);
    pMotionData = NULL;
  }
  MotionDataCount = 0;

  if(vlcTableB15)
  {
    ippiHuffmanTableFree_32s(vlcTableB15);
    vlcTableB15 = NULL;
  }
  if(vlcTableB5c_e)
  {
    ippiHuffmanTableFree_32s(vlcTableB5c_e);
    vlcTableB5c_e = NULL;
  }

  DeleteFrameBuffer();

  if (frame_loader != NULL) {
    delete frame_loader;
    frame_loader = NULL;
  }


  m_Inited = 0;

  VideoEncoder::Close();
  return UMC_OK;
}

// Get source frame size (summary size of Y, U and V frames)
Ipp32s MPEG2VideoEncoderBase::GetYUVFrameSize()
{
  Ipp32s srcUVFrameHSize = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV444) ?
      encodeInfo.m_info.videoInfo.m_iWidth : (encodeInfo.m_info.videoInfo.m_iWidth >> 1);
  Ipp32s srcUVFrameVSize = (encodeInfo.m_info.videoInfo.m_colorFormat == YUV420) ?
      (encodeInfo.m_info.videoInfo.m_iHeight >> 1) : encodeInfo.m_info.videoInfo.m_iHeight;
  return encodeInfo.m_info.videoInfo.m_iWidth*encodeInfo.m_info.videoInfo.m_iHeight + 2*srcUVFrameHSize*srcUVFrameVSize;
}

// THIS METHOD IS TO BE DELETED!

// Get pointer to internal encoder memory, where
// next YUV frame can be stored before passing
// this frame to encode function
VideoData* MPEG2VideoEncoderBase::GetNextYUVPointer()
{
  Status ret;
  ret = frames.buf_aux->Lock();
  if(ret!=UMC_OK)
    return 0;
  frames.buf_aux->Unlock();
  return frames.buf_aux;
}

// Encode reordered frames (B frames following the corresponding I/P frames).
// pict_type must be supplied (I_PICTURE, P_PICTURE, B_PICTURE).
// No buffering because the frames are already reordered.
Status MPEG2VideoEncoderBase::EncodeFrameReordered()
{
  vm_tick t_start;

  if (frames.curenc == NULL) {
    return UMC_ERR_NULL_PTR;
  }

  Ipp32s w_shift = (encodeInfo.FieldPicture) ? 1 : 0;
  Ipp32s nYPitch = YFrameHSize << w_shift;
  Ipp32s nUPitch = UVFrameHSize << w_shift;
  Ipp32s nVPitch = UVFrameHSize << w_shift;

  block_offset_frm[2] = 8*nYPitch;
  block_offset_frm[3] = 8*nYPitch + 8;
  block_offset_frm[6] = 8*nUPitch;
  block_offset_frm[7] = 8*nVPitch;
  block_offset_frm[10] = 8*nUPitch + 8;
  block_offset_frm[11] = 8*nVPitch + 8;

  block_offset_fld[2] = nYPitch;
  block_offset_fld[3] = nYPitch + 8;
  block_offset_fld[6] = nUPitch;
  block_offset_fld[7] = nVPitch;
  block_offset_fld[10] = nUPitch + 8;
  block_offset_fld[11] = nVPitch + 8;

  // prepare out buffers
  mEncodedSize = 0;
  if(output_buffer_size*8 < rc_ave_frame_bits*2) {
    return UMC_ERR_NOT_ENOUGH_BUFFER;
  }


  if(picture_coding_type == MPEG2_I_PICTURE) {
    mp_f_code = 0;
    m_GOP_Start_tmp = m_GOP_Start;
    m_GOP_Start = encodeInfo.m_iFramesCounter;
  } else {
    mp_f_code = pMotionData[B_count].f_code;
  }

  if (!encodeInfo.FieldPicture) {
    curr_frame_pred = curr_frame_dct = encodeInfo.frame_pred_frame_dct[picture_coding_type - 1];
  } else {
    curr_frame_pred = 0; //!encodeInfo.allow_prediction16x8;
    curr_frame_dct = 1;
  }
  if(curr_frame_dct == 1)
    curr_scan = encodeInfo.altscan_tab[picture_coding_type - 1] = encodeInfo.FieldPicture;
  else
    curr_scan = encodeInfo.altscan_tab[picture_coding_type - 1];

  curr_intra_vlc_format = encodeInfo.intraVLCFormat[picture_coding_type - 1];
  ipflag = 0;
  t_start = GET_TICKS;

  if (!encodeInfo.FieldPicture) {
    picture_structure = MPS_PROGRESSIVE;
    curr_field = 0;
    second_field = 0;
    PutPicture();
  } else {
    Ipp8u *pSrc[3] = {Y_src, U_src, V_src};
    MBInfo *pMBInfo0 = pMBInfo;
    Ipp64s field_endpos = 0;
    Ipp32s ifield;

    YFrameVSize >>= 1;
    UVFrameVSize >>= 1;
    MBcountV >>= 1;

    YFrameHSize<<=1;
    UVFrameHSize<<=1;

    for (ifield = 0; ifield < 2; ifield++) {
      picture_structure = (ifield != top_field_first) ? MPS_TOP_FIELD : MPS_BOTTOM_FIELD;
      curr_field   = (picture_structure == MPS_BOTTOM_FIELD) ? 1 :0;
      second_field = ifield;
      if (second_field && picture_coding_type == MPEG2_I_PICTURE) {
        ipflag = 1;
        picture_coding_type = MPEG2_P_PICTURE;
        mp_f_code = pMotionData[0].f_code;
      }

      if (picture_structure == MPS_TOP_FIELD) {
        Y_src = pSrc[0];
        U_src = pSrc[1];
        V_src = pSrc[2];
        if (picture_coding_type == MPEG2_P_PICTURE && ifield) {
          YRefFrame[1][0] = YRefFrame[1][1];
          URefFrame[1][0] = URefFrame[1][1];
          VRefFrame[1][0] = VRefFrame[1][1];
          YRecFrame[1][0] = YRecFrame[1][1];
          URecFrame[1][0] = URecFrame[1][1];
          VRecFrame[1][0] = VRecFrame[1][1];
        }
        pMBInfo = pMBInfo0;
      } else {
        Y_src = pSrc[0] + (YFrameHSize >> 1);
        U_src = pSrc[1] + (UVFrameHSize >> 1);
        V_src = pSrc[2] + (UVFrameHSize >> 1);
        if (picture_coding_type == MPEG2_P_PICTURE && ifield) {
          YRefFrame[0][0] = YRefFrame[0][1];
          URefFrame[0][0] = URefFrame[0][1];
          VRefFrame[0][0] = VRefFrame[0][1];
          YRecFrame[0][0] = YRecFrame[0][1];
          URecFrame[0][0] = URecFrame[0][1];
          VRecFrame[0][0] = VRecFrame[0][1];
        }
        pMBInfo = pMBInfo0 + (YFrameSize/(2*16*16));
      }

      PutPicture();
      field_endpos = 8*mEncodedSize;
      ipflag = 0;
    }

    Y_src = pSrc[0];
    U_src = pSrc[1];
    V_src = pSrc[2];

    // restore params
    pMBInfo = pMBInfo0;

    YFrameVSize <<= 1;
    UVFrameVSize <<= 1;
    YFrameHSize>>=1;
    UVFrameHSize>>=1;
    MBcountV <<= 1;
  }

  if (encodeInfo.me_auto_range)
  { // adjust ME search range
    if (picture_coding_type == MPEG2_P_PICTURE) {
      AdjustSearchRange(0, 0);
    } else if (picture_coding_type == MPEG2_B_PICTURE) {
      AdjustSearchRange(B_count, 0);
      AdjustSearchRange(B_count, 1);
    }
  }

#ifdef MPEG2_DEBUG_CODE
/*
  t_end = GET_TICKS;
  encodeInfo.encode_time += (Ipp64f)(Ipp64s)(t_end-t_start)/cpu_freq;
  encodeInfo.performance = (Ipp64f)encodeInfo.m_numEncodedFrames/encodeInfo.encode_time;
  encodeInfo.motion_estimation_perf = (Ipp64f)encodeInfo.m_numEncodedFrames*cpu_freq/motion_estimation_time;
*/
//logi(encodeInfo.m_numEncodedFrames);
//logi(mEncodedSize);
  if (encodeInfo.m_numEncodedFrames > 600 && mEncodedSize > 60000) {
//logs("Save!!!!!!!!!!!!!!!!!!!!!!!!!");
    vm_char bmp_fname[256];
    vm_char frame_ch = (picture_coding_type == MPEG2_I_PICTURE) ? 'i' : (picture_coding_type == MPEG2_P_PICTURE) ? 'p' : 'b';

    vm_string_sprintf(bmp_fname, VM_STRING("C:\\Documents and Settings\\All Users\\Documents\\bmp\\frame_%03d%c.bmp"), encodeInfo.m_numEncodedFrames, frame_ch);
    save_bmp(bmp_fname, -1);
    if (encodeInfo.IPDistance > 1) {
      vm_string_sprintf(bmp_fname, VM_STRING("C:\\Documents and Settings\\All Users\\Documents\\bmp\\frame_%03d%c_f.bmp"), encodeInfo.m_numEncodedFrames, frame_ch);
      save_bmp(bmp_fname, 1);
    }
    vm_string_sprintf(bmp_fname, VM_STRING("C:\\Documents and Settings\\All Users\\Documents\\bmp\\frame_%03d%c_b.bmp"), encodeInfo.m_numEncodedFrames, frame_ch);
    save_bmp(bmp_fname, 0);
  }

  //DumpPSNR();
#endif /* MPEG2_DEBUG_CODE */

  if(encodeInfo.IPDistance>1) closed_gop = 0;
  encodeInfo.m_iFramesCounter++;

  //if (encoded_size) *encoded_size = mEncodedSize;

  return (UMC_OK);
}

void MPEG2VideoEncoderBase::setUserData(vm_char* data, Ipp32s len)
{
  encodeInfo.UserData = data;
  encodeInfo.UserDataLen = len;
}

Status MPEG2VideoEncoderBase::PutPicture()
{
  Ipp32s i;
  Ipp32s isfield;
  Ipp32s bitsize;
  Ipp64f target_size;
  Ipp32s wanted_size;
  Ipp32s size;
  Status status;

  Ipp32s prevq=0, oldq;
  Ipp32s ntry;

  bQuantiserChanged = false;
  bSceneChanged = false;
  bExtendGOP = false;

  for (ntry=0; ; ntry++) {
    // Save position after headers (only 0th thread)
    Ipp32s DisplayFrameNumber;

    PrepareBuffers();
    size = 0;

    // Mpeg2 sequence header
    if (m_FirstFrame || (picture_coding_type == MPEG2_I_PICTURE)) {
      PutSequenceHeader();
      PutSequenceExt();
      PutSequenceDisplayExt(); // no need

      // optionally output some text data
      PutUserData(0);

      PutGOPHeader(encodeInfo.m_iFramesCounter);
      size = (32+7 + BITPOS(threadSpec[0]))>>3;
    }

    PictureRateControl(size*8+32);

    DisplayFrameNumber = encodeInfo.m_iFramesCounter;
    if (picture_coding_type == MPEG2_B_PICTURE)
      DisplayFrameNumber--;
    else if(!closed_gop) { // closed can be only while I is encoded
      DisplayFrameNumber += P_distance-1; // can differ for the tail
    }

    temporal_reference = DisplayFrameNumber - m_GOP_Start;

    if(picture_coding_type == MPEG2_I_PICTURE) {
      if(quantiser_scale_value < 5)
        encodeInfo.intraVLCFormat[picture_coding_type - MPEG2_I_PICTURE] = 1;
      else if(quantiser_scale_value > 15)
        encodeInfo.intraVLCFormat[picture_coding_type - MPEG2_I_PICTURE] = 0;
      curr_intra_vlc_format = encodeInfo.intraVLCFormat[picture_coding_type - MPEG2_I_PICTURE];
    }


    PutPictureHeader();
    PutPictureCodingExt();
    PutUserData(2);

    //logi(encodeInfo.m_numThreads);
    if (encodeInfo.m_iThreads > 1) {
      // start additional thread(s)
      for (i = 0; i < (Ipp32s)encodeInfo.m_iThreads - 1; i++) {
        vm_event_signal(&threads[i]->start_event);
      }
    }

    encodePicture(0);

    if (encodeInfo.m_iThreads > 1) {
      // wait additional thread(s)
      for (i = 0; i < (Ipp32s)encodeInfo.m_iThreads - 1; i++) {
        vm_event_wait(&threads[i]->stop_event);
      }
    }

    // flush buffer
    status = FlushBuffers(&size);

    isfield = (picture_structure != MPS_PROGRESSIVE);
    bitsize = size*8;
    if(encodeInfo.rc_mode == RC_CBR) {
      target_size = rc_tagsize[picture_coding_type-MPEG2_I_PICTURE];
      wanted_size = (Ipp32s)(target_size - rc_dev / 3 * target_size / rc_tagsize[0]);
      wanted_size >>= isfield;
    } else {
      target_size = wanted_size = bitsize; // no estimations
    }

    if(picture_coding_type == MPEG2_I_PICTURE) {
      if(bitsize > (MBcount*128>>isfield))
        encodeInfo.intraVLCFormat[0] = 1;
      else //if(bitsize < MBcount*192)
        encodeInfo.intraVLCFormat[0] = 0;
    }

#ifdef SCENE_DETECTION
    bSceneChanged = false;
    if ( picture_coding_type == MPEG2_P_PICTURE && second_field == 0 && bExtendGOP == false /*&& bQuantiserChanged == false*/) { //
      Ipp32s numIntra = 0;
      Ipp32s t;
      for (t = 0; t < (Ipp32s)encodeInfo.m_iThreads; t++) {
        numIntra += threadSpec[t].numIntra;
      }
      if(picture_structure != MPS_PROGRESSIVE)
        numIntra <<= 1; // MBcount for the frame picture

      if (numIntra*2 > MBcount*1 // 2/3 Intra: I frame instead of P
         ) {
        picture_coding_type = MPEG2_I_PICTURE;
        //curr_gop = 0;
        GOP_count = 0;
        bSceneChanged = true;
        mp_f_code = 0;
        m_GOP_Start_tmp = m_GOP_Start;
        m_GOP_Start = encodeInfo.m_iFramesCounter;
        ntry = -1;
        //logi(encodeInfo.m_numEncodedFrames);
        continue;
      }
    }
    // scene change not detected
#endif //SCENE_DETECTION

    oldq = prevq;
    prevq = quantiser_scale_value;
    bQuantiserChanged = false;

    if (status == UMC_ERR_NOT_ENOUGH_BUFFER || bitsize > rc_vbv_max
      || (bitsize > wanted_size*2 && (ntry==0 || oldq<prevq))
      //&& rc_dev >= 0 &&
      //    qscale[picture_coding_type-MPEG2_I_PICTURE] <= quantiser_scale_value
      )
    {
      changeQuant(quantiser_scale_value + 1);
      if(prevq == quantiser_scale_value) {
        if (picture_coding_type == MPEG2_I_PICTURE &&
           !m_FirstFrame && encodeInfo.gopSize > 1 &&
           (status == UMC_ERR_NOT_ENOUGH_BUFFER || bitsize > rc_vbv_max)) {
          picture_coding_type = MPEG2_P_PICTURE;
          m_GOP_Start = m_GOP_Start_tmp;
          mp_f_code = pMotionData[0].f_code;
          bExtendGOP = true;
          //curr_gop += encodeInfo.IPDistance;
          GOP_count -= encodeInfo.IPDistance; // add PB..B group
          ntry = -1;
          continue;
        }
        status = UMC_ERR_FAILED; // already maximum;
      } else {
        bQuantiserChanged = true;
        continue;
      }
    } else if (bitsize < rc_vbv_min
      //|| (bitsize*3 < wanted_size && rc_dev <= 0 &&
      //   qscale[picture_coding_type-MPEG2_I_PICTURE] >= quantiser_scale_value)
      )
    {
      if(ntry==0 || oldq>prevq)
        changeQuant(quantiser_scale_value - 1);
      if(prevq == quantiser_scale_value) {
        Ipp8u *p = out_pointer + size;
        while(bitsize < rc_vbv_min) {
          *(Ipp32u*)p = 0;
          p += 4;
          size += 4;
          bitsize += 32;
        }
        status = UMC_OK; // already minimum;
      } else {
        bQuantiserChanged = true;
        continue;
      }
    }

    // if (!bQuantiserChanged && !bSceneChanged)
    m_FirstFrame = 0;
    qscale[picture_coding_type-MPEG2_I_PICTURE] = quantiser_scale_value;
    out_pointer += size;
    output_buffer_size -= size;
    mEncodedSize += size;
    PostPictureRateControl(size*8);
    return status;

  }
}


const Ipp32s sorted_ratio[][2] = {
  {1,32},{1,31},{1,30},{1,29},{1,28},{1,27},{1,26},{1,25},{1,24},{1,23},{1,22},{1,21},{1,20},{1,19},{1,18},{1,17},
  {1,16},{2,31},{1,15},{2,29},{1,14},{2,27},{1,13},{2,25},{1,12},{2,23},{1,11},{3,32},{2,21},{3,31},{1,10},{3,29},
  {2,19},{3,28},{1, 9},{3,26},{2,17},{3,25},{1, 8},{4,31},{3,23},{2,15},{3,22},{4,29},{1, 7},{4,27},{3,20},{2,13},
  {3,19},{4,25},{1, 6},{4,23},{3,17},{2,11},{3,16},{4,21},{1, 5},{4,19},{3,14},{2, 9},{3,13},{4,17},{1, 4},{4,15},
  {3,11},{2, 7},{3,10},{4,13},{1, 3},{4,11},{3, 8},{2, 5},{3, 7},{4, 9},{1, 2},{4, 7},{3, 5},{2, 3},{3, 4},{4, 5},
  {1,1},{4,3},{3,2},{2,1},{3,1},{4,1}
};

// sets framerate code and extensions
Status MPEG2VideoEncoderBase::SetFrameRate(Ipp64f new_fr, Ipp32s is_mpeg1)
{
  const Ipp32s srsize = sizeof(sorted_ratio)/sizeof(sorted_ratio[0]);
  const Ipp32s rtsize = sizeof(ratetab)/sizeof(ratetab[0]);
  Ipp32s i, j, besti=0, bestj=0;
  Ipp64f ratio, bestratio = 1.5;
  Ipp32s fr1001 = (Ipp32s)(new_fr*1001+.5);

  frame_rate_code = 5;
  frame_rate_extension_n = frame_rate_extension_d = 0;
  for(j=0;j<rtsize;j++) {
    Ipp32s try1001 = (Ipp32s)(ratetab[j]*1001+.5);
    if(fr1001 == try1001) {
      frame_rate_code = j+1;
      return UMC_OK;
    }
  }
  if(is_mpeg1) { // for mpeg2 will find frame_rate_extention
    return UMC_ERR_FAILED; // requires quite precise values: 0.05% or 0.02 Hz
  }

  if(new_fr < ratetab[0]/sorted_ratio[0][1]*0.7)
    return UMC_ERR_FAILED;

  for(j=0;j<rtsize;j++) {
    ratio = ratetab[j] - new_fr; // just difference here
    if(ratio < 0.0001 && ratio > -0.0001) { // was checked above with bigger range
      frame_rate_code = j+1;
      frame_rate_extension_n = frame_rate_extension_d = 0;
      return UMC_OK;
    }
    if(!is_mpeg1)
    for(i=0;i<srsize+1;i++) { // +1 because we want to analyze last point as well
      if(ratetab[j]*sorted_ratio[i][0] > new_fr*sorted_ratio[i][1] || i==srsize) {
        if(i>0) {
          ratio = ratetab[j]*sorted_ratio[i-1][0] / (new_fr*sorted_ratio[i-1][1]); // up to 1
          if(1/ratio < bestratio) {
            besti = i-1;
            bestj = j;
            bestratio = 1/ratio;
          }
        }
        if(i<srsize) {
          ratio = ratetab[j]*sorted_ratio[i][0] / (new_fr*sorted_ratio[i][1]); // down to 1
          if(ratio < bestratio) {
            besti = i;
            bestj = j;
            bestratio = ratio;
          }
        }
        break;
      }
    }
  }

  if(bestratio > 1.49)
    return UMC_ERR_FAILED;

  frame_rate_code = bestj+1;
  frame_rate_extension_n = sorted_ratio[besti][0]-1;
  frame_rate_extension_d = sorted_ratio[besti][1]-1;
  return UMC_OK;
}

Status MPEG2VideoEncoderBase::SetAspectRatio(Ipp32s hor, Ipp32s ver) // sets aspect code from h/v value
{
  if(hor*3 == ver*4) {
    aspectRatio_code = 2;
    encodeInfo.m_info.videoInfo.m_iSAWidth  = 4;
    encodeInfo.m_info.videoInfo.m_iSAHeight = 3;
  } else if(hor*9 == ver*16) {
    aspectRatio_code = 3;
    encodeInfo.m_info.videoInfo.m_iSAWidth  = 16;
    encodeInfo.m_info.videoInfo.m_iSAHeight = 9;
  } else if(hor*100 == ver*221) {
    aspectRatio_code = 4;
    encodeInfo.m_info.videoInfo.m_iSAWidth  = 221;
    encodeInfo.m_info.videoInfo.m_iSAHeight = 100;
  } else {
    Ipp64f ratio = (Ipp64f)hor/ver;
    Ipp32u parx, pary;
    Ipp32u w = encodeInfo.m_info.videoInfo.m_iWidth;
    Ipp32u h = encodeInfo.m_info.videoInfo.m_iHeight;

    // in video_data
    DARtoPAR(w, h, hor, ver, &parx, &pary);
    DARtoPAR(h, w, parx, pary,
        &encodeInfo.m_info.videoInfo.m_iSAWidth,
        &encodeInfo.m_info.videoInfo.m_iSAHeight); // PAR to DAR

    if (parx == pary) aspectRatio_code = 1;
    else if (ratio > 1.25 && ratio < 1.4) aspectRatio_code = 2;
    else if (ratio > 1.67 && ratio < 1.88) aspectRatio_code = 3;
    else if (ratio > 2.10 && ratio < 2.30) aspectRatio_code = 4;
    else {
      aspectRatio_code = 1;
    }
  }
  return UMC_OK;
}

#define SHIFT_PTR(oldptr, oldbase, newbase) \
  ( (newbase) + ( (Ipp8u*)(oldptr) - (Ipp8u*)(oldbase) ) )

Status MPEG2VideoEncoderBase::LockBuffers(void)
{
  VideoDataBuffer*  vin = DynamicCast<VideoDataBuffer, VideoData> (frames.curenc);
  if(vin)
    vin->Lock(); // input was copied to buffer

  frames.buf_ref[1][0].Lock(); // dst recon for I,P or bwd recon ref for B
#ifdef ME_REF_ORIGINAL
  if(picture_coding_type == MPEG2_B_PICTURE) {
    frames.buf_ref[1][1].Lock(); // bwd orig ref for B
  }
#endif
  // lock ref frames
  if(picture_coding_type != MPEG2_I_PICTURE) {
    frames.buf_ref[0][0].Lock();
#ifdef ME_REF_ORIGINAL
    frames.buf_ref[0][1].Lock();
#endif
  }

  if(picture_coding_type != MPEG2_B_PICTURE) {
      frames.buf_ref[1][0].m_iInvalid = 1; // to be reconstucted
  }
  // can return UMC_OK;

  return UMC_OK;
}

Status MPEG2VideoEncoderBase::UnlockBuffers(void)
{
  Status ret = UMC_OK;
  VideoDataBuffer*  vin = DynamicCast<VideoDataBuffer, VideoData> (frames.curenc);
  if(vin)
    vin->Unlock(); // input was copied to buffer

  frames.buf_ref[1][0].Unlock(); // dst recon for I,P or bwd recon ref for B
#ifdef ME_REF_ORIGINAL
  if(picture_coding_type == MPEG2_B_PICTURE) {
    frames.buf_ref[1][1].Unlock(); // bwd orig ref for B
  }
#endif
  if(picture_coding_type != MPEG2_I_PICTURE) {
    frames.buf_ref[0][0].Unlock();
#ifdef ME_REF_ORIGINAL
    frames.buf_ref[0][1].Unlock();
#endif
  }

  if(picture_coding_type != MPEG2_B_PICTURE) {
    frames.buf_ref[1][0].m_iInvalid = 0; // just reconstucted
  }

  return ret;
}

// VideoData + Memory allocator interface for internal buffers
// Only single alloc and lock
// It is simple and should be used accurately

MPEG2VideoEncoderBase::VideoDataBuffer::VideoDataBuffer()
{
  m_mid = MID_INVALID;
  m_pMemoryAllocator = 0;
  m_bLocked = 0;
}

MPEG2VideoEncoderBase::VideoDataBuffer::~VideoDataBuffer()
{
  if(m_mid != MID_INVALID || m_pMemoryAllocator != 0) {
    if(m_bLocked)
      m_pMemoryAllocator->Unlock(m_mid);
    m_pMemoryAllocator->Free(m_mid);
  }
  m_mid = MID_INVALID;
  m_pMemoryAllocator = 0;
}

Status MPEG2VideoEncoderBase::VideoDataBuffer::SetAllocator(MemoryAllocator * allocator)
{
  if(m_pMemoryAllocator != 0)
    return UMC_ERR_FAILED; // Set only once
  m_pMemoryAllocator = allocator;
  return UMC_OK;
}

Status MPEG2VideoEncoderBase::VideoDataBuffer::Alloc()
{
  Status ret;
  if(m_pMemoryAllocator == 0)
    return UMC_ERR_NULL_PTR;
  if(m_mid != MID_INVALID)
    return UMC_ERR_FAILED; // single alloc
  size_t sz = GetMappingSize();
  if(sz==0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = m_pMemoryAllocator->Alloc(&m_mid, sz, UMC_ALLOC_PERSISTENT, m_iAlignment);
  //if(ret != UMC_OK)
  return ret;
}

Status MPEG2VideoEncoderBase::VideoDataBuffer::Lock()
{
  void * ptr;
  if(m_mid == MID_INVALID || m_pMemoryAllocator == 0)
    return UMC_ERR_NULL_PTR;
  if(m_bLocked)
    return UMC_ERR_FAILED; // single lock

  ptr = m_pMemoryAllocator->Lock(m_mid);
  if(ptr == 0)
    return UMC_ERR_FAILED;
  m_bLocked = 1;
  return SetBufferPointer((Ipp8u*)ptr, GetMappingSize());
}
Status MPEG2VideoEncoderBase::VideoDataBuffer::Unlock()
{
  if(m_mid == MID_INVALID || m_pMemoryAllocator == 0)
    return UMC_ERR_NULL_PTR;
  if(!m_bLocked)
    return UMC_ERR_FAILED; // single lock
  m_bLocked = 0;
  return  m_pMemoryAllocator->Unlock(m_mid);
}

Status MPEG2VideoEncoderBase::VideoDataBuffer::SetMemSize(Ipp32s Ysize, Ipp32s UVsize)
{
  if(!m_pPlaneData || 3 != m_iPlanes)
    return UMC_ERR_NOT_INITIALIZED;
  m_pPlaneData[0].m_iMemSize = Ysize;
  m_pPlaneData[1].m_iMemSize = UVsize;
  m_pPlaneData[2].m_iMemSize = UVsize;
  return UMC_OK;
}

Status MPEG2VideoEncoderBase::CreateFrameBuffer()
{
  Ipp32s i;
  Status ret;
  if(frames.frame_buffer != 0)
    DeleteFrameBuffer();

#ifdef ME_REF_ORIGINAL
  const Ipp32s morig = 1;
#else /* ME_REF_ORIGINAL */
  const Ipp32s morig = 0;
#endif /* ME_REF_ORIGINAL */
  frames.frame_buffer_size = 2*(1+morig) + (encodeInfo.IPDistance - 1) + 1;
  frames.frame_buffer = (VideoDataBuffer*) new VideoDataBuffer [frames.frame_buffer_size];
  if(frames.frame_buffer == 0)
    return UMC_ERR_ALLOC;
  frames.buf_ref[0] = frames.frame_buffer;
  frames.buf_ref[1] = frames.frame_buffer + (1+morig);
  frames.buf_B      = frames.frame_buffer + 2*(1+morig);
  frames.buf_aux    = frames.buf_B + (encodeInfo.IPDistance - 1);

  for(i=0; i<frames.frame_buffer_size; i++) {
    frames.frame_buffer[i].SetAlignment(16);
    ret = frames.frame_buffer[i].Init(encodeInfo.m_info.videoInfo.m_iWidth, encodeInfo.m_info.videoInfo.m_iHeight,
        encodeInfo.m_info.videoInfo.m_colorFormat, 8);
    if(ret != UMC_OK)
      return ret;
    // !!! what alignment for downsampled? 16 or 8 ???
    frames.frame_buffer[i].SetPlanePitch(YFrameHSize, 0);
    frames.frame_buffer[i].SetPlanePitch(UVFrameHSize, 1);
    frames.frame_buffer[i].SetPlanePitch(UVFrameHSize, 2);
    ret = frames.frame_buffer[i].SetMemSize(YFrameSize, UVFrameSize);
    frames.frame_buffer[i].m_fPTSStart = -1;
    frames.frame_buffer[i].SetAllocator(m_pMemoryAllocator);
    ret = frames.frame_buffer[i].Alloc(); // Need to Lock() prior to use
    if(ret != UMC_OK)
      return ret;
    frames.frame_buffer[i].m_iInvalid = 1; // no data in it /// better to Init() func
  }

  return UMC_OK;
}

Status MPEG2VideoEncoderBase::DeleteFrameBuffer()
{
  if(frames.frame_buffer != 0) {
    delete[] frames.frame_buffer;
    frames.frame_buffer = 0;
  }
  return UMC_OK;

}

// no use
Status MPEG2VideoEncoderBase::RotateReferenceFrames()
{
  if(picture_coding_type == MPEG2_P_PICTURE ||
     picture_coding_type == MPEG2_I_PICTURE) {
    VideoDataBuffer* ptr = frames.buf_ref[0];
    frames.buf_ref[0] = frames.buf_ref[1];
    frames.buf_ref[1] = ptr; // rotated
  }
  return UMC_OK;
}

// sets frames.curenc and picture_coding_type
Status MPEG2VideoEncoderBase::SelectPictureType(VideoData *in)
{
  FrameType pictype;
  frames.curenc = in;
  if(!in) { // tail processing
    if(B_count>0) { // encode frames from B buffer
      picture_coding_type = MPEG2_B_PICTURE;
      frames.curenc = &frames.buf_B[B_count-1]; // take buffered B
      if(frames.curenc->m_iInvalid) { //
        return UMC_ERR_NOT_ENOUGH_DATA; // no buffered yet
      }
    }
    else { // if(B_count==0) - should have been P - take last valid from B-buffer
      Ipp32s i = P_distance - 2;
      for( ; i>=0; i--) {
        if(!frames.buf_B[i].m_iInvalid) {
          picture_coding_type = MPEG2_P_PICTURE; // reset B_count to 1 after
          P_distance = i+1; // P is closer than usually
          frames.curenc = &frames.buf_B[i];
          B_count = 0; // will be +1 next time
          return UMC_OK;
        }
      }
      return UMC_ERR_NOT_ENOUGH_DATA; // no more frames
    }
    //return UMC_ERR_FAILED;
  } else if(encodeInfo.lFlags & FLAG_VENC_REORDER) {
    if(B_count > 0) {
      picture_coding_type = MPEG2_B_PICTURE;
      frames.curenc = &frames.buf_B[B_count-1]; // take buffered B
      if(frames.curenc->m_iInvalid)
        return UMC_ERR_NOT_ENOUGH_DATA; // no buffered yet
    } else {
      if (GOP_count == 0)
        picture_coding_type = MPEG2_I_PICTURE;
      else {
        picture_coding_type = MPEG2_P_PICTURE;
        P_distance = encodeInfo.IPDistance; // normal distance
      }
    }
  } else {
      pictype = in->m_frameType; // must match IPDistance if !reorder
    switch(pictype) {
      case B_PICTURE: picture_coding_type = MPEG2_B_PICTURE; break;
      case P_PICTURE: picture_coding_type = MPEG2_P_PICTURE; break;
      case I_PICTURE: picture_coding_type = MPEG2_I_PICTURE; break;
      default: return UMC_ERR_INVALID_PARAMS;
    }
  }
  return UMC_OK;
}

Ipp32s MPEG2VideoEncoderBase::FormatMismatch(VideoData *in)
{
    return ( in == 0 || (in->m_colorFormat != encodeInfo.m_info.videoInfo.m_colorFormat) ||
        ((Ipp32s)in->GetPlanePitch(0) != YFrameHSize) ||
        ((Ipp32s)in->GetPlanePitch(1) != UVFrameHSize) ||
        ((Ipp32s)in->GetPlanePitch(2) != UVFrameHSize) ||
        (picture_coding_type != MPEG2_B_PICTURE && (Ipp32s)in->m_iHeight < 16*MBcountV)); // B is in buffer already
}

Status MPEG2VideoEncoderBase::TakeNextFrame(VideoData *in)
{
  Status ret;

  ret = SelectPictureType(in); // sets frames.curenc and picture_coding_type
  if(ret != UMC_OK)
    return ret;
  RotateReferenceFrames();

#ifdef ME_REF_ORIGINAL
  if(picture_coding_type != MPEG2_B_PICTURE) {
    LoadToBuffer(frames.buf_ref[1]+1, in); // store original frame
    frames.curenc = frames.buf_ref[1]+1;
  }
#endif /* ME_REF_ORIGINAL */

  if( FormatMismatch(frames.curenc) ) {
    LoadToBuffer(frames.buf_aux, in); // store original frame
    frames.curenc = frames.buf_aux;
  }

  return UMC_OK;
}

// in the end of GetFrame()
Status MPEG2VideoEncoderBase::FrameDone(VideoData *in)
{
  if(in == 0 && frames.curenc != 0) // no input, encoded from B buffer
    frames.curenc->m_iInvalid = 1;
  if(picture_coding_type == MPEG2_B_PICTURE) {
    if(in != 0) {
      LoadToBuffer(&frames.buf_B[B_count-1], in); // take new to B buffer
      frames.buf_B[B_count-1].m_fPTSStart = m_PTS;
      frames.buf_B[B_count-1].m_fPTSEnd   = m_DTS;
    }
  }

  // Update GOP counters
  if( ++B_count >= encodeInfo.IPDistance )
    B_count = 0;
  if( ++GOP_count >= encodeInfo.gopSize )
    GOP_count = 0;

  return UMC_OK;
}

Status MPEG2VideoEncoderBase::PrepareFrame()
{
  Ipp32s i;

  Y_src = (Ipp8u*)frames.curenc->GetPlaneDataPtr(0);
  U_src = (Ipp8u*)frames.curenc->GetPlaneDataPtr(1);
  V_src = (Ipp8u*)frames.curenc->GetPlaneDataPtr(2);
  if (Y_src == NULL || U_src == NULL || V_src == NULL) {
    return UMC_ERR_NULL_PTR;
  }
  // can set picture coding params here according to VideoData
  PictureStructure ps = frames.curenc->m_picStructure;
  top_field_first = ((ps == PS_BOTTOM_FIELD_FIRST || encodeInfo.m_info.videoInfo.m_picStructure == PS_PROGRESSIVE) ? 0 : 1);

#ifdef ME_REF_ORIGINAL
  const Ipp32s morig = 1;
#else /* ME_REF_ORIGINAL */
  const Ipp32s morig = 0;
#endif /* ME_REF_ORIGINAL */
  for(i=0; i<2; i++) { // forward/backward
    YRefFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][0].GetPlaneDataPtr(0));
    YRecFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][morig].GetPlaneDataPtr(0));
    URefFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][0].GetPlaneDataPtr(1));
    URecFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][morig].GetPlaneDataPtr(1));
    VRefFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][0].GetPlaneDataPtr(2));
    VRecFrame[0][i] = (Ipp8u*)(frames.buf_ref[i][morig].GetPlaneDataPtr(2));
    YRefFrame[1][i] = YRefFrame[0][i] + YFrameHSize;
    YRecFrame[1][i] = YRecFrame[0][i] + YFrameHSize;
    URefFrame[1][i] = URefFrame[0][i] + UVFrameHSize;
    URecFrame[1][i] = URecFrame[0][i] + UVFrameHSize;
    VRefFrame[1][i] = VRefFrame[0][i] + UVFrameHSize;
    VRecFrame[1][i] = VRecFrame[0][i] + UVFrameHSize;
  }
  return UMC_OK;
}

Status MPEG2VideoEncoderBase::LoadToBuffer(VideoDataBuffer* frame, VideoData *in)
{
  Status ret;
  if(frame == 0 || in == 0)
    return UMC_ERR_NULL_PTR;
  frame->Lock();
  if(frame_loader == NULL) {
    frame_loader = new VideoProcessing;
  }
  ret = frame_loader->GetFrame(in, frame);
  frame->Unlock();
  frame->m_iInvalid = 0;
  return ret;
}

Status MPEG2VideoEncoderBase::FlushBuffers(Ipp32s * size_bytes)
{
    // flush buffer
    Status status = UMC_OK;
    Ipp8u* p = out_pointer;
    Ipp32s fieldCount = 0;
    Ipp32s size, i;

    // move encoded data from all threads to first
    for (i = 0; i < (Ipp32s)encodeInfo.m_iThreads; i++) {
      // align to byte border (not word)
      FLUSH_BITSTREAM(threadSpec[i].bBuf.current_pointer, threadSpec[i].bBuf.bit_offset);
      size = BITPOS(threadSpec[i])>>3;
      fieldCount += threadSpec[i].fieldCount;
      if (size > thread_buffer_size) {
        status = UMC_ERR_NOT_ENOUGH_BUFFER;
        break;
      }
      if (i) {
        memcpy(p, threadSpec[i].bBuf.start_pointer, size);
      }
      p += size;
    }

    if(!curr_frame_dct)
      encodeInfo.altscan_tab[picture_coding_type-MPEG2_I_PICTURE] =
        (fieldCount > 0) ? 1 : 0;

    // align encoded size to 4
    while ((size_t)p & 3) *p++ = 0;
#ifdef MPEG2_DEBUG_CODE
    if (status != UMC_OK) {
      for (i = 0; i < encodeInfo.m_numThreads; i++) {
        size = (Ipp8u*)threadSpec[i].current_pointer - (Ipp8u*)threadSpec[i].start_pointer - (threadSpec[i].bit_offset>>3);
        printf("%d: %d (%d)\n", i, size, thread_buffer_size);
      }
      //exit(1);
    }
#endif

    // summary size
    *size_bytes = (Ipp32s)(p - out_pointer);

    return status;
}

Status MPEG2VideoEncoderBase::ComputeTS(VideoData *in, MediaData *out)
{
  Ipp64f PTS, DTS;
  if( frames.curenc == 0 )
    return UMC_ERR_NOT_ENOUGH_DATA;
  if( encodeInfo.lFlags & FLAG_VENC_REORDER ) { // consequent input PTS
    if(picture_coding_type == MPEG2_B_PICTURE) {
      if(in != 0) { // this frame will be buffered
        PTS = in->m_fPTSStart;
        DTS = in->m_fPTSEnd;
        if(PTS > 0) m_PTS = PTS;
        else        m_PTS += 1./encodeInfo.m_info.fFramerate;
        m_DTS = -1;
      }
      PTS = out->m_fPTSStart = frames.curenc->m_fPTSStart;
      DTS = out->m_fPTSEnd   = frames.curenc->m_fPTSEnd;
      //frames.curenc->SetTime(m_PTS, m_DTS); // moved to be set after copy to buffer
    } else { // I or P
      PTS = frames.curenc->m_fPTSStart;
      DTS = frames.curenc->m_fPTSEnd;
      if(PTS > 0) m_PTS = PTS;
      else {
        if(m_PTS < 0)
          m_PTS = 0;
        m_PTS += 1./encodeInfo.m_info.fFramerate;
      }
      if(DTS > 0) m_DTS = DTS;
      else        m_DTS = m_PTS - P_distance/encodeInfo.m_info.fFramerate;
      out->m_fPTSStart = m_PTS;
      out->m_fPTSEnd   = m_DTS;
    }
  } else { // consequent input DTS
    PTS = frames.curenc->m_fPTSStart;
    DTS = frames.curenc->m_fPTSEnd;
    if(PTS > 0) m_DTS = DTS;
    else {
      if(m_DTS < 0)
        m_DTS = 0;
      else
        m_DTS += 1./encodeInfo.m_info.fFramerate;
    }
    if(PTS > 0) m_PTS = PTS;
    else {
      if(picture_coding_type == MPEG2_B_PICTURE)
        m_PTS = m_DTS;
      else
        m_PTS = m_DTS + P_distance/encodeInfo.m_info.fFramerate;
    }
    out->m_fPTSStart = m_PTS;
    out->m_fPTSEnd   = m_DTS;
  }

  return UMC_OK;
}
#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
