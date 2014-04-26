/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3ENC_OWN_FP_H__
#define __MP3ENC_OWN_FP_H__

#include "math.h"
#include "mp3enc_own.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#define ATTENUATION_DB ((Ipp32f)2.9)
//#define ATTENUATION    ((Ipp32f)0.0012589) /* -29 dB */
#define ATTENUATION    ((Ipp32f)pow(10, -1*ATTENUATION_DB)) /* -29 dB */
#define MAX_TRH_SHORT  ((Ipp32f)1e-9)

typedef Ipp32f samplefbout[2][18][32];


typedef struct
{
  VM_ALIGN32_DECL(Ipp32f) nb_long[2][__ALIGNED(MAX_PPT_LONG)];
  VM_ALIGN32_DECL(Ipp32f) nb_short[3][__ALIGNED(MAX_PPT_SHORT)];

  Ipp32s              block_type;
  Ipp32s              desired_block_type;
  Ipp32s              next_desired_block_type;

  Ipp32f              avWinEnergy;
  Ipp32f              lastWindowEnergy;
  Ipp32f              avWinEnergyLow;
  Ipp32f              lastWindowEnergyLow;
  Ipp32f              bitsToPECoeff;
  Ipp32f              scalefactorDataBits;
  Ipp32f              PEtoNeededPECoeff;


  Ipp32f              peMin;
  Ipp32f              peMax;

  Ipp32s              attackIndex;
  Ipp32s              lastAttackIndex;
} mpaPsychoacousticBlock;

typedef struct _ptbl {
  Ipp32s numlines;
  Ipp32f bval;
} ptbl;

typedef struct
{
  Ipp32s              num_ptt;
  Ipp32s*             numlines;
  Ipp32f*             bval;
  Ipp32f*             qsthr;
} mpaPsyPartitionTable;

typedef struct _p2sb {
  Ipp32s bu;
  Ipp32s bo;
  Ipp32f w1;
  Ipp32f w2;
} p2sb;

typedef struct
{
  VM_ALIGN32_DECL (Ipp32f)      noiseThr[8][__ALIGNED(SBBND_S * 3 + 3)];

  VM_ALIGN32_DECL (Ipp32f)      sprdngf_long[MAX_PPT_LONG * MAX_PPT_LONG];
  VM_ALIGN32_DECL (Ipp32f)      sprdngf_short[MAX_PPT_SHORT * MAX_PPT_SHORT];

  VM_ALIGN32_DECL (Ipp32f)      rnorm_long[MAX_PPT_LONG];
  VM_ALIGN32_DECL (Ipp32f)      rnorm_short[MAX_PPT_SHORT];

  Ipp32f*             input_data[2][3];

  Ipp32s              nb_curr_index;
  Ipp32s              nb_prev_index;

  const Ipp32u*       sfb_offset_long;
  const Ipp32u*       sfb_offset_short;
  const Ipp32u*       sfb_offset_short_interleave;
  Ipp32s              num_sfb_long;
  Ipp32s              num_sfb_short;

  Ipp32s              ns_mode;

  mpaPsyPartitionTable* longWindow;
  mpaPsyPartitionTable* shortWindow;

  Ipp32f              attackThreshold;
} mpaPsychoacousticBlockCom;

#define MAX_SECTION_NUMBER (3*SBBND_S+3)

typedef struct {
  Ipp32s windows_sequence;
  Ipp32s max_sfb;
  Ipp32s max_line;
  Ipp32s attackWindow;
  Ipp16s scale_factors[MAX_SECTION_NUMBER];
  Ipp32s sfb_width[MAX_SECTION_NUMBER];
  Ipp32f *minSNR;
  const Ipp32u* sfb_offset;
} mpaICS;

struct _MP3Enc {
    MP3Enc_com com;                                                         // 38152 bytes - OK

// mdct
    IppsMDCTFwdSpec_32f *pMDCTSpec12;                                       // 4|8 bytes
    IppsMDCTFwdSpec_32f *pMDCTSpec36;                                       // 4|8 bytes
    Ipp8u *pMDCTbuf;                                                        // 4|8 bytes

    IppsFilterSpec_PQMF_MP3 *pPQMFSpec[NUM_CHANNELS];                       // 20|40 bytes
    Ipp8u *pqmf_mem;                                                        // 4|8 bytes
    Ipp32s pqmf_size;                                                       // 4 bytes

