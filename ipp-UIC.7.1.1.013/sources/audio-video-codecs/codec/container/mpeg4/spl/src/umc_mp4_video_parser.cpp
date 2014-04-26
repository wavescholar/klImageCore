/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    MPEG-4 video headers parsing.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MP4_SPLITTER

#include "umc_mp4_video_parser.h"
#include "umc_mp4_splitter.h"
#include "umc_automatic_mutex.h"
#include "umc_memory_reader.h"

using namespace UMC;

const Ipp8u mp4_aux_comp_count[MP4_SHAPE_EXT_NUM+1] = {1, 1, 2, 2, 3, 1, 2, 1, 1, 2, 3, 2, 3, 0};

Ipp32u mp4_ShowBits(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp <<= pInfo->bitoff;
    tmp >>= 32 - n;
    return tmp;
}


Ipp32u mp4_ShowBit(mp4_Info* pInfo)
{
    Ipp32u tmp = pInfo->bufptr[0];
    tmp >>= 7 - pInfo->bitoff;
    return (tmp & 1);
}


Ipp32u mp4_ShowBits9(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] <<  8) | ptr[1];
    tmp <<= (pInfo->bitoff + 16);
    tmp >>= 32 - n;
    return tmp;
}


void mp4_FlushBits(mp4_Info* pInfo, Ipp32s n)
{
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
}


Ipp32u mp4_GetBits(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp <<= pInfo->bitoff;
    tmp >>= 32 - n;
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
    return tmp;
}


Ipp32u mp4_GetBit(mp4_Info* pInfo)
{
    Ipp32u tmp = pInfo->bufptr[0];
    if (pInfo->bitoff == 7) {
        pInfo->bitoff = 0;
        pInfo->bufptr ++;
    } else {
        tmp >>= 7 - pInfo->bitoff;
        pInfo->bitoff ++;
    }
    return (tmp & 1);
}


Ipp32u mp4_GetBits9(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 8) | ptr[1];
    tmp <<= (pInfo->bitoff + 16);
    tmp >>= 32 - n;
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
    return tmp;
}


Ipp32s mp4_GetMarkerBit(mp4_Info* pInfo) {
    if (!mp4_GetBit(pInfo))
        return 0;
    return 1;
}

mp4_Status mp4_Parse_VisualObject(mp4_Info* pInfo)
{
    mp4_VisualObject  *VO = &pInfo->VisualObject;

    VO->is_identifier = mp4_GetBit(pInfo);
    if (VO->is_identifier) {
        VO->verid = mp4_GetBits9(pInfo, 4);
        if ((VO->verid != 1) && (VO->verid != 2) && (VO->verid != 4) && (VO->verid != 5))
            VO->verid = 1;
        VO->priority = mp4_GetBits9(pInfo, 3);
    } else
        VO->verid = 1;
    VO->type = mp4_GetBits9(pInfo, 4);
    if (VO->type != MP4_VISUAL_OBJECT_TYPE_VIDEO)
        return MP4_STATUS_NOTSUPPORT;
    VO->VideoSignalType.video_range = 0;
    VO->VideoSignalType.matrix_coefficients = MP4_VIDEO_COLORS_ITU_R_BT_709;
    if (VO->type == MP4_VISUAL_OBJECT_TYPE_VIDEO || VO->type == MP4_VISUAL_OBJECT_TYPE_TEXTURE) {
        VO->VideoSignalType.is_video_signal_type = mp4_GetBit(pInfo);
        if (VO->VideoSignalType.is_video_signal_type) {
            VO->VideoSignalType.video_format = mp4_GetBits9(pInfo, 3);
            VO->VideoSignalType.video_range = mp4_GetBit(pInfo);
            VO->VideoSignalType.is_colour_description = mp4_GetBit(pInfo);
            if (VO->VideoSignalType.is_colour_description) {
                VO->VideoSignalType.colour_primaries = mp4_GetBits9(pInfo, 8);
                VO->VideoSignalType.transfer_characteristics = mp4_GetBits9(pInfo, 8);
                VO->VideoSignalType.matrix_coefficients = mp4_GetBits9(pInfo, 8);
            }
        }
    }
    return MP4_STATUS_OK;
}

static mp4_Status mp4_Parse_QuantMatrix(mp4_Info* pInfo)
{
    Ipp32u  code;
    Ipp32s  i;

    for (i = 0; i < 64; i ++) {
        code = mp4_GetBits9(pInfo, 8);
        if (code == 0) break;
    }
    return MP4_STATUS_OK;
}

