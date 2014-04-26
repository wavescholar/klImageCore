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

#include "mp3dec_own_fp.h"
#include "ipps.h"

static Ipp32f mp3dec_l1_dequant_table[] = {
    0,
    2.03450527e-05f,
    1.74386169e-05f,
    1.62760425e-05f,
    1.57510076e-05f,
    1.55009930e-05f,
    1.53789370e-05f,
    1.53186284e-05f,
    1.52886496e-05f,
    1.52737048e-05f,
    1.52662433e-05f,
    1.52625162e-05f,
    1.52606517e-05f,
    1.52597204e-05f,
    1.52592547e-05f,
};

Ipp32s mp3dec_decode_data_LayerI(MP3Dec *state)
{
    Ipp32s i, ch, sb;
    Ipp32f *pSampl[2];
    Ipp32f scale;
    Ipp32s (*sample)[32][36] = state->com.sample;
    Ipp16s (*scalefactor)[32] = state->com.scalefactor_l1;
    Ipp16s (*allocation)[32] = state->com.allocation;
    samplefloatrw *smpl_rw = state->smpl_rw;     // out of imdct
    samplefloat *smpl_sb = state->smpl_sb;       // out of subband synth
    Ipp32s stereo = state->com.stereo;
    Ipp16s *m_pOutSamples = state->com.m_pOutSamples;

    pSampl[0] = (*smpl_sb)[0]; pSampl[1] = (*smpl_sb)[1];

    for (ch = 0; ch < stereo; ch++) {
        for (sb = 0; sb < 32; sb++) {
            if (allocation[ch][sb] != 0) {
                Ipp32s idx = allocation[ch][sb] + 1;
                Ipp32s xor_coef;
                Ipp32s *sample_ptr = &sample[ch][sb][0];

                xor_coef = (1 << (idx - 1));

                scale = mp3dec_scale_values[scalefactor[ch][sb]] *
                    mp3dec_l1_dequant_table[allocation[ch][sb]];

                for (i = 0; i < 12; i++) {
                    (*smpl_rw)[ch][i][sb] =
                        ((Ipp32f)(((sample_ptr[i] ^ xor_coef) + 1) << (32 - idx))) * scale;
                }
            } else {
                for (i = 0; i < 12; i++) {
                    (*smpl_rw)[ch][i][sb] = 0;
                }
            }
        }  // for sb

        for (i = 0; i < 12; i++) {
          ippsSynthesisFilter_PQMF_MP3_32f((*(state->smpl_rw))[ch][i],
            pSampl[ch] + i * 32,
            state->pPQMFSpec[ch], 1);
        }
    }    // for ch

    ippsJoin_32f16s_D2L((const Ipp32f **)pSampl, stereo, 384, m_pOutSamples);

    return 1;
}

#endif //UMC_ENABLE_MP3_AUDIO_DECODER
