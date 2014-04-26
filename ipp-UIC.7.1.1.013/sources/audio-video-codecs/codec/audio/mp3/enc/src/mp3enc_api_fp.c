/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own_fp.h"

/****************************************************************************/

MP3Status mp3encUpdateMemMap(MP3Enc *state, Ipp32s shift)
{
  Ipp32s i;

  for (i = 0; i < 32; i++) {
    MP3_UPDATE_PTR(void, state->com.htables[i].phuftable, shift)
  }
  MP3_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->pMDCTSpec12, shift)
  MP3_UPDATE_PTR(IppsMDCTFwdSpec_32f, state->pMDCTSpec36, shift)
  MP3_UPDATE_PTR(Ipp8u, state->pMDCTbuf, shift)
  MP3_UPDATE_PTR(IppsFFTSpec_R_32f, state->pa_pFFTSpecShort, shift)
  MP3_UPDATE_PTR(IppsFFTSpec_R_32f, state->pa_pFFTSpecLong, shift)
  MP3_UPDATE_PTR(Ipp8u, state->pa_pBuffer, shift)

  MP3_UPDATE_PTR(Ipp8u, state->pqmf_mem, shift)
  for (i = 0; i < NUM_CHANNELS; i++) {
    MP3_UPDATE_PTR(IppsFilterSpec_PQMF_MP3, state->pPQMFSpec[i], shift)
  }
  for (i = 0; i < 3; i++) {
    MP3_UPDATE_PTR(samplefbout, state->fbout[i], shift);
  }
  return MP3_OK;
}

/****************************************************************************/

static void mp3encInit_layer1(MP3Enc *state)
{
  Ipp32s sb, ch, s;
  Ipp32s bit_rate, sblimit_real;

  if (state) {
    state->com.sblimit = 32;

    bit_rate = mp3_bitrate[state->com.header.id][state->com.header.layer - 1][state->com.header.bitRate] >>
      (state->com.stereo - 1);
    if (state->com.header.id == 0)
      bit_rate <<= 1;

    if (bit_rate >= 128) {
      sblimit_real = 28;
    } else if (bit_rate >= 96) {
      sblimit_real = 23;
    } else if (bit_rate >= 56) {
      sblimit_real = 22;
    } else {
      sblimit_real = 16;
    }

    state->com.sblimit_real = sblimit_real;

    for(sb = 0; sb < 32; sb++)
      for(ch = 0; ch < 2; ch++)
        state->com.allocation[ch][sb] = 0;

    for(sb = 0; sb < 32; sb++)
      for(ch = 0; ch < 2; ch++)
        state->com.scalefactor_l1[ch][sb] = 0;

    for(s = 0; s < 12; s++)
      for(sb = 0; sb < 32; sb++)
        for(ch = 0; ch < 2; ch++)
          state->com.sample[ch][sb][s] = 0;
  }
}

/****************************************************************************/

static void mp3encInit_layer2(MP3Enc *state)
{
  Ipp32s sb, ch, s;
  Ipp32s bit_rate, sblimit_real;

  if (state) {
    mp3_SetAllocTable(state->com.header.id,
      0,
      state->com.header.layer,
      state->com.header.bitRate,
      state->com.header.samplingFreq,
      state->com.stereo,
      (const Ipp32s**)&state->com.nbal_alloc_table,
      (const Ipp8u**)&state->com.alloc_table,
      &state->com.sblimit);

    state->com.mc_sblimit = state->com.sblimit;
    state->com.mc_tc_sbgr_select = 1;

    bit_rate = mp3_bitrate[state->com.header.id][state->com.header.layer - 1][state->com.header.bitRate] >>
      (state->com.stereo - 1);
    if (state->com.header.id == 0)
      bit_rate <<= 1;

    if (state->com.stereo_mode_param == MPA_JOINT_STEREO && bit_rate > 48)
      state->com.stereo_mode_param = MPA_LR_STEREO;

    if (bit_rate >= 128) {
      sblimit_real = 28;
    } else if (bit_rate >= 96) {
      sblimit_real = 23;
    } else if (bit_rate >= 56) {
      sblimit_real = 22;
    } else {
      sblimit_real = 32;
    }

    if (sblimit_real > state->com.sblimit)
      sblimit_real = state->com.sblimit;
    state->com.sblimit_real = sblimit_real;

    state->com.jsbound = state->com.sblimit;

    for(sb = 0; sb < 32; sb++)
      for(ch = 0; ch < NUM_CHANNELS; ch++)
        state->com.allocation[ch][sb] = 0;

    for(ch = 0; ch < NUM_CHANNELS; ch++)
      for (s = 0; s < 3; s++)
        for(sb = 0; sb < 32; sb++)
          state->com.scalefactor[ch][s][sb] = 0;

    for(s = 0; s < 36; s++)
      for(sb = 0; sb < 32; sb++)
        for(ch = 0; ch < NUM_CHANNELS; ch++)
          state->com.sample[ch][sb][s] = 0;
  }
}

/****************************************************************************/

#define MIN_SNR_LONG 0.00316f
#define MAX_SNR_LONG 0.631f

#define MIN_SNR_SHORT 0.00316f
#define MAX_SNR_SHORT 0.631f

typedef struct {
  Ipp32f bitrate;
  Ipp32f cutoffFreq;
} mp3encCutoffFreq;

static mp3encCutoffFreq cutoffFreqTable[] = {
  {0,      0}, { 4,  2200}, { 8,  3600}, {12,  4500},
  {16,  5800}, {20,  7200}, {24,  8200}, {28,  9300},
  {32, 10300}, {40, 14000}, {48, 15700}, {56, 17500},
  {64, 19000}, {96, 19500}, {128, 20000}, {160, 20500}
};