mp4_Status mp4_Parse_VideoObject(mp4_Info* pInfo)
{
    Ipp32u  code;
    Ipp32s  i;
    mp4_VisualObject  *VO = &pInfo->VisualObject;
    mp4_VideoObject  *VOL = &pInfo->VisualObject.VideoObject;

    VOL->short_video_header = 0;
    VOL->random_accessible_vol = mp4_GetBit(pInfo);
    VOL->type_indication = mp4_GetBits9(pInfo, 8);

    VOL->is_identifier = mp4_GetBit(pInfo);
    if (VOL->is_identifier) {
        VOL->verid = mp4_GetBits9(pInfo, 4);
        if ((VOL->verid != 1) && (VOL->verid != 2) && (VOL->verid != 4) && (VOL->verid != 5))
            VOL->verid = VO->verid;
        VOL->priority = mp4_GetBits9(pInfo, 3);
    } else
        VOL->verid = VO->verid;
    VOL->aspect_ratio_info = mp4_GetBits9(pInfo, 4);
    if (VOL->aspect_ratio_info == MP4_ASPECT_RATIO_EXTPAR) {
        VOL->aspect_ratio_info_par_width = mp4_GetBits9(pInfo, 8);
        VOL->aspect_ratio_info_par_height = mp4_GetBits9(pInfo, 8);
    }
    VOL->is_vol_control_parameters = mp4_GetBit(pInfo);
    if (VOL->is_vol_control_parameters) {
        VOL->VOLControlParameters.chroma_format = mp4_GetBits9(pInfo, 2);
        if (VOL->VOLControlParameters.chroma_format != MP4_CHROMA_FORMAT_420)
            return MP4_STATUS_PARSE_ERROR;
        VOL->VOLControlParameters.low_delay = mp4_GetBit(pInfo);
        VOL->VOLControlParameters.vbv_parameters = mp4_GetBit(pInfo);
        if (VOL->VOLControlParameters.vbv_parameters) {
            VOL->VOLControlParameters.bit_rate = mp4_GetBits(pInfo, 15) << 15;
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOL->VOLControlParameters.bit_rate += mp4_GetBits(pInfo, 15);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            if (VOL->VOLControlParameters.bit_rate == 0)
                return MP4_STATUS_PARSE_ERROR;
            VOL->VOLControlParameters.vbv_buffer_size = mp4_GetBits(pInfo, 15) << 3;
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOL->VOLControlParameters.vbv_buffer_size += mp4_GetBits9(pInfo, 3);
            if (VOL->VOLControlParameters.vbv_buffer_size == 0)
                return MP4_STATUS_PARSE_ERROR;
            VOL->VOLControlParameters.vbv_occupancy = mp4_GetBits(pInfo, 11) << 15;
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOL->VOLControlParameters.vbv_occupancy += mp4_GetBits(pInfo, 15);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        }
    }
    VOL->shape = mp4_GetBits9(pInfo, 2);
    if (VOL->shape != MP4_SHAPE_TYPE_RECTANGULAR)
        return MP4_STATUS_NOTSUPPORT;
    if (VOL->verid != 1 && VOL->shape == MP4_SHAPE_TYPE_GRAYSCALE) {
        VOL->shape_extension = mp4_GetBits9(pInfo, 4);
        if (VOL->shape_extension >= MP4_SHAPE_EXT_NUM)
            return MP4_STATUS_PARSE_ERROR;
    } else
        VOL->shape_extension = MP4_SHAPE_EXT_NUM;
    if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
    VOL->vop_time_increment_resolution = mp4_GetBits(pInfo, 16);
    if (VOL->vop_time_increment_resolution == 0)
        return MP4_STATUS_PARSE_ERROR;
    if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
    // define number bits in vop_time_increment_resolution
    code = VOL->vop_time_increment_resolution - 1;
    i = 0;
    do {
        code >>= 1;
        i ++;
    } while (code);
    VOL->vop_time_increment_resolution_bits = i;
    VOL->fixed_vop_rate = mp4_GetBit(pInfo);
    if (VOL->fixed_vop_rate) {
        VOL->fixed_vop_time_increment = mp4_GetBits(pInfo, VOL->vop_time_increment_resolution_bits);
    }
    if (VOL->shape != MP4_SHAPE_TYPE_BINARYONLY) {
        if (VOL->shape == MP4_SHAPE_TYPE_RECTANGULAR) {
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOL->width = mp4_GetBits(pInfo, 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOL->height = mp4_GetBits(pInfo, 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        }
        VOL->interlaced = mp4_GetBit(pInfo);
        VOL->obmc_disable = mp4_GetBit(pInfo);
        VOL->sprite_enable = mp4_GetBits9(pInfo, VOL->verid != 1 ? 2 : 1);
        if (VOL->sprite_enable == MP4_SPRITE_STATIC || VOL->sprite_enable == MP4_SPRITE_GMC) {
            if (VOL->sprite_enable == MP4_SPRITE_STATIC) {
                VOL->sprite_width = mp4_GetBits(pInfo, 13);
                if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
                VOL->sprite_height = mp4_GetBits(pInfo, 13);
                if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
                VOL->sprite_left_coordinate = mp4_GetBits(pInfo, 13);
                VOL->sprite_left_coordinate <<= (32 - 13);
                VOL->sprite_left_coordinate >>= (32 - 13);
                if (VOL->sprite_left_coordinate & 1)
                    return MP4_STATUS_PARSE_ERROR;
                if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
                VOL->sprite_top_coordinate = mp4_GetBits(pInfo, 13);
                VOL->sprite_top_coordinate <<= (32 - 13);
                VOL->sprite_top_coordinate >>= (32 - 13);
                if (VOL->sprite_top_coordinate & 1)
                    return MP4_STATUS_PARSE_ERROR;
                if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            }
            VOL->sprite_warping_points = mp4_GetBits9(pInfo, 6);
            if (VOL->sprite_warping_points > 4 ||
                (VOL->sprite_warping_points == 4 &&
                 VOL->sprite_enable == MP4_SPRITE_GMC))
                return MP4_STATUS_PARSE_ERROR;
            VOL->sprite_warping_accuracy = mp4_GetBits9(pInfo, 2);
            VOL->sprite_brightness_change = mp4_GetBit(pInfo);
            if (VOL->sprite_enable == MP4_SPRITE_GMC) {
                if (VOL->sprite_brightness_change)
                    return MP4_STATUS_PARSE_ERROR;
            }
            if (VOL->sprite_enable != MP4_SPRITE_GMC) {
                VOL->low_latency_sprite_enable = mp4_GetBit(pInfo);
                if (VOL->low_latency_sprite_enable)
                    return MP4_STATUS_PARSE_ERROR;
            }
        }
        if (VOL->verid != 1 && VOL->shape != MP4_SHAPE_TYPE_RECTANGULAR) {
            VOL->sadct_disable = mp4_GetBit(pInfo);
            if (!VOL->sadct_disable)
                return MP4_STATUS_PARSE_ERROR;
        }
        VOL->not_8_bit = mp4_GetBit(pInfo);
        if (VOL->not_8_bit)
            return MP4_STATUS_PARSE_ERROR;
        if (VOL->not_8_bit) {
            VOL->quant_precision = mp4_GetBits9(pInfo, 4);
            if (VOL->quant_precision < 3 || VOL->quant_precision > 9)
                return MP4_STATUS_PARSE_ERROR;
            VOL->bits_per_pixel = mp4_GetBits9(pInfo, 4);
            if (VOL->bits_per_pixel < 4 || VOL->bits_per_pixel > 12)
                return MP4_STATUS_PARSE_ERROR;
        } else {
            VOL->quant_precision = 5;
            VOL->bits_per_pixel = 8;
        }
        if (VOL->shape == MP4_SHAPE_TYPE_GRAYSCALE) {
            VOL->no_gray_quant_update = mp4_GetBit(pInfo);
            VOL->composition_method = mp4_GetBit(pInfo);
            VOL->linear_composition = mp4_GetBit(pInfo);
        }
        VOL->quant_type = mp4_GetBit(pInfo);
        if (VOL->quant_type) {
            VOL->load_intra_quant_mat = mp4_GetBit(pInfo);
            if (VOL->load_intra_quant_mat) {
                if (mp4_Parse_QuantMatrix(pInfo) != MP4_STATUS_OK)
                    return MP4_STATUS_PARSE_ERROR;
            }
            VOL->load_nonintra_quant_mat = mp4_GetBit(pInfo);
            if (VOL->load_nonintra_quant_mat) {
                if (mp4_Parse_QuantMatrix(pInfo) != MP4_STATUS_OK)
                    return MP4_STATUS_PARSE_ERROR;
            }
            if (VOL->shape == MP4_SHAPE_TYPE_GRAYSCALE) {
                Ipp32s   ac, i;

                ac = mp4_aux_comp_count[VOL->shape_extension];
                for (i = 0; i < ac; i ++) {
                    VOL->load_intra_quant_mat_grayscale[i] = mp4_GetBit(pInfo);
                    if (VOL->load_intra_quant_mat_grayscale[i]) {
                        if (mp4_Parse_QuantMatrix(pInfo) != MP4_STATUS_OK)
                            return MP4_STATUS_PARSE_ERROR;
                    }
                    VOL->load_nonintra_quant_mat_grayscale[i] = mp4_GetBit(pInfo);
                    if (VOL->load_nonintra_quant_mat_grayscale[i]) {
                        if (mp4_Parse_QuantMatrix(pInfo) != MP4_STATUS_OK)
                            return MP4_STATUS_PARSE_ERROR;
                    }
                }
            }
        }
        if (VOL->verid != 1)
            VOL->quarter_sample = mp4_GetBit(pInfo);
        VOL->complexity_estimation_disable = mp4_GetBit(pInfo);
        if (!VOL->complexity_estimation_disable) {
            VOL->ComplexityEstimation.estimation_method = mp4_GetBits9(pInfo, 2);
            if (VOL->ComplexityEstimation.estimation_method <= 1) {
                VOL->ComplexityEstimation.shape_complexity_estimation_disable = mp4_GetBit(pInfo);
                if (!VOL->ComplexityEstimation.shape_complexity_estimation_disable) {
                    VOL->ComplexityEstimation.opaque =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.transparent =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.intra_cae =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.inter_cae =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.no_update =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.upsampling =  mp4_GetBit(pInfo);
                }
                VOL->ComplexityEstimation.texture_complexity_estimation_set_1_disable =  mp4_GetBit(pInfo);
                if (!VOL->ComplexityEstimation.texture_complexity_estimation_set_1_disable) {
                    VOL->ComplexityEstimation.intra_blocks =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.inter_blocks =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.inter4v_blocks =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.not_coded_blocks =  mp4_GetBit(pInfo);
                }
                if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
                VOL->ComplexityEstimation.texture_complexity_estimation_set_2_disable =  mp4_GetBit(pInfo);
                if (!VOL->ComplexityEstimation.texture_complexity_estimation_set_2_disable) {
                    VOL->ComplexityEstimation.dct_coefs =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.dct_lines =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.vlc_symbols =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.vlc_bits =  mp4_GetBit(pInfo);
                }
                VOL->ComplexityEstimation.motion_compensation_complexity_disable =  mp4_GetBit(pInfo);
                if (!VOL->ComplexityEstimation.motion_compensation_complexity_disable) {
                    VOL->ComplexityEstimation.apm =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.npm =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.interpolate_mc_q =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.forw_back_mc_q =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.halfpel2 =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.halfpel4 =  mp4_GetBit(pInfo);
                }
            }
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            if (VOL->ComplexityEstimation.estimation_method == 1) {
                // verid != 1
                VOL->ComplexityEstimation.version2_complexity_estimation_disable =  mp4_GetBit(pInfo);
                if (!VOL->ComplexityEstimation.version2_complexity_estimation_disable) {
                    VOL->ComplexityEstimation.sadct =  mp4_GetBit(pInfo);
                    VOL->ComplexityEstimation.quarterpel =  mp4_GetBit(pInfo);
                }
            }
        }
        VOL->resync_marker_disable = mp4_GetBit(pInfo);
        VOL->data_partitioned = mp4_GetBit(pInfo);
//f GrayScale Shapes does not support data_part
        if (VOL->data_partitioned)
            VOL->reversible_vlc = mp4_GetBit(pInfo);
        if (VOL->verid != 1) {
            VOL->newpred_enable = mp4_GetBit(pInfo);
            if (VOL->newpred_enable)
                return MP4_STATUS_PARSE_ERROR;
            VOL->reduced_resolution_vop_enable = mp4_GetBit(pInfo);
            if (VOL->reduced_resolution_vop_enable)
                return MP4_STATUS_PARSE_ERROR;
        }
        VOL->scalability = mp4_GetBit(pInfo);
        if (VOL->scalability)
            return MP4_STATUS_PARSE_ERROR;
    } else {
        if (VOL->verid != 1) {
            VOL->scalability = mp4_GetBit(pInfo);
            if (VOL->scalability)
                return MP4_STATUS_PARSE_ERROR;
        }
        VOL->resync_marker_disable = mp4_GetBit(pInfo);
    }
    VOL->VideoObjectPlane.sprite_transmit_mode = MP4_SPRITE_TRANSMIT_MODE_PIECE;
    return MP4_STATUS_OK;
}

static mp4_Status mp4_Sprite_Trajectory(mp4_Info* pInfo) {
    Ipp32s  i, dmv_code, dmv_length, fb;
    Ipp32u  code;

    for (i = 0; i < pInfo->VisualObject.VideoObject.sprite_warping_points; i ++) {
        code = mp4_ShowBits9(pInfo, 3);
        if (code == 7) {
            mp4_FlushBits(pInfo, 3);
            code = mp4_ShowBits9(pInfo, 9);
            fb = 1;
            while (code & 256) {
                code <<= 1;
                fb ++;
            }
            if (fb > 9)
                return MP4_STATUS_PARSE_ERROR;
            dmv_length = fb + 5;
        } else {
            fb = (code <= 1) ? 2 : 3;
            dmv_length = code - 1;
        }
        mp4_FlushBits(pInfo, fb);
        if (dmv_length <= 0)
            dmv_code = 0;
        else {
            dmv_code = mp4_GetBits(pInfo, dmv_length);
            if ((dmv_code & (1 << (dmv_length - 1))) == 0)
                dmv_code -= (1 << dmv_length) - 1;
        }
        if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        pInfo->VisualObject.VideoObject.VideoObjectPlane.warping_mv_code_du[i] = dmv_code;
        code = mp4_ShowBits9(pInfo, 3);
        if (code == 7) {
            mp4_FlushBits(pInfo, 3);
            code = mp4_ShowBits9(pInfo, 9);
            fb = 1;
            while (code & 256) {
                code <<= 1;
                fb ++;
            }
            if (fb > 9)
                return MP4_STATUS_PARSE_ERROR;
            dmv_length = fb + 5;
        } else {
            fb = (code <= 1) ? 2 : 3;
            dmv_length = code - 1;
        }
        mp4_FlushBits(pInfo, fb);
        if (dmv_length <= 0)
            dmv_code = 0;
        else {
            dmv_code = mp4_GetBits(pInfo, dmv_length);
            if ((dmv_code & (1 << (dmv_length - 1))) == 0)
                dmv_code -= (1 << dmv_length) - 1;
        }
        if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        pInfo->VisualObject.VideoObject.VideoObjectPlane.warping_mv_code_dv[i] = dmv_code;
    }
    return MP4_STATUS_OK;
}

mp4_Status mp4_Parse_VideoObjectPlane(mp4_Info* pInfo)
{
    Ipp32u  code;
    mp4_VideoObject          *VOL = &pInfo->VisualObject.VideoObject;
    mp4_VideoObjectPlane     *VOP = &pInfo->VisualObject.VideoObject.VideoObjectPlane;
    mp4_VideoObjectPlaneH263 *VOPSH = &pInfo->VisualObject.VideoObject.VideoObjectPlaneH263;

    if (VOL->short_video_header) {
        code = mp4_GetBits9(pInfo, 6); // read rest bits of short_video_start_marker
        VOPSH->temporal_reference = mp4_GetBits9(pInfo, 8);
        if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        code = mp4_GetBit(pInfo); // zero_bit
        VOPSH->split_screen_indicator = mp4_GetBit(pInfo);
        VOPSH->document_camera_indicator = mp4_GetBit(pInfo);
        VOPSH->full_picture_freeze_release = mp4_GetBit(pInfo);
        VOPSH->source_format = mp4_GetBits9(pInfo, 3);
        if (VOPSH->source_format == 0 || VOPSH->source_format > 5)
            return MP4_STATUS_PARSE_ERROR;
        VOPSH->picture_coding_type = mp4_GetBit(pInfo);
        VOP->coding_type = VOPSH->picture_coding_type;
        code = mp4_GetBits9(pInfo, 4); // four_reserved_zero_bits
        VOPSH->vop_quant = mp4_GetBits9(pInfo, 5);
        code = mp4_GetBit(pInfo); // zero_bit
        for (;;) {
            code = mp4_GetBit(pInfo); // pei
            if (!code)
                break;
            code = mp4_GetBits9(pInfo, 8); // psupp
        }
        return MP4_STATUS_OK;
    }
    VOP->coding_type = mp4_GetBits9(pInfo, 2);
    VOP->modulo_time_base = 0;
    do {
        code = mp4_GetBit(pInfo);
        VOP->modulo_time_base += code;
    } while (code);
    if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
    if (VOL->vop_time_increment_resolution_bits != 0 ) {
        VOP->time_increment = mp4_GetBits(pInfo, VOL->vop_time_increment_resolution_bits);
    }
    if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
    VOP->coded = mp4_GetBit(pInfo);
    if (!VOP->coded)
        return MP4_STATUS_OK;
    //f if (newpred_enable)
    if (VOL->shape != MP4_SHAPE_TYPE_BINARYONLY && (VOP->coding_type == MP4_VOP_TYPE_P ||
        (VOP->coding_type == MP4_VOP_TYPE_S && VOL->sprite_enable == MP4_SPRITE_GMC)))
        VOP->rounding_type = mp4_GetBit(pInfo);
    if (VOL->reduced_resolution_vop_enable && VOL->shape == MP4_SHAPE_TYPE_RECTANGULAR &&
        (VOP->coding_type == MP4_VOP_TYPE_I || VOP->coding_type == MP4_VOP_TYPE_P)) {
        VOP->reduced_resolution = mp4_GetBit(pInfo);
        if (VOP->reduced_resolution)
            return MP4_STATUS_PARSE_ERROR;
    }
    if (VOL->shape != MP4_SHAPE_TYPE_RECTANGULAR) {
        if (!(VOL->sprite_enable == MP4_SPRITE_STATIC && VOP->coding_type == MP4_VOP_TYPE_I)) {
            VOP->vop_width = mp4_GetBits(pInfo, 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOP->vop_height = mp4_GetBits(pInfo, 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOP->vop_horizontal_mc_spatial_ref = mp4_GetBits(pInfo, 13);
            VOP->vop_horizontal_mc_spatial_ref <<= (32 - 13);
            VOP->vop_horizontal_mc_spatial_ref >>= (32 - 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
            VOP->vop_vertical_mc_spatial_ref = mp4_GetBits(pInfo, 13);
            VOP->vop_vertical_mc_spatial_ref <<= (32 - 13);
            VOP->vop_vertical_mc_spatial_ref >>= (32 - 13);
            if (!mp4_GetMarkerBit(pInfo)) return MP4_STATUS_PARSE_ERROR;
        }
//f        if ((VOL->shape != MP4_SHAPE_TYPE_BINARYONLY) && VOL->scalability && enhancement_type)
//f            background_composition = mp4_GetBit(pInfo);
        VOP->change_conv_ratio_disable = mp4_GetBit(pInfo);
        VOP->vop_constant_alpha = mp4_GetBit(pInfo);
        if (VOP->vop_constant_alpha)
            VOP->vop_constant_alpha_value = mp4_GetBits9(pInfo, 8);
        else
            VOP->vop_constant_alpha_value = 255;
    }
    if (VOL->shape != MP4_SHAPE_TYPE_BINARYONLY) {
        if (!VOL->complexity_estimation_disable) {
            if (VOL->ComplexityEstimation.estimation_method == 0) {
                if (VOP->coding_type == MP4_VOP_TYPE_I) {
                    if (VOL->ComplexityEstimation.opaque) VOL->ComplexityEstimation.dcecs_opaque =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.transparent) VOL->ComplexityEstimation.dcecs_transparent =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_cae) VOL->ComplexityEstimation.dcecs_intra_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter_cae) VOL->ComplexityEstimation.dcecs_inter_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.no_update) VOL->ComplexityEstimation.dcecs_no_update =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.upsampling) VOL->ComplexityEstimation.dcecs_upsampling =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_blocks) VOL->ComplexityEstimation.dcecs_intra_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.not_coded_blocks) VOL->ComplexityEstimation.dcecs_not_coded_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_coefs) VOL->ComplexityEstimation.dcecs_dct_coefs =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_lines) VOL->ComplexityEstimation.dcecs_dct_lines =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_symbols) VOL->ComplexityEstimation.dcecs_vlc_symbols =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_bits) VOL->ComplexityEstimation.dcecs_vlc_bits =  mp4_GetBits9(pInfo, 4);
                    if (VOL->ComplexityEstimation.sadct) VOL->ComplexityEstimation.dcecs_sadct =  mp4_GetBits9(pInfo, 8);
                }
                if (VOP->coding_type == MP4_VOP_TYPE_P) {
                    if (VOL->ComplexityEstimation.opaque) VOL->ComplexityEstimation.dcecs_opaque =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.transparent) VOL->ComplexityEstimation.dcecs_transparent =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_cae) VOL->ComplexityEstimation.dcecs_intra_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter_cae) VOL->ComplexityEstimation.dcecs_inter_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.no_update) VOL->ComplexityEstimation.dcecs_no_update =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.upsampling) VOL->ComplexityEstimation.dcecs_upsampling =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_blocks) VOL->ComplexityEstimation.dcecs_intra_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.not_coded_blocks) VOL->ComplexityEstimation.dcecs_not_coded_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_coefs) VOL->ComplexityEstimation.dcecs_dct_coefs =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_lines) VOL->ComplexityEstimation.dcecs_dct_lines =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_symbols) VOL->ComplexityEstimation.dcecs_vlc_symbols =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_bits) VOL->ComplexityEstimation.dcecs_vlc_bits =  mp4_GetBits9(pInfo, 4);
                    if (VOL->ComplexityEstimation.inter_blocks) VOL->ComplexityEstimation.dcecs_inter_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter4v_blocks) VOL->ComplexityEstimation.dcecs_inter4v_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.apm) VOL->ComplexityEstimation.dcecs_apm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.npm) VOL->ComplexityEstimation.dcecs_npm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.forw_back_mc_q) VOL->ComplexityEstimation.dcecs_forw_back_mc_q =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel2) VOL->ComplexityEstimation.dcecs_halfpel2 =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel4) VOL->ComplexityEstimation.dcecs_halfpel4 =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.sadct) VOL->ComplexityEstimation.dcecs_sadct =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.quarterpel) VOL->ComplexityEstimation.dcecs_quarterpel =  mp4_GetBits9(pInfo, 8);
                }
                if (VOP->coding_type == MP4_VOP_TYPE_B) {
                    if (VOL->ComplexityEstimation.opaque) VOL->ComplexityEstimation.dcecs_opaque =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.transparent) VOL->ComplexityEstimation.dcecs_transparent =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_cae) VOL->ComplexityEstimation.dcecs_intra_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter_cae) VOL->ComplexityEstimation.dcecs_inter_cae =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.no_update) VOL->ComplexityEstimation.dcecs_no_update =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.upsampling) VOL->ComplexityEstimation.dcecs_upsampling =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.intra_blocks) VOL->ComplexityEstimation.dcecs_intra_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.not_coded_blocks) VOL->ComplexityEstimation.dcecs_not_coded_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_coefs) VOL->ComplexityEstimation.dcecs_dct_coefs =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_lines) VOL->ComplexityEstimation.dcecs_dct_lines =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_symbols) VOL->ComplexityEstimation.dcecs_vlc_symbols =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_bits) VOL->ComplexityEstimation.dcecs_vlc_bits =  mp4_GetBits9(pInfo, 4);
                    if (VOL->ComplexityEstimation.inter_blocks) VOL->ComplexityEstimation.dcecs_inter_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter4v_blocks) VOL->ComplexityEstimation.dcecs_inter4v_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.apm) VOL->ComplexityEstimation.dcecs_apm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.npm) VOL->ComplexityEstimation.dcecs_npm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.forw_back_mc_q) VOL->ComplexityEstimation.dcecs_forw_back_mc_q =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel2) VOL->ComplexityEstimation.dcecs_halfpel2 =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel4) VOL->ComplexityEstimation.dcecs_halfpel4 =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.interpolate_mc_q) VOL->ComplexityEstimation.dcecs_interpolate_mc_q =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.sadct) VOL->ComplexityEstimation.dcecs_sadct =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.quarterpel) VOL->ComplexityEstimation.dcecs_quarterpel =  mp4_GetBits9(pInfo, 8);
                }
                if (VOP->coding_type == MP4_VOP_TYPE_S && VOL->sprite_enable == MP4_SPRITE_STATIC) {
                    if (VOL->ComplexityEstimation.intra_blocks) VOL->ComplexityEstimation.dcecs_intra_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.not_coded_blocks) VOL->ComplexityEstimation.dcecs_not_coded_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_coefs) VOL->ComplexityEstimation.dcecs_dct_coefs =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.dct_lines) VOL->ComplexityEstimation.dcecs_dct_lines =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_symbols) VOL->ComplexityEstimation.dcecs_vlc_symbols =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.vlc_bits) VOL->ComplexityEstimation.dcecs_vlc_bits =  mp4_GetBits9(pInfo, 4);
                    if (VOL->ComplexityEstimation.inter_blocks) VOL->ComplexityEstimation.dcecs_inter_blocks =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.inter4v_blocks) VOL->ComplexityEstimation.dcecs_inter4v_blocks =  mp4_GetBits(pInfo, 8);
                    if (VOL->ComplexityEstimation.apm) VOL->ComplexityEstimation.dcecs_apm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.npm) VOL->ComplexityEstimation.dcecs_npm =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.forw_back_mc_q) VOL->ComplexityEstimation.dcecs_forw_back_mc_q =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel2) VOL->ComplexityEstimation.dcecs_halfpel2 =  mp4_GetBits9(pInfo, 8);
                    if (VOL->ComplexityEstimation.halfpel4) VOL->ComplexityEstimation.dcecs_halfpel4 =  mp4_GetBits(pInfo, 8);
                    if (VOL->ComplexityEstimation.interpolate_mc_q) VOL->ComplexityEstimation.dcecs_interpolate_mc_q =  mp4_GetBits9(pInfo, 8);
                }
            }
        }
        VOP->intra_dc_vlc_thr = mp4_GetBits9(pInfo, 3);
        if (VOL->interlaced) {
            VOP->top_field_first = mp4_GetBit(pInfo);
            VOP->alternate_vertical_scan_flag = mp4_GetBit(pInfo);
        }
    }
    if ((VOL->sprite_enable == MP4_SPRITE_STATIC || VOL->sprite_enable == MP4_SPRITE_GMC) && VOP->coding_type == MP4_VOP_TYPE_S) {
        if (VOL->sprite_warping_points > 0)
            if (mp4_Sprite_Trajectory(pInfo) != MP4_STATUS_OK)
                return MP4_STATUS_PARSE_ERROR;
        if (VOL->sprite_brightness_change) {
            code = mp4_ShowBits9(pInfo, 4);
            if (code == 15) {
                mp4_FlushBits(pInfo, 4);
                VOP->brightness_change_factor = 625 + mp4_GetBits(pInfo, 10);
            } else if (code == 14) {
                mp4_FlushBits(pInfo, 4);
                VOP->brightness_change_factor = 113 + mp4_GetBits9(pInfo, 9);
            } else if (code >= 12) {
                mp4_FlushBits(pInfo, 3);
                code = mp4_GetBits9(pInfo, 7);
                VOP->brightness_change_factor = (code < 64) ? code - 112 : code - 15;
            } else if (code >= 8) {
                mp4_FlushBits(pInfo, 2);
                code = mp4_GetBits9(pInfo, 6);
                VOP->brightness_change_factor = (code < 32) ? code - 48 : code - 15;
            } else {
                mp4_FlushBits(pInfo, 1);
                code = mp4_GetBits9(pInfo, 5);
                VOP->brightness_change_factor = (code < 16) ? code - 16 : code - 15;
            }
        } else
            VOP->brightness_change_factor = 0;
        if (VOL->sprite_enable == MP4_SPRITE_STATIC)
            return MP4_STATUS_OK;
    }
    if (VOL->shape != MP4_SHAPE_TYPE_BINARYONLY) {
        VOP->quant = mp4_GetBits9(pInfo, VOL->quant_precision);
        if (VOL->shape == MP4_SHAPE_TYPE_GRAYSCALE) {
            Ipp32s   ac, i;

            ac = mp4_aux_comp_count[VOL->shape_extension];
            for (i = 0; i < ac; i ++)
                VOP->alpha_quant[i] = mp4_GetBits9(pInfo, 6);
        }
        if (VOP->coding_type != MP4_VOP_TYPE_I) {
            VOP->fcode_forward = mp4_GetBits9(pInfo, 3);
            if (VOP->fcode_forward == 0)
                return MP4_STATUS_PARSE_ERROR;
        }
        if (VOP->coding_type == MP4_VOP_TYPE_B) {
            VOP->fcode_backward = mp4_GetBits9(pInfo, 3);
            if (VOP->fcode_backward == 0)
                return MP4_STATUS_PARSE_ERROR;
        }
        if (!VOL->scalability) {
            if (VOL->shape != MP4_SHAPE_TYPE_RECTANGULAR && VOP->coding_type != MP4_VOP_TYPE_I)
                VOP->shape_coding_type = mp4_GetBit(pInfo);
        } else {
            //f if (VOL->enhancement_type) {
                //f VOP->load_backward_shape = mp4_GetBit(pInfo);
                //f if (VOP->load_backward_shape) {
                    //f shape
                //f }
            //f }
            VOP->ref_select_code = mp4_GetBits9(pInfo, 2);
        }
    }
    return MP4_STATUS_OK;
}

