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

#include "mp3dec_own.h"
#include "ipps.h"

/*
//  Functions in this file.
//
//  Ipp32s mp3dec_SetAllocTable(MP3Dec *state)
//  Ipp32s mp3dec_GetSynch(sDecoderContext* DC)
//  Ipp32s main_data_slots(sDecoderContext* DC)
//  Ipp32s mp3dec_ReadMainData(sDecoderContext* DC)
*/

#define HDR_VERSION(x)       ((x & 0x80000) >> 19)
#define HDR_LAYER(x)         (4 - ((x & 0x60000) >> 17))
#define HDR_ERRPROTECTION(x) (1 - ((x & 0x10000) >> 16))
#define HDR_BITRADEINDEX(x)  ((x & 0x0f000) >> 12)
#define HDR_SAMPLINGFREQ(x)  ((x & 0x00c00) >> 10)
#define HDR_PADDING(x)       ((x & 0x00200) >> 9)
#define HDR_EXTENSION(x)     ((x & 0x00100) >> 8)
#define HDR_MODE(x)          ((x & 0x000c0) >> 6)
#define HDR_MODEEXT(x)       ((x & 0x00030) >> 4)
#define HDR_COPYRIGHT(x)     ((x & 0x00008) >> 3)
#define HDR_ORIGINAL(x)      ((x & 0x00004) >> 2)
#define HDR_EMPH(x)          ((x & 0x00003))

/* MPEG-1 12-bit code embedded in the audio bitstream that identifies
the start of a frame (p.20 ISO/IEC 11172-3) */
static Ipp32u SYNC_WORD = 0xffe;

#define MP3DEC_END_OF_BUFFER(BS)                            \
  ((((((BS)->pCurrent_dword) - ((BS)->pBuffer)) * 32 +      \
  (BS)->init_nBit_offset - ((BS)->nBit_offset)) <=          \
        (BS)->nDataLen * 8 - 8) ? 0 : 1)

#define MAX_FRAME_LEN 1800

/******************************************************************************
//  Name:
//    mp3dec_main_data_slots
//
//  Description:
//    calculate number of bytes beetween two successive frames minus length of
//    (header & side info)
//  Input Arguments:
//    DC - point to Decoder context
//
//  Returns:
//    number of bytes beetween two successive frames minus length of
//    (header & side info)
//
******************************************************************************/

static Ipp32s mp3dec_main_data_slots(Ipp32s stereo, Ipp32s header_id, Ipp32s header_protectionBit)
{
    Ipp32s  nSlots = 0;

    if (header_id == 1) {
        if (stereo == 1)
            nSlots -= 17;
        else
            nSlots -= 32;
    }

    if (header_id == 0) {
        if (stereo == 1)
            nSlots -= 9;
        else
            nSlots -= 17;
    }

    if (header_protectionBit)
        nSlots -= 2;

    return nSlots;
}

static Ipp32s mp3dec_SynchCheckCRC(MP3Dec_com *state, Ipp32s MP3Header)
{
  IppMP3FrameHeader *header = &(state->header);
  sBitsreamBuffer *BS = &state->m_StreamData;
  Ipp32s crc_nbits;
  Ipp32u crc, crc_check;

  if (header->layer == 3) {
    if (header->id == 1) {
      if (header->mode == 0x3)
        crc_nbits = 136;
      else
        crc_nbits = 256;
    } else {
      if (header->mode == 0x3)
        crc_nbits = 72;
      else
        crc_nbits = 136;
    }
  } else {
    Ipp32s jsbound, stereo;
    jsbound = 32;
    stereo = (header->mode == 0x3) ? 1 : 2;

    if (header->mode  == 0x01) {
      jsbound = (header->modeExt + 1) * 4;
    }

    crc_nbits = (jsbound << (stereo + 1)) + ((32 - jsbound) << 2);
  }

  bs_save(BS);

  GET_BITS(BS, crc_check, 16, Ipp32s);

  bs_CRC_reset(&crc);
  bs_CRC_update_imm(MP3Header, 16, &crc);
  bs_CRC_update_bs(BS, crc_nbits, &crc);
  bs_restore(BS);

  if (crc != crc_check)
    return 1;

  return 0;
}

/*****************************************************************************
//  Name:
//    mp3dec_GetSynch
//
//  Description:
//    check the MP3 frame header and get information if the header is valid
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//
//  Output Arguments:
//    DC - point to sDecoderContext structure
//
//  Returns:
//     1 - if the header is valid and supported
//    -1 - if synchronization is lost
//    -2 - if it is the end of stream
//    -4 - if the header is not supported
//
******************************************************************************/

