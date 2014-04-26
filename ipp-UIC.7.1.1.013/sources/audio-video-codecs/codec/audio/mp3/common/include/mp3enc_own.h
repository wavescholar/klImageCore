/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3ENC_OWN_H__
#define __MP3ENC_OWN_H__

#include "mp3_own.h"
#include "mp3enc.h"
#include "mp3enc_tables.h"
#include "mp3enc_hufftables.h"
#include "mp3enc_psychoacoustic.h"
#include "align.h"
#include "bstream.h"
#include "vm_types.h"

#include "ippac.h"
#include "ipps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SI_MAX 10

#define NUM_CHANNELS 5
#define LFE_CHANNEL 1

#define MAX_GBUF_COEF 16

typedef struct {
    void    *phuftable;                                             // 4|8 bytes
    Ipp16s  mav_value;                                              // 2 bytes
    Ipp16s  linbits;                                                // 2 bytes
} MP3Enc_HuffmanTable;                                              // 8|12 bytes

typedef struct {
//  Quantization
    VM_ALIGN32_DECL(Ipp16s) quant_ix[2][2][LEN_MDCT_LINE];          // 4608 bytes - OK

    VM_ALIGN32_DECL(Ipp16u) scalefac_l[2][2][32];                   // 288 bytes - OK
    VM_ALIGN32_DECL(Ipp16u) scalefac_s[2][2][12][3];                // 288 bytes - OK
    Ipp32s max_bits[2][2];   // max bit for encode granule          // 16 bytes

//  huffman tables
    MP3Enc_HuffmanTable htables[32];                                // 256|384 bytes

    IppMP3FrameHeader header;                                       // 52 bytes

// wav parameters
    Ipp32s stereo;                                                  // 4 bytes
    Ipp32s frameSize;                                               // 4 bytes
    Ipp32s grnum;                                                   // 4 bytes
    Ipp32s jschannel;                                               // 4 bytes
    Ipp32s br_mode;                                                 // 4 bytes
    Ipp32s frameBits;                                               // 4 bytes

    /* multichannel */
    Ipp32s mc_channel_conf;                                         // 4 bytes
    Ipp32s mc_sblimit, mc_lfe_filter_off;                           // 8 bytes
    mp3_mc_header mc_header;                                        // 48 bytes

    Ipp32s mc_tc_sbgr_select;                                       // 4 bytes
    Ipp32s mc_dyn_cross_on;                                         // 4 bytes
    Ipp32s mc_dyn_cross_LR;                                         // 4 bytes
    Ipp32s mc_prediction_on;                                        // 4 bytes
    Ipp32s mc_channel;                                              // 4 bytes
    Ipp32s mc_alloc_bits;                                           // 4 bytes
    Ipp32s mc_dyn_cross_bits;                                       // 4 bytes
    Ipp32s mc_pred_mode;                                            // 4 bytes
    Ipp32s mc_tc_allocation;                                        // 4 bytes
    Ipp32s mc_tc_alloc[12];                                         // 48 bytes
    Ipp32s mc_dyn_cross_mode[12];                                   // 48 bytes
    Ipp32s mc_dyn_second_stereo[12];                                // 48 bytes
    Ipp32s mc_prediction[8];                                        // 32 bytes
    Ipp32s mc_predsi[8][6];                                         // 192 bytes
    Ipp32s mc_pred_coeff[8][6][3];                                  // 576 bytes
    Ipp32s mc_delay_comp[8][6];                                     // 192 bytes
    Ipp32s mc_lfe_alloc;                                            // 4 bytes
    Ipp32s mc_lfe_scf;                                              // 4 bytes
    Ipp32s mc_lfe_spl[12];                                          // 48 bytes

// SIDE INFO
    Ipp32u si_main_data_begin;                                      // 4 bytes
    Ipp32u si_private_bits;                                         // 4 bytes
    Ipp32u si_part23Len[2][2];                                      // 16 bytes
    Ipp32u si_bigVals[2][2];                                        // 16 bytes
    Ipp32u si_count1[2][2];                                         // 16 bytes

    Ipp16s si_globGain[2][2];                                       // 8 bytes
    Ipp32u si_sfCompress[2][2];                                     // 16 bytes
    Ipp32u si_winSwitch[2][4];                                      // 32 bytes
    Ipp32u si_blockType[2][4];                                      // 32 bytes
    Ipp32u si_mixedBlock[2][4];                                     // 32 bytes
    Ipp32u si_pTableSelect[2][2][3];                                // 48 bytes
    Ipp16s si_pSubBlkGain[2][2][3];                                 // 24 bytes
    Ipp32u si_address[2][2][3];                                     // 48 bytes
    Ipp32u si_reg0Cnt[2][2];                                        // 16 bytes
    Ipp32u si_reg1Cnt[2][2];                                        // 16 bytes
    Ipp32u si_preFlag[2][2];                                        // 16 bytes
    Ipp32u si_sfScale[2][2];                                        // 16 bytes
    Ipp32u si_cnt1TabSel[2][2];                                     // 16 bytes
    Ipp32u si_scfsi[2][4];                                          // 32 bytes
    Ipp32u si_part2Len[2][2];                                       // 16 bytes

    Ipp32s slen[2][4];                                              // 32 bytes
    const Ipp32s* sfb_part_table[2];                                // 8|16 bytes

    Ipp32s sfb_l_max, sfb_s_max;                                    // 8 bytes

// END OF SIDE INFO

    sBitsreamBuffer mainStream;                                     // 56|72 bytes
    sBitsreamBuffer sideStream;                                     // 56|72 bytes

    VM_ALIGN32_DECL(Ipp32u) buffer_main_data[1024];                 // 4096 bytes - OK
    VM_ALIGN32_DECL(Ipp32u) buffer_side_info[256];                  // 1024 bytes - OK

/* l1 l2 start */
    VM_ALIGN32_DECL(Ipp16s)    allocation[NUM_CHANNELS][32];        // 320 bytes - OK
    VM_ALIGN32_DECL(Ipp16s)    scalefactor_l1[2][32];               // 128 bytes - OK
    VM_ALIGN32_DECL(Ipp32s)    sample[NUM_CHANNELS][32][36];        // 23040 bytes - OK
    VM_ALIGN32_DECL(Ipp16s)    scalefactor[NUM_CHANNELS][3][32];    // 960 bytes - OK
    VM_ALIGN32_DECL(Ipp16u)    scfsi[NUM_CHANNELS][32];             // 320 bytes - OK
    Ipp32s *nbal_alloc_table;                                       // 4|8 bytes
    Ipp8u  *alloc_table;                                            // 4|8 bytes
    Ipp32s jsbound;                                                 // 4 bytes
    Ipp32s sblimit;                                                 // 4 bytes
    Ipp32s sblimit_real;                                            // 4 bytes
/* ************ */

    Ipp32s slot_sizes[16];                                          // 64 bytes - OK
    Ipp32s slot_size;                                               // 4 bytes
    Ipp32s main_data_ptr;
    Ipp32s resr_bytes;                                              // 4 bytes
    Ipp32s resr_mod_slot;                                           // 4 bytes
    Ipp32s bytes_in_gbuf;

    Ipp32s framesNum;
    Ipp32s ns_mode;                                                 // 4 bytes
    Ipp32s stereo_mode;                                             // 4 bytes
    Ipp32s stereo_mode_param;                                       // 4 bytes
    Ipp32s stereo_mode_ext;                                         // 4 bytes

    Ipp32s quant_mode_fast;                                         // 4 bytes

    Ipp32s lowpass_maxline;                                         // 4 bytes

    VM_ALIGN32_DECL(Ipp8u)si_buf[SI_MAX][40];                       // 400 bytes
    Ipp32s si_beg, si_new, si_num;                                  // 12 bytes

    Ipp32s upsample;                                                // 4 bytes
    Ipp32s si_bits;                                                 // 4 bytes
} MP3Enc_com;

