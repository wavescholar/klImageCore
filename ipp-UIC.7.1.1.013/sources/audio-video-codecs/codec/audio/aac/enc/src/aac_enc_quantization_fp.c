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

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include "math.h"
#include "aac_enc_quantization_fp.h"
#include "aac_enc_search.h"

extern Ipp32u sf_huff_codebook[];

/****************************************************************************/

void main_loop(sQuantizationBlock* pBlock,
               sEnc_individual_channel_stream* pStream,
               Ipp32f* mdct_line,
               Ipp32f* mdct_scaled,
               Ipp32s* bitsForScaleFactorData);

void aac_FindSF(sEnc_individual_channel_stream* pStream,
                sQuantizationData* qData,
                Ipp32f* mdct_scaled,
                Ipp32f* mdct_line_abs,
                Ipp32s* startSF,
                Ipp32s* finishSF,
                Ipp16s* scalefac,
                Ipp16s* maxXQuant);

void aac_UpdateSF(sEnc_individual_channel_stream* pStream,
                  Ipp32f* mdct_scaled,
                  Ipp32f* mdct_line_abs,
                  Ipp32s* startSF,
                  Ipp16s* scalefac,
                  Ipp16s* maxXQuant,
                  Ipp32s  minSf,
                  Ipp32f  bitsToPeCoeff,
                  Ipp32f  possibleAddBits);

/****************************************************************************/

