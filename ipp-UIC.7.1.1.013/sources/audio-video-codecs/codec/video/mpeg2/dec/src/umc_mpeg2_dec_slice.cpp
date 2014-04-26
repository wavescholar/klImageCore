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

Status MPEG2VideoDecoderBase::DecodeSliceHeader(Mpeg2VideoContext *video)
{
    Ipp32u extra_bit_slice;
    Ipp32u code;

    FIND_START_CODE(video->bs, code)
    if(code == (Ipp32u)UMC_ERR_NOT_ENOUGH_DATA) {
      SKIP_TO_END(video->bs);
      return UMC_ERR_NOT_ENOUGH_DATA;
    }


    if(code == PICTURE_START_CODE ||
       code > 0x1AF)
    {
      return UMC_ERR_NOT_ENOUGH_DATA;
    }
    SKIP_BITS_32(video->bs);

    video->slice_vertical_position = (code & 0xff);

    if(m_ClipInfo.videoInfo.m_iHeight > 2800)
    {
        if(video->slice_vertical_position > 0x80)
          return UMC_ERR_INVALID_STREAM;
        GET_TO9BITS(video->bs, 3, /*video->slice_vertical_position_extension*/code)
        video->slice_vertical_position += code << 7;
    }
    if( video->slice_vertical_position > PictureHeader.max_slice_vert_pos)
      return UMC_ERR_INVALID_STREAM;

    if((sequenceHeader.extension_start_code_ID == SEQUENCE_SCALABLE_EXTENSION_ID) &&
        (sequenceHeader.scalable_mode == DATA_PARTITIONING))
    {
        GET_TO9BITS(video->bs, 7, code)
        return UMC_ERR_UNSUPPORTED;
    }

    GET_TO9BITS(video->bs, 5, video->cur_q_scale)
    if(video->cur_q_scale == 0)
    {
        return UMC_ERR_INVALID_STREAM;
    }

    GET_1BIT(video->bs,extra_bit_slice)
    while(extra_bit_slice)
    {
        GET_TO9BITS(video->bs, 9, code)
        extra_bit_slice = code & 1;
    }




    if (m_ClipInfo.streamType != MPEG1_VIDEO) {
      video->cur_q_scale = q_scale[PictureHeader.q_scale_type][video->cur_q_scale];
    }

    RESET_PMV(video->PMV)

    video->macroblock_motion_forward  = 0;
    video->macroblock_motion_backward = 0;

    video->m_bNewSlice = true;

    return UMC_OK;
}

Status MPEG2VideoDecoderBase::DecodeSlice(Mpeg2VideoContext  *video)
{



    if(m_ClipInfo.streamType == MPEG1_VIDEO) {
      return DecodeSlice_MPEG1(video);
    }

    switch(m_ClipInfo.videoInfo.m_colorFormat)
    {
    case YUV420:
        if (PictureHeader.picture_coding_type == MPEG2_I_PICTURE) {
          return DecodeSlice_FrameI_420(video);
        } else {
          if(PictureHeader.picture_structure == FRAME_PICTURE) {
            return DecodeSlice_FramePB_420(video);
          } else {
            return DecodeSlice_FieldPB_420(video);
          }
        }
        break;
    case YUV422:
        if (PictureHeader.picture_coding_type == MPEG2_I_PICTURE) {
          return DecodeSlice_FrameI_422(video);
        } else {
          if(PictureHeader.picture_structure == FRAME_PICTURE) {
            return DecodeSlice_FramePB_422(video);
          } else {
            return DecodeSlice_FieldPB_422(video);
          }
        }
        break;
    default:
        VM_ASSERT(0);
    }

    return UMC_ERR_INVALID_STREAM;
}

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