Ipp32s mp3enc_formatBitstream_l12(MP3Enc_com *state, Ipp8u *pOutputData);
Ipp32s mp3enc_formatBitstream_l3(MP3Enc_com *state, Ipp32s (*mdct_out)[2][LEN_MDCT_LINE],
                                 Ipp8u *pOutputData);

Ipp32s mp3enc_writeLastFrames(MP3Enc_com *state,
                              Ipp8u *pOutputData);

Ipp32s mp3enc_huffInit(MP3Enc_HuffmanTable *htables, Ipp8u *mem, Ipp32s *size);

Ipp32s mp3enc_quantChooseTableLong(MP3Enc_com *state, Ipp32s gr, Ipp32s ch, Ipp16s *pInput, Ipp32s length,
                                   Ipp32s table);
Ipp32s mp3enc_quantCalcBitsLong(MP3Enc_com *state, Ipp16s *pInput, Ipp32s gr, Ipp32s ch);
Ipp32s mp3enc_quantCalcBits(MP3Enc_com *state, Ipp32s gr, Ipp32s ch);

void   mp3enc_quantIterReset(MP3Enc_com *state, Ipp32s gr, Ipp32s ch);
Ipp32s mp3enc_quantCalcPart2Len(MP3Enc_com *state, Ipp32s gr, Ipp32s ch);
Ipp32s mp3enc_quantScaleBitCount(MP3Enc_com *state, Ipp32s gr, Ipp32s ch);
Ipp32s mp3enc_quantcalcPart2Len(MP3Enc_com *state, Ipp32s gr, Ipp32s ch);

Ipp32s mp3enc_mc_trans_channel(MP3Enc_com *state, Ipp32s sbgr, Ipp32s ch);

Ipp32s mp3encLEBitrate(MP3Enc_com *state, Ipp32s slot_size);
Ipp32s mp3encGEBitrate(MP3Enc_com *state, Ipp32s slot_size);

#ifdef __cplusplus
}
#endif

#endif