static void mp3encInit_layer3(MP3Enc *state,
                              Ipp8u  *mem,
                              Ipp32s *size_all)
{
  const Ipp32u* sfb_long = NULL, *sfb_short = NULL;
  Ipp32f norm;
  Ipp32s j;
  Ipp32s sfb_l_max = 0, sfb_s_max = 0;
  Ipp32s size, ts, ind;
  Ipp32f cutoff_frequency, samplingFreq;
  Ipp32f normBitratePerChannel;

  size = 0;

  mp3enc_mdctInit(state, mem, &ts);
  size += ts;
  if (mem) mem += ts;

  mp3enc_huffInit(mem ? state->com.htables : NULL, mem, &ts);
  size += ts;
  if (mem) mem += ts;

  if (state) {
    state->com.si_main_data_begin = 0;
    state->com.resr_bytes = 0;
    state->com.resr_mod_slot = 0;

    samplingFreq = (Ipp32f)(mp3_frequency[state->com.header.id][state->com.header.samplingFreq]);

    normBitratePerChannel = state->bit_rate_per_ch;

    if (normBitratePerChannel >= 160) {
      cutoff_frequency = cutoffFreqTable[15].cutoffFreq;
    } else {
      ind = 0;
      while(cutoffFreqTable[ind+1].bitrate < normBitratePerChannel) {
        ind++;
      }

      cutoff_frequency = cutoffFreqTable[ind].cutoffFreq +
                         ((normBitratePerChannel - cutoffFreqTable[ind].bitrate) *
                          (cutoffFreqTable[ind+1].cutoffFreq - cutoffFreqTable[ind].cutoffFreq) /
                          (cutoffFreqTable[ind+1].bitrate - cutoffFreqTable[ind].bitrate));
    }

    cutoff_frequency *= (samplingFreq / 44100);

    if (cutoff_frequency > 0.5f * samplingFreq) {
      cutoff_frequency = 0.5f * samplingFreq;
    }

    state->com.lowpass_maxline = (Ipp32s)(64 * cutoff_frequency / samplingFreq);

    if ((state->com.lowpass_maxline * samplingFreq) < (64 * cutoff_frequency)) {
      state->com.lowpass_maxline++;
    }

    if (state->com.lowpass_maxline > 32)
      state->com.lowpass_maxline = 32;

    state->non_zero_line[0] = (Ipp32s)((1152 * cutoff_frequency) / samplingFreq);
    state->non_zero_line[1] = (Ipp32s)((384 * cutoff_frequency) / samplingFreq);

    sfb_long = mp3enc_sfBandIndex[state->com.header.id][state->com.header.samplingFreq].l;
    sfb_short = mp3enc_sfBandIndex[state->com.header.id][state->com.header.samplingFreq].s;

    for (j = 0; j <= SBBND_L; j++) {
      if ((Ipp32s)sfb_long[j] < state->non_zero_line[0]) {
        sfb_l_max = j + 1;
      }
    }

    for (j = 0; j <= SBBND_S; j++) {
      if ((Ipp32s)sfb_short[j] < state->non_zero_line[1]) {
        sfb_s_max = j + 1;
      }
    }

    state->com.sfb_l_max = sfb_l_max;
    state->com.sfb_s_max = sfb_s_max;

    state->psychoacoustic_block_com.num_sfb_long = state->com.sfb_l_max;
    state->psychoacoustic_block_com.num_sfb_short = state->com.sfb_s_max;

    for (j = 0; j < state->com.stereo; j++) {
      Ipp32f *peMin = &state->psychoacoustic_block[j].peMin;
      Ipp32f *peMax = &state->psychoacoustic_block[j].peMax;

      peMin[0] = 0.95f * ATTENUATION_DB * state->non_zero_line[0];
      peMax[0] = 1.05f * ATTENUATION_DB * state->non_zero_line[0];
      if (state->com.stereo == 2) {
        peMin[0] *= 2;
        peMax[0] *= 2;
      }
    }

    state->common_scalefactor_update[0] = 2;
    state->common_scalefactor_update[1] = 2;
    state->last_frame_common_scalefactor[0] = 210;
    state->last_frame_common_scalefactor[1] = 210;

    state->com.si_private_bits = 0;

    state->com.si_beg = 0;
    state->com.si_new = 0;
    state->com.si_num = 0;
    if (state->com.header.id)
      state->com.grnum = 2;
    else
      state->com.grnum = 1;

    state->fbout_prev = 0;

    for (j = 0; j < 4; j++) {
      Ipp32s ind;
      ind = state->fbout_prev + j;
      if (ind > 3) ind -= 4;
      state->fbout[j] = &state->fbout_data[ind];
    }

    /* state->mean_bits calculation */
    mp3enc_quantCalcAvailableBitsFrame(state);

    /* min SNR calculation */
    /* long window */

    for (j = 0; j <= SBBND_L; j++) {
      Ipp32f normFreq = sfb_long[j+1] * (Ipp32f)(mp3_frequency[state->com.header.id][state->com.header.samplingFreq])/1152.0f;

      state->minSNRLong[j] =
        (Ipp32f)(13*atan(0.00076*normFreq) + 3.5*atan(normFreq*normFreq/(7500 * 7500)));
    }

    norm = state->minSNRLong[0];
    for (j = state->com.sfb_l_max - 1; j > 0; j--) {
      state->minSNRLong[j] -= state->minSNRLong[j-1];
      norm += state->minSNRLong[j];
    }

    norm = (Ipp32f)(0.5 * 1.6 * (state->mean_bits/state->com.stereo - 50) / norm);

    for (j = 0; j < state->com.sfb_l_max; j++) {
      Ipp32s width = sfb_long[j+1] - sfb_long[j];
      Ipp32f tmp;
      tmp = (Ipp32f)pow(2, state->minSNRLong[j]*norm/width);
      if (tmp < 1) tmp = 1;
      tmp = 1 / tmp;
      if (tmp < MIN_SNR_LONG) tmp = MIN_SNR_LONG;
      if (tmp > MAX_SNR_LONG) tmp = MAX_SNR_LONG;
      state->minSNRLong[j] = tmp;
    }

    /* short window */

    for (j = 0; j <= SBBND_S; j++) {
      Ipp32f normFreq = sfb_short[j+1] * (Ipp32f)(mp3_frequency[state->com.header.id][state->com.header.samplingFreq])/384.0f;

      state->minSNRShort[j] =
        (Ipp32f)(13*atan(0.00076*normFreq) + 3.5*atan(normFreq*normFreq/(7500 * 7500)));
    }

    norm = state->minSNRShort[0];
    for (j = state->com.sfb_s_max - 1; j > 0; j--) {
      state->minSNRShort[j] -= state->minSNRShort[j-1];
      norm += state->minSNRShort[j];
    }

    norm = (Ipp32f)(0.5 * 1.6 * (state->mean_bits/state->com.stereo - 100) / (3 * norm));

    for (j = 0; j < state->com.sfb_s_max; j++) {
      Ipp32s width = sfb_short[j+1] - sfb_short[j];
      Ipp32f tmp;
      tmp = (Ipp32f)pow(2, state->minSNRShort[j]*norm/width);
      if (tmp < 1) tmp = 1;
      tmp = 1 / tmp;
      if (tmp < MIN_SNR_SHORT) tmp = MIN_SNR_SHORT;
      if (tmp > MAX_SNR_SHORT) tmp = MAX_SNR_SHORT;
      state->minSNRShort[j] = tmp;
    }

    for (j = state->com.sfb_s_max - 1; j >= 0; j--) {
      state->minSNRShort[3*j] = state->minSNRShort[j];
      state->minSNRShort[3*j+1] = state->minSNRShort[j];
      state->minSNRShort[3*j+2] = state->minSNRShort[j];
    }
  }

  if(size_all)
    *size_all = size;
}

/****************************************************************************/

