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
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include <math.h>

#include "ippdefs.h"

//--------------------------------------------------------

static
Ipp32s ippsAnalysisFilter_Q2_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12])
{
  int i;
  const Ipp32f c1 = 0.01899487526049f;
  const Ipp32f c3 = -0.07293139167538f;
  const Ipp32f c5 = 0.30596630545168f;

  for (i = 0; i < 32; i++){

    Ipp32f r1 = c1 * (pSrc[1+i].re + pSrc[11+i].re);
    Ipp32f r3 = c3 * (pSrc[3+i].re + pSrc[9+i].re);
    Ipp32f r5 = c5 * (pSrc[5+i].re + pSrc[7+i].re);

    Ipp32f r6 = 0.5f * pSrc[6+i].re;

    Ipp32f i1 = c1 * (pSrc[1+i].im + pSrc[11+i].im);
    Ipp32f i3 = c3 * (pSrc[3+i].im + pSrc[9+i].im);
    Ipp32f i5 = c5 * (pSrc[5+i].im + pSrc[7+i].im);

    Ipp32f i6 = 0.5f * pSrc[6+i].im;

    /* q = 0 */
    ppDst[i][0].re = r1 + r3 + r5 + r6;
    ppDst[i][0].im = i1 + i3 + i5 + i6;

    /* q = 1 */
    //ppDst[i][1].re = - (r1 + r3 + r5 - r6);
    ppDst[i][1].re = - ppDst[i][0].re + 2.f * r6;
    //ppDst[i][1].im = - (i1 + i3 + i5 - i6);
    ppDst[i][1].im = - ppDst[i][0].im + 2.f * i6;
  }

  return 0;//OK
}

//--------------------------------------------------------

static
Ipp32s ippsAnalysisFilter_Q4_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12])
{
  const Ipp32f c = 0.70710678118655f;
  Ipp32f sum1, sum2, sum3;
  const Ipp32f tabQ4[7] = { -0.05908211155639f, -0.04871498374946f, 0.f, 0.07778723915851f,
    0.16486303567403f,  0.23279856662996f, 0.25f};
  Ipp32f bufW[8];
  Ipp32s i;

  for (i = 0; i < 32; i++) {

    bufW[0] = -tabQ4[2] * (pSrc[i+2].re + pSrc[i+10].re) + tabQ4[6] * pSrc[i+6].re;

    sum1 = tabQ4[1] * (pSrc[i+1].re + pSrc[i+11].re);
    sum2 = tabQ4[3] * (pSrc[i+3].re + pSrc[i+9].re);
    sum3 = tabQ4[5] * (pSrc[i+5].re + pSrc[i+7].re);
    bufW[1] = -c * ( sum1 + sum2 - sum3 );

    sum2 = tabQ4[0] * (pSrc[i+0].im - pSrc[i+12].im);
    sum3 = tabQ4[4] * (pSrc[i+4].im - pSrc[i+ 8].im);
    bufW[2] = sum2 - sum3;

    sum1 = tabQ4[1] * (pSrc[i+1].im - pSrc[i+11].im);
    sum2 = tabQ4[3] * (pSrc[i+3].im - pSrc[i+ 9].im);
    sum3 = tabQ4[5] * (pSrc[i+5].im - pSrc[i+ 7].im);
    bufW[3] = c * (sum1 - sum2 - sum3);

    sum2 = tabQ4[0] * (pSrc[i+0].re - pSrc[i+12].re);
    sum3 = tabQ4[4] * (pSrc[i+4].re - pSrc[i+ 8].re);
    bufW[4] = sum2 - sum3;

    sum1 = tabQ4[1] * ( pSrc[i+1].re - pSrc[i+11].re );
    sum2 = tabQ4[3] * ( pSrc[i+3].re - pSrc[i+9].re );
    sum3 = tabQ4[5] * ( pSrc[i+5].re - pSrc[i+7].re );
    bufW[5] = c * ( sum1 - sum2 - sum3 );

    sum2 = -tabQ4[2] * (pSrc[i+2].im + pSrc[i+10].im);
    sum3 =  tabQ4[6] * pSrc[i+6].im;
    bufW[6] = sum2 + sum3;

    sum1 = tabQ4[1] * (pSrc[i+1].im + pSrc[i+11].im);
    sum2 = tabQ4[3] * (pSrc[i+3].im + pSrc[i+ 9].im);
    sum3 = tabQ4[5] * (pSrc[i+5].im + pSrc[i+ 7].im);
    bufW[7] = -c * (sum1 + sum2 - sum3);

    /* q0 */
    ppDst[i][0].re =  bufW[0] + bufW[1] + bufW[2] + bufW[3];
    ppDst[i][0].im = -bufW[4] - bufW[5] + bufW[6] + bufW[7];

    /* q1 */
    ppDst[i][1].re =  bufW[0] - bufW[1] - bufW[2] + bufW[3];
    ppDst[i][1].im =  bufW[4] - bufW[5] + bufW[6] - bufW[7];

    /* q2 */
    ppDst[i][2].re =  bufW[0] - bufW[1] + bufW[2] - bufW[3];
    ppDst[i][2].im = -bufW[4] + bufW[5] + bufW[6] - bufW[7];

    /* q3 */
    ppDst[i][3].re =  bufW[0] + bufW[1] - bufW[2] - bufW[3];
    ppDst[i][3].im =  bufW[4] + bufW[5] + bufW[6] + bufW[7];
  }

  return 0; //OK
}