static Ipp32f scalefac_pow[] = {
  (Ipp32f)4.408719e+005, (Ipp32f)3.871411e+005, (Ipp32f)3.399587e+005,
    (Ipp32f)2.985266e+005, (Ipp32f)2.621440e+005, (Ipp32f)2.301955e+005,
    (Ipp32f)2.021407e+005, (Ipp32f)1.775050e+005, (Ipp32f)1.558718e+005,
    (Ipp32f)1.368751e+005, (Ipp32f)1.201936e+005, (Ipp32f)1.055451e+005,
    (Ipp32f)9.268190e+004, (Ipp32f)8.138639e+004, (Ipp32f)7.146751e+004,
    (Ipp32f)6.275749e+004, (Ipp32f)5.510899e+004, (Ipp32f)4.839264e+004,
    (Ipp32f)4.249484e+004, (Ipp32f)3.731583e+004, (Ipp32f)3.276800e+004,
    (Ipp32f)2.877444e+004, (Ipp32f)2.526758e+004, (Ipp32f)2.218812e+004,
    (Ipp32f)1.948397e+004, (Ipp32f)1.710938e+004, (Ipp32f)1.502419e+004,
    (Ipp32f)1.319314e+004, (Ipp32f)1.158524e+004, (Ipp32f)1.017330e+004,
    (Ipp32f)8.933439e+003, (Ipp32f)7.844686e+003, (Ipp32f)6.888623e+003,
    (Ipp32f)6.049080e+003, (Ipp32f)5.311855e+003, (Ipp32f)4.664478e+003,
    (Ipp32f)4.096000e+003, (Ipp32f)3.596804e+003, (Ipp32f)3.158448e+003,
    (Ipp32f)2.773515e+003, (Ipp32f)2.435496e+003, (Ipp32f)2.138673e+003,
    (Ipp32f)1.878024e+003, (Ipp32f)1.649142e+003, (Ipp32f)1.448155e+003,
    (Ipp32f)1.271662e+003, (Ipp32f)1.116680e+003, (Ipp32f)9.805858e+002,
    (Ipp32f)8.610779e+002, (Ipp32f)7.561350e+002, (Ipp32f)6.639819e+002,
    (Ipp32f)5.830598e+002, (Ipp32f)5.120000e+002, (Ipp32f)4.496006e+002,
    (Ipp32f)3.948060e+002, (Ipp32f)3.466894e+002, (Ipp32f)3.044370e+002,
    (Ipp32f)2.673341e+002, (Ipp32f)2.347530e+002, (Ipp32f)2.061428e+002,
    (Ipp32f)1.810193e+002, (Ipp32f)1.589578e+002, (Ipp32f)1.395850e+002,
    (Ipp32f)1.225732e+002, (Ipp32f)1.076347e+002, (Ipp32f)9.451687e+001,
    (Ipp32f)8.299773e+001, (Ipp32f)7.288247e+001, (Ipp32f)6.400000e+001,
    (Ipp32f)5.620007e+001, (Ipp32f)4.935075e+001, (Ipp32f)4.333618e+001,
    (Ipp32f)3.805463e+001, (Ipp32f)3.341676e+001, (Ipp32f)2.934413e+001,
    (Ipp32f)2.576785e+001, (Ipp32f)2.262742e+001, (Ipp32f)1.986972e+001,
    (Ipp32f)1.744812e+001, (Ipp32f)1.532165e+001, (Ipp32f)1.345434e+001,
    (Ipp32f)1.181461e+001, (Ipp32f)1.037472e+001, (Ipp32f)9.110309e+000,
    (Ipp32f)8.000000e+000, (Ipp32f)7.025009e+000, (Ipp32f)6.168843e+000,
    (Ipp32f)5.417022e+000, (Ipp32f)4.756828e+000, (Ipp32f)4.177095e+000,
    (Ipp32f)3.668016e+000, (Ipp32f)3.220981e+000, (Ipp32f)2.828427e+000,
    (Ipp32f)2.483716e+000, (Ipp32f)2.181015e+000, (Ipp32f)1.915207e+000,
    (Ipp32f)1.681793e+000, (Ipp32f)1.476826e+000, (Ipp32f)1.296840e+000,
    (Ipp32f)1.138789e+000, (Ipp32f)1.000000e+000, (Ipp32f)8.781261e-001,
    (Ipp32f)7.711054e-001, (Ipp32f)6.771278e-001, (Ipp32f)5.946036e-001,
    (Ipp32f)5.221369e-001, (Ipp32f)4.585020e-001, (Ipp32f)4.026226e-001,
    (Ipp32f)3.535534e-001, (Ipp32f)3.104645e-001, (Ipp32f)2.726269e-001,
    (Ipp32f)2.394008e-001, (Ipp32f)2.102241e-001, (Ipp32f)1.846033e-001,
    (Ipp32f)1.621049e-001, (Ipp32f)1.423486e-001, (Ipp32f)1.250000e-001,
    (Ipp32f)1.097658e-001, (Ipp32f)9.638818e-002, (Ipp32f)8.464097e-002,
    (Ipp32f)7.432544e-002, (Ipp32f)6.526711e-002, (Ipp32f)5.731275e-002,
    (Ipp32f)5.032782e-002, (Ipp32f)4.419417e-002, (Ipp32f)3.880806e-002,
    (Ipp32f)3.407837e-002, (Ipp32f)2.992510e-002, (Ipp32f)2.627801e-002,
    (Ipp32f)2.307541e-002, (Ipp32f)2.026312e-002, (Ipp32f)1.779357e-002,
    (Ipp32f)1.562500e-002, (Ipp32f)1.372072e-002, (Ipp32f)1.204852e-002,
    (Ipp32f)1.058012e-002, (Ipp32f)9.290681e-003, (Ipp32f)8.158389e-003,
    (Ipp32f)7.164094e-003, (Ipp32f)6.290978e-003, (Ipp32f)5.524272e-003,
    (Ipp32f)4.851007e-003, (Ipp32f)4.259796e-003, (Ipp32f)3.740638e-003,
    (Ipp32f)3.284752e-003, (Ipp32f)2.884426e-003, (Ipp32f)2.532890e-003,
    (Ipp32f)2.224197e-003, (Ipp32f)1.953125e-003, (Ipp32f)1.715090e-003,
    (Ipp32f)1.506065e-003, (Ipp32f)1.322515e-003, (Ipp32f)1.161335e-003,
    (Ipp32f)1.019799e-003, (Ipp32f)8.955118e-004, (Ipp32f)7.863722e-004,
    (Ipp32f)6.905340e-004, (Ipp32f)6.063759e-004, (Ipp32f)5.324745e-004,
    (Ipp32f)4.675797e-004, (Ipp32f)4.105940e-004, (Ipp32f)3.605533e-004,
    (Ipp32f)3.166112e-004, (Ipp32f)2.780246e-004, (Ipp32f)2.441406e-004,
    (Ipp32f)2.143863e-004, (Ipp32f)1.882582e-004, (Ipp32f)1.653144e-004,
    (Ipp32f)1.451669e-004, (Ipp32f)1.274748e-004, (Ipp32f)1.119390e-004,
    (Ipp32f)9.829653e-005, (Ipp32f)8.631675e-005, (Ipp32f)7.579699e-005,
    (Ipp32f)6.655931e-005, (Ipp32f)5.844747e-005, (Ipp32f)5.132424e-005,
    (Ipp32f)4.506916e-005, (Ipp32f)3.957640e-005, (Ipp32f)3.475307e-005,
    (Ipp32f)3.051758e-005, (Ipp32f)2.679828e-005, (Ipp32f)2.353227e-005,
    (Ipp32f)2.066430e-005, (Ipp32f)1.814586e-005, (Ipp32f)1.593435e-005,
    (Ipp32f)1.399237e-005, (Ipp32f)1.228707e-005, (Ipp32f)1.078959e-005,
    (Ipp32f)9.474623e-006, (Ipp32f)8.319914e-006, (Ipp32f)7.305933e-006,
    (Ipp32f)6.415531e-006, (Ipp32f)5.633645e-006, (Ipp32f)4.947050e-006,
    (Ipp32f)4.344134e-006, (Ipp32f)3.814697e-006, (Ipp32f)3.349785e-006,
    (Ipp32f)2.941534e-006, (Ipp32f)2.583037e-006, (Ipp32f)2.268233e-006,
    (Ipp32f)1.991794e-006, (Ipp32f)1.749046e-006, (Ipp32f)1.535883e-006,
    (Ipp32f)1.348699e-006, (Ipp32f)1.184328e-006, (Ipp32f)1.039989e-006,
    (Ipp32f)9.132417e-007, (Ipp32f)8.019413e-007, (Ipp32f)7.042056e-007,
    (Ipp32f)6.183813e-007, (Ipp32f)5.430167e-007, (Ipp32f)4.768372e-007,
    (Ipp32f)4.187231e-007, (Ipp32f)3.676917e-007, (Ipp32f)3.228797e-007,
    (Ipp32f)2.835291e-007, (Ipp32f)2.489743e-007, (Ipp32f)2.186308e-007,
    (Ipp32f)1.919854e-007, (Ipp32f)1.685874e-007, (Ipp32f)1.480410e-007,
    (Ipp32f)1.299987e-007, (Ipp32f)1.141552e-007, (Ipp32f)1.002427e-007,
    (Ipp32f)8.802570e-008, (Ipp32f)7.729766e-008, (Ipp32f)6.787709e-008,
    (Ipp32f)5.960464e-008, (Ipp32f)5.234039e-008, (Ipp32f)4.596146e-008,
    (Ipp32f)4.035996e-008, (Ipp32f)3.544113e-008, (Ipp32f)3.112178e-008,
    (Ipp32f)2.732885e-008, (Ipp32f)2.399818e-008, (Ipp32f)2.107342e-008,
    (Ipp32f)1.850512e-008, (Ipp32f)1.624983e-008, (Ipp32f)1.426940e-008,
    (Ipp32f)1.253033e-008, (Ipp32f)1.100321e-008, (Ipp32f)9.662208e-009,
    (Ipp32f)8.484637e-009, (Ipp32f)7.450581e-009, (Ipp32f)6.542549e-009,
    (Ipp32f)5.745183e-009, (Ipp32f)5.044995e-009, (Ipp32f)4.430142e-009,
    (Ipp32f)3.890223e-009, (Ipp32f)3.416106e-009, (Ipp32f)2.999772e-009,
    (Ipp32f)2.634178e-009, (Ipp32f)2.313140e-009, (Ipp32f)2.031229e-009,
    (Ipp32f)1.783675e-009
};

