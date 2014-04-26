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

#include "vm_sys_info.h"
#include "vm_time.h"
#include "umc_video_data.h"
#include "umc_video_processing.h"
#include "umc_memory_allocator.h"
#include "umc_mpeg2_dec_base.h"
#include "umc_mpeg2_video_decoder.h"
#include "umc_mpeg2_dec_tbl.h"

using namespace UMC;
#define OFFSET_PTR(p, n) (((Ipp8u*)(p) - (Ipp8u*)(0)) & ((n)-1))
#define ALIGN_PTR(p, n) ((Ipp8u*)(p) - OFFSET_PTR(p, n))


#define ALIGN_VALUE  16

static IppStatus mp2_HuffmanTableInitAlloc(Ipp32s *tbl, Ipp32s bits_table0, mp2_VLCTable *vlc)
{
  Ipp32s *ptbl;
  Ipp16s bad_value = 0;
  Ipp32s max_bits;
  Ipp32s num_tbl;
  Ipp32s i, j, k, m, n;
  Ipp32s bits, code, value;
  Ipp32s bits0, bits1;
  Ipp32s min_value, max_value, spec_value;
  Ipp32s min_code0, min_code1;
  Ipp32s max_code0, max_code1;
  Ipp32s prefix_code1 = -1;
  Ipp32s bits_table1 = 0;
  Ipp32s *buffer = NULL;
  Ipp32s *codes;
  Ipp32s *cbits;
  Ipp32s *values;
  Ipp16s *table0 = NULL;
  Ipp16s *table1 = NULL;

  /* get number of entries (n) */
  max_bits = *tbl; tbl++;
  num_tbl  = *tbl; tbl++;
  for (i = 0; i < num_tbl; i++) {
    tbl++;
  }
  n = 0;
  ptbl = tbl;
  for (bits = 1; bits <= max_bits; bits++) {
    m = *ptbl;
    if (m < 0) break;
    n += m;
    ptbl += 2*m + 1;
  }

  /* alloc internal table */
  buffer = new Ipp32s[3*n];
  if (!buffer) return ippStsErr;
  codes = buffer;
  cbits = buffer + n;
  values = buffer + 2*n;

  /* read VLC to internal table */
  min_value = 0x7fffffff;
  max_value = 0;
  spec_value = 0;
  ptbl = tbl;
  k = 0;
  for (bits = 1; bits <= max_bits; bits++) {
    if (*ptbl < 0) break;
    m = *ptbl++;
    for (i = 0; i < m; i++) {
      code = *ptbl; ptbl++;
      value = *ptbl; ptbl++;
      code &= ((1 << bits) - 1);
      if (value < min_value) min_value = value;
      if (value > max_value) {
        if (!spec_value && value >= 0xffff) {
          spec_value = value;
        } else {
          max_value = value;
        }
      }
#ifdef M2D_DEBUG
      if (vlc_print) {
        for (j = 0; j < bits; j++) {
          printf("%c", (code & (1 << (bits - 1 - j))) ? '1' : '0');
        }
        for (j = bits; j < max_bits; j++) {
          printf("%c", ' ');
        }
        printf("  -> %d", value);
        printf("\n");
      }
#endif
      codes[k] = code << (30 - bits);
      cbits[k] = bits;
      values[k] = value;
      k++;
    }
  }
#ifdef M2D_DEBUG
  if (vlc_print) {
    printf("Values: [%d, %d]", min_value, max_value);
    if (spec_value) printf("   special value = %x\n", spec_value);
    printf("\n");
    printf("---------------------------\n");
  }
#endif

  if (!bits_table0) {
    bits_table0 = max_bits;
    bits_table1 = 0;
    vlc->threshold_table0 = 0;
  }
  bits0 = bits_table0;
  //for (bits0 = 1; bits0 < max_bits; bits0++)
  if (bits0 > 0 && bits0 < max_bits) {
    min_code0 = min_code1 = 0x7fffffff;
    max_code0 = max_code1 = 0;
    for (i = 0; i < n; i++) {
      code = codes[i];
      bits = cbits[i];
      if (bits <= bits0) {
        if (code > max_code0) max_code0 = code;
        if (code < min_code0) min_code0 = code;
      } else {
        if (code > max_code1) max_code1 = code;
        if (code < min_code1) min_code1 = code;
      }
    }
#ifdef M2D_DEBUG
    if (vlc_print) {
      printf("bits0 = %2d: table0 [0x%3x,0x%3x], table1 [0x%3x,0x%3x]",
        bits0,
        max_code0 >> (30 - max_bits),
        min_code0 >> (30 - max_bits),
        max_code1 >> (30 - max_bits),
        min_code1 >> (30 - max_bits));
    }
#endif
    if ((max_code0 < min_code1) || (max_code1 < min_code0)) {
      for (j = 0; j < 29; j++) {
        if ((min_code1 ^ max_code1) & (1 << (29 - j))) break;
      }
      bits1 = max_bits - j;
      if (bits0 == bits_table0) {
        bits_table1 = bits1;
        prefix_code1 = min_code1 >> (30 - bits0);
        vlc->threshold_table0 = min_code0 >> (30 - max_bits);
      }
#ifdef M2D_DEBUG
      if (vlc_print) {
        printf(" # Ok: Tables: %2d + %2d = %d bits", bits0, bits1, bits0 + bits1);
      }
#endif
    }
  }

  if (bits_table0 > 0 && bits_table0 < max_bits && !bits_table1) {
    if (buffer) delete[] buffer;
    return ippStsErr;
  }

  bad_value = (bad_value << 8) | VLC_BAD;

  table0 = (Ipp16s*)ippMalloc((int)(1 << bits_table0)*sizeof(Ipp16s));
  if(!table0) {
    if (buffer) delete[] buffer;
    return ippStsErr;
  }
  ippsSet_16s(bad_value, table0, 1 << bits_table0);
  if (bits_table1) {
    table1 = (Ipp16s*)ippMalloc((int)(1 << bits_table1)*sizeof(Ipp16s));
    if (!table1) {
        if (buffer) delete[] buffer;
        if (table0) ippFree(table0);
        return ippStsErr;
    }
    ippsSet_16s(bad_value, table1, 1 << bits_table1);
  }
  for (i = 0; i < n; i++) {
    code = codes[i];
    bits = cbits[i];
    value = values[i];
    if (bits <= bits_table0) {
      code = code >> (30 - bits_table0);
      for (j = 0; j < (1 << (bits_table0 - bits)); j++) {
        table0[code + j] = (Ipp16s)((value << 8) | bits);
      }
    } else {
      code = code >> (30 - max_bits);
      code = code & ((1 << bits_table1) - 1);
      for (j = 0; j < (1 << (max_bits - bits)); j++) {
        table1[code + j] = (Ipp16s)((value << 8) | bits);
      }
    }
  }

  if (bits_table1) { // fill VLC_NEXTTABLE
    if (prefix_code1 == -1) {
      if (buffer) delete[] buffer;
      return ippStsErr;
    }
    bad_value = (bad_value &~ 255) | VLC_NEXTTABLE;
    for (j = 0; j < (1 << ((bits_table0 - (max_bits - bits_table1)))); j++) {
      table0[prefix_code1 + j] = bad_value;
    }
  }

  vlc->max_bits = max_bits;
  vlc->bits_table0 = bits_table0;
  vlc->bits_table1 = bits_table1;
  vlc->table0 = table0;
  vlc->table1 = table1;

  if (buffer) delete[] buffer;
  return ippStsNoErr;
}