MP3Status  mp3dec_GetSynch(MP3Dec_com *state)
{
    Ipp32s MP3Header;
    Ipp32u val, val_t;
    Ipp32s start_db = state->decodedBytes;
    IppMP3FrameHeader *header = &(state->header);
    IppMP3FrameHeader *header_good = &(state->header_good);
    sBitsreamBuffer *BS = &state->m_StreamData;
    Ipp8u *ptrStart = (Ipp8u *)BS->pCurrent_dword + ((32 - BS->nBit_offset) >> 3);
    Ipp32s buflen = BS->nDataLen;
    Ipp32s cont_flag;

    if (buflen <= (BS->pCurrent_dword - BS->pBuffer) * 4 || buflen < 4)
        return MP3_NOT_FIND_SYNCWORD;     // end of file

    GET_BITS(BS, val, 24, Ipp32s);
    state->decodedBytes += 3;

    do {
      cont_flag = 0;
      for(;;) {
        val <<= 8;
        GET_BITS(BS, val_t, 8, Ipp32s);
        val |= val_t;
        state->decodedBytes++;
        if (((val >> 20) & SYNC_WORD) == SYNC_WORD) {
          MP3Header = val;
          header->layer = HDR_LAYER(MP3Header);
          header->samplingFreq = HDR_SAMPLINGFREQ(MP3Header);
          header->bitRate = HDR_BITRADEINDEX(MP3Header);
          header->emphasis = HDR_EMPH(MP3Header);
          header->id = HDR_VERSION(MP3Header);
          state->mpg25 = ((val >> 20) & 1) ? 0 : 2;

          if (state->m_bInit) {
            if ((header->samplingFreq == header_good->samplingFreq) &&
              (header->samplingFreq != 3) &&
              (header->layer == header_good->layer) &&
              (header->id == header_good->id) &&
              (state->mpg25 == state->mpg25_good) &&
              (header->bitRate != 15) &&
              (header->emphasis != 2)) {
                break;
              }
          } else {
            if ((header->samplingFreq != 3) &&
              (header->layer != 4) &&
              (header->bitRate != 15) &&
              (header->emphasis != 2) &&
              (((val >> 19) & 3) != 1)) {
                break;
              }
          }
        }
        if (MP3DEC_END_OF_BUFFER(BS)) {
          state->decodedBytes -= 3;
          return MP3_NOT_FIND_SYNCWORD;
        }
      }

      header->mode = HDR_MODE(MP3Header);
      header->modeExt = HDR_MODEEXT(MP3Header);
      header->copyright = HDR_COPYRIGHT(MP3Header);
      header->originalCopy = HDR_ORIGINAL(MP3Header);
      header->paddingBit = HDR_PADDING(MP3Header);
      header->privateBit = HDR_EXTENSION(MP3Header);
      header->protectionBit = HDR_ERRPROTECTION(MP3Header);

      if (header->bitRate != 0) {
        Ipp32s size = 0;

        if (header->layer == 3) {
          size = 72000 * (header->id + 1);
        } else if (header->layer == 2) {
          size = 72000 * 2;
        } else if (header->layer == 1) {
          size = 12000;
        }

        state->MP3nSlots =
          size * mp3_bitrate[header->id][header->layer - 1][header->bitRate] /
          mp3_frequency[header->id + state->mpg25][header->samplingFreq] + header->paddingBit;

        if (header->layer == 1)
          state->MP3nSlots *= 4;

        state->MP3nSlots -= 4; /* header */
        state->decodedBytes += state->MP3nSlots;
#if defined(ANDROID)
        // Fixes decoder fail with MP3_NOT_ENOUGH_DATA error when processing single frame of the stream:
        state->m_bInit = 1;
#endif

        if (BS->nDataLen < state->decodedBytes - start_db +
          ((!state->m_bInit || state->synchro_mode) ? 3 : 0)) {
          state->decodedBytes -= state->MP3nSlots + 4;
          return MP3_NOT_ENOUGH_DATA;
        }
        if (!state->m_bInit || state->synchro_mode) {
            Ipp32s next_header =
              (ptrStart[state->decodedBytes - start_db + 0] << 16) |
              (ptrStart[state->decodedBytes - start_db + 1] << 8) |
              (ptrStart[state->decodedBytes - start_db + 2]);
           if ((next_header ^ (MP3Header >> 8)) & 0xfffe0c) {
             state->decodedBytes -= state->MP3nSlots;

            if (MP3DEC_END_OF_BUFFER(BS)) {
              state->decodedBytes -= 3;
              return MP3_NOT_FIND_SYNCWORD;
            }
            cont_flag = 1;
            continue;
          }
        }
        if (header->protectionBit && header->layer != 2) {
          if (mp3dec_SynchCheckCRC(state, MP3Header)) {
            state->decodedBytes -= state->MP3nSlots;
            cont_flag = 1;
            continue;
          }
        }
      } else {
        Ipp32u ubuf, usyncbuf, good;
        if (header->protectionBit && header->layer != 2) {
          if (mp3dec_SynchCheckCRC(state, MP3Header)) {
            cont_flag = 1;
            continue;
          }
        }

        bs_save(BS);

        if (state->m_nBitrate != header->bitRate) { /* VBR is forbidden */
          state->decodedBytes -= 3;
          return MP3_BAD_STREAM;
        }

        usyncbuf = 0xffe000 | ((2 - state->mpg25) << 11) | (header->id << 11) | ((4 - header->layer) << 9) |
          (header->samplingFreq << 2);

        state->MP3nSlots = -2;
        ubuf = 0;
        good = 0;

        while (MP3DEC_END_OF_BUFFER(BS) != 1) {
          GET_BITS(BS, val, 8, Ipp32s);
          ubuf = (ubuf << 8) | val;
          if ((ubuf & 0xfffefc) == usyncbuf)
          {
            good = 1;
            break;
          }
          state->MP3nSlots++;
        }

        bs_restore(BS);

        if (good == 0) {
          if (BS->nDataLen > (state->decodedBytes - start_db + MAX_FRAME_LEN)) {
            if (MP3DEC_END_OF_BUFFER(BS)) {
              state->decodedBytes -= 3;
              return MP3_NOT_FIND_SYNCWORD;
            }
            cont_flag = 1;
            continue;
          } else {
            state->decodedBytes -= 4;
            return MP3_NOT_ENOUGH_DATA;
          }
        }

        state->decodedBytes += (state->MP3nSlots);
      }
    } while (cont_flag);

    state->m_nBitrate = header->bitRate;

    state->stereo = (header->mode == 0x3) ? 1 : 2;

    if (header->mode == 0x01) {
        state->intensity = 0;
        state->ms_stereo = 0;

        if (header->modeExt & 0x1)
            state->intensity = 1;

        if (header->modeExt & 0x2)
            state->ms_stereo = 1;
    }

    state->start_ptr = BS->pCurrent_dword;
    state->start_offset = BS->nBit_offset;

    if (header->protectionBit) {
      bs_CRC_reset(&state->crc);
      bs_CRC_update_imm(MP3Header, 16, &state->crc);
      GET_BITS(BS, state->crc_check, 16, Ipp32s);
    }

    if (header->layer == 2) {
        if ((state->header.id + state->mpg25 ) < 3) {
                mp3_SetAllocTable(state->header.id,
                state->mpg25,
                state->header.layer,
                state->header.bitRate,
                state->header.samplingFreq,
                state->stereo,
                &state->nbal_alloc_table,
                &state->alloc_table,
                &state->sblimit);
        }
        else {
            return MP3_NOT_ENOUGH_BUFFER;
        }

        state->jsbound = 32;

        if (header->mode  == 0x01) {
            state->jsbound = (header->modeExt + 1) * 4;
        }
        if (state->jsbound > state->sblimit) {
            state->jsbound = state->sblimit;
        }
    } else if (header->layer == 1) {
        state->jsbound = 32;

        if (header->mode  == 0x01) {
            state->jsbound = (header->modeExt + 1) * 4;
        }
    }
    return MP3_OK;
}

