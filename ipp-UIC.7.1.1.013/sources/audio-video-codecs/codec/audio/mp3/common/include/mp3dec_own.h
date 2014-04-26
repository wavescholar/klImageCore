/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3DEC_OWN_H__
#define __MP3DEC_OWN_H__

#include "mp3_own.h"
#include "mp3dec.h"
#include "mp3dec_huftabs.h"
#include "bstream.h"
#include "vm_debug.h"

#include "ippac.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_CHANNELS 5
#define MAINDATABUFSIZE (2 * 56000)

typedef Ipp16s sampleshort[2][LEN_MDCT_LINE];

typedef struct {
    Ipp16s l[23];    /* [sb] */
    Ipp16s s[3][13]; /* [window][sb] */
} sScaleFactors;

typedef struct
{
    Ipp32s linbits;
    void   *phuftable;
} sHuffmanTable;

typedef struct {
    IppMP3FrameHeader header;
    IppMP3FrameHeader header_good;
    Ipp32s mpg25, mpg25_good;

    Ipp32s stereo;
    Ipp32s intensity;
    Ipp32s ms_stereo;
    Ipp32s MP3nSlots;
    Ipp32u *crc_ptr, crc, crc_check;
    Ipp32s crc_nbits, crc_offset;

    mp3_mc_header mc_header;
    Ipp32s mc_tc_sbgr_select;
    Ipp32s mc_dyn_cross_on;
    Ipp32s mc_dyn_cross_LR;
    Ipp32s mc_prediction_on;
    Ipp32s mc_channel;
    Ipp32s mc_alloc_bits;
    Ipp32s mc_dyn_cross_bits;
    Ipp32s mc_pred_mode;
    Ipp32s mc_tc_allocation;
    Ipp32s mc_tc_alloc[12];
    Ipp32s mc_dyn_cross_mode[12];
    Ipp32s mc_dyn_second_stereo[12];
    Ipp32s mc_prediction[8];
    Ipp32s mc_predsi[8][6];
    Ipp32s mc_pred_coeff[8][6][3];
    Ipp32s mc_delay_comp[8][6];
    Ipp32s mc_lfe_alloc;
    Ipp32s mc_lfe_scf;
    Ipp32s mc_lfe_spl[12];
    Ipp32s mc_lfe_filter_off;
/*
 * SIDE INFO
 */
    Ipp32u si_main_data_begin;
    Ipp32u si_private_bits;
    Ipp32u si_part23Len[2][2];
    Ipp32u si_bigVals[2][2];
    Ipp16s si_globGain[2][2];
    Ipp32u si_sfCompress[2][2];
    Ipp32u si_winSwitch[2][2];
    Ipp32u si_blockType[2][2];
    Ipp32u si_mixedBlock[2][2];
    Ipp32u si_pTableSelect[2][2][3];
    Ipp16s si_pSubBlkGain[2][2][3];
    Ipp32u si_reg0Cnt[2][2];
    Ipp32u si_reg1Cnt[2][2];
    Ipp32u si_preFlag[2][2];
    Ipp32u si_sfScale[2][2];
    Ipp32u si_cnt1TabSel[2][2];
    Ipp32u si_scfsi[2];
    Ipp32s s_len[4];
    Ipp32s blocknumber;
    Ipp32s blocktypenumber;
/*
 * END OF SIDE INFO
 */

    sampleshort *smpl_xs;       // out of huffman

    sScaleFactors ScaleFactors[2];
    sHuffmanTable huff_table[34];
    Ipp32s part2_start;

    sBitsreamBuffer m_StreamData;
    sBitsreamBuffer m_MainData;

    Ipp32s non_zero[2];
    Ipp16s *m_pOutSamples;

    Ipp32s decodedBytes;

    Ipp32s MAINDATASIZE;

    Ipp16s allocation[NUM_CHANNELS][32];
    Ipp16s scfsi[NUM_CHANNELS][32];
    Ipp16s scalefactor[NUM_CHANNELS][3][32];
    Ipp16s scalefactor_l1[2][32];
    Ipp32s sample[NUM_CHANNELS][32][36];
    const Ipp32s* nbal_alloc_table;
    const Ipp8u* alloc_table;
    Ipp32s jsbound;
    Ipp32s sblimit;

    Ipp32s m_layer;

    Ipp32s m_nBitrate;
    Ipp32s m_frame_num;
    Ipp32s m_bInit;

    Ipp32s id3_size;
    Ipp32u *start_ptr;
    Ipp32s start_offset;
    Ipp32s synchro_mode;
} MP3Dec_com;

typedef Ipp32s ssfBandIndex_l[23];
typedef Ipp32s ssfBandIndex_s[14];

extern const Ipp8u mp3dec_nr_of_sfb[6][3][4];
extern const ssfBandIndex_l mp3dec_sfBandIndex_l[3][3];
extern const ssfBandIndex_s mp3dec_sfBandIndex_s[3][3];

MP3Status mp3decGetSize_com(Ipp32s *size);
MP3Status mp3decUpdateMemMap_com(MP3Dec_com *state, Ipp32s shift);
MP3Status mp3decInit_com(MP3Dec_com *state_ptr, void *mem);

MP3Status mp3decClose_com(/*MP3Dec_com *state*/);
MP3Status mp3decReset_com(MP3Dec_com *state);

Ipp32s mp3dec_audio_data_LayerI(MP3Dec_com *state);
Ipp32s mp3dec_audio_data_LayerII(MP3Dec_com *state);

Ipp32s mp3dec_initialize_huffman(sHuffmanTable *huff_table, Ipp8u *mem, Ipp32s *size);
Ipp32s mp3idec_initialize_huffman(sHuffmanTable *huff_table, Ipp8u *mem, Ipp32s *size);

Ipp32s mp3dec_Huffmancodebits(MP3Dec_com *state, Ipp32s gr, Ipp32s ch);
Ipp32s mp3idec_Huffmancodebits(MP3Dec_com *state, Ipp32s gr, Ipp32s ch);

Ipp32s mp3dec_GetScaleFactorsL3(MP3Dec_com *state, Ipp32s gr, Ipp32s ch);
Ipp32s mp3dec_GetScaleFactorsL3_LSF(MP3Dec_com *state, Ipp32s ch);

Ipp32s mp3dec_audio_data_LayerIII(MP3Dec_com *state);
Ipp32s mp3dec_audio_data_LSF_LayerIII(MP3Dec_com *state);

Ipp32s mp3dec_ReadMainData(MP3Dec_com *state);
MP3Status  mp3dec_GetSynch(MP3Dec_com *state);
MP3Status mp3dec_ReceiveBuffer(sBitsreamBuffer *m_StreamData, void *in_GetPointer, Ipp32s in_GetDataSize);
MP3Status mp3dec_GetID3Len(Ipp8u *in, Ipp32s inDataSize, MP3Dec_com *state);
MP3Status mp3dec_SkipID3(Ipp32s inDataSize, Ipp32s *skipped, MP3Dec_com *state);

void mp3dec_CRC_start(MP3Dec_com *state);
void mp3dec_CRC_update(MP3Dec_com *state, Ipp32u *crc);

void mp3dec_mc_header(MP3Dec_com *state);
void mp3dec_mc_params(MP3Dec_com *state);
void mp3dec_mc_composite_status_info(MP3Dec_com *state);
void mp3dec_mc_audio_data_l2(MP3Dec_com *state);
void mp3dec_mc_decode_scale_l2(MP3Dec_com *state);
void mp3dec_mc_decode_sample_l2 (MP3Dec_com *state);

#ifdef __cplusplus
}
#endif

#endif