static void mp2_HuffmanTableFree(mp2_VLCTable *vlc) {
  if (vlc->table0) {
    ippFree(vlc->table0);
    vlc->table0 = NULL;
  }
  if (vlc->table1) {
    ippFree(vlc->table1);
    vlc->table1 = NULL;
  }
}

bool MPEG2VideoDecoderBase::InitTables()
{
  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(MBAdressing, 5, &vlcMBAdressing))
    return false;

  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(IMBType, 0, &vlcMBType[0]))
    return false;

  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(PMBType, 0, &vlcMBType[1]))
    return false;

  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(BMBType, 0, &vlcMBType[2]))
    return false;

  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(MBPattern, 5, &vlcMBPattern))
    return false;

  if (ippStsNoErr != mp2_HuffmanTableInitAlloc(MotionVector, 5, &vlcMotionVector))
    return false;

  return true;
}


bool MPEG2VideoDecoderBase::DeleteTables()
{
  // release tools
    if(Video)
    {
        ippFree(Video[0]);
        Video[0] = NULL;
        ippFree(Video);
        Video = NULL;
    }

#ifdef SLICE_THREADING_2
    if (m_pSliceInfo) {
      ippsFree(m_pSliceInfo);
      m_pSliceInfo = NULL;
    }
#endif

    if(frame_buffer.ptr_context_data)
    {
        //ippsFree(frame_buffer.ptr_context_data);
      m_pMemoryAllocator->Free(frame_buffer.mid_context_data);
      frame_buffer.ptr_context_data = NULL;
    }

    // release tables
    mp2_HuffmanTableFree(&vlcMBAdressing);
    mp2_HuffmanTableFree(&vlcMBType[0]);
    mp2_HuffmanTableFree(&vlcMBType[1]);
    mp2_HuffmanTableFree(&vlcMBType[2]);
    mp2_HuffmanTableFree(&vlcMBPattern);
    mp2_HuffmanTableFree(&vlcMotionVector);

    return UMC_OK;
}

Ipp32u VM_THREAD_CALLCONVENTION MPEG2VideoDecoderBase::ThreadWorkingRoutine(void *lpv)
{
    THREAD_ID *             lpThreadId;
    MPEG2VideoDecoderBase * lpOwner;

    // check error(s)
    if (NULL == lpv)
        return 0xbaadf00d;

    lpThreadId  = reinterpret_cast<THREAD_ID *> (lpv);
    lpOwner     = reinterpret_cast<MPEG2VideoDecoderBase *> (lpThreadId->m_lpOwner);

    //_alloca(lpThreadId->m_nNumber*1024*2);
    // wait for start
    vm_event_wait(lpOwner->m_lpStartEvent + lpThreadId->m_nNumber);
    lpThreadId->m_status = UMC_OK;

    while (VM_TIMEOUT == vm_event_timed_wait(&(lpOwner->m_lpQuit[lpThreadId->m_nNumber]), 0))
    {
        lpThreadId->m_status = lpOwner->DecodeSlices(lpThreadId->m_nNumber);
        // set finish
        vm_event_signal(lpOwner->m_lpStopEvent + lpThreadId->m_nNumber);

        //vm_time_sleep(0);

        // wait for start
        vm_event_wait(lpOwner->m_lpStartEvent + lpThreadId->m_nNumber);
    }

    return lpThreadId->m_nNumber;

}


Status MPEG2VideoDecoderBase::ThreadingSetup(Ipp32s maxThreads)
{
    Ipp32s i;
    Ipp32s aligned_size;


    if(maxThreads == 0)
        m_nNumberOfThreads = vm_sys_info_get_cpu_num();
    else
        m_nNumberOfThreads = maxThreads;

    if (0 >= m_nNumberOfThreads)
        m_nNumberOfThreads = 1;
    else if (8 < m_nNumberOfThreads)
        m_nNumberOfThreads = 8;

    Video = (Mpeg2VideoContext**)ippMalloc(m_nNumberOfThreads*sizeof(Mpeg2VideoContext*));
    if(!Video) return UMC_ERR_ALLOC;

    aligned_size = (Ipp32s)((sizeof(Mpeg2VideoContext) + 15) &~ 15);

    Video[0] = (Mpeg2VideoContext*)ippMalloc(m_nNumberOfThreads*aligned_size);
    if(!Video[0]) return UMC_ERR_ALLOC;
    memset(Video[0], 0, m_nNumberOfThreads*aligned_size);
    for(i = 0; i < m_nNumberOfThreads; i++)
    {
      if (i)
        Video[i] = (Mpeg2VideoContext*)((Ipp8u *)Video[0] + i*aligned_size);

      // Intra&inter spec
      memset(&Video[i]->decodeIntraSpec, 0, sizeof(DecodeIntraSpec_MPEG2));
      memset(&Video[i]->decodeInterSpec, 0, sizeof(DecodeInterSpec_MPEG2));
      ownDecodeInterInit_MPEG2(NULL, IPPVC_MPEG1_STREAM, &Video[i]->decodeInterSpec);
      Video[i]->decodeInterSpecChroma = Video[i]->decodeInterSpec;
      Video[i]->decodeInterSpec.idxLastNonZero = 63;
      Video[i]->decodeIntraSpec.intraVLCFormat = PictureHeader.intra_vlc_format;
      Video[i]->decodeIntraSpec.intraShiftDC = PictureHeader.curr_intra_dc_multi;
    }

    // create threading tools
    if (1 < m_nNumberOfThreads)
    {
        // create exit event(s)
        m_lpQuit = new vm_event[m_nNumberOfThreads];
        if (NULL == m_lpQuit)
            return UMC_ERR_ALLOC;

        for (i = 1; i < m_nNumberOfThreads; i++)
        {
            vm_event_set_invalid(m_lpQuit + i);
            if (VM_OK != vm_event_init(m_lpQuit + i, 0, 0))
                return UMC_ERR_INIT;
        }

        // create start event(s)
        m_lpStartEvent = new vm_event[m_nNumberOfThreads];
        if (NULL == m_lpStartEvent)
            return UMC_ERR_ALLOC;

        for (i = 1; i < m_nNumberOfThreads; i++)
        {
            vm_event_set_invalid(m_lpStartEvent + i);
            if (VM_OK != vm_event_init(m_lpStartEvent + i, 0, 0))
                return UMC_ERR_INIT;
        }

        // create stop event(s)
        m_lpStopEvent = new vm_event[m_nNumberOfThreads];
        if (NULL == m_lpStopEvent)
            return UMC_ERR_ALLOC;
        for (i = 1;i < m_nNumberOfThreads;i += 1)
        {
            vm_event_set_invalid(m_lpStopEvent + i);
            if (VM_OK != vm_event_init(m_lpStopEvent + i, 0, 0))
                return UMC_ERR_INIT;
        }

        // allocate thread ID(s)
        m_lpThreadsID = new THREAD_ID[m_nNumberOfThreads];
        if (NULL == m_lpThreadsID)
            return UMC_ERR_ALLOC;

        // starting thread(s)
        m_lpThreads = new vm_thread[m_nNumberOfThreads];
        if (NULL == m_lpThreads)
            return UMC_ERR_ALLOC;

        for (i = 1;i < m_nNumberOfThreads;i += 1)
        {
            vm_thread_set_invalid(m_lpThreads + i);
            m_lpThreadsID[i].m_nNumber = i;
            m_lpThreadsID[i].m_lpOwner = this;
            if (0 == vm_thread_create(m_lpThreads + i, ThreadWorkingRoutine, m_lpThreadsID + i))
                return UMC_ERR_INIT;

#ifdef SLICE_THREADING_2
//  vm_thread_set_priority(m_lpThreads + i, VM_THREAD_PRIORITY_LOWEST);
#endif
        }
    }

    m_nNumberOfAllocatedThreads = m_nNumberOfThreads;

    return UMC_OK;
}