//--------------------------------------------------------
//--------------------------------------------------------

static void ownDCT3_4_32f_I(Ipp32f* pSrc)
{
  const Ipp32f c0 = 0.7071067811865476f;
  const Ipp32f c4 = 1.3065629648763766f;
  const Ipp32f c5 = -0.9238795325112866f;
  const Ipp32f c6 = -0.5411961001461967f;
  Ipp32f bufW[9];

  /* direct */
  bufW[0] = pSrc[2] * c0;
  bufW[1] = pSrc[0] - bufW[0];
  bufW[2] = pSrc[0] + bufW[0];
  bufW[3] = pSrc[1] + pSrc[3];
  bufW[4] = pSrc[1] * c4;
  bufW[5] = bufW[3] * c5;
  bufW[6] = pSrc[3] * c6;
  bufW[7] = bufW[4] + bufW[5];
  bufW[8] = bufW[6] - bufW[5];

  /* reordering */
  pSrc[0] = bufW[2] + bufW[8];
  pSrc[1] = bufW[1] + bufW[7];
  pSrc[3] = bufW[2] - bufW[8];
  pSrc[2] = bufW[1] - bufW[7];

  return;
}

//--------------------------------------------------------

#ifndef NULL
#define NULL 0
#endif

static
Ipp32s ippsAnalysisFilter_Q8_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12], int flag34)
{

  Ipp32s i, j;
  Ipp32f bufW[16], bufZ[4];
  Ipp32f sum1, sum2;

  const Ipp32f tabQ8B20[7] = { 0.00746082949812f, 0.02270420949825f, 0.04546865930473f,
    0.07266113929591f, 0.09885108575264f, 0.11793710567217f, 0.125f};

  const Ipp32f tabQ8B34[7] = { 0.01565675600122f, 0.03752716391991f, 0.05417891378782f,
    0.08417044116767f, 0.10307344158036f, 0.12222452249753f, 0.125f};

  Ipp32f* pTab = NULL;

  pTab = ( flag34 ) ? (Ipp32f*)tabQ8B34 : (Ipp32f*)tabQ8B20;

  for (i = 0; i < 32; i++){

    bufW[0] =  pTab[6] * pSrc[6+i].re;
    bufW[1] =  pTab[5] * (pSrc[5+i].re + pSrc[7+i].re);

    sum1 = -pTab[0] * (pSrc[0+i].re + pSrc[12+i].re);
    sum2 =  pTab[4] * (pSrc[4+i].re + pSrc[8+i].re);
    bufW[2] = sum1 + sum2;

    sum1 = -pTab[1] * (pSrc[1+i].re + pSrc[11+i].re);
    sum2 =  pTab[3] * (pSrc[3+i].re + pSrc[9+i].re);
    bufW[3] = sum1 + sum2;

    bufW[4] = pTab[5] * (pSrc[7+i].im - pSrc[5+i].im);

    sum1 = pTab[0] * (pSrc[12+i].im - pSrc[0+i].im);
    sum2 = pTab[4] * (pSrc[8+i].im - pSrc[4+i].im);
    bufW[5] = sum1 + sum2;

    sum1 = pTab[1] * (pSrc[11+i].im - pSrc[1+i].im);
    sum2 = pTab[3] * (pSrc[9+i].im - pSrc[3+i].im);
    bufW[6] = sum1 + sum2;

    bufW[7] = pTab[2] * (pSrc[10+i].im - pSrc[2+i].im);

    for (j = 0; j < 4; j++) {
      bufZ[j] = bufW[j] - bufW[7 - j];
    }
    ownDCT3_4_32f_I(bufZ);

    ppDst[i][7].re = bufZ[0];
    ppDst[i][5].re = bufZ[2];
    ppDst[i][3].re = bufZ[3];
    ppDst[i][1].re = bufZ[1];

    for (j = 0; j < 4; j++) {
      bufZ[j] = bufW[j] + bufW[7 - j];
    }
    ownDCT3_4_32f_I(bufZ);

    ppDst[i][6].re = bufZ[1];
    ppDst[i][4].re = bufZ[3];
    ppDst[i][2].re = bufZ[2];
    ppDst[i][0].re = bufZ[0];

    bufW[12] =  pTab[6] * pSrc[6+i].im;
    bufW[13] =  pTab[5] * (pSrc[5+i].im + pSrc[7+i].im);

    sum1 = -pTab[0] * (pSrc[0+i].im + pSrc[12+i].im);
    sum2 =  pTab[4] * (pSrc[4+i].im + pSrc[8+i].im);
    bufW[14] = sum1 + sum2;

    sum1 = -pTab[1] * (pSrc[1+i].im + pSrc[11+i].im);
    sum2 =  pTab[3] * (pSrc[3+i].im + pSrc[9+i].im);
    bufW[15] = sum1 + sum2;

    bufW[8] = pTab[5] * (pSrc[7+i].re - pSrc[5+i].re);

    sum1 = pTab[0] * (pSrc[12+i].re - pSrc[0+i].re);
    sum2 = pTab[4] * (pSrc[8+i].re - pSrc[4+i].re);
    bufW[9] =  sum1 + sum2;

    sum1 = pTab[1] * (pSrc[11+i].re - pSrc[1+i].re);
    sum2 = pTab[3] * (pSrc[9+i].re -  pSrc[3+i].re);
    bufW[10] = sum1 + sum2;

    bufW[11] = pTab[2] * (pSrc[10+i].re - pSrc[2+i].re);

    for (j = 0; j < 4; j++) {
      bufZ[j] = bufW[12 + j] + bufW[11 - j];
    }
    ownDCT3_4_32f_I(bufZ);

    ppDst[i][7].im = bufZ[0];
    ppDst[i][5].im = bufZ[2];
    ppDst[i][3].im = bufZ[3];
    ppDst[i][1].im = bufZ[1];

    for (j = 0; j < 4; j++){
      bufZ[j] = bufW[12 + j] - bufW[11 - j];
    }
    ownDCT3_4_32f_I(bufZ);

    ppDst[i][6].im = bufZ[1];
    ppDst[i][4].im = bufZ[3];
    ppDst[i][2].im = bufZ[2];
    ppDst[i][0].im = bufZ[0];
  }

  return 0;//OK
}