    VM_ALIGN32_DECL(Ipp32f) mdct_win[4][36];                                // 576 bytes - OK
    Ipp32f ca[8], cs[8];                                                    // 64 bytes  - OK

//  ------//-------
    VM_ALIGN32_DECL(Ipp32f) input_buffer[NUM_CHANNELS+LFE_CHANNEL][1952];   // 46848 bytes - Ok
    VM_ALIGN32_DECL(samplefbout) fbout_data[NUM_CHANNELS];                  // 23040 bytes  - Ok
    VM_ALIGN32_DECL(Ipp32f) mdct_out[2][2][LEN_MDCT_LINE];                  // 9216 bytes  - Ok
    samplefbout *fbout[4];                                                  // 16|32 bytes
    Ipp32s fbout_prev;                                                      // 4 bytes
    Ipp32f mc_lfe_buf[12];                                                  // 48 bytes

    VM_ALIGN32_DECL(Ipp32f) pa_hann_window_long[512*2];                     // 4096 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) mdct_line_abs[LEN_MDCT_LINE];                   // 2304 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) mdct_scaled[LEN_MDCT_LINE];                     // 2304 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) mdct_rqnt[LEN_MDCT_LINE];                       // 2304 bytes - OK

    VM_ALIGN32_DECL(Ipp32f) pa_sprdngf_long[MAX_PPT_LONG * MAX_PPT_LONG];   // 20736 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) pa_rnorm_long[MAX_PPT_LONG];                    // 288 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) pa_nb_long[NUM_CHANNELS][2][MAX_PPT_LONG];      // 2880 bytes - OK

    VM_ALIGN32_DECL(Ipp32f)  fft_line_long[4][2*512+2];                     // 16416 bytes - OK

    IppsFFTSpec_R_32f* pa_pFFTSpecShort;                                    // 4|8 bytes
    IppsFFTSpec_R_32f* pa_pFFTSpecLong;                                     // 4|8 bytes
    Ipp8u* pa_pBuffer;                                                      // 4|8 bytes

    VM_ALIGN32_DECL(Ipp32f) pa_r[NUM_CHANNELS][3][512];                     // 30720 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) pa_re[NUM_CHANNELS][3][512];                    // 30720 bytes - OK
    VM_ALIGN32_DECL(Ipp32f) pa_im[NUM_CHANNELS][3][512];                    // 30720 bytes - OK

/* l12 */
    VM_ALIGN32_DECL(Ipp32f) pa_snr[NUM_CHANNELS+1][32];                     // 768 bytes - OK
    Ipp32f *snr;                                                            // 4|8 bytes
/* *** */

    Ipp32s start_common_scalefac;                                           // 4 bytes
    Ipp32s finish_common_scalefac;                                          // 4 bytes
    Ipp32s common_scalefactor_update[2];                                    // 8 bytes
    Ipp32s last_frame_common_scalefactor[2];                                // 8 bytes

    Ipp32s pa_current_f_r_index[NUM_CHANNELS];  // Can be 0, 1, 2           // 20 bytes

    Ipp32s pa_num_ptt_long;                                                 // 4 bytes
    Ipp32s pa_num_ptt_long_algn4;// pa_num_ptt_long rounded by 4 (for aligned step)

    Ipp32f bit_rate_per_ch;                                                 // 4 bytes

    VM_ALIGN32_DECL(Ipp32f)minSNRLong[MAX_SECTION_NUMBER];                  // 156 bytes OK
    VM_ALIGN32_DECL(Ipp32f)minSNRShort[MAX_SECTION_NUMBER];                 // 156 bytes OK

    VM_ALIGN32_DECL(Ipp32f)energy[8][MAX_SECTION_NUMBER];                   // 1248 bytes OK
    VM_ALIGN32_DECL(Ipp32f)logEnergy[8][MAX_SECTION_NUMBER];                // 1248 bytes OK
    VM_ALIGN32_DECL(Ipp32f)sfbPE[8][MAX_SECTION_NUMBER];                    // 1248 bytes OK
    VM_ALIGN32_DECL(Ipp32s)msMask[2][MAX_SECTION_NUMBER];                   // 1248 bytes OK

    Ipp32f chPe[8];                                                         // 32 bytes OK

    Ipp32s non_zero_line[2];                                                // 8 bytes
    Ipp32s attackWindow[4];                                                 // 16 bytes

    Ipp32s PEisCounted;                                                     // 4 bytes