Status MPEG2VideoDecoderBase::Init(BaseCodecParams *pInit)
{
    MediaData *data;
    Status ret;

    VideoDecoderParams *init = DynamicCast<VideoDecoderParams, BaseCodecParams>(pInit);
    if(!init)
        return UMC_ERR_NULL_PTR;
    if(!init->m_pPostProcessor)
        return UMC_ERR_NULL_PTR;

    data = init->m_pData;
    if (data != 0 && data->GetDataSize() == 0)
      data = 0;

    // checks or create memory allocator;
    ret = BaseCodec::Init(pInit);
    if(ret != UMC_OK)
      return ret;

    if((init->m_iFlags & FLAG_VDEC_4BYTE_ACCESS) != 0)
      return UMC_ERR_UNSUPPORTED;

    Reset();

    m_ClipInfo = init->m_info;
    m_lFlags = init->m_iFlags;
    m_pPostProcessor = init->m_pPostProcessor;

    // creates thread's structures (Video[])
    if(UMC_OK != ThreadingSetup(init->m_iThreads))
        return UMC_ERR_ALLOC;

    if (data != 0) 
    {
        PrepareBuffer(data);

        // get sequence header start code
        // search while not found or EOD
        if(UMC_OK != FindSequenceHeader(Video[0]))
            return (UMC_ERR_SYNC);

        if(UMC_OK != DecodeSequenceHeader(Video[0]))
            return (UMC_ERR_INVALID_STREAM);

        if(m_ClipInfo.videoInfo.m_colorFormat != YUV420 && m_ClipInfo.videoInfo.m_colorFormat != YUV422)
            return UMC_ERR_UNSUPPORTED;

        FlushBuffer(data, false);
    }
    else
    {
        // get stream parameters from sVideoInfo
        if(UMC_OK != DecodeSequenceHeader( 0 ))
            return (UMC_ERR_INVALID_PARAMS);
    }

    m_ccCurrData.SetBufferPointer(NULL, 0);
    m_pCCData = new SampleBuffer;
    if(m_pCCData)
    {
        Status res = UMC_OK;
        MediaBufferParams initPar;
        initPar.m_prefInputBufferSize = initPar.m_prefOutputBufferSize = 1024;
        initPar.m_numberOfFrames = 20;
        initPar.m_pMemoryAllocator = m_pMemoryAllocator;

        res = m_pCCData->Init(&initPar);
        if(res != UMC_OK)
        {
            m_pCCData->Close();
            delete m_pCCData;
            m_pCCData = NULL;
        }
    }

    if (false == InitTables())
        return UMC_ERR_INIT;

    sequenceHeader.frame_count    = 0;
    sequenceHeader.stream_time    = 0;
    sequenceHeader.is_skipped_b   = 0;

    sequenceHeader.num_of_skipped   = 0;
    sequenceHeader.bdied            = 0;

    frame_buffer.prev_index =  0;
    frame_buffer.curr_index =  0;
    frame_buffer.next_index =  1;
    frame_buffer.retrieve   = -1;
    frame_buffer.ind_his_p = 0;
    frame_buffer.ind_his_b = 1;
    frame_buffer.ind_his_free = 2;
    frame_buffer.frame_p_c_n[0].frame_time = -1;
    frame_buffer.frame_p_c_n[1].frame_time = -1;
    frame_buffer.frame_p_c_n[0].duration = 0;
    frame_buffer.frame_p_c_n[1].duration = 0;

    sequenceHeader.stream_time_temporal_reference = -1;
    sequenceHeader.stream_time = 0;

    m_dispSize.width = m_dispSize.height = 0;

    //if (data != 0) {
    //  PrepareBuffer(data);

    //  if(UMC_OK > DecodeBegin(data->GetTime(), &init->info)) {
    //      return UMC_ERR_NULL_PTR;
    //  }

    //  FlushBuffer(data, false);
    //}

    return UMC_OK;
}


Status MPEG2VideoDecoderBase::FlushBuffer(MediaData* data, bool /*threaded*/)
{
    Mpeg2VideoContext *video = Video[0]; // it decodes tail of frame
    Ipp8u* plast = GET_BYTE_PTR(video->bs);
    Ipp8u* pend = GET_END_PTR(video->bs);
    if (plast > pend)
        plast = pend;
    return data->MoveDataPointer((Ipp32s)(plast - (Ipp8u*)data->GetDataPointer()));
}

Status MPEG2VideoDecoderBase::PrepareBuffer(MediaData* data)
{
    Mpeg2VideoContext *video = Video[0];
    Ipp8u  *ptr;
    Ipp32s size;

    if (data == 0) {
      return UMC_OK;
    }
    ptr = (Ipp8u*)data->GetDataPointer();
    size = (Ipp32s)data->GetDataSize();

    INIT_BITSTREAM(video->bs, ptr, ptr + size);

    return UMC_OK;
}

bool MPEG2VideoDecoderBase::IsPictureToSkip()
{
  if (MPEG2_I_PICTURE > PictureHeader.picture_coding_type ||
      MPEG2_B_PICTURE < PictureHeader.picture_coding_type)
  {
    if(!(PictureHeader.d_picture &&
         m_ClipInfo.streamType == MPEG1_VIDEO))
    {
      return true;
    }
  }

  if(frame_buffer.field_buffer_index == 0) // not for second field
  {
    sequenceHeader.stream_time_temporal_reference++;
    if(PictureHeader.picture_coding_type == MPEG2_I_PICTURE)
    {
      sequenceHeader.bdied = 0;
      if(sequenceHeader.first_i_occure)
        sequenceHeader.first_p_occure = 1;
      sequenceHeader.first_i_occure = 1;
    }
    else if(!sequenceHeader.first_i_occure) {
      return true;
    }
    else if(PictureHeader.picture_coding_type == MPEG2_P_PICTURE) {
      sequenceHeader.bdied = 0;
      sequenceHeader.first_p_occure = 1;
    }
    else if(PictureHeader.picture_coding_type == MPEG2_B_PICTURE) {
      if(!sequenceHeader.first_p_occure &&
        (!sequenceHeader.closed_gop || sequenceHeader.broken_link )) {
          return true;
        }
    }
  }

  if(PictureHeader.picture_coding_type == 3 && sequenceHeader.is_skipped_b ||
    !sequenceHeader.first_p_occure ||
    ((PictureHeader.picture_coding_type == 3) && m_bNoBframes) ||
    ((PictureHeader.picture_coding_type == 2) && m_bNoPframes))
  {
    if(sequenceHeader.bdied && sequenceHeader.first_p_occure &&
      !(((PictureHeader.picture_coding_type == 3) && m_bNoBframes) ||
      ((PictureHeader.picture_coding_type == 2) && m_bNoPframes)))
    {
      sequenceHeader.bdied = 0;
    } else {
      if(sequenceHeader.is_skipped_b > 0)
        sequenceHeader.is_skipped_b--;

      if(sequenceHeader.first_p_occure)
      {
        sequenceHeader.num_of_skipped++;
        sequenceHeader.bdied = 1;
        return true;
      }
    }
  }

  return false;
}