/****************************************************************************/
static void aac_LimitSF(Ipp32f* mdct_scaled,
                        Ipp32s  size,
                        Ipp32s* startSF,
                        Ipp32s* finishSF)
{
  Ipp32f start_scalefac, finish_scalefac, max_mdct_line;

  startSF[0] = -SF_OFFSET;
  finishSF[0] = -SF_OFFSET;
  max_mdct_line = 0;

  ippsMax_32f(mdct_scaled, size, &max_mdct_line);

  if (max_mdct_line > 0) {
    start_scalefac = (Ipp32f)log(max_mdct_line/MAX_QUANT)/(Ipp32f)log(2);
    start_scalefac *= (Ipp32f)16/(Ipp32f)3;
    startSF[0] = (Ipp32s)floor((Ipp64f)start_scalefac);

    if (startSF[0] < start_scalefac) startSF[0]++;

    finish_scalefac = (Ipp32f)log(max_mdct_line/(1 - MAGIC_NUMBER))/(Ipp32f)log(2);
    finish_scalefac *= (Ipp32f)16/(Ipp32f)3;
    finishSF[0] = (Ipp32s)floor((Ipp64f)finish_scalefac);

    if (finishSF[0] < finish_scalefac) finishSF[0]++;

    finishSF[0]++;

    if (startSF[0] > 255 - SF_OFFSET) startSF[0] = 255 - SF_OFFSET;
    else if (startSF[0] < -SF_OFFSET) startSF[0] = -SF_OFFSET;

    if (finishSF[0] > 255 - SF_OFFSET) finishSF[0] = 255 - SF_OFFSET;
    else if (finishSF[0] < -SF_OFFSET) finishSF[0] = -SF_OFFSET;
  }
}

/****************************************************************************/

void aac_MinMaxSF(sEnc_individual_channel_stream* pStream,
                  Ipp32f* mdct_scaled,
                  Ipp32s* startSF,
                  Ipp16s* scalefac,
                  Ipp16s* maxXQuant,
                  Ipp32s* minSf,
                  Ipp32s* maxSf,
                  Ipp32s  maxSfDelta)
{
  Ipp32s *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width = pStream->sfb_width;
  Ipp32s numSfb = pStream->num_window_groups * pStream->max_sfb;
  int sfb, update;

  minSf[0] = 100000;
  maxSf[0] = -100000;

  for (sfb = 0; sfb < numSfb; sfb++) {
    if (maxXQuant[sfb] != 0) {
      if (scalefac[sfb] > maxSf[0]) maxSf[0] = scalefac[sfb];
      if (scalefac[sfb] < minSf[0]) minSf[0] = scalefac[sfb];
    } else {
      ippsZero_32f(mdct_scaled + sfb_offset[sfb], sfb_width[sfb]);
    }
  }

  update = 0;
  if ((maxSf[0] - minSf[0]) > maxSfDelta) {
    for (sfb = 0; sfb < numSfb; sfb++) {
      if (maxXQuant[sfb] != 0) {
        /* very big difference */
        if ((minSf[0] + maxSfDelta) < startSF[sfb]) {
          minSf[0] = startSF[sfb] - maxSfDelta;
          update = 1;
        }
      }
    }
  }

  if (update == 1) {
    for (sfb = 0; sfb < numSfb; sfb++) {
      /* re quantization */
      if (scalefac[sfb] < minSf[0]) {
#if !defined(ANDROID)
        IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
        IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
#else
        static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
        static IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
#endif
        Ipp32f sf = (Ipp32f)scalefac_pow[minSf[0] + SF_OFFSET];
        Ipp32f temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;

        ippsAddC_32f(mdct_scaled + sfb_offset[sfb], temp, mdct_rqnt, sfb_width[sfb]);
        ippsMulC_Low_32f16s(mdct_rqnt, sf, x_quant_unsigned, sfb_width[sfb]);
        ippsMax_16s(x_quant_unsigned, sfb_width[sfb], maxXQuant + sfb);

        scalefac[sfb] = (Ipp16s)minSf[0];
      }
    }
  }

  if ((maxSf[0] - minSf[0]) > maxSfDelta) {
    for (sfb = 0; sfb < numSfb; sfb++) {
      if (maxXQuant[sfb] != 0) {
        if (scalefac[sfb] > minSf[0] + maxSfDelta)
          scalefac[sfb] = (Ipp16s)(minSf[0] + maxSfDelta);
      }
    }
    maxSf[0] = minSf[0] + maxSfDelta;
  }
}