Status MP4Splitter::ParseESDSHeader(T_trak_data *pTrak, Ipp32u nTrack)
{
  Ipp32u      j(0), k(0);
  mp4_Info    vInfo;
  VideoStreamInfo *pVideoInfo = (VideoStreamInfo *)(m_pInfo->m_ppTrackInfo[nTrack]->m_pStreamInfo);
  T_esds_data sEsds = pTrak->mdia.minf.stbl.stsd.table->esds;

  vInfo.VisualObject.verid = 1;
  // find start code VideoObjectLayer and parse width and height
  for (j = 0; j < sEsds.decoderConfigLen; j++ ) {
    if (0x00 == sEsds.decoderConfig[j + 0] &&
        0x00 == sEsds.decoderConfig[j + 1] &&
        0x01 == sEsds.decoderConfig[j + 2]) {
      if (MP4_VISUAL_OBJECT_SC == sEsds.decoderConfig[j + 3]) {
        vInfo.bufptr= sEsds.decoderConfig + j + 4;
        vInfo.bitoff = 0;
        if (MP4_STATUS_NOTSUPPORT == mp4_Parse_VisualObject(&vInfo))
          return UMC_ERR_FAILED; // unsupported Video Object
        j += 5;
        continue;
      }
      if (MP4_VIDEO_OBJECT_LAYER_MIN_SC <= sEsds.decoderConfig[j + 3] &&
          MP4_VIDEO_OBJECT_LAYER_MAX_SC >= sEsds.decoderConfig[j + 3]) {
        vInfo.bufptr = sEsds.decoderConfig + j + 4;
        vInfo.bitoff = 0;
        if (MP4_STATUS_NOTSUPPORT == mp4_Parse_VideoObject(&vInfo))
          return UMC_ERR_FAILED; // unsupported Video Object
        ////////////////////////////////////////////////////////////////////////////////////////
        // set aspect ratio info
        switch (vInfo.VisualObject.VideoObject.aspect_ratio_info) {
          case MP4_ASPECT_RATIO_FORBIDDEN:
          case MP4_ASPECT_RATIO_1_1:
            pVideoInfo->videoInfo.m_iSAWidth = 1;
            pVideoInfo->videoInfo.m_iSAHeight = 1;
            break;
          case MP4_ASPECT_RATIO_12_11:
            pVideoInfo->videoInfo.m_iSAWidth = 12;
            pVideoInfo->videoInfo.m_iSAHeight = 11;
            break;
          case MP4_ASPECT_RATIO_10_11:
            pVideoInfo->videoInfo.m_iSAWidth = 10;
            pVideoInfo->videoInfo.m_iSAHeight = 11;
            break;
          case MP4_ASPECT_RATIO_16_11:
            pVideoInfo->videoInfo.m_iSAWidth = 16;
            pVideoInfo->videoInfo.m_iSAHeight = 11;
            break;
          case MP4_ASPECT_RATIO_40_33:
            pVideoInfo->videoInfo.m_iSAWidth = 40;
            pVideoInfo->videoInfo.m_iSAHeight = 33;
            break;
          default:
            pVideoInfo->videoInfo.m_iSAWidth = vInfo.VisualObject.VideoObject.aspect_ratio_info_par_width;
            pVideoInfo->videoInfo.m_iSAHeight = vInfo.VisualObject.VideoObject.aspect_ratio_info_par_height;
        }
        pVideoInfo->videoInfo.m_iWidth = vInfo.VisualObject.VideoObject.width;
        pVideoInfo->videoInfo.m_iHeight = vInfo.VisualObject.VideoObject.height;

        if(0 == vInfo.VisualObject.VideoObject.interlaced)
            pVideoInfo->videoInfo.m_picStructure = PS_PROGRESSIVE;
        else if(2 == vInfo.VisualObject.VideoObject.interlaced)
            pVideoInfo->videoInfo.m_picStructure = PS_TOP_FIELD_FIRST;
        else if(3 == vInfo.VisualObject.VideoObject.interlaced)
            pVideoInfo->videoInfo.m_picStructure = PS_BOTTOM_FIELD_FIRST;

        if (pVideoInfo->videoInfo.m_picStructure != PS_PROGRESSIVE) {
          Status umcRes;
          TrackIndex *pIndex = &m_pTrackIndex[nTrack];
          IndexEntry m_Frame;
          Ipp8u *p_frame;

          // get first frame (index is already set to First frame)
          umcRes = pIndex->Get(m_Frame);
          UMC_CHECK_STATUS(umcRes)

          p_frame = (Ipp8u *)ippMalloc((Ipp32u)m_Frame.uiSize);
          UMC_CHECK_PTR(p_frame)

          m_ReaderMutex.Lock();
          m_pReader->SetPosition((Ipp64u)m_Frame.stPosition);
          m_pReader->GetData(p_frame, m_Frame.uiSize);
          m_ReaderMutex.Unlock();

          vInfo.bitoff = 0;
          vInfo.bufptr = p_frame;

          for (k = 0; k < m_Frame.uiSize; k++) {
            if (0x00 == vInfo.bufptr[k + 0] &&
                0x00 == vInfo.bufptr[k + 1] &&
                0x01 == vInfo.bufptr[k + 2] &&
                0xb6 == vInfo.bufptr[k + 3]) {
              vInfo.bufptr += (k + 4);
              mp4_Status mp4Status = mp4_Parse_VideoObjectPlane(&vInfo);
              if (MP4_STATUS_OK == mp4Status) {
                if (pVideoInfo->videoInfo.m_picStructure != PS_PROGRESSIVE) {
                  if (1 == vInfo.VisualObject.VideoObject.VideoObjectPlane.top_field_first) {
                    pVideoInfo->videoInfo.m_picStructure = PS_TOP_FIELD_FIRST;
                  } else {
                    pVideoInfo->videoInfo.m_picStructure = PS_BOTTOM_FIELD_FIRST;
                  }//if
                  break;
                }
              }
            }//if
          }
          ippFree(p_frame);
        }//if
        return UMC_OK;
      }//if
    }//if
  }// for

  // headers was not found
  return UMC_ERR_FAILED;
}//Status ParseESDSHeader(T_esds_data &sEsds, VideoStreamInfo* pVideoInfo)