Status MPEG2VideoDecoderBase::GetFrame(MediaData* input, MediaData* output)
{
  Status umcRes;

  if ((NULL == input) || (NULL == input->GetDataPointer()))
  {
    Ipp64f dTime;
    if (output == 0) {
      return UMC_ERR_NULL_PTR;
    }
    if (m_decodedFrameNum <= 0) {
      return UMC_ERR_NOT_ENOUGH_DATA;
    }
    CalculateFrameTime(frame_buffer.frame_p_c_n[frame_buffer.next_index].frame_time, &dTime);
    m_decodedFrameNum--;
    frame_buffer.retrieve = frame_buffer.next_index;
    return SaveFrame(frame_buffer.next_index,
                     dTime,
                     output);
  }


  if (NULL == output) {
    return UMC_ERR_NULL_PTR;
  }

  PrepareBuffer(input);

  umcRes = DecodeFrame(Video[0], input->m_fPTSStart, output);

#ifdef UMC_STREAM_ANALYZER
  if(pack_w.is_analyzer)
  {
      Ipp64f start, end;
      output->GetTime(start, end);
      pack_w.PicAnalyzerData->PTS = start;
  }
#endif
  FlushBuffer(input, m_ClipInfo.streamType == MPEG2_VIDEO); // ?

  return umcRes;
}

Status MPEG2VideoDecoderBase::DecodeFrame(Mpeg2VideoContext  *video,
                                          Ipp64f           currentTime,
                                          MediaData        *output)
{
  Status umcRes = UMC_OK;
  Ipp32u code;
#ifdef UMC_STREAM_ANALYZER
   Ipp32s bit_ptr;
#endif

  if (!sequenceHeader.is_decoded) {
    if(UMC_OK != FindSequenceHeader(Video[0]))
      return (UMC_ERR_NOT_ENOUGH_DATA);
    umcRes = DecodeSequenceHeader(Video[0]);
    if(UMC_OK != umcRes)
      return umcRes;
  }

  SHOW_BITS(video->bs, 32, code);
  do {
    GET_START_CODE(video->bs, code);
    // some headers are possible here
    if(code == (Ipp32u)UMC_ERR_NOT_ENOUGH_DATA)
    {
      if(GET_OFFSET(video->bs) > 0) // some data was decoded
        return UMC_ERR_NOT_ENOUGH_DATA;
      else                          // no start codes found
        return UMC_ERR_SYNC;
    }
    if(code == SEQUENCE_END_CODE)
      continue;
    if(code != PICTURE_START_CODE) {
      if (DecodeHeader(code, video) == UMC_ERR_NOT_ENOUGH_DATA) {
        return (UMC_ERR_NOT_ENOUGH_DATA);
      }
    }
  } while (code != PICTURE_START_CODE);

  umcRes = DecodeHeader(PICTURE_START_CODE, video);
  if(umcRes != UMC_OK)
    return (umcRes);

  //  We don't advance PTS for input data - we don't want to guess it
  //  without decoding.
  Ipp64f dTime;
  CalculateFrameTime(currentTime, &dTime);

  if (IsPictureToSkip()) {
    return UMC_ERR_NOT_ENOUGH_DATA;
  }
  if(frame_buffer.field_buffer_index == 0) // not for second field
    frame_buffer.frame_p_c_n[frame_buffer.curr_index].is_invalid = 0;

#ifdef UMC_STREAM_ANALYZER
  if(pack_w.is_analyzer)
  {
      bit_ptr = GET_BIT_OFFSET(video->bs);
  }
#endif
  umcRes = DecodePicture();

#ifdef UMC_STREAM_ANALYZER
  if(pack_w.is_analyzer)
  {
      pack_w.PicAnalyzerData->dwNumBitsPic = (Ipp32u)(GET_BIT_OFFSET(video->bs) - bit_ptr);
  }
#endif
  if (umcRes == UMC_WRN_INVALID_STREAM) {
    frame_buffer.frame_p_c_n[frame_buffer.curr_index].is_invalid = 1;
    umcRes = UMC_OK;
  }
  if (UMC_OK != umcRes)
  { // Error in DecodePicture()
    return umcRes;
  }


  if (PictureHeader.picture_structure != FRAME_PICTURE &&
      frame_buffer.field_buffer_index == 1)
  {
    return UMC_ERR_NOT_ENOUGH_DATA;
  }
  m_decodedFrameNum++; // full frame decoded

  if (frame_buffer.retrieve < 0) {
    return UMC_ERR_NOT_ENOUGH_DATA;
  }
  m_decodedFrameNum--; // frame retrieved

  return SaveFrame(frame_buffer.retrieve, dTime, output);
}

// Close decoding
Status MPEG2VideoDecoderBase::Close()
{
    Ipp32s i;

    DeleteTables();

    m_dPlaybackRate = 1;

    if (m_lpThreads)
    {
        for (i = 1; i < m_nNumberOfAllocatedThreads;i += 1)
            vm_event_signal(m_lpQuit + i);

        for (i = 1; i < m_nNumberOfAllocatedThreads;i += 1)
            vm_event_signal(m_lpStartEvent + i);

        for (i = 1; i < m_nNumberOfAllocatedThreads;i += 1)
        {
            if (vm_thread_is_valid(m_lpThreads + i))
                vm_thread_wait(m_lpThreads + i);
        }
    }

    // delete all threading tools
    if (m_lpQuit)
    {
        for (i = 1;i < m_nNumberOfAllocatedThreads;i += 1)
        {
            if (vm_event_is_valid(m_lpQuit + i))
            {
                vm_event_destroy(m_lpQuit + i);
                vm_event_set_invalid(m_lpQuit + i);
            }
        }

        delete [] m_lpQuit;
        m_lpQuit = NULL;
    }

    if (m_lpThreads)
    {
        for (i = 1;i < m_nNumberOfAllocatedThreads;i += 1)
            vm_thread_set_invalid(m_lpThreads + i);

        delete [] m_lpThreads;
        m_lpThreads = NULL;
    }

    if (m_lpStartEvent)
    {
        for (i = 1;i < m_nNumberOfAllocatedThreads;i += 1)
        {
            if (vm_event_is_valid(m_lpStartEvent + i))
            {
                vm_event_destroy(m_lpStartEvent + i);
                vm_event_set_invalid(m_lpStartEvent + i);
            }
        }

        delete [] m_lpStartEvent;
        m_lpStartEvent = NULL;
    }

    if (m_lpStopEvent)
    {
        for (i = 1;i < m_nNumberOfAllocatedThreads;i += 1)
        {
            if (vm_event_is_valid(m_lpStopEvent + i))
            {
                vm_event_destroy(m_lpStopEvent + i);
                vm_event_set_invalid(m_lpStopEvent + i);
            }
        }
        delete [] m_lpStopEvent;
        m_lpStopEvent = NULL;
    }

    if (m_lpThreadsID)
    {
        delete [] m_lpThreadsID;
        m_lpThreadsID = NULL;
    }
    if (m_pCCData)
    {
        delete m_pCCData;
        m_pCCData = NULL;
    }

    return VideoDecoder::Close();
}


