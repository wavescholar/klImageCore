/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class VideoEncoderMPEG4
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include <math.h>
#include "mp4_enc.h"

namespace MPEG4_ENC
{


mp4_Param::mp4_Param() {
    ippsSet_8u(0, (Ipp8u*)(this), sizeof(mp4_Param));
    quantIVOP = 4;
    quantPVOP = 4;
    quantBVOP = 6;
    IVOPdist = 300;
    PVOPsearchWidth = -15;
    PVOPsearchHeight = 15;
    BVOPsearchWidthForw = 15;
    BVOPsearchHeightForw = 15;
    BVOPsearchWidthBack = 15;
    BVOPsearchHeightBack = 15;
    MEalgorithm = 1;
    MEaccuracy = 2;
    obmc_disable = 1;
    RoundingControl = 1;
    RateControl = 1;
    SceneChangeThreshold = 45;
    bsBuffer = (Ipp8u*)1;
    bsBuffSize = 1; // encoder will not allocate buffer
    padType = 0; // set 1 for QuickTime(tm) and 2 for DivX (tm) v. >= 5
    TimeResolution = 30;
    TimeIncrement = 1;
    //numThreads = 0;
    profile_and_level = 1;
    aspect_ratio_width = 1;
    aspect_ratio_height = 1;
    load_intra_quant_mat = 0;
    load_intra_quant_mat_len = 0;
    load_nonintra_quant_mat = 0;
    load_nonintra_quant_mat_len = 0;

}

void VideoEncoderMPEG4::Close()
{
    if (mIsInit) {
        // free
        if (MBinfo)
            delete [] MBinfo;
        if (mbsAlloc && cBS.mBuffer)
            ippFree(cBS.mBuffer);
        if (mBuffer_1)
            ippFree(mBuffer_1);
        if (mBuffer_2)
            ippFree(mBuffer_2);
#ifdef USE_ME_SADBUFF
        if (mMEfastSAD)
            ippFree(mMEfastSAD);
#endif
        if (mQuantInvIntraSpec)
            ippFree(mQuantInvIntraSpec);
        if (mQuantInvInterSpec)
            ippFree(mQuantInvInterSpec);
        if (mQuantIntraSpec)
            ippFree(mQuantIntraSpec);
        if (mQuantInterSpec)
            ippFree(mQuantInterSpec);
        if (mWarpSpec)
            ippFree(mWarpSpec);
        if (mGMCPredY)
            ippFree(mGMCPredY);
        if (mMBpos)
            ippFree(mMBpos);
        if (mMBquant)
            ippFree(mMBquant);
        if (mMBpredMV)
            ippFree(mMBpredMV);
        if (mFrame) {
            //for (Ipp32s i = 0; i < mPlanes; i ++) {
            //    if (mFrame[i].ap)
            //        ippFree(mFrame[i].ap);
            //}
            delete [] mFrame;
        }
#ifdef USE_CV_GME
        if (mPyramid[0]) {
            if (mPyramid[0]->pState) {
                ippiPyramidLayerDownFree_8u_C1R((IppiPyramidDownState_8u_C1R*)mPyramid[0]->pState);
                for (Ipp32s i = 1; i <= mPyramid[0]->level; i ++) {
                    if (mPyramid[0]->pImage[i])
                        ippiFree(mPyramid[0]->pImage[i]);
                }
            }
            ippiPyramidFree(mPyramid[0]);
        }
        if (mPyramid[1]) {
            if (mPyramid[1]->pState) {
                ippiPyramidLayerDownFree_8u_C1R((IppiPyramidDownState_8u_C1R*)mPyramid[1]->pState);
                for (Ipp32s i = 1; i <= mPyramid[1]->level; i ++) {
                    if (mPyramid[1]->pImage[i])
                        ippiFree(mPyramid[1]->pImage[i]);
                }
            }
            ippiPyramidFree(mPyramid[1]);
        }
        if (mOptFlowState)
            ippiOpticalFlowPyrLKFree_8u_C1R(mOptFlowState);
        if (mOptFlowPoints)
            ippFree(mOptFlowPoints);
        if (mOptFlowPtSts)
            ippFree(mOptFlowPtSts);
        if (mOptFlowPatchDiff)
            ippFree(mOptFlowPatchDiff);
        if (mOptFlowMask)
            ippFree(mOptFlowMask);
#endif
#ifdef _OMP_KARABAS
        if (mSliceMT)
            delete [] mSliceMT;
        if (MBinfoMT)
            ippsFree(MBinfoMT);
        if (mCurRowMT)
            delete [] mCurRowMT;
        if (mBuffer)
            ippsFree(mBuffer);
#ifdef _OPENMP
        if (mLockMT) {
            for (Ipp32s i = 0; i < mNumMacroBlockPerCol; i ++)
                omp_destroy_lock(mLockMT+i);
        }
        delete [] mLockMT;
#endif
#endif // _OMP_KARABAS
    }
    mIsInit = false;
}

Ipp32s VideoEncoderMPEG4::Init(mp4_Param *par)
{
    Ipp32s  i, j, specSize, intBuffSize;
    IppStatus stsAlloc = ippStsNoErr;

    // check parameters correctness
    if (par->Width < 1 || par->Width > 8191)
        return MP4_STS_ERR_PARAM;
    if (par->Width & 1)
        return MP4_STS_ERR_PARAM;
    if (par->Height < 1 || par->Height > 8191)
        return MP4_STS_ERR_PARAM;
    if (par->Height & 1)
        return MP4_STS_ERR_PARAM;
    if (par->RateControl == 0) {
        if (par->quantIVOP < 1 || par->quantIVOP > 31)
            return MP4_STS_ERR_PARAM;
        if (par->quantPVOP < 1 || par->quantPVOP > 31)
            return MP4_STS_ERR_PARAM;
        if (par->quantBVOP < 1 || par->quantBVOP > 31)
            return MP4_STS_ERR_PARAM;
    }
    if (par->IVOPdist < 1)
        return MP4_STS_ERR_PARAM;
    if (par->padType < 0 || par->padType > 2)
        return MP4_STS_ERR_PARAM;
    if (par->RateControl != 0 && par->BitRate <= 0)
        return MP4_STS_ERR_PARAM;
    if (par->SceneChangeThreshold < 0)
        return MP4_STS_ERR_PARAM;
    if (par->short_video_header) {
        if (par->TimeResolution != 30000)
            return MP4_STS_ERR_PARAM;
        if ((par->TimeIncrement % 1001) || par->TimeIncrement < 1001 || par->TimeIncrement > 256 * 1001)
            return MP4_STS_ERR_PARAM;
    } else {
        if (par->TimeResolution < 1 || par->TimeResolution > 65536)
            return MP4_STS_ERR_PARAM;
        if (par->TimeIncrement < 1 || par->TimeIncrement > 65536)
            return MP4_STS_ERR_PARAM;
        if (par->BVOPdist < 0)
            return MP4_STS_ERR_PARAM;
        if (par->resync && par->VideoPacketLenght <= 0)
            return MP4_STS_ERR_PARAM;
        if (par->data_partitioned && par->interlaced)
            return MP4_STS_ERR_PARAM;
        if (par->sprite_enable == MP4_SPRITE_STATIC) {
            if (par->no_of_sprite_warping_points < 0 || par->no_of_sprite_warping_points > 4)
                return MP4_STS_ERR_PARAM;
        }
        if (par->sprite_enable == MP4_SPRITE_GMC) {
            if (par->no_of_sprite_warping_points < 0 || par->no_of_sprite_warping_points > 3)
                return MP4_STS_ERR_PARAM;
        }
        if (par->sprite_enable == MP4_SPRITE_STATIC || par->sprite_enable == MP4_SPRITE_GMC) {
            if (par->sprite_warping_accuracy < 0 || par->sprite_warping_accuracy > 3)
                return MP4_STS_ERR_PARAM;
        }
        if (par->sprite_enable == MP4_SPRITE_STATIC) {
            if (par->sprite_left_coordinate < -4096 || par->sprite_left_coordinate > 4095)
                return MP4_STS_ERR_PARAM;
            if (par->sprite_left_coordinate & 1)
                return MP4_STS_ERR_PARAM;
            if (par->sprite_top_coordinate < -4096 || par->sprite_top_coordinate > 4095)
                return MP4_STS_ERR_PARAM;
            if (par->sprite_top_coordinate & 1)
                return MP4_STS_ERR_PARAM;
            if (par->sprite_width < 1 || par->sprite_width > 8191)
                return MP4_STS_ERR_PARAM;
            if (par->sprite_height < 1 || par->sprite_height > 8191)
                return MP4_STS_ERR_PARAM;
        }
    }
    Close();
    ippsSet_8u(0, (Ipp8u*)this, sizeof(VideoEncoderMPEG4));
    mNumOfFrames = par->NumOfFrames;
    mNumOfFrames = -1;
    mPadType = par->padType;
    mIVOPdist = par->IVOPdist;
    mBVOPdist = par->BVOPdist;
    if (par->sprite_enable == MP4_SPRITE_STATIC)
        mBVOPdist = 0;
    if (mBVOPdist >= mIVOPdist)
        mBVOPdist = mIVOPdist - 1;
    if (mIVOPdist % (mBVOPdist + 1) != 0)
        mIVOPdist = (mIVOPdist + mBVOPdist) / (mBVOPdist + 1) * (mBVOPdist + 1);
    if (mIVOPdist == 1)
        mBVOPdist = 0;
    mMEflags = ME_USE_MVWEIGHT | ME_USE_MANYPRED | ME_SUBPEL_FINE /*| RD_MODE_DECISION | RD_TRELLIS*/;
    mMEmethod = par->MEalgorithm & 15;
    if (par->MEaccuracy == 2)
        mMEflags |= ME_HP;
    else if (par->MEaccuracy == 4)
        mMEflags |= ME_QP;
    if (par->ME4mv)
        mMEflags |= ME_4MV;
    if (par->MEalgorithm & ME_HP_FAST)
        mMEflags |= ME_HP_FAST;
    if (par->MEalgorithm & ME_CHROMA)
        mMEflags |= ME_CHROMA;
    if (par->MEalgorithm & RD_MODE_DECISION)
        mMEflags |= RD_MODE_DECISION;
    if (par->MEalgorithm & RD_TRELLIS)
        mMEflags |= RD_TRELLIS;
    mReconstructAlways = par->calcPSNR;
    mRoundingControl = par->RoundingControl & 1;
    mInsertGOV = par->insertGOV;
    GOV.closed_gov = mBVOPdist == 0 ? 1 : 0;
    mRepeatHeaders = par->repeatHeaders;
    mFrameCount = 0;
    mLastIVOP = -mIVOPdist;
    // setup MPEG-4 headers variables
    //memset(&VOS, 0, sizeof(VOS));
    //memset(&VO, 0, sizeof(VO));
    //memset(&VOL, 0, sizeof(VOL));
    //memset(&GOV, 0, sizeof(GOV));
    //memset(&VOP, 0, sizeof(VOP));
    VOS.profile_and_level_indication = par->profile_and_level;
    if (VOS.profile_and_level_indication == 0)
        VOS.profile_and_level_indication = 1;
    //f TODO - add autoselect and checking of profile_and_level
    if (par->aspect_ratio_width == 1 && par->aspect_ratio_height == 1)
        VOL.aspect_ratio_info = MP4_ASPECT_RATIO_1_1;
    else if (par->aspect_ratio_width == 12 && par->aspect_ratio_height == 11)
        VOL.aspect_ratio_info = MP4_ASPECT_RATIO_12_11;
    else if (par->aspect_ratio_width == 10 && par->aspect_ratio_height == 11)
        VOL.aspect_ratio_info = MP4_ASPECT_RATIO_10_11;
    else if (par->aspect_ratio_width == 16 && par->aspect_ratio_height == 11)
        VOL.aspect_ratio_info = MP4_ASPECT_RATIO_16_11;
    else if (par->aspect_ratio_width == 40 && par->aspect_ratio_height == 33)
        VOL.aspect_ratio_info = MP4_ASPECT_RATIO_40_33;
    else {
        if (par->aspect_ratio_width == 0 || par->aspect_ratio_height == 0) {
            VOL.aspect_ratio_info = MP4_ASPECT_RATIO_1_1;
        } else {
            VOL.aspect_ratio_info = MP4_ASPECT_RATIO_EXTPAR;
            VOL.par_width = par->aspect_ratio_width;
            VOL.par_height = par->aspect_ratio_height;
        }
    }
    VO.is_visual_object_identifier = 1;
    VO.visual_object_verid = 1;
    VO.visual_object_priority = 1;
    VO.visual_object_type = MP4_VISUAL_OBJECT_TYPE_VIDEO;
    VO.video_signal_type = 0;
    //f VO.video_format;
    //f VO.video_range;
    //f VO.colour_description;
    //f VO.colour_primaries;
    //f VO.transfer_characteristics;
    //f VO.matrix_coefficients;
    VOL.short_video_header = par->short_video_header;
    VOL.random_accessible_vol = mIVOPdist == 1;
    VOL.video_object_type_indication = 0; //MP4_VIDEO_OBJECT_TYPE_SIMPLE;
    VOL.is_object_layer_identifier = 1;
    VOL.video_object_layer_verid = 1;
    VOL.video_object_layer_priority = 1;
    VOL.aspect_ratio_info = MP4_ASPECT_RATIO_1_1;
    //f VOL.par_width
    //f VOL.par_height
    VOL.vol_control_parameters = 0;
    //f VOL.chroma_format;
    //f VOL.low_delay;
    //f VOL.vbv_parameters;
    //f VOL.first_half_bit_rate;
    //f VOL.latter_half_bit_rate;
    //f VOL.first_half_vbv_buffer_size;
    //f VOL.latter_half_vbv_buffer_size;
    //f VOL.first_half_vbv_occupancy;
    //f VOL.latter_half_vbv_occupancy;
    VOL.video_object_layer_shape = MP4_SHAPE_TYPE_RECTANGULAR;
    //f VOL.video_object_layer_shape_extension
    VOL.vop_time_increment_resolution = par->TimeResolution;
    VOL.fixed_vop_rate = (par->TimeResolution > par->TimeIncrement) ? 1 : 0;
    VOL.fixed_vop_time_increment = par->TimeIncrement;
    VOL.vop_time_increment_resolution_bits = 1;
    i = VOL.vop_time_increment_resolution - 1;
    while (i >> VOL.vop_time_increment_resolution_bits)
        VOL.vop_time_increment_resolution_bits ++;
    VOL.video_object_layer_width = par->Width;
    VOL.video_object_layer_height = par->Height;
    VOL.interlaced = par->interlaced;
    VOL.obmc_disable = par->obmc_disable;
    VOL.sprite_enable = par->sprite_enable;
    VOL.sprite_width = par->sprite_width;
    VOL.sprite_height = par->sprite_height;
    VOL.sprite_left_coordinate = par->sprite_left_coordinate;
    VOL.sprite_top_coordinate = par->sprite_top_coordinate;
    VOL.no_of_sprite_warping_points = par->no_of_sprite_warping_points;
    VOL.sprite_warping_accuracy = par->sprite_warping_accuracy;
    VOL.sprite_brightness_change = VOL.sprite_enable == MP4_SPRITE_GMC ? 0 : par->sprite_brightness_change;
    VOL.low_latency_sprite_enable = 0;
    VOL.sadct_disable = 1;
    VOL.not_8_bit = 0;
    VOL.quant_precision = 5;
    VOL.bits_per_pixel = 8;
    //f VOL.no_gray_quant_update;
    //f VOL.composition_method;
    //f VOL.linear_composition;
    VOL.quant_type = par->quant_type;
    VOL.load_intra_quant_mat = par->load_intra_quant_mat;
    VOL.load_intra_quant_mat_len = par->load_intra_quant_mat_len;
    VOL.load_nonintra_quant_mat = par->load_nonintra_quant_mat;
    VOL.load_nonintra_quant_mat_len = par->load_nonintra_quant_mat_len;
    //f VOL.quant_mat_grayscale;
    VOL.quarter_sample = (mMEflags & ME_QP) ? 1 : 0;
    VOL.complexity_estimation_disable = 1;
    //f VOL.ComplexityEstimation;
    VOL.resync_marker_disable = par->resync ? 0 : 1;
    mInsertResync = par->resync;
    mVideoPacketLength = par->VideoPacketLenght;
    VOL.data_partitioned = par->data_partitioned;
    VOL.reversible_vlc = (VOL.data_partitioned) ? par->reversible_vlc : 0;
    VOL.newpred_enable = 0;
    //f VOL.requested_upstream_message_type;
    //f VOL.newpred_segment_type;
    VOL.reduced_resolution_vop_enable = 0;
    VOL.scalability = 0;
    //f VOL.ScalabilityParameters;
    VOP.vop_time_increment = 0;
    VOP.intra_dc_vlc_thr = 0; //par->intra_dc_vlc_thr;
    if (VOL.data_partitioned)
        VOL.interlaced = 0;
    if (VOL.interlaced) {
        VOP.top_field_first = par->top_field_first;
        VOP.alternate_vertical_scan_flag = par->alternate_vertical_scan_flag;
        if (par->interlacedME == 0)
            mMEflags |= ME_FIELD;
        else if (par->interlacedME == 1)
            mMEflags |= ME_FRAME;
        else  if (par->interlacedME == 2)
            mMEflags |= ME_FRAME | ME_FIELD;
        //f  if ((mMEflags & ME_CHROMA) && (mMEflags & ME_FIELD)) {
        //    mMEflags &= ~ME_CHROMA;
        //    ErrorMessage("chroma ME is not implemented for field ME");
        //}
    } else {
        mMEflags |= ME_FRAME;
    }
    VOP.vop_rounding_type = 0;
    if (VOL.short_video_header) {
        mBVOPdist = 0;
        VO.colour_primaries = 1;
        VO.transfer_characteristics = 1;
        VO.matrix_coefficients = 6;
        VOL.quant_precision  = 5;
        VOL.video_object_layer_shape = MP4_SHAPE_TYPE_RECTANGULAR;
        VOL.obmc_disable = 1;
        VOL.quant_type = 0;
        VOL.resync_marker_disable = 1;
        VOL.data_partitioned = 0;
        VOL.reversible_vlc = 0;
        VOL.interlaced = 0;
        VOL.complexity_estimation_disable = 1;
        VOL.scalability = 0;
        VOL.not_8_bit = 0;
        VOL.bits_per_pixel = 8;
        VOP.vop_rounding_type = 0;
        VOP.vop_fcode_forward = 1;
        VOP.vop_coded = 1;
        VOP.split_screen_indicator = 0;
        VOP.document_camera_indicator = 0;
        VOP.full_picture_freeze_release = 0;
        if (par->Width == 128 && par->Height == 96) {
            VOP.source_format = 1;
            VOP.num_gobs_in_vop = 6;
            VOP.num_macroblocks_in_gob = 8;
        } else if (par->Width == 176 && par->Height == 144) {
            VOP.source_format = 2;
            VOP.num_gobs_in_vop = 9;
            VOP.num_macroblocks_in_gob = 11;
        } else if (par->Width == 352 && par->Height == 288) {
            VOP.source_format = 3;
            VOP.num_gobs_in_vop = 18;
            VOP.num_macroblocks_in_gob = 22;
        } else if (par->Width == 704 && par->Height == 576) {
            VOP.source_format = 4;
            VOP.num_gobs_in_vop = 18;
            VOP.num_macroblocks_in_gob = 88;
        } else if (par->Width == 1408 && par->Height == 1152) {
            VOP.source_format = 5;
            VOP.num_gobs_in_vop = 18;
            VOP.num_macroblocks_in_gob = 352;
        } else
            return MP4_STS_ERR_PARAM;

        VOP.temporal_reference_increment = VOL.fixed_vop_time_increment / 1001;
        VOL.quarter_sample = 0;
        if (mMEflags & ME_QP) {
            mMEflags &= ~ME_QP;
            mMEflags |= ME_HP;
        }
        mMEflags &= ~ME_4MV;
        Ipp32s sw = IPP_MIN(15, par->Width);
        Ipp32s sh = IPP_MIN(15, par->Height);
        mBVOPsearchHorForw = mBVOPsearchVerForw = mBVOPsearchHorBack = mBVOPsearchVerBack = 0;
        mPVOPsearchHor = par->PVOPsearchWidth;
        mPVOPsearchVer = par->PVOPsearchHeight;
        mp4_ClipR(mPVOPsearchHor, sw);
        mp4_ClipR(mPVOPsearchVer, sh);
        if (mPVOPsearchHor < 0)
            mPVOPsearchHor = sw;
        if (mPVOPsearchVer < 0)
            mPVOPsearchVer = sh;
        if (mPVOPsearchHor == 0 || mPVOPsearchVer == 0)
            mMEflags |= ME_ZERO_MV;
        mPVOPfcodeForw = 1;
        mRoundingControl = 0;
        mInsertGOV = 0;
        mRepeatHeaders = 0;
    } else {
        Ipp32s sw = IPP_MIN(VOL.quarter_sample ? 511 : 1023, par->Width);
        Ipp32s sh = IPP_MIN(VOL.quarter_sample ? 511 : 1023, par->Height);
        mPVOPsearchHor = par->PVOPsearchWidth;
        mPVOPsearchVer = par->PVOPsearchHeight;
        mBVOPsearchHorForw = par->BVOPsearchWidthForw;
        mBVOPsearchVerForw = par->BVOPsearchHeightForw;
        mBVOPsearchHorBack = par->BVOPsearchWidthBack;
        mBVOPsearchVerBack = par->BVOPsearchHeightBack;
        mp4_ClipR(mPVOPsearchHor, sw);
        mp4_ClipR(mPVOPsearchVer, sh);
        mp4_ClipR(mBVOPsearchHorForw, sw);
        mp4_ClipR(mBVOPsearchVerForw, sh);
        mp4_ClipR(mBVOPsearchHorBack, sw);
        mp4_ClipR(mBVOPsearchVerBack, sh);
        if (mPVOPsearchHor < 0 || mPVOPsearchVer < 0 || (mBVOPdist && (mBVOPsearchHorForw < 0 || mBVOPsearchVerForw < 0 || mBVOPsearchHorBack < 0 || mBVOPsearchVerBack < 0))) {
            mMEflags |= ME_AUTO_RANGE;
            mPVOPsearchHor = mPVOPsearchVer = mBVOPsearchHorForw = mBVOPsearchVerForw = mBVOPsearchHorBack = mBVOPsearchVerBack = 31;
            mPVOPfcodeForw = mBVOPfcodeForw = mBVOPfcodeBack = 2;
        } else if (mPVOPsearchHor == 0 || mPVOPsearchVer == 0 || (mBVOPdist && (mBVOPsearchHorForw == 0 || mBVOPsearchVerForw == 0 || mBVOPsearchHorBack == 0 || mBVOPsearchVerBack == 0))) {
            mMEflags |= ME_ZERO_MV;
            mPVOPfcodeForw = mBVOPfcodeForw = mBVOPfcodeBack = 1;
        } else {
            // calc vop_fcode_forward for PVOPs
            i = IPP_MAX(mPVOPsearchHor, mPVOPsearchVer);
            j = i << 1;
            mPVOPfcodeForw = 1;
            while (j > ((16 << mPVOPfcodeForw) - 1))
                mPVOPfcodeForw ++;
            // calc vop_fcode_forward for BVOPs
            i = IPP_MAX(mBVOPsearchHorForw, mBVOPsearchVerForw);
            j = i << 1;
            mBVOPfcodeForw = 1;
            while (j > ((16 << mBVOPfcodeForw) - 1))
                mBVOPfcodeForw ++;
            // calc vop_fcode_backward for BVOPs
            i = IPP_MAX(mBVOPsearchHorBack, mBVOPsearchVerBack);
            j = i << 1;
            mBVOPfcodeBack = 1;
            while (j > ((16 << mBVOPfcodeBack) - 1))
                mBVOPfcodeBack ++;
        }
        if (VOL.quarter_sample) {
            if ((IPP_MAX(mPVOPsearchHor, mPVOPsearchVer) << 2) > (16 << mPVOPfcodeForw) - 1)
                mPVOPfcodeForw ++;
            if ((IPP_MAX(mBVOPsearchHorForw, mBVOPsearchVerForw) << 2) > (16 << mBVOPfcodeForw) - 1)
                mBVOPfcodeForw ++;
            if ((IPP_MAX(mBVOPsearchHorBack, mBVOPsearchVerBack) << 2) > (16 << mBVOPfcodeBack) - 1)
                mBVOPfcodeBack ++;
            VO.visual_object_verid = 2;
            VOL.video_object_layer_verid = 2;
        }
        if (VOL.sprite_enable == MP4_SPRITE_GMC) {
            VO.visual_object_verid = 2;
            VOL.video_object_layer_verid = 2;
        }
        mTframe = 0; // for Direct field MC
    }
    if (VOL.sprite_enable == MP4_SPRITE_STATIC) {
        mPlanes = 1;
        mSourceWidth = par->sprite_width;
        mSourceHeight = par->sprite_height;
    } else {
        mPlanes = (mIVOPdist == 1) ? 1 : (mBVOPdist == 0) ? 2 : 2 + mBVOPdist + 1;
        mSourceWidth = par->Width;
        mSourceHeight = par->Height;
    }
    VOP.warping_mv_code_du[0] = VOP.warping_mv_code_du[1] = VOP.warping_mv_code_du[2] = VOP.warping_mv_code_du[3] = 0;
    VOP.warping_mv_code_dv[0] = VOP.warping_mv_code_dv[1] = VOP.warping_mv_code_dv[2] = VOP.warping_mv_code_dv[3] = 0;
    mSourceFormat = YUV_CHROMA_420;
    mExpandSize = 16;
    mExpandSizeA = (mExpandSize + 15) & (~15);
    mNumMacroBlockPerRow = (mSourceWidth + 15) >> 4;
    mNumMacroBlockPerCol = (mSourceHeight + 15) >> 4;
    mNumMacroBlockPerVOP = mNumMacroBlockPerRow * mNumMacroBlockPerCol;
    mStepLuma = mExpandSizeA * 2 + mNumMacroBlockPerRow * 16;
    mLumaPlaneSize = mStepLuma * (mExpandSizeA * 2 + mNumMacroBlockPerCol * 16);
    mStepChroma = (mExpandSizeA >> 1) * 2 + mNumMacroBlockPerRow * 8;
    mChromaPlaneSize = mStepChroma * ((mExpandSizeA >> 1) * 2 + mNumMacroBlockPerCol * 8);
    mSceneChangeThreshold = (mNumMacroBlockPerVOP * par->SceneChangeThreshold + 50) / 100;
    mIsInit = true;
    // buffers allocation
    mFrame = new mp4_Frame [mPlanes];
    if (mFrame) {
        for (i = 0; i < mPlanes; i ++) {
            //mFrame[i].ap = ippsMalloc_8u(mLumaPlaneSize + mChromaPlaneSize + mChromaPlaneSize);
            //if (!mFrame[i].ap)
            //    stsAlloc = ippStsMemAllocErr;
            //mFrame[i].pY = mFrame[i].ap + mExpandSizeA + mExpandSizeA * mStepLuma;
            //mFrame[i].pU = mFrame[i].ap + mLumaPlaneSize + (mExpandSizeA >> 1) + (mExpandSizeA >> 1) * mStepChroma;
            //mFrame[i].pV = mFrame[i].ap + mLumaPlaneSize + mChromaPlaneSize + (mExpandSizeA >> 1) + (mExpandSizeA >> 1) * mStepChroma;
        }
    } else
        stsAlloc = ippStsMemAllocErr;
    if (VOL.sprite_enable == MP4_SPRITE_GMC) {
        ippiWarpGetSize_MPEG4(&specSize);
        mWarpSpec = (IppiWarpSpec_MPEG4*)ippMalloc(specSize);
        if (!mWarpSpec)
            stsAlloc = ippStsMemAllocErr;
        mGMCPredY = mGMCPredU = mGMCPredV = NULL;
        //mGMCPredY = ippsMalloc_8u(3*16*16*mNumMacroBlockPerRow >> 1);
        //mGMCPredU = mGMCPredY + 16*16*mNumMacroBlockPerRow;
        //mGMCPredV = mGMCPredU + 8*8*mNumMacroBlockPerRow;
#ifdef USE_CV_GME
        // pyramids
        mPyrLevel = 3;
        while (mSourceWidth < (1 << mPyrLevel) && mSourceHeight < (1 << mPyrLevel))
            mPyrLevel --;
        Ipp16s pKernel[5] = {1, 4, 6, 4, 1};
        IppiSize vopSize;
        const Ipp32f  rate = 2.0f;
        vopSize.width = mSourceWidth;
        vopSize.height = mSourceHeight;
        mPyramid[0] = NULL;
        stsPyr = ippiPyramidInitAlloc(&mPyramid[0], mPyrLevel, vopSize, rate);
        if (stsPyr == ippStsNoErr) {
            mPyramid[0]->pState = NULL;
            stsPyr = ippiPyramidLayerDownInitAlloc_8u_C1R((IppiPyramidDownState_8u_C1R**)(&(mPyramid[0]->pState)), vopSize, rate, pKernel, 5, IPPI_INTER_LINEAR);
            if (stsPyr == ippStsNoErr && mPyramid[0]->level == mPyrLevel) {
                mPyramid[1] = NULL;
                stsPyr = ippiPyramidInitAlloc(&mPyramid[1], mPyrLevel, vopSize, rate);
                if (stsPyr == ippStsNoErr) {
                    mPyramid[1]->pState = NULL;
                    stsPyr = ippiPyramidLayerDownInitAlloc_8u_C1R((IppiPyramidDownState_8u_C1R**)(&(mPyramid[1]->pState)), vopSize, rate, pKernel, 5, IPPI_INTER_LINEAR);
                    if (mPyramid[1]->level != mPyrLevel)
                        stsPyr = ippStsMemAllocErr;
                    else {
                        for (i = 1; i <= mPyrLevel; i ++) {
                            mPyramid[0]->pImage[i] = ippiMalloc_8u_C1(mPyramid[0]->pRoi[i].width, mPyramid[0]->pRoi[i].height, &mPyramid[0]->pStep[i]);
                            if (!mPyramid[0]->pImage[i])
                                stsPyr = ippStsMemAllocErr;
                            mPyramid[1]->pImage[i] = ippiMalloc_8u_C1(mPyramid[1]->pRoi[i].width, mPyramid[1]->pRoi[i].height, &mPyramid[1]->pStep[i]);
                            if (!mPyramid[1]->pImage[i])
                                stsPyr = ippStsMemAllocErr;
                        }
                    }
                }
            }
        }
        if (stsPyr != ippStsNoErr)
            stsAlloc = ippStsMemAllocErr;
        mPyrC = mPyramid[0];
        mPyrR = mPyramid[1];
        mOptFlowNumPoint = mSourceWidth * mSourceHeight / (32 * 32);
        if (mOptFlowNumPoint <= 0)
            mOptFlowNumPoint = 10;
        else if (mOptFlowNumPoint > 10000)
            mOptFlowNumPoint = 10000;
        Ipp64f ratio_xy = (Ipp64f)mPyramid[0]->pRoi[0].width / mPyramid[0]->pRoi[0].height;
        mOptFlowNumPointY = (Ipp32s)floor(sqrt(mOptFlowNumPoint / ratio_xy) + 0.5);
        mOptFlowNumPointX = (Ipp32s)floor(mOptFlowNumPointY * ratio_xy + 0.5);
        mOptFlowNumPoint = mOptFlowNumPointX * mOptFlowNumPointY;
        mOptFlowWinSize = 11;
        if (ippiOpticalFlowPyrLKInitAlloc_8u_C1R(&mOptFlowState, mPyramid[0]->pRoi[0], mOptFlowWinSize, ippAlgHintAccurate) != ippStsNoErr)
            stsAlloc = ippStsMemAllocErr;
        mOptFlowPoints = (IppiPoint_32f*)ippMalloc(mOptFlowNumPoint * 2 * sizeof(mOptFlowPoints[0]));
        mOptFlowPtC = mOptFlowPoints;
        mOptFlowPtR = mOptFlowPoints + mOptFlowNumPoint;
        mOptFlowPtSts = (Ipp8s*)ippMalloc(mOptFlowNumPoint * sizeof(mOptFlowPtSts[0]));
        mOptFlowPatchDiff = (Ipp32f*)ippMalloc(mOptFlowNumPoint * sizeof(mOptFlowPatchDiff[0]));
        mOptFlowMask = (Ipp8s*)ippMalloc(mOptFlowNumPoint * 2 * sizeof(mOptFlowMask[0]));
        if (!mOptFlowPoints || !mOptFlowPtSts || !mOptFlowPatchDiff ||!mOptFlowMask)
            stsAlloc = ippStsMemAllocErr;
#else
        mp4_ClipR(VOL.no_of_sprite_warping_points, 1);
#endif
    } else {
        mWarpSpec = NULL;
        mGMCPredY = mGMCPredU = mGMCPredV = NULL;
#ifdef USE_CV_GME
        mPyramid[0] = mPyramid[1] = NULL;
#endif
    }
    if (!VOL.short_video_header) {
        ippiQuantInvIntraGetSize_MPEG4(&specSize);
        mQuantInvIntraSpec = (IppiQuantInvIntraSpec_MPEG4*)ippMalloc(specSize);
        ippiQuantInvInterGetSize_MPEG4(&specSize);
        mQuantInvInterSpec = (IppiQuantInvInterSpec_MPEG4*)ippMalloc(specSize);
        ippiQuantIntraGetSize_MPEG4(&specSize);
        mQuantIntraSpec = (IppiQuantIntraSpec_MPEG4*)ippMalloc(specSize);
        ippiQuantInterGetSize_MPEG4(&specSize);
        mQuantInterSpec = (IppiQuantInterSpec_MPEG4*)ippMalloc(specSize);
        if (!mQuantInvIntraSpec || !mQuantInvInterSpec || !mQuantIntraSpec || !mQuantInterSpec)
            stsAlloc = ippStsMemAllocErr;
        mMBpos = NULL;
        mMBquant = NULL;
        mMBpredMV = NULL;
    } else {
        mQuantInvIntraSpec = NULL;
        mQuantInvInterSpec = NULL;
        mQuantIntraSpec = NULL;
        mQuantInterSpec = NULL;
        mMBpos = (Ipp32u*)ippMalloc(mNumMacroBlockPerVOP*sizeof(Ipp32u));
        mMBquant = (Ipp8u*)ippMalloc(mNumMacroBlockPerVOP);
        mMBpredMV = (IppMotionVector*)ippMalloc(mNumMacroBlockPerVOP * sizeof(IppMotionVector));
        if (!mMBpos || !mMBquant || !mMBpredMV)
            stsAlloc = ippStsMemAllocErr;
    }
    intBuffSize = mSourceWidth * mSourceHeight;
    if (par->bsBuffer && (par->bsBuffSize > 0)) {
        cBS.Init(par->bsBuffer, par->bsBuffSize);
        mbsAlloc = false;
    } else {
        Ipp8u *buff = (Ipp8u*)ippMalloc(intBuffSize);
        if (!buff)
            stsAlloc = ippStsMemAllocErr;
        cBS.Init(buff, intBuffSize);
        mbsAlloc = true;
    }
    mNumThreads = 1;
#ifdef _OMP_KARABAS
    mThreadingAlg = 1;
    if (par->RateControl == 2 || VOL.data_partitioned || (!VOL.short_video_header && mInsertResync))
        mThreadingAlg = 1;
    if (mThreadingAlg == 1)
        VOL.resync_marker_disable = 0;
    mNumThreads = par->numThreads;
    if (par->numThreads < 1) {
        mNumThreads = 1;
#ifdef _OPENMP
#pragma omp parallel
        {
#pragma omp master
            {
                mNumThreads = omp_get_num_threads();
            }
        }
#endif
    }
    if (mNumThreads > mNumMacroBlockPerCol)
        mNumThreads = mNumMacroBlockPerCol;
    if (VOL.short_video_header && mThreadingAlg == 1) {
        if (mNumThreads > VOP.num_gobs_in_vop)
            mNumThreads = VOP.num_gobs_in_vop;
    }
    MBinfoMT = (mp4_MacroBlockMT*)ippsMalloc_8u(mNumMacroBlockPerRow * mNumThreads * sizeof(mp4_MacroBlockMT));
    mSliceMT = new mp4_Slice[mNumThreads];
    mCurRowMT = new Ipp32s[mNumMacroBlockPerCol];
    if (!MBinfoMT || !mSliceMT || !mCurRowMT)
        stsAlloc = ippStsMemAllocErr;
    else
        ippsZero_8u((Ipp8u*)MBinfoMT, mNumMacroBlockPerRow * mNumThreads * sizeof(mp4_MacroBlockMT));
    if (mNumThreads > 1) {
        mBuffer = ippsMalloc_8u(intBuffSize * (mNumThreads - 1));
        if (!mBuffer)
            stsAlloc = ippStsMemAllocErr;
    }
#ifdef _OPENMP
    mLockMT = new omp_lock_t[mNumMacroBlockPerCol];
    if (mLockMT) {
        for (i = 0; i < mNumMacroBlockPerCol; i ++)
            omp_init_lock(mLockMT+i);
    } else
        stsAlloc = ippStsMemAllocErr;
#endif
#endif // _OMP_KARABAS
    if (VOL.data_partitioned) {
        mBuffer_1 = (Ipp8u*)ippMalloc(intBuffSize * mNumThreads);
        mBuffer_2 = (Ipp8u*)ippMalloc(intBuffSize * mNumThreads);
        if (!mBuffer_1 || !mBuffer_2)
            stsAlloc = ippStsMemAllocErr;
    } else {
        mBuffer_1 = mBuffer_2 = NULL;
    }
    MBinfo = new mp4_MacroBlock[mNumMacroBlockPerVOP];
    if (!MBinfo)
        stsAlloc = ippStsMemAllocErr;
    else
        ippsSet_8u(0, (Ipp8u*)MBinfo, mNumMacroBlockPerVOP * sizeof(mp4_MacroBlock));
#ifdef USE_ME_SADBUFF
    if (mMEflags & ME_AUTO_RANGE)
        mMEfastSADsize = (63 * 2 + 1) * (63 * 2 + 1);
    else
        mMEfastSADsize = (IPP_MAX(IPP_MAX(mPVOPsearchHor, mBVOPsearchHorForw), mBVOPsearchHorBack) * 2 + 1) * (IPP_MAX(IPP_MAX(mPVOPsearchVer, mBVOPsearchVerForw), mBVOPsearchVerBack) * 2 + 1);
    mMEfastSADsize = (mMEfastSADsize + 3) & (~3);
    mMEfastSAD = ippsMalloc_32s(mMEfastSADsize * mNumThreads);
    if (!mMEfastSAD)
        stsAlloc = ippStsMemAllocErr;
#endif
    if (stsAlloc != ippStsNoErr) {
        Close();
        return MP4_STS_ERR_NOMEM;
    }
    // setup quant matrix
    if (!VOL.short_video_header) {
        if (VOL.quant_type) {
            if (par->load_intra_quant_mat ) {
                for (i = 0; i < par->load_intra_quant_mat_len; i++)
                    VOL.intra_quant_mat[mp4_ZigZagScan[i]] = par->intra_quant_mat[i];
                j = par->intra_quant_mat[i-1];
                for (; i < 64; i++)
                    VOL.intra_quant_mat[mp4_ZigZagScan[i]] = (Ipp8u)j;
            } else
                ippsCopy_8u(mp4_DefaultIntraQuantMatrix, VOL.intra_quant_mat, 64);
            if (par->load_nonintra_quant_mat ) {
                for (i = 0; i < par->load_nonintra_quant_mat_len; i++)
                    VOL.nonintra_quant_mat[mp4_ZigZagScan[i]] = par->nonintra_quant_mat[i];
                j = par->nonintra_quant_mat[i-1];
                for (; i < 64; i++)
                    VOL.nonintra_quant_mat[mp4_ZigZagScan[i]] = (Ipp8u)j;
            } else
                ippsCopy_8u(mp4_DefaultNonIntraQuantMatrix, VOL.nonintra_quant_mat, 64);
        }
        ippiQuantInvIntraInit_MPEG4(VOL.quant_type ? VOL.intra_quant_mat : NULL, mQuantInvIntraSpec, 8);
        ippiQuantInvInterInit_MPEG4(VOL.quant_type ? VOL.nonintra_quant_mat : NULL, mQuantInvInterSpec, 8);
        ippiQuantIntraInit_MPEG4(VOL.quant_type ? VOL.intra_quant_mat : NULL, mQuantIntraSpec, 8);
        ippiQuantInterInit_MPEG4(VOL.quant_type ? VOL.nonintra_quant_mat : NULL, mQuantInterSpec, 8);
    }
    // setup info for DC AC prediction
    mDefDC = (Ipp16s)(1 << (VOL.bits_per_pixel + 2));
    if (!VOL.short_video_header) {
        mp4_MacroBlock *mbCurr = MBinfo;
        for (i = 0; i < mNumMacroBlockPerCol; i ++) {
            for (j = 0; j < mNumMacroBlockPerRow; j ++) {
                mp4_MacroBlock *mbA = mbCurr - 1;
                mp4_MacroBlock *mbB = mbCurr - 1 - mNumMacroBlockPerRow;
                mp4_MacroBlock *mbC = mbCurr - mNumMacroBlockPerRow;
                mbCurr->block[0].predA = &mbA->block[1];
                mbCurr->block[0].predB = &mbB->block[3];
                mbCurr->block[0].predC = &mbC->block[2];
                mbCurr->block[1].predA = &mbCurr->block[0];
                mbCurr->block[1].predB = &mbC->block[2];
                mbCurr->block[1].predC = &mbC->block[3];
                mbCurr->block[2].predA = &mbA->block[3];
                mbCurr->block[2].predB = &mbA->block[1];
                mbCurr->block[2].predC = &mbCurr->block[0];
                mbCurr->block[3].predA = &mbCurr->block[2];
                mbCurr->block[3].predB = &mbCurr->block[0];
                mbCurr->block[3].predC = &mbCurr->block[1];
                mbCurr->block[4].predA = &mbA->block[4];
                mbCurr->block[4].predB = &mbB->block[4];
                mbCurr->block[4].predC = &mbC->block[4];
                mbCurr->block[5].predA = &mbA->block[5];
                mbCurr->block[5].predB = &mbB->block[5];
                mbCurr->block[5].predC = &mbC->block[5];
                mbCurr ++;
            }
        }
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            mp4_MacroBlock *mbCurr = &MBinfo[j];
            mbCurr->block[0].predB = mbCurr->block[0].predC = NULL;
            mbCurr->block[1].predB = mbCurr->block[1].predC = NULL;
            mbCurr->block[4].predB = mbCurr->block[4].predC = NULL;
            mbCurr->block[5].predB = mbCurr->block[5].predC = NULL;
        }
        for (i = 0; i < mNumMacroBlockPerCol; i ++) {
            mp4_MacroBlock *mbCurr = &MBinfo[i*mNumMacroBlockPerRow];
            mbCurr->block[0].predA = mbCurr->block[0].predB = NULL;
            mbCurr->block[2].predA = mbCurr->block[2].predB = NULL;
            mbCurr->block[4].predA = mbCurr->block[4].predB = NULL;
            mbCurr->block[5].predA = mbCurr->block[5].predB = NULL;
        }
    }
    // setup frames
    if (mBVOPdist == 0) {
        mFrameC = &mFrame[0];
        if (mIVOPdist > 1)
            mFrameF = &mFrame[1];
    } else {
        mFrameC = &mFrame[0];
        mFrameF = &mFrame[1];
        mFrameB = &mFrame[0];
        mIndxBVOP = 2;
        mNumBVOP = 0;
    }
    mFrameD = mFrameC;
    // setup RateControl vars
    mRateControl = par->RateControl;
    if (VOL.sprite_enable == MP4_SPRITE_STATIC)
        mRateControl = 0;
    mBitsEncodedTotal = 0;
    mBitRate = par->BitRate;
    if (mRateControl == 1) {
        mRC.Init(0, 0, 0, mBitRate, (Ipp64f)par->TimeResolution / par->TimeIncrement, mSourceWidth, mSourceHeight, VOL.short_video_header ? 3 : 1, 31);
        mQuantIVOP = mRC.GetQP(MP4_VOP_TYPE_I);
        mQuantPVOP = mRC.GetQP(MP4_VOP_TYPE_P);
        mQuantBVOP = mRC.GetQP(MP4_VOP_TYPE_B);
    } else if (mRateControl == 2) {
        mRC_MB.Init(mBitRate, (Ipp64f)par->TimeResolution / par->TimeIncrement, mSourceWidth, mSourceHeight, mNumMacroBlockPerRow, mNumMacroBlockPerCol, mIVOPdist, VOL.data_partitioned, VOL.short_video_header ? 3 : 1, 31);
        mQuantIVOP = mQuantPVOP = mQuantBVOP = mRC_MB.GetQP();
        mMEflags &= ~ME_4MV;
    } else {
        mQuantIVOP = par->quantIVOP;
        mQuantPVOP = par->quantPVOP;
        mQuantBVOP = par->quantBVOP;
    }
    // setup VOP time
    mVOPtime = mSyncTime = mSyncTimeB = 0;
    // calc length of macroblock_number code
    i = mNumMacroBlockPerVOP - 1;
    mMBNlength = 0;
    do {
        i >>= 1;
        mMBNlength ++;
    } while (i);
    // setup Slices
    mSlice.startRow = 0;
    mSlice.gob_number = 0;
    mSlice.numRow = mNumMacroBlockPerCol;
    mSlice.cBS_1.Init(mBuffer_1, intBuffSize);
    mSlice.cBS_2.Init(mBuffer_2, intBuffSize);
#ifdef USE_ME_SADBUFF
    mSlice.meBuff = mMEfastSAD;
#endif
#ifdef _OMP_KARABAS
    if (VOL.short_video_header)
        j = (VOP.num_gobs_in_vop / mNumThreads) * (VOP.num_macroblocks_in_gob / mNumMacroBlockPerRow);
    else
        j = mNumMacroBlockPerCol / mNumThreads;
    for (i = 0; i < mNumThreads; i ++) {
        mSliceMT[i].startRow = i * j;
        if (VOL.short_video_header)
            mSliceMT[i].gob_number = mSliceMT[i].startRow * mNumMacroBlockPerRow / VOP.num_macroblocks_in_gob;
        mSliceMT[i].numRow = (i < mNumThreads - 1) ? j : mNumMacroBlockPerCol - i * j;
        if (i != 0)
            mSliceMT[i].cBS.Init(mBuffer + (i - 1) * intBuffSize, intBuffSize);
        mSliceMT[i].cBS_1.Init(mBuffer_1 + i * intBuffSize, intBuffSize);
        mSliceMT[i].cBS_2.Init(mBuffer_2 + i * intBuffSize, intBuffSize);
#ifdef USE_ME_SADBUFF
        mSliceMT[i].meBuff = mMEfastSAD + mMEfastSADsize * i;
#endif
    }
#endif // _OM_KARABAS
    return MP4_STS_NOERR;
}