// object extends base bitstream reader for h264 video
// it removes start code emulation prevention bytes
class H264BitstreamReader : public BitstreamReader
{
public:
    H264BitstreamReader(void)
    {
        m_iZeroes  = 0;
    }

    virtual void Init(Ipp8u *pStream, size_t len)
    {
        MemoryReaderParams params;

        m_iZeroes = 0;
        params.m_pBuffer = pStream;
        params.m_iBufferSize = len;
        m_reader.Init(&params);
        BitstreamReader::Init(&m_reader);
    }

protected:
    virtual void FillCache()
    {
        Ipp8u  iWord[2];
        size_t iSize = 2;

        while (56 >= m_iCacheSize)
        {
            if(m_pReader->CacheData(&iWord[0], iSize, 0) == UMC_ERR_END_OF_STREAM)
                return;

            // remove preventing byte
            if (m_iZeroes >= 2 && 0x03 == iWord[0] && 0x00 == (iWord[1] & 0xfc))
            {
                iSize = 1;
                m_pReader->MovePosition(1);
                m_pReader->CacheData(&iWord[0], iSize, 0);
                m_iZeroes = 0;
            }

            // count sequential zeroes
            if (0 == iWord[0])
                m_iZeroes++;
            else
                m_iZeroes = 0;

            m_iCache = (m_iCache << 8) | iWord[0];
            m_iCacheSize += 8;
            m_pReader->MovePosition(1);
        }
    }