/****************************************************************************/

void Quantization(sQuantizationBlock* pBlock,
                  sEnc_individual_channel_stream* pStream,
                  sQuantizationData* qData)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_abs, N_LONG/2);
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_scaled, N_LONG/2);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_abs, N_LONG/2);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_scaled, N_LONG/2);
#endif
  Ipp32f* mdct_line      = qData->mdct_line;
  Ipp32f* bitsToPeCoeff  = qData->bitsToPeCoeff;
  Ipp32f* scalefactorDataBits = qData->scalefactorDataBits;
  Ipp32s* sfb_offset = pStream->sfb_offset;
  Ipp32s* sfb_width = pStream->sfb_width;
  Ipp32s  win, sfb;
  Ipp32s  firstNonZeroSfb;
  Ipp32s  bitsForScaleFactorData;

  ippsAbs_32f(mdct_line, mdct_line_abs, pStream->max_line);
  ippsPow34_32f(mdct_line_abs, mdct_scaled, pStream->max_line);

  pBlock->start_common_scalefac = -SF_OFFSET;
  pBlock->finish_common_scalefac = -SF_OFFSET;

  if (pBlock->ns_mode) {
#if !defined(ANDROID)
    Ipp32s  startSF[MAX_SECTION_NUMBER];
    Ipp32s  finishSF[MAX_SECTION_NUMBER];
    Ipp16s  maxXQuant[MAX_SECTION_NUMBER];
#else
    static Ipp32s  startSF[MAX_SECTION_NUMBER];
    static Ipp32s  finishSF[MAX_SECTION_NUMBER];
    static Ipp16s  maxXQuant[MAX_SECTION_NUMBER];
#endif
    Ipp16s* ns_scale_factors = pBlock->ns_scale_factors;
    Ipp16s* pMaxXQuant = maxXQuant;
    Ipp32s  numSfb = pStream->num_window_groups * pStream->max_sfb;
    Ipp32s  minSf = 100000;
    Ipp32s  maxSf = -100000;

    for (sfb = 0; sfb < numSfb; sfb++) {
      aac_LimitSF(mdct_scaled + sfb_offset[sfb], sfb_width[sfb],
                  &startSF[sfb], &finishSF[sfb]);
    }

    aac_FindSF(pStream, qData, mdct_scaled, mdct_line_abs, startSF,
               finishSF, ns_scale_factors, maxXQuant);

    aac_MinMaxSF(pStream, mdct_scaled, startSF, ns_scale_factors,
                 maxXQuant, &minSf, &maxSf, 60);

    aac_UpdateSF(pStream, mdct_scaled, mdct_line_abs, startSF,
                 ns_scale_factors, maxXQuant, minSf, *bitsToPeCoeff,
                 0.2f * pBlock->bits_per_frame);

    aac_MinMaxSF(pStream, mdct_scaled, startSF, ns_scale_factors,
                 maxXQuant, &minSf, &maxSf, 60);

    for (sfb = 0; sfb < numSfb; sfb++) {
      if (maxXQuant[sfb] != 0) {
        ns_scale_factors[sfb] = (Ipp16s)(maxSf - ns_scale_factors[sfb]);
        if (ns_scale_factors[sfb] > 0) {
          Ipp32f mul = (Ipp32f)scalefac_pow[SF_OFFSET - ns_scale_factors[sfb]];
          ippsMulC_32f_I(mul, mdct_scaled + sfb_offset[sfb], sfb_width[sfb]);
        }
      }
    }

    for (win = 0; win < pStream->num_window_groups; win++) {
      firstNonZeroSfb = -1;

      for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
        if (pMaxXQuant[sfb] != 0) {
          if (firstNonZeroSfb < 0) {
            firstNonZeroSfb = sfb;
          }
        }
      }

      if (firstNonZeroSfb < 0) {
        for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
          ns_scale_factors[sfb] = 0;
        }
      } else {
        for (sfb = 0; sfb < firstNonZeroSfb; sfb++) {
          ns_scale_factors[sfb] = ns_scale_factors[firstNonZeroSfb];
        }

        for (sfb = firstNonZeroSfb + 1; sfb < pStream->max_sfb; sfb++) {
          if (pMaxXQuant[sfb] == 0) {
            ns_scale_factors[sfb] = ns_scale_factors[sfb - 1];
          }
        }
      }
      ns_scale_factors += pStream->max_sfb;
      pMaxXQuant += pStream->max_sfb;
    }
    ns_scale_factors = pBlock->ns_scale_factors;
    pBlock->last_frame_common_scalefactor[0] = maxSf;
    pBlock->common_scalefactor_update[0] = 1;
  }

  aac_LimitSF(mdct_scaled, pStream->max_line,
              &pBlock->start_common_scalefac,
              &pBlock->finish_common_scalefac);

  main_loop(pBlock, pStream, mdct_line, mdct_scaled,
            &bitsForScaleFactorData);

  if (pBlock->ns_mode) {
#if !defined(ANDROID)
    IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
    IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
    Ipp32f noise[MAX_SECTION_NUMBER];
#else
    static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
    static IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
    static Ipp32f noise[MAX_SECTION_NUMBER];
#endif
    Ipp32s numSfb = pStream->num_window_groups * pStream->max_sfb;
    Ipp32f pe = 0, real_sf;

    *scalefactorDataBits = (*scalefactorDataBits) * 0.9f +
                           bitsForScaleFactorData * 0.1f;

    for (sfb = 0; sfb < numSfb; sfb++) {
      Ipp32s scalefactor = pStream->scale_factors[sfb] + pBlock->ns_scale_factors[sfb];
      Ipp32f sf = (Ipp32f)scalefac_pow[scalefactor];
      Ipp32f temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;
      Ipp32s sfb_start = sfb_offset[sfb];
      Ipp32f energy = qData->energy[sfb];

#if 0
    ippsNoiseCalculation_32f(mdct_scaled + sfb_start, mdct_line_abs + sfb_start, temp, sf, 
      x_quant_unsigned, sfb_width[sfb], pStream->scale_factors[sfb]-SF_OFFSET, &noise[sfb], (Ipp8u*)mdct_rqnt);
#else
      ippsAddC_32f(mdct_scaled + sfb_start, temp, mdct_rqnt, sfb_width[sfb]);
      ippsMulC_Low_32f16s(mdct_rqnt, sf, (Ipp16s*)x_quant_unsigned, sfb_width[sfb]);
      ippsPow43_16s32f(x_quant_unsigned, mdct_rqnt, sfb_width[sfb]);
      ippsCalcSF_16s32f(&pStream->scale_factors[sfb], SF_OFFSET, &real_sf, 1);
      ippsMulC_32f_I(real_sf, mdct_rqnt, sfb_width[sfb]);
      ippsSub_32f_I(mdct_line_abs + sfb_start, mdct_rqnt, sfb_width[sfb]);
      ippsDotProd_32f(mdct_rqnt, mdct_rqnt, sfb_width[sfb], &noise[sfb]);
#endif

      if (energy > noise[sfb]) {
        pe += (Ipp32f)(sfb_width[sfb] * log10(energy / noise[sfb]));
      }
    }

    qData->outPe = pe;

    if (pe > 0) {
      *bitsToPeCoeff = (*bitsToPeCoeff) * 0.9f +
        (pe/(pBlock->used_bits - bitsForScaleFactorData)) * 0.1f;
    }
  }
}