Ipp32s VideoEncoderMPEG4::SetFrameBasicSpriteWarpCoeffs(Ipp32s *du, Ipp32s *dv, Ipp32s bcf)
{
    Ipp32s  i;

    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    for (i = 0; i < VOL.no_of_sprite_warping_points; i ++)
    {
        if (du[i] < -16383 || du[i] > 16383 || dv[i] < -16383 || dv[i] > 16383)
            return MP4_STS_ERR_PARAM;

        VOP.warping_mv_code_du[i] = du[i];
        VOP.warping_mv_code_dv[i] = dv[i];
    }
    if (VOL.sprite_brightness_change) {
        if (bcf < -112 || bcf > 1648)
            return MP4_STS_ERR_PARAM;

        VOP.brightness_change_factor = bcf;
    }
    return MP4_STS_NOERR;
}

static void mp4_ExpandFrameReplicate(Ipp8u *pSrcDstPlane, Ipp32s frameWidth, Ipp32s frameHeight, Ipp32s expandPels, Ipp32s step)
{
    Ipp8u   *pDst1, *pDst2, *pSrc1, *pSrc2;
    Ipp32s  i, j;
    Ipp32u  t1, t2;

    pDst1 = pSrcDstPlane + step * expandPels;
    pDst2 = pDst1 + frameWidth + expandPels;
    if (expandPels == 8) {
        for (i = 0; i < frameHeight; i ++) {
            t1 = pDst1[8] + (pDst1[8] << 8);
            t2 = pDst2[-1] + (pDst2[-1] << 8);
            t1 = (t1 << 16) + t1;
            t2 = (t2 << 16) + t2;
            ((Ipp32u*)pDst1)[0] = t1;
            ((Ipp32u*)pDst1)[1] = t1;
            ((Ipp32u*)pDst2)[0] = t2;
            ((Ipp32u*)pDst2)[1] = t2;
            pDst1 += step;
            pDst2 += step;
        }
    } else if (expandPels == 16) {
        for (i = 0; i < frameHeight; i ++) {
            t1 = pDst1[16] + (pDst1[16] << 8);
            t2 = pDst2[-1] + (pDst2[-1] << 8);
            t1 = (t1 << 16) + t1;
            t2 = (t2 << 16) + t2;
            ((Ipp32u*)pDst1)[0] = t1;
            ((Ipp32u*)pDst1)[1] = t1;
            ((Ipp32u*)pDst1)[2] = t1;
            ((Ipp32u*)pDst1)[3] = t1;
            ((Ipp32u*)pDst2)[0] = t2;
            ((Ipp32u*)pDst2)[1] = t2;
            ((Ipp32u*)pDst2)[2] = t2;
            ((Ipp32u*)pDst2)[3] = t2;
            pDst1 += step;
            pDst2 += step;
        }
    } else {
        for (i = 0; i < frameHeight; i ++) {
            ippsSet_8u(pDst1[expandPels], pDst1, expandPels);
            ippsSet_8u(pDst2[-1], pDst2, expandPels);
            pDst1 += step;
            pDst2 += step;
        }
    }
    pDst1 = pSrcDstPlane;
    pSrc1 = pSrcDstPlane + expandPels * step;
    pDst2 = pSrc1 + frameHeight * step;
    pSrc2 = pDst2 - step;
    j = frameWidth + 2 * expandPels;
    for (i = 0; i < expandPels; i ++) {
        ippsCopy_8u(pSrc1, pDst1, j);
        ippsCopy_8u(pSrc2, pDst2, j);
        pDst1 += step;
        pDst2 += step;
    }
}

