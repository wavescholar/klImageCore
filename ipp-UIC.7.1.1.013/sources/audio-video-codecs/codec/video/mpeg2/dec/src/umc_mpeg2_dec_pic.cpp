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

#include "umc_mpeg2_dec_defs.h"
#include "umc_mpeg2_dec_base.h"
#include "umc_video_data.h"
#include "umc_structures.h"

#ifdef SLICE_THREADING_2
#include "vm_time.h"
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4244)
#endif

using namespace UMC;

extern Ipp8u default_intra_quantizer_matrix[64];

Ipp32s ar_m1_tab[15][2] = { // 11172-2 2.4.3.2
    { 0, 0},    // 0   forbidden
    { 1, 1},    // 1   1.0000 VGA etc.
    {33,49},    // 2   0.6735
    {45,64},    // 3   0.7031 16:9, 625line
    {16,21},    // 4   0.7615
    {29,36},    // 5   0.8055
    {27,32},    // 6   0.8437 16:9, 525line
    {42,47},    // 7   0.8935
    {15,16},    // 8   0.9375 CCIR601, 625line
    {53,54},    // 9   0.9815
    {40,39},    // 10  1.0255
    {77,72},    // 11  1.0695
    { 9, 8},    // 12  1.1250 CCIR601, 525line
    {22,19},    // 13  1.1575
    {161,134}   // 14  1.2015
};

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

// can decode all headers, except of slices
// bitstream should point to next after header byte
Status MPEG2VideoDecoderBase::DecodeHeader(Ipp32s startcode, Mpeg2VideoContext* video)
{
  Ipp32u code;

  switch(startcode) {
    case SEQUENCE_HEADER_CODE:
      return (DecodeSequenceHeader(video));
    case SEQUENCE_END_CODE:
      return (UMC_OK);
    case GROUP_START_CODE:
      GET_BITS_LONG(video->bs, 27, code)
      sequenceHeader.broken_link = code & 1;
      sequenceHeader.closed_gop  = code & 2;
      sequenceHeader.gop_picture = (code>>2) & 0x3f;
      sequenceHeader.gop_second  = ((code>>8) & 0x3f) + ((code>>15) & 0x3f)*60 + ((code>>21) & 0x1f)*3600;
      sequenceHeader.stream_time_temporal_reference = -1; //-2; // new count
      return (UMC_OK); // ignore for a while
    case EXTENSION_START_CODE:
      SHOW_TO9BITS(video->bs, 4, code)
      //extensions
      switch(code) {
        case SEQUENCE_EXTENSION_ID:
          {
            Ipp32u bit_rate_extension, chroma_format;
            GET_BITS32(video->bs, code)
            sequenceHeader.profile                          = (code >> 24) & 0x7;//ignore escape bit
            sequenceHeader.level                            = (code >> 20) & 0xf;
            sequenceHeader.progressive_sequence             = (code >> 19) & 1;
            chroma_format               = 1 << ((code >> 17) & ((1 << 2) - 1));
            m_ClipInfo.videoInfo.m_iWidth  = (m_ClipInfo.videoInfo.m_iWidth  & 0xfff)
              | ((code >> (15-12)) & 0x3000);
            m_ClipInfo.videoInfo.m_iHeight = (m_ClipInfo.videoInfo.m_iHeight & 0xfff)
              | ((code >> (13-12)) & 0x3000);
            bit_rate_extension = (code >> 1) & 0xfff;

            GET_BITS(video->bs, 16, code)
            sequenceHeader.frame_rate_extension_d           = code & 31;
            sequenceHeader.frame_rate_extension_n           = (code >> 5) & 3;

            switch (chroma_format)
            {
            case 2:
                m_ClipInfo.videoInfo.m_colorFormat = YUV420;
                break;
            case 4:
                m_ClipInfo.videoInfo.m_colorFormat = YUV422;
                break;
            case 8:
                m_ClipInfo.videoInfo.m_colorFormat = YUV444;
                break;
            default:
                break;
            }

            if(bit_rate_extension != 0) {
              m_ClipInfo.iBitrate = (m_ClipInfo.iBitrate / 400) & 0x3ffff;
              m_ClipInfo.iBitrate |= (bit_rate_extension<<18);
              if(m_ClipInfo.iBitrate >= 0x40000000/100) // check if fit to 32u
                m_ClipInfo.iBitrate = 0xffffffff;
              else
                m_ClipInfo.iBitrate *= 400;
            }
            sequenceHeader.extension_start_code_ID          = SEQUENCE_EXTENSION_ID; //needed?
          }
          break;
        case SEQUENCE_DISPLAY_EXTENSION_ID:
          sequence_display_extension();
          break;
        case QUANT_MATRIX_EXTENSION_ID:
          quant_matrix_extension();
          break;
        case COPYRIGHT_EXTENSION_ID:
          copyright_extension();
          break;
        case PICTURE_DISPLAY_EXTENSION_ID:
          picture_display_extension();
          break;
        case SEQUENCE_SCALABLE_EXTENSION_ID:
          sequence_scalable_extension();
          break;
        case PICTURE_CODING_EXTENSION_ID: // can move after picture header (if mpeg2)
          //picture_coding_extension();
          {
            GET_BITS32(video->bs,code)
            UMC_CHECK((code>>28) == PICTURE_CODING_EXTENSION_ID, UMC_ERR_INVALID_STREAM);
            PictureHeader.repeat_first_field           = (code >> 1) & 1;
            PictureHeader.alternate_scan               = (code >> 2) & 1;
            PictureHeader.intra_vlc_format             = (code >> 3) & 1;
            PictureHeader.q_scale_type                 = (code >> 4) & 1;
            PictureHeader.concealment_motion_vectors   = (code >> 5) & 1;
            PictureHeader.frame_pred_frame_dct         = (code >> 6) & 1;
            PictureHeader.top_field_first              = (code >> 7) & 1;
            PictureHeader.picture_structure            = (code >> 8) & 3;
            UMC_CHECK(PictureHeader.picture_structure != 0, UMC_ERR_INVALID_STREAM);
            PictureHeader.intra_dc_precision           = (code >> 10) & 3;
            PictureHeader.f_code[3]                    = (code >> 12) & 15;
            UMC_CHECK(PictureHeader.f_code[3] != 0 && (PictureHeader.f_code[3] <= 9 || PictureHeader.f_code[3] == 15), UMC_ERR_INVALID_STREAM);
            PictureHeader.f_code[2]                    = (code >> 16) & 15;
            UMC_CHECK(PictureHeader.f_code[2] != 0 && (PictureHeader.f_code[2] <= 9 || PictureHeader.f_code[2] == 15), UMC_ERR_INVALID_STREAM);
            PictureHeader.f_code[1]                    = (code >> 20) & 15;
            UMC_CHECK(PictureHeader.f_code[1] != 0 && (PictureHeader.f_code[1] <= 9 || PictureHeader.f_code[1] == 15), UMC_ERR_INVALID_STREAM);
            PictureHeader.f_code[0]                    = (code >> 24) & 15;
            UMC_CHECK(PictureHeader.f_code[0] != 0 && (PictureHeader.f_code[0] <= 9 || PictureHeader.f_code[0] == 15), UMC_ERR_INVALID_STREAM);

            GET_1BIT(video->bs, PictureHeader.progressive_frame)
            GET_1BIT(video->bs, code)
            PictureHeader.r_size[0] = PictureHeader.f_code[0] - 1;
            PictureHeader.r_size[1] = PictureHeader.f_code[1] - 1;
            PictureHeader.r_size[2] = PictureHeader.f_code[2] - 1;
            PictureHeader.r_size[3] = PictureHeader.f_code[3] - 1;

            PictureHeader.curr_intra_dc_multi = intra_dc_multi[PictureHeader.intra_dc_precision];
            PictureHeader.curr_reset_dc          = reset_dc[PictureHeader.intra_dc_precision];
            Ipp32s i, f;
            for(i = 0; i < 4; i++) {
                f = 1 << PictureHeader.r_size[i];
                PictureHeader.low_in_range[i] = -(f * 16);
                PictureHeader.high_in_range[i] = (f * 16) - 1;
                PictureHeader.range[i] = f * 32;
            }
            if(code)
            {
                GET_BITS_LONG(video->bs, 20, code)
            }
            if(PictureHeader.picture_structure == FRAME_PICTURE) {
              frame_buffer.field_buffer_index = 0; // avoid field parity loss in resync
            }

            for (i = 0; i < m_nNumberOfThreads; i++) {
              ownDecodeInterInit_MPEG2(NULL, PictureHeader.alternate_scan | IPPVC_LEAVE_QUANT_UNCHANGED, &Video[i]->decodeInterSpec);
              Video[i]->decodeIntraSpec.intraVLCFormat = PictureHeader.intra_vlc_format;
              Video[i]->decodeIntraSpec.intraShiftDC = PictureHeader.curr_intra_dc_multi;
              ownDecodeIntraInit_MPEG2(NULL, PictureHeader.alternate_scan | IPPVC_LEAVE_QUANT_UNCHANGED, Video[i]->decodeIntraSpec.intraVLCFormat, Video[i]->decodeIntraSpec.intraShiftDC, &Video[i]->decodeIntraSpec);

              ownDecodeInterInit_MPEG2(NULL, PictureHeader.alternate_scan | IPPVC_LEAVE_QUANT_UNCHANGED, &Video[i]->decodeInterSpecChroma);
              ownDecodeIntraInit_MPEG2(NULL, PictureHeader.alternate_scan | IPPVC_LEAVE_QUANT_UNCHANGED, Video[i]->decodeIntraSpec.intraVLCFormat, Video[i]->decodeIntraSpec.intraShiftDC, &Video[i]->decodeIntraSpecChroma);
            }
          }
          break;
        case PICTURE_SPARTIAL_SCALABLE_EXTENSION_ID:
          picture_spartial_scalable_extension();
          break;
        case PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID:
          picture_temporal_scalable_extension();
          break;
        default:
          //VM_ASSERT(0);
          break;
      }
      return (UMC_OK);
    case USER_DATA_START_CODE:
      {
        //Ipp32s i;
        //Ipp32s ATSC_identifier;
        //SHOW_BITS_32(video->bs, ATSC_identifier)
        ////Closed caption data
        //if(ATSC_identifier == 0x47413934)
        //{
        //    //ATSC_identifier       32 bslbf = 0x47413934
        //    SKIP_BITS_32(video->bs)
        //    //user_data_type_code   8 uimsbf
        //    GET_TO9BITS(video->bs, 8, code)

        //    if (code == 0x03)
        //    {
        //        Ipp32s cc_count;
        //        Ipp32s additional_data_flag;
        //        //process_em_data_flag  1 bslbf
        //        //process_cc_data_flag  1 bslbf
        //        //additional_data_flag  1 bslbf
        //        //cc_count              5 uimsbf
        //        GET_TO9BITS(video->bs, 8, cc_count)
        //        additional_data_flag = cc_count&0x20;
        //        cc_count &= 0x1F;
        //        //em_data               8 bslbf
        //        GET_TO9BITS(video->bs, 8, code)

        //        //TBD save and output DTV closed caption data
        //        for (i = 0 ; i < cc_count ; i++ )
        //        {
        //            //marker_bits   5 1111 1
        //            //cc_valid      1 bslbf
        //            //cc_type       2 bslbf
        //            GET_TO9BITS(video->bs, 8, code)
        //            //cc_data_1     8 bslbf
        //            GET_TO9BITS(video->bs, 8, code)
        //            //cc_data_2     8 bslbf
        //            GET_TO9BITS(video->bs, 8, code)
        //        }

        //        //marker_bits 8 1111 1111
        //        GET_TO9BITS(video->bs, 8, code)
        //        VM_ASSERT(code == 0xff);

        //        if (additional_data_flag)
        //        {
        //            //additional_user_data 8
        //            SHOW_BITS_LONG(video->bs,24,code)
        //            while(code != 0x000001)
        //            {
        //                GET_TO9BITS(video->bs, 8, code)
        //                SHOW_BITS_LONG(video->bs,24,code)
        //            }
        //        }
        //    }
        //    else
        //    {
        //        //ATSC_reserved_user_data 8
        //        SHOW_BITS_LONG(video->bs,24,code)
        //        while(code != 0x000001)
        //        {
        //            GET_TO9BITS(video->bs, 8, code)
        //            SHOW_BITS_LONG(video->bs,24,code)
        //        }
        //    }
        //}
        //else
        if(m_pCCData /*&& gop_flag*/)
        {
            ReadCCData();
        }
        else
        {
            //other user info, skip it
            SHOW_BITS_LONG(video->bs,24,code)
            while(code != 0x000001 && (video->bs_end_ptr - video->bs_curr_ptr) > 0)
            {
                GET_TO9BITS(video->bs, 8, code)
                SHOW_BITS_LONG(video->bs,24,code)
            }
        }
      }
      return (UMC_OK);
    case PICTURE_START_CODE:
      return (DecodePictureHeader());
    default:
      return (UMC_ERR_INVALID_STREAM); // unexpected
  }

}