/****************************************************************************/

void main_loop(sQuantizationBlock* pBlock,
               sEnc_individual_channel_stream* pStream,
               Ipp32f* mdct_line,
               Ipp32f* mdct_scaled,
               Ipp32s* bitsForScaleFactorData)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_x_quant, N_LONG/2);
  IPP_ALIGNED_ARRAY(32, Ipp16s, mdct_sign, N_LONG/2);
  IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
  IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_signed_pred, N_LONG/2);
  Ipp16s maxXQuant[MAX_SECTION_NUMBER];
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_x_quant, N_LONG/2);
  static IPP_ALIGNED_ARRAY(32, Ipp16s, mdct_sign, N_LONG/2);
  static IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
  static IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_signed_pred, N_LONG/2);
  static Ipp16s maxXQuant[MAX_SECTION_NUMBER];
#endif
  Ipp32f sf, temp;
  Ipp32s start_common_scalefac = pBlock->start_common_scalefac;
  Ipp32s finish_common_scalefac = pBlock->finish_common_scalefac;
  Ipp32s common_scalefactor;
  Ipp32s common_scalefactor_update;
  Ipp32s needed_bits, bits_for_scale_factor_data;
  Ipp32s num_scale_factor;
  Ipp32s i;

  num_scale_factor = pStream->num_window_groups * pStream->max_sfb;

  for (i = 0; i < pStream->max_line; i++) {
    mdct_sign[i] = SIGN(mdct_line[i]);
  }

  common_scalefactor = pBlock->last_frame_common_scalefactor[0];
  common_scalefactor_update = pBlock->common_scalefactor_update[0];

  if (common_scalefactor < start_common_scalefac)
    common_scalefactor = start_common_scalefac;

  if (common_scalefactor > finish_common_scalefac)
    common_scalefactor = finish_common_scalefac;

  for(;;) {
    sf = (Ipp32f)scalefac_pow[common_scalefactor + SF_OFFSET];
    temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;
#if 0
    ippsAddMulC_Low_32f16u(mdct_scaled, temp, sf, (Ipp16u*)x_quant_unsigned, pStream->max_line);
#else
    ippsAddC_32f(mdct_scaled, temp, tmp_x_quant, pStream->max_line);
    ippsMulC_Low_32f16s(tmp_x_quant, sf, x_quant_unsigned, pStream->max_line);
#endif
    ippsMul_16s(mdct_sign, x_quant_unsigned, pStream->x_quant, pStream->max_line);

    if (pBlock->ns_mode) {
      int win, sfb, firstNonZeroSfb;
      Ipp16s* ns_scale_factors = pBlock->ns_scale_factors;
      Ipp16s* pMaxXQuant = maxXQuant;

      for (sfb = 0; sfb < pStream->num_window_groups * pStream->max_sfb; sfb++) {
        Ipp32s sfb_start = pStream->sfb_offset[sfb];
        Ipp32s width = pStream->sfb_width[sfb];

        ippsMax_16s(x_quant_unsigned + sfb_start, width, maxXQuant + sfb);
      }

      for (win = 0; win < pStream->num_window_groups; win++) {
        firstNonZeroSfb = -1;

        for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
          if (pMaxXQuant[sfb] != 0) {
            if (firstNonZeroSfb < 0) {
              firstNonZeroSfb = sfb;
            }
          }
        }

        if (firstNonZeroSfb < 0) {
          for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
            ns_scale_factors[sfb] = 0;
          }
        } else {
          for (sfb = 0; sfb < firstNonZeroSfb; sfb++) {
            ns_scale_factors[sfb] = ns_scale_factors[firstNonZeroSfb];
          }

          for (sfb = firstNonZeroSfb + 1; sfb < pStream->max_sfb; sfb++) {
            if (pMaxXQuant[sfb] == 0) {
              ns_scale_factors[sfb] = ns_scale_factors[sfb - 1];
            }
          }
        }
        ns_scale_factors += pStream->max_sfb;
        pMaxXQuant += pStream->max_sfb;
      }
    }

    needed_bits = best_codebooks_search(pStream, x_quant_unsigned,
                                        pStream->x_quant, NULL,
                                        x_quant_signed_pred);


    if (pBlock->ns_mode) {
      common_scalefactor += SF_OFFSET;
      for (i = 0; i < num_scale_factor; i++) {
        pStream->scale_factors[i] = (Ipp16s)(common_scalefactor -
          pBlock->ns_scale_factors[i]);
      }
      common_scalefactor -= SF_OFFSET;
      bits_for_scale_factor_data = enc_scale_factor_data(pStream, NULL, 0);
    } else {
      bits_for_scale_factor_data = num_scale_factor;

      for (i = 0; i < num_scale_factor; i++) {
        if (pStream->sfb_cb[i] == 0)
          bits_for_scale_factor_data--;
      }
    }

    needed_bits += bits_for_scale_factor_data;

    if (needed_bits == pBlock->available_bits)
      break;

    if (needed_bits > pBlock->available_bits) {
      if (common_scalefactor == finish_common_scalefac)
        break;

      if (common_scalefactor_update < 0) {
        common_scalefactor_update = -common_scalefactor_update;
      }
      common_scalefactor_update = (common_scalefactor_update + 1) >> 1;
    } else {
      if (common_scalefactor == start_common_scalefac)
        break;

      if (common_scalefactor_update == 1)
        break;

      if (common_scalefactor_update > 0) {
        common_scalefactor_update = -common_scalefactor_update;
      }
      common_scalefactor_update >>= 1;
    }

    common_scalefactor += common_scalefactor_update;

    if (common_scalefactor < start_common_scalefac)
      common_scalefactor = start_common_scalefac;

    if (common_scalefactor > finish_common_scalefac)
      common_scalefactor = finish_common_scalefac;

  }

  pBlock->common_scalefactor_update[0] =
    common_scalefactor - pBlock->last_frame_common_scalefactor[0];
  pBlock->last_frame_common_scalefactor[0] = common_scalefactor;

  if (pBlock->common_scalefactor_update[0] >= 0) {
    if (pBlock->common_scalefactor_update[0] <= 2)
      pBlock->common_scalefactor_update[0] = 2;
  } else {
    if (pBlock->common_scalefactor_update[0] >= -2)
      pBlock->common_scalefactor_update[0] = -2;
  }

  pBlock->used_bits = needed_bits;
  *bitsForScaleFactorData = bits_for_scale_factor_data;

  common_scalefactor += SF_OFFSET;

  if (!pBlock->ns_mode) {
    for (i = 0; i < num_scale_factor; i++) {
      pStream->scale_factors[i] = (Ipp16s)common_scalefactor;
    }
  }
}