void VideoEncoderMPEG4::ExpandFrame(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV)
{
    if (mExpandSize) {
        Ipp32s  wL = mNumMacroBlockPerRow << 4;
        Ipp32s  hL = mNumMacroBlockPerCol << 4;
        Ipp32s  wC = mNumMacroBlockPerRow << 3;
        Ipp32s  hC = mNumMacroBlockPerCol << 3;
        Ipp32s  es = (mExpandSize + 1) >> 1;
        mp4_ExpandFrameReplicate(pY-mExpandSize*mStepLuma-mExpandSize, wL, hL, mExpandSize, mStepLuma);
        mp4_ExpandFrameReplicate(pU-es*mStepChroma-es, wC, hC, es, mStepChroma);
        mp4_ExpandFrameReplicate(pV-es*mStepChroma-es, wC, hC, es, mStepChroma);
/*
        if (VOL.interlaced) {
            Ipp8u *psb, *pdb, *pst, *pdt;
            // pad fields
            psb = pY + mStepLuma - mExpandSize;
            pdb = psb - mStepLuma - mStepLuma;
            pst = pY + mStepLuma * (mSourceHeight - 2) - mExpandSize;
            pdt = pst + mStepLuma + mStepLuma;
            for (i = 0; i < 8; i ++) {
                ippsCopy_8u(psb, pdb, mStepLuma);
                pdb -= mStepLuma + mStepLuma;
                ippsCopy_8u(pst, pdt, mStepLuma);
                pdt += mStepLuma + mStepLuma;
            }
        }
*/
    }
}