    mpaPsychoacousticBlock    *psychoacoustic_block;                        // 4|8 bytes
    VM_ALIGN32_DECL(mpaPsychoacousticBlockCom) psychoacoustic_block_com;    // 31704 bytes OK
    mpaICS ics[4];

    Ipp32s mean_bits, bits_in_buf, max_bits_in_buf, maxPossibleBits;        // 16 bytes
};

typedef struct _ptblthr_12 {
  Ipp32s numlines;
  Ipp32f absthr;
} ptblthr_12;

typedef struct
{
  Ipp32s *common_scalefactor_update;
  Ipp32s *last_frame_common_scalefactor;

  /// Special variables...
  Ipp32s  start_common_scalefac;
  Ipp32s  finish_common_scalefac;
  Ipp32s  available_bits;
  Ipp32s  bits_per_frame;
  Ipp32s  min_bits_per_frame;
  Ipp32s  used_bits;
  Ipp32s  ns_mode;
  Ipp32s  sfb_width_scale;
  Ipp16s  ns_scale_factors[MAX_SECTION_NUMBER];
} sQuantizationBlock;

typedef struct {
  Ipp32f *mdct_line;
  Ipp32f *noiseThr;
  Ipp32f *minSNR;
  Ipp32f *energy;
  Ipp32f *logEnergy;
  Ipp32f *sfbPE;
  Ipp32s *isBounded;
  Ipp32f outPe;
  Ipp32s predAttackWindow;
  Ipp32s allowHoles;
} sQuantizationData;

extern const Ipp32f mp3enc_ptbl_TMN_l2[3][CBANDS_12];
extern const Ipp32f mp3enc_ptbl_minval_l12[3][CBANDS_12];
extern const Ipp32f mp3enc_ptbl_bval_l12[3][CBANDS_12];
extern const Ipp32s mp3enc_ptbl_numlines_l12[3][CBANDS_12];

extern const ptblthr_12 mp3enc_absthr[3][132];

extern const p2sb *mp3enc_p2sb_l[3];
extern const p2sb *mp3enc_p2sb_s[3];

extern const mpaPsyPartitionTable mp3_psy_partition_tables_long[2][3];
extern const mpaPsyPartitionTable mp3_psy_partition_tables_short[2][3];

MP3Status mp3enc_mdctInit(MP3Enc *state, Ipp8u *mem, Ipp32s *size);
Ipp32s mp3enc_mdctInSubband(MP3Enc *state, Ipp32f *in, Ipp32f *out, Ipp32s block_type);
Ipp32s mp3enc_mdctBlock(MP3Enc *state, int gr);

Ipp32s mp3enc_quantization_l1(MP3Enc *state);
Ipp32s mp3enc_quantization_l2(MP3Enc *state);
Ipp32s mp3enc_quantization_l3(MP3Enc *state);

MP3Status   mp3enc_psychoacousticInit(MP3Enc *state, Ipp8u *mem, Ipp32s *size);
void   mp3enc_psychoacoustic_l1(MP3Enc *state, Ipp32f **pa_buffer);
void   mp3enc_psychoacoustic_l2(MP3Enc *state, Ipp32f **pa_buffer);
void   mp3enc_psychoacoustic_l3(MP3Enc *state, Ipp32s gr, Ipp32f* pTmpBuf);

void mp3encQuantization(MP3Enc             *state,
                        sQuantizationBlock *pBlock,
                        mpaICS             *pStream,
                        sQuantizationData  *qData,
                        Ipp32s gr,
                        Ipp32s ch);

void mp3encUpdateThr(mpaICS            *pStream,
                     sQuantizationData *qData,
                     Ipp32f            NeededPE,
                     Ipp32s            *msMask,
                     Ipp32s            ms_mask_present,
                     Ipp32s            startCh,
                     Ipp32s            numCh);

MP3Status mp3encInitPsychoacousticCom(MP3Enc *state,
                                      Ipp32s mpeg_id,
                                      Ipp32s freq_index,
                                      Ipp32s ns_mode);
void mp3encInitPsychoacoustic(mpaPsychoacousticBlockCom *pBlockCom,
                              mpaPsychoacousticBlock *pBlock);

void mp3enc_mc_normalizing (MP3Enc *state);
void mp3enc_mc_matricing (MP3Enc *state);


void mp3enc_quantCalcAvailableBitsFrame(MP3Enc *state);

#ifdef __cplusplus
}
#endif

#endif
