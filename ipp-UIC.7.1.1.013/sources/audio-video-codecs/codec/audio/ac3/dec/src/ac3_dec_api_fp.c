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
#if defined (UMC_ENABLE_AC3_AUDIO_DECODER)

#include "ac3_dec.h"
#include "ac3_dec_own_fp.h"
#include "ac3_dec_tables.h"
#include "ipps.h"

#define AC3_FRAME_SIZE 1920 * 2

#define AC3_UPDATE_PTR(type, ptr, inc)    \
  if (ptr) {                              \
  ptr = (type*)((Ipp8u*)(ptr) + inc);     \
}

/********************************************************************/

void ac3decUpdateMemMap(AC3Dec *state,
                        Ipp32s shift)
{
  Ipp32s i;

  for (i = 0; i < 2; i++) {
    AC3_UPDATE_PTR(Ipp32f, state->ShortBuff[i], shift)
  }

  for (i = 0; i < 6; i++) {
    AC3_UPDATE_PTR(Ipp32f, state->temp[i], shift)
  }

  AC3_UPDATE_PTR(IppsMDCTInvSpec_32f, state->allocation_imdct.pMDCTSpecLong, shift)
  AC3_UPDATE_PTR(IppsMDCTInvSpec_32f, state->allocation_imdct.pMDCTSpecShort, shift)
  AC3_UPDATE_PTR(Ipp8u, state->allocation_imdct.pBufferShort, shift)
}

/********************************************************************/