/*
//  padding the VOPs with size not multiple 16
//  for not complete blocks padd by
//      0 for DivX(tm) 5.1xx-5.3xx AVI streams
//      128 for QuickTime(tm) MP4 streams
//      replication for other
*/
void VideoEncoderMPEG4::PadFrame(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV)
{
    Ipp32s i, j;
    Ipp8u  *py, *pu, *pv;
    Ipp8u  pad = (Ipp8u)(mPadType == 1 ? 128 : 0);

    if (mSourceWidth & 15) {
        py = pY + mSourceWidth;
        for (i = 0; i < mSourceHeight; i ++) {
            if (mPadType)
                for (j = 0; j < (mNumMacroBlockPerRow << 4) - mSourceWidth; j ++)
                    py[j] = pad;
            else
                for (j = 0; j < (mNumMacroBlockPerRow << 4) - mSourceWidth; j ++)
                    py[j] = py[-1];
            py += mStepLuma;
        }
        pu = pU + (mSourceWidth >> 1);
        pv = pV + (mSourceWidth >> 1);
        for (i = 0; i < (mSourceHeight >> 1); i ++) {
            if (mPadType)
                for (j = 0; j < (mNumMacroBlockPerRow << 3) - (mSourceWidth >> 1); j ++) {
                    pu[j] = pad;
                    pv[j] = pad;
                }
            else
                for (j = 0; j < (mNumMacroBlockPerRow << 3) - (mSourceWidth >> 1); j ++) {
                    pu[j] = pu[-1];
                    pv[j] = pv[-1];
                }
            pu += mStepChroma;
            pv += mStepChroma;
        }
    }
    if (mSourceHeight & 15) {
        py = pY + mSourceHeight * mStepLuma;
        pY = py - mStepLuma;
        for (j = mSourceHeight; j < (mNumMacroBlockPerCol << 4); j ++) {
            if (mPadType)
                ippsSet_8u(pad, py, mNumMacroBlockPerRow << 4);
            else
                ippsCopy_8u(pY, py, mNumMacroBlockPerRow << 4);
            py += mStepLuma;
        }
        pu = pU + (mSourceHeight >> 1) * mStepChroma;
        pv = pV + (mSourceHeight >> 1) * mStepChroma;
        pU = pu - mStepChroma;
        pV = pv - mStepChroma;
        for (j = (mSourceHeight >> 1); j < (mNumMacroBlockPerCol << 3); j ++) {
            if (mPadType) {
                ippsSet_8u(pad, pu, mNumMacroBlockPerRow << 3);
                ippsSet_8u(pad, pv, mNumMacroBlockPerRow << 3);
            } else {
                ippsCopy_8u(pU, pu, mNumMacroBlockPerRow << 3);
                ippsCopy_8u(pV, pv, mNumMacroBlockPerRow << 3);
            }
            pu += mStepChroma;
            pv += mStepChroma;
        }
    }
}

