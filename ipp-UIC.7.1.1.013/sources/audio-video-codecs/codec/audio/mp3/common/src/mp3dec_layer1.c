/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_DECODER)

#include "mp3dec_own.h"

#define MP3DEC_END_OF_BUFFER(BS)                           \
((((((BS)->pCurrent_dword) - ((BS)->pBuffer)) * 32 +      \
 (BS)->init_nBit_offset - ((BS)->nBit_offset)) <=         \
 (BS)->nDataLen * 8) ? 0 : 1)

Ipp32s mp3dec_audio_data_LayerI(MP3Dec_com *state)
{
    Ipp32s gr, ch, sb;
    Ipp32s jsbound = state->jsbound;
    Ipp32s stereo = state->stereo;
    sBitsreamBuffer *m_StreamData = &(state->m_StreamData);
    Ipp16s (*allocation)[32] = state->allocation;
    Ipp32s (*sample)[32][36] = state->sample;
    Ipp16s (*scalefactor)[32] = state->scalefactor_l1;
    Ipp32s end_flag = 0;

    for (sb = 0; sb < jsbound; sb++) {
        for (ch = 0; ch < stereo; ch++) {
            GET_BITS(m_StreamData, allocation[ch][sb], 4, Ipp16s);
        }
    }

    for (sb = jsbound; sb < 32; sb++) {
        GET_BITS(m_StreamData, allocation[0][sb], 4, Ipp16s);
        allocation[1][sb] = allocation[0][sb];
    }

    for (sb = 0; sb < 32; sb++) {
        for (ch = 0; ch < stereo; ch++) {
            if (allocation[ch][sb]!=0) {
                GET_BITS(m_StreamData, scalefactor[ch][sb], 6, Ipp16s);
            }
        }
    }

    for (gr = 0; gr < 12; gr++) {
      if (end_flag || MP3DEC_END_OF_BUFFER(m_StreamData)) {
        end_flag = 1;
        for (sb = 0; sb < 32; sb++)
          for (ch = 0; ch < stereo; ch++)
            sample[ch][sb][gr] = 0;
      } else {
        for (sb = 0; sb < 32; sb++) {
            for (ch = 0; ch < ((sb < jsbound) ? stereo : 1); ch++) {
                if (allocation[ch][sb] != 0) {
                    Ipp32s idx = allocation[ch][sb] + 1;
                    Ipp32s c;

                    GET_BITS(m_StreamData, c, idx, Ipp32s);

                    sample[ch][sb][gr] = c;

                    // joint stereo : copy L to R
                    if (stereo == 2 && sb >= jsbound) {
                        sample[1][sb][gr] = sample[0][sb][gr];
                    }
                }
            }
        }
      }
    }
    return 1;
}

#endif //UMC_ENABLE_XXX
