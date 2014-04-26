/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "params.h"

using namespace UMC;

Ipp32s params_fill_by_default(ProgramParameters * p_param)
{
  p_param->channel_conf = 0;
  p_param->bitrate = 128000;
  p_param->timing = 0;
  p_param->puttocsv = 0;
  p_param->isMP4MuxerRequired = 0;

  return 0;
}

Status GetParams(ParserCmd *pCmd, ProgramParameters *pParams)
{
    DString sTemp;
    Ipp32u  iTemp;

#if defined UMC_ENABLE_AAC_AUDIO_ENCODER
    if(pCmd->GetParam(VM_STRING(""), VM_STRING("aac_o"),  &sTemp,     1))
    {
        if(sTemp == VM_STRING("adif"))
            (pParams->aacenc_params).outputFormat = UMC_AAC_ADIF;
        else if(sTemp == VM_STRING("adts"))
            (pParams->aacenc_params).outputFormat = UMC_AAC_ADTS;
        else if (sTemp == VM_STRING("mp4"))
        {
            (pParams->aacenc_params).outputFormat = UMC_AAC_ADTS;
            pParams->isMP4MuxerRequired = 1;
        }
        else
            return UMC_ERR_INVALID_PARAMS;
    }
    if(pCmd->GetParam(VM_STRING(""), VM_STRING("aac_p"),  &sTemp,     1))
    {
        if (sTemp == VM_STRING("lc"))
            (pParams->aacenc_params).audioObjectType = AOT_AAC_LC;
        else if (sTemp == VM_STRING("ltp"))
            (pParams->aacenc_params).audioObjectType = AOT_AAC_LTP;
        else if(sTemp == VM_STRING("he"))
        {
            (pParams->aacenc_params).audioObjectType = AOT_AAC_LC;
            (pParams->aacenc_params).auxAudioObjectType = AOT_SBR;
        }
        else
            return UMC_ERR_INVALID_PARAMS;
    }
    if(pCmd->GetParam(VM_STRING(""), VM_STRING("aac_s"),  &iTemp,     1))
    {
        if(iTemp == 0)
            pParams->aacenc_params.stereo_mode = UMC_AAC_LR_STEREO;
        else if(iTemp == 1)
            pParams->aacenc_params.stereo_mode = UMC_AAC_JOINT_STEREO;
        else if(iTemp == 2)
            pParams->aacenc_params.stereo_mode = UMC_AAC_MS_STEREO;
        else
            return UMC_ERR_INVALID_PARAMS;
    }
    pCmd->GetParam(VM_STRING(""), VM_STRING("aac_ns"), &pParams->aacenc_params.ns_mode,     1);
    if(pParams->aacenc_params.ns_mode < 0 || pParams->aacenc_params.ns_mode > 1 )
        return UMC_ERR_INVALID_PARAMS;
#endif

#if defined UMC_ENABLE_MP3_AUDIO_ENCODER
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_l"), &pParams->mp3enc_params.layer,     1);
    if(pParams->mp3enc_params.layer < 1 || pParams->mp3enc_params.layer > 3)
        return UMC_ERR_INVALID_PARAMS;
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_rc"), &pParams->mp3enc_params.mode,     1);
    if(pParams->mp3enc_params.mode < 0 || pParams->mp3enc_params.mode > 1)
        return UMC_ERR_INVALID_PARAMS;
    if(pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_s"),  &iTemp,     1))
    {
        if(iTemp == 0)
            pParams->mp3enc_params.stereo_mode = UMC_MPA_LR_STEREO;
        else if(iTemp == 1)
            pParams->mp3enc_params.stereo_mode = UMC_MPA_JOINT_STEREO;
        else if(iTemp == 2)
            pParams->mp3enc_params.stereo_mode = UMC_MPA_MS_STEREO;
        else
            return UMC_ERR_INVALID_PARAMS;
    }
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_ns"), &pParams->mp3enc_params.ns_mode,     1);
    if(pParams->mp3enc_params.ns_mode < 0 || pParams->mp3enc_params.ns_mode > 1)
        return UMC_ERR_INVALID_PARAMS;
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_mp"), &pParams->mp3enc_params.mc_matrix_procedure,     1);
    if(pParams->mp3enc_params.mc_matrix_procedure < 0 || pParams->mp3enc_params.mc_matrix_procedure > 3)
        return UMC_ERR_INVALID_PARAMS;
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_cc"),           &pParams->channel_conf,     1);
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_lfe_filt_off"), &pParams->mp3enc_params.mc_lfe_filter_off,     1);
#endif

    return UMC_OK;
}