Ipp32s VideoEncoderMPEG4::GetCurrentFrameInfo(Ipp8u **pY, Ipp8u **pU, Ipp8u **pV, Ipp32s *stepLuma, Ipp32s *stepChroma)
{
    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    *pY = mFrameC->pY;
    *pU = mFrameC->pU;
    *pV = mFrameC->pV;
    *stepLuma = mStepLuma;
    *stepChroma = mStepChroma;
    return MP4_STS_NOERR;
}

Ipp32s VideoEncoderMPEG4::GetDecodedFrameInfo(Ipp8u **pY, Ipp8u **pU, Ipp8u **pV, Ipp32s *stepLuma, Ipp32s *stepChroma)
{
    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    *pY = mFrameD->pY;
    *pU = mFrameD->pU;
    *pV = mFrameD->pV;
    *stepLuma = mStepLuma;
    *stepChroma = mStepChroma;
    return MP4_STS_NOERR;
}


int mp4_WeightMV(int xv, int yv, IppMotionVector mvPred, int fcode, int quant, int qpel)
{
    int  b, a, dx, dy;

    if (qpel) {
        xv += xv;
        yv += yv;
    }
    dx = xv - mvPred.dx;
    dy = yv - mvPred.dy;
    if (fcode == 1) {
        if (dx < -32) dx += 64;
        if (dx >  31) dx -= 64;
        if (dy < -32) dy += 64;
        if (dy >  31) dy -= 64;
        b = mp4_VLC_MVD_TB12[dx+32].len + mp4_VLC_MVD_TB12[dy+32].len;
    } else {
        Ipp32s  fRangeMin = -(16 << fcode), fRangeMax = (16 << fcode) - 1, fRange = fRangeMax - fRangeMin + 1;
        if (dx < fRangeMin) dx += fRange;
        if (dx > fRangeMax) dx -= fRange;
        if (dy < fRangeMin) dy += fRange;
        if (dy > fRangeMax) dy -= fRange;
        fcode --;
        if (dx == 0) {
            b = 1;
        } else {
            if (dx < 0)
                dx = -dx;
            a = ((dx - 1) >> fcode) + 1;
            b = mp4_VLC_MVD_TB12[a+32].len + fcode;
        }
        if (dy == 0) {
            b ++;
        } else {
            if (dy < 0)
                dy = -dy;
            a = ((dy - 1) >> fcode) + 1;
            b += mp4_VLC_MVD_TB12[a+32].len + fcode;
        }
    }
    return b * mp4_MV_Weigth[quant];
}