MPEG2VideoDecoderBase::MPEG2VideoDecoderBase()
{
    Video                          = NULL;
    m_decodedFrameNum              = 0;
    m_lFlags                       = 0;
    m_ClipInfo.fFramerate          = 0;
    m_ClipInfo.videoInfo.m_iWidth  = 100;
    m_ClipInfo.videoInfo.m_iHeight = 100;

    memset(&sequenceHeader, 0, sizeof(sequenceHeader));
    memset(&PictureHeader, 0, sizeof(PictureHeader));
    memset(&frame_buffer, 0, sizeof(frame_buffer));

    m_ClipInfo.streamType = UNDEF_VIDEO;
    sequenceHeader.profile = MPEG2_PROFILE_MAIN;
    sequenceHeader.level = MPEG2_LEVEL_MAIN;
    frame_buffer.allocated_mb_width = 0;
    frame_buffer.allocated_mb_height = 0;
    frame_buffer.allocated_cformat = NONE;
    frame_buffer.allocated_size = 0;
    frame_buffer.mid_context_data = MID_INVALID;
    frame_buffer.ptr_context_data = NULL; // internal buffer not allocated

    m_nNumberOfThreads  = 1;
    m_nNumberOfAllocatedThreads  = 1;
    m_lpQuit            = NULL;
    m_lpThreads         = NULL;
    m_lpStartEvent      = NULL;
    m_lpStopEvent       = NULL;
    m_lpThreadsID       = NULL;
    //m_pStartCodesData   = NULL;
    m_pCCData           = NULL;

    m_dPlaybackRate     = 1;

    vlcMBAdressing.table0 = vlcMBAdressing.table1 = NULL;
    vlcMBType[0].table0 = vlcMBType[0].table1 = NULL;
    vlcMBType[1].table0 = vlcMBType[1].table1 = NULL;
    vlcMBType[2].table0 = vlcMBType[2].table1 = NULL;
    vlcMBPattern.table0 = vlcMBPattern.table1 = NULL;
    vlcMotionVector.table0 = vlcMotionVector.table1 = NULL;
#if defined(SLICE_THREADING) || defined(SLICE_THREADING_2)
    vm_mutex_set_invalid(&m_Locker);
    vm_mutex_init(&m_Locker);
#endif

}

MPEG2VideoDecoderBase::~MPEG2VideoDecoderBase()
{
    Close();
}


Status    MPEG2VideoDecoderBase::ResetSkipCount()
{
    AdjustSpeed(-10);
    return UMC_OK;
}


Status    MPEG2VideoDecoderBase::SkipVideoFrame(Ipp32s count)
{
    AdjustSpeed(count);
    return UMC_OK;
}


bool MPEG2VideoDecoderBase::AdjustSpeed(Ipp32s nframe)
{
    if (nframe>0) IncreaseSpeedN(nframe);
    if (nframe<0) DecreaseSpeed(-(Ipp64f)nframe/
                                (Ipp64f)m_ClipInfo.fFramerate);
    return true;
}

Ipp32s MPEG2VideoDecoderBase::IncreaseSpeedN (Ipp32s numoffr)
{
    sequenceHeader.is_skipped_b+=numoffr;

    return numoffr;
}

Ipp32s MPEG2VideoDecoderBase::DecreaseSpeed(Ipp64f delta)
{
    Ipp32s num_skipp_frames = (Ipp32s)(delta * m_ClipInfo.fFramerate);

    if(num_skipp_frames <= 3)
        num_skipp_frames = 1;

    if(!num_skipp_frames)
        return 0;

    if(sequenceHeader.is_skipped_b > 0)
        sequenceHeader.is_skipped_b-=num_skipp_frames;

    if(sequenceHeader.is_skipped_b < 0)
        sequenceHeader.is_skipped_b = 0;
    return num_skipp_frames;

}


Ipp32s MPEG2VideoDecoderBase::IncreaseSpeed(Ipp64f delta)
{
//    Mpeg2VideoContext* video = &pContext->Video[0];
    Ipp32s num_skipp_frames = (Ipp32s)(delta * m_ClipInfo.fFramerate);

    if(num_skipp_frames <= 3)
        num_skipp_frames = 1;

    if(num_skipp_frames > 5)
        num_skipp_frames = 5;

    sequenceHeader.is_skipped_b+=num_skipp_frames;

    return num_skipp_frames;

}
bool MPEG2VideoDecoderBase::AdjustSpeed(Ipp64f delta)
{
    if (delta>0) IncreaseSpeed(delta);
    if (delta<0) DecreaseSpeed(-delta);
    return true;
}


Ipp32u MPEG2VideoDecoderBase::GetNumOfSkippedFrames()
{
    return sequenceHeader.num_of_skipped;
}

Status MPEG2VideoDecoderBase::Reset()
{
    sequenceHeader.first_i_occure  = 0;
    sequenceHeader.first_p_occure  = 0;
    sequenceHeader.bdied           = 0;
    sequenceHeader.broken_link = 0;
    sequenceHeader.closed_gop  = 0;
    sequenceHeader.gop_picture = 0;
    sequenceHeader.gop_second  = 0;
    sequenceHeader.stream_time = 0; //-sequenceHeader.delta_frame_time;
    sequenceHeader.frame_rate_extension_d = 0;
    sequenceHeader.frame_rate_extension_n = 0;

    PictureHeader.intra_vlc_format    = 0;
    PictureHeader.curr_intra_dc_multi = intra_dc_multi[0];

    m_decodedFrameNum          = 0;
    m_bNoBframes               = false;
    m_bNoPframes               = false;

    frame_buffer.prev_index = 0;
    frame_buffer.curr_index = 0;
    frame_buffer.next_index = 1;
    frame_buffer.retrieve   = -1;
    frame_buffer.ind_his_p  = 0;
    frame_buffer.ind_his_b  = 1;
    frame_buffer.ind_his_free = 2;
    frame_buffer.frame_p_c_n[0].frame_time = -1;
    frame_buffer.frame_p_c_n[1].frame_time = -1;
    frame_buffer.frame_p_c_n[0].duration = 0;
    frame_buffer.frame_p_c_n[1].duration = 0;
    frame_buffer.field_buffer_index  = 0;

    frame_buffer.frame_p_c_n[0].is_invalid = 1;
    frame_buffer.frame_p_c_n[1].is_invalid = 1;
    frame_buffer.frame_p_c_n[2].is_invalid = 1;

    frame_buffer.frame_p_c_n[0].va_index = -1;
    frame_buffer.frame_p_c_n[1].va_index = -1;
    frame_buffer.frame_p_c_n[2].va_index = -1;

    if (m_pCCData)
    {
      m_ccCurrData.SetBufferPointer(0,0);
      m_pCCData->Reset();
    }

    return UMC_OK;
}