//Sequence Header search. Stops after header start code
Status MPEG2VideoDecoderBase::FindSequenceHeader(Mpeg2VideoContext *video)
{
  Ipp32u code;
  do {
    GET_START_CODE(video->bs, code);
    if(SEQUENCE_HEADER_CODE == code)
      return (UMC_OK);
  } while ( code != UMC_ERR_NOT_ENOUGH_DATA );

  return UMC_ERR_NOT_ENOUGH_DATA;
}

// when video == 0 uses m_ClipInfo values
Status MPEG2VideoDecoderBase::DecodeSequenceHeader(Mpeg2VideoContext* video)
{
    Ipp32u load_non_intra_quantizer_matrix = 0;
    Ipp32u load_intra_quantizer_matrix = 0;
    Ipp32u constrained_parameters_flag;
    Ipp32u frame_rate_code = 0xff, dar_code = 0xff;
    Ipp32s i;
    Ipp32u code;
    Status ret;
    Ipp8u  iqm[64];
    Ipp8u  niqm[64];

    if (video != 0)
    {
        if(GET_REMAINED_BYTES(video->bs) < 8)
        {
            // return header back
            UNGET_BITS_32(video->bs)
            return (UMC_ERR_NOT_ENOUGH_DATA);
        }

        GET_BITS32(video->bs, code)
        frame_rate_code = code  & ((1 << 4) - 1);
        if(frame_rate_code == 0)
            return UMC_ERR_INVALID_STREAM;

        m_ClipInfo.videoInfo.m_iHeight    = (code >> 8) & ((1 << 12) - 1);
        m_ClipInfo.videoInfo.m_iWidth     = (code >> 20) & ((1 << 12) - 1);

        dar_code = (code >> 4) & ((1 << 4) - 1);
        if(dar_code == 0)
            return UMC_ERR_INVALID_STREAM;

        GET_BITS32(video->bs, code)
        m_ClipInfo.iBitrate                 = (code >> 14) * 400;
        constrained_parameters_flag        = (code >> 2)  & 1;
        load_intra_quantizer_matrix        = (code >> 1)  & 1;
        load_non_intra_quantizer_matrix    = code  & 1; // could be changed

        if(load_intra_quantizer_matrix)
        {
            GET_BITS(video->bs, 7, iqm[0])
            iqm[0] |= (load_non_intra_quantizer_matrix << 7);
            for(i=1; i<64; i++)
            {
                GET_BITS(video->bs, 8, iqm[i])
            }
            GET_1BIT(video->bs, load_non_intra_quantizer_matrix);
        }
        if(load_non_intra_quantizer_matrix)
        {
            for(i=0; i<64; i++)
            {
                GET_BITS(video->bs, 8, niqm[i])
            }
        }

        VideoStreamType newtype = MPEG1_VIDEO;
        m_ClipInfo.streamType = MPEG1_VIDEO;
        if(!constrained_parameters_flag)
        {
            FIND_START_CODE(video->bs,code)
            if(code == EXTENSION_START_CODE)
            {
                SKIP_BITS_32(video->bs);
                SHOW_TO9BITS(video->bs, 4, code)
                if(code == SEQUENCE_EXTENSION_ID)
                {
                    newtype = MPEG2_VIDEO;
                    m_ClipInfo.streamType = MPEG2_VIDEO;
                    DecodeHeader(EXTENSION_START_CODE, video);
                }
            }
        }
        if(m_ClipInfo.streamType == UNDEF_VIDEO)
            m_ClipInfo.streamType = newtype;
        else if(m_ClipInfo.streamType != newtype)
            return (UMC_ERR_INVALID_STREAM);
        FIND_START_CODE(video->bs, code);

        while(code == EXTENSION_START_CODE || code == USER_DATA_START_CODE)
        {
            SKIP_BITS_32(video->bs);
            DecodeHeader(code, video);
            FIND_START_CODE(video->bs, code);
        }
    }


    if(m_ClipInfo.streamType == MPEG1_VIDEO) {
      sequenceHeader.progressive_sequence = 1;
      m_ClipInfo.videoInfo.m_colorFormat             = YUV420;
    }
    if(m_ClipInfo.videoInfo.m_colorFormat != YUV422 && m_ClipInfo.videoInfo.m_colorFormat != YUV444) {
      m_ClipInfo.videoInfo.m_colorFormat = YUV420;
    }

    if(m_ClipInfo.streamType == UNDEF_VIDEO ||
       m_ClipInfo.videoInfo.m_iWidth <= 0 || m_ClipInfo.videoInfo.m_iWidth >= 0x10000 ||
       m_ClipInfo.videoInfo.m_iHeight <= 0 || m_ClipInfo.videoInfo.m_iHeight >= 0x10000)
      return UMC_ERR_INVALID_PARAMS; // if was not set in params
    if (m_ClipInfo.fFramerate <= 0)
      m_ClipInfo.fFramerate = 30; // to avoid divide by zero

    // now compute and set params

    frame_buffer.field_buffer_index = 0;

    sequenceHeader.mb_width = (m_ClipInfo.videoInfo.m_iWidth + 15) >> 4;
    sequenceHeader.mb_height= (m_ClipInfo.videoInfo.m_iHeight + 15) >> 4;
    if(!sequenceHeader.progressive_sequence) {
      sequenceHeader.mb_height = (sequenceHeader.mb_height + 1) & ~1;
    }
    sequenceHeader.numMB = sequenceHeader.mb_width*sequenceHeader.mb_height;

    if (UMC_OK != UpdateFrameBuffer()) { // create or update internal buffer
      return (UMC_ERR_ALLOC);
    }

    Ipp32s flag_mpeg1 = (m_ClipInfo.streamType == MPEG1_VIDEO) ? IPPVC_MPEG1_STREAM : 0;
    for (i = 0; i < m_nNumberOfThreads; i++) {
      ownDecodeIntraInit_MPEG2(NULL, flag_mpeg1, PictureHeader.intra_vlc_format, PictureHeader.curr_intra_dc_multi, &Video[i]->decodeIntraSpec);
      ownDecodeInterInit_MPEG2(NULL, flag_mpeg1, &Video[i]->decodeInterSpec);

      Video[i]->decodeInterSpecChroma = Video[i]->decodeInterSpec;
      Video[i]->decodeIntraSpecChroma = Video[i]->decodeIntraSpec;
    }

    if (video != 0) {
      sequenceHeader.is_decoded = 1;

      if(load_intra_quantizer_matrix)
      {
        for (i = 0; i < m_nNumberOfThreads; i++) {
          ownDecodeIntraInit_MPEG2(iqm, flag_mpeg1, PictureHeader.intra_vlc_format, PictureHeader.curr_intra_dc_multi, &Video[i]->decodeIntraSpec);
          Video[i]->decodeIntraSpecChroma = Video[i]->decodeIntraSpec;
        }
      }

      if(load_non_intra_quantizer_matrix)
      {
        for (i = 0; i < m_nNumberOfThreads; i++) {
          ownDecodeInterInit_MPEG2(niqm, flag_mpeg1, &Video[i]->decodeInterSpec);
          Video[i]->decodeInterSpecChroma = Video[i]->decodeInterSpec;
        }
      }

      switch(frame_rate_code)
      {
      case 0:
          sequenceHeader.delta_frame_time = 1000./30000.; break;
      case 1:
          sequenceHeader.delta_frame_time = 1001./24000.; break;
      case 2:
          sequenceHeader.delta_frame_time = 1000./24000.; break;
      case 3:
          sequenceHeader.delta_frame_time = 1000./25000.; break;
      case 4:
          sequenceHeader.delta_frame_time = 1001./30000.; break;
      case 5:
          sequenceHeader.delta_frame_time = 1000./30000.; break;
      case 6:
          sequenceHeader.delta_frame_time = 1000./50000.; break;
      case 7:
          sequenceHeader.delta_frame_time = 1001./60000.; break;
      case 8:
          sequenceHeader.delta_frame_time = 1000./60000.; break;
      default:
          sequenceHeader.delta_frame_time = 1000./30000.;
          //VM_ASSERT(0);
          break;
      }
      m_ClipInfo.fFramerate        = 1.0 / sequenceHeader.delta_frame_time;

      if(m_ClipInfo.streamType == MPEG1_VIDEO) {
        m_ClipInfo.videoInfo.m_iSAWidth = ar_m1_tab[dar_code][0];
        m_ClipInfo.videoInfo.m_iSAHeight = ar_m1_tab[dar_code][1];
        return (UMC_OK);
      }
      Ipp32s W, H;

      if(m_dispSize.width == 0 ||  m_dispSize.height == 0)
      {
          W = m_ClipInfo.videoInfo.m_iWidth;
          H = m_ClipInfo.videoInfo.m_iHeight;
      }
      else
      {
          W = m_dispSize.width;
          H = m_dispSize.height;
      }

      if(dar_code == 2)
      {
        ret = DARtoPAR(W, H, 4, 3,
          &m_ClipInfo.videoInfo.m_iSAWidth, &m_ClipInfo.videoInfo.m_iSAHeight);
      }
      else if(dar_code == 3)
      {
        ret = DARtoPAR(W, H, 16, 9,
          &m_ClipInfo.videoInfo.m_iSAWidth, &m_ClipInfo.videoInfo.m_iSAHeight);
      }
      else if(dar_code == 4)
      {
        ret = DARtoPAR(W, H, 221, 100,
          &m_ClipInfo.videoInfo.m_iSAWidth, &m_ClipInfo.videoInfo.m_iSAHeight);
      }
      else // dar_code == 1 or unknown
      {
        ret = UMC_OK;
        m_ClipInfo.videoInfo.m_iSAWidth = 1;
        m_ClipInfo.videoInfo.m_iSAHeight = 1;
      }
      sequenceHeader.delta_frame_time *= (Ipp64f)(sequenceHeader.frame_rate_extension_d + 1) /
                                                 (sequenceHeader.frame_rate_extension_n + 1);
      m_ClipInfo.fFramerate = 1.0 / sequenceHeader.delta_frame_time;
    }
    else
      sequenceHeader.delta_frame_time = 1.0 / m_ClipInfo.fFramerate;

    return (UMC_OK);
}