void mp4_ComputeChroma4MV(const IppMotionVector mvLuma[4], IppMotionVector *mvChroma)
{
    Ipp32s  dx, dy, cdx, cdy, adx, ady;

    dx = mvLuma[0].dx + mvLuma[1].dx + mvLuma[2].dx + mvLuma[3].dx;
    dy = mvLuma[0].dy + mvLuma[1].dy + mvLuma[2].dy + mvLuma[3].dy;
    adx = mp4_Abs(dx);
    ady = mp4_Abs(dy);
    cdx = mp4_cCbCrMvRound16[adx & 15] + (adx >> 4) * 2;
    cdy = mp4_cCbCrMvRound16[ady & 15] + (ady >> 4) * 2;
    mvChroma->dx = (Ipp16s)((dx >= 0) ? cdx : -cdx);
    mvChroma->dy = (Ipp16s)((dy >= 0) ? cdy : -cdy);
}

void mp4_ComputeChroma4MVQ(const IppMotionVector mvLuma[4], IppMotionVector *mvChroma)
{
    Ipp32s  dx, dy, cdx, cdy, adx, ady;

    dx = mp4_Div2(mvLuma[0].dx) + mp4_Div2(mvLuma[1].dx) + mp4_Div2(mvLuma[2].dx) + mp4_Div2(mvLuma[3].dx);
    dy = mp4_Div2(mvLuma[0].dy) + mp4_Div2(mvLuma[1].dy) + mp4_Div2(mvLuma[2].dy) + mp4_Div2(mvLuma[3].dy);
    adx = mp4_Abs(dx);
    ady = mp4_Abs(dy);
    cdx = mp4_cCbCrMvRound16[adx & 15] + (adx >> 4) * 2;
    cdy = mp4_cCbCrMvRound16[ady & 15] + (ady >> 4) * 2;
    mvChroma->dx = (Ipp16s)((dx >= 0) ? cdx : -cdx);
    mvChroma->dy = (Ipp16s)((dy >= 0) ? cdy : -cdy);
}

