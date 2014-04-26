/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_DECODER)

#include "mp3dec_own.h"
#include "ipps.h"

MP3Status mp3decGetSize_com(Ipp32s *size)
{
  *size = MAINDATABUFSIZE;
  return MP3_OK;
}

MP3Status mp3decUpdateMemMap_com(MP3Dec_com *state, Ipp32s shift)
{
  MP3_UPDATE_PTR(Ipp32u, state->m_MainData.pBuffer, shift)
  return MP3_OK;
}

MP3Status mp3decInit_com(MP3Dec_com *state, void *mem)
{
    state->MAINDATASIZE = MAINDATABUFSIZE;

    ippsZero_8u((Ipp8u *)&(state->header), sizeof(IppMP3FrameHeader));
    ippsZero_8u((Ipp8u *)&(state->header_good), sizeof(IppMP3FrameHeader));
    state->mpg25 = state->mpg25_good = 0;
    state->stereo = 0;
    state->intensity = 0;
    state->ms_stereo = 0;
    state->mc_channel = 0;
    ippsZero_8u((Ipp8u *)&state->mc_header, sizeof(mp3_mc_header));

    state->si_main_data_begin = 0;
    state->si_private_bits = 0;
    state->si_part23Len[0][0] = 0;
    state->si_part23Len[0][1] = 0;
    state->si_part23Len[1][0] = 0;
    state->si_part23Len[1][1] = 0;

    state->si_bigVals[0][0] = 0;
    state->si_bigVals[0][1] = 0;
    state->si_bigVals[1][0] = 0;
    state->si_bigVals[1][1] = 0;

    state->si_globGain[0][0] = 0;
    state->si_globGain[0][1] = 0;
    state->si_globGain[1][0] = 0;
    state->si_globGain[1][1] = 0;

    state->si_sfCompress[0][0] = 0;
    state->si_sfCompress[0][1] = 0;
    state->si_sfCompress[1][0] = 0;
    state->si_sfCompress[1][1] = 0;

    state->si_winSwitch[0][0] = 0;
    state->si_winSwitch[0][1] = 0;
    state->si_winSwitch[1][0] = 0;
    state->si_winSwitch[1][1] = 0;

    state->si_blockType[0][0] = 0;
    state->si_blockType[0][1] = 0;
    state->si_blockType[1][0] = 0;
    state->si_blockType[1][1] = 0;

    state->si_mixedBlock[0][0] = 0;
    state->si_mixedBlock[0][1] = 0;
    state->si_mixedBlock[1][0] = 0;
    state->si_mixedBlock[1][1] = 0;

    state->si_pTableSelect[0][0][0] = 0;
    state->si_pTableSelect[0][0][1] = 0;
    state->si_pTableSelect[0][0][2] = 0;
    state->si_pTableSelect[0][1][0] = 0;
    state->si_pTableSelect[0][1][1] = 0;
    state->si_pTableSelect[0][1][2] = 0;
    state->si_pTableSelect[1][0][0] = 0;
    state->si_pTableSelect[1][0][1] = 0;
    state->si_pTableSelect[1][0][2] = 0;
    state->si_pTableSelect[1][1][0] = 0;
    state->si_pTableSelect[1][1][1] = 0;
    state->si_pTableSelect[1][1][2] = 0;

    state->si_pSubBlkGain[0][0][0] = 0;
    state->si_pSubBlkGain[0][0][1] = 0;
    state->si_pSubBlkGain[0][0][2] = 0;
    state->si_pSubBlkGain[0][1][0] = 0;
    state->si_pSubBlkGain[0][1][1] = 0;
    state->si_pSubBlkGain[0][1][2] = 0;
    state->si_pSubBlkGain[1][0][0] = 0;
    state->si_pSubBlkGain[1][0][1] = 0;
    state->si_pSubBlkGain[1][0][2] = 0;
    state->si_pSubBlkGain[1][1][0] = 0;
    state->si_pSubBlkGain[1][1][1] = 0;
    state->si_pSubBlkGain[1][1][2] = 0;

    state->si_reg0Cnt[0][0] = 0;
    state->si_reg0Cnt[0][1] = 0;
    state->si_reg0Cnt[1][0] = 0;
    state->si_reg0Cnt[1][1] = 0;

    state->si_reg1Cnt[0][0] = 0;
    state->si_reg1Cnt[0][1] = 0;
    state->si_reg1Cnt[1][0] = 0;
    state->si_reg1Cnt[1][1] = 0;

    state->si_preFlag[0][0] = 0;
    state->si_preFlag[0][1] = 0;
    state->si_preFlag[1][0] = 0;
    state->si_preFlag[1][1] = 0;

    state->si_sfScale[0][0] = 0;
    state->si_sfScale[0][1] = 0;
    state->si_sfScale[1][0] = 0;
    state->si_sfScale[1][1] = 0;

    state->si_cnt1TabSel[0][0] = 0;
    state->si_cnt1TabSel[0][1] = 0;
    state->si_cnt1TabSel[1][0] = 0;
    state->si_cnt1TabSel[1][1] = 0;

    state->si_scfsi[0] = 0;
    state->si_scfsi[1] = 0;

    ippsZero_8u((Ipp8u *)&(state->ScaleFactors), 2 * sizeof(sScaleFactors));
    ippsZero_8u((Ipp8u *)&(state->huff_table), 34 * sizeof(sHuffmanTable));
    state->part2_start = 0;

    ippsZero_8u((Ipp8u *)&(state->m_MainData), sizeof(sBitsreamBuffer));

    state->decodedBytes = 0;

    ippsZero_8u((Ipp8u *)(state->allocation), 2 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scfsi), 2 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scalefactor), 2 * 3 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scalefactor), 2 * 32 * 36 * sizeof(Ipp16u));
    state->nbal_alloc_table = NULL;
    state->alloc_table = NULL;

    state->m_StreamData.pBuffer = NULL;
    state->m_StreamData.pCurrent_dword = state->m_StreamData.pBuffer;
    state->m_StreamData.nDataLen = 0;
    state->m_StreamData.nBit_offset = 0;

    state->m_MainData.pBuffer = (Ipp32u *)mem;

    state->m_MainData.nBit_offset = 0;
    state->m_MainData.nDataLen = 0;
    state->m_MainData.pCurrent_dword = state->m_MainData.pBuffer;

    state->non_zero[0] = 0;
    state->non_zero[1] = 0;

    state->m_layer = 0;

    state->m_nBitrate = 0;
    state->m_frame_num = 0;
    state->m_bInit = 0;
    state->id3_size = 0;

    return MP3_OK;
}

