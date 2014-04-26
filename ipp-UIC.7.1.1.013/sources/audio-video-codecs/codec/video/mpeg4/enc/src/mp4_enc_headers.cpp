/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class VideoEncoderMPEG4 (put headers to bitstream)
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include <stdio.h>

#include "mp4_enc.h"

namespace MPEG4_ENC
{

inline void VideoEncoderMPEG4::EncodeStartCode(Ipp8u sc)
{
    cBS.PutBits(256 + sc, 32);
}

void VideoEncoderMPEG4::EncodeZeroBitsAlign()
{
    if (cBS.mBitOff != 0)
        cBS.PutBits(0, 8 - cBS.mBitOff);
}

void VideoEncoderMPEG4::EncodeStuffingBitsAlign()
{
    cBS.PutBits(0xFF >> (cBS.mBitOff + 1), 8 - cBS.mBitOff);
}

void VideoEncoderMPEG4::EncodeVideoPacketHeader(BitStream &cBS, Ipp32s mbn, Ipp32s quant)
{
    Ipp32s   rml;

    if (VOP.vop_coding_type == MP4_VOP_TYPE_I)
        rml = 17;
    else if (VOP.vop_coding_type == MP4_VOP_TYPE_B)
        rml = 16 + IPP_MAX(VOP.vop_fcode_forward, VOP.vop_fcode_backward);
    else
        rml = 16 + VOP.vop_fcode_forward;
//f    EncodeStuffingBitsAlign();
    cBS.PutBits(1, rml);
    cBS.PutBits(mbn, mMBNlength);
    cBS.PutBits(quant, VOL.quant_precision);
    cBS.PutZeroBit();
}

void VideoEncoderMPEG4::EncodeGOBHeader(BitStream &cBS, Ipp32s gob_number, Ipp32s quant)
{
    cBS.PutBits(1, 17);
    cBS.PutBits(gob_number, 5);
    cBS.PutBits(VOP.gob_frame_id, 2);
    cBS.PutBits(quant, 5);
}

void VideoEncoderMPEG4::EncodeVOS_Header()
{
    if (!VOL.short_video_header) {
        EncodeStartCode(MP4_VISUAL_OBJECT_SEQUENCE_SC);
        cBS.PutBits(VOS.profile_and_level_indication, 8);
        //f EncodeStartCode(MP4_USER_DATA_SC);
        //f user_data
    }
}

void VideoEncoderMPEG4::EncodeVO_Header()
{
    EncodeStartCode(MP4_VISUAL_OBJECT_SC);
    cBS.PutBits(VO.is_visual_object_identifier, 1);
    if (VO.is_visual_object_identifier) {
        cBS.PutBits(VO.visual_object_verid, 4);
        cBS.PutBits(VO.visual_object_priority, 3);
    }
    cBS.PutBits(VO.visual_object_type, 4);
    if (VO.visual_object_type == MP4_VISUAL_OBJECT_TYPE_VIDEO || VO.visual_object_type == MP4_VISUAL_OBJECT_TYPE_TEXTURE) {
        cBS.PutBits(VO.video_signal_type, 1);
        if (VO.video_signal_type) {
            cBS.PutBits(VO.video_format, 3);
            cBS.PutBits(VO.video_range, 1);
            cBS.PutBits(VO.colour_description, 1);
            if (VO.colour_description) {
                cBS.PutBits(VO.colour_primaries, 8);
                cBS.PutBits(VO.transfer_characteristics, 8);
                cBS.PutBits(VO.matrix_coefficients, 8);
            }
        }
    }
    EncodeStuffingBitsAlign();
    //f EncodeStartCode(MP4_USER_DATA_SC);
    //f user_data
    if (VO.visual_object_type == MP4_VISUAL_OBJECT_TYPE_VIDEO)
        EncodeStartCode(MP4_VIDEO_OBJECT_MIN_SC + 2);
}

void VideoEncoderMPEG4::EncodeVOL_Header()
{
    EncodeStartCode(MP4_VIDEO_OBJECT_LAYER_MIN_SC + 2);
    cBS.PutBits(VOL.random_accessible_vol, 1);
    cBS.PutBits(VOL.video_object_type_indication, 8);
    cBS.PutBits(VOL.is_object_layer_identifier, 1);
    if (VOL.is_object_layer_identifier) {
        cBS.PutBits(VOL.video_object_layer_verid, 4);
        cBS.PutBits(VOL.video_object_layer_priority, 3);
    }
    cBS.PutBits(VOL.aspect_ratio_info, 4);
    if (VOL.aspect_ratio_info == MP4_ASPECT_RATIO_EXTPAR) {
        cBS.PutBits(VOL.par_width, 8);
        cBS.PutBits(VOL.par_height, 8);
    }
    cBS.PutBits(VOL.vol_control_parameters, 1);
    if (VOL.vol_control_parameters) {
        cBS.PutBits(VOL.chroma_format, 2);
        cBS.PutBits(VOL.low_delay, 1);
        cBS.PutBits(VOL.vbv_parameters, 1);
        if (VOL.vbv_parameters) {
            cBS.PutBits(VOL.first_half_bit_rate, 15);
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.latter_half_bit_rate, 5);
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.first_half_vbv_buffer_size, 15);
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.latter_half_vbv_buffer_size, 3);
            cBS.PutBits(VOL.first_half_vbv_occupancy, 11);
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.latter_half_vbv_occupancy, 15);
            cBS.PutMarkerBit();
        }
    }
    cBS.PutBits(VOL.video_object_layer_shape, 2);
    if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_GRAYSCALE && VOL.video_object_layer_verid != 1)
        cBS.PutBits(VOL.video_object_layer_shape_extension, 4);
    cBS.PutMarkerBit();
    cBS.PutBits(VOL.vop_time_increment_resolution, 16);
    cBS.PutMarkerBit();
    cBS.PutBits(VOL.fixed_vop_rate, 1);
    if (VOL.fixed_vop_rate)
        cBS.PutBits(VOL.fixed_vop_time_increment, VOL.vop_time_increment_resolution_bits);
    if (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY) {
        if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_RECTANGULAR) {
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.video_object_layer_width, 13);
            cBS.PutMarkerBit();
            cBS.PutBits(VOL.video_object_layer_height, 13);
            cBS.PutMarkerBit();
        }
        cBS.PutBits(VOL.interlaced, 1);
        cBS.PutBits(VOL.obmc_disable, 1);
        cBS.PutBits(VOL.sprite_enable, (VOL.video_object_layer_verid == 1) ? 1 : 2);
        if (VOL.sprite_enable == MP4_SPRITE_STATIC || VOL.sprite_enable == MP4_SPRITE_GMC) {
            if (VOL.sprite_enable != MP4_SPRITE_GMC) {
                cBS.PutBits(VOL.sprite_width, 13);
                cBS.PutMarkerBit();
                cBS.PutBits(VOL.sprite_height, 13);
                cBS.PutMarkerBit();
                cBS.PutBits(VOL.sprite_left_coordinate, 13);
                cBS.PutMarkerBit();
                cBS.PutBits(VOL.sprite_top_coordinate, 13);
                cBS.PutMarkerBit();
            }
            cBS.PutBits(VOL.no_of_sprite_warping_points, 6);
            cBS.PutBits(VOL.sprite_warping_accuracy, 2);
            cBS.PutBits(VOL.sprite_brightness_change, 1);
            if (VOL.sprite_enable != MP4_SPRITE_GMC)
                cBS.PutBits(VOL.low_latency_sprite_enable, 1);
        }
        if (VOL.video_object_layer_verid != 1 && VOL.video_object_layer_shape != MP4_SHAPE_TYPE_RECTANGULAR)
            cBS.PutBits(VOL.sadct_disable, 1);
        cBS.PutBits(VOL.not_8_bit, 1);
        if (VOL.not_8_bit) {
            cBS.PutBits(VOL.quant_precision, 4);
            cBS.PutBits(VOL.bits_per_pixel, 4);
        }
        if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_GRAYSCALE) {
            cBS.PutBits(VOL.no_gray_quant_update, 1);
            cBS.PutBits(VOL.composition_method, 1);
            cBS.PutBits(VOL.linear_composition, 1);
        }
        cBS.PutBits(VOL.quant_type, 1);
        if (VOL.quant_type) {
            Ipp32s  i;
            cBS.PutBits(VOL.load_intra_quant_mat, 1);
            if (VOL.load_intra_quant_mat) {
                for (i = 0; i < VOL.load_intra_quant_mat_len; i ++)
                    cBS.PutBits(VOL.intra_quant_mat[mp4_ZigZagScan[i]], 8);
                if (VOL.load_intra_quant_mat_len < 64)
                    cBS.PutBits(0, 8);
            }
            cBS.PutBits(VOL.load_nonintra_quant_mat, 1);
            if (VOL.load_nonintra_quant_mat) {
                for (i = 0; i < VOL.load_nonintra_quant_mat_len; i ++)
                    cBS.PutBits(VOL.nonintra_quant_mat[mp4_ZigZagScan[i]], 8);
                if (VOL.load_nonintra_quant_mat_len < 64)
                    cBS.PutBits(0, 8);
            }
            if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_GRAYSCALE)  {
                //f for(i=0; i<aux_comp_count; i++) {
                //f     load_intra_quant_mat_grayscale    1
                //f     if(load_intra_quant_mat_grayscale)
                //f         intra_quant_mat_grayscale[i]    8*[2-64]
                //f     load_nonintra_quant_mat_grayscale    1
                //f     if(load_nonintra_quant_mat_grayscale)
                //f         nonintra_quant_mat_grayscale[i]    8*[2-64]
                //f }
            }
        }
        if (VOL.video_object_layer_verid != 1)
            cBS.PutBits(VOL.quarter_sample, 1);
        cBS.PutBits(VOL.complexity_estimation_disable, 1);
        if (!VOL.complexity_estimation_disable) {
            //f define_vop_complexity_estimation_header()
        }
        cBS.PutBits(VOL.resync_marker_disable, 1);
        cBS.PutBits(VOL.data_partitioned, 1);
        if (VOL.data_partitioned)
            cBS.PutBits(VOL.reversible_vlc, 1);
        if (VOL.video_object_layer_verid != 1) {
            cBS.PutBits(VOL.newpred_enable, 1);
            if (VOL.newpred_enable) {
                cBS.PutBits(VOL.requested_upstream_message_type, 2);
                cBS.PutBits(VOL.newpred_segment_type, 1);
            }
            cBS.PutBits(VOL.reduced_resolution_vop_enable, 1);
        }
        cBS.PutBits(VOL.scalability, 1);
        if (VOL.scalability) {
            cBS.PutBits(VOL.hierarchy_type, 1);
            cBS.PutBits(VOL.ref_layer_id, 4);
            cBS.PutBits(VOL.ref_layer_sampling_direct, 1);
            cBS.PutBits(VOL.hor_sampling_factor_n, 5);
            cBS.PutBits(VOL.hor_sampling_factor_m, 5);
            cBS.PutBits(VOL.vert_sampling_factor_n, 5);
            cBS.PutBits(VOL.vert_sampling_factor_m, 5);
            cBS.PutBits(VOL.enhancement_type, 1);
            if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_BINARY && VOL.hierarchy_type == 0) {
                cBS.PutBits(VOL.use_ref_shape, 1);
                cBS.PutBits(VOL.use_ref_texture, 1);
                cBS.PutBits(VOL.shape_hor_sampling_factor_n, 5);
                cBS.PutBits(VOL.shape_hor_sampling_factor_m, 5);
                cBS.PutBits(VOL.shape_vert_sampling_factor_n, 5);
                cBS.PutBits(VOL.shape_vert_sampling_factor_m, 5);
            }
        }
    } else {
        if (VOL.video_object_layer_verid != 1) {
            cBS.PutBits(VOL.scalability, 1);
            if (VOL.scalability) {
                cBS.PutBits(VOL.shape_hor_sampling_factor_n, 5);
                cBS.PutBits(VOL.shape_hor_sampling_factor_m, 5);
                cBS.PutBits(VOL.shape_vert_sampling_factor_n, 5);
                cBS.PutBits(VOL.shape_vert_sampling_factor_m, 5);
            }
        }
        cBS.PutBits(VOL.resync_marker_disable, 1);
    }
    EncodeStuffingBitsAlign();
    EncodeStartCode(MP4_USER_DATA_SC);