void VideoEncoderMPEG4::LimitMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV->dx, (Ipp16s)((-16 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x) << 1));
    mp4_Clip(pMV->dy, (Ipp16s)((-16 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y) << 1));
}

void VideoEncoderMPEG4::LimitMVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV->dx, (Ipp16s)((-16 - x) << 2), (Ipp16s)((VOL.video_object_layer_width - x) << 2));
    mp4_Clip(pMV->dy, (Ipp16s)((-16 - y) << 2), (Ipp16s)((VOL.video_object_layer_height - y) << 2));
}

void VideoEncoderMPEG4::Limit4MV(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV[0].dx, (Ipp16s)((-8 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x) << 1));
    mp4_Clip(pMV[0].dy, (Ipp16s)((-8 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y) << 1));
    mp4_Clip(pMV[1].dx, (Ipp16s)((-8 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x - 8) << 1));
    mp4_Clip(pMV[1].dy, (Ipp16s)((-8 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y) << 1));
    mp4_Clip(pMV[2].dx, (Ipp16s)((-8 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x) << 1));
    mp4_Clip(pMV[2].dy, (Ipp16s)((-8 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y - 8) << 1));
    mp4_Clip(pMV[3].dx, (Ipp16s)((-8 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x - 8) << 1));
    mp4_Clip(pMV[3].dy, (Ipp16s)((-8 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y - 8) << 1));
}

void VideoEncoderMPEG4::Limit4MVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV[0].dx, (Ipp16s)((-8 - x) << 2), (Ipp16s)((VOL.video_object_layer_width - x) << 2));
    mp4_Clip(pMV[0].dy, (Ipp16s)((-8 - y) << 2), (Ipp16s)((VOL.video_object_layer_height - y) << 2));
    mp4_Clip(pMV[1].dx, (Ipp16s)((-8 - x) << 2), (Ipp16s)((VOL.video_object_layer_width - x - 8) << 2));
    mp4_Clip(pMV[1].dy, (Ipp16s)((-8 - y) << 2), (Ipp16s)((VOL.video_object_layer_height - y) << 2));
    mp4_Clip(pMV[2].dx, (Ipp16s)((-8 - x) << 2), (Ipp16s)((VOL.video_object_layer_width - x) << 2));
    mp4_Clip(pMV[2].dy, (Ipp16s)((-8 - y) << 2), (Ipp16s)((VOL.video_object_layer_height - y - 8) << 2));
    mp4_Clip(pMV[3].dx, (Ipp16s)((-8 - x) << 2), (Ipp16s)((VOL.video_object_layer_width - x - 8) << 2));
    mp4_Clip(pMV[3].dy, (Ipp16s)((-8 - y) << 2), (Ipp16s)((VOL.video_object_layer_height - y - 8) << 2));
}

void VideoEncoderMPEG4::LimitFMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV->dx, (Ipp16s)((-16 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x) << 1));
    mp4_Clip(pMV->dy, (Ipp16s)((-16 - y)), (Ipp16s)((VOL.video_object_layer_height - y)));
}

void VideoEncoderMPEG4::LimitFMVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV->dx, (Ipp16s)((-16 - x) << 1), (Ipp16s)((VOL.video_object_layer_width - x) << 1));
    mp4_Clip(pMV->dy, (Ipp16s)((-16 - y) << 1), (Ipp16s)((VOL.video_object_layer_height - y) << 1));
}

void VideoEncoderMPEG4::LimitCMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y)
{
    mp4_Clip(pMV->dx, (Ipp16s)((-8 - x) << 1), (Ipp16s)(((VOL.video_object_layer_width >> 1) - x) << 1));
    mp4_Clip(pMV->dy, (Ipp16s)((-8 - y) << 1), (Ipp16s)(((VOL.video_object_layer_height >> 1) - y) << 1));
}

void VideoEncoderMPEG4::AdjustSearchRange()
{
    if (mMEflags & ME_AUTO_RANGE) {
        Ipp32s  fcRangeMin, fcRangeMax, flRangeMin, flRangeMax, co, ci, cnz, r, i;
        const Ipp32s p = 1;

        fcRangeMin = -(16 << mPVOPfcodeForw);
        fcRangeMax =  (16 << mPVOPfcodeForw) - 1;
        flRangeMin = -(16 << (mPVOPfcodeForw - 1));
        flRangeMax =  (16 << (mPVOPfcodeForw - 1)) - 1;
        co = ci = cnz = 0;
        r = 2;
        if (mMEflags & ME_HP)
            r <<= 1;
        else if (mMEflags & ME_QP)
            r <<= 2;
        for (i = 0; i < mNumMacroBlockPerVOP; i ++) {
            if (MBinfo[i].type < IPPVC_MBTYPE_INTRA && ((VOP.vop_coding_type != MP4_VOP_TYPE_S && !MBinfo[i].not_coded) || (VOP.vop_coding_type == MP4_VOP_TYPE_S && !MBinfo[i].mcsel))) {
                if (MBinfo[i].mv[0].dx > (fcRangeMax - r) || MBinfo[i].mv[0].dx < (fcRangeMin + r) || MBinfo[i].mv[0].dy > (fcRangeMax - r) || MBinfo[i].mv[0].dy < (fcRangeMin + r))
                    co ++;
                if (MBinfo[i].mv[0].dx > (flRangeMax - r) || MBinfo[i].mv[0].dx < (flRangeMin + r) || MBinfo[i].mv[0].dy > (flRangeMax - r) || MBinfo[i].mv[0].dy < (flRangeMin + r))
                    ci ++;
                if (MBinfo[i].mv[0].dx != 0 || MBinfo[i].mv[0].dy != 0)
                    cnz ++;
            }
        }
        if (cnz == 0) {
            co = 0;
            cnz = 1;
        }
        mBVOPsearchHorBack = mPVOPsearchHor;
        mBVOPsearchVerBack = mPVOPsearchVer;
        mBVOPfcodeBack = mPVOPfcodeForw;
        if (co * 100 / cnz >= p) {
            if (VOL.quarter_sample) {
                if (mPVOPfcodeForw < 4) {
                    mPVOPsearchHor = mPVOPsearchVer = (16 << (mPVOPfcodeForw - 1)) - 1;
                    mPVOPfcodeForw ++;
                }
            } else {
                if (mPVOPfcodeForw < 3) {
                    mPVOPsearchHor = mPVOPsearchVer = (16 << mPVOPfcodeForw) - 1;
                    mPVOPfcodeForw ++;
                }
            }
        } else if (!ci/*ci * 400 / cnz < p*/) {
            if (VOL.quarter_sample) {
                if (mPVOPfcodeForw > 2) {
                    mPVOPsearchHor = mPVOPsearchVer = (16 << (mPVOPfcodeForw - 3)) - 1;
                    mPVOPfcodeForw --;
                }
            } else {
                if (mPVOPfcodeForw > 1) {
                    mPVOPsearchHor = mPVOPsearchVer = (16 << (mPVOPfcodeForw - 2)) - 1;
                    mPVOPfcodeForw --;
                }
            }
        }
        mBVOPsearchHorForw = mPVOPsearchHor;
        mBVOPsearchVerForw = mPVOPsearchVer;
        mBVOPfcodeForw = mPVOPfcodeForw;
    }
}