/****************************************************************************/

static Ipp32f aac_NoiseCalculation(Ipp32f* mdct_scaled,
                                   Ipp32f* mdct_line_abs,
                                   Ipp16s* scalefactor,
                                   Ipp16s* maxXQuant,
                                   Ipp32s  width)

{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
  IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, N_LONG/2);
  static IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, N_LONG/2);
#endif
  Ipp32f sf, temp, real_sf, noise;

  sf = (Ipp32f)scalefac_pow[scalefactor[0] + SF_OFFSET];
  temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;
#if 0
    ippsNoiseCalculation_32f(mdct_scaled, mdct_line_abs, temp, sf, 
      x_quant_unsigned, width, *scalefactor, &noise, (Ipp8u*)mdct_rqnt);
#else
  ippsAddC_32f(mdct_scaled, temp, mdct_rqnt, width);
  ippsMulC_Low_32f16s(mdct_rqnt, sf, (Ipp16s*)x_quant_unsigned, width);
  ippsPow43_16s32f(x_quant_unsigned, mdct_rqnt, width);
  ippsCalcSF_16s32f(scalefactor, 0, &real_sf, 1);
  ippsMulC_32f_I(real_sf, mdct_rqnt, width);
  ippsSub_32f_I(mdct_line_abs, mdct_rqnt, width);
  ippsDotProd_32f(mdct_rqnt, mdct_rqnt, width, &noise);
#endif
  ippsMax_16s(x_quant_unsigned, width, maxXQuant);

  return noise;
}

/****************************************************************************/