void MPEG2VideoDecoderBase::sequence_display_extension()
{
    Ipp32u  code;
    Mpeg2VideoContext* video = Video[0];
    Ipp8u* ptr = video->bs_curr_ptr - 4; 
    
    // skip 3 bits, get 1
    Ipp32u colour = ptr[4] & 0x01;

    if (colour)
    {
        // skip 4 + 24 bits (28 bit)
        code = ((ptr[8]) << 24) | (ptr[9] << 16) | (ptr[10] << 8) | ptr[11];
    }
    else
    {
        // skip 4 bits (28 bit)
        code = ((ptr[5]) << 24) | (ptr[6] << 16) | (ptr[7] << 8) | ptr[8];
    }

    m_dispSize.width = (code >> 18);
    m_dispSize.height = (code >> 3) & 0x00003fff;
}

void MPEG2VideoDecoderBase::sequence_scalable_extension()
{
    Ipp32u code;
    Mpeg2VideoContext* video = Video[0];
    sequenceHeader.extension_start_code_ID = SEQUENCE_SCALABLE_EXTENSION_ID;

    GET_TO9BITS(video->bs, 2, sequenceHeader.scalable_mode)//scalable_mode
    GET_TO9BITS(video->bs, 4, code);//layer_id
    if(sequenceHeader.scalable_mode == SPARTIAL_SCALABILITY)
    {
        SKIP_BITS_32(video->bs)
        SKIP_BITS_LONG(video->bs, 17)
    }
    else if(sequenceHeader.scalable_mode == TEMPORAL_SCALABILITY)
    {
        GET_1BIT(video->bs,code)//picture mux enable
        if(code)
            GET_1BIT(video->bs,code)//mux to progressive sequence
        SKIP_BITS(video->bs, 6)
    }
}