int mp4_CalcBitsMV(IppMotionVector *mv, int fcode)
{
    int  b, a, dx, dy;

    dx = mv->dx;
    dy = mv->dy;
    if (fcode == 1) {
        if (dx < -32) dx += 64;
        if (dx >  31) dx -= 64;
        if (dy < -32) dy += 64;
        if (dy >  31) dy -= 64;
        b = mp4_VLC_MVD_TB12[dx+32].len + mp4_VLC_MVD_TB12[dy+32].len;
    } else {
        int     fRangeMin = -(16 << fcode), fRangeMax = (16 << fcode) - 1, fRange = fRangeMax - fRangeMin + 1;
        if (dx < fRangeMin) dx += fRange;
        if (dx > fRangeMax) dx -= fRange;
        if (dy < fRangeMin) dy += fRange;
        if (dy > fRangeMax) dy -= fRange;
        fcode --;
        if (dx == 0) {
            b = 1;
        } else {
            if (dx < 0)
                dx = -dx;
            a = ((dx - 1) >> fcode) + 1;
            b = mp4_VLC_MVD_TB12[a+32].len + fcode;
        }
        if (dy == 0) {
            b ++;
        } else {
            if (dy < 0)
                dy = -dy;
            a = ((dy - 1) >> fcode) + 1;
            b += mp4_VLC_MVD_TB12[a+32].len + fcode;
        }
    }
    return b;
}


int mp4_CalcBitsCoeffsIntra(const Ipp16s* pCoeffs, int countNonZero, int rvlcFlag, int dc_vlc, const Ipp8u* pScanTable, int blockNum)
{
    int  dcSize, run, i, cnt, level, lev, last, offs, esc, f, sum;

    if (dc_vlc) {
        dcSize = 0;
        i = pCoeffs[0] >= 0 ? pCoeffs[0] : -pCoeffs[0];
        while (i) {
            i >>= 1;
            dcSize ++;
        }
        sum = (blockNum <= 3) ? mp4_VLC_DCSIZE_TB13_len[dcSize] : mp4_VLC_DCSIZE_TB14_len[dcSize];
        if (dcSize)
            sum += dcSize;
        if (dcSize > 8)
            sum ++;
        if (pCoeffs[0] != 0)
            countNonZero --;
        f = 1;
    } else {
        sum = 0;
        f = 0;
    }
    if (countNonZero == 0)
        return sum;
    cnt = 1;
    run = 0;
    if (!rvlcFlag) {
        for (i = f; i < 64; i ++) {
            level = pCoeffs[pScanTable[i]];
            if (level) {
                lev = (level < 0) ? (-level) : level;
                last = (cnt == countNonZero);
                cnt ++;
                if (run <= mp4_VLC_TB16.runMax[last]) {
                    if (lev <= mp4_VLC_TB16.levMax[last][run]) {
                        offs = mp4_VLC_TB16.runOff[last][run] + lev - 1;
                        sum += mp4_VLC_TB16.vlc[offs].len;
                        esc = 0;
                    } else
                        esc = 1;
                } else
                    esc = 1;
                if (esc) {
                    if (run <= mp4_VLC_TB16.runMax[last]) {
                        if (lev <= 2 * mp4_VLC_TB16.levMax[last][run]) {
                            // ESC 1
                            lev -= mp4_VLC_TB16.levMax[last][run];
                            offs = mp4_VLC_TB16.runOff[last][run] + lev - 1;
                            sum += 8 + mp4_VLC_TB16.vlc[offs].len;
                            esc = 0;
                        }
                    }
                    if (esc) {
                        if (lev <= mp4_VLC_TB16.levMax[last][0]) {
                            offs = 0;
                            while (lev > mp4_VLC_RMAX_TB21[last][offs].code)
                                offs ++;
                            if (run - mp4_VLC_RMAX_TB21[last][offs].len - 1 <= mp4_VLC_RMAX_TB21[last][offs].len) {
                                // ESC 2
                                run -= mp4_VLC_RMAX_TB21[last][offs].len + 1;
                                offs = mp4_VLC_TB16.runOff[last][run] + lev - 1;
                                sum += 9 + mp4_VLC_TB16.vlc[offs].len;
                                esc = 0;
                            }
                        }
                    }
                    if (esc) {
                        // ESC 3
                        sum += 30;
                    }
                }
                if (last)
                    break;
                run = 0;
            } else
                run ++;
        }
    } else {
        for (i = f; i < 64; i ++) {
            level = pCoeffs[pScanTable[i]];
            if (level) {
                lev = (level < 0) ? (-level) : level;
                last = (cnt == countNonZero);
                cnt ++;
                if (run <= mp4_VLC_TB23a.runMax[last]) {
                    if (lev <= mp4_VLC_TB23a.levMax[last][run]) {
                        offs = mp4_VLC_TB23a.runOff[last][run] + lev - 1;
                        sum += mp4_VLC_TB23a.vlc[offs].len;
                        esc = 0;
                    } else
                        esc = 1;
                } else
                    esc = 1;
                if (esc) {
                    sum += 30;
                }
                if (last)
                    break;
                run = 0;
            } else
                run ++;
        }
    }
    return sum;
}


int mp4_CalcBitsCoeffsInter(const Ipp16s* pCoeffs, int countNonZero, int rvlcFlag, const Ipp8u* pScanTable)
{
    int     run, i, cnt, level, lev, last, offs, esc, sum;

    if (countNonZero == 0)
        return 0;
    sum = 0;
    cnt = 1;
    run = 0;
    if (!rvlcFlag) {
        for (i = 0; i < 64; i ++) {
            level = pCoeffs[pScanTable[i]];
            if (level) {
                lev = (level < 0) ? (-level) : level;
                last = (cnt == countNonZero);
                cnt ++;
                if (run <= mp4_VLC_TB17.runMax[last]) {
                    if (lev <= mp4_VLC_TB17.levMax[last][run]) {
                        offs = mp4_VLC_TB17.runOff[last][run] + lev - 1;
                        sum += mp4_VLC_TB17.vlc[offs].len;
                        esc = 0;
                    } else
                        esc = 1;
                } else
                    esc = 1;
                if (esc) {
                    if (run <= mp4_VLC_TB17.runMax[last]) {
                        if (lev <= 2 * mp4_VLC_TB17.levMax[last][run]) {
                            // ESC 1
                            lev -= mp4_VLC_TB17.levMax[last][run];
                            offs = mp4_VLC_TB17.runOff[last][run] + lev - 1;
                            sum += 8 + mp4_VLC_TB17.vlc[offs].len;
                            esc = 0;
                        }
                    }
                    if (esc) {
                        if (lev <= mp4_VLC_TB17.levMax[last][0]) {
                            offs = 0;
                            while (lev > mp4_VLC_RMAX_TB22[last][offs].code)
                                offs ++;
                            if (run - mp4_VLC_RMAX_TB22[last][offs].len - 1 <= mp4_VLC_RMAX_TB22[last][offs].len) {
                                // ESC 2
                                run -= mp4_VLC_RMAX_TB22[last][offs].len + 1;
                                offs = mp4_VLC_TB17.runOff[last][run] + lev - 1;
                                sum += 9 + mp4_VLC_TB17.vlc[offs].len;
                                esc = 0;
                            }
                        }
                    }
                    if (esc) {
                        // ESC 3
                        sum += 30;
                    }
                }
                if (last)
                    break;
                run = 0;
            } else
                run ++;
        }
    } else {
        for (i = 0; i < 64; i ++) {
            level = pCoeffs[pScanTable[i]];
            if (level) {
                lev = (level < 0) ? (-level) : level;
                last = (cnt == countNonZero);
                cnt ++;
                if (run <= mp4_VLC_TB23b.runMax[last]) {
                    if (lev <= mp4_VLC_TB23b.levMax[last][run]) {
                        offs = mp4_VLC_TB23b.runOff[last][run] + lev - 1;
                        sum += mp4_VLC_TB23b.vlc[offs].len;
                        esc = 0;
                    } else
                        esc = 1;
                } else
                    esc = 1;
                if (esc) {
                    sum += 30;
                }
                if (last)
                    break;
                run = 0;
            } else
                run ++;
        }
    }
    return sum;
}


void VideoEncoderMPEG4::ResetRC(Ipp32s bitRate, Ipp64f frameRate)
{
    if (mRateControl == 1) {
        mRC.Init(0, 0, 0, bitRate, frameRate, mSourceWidth, mSourceHeight, VOL.short_video_header ? 3 : 1, 31);
        mQuantIVOP = mRC.GetQP(MP4_VOP_TYPE_I);
        mQuantPVOP = mRC.GetQP(MP4_VOP_TYPE_P);
        mQuantBVOP = mRC.GetQP(MP4_VOP_TYPE_B);
    } else if (mRateControl == 2) {
        mRC_MB.Init(bitRate, frameRate, mSourceWidth, mSourceHeight, mNumMacroBlockPerRow, mNumMacroBlockPerCol, mIVOPdist, VOL.data_partitioned, VOL.short_video_header ? 3 : 1, 31);
        mQuantIVOP = mQuantPVOP = mQuantBVOP = mRC_MB.GetQP();
        mMEflags &= ~ME_4MV;
    }
}

} // namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