//--------------------------------------------------------
//--------------------------------------------------------

static void ownDCT3_6_32f_I(Ipp32f* pSrc)
{
  Ipp32f bufW[8];
  const Ipp32f c0  = 0.70710678118655f;
  const Ipp32f c6a = 0.96592582628907f;
  const Ipp32f c6b = 0.25881904510252f;
  const Ipp32f c2  = 0.86602540378444f;

  bufW[0] = pSrc[3] * c0;
  bufW[1] = pSrc[0] + bufW[0];
  bufW[2] = pSrc[0] - bufW[0];
  bufW[3] = (pSrc[1] - pSrc[5]) * c0;
  bufW[4] = pSrc[2] * c2 + pSrc[4] * 0.5f;
  bufW[5] = bufW[4] - pSrc[4];
  bufW[6] = pSrc[1] * c6a + pSrc[5] * c6b;
  bufW[7] = bufW[6] - bufW[3];

  pSrc[0] = bufW[1] + bufW[4] + bufW[6];
  pSrc[1] = bufW[2] + bufW[3] - pSrc[4];
  pSrc[2] = bufW[2] - bufW[5] + bufW[7];
  pSrc[3] = bufW[1] - bufW[5] - bufW[7];
  pSrc[4] = bufW[1] - bufW[3] - pSrc[4];
  pSrc[5] = bufW[2] + bufW[4] - bufW[6];

  return;
}