void aac_FindSF(sEnc_individual_channel_stream* pStream,
                sQuantizationData* qData,
                Ipp32f* mdct_scaled,
                Ipp32f* mdct_line_abs,
                Ipp32s* startSF,
                Ipp32s* finishSF,
                Ipp16s* scalefac,
                Ipp16s* maxXQuant)
{
  Ipp16s scalefactor;
  Ipp32f noise;
  Ipp32s sfb;
  Ipp32f *noiseThr = qData->noiseThr;
  Ipp32s stopSF;
  Ipp32s *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width = pStream->sfb_width;

  for (sfb = 0; sfb < pStream->num_window_groups * pStream->max_sfb; sfb++) {
    Ipp32s sfb_start = sfb_offset[sfb];
    Ipp32s width = sfb_width[sfb];
    Ipp32s start_scalefac = startSF[sfb];
    Ipp32s finish_scalefac = finishSF[sfb];
    Ipp32s i, ind;
    Ipp32f sqrtMdctLine = 0;
    Ipp16s maxXQuantSaved;
    Ipp32f minNoise;

    for (i = 0; i < width; i++) {
      sqrtMdctLine += (Ipp32f)sqrt(mdct_line_abs[sfb_start+i]);
    }

    if ((noiseThr[sfb] > 0) && (qData->energy[sfb] >= noiseThr[sfb])) {
      Ipp32f tmp;

      tmp = (Ipp32f)log((27.0/4.0)*noiseThr[sfb]/sqrtMdctLine)/(Ipp32f)log(2);
      tmp *= (Ipp32f)8/(Ipp32f)3;
      scalefactor = (Ipp16s)(tmp+0.5);
      if (scalefactor < start_scalefac)  scalefactor = (Ipp16s)start_scalefac;
      if (scalefactor > finish_scalefac) scalefactor = (Ipp16s)finish_scalefac;

      noise = aac_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                                   &scalefactor, maxXQuant + sfb, width);

      minNoise = noise;
      ind = 0;
      maxXQuantSaved = maxXQuant[sfb];

      /* Let's check other scalefactors. */
      /* Perhaps quantization with bigger scalefactors will give us less noise. */

      stopSF = finish_scalefac - scalefactor + 1;
      if (stopSF > 5) stopSF = 5; /* magic value :) */

      for (i = 1; i < stopSF; i++) {
        Ipp16s scalefactor1 = scalefactor + (Ipp16s)i;
        if (scalefactor1 > finish_scalefac) scalefactor1 = (Ipp16s)finish_scalefac;
        noise = aac_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                                     &scalefactor1, maxXQuant + sfb, width);

        if (noise < minNoise) {
          minNoise = noise;
          ind = i;
          maxXQuantSaved = maxXQuant[sfb];
        }
      }
      scalefac[sfb] = scalefactor + (Ipp16s)ind;
      maxXQuant[sfb] = maxXQuantSaved;
    } else {
      scalefac[sfb] = -100;
      maxXQuant[sfb] = 0;
    }
  }
}

/****************************************************************************/