MP3Status mp3decReset_com(MP3Dec_com *state)
{
    state->MAINDATASIZE = MAINDATABUFSIZE;

    ippsZero_8u((Ipp8u *)&(state->header), sizeof(IppMP3FrameHeader));
    state->mpg25 = 0;
    state->stereo = 0;
    state->intensity = 0;
    state->ms_stereo = 0;
    state->mc_channel = 0;
    state->mc_header.lfe = 0;

    state->si_main_data_begin = 0;
    state->si_private_bits = 0;
    state->si_part23Len[0][0] = 0;
    state->si_part23Len[0][1] = 0;
    state->si_part23Len[1][0] = 0;
    state->si_part23Len[1][1] = 0;

    state->si_bigVals[0][0] = 0;
    state->si_bigVals[0][1] = 0;
    state->si_bigVals[1][0] = 0;
    state->si_bigVals[1][1] = 0;

    state->si_globGain[0][0] = 0;
    state->si_globGain[0][1] = 0;
    state->si_globGain[1][0] = 0;
    state->si_globGain[1][1] = 0;

    state->si_sfCompress[0][0] = 0;
    state->si_sfCompress[0][1] = 0;
    state->si_sfCompress[1][0] = 0;
    state->si_sfCompress[1][1] = 0;

    state->si_winSwitch[0][0] = 0;
    state->si_winSwitch[0][1] = 0;
    state->si_winSwitch[1][0] = 0;
    state->si_winSwitch[1][1] = 0;

    state->si_blockType[0][0] = 0;
    state->si_blockType[0][1] = 0;
    state->si_blockType[1][0] = 0;
    state->si_blockType[1][1] = 0;

    state->si_mixedBlock[0][0] = 0;
    state->si_mixedBlock[0][1] = 0;
    state->si_mixedBlock[1][0] = 0;
    state->si_mixedBlock[1][1] = 0;

    state->si_pTableSelect[0][0][0] = 0;
    state->si_pTableSelect[0][0][1] = 0;
    state->si_pTableSelect[0][0][2] = 0;
    state->si_pTableSelect[0][1][0] = 0;
    state->si_pTableSelect[0][1][1] = 0;
    state->si_pTableSelect[0][1][2] = 0;
    state->si_pTableSelect[1][0][0] = 0;
    state->si_pTableSelect[1][0][1] = 0;
    state->si_pTableSelect[1][0][2] = 0;
    state->si_pTableSelect[1][1][0] = 0;
    state->si_pTableSelect[1][1][1] = 0;
    state->si_pTableSelect[1][1][2] = 0;

    state->si_pSubBlkGain[0][0][0] = 0;
    state->si_pSubBlkGain[0][0][1] = 0;
    state->si_pSubBlkGain[0][0][2] = 0;
    state->si_pSubBlkGain[0][1][0] = 0;
    state->si_pSubBlkGain[0][1][1] = 0;
    state->si_pSubBlkGain[0][1][2] = 0;
    state->si_pSubBlkGain[1][0][0] = 0;
    state->si_pSubBlkGain[1][0][1] = 0;
    state->si_pSubBlkGain[1][0][2] = 0;
    state->si_pSubBlkGain[1][1][0] = 0;
    state->si_pSubBlkGain[1][1][1] = 0;
    state->si_pSubBlkGain[1][1][2] = 0;

    state->si_reg0Cnt[0][0] = 0;
    state->si_reg0Cnt[0][1] = 0;
    state->si_reg0Cnt[1][0] = 0;
    state->si_reg0Cnt[1][1] = 0;

    state->si_reg1Cnt[0][0] = 0;
    state->si_reg1Cnt[0][1] = 0;
    state->si_reg1Cnt[1][0] = 0;
    state->si_reg1Cnt[1][1] = 0;

    state->si_preFlag[0][0] = 0;
    state->si_preFlag[0][1] = 0;
    state->si_preFlag[1][0] = 0;
    state->si_preFlag[1][1] = 0;

    state->si_sfScale[0][0] = 0;
    state->si_sfScale[0][1] = 0;
    state->si_sfScale[1][0] = 0;
    state->si_sfScale[1][1] = 0;

    state->si_cnt1TabSel[0][0] = 0;
    state->si_cnt1TabSel[0][1] = 0;
    state->si_cnt1TabSel[1][0] = 0;
    state->si_cnt1TabSel[1][1] = 0;

    state->si_scfsi[0] = 0;
    state->si_scfsi[1] = 0;

    ippsZero_8u((Ipp8u *)&(state->ScaleFactors), 2 * sizeof(sScaleFactors));
    state->part2_start = 0;

    state->decodedBytes = 0;

    ippsZero_8u((Ipp8u *)(state->allocation), 2 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scfsi), 2 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scalefactor), 2 * 3 * 32 * sizeof(Ipp16s));
    ippsZero_8u((Ipp8u *)(state->scalefactor), 2 * 32 * 36 * sizeof(Ipp16u));

    state->m_StreamData.pBuffer = NULL;
    state->m_StreamData.pCurrent_dword = state->m_StreamData.pBuffer;
    state->m_StreamData.nDataLen = 0;
    state->m_StreamData.nBit_offset = 0;

    state->m_MainData.nBit_offset = 0;
    state->m_MainData.nDataLen = 0;
    state->m_MainData.pCurrent_dword = state->m_MainData.pBuffer;

    state->non_zero[0] = 0;
    state->non_zero[1] = 0;

    state->m_layer = 0;

//    state->m_nBitrate = 0;
    state->m_frame_num = 0;
//    state->m_bInit = 0;
    state->id3_size = -1;

    return MP3_OK;
}

MP3Status mp3decClose_com(/*MP3Dec_com *state*/)
{
    return MP3_OK;
}

#endif //UMC_ENABLE_XXX
