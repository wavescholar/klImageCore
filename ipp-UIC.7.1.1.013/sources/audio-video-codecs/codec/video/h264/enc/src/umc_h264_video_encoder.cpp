//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_memory_allocator.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_defs.h"
#include "umc_h264_tables.h"
#include "umc_h264_deblocking_tools.h"
#include "umc_h264_core_enc.h"
#include "vm_debug.h"

using namespace UMC;

#define H264ENC_CALL_NEW_8U(STS, TYPE, PTR) {      \
    STS = UMC_ERR_ALLOC;                   \
    PTR = (TYPE<Ipp16s, Ipp8u>*)H264_Malloc(sizeof(TYPE<Ipp16s, Ipp8u>));  \
    if (PTR)                                    \
        STS = TYPE##_Create<Ipp16s, Ipp8u>(PTR);               \
}

#define H264ENC_CALL_DELETE_8U(TYPE, PTR)  \
    if (PTR) {                          \
        TYPE##_Destroy<Ipp16s, Ipp8u>(PTR);           \
        H264_Free(PTR);                 \
        PTR = 0;                        \
    }


#define H264ENC_CALL_NEW_16U(STS, TYPE, PTR) {      \
    STS = UMC_ERR_ALLOC;                   \
    PTR = (TYPE<Ipp32s, Ipp16u>*)H264_Malloc(sizeof(TYPE<Ipp32s, Ipp16u>));  \
    if (PTR)                                    \
        STS = TYPE##_Create<Ipp32s, Ipp16u>(PTR);               \
}

#define H264ENC_CALL_DELETE_16U(TYPE, PTR)  \
    if (PTR) {                          \
        TYPE##_Destroy<Ipp32s, Ipp16u>(PTR);           \
        H264_Free(PTR);                 \
        PTR = 0;                        \
    }


H264EncoderParams::H264EncoderParams()
{
    key_interval           = 250;
    idr_interval           = 0;
    B_frame_rate           = 0;
    B_reference_mode       = 1;
    num_ref_frames         = 5;
    num_ref_to_start_code_B_slice = 1;

    constraint_set_flag[0] = false;
    constraint_set_flag[1] = true;
    constraint_set_flag[2] = false;
    constraint_set_flag[3] = false;
    constraint_set_flag[4] = false;
    constraint_set_flag[5] = false;

    num_slices                 = 0; // Autoselect
    max_slice_size             = 0;
    profile_idc                = 0; // Autoselect
    level_idc                  = 0; // Autoselect
    chroma_format_idc          = 1; // YUV420
    bit_depth_luma             = 8;
    bit_depth_chroma           = 8;
    aux_format_idc             = 0;
    bit_depth_aux              = 8;
    alpha_incr_flag            = 0;
    alpha_opaque_value         = 0;
    alpha_transparent_value    = 0;
    rate_controls.method       = H264_RCM_VBR;
    rate_controls.quantI       = 0;
    rate_controls.quantP       = 0;
    rate_controls.quantB       = 0;
    m_info.iBitrate            = 2500000;
    mv_search_method           = 2;
    me_split_mode              = 1;
    me_search_x                = 8;
    me_search_y                = 8;
    weighted_pred_flag         = false;
    weighted_bipred_idc        = 0;
    direct_pred_mode           = 0;
    deblocking_filter_idc      = 0;
    deblocking_filter_alpha    = 2;
    deblocking_filter_beta     = 2;
    transform_8x8_mode_flag    = true;
    use_default_scaling_matrix = 0;
    cabac_init_idc             = 1;
    coding_type                = 0; // frame
    m_QualitySpeed             = 0;
    quant_opt_level            = 0;
    direct_8x8_inference_flag            = false;
    qpprime_y_zero_transform_bypass_flag = false;
    entropy_coding_mode_flag             = true;

    m_do_weak_forced_key_frames      = false;
    write_access_unit_delimiters     = 0;
    use_transform_for_intra_decision = true;
}