static Ipp32s mp3encCalcSlotSize(MP3Enc *state) {
  IppMP3FrameHeader *header = &state->com.header;
  Ipp32s i, size;

  for (i = 1; i < 15; i++) {
    size = 0;

    header->paddingBit = 0;

    if (header->layer == 3) {
      size = 72000 * (header->id + 1);
    } else if (header->layer == 2) {
      size = 72000 * 2;
    } else if (header->layer == 1) {
      size = 12000;
    }

    size = size * mp3_bitrate[header->id][header->layer - 1][i] /
      mp3_frequency[header->id][header->samplingFreq];

    if (header->layer == 1)
      size *= 4;

    state->com.slot_sizes[i] = size;
  }

  state->com.slot_size = state->com.slot_sizes[header->bitRate];
  /*
  up = state->com.frameSize * mp3enc_bitrate[header->layer-1][header->bitRate] * 1000;
  state->com.slot_size = up / (mp3enc_frequency[header->samplingFreq] << 3);
  l = state->com.slot_size * mp3enc_frequency[header->samplingFreq] << 3;

  if (up - l < mp3enc_frequency[header->samplingFreq] << 2)
  header->paddingBit = 0;
  else
  header->paddingBit = 1;

  state->com.slot_size += header->paddingBit;*/
  return 0;
}

/****************************************************************************/

MP3Status mp3encInit(MP3Enc *state,
                     Ipp32s sampling_frequency,
                     Ipp32s stereo,
                     Ipp32s channel_mask,
                     Ipp32s layer,
                     Ipp32s bitrate,
                     Ipp32s br_mode,
                     Ipp32s stereo_mode,
                     Ipp32s ns_mode,
                     Ipp32s force_mpeg1,
                     Ipp32s dematrix_procedure,
                     Ipp32s mc_lfe_filter_off,
                     Ipp32s *size_all)
{
    Ipp32s j;
    Ipp32s freq, id, br, upsample = 0;
    Ipp32s size, ts;
    Ipp8u *mem;
    Ipp32s channels, mc_center, mc_surround, mc_lfe, mc_conf, mc_channel;

    mc_center = mc_surround = mc_lfe = mc_conf = 0;
    mc_channel = 0;

    size = 0;
    mem = NULL;

    switch (sampling_frequency) {
    case 44100:
      freq = 0;
      id = 1;
      break;
    case 48000:
      freq = 1;
      id = 1;
      break;
    case 32000:
      freq = 2;
      id = 1;
      break;
    case 22050:
      freq = 0;
      id = 0;
      break;
    case 24000:
      freq = 1;
      id = 0;
      break;
    case 16000:
      freq = 2;
      id = 0;
      break;
    case 11025:
      freq = 0;
      upsample = 1;
      id = 0;
      break;
    case 12000:
      freq = 1;
      id = 0;
      upsample = 1;
      break;
    case 8000:
      freq = 2;
      id = 0;
      upsample = 1;
      break;
    default:
      return MP3_FAILED_TO_INITIALIZE;
    }

    if (layer < 1 || layer > 3) {
      return MP3_FAILED_TO_INITIALIZE;
    }
    if (ns_mode < 0 || ns_mode > 1) {
      return MP3_FAILED_TO_INITIALIZE;
    }
    if (mc_lfe_filter_off < 0 || mc_lfe_filter_off > 1) {
      return MP3_FAILED_TO_INITIALIZE;
    }

    if (br_mode != MPAENC_CBR && br_mode != MPAENC_ABR) {
      return MP3_FAILED_TO_INITIALIZE;
    }

    if (layer == 2) {
      Ipp32s chnum = 0;
      channels = stereo;
      stereo = 0;

      if (!channel_mask) {
        switch(channels) {
        case 1:
          channel_mask = MP3_CHANNEL_CENTER;
          break;
        case 2:
          channel_mask = MP3_CHANNEL_STEREO;
          break;
        case 3:
          channel_mask = MP3_CHANNEL_CENTER;
          channel_mask |= MP3_CHANNEL_STEREO;
          break;
        case 4:
          channel_mask = MP3_CHANNEL_CENTER;
          channel_mask |= MP3_CHANNEL_STEREO;
          channel_mask |= MP3_CHANNEL_LOW_FREQUENCY;
          break;
        case 5:
          channel_mask = MP3_CHANNEL_CENTER;
          channel_mask |= MP3_CHANNEL_STEREO;
          channel_mask |= MP3_CHANNEL_SURROUND_STEREO;
          break;
        case 6:
          channel_mask = MP3_CHANNEL_CENTER;
          channel_mask |= MP3_CHANNEL_STEREO;
          channel_mask |= MP3_CHANNEL_SURROUND_STEREO;
          channel_mask |= MP3_CHANNEL_LOW_FREQUENCY;
          break;
        default:
          return MP3_FAILED_TO_INITIALIZE;
        }
      }

      if (channel_mask & MP3_CHANNEL_STEREO) {
        if (channel_mask & MP3_CHANNEL_CENTER) {
          chnum += 3;
          stereo = 2;
          mc_center = 1;
        } else {
          chnum += 2;
          stereo = 2;
        }
      } else if (channel_mask & MP3_CHANNEL_CENTER) {
        chnum ++;
        stereo = 1;
      }

      if ((channel_mask & MP3_CHANNEL_SURROUND_STEREO) && (channel_mask & MP3_CHANNEL_SURROUND_MONO)) {
        return MP3_FAILED_TO_INITIALIZE;
      } else if (channel_mask & MP3_CHANNEL_SURROUND_STEREO) {
        chnum += 2;
        mc_surround = 2;
      } else if (channel_mask & MP3_CHANNEL_SURROUND_MONO) {
        chnum ++;
        mc_surround = 1;
      } else if (channel_mask & MP3_CHANNEL_SURROUND_STEREO_P2) {
        chnum += 2;
        mc_surround = 3;
      }

      if (channel_mask & MP3_CHANNEL_LOW_FREQUENCY) {
        if (!mc_surround && !mc_center) {
          return MP3_FAILED_TO_INITIALIZE;
        }
        mc_lfe = 1;
      }

      mc_conf = (stereo + mc_center) * 100;
      if (mc_surround < 3)
        mc_conf += mc_surround * 10;
      else
        mc_conf += 2;

      switch (mc_conf) {
        case 320:
        case 302:
        case 310:
        case 220:
        case 202:
        case 300:
        case 210:
        case 200:
        case 100:
          break;
        default:
          return MP3_FAILED_TO_INITIALIZE;
      }
      mc_channel = chnum - stereo;
    }

    if (mc_channel)
      force_mpeg1 = 1;

    if (force_mpeg1 && id == 0) {
      id = 1;
      upsample++;
    }

    if (stereo != 1 && stereo != 2)
      return MP3_UNSUPPORTED;
    if(!mp3enc_checkBitRate(id, layer, stereo + mc_channel, bitrate, &br)) {
      return MP3_FAILED_TO_INITIALIZE;
    }

    if ((layer == 1 || (layer == 2 && mc_channel)) &&
      (stereo_mode == MPA_JOINT_STEREO || stereo_mode == MPA_MS_STEREO)) {
        stereo_mode = MPA_LR_STEREO;
    }

    ts = __ALIGNED(sizeof(MP3Enc));
    size += ts;
    if (state)
      mem = (Ipp8u *)state + ts;

    if(state) {
      ippsZero_8u((Ipp8u *)state, ts);

      state->com.mc_header.center = mc_center;
      state->com.mc_header.surround = mc_surround;
      state->com.mc_header.lfe = mc_lfe;
      state->com.mc_channel_conf = mc_conf;
      state->com.mc_channel = mc_channel;
      state->com.mc_header.dematrix_procedure = dematrix_procedure;
      state->com.mc_lfe_filter_off = mc_lfe_filter_off;

      state->com.upsample = upsample;
      state->com.header.samplingFreq = freq;
      state->com.header.bitRate = br;

      state->com.header.id = id;
      state->com.header.layer = layer;   // layer 3
      state->com.header.protectionBit = 1; // enable CRC

      if (br == 1)
        br_mode = MPAENC_CBR; // use CBR for minimal bitrate

      state->com.br_mode = br_mode;
      state->com.stereo = stereo;
      mp3encGetFrameSize(&(state->com.frameSize), id ,layer, state->com.upsample);

      state->com.si_main_data_begin = 0;
      state->com.resr_bytes = 0;
      state->com.resr_mod_slot = 0;

      /* ns_mode is obsolete now because */
      /* to increase performance of encoder we will use */
      /* fast quantization mode only */

      if (ns_mode == 0) {
        state->com.quant_mode_fast = 1;
      } else {
        state->com.quant_mode_fast = 0;
      }

      ns_mode = 1;

      state->com.ns_mode = ns_mode;
      state->com.stereo_mode =
        state->com.stereo_mode_param = stereo_mode;

      if (stereo == 2 && state->com.stereo_mode_param == MPA_JOINT_STEREO)
        state->com.jschannel = 1;
      else
        state->com.jschannel = 0;

      state->bit_rate_per_ch = (Ipp32f)(mp3_bitrate[state->com.header.id]
      [state->com.header.layer-1][state->com.header.bitRate] / state->com.stereo);

      state->bit_rate_per_ch = state->bit_rate_per_ch * 44100.0f/
                             ((Ipp32f)(mp3_frequency[state->com.header.id][state->com.header.samplingFreq]));

      /* filling Hann windows */
      if (layer == 3) {
        for (j = 0; j < 1024; j++)
          state->pa_hann_window_long[j] =
          (Ipp32f)(0.5f * (1 - cos(PI * (j + 0.5f) / 512)));
      }


      ippsZero_32f(&state->input_buffer[0][0],2*(1952));

      mp3encCalcSlotSize(state);
    }

    ippsAnalysisFilterGetSize_PQMF_MP3_32f(&ts);
    ts = __ALIGNED(ts);
    size += ts * NUM_CHANNELS;

    if (mem) {
      state->pqmf_size = ts;
      state->pqmf_mem = mem;
      for (j = 0; j < NUM_CHANNELS; j++) {
        state->pPQMFSpec[j] = (IppsFilterSpec_PQMF_MP3*)(state->pqmf_mem + state->pqmf_size * j);
        ippsAnalysisFilterInit_PQMF_MP3_32f(&state->pPQMFSpec[j],
          state->pqmf_mem + state->pqmf_size * j);
      }
      mem += ts * NUM_CHANNELS;
    }

    if (layer == 3) {
      mp3encInitPsychoacousticCom(state, id, freq, ns_mode);

      size += 2 * __ALIGNED(sizeof(mpaPsychoacousticBlock));
      if (mem) {
        state->psychoacoustic_block = (mpaPsychoacousticBlock *)mem;
        mem += 2 *  __ALIGNED(sizeof(mpaPsychoacousticBlock));
        for (j = 0; j < 2; j++) {
          mp3encInitPsychoacoustic(&state->psychoacoustic_block_com,
                                   &state->psychoacoustic_block[j]);
        }
      }
    } else {
      mp3enc_psychoacousticInit(state, mem, &ts);
      ts =  __ALIGNED(ts);
      size += ts;
      if (mem) mem += ts;
    }

    ts = 0;
    switch(layer) {
      case 1:
        mp3encInit_layer1(state);
        break;
      case 2:
        mp3encInit_layer2(state);
        break;
      case 3:
        mp3encInit_layer3(state, mem, &ts);
        break;
      default:
        break;
    }

    ts = __ALIGNED(ts);
    size += ts;
    if (mem) mem += ts;

    if(size_all)
      *size_all = size;

    return MP3_OK;
}

/****************************************************************************/

static void mp3encClose_layer3(MP3Enc *state)
{
  state->com.si_private_bits = 0;
}

/****************************************************************************/

MP3Status mp3encClose(MP3Enc *state)
{
  if (NULL == state){
    return MP3_OK;
  }

  switch(state->com.header.layer) {
      case 1:
        break;
      case 2:
        break;
      case 3:
        mp3encClose_layer3(state);
        break;
      default:
        break;
  }

  return MP3_OK;
}

/****************************************************************************/

static void mp3encPEcalculation(MP3Enc *state,
                                const Ipp32u* sfb_offset,
                                Ipp32s max_sfb,
                                Ipp32s gr,
                                Ipp32s ch)
{
  Ipp32s  curCh = gr * 4 + ch;
  Ipp32f *energy = state->energy[curCh];
  Ipp32f *logEnergy = state->logEnergy[curCh];
  Ipp32f *noiseThr = state->psychoacoustic_block_com.noiseThr[curCh];
  Ipp32f *sfbPE = state->sfbPE[curCh];
  Ipp32s sfb;

  state->chPe[curCh] = 0;

  for (sfb = 0; sfb < max_sfb; sfb++) {
    Ipp32s width = sfb_offset[sfb+1] - sfb_offset[sfb];
    Ipp32f tmp;

    if (energy[sfb] <= 0) {
      logEnergy[sfb] = -100000000;
    } else {
      logEnergy[sfb] = (Ipp32f)log10(energy[sfb]);
    }

    if (noiseThr[sfb] > 0) {
      tmp = (Ipp32f)log10(noiseThr[sfb]);
    } else {
      tmp = -100000000;
    }

    if (logEnergy[sfb] > tmp) {
      sfbPE[sfb] = width * (logEnergy[sfb] - tmp);
      state->chPe[curCh] += sfbPE[sfb];
    } else {
      sfbPE[sfb] = -1;
    }
  }
}

/****************************************************************************/

static void mp3encMSdecision(MP3Enc *state)
{
  const Ipp32u* sfb_offset = NULL;
  Ipp32f noiseThrLeft, noiseThrRight;
  Ipp32s gr, i;
  Ipp32f peLR, peMS;
  Ipp32s sfb, max_sfb;

  state->com.stereo_mode = MPA_MS_STEREO;
  peLR = 0;
  peMS = 0;

  for (gr = 0; gr < state->com.grnum; gr++) {
    mpaPsychoacousticBlockCom *pBlockCom = &(state->psychoacoustic_block_com);

    if (state->com.si_blockType[gr][0] != SHORT_TYPE) {
      max_sfb = state->com.sfb_l_max;
      sfb_offset = pBlockCom->sfb_offset_long;

    } else {
      max_sfb = 3 * state->com.sfb_s_max;
      sfb_offset = pBlockCom->sfb_offset_short_interleave;
    }

    for (sfb = 0; sfb < max_sfb; sfb++) {
      Ipp32f tmp;
      Ipp32s sfb_start = sfb_offset[sfb];
      Ipp32s width = sfb_offset[sfb+1] - sfb_offset[sfb];

      state->ics[0].sfb_width[sfb] = state->ics[1].sfb_width[sfb] = width;

      ippsDotProd_32f(state->mdct_out[gr][0] + sfb_start,
                      state->mdct_out[gr][0] + sfb_start,
                      width, state->energy[4 * gr + 0] + sfb);

      ippsDotProd_32f(state->mdct_out[gr][1] + sfb_start,
                      state->mdct_out[gr][1] + sfb_start,
                      width, state->energy[4 * gr + 1] + sfb);

      ippsDotProd_32f(state->mdct_out[gr][0] + sfb_start,
                      state->mdct_out[gr][1] + sfb_start,
                      width, &tmp);

      state->energy[4 * gr + 2][sfb] = (state->energy[4 * gr + 0][sfb] +
                                        state->energy[4 * gr + 1][sfb]) * 0.5f + tmp;
      state->energy[4 * gr + 3][sfb] = (state->energy[4 * gr + 0][sfb] +
                                        state->energy[4 * gr + 1][sfb]) * 0.5f - tmp;

      noiseThrLeft = state->psychoacoustic_block_com.noiseThr[4 * gr + 0][sfb];
      noiseThrRight = state->psychoacoustic_block_com.noiseThr[4 * gr + 1][sfb];

      if (noiseThrLeft < noiseThrRight) {
        state->psychoacoustic_block_com.noiseThr[4 * gr + 2][sfb] = noiseThrLeft;
        state->psychoacoustic_block_com.noiseThr[4 * gr + 3][sfb] = noiseThrLeft;
      } else {
        state->psychoacoustic_block_com.noiseThr[4 * gr + 2][sfb] = noiseThrRight;
        state->psychoacoustic_block_com.noiseThr[4 * gr + 3][sfb] = noiseThrRight;
      }
    }

    for (i = 0; i < 4; i++) {
      mp3encPEcalculation(state, sfb_offset, max_sfb, gr, i);
    }

    for (sfb = 0; sfb < max_sfb; sfb++) {
      Ipp32f tmpPe[4];

      tmpPe[0] = state->sfbPE[4 * gr + 0][sfb];
      tmpPe[1] = state->sfbPE[4 * gr + 1][sfb];
      tmpPe[2] = state->sfbPE[4 * gr + 2][sfb];
      tmpPe[3] = state->sfbPE[4 * gr + 3][sfb];

      for (i = 0; i < 4; i++) {
        if (tmpPe[i] < 0)
          tmpPe[i] = 0;
      }

      state->msMask[gr][sfb] = MPA_LR_STEREO;

      peLR += (tmpPe[0] + tmpPe[1]);
      peMS += (tmpPe[2] + tmpPe[3]);

      if (tmpPe[0] + tmpPe[1] > tmpPe[2] + tmpPe[3]) {
          state->msMask[gr][sfb] = MPA_MS_STEREO;
      }
    }
  }

  if (peLR < 0.9f * peMS) {
    state->com.stereo_mode = MPA_LR_STEREO;
  }
}

/****************************************************************************/

static Ipp32s mpaencMaxBitsForScalef[2][2] =
{
  {86, 144},
  {59, 126}
};

#define MIN_PE_UP     0.3f
#define MIN_PE_DOWN   0.14f
#define MAX_PE_UP     1.0f
#define MAX_PE_DOWN   0.07f

#define MIN_DELTA_PE  (1.0f/6.0f)

static void layer3_enc(MP3Enc *state,
                       Ipp32s gr)
{
  Ipp32s i, win_seq, sfb;
  Ipp32s stereo = state->com.stereo;
  sQuantizationBlock quantization_block;
  sQuantizationData  qData[2];
  Ipp32f minSNR[2][MAX_SECTION_NUMBER];
  Ipp32s isBounded[2][MAX_SECTION_NUMBER];
  Ipp32f *p_mdct_line;
  Ipp32f *bitsToPeCoeff;
  Ipp32s maxBitsPerFrame, minBitsPerFrame, available_bits;
  Ipp32f minMul, maxMul, bitMul;
  Ipp32f startMinMul, startMaxMul;
  Ipp32f slopeMinMul, slopeMaxMul;
  Ipp32f deltaPe, maxDeltaPe, peCoeff;
  Ipp32f deltaFullness;
  Ipp32f neededPE;
  Ipp32s max_bits_in_buf;
  Ipp32s bits_per_frame, bits_in_buf;
  Ipp32f *peMin, *peMax;
  Ipp32s used_bits, bitsForHuffman;
  Ipp32f chPe[2], pe, outPe;
  Ipp32f jointStereo;

  quantization_block.ns_mode = state->psychoacoustic_block_com.ns_mode;

  for (i = 0; i < stereo; i++) {
    mpaPsychoacousticBlockCom *pBlockCom = &(state->psychoacoustic_block_com);

    win_seq = state->ics[i].windows_sequence =
      ((state->com.si_blockType[gr][i] == SHORT_TYPE) ? 1 : 0);

    if (win_seq == 0) {
      state->ics[i].max_sfb = state->com.sfb_l_max;
      state->ics[i].max_line = state->non_zero_line[0];
      state->ics[i].minSNR = state->minSNRLong;
      state->ics[i].sfb_offset = pBlockCom->sfb_offset_long;

    } else {
      state->ics[i].max_sfb = 3 * state->com.sfb_s_max;
      state->ics[i].max_line = 384 + state->non_zero_line[1];
      state->ics[i].minSNR = state->minSNRShort;
      state->ics[i].sfb_offset = pBlockCom->sfb_offset_short_interleave;
    }

    for (sfb = 0; sfb < state->ics[i].max_sfb; sfb++) {
      state->ics[i].sfb_width[sfb] =
        state->ics[i].sfb_offset[sfb+1] - state->ics[i].sfb_offset[sfb];
    }

    ippsCopy_32f(state->ics[i].minSNR, minSNR[i], state->ics[i].max_sfb);
  }

  if (!state->PEisCounted) {
    for (i = 0; i < stereo; i++) {

      /* Energy calculation */
      p_mdct_line = state->mdct_out[gr][i];

      for (sfb = 0; sfb < state->ics[i].max_sfb; sfb++) {
        Ipp32s sfb_start = state->ics[i].sfb_offset[sfb];

        ippsDotProd_32f(p_mdct_line + sfb_start, p_mdct_line + sfb_start,
                        state->ics[i].sfb_width[sfb],
                        state->energy[4 * gr + i] + sfb);
      }

      /* Perceptual entropy calculation */
      mp3encPEcalculation(state, state->ics[i].sfb_offset,
                          state->ics[i].max_sfb, gr, i);
    }
  }

  for (i = 0; i < stereo; i++) {
    qData[i].mdct_line = state->mdct_out[gr][i];
    qData[i].noiseThr = &state->psychoacoustic_block_com.noiseThr[4 * gr + i][0];
    qData[i].minSNR = &minSNR[i][0];
    qData[i].energy = &state->energy[4 * gr + i][0];
    qData[i].logEnergy = &state->logEnergy[4 * gr + i][0];
    qData[i].sfbPE = &state->sfbPE[4 * gr + i][0];
    qData[i].isBounded = &isBounded[i][0];
  }

  bitsToPeCoeff = &state->psychoacoustic_block[0].bitsToPECoeff;

  jointStereo = 0;

  /* Mid-Side */
  if ((state->com.stereo_mode_param == MPA_JOINT_STEREO) &&
      (state->com.stereo_mode == MPA_MS_STEREO)) {
    const Ipp32f mult = 0.7071067811865475244f;

    Ipp32f *ptrL = &(state->mdct_out[gr][0][0]);
    Ipp32f *ptrR = &(state->mdct_out[gr][1][0]);
    Ipp32f li, ri;

    for (i = 0; i < LEN_MDCT_LINE; i++) {
      li = mult * (ptrL[0] + ptrR[0]);
      ri = mult * (ptrL[0] - ptrR[0]);
      ptrL[0] = li;
      ptrR[0] = ri;
      ptrL++; ptrR++;
    }

    qData[0].noiseThr = &state->psychoacoustic_block_com.noiseThr[4 * gr + 2][0];
    qData[1].noiseThr = &state->psychoacoustic_block_com.noiseThr[4 * gr + 3][0];
    qData[0].energy = &state->energy[4 * gr + 2][0];
    qData[1].energy = &state->energy[4 * gr + 3][0];
    qData[0].logEnergy = &state->logEnergy[4 * gr + 2][0];
    qData[1].logEnergy = &state->logEnergy[4 * gr + 3][0];
    qData[0].sfbPE = &state->sfbPE[4 * gr + 2][0];
    qData[1].sfbPE = &state->sfbPE[4 * gr + 3][0];

    jointStereo = 1;
  }

  pe = 0;
  for (i = 0; i < stereo; i++) {
    chPe[i] = 0;
    for (sfb = 0; sfb < state->ics[i].max_sfb; sfb++) {
      if (qData[i].sfbPE[sfb] > 0) {
        chPe[i] += qData[i].sfbPE[sfb];
      }
    }
    pe += chPe[i];
  }

  /* bits_per_frame counting */
  bits_in_buf = state->bits_in_buf;
  max_bits_in_buf = state->max_bits_in_buf;

  maxBitsPerFrame = (Ipp32s)(0.7 * state->mean_bits + bits_in_buf);

  if (state->com.br_mode == MPAENC_CBR) {
    minBitsPerFrame = bits_in_buf + state->mean_bits - max_bits_in_buf;
  } else {
    minBitsPerFrame = state->com.si_main_data_begin * 8;
  }

  win_seq = 0;

  for (i = 0; i < stereo; i++) {
    if (state->ics[i].windows_sequence) {
      win_seq = 1;
    }
  }

  if (win_seq != 1) {
    startMinMul  =  0.7f;
    startMaxMul =  0.9f;
    slopeMinMul  = 0.35f;
    slopeMaxMul = 0.5f;
  } else {
    startMinMul  =  0.8f;
    startMaxMul =  0.95f;
    slopeMinMul  = 0.2f;
    slopeMaxMul = 0.55f;
  }

  peMin = &state->psychoacoustic_block[0].peMin;
  peMax = &state->psychoacoustic_block[0].peMax;

  deltaPe = pe - state->psychoacoustic_block[0].peMin;
  maxDeltaPe = state->psychoacoustic_block[0].peMax -
    state->psychoacoustic_block[0].peMin;

  peCoeff = deltaPe/maxDeltaPe;

  if (peCoeff < 0)
    peCoeff = 0;

  /* peMin and peMax adjustment */

  if (!win_seq) {
    Ipp32f minDelta = pe * MIN_DELTA_PE;
    Ipp32f delta;

    if (deltaPe > maxDeltaPe) {
      peMin[0] += (deltaPe-maxDeltaPe) * MIN_PE_UP;
      peMax[0] += (deltaPe-maxDeltaPe) * MAX_PE_UP;

      delta = peMax[0] - peMin[0];
      if (delta < minDelta) peMin[0] = peMax[0] - minDelta;

    } else if (deltaPe < 0) {
      peMin[0] += deltaPe * MIN_PE_DOWN;
      peMax[0] += deltaPe * MAX_PE_DOWN;

      delta = peMax[0] - peMin[0];
      if (delta < minDelta) peMax[0] = peMin[0] + minDelta;

    } else {
      peMin[0] += deltaPe * MIN_PE_UP;
      peMax[0] += (deltaPe - maxDeltaPe) * MAX_PE_DOWN;

      delta = peMax[0] - peMin[0];

      if (delta < minDelta) {
        peMin[0] = pe - (pe - peMin[0])/delta * minDelta;
        peMax[0] = peMin[0] + minDelta;
      }
    }

    if (peMin[0] < 0) {
      peMin[0] = 0;
    }
  }

  deltaFullness = (Ipp32f)bits_in_buf / (Ipp32f)max_bits_in_buf;

  if (deltaFullness < 0) deltaFullness = 0;
  if (state->com.br_mode == MPAENC_CBR) {
    if (deltaFullness > 1) deltaFullness = 1;
  }

  minMul = startMinMul + slopeMinMul * deltaFullness;
  maxMul = startMaxMul + slopeMaxMul * deltaFullness;

  bitMul = minMul + (maxMul - minMul) * peCoeff * 0.8f;

  bits_per_frame = (Ipp32s)(state->mean_bits * bitMul);

  available_bits = state->bits_in_buf + state->mean_bits;

  if (state->com.br_mode == MPAENC_ABR) {
    if (available_bits > state->maxPossibleBits) {
      available_bits = state->maxPossibleBits;
    }
  }

  if (minBitsPerFrame > available_bits) {
    minBitsPerFrame = available_bits;
  }

  if (bits_per_frame > maxBitsPerFrame) {
    bits_per_frame = maxBitsPerFrame;
  }

  if (bits_per_frame < minBitsPerFrame) {
    bits_per_frame = minBitsPerFrame;
  }

  neededPE = bits_per_frame * bitsToPeCoeff[0];

  for (i = 0; i < stereo; i++) {
    qData[i].predAttackWindow = -1;
    if (state->ics[i].windows_sequence == 1) {
      qData[i].predAttackWindow = state->attackWindow[2*gr+i] - 1;
    }
  }

  if (!jointStereo) {
    for (i = 0; i < stereo; i++) {
      Ipp32f tmpPE;
      if (pe > 0) {
        tmpPE = neededPE * chPe[i]/pe;
      } else {
        tmpPE = neededPE / stereo;
      }
      /*tmpPE -= mpaencMaxBitsForScalef[state->com.header.id][state->ics[i].windows_sequence] *
               bitsToPeCoeff[0];*/
      mp3encUpdateThr(state->ics, qData, tmpPE, NULL, 0, i, 1);
    }
  } else {
    Ipp32f tmpPE = neededPE/* - bitsToPeCoeff[0] * (
      mpaencMaxBitsForScalef[state->com.header.id][state->ics[0].windows_sequence]+
      mpaencMaxBitsForScalef[state->com.header.id][state->ics[1].windows_sequence])*/;
    mp3encUpdateThr(state->ics, qData, tmpPE, state->msMask[gr], 1, 0, stereo);
  }

  pe = 0;
  for (i = 0; i < stereo; i++) {
    chPe[i] = 0;
    for (sfb = 0; sfb < state->ics[i].max_sfb; sfb++) {
      if (qData[i].sfbPE[sfb] > 0) {
        chPe[i] += qData[i].sfbPE[sfb];
      }
    }
    pe += chPe[i];
  }

  used_bits = 0;
  bitsForHuffman = 0;

  for (i = 0; i < stereo; i++) {
    if (pe > 0) {
      quantization_block.bits_per_frame = (Ipp32s)(bits_per_frame * chPe[i]/pe);
      quantization_block.min_bits_per_frame =
        (Ipp32s)(minBitsPerFrame * chPe[i]/pe);
      qData[i].outPe = neededPE * chPe[i]/pe;
    } else {
      quantization_block.bits_per_frame = bits_per_frame / stereo;
      quantization_block.min_bits_per_frame = minBitsPerFrame / stereo;
      qData[i].outPe = neededPE / stereo;
    }

    if (i != 0) {
      quantization_block.min_bits_per_frame = minBitsPerFrame - used_bits;
    }

    quantization_block.available_bits = available_bits;

    if (i == 0) {
      if (pe > 0) {
        quantization_block.available_bits =
          (Ipp32s)(quantization_block.available_bits * chPe[i]/pe);
      } else {
        quantization_block.available_bits /= stereo;
      }

      /* Bits should be enough to encode scafefactors */
      if (quantization_block.available_bits <
          mpaencMaxBitsForScalef[state->com.header.id][state->ics[0].windows_sequence]) {
          quantization_block.available_bits =
            mpaencMaxBitsForScalef[state->com.header.id][state->ics[0].windows_sequence];
      }

      /* The same for the next channel */
      if (stereo == 2) {
        if (available_bits - quantization_block.available_bits <
          mpaencMaxBitsForScalef[state->com.header.id][state->ics[1].windows_sequence]) {
            quantization_block.available_bits =
              available_bits -
              mpaencMaxBitsForScalef[state->com.header.id][state->ics[1].windows_sequence];
        }
      }
    }

    if (quantization_block.available_bits > 4096) {
      quantization_block.available_bits = 4096;
    }

    if (quantization_block.min_bits_per_frame > quantization_block.available_bits)
        quantization_block.min_bits_per_frame = quantization_block.available_bits;

    state->com.max_bits[gr][i] = quantization_block.available_bits;

    quantization_block.common_scalefactor_update =
      &(state->common_scalefactor_update[i]);
    quantization_block.last_frame_common_scalefactor =
      &(state->last_frame_common_scalefactor[i]);

    mp3encQuantization(state, &quantization_block,
                       state->ics + i, qData + i, gr, i);
    available_bits -= state->com.si_part23Len[gr][i];
    used_bits += state->com.si_part23Len[gr][i];
    bitsForHuffman += (state->com.si_part23Len[gr][i] - state->com.si_part2Len[gr][i]);
  }

  if (quantization_block.ns_mode) {
//    Ipp32s bitForScalef = used_bits - bitsForHuffman;
    outPe = 0;

    for (i = 0; i < stereo; i++) {
      if (qData[i].outPe > 0) {
        outPe += qData[i].outPe;
      }
    }

    if (bitsForHuffman > 0) {
      bitsToPeCoeff[0] =
        bitsToPeCoeff[0] * 0.9f + (outPe/bitsForHuffman) * 0.1f;
    }
  }

  state->bits_in_buf = state->bits_in_buf + state->mean_bits - used_bits;
}

/****************************************************************************/

MP3Status mp3encGetFrame(Ipp16s *inPointer, Ipp32s *encodedBytes,
                         Ipp8u *outPointer, MP3Enc *state)
{
    IPP_ALIGNED_ARRAY(32, Ipp16s, buffer, (NUM_CHANNELS+LFE_CHANNEL)*1152);
    IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf, LEN_MDCT_LINE);//for UpSample and Psychoacoustic
    Ipp16s  *buff[NUM_CHANNELS+LFE_CHANNEL];

    Ipp32s  ch, gr, i;
    Ipp32s  stereo;

    Ipp32f  *input_data[NUM_CHANNELS];
    Ipp32s  frame_size, upsample, used_bits;
    Ipp32s  upsample_len;

    if (!state || !encodedBytes || !outPointer)
      return MP3_NULL_PTR;

    if (inPointer == NULL) {
      if (state->com.header.layer != 3) {
        return MP3_NULL_PTR;
      }
    }

    for (i = 0; i < NUM_CHANNELS+LFE_CHANNEL; i++)
      buff[i] = &buffer[i*1152];

    for (i = 0; i < NUM_CHANNELS; i++)
      input_data[i] = state->input_buffer[i];

    stereo = state->com.stereo;// + state->com.mc_channel + state->com.mc_header.lfe;

    upsample_len = 0;
    if (state->com.upsample)
        upsample_len = 32 >> state->com.upsample;

    if (inPointer != NULL) {
      ippsDeinterleave_16s(inPointer, stereo + state->com.mc_channel +
        state->com.mc_header.lfe, state->com.frameSize, buff);
    }

    switch (state->com.header.layer) {
      case 1:
        for (ch = 0; ch < stereo; ch++) {
          Ipp32s  upsample = state->com.upsample;
          ippsMove_32f(&(state->input_buffer[ch][480]), &(state->input_buffer[ch][0]), LEN_MDCT_LINE);
          ippsCopy_32f(&(state->input_buffer[ch][960]), &(state->input_buffer[ch][LEN_MDCT_LINE]), 64);
          if (upsample) {
            Ipp32s len = 384, phase = 0;
            ippsConvert_16s32f(buff[ch], tbuf, 384 >> upsample);
            ippsSampleUp_32f(tbuf, 384 >> upsample, &state->input_buffer[ch][LEN_MDCT_LINE+64], &len, 1 << upsample, &phase);
          } else {
            ippsConvert_16s32f(buff[ch], &state->input_buffer[ch][LEN_MDCT_LINE+64], 384);
          }
        }
        mp3enc_psychoacoustic_l1(state, input_data);

        for (ch = 0; ch < stereo; ch++) {
          for (i = 0; i < 12; i++) {
            Ipp32f *out_ptr = &(state->fbout_data[0][ch][0][0]) + i * 32;
            ippsAnalysisFilter_PQMF_MP3_32f(&state->input_buffer[ch][LEN_MDCT_LINE] + i * 32,
              out_ptr, state->pPQMFSpec[ch], 1);
            if (upsample_len)
                ippsSet_32f (0, out_ptr + upsample_len, 32 - upsample_len);
          }
        }

        mp3enc_quantization_l1(state);
        *encodedBytes = mp3enc_formatBitstream_l12(&state->com, outPointer);
        break;
      case 2:
        state->com.jsbound = state->com.sblimit;
        upsample = state->com.upsample;

        for (ch = 0; ch < stereo + state->com.mc_channel + state->com.mc_header.lfe; ch++) {
          ippsCopy_32f(&(state->input_buffer[ch][1152]), &(state->input_buffer[ch][0]), 480);
          if (upsample) {
            Ipp32s len = 1152, phase = 0;
            ippsConvert_16s32f(buff[ch], tbuf, 1152 >> upsample);
            ippsSampleUp_32f(tbuf, 1152 >> upsample, &state->input_buffer[ch][480], &len, 1 << upsample, &phase);
          } else {
            ippsConvert_16s32f(buff[ch], &state->input_buffer[ch][480], 1152);
          }
        }
        if (state->com.mc_header.lfe) {
          if (state->com.mc_lfe_filter_off) {
            Ipp32f *ptr = &state->input_buffer[stereo + state->com.mc_channel][480];
            for (i = 0; i < 12; i++) {
              state->mc_lfe_buf[i] = *ptr * (1.0f / 32768.0f);
              ptr += 96;
            }
          } else {
            Ipp32f lfe_val;
            Ipp32f *ptr = &state->input_buffer[stereo + state->com.mc_channel][0];
            for (i = 0; i < 12; i++) {
              ippsDotProd_32f(ptr, mp3_lfe_filter, 480, &lfe_val);
              state->mc_lfe_buf[i] = lfe_val * (1.0f / 32768.0f);
              ptr += 96;
            }
          }
        }

        for (ch = 0; ch < stereo + state->com.mc_channel; ch++) {
          for (i = 0; i < 36; i++) {
            Ipp32f *out_ptr = &(state->fbout_data[ch][0][0][0]) + i * 32;
            ippsAnalysisFilter_PQMF_MP3_32f(&state->input_buffer[ch][480] + i * 32,
              out_ptr, state->pPQMFSpec[ch], 1);
            if (upsample_len)
                ippsSet_32f (0, out_ptr + upsample_len, 32 - upsample_len);
          }
        }

        if (state->com.mc_channel) {
          mp3enc_mc_normalizing(state);
          mp3enc_mc_matricing(state);
        }

        mp3enc_psychoacoustic_l2(state, input_data);

        mp3enc_quantization_l2(state);
        *encodedBytes = mp3enc_formatBitstream_l12(&state->com, outPointer);
        break;
      case 3:
        if (inPointer == NULL) {
          *encodedBytes = mp3enc_writeLastFrames(&state->com, outPointer);
          if (*encodedBytes < 0) {
            *encodedBytes = 0;
            return MP3_END_OF_STREAM;
          } else {
            return MP3_OK;
          }
        }
        frame_size = state->com.frameSize;
        state->com.stereo_mode = state->com.stereo_mode_param;
        for (ch = 0; ch < stereo; ch++) {
            Ipp32s  upsample = state->com.upsample;
            if (upsample) {
              Ipp32s len = frame_size << upsample, phase = 0;
              ippsConvert_16s32f(buff[ch], tbuf, frame_size);
              ippsSampleUp_32f(tbuf, frame_size, &state->input_buffer[ch][0],
                               &len, 1 << upsample, &phase);
            } else {
              ippsConvert_16s32f(buff[ch], &state->input_buffer[ch][0], frame_size);
            }
        }

        mp3enc_quantCalcAvailableBitsFrame(state);

        // filterbank
        for (gr = 0; gr < state->com.grnum; gr++) {
          for (ch = 0; ch < stereo; ch++) {
            for (i = 0; i < 18; i++) {
                Ipp32f *out_ptr = (Ipp32f *)&((*(state->fbout[gr+2]))[ch][0][0]) + i * 32;
                ippsAnalysisFilter_PQMF_MP3_32f(&state->input_buffer[ch][0 + gr * LEN_MDCT_LINE] + i * 32,
                    out_ptr, state->pPQMFSpec[ch], 1);
                if (upsample_len)
                    ippsSet_32f (0, out_ptr + upsample_len, 32 - upsample_len);
            }
          }

          mp3enc_psychoacoustic_l3(state, gr, tbuf);
        }

        state->PEisCounted = 0;

        /* Mid-Side decision */
        if ((stereo == 2) && (state->com.stereo_mode == MPA_JOINT_STEREO)) {
          Ipp32s test = 1;
          for (gr = 0; gr < state->com.grnum; gr++) {
            if ((state->com.si_blockType[gr][0] != state->com.si_blockType[gr][1])) {
              test = 0;
            }
          }

          if (test) {
            state->PEisCounted = 1;
            mp3encMSdecision(state);
          }
        }

        used_bits = 0;

        for (gr = 0; gr < state->com.grnum; gr++) {
          Ipp32s bits_in_buf = state->bits_in_buf;
          layer3_enc(state, gr);
          used_bits += (bits_in_buf + state->mean_bits - state->bits_in_buf);
        }

        if (state->com.br_mode == MPAENC_ABR) {
          state->com.header.bitRate =
            mp3encGEBitrate(&state->com, ((state->com.frameBits + 7) >> 3) - state->com.si_main_data_begin);
        }

        *encodedBytes = mp3enc_formatBitstream_l3(&state->com, (Ipp32s (*)[2][LEN_MDCT_LINE])state->mdct_out, outPointer);
        break;
    }

    return MP3_OK;
}

/****************************************************************************/

MP3Status mp3encGetMpegId(MP3Enc *state, Ipp32s *id)
{
  *id = state->com.header.id;
  return MP3_OK;
}

/****************************************************************************/

MP3Status mp3encGetUpsample(MP3Enc *state, Ipp32s *upsample)
{
  *upsample = state->com.upsample;
  return MP3_OK;
}

/****************************************************************************/

MP3Status mp3encGetSlotSize(MP3Enc *state, Ipp32s *slot_size)
{
  *slot_size = state->com.slot_size;
  return MP3_OK;
}

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