//Status MPEG2VideoDecoderBase::DecodeBegin(Ipp64f time, sVideoStreamInfo * /*info*/)
//{
//    Mpeg2VideoContext *video = Video[0];
//    Ipp32u          code;
//    Status          umcRes;
//
//    if(time > 0)
//      sequenceHeader.stream_time = time;
//
//    //get gop or picture start code
//    FIND_START_CODE(video->bs, code);
//
//    //TBD GOP process
//    while(code != PICTURE_START_CODE && code != (Ipp32u)UMC_ERR_NOT_ENOUGH_DATA)
//    {
//        SKIP_BITS_32(video->bs);
//        DecodeHeader(code, video);
//        FIND_START_CODE(video->bs, code);
//    }
//    //if(code != PICTURE_START_CODE)
//    //  return (m_lFlags & FLAG_VDEC_REORDER)? UMC_ERR_NOT_ENOUGH_DATA : UMC_OK;
//
//    if ((m_lFlags & FLAG_VDEC_REORDER) && GET_REMAINED_BYTES(video->bs) > 15)
//    {
//      Ipp64f start_time; // result unused
//
//        //sequenceHeader.stream_time_temporal_reference = -1;
//        GET_BITS32(video->bs, code);
//        VM_ASSERT(code == PICTURE_START_CODE);
//        umcRes = DecodeHeader(code, video);
//        if(umcRes != UMC_OK)
//          return (umcRes);
//
//        sequenceHeader.stream_time_temporal_reference++;
//
//        if(PictureHeader.picture_coding_type == MPEG2_I_PICTURE)
//        {
//            sequenceHeader.bdied = 0;
//            sequenceHeader.first_i_occure = 1;
//        }
//        else if(!PictureHeader.d_picture)
//            return (UMC_ERR_NOT_ENOUGH_BUFFER);
//
//        CalculateFrameTime(time, &start_time);
//
//        umcRes = DecodePicture();
//        if(umcRes != UMC_OK) {
//          return (umcRes);
//        }
//        if (PictureHeader.picture_structure == FRAME_PICTURE ||
//            frame_buffer.field_buffer_index == 0)
//        {
//          m_decodedFrameNum++;
//        }
//    }
//
//    return (UMC_OK);
//}

// compute and store PT for input, return PT of the frame to be out
// called after field_buffer_index switching
// when FLAG_VDEC_REORDER isn't set, time can be wrong
//   if repeat_first_field happens != 0
void MPEG2VideoDecoderBase::CalculateFrameTime(Ipp64f in_time, Ipp64f * out_time)
{
    //Mpeg2VideoContext* video        = Video[0];
    Ipp32s index;
    Ipp64f duration = sequenceHeader.delta_frame_time;
    if(PictureHeader.repeat_first_field) {
      if(sequenceHeader.progressive_sequence != 0) { // 2 or 3 frames duration
        duration += sequenceHeader.delta_frame_time;
        if(PictureHeader.top_field_first)
          duration += sequenceHeader.delta_frame_time;
      } else { // 3 fields duration
        duration *= 1.5;
      }
    }
    frame_buffer.frame_p_c_n [frame_buffer.curr_index].duration = duration;

    if(PictureHeader.picture_structure == FRAME_PICTURE ||
       frame_buffer.field_buffer_index == 0)
    { // Save time provided for the frame, ignore for second field
      frame_buffer.frame_p_c_n [frame_buffer.curr_index].frame_time =
              in_time;
      // Compute current time, ignoring second field, don't recalc if absent
      if(in_time > 0) {
        if(PictureHeader.picture_coding_type == MPEG2_B_PICTURE)
          sequenceHeader.stream_time = in_time;
        else if ( !(m_lFlags & FLAG_VDEC_REORDER) ||
          m_decodedFrameNum == 0){ // can happen in the beginning
          // can become wrong
          sequenceHeader.stream_time = in_time - sequenceHeader.delta_frame_time *
            (PictureHeader.temporal_reference - sequenceHeader.stream_time_temporal_reference);
        }
      }
      if(frame_buffer.retrieve >= 0 && //stream time is of output other IP
        PictureHeader.picture_coding_type != MPEG2_B_PICTURE &&
          frame_buffer.frame_p_c_n [1-frame_buffer.curr_index].frame_time >= 0)
        sequenceHeader.stream_time =
          frame_buffer.frame_p_c_n [1-frame_buffer.curr_index].frame_time;
    }

    // Compute time to be out
    index = frame_buffer.retrieve;
    if(index>=0 && frame_buffer.frame_p_c_n [index].frame_time < 0) {
      // Frame to be out hasn't proper time
      if(PictureHeader.picture_coding_type == MPEG2_B_PICTURE ||
         m_lFlags & FLAG_VDEC_REORDER) { // use current time
        *out_time = sequenceHeader.stream_time;
      } else { // compute next ref_frame, really curr_time + IPdistance. But repeat field!
        *out_time = sequenceHeader.stream_time +  sequenceHeader.delta_frame_time *
          (PictureHeader.temporal_reference - sequenceHeader.stream_time_temporal_reference);
      }
    } else if(index>=0) {
      *out_time = frame_buffer.frame_p_c_n [index].frame_time;
    } else {
      *out_time = -1;
    }

    // Update current time after second field
    if(PictureHeader.picture_structure == FRAME_PICTURE ||
      frame_buffer.field_buffer_index == 1)
    {
      if(PictureHeader.picture_coding_type == MPEG2_B_PICTURE)
        sequenceHeader.stream_time +=
          frame_buffer.frame_p_c_n [frame_buffer.curr_index].duration
          * (m_dPlaybackRate > 0 ? 1 : -1);
      else if(index>=0) // previous reference frame is returned
        sequenceHeader.stream_time +=
          frame_buffer.frame_p_c_n [1-frame_buffer.curr_index].duration
          * (m_dPlaybackRate > 0 ? 1 : -1);
    }

    //printf("tref %2d in %5.3f out %5.3f st %5.3f typ %d  cur %d  retr %d fi %d rep:%d tff:%d\n",
    //        PictureHeader.temporal_reference,
    //        in_time, *out_time, sequenceHeader.stream_time,
    //        PictureHeader.picture_coding_type, frame_buffer.curr_index,
    //        frame_buffer.retrieve, frame_buffer.field_buffer_index,
    //        PictureHeader.repeat_first_field, PictureHeader.top_field_first);
}


