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
  p_param->timing = 0;
  p_param->puttocsv = 0;

#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
  (p_param->aacdec_params).ModeDecodeHEAACprofile = HEAAC_HQ_MODE;
  (p_param->aacdec_params).ModeDwnsmplHEAACprofile= HEAAC_DWNSMPL_ON;
  (p_param->aacdec_params).layer = -1;
#endif

#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
  (p_param->ac3dec_params).out_acmod = 7;
  (p_param->ac3dec_params).outlfeon = 1;
#endif

  return 0;
}

Status GetParams(ParserCmd *pCmd, ProgramParameters *pParams)
{
#if defined UMC_ENABLE_AC3_AUDIO_DECODER
    bool bDisableLFE = false;

    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_u"), &pParams->ac3dec_params.dualmonomode,      1);
    if(pParams->ac3dec_params.dualmonomode > 3)
        return UMC_ERR_INVALID_PARAMS;

    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_m"), &pParams->ac3dec_params.out_acmod,          1);
    if(pParams->ac3dec_params.out_acmod > 7)
        return UMC_ERR_INVALID_PARAMS;

    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_c"), &pParams->ac3dec_params.karaokeCapable,    1);
    if(pParams->ac3dec_params.karaokeCapable > 3)
        return UMC_ERR_INVALID_PARAMS;

    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_k"), &pParams->ac3dec_params.out_compmod,        1);
    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_x"), &pParams->ac3dec_params.drc_scaleHigh,      1);
    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_y"), &pParams->ac3dec_params.drc_scaleLow,       1);
    pCmd->GetParam(VM_STRING(""), VM_STRING("ac3_lfe_off"), &bDisableLFE,      1);
    if(bDisableLFE)
        pParams->ac3dec_params.outlfeon = 0;
#endif

#if defined UMC_ENABLE_AAC_AUDIO_DECODER
    Ipp32s iSBRMode       = 1;
    Ipp32s iPSMode        = 0;
    bool   bSBRDSOff      = false;
    pCmd->GetParam(VM_STRING(""), VM_STRING("aac_sbr_mode"),   &iSBRMode,      1);
    if(iSBRMode == 0)
        pParams->aacdec_params.flag_SBR_support_lev = SBR_DISABLE;
    else if(iSBRMode == 1)
    {
        pParams->aacdec_params.flag_SBR_support_lev   = SBR_ENABLE;
        pParams->aacdec_params.ModeDecodeHEAACprofile = HEAAC_HQ_MODE;
    }
    else if(iSBRMode == 2)
    {
        pParams->aacdec_params.flag_SBR_support_lev   = SBR_ENABLE;
        pParams->aacdec_params.ModeDecodeHEAACprofile = HEAAC_LP_MODE;
    }
    else
        return UMC_ERR_INVALID_PARAMS;

    pCmd->GetParam(VM_STRING(""), VM_STRING("aac_sbr_ds_off"), &bSBRDSOff,     1);
    if(bSBRDSOff)
        pParams->aacdec_params.ModeDwnsmplHEAACprofile = HEAAC_DWNSMPL_OFF;
    else
        pParams->aacdec_params.ModeDwnsmplHEAACprofile = HEAAC_DWNSMPL_ON;

    pCmd->GetParam(VM_STRING(""), VM_STRING("aac_ps_mode"),    &iPSMode,     1);
    if(iPSMode == 0)
        pParams->aacdec_params.flag_PS_support_lev = PS_DISABLE;
    else if(iPSMode == 1)
        pParams->aacdec_params.flag_PS_support_lev = PS_PARSER;
    else if(iPSMode == 2)
        pParams->aacdec_params.flag_PS_support_lev = PS_ENABLE_BL;
    else if(iPSMode == 3)
        pParams->aacdec_params.flag_PS_support_lev = PS_ENABLE_UR;
    else
        return UMC_ERR_INVALID_PARAMS;
#endif

#if defined UMC_ENABLE_MP3_AUDIO_DECODER
    pCmd->GetParam(VM_STRING(""), VM_STRING("mpeg_lfe_filt_off"), &pParams->mp3dec_params.mc_lfe_filter_off,     1);
#endif

    return UMC_OK;
}

