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
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER) || defined (UMC_ENABLE_MP4_SPLITTER)

#include "bstream.h"
#include "mp4cmn_config.h"
#include "mp4cmn_pce.h"
#include <string.h>

static Ipp32s
getAudioObjectType(sBitsreamBuffer * p_bs)
{
  Ipp32s audioObjectType;
  Ipp32s tmp;

  audioObjectType = Getbits(p_bs,5);
  if (0x1f == audioObjectType) {
    audioObjectType = Getbits(p_bs,6);
    tmp = Getbits(p_bs,5);
    audioObjectType += 32;
  }

  return audioObjectType;
}


Ipp32s
dec_audio_specific_config(sAudio_specific_config * p_data, sBitsreamBuffer * p_bs)
{
    Ipp32s decodedBits;
    Ipp32s tmp;

    memset(&p_data->GASpecificConfig, 0, sizeof(sGA_specific_config));
    p_data->channelConfiguration = 0;
    p_data->samplingFrequency = 0;

    p_data->audioObjectType = getAudioObjectType(p_bs);

    p_data->samplingFrequencyIndex = Getbits(p_bs,4);

    if ( 0x0f == p_data->samplingFrequencyIndex )
    {
        p_data->samplingFrequency = Getbits(p_bs,24);
    }
    p_data->channelConfiguration = Getbits(p_bs,4);

    p_data->sbrPresentFlag = -1;
    p_data->psPresentFlag  = -1;

    p_data->extensionAudioObjectType = 0;
    p_data->extensionSamplingFrequency = 0;
    p_data->extensionSamplingFrequencyIndex = 0;

    if (5 == p_data->audioObjectType || 29 == p_data->audioObjectType)
    {
        p_data->extensionAudioObjectType = 5;//SBR_AOT
        p_data->sbrPresentFlag = 1;

        if( 29 == p_data->audioObjectType ){
          p_data->psPresentFlag  = 1;
        }

        p_data->extensionSamplingFrequencyIndex = Getbits(p_bs,4);
        if ( 0x0f == p_data->extensionSamplingFrequencyIndex )
        {
            p_data->extensionSamplingFrequency = Getbits(p_bs,24);
        }
        p_data->audioObjectType = getAudioObjectType(p_bs);
    }

    switch (p_data->audioObjectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 22:
        dec_ga_specific_config(&p_data->GASpecificConfig, p_bs, /* p_data->samplingFrequencyIndex, */
                               p_data->channelConfiguration, p_data->audioObjectType);
        break;
    case 36:
        tmp = Getbits(p_bs,32); /* ALS */
        if (decALSSpecificConfig(&p_data->ALSSpecificConfig, p_bs) < 0) {
          return -1;
        }
        break;
    default:
        return -1;
    }

    if (p_data->audioObjectType == 22) {
      p_data->epConfig = Getbits(p_bs,2);
      //if (p_data->epConfig > 1) {
      //  return -1;
      //}
    }

    GET_BITS_COUNT(p_bs, decodedBits)

    if (p_data->extensionAudioObjectType != 5 && (p_bs->nDataLen*8 - decodedBits) >= 16)
    {
        Ipp32s syncExtensionType = Getbits(p_bs,11);

        if (0x2b7 == syncExtensionType)
        {
            p_data->extensionAudioObjectType = Getbits(p_bs,5);

            if ( p_data->extensionAudioObjectType == 5 ) {

                p_data->sbrPresentFlag = Getbits(p_bs,1);
                if (p_data->sbrPresentFlag == 1) {
                    p_data->extensionSamplingFrequencyIndex = Getbits(p_bs, 4);

                    if ( p_data->extensionSamplingFrequencyIndex == 0xf ){
                        p_data->extensionSamplingFrequency = Getbits(p_bs, 24);
                    }

                    if (p_bs->nDataLen*8 - decodedBits >= 12) {
                      Ipp32s syncExtType;
                        syncExtType = Getbits(p_bs, 11);
                        if (syncExtType == 0x548) {
                            p_data->psPresentFlag = Getbits(p_bs, 1);
                        }
                    }
                }
            }
        }
    }

    return 0;
}