#ifdef SLICE_THREADING_2

Status MPEG2VideoDecoderBase::FindSliceStartCode()
{
  Ipp8u *ptr = m_bs_curr_ptr;
  Ipp32s code;
  Ipp32s i;
  Ipp32s mb_height = sequenceHeader.mb_height;
  Ipp32s prev_code = 0;
  if (PictureHeader.picture_structure != FRAME_PICTURE)
    mb_height >>= 1;

  for (i = 0;; i++) {

    while (ptr < m_bs_end_ptr - 1 && (ptr[0] || ptr[1] || ptr[2] != 1)) ptr++;
    code = ptr[3];

    if(code > 0 && code < 0xb0 && ptr < m_bs_end_ptr - 1) { // start of slice
      m_pSliceInfo[i].startPtr = ptr;
      ptr += 4;
      m_pSliceInfo[i].flag = 1;
    } else {
      m_bs_curr_ptr = ptr;
      m_endOfFrame = true;
      m_numOfSlices = i + 1;
      return (UMC_ERR_NOT_ENOUGH_DATA);
    }
  }
  return (UMC_OK);
}


Status MPEG2VideoDecoderBase::DecodeSlices(Ipp32s threadID)
{
  Mpeg2VideoContext *video = Video[threadID];
  Status umcRes;
  Ipp32s i;
  Ipp32s gotSlice;
  Ipp32s mb_height = sequenceHeader.mb_height;
  if (PictureHeader.picture_structure != FRAME_PICTURE)
    mb_height >>= 1;

  for (;;) {

    for (i = m_currSliceNum; i < mb_height*sequenceHeader.mb_width;) {
      gotSlice = 0;
      if (m_pSliceInfo[i].flag == 0) {
        if (m_endOfFrame) {
          return (UMC_OK);
        } else {
          vm_time_sleep(0);
          continue;
        }
      }
      vm_mutex_lock(&m_Locker);
      if (m_pSliceInfo[i].flag == 1) {
        m_currSliceNum = i + 1;
        m_pSliceInfo[i].flag = 2;
        video->bs_start_ptr = m_pSliceInfo[i].startPtr;
        SET_PTR(video->bs, m_pSliceInfo[i].startPtr)
        video->bs_end_ptr = m_bs_end_ptr;
        gotSlice = 1;
      }
      vm_mutex_unlock(&m_Locker);
      if (gotSlice) break;
      else {
        i++;
        continue;
      }
    }

    if (i >= mb_height*sequenceHeader.mb_width)
      return UMC_ERR_NOT_ENOUGH_DATA;

    umcRes = DecodeSliceHeader(video);
    if(umcRes != UMC_OK) break;
    umcRes = DecodeSlice(video);
    if(umcRes != UMC_OK) break;

  }

  return (UMC_OK); //umcRes;
}


#elif defined(SLICE_THREADING)

#define IS_SLICE_START(ptr) \
  ((ptr[0] | ptr[1]) == 0 && ptr[2] == 1 && ptr[3] < 0xb0 && ptr[3] > 0)

#define FIND_SLICE_START(ptr, end_ptr) \
  /*while (ptr < end_ptr && ((ptr[0] | ptr[1]) || ptr[2] != 1)) ptr++;*/ \
  for (;;) { \
    while (ptr < end_ptr && ptr[0]) ptr++; \
    if (!(ptr < end_ptr) || (ptr[1] == 0 && ptr[2] == 1)) break; \
    ptr++; \
  }

Status MPEG2VideoDecoderBase::DecodeSlices(Ipp32s threadID)
{
  Mpeg2VideoContext *video = Video[threadID];
  Status umcRes;

  for (;;) {
    if(m_nNumberOfThreads > 1) {
      Ipp8u *ptr, *end_ptr;

      vm_mutex_lock(&m_Locker); // access to m_bs_curr_ptr

      ptr = m_bs_curr_ptr;
      end_ptr = m_bs_end_ptr;

      SET_PTR(video->bs, ptr)
      //video->bs_start_ptr = ptr;
      video->bs_end_ptr = end_ptr;

      if (ptr < m_bs_end_ptr && IS_SLICE_START(ptr)) {
        ptr += 4;
        FIND_SLICE_START(ptr, end_ptr);
        m_bs_curr_ptr = ptr;
        umcRes = (UMC_OK);
      } else {
        umcRes = (UMC_ERR_NOT_ENOUGH_DATA);
      }

      vm_mutex_unlock(&m_Locker); // access to m_bs_curr_ptr

      if (umcRes != UMC_OK) break;
    }

    umcRes = DecodeSliceHeader(video);
    if(umcRes != UMC_OK) break;
    umcRes = DecodeSlice(video);
    if(umcRes != UMC_OK) break;
  }

  return (UMC_OK); //umcRes;
}


#else // SLICE_THREADING

Status MPEG2VideoDecoderBase::DecodeSlices(Ipp32s threadID)
{
    Mpeg2VideoContext *video = Video[threadID];
    Status umcRes, ret = UMC_OK;

    for (;;) {
      umcRes = DecodeSliceHeader(video);
      if(umcRes != UMC_OK) break;
      umcRes = DecodeSlice(video);
#ifdef UMC_STREAM_ANALYZER
      if(pack_w.is_analyzer)
      {
          pack_w.PicAnalyzerData->dwSliceNum++;
      }
#endif
      if(umcRes == UMC_ERR_INVALID_STREAM || umcRes == UMC_WRN_INVALID_STREAM)
        ret = UMC_WRN_INVALID_STREAM;
    }

    return (ret); //umcRes;
}

#endif

