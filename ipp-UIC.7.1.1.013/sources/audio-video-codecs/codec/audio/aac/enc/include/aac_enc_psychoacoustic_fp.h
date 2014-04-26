/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel Integrated Performance Primitives AAC Encode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#ifndef __PSYCHOACOUSTIC_H
#define __PSYCHOACOUSTIC_H

#include "aaccmn_const.h"
#include "aac_enc_fp.h"
#include "align.h"
#include "aac_filterbank_fp.h"

#include "ipps.h"
#include "ippac.h"

#define MAX_PPT_SHORT 48
#define MAX_PPT_LONG  72
#define ATTENUATION    ((Ipp32f)0.0012589) /* -29 dB */
#define ATTENUATION_DB ((Ipp32f)2.9)

typedef struct {
  Ipp32s bu;
  Ipp32s bo;
  Ipp32f w1;
  Ipp32f w2;
} AACp2sb;

typedef struct
{
  Ipp32s              sampling_frequency;
  Ipp32s              num_ptt;
  Ipp32s*             w_low;
  Ipp32s*             w_high;
  Ipp32f*             w_width;
  Ipp32f*             bval;
  Ipp32f*             qsthr;

} sPsyPartitionTable;

typedef struct
{
  __ALIGN Ipp32f      r[2][__ALIGNED(1024)];
  __ALIGN Ipp32f      nb_long[2][__ALIGNED(MAX_PPT_LONG)];
  __ALIGN Ipp32f      nb_short[8][__ALIGNED(MAX_PPT_SHORT)];

  Ipp32s              block_type;
  Ipp32s              desired_block_type;
  Ipp32s              next_desired_block_type;

  IppsIIRState_32f*   IIRfilterState;
  Ipp32f              avWinEnergy;
  Ipp32f              bitsToPECoeff;
  Ipp32f              scalefactorDataBits;
  Ipp32f              PEtoNeededPECoeff;

  Ipp32f              peMin;
  Ipp32f              peMax;

  Ipp32s              attackIndex;
  Ipp32s              lastAttackIndex;
} sPsychoacousticBlock;

typedef struct
{
  __ALIGN Ipp32f      noiseThr[2][__ALIGNED(MAX_SFB_SHORT * 8)];

  __ALIGN Ipp32f      sprdngf_long[MAX_PPT_LONG * MAX_PPT_LONG];
  __ALIGN Ipp32f      sprdngf_short[MAX_PPT_SHORT * MAX_PPT_SHORT];

  __ALIGN Ipp32f      rnorm_long[MAX_PPT_LONG];
  __ALIGN Ipp32f      rnorm_short[MAX_PPT_SHORT];

  Ipp32f*             input_data[2][3];

  Ipp32s              iblen_long;
  Ipp32s              iblen_short;

  Ipp32s              nb_curr_index;
  Ipp32s              nb_prev_index;

  Ipp32s*             sfb_offset_long;
  Ipp32s*             sfb_offset_short;
  Ipp32s              num_sfb_long;
  Ipp32s              num_sfb_short;

  Ipp32s              ns_mode;

  sPsyPartitionTable* longWindow;
  sPsyPartitionTable* shortWindow;
  AACp2sb*            aacenc_p2sb_l;
  AACp2sb*            aacenc_p2sb_s;

  IppsFFTSpec_R_32f*  pFFTSpecShort;
  IppsFFTSpec_R_32f*  pFFTSpecLong;
  Ipp8u*              pBuffer;

  sFilterbank*        filterbank_block;

  Ipp32s              non_zero_line_long;
  Ipp32s              non_zero_line_short;

  Ipp32f              attackThreshold;
} sPsychoacousticBlockCom;

extern sPsyPartitionTable psy_partition_tables_long[];
extern sPsyPartitionTable psy_partition_tables_short[];
extern AACp2sb *aacenc_p2sb_l[];
extern AACp2sb *aacenc_p2sb_s[];

#ifdef  __cplusplus
extern "C" {
#endif

  AACStatus InitPsychoacousticCom(sPsychoacousticBlockCom* pBlock,
                                  Ipp8u* mem,
                                  Ipp32s sf_index,
                                  Ipp32s ns_mode,
                                  Ipp32s *size_all);
  void InitPsychoacoustic(sPsychoacousticBlockCom* pBlockCom,
                          sPsychoacousticBlock* pBlock);
  void Psychoacoustic(sPsychoacousticBlock** pBlock,
                      sPsychoacousticBlockCom* pBlockCom,
                      Ipp32f **mdct_line,
                      Ipp32s *window_shape,
                      Ipp32s *prev_window_shape,
                      Ipp32s stereo_mode,
                      Ipp32s numCh);

#ifdef  __cplusplus
}
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#endif//__PSYCHOACOUSTIC_H