static
Ipp32s ippsAnalysisFilter_Q12_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12])
{
  int i, n;
  Ipp32f bufW[24];

  const Ipp32f tabQ12[7] = { 0.04081179924692f, 0.03812810994926f, 0.05144908135699f,
                             0.06399831151592f, 0.07428313801106f, 0.08100347892914f,
                             0.08333333333333f };

  for (i = 0; i < 32; i++){
    // n == 0
    bufW[0]  = pSrc[6+i].re * tabQ12[6];
    bufW[6]  = (pSrc[i].im - pSrc[12+i].im) * tabQ12[0];
    bufW[12] = pSrc[6+i].im * tabQ12[6];
    bufW[18] = (pSrc[i].re - pSrc[12+i].re) * tabQ12[0];

    for (n = 1; n < 6; n++) {
      bufW[6+n]  = (pSrc[n+i].im - pSrc[12-n+i].im) * tabQ12[n];
      bufW[6-n]  = (pSrc[n+i].re + pSrc[12-n+i].re) * tabQ12[n];
      bufW[18+n] = (pSrc[n+i].re - pSrc[12-n+i].re) * tabQ12[n];
      bufW[18-n] = (pSrc[n+i].im + pSrc[12-n+i].im) * tabQ12[n];
    }

    ownDCT3_6_32f_I(bufW);
    ownDCT3_6_32f_I(bufW+6);
    ownDCT3_6_32f_I(bufW+12);
    ownDCT3_6_32f_I(bufW+18);

    for (n = 0; n < 6; n += 2){
      ppDst[i][n].re   = bufW[n]    - bufW[6+n];
      ppDst[i][n].im   = bufW[12+n] + bufW[18+n];
      ppDst[i][n+1].re = bufW[1+n]  + bufW[7+n];
      ppDst[i][n+1].im = bufW[13+n] - bufW[19+n];

      ppDst[i][10-n].re = bufW[n+1]  - bufW[7+n];
      ppDst[i][10-n].im = bufW[13+n] + bufW[19+n];
      ppDst[i][11-n].re = bufW[n]    + bufW[6+n];
      ppDst[i][11-n].im = bufW[12+n] - bufW[18+n];
    }
  }

  return 0;//OK
}

//--------------------------------------------------------
//--------------------------------------------------------

Ipp32s ownAnalysisFilter_PSDec_Kernel_v2_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12], int len, int flag34)
{
  switch( len ){

    case 2:
      ippsAnalysisFilter_Q2_32fc(pSrc, ppDst);
      break;

    case 4:
      ippsAnalysisFilter_Q4_32fc(pSrc, ppDst);
      break;

    case 8:
      ippsAnalysisFilter_Q8_32fc(pSrc, ppDst, flag34);
      break;

    default: //12
      ippsAnalysisFilter_Q12_32fc(pSrc, ppDst);
  }

  return 0;
}

//--------------------------------------------------------
//--------------------------------------------------------
/* EOF */
#endif