Status MPEG2VideoDecoderBase::DecodePicture()
{
    Mpeg2VideoContext* video = Video[0];
    Status umcRes;
    Ipp32s i;

    LockBuffers(); // lock internal buffers

#ifdef KEEP_HISTORY
    Ipp8u *frame_history = frame_buffer.frame_p_c_n[frame_buffer.ind_his_curr].frame_history;
    if (frame_history) {
      memset(frame_history, 0, sequenceHeader.numMB);
    }
#endif

    Video[0]->slice_vertical_position = 1;
    //Video[m_nNumberOfThreads-1]->next_end_slice_pos = PictureHeader.max_slice_vert_pos + 1;

    //if(m_nNumberOfThreads > 1)
    //{
    //  Ipp8u *start_ptr = video->bs_start_ptr;
    //  Ipp8u *end_ptr = GET_END_PTR(video->bs);
    //  for (i = 1; i < m_nNumberOfThreads; i += 1)
    //  {
    //    Ipp32s j;
    //    INIT_BITSTREAM(Video[i]->bs, start_ptr, end_ptr);
    //    Video[i]->frame_buffer = frame_buffer;
    //    Video[i]->slice_vertical_position = i*PictureHeader.max_slice_vert_pos/m_nNumberOfThreads+1;
    //    Video[i-1]->next_end_slice_pos = Video[i]->slice_vertical_position;

    //    for(j = 0; j < (Ipp32s)m_pStartCodesData->count; j++)
    //    {
    //        if((Ipp32s)(m_pStartCodesData->values[j] & 0xff) == Video[i]->slice_vertical_position)
    //        {
    //           INIT_BITSTREAM(Video[i]->bs, start_ptr + m_pStartCodesData->offsets[j], end_ptr);
    //           break;
    //        }
    //    }
    //  }

    //  m_pStartCodesData->index = 1;

    //  for (i = 1;i < m_nNumberOfThreads;i += 1)
    //  {
    //    vm_event_signal(m_lpStartEvent + i);
    //  }
    //}

    Ipp32s saveNumberOfThreads = m_nNumberOfThreads;

#ifdef SLICE_THREADING_2
    m_endOfFrame = false;
    m_currSliceNum = 0;
#endif

#if defined(SLICE_THREADING) || defined (SLICE_THREADING_2)
    m_bs_curr_ptr = GET_BYTE_PTR(video->bs);
    m_bs_end_ptr  = GET_END_PTR(video->bs) - 3;
#else
    Ipp32s curr_thread = 0;
    if (m_nNumberOfThreads > 1) {
#define MAX_START_CODES 1024
      Ipp8u *start_ptr = GET_BYTE_PTR(video->bs);
      Ipp8u *end_ptr = GET_END_PTR(video->bs)-3;
      Ipp8u *ptr = start_ptr, *ptrz = start_ptr;
      Ipp8u *prev_ptr;
      Ipp32s len = end_ptr - start_ptr;
      Ipp32s j, start_count = 0;
      Ipp32s start_pos[MAX_START_CODES];
      for(start_count = 0; start_count < MAX_START_CODES; start_count++) {
        Ipp32s code;
        do {
          while(ptr<end_ptr && (ptr[0] || ptr[1] || ptr[2] > 1)) ptr++;
          ptrz = ptr;
          while(ptr<end_ptr && !ptr[2]) ptr++;
        } while(ptr<end_ptr && ptr[2] != 1);
        //while(ptr<end_ptr && (ptr[0] || ptr[1] || ptr[2]!=1)) ptr++;
        //ptrz = ptr;
        if(ptr>=end_ptr) {
          ptr = GET_END_PTR(video->bs);
          break;
        }
        code = ptr[3];
        if(code > 0 && code<0xb0) { // start of slice
          start_pos[start_count] = ptrz - start_ptr;
          ptr+=4;
        } else {
          break;
        }
      }

      if (start_count == MAX_START_CODES) {
        while(ptr<end_ptr && (ptr[0] || ptr[1] || ptr[2] != 1 || (ptr[3] > 0 && ptr[3] < 0xb0))) ptr++;
        ptrz = ptr;
      }

      len = (ptrz - start_ptr);
      prev_ptr = start_ptr;
      curr_thread = 1; // 0th will be last
      for(i=0, j=0; i<m_nNumberOfThreads; i++) {
        Ipp32s approx = len * (i+1) / m_nNumberOfThreads;
        if(start_pos[j] > approx) {
          m_nNumberOfThreads --; // no data for thread - covered by previous
          continue;
        }
        while(j<start_count && start_pos[j] < approx)
          j++;
        if(j==start_count) { // it will be last thread -> to 0th
          SET_PTR(Video[0]->bs, prev_ptr)
          m_nNumberOfThreads = curr_thread;
          break;
        }
        //Video[curr_thread]->frame_buffer = frame_buffer;
        //Video[curr_thread]->next_end_slice_pos = Video[i]->slice_vertical_position;
        INIT_BITSTREAM(Video[curr_thread]->bs, prev_ptr, start_ptr + start_pos[j]);
        curr_thread ++;
        prev_ptr = start_ptr + start_pos[j];
      }

      INIT_BITSTREAM(Video[0]->bs, prev_ptr, end_ptr + 3);
  }
  if (curr_thread < m_nNumberOfThreads)
    m_nNumberOfThreads = curr_thread;


#endif

#ifdef SLICE_THREADING
    if (m_nNumberOfThreads > 1) {
      Ipp8u *ptr = m_bs_curr_ptr;
      while (ptr < m_bs_end_ptr && (ptr[0] || ptr[1] || ptr[2] > 1)) ptr++;
      while (ptr < m_bs_end_ptr && !ptr[2]) ptr++;
      if (ptr < m_bs_end_ptr && ptr[3] > 0 && ptr[3] < 0xb0) {
        m_bs_curr_ptr = ptr;
      } else {
        vm_debug_trace(VM_DEBUG_INFO, VM_STRING("No slice startcode\n"));\
        Unlock();
        return (UMC_ERR_NOT_ENOUGH_DATA);
      }
    }
#endif


    for (i = 1; i < m_nNumberOfThreads; i += 1)
    {
      vm_event_signal(m_lpStartEvent + i);
    }

#ifdef SLICE_THREADING_2
    FindSliceStartCode();
#endif

    umcRes = DecodeSlices(0);

    // wait additional thread(s)
    for (i = 1;i < m_nNumberOfThreads;i += 1) {
      vm_event_wait(m_lpStopEvent + i);
      if(m_lpThreadsID[i].m_status != UMC_OK)
        umcRes = m_lpThreadsID[i].m_status;
    }

    video = Video[0];
    //UNGET_BITS_32(video->bs)

    m_nNumberOfThreads = saveNumberOfThreads; // restore, could have been decreased

    if(PictureHeader.picture_structure != FRAME_PICTURE)
    {
        frame_buffer.field_buffer_index ^= 1;
    }
    sequenceHeader.frame_count++;

#ifdef SLICE_THREADING_2
    ippsSet_8u(0, (Ipp8u *)m_pSliceInfo, sizeof(sliceInfo)*m_numOfSlices);
#endif

    UnlockBuffers();  // lock internal buffers
    return (umcRes);
}