    MemoryReader m_reader;
    // amount of sequential zeroes
    Ipp32u m_iZeroes;
};

Status MP4Splitter::ParseAVCCHeader(T_trak_data *pTrak,Ipp32u nTrack)
{
  H264BitstreamReader stream;
  Ipp32u profile_idc = 0;
  Ipp32u aspect_ratio_idc = 0;
  Ipp32u pic_order_cnt_type = 0;
  bool bTopFirst = true;
  Ipp32u frame_mbs_only_flag = 0;
  VideoStreamInfo *pVideoInfo = (VideoStreamInfo *)(m_pInfo->m_ppTrackInfo[nTrack]->m_pStreamInfo);
  T_xvcC_data sAvcc = pTrak->mdia.minf.stbl.stsd.table->avcC;

  // check error(s)
  if (NULL == sAvcc.decoderConfig)
    return UMC_OK;

  // initialize stream reader
  stream.Init(sAvcc.decoderConfig, pTrak->mdia.minf.stbl.stsd.table->avcC.decoderConfigLen);

  // configuration_version
  stream.GetBits(8);
  // AVCProfileIndication
  stream.GetBits(8);
  // profile_compatibility
  stream.GetBits(8);
  // AVCLevelIndication
  stream.GetBits(8);

  // reserved '111111'
  stream.GetBits(6);
  // lengthSizeMinusOne
  stream.GetBits(2);
  // reserved '111'
  stream.GetBits(3);
  // numOfSPSes
  Ipp32u numOfSPSes = stream.GetBits(5);
  if (numOfSPSes)
  {
    // SPSlen
    stream.GetBits(16);
    /*** SPS NALU parsing ***/

    // nalu_idc nalu_type(5)
    stream.GetBits(8);

    profile_idc = stream.GetBits(8);
    // constraint set flags
    stream.GetBits(4);
    // reserved 4 bits
    stream.GetBits(4);
    // level idc
    stream.GetBits(8);
    // seq parameter set id
    stream.GetUE();

    if ((100 == profile_idc) ||
        (110 == profile_idc) ||
        (122 == profile_idc) ||
        (244 == profile_idc) ||
        (44 == profile_idc) ||
        (83 == profile_idc) ||
        (86 == profile_idc)) {
      Ipp32u chroma_format_idc;
      Ipp32u seq_scaling_matrix_present_flag;

      // chroma_format_idc
      chroma_format_idc = stream.GetUE();
      if(chroma_format_idc == 0)
          pVideoInfo->videoInfo.m_colorFormat = GRAY;
      else if(chroma_format_idc == 1)
          pVideoInfo->videoInfo.m_colorFormat = YUV420;
      else if(chroma_format_idc == 2)
          pVideoInfo->videoInfo.m_colorFormat = YUV422;
      if (3 == chroma_format_idc) {
          pVideoInfo->videoInfo.m_colorFormat = YUV444;
        // residual_colour_transform_flag
        stream.GetBits(1);
      }
      // bit_depth_luma_minus8
      stream.GetUE();
      // bit_depth_chroma_minus8
      stream.GetUE();
      // qpprime_y_zero_transform_bypass_flag
      stream.GetBits(1);
      // seq_scaling_matrix_present_flag
      seq_scaling_matrix_present_flag = stream.GetBits(1);
      if (seq_scaling_matrix_present_flag) {
        Ipp32s i;
        for (i = 0; i < 8; i += 1) {
          Ipp32u seq_scaling_list_present_flag;

              // seq_scaling_list_present_flag
          seq_scaling_list_present_flag = stream.GetBits(1);
          if (seq_scaling_list_present_flag) {
            Ipp32s iScalingListSize;
            Ipp32s lastScale, nextScale;
            Ipp32s j;

            if (6 > i)
              iScalingListSize = 16;
            else
              iScalingListSize = 64;

            lastScale = 8;
            nextScale = 8;
            for (j = 0; j < iScalingListSize; j += 1) {
              if (nextScale) {
                Ipp32s delta_scale;

                delta_scale = stream.GetSE();
                nextScale = (lastScale + delta_scale + 256) % 256;
                lastScale = nextScale;
              }
            }
          }
        }
      }
    }

    // log2 max frame num minus4
    stream.GetUE();
    // pic order cnt type
    pic_order_cnt_type = stream.GetUE();
    if (0 == pic_order_cnt_type) {
      // log2 max pic order cnt lsb minus4
      stream.GetUE();
    } else if (1 == pic_order_cnt_type)
    {
      Ipp32s num_ref_frames_in_pic_order_cnt_cycle;
      Ipp32s i;

      // delta_pic_order_always_zero_flag
      stream.GetBits(1);
      // offset_for_non_ref_pic
      stream.GetSE();
      // offset_for_top_to_bottom_field
      bTopFirst = stream.GetSE() >= 0;
      // num_ref_frames_in_pic_order_cnt_cycle
      num_ref_frames_in_pic_order_cnt_cycle = stream.GetUE();
      for (i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
        // offset_for_ref_frame
        stream.GetSE();
    }
    // num_ref_frames
    stream.GetUE();
    // gaps_in_frame_num_value_allowed_flag
    stream.GetBits(1);
    // pic_width_in_mbs_minus1
    stream.GetUE();
    // pic_height_in_map_units_minus1
    stream.GetUE();

    // frame_mbs_only_flag
    frame_mbs_only_flag = stream.GetBits(1);
    if (!frame_mbs_only_flag) {
      // mb_adaptive_frame_field_flag
      stream.GetBits(1);
      if (bTopFirst) {
        pVideoInfo->videoInfo.m_picStructure = PS_TOP_FIELD_FIRST;
      } else {
        pVideoInfo->videoInfo.m_picStructure = PS_BOTTOM_FIELD_FIRST;
      }
    } else
    {
      pVideoInfo->videoInfo.m_picStructure = PS_PROGRESSIVE;
    }

    // direct_8x8_inference_flag
    stream.GetBits(1);
    // frame_cropping_flag
    if(stream.GetBits(1))
    {
      // frame_crop_left_offset
      stream.GetUE();
      // frame_crop_right_offset
      stream.GetUE();
      // frame_crop_top_offset
      stream.GetUE();
      // frame_crop_bottom_offset
      stream.GetUE();
    }

    // vui_parameters_present_flag
    if(stream.GetBits(1))
    {
      // aspect_ratio_info_present_flag
      if(stream.GetBits(1))
      {
        Ipp32u pixel_aspect_ratio_width = 0;
        Ipp32u pixel_aspect_ratio_height = 0;
        // aspect_ratio_idc
        aspect_ratio_idc = stream.GetBits(8);
        // set aspect ratio info
        switch (aspect_ratio_idc) {
          case H264_ASPECT_RATIO_12_11:
            pixel_aspect_ratio_width = 12;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_10_11:
            pixel_aspect_ratio_width = 10;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_16_11:
            pixel_aspect_ratio_width = 16;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_40_33:
            pixel_aspect_ratio_width = 40;
            pixel_aspect_ratio_height = 33;
            break;
          case H264_ASPECT_RATIO_24_11:
            pixel_aspect_ratio_width = 24;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_20_11:
            pixel_aspect_ratio_width = 20;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_32_11:
            pixel_aspect_ratio_width = 32;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_80_33:
            pixel_aspect_ratio_width = 80;
            pixel_aspect_ratio_height = 33;
            break;
          case H264_ASPECT_RATIO_18_11:
            pixel_aspect_ratio_width = 18;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_15_11:
            pixel_aspect_ratio_width = 15;
            pixel_aspect_ratio_height = 11;
            break;
          case H264_ASPECT_RATIO_64_33:
            pixel_aspect_ratio_width = 64;
            pixel_aspect_ratio_height = 33;
            break;
          case H264_ASPECT_RATIO_160_99:
            pixel_aspect_ratio_width = 160;
            pixel_aspect_ratio_height = 99;
            break;
          case H264_ASPECT_RATIO_4_3:
            pixel_aspect_ratio_width = 4;
            pixel_aspect_ratio_height = 3;
            break;
          case H264_ASPECT_RATIO_3_2:
            pixel_aspect_ratio_width = 3;
            pixel_aspect_ratio_height = 2;
            break;
          case H264_ASPECT_RATIO_2_1:
            pixel_aspect_ratio_width = 2;
            pixel_aspect_ratio_height = 1;
            break;
          case H264_EXTENDED_SAR:
            pixel_aspect_ratio_width = stream.GetBits(16);
            pixel_aspect_ratio_height = stream.GetBits(16);
            break;
          case H264_ASPECT_RATIO_UNSPECIFIED:
          case H264_ASPECT_RATIO_1_1:
          default:
            pixel_aspect_ratio_width = 1;
            pixel_aspect_ratio_height = 1;
        }
        pVideoInfo->videoInfo.m_iSAWidth = pixel_aspect_ratio_width;
        pVideoInfo->videoInfo.m_iSAHeight = pixel_aspect_ratio_height;
      }
      // overscan_info_present_flag
      if(stream.GetBits(1))
        stream.GetBits(1);

      // video_signal_type_present_flag
      if(stream.GetBits(1))
      {
          stream.SkipBits(4);
          // colour_description_present_flag
          if(stream.GetBits(1))
            stream.SkipBits(24);
      }

      // chroma_loc_info_present_flag
      if(stream.GetBits(1))
      {
        stream.GetUE();
        stream.GetUE();
      }

      // timing_info_present_flag
      if(stream.GetBits(1))
      {
          Ipp32u num_units_in_tick = stream.GetBits(32);
          Ipp32u time_scale        = stream.GetBits(32);
          pVideoInfo->fFramerate = ((Ipp32f)time_scale)/(num_units_in_tick*2);
      }
    }

  }

  return UMC_OK;
}//Status ParseAVCCHeader(T_avcC_data &sAVCc, VideoStreamInfo* pVideoInfo)

Ipp32u CalcAVCHeadLen(Ipp8u *pSrc)
{
    Ipp32u i = 0, len = 0, count = 0;
    Ipp8u num = *pSrc++; //num of SPSes

    num &= 0x1F;   // remove first '111' bits
    for (i = 0; i < num; i++)
    {
        len = (pSrc[0] << 8) | pSrc[1];  //sps len
        pSrc += len + 2;
        count += len + 3;   //3 bytes reserved for 000001 SC
    }

    num = *pSrc++; //num of PPSes

    for (i = 0; i < num; i++)
    {
        len = (pSrc[0] << 8) | pSrc[1];  //pps len
        pSrc += len + 2;
        count += len + 3;   //3 bytes reserved for 000001 SC
    }
    return count;
}

Status MP4Splitter::SaveHeader(MediaData **pHeader, T_xvcC_data *pXvcc)
{
    Ipp32u i = 0, len = 0;
    MediaData *pAVCHeader = NULL;
    Ipp8u *pDst = NULL;
    Ipp32u dstLen = 0;

    Ipp8u *phead = pXvcc->decoderConfig;
    Ipp32u headlen = pXvcc->decoderConfigLen;

    if (headlen == 0)
        return UMC_OK;

    // configuration_version
    // AVCProfileIndication
    // profile_compatibility
    // AVCLevelIndication
    phead += 4;

    Ipp8u lengthSizeMinusOne = *phead++;
    lengthSizeMinusOne &= 0x3;   // remove first '111111' bits
    m_AVC_NALUlen_fld = lengthSizeMinusOne + 1;   // 1, 2 or 4

    len = CalcAVCHeadLen(phead);
    pAVCHeader = new MediaData();
    pAVCHeader->Alloc(len);
    UMC_CHECK_PTR(pAVCHeader)
    pDst = (Ipp8u *)pAVCHeader->GetDataPointer();

    Ipp8u numOfSPSes = *phead++;
    numOfSPSes &= 0x1F;   // remove first '111' bits

    /* copy spses */
    for (i = 0; i < numOfSPSes; i++)
    {
        Ipp32u spslen = (phead[0] << 8) | phead[1];
        phead += 2;
        pDst[0] = (Ipp8u)0x00; pDst[1] = (Ipp8u)0x00; pDst[2] = (Ipp8u)0x01;
        pDst += 3;
        memcpy(pDst, phead, spslen);
        phead += spslen;
        pDst += spslen;
        dstLen += spslen + 3;
    }

    Ipp8u numOfPPSes = *phead++;

    /* copy ppses */
    for (i = 0; i < numOfPPSes; i++)
    {
        Ipp32u ppslen = (phead[0] << 8) | phead[1];
        phead += 2;
        pDst[0] = (Ipp8u)0x00; pDst[1] = (Ipp8u)0x00; pDst[2] = (Ipp8u)0x01;
        pDst += 3;
        memcpy(pDst, phead, ppslen);
        phead += ppslen;
        pDst += ppslen;
        dstLen += ppslen + 3;
    }

    pAVCHeader->SetDataSize(dstLen);
    pAVCHeader->m_fPTSStart = pAVCHeader->m_fPTSEnd = 0;
    *pHeader = pAVCHeader;

    return UMC_OK;
}

#endif