Ipp32s
enc_audio_specific_config(sAudio_specific_config * p_data, sBitsreamBuffer * p_bs)
{
//    memset(&p_data->GASpecificConfig, 0, sizeof(sGA_specific_config));
//    p_data->channelConfiguration = 0;
//    p_data->samplingFrequency = 0;

    PUT_BITS(p_bs, p_data->audioObjectType, 5);
    PUT_BITS(p_bs, p_data->samplingFrequencyIndex, 4);

    if ( 0x0f == p_data->samplingFrequencyIndex )
    {
      PUT_BITS(p_bs, p_data->samplingFrequency, 24);
    }

    PUT_BITS(p_bs, p_data->channelConfiguration, 4);

//    p_data->sbrPresentFlag = -1;

//    p_data->extensionAudioObjectType = 0;
//    p_data->extensionSamplingFrequency = 0;
//    p_data->extensionSamplingFrequencyIndex = 0;

    if (5 == p_data->audioObjectType)
    {
//        p_data->extensionAudioObjectType = p_data->audioObjectType;
//        p_data->sbrPresentFlag = 1;
        PUT_BITS(p_bs, p_data->extensionSamplingFrequencyIndex, 4);
        if ( 0x0f == p_data->extensionSamplingFrequencyIndex )
        {
          PUT_BITS(p_bs, p_data->extensionSamplingFrequency, 24);
        }
        PUT_BITS(p_bs, p_data->audioObjectType, 5);
    }

    switch (p_data->audioObjectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
        enc_ga_specific_config(&p_data->GASpecificConfig, p_bs /*, p_data->channelConfiguration */);
        break;
    default:
        return -1;
    }

/*
    if (p_data->extensionAudioObjectType != 5 && p_bs->nBit_offset >= 16 )
    {
        Ipp32s syncExtensionType = Getbits(p_bs,11);

        if (0x2b7 == syncExtensionType)
        {
            p_data->extensionAudioObjectType = Getbits(p_bs,5);

            if ( p_data->extensionAudioObjectType == 5 )
            {
                p_data->sbrPresentFlag = Getbits(p_bs,1);
                if (p_data->sbrPresentFlag == 1)
                {
                    p_data->extensionSamplingFrequencyIndex = Getbits(p_bs, 4);

                    if ( p_data->extensionSamplingFrequencyIndex == 0xf )
                        p_data->extensionSamplingFrequency = Getbits(p_bs, 24);
                }
            }
        }
    }
*/

    SAVE_BITSTREAM(p_bs)
    Byte_alignment(p_bs);
    return 0;
}





Ipp32s
enc_ga_specific_config(sGA_specific_config * p_data,sBitsreamBuffer * p_bs /*, Ipp32s channelConfiguration */)
{

    /// GASpecificConfig();

    PUT_BITS(p_bs, p_data->frameLengthFlag, 1);
    PUT_BITS(p_bs, p_data->dependsOnCoreCoder, 1);
    if (p_data->dependsOnCoreCoder)
    {
        PUT_BITS(p_bs, p_data->coreCoderDelay, 14);
    }
    PUT_BITS(p_bs, p_data->extensionFlag, 1);

/*
    if (0 == channelConfiguration)
    {
        enc_program_config_element(&p_data->pce,p_bs);
    }
*/

    SAVE_BITSTREAM(p_bs)
    return 0;
}

Ipp32s
dec_ga_specific_config(sGA_specific_config * p_data,sBitsreamBuffer * p_bs, /* Ipp32s samplingFrequencyIndex, */
                       Ipp32s channelConfiguration, Ipp32s audioObjectType)
{

    /// GASpecificConfig();

    p_data->frameLengthFlag = Getbits(p_bs,1);
    p_data->dependsOnCoreCoder = Getbits(p_bs,1);
    if (p_data->dependsOnCoreCoder)
    {
        p_data->coreCoderDelay = Getbits(p_bs,14);
    }
    p_data->extensionFlag = Getbits(p_bs,1);

    if (0 == channelConfiguration)
    {
        dec_program_config_element(&p_data->pce,p_bs);
    }

    if (p_data->extensionFlag != 0) {
      if (audioObjectType == 22) {
        p_data->numOfSubFrame = Getbits(p_bs,5);
        p_data->layerLength = Getbits(p_bs,11);
      }
    }

    return 0;
}