/******************************************************************************
//  Name:
//    ReadMainData
//
//  Description:
//    Copy data form global buffer, which contains all streams, into special buffer,
//    which contains only main data (i.e. headers of frames & side information are excluded)
//
//  Input Arguments:
//    DC - point to sDecoderContext structure
//
//  Output Arguments:
//    MainData - special buffer
//
//  Returns:
//    1 - is not ok.
//    0 - all ok
//
******************************************************************************/

Ipp32s mp3dec_ReadMainData(MP3Dec_com *state)
{
    Ipp8u *GlBS;
    Ipp8u *MDBS;
    Ipp32s nSlots;
    Ipp32s status = 0;

    sBitsreamBuffer *m_MainData = &state->m_MainData;
    sBitsreamBuffer *m_StreamData = &state->m_StreamData;

    // detect start point
    if ((Ipp32s)(m_MainData->nDataLen - state->si_main_data_begin) >= 0)
    {
        m_MainData->pCurrent_dword =
            m_MainData->pBuffer + (m_MainData->nDataLen - state->si_main_data_begin) / 4;
        m_MainData->dword = BSWAP(m_MainData->pCurrent_dword[0]);
        m_MainData->nBit_offset =
            32 - ((m_MainData->nDataLen - state->si_main_data_begin) % 4) * 8;
    } else {
        status = 1;
    }

    // GIBS points to the start point of the undecoded main data in globalBuffer
    GlBS =
        (Ipp8u *)(m_StreamData->pCurrent_dword) + (4 -
        m_StreamData->
        nBit_offset / 8);

    // nSlots = number of bytes between two successive frames (without header and
    // side info).
    nSlots = state->MP3nSlots + mp3dec_main_data_slots(state->stereo, state->header.id,
        state->header.protectionBit);

    // nSlots contains only main data cannot be bigger than the defined maximum
    // (MAINDATASIZE).
    if (nSlots > state->MAINDATASIZE) {
#if !defined(ANDROID)
        return 1;
#else
        return -1; // UMG change for Android compatibility
#endif
    }

    if (nSlots + m_MainData->nDataLen > state->MAINDATASIZE) {
        // bytes = number of bytes which have been received, but not decoded yet.
        Ipp32s bytes;
        if (status == 0) {
            bytes =
                (Ipp32s)(m_MainData->nDataLen - (m_MainData->pCurrent_dword -
                m_MainData->pBuffer) * 4);
            // copies the undecoded bytes (bytes beginning at pBuffer+iDWDoneNum)
            // to the start of the MainData buffer.
            ippsCopy_8u((Ipp8u *)(m_MainData->pCurrent_dword),
                (Ipp8u *)m_MainData->pBuffer, bytes);
        } else {
            bytes =
                m_MainData->nDataLen - (state->MAINDATASIZE - nSlots);
            ippsCopy_8u((Ipp8u *)m_MainData->pBuffer +
                m_MainData->nDataLen - bytes,
                (Ipp8u *)m_MainData->pBuffer, bytes);
        }

        // effectively shifts the undecoded bytes to the start of the MainData buffer
        m_MainData->pCurrent_dword = m_MainData->pBuffer;
        m_MainData->nDataLen = bytes;
    }
    // MDBS points to the first
    MDBS = (Ipp8u *)m_MainData->pBuffer + m_MainData->nDataLen;
    ippsCopy_8u(GlBS, MDBS, nSlots);
    m_MainData->nDataLen += nSlots;

    return status;
}