Status MPEG2VideoDecoderBase::DecodePictureHeader()
{
    Status          status;
    Ipp32u          code;
    Ipp32s          pic_type = 0;
    Mpeg2VideoContext *video   = Video[0];
    sPictureHeader  *pPic    = &PictureHeader;
    MPEG2FrameType picture_coding_type_save = PictureHeader.picture_coding_type;
    if (GET_REMAINED_BYTES(video->bs) < 4) {
      // return header back
      UNGET_BITS_32(video->bs)
      return (UMC_ERR_NOT_ENOUGH_DATA);
    }

    memset(&PictureHeader, 0, sizeof(PictureHeader));

    GET_BITS(video->bs, 10, PictureHeader.temporal_reference)
    GET_TO9BITS(video->bs, 3, pic_type)
    GET_BITS(video->bs, 16 ,code)

    PictureHeader.d_picture = 0;
    if(pic_type == 3) PictureHeader.picture_coding_type = MPEG2_B_PICTURE;
    else if(pic_type == 2) PictureHeader.picture_coding_type = MPEG2_P_PICTURE;
    else if(pic_type == 1) PictureHeader.picture_coding_type = MPEG2_I_PICTURE;
    else if(pic_type == 4) {
      PictureHeader.picture_coding_type = MPEG2_I_PICTURE;
      PictureHeader.d_picture = 1;
    } else
      return (UMC_ERR_INVALID_STREAM);

    if(PictureHeader.d_picture) {
      sequenceHeader.first_i_occure = 1; // no refs in this case
      //PictureHeader.temporal_reference = sequenceHeader.stream_time_temporal_reference + 1;
    } else if(frame_buffer.field_buffer_index != 0) { //  second field must be the same, except IP
      if(picture_coding_type_save != PictureHeader.picture_coding_type &&
         picture_coding_type_save != MPEG2_I_PICTURE && PictureHeader.picture_coding_type != MPEG2_P_PICTURE)
      {
        frame_buffer.field_buffer_index = 0;
      }
    }

    if(m_dPlaybackRate < 0 && m_dPlaybackRate > -4 && PictureHeader.picture_coding_type == MPEG2_I_PICTURE)
        Reset();

    pPic->low_in_range[0] = pPic->low_in_range[1] =
    pPic->low_in_range[2] = pPic->low_in_range[3] = -16;
    pPic->high_in_range[0] = pPic->high_in_range[1] =
    pPic->high_in_range[2] = pPic->high_in_range[3] = 15;
    pPic->range[0] = pPic->range[1] =
    pPic->range[2] = pPic->range[3] = 32;

    if(PictureHeader.picture_coding_type == MPEG2_B_PICTURE)
    {
        GET_1BIT(video->bs,PictureHeader.full_pel_forward_vector)
        GET_TO9BITS(video->bs, 3, PictureHeader.f_code[0])
        GET_1BIT(video->bs,PictureHeader.full_pel_backward_vector)
        GET_TO9BITS(video->bs, 3, PictureHeader.f_code[2])

        PictureHeader.f_code[1] = PictureHeader.f_code[0];
        PictureHeader.f_code[3] = PictureHeader.f_code[2];
        if(PictureHeader.f_code[0] == 0 || PictureHeader.f_code[2] == 0)
        {
            VM_ASSERT(PictureHeader.f_code[0] != 0 && PictureHeader.f_code[2] != 0);
            return (UMC_ERR_INVALID_STREAM);
        }
        pPic->r_size[0] = PictureHeader.f_code[0] - 1;
        pPic->r_size[1] = PictureHeader.f_code[1] - 1;
        pPic->r_size[2] = PictureHeader.f_code[2] - 1;
        pPic->r_size[3] = PictureHeader.f_code[3] - 1;
    }
    else if(PictureHeader.picture_coding_type == MPEG2_P_PICTURE )
    {
        GET_1BIT(video->bs,PictureHeader.full_pel_forward_vector)
        GET_TO9BITS(video->bs, 3, PictureHeader.f_code[0])
        PictureHeader.f_code[1] = PictureHeader.f_code[0];
        VM_ASSERT(PictureHeader.f_code[0] != 0);

        if(PictureHeader.f_code[0] == 0)
            return (UMC_ERR_INVALID_STREAM);
        pPic->r_size[0] = PictureHeader.f_code[0] - 1;
        pPic->r_size[1] = PictureHeader.f_code[1] - 1;
        pPic->r_size[2] = 0;
        pPic->r_size[3] = 0;
    }
    GET_1BIT(video->bs,code)
    while(code)
    {
        GET_TO9BITS(video->bs, 8 ,code) //extra information picture
        GET_1BIT(video->bs,code)
    }
    //set for MPEG1
    PictureHeader.picture_structure          = FRAME_PICTURE;
    PictureHeader.frame_pred_frame_dct       = 1;
    PictureHeader.intra_vlc_format           = 0;
    PictureHeader.progressive_frame          = 1;
    PictureHeader.curr_intra_dc_multi        = intra_dc_multi[0];
    PictureHeader.curr_reset_dc              = reset_dc[0];

    Ipp32s i, f;
    for(i = 0; i < 4; i++) {
        f = 1 << pPic->r_size[i];
        pPic->low_in_range[i] = -(f * 16);
        pPic->high_in_range[i] = (f * 16) - 1;
        pPic->range[i] = f * 32;
    }

    FIND_START_CODE(video->bs, code);

    //VM_ASSERT(code == EXTENSION_START_CODE);
    Ipp32u ecode = 0;
    if(code == EXTENSION_START_CODE)
    {
        SKIP_BITS_32(video->bs);
        SHOW_TO9BITS(video->bs, 4, ecode)
    }
    if((ecode == PICTURE_CODING_EXTENSION_ID) != (m_ClipInfo.streamType == MPEG2_VIDEO))
      return (UMC_ERR_INVALID_STREAM);

    if(code == EXTENSION_START_CODE)
    {
        status = DecodeHeader(EXTENSION_START_CODE, video);
        if(status != UMC_OK)
            return status;

        FIND_START_CODE(video->bs, code);
    }

    // compute maximum slice vertical position
    if(PictureHeader.picture_structure == FRAME_PICTURE)
      PictureHeader.max_slice_vert_pos = sequenceHeader.mb_height;
    else
      PictureHeader.max_slice_vert_pos = sequenceHeader.mb_height >> 1;

    while (code == EXTENSION_START_CODE || code == USER_DATA_START_CODE)
    {
        SKIP_BITS_32(video->bs);
        DecodeHeader(code, video);

        FIND_START_CODE(video->bs, code);
    }

    if(!sequenceHeader.first_i_occure)
    {
      if (MPEG2_I_PICTURE != PictureHeader.picture_coding_type) // including D-picture
      {
        return (UMC_ERR_NOT_ENOUGH_DATA);
      }
    }

    switch(PictureHeader.picture_coding_type)
    {
    case MPEG2_I_PICTURE:
    case MPEG2_P_PICTURE:
    //case MPEG2_D_PICTURE:
        sequenceHeader.b_curr_number = 0;

        if (PictureHeader.picture_structure == FRAME_PICTURE
            || frame_buffer.field_buffer_index == 0)
        {
            // reset index(es) of frames
            frame_buffer.prev_index = frame_buffer.next_index;
            frame_buffer.next_index = (0 == frame_buffer.next_index) ? (1) : (0);
            frame_buffer.curr_index = frame_buffer.next_index;

#ifdef KEEP_HISTORY
            frame_buffer.ind_his_ref  = frame_buffer.ind_his_p;
            frame_buffer.ind_his_curr = frame_buffer.ind_his_free;
            SWAP(Ipp32s, frame_buffer.ind_his_p, frame_buffer.ind_his_free);
#endif /* KEEP_HISTORY */
        }

        // to optimize logic !!
        if (PictureHeader.picture_structure == FRAME_PICTURE
            || frame_buffer.field_buffer_index == 1)
        { // complete frame
          // reorder frames
          if (m_lFlags & FLAG_VDEC_REORDER) {
            if(m_decodedFrameNum > 0)
              frame_buffer.retrieve = frame_buffer.prev_index;
            else
              frame_buffer.retrieve = -1;
          }
          // not reorder frames
          else
            frame_buffer.retrieve = frame_buffer.curr_index;
        }
        else
          frame_buffer.retrieve = -1;
        //printf("IP: %d %d\n", frame_buffer.ind_his_ref, frame_buffer.ind_his_curr);
        break;
    case MPEG2_B_PICTURE:

        if(frame_buffer.field_buffer_index == 0) {
          sequenceHeader.b_curr_number++;
        }
        // reset index(es) of frames
        frame_buffer.curr_index = 2;

        if (PictureHeader.picture_structure == FRAME_PICTURE
            || frame_buffer.field_buffer_index == 1)
        { // complete frame
          frame_buffer.retrieve = 2;
        } else {
          frame_buffer.retrieve = -1;
        }
#ifdef KEEP_HISTORY
        frame_buffer.ind_his_ref  = frame_buffer.ind_his_b;
        frame_buffer.ind_his_curr = frame_buffer.ind_his_free;
        SWAP(Ipp32s, frame_buffer.ind_his_b, frame_buffer.ind_his_free);
#endif /* KEEP_HISTORY */
        //printf("B%d: %d %d\n", sequenceHeader.b_curr_number, frame_buffer.ind_his_ref, frame_buffer.ind_his_curr);
        break;
    default:
        VM_ASSERT(0);
        return (UMC_ERR_INVALID_STREAM);
    }

    frame_buffer.frame_p_c_n[frame_buffer.curr_index].frame_type =
      PictureHeader.picture_coding_type == MPEG2_B_PICTURE ? B_PICTURE
      : (PictureHeader.picture_coding_type == MPEG2_P_PICTURE ? P_PICTURE : I_PICTURE);


    return (UMC_OK);
}