Status H264EncoderParams::ReadParams(ParserCfg *par)
{
    UMC_CHECK(par, UMC_ERR_NULL_PTR);
    if(!par->GetNumOfkeys())
        return UMC_ERR_NOT_INITIALIZED;

    par->GetParam(VM_STRING("iProfile"),           &profile_idc, 1);
    par->GetParam(VM_STRING("iLevel"),             &level_idc, 1);
    par->GetParam(VM_STRING("bConstrained"),       &constraint_set_flag[0], 6);
    par->GetParam(VM_STRING("iChromaFormat"),      &chroma_format_idc, 1);
    par->GetParam(VM_STRING("iBitDepthLuma"),      &bit_depth_luma,    1);
    par->GetParam(VM_STRING("iBitDepthChroma"),    &bit_depth_chroma,  1);
    par->GetParam(VM_STRING("iAuxFormat"),         &aux_format_idc, 1);
    par->GetParam(VM_STRING("iBitDepthAux"),       &bit_depth_aux, 1);
    par->GetParam(VM_STRING("iAlphaOpaque"),       &alpha_opaque_value, 1);
    par->GetParam(VM_STRING("iAlphaTransparent"),  &alpha_transparent_value, 1);
    par->GetParam(VM_STRING("bAlphaIncrFlag"),     &alpha_incr_flag, 1);
    par->GetParam(VM_STRING("iPictureType"),       &coding_type, 1);
    par->GetParam(VM_STRING("iIFramesInterval"),   &key_interval, 1);
    par->GetParam(VM_STRING("iIDRInterval"),       &idr_interval, 1);
    par->GetParam(VM_STRING("iBFramesCount"),      &B_frame_rate, 1);
    par->GetParam(VM_STRING("iBReferenceMode"),    &B_reference_mode, 1);
    par->GetParam(VM_STRING("iRefFramesNum"),      &num_ref_frames, 1);
    par->GetParam(VM_STRING("iMinList1Length"),    &num_ref_to_start_code_B_slice, 1);
    par->GetParam(VM_STRING("iSlicesNum"),         &num_slices, 1);
    par->GetParam(VM_STRING("iSliceSizeLimit"),    &max_slice_size, 1);

    par->GetParam(VM_STRING("iRateControl"),        &rate_controls.method, 1);
    par->GetParam(VM_STRING("iStartQPI"),           &rate_controls.quantI, 1);
    par->GetParam(VM_STRING("iStartQPP"),           &rate_controls.quantP, 1);
    par->GetParam(VM_STRING("iStartQPB"),           &rate_controls.quantB, 1);
    par->GetParam(VM_STRING("bOptimalQuantFlag"),   &quant_opt_level, 1);

    par->GetParam(VM_STRING("iMEMethod"),           &mv_search_method, 1);
    par->GetParam(VM_STRING("iSubSplit"),           &me_split_mode, 1);
    par->GetParam(VM_STRING("iSearchX"),            &me_search_x, 1);
    par->GetParam(VM_STRING("iSearchY"),            &me_search_y, 1);
    par->GetParam(VM_STRING("bWeightPred"),         &weighted_pred_flag, 1);
    par->GetParam(VM_STRING("iWeightBipredIdc"),    &weighted_bipred_idc, 1);
    par->GetParam(VM_STRING("iDirectType"),         &direct_pred_mode, 1);
    par->GetParam(VM_STRING("bDirectInfFlag"),      &direct_8x8_inference_flag, 1);
    par->GetParam(VM_STRING("iDeblockingMode"),     &deblocking_filter_idc, 1);
    par->GetParam(VM_STRING("iDeblockFilterA"),     &deblocking_filter_alpha, 1);
    par->GetParam(VM_STRING("iDeblockFilterB"),     &deblocking_filter_beta, 1);
    deblocking_filter_alpha = deblocking_filter_alpha&~1; // must be even, since a value div2 is coded.
    deblocking_filter_beta  = deblocking_filter_beta&~1;

    par->GetParam(VM_STRING("bTransformMode"),      &transform_8x8_mode_flag, 1);
    par->GetParam(VM_STRING("iScalingMatrices"),    &use_default_scaling_matrix, 1);
    par->GetParam(VM_STRING("bTransformBypass"),    &qpprime_y_zero_transform_bypass_flag, 1);
    par->GetParam(VM_STRING("bEntropyMode"),        &entropy_coding_mode_flag, 1);
    par->GetParam(VM_STRING("iCABACInit"),          &cabac_init_idc, 1);
    par->GetParam(VM_STRING("iQuality"),            &m_QualitySpeed, 1);

    return UMC_OK;
}


H264VideoEncoder::H264VideoEncoder()
{
    m_CurrEncoderType = H264_VIDEO_ENCODER_NONE;
    m_pEncoder = NULL;
}

H264VideoEncoder::~H264VideoEncoder()
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
    {
        H264ENC_CALL_DELETE_8U(H264CoreEncoder, m_pEncoder);
    }
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
    {
        H264ENC_CALL_DELETE_16U(H264CoreEncoder, m_pEncoder);
    }
#endif
}