void aac_UpdateSF(sEnc_individual_channel_stream* pStream,
                  Ipp32f* mdct_scaled,
                  Ipp32f* mdct_line_abs,
                  Ipp32s* startSF,
                  Ipp16s* scalefac,
                  Ipp16s* maxXQuant,
                  Ipp32s  minSf,
                  Ipp32f  bitsToPeCoeff,
                  Ipp32f  possibleAddBits)
{
#if !defined(ANDROID)
  Ipp32s numScf[MAX_SECTION_NUMBER];
  Ipp32s sfb_start[MAX_SECTION_NUMBER];
  Ipp32s width[MAX_SECTION_NUMBER];
  Ipp32s transferTable[MAX_SECTION_NUMBER];
  Ipp32s index[MAX_SECTION_NUMBER][61];
  Ipp32s tmpMaxXQuant[MAX_SECTION_NUMBER][61];
  Ipp32f numBits[MAX_SECTION_NUMBER][61];
#else
  static Ipp32s numScf[MAX_SECTION_NUMBER];
  static Ipp32s sfb_start[MAX_SECTION_NUMBER];
  static Ipp32s width[MAX_SECTION_NUMBER];
  static Ipp32s transferTable[MAX_SECTION_NUMBER];
  static Ipp32s index[MAX_SECTION_NUMBER][61];
  static Ipp32s tmpMaxXQuant[MAX_SECTION_NUMBER][61];
  static Ipp32f numBits[MAX_SECTION_NUMBER][61];
#endif
  Ipp16s scalefactor0, mXQuant;
  Ipp32f minBits, iniBits;
  Ipp32f minNoise;
  Ipp32s i, j, sfb, bestIndx, realSfb;
  Ipp32s *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width = pStream->sfb_width;
  Ipp32s numSfb = pStream->num_window_groups * pStream->max_sfb;
  Ipp32f coeff = 0.3f * 3.0f/8.0f / (bitsToPeCoeff * 3.32f); /* 3.32 ~ 1/log10(2) */

  realSfb = 0;
  for (sfb = 0; sfb < numSfb; sfb++) {

    /* Exclude scalefactor bands which contain only zero elements */
    if (maxXQuant[sfb] != 0) {
      if (minSf > startSF[sfb]) {
        numScf[realSfb] = scalefac[sfb] - minSf + 1;
      } else {
        numScf[realSfb] = scalefac[sfb] - startSF[sfb] + 1;
      }

      width[realSfb] = sfb_width[sfb];

      for (i = 0; i < numScf[realSfb]; i++) {
        numBits[realSfb][i] = coeff * i * width[realSfb];
      }

      sfb_start[realSfb] = sfb_offset[sfb];
      transferTable[realSfb] = sfb;
      realSfb++;
    }
  }

  if (realSfb > 0) {
    numSfb = realSfb;

    iniBits = 0;

    for (sfb = 1; sfb < numSfb; sfb++) {
#if !defined(ANDROID)
      Ipp32f noiseBuffer[61];
      Ipp16s mXQuantBuffer[61];
#else
      static Ipp32f noiseBuffer[61];
      static Ipp16s mXQuantBuffer[61];
#endif
      Ipp32f addBits = possibleAddBits/(numSfb - sfb + 1);

      iniBits += sf_huff_codebook[2*(scalefac[transferTable[sfb]] -
                                     scalefac[transferTable[sfb-1]] + SF_MID)];

      /* Clean noiseBuffer */

      for (i = 0; i < 61; i++) {
        noiseBuffer[i] = -1;
      }

      for (i = 0; i < numScf[sfb]; i++) {
#if !defined(ANDROID)
        Ipp32f bits[61];
#else
        static Ipp32f bits[61];
#endif
        Ipp32s bestIdx;
        Ipp32s deltaSF = scalefac[transferTable[sfb]] - i -
                         scalefac[transferTable[sfb-1]];

        minBits = bits[0] = sf_huff_codebook[2*(deltaSF + SF_MID)] +
                            numBits[sfb-1][0];
        bestIdx = 0;

        deltaSF++;

        for (j = 1; j < numScf[sfb-1]; j++) {
          bits[j] = sf_huff_codebook[2*(deltaSF + SF_MID)] + numBits[sfb-1][j];
          if (minBits > bits[j]) {
            minBits = bits[j];
            bestIdx = j;
          }
          deltaSF++;
        }

        scalefactor0 = scalefac[transferTable[sfb-1]] - (Ipp16s)bestIdx;

        if (noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] < 0) {

          minNoise = aac_NoiseCalculation(mdct_scaled + sfb_start[sfb-1],
                                          mdct_line_abs + sfb_start[sfb-1],
                                          &scalefactor0, &mXQuant, width[sfb-1]);

          noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] = minNoise;
          mXQuantBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] = mXQuant;
        } else {
          minNoise = noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0];
          mXQuant = mXQuantBuffer[scalefac[transferTable[sfb-1]] - scalefactor0];
        }

        index[sfb-1][i] = bestIdx;
        tmpMaxXQuant[sfb-1][i] = mXQuant;
        scalefactor0 = scalefac[transferTable[sfb-1]];

        for (j = 0; j < numScf[sfb-1]; j++) {
          if (bits[j] <= minBits + addBits) {
            Ipp32f tmpNoise;

            if (noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] < 0) {

              tmpNoise = aac_NoiseCalculation(mdct_scaled + sfb_start[sfb-1],
                                              mdct_line_abs + sfb_start[sfb-1],
                                              &scalefactor0, &mXQuant, width[sfb-1]);

              noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] = tmpNoise;
              mXQuantBuffer[scalefac[transferTable[sfb-1]] - scalefactor0] = mXQuant;
            } else {
              tmpNoise = noiseBuffer[scalefac[transferTable[sfb-1]] - scalefactor0];
              mXQuant = mXQuantBuffer[scalefac[transferTable[sfb-1]] - scalefactor0];
            }

            if (tmpNoise <= minNoise) {
              minNoise = tmpNoise;
              index[sfb-1][i] = j;
              tmpMaxXQuant[sfb-1][i] = mXQuant;
            }
          }
          scalefactor0--;
        }
        numBits[sfb][i] += bits[index[sfb-1][i]];
        possibleAddBits -= bits[index[sfb-1][i]] - minBits;
      }
    }

    scalefactor0 = scalefac[transferTable[numSfb - 1]];

    minBits = numBits[numSfb-1][0];

    minNoise = aac_NoiseCalculation(mdct_scaled + sfb_start[numSfb-1],
                                    mdct_line_abs + sfb_start[numSfb-1],
                                    &scalefactor0, &mXQuant, width[numSfb-1]);

    index[sfb-1][0] = 0;
    tmpMaxXQuant[sfb-1][0] = mXQuant;
    scalefactor0--;

    for (j = 1; j < numScf[numSfb - 1]; j++) {
      Ipp32f tmpBits;

      tmpBits = numBits[numSfb - 1][j];

      if (tmpBits < minBits) {
        Ipp32f tmpNoise;

        tmpNoise = aac_NoiseCalculation(mdct_scaled + sfb_start[numSfb-1],
                                        mdct_line_abs + sfb_start[numSfb-1],
                                        &scalefactor0, &mXQuant, width[numSfb-1]);

        if (tmpNoise < minNoise) {
          minNoise = tmpNoise;
          minBits = tmpBits;
          index[numSfb-1][0] = j;
          tmpMaxXQuant[numSfb-1][0] = mXQuant;
        }
      }
      scalefactor0--;
    }

    bestIndx = index[numSfb-1][0];
    mXQuant = (Ipp16s)tmpMaxXQuant[numSfb-1][0];
    for (sfb = numSfb-1; sfb > 0; sfb--) {
      /* Include scalefactor bands which contain only zero elements */
      scalefac[transferTable[sfb]] = 
          scalefac[transferTable[sfb]] - (Ipp16s)bestIndx;
      maxXQuant[transferTable[sfb]] = mXQuant;
      mXQuant = (Ipp16s)tmpMaxXQuant[sfb-1][bestIndx];
      bestIndx = index[sfb-1][bestIndx];
    }
    /* Include scalefactor bands which contain only zero elements */
    scalefac[transferTable[0]] = scalefac[transferTable[0]] - (Ipp16s)bestIndx;
    maxXQuant[transferTable[0]] = mXQuant;
  }
}

/****************************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

