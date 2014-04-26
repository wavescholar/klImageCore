/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

/* SYSTEM */
#include <math.h>
/* SBR */
#include "sbr_dec_tabs_fp.h"
/* PS */
#include "ps_dec_struct.h"

/********************************************************************/

const Ipp32s tabResBand1020[] = {3, 8, 2, 2};
const Ipp32s tabResBand34[]   = {5, 12, 8, 4, 4, 4};

const Ipp32f tabQuantIidStd[]  = { 0.0562341301f, 0.1258925373f, 0.1995262267f,
                                   0.3162277606f, 0.4466835868f, 0.6309573402f,
                                   0.7943282320f, 1.0000000000f, 1.2589254161f,
                                   1.5848932033f, 2.2387211655f, 3.1622777144f,
                                   5.0118724566f, 7.9432825925f, 17.7827948631f };

const Ipp32f tabQuantIidFine[] = { 0.0031622774f, 0.0056234128f, 0.0099999993f,
                                  0.0177827930f, 0.0316227750f, 0.0562341301f,
                                  0.0794328205f, 0.1122018418f, 0.1584893149f,
                                  0.2238721089f, 0.3162277606f, 0.3981071651f,
                                  0.5011872285f, 0.6309573402f, 0.7943282320f,
                                  1.0000000000f, 1.2589254161f, 1.5848932033f,
                                  1.9952623355f, 2.5118864660f, 3.1622777144f,
                                  4.4668360211f, 6.3095736180f, 8.9125096718f,
                                  12.5892545931f, 17.7827948631f, 31.6227782292f,
                                  56.2341358956f, 100.0000068622f, 177.8279547322f,
                                  316.2277931422f };

/* stereo processing algorithm */
const Ipp32f tabQuantRHO[] = { 1.0f, 0.937f, 0.84118f, 0.60092f,
                               0.36764f, 0.0f, -0.589f, -1.0f};

/********************************************************************/
// tables of scale factor for stereo processing algorithm
// c[b] = 10^(iid[b] / 20)
//
const Ipp32f tabScaleIidStd[] = {
  1.4119828f, 1.4031382f,  1.3868767f,  1.3483998f,  1.2912494f,
  1.1960374f, 1.1073724f,  1.f,         0.87961715f, 0.75464857f,
  0.5767799f, 0.42640144f, 0.27671829f, 0.17664462f, 0.079401627f
};

const Ipp32f tabScaleIidFine[] = {
  1.4142065f,  1.4141912f,  1.4141428f,  1.41399f,     1.413507f,   1.4119828f,
  1.409773f,   1.4053948f,  1.3967797f,  1.380053f,    1.3483998f,  1.3139201f,
  1.2643101f,  1.1960374f,  1.1073724f,  1.f,          0.87961715f, 0.75464857f,
  0.63365608f, 0.52308106f, 0.42640144f, 0.3089554f,   0.22137465f, 0.15768789f,
  0.11198225f, 0.079401627f,0.044699017f,0.025144693f, 0.014141428f,0.007952581f,
  0.004472114f
};

/********************************************************************/
/* transient detection tables [8.6.4.5.3] */
/* BAND_20 */
const Ipp32s tabStartBord_Band20[] = {6, 7, 0, 1, 2, 3, 9, 8, 10, 11,
  3, 4, 5, 6, 7, 8, 9, 11, 14, 18, 23, 35, 64};

const Ipp32s tabStopBord_Band20[] = { 7, 8, 1, 2, 3,  4, 10,  9, 11, 12,  4,
  5, 6, 7, 8, 9, 11, 14, 18, 23, 35, 64 };

const Ipp32s tabBin_Band20[] = { 1,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,
  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

const Ipp32s tabPostBinMap_Band20[] = {-1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/* BAND34 */
const Ipp32s tabBin_Band34[] = { 0,  1,  2,  3, 4, 5, 6, 6, 7,   2,  1, 0,
  10, 10,  4,  5, 6, 7, 8, 9,10,  11, 12, 9,
  14, 11, 12, 13,14,15,16,13,16,  17, 18,19,
  20, 21, 22, 23,24,25,26,27,28,  29, 30,31,
  32,33};

const Ipp32s tabStartBord_Band34[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  11,12,13,14,15,16,17,18,19,20, 21,
  22,23,24,25,26,27,28,29,30,31,  5,
  6, 7, 8, 9,10,11,13,15,17,19, 21,
  24, 27, 30, 33, 37, 41 };

const Ipp32s tabStopBord_Band34[]  = {  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  11,12,13,14,15,16,17,18,19, 20,
  21,22,23,24,25,26,27,28,29, 30,
  31,32,6,7,8,9,10,11,13,15,17,
  19,21,24,27,30,33,37,41,64 };

const Ipp32s tabPostBinMap_Band34[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1 };

const sTDSpec setConfBand20 = {22, 20, 3, 10,  10, 23,
                               (Ipp32s*)tabStartBord_Band20,
                               (Ipp32s*)tabStopBord_Band20,
                               (Ipp32s*)tabBin_Band20,
                               (Ipp32s*)tabPostBinMap_Band20};

const sTDSpec setConfBand34 = {50, 34, 5, 32, 32, 23,
                               (Ipp32s*)tabStartBord_Band34,
                               (Ipp32s*)tabStopBord_Band34,
                               (Ipp32s*)tabBin_Band34,
                               (Ipp32s*)tabPostBinMap_Band34};

/********************************************************************/
/* EOF */

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