Status H264VideoEncoder::Init(BaseCodecParams *init)
{
    H264EncoderParams *info = DynamicCast<H264EncoderParams, BaseCodecParams> (init);
    Status res = UMC_OK;

    //Init base class (needed for external memory allocator)
    if((res = BaseCodec::Init( init )) != UMC_OK )
        return res;

    m_CurrEncoderType = H264_VIDEO_ENCODER_8U_16S;

    if(info)
    {
        if(info->bit_depth_luma > 8 || (info->chroma_format_idc && info->bit_depth_chroma > 8) || (info->aux_format_idc && info->bit_depth_aux > 8))
            m_CurrEncoderType = H264_VIDEO_ENCODER_16U_32S;
    }

    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
    {
        H264ENC_CALL_NEW_8U(res, H264CoreEncoder, m_pEncoder);
        if (res != UMC_OK)
            return (res);

        return H264CoreEncoder_Init<Ipp16s, Ipp8u>(m_pEncoder, init, m_pMemoryAllocator);
    }
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
    {
        H264ENC_CALL_NEW_16U(res, H264CoreEncoder, m_pEncoder);
        if (res != UMC_OK)
            return (res);

        return H264CoreEncoder_Init<Ipp32s, Ipp16u>(m_pEncoder, init, m_pMemoryAllocator);
    }
#endif

    return UMC_ERR_UNSUPPORTED;
}

Status H264VideoEncoder::GetFrame(MediaData *in, MediaData *out)
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_GetFrame<Ipp16s, Ipp8u>(m_pEncoder, in, out);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_GetFrame<Ipp32s, Ipp16u>(m_pEncoder, in, out);
#endif

    return UMC_ERR_NOT_INITIALIZED;
}

Status H264VideoEncoder::GetInfo(BaseCodecParams *info)
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_GetInfo<Ipp16s, Ipp8u>(m_pEncoder, info);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_GetInfo<Ipp32s, Ipp16u>(m_pEncoder, info);
#endif

    return UMC_ERR_NOT_INITIALIZED;
}

const H264PicParamSet* H264VideoEncoder::GetPicParamSet()
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_GetPicParamSet<Ipp16s, Ipp8u>(m_pEncoder);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_GetPicParamSet<Ipp32s, Ipp16u>(m_pEncoder);
#endif

    return NULL;
}

const H264SeqParamSet* H264VideoEncoder::GetSeqParamSet()
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_GetSeqParamSet<Ipp16s, Ipp8u>(m_pEncoder);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_GetSeqParamSet<Ipp32s, Ipp16u>(m_pEncoder);
#endif

    return NULL;
}

Status H264VideoEncoder::Close()
{
    Status status = UMC_OK;

    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
    {
        status = H264CoreEncoder_Close<Ipp16s, Ipp8u>(m_pEncoder);
        H264EncoderFrameList_Destroy<Ipp8u>(&((H264CoreEncoder<Ipp16s, Ipp8u>*)m_pEncoder)->m_dpb);
        H264EncoderFrameList_Destroy<Ipp8u>(&((H264CoreEncoder<Ipp16s, Ipp8u>*)m_pEncoder)->m_cpb);
    }
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
    {
        status = H264CoreEncoder_Close<Ipp32s, Ipp16u>(m_pEncoder);
        H264EncoderFrameList_Destroy<Ipp16u>(&((H264CoreEncoder<Ipp32s, Ipp16u>*)m_pEncoder)->m_dpb);
        H264EncoderFrameList_Destroy<Ipp16u>(&((H264CoreEncoder<Ipp32s, Ipp16u>*)m_pEncoder)->m_cpb);
    }
#endif
    else
        return UMC_ERR_NOT_INITIALIZED;

    BaseCodec::Close();
    return status;
}

Status H264VideoEncoder::Reset()
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_Reset<Ipp16s, Ipp8u>(m_pEncoder);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_Reset<Ipp32s, Ipp16u>(m_pEncoder);
#endif

    return UMC_ERR_NOT_INITIALIZED;
}

Status H264VideoEncoder::SetParams(BaseCodecParams* params)
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_SetParams<Ipp16s, Ipp8u>(m_pEncoder, params);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_SetParams<Ipp32s, Ipp16u>(m_pEncoder, params);
#endif

    return UMC_ERR_NOT_INITIALIZED;
}

VideoData* H264VideoEncoder::GetReconstructedFrame()
{
    if(m_CurrEncoderType == H264_VIDEO_ENCODER_8U_16S)
        return H264CoreEncoder_GetReconstructedFrame<Ipp16s, Ipp8u>(m_pEncoder);
#ifdef BITDEPTH_9_12
    else if(m_CurrEncoderType == H264_VIDEO_ENCODER_16U_32S)
        return H264CoreEncoder_GetReconstructedFrame<Ipp32s, Ipp16u>(m_pEncoder);
#endif

    return NULL;
}

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