MP3Status mp3dec_ReceiveBuffer(sBitsreamBuffer *m_StreamData, void *in_GetPointer, Ipp32s in_GetDataSize)
{
    GET_INIT_BITSTREAM(m_StreamData, in_GetPointer)
    m_StreamData->nDataLen = in_GetDataSize;
    return MP3_OK;
}

MP3Status mp3dec_GetID3Len(Ipp8u *in,
                           Ipp32s inDataSize,
                           MP3Dec_com *state)
{
  if ((state->m_bInit != 0) || (state->id3_size) > 0)
    return MP3_OK;

  if (inDataSize < 10)
    return MP3_NOT_ENOUGH_DATA;

  if ((in[0] == 'I') && (in[1] == 'D') && (in[2] == '3') && /* 'ID3' */
      (in[3] < 0xFF) && (in[4] < 0xFF) &&                  /* Version or revision will never be 0xFF */
      (in[6] < 0x80) && (in[7] < 0x80) && (in[8] < 0x80) && (in[9] < 0x80)) { /* size */
    state->id3_size = (in[6] << 21) + (in[7] << 14) + (in[8] << 7) + in[9] + 10;
  } else {
    state->id3_size = 0;
  }

  return MP3_OK;
}

MP3Status mp3dec_SkipID3(Ipp32s inDataSize,
                         Ipp32s *skipped,
                         MP3Dec_com *state)
{
  *skipped = 0;
  if (state->id3_size > 0) {
    if (inDataSize < state->id3_size) {
      *skipped = inDataSize;
      state->id3_size -= inDataSize;
      return MP3_NOT_ENOUGH_DATA;
    } else {
      *skipped = state->id3_size;
      state->id3_size = 0;
      return MP3_OK;
    }
  }
  return MP3_OK;
}

