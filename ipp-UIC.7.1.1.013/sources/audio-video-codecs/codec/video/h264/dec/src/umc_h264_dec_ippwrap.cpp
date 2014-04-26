/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_dec_ippwrap.h"

namespace UMC
{
    IppStatus FilterDeblockingLuma_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingLuma_HorEdge_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha,
                                                             pDeblockInfo->pBeta,
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }

    IppStatus FilterDeblockingLuma_VerEdge_MBAFF(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingLuma_VerEdge_MBAFF_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha[0],
                                                             pDeblockInfo->pBeta[0],
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }

    IppStatus FilterDeblockingLuma_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingLuma_VerEdge_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha,
                                                             pDeblockInfo->pBeta,
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }

    IppStatus FilterDeblockingChroma_VerEdge_MBAFF(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingChroma_VerEdge_MBAFF_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha[0],
                                                             pDeblockInfo->pBeta[0],
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }

    IppStatus FilterDeblockingChroma_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingChroma_VerEdge_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha,
                                                             pDeblockInfo->pBeta,
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }

    IppStatus FilterDeblockingChroma_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingChroma_HorEdge_H264_8u_C1IR(pDeblockInfo->pSrcDstPlane,
                                                             pDeblockInfo->srcDstStep,
                                                             pDeblockInfo->pAlpha,
                                                             pDeblockInfo->pBeta,
                                                             pDeblockInfo->pThresholds,
                                                             pDeblockInfo->pBs);
    }


    IppStatus FilterDeblockingChroma_VerEdge_NV12(const IppiFilterDeblock_8u * )
    {
        return ippStsNoErr;//ippiFilterDeblockingChroma_VerEdge_H264_8u_C2I(pDeblockInfo);
    }

    IppStatus FilterDeblockingChroma_HorEdge_NV12(const IppiFilterDeblock_8u * )
    {
        return ippStsNoErr;//ippiFilterDeblockingChroma_HorEdge_H264_8u_C2I(pDeblockInfo);
    }

    IppStatus FilterDeblockingChroma_VerEdge_MBAFF_NV12(const IppiFilterDeblock_8u * )
    {
        return ippStsNoErr;//ippiFilterDeblockingChroma_NV12_VerEdge_MBAFF_H264_8u_C1IR(pDeblockInfo);
    }

    IppStatus FilterDeblockingChroma422_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingChroma422VerEdge_H264_8u_C1IR(pDeblockInfo);
    }

    IppStatus FilterDeblockingChroma422_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo)
    {
        return ippiFilterDeblockingChroma422HorEdge_H264_8u_C1IR(pDeblockInfo);
    }

    IppStatus FilterDeblockingChroma444_VerEdge(const IppiFilterDeblock_8u * )
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }

    IppStatus FilterDeblockingChroma444_HorEdge(const IppiFilterDeblock_8u * )
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }


    ///****************************************************************************************/
    // 16 bits functions
    ///****************************************************************************************/
#define CONVERT_TO_16U(size_alpha, size_clipping)   \
        Ipp32u i;\
        Ipp32s bitDepthScale = 1 << (bit_depth - 8);\
        \
        Ipp16u pAlpha_16u[size_alpha];\
        Ipp16u pBeta_16u[size_alpha];\
        Ipp16u pThresholds_16u[size_clipping];\
        IppiFilterDeblock_16u info;\
        info.pSrcDstPlane = pSrcDst;\
        info.srcDstStep = srcdstStep;\
        info.pAlpha = pAlpha_16u;\
        info.pBeta = pBeta_16u;\
        info.pThresholds = pThresholds_16u;\
        info.pBs = pBS;\
        info.bitDepth = bit_depth;\
\
        for (i = 0; i < sizeof(pAlpha_16u)/sizeof(pAlpha_16u[0]); i++)\
        {\
            pAlpha_16u[i]   = (Ipp16u)(pAlpha[i]*bitDepthScale);\
            pBeta_16u[i]    = (Ipp16u)(pBeta[i]*bitDepthScale);\
        }\
\
        for (i = 0; i < sizeof(pThresholds_16u)/sizeof(pThresholds_16u[0]); i++)\
        {\
            pThresholds_16u[i] = (Ipp16u)(pThresholds[i]*bitDepthScale);\
        }

    IppStatus FilterDeblockingLuma_HorEdge(Ipp16u* pSrcDst,
                                           Ipp32s  srcdstStep,
                                           Ipp8u*  pAlpha,
                                           Ipp8u*  pBeta,
                                           Ipp8u*  pThresholds,
                                           Ipp8u*  pBS,
                                           Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 16);
        return ippiFilterDeblockingLumaHorEdge_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingLuma_VerEdge_MBAFF(Ipp16u* pSrcDst,
                                                 Ipp32s  srcdstStep,
                                                 Ipp8u*  pAlpha,
                                                 Ipp8u*  pBeta,
                                                 Ipp8u*  pThresholds,
                                                 Ipp8u*  pBS,
                                                 Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(1, 4);
        return ippiFilterDeblockingLumaVerEdgeMBAFF_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingLuma_VerEdge(Ipp16u* pSrcDst,
                                           Ipp32s  srcdstStep,
                                           Ipp8u*  pAlpha,
                                           Ipp8u*  pBeta,
                                           Ipp8u*  pThresholds,
                                           Ipp8u*  pBS,
                                           Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 16);
        return ippiFilterDeblockingLumaVerEdge_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingChroma_VerEdge_MBAFF(Ipp16u* pSrcDst,
                                                   Ipp32s  srcdstStep,
                                                   Ipp8u*  pAlpha,
                                                   Ipp8u*  pBeta,
                                                   Ipp8u*  pThresholds,
                                                   Ipp8u*  pBS,
                                                   Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(1, 4);
        return ippiFilterDeblockingChromaVerEdgeMBAFF_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingChroma_VerEdge(Ipp16u* pSrcDst,
                                             Ipp32s  srcdstStep,
                                             Ipp8u*  pAlpha,
                                             Ipp8u*  pBeta,
                                             Ipp8u*  pThresholds,
                                             Ipp8u*  pBS,
                                             Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 8);
        return ippiFilterDeblockingChromaVerEdge_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingChroma_HorEdge(Ipp16u* pSrcDst,
                                             Ipp32s  srcdstStep,
                                             Ipp8u*  pAlpha,
                                             Ipp8u*  pBeta,
                                             Ipp8u*  pThresholds,
                                             Ipp8u*  pBS,
                                             Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 8);
        return ippiFilterDeblockingChromaHorEdge_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingChroma422_VerEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 8);
        return ippiFilterDeblockingChroma422VerEdge_H264_16u_C1IR(&info);
    }

    IppStatus FilterDeblockingChroma422_HorEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth)
    {
        CONVERT_TO_16U(2, 16);
        return ippiFilterDeblockingChroma422HorEdge_H264_16u_C1IR(&info);
    }


    IppStatus FilterDeblockingChroma444_VerEdge(Ipp16u* ,
                                                Ipp32s  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp32s  )
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }

    IppStatus FilterDeblockingChroma444_HorEdge(Ipp16u* ,
                                                Ipp32s  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp8u*  ,
                                                Ipp32s  )
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }


} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