//    Ipp8s ippStr[1024];
//    cBS.PutStr(ippStr);
}

void VideoEncoderMPEG4::EncodeGOV_Header()
{
    Ipp32s  sec, min, hour;

    EncodeStartCode(MP4_GROUP_OF_VOP_SC);
    sec = GOV.time_code;
    hour = sec / 3600;
    sec -= hour * 3600;
    min = sec / 60;
    sec -= min * 60;
    GOV.time_code = (hour << 13) + (min << 7) + (1 << 6) + sec;
    cBS.PutBits(GOV.time_code, 18);
    cBS.PutBits(GOV.closed_gov, 1);
    cBS.PutBits(GOV.broken_link, 1);
    EncodeStuffingBitsAlign();
    //f EncodeStartCode(MP4_USER_DATA_SC);
    //f user_data
}

void VideoEncoderMPEG4::EncodeVOP_Header()
{
    EncodeStartCode(MP4_VIDEO_OBJECT_PLANE_SC);
    cBS.PutBits(VOP.vop_coding_type, 2);
    Ipp32s i = VOP.modulo_time_base;
    while (i > 0) {
        cBS.PutMarkerBit();
        i --;
    }
    cBS.PutZeroBit();
    cBS.PutMarkerBit();
    cBS.PutBits(VOP.vop_time_increment, VOL.vop_time_increment_resolution_bits);
    cBS.PutMarkerBit();
    cBS.PutBits(VOP.vop_coded, 1);
    if (VOP.vop_coded == 0) {
        EncodeStuffingBitsAlign();
        return;
    }
    if (VOL.newpred_enable) {
//f        vop_id    4-15
        cBS.PutBits(VOP.vop_id_for_prediction_indication, 1);
        if (VOP.vop_id_for_prediction_indication) {
//f            vop_id_for_prediction    4-15
            cBS.PutMarkerBit();
        }
    }
    if ((VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY) &&
        (VOP.vop_coding_type == MP4_VOP_TYPE_P ||
        (VOP.vop_coding_type == MP4_VOP_TYPE_S && VOL.sprite_enable == MP4_SPRITE_GMC)))
        cBS.PutBits(VOP.vop_rounding_type, 1);
    if ((VOL.reduced_resolution_vop_enable) && (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_RECTANGULAR) &&
       ((VOP.vop_coding_type == MP4_VOP_TYPE_P) || (VOP.vop_coding_type == MP4_VOP_TYPE_I)))
        cBS.PutBits(VOP.vop_reduced_resolution, 1);
    if (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_RECTANGULAR) {
        if (!(VOL.sprite_enable == MP4_SPRITE_STATIC && VOP.vop_coding_type == MP4_VOP_TYPE_I)) {
            cBS.PutBits(VOP.vop_width, 13);
            cBS.PutMarkerBit();
            cBS.PutBits(VOP.vop_height, 13);
            cBS.PutMarkerBit();
            cBS.PutBits(VOP.vop_horizontal_mc_spatial_ref, 13);
            cBS.PutMarkerBit();
            cBS.PutBits(VOP.vop_vertical_mc_spatial_ref, 13);
            cBS.PutMarkerBit();
        }
        if ((VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY) && VOL.scalability && VOL.enhancement_type)
            cBS.PutBits(VOP.background_composition, 1);
        cBS.PutBits(VOP.change_conv_ratio_disable, 1);
        cBS.PutBits(VOP.vop_constant_alpha, 1);
        if (VOP.vop_constant_alpha)
            cBS.PutBits(VOP.vop_constant_alpha_value, 8);
    }
//f    if (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY)
//f        if (!complexity_estimation_disable)
//f            read_vop_complexity_estimation_header()
    if  (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY) {
        cBS.PutBits(VOP.intra_dc_vlc_thr, 3);
        if (VOL.interlaced) {
            cBS.PutBits(VOP.top_field_first, 1);
            cBS.PutBits(VOP.alternate_vertical_scan_flag, 1);
        }
    }
    if ((VOL.sprite_enable == MP4_SPRITE_STATIC || VOL.sprite_enable == MP4_SPRITE_GMC) && VOP.vop_coding_type == MP4_VOP_TYPE_S) {
        if (VOL.no_of_sprite_warping_points > 0) {
            Ipp32s  dmv, dmva, i, j;
            static const Ipp32s wc[15] = {0, 2, 3, 4, 5, 6, 14, 30, 62, 126, 254, 510, 1022, 2046, 4094};
            static const Ipp32s wl[15] = {2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

            for (i = 0; i < VOL.no_of_sprite_warping_points; i ++) {
                dmv = VOP.warping_mv_code_du[i];
                if (dmv == 0) {
                    cBS.PutBits(0, 2);
                } else {
                    dmva = (dmv < 0) ? -dmv : dmv;
                    j = 1;
                    while (dmva >>= 1)
                        j ++;
                    cBS.PutBits(wc[j], wl[j]);
                    if (dmv < 0)
                        dmv += (1 << j) - 1;
                    cBS.PutBits(dmv, j);
                }
                cBS.PutMarkerBit();
                dmv = VOP.warping_mv_code_dv[i];
                if (dmv == 0) {
                    cBS.PutBits(0, 2);
                } else {
                    dmva = (dmv < 0) ? -dmv : dmv;
                    j = 1;
                    while (dmva >>= 1)
                        j ++;
                    cBS.PutBits(wc[j], wl[j]);
                    if (dmv < 0)
                        dmv += (1 << j) - 1;
                    cBS.PutBits(dmv, j);
                }
                cBS.PutMarkerBit();
            }
        }
        if (VOL.sprite_brightness_change) {
            Ipp32s  i, j;
            j = VOP.brightness_change_factor;
            if (j < 0)
                j = -j;
            if (j <= 16) {
                j += (VOP.brightness_change_factor < 0) ? 16 : 15;
                i = 6;
            } else if (j <= 48) {
                j += (VOP.brightness_change_factor < 0) ? 48 + 128 : 15 + 128;
                i = 8;
            } else if (j <= 112) {
                j += (VOP.brightness_change_factor < 0) ? 112 + 768 : 15 + 768;
                i = 10;
            } else if (j <= 624) {
                j -= 113 - 7168;
                i = 13;
            } else {
                j -= 625 - 15360;
                i = 14;
            }
            cBS.PutBits(j, i);
        }
        if (VOL.sprite_enable == MP4_SPRITE_STATIC)
            return;
    }
    if (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_BINARYONLY) {
        cBS.PutBits(VOP.vop_quant, VOL.quant_precision);
//f        if (VOL.video_object_layer_shape == MP4_SHAPE_TYPE_GRAYSCALE)
//f            for(i=0; i<aux_comp_count; i++)
//f                vop_alpha_quant[i]    6    uimsbf
        if (VOP.vop_coding_type != MP4_VOP_TYPE_I)
            cBS.PutBits(VOP.vop_fcode_forward, 3);
        if (VOP.vop_coding_type == MP4_VOP_TYPE_B)
            cBS.PutBits(VOP.vop_fcode_backward, 3);
        if (!VOL.scalability) {
            if (VOL.video_object_layer_shape != MP4_SHAPE_TYPE_RECTANGULAR && VOP.vop_coding_type != MP4_VOP_TYPE_I)
                cBS.PutBits(VOP.vop_shape_coding_type, 1);
        } else {
//f  scalability
        }
    }
}

void VideoEncoderMPEG4::EncodeVOPSH_Header()
{
    cBS.PutBits(32, 22);
    cBS.PutBits(VOP.temporal_reference, 8);
    cBS.PutMarkerBit();
    cBS.PutZeroBit();
    cBS.PutBits(VOP.split_screen_indicator, 1);
    cBS.PutBits(VOP.document_camera_indicator, 1);
    cBS.PutBits(VOP.full_picture_freeze_release, 1);
    cBS.PutBits(VOP.source_format, 3);
    cBS.PutBits(VOP.picture_coding_type, 1);
    cBS.PutBits(0, 4);
    cBS.PutBits(VOP.vop_quant, 5);
    cBS.PutZeroBit();
    cBS.PutZeroBit();  //f pei
}

Ipp32s VideoEncoderMPEG4::EncodeHeader()
{
    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    if (!VOL.short_video_header) {
//        Ipp8u *sPtr = cBS.GetBitPtr();
        EncodeVOS_Header();
        EncodeVO_Header();
        EncodeVOL_Header();
        //mBitsEncodedTotal += (cBS.GetBitPtr() - sPtr) << 3;
    }
    return MP4_STS_NOERR;
}

} // namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