Status MPEG2VideoDecoderBase::GetInfo(BaseCodecParams* info)
{
  VideoDecoderParams *pParams;
  if(info == NULL)
    return UMC_ERR_NULL_PTR;

  pParams = DynamicCast<VideoDecoderParams, BaseCodecParams> (info);

  if (NULL != pParams)
  {
    // BaseCodecParams
    pParams->m_info.iProfile = sequenceHeader.profile;
    pParams->m_info.iLevel = sequenceHeader.level;

    pParams->m_info = m_ClipInfo;
    //VideoDecoder::GetInfo(pParams);
    pParams->m_iFlags = m_lFlags;
    if(m_bOwnAllocator)
      pParams->m_lpMemoryAllocator = 0;
    else
      pParams->m_lpMemoryAllocator = m_pMemoryAllocator;
    pParams->m_pPostProcessor = m_pPostProcessor;
    pParams->m_iThreads = m_nNumberOfThreads;
  }
  return UMC_OK;
}

Status MPEG2VideoDecoderBase::SaveFrame(Ipp32s index, Ipp64f dTime, MediaData *output)
{
  Status ret;
  VM_ASSERT(index >= 0 && index < 3);

  if (m_lastDecodedFrame.m_colorFormat != m_ClipInfo.videoInfo.m_colorFormat ||
      m_lastDecodedFrame.m_iWidth != m_ClipInfo.videoInfo.m_iWidth ||
      m_lastDecodedFrame.m_iHeight != m_ClipInfo.videoInfo.m_iHeight) {
    m_lastDecodedFrame.Init(m_ClipInfo.videoInfo.m_iWidth, m_ClipInfo.videoInfo.m_iHeight, m_ClipInfo.videoInfo.m_colorFormat);
  }
  m_lastDecodedFrame.m_frameType = frame_buffer.frame_p_c_n[index].frame_type;
  m_lastDecodedFrame.m_fPTSStart = dTime;
  m_lastDecodedFrame.SetPlanePitch(frame_buffer.Y_comp_pitch, 0);
  m_lastDecodedFrame.SetPlanePitch(frame_buffer.U_comp_pitch, 1);
  m_lastDecodedFrame.SetPlanePitch(frame_buffer.V_comp_pitch, 2);
  if (sequenceHeader.progressive_sequence || PictureHeader.progressive_frame) {
      m_lastDecodedFrame.m_picStructure = PS_PROGRESSIVE;
  } else {
    if(PictureHeader.picture_structure == FRAME_PICTURE)
      m_lastDecodedFrame.m_picStructure = (
        (PictureHeader.top_field_first) ? PS_TOP_FIELD_FIRST : PS_BOTTOM_FIELD_FIRST);
    else
      m_lastDecodedFrame.m_picStructure = ( // check last field type
        (PictureHeader.picture_structure == BOTTOM_FIELD) ? PS_TOP_FIELD_FIRST : PS_BOTTOM_FIELD_FIRST);
  }

  if (UMC_OK != (ret = LockBuffers()))
    return ret;
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[index].Y_comp_data, 0);
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[index].U_comp_data, 1);
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[index].V_comp_data, 2);
  ret = m_pPostProcessor->GetFrame(&m_lastDecodedFrame, output);
  output->m_iInvalid = frame_buffer.frame_p_c_n[index].is_invalid;
  UnlockBuffers();
  return ret;
}

void MPEG2VideoDecoderBase::ReadCCData()
{
    Status umcRes = UMC_OK;
    MediaData ccData;
    //we will not wait if there is not buffer
    //while(umcRes != UMC_OK)
    {
        umcRes = m_pCCData->LockInputBuffer(&ccData);
        //vm_time_sleep(1);
    }
    if(umcRes == UMC_OK)
    {
      Mpeg2VideoContext* video = Video[0];
      Ipp8u* ptr;
      Ipp8u* readptr;
      Ipp32s size;
      Ipp32s inputsize;
      Ipp32s code;

      ptr = (Ipp8u*)ccData.GetDataPointer() + ccData.GetDataSize();
      size = (Ipp32s)((Ipp8u*)ccData.GetBufferPointer() + ccData.GetBufferSize() - ptr);
      if(ptr==0 || size<4)
        return;

      inputsize = (Ipp32s)GET_REMAINED_BYTES(video->bs);
      readptr = GET_BYTE_PTR(video->bs);
      FIND_START_CODE(video->bs, code);
      if(code != UMC_ERR_NOT_ENOUGH_DATA)
        inputsize = (Ipp32s)(GET_BYTE_PTR(video->bs) - readptr);

      ptr[0] = 0; ptr[1] = 0; ptr[2] = 1; ptr[3] = 0xb2;
      size -= 4;
      size = IPP_MIN(size, inputsize);

      ippsCopy_8u(readptr, ptr+4, size);
      ccData.SetDataSize(ccData.GetDataSize() + size+4);
      umcRes = m_pCCData->UnLockInputBuffer(&ccData);

      if(code == UMC_ERR_NOT_ENOUGH_DATA) {
        ptr = GET_END_PTR(video->bs);
        if(ptr[-1] <= 1) ptr--;
        if(ptr[-1] == 0) ptr--;
        if(ptr[-1] == 0) ptr--;
        SET_PTR(video->bs, ptr);
      }
    }
}

MPEG2VideoDecoder::MPEG2VideoDecoder()
{
    m_pDec = NULL;
}

MPEG2VideoDecoder::~MPEG2VideoDecoder(void)
{
    Close();
}

Status MPEG2VideoDecoder::Init(BaseCodecParams *init)
{
    Close();

    m_pDec = new MPEG2VideoDecoderBase;
    if(!m_pDec) return UMC_ERR_ALLOC;

    Status res = m_pDec->Init(init);

    if (UMC_OK != res) {
      Close();
    }

    return res;
}