void mp3dec_mc_header(MP3Dec_com *state)
{
  sBitsreamBuffer *m_data = &(state->m_StreamData);
  mp3_mc_header *mc_header = &(state->mc_header);

  GET_BITS(m_data, mc_header->ext_bit_stream_present, 1, Ipp32s);
  if (mc_header->ext_bit_stream_present)
    GET_BITS(m_data, mc_header->n_ad_bytes, 8, Ipp32s);
  GET_BITS(m_data, mc_header->center, 2, Ipp32s);
  GET_BITS(m_data, mc_header->surround, 2, Ipp32s);
  GET_BITS(m_data, mc_header->lfe, 1, Ipp32s);
  GET_BITS(m_data, mc_header->audio_mix, 1, Ipp32s);
  GET_BITS(m_data, mc_header->dematrix_procedure, 2, Ipp32s);
  GET_BITS(m_data, mc_header->no_of_multi_lingual_ch, 3, Ipp32s);
  GET_BITS(m_data, mc_header->multi_lingual_fs, 1, Ipp32s);
  GET_BITS(m_data, mc_header->multi_lingual_layer, 1, Ipp32s);
  GET_BITS(m_data, mc_header->copyright_identification_bit, 1, Ipp32s);
  GET_BITS(m_data, mc_header->copyright_identification_start, 1, Ipp32s);

  if (mc_header->ext_bit_stream_present)
    state->crc_nbits = 24;
  else
    state->crc_nbits = 16;

  GET_BITS(m_data, state->crc_check, 16, Ipp32s);
/*
  printf("\nMC Header:\n");
  printf("ext_bit_stream_present = %d\n",mc_header->ext_bit_stream_present);
  printf("n_ad_bytes = %d\n",mc_header->n_ad_bytes);
  printf("center = %d\n",mc_header->center);
  printf("surround = %d\n",mc_header->surround);
  printf("lfe = %d\n",mc_header->lfe);
  printf("audio_mix = %d\n",mc_header->audio_mix);
  printf("dematrix_procedure = %d\n",mc_header->dematrix_procedure);
  printf("no_of_multi_lingual_ch = %d\n",mc_header->no_of_multi_lingual_ch);
  printf("multi_lingual_fs = %d\n",mc_header->multi_lingual_fs);
  printf("multi_lingual_layer = %d\n",mc_header->multi_lingual_layer);
  printf("copyright_identification_bit = %d\n",mc_header->copyright_identification_bit);
  printf("copyright_identification_start = %d\n",mc_header->copyright_identification_start);
*/
}

void mp3dec_mc_params(MP3Dec_com *state)
{
  mp3_mc_header *mc_header = &(state->mc_header);

  if (mp3_frequency[state->header.id + state->mpg25][state->header.samplingFreq] ==  48000) {
    state->alloc_table = mp3_alloc_table1;
    state->nbal_alloc_table = mp3_nbal_alloc_table1;
    state->sblimit = mp3_sblimit_table[0];
  } else {
    state->alloc_table = mp3_alloc_table2;
    state->nbal_alloc_table = mp3_nbal_alloc_table2;
    state->sblimit = mp3_sblimit_table[1];
  }

  if (mc_header->surround == 3)
    if (mc_header->center == 1 || mc_header->center == 3) {
      state->mc_channel = 3;
      state->mc_alloc_bits = 2;
      state->mc_dyn_cross_bits = 1;
      state->mc_pred_mode = 2;
    }
    else {
      state->mc_channel = 2;
      state->mc_alloc_bits = 0;
      state->mc_dyn_cross_bits = 0;
      state->mc_pred_mode = 5;
    }
  else if (mc_header->surround == 2)
    if (mc_header->center == 1 || mc_header->center == 3) {
      state->mc_channel = 3;
      state->mc_alloc_bits = 3;
      state->mc_dyn_cross_bits = 4;
      state->mc_pred_mode = 0;
    }
    else {
      state->mc_channel = 2;
      state->mc_alloc_bits = 2;
      state->mc_dyn_cross_bits = 3;
      state->mc_pred_mode = 3;
    }
  else if (mc_header->surround == 1)
    if (mc_header->center == 1 || mc_header->center == 3) {
      state->mc_channel = 2;
      state->mc_alloc_bits = 3;
      state->mc_dyn_cross_bits = 3;
      state->mc_pred_mode = 1;
    }
    else {
      state->mc_channel = 1;
      state->mc_alloc_bits = 2;
      state->mc_dyn_cross_bits = 1;
      state->mc_pred_mode = 4;
    }
  else
    if (mc_header->center == 1 || mc_header->center == 3) {
      state->mc_channel = 1;
      state->mc_alloc_bits = 2;
      state->mc_dyn_cross_bits = 1;
      state->mc_pred_mode = 2;
    }
    else {
      state->mc_channel = 0;
      state->mc_alloc_bits = 0;
      state->mc_dyn_cross_bits = 0;
      state->mc_pred_mode = 5;
    }

    if ( (mc_header->no_of_multi_lingual_ch > 0) &&
      (mc_header->multi_lingual_layer > 0) ) {
        mc_header->no_of_multi_lingual_ch = 0;
      }
}

