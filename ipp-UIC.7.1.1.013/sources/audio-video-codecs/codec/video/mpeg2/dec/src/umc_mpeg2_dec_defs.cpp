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
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER

#include "umc_mpeg2_dec_defs.h"

namespace UMC
{

Ipp16s reset_dc[4] = {128, 256, 512, 1024};

Ipp16s intra_dc_multi[4] = {3, 2, 1, 0};

Ipp16s q_scale[2][32] =
{
  {
    0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
   32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62
  },
  {
    0,  1,  2,  3,  4,  5,  6,  7,  8, 10, 12, 14, 16, 18, 20, 22,
   24, 28, 32, 36, 40, 44, 48, 52, 56, 64, 72, 80, 88, 96,104,112
  }
};

}

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