AC3Status ac3decInit(AC3Dec *state,
                     Ipp32s *sizeAll)
{
  Ipp8u  *ptr, *ptrWork, *ptrInit;
  Ipp32s i, size, sizeOfAC3Dec;
  Ipp32s sizeInit = 0;
  Ipp32s sizeWork = 0;
  Ipp32s sizeSpecInvLong = 0;
  Ipp32s sizeSpecInvShort = 0;
  Ipp32s sizeInitTmp = 0;
  Ipp32s sizeWorkTmp = 0;

  sizeOfAC3Dec = (sizeof(AC3Dec) + 15) & (~15);
  size = sizeOfAC3Dec + 256 * sizeof(Ipp32f);

  if (ippsMDCTInvGetSize_32f(512, &sizeSpecInvLong, &sizeInitTmp, &sizeWorkTmp) != ippStsNoErr) {
    return AC3_ALLOC;
  }

  if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
  if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;

  if (ippsMDCTInvGetSize_32f(256, &sizeSpecInvShort, &sizeInitTmp, &sizeWorkTmp) != ippStsNoErr) {
    return AC3_ALLOC;
  }

  if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
  if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;

  size += sizeSpecInvLong + sizeSpecInvShort + sizeWork + sizeInit;
  *sizeAll = size;

  if (state) {
    ippsSet_8u(0, (Ipp8u*)state, size);
    state->ShortBuff[0] = (Ipp32f*)((Ipp8u*)state + sizeOfAC3Dec);
    state->ShortBuff[1] = state->ShortBuff[0] + 128;
    ptr = (Ipp8u*)(state->ShortBuff[1] + 128);

    state->allocation_imdct.pMDCTSpecLong = NULL;
    state->allocation_imdct.pMDCTSpecShort = NULL;
    state->allocation_imdct.pBufferLong = NULL;
    state->allocation_imdct.pBufferShort = NULL;

    ptrWork = ptr + sizeSpecInvLong + sizeSpecInvShort;
    ptrInit = ptrWork + sizeWork;

    if (ippsMDCTInvInit_32f(&(state->allocation_imdct.pMDCTSpecLong), 512,
        ptr, ptrInit) != ippStsOk)
      return AC3_ALLOC;

    if (ippsMDCTInvInit_32f(&(state->allocation_imdct.pMDCTSpecShort), 256,
        ptr + sizeSpecInvLong, ptrInit) != ippStsOk)
      return AC3_ALLOC;

    state->allocation_imdct.pBufferLong =
    state->allocation_imdct.pBufferShort = ptrWork;

    state->out_acmod      = 0;
    state->outlfeon       = 0;
    state->dualmonomode   = 0;
    state->drc_scaleLow   = 1;
    state->drc_scaleHigh  = 1;
    state->out_compmod    = 0;
    state->karaokeCapable = 3;
    state->crc_mute       = 0;
    state->as_input       = 0;
    state->nChannelOut    = 2;
    state->gainScale      = 1;

    for (i = 0; i < 7; i++) {
      state->lfedeltba[i] = 0;
    }

    ac3decReset(state);
  }

  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decReset(AC3Dec *state)
{
  Ipp32s i;

  if (!state)
    return AC3_NULL_PTR;

  state->m_frame_number = 0;
  state->syncinfo.frame_size = AC3_FRAME_SIZE;

  for (i = 0; i < 6; i++) {
    ippsZero_32f(state->coeffs[i], 256);
    ippsZero_32f(state->delay[i], 256);
    state->temp[i] = state->coeffs[i];
  }

  state->mants_tabls.dithtemp = 1;

  state->m_frame_number = 0;
  state->nChannelOut = NFCHANS[state->out_acmod] +
                               state->outlfeon;

  for (i = 0; i < 5; i++) {
    state->audblk.chexpstr[i] = 0;
  }
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decClose(/* AC3Dec *state */)
{
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decGetFrame(Ipp8u *inPointer,
                         Ipp32s inDataSize,
                         Ipp32s *decodedBytes,
                         Ipp16s *outPointer,
                         Ipp32s outBufferSize,
                         AC3Dec *state)
{
  AC3Status result;
  Ipp32s cbErrors = 0;
  Ipp32s unused_bits_sync, unused_bits;
  Ipp32s nChannelOut;
  Ipp32s nblk;
  Ipp32s cbMantErrors = 0, cbExpErrors = 0;
  Ipp32s sizeCrc1, firstBadBlock;
  Ipp32s start_bits;
  Ipp32s crc1, crc2;
  Ipp32s goodFrame;
  sBitsreamBuffer BS;
  sBitsreamBuffer *pBS = &BS;
  Ipp8u *ptrCrc;

  if (!inPointer || !outPointer || !state)
    return AC3_NULL_PTR;

  GET_INIT_BITSTREAM(pBS, inPointer)

  result = GetSynch(pBS, inDataSize);

  GET_BITS_COUNT(pBS, unused_bits_sync)
  start_bits = unused_bits_sync - 16;
  unused_bits_sync = inDataSize * 8 - unused_bits_sync;

  if (result != AC3_OK) {
    *decodedBytes = inDataSize - ((unused_bits_sync + 16 + 7) >> 3);
    if (*decodedBytes < 0) *decodedBytes = 0;
    return AC3_NOT_FIND_SYNCWORD;
  }

  if (unused_bits_sync < 24) {
    *decodedBytes = inDataSize - ((unused_bits_sync + 16 + 7) >> 3);
    if (*decodedBytes < 0) *decodedBytes = 0;
    return AC3_NOT_ENOUGH_DATA;
  }

  ptrCrc = (Ipp8u*)(pBS->pCurrent_dword) - (pBS->nBit_offset >> 3) + 4;
  cbErrors = ParseSyncInfo(state, pBS);

  GET_BITS_COUNT(pBS, unused_bits)
  unused_bits = inDataSize * 8 - unused_bits;

  if (cbErrors != 0) {
    *decodedBytes = inDataSize - ((unused_bits + 7) >> 3);
    if (*decodedBytes < 0) *decodedBytes = 0;
    return AC3_BAD_STREAM;
  }

  if (unused_bits < state->syncinfo.frame_size * 16 - 16 - 24) {
    *decodedBytes = inDataSize - ((unused_bits_sync + 16 + 7) >> 3);
    if (*decodedBytes < 0) *decodedBytes = 0;
    return AC3_NOT_ENOUGH_DATA;
  }

  sizeCrc1 = (state->syncinfo.frame_size >> 1) + (state->syncinfo.frame_size >> 3);
  crc1 = crcCheck(2 * (sizeCrc1 - 1), ptrCrc);

  ptrCrc += 2 * (sizeCrc1 - 1);
  crc2 = crcCheck(2 * (state->syncinfo.frame_size - sizeCrc1), ptrCrc);
  crc2 += crc1;

  if (crc1 == 0) {
    ippsZero_8u((Ipp8u*)&(state->bsi), sizeof(_BSI));
    cbErrors = ParseBsi(state, pBS);

    GET_BITS_COUNT(pBS, unused_bits)
    unused_bits = inDataSize * 8 - unused_bits;

    if (cbErrors != 0) {
      *decodedBytes = inDataSize - ((unused_bits + 7) >> 3);
      if (*decodedBytes < 0) *decodedBytes = 0;
      return AC3_BAD_STREAM;
    }
  }

  nChannelOut = state->nChannelOut;

  if (outBufferSize < (Ipp32s)(nChannelOut*256*6*sizeof(Ipp16s))) {
    *decodedBytes = 0;
    return AC3_NOT_ENOUGH_BUFFER;
  }

  firstBadBlock = 1;
  goodFrame = 1;

  for (nblk = 0; nblk < 6; nblk++) {
    if (((nblk <= 1) && (crc1 != 0)) ||
        ((nblk > 1) && (crc2 != 0))) {
      goodFrame = 0;
    }

    if (goodFrame) {
      if (ParseAudblk(nblk, state, pBS) < 1) {
        goodFrame = 0;
      }
    }

    if (goodFrame) {
      cbExpErrors += DecodeExponents(state);
      BitAllocation(state);
      cbMantErrors += UnpackMantissas(state, pBS);

      if (state->bsi.acmod == 0x2)
        Rematrix(state);

      if (((state->audblk.blksw == 0) ||
           (state->audblk.blksw == ((1 << state->bsi.nfchans) - 1))) &&
          ((state->nChannelOut - state->bsi.lfeon) < state->bsi.nfchans)) {
          Downmix(state->audblk.dynrng, state->audblk.dynrng2,
                  1, state);
          InverseTransform(0, state);
      } else {
        InverseTransform(1, state);
        Downmix(state->audblk.dynrng, state->audblk.dynrng2,
               0, state);

      }
      WindowingOverlap(outPointer, state);
    } else {
      if (state->crc_mute) {
        if (firstBadBlock) {
          Ipp32s i;
          firstBadBlock = 0;

          for (i = 0; i < nChannelOut; i++) {
            ippsZero_32f(state->temp[i], 512);
          }

          WindowingOverlap(outPointer, state);

        }   else {
          ippsZero_8u((Ipp8u*)outPointer, nChannelOut*256*2);
        }
      } else {
        Ipp32s i;
        Ipp32f *pDataTemp[6];

        for (i = 0; i < state->nChannelOut; i++) {
          ippsAddProduct_32f(WindowTable, state->temp[i], state->delay[i], 256);
          pDataTemp[i] = state->delay[i];
        }

        if (state->gainScale != 1) {
          for (i = 0; i < state->nChannelOut; i++) {
            ippsMulC_32f_I(state->gainScale, state->delay[i], 256);
          }
        }

        ippsJoin_32f16s_D2L((const Ipp32f **)pDataTemp, nChannelOut,
                            256, (Ipp16s*)outPointer);

        for (i = 0; i < state->nChannelOut; i++) {
          ippsMul_32f(WindowTable + 256, state->temp[i] + 256,
                      state->delay[i], 256);
        }
      }
    }

    outPointer += nChannelOut*256;
  }

  if (goodFrame)  {
    ParseAuxdata(state, pBS, start_bits);
    GET_BITS_COUNT(pBS, (*decodedBytes))
    *decodedBytes >>= 3;
  } else {
    *decodedBytes = state->syncinfo.frame_size * 2;
  }

  state->m_frame_number++;
  return AC3_OK;
}

/********************************************************************/

AC3Status GetSynch(sBitsreamBuffer *pBS,
                   Ipp32s inDataSize)
{
  Ipp32s syncw, tmp;

  inDataSize *= 8;

  GET_BITS_COUNT(pBS, tmp)
  tmp = inDataSize - tmp;

  if (tmp < 16)
    return AC3_NOT_FIND_SYNCWORD;

  GET_BITS(pBS, syncw, 16, Ipp32s)
  for (;;) {
    if ((syncw & 0xffff) == 0x0b77)
      break;

    GET_BITS_COUNT(pBS, tmp)
    tmp = inDataSize - tmp;

    if (tmp < 8)
      return AC3_NOT_FIND_SYNCWORD;

    GET_BITS(pBS, tmp, 8, Ipp32s)
    syncw = (syncw << 8) | tmp;
  }

  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetOutAcmod(Ipp32s out_acmod,
                            AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((out_acmod < 0) || (out_acmod > 8))
    return AC3_FLAGS_ERROR;

  state->as_input = 0;
  if (8 == out_acmod) {
    state->as_input = 1;
    state->out_acmod = 7; /* Doesn't matter */
  } else {
    state->out_acmod = out_acmod;
  }

  state->nChannelOut = NFCHANS[state->out_acmod] +
                       state->outlfeon;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetOuLfeOn(Ipp32s outlfeon,
                           AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((outlfeon < 0) || (outlfeon > 1))
    return AC3_FLAGS_ERROR;

  state->outlfeon = outlfeon;
  state->nChannelOut = NFCHANS[state->out_acmod] +
                       state->outlfeon;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetDualMonoMode(Ipp32s dualmonomode,
                                AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((dualmonomode < 0) || (dualmonomode > 3))
    return AC3_FLAGS_ERROR;

  state->dualmonomode = dualmonomode;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetDrcScaleLow(Ipp32f drc_scaleLow,
                               AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((drc_scaleLow < 0) || (drc_scaleLow > 1))
    return AC3_FLAGS_ERROR;

  state->drc_scaleLow = drc_scaleLow;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetDrcScaleHigh(Ipp32f drc_scaleHigh,
                                AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((drc_scaleHigh < 0) || (drc_scaleHigh > 1))
    return AC3_FLAGS_ERROR;

  state->drc_scaleHigh = drc_scaleHigh;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetOutCompMod(Ipp32s out_compmod,
                              AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((out_compmod < 0) || (out_compmod > 3))
    return AC3_FLAGS_ERROR;

  state->out_compmod = out_compmod;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetKaraokeCapable(Ipp32s karaokeCapable,
                                  AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((karaokeCapable < -1) || (karaokeCapable > 3))
    return AC3_FLAGS_ERROR;

  state->karaokeCapable = karaokeCapable;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetCrcMute(Ipp32s crc_mute,
                           AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if ((crc_mute < 0) || (crc_mute > 1))
    return AC3_FLAGS_ERROR;

  state->crc_mute = crc_mute;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decSetGainScale(Ipp32f gainScale,
                             AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  if (gainScale < 0)
    return AC3_FLAGS_ERROR;

  state->gainScale = gainScale;
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decGetNumChannelOut(Ipp32s *nChannelOut,
                                 AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  *nChannelOut = state->nChannelOut;

  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decGetSampleFrequency(Ipp32s *SampleRate,
                                   AC3Dec *state)
{
  if (!state)
    return AC3_NULL_PTR;

  *SampleRate = state->syncinfo.SampleRate;

  return AC3_OK;
}

/********************************************************************/


AC3Status ac3decGetInfo(cAudioCodecParams *a_info,
                        AC3Dec *state)
{
  if (!a_info)
    return AC3_NULL_PTR;

  a_info->m_SuggestedInputSize = AC3_FRAME_SIZE;
  a_info->m_SuggestedOutputSize = 6 * (256 * 6) * sizeof(Ipp16s);
  a_info->m_info_in.stream_type = AC3_AUD;
  a_info->m_info_out.stream_type = PCM_AUD;
  a_info->is_valid = 0;

  if (!state)
    return AC3_OK;

  a_info->m_info_in.channels = state->nChannelOut;
  a_info->m_info_out.channels = state->nChannelOut;

  if (state->m_frame_number > 0) {
    a_info->m_info_in.bitPerSample = 0;
    a_info->m_info_out.bitPerSample = 16;

    a_info->m_info_in.bitrate = state->syncinfo.bit_rate * 1000;
    a_info->m_info_out.bitrate = 0;

    a_info->m_info_in.sample_frequency = state->syncinfo.SampleRate;
    a_info->m_info_out.sample_frequency = state->syncinfo.SampleRate;

    a_info->m_frame_num = state->m_frame_number;
    a_info->is_valid = 1;

    switch (state->bsi.acmod) {
      case ACMOD_0:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_10:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        break;
      case ACMOD_20:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_30:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_21:
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_CENTER;
        break;
      case ACMOD_31:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_CENTER;
        break;
      case ACMOD_22:
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_RIGHT;
        break;
      case ACMOD_32:
        a_info->m_info_in.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_LEFT;
        a_info->m_info_in.channel_mask |= AC3_CHANNEL_BACK_RIGHT;
        break;
      default:
        break;
    }
    if (state->bsi.lfeon) {
      a_info->m_info_in.channel_mask |= AC3_CHANNEL_LOW_FREQUENCY;
    }

    switch (state->out_acmod) {
      case ACMOD_0:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_10:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        break;
      case ACMOD_20:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_30:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        break;
      case ACMOD_21:
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_CENTER;
        break;
      case ACMOD_31:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_CENTER;
        break;
      case ACMOD_22:
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_RIGHT;
        break;
      case ACMOD_32:
        a_info->m_info_out.channel_mask  = AC3_CHANNEL_FRONT_CENTER;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_FRONT_RIGHT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_LEFT;
        a_info->m_info_out.channel_mask |= AC3_CHANNEL_BACK_RIGHT;
        break;
      default:
        break;
    }
    if (state->outlfeon) {
      a_info->m_info_out.channel_mask |= AC3_CHANNEL_LOW_FREQUENCY;
    }
  }
  return AC3_OK;
}

/********************************************************************/

AC3Status ac3decGetDuration(Ipp32f *p_duration,
                            AC3Dec *state)
{
  Ipp32f  duration;

  duration = (Ipp32f)(state->m_frame_number) * 6 * 256;
  *p_duration = duration / (Ipp32f)(state->syncinfo.SampleRate);

  return AC3_OK;
}

/********************************************************************/

#endif //UMC_ENABLE_AC3_AUDIO_DECODER
