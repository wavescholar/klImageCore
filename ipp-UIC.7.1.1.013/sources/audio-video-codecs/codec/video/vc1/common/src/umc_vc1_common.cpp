/* ///////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) common
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common.h"

namespace UMC
{
    namespace VC1Common
    {

        void SwapData(Ipp8u* src, Ipp32u dataSize)
        {
            Ipp32u  i;
            Ipp32u  counter = 0;
            Ipp32u* pDst = (Ipp32u*)src;
            Ipp32u  iCur = 0;

            for(i = 0; i < dataSize+4; i++)
            {
                if(4 == counter)
                {
                    counter = 0;
                    *pDst = iCur;
                    pDst++;
                    iCur = 0;
                }

                if(0 == counter)
                    iCur = src[i];

                iCur <<= 8;
                iCur |= src[i];
                ++counter;
            }
        }

    }
}

#endif // UMC_ENABLE_VC1_VIDEO_DECODER