/********************************************************************

    Unpack functions (support alternative bitstream representation)

********************************************************************/

Ipp32s
unpack_audio_specific_config(sAudio_specific_config * p_data, Ipp8u **pp_bitstream, Ipp32s *p_offset)
{
    p_data->audioObjectType = get_bits(pp_bitstream,p_offset,5);
    p_data->samplingFrequencyIndex = get_bits(pp_bitstream,p_offset,4);

    if ( 0x0f == p_data->samplingFrequencyIndex )
    {
        p_data->samplingFrequency = get_bits(pp_bitstream,p_offset,24);
    }
    p_data->channelConfiguration = get_bits(pp_bitstream,p_offset,4);


    switch (p_data->audioObjectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
        unpack_ga_specific_config(&p_data->GASpecificConfig, pp_bitstream, p_offset,
                                  /* p_data->samplingFrequencyIndex, */ p_data->channelConfiguration /*, p_data->audioObjectType */);
        break;
    default:
        return -1;
    }

    return 0;
}

Ipp32s
unpack_ga_specific_config(sGA_specific_config * p_data, Ipp8u **pp_bitstream, Ipp32s *p_offset,
                          /* Ipp32s samplingFrequencyIndex,*/ Ipp32s channelConfiguration /*, Ipp32s audioObjectType */)
{
    /// GASpecificConfig();

    p_data->frameLengthFlag = get_bits(pp_bitstream,p_offset,1);
    p_data->dependsOnCoreCoder = get_bits(pp_bitstream,p_offset,1);
    if (p_data->dependsOnCoreCoder)
    {
        p_data->coreCoderDelay = get_bits(pp_bitstream,p_offset,14);
    }
    p_data->extensionFlag = get_bits(pp_bitstream,p_offset,1);


    if (0 == channelConfiguration)
    {
        unpack_program_config_element(&p_data->pce,pp_bitstream,p_offset);
    }

    return 0;
}