void MPEG2VideoDecoderBase::quant_matrix_extension(void)
{
    Ipp32s i;
    Ipp32u code;
    Mpeg2VideoContext* video = Video[0];
    Ipp32s load_intra_quantizer_matrix, load_non_intra_quantizer_matrix, load_chroma_intra_quantizer_matrix, load_chroma_non_intra_quantizer_matrix;
    Ipp8u q_matrix[4][64];

    GET_TO9BITS(video->bs, 4 ,code)
    GET_1BIT(video->bs,load_intra_quantizer_matrix)
    if(load_intra_quantizer_matrix)
    {
        for(i= 0; i < 64; i++) {
            GET_BITS(video->bs, 8, q_matrix[0][i]);
        }
        for (i = 0; i < m_nNumberOfThreads; i++) {
          ownDecodeIntraInit_MPEG2(q_matrix[0], IPPVC_LEAVE_SCAN_UNCHANGED, PictureHeader.intra_vlc_format, PictureHeader.curr_intra_dc_multi, &Video[i]->decodeIntraSpec);
        }
    }

    GET_1BIT(video->bs,load_non_intra_quantizer_matrix)
    if(load_non_intra_quantizer_matrix)
    {
        for(i= 0; i < 64; i++) {
            GET_BITS(video->bs, 8, q_matrix[1][i]);
        }
        for (i = 0; i < m_nNumberOfThreads; i++) {
          ownDecodeInterInit_MPEG2(q_matrix[1], IPPVC_LEAVE_SCAN_UNCHANGED, &Video[i]->decodeInterSpec);
        }
    }

    GET_1BIT(video->bs,load_chroma_intra_quantizer_matrix);
    if(load_chroma_intra_quantizer_matrix && m_ClipInfo.videoInfo.m_colorFormat != YUV420)
    {
        for(i= 0; i < 64; i++) {
            GET_TO9BITS(video->bs, 8, q_matrix[2][i])
        }
        for (i = 0; i < m_nNumberOfThreads; i++) {
            ownDecodeIntraInit_MPEG2(q_matrix[2], IPPVC_LEAVE_SCAN_UNCHANGED, PictureHeader.intra_vlc_format, PictureHeader.curr_intra_dc_multi, &Video[i]->decodeIntraSpecChroma);
        }
    }
    else
    {
        for (i = 0; i < m_nNumberOfThreads; i++) {
            Video[i]->decodeIntraSpecChroma = Video[i]->decodeIntraSpec;
        }
    }

    GET_1BIT(video->bs,load_chroma_non_intra_quantizer_matrix);
    if(load_chroma_non_intra_quantizer_matrix && m_ClipInfo.videoInfo.m_colorFormat != YUV420)
    {
        for(i= 0; i < 64; i++) {
            GET_TO9BITS(video->bs, 8, q_matrix[3][i])
        }
        for (i = 0; i < m_nNumberOfThreads; i++) {
            ownDecodeInterInit_MPEG2(q_matrix[3], IPPVC_LEAVE_SCAN_UNCHANGED, &Video[i]->decodeInterSpecChroma);
        }
    }
    else
    {
        for (i = 0; i < m_nNumberOfThreads; i++) {
            Video[i]->decodeInterSpecChroma = Video[i]->decodeInterSpec;
        }
    }


} //void quant_matrix_extension()


void MPEG2VideoDecoderBase::copyright_extension()
{
    Mpeg2VideoContext* video = Video[0];

    SKIP_BITS_32(video->bs)
    SKIP_BITS_32(video->bs)
    SKIP_BITS_LONG(video->bs, 22)
}


void MPEG2VideoDecoderBase::picture_display_extension()
{
    Ipp32s number_of_frame_center_offsets = 0, i;
    Ipp32u code;
    Mpeg2VideoContext* video = Video[0];

    GET_TO9BITS(video->bs, 4 ,code)

    if(sequenceHeader.progressive_sequence)
    {
        if(PictureHeader.repeat_first_field)
        {
            if(PictureHeader.top_field_first)
                number_of_frame_center_offsets = 3;
            else
                number_of_frame_center_offsets = 2;
        }
        else
            number_of_frame_center_offsets = 1;
    }
    else
    {
        if(PictureHeader.picture_structure != FRAME_PICTURE)
            number_of_frame_center_offsets = 1;
        else
        {
            if(PictureHeader.repeat_first_field)
                number_of_frame_center_offsets = 3;
            else
                number_of_frame_center_offsets = 2;
        }
    }
    for(i = 0; i < number_of_frame_center_offsets; i++)
    {
        SKIP_BITS_32(video->bs)
        SKIP_BITS(video->bs, 2)
    }

} //void picture_display_extension()


void MPEG2VideoDecoderBase::picture_spartial_scalable_extension()
{
    Mpeg2VideoContext* video = Video[0];
    SKIP_BITS_32(video->bs)
    SKIP_BITS_LONG(video->bs, 18)
}


void MPEG2VideoDecoderBase::picture_temporal_scalable_extension()
{
    Mpeg2VideoContext* video = Video[0];
    SKIP_BITS_LONG(video->bs, 27)
}

Status MPEG2VideoDecoderBase::GetUserData(MediaData* pCC)
{
    Status umcRes = UMC_OK;

    if(m_ccCurrData.GetBufferSize())
    {
        umcRes = m_pCCData->UnLockOutputBuffer(&m_ccCurrData);
        m_ccCurrData.SetBufferPointer(0,0);
    }

    umcRes = m_pCCData->LockOutputBuffer(&m_ccCurrData);

    if(UMC_OK == umcRes)
    {
        *pCC = m_ccCurrData;
        m_ccCurrData.SetDataSize(0);
    }

    return (umcRes);
}

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