void mp3dec_mc_composite_status_info(MP3Dec_com *state)
{
  sBitsreamBuffer *m_data = &(state->m_StreamData);
  Ipp32s i, sb, nbits = 0;

  GET_BITS(m_data, state->mc_tc_sbgr_select, 1, Ipp32s);
  GET_BITS(m_data, state->mc_dyn_cross_on, 1, Ipp32s);
  GET_BITS(m_data, state->mc_prediction_on, 1, Ipp32s);

  nbits += 3;

  for(sb = 0; sb < 12; sb++) {
    state->mc_tc_alloc[sb] = 0;
    state->mc_dyn_cross_mode[sb] = 0;
  }

  if(!state->mc_alloc_bits) {
    state->mc_tc_allocation = 0;
  }
  else if(state->mc_tc_sbgr_select == 1) {
    GET_BITS(m_data, state->mc_tc_allocation, state->mc_alloc_bits, Ipp32s);
    nbits += state->mc_alloc_bits;
    for(sb = 0; sb < 12; sb++)
      state->mc_tc_alloc[sb] = state->mc_tc_allocation;
  }
  else {
    state->mc_tc_allocation = 0;
    for(sb = 0; sb < 12; sb++) {
      GET_BITS(m_data, state->mc_tc_alloc[sb], state->mc_alloc_bits, Ipp32s);
      nbits += state->mc_alloc_bits;
    }
  }

  if( state->mc_dyn_cross_on == 1) {
    GET_BITS(m_data, state->mc_dyn_cross_LR, 1, Ipp32s);
    nbits++;
    if(state->mc_dyn_cross_bits)
      for(sb = 0; sb < 12; sb++) {
        GET_BITS(m_data, state->mc_dyn_cross_mode[sb], state->mc_dyn_cross_bits, Ipp32s);
        nbits += state->mc_dyn_cross_bits;
        if (state->mc_header.surround == 3) {
          GET_BITS(m_data, state->mc_dyn_second_stereo[sb], 1, Ipp32s);
          nbits++;
        }
      }
  }
  else state->mc_dyn_cross_LR = 0;

  if( state->mc_prediction_on == 1) {
    for(sb = 0; sb < 8; sb++) {
      GET_BITS(m_data, state->mc_prediction[sb], 1, Ipp32s);
      nbits++;
      if ( state->mc_prediction[sb] == 1 ) {
        Ipp32s val;
        val = mp3_mc_pred_coef_table[state->mc_pred_mode][state->mc_dyn_cross_mode[sb]];
        for(i = 0; i < val ; i++) {
          GET_BITS(m_data, state->mc_predsi[sb][i], 2, Ipp32s);
          nbits += 2;
        }
      }
    }
  }
  state->crc_nbits += nbits;
}

void mp3dec_CRC_start(MP3Dec_com *state)
{
  sBitsreamBuffer *m_data = &(state->m_StreamData);

  state->crc_nbits = 0;
  state->crc_ptr = m_data->pCurrent_dword;
  state->crc_offset = m_data->nBit_offset;
}

void mp3dec_CRC_update(MP3Dec_com *state, Ipp32u *crc)
{
  bs_CRC_update(state->crc_ptr, state->crc_offset,
    state->crc_nbits, crc);
}

#endif //UMC_ENABLE_XXX