Ipp32s
get_channels_number(sAudio_specific_config * p_data)
{
    Ipp32s ch;
    Ipp32s i;
    sProgram_config_element* p_pce;

    switch(p_data->channelConfiguration)
    {
    case 0:
        {
            p_pce = &p_data->GASpecificConfig.pce;
            ch = 0;
            for ( i = 0; i < p_pce->num_front_channel_elements; i ++)
            {
                ch ++;
                if (0 != p_pce->front_element_is_cpe[i])
                {
                    ch ++;
                }
            }
            for ( i = 0; i < p_pce->num_back_channel_elements; i ++)
            {
                ch ++;
                if (0 != p_pce->back_element_is_cpe[i])
                {
                    ch ++;
                }
            }
            for ( i = 0; i < p_pce->num_side_channel_elements; i ++)
            {
                ch ++;
                if (0 != p_pce->side_element_is_cpe[i])
                {
                    ch ++;
                }
            }
            for ( i = 0; i < p_pce->num_lfe_channel_elements; i ++)
            {
                ch ++;
            }
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        ch = p_data->channelConfiguration;
        break;
    case 7:
        ch = 8;
        break;
    default:
        ch = 0;
    }

    return ch;
}

static Ipp32s g_sampling_frequency_table[] =
{
    96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,7350,0,0,0
};

static Ipp32s g_sampling_frequency_mapping_table[] =
{
  // minimum for each sampling frequency
  92017, // 96000
  75132, // 88200
  55426, // 64000
  46009, // 48000
  37566, // 44100
  27713, // 32000
  23004, // 24000
  18783, // 22050
  13856, // 16000
  11502, // 12000
  9391,  // 11025
  0,     // 8000
  0
};

Ipp32s
get_sampling_frequency_index(sAudio_specific_config * p_data)
{
    Ipp32s sampling_frequency_index;
    Ipp32s sampling_frequency;

    if (0x0f == p_data->samplingFrequencyIndex)
    {
        if ( 0 != p_data->channelConfiguration)
        {
            sampling_frequency = get_sampling_frequency(p_data, 0);
            sampling_frequency_index = 0;
            while (g_sampling_frequency_mapping_table[sampling_frequency_index] > sampling_frequency) sampling_frequency_index ++;
        }
        else
        {
            sampling_frequency_index = p_data->GASpecificConfig.pce.sampling_frequency_index;
        }
    }
    else
    {
        sampling_frequency_index = p_data->samplingFrequencyIndex;
    }

   return sampling_frequency_index;
}

Ipp32s
get_sampling_frequency(sAudio_specific_config * p_data, Ipp32s bHEAAC)
{
    Ipp32s sampling_frequency;

    Ipp32s dataSamplingFrequency;
    Ipp32s dataSamplingFrequencyIndex;

    if (bHEAAC)
    {
        dataSamplingFrequency      = p_data->extensionSamplingFrequency;
        dataSamplingFrequencyIndex = p_data->extensionSamplingFrequencyIndex;
    }
    else
    {
        dataSamplingFrequency      = p_data->samplingFrequency;
        dataSamplingFrequencyIndex = p_data->samplingFrequencyIndex;
    }

    if ( 0x0f == dataSamplingFrequencyIndex)
    {
        sampling_frequency = dataSamplingFrequency;
    }
    else
    {
        sampling_frequency = g_sampling_frequency_table[dataSamplingFrequencyIndex];
    }

    return sampling_frequency;
}

static Ipp8u logTab[] = {
  1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

Ipp32s decALSSpecificConfig(sALS_specific_config *state,
                            sBitsreamBuffer *pBS)
{
  Ipp32u tmp;
  Ipp32s ch;
  Ipp32s i, decodedBits, remainBits;
  Ipp32s size0, size1;

  GET_BITS(pBS, state->sampFreq, 32, Ipp32u)
  GET_BITS(pBS, state->numSamples, 32, Ipp32u)
  GET_BITS(pBS, state->numChannels, 16, Ipp32s)

  ch = state->numChannels;
  if (ch == 0)
    ch = 1;

  if (ch < 256) {
    state->chNeededLen = (Ipp32s)logTab[ch];
  } else {
    state->chNeededLen = (Ipp32s)logTab[ch >> 8] + 8;
  }

  state->numChannels += 1;

  GET_BITS(pBS, tmp, 32, Ipp32u)

  state->fileType = (tmp >> 29) & 7;
  state->resolution = ((tmp >> 23) & 0x38) + 8;
  state->floating = (tmp >> 25) & 1;
  state->msbFirst = (tmp >> 24) & 1;
  state->frameLength = ((tmp >> 8) & 0xffff) + 1;
  state->ra = tmp & 0xff;

  GET_BITS(pBS, tmp, 32, Ipp32u)

  state->raFlag = (tmp >> 30) & 3;
  state->adaptOrder = (tmp >> 29) & 1;
  state->coefTable = (tmp >> 27) & 3;

  state->LongTermPrediction = (tmp >> 26) & 1;
  state->maxOrder = (tmp >> 16) & 0x3ff;
  state->blockSwitching = (tmp >> 14) & 3;
  state->bgmcMode = (tmp >> 13) & 1;
  state->sbPart = (tmp >> 12) & 1;
  state->jointStereo = (tmp >> 11) & 1;
  state->mcCoding = (tmp >> 10) & 1;
  state->chanConfig = (tmp >> 9) & 1;
  state->chanSort = (tmp >> 8) & 1;
  state->crcEnabled = (tmp >> 7) & 1;
  state->RLSLMS = (tmp >> 6) & 1;
  state->auxDataEnabled = tmp & 1;

  if (state->numChannels == 1) {
    state->jointStereo = 0;
  }

  if (state->chanConfig) {
    GET_BITS(pBS, state->chanConfigInfo, 16, Ipp32s)
  }

  bs_copy(pBS, &(state->savedBS));

  GET_BITS_COUNT(pBS, decodedBits)
  remainBits = pBS->nDataLen*8 - decodedBits;

  if (state->chanSort) {
    if (state->numChannels * state->chNeededLen > remainBits) {
      return -1;
    } else {
      for (ch = 0; ch < state->numChannels; ch++) {
        GET_BITS(pBS, tmp, state->chNeededLen, Ipp32u)
      }
      remainBits -= state->numChannels * state->chNeededLen;
    }
  }
  Byte_alignment(pBS);

  GET_BITS(pBS, state->headerSize, 32, Ipp32u)
  GET_BITS(pBS, state->trailerSize, 32, Ipp32u)

  GET_BITS_COUNT(pBS, decodedBits)
  remainBits = pBS->nDataLen*8 - decodedBits;

  if (state->headerSize == 0xffffffff)
    state->headerSize = 0;

  if (state->trailerSize == 0xffffffff)
    state->trailerSize = 0;

  if (state->headerSize > 0x7fffffff) {
    size0 = 0x7fffffff;
    size1 = (Ipp32s)(state->headerSize - 0x7fffffff);
  } else {
    size0 = (Ipp32s)state->headerSize;
    size1 = 0;
  }

  if (size0 * 8 > remainBits) {
    return -1;
  }

  remainBits -= size0 * 8;

  if (size1 * 8 > remainBits) {
    return -1;
  }

  remainBits -= size1 * 8;

  for (i = 0; i < size0; i++) {
    GET_BITS(pBS, tmp, 8, Ipp32u)
  }

  for (i = 0; i < size1; i++) {
    GET_BITS(pBS, tmp, 8, Ipp32u)
  }

  if (state->trailerSize > 0x7fffffff) {
    size0 = 0x7fffffff;
    size1 = (Ipp32s)(state->trailerSize - 0x7fffffff);
  } else {
    size0 = (Ipp32s)state->trailerSize;
    size1 = 0;
  }

  if (size0 * 8 > remainBits) {
    return -1;
  }

  remainBits -= size0 * 8;

  if (size1 * 8 > remainBits) {
    return -1;
  }

  remainBits -= size1 * 8;

  for (i = 0; i < size0; i++) {
    GET_BITS(pBS, tmp, 8, Ipp32u)
  }

  for (i = 0; i < size1; i++) {
    GET_BITS(pBS, tmp, 8, Ipp32u)
  }

  if (state->crcEnabled) {
    GET_BITS(pBS, state->crc, 32, Ipp32u)
    remainBits -= 32;
  }

  state->numFrames = state->numSamples / state->frameLength;
  state->lastFrameLength = state->numSamples % state->frameLength;

  if (state->lastFrameLength != 0) {
    state->numFrames++;
  } else {
    state->lastFrameLength = state->frameLength;
  }

  state->raTotNumUnits = 0;

  if ((state->raFlag == 2) && (state->ra > 0)) {
    if (state->numFrames == 0) {
      state->raTotNumUnits = 0;
    } else {
      state->raTotNumUnits = ((state->numFrames - 1) / state->ra) + 1;
    }

    if (state->raTotNumUnits * 32 > remainBits) {
      return -1;
    }

    remainBits -= state->raTotNumUnits * 32;

    for (i = 0; i < state->raTotNumUnits; i++) {
      GET_BITS(pBS, tmp, 32, Ipp32u)
    }
  }

  if (state->auxDataEnabled) {
    Ipp32u size;

    GET_BITS(pBS, size, 32, Ipp32u)

    remainBits -= 32;

    if (size > 0x7fffffff) {
      size0 = 0x7fffffff;
      size1 = (Ipp32s)(size - 0x7fffffff);
    } else {
      size0 = (Ipp32s)size;
      size1 = 0;
    }

    if (size0 * 8 > remainBits) {
      return -1;
    }

    remainBits -= size0 * 8;

    if (size1 * 8 > remainBits) {
      return -1;
    }

    for (i = 0; i < size0; i++) {
      GET_BITS(pBS, tmp, 8, Ipp32u)
    }

    for (i = 0; i < size1; i++) {
      GET_BITS(pBS, tmp, 8, Ipp32u)
    }
  }
  return 0;
}

#endif //UMC_ENABLE_XXX