Status MPEG2VideoDecoder::GetFrame(MediaData* in, MediaData* out)
{
    if(m_pDec)
    {
        return m_pDec->GetFrame(in, out);
    }

    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::Close()
{
    if(m_pDec)
    {
        m_pDec->Close();
        delete m_pDec;
        m_pDec = NULL;
    }

    return UMC_OK;
}

Status MPEG2VideoDecoder::Reset()
{
    if(m_pDec)
    {
        return m_pDec->Reset();
    }

    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::GetInfo(BaseCodecParams* info)
{
    if(m_pDec)
    {
        return m_pDec->GetInfo(info);
    }

    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::ResetSkipCount()
{
    if(m_pDec)
    {
        return m_pDec->ResetSkipCount();
    }

    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::SkipVideoFrame(Ipp32s count)
{
    if(m_pDec)
    {
        return m_pDec->SkipVideoFrame(count);
    }

    return UMC_ERR_NOT_INITIALIZED;
}


Ipp32u MPEG2VideoDecoder::GetNumOfSkippedFrames()
{
    if(m_pDec)
    {
        return m_pDec->GetNumOfSkippedFrames();
    }

    return (Ipp32u)(-1); // nobody check for -1 really

}

Status MPEG2VideoDecoder::PreviewLastFrame(VideoData *out, BaseCodec *pPostProcessing)
{
    if(m_pDec)
    {
        return m_pDec->PreviewLastFrame(out, pPostProcessing);
    }
    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::GetUserData(MediaData* pCC)
{
    if(m_pDec)
    {
        return m_pDec->GetUserData(pCC);
    }

    return UMC_ERR_NOT_INITIALIZED;
}

Status MPEG2VideoDecoder::SetParams(BaseCodecParams* params)
{
    if(m_pDec)
    {
        return m_pDec->SetParams(params);
    }
    return UMC_ERR_NOT_INITIALIZED;

}

Status MPEG2VideoDecoderBase::SetParams(BaseCodecParams* params)
{
    VideoDecoderParams *pParams = DynamicCast<VideoDecoderParams, BaseCodecParams>(params);

    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    if(pParams->m_iTrickModes == 3)
    {
        m_bNoBframes = true;
        m_bNoPframes = false;
    }
    else if(pParams->m_iTrickModes == 5)
    {
         m_bNoBframes = true;
         m_bNoPframes = true;
    }
    else
    {
         m_bNoBframes = false;
         m_bNoPframes = false;
    }

    if(pParams->m_iTrickModes == 7)
    {
        m_dPlaybackRate = pParams->m_fPlaybackRate;
    }

    return UMC_OK;
}


Status MPEG2VideoDecoderBase::UpdateFrameBuffer(void)
{
  Ipp32s pitch_l, pitch_c;
  Ipp32s size_l, size_c;
  Ipp8u* ptr;
  Ipp32s i;
  Ipp32s buff_size;

  if(frame_buffer.ptr_context_data &&
    frame_buffer.allocated_mb_width == sequenceHeader.mb_width &&
    frame_buffer.allocated_mb_height == sequenceHeader.mb_height &&
    frame_buffer.allocated_cformat == m_ClipInfo.videoInfo.m_colorFormat)
    return UMC_OK; // all is the same

  frame_buffer.allocated_mb_width  = sequenceHeader.mb_width;
  frame_buffer.allocated_mb_height = sequenceHeader.mb_height;
  frame_buffer.allocated_cformat   = m_ClipInfo.videoInfo.m_colorFormat;

  pitch_l = align_value<Ipp32s>(sequenceHeader.mb_width*16, ALIGN_VALUE);
  size_l = sequenceHeader.mb_height*16*pitch_l;
  if (m_ClipInfo.videoInfo.m_colorFormat != YUV444) {
    pitch_c = align_value<Ipp32s>(sequenceHeader.mb_width*8, ALIGN_VALUE);
    size_c = sequenceHeader.mb_height*8*pitch_c;
    if (m_ClipInfo.videoInfo.m_colorFormat == YUV422)
      size_c *= 2;
  } else {
    pitch_c = pitch_l;
    size_c = size_l;
  }

  frame_buffer.Y_comp_pitch = pitch_l;
  frame_buffer.U_comp_pitch = pitch_c;
  frame_buffer.V_comp_pitch = pitch_c;
  frame_buffer.pic_size = size_l;

  blkOffsets[0][0] = 0;
  blkOffsets[0][1] = 8;
  blkOffsets[0][2] = 8*pitch_l;
  blkOffsets[0][3] = 8*pitch_l + 8;
  blkOffsets[0][4] = 0;
  blkOffsets[0][5] = 0;
  blkOffsets[0][6] = 8*pitch_c;
  blkOffsets[0][7] = 8*pitch_c;
  blkOffsets[1][0] = 0;
  blkOffsets[1][1] = 8;
  blkOffsets[1][2] = pitch_l;
  blkOffsets[1][3] = pitch_l + 8;
  blkOffsets[1][4] = 0;
  blkOffsets[1][5] = 0;
  blkOffsets[1][6] = pitch_c;
  blkOffsets[1][7] = pitch_c;
  blkOffsets[2][0] = 0;
  blkOffsets[2][1] = 8;
  blkOffsets[2][2] = 16*pitch_l;
  blkOffsets[2][3] = 16*pitch_l + 8;
  blkOffsets[2][4] = 0;
  blkOffsets[2][5] = 0;
  blkOffsets[2][6] = 16*pitch_c;
  blkOffsets[2][7] = 16*pitch_c;

  blkPitches[0][0] = pitch_l;
  blkPitches[0][1] = pitch_c;
  blkPitches[1][0] = 2*pitch_l;
  blkPitches[1][1] = pitch_c;
  if (m_ClipInfo.videoInfo.m_colorFormat != YUV420)
      blkPitches[1][1] = 2 * pitch_c;
  blkPitches[2][0] = 2*pitch_l;
  blkPitches[2][1] = 2*pitch_c;


  // Alloc frames
  buff_size = 3*size_l + 6*size_c;
#ifdef KEEP_HISTORY
  buff_size += 3*sequenceHeader.numMB;
#endif

  if(frame_buffer.allocated_size >= buff_size)
      return UMC_OK;

  frame_buffer.allocated_size = buff_size;

  for (i = 0; i < 3; i++)
    frame_buffer.frame_p_c_n[i].va_index = -1;

#ifdef SLICE_THREADING_2
  if (m_pSliceInfo)
    ippsFree(m_pSliceInfo);
  m_pSliceInfo = (sliceInfo *)ippsMalloc_8u(sizeof(sliceInfo)*sequenceHeader.numMB);
  if (!m_pSliceInfo) return UMC_ERR_ALLOC;
  ippsSet_8u(0, (Ipp8u *)m_pSliceInfo, sizeof(sliceInfo)*sequenceHeader.numMB);
#endif

  if(frame_buffer.mid_context_data != MID_INVALID)
    m_pMemoryAllocator->Free(frame_buffer.mid_context_data);

  if(UMC_OK != m_pMemoryAllocator->Alloc(&frame_buffer.mid_context_data, frame_buffer.allocated_size, UMC_ALLOC_PERSISTENT, ALIGN_VALUE))
    return UMC_ERR_ALLOC;

  frame_buffer.ptr_context_data = (Ipp8u*)m_pMemoryAllocator->Lock(frame_buffer.mid_context_data);
  if(!frame_buffer.ptr_context_data)
    return UMC_ERR_ALLOC;

  ptr = frame_buffer.ptr_context_data;
  ptr = align_pointer<Ipp8u*>(ptr, ALIGN_VALUE);

  for (i = 0; i < 3; i++) {
    frame_buffer.frame_p_c_n[i].Y_comp_data = ptr;
    ptr += size_l;
    frame_buffer.frame_p_c_n[i].U_comp_data = ptr;
    ptr += size_c;
    frame_buffer.frame_p_c_n[i].V_comp_data = ptr;
    ptr += size_c;
  }

#ifdef KEEP_HISTORY
    ippsSet_8u(0, ptr, 3*sequenceHeader.numMB);
    frame_buffer.frame_p_c_n[0].frame_history = ptr;
    frame_buffer.frame_p_c_n[1].frame_history = ptr + sequenceHeader.numMB;
    frame_buffer.frame_p_c_n[2].frame_history = ptr + 2*sequenceHeader.numMB;
#endif
  if (UMC_OK != m_pMemoryAllocator->Unlock(frame_buffer.mid_context_data)) {
    //vm_debug_trace(VM_DEBUG_ERROR, VM_STRING("External Unlock failed\n"));
  }

  return UMC_OK;
}


#define SHIFT_PTR(oldptr, oldbase, newbase) \
  ( (newbase) + ( (Ipp8u*)(oldptr) - (Ipp8u*)(oldbase) ) )

Status MPEG2VideoDecoderBase::LockBuffers(void)
{
  Ipp32s i;
  Ipp8u* newptr;


  if (frame_buffer.ptr_context_data == 0) // wasn't allocated, error
    return UMC_ERR_NOT_INITIALIZED;
  newptr = (Ipp8u*) m_pMemoryAllocator->Lock(frame_buffer.mid_context_data);
  if(newptr == frame_buffer.ptr_context_data) //ptr not changed
    return UMC_OK;
  // else need to reset pointers
  for (i = 0; i < 3; i++) {
    frame_buffer.frame_p_c_n[i].Y_comp_data =
      (Ipp8u*)SHIFT_PTR(frame_buffer.frame_p_c_n[i].Y_comp_data,
      frame_buffer.ptr_context_data, newptr);
    frame_buffer.frame_p_c_n[i].U_comp_data =
      (Ipp8u*)SHIFT_PTR(frame_buffer.frame_p_c_n[i].U_comp_data,
      frame_buffer.ptr_context_data, newptr);
    frame_buffer.frame_p_c_n[i].V_comp_data =
      (Ipp8u*)SHIFT_PTR(frame_buffer.frame_p_c_n[i].V_comp_data,
      frame_buffer.ptr_context_data, newptr);
#ifdef KEEP_HISTORY
    frame_buffer.frame_p_c_n[i].frame_history =
      (Ipp8u*)SHIFT_PTR(frame_buffer.frame_p_c_n[i].frame_history,
      frame_buffer.ptr_context_data, newptr);
#endif
  }
  frame_buffer.ptr_context_data = newptr;
  return UMC_OK;
}

Status MPEG2VideoDecoderBase::UnlockBuffers(void)
{

  return m_pMemoryAllocator->Unlock(frame_buffer.mid_context_data);
}

// need to lock internal buffer before call
Status MPEG2VideoDecoderBase::PreviewLastFrame(VideoData *out, BaseCodec *pPostProcessing)
{
  Status ret = UMC_ERR_NOT_ENOUGH_DATA;
  if (frame_buffer.retrieve < 0 || UMC_OK != (ret = LockBuffers()))
    return ret;
  VM_ASSERT(frame_buffer.retrieve >= 0 && frame_buffer.retrieve < 3);
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[frame_buffer.retrieve].Y_comp_data, 0);
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[frame_buffer.retrieve].U_comp_data, 1);
  m_lastDecodedFrame.SetPlaneDataPtr(frame_buffer.frame_p_c_n[frame_buffer.retrieve].V_comp_data, 2);
  ret = VideoDecoder::PreviewLastFrame(out);
  UnlockBuffers();
  return ret;
}


#ifdef OVERLAY_SUPPORT

#define SAVE_OLD_BUFFERS()                                          \
  Ipp8u *old_buffers[3][3];                                         \
  Ipp32s old_steps[3];                                              \
  Ipp32s i;                                                         \
                                                                    \
  /* save pointers */                                               \
  for (i = 0; i < 3; i++) {                                         \
    old_buffers[i][0] = frame_buffer.frame_p_c_n[i].Y_comp_data;    \
    old_buffers[i][1] = frame_buffer.frame_p_c_n[i].U_comp_data;    \
    old_buffers[i][2] = frame_buffer.frame_p_c_n[i].V_comp_data;    \
  }                                                                 \
  old_steps[0] = frame_buffer.Y_comp_pitch;                         \
  old_steps[1] = frame_buffer.U_comp_pitch;                         \
  old_steps[2] = frame_buffer.V_comp_pitch

#define COPY_FROM_OLD_BUFFERS()                                                     \
  IppiSize size_l = {sequenceHeader.mb_width*16, sequenceHeader.mb_height*16};      \
  IppiSize size_c = {sequenceHeader.mb_width*8,  sequenceHeader.mb_height*8};       \
  for (i = 0; i < 3; i++) {                                                         \
    ippiCopy_8u_C1R(old_buffers[i][0], old_steps[0], frame_buffer.frame_p_c_n[i].Y_comp_data, frame_buffer.Y_comp_pitch, size_l); \
    ippiCopy_8u_C1R(old_buffers[i][1], old_steps[1], frame_buffer.frame_p_c_n[i].U_comp_data, frame_buffer.U_comp_pitch, size_c); \
    ippiCopy_8u_C1R(old_buffers[i][2], old_steps[2], frame_buffer.frame_p_c_n[i].V_comp_data, frame_buffer.V_comp_pitch, size_c); \
  }

void MPEG2VideoDecoderBase::SwitchToExternalFrameBuffer(Ipp8u *ext_buffer[], Ipp32s bufstep[])
{
  //if (frame_buffer.frame_p_c_n[0].Y_comp_data == ext_buffer[0]) {
  //  return; // already external pointers
  //}

  SAVE_OLD_BUFFERS();

  // set external pointers
  {
    Ipp32s i;
    for (i = 0; i < 3; i++) {
      frame_buffer.frame_p_c_n[i].Y_comp_data = ext_buffer[3*i];
      frame_buffer.frame_p_c_n[i].U_comp_data = ext_buffer[3*i + 1];
      frame_buffer.frame_p_c_n[i].V_comp_data = ext_buffer[3*i + 2];
    }
    frame_buffer.Y_comp_pitch = bufstep[0];
    frame_buffer.U_comp_pitch = bufstep[1];
    frame_buffer.V_comp_pitch = bufstep[2]; // == bufstep[1] !!!
  }

  COPY_FROM_OLD_BUFFERS();
}

void MPEG2VideoDecoderBase::SwitchToInternalFrameBuffer(void)
{
  SAVE_OLD_BUFFERS();

  // set internal pointers
  UpdateFrameBuffer();

  COPY_FROM_OLD_BUFFERS();
}

void* MPEG2VideoDecoderBase::GetCurrentFramePtr()
{
  if (frame_buffer.retrieve < 0) {
    return NULL;
  }
  return frame_buffer.frame_p_c_n[frame_buffer.retrieve].Y_comp_data;
}

Ipp32s MPEG2VideoDecoderBase::GetNumberOfBuffers()
{
  return 3;
}

void MPEG2VideoDecoder::SwitchToExternalFrameBuffer(Ipp8u *ext_buffer[], Ipp32s bufstep[])
{
  if(m_pDec)
  {
    return m_pDec->SwitchToExternalFrameBuffer(ext_buffer, bufstep);
  }
}

void MPEG2VideoDecoder::SwitchToInternalFrameBuffer()
{
  if(m_pDec)
  {
    return m_pDec->SwitchToInternalFrameBuffer();
  }
}

void* MPEG2VideoDecoder::GetCurrentFramePtr()
{
  if(m_pDec)
  {
    return m_pDec->GetCurrentFramePtr();
  }
  return NULL;
}

Ipp32s MPEG2VideoDecoder::GetNumberOfBuffers()
{
  if(m_pDec)
  {
    return m_pDec->GetNumberOfBuffers();
  }
  return 0;
}

#endif /* OVERLAY_SUPPORT */

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER

