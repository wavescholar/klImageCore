/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_DECODER)

#include "align.h"
#include "mp3dec_own_fp.h"

#include "ippcore.h"
#include "ipps.h"

MP3Status mp3decUpdateMemMap(MP3Dec *state, Ipp32s shift)
{
  Ipp32s i;
  mp3decUpdateMemMap_com(&state->com, shift);

  for (i = 0; i < 34; i++) {
    MP3_UPDATE_PTR(void, state->com.huff_table[i].phuftable, shift)
  }
  MP3_UPDATE_PTR(IppsMDCTInvSpec_32f, state->pMDCTSpecLong, shift)
  MP3_UPDATE_PTR(IppsMDCTInvSpec_32f, state->pMDCTSpecShort, shift)
  MP3_UPDATE_PTR(Ipp8u, state->mdct_buffer, shift)
  MP3_UPDATE_PTR(Ipp8u, state->pqmf_mem, shift)
  for (i = 0; i < NUM_CHANNELS; i++) {
    MP3_UPDATE_PTR(IppsFilterSpec_PQMF_MP3, state->pPQMFSpec[i], shift)
  }

  MP3_UPDATE_PTR(samplefloat, state->smpl_xr, shift)
  MP3_UPDATE_PTR(samplefloat, state->smpl_ro, shift)
  MP3_UPDATE_PTR(samplefloat, state->smpl_re, shift)
  MP3_UPDATE_PTR(samplefloatrw, state->smpl_rw, shift)
  MP3_UPDATE_PTR(samplefloat, state->smpl_sb, shift)
  MP3_UPDATE_PTR(sampleshort, state->com.smpl_xs, shift)

  return MP3_OK;
}

MP3Status mp3decInit(MP3Dec *state, Ipp32s mc_lfe_filter_off, Ipp32s synchro_mode, Ipp32s *size_all)
{
    Ipp32s ch, size, ts, size_init, size_buf;
    Ipp32s ts1, size_init1, size_buf1;
    Ipp8u *mem, *mem_init;

    if (mc_lfe_filter_off < 0 || mc_lfe_filter_off > 1) {
      return MP3_FAILED_TO_INITIALIZE;
    }

    if (synchro_mode < 0 || synchro_mode > 1) {
      return MP3_FAILED_TO_INITIALIZE;
    }

    size = 0;
    mem = 0;

    ts = sizeof(MP3Dec);
    size += ts;
    if (state) {
      ippsZero_8u((Ipp8u *)state, sizeof(MP3Dec));
      mem = (Ipp8u *)state + ts;
    }

    mp3decGetSize_com(&ts);
    size += ts;
    if (mem) {
      mp3decInit_com(&state->com, mem);
      mem += ts;
    }

    mp3dec_initialize_huffman(mem ? state->com.huff_table : 0, mem, &ts);
    size += ts;
    if (mem) mem += ts;

    ippsMDCTInvGetSize_32f(36, &ts, &size_init, &size_buf);
    ippsMDCTInvGetSize_32f(12, &ts1, &size_init1, &size_buf1);
    if (size_buf < size_buf1) size_buf = size_buf1;
    if (size_init < size_init1) size_init = size_init1;

    size += ts + ts1 + size_buf;

    if (mem) {
      mem_init = 0;
      if (size_init) {
        mem_init = (Ipp8u*)ippMalloc(size_init);
        if (!mem_init)
          return MP3_ALLOC;
      }
      state->pMDCTSpecLong = (IppsMDCTInvSpec_32f*)mem;
      ippsMDCTInvInit_32f(&(state->pMDCTSpecLong), 36, mem, mem_init);
      mem += ts;
      state->pMDCTSpecShort = (IppsMDCTInvSpec_32f*)mem;
      ippsMDCTInvInit_32f(&(state->pMDCTSpecShort), 12, mem, mem_init);
      mem += ts1;
      state->mdct_buffer = mem;
      mem += size_buf;
      if(mem_init)
        ippFree(mem_init);
    }

    ippsSynthesisFilterGetSize_PQMF_MP3_32f(&ts);
    size += ts * NUM_CHANNELS;

    if (mem) {
      state->pqmf_size = ts;
      state->pqmf_mem = mem;
      for (ch = 0; ch < NUM_CHANNELS; ch++) {
        state->pPQMFSpec[ch] = (IppsFilterSpec_PQMF_MP3*)(state->pqmf_mem + state->pqmf_size * ch);
        ippsSynthesisFilterInit_PQMF_MP3_32f(&state->pPQMFSpec[ch],
          state->pqmf_mem + state->pqmf_size * ch);
      }
      mem += ts * NUM_CHANNELS;
    }

    if (state) {
      /* out of dequantizer */
      state->smpl_xr =
      /* out of reordering */
      state->smpl_ro =
      /* out of antialiasing */
      state->smpl_re =
      /* out of subband synth */
      state->smpl_sb = (samplefloat *) state->global;
      /* out of imdct */
      state->smpl_rw = (samplefloatrw *) ((Ipp8s *)(state->global) + sizeof(samplefloat));
      /* out of huffman */
      state->com.smpl_xs = (sampleshort *) ((Ipp8s *)(state->global) +
          sizeof(samplefloat) * 2);

      state->com.mc_lfe_filter_off = mc_lfe_filter_off;
      state->com.synchro_mode = synchro_mode;
    }

    if(size_all)
      *size_all = size;

    return MP3_OK;
}

MP3Status mp3decReset(MP3Dec *state)
{
  Ipp32s ch;
  mp3decReset_com(&state->com);

  state->GlobalScaleFactor[0][0] = 0;
  state->GlobalScaleFactor[0][1] = 0;
  state->GlobalScaleFactor[1][0] = 0;
  state->GlobalScaleFactor[1][1] = 0;

  ippsZero_8u((Ipp8u *)(state->global), (1152 * NUM_CHANNELS * 2 + LEN_MDCT_LINE) * sizeof(Ipp32f));
  ippsZero_8u((Ipp8u *)(state->prevblk), 2 * LEN_MDCT_LINE * sizeof(Ipp32f));
  ippsZero_8u((Ipp8u *)(state->mc_pred_buf), 2 * 8 * (36 + 9) * sizeof(Ipp32f));

  for (ch = 0; ch < NUM_CHANNELS; ch++) {
    state->m_ptr[ch][0] = state->m_ptr[ch][1] = 0;
    state->m_even[ch] = 0;
  }
  state->dctnum_prev[0] = state->dctnum_prev[1] = 0;

  for (ch = 0; ch < NUM_CHANNELS; ch++) {
    state->pPQMFSpec[ch] = (IppsFilterSpec_PQMF_MP3*)(state->pqmf_mem + state->pqmf_size * ch);
    ippsSynthesisFilterInit_PQMF_MP3_32f(&state->pPQMFSpec[ch],
      state->pqmf_mem + state->pqmf_size * ch);
  }

  return MP3_OK;
}

MP3Status mp3decClose(MP3Dec *state)
{
    if (state == NULL)
        return MP3_OK;

    mp3decClose_com(/*&state->com*/);

    return MP3_OK;
}

MP3Status mp3decGetFrame(Ipp8u *inPointer,
                         Ipp32s inDataSize,
                         Ipp32s *decodedBytes,
                         Ipp16s *outPointer,
                         Ipp32s outBufferSize,
                         MP3Dec *state)
{
    IPP_ALIGNED_ARRAY(32, Ipp32f,  buf, 1152);
    Ipp32f lfe_sample[12];
    Ipp32f *pSampl[NUM_CHANNELS+1];
    Ipp32s prev_decodedBytes = state->com.decodedBytes;
    Ipp32s i, j, ch, frameSize, channels, channels_mask, bits;
    IppMP3FrameHeader *header = &(state->com.header);
    samplefloat *smpl_sb = state->smpl_sb;       // out of subband synth
    MP3Status res;

    if (!inPointer || !outPointer)
        return MP3_NULL_PTR;

    res = mp3dec_GetID3Len(inPointer, inDataSize, &(state->com));

    if (res != MP3_OK)
      return res;

    res = mp3dec_SkipID3(inDataSize, decodedBytes, &(state->com));

    if (res != MP3_OK) {
      return res;
    } else {
      inDataSize -= *decodedBytes;
      inPointer += *decodedBytes;
    }

    if (inDataSize == 0 && (state->com.m_StreamData.nDataLen <= state->com.decodedBytes + 32))
        return MP3_NOT_ENOUGH_DATA;
    state->com.m_pOutSamples = (Ipp16s *)outPointer;

    do {
        mp3dec_ReceiveBuffer(&(state->com.m_StreamData), inPointer, inDataSize);
        res = mp3dec_GetSynch(&state->com);

        if (res == MP3_BAD_STREAM || res == MP3_UNSUPPORTED || res == MP3_NOT_FIND_SYNCWORD) {
          *decodedBytes += state->com.decodedBytes - prev_decodedBytes;
          return res;
        } else if (res == MP3_NOT_ENOUGH_DATA ) {
          *decodedBytes += state->com.decodedBytes - prev_decodedBytes;
          return MP3_NOT_ENOUGH_DATA;
        }

        mp3decGetFrameSize(&frameSize, state);
        mp3decGetChannels(&channels, &channels_mask, state);

        if (outBufferSize < frameSize * channels) {
            return MP3_NOT_ENOUGH_BUFFER;
        }

        //CMC END
        state->com.m_frame_num++;

        res = MP3_OK;

        switch (header->layer) {
        case 3:
            if (header->id)
                mp3dec_audio_data_LayerIII(&state->com);
            else
                mp3dec_audio_data_LSF_LayerIII(&state->com);
            res = mp3dec_decode_data_LayerIII(state);
            break;
        case 2:
            if (state->com.nbal_alloc_table) {
              if (mp3dec_audio_data_LayerII(&state->com)) {
                res = MP3_NOT_FIND_SYNCWORD;
                state->com.decodedBytes -= state->com.MP3nSlots + 3;
                break;
              }

              mp3dec_decode_data_LayerII(state);

              bits = (Ipp32s)(((state->com.m_StreamData.pCurrent_dword -
                state->com.start_ptr) << 5) +
                state->com.start_offset -
                state->com.m_StreamData.nBit_offset);
              bits = (state->com.MP3nSlots << 3) - bits;

              state->com.mc_channel = 0;
              state->com.mc_header.lfe = 0;

              if (bits >= 35) {
                bs_CRC_reset(&state->com.crc);
                mp3dec_CRC_start(&state->com);
                mp3dec_mc_header(&state->com);
                mp3dec_CRC_update(&state->com, &state->com.crc);

                mp3dec_CRC_start(&state->com);
                mp3dec_mc_params(&state->com);
                mp3dec_mc_composite_status_info(&state->com);
                mp3dec_mc_audio_data_l2(&state->com);
                mp3dec_CRC_update(&state->com, &state->com.crc);

                if (state->com.crc == state->com.crc_check) {
                  mp3dec_mc_decode_scale_l2(&state->com);

                  mp3dec_mc_decode_sample_l2(&state->com);

                  mp3dec_dequantize_sample_mc_l2(state);

                  mp3dec_mc_denormalize_sample_l2(state);

                  mp3dec_mc_dematricing   (state);
                  mp3dec_mc_denormalizing (state);

                  if(state->com.mc_header.lfe) {
                    mp3dec_mc_lfe_l2(state, lfe_sample);
                  }
                } else {
                  state->com.mc_channel = 0;
                  state->com.mc_header.lfe = 0;
                }
              }

              mp3decGetChannels(&channels, &channels_mask, state);
              if (outBufferSize < frameSize * channels) {
                return MP3_NOT_ENOUGH_BUFFER;
              }

              for (ch = 0; ch < state->com.stereo + state->com.mc_channel; ch++) {
                pSampl[ch] = (*smpl_sb)[ch];
                for (i = 0; i < 36; i++) {
                  ippsSynthesisFilter_PQMF_MP3_32f((*(state->smpl_rw))[ch][i],
                    (*smpl_sb)[ch] + i * 32,
                    state->pPQMFSpec[ch], 1);
                }
              }
              if (state->com.mc_header.lfe) {
                Ipp32s len = 1152, phase = 0;
                if (state->com.mc_lfe_filter_off) {
                  ippsSampleUp_32f(lfe_sample, 12, buf, &len, 96, &phase);
                } else {
                  Ipp32f tval;
                  Ipp32s div96 = 0, lfe_offset = 0;
                  ippsCopy_32f(&state->mc_lfe_buf[12], &state->mc_lfe_buf[0], 5);
                  ippsCopy_32f(lfe_sample, &state->mc_lfe_buf[5], 12);
                  for (i = 0; i < 1152; i++) {
                    Ipp32f *ptr = (Ipp32f *)mp3_lfe_filter + div96;
                    tval = 0.0f;
                    for (j = 0; j < 5; j++) {
                      tval += state->mc_lfe_buf[lfe_offset + j] * *ptr;
                      ptr += 96;
                    }
                    buf[i] = tval;

                    if (div96 == 0) {
                       div96 = 96;
                       lfe_offset++;
                    }
                    div96--;
                  }
                }
                pSampl[state->com.stereo + state->com.mc_channel] = buf;
              }
              ippsJoin_32f16s_D2L((const Ipp32f **)pSampl,
                state->com.stereo + state->com.mc_channel + state->com.mc_header.lfe,
                1152, state->com.m_pOutSamples);
/*              if(state->com.mc_channel) {
                FILE *fp = fopen("o.pcm","wb");
                fwrite(state->com.m_pOutSamples, 2*1152 * state->com.stereo + state->com.mc_channel + state->com.mc_header.lfe, 1, fp);
                fclose(fp);
              }*/
            }
            break;
        case 1:
            mp3dec_audio_data_LayerI(&state->com);
            mp3dec_decode_data_LayerI(state);
            break;
        default:
            res = MP3_UNSUPPORTED;  // unsupported layer
        }
        if (res == MP3_NOT_FIND_SYNCWORD) {
          Ipp32s len = state->com.decodedBytes - prev_decodedBytes;
          *decodedBytes += len;
          inPointer += len;
          inDataSize -= len;
          prev_decodedBytes = state->com.decodedBytes;
        }
    } while (res == MP3_NOT_FIND_SYNCWORD);

    *decodedBytes += (state->com.decodedBytes - prev_decodedBytes);

    ippsCopy_8u((Ipp8u *)&state->com.header, (Ipp8u *)&state->com.header_good,
        sizeof(IppMP3FrameHeader));
    state->com.mpg25_good = state->com.mpg25;
    state->com.m_bInit = 1;

    return res;
}

MP3Status mp3decGetInfo(cAudioCodecParams *a_info, MP3Dec *state)
{
    IppMP3FrameHeader *header;
//    Ipp32s  ch[] = { 2, 2, 2, 1 };
    cAudioStreamType mpeg_type[] = { MPEG2_AUD, MPEG1_AUD };
    cAudioStreamType layer_type[] =
    { MPEG_AUD_LAYER1, MPEG_AUD_LAYER2, MPEG_AUD_LAYER3 };
    Ipp32s ch, ch_mask;

    if (!a_info)
        return MP3_NULL_PTR;

    a_info->m_SuggestedInputSize = 4096;
    a_info->m_SuggestedOutputSize = 1152 * (NUM_CHANNELS + 1) * sizeof(Ipp16s);
    a_info->is_valid = 0;

    if (!state)
      return MP3_OK;

    header = &(state->com.header_good);

    if (state->com.m_bInit) {
        a_info->m_info_in.bitPerSample = 0;
        a_info->m_info_out.bitPerSample = 16;

        a_info->m_info_in.bitrate =
            mp3_bitrate[header->id][3 - header->layer][header->bitRate] * 1000;
        a_info->m_info_out.bitrate = 0;

        mp3decGetChannels(&ch, &ch_mask, state);
        a_info->m_info_in.channels = ch;
        a_info->m_info_out.channels = ch;
        a_info->m_info_in.channel_mask = ch_mask;
        a_info->m_info_out.channel_mask = ch_mask;
        a_info->m_info_in.sample_frequency =
            mp3_frequency[header->id + state->com.mpg25_good][header->samplingFreq];
        a_info->m_info_out.sample_frequency =
            mp3_frequency[header->id + state->com.mpg25_good][header->samplingFreq];

        a_info->m_info_in.stream_type = (cAudioStreamType)
            (mpeg_type[header->id] | layer_type[header->layer - 1]);
        a_info->m_info_out.stream_type = PCM_AUD;

        a_info->m_frame_num = state->com.m_frame_num;

        a_info->is_valid = 1;

        return MP3_OK;
    }

    return MP3_OK;
}

MP3Status mp3decGetDuration(Ipp32f *p_duration, MP3Dec *state)
{
    Ipp32f duration;
    Ipp32s frameSize;
    IppMP3FrameHeader *header;

    if (!state)
        return MP3_NULL_PTR;

    if (state->com.m_bInit)
      header = &(state->com.header_good);
    else
      header = &(state->com.header);

    mp3decGetFrameSize(&frameSize, state);
    duration = (Ipp32f)state->com.m_frame_num * frameSize;

    duration /= (Ipp32f)mp3_frequency[header->id + state->com.mpg25_good][header->samplingFreq];

    *p_duration = duration;

    return MP3_OK;
}

MP3Status mp3decGetChannels(Ipp32s *ch, Ipp32s *ch_mask, MP3Dec *state)
{
/*    IppMP3FrameHeader *header;
    Ipp32s  ch_table[] = { 2, 2, 2, 1 };*/
    Ipp32s nch, mask;

    if (!state)
        return MP3_NULL_PTR;

//    header = &(state->com.header_good);

    nch = state->com.stereo + state->com.mc_channel + state->com.mc_header.lfe;

    if (state->com.stereo == 1) {
      mask = MP3_CHANNEL_CENTER;
    } else {
      mask = MP3_CHANNEL_STEREO;
    }

    if (nch > state->com.stereo) {
      if (state->com.mc_header.center & 1)
        mask |= MP3_CHANNEL_CENTER;
      if (state->com.mc_header.lfe)
        mask |= MP3_CHANNEL_LOW_FREQUENCY;
      switch (state->com.mc_header.surround) {
        case 1:
          mask |= MP3_CHANNEL_SURROUND_MONO; break;
        case 2:
          mask |= MP3_CHANNEL_SURROUND_STEREO; break;
        case 3:
          mask |= MP3_CHANNEL_SURROUND_STEREO_P2; break;
        default: break;
      }
    }

    *ch = nch;
    *ch_mask = mask;

    return MP3_OK;
}

MP3Status mp3decGetFrameSize(Ipp32s *frameSize, MP3Dec *state)
{
    IppMP3FrameHeader *header;
    Ipp32s fs[2][4] = {
        { 0, 384, 1152,  576 },
        { 0, 384, 1152, 1152 }
    };

    if (!state)
        return MP3_NULL_PTR;

    if (state->com.m_bInit)
      header = &(state->com.header_good);
    else
      header = &(state->com.header);

    *frameSize = fs[header->id][header->layer];

    return MP3_OK;
}

MP3Status mp3decGetSampleFrequency(Ipp32s *freq, MP3Dec *state)
{
    IppMP3FrameHeader *header;

    if (!state)
        return MP3_NULL_PTR;

    if (state->com.m_bInit)
      header = &(state->com.header_good);
    else
      header = &(state->com.header);

    *freq = mp3_frequency[header->id + state->com.mpg25_good][header->samplingFreq];

    return MP3_OK;
}

#endif //UMC_ENABLE_MP3_AUDIO_DECODER
