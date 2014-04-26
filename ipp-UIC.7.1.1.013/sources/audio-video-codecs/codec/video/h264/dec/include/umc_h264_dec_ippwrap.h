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

#ifndef __UMC_H264_DEC_IPP_WRAP_H
#define __UMC_H264_DEC_IPP_WRAP_H

#include "umc_h264_dec_defs_dec.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippvc.h"

struct VCBidir1_8u
{
    const Ipp8u * pSrc[2];
    Ipp32s   srcStep[2];
    Ipp8u*   pDst;
    Ipp32s   dstStep;
    IppiSize roiSize;
};

struct VCBidir1_16u
{
    const Ipp16u * pSrc[2];
    Ipp32s   srcStep[2];
    Ipp16u*   pDst;
    Ipp32s   dstStep;
    IppiSize roiSize;
    Ipp32s   bitDepth;
};

namespace UMC
{
#if defined(_MSC_VER)
#pragma warning(disable: 4100)
#endif

    inline IppStatus SetPlane(Ipp8u val, Ipp8u* pDst, Ipp32s len)
    {
        if (!pDst)
            return ippStsNullPtrErr;

        return ippsSet_8u(val, pDst, len);
    }

    inline IppStatus CopyPlane(const Ipp8u *pSrc, Ipp8u *pDst, Ipp32s len)
    {
        if (!pSrc || !pDst)
            return ippStsNullPtrErr;

        return ippsCopy_8u(pSrc, pDst, len);
    }

    inline  IppStatus ippiInterpolateLuma(const Ipp8u *, const IppVCInterpolateBlock_16u *interpolateInfo_)
    {
        const IppVCInterpolateBlock_8u * interpolateInfo = (const IppVCInterpolateBlock_8u *) interpolateInfo_;
        return ippiInterpolateLuma_H264_8u_C1R(
            interpolateInfo->pSrc[0], interpolateInfo->srcStep, interpolateInfo->pDst[0], interpolateInfo->dstStep,
            interpolateInfo->pointVector.x, interpolateInfo->pointVector.y, interpolateInfo->sizeBlock);
    }

    inline IppStatus ippiInterpolateLumaBlock(const Ipp8u *, const IppVCInterpolateBlock_16u *interpolateInfo)
    {
        return ippiInterpolateLumaBlock_H264_8u_P1R((const IppVCInterpolateBlock_8u *)interpolateInfo);
    }

    inline IppStatus ippiInterpolateChromaBlock(const Ipp8u *, const IppVCInterpolateBlock_16u *interpolateInfo)
    {
        return ippiInterpolateChromaBlock_H264_8u_P2R((const IppVCInterpolateBlock_8u *)interpolateInfo);
    }

    inline IppStatus MyDecodeCAVLCChromaDcCoeffs_H264(Ipp32u **ppBitStream,
                                                    Ipp32s *pOffset,
                                                    Ipp16s *pNumCoeff,
                                                    Ipp16s **ppDstCoeffs,
                                                    const Ipp32s *tblCoeffToken,
                                                    const Ipp32s **tblTotalZerosCR,
                                                    const Ipp32s **tblRunBefore)
    {
        return ippiDecodeCAVLCChromaDcCoeffs_H264_1u16s(ppBitStream,
                                                             pOffset,
                                                             pNumCoeff,
                                                             ppDstCoeffs,
                                                             tblCoeffToken,
                                                             tblTotalZerosCR,
                                                             tblRunBefore);
    }

    inline IppStatus DecodeCAVLCChromaDcCoeffs422_H264(Ipp32u **ppBitStream,
                                                       Ipp32s *pOffset,
                                                       Ipp16s *pNumCoeff,
                                                       Ipp16s **ppDstCoeffs,
                                                       const Ipp32s *pTblCoeffToken,
                                                       const Ipp32s **ppTblTotalZerosCR,
                                                       const Ipp32s **ppTblRunBefore)
    {
        return ippiDecodeCAVLCChroma422DcCoeffs_H264_1u16s(ppBitStream,
                                                            pOffset,
                                                            pNumCoeff,
                                                            ppDstCoeffs,
                                                            pTblCoeffToken,
                                                            ppTblTotalZerosCR,
                                                            ppTblRunBefore);
    }

    inline IppStatus DecodeCAVLCCoeffs_H264(Ipp32u **ppBitStream,
                                            Ipp32s *pOffset,
                                            Ipp16s *pNumCoeff,
                                            Ipp16s **ppDstCoeffs,
                                            Ipp32u uVLCSelect,
                                            Ipp16s uMaxNumCoeff,
                                            const Ipp32s **ppTblCoeffToken,
                                            const Ipp32s **ppTblTotalZeros,
                                            const Ipp32s **ppTblRunBefore,
                                            const Ipp32s *pScanMatrix)
    {
        return ippiDecodeCAVLCCoeffs_H264_1u16s(ppBitStream,
                                                pOffset,
                                                pNumCoeff,
                                                ppDstCoeffs,
                                                uVLCSelect,
                                                uMaxNumCoeff,
                                                ppTblCoeffToken,
                                                ppTblTotalZeros,
                                                ppTblRunBefore,
                                                pScanMatrix);
    }

    inline IppStatus MyDecodeCAVLCCoeffs_H264(Ipp32u **ppBitStream,
                                            Ipp32s *pOffset,
                                            Ipp16s *pNumCoeff,
                                            Ipp16s **ppDstCoeffs,
                                            Ipp32u uVLCSelect,
                                            Ipp16s uMaxNumCoeff,
                                            const Ipp32s **tblCoeffToken,
                                            const Ipp32s **tblTotalZeros,
                                            const Ipp32s **tblRunBefore,
                                            const Ipp32s *pScanMatrix)
    {
        return ippiDecodeCAVLCCoeffs_H264_1u16s(ppBitStream,
                                                         pOffset,
                                                         pNumCoeff,
                                                         ppDstCoeffs,
                                                         uVLCSelect,
                                                         uMaxNumCoeff,
                                                         tblCoeffToken,
                                                         tblTotalZeros,
                                                         tblRunBefore,
                                                         pScanMatrix);
    }

    inline IppStatus SetPlane(Ipp8u value, Ipp8u* pDst, Ipp32s dstStep,
                              IppiSize roiSize )
    {
        return ippiSet_8u_C1R(value, pDst, dstStep, roiSize);
    }

    inline IppStatus CopyPlane(Ipp8u* pSrc, Ipp32s srcStep, Ipp8u* pDst, Ipp32s dstStep, IppiSize roiSize )
    {
        return ippiCopy_8u_C1R(pSrc, srcStep, pDst, dstStep, roiSize);
    }

    inline IppStatus ExpandPlane(Ipp8u *StartPtr,
                                 Ipp32u uFrameWidth,
                                 Ipp32u uFrameHeight,
                                 Ipp32u uPitch,
                                 Ipp32u uPels,
                                 IppvcFrameFieldFlag uFrameFieldFlag)
    {
        return ippiExpandPlane_H264_8u_C1R(StartPtr, uFrameWidth, uFrameHeight, uPitch, uPels, uFrameFieldFlag);
    }

    inline IppStatus BiDirWeightBlock(Ipp8u * , VCBidir1_16u * info_,
                                      Ipp32u ulog2wd,
                                      Ipp32s iWeight1,
                                      Ipp32s iOffset1,
                                      Ipp32s iWeight2,
                                      Ipp32s iOffset2)
    {
        VCBidir1_8u * info = (VCBidir1_8u *)info_;
        return ippiBiDirWeightBlock_H264_8u_P3P1R(info->pSrc[0], info->pSrc[1], info->pDst,
            info->srcStep[0], info->srcStep[1], info->dstStep, ulog2wd, iWeight1, iOffset1, iWeight2, iOffset2, info->roiSize);
    }

    inline IppStatus BiDirWeightBlockImplicit(Ipp8u * , VCBidir1_16u * info_,
                                              Ipp32s iWeight1,
                                              Ipp32s iWeight2)
    {
        VCBidir1_8u * info = (VCBidir1_8u *)info_;
        return ippiBiDirWeightBlockImplicit_H264_8u_P3P1R(info->pSrc[0], info->pSrc[1], info->pDst,
            info->srcStep[0], info->srcStep[1], info->dstStep, iWeight1, iWeight2, info->roiSize);
    }

    inline IppStatus InterpolateBlock(Ipp8u * , VCBidir1_16u * info_)
    {
        VCBidir1_8u * info = (VCBidir1_8u *)info_;
        return ippiInterpolateBlock_H264_8u_P3P1R(info->pSrc[0], info->pSrc[1], info->pDst, info->roiSize.width, info->roiSize.height,
            info->srcStep[0], info->srcStep[1], info->dstStep);
    }

    inline IppStatus UniDirWeightBlock(Ipp8u *, VCBidir1_16u * info_,
                                       Ipp32u ulog2wd,
                                       Ipp32s iWeight,
                                       Ipp32s iOffset)
    {
        VCBidir1_8u * info = (VCBidir1_8u *)info_;
        return ippiUniDirWeightBlock_H264_8u_C1R(info->pDst, info->dstStep, ulog2wd, iWeight, iOffset, info->roiSize);
    }

    inline IppStatus ReconstructLumaIntraHalfMB(Ipp16s **ppSrcCoeff,
                                                Ipp8u *pSrcDstYPlane,
                                                Ipp32s srcdstYStep,
                                                IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                Ipp32u cbp4x2,
                                                Ipp32s QP,
                                                Ipp8u edgeType,
                                                Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntraHalfMB_H264_16s8u_C1R(ppSrcCoeff,
                                                             pSrcDstYPlane,
                                                             srcdstYStep,
                                                             pMBIntraTypes,
                                                             cbp4x2,
                                                             QP,
                                                             edgeType);
    }

    inline IppStatus ReconstructLumaInter8x8MB(Ipp16s **ppSrcDstCoeff,
                                               Ipp8u *pSrcDstYPlane,
                                               Ipp32u srcdstYStep,
                                               Ipp32u cbp8x8,
                                               Ipp32s QP,
                                               Ipp16s *pQuantTable,
                                               Ipp8u bypass_flag,
                                               Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaInter8x8MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                            pSrcDstYPlane,
                                                            srcdstYStep,
                                                            cbp8x8,
                                                            QP,
                                                            pQuantTable,
                                                            bypass_flag);
    }

    inline IppStatus ReconstructLumaInter4x4MB(Ipp16s **ppSrcDstCoeff,
                                               Ipp8u *pSrcDstYPlane,
                                               Ipp32u srcdstYStep,
                                               Ipp32u cbp4x4,
                                               Ipp32s QP,
                                               Ipp16s *pQuantTable,
                                               Ipp8u bypass_flag,
                                               Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaInter4x4MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                            pSrcDstYPlane,
                                                            srcdstYStep,
                                                            cbp4x4,
                                                            QP,
                                                            pQuantTable,
                                                            bypass_flag);
    }


    inline IppStatus ReconstructLumaIntra_16x16MB(Ipp16s **ppSrcDstCoeff,
                                                  Ipp8u *pSrcDstYPlane,
                                                  Ipp32u srcdstYStep,
                                                  IppIntra16x16PredMode_H264 intra_luma_mode,
                                                  Ipp32u cbp4x4,
                                                  Ipp32s QP,
                                                  Ipp8u edge_type,
                                                  Ipp16s *pQuantTable,
                                                  Ipp8u bypass_flag,
                                                  Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntra_16x16MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                               pSrcDstYPlane,
                                                               srcdstYStep,
                                                               intra_luma_mode,
                                                               cbp4x4,
                                                               QP,
                                                               edge_type,
                                                               pQuantTable,
                                                               bypass_flag);
    }

    inline IppStatus ReconstructLumaIntraHalf8x8MB(Ipp16s **ppSrcDstCoeff,
                                                   Ipp8u *pSrcDstYPlane,
                                                   Ipp32s srcdstYStep,
                                                   IppIntra8x8PredMode_H264 *pMBIntraTypes,
                                                   Ipp32u cbp8x2,
                                                   Ipp32s QP,
                                                   Ipp8u edgeType,
                                                   Ipp16s *pQuantTable,
                                                   Ipp8u bypass_flag,
                                                   Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntraHalf8x8MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                                pSrcDstYPlane,
                                                                srcdstYStep,
                                                                pMBIntraTypes,
                                                                cbp8x2,
                                                                QP,
                                                                edgeType,
                                                                pQuantTable,
                                                                bypass_flag);
    }


    inline IppStatus ReconstructLumaIntraHalf4x4MB(Ipp16s **ppSrcDstCoeff,
                                                   Ipp8u *pSrcDstYPlane,
                                                   Ipp32s srcdstYStep,
                                                   IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                   Ipp32u cbp4x2,
                                                   Ipp32s QP,
                                                   Ipp8u edgeType,
                                                   Ipp16s *pQuantTable,
                                                   Ipp8u bypass_flag,
                                                   Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntraHalf4x4MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                                pSrcDstYPlane,
                                                                srcdstYStep,
                                                                pMBIntraTypes,
                                                                cbp4x2,
                                                                QP,
                                                                edgeType,
                                                                pQuantTable,
                                                                bypass_flag);
    }

    inline IppStatus ReconstructLumaIntra8x8MB(Ipp16s **ppSrcDstCoeff,
                                               Ipp8u *pSrcDstYPlane,
                                               Ipp32s srcdstYStep,
                                               IppIntra8x8PredMode_H264 *pMBIntraTypes,
                                               Ipp32u cbp8x8,
                                               Ipp32s QP,
                                               Ipp8u edgeType,
                                               Ipp16s *pQuantTable,
                                               Ipp8u bypass_flag,
                                               Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntra8x8MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                            pSrcDstYPlane,
                                                            srcdstYStep,
                                                            pMBIntraTypes,
                                                            cbp8x8,
                                                            QP,
                                                            edgeType,
                                                            pQuantTable,
                                                            bypass_flag);
    }

    inline  IppStatus ReconstructLumaIntra4x4MB(Ipp16s **ppSrcDstCoeff,
                                                Ipp8u *pSrcDstYPlane,
                                                Ipp32s srcdstYStep,
                                                IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                Ipp32u cbp4x4,
                                                Ipp32s QP,
                                                Ipp8u edgeType,
                                                Ipp16s *pQuantTable,
                                                Ipp8u bypass_flag,
                                                Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntra4x4MB_H264_16s8u_C1R(ppSrcDstCoeff,
                                                            pSrcDstYPlane,
                                                            srcdstYStep,
                                                            pMBIntraTypes,
                                                            cbp4x4,
                                                            QP,
                                                            edgeType,
                                                            pQuantTable,
                                                            bypass_flag);
    }

    inline IppStatus ReconstructLumaIntra16x16MB(Ipp16s **ppSrcCoeff,
                                                 Ipp8u *pSrcDstYPlane,
                                                 Ipp32u srcdstYStep,
                                                 const IppIntra16x16PredMode_H264 intra_luma_mode,
                                                 const Ipp32u cbp4x4,
                                                 const Ipp32s QP,
                                                 const Ipp8u edge_type,
                                                 Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntra16x16MB_H264_16s8u_C1R(ppSrcCoeff,
                                                              pSrcDstYPlane,
                                                              srcdstYStep,
                                                              intra_luma_mode,
                                                              cbp4x4,
                                                              QP,
                                                              edge_type);
    }

    inline IppStatus ReconstructLumaIntraMB(Ipp16s **ppSrcCoeff,
                                            Ipp8u *pSrcDstYPlane,
                                            Ipp32s srcdstYStep,
                                            const IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                            const Ipp32u cbp4x4,
                                            const Ipp32s QP,
                                            const Ipp8u edgeType,
                                            Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaIntraMB_H264_16s8u_C1R(ppSrcCoeff,
                                                         pSrcDstYPlane,
                                                         srcdstYStep,
                                                         pMBIntraTypes,
                                                         cbp4x4,
                                                         QP,
                                                         edgeType);
    }

    inline IppStatus ReconstructLumaInterMB(Ipp16s **ppSrcCoeff,
                                            Ipp8u *pSrcDstYPlane,
                                            Ipp32u srcdstYStep,
                                            Ipp32u cbp4x4,
                                            Ipp32s QP,
                                            Ipp32s bit_depth = 8)
    {
        return ippiReconstructLumaInterMB_H264_16s8u_C1R(ppSrcCoeff,
                                                         pSrcDstYPlane,
                                                         srcdstYStep,
                                                         cbp4x4,
                                                         QP);
    }

    inline IppStatus ReconstructChromaInter4x4MB(Ipp16s **ppSrcDstCoeff,
                                                 Ipp8u *pSrcDstUPlane,
                                                 Ipp8u *pSrcDstVPlane,
                                                 Ipp32u srcdstUVStep,
                                                 Ipp32u cbpU,
                                                 Ipp32u cbpV,
                                                 Ipp32u chromaQPU,
                                                 Ipp32u chromaQPV,
                                                 Ipp16s *pQuantTableU,
                                                 Ipp16s *pQuantTableV,
                                                 Ipp8u bypass_flag,
                                                 Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaInter4x4MB_H264_16s8u_P2R(ppSrcDstCoeff,
                                                              pSrcDstUPlane,
                                                              pSrcDstVPlane,
                                                              srcdstUVStep,
                                                              CreateIPPCBPMask420(cbpU, cbpV),
                                                              chromaQPU,
                                                              chromaQPV,
                                                              pQuantTableU,
                                                              pQuantTableV,
                                                              bypass_flag);
    }

    inline IppStatus ReconstructChromaInterMB(Ipp16s **ppSrcCoeff,
                                              Ipp8u *pSrcDstUPlane,
                                              Ipp8u *pSrcDstVPlane,
                                              const Ipp32u srcdstStep,
                                              Ipp32u cbpU,
                                              Ipp32u cbpV,
                                              const Ipp32u ChromaQP,
                                              Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaInterMB_H264_16s8u_P2R(ppSrcCoeff,
                                                           pSrcDstUPlane,
                                                           pSrcDstVPlane,
                                                           srcdstStep,
                                                           CreateIPPCBPMask420(cbpU, cbpV),
                                                           ChromaQP);
    }

    inline IppStatus ReconstructChromaIntra4x4MB(Ipp16s **ppSrcDstCoeff,
                                                 Ipp8u *pSrcDstUPlane,
                                                 Ipp8u *pSrcDstVPlane,
                                                 Ipp32u srcdstUVStep,
                                                 IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                 Ipp32u cbpU,
                                                 Ipp32u cbpV,
                                                 Ipp32u chromaQPU,
                                                 Ipp32u chromaQPV,
                                                 Ipp8u edge_type,
                                                 Ipp16s *pQuantTableU,
                                                 Ipp16s *pQuantTableV,
                                                 Ipp8u bypass_flag,
                                                 Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaIntra4x4MB_H264_16s8u_P2R(ppSrcDstCoeff,
                                                              pSrcDstUPlane,
                                                              pSrcDstVPlane,
                                                              srcdstUVStep,
                                                              intra_chroma_mode,
                                                              CreateIPPCBPMask420(cbpU, cbpV),
                                                              chromaQPU,
                                                              chromaQPV,
                                                              edge_type,
                                                              pQuantTableU,
                                                              pQuantTableV,
                                                              bypass_flag);
    }

    inline IppStatus ReconstructChromaIntraMB(Ipp16s **ppSrcCoeff,
                                              Ipp8u *pSrcDstUPlane,
                                              Ipp8u *pSrcDstVPlane,
                                              const Ipp32u srcdstUVStep,
                                              const IppIntraChromaPredMode_H264 intra_chroma_mode,
                                              Ipp32u cbpU,
                                              Ipp32u cbpV,
                                              const Ipp32u ChromaQP,
                                              const Ipp8u edge_type,
                                              Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaIntraMB_H264_16s8u_P2R(ppSrcCoeff,
                                                           pSrcDstUPlane,
                                                           pSrcDstVPlane,
                                                           srcdstUVStep,
                                                           intra_chroma_mode,
                                                           CreateIPPCBPMask420(cbpU, cbpV),
                                                           ChromaQP,
                                                           edge_type);
    }

    inline IppStatus ReconstructChromaIntraHalfs4x4MB(Ipp16s **ppSrcDstCoeff,
                                                      Ipp8u *pSrcDstUPlane,
                                                      Ipp8u *pSrcDstVPlane,
                                                      Ipp32u srcdstUVStep,
                                                      IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                      Ipp32u cbpU,
                                                      Ipp32u cbpV,
                                                      Ipp32u chromaQPU,
                                                      Ipp32u chromaQPV,
                                                      Ipp8u edge_type_top,
                                                      Ipp8u edge_type_bottom,
                                                      Ipp16s *pQuantTableU,
                                                      Ipp16s *pQuantTableV,
                                                      Ipp8u bypass_flag,
                                                      Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaIntraHalfs4x4MB_H264_16s8u_P2R(ppSrcDstCoeff,
                                                                   pSrcDstUPlane,
                                                                   pSrcDstVPlane,
                                                                   srcdstUVStep,
                                                                   intra_chroma_mode,
                                                                   CreateIPPCBPMask420(cbpU, cbpV),
                                                                   chromaQPU,
                                                                   chromaQPV,
                                                                   edge_type_top,
                                                                   edge_type_bottom,
                                                                   pQuantTableU,
                                                                   pQuantTableV,
                                                                   bypass_flag);
    }

    inline IppStatus ReconstructChromaIntraHalfsMB(Ipp16s **ppSrcCoeff,
                                                   Ipp8u *pSrcDstUPlane,
                                                   Ipp8u *pSrcDstVPlane,
                                                   Ipp32u srcdstUVStep,
                                                   IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                   Ipp32u cbpU,
                                                   Ipp32u cbpV,
                                                   Ipp32u ChromaQP,
                                                   Ipp8u edge_type_top,
                                                   Ipp8u edge_type_bottom,
                                                   Ipp32s bit_depth = 8)
    {
        return ippiReconstructChromaIntraHalfsMB_H264_16s8u_P2R(ppSrcCoeff,
                                                                pSrcDstUPlane,
                                                                pSrcDstVPlane,
                                                                srcdstUVStep,
                                                                intra_chroma_mode,
                                                                CreateIPPCBPMask420(cbpU, cbpV),
                                                                ChromaQP,
                                                                edge_type_top,
                                                                edge_type_bottom);
    }

#define FILL_CHROMA_RECONSTRUCT_INFO_8U \
    IppiReconstructHighMB_16s8u info_temp[2];\
    const IppiReconstructHighMB_16s8u *info[2];\
    info[0] = &info_temp[0];\
    info[1] = &info_temp[1];\
    info_temp[0].ppSrcDstCoeff = ppSrcDstCoeff;   \
    info_temp[0].pSrcDstPlane = pSrcDstUPlane;  \
    info_temp[0].srcDstStep = srcdstUVStep;    \
    info_temp[0].cbp = (Ipp32u)cbpU;          \
    info_temp[0].qp = chromaQPU;               \
    info_temp[0].pQuantTable = pQuantTableU;  \
    info_temp[0].bypassFlag = bypass_flag;      \
    info_temp[1].ppSrcDstCoeff = ppSrcDstCoeff;   \
    info_temp[1].pSrcDstPlane = pSrcDstVPlane;  \
    info_temp[1].srcDstStep = srcdstUVStep;    \
    info_temp[1].cbp = (Ipp32u)cbpV;          \
    info_temp[1].qp = chromaQPV;\
    info_temp[1].pQuantTable = pQuantTableV;\
    info_temp[1].bypassFlag = bypass_flag;

    inline IppStatus ReconstructChromaIntraHalfs4x4MB422(Ipp16s **ppSrcDstCoeff,
                                                         Ipp8u *pSrcDstUPlane,
                                                         Ipp8u *pSrcDstVPlane,
                                                         Ipp32u srcdstUVStep,
                                                         IppIntraChromaPredMode_H264 intraChromaMode,
                                                         Ipp32u cbpU,
                                                         Ipp32u cbpV,
                                                         Ipp32u chromaQPU,
                                                         Ipp32u chromaQPV,
                                                         Ipp32u levelScaleDCU,
                                                         Ipp32u levelScaleDCV,
                                                         Ipp8u edgeTypeTop,
                                                         Ipp8u edgeTypeBottom,
                                                         Ipp16s *pQuantTableU,
                                                         Ipp16s *pQuantTableV,
                                                         Ipp8u bypass_flag,
                                                         Ipp32s bit_depth = 8)
    {
        FILL_CHROMA_RECONSTRUCT_INFO_8U;
        return ippiReconstructChroma422IntraHalf4x4_H264High_16s8u_IP2R(info,
                                                                        intraChromaMode,
                                                                        edgeTypeTop,
                                                                        edgeTypeBottom,
                                                                        levelScaleDCU,
                                                                        levelScaleDCV);
    }

    inline IppStatus ReconstructChromaIntraHalfs4x4MB444(Ipp16s **ppSrcDstCoeff,
                                                         Ipp8u *pSrcDstUPlane,
                                                         Ipp8u *pSrcDstVPlane,
                                                         Ipp32u srcdstUVStep,
                                                         IppIntraChromaPredMode_H264 intraChromaMode,
                                                         Ipp32u cbpU,
                                                         Ipp32u cbpV,
                                                         Ipp32u chromaQPU,
                                                         Ipp32u chromaQPV,
                                                         Ipp8u edgeTypeTop,
                                                         Ipp8u edgeTypeBottom,
                                                         Ipp16s *pQuantTableU,
                                                         Ipp16s *pQuantTableV,
                                                         Ipp8u bypassFlag,
                                                         Ipp32s bit_depth = 8)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructChromaIntraHalfs4x4MB444_H264_16s8u_P2R_(ppSrcDstCoeff,
            pSrcDstUPlane,
            pSrcDstVPlane,
            srcdstUVStep,
            intraChromaMode,
            CreateIPPCBPMask444(cbpU, cbpV),
            chromaQPU,
            chromaQPV,
            edgeTypeTop,
            edgeTypeBottom,
            pQuantTableU,
            pQuantTableV,
            bypassFlag);*/
    }

   inline IppStatus ReconstructChromaInter4x4MB422(Ipp16s **ppSrcDstCoeff,
                                                   Ipp8u *pSrcDstUPlane,
                                                   Ipp8u *pSrcDstVPlane,
                                                   Ipp32u srcdstUVStep,
                                                   Ipp32u cbpU,
                                                   Ipp32u cbpV,
                                                   Ipp32u chromaQPU,
                                                   Ipp32u chromaQPV,
                                                   Ipp32u levelScaleDCU,
                                                   Ipp32u levelScaleDCV,
                                                   Ipp16s *pQuantTableU,
                                                   Ipp16s *pQuantTableV,
                                                   Ipp8u bypass_flag,
                                                   Ipp32s bit_depth = 8)
   {
       FILL_CHROMA_RECONSTRUCT_INFO_8U;
       return ippiReconstructChroma422Inter4x4_H264High_16s8u_IP2R(info,
                                                                   levelScaleDCU,
                                                                   levelScaleDCV);
   }

    inline IppStatus ReconstructChromaInter4x4MB444(Ipp16s **ppSrcDstCoeff,
                                                    Ipp8u *pSrcDstUPlane,
                                                    Ipp8u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u bypassFlag,
                                                    Ipp32s bit_depth = 8)
   {
       VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructChromaInter4x4MB444_H264_16s8u_P2R_(ppSrcDstCoeff,
            pSrcDstUPlane,
            pSrcDstVPlane,
            srcdstUVStep,
            CreateIPPCBPMask444(cbpU, cbpV),
            chromaQPU,
            chromaQPV,
            pQuantTableU,
            pQuantTableV,
            bypassFlag);*/
   }


    inline IppStatus ReconstructChromaIntra4x4MB422(Ipp16s **ppSrcDstCoeff,
                                                    Ipp8u *pSrcDstUPlane,
                                                    Ipp8u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    IppIntraChromaPredMode_H264 intraChromaMode,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp32u levelScaleDCU,
                                                    Ipp32u levelScaleDCV,
                                                    Ipp8u edgeType,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u bypass_flag,
                                                    Ipp32s bit_depth = 8)
   {
       FILL_CHROMA_RECONSTRUCT_INFO_8U;
       return ippiReconstructChroma422Intra4x4_H264High_16s8u_IP2R(info,
                                                                   intraChromaMode,
                                                                   edgeType,
                                                                   levelScaleDCU,
                                                                   levelScaleDCV);
   }


    inline IppStatus ReconstructChromaIntra4x4MB444(Ipp16s **ppSrcDstCoeff,
                                                    Ipp8u *pSrcDstUPlane,
                                                    Ipp8u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    IppIntraChromaPredMode_H264 intraChromaMode,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp8u edgeType,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u bypassFlag,
                                                    Ipp32s bit_depth = 8)
   {
       VM_ASSERT(false);
       return ippStsNoErr;/*ippiReconstructChromaIntra4x4MB444_H264_16s8u_P2R_(ppSrcDstCoeff,
            pSrcDstUPlane,
            pSrcDstVPlane,
            srcdstUVStep,
            intraChromaMode,
            CreateIPPCBPMask444(cbpU, cbpV),
            chromaQPU,
            chromaQPV,
            edgeType,
            pQuantTableU,
            pQuantTableV,
            bypassFlag);*/
   }

    IppStatus FilterDeblockingLuma_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingLuma_VerEdge_MBAFF(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingLuma_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma_VerEdge_MBAFF(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma422_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma422_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma444_VerEdge(const IppiFilterDeblock_8u * pDeblockInfo);

    IppStatus FilterDeblockingChroma444_HorEdge(const IppiFilterDeblock_8u * pDeblockInfo);


    ///****************************************************************************************/
    // 16 bits functions
    ///****************************************************************************************/
    inline void SetPlane(Ipp16u val, Ipp16u* pDst, Ipp32s len)
    {
        ippsSet_16s(val, (Ipp16s *)pDst, len);
    }

    inline void CopyPlane(const Ipp16u *pSrc, Ipp16u *pDst, Ipp32s len)
    {
        ippsCopy_8u((const Ipp8u *)pSrc, (Ipp8u *)pDst, len * sizeof(Ipp16s));
    }

    inline IppStatus SetPlane(Ipp16u value, Ipp16u* pDst, Ipp32s dstStep, IppiSize roiSize)
    {
        if (!pDst)
            return ippStsNullPtrErr;
        return ippiSet_16s_C1R(value, (Ipp16s*)pDst, dstStep, roiSize);
    }

    inline IppStatus CopyPlane(const Ipp16u* pSrc, Ipp32s srcStep, Ipp16u* pDst, Ipp32s dstStep, IppiSize roiSize)
    {
        if (!pSrc || !pDst)
            return ippStsNullPtrErr;
        return ippiCopy_8u_C1R((const Ipp8u*)pSrc, srcStep*sizeof(Ipp16u), (Ipp8u*)pDst, dstStep*sizeof(Ipp16u), roiSize);
    }

    inline  IppStatus ippiInterpolateLuma(const Ipp16u* , const IppVCInterpolateBlock_16u *info_)
    {
        IppVCInterpolate_16u info;

        info.pSrc = info_->pSrc[0];
        info.srcStep = info_->srcStep;
        info.pDst = info_->pDst[0];
        info.dstStep = info_->dstStep;
        info.dx = info_->pointVector.x;
        info.dy = info_->pointVector.y;
        info.roiSize = info_->sizeBlock;
        info.bitDepth = info_->bitDepth;

        return ippiInterpolateLuma_H264_16u_C1R(&info);
    }

    inline IppStatus ippiInterpolateLumaBlock(const Ipp16u *, const IppVCInterpolateBlock_16u *interpolateInfo)
    {
        return ippiInterpolateLumaBlock_H264_16u_P1R(interpolateInfo);
    }

    inline IppStatus ippiInterpolateChromaBlock(const Ipp16u *, const IppVCInterpolateBlock_16u *interpolateInfo)
    {
        return ippiInterpolateChromaBlock_H264_16u_P2R(interpolateInfo);
    }

    inline IppStatus ExpandPlane(Ipp16u *StartPtr,
                                 Ipp32u uFrameWidth,
                                 Ipp32u uFrameHeight,
                                 Ipp32u uPitch,
                                 Ipp32u uPels,
                                 IppvcFrameFieldFlag uFrameFieldFlag)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiExpandPlane_H264_8u_C1R(StartPtr,
                                            uFrameWidth,
                                            uFrameHeight,
                                            uPitch,
                                            uPels,
                                            uFrameFieldFlag);*/
    }

    inline IppStatus UniDirWeightBlock(Ipp16u *, VCBidir1_16u * info,
                                       Ipp32u ulog2wd,
                                       Ipp32s iWeight,
                                       Ipp32s iOffset)
    {
        return ippiUnidirWeight_H264_16u_IP2P1R(info->pDst, info->dstStep, ulog2wd, iWeight, iOffset, info->roiSize, info->bitDepth);
    }


#define FILL_RECONSTRUCT_INFO   \
    IppiReconstructHighMB_32s16u info;\
    info.ppSrcDstCoeff = ppSrcDstCoeff;\
    info.pSrcDstPlane = pSrcDstYPlane;\
    info.srcDstStep = srcDstStep;\
    info.cbp = cbp;\
    info.qp = QP;\
    info.pQuantTable = pQuantTable;\
    info.bypassFlag = bypass_flag;\
    info.bitDepth = bit_depth;

    inline IppStatus ReconstructLumaIntraHalfMB(Ipp32s **ppSrcCoeff,
                                                Ipp16u *pSrcDstYPlane,
                                                Ipp32s srcdstYStep,
                                                IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                Ipp32u cbp4x2,
                                                Ipp32s QP,
                                                Ipp8u  edgeType,
                                                Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructLumaIntraHalfMB_H264_16s8u_C1R(ppSrcCoeff,
                                                                    pSrcDstYPlane,
                                                                    srcdstYStep,
                                                                    pMBIntraTypes,
                                                                    cbp4x2,
                                                                    QP,
                                                                    edgeType);*/
    }

    inline IppStatus ReconstructLumaInter8x8MB(Ipp32s **ppSrcDstCoeff,
                                               Ipp16u *pSrcDstYPlane,
                                               Ipp32u srcDstStep,
                                               Ipp32u cbp,
                                               Ipp32s QP,
                                               Ipp16s *pQuantTable,
                                               Ipp8u  bypass_flag,
                                               Ipp32s bit_depth)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaInter8x8_H264High_32s16u_IP1R(&info);
    }

    inline IppStatus ReconstructLumaInter4x4MB(Ipp32s **ppSrcDstCoeff,
                                               Ipp16u *pSrcDstYPlane,
                                               Ipp32u srcDstStep,
                                               Ipp32u cbp,
                                               Ipp32s QP,
                                               Ipp16s *pQuantTable,
                                               Ipp8u  bypass_flag,
                                               Ipp32s bit_depth)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaInter4x4_H264High_32s16u_IP1R(&info);
    }

    inline IppStatus ReconstructLumaIntra_16x16MB(Ipp32s **ppSrcDstCoeff,
                                                  Ipp16u *pSrcDstYPlane,
                                                  Ipp32u srcDstStep,
                                                  IppIntra16x16PredMode_H264 intra_luma_mode,
                                                  Ipp32u cbp,
                                                  Ipp32s QP,
                                                  Ipp8u  edge_type,
                                                  Ipp16s *pQuantTable,
                                                  Ipp8u  bypass_flag,
                                                  Ipp32s bit_depth)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaIntra16x16_H264High_32s16u_IP1R(&info,
                                                                  intra_luma_mode,
                                                                  edge_type);
    }

    inline IppStatus ReconstructLumaIntraHalf8x8MB(Ipp32s **ppSrcDstCoeff,
                                                   Ipp16u *pSrcDstYPlane,
                                                   Ipp32s srcDstStep,
                                                   IppIntra8x8PredMode_H264 *pMBIntraTypes,
                                                   Ipp32u cbp,
                                                   Ipp32s QP,
                                                   Ipp8u  edgeType,
                                                   Ipp16s *pQuantTable,
                                                   Ipp8u  bypass_flag,
                                                   Ipp32s bit_depth = 10)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaIntraHalf8x8_H264High_32s16u_IP1R(&info,
                                                                    pMBIntraTypes,
                                                                    edgeType);
    }

    inline IppStatus ReconstructLumaIntraHalf4x4MB(Ipp32s **ppSrcDstCoeff,
                                                   Ipp16u *pSrcDstYPlane,
                                                   Ipp32s srcDstStep,
                                                   IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                   Ipp32u cbp,
                                                   Ipp32s QP,
                                                   Ipp8u  edgeType,
                                                   Ipp16s *pQuantTable,
                                                   Ipp8u  bypass_flag,
                                                   Ipp32s bit_depth = 10)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaIntraHalf4x4_H264High_32s16u_IP1R(&info,
                                                                    pMBIntraTypes,
                                                                    edgeType);
    }

    inline IppStatus ReconstructLumaIntra8x8MB(Ipp32s **ppSrcDstCoeff,
                                               Ipp16u *pSrcDstYPlane,
                                               Ipp32s srcDstStep,
                                               IppIntra8x8PredMode_H264 *pMBIntraTypes,
                                               Ipp32u cbp,
                                               Ipp32s QP,
                                               Ipp8u  edgeType,
                                               Ipp16s *pQuantTable,
                                               Ipp8u  bypass_flag,
                                               Ipp32s bit_depth)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaIntra8x8_H264High_32s16u_IP1R(&info,
                                                                pMBIntraTypes,
                                                                edgeType);
    }

    inline  IppStatus ReconstructLumaIntra4x4MB(Ipp32s **ppSrcDstCoeff,
                                                Ipp16u *pSrcDstYPlane,
                                                Ipp32s srcDstStep,
                                                IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                                Ipp32u cbp,
                                                Ipp32s QP,
                                                Ipp8u  edgeType,
                                                Ipp16s *pQuantTable,
                                                Ipp8u  bypass_flag,
                                                Ipp32s bit_depth)
    {
        FILL_RECONSTRUCT_INFO;
        return ippiReconstructLumaIntra4x4_H264High_32s16u_IP1R(&info,
                                                                pMBIntraTypes,
                                                                edgeType);
    }

    inline IppStatus ReconstructLumaIntra16x16MB(Ipp32s **ppSrcCoeff,
                                                 Ipp16u *pSrcDstYPlane,
                                                 Ipp32u srcdstYStep,
                                                 const IppIntra16x16PredMode_H264 intra_luma_mode,
                                                 const Ipp32u cbp4x4,
                                                 const Ipp32s QP,
                                                 const Ipp8u edge_type,
                                                 Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructLumaIntra16x16MB_H264_16s8u_C1R(ppSrcCoeff,
                                                                   pSrcDstYPlane,
                                                                   srcdstYStep,
                                                                   intra_luma_mode,
                                                                   cbp4x4,
                                                                   QP,
                                                                   edge_type);*/
    }

    inline IppStatus ReconstructLumaIntraMB(Ipp32s **ppSrcCoeff,
                                            Ipp16u *pSrcDstYPlane,
                                            Ipp32s srcdstYStep,
                                            const IppIntra4x4PredMode_H264 *pMBIntraTypes,
                                            const Ipp32u cbp4x4,
                                            const Ipp32s QP,
                                            const Ipp8u edgeType,
                                            Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructLumaIntraMB_H264_16s8u_C1R(ppSrcCoeff,
                                                        pSrcDstYPlane,
                                                        srcdstYStep,
                                                        pMBIntraTypes,
                                                        cbp4x4,
                                                        QP,
                                                        edgeType);*/
    }

    inline IppStatus ReconstructLumaInterMB(Ipp32s **ppSrcCoeff,
                                            Ipp16u *pSrcDstYPlane,
                                            Ipp32u srcdstYStep,
                                            Ipp32u cbp4x4,
                                            Ipp32s QP,
                                            Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;/*ippiReconstructLumaInterMB_H264_16s8u_C1R(ppSrcCoeff,
                                                        pSrcDstYPlane,
                                                        srcdstYStep,
                                                        cbp4x4,
                                                        QP);*/
    }

#define FILL_CHROMA_RECONSTRUCT_INFO    \
    IppiReconstructHighMB_32s16u info_temp[2];\
    const IppiReconstructHighMB_32s16u *info[2];\
    info[0] = &info_temp[0];\
    info[1] = &info_temp[1];\
    info_temp[0].ppSrcDstCoeff = ppSrcDstCoeff;   \
    info_temp[0].pSrcDstPlane = pSrcDstUPlane;  \
    info_temp[0].srcDstStep = srcdstUVStep;    \
    info_temp[0].cbp = (Ipp32u)cbpU;          \
    info_temp[0].qp = chromaQPU;               \
    info_temp[0].pQuantTable = pQuantTableU;  \
    info_temp[0].bypassFlag = bypass_flag;      \
    info_temp[0].bitDepth = bit_depth;            \
    info_temp[1].ppSrcDstCoeff = ppSrcDstCoeff;   \
    info_temp[1].pSrcDstPlane = pSrcDstVPlane;  \
    info_temp[1].srcDstStep = srcdstUVStep;    \
    info_temp[1].cbp = (Ipp32u)cbpV;          \
    info_temp[1].qp = chromaQPV;\
    info_temp[1].pQuantTable = pQuantTableV;\
    info_temp[1].bypassFlag = bypass_flag;\
    info_temp[1].bitDepth = bit_depth;

    inline IppStatus ReconstructChromaInter4x4MB(Ipp32s **ppSrcDstCoeff,
                                                 Ipp16u *pSrcDstUPlane,
                                                 Ipp16u *pSrcDstVPlane,
                                                 Ipp32u srcdstUVStep,
                                                 Ipp32u cbpU,
                                                 Ipp32u cbpV,
                                                 Ipp32u chromaQPU,
                                                 Ipp32u chromaQPV,
                                                 Ipp16s *pQuantTableU,
                                                 Ipp16s *pQuantTableV,
                                                 Ipp8u  bypass_flag,
                                                 Ipp32s bit_depth)
    {
        FILL_CHROMA_RECONSTRUCT_INFO;
        return ippiReconstructChromaInter4x4_H264High_32s16u_IP2R(info);
    }

    inline IppStatus ReconstructChromaIntra4x4MB(Ipp32s **ppSrcDstCoeff,
                                                 Ipp16u *pSrcDstUPlane,
                                                 Ipp16u *pSrcDstVPlane,
                                                 Ipp32u srcdstUVStep,
                                                 IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                 Ipp32u cbpU,
                                                 Ipp32u cbpV,
                                                 Ipp32u chromaQPU,
                                                 Ipp32u chromaQPV,
                                                 Ipp8u  edge_type,
                                                 Ipp16s *pQuantTableU,
                                                 Ipp16s *pQuantTableV,
                                                 Ipp8u  bypass_flag,
                                                 Ipp32s bit_depth)
    {
        FILL_CHROMA_RECONSTRUCT_INFO;
        return ippiReconstructChromaIntra4x4_H264High_32s16u_IP2R(info,
            intra_chroma_mode,
            edge_type);
    }


    inline IppStatus ReconstructChromaIntraHalfs4x4MB(Ipp32s **ppSrcDstCoeff,
                                                      Ipp16u *pSrcDstUPlane,
                                                      Ipp16u *pSrcDstVPlane,
                                                      Ipp32u srcdstUVStep,
                                                      IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                      Ipp32u cbpU,
                                                      Ipp32u cbpV,
                                                      Ipp32u chromaQPU,
                                                      Ipp32u chromaQPV,
                                                      Ipp8u  edge_type_top,
                                                      Ipp8u  edge_type_bottom,
                                                      Ipp16s *pQuantTableU,
                                                      Ipp16s *pQuantTableV,
                                                      Ipp8u  bypass_flag,
                                                      Ipp32s bit_depth = 10)
    {
        FILL_CHROMA_RECONSTRUCT_INFO;
        return ippiReconstructChromaIntraHalf4x4_H264High_32s16u_IP2R(info,
                                                                      intra_chroma_mode,
                                                                      edge_type_top,
                                                                      edge_type_bottom);
    }

    inline IppStatus ReconstructChromaIntraHalfs4x4MB422(Ipp32s **ppSrcDstCoeff,
                                                         Ipp16u *pSrcDstUPlane,
                                                         Ipp16u *pSrcDstVPlane,
                                                         Ipp32u srcdstUVStep,
                                                         IppIntraChromaPredMode_H264 intraChromaMode,
                                                         Ipp32u cbpU,
                                                         Ipp32u cbpV,
                                                         Ipp32u chromaQPU,
                                                         Ipp32u chromaQPV,
                                                         Ipp32u levelScaleDCU,
                                                         Ipp32u levelScaleDCV,
                                                         Ipp8u  edgeTypeTop,
                                                         Ipp8u  edgeTypeBottom,
                                                         Ipp16s *pQuantTableU,
                                                         Ipp16s *pQuantTableV,
                                                         Ipp8u  bypass_flag,
                                                         Ipp32s bit_depth = 10)
    {
        FILL_CHROMA_RECONSTRUCT_INFO;
        return ippiReconstructChroma422IntraHalf4x4_H264High_32s16u_IP2R(info,
            intraChromaMode,
            edgeTypeTop,
            edgeTypeBottom,
            levelScaleDCU,
            levelScaleDCV);
    }

    inline IppStatus ReconstructChromaIntraHalfs4x4MB444(Ipp32s **ppSrcDstCoeff,
                                                         Ipp16u *pSrcDstUPlane,
                                                         Ipp16u *pSrcDstVPlane,
                                                         Ipp32u srcdstUVStep,
                                                         IppIntraChromaPredMode_H264 intraChromaMode,
                                                         Ipp32u cbpU,
                                                         Ipp32u cbpV,
                                                         Ipp32u chromaQPU,
                                                         Ipp32u chromaQPV,
                                                         Ipp8u  edgeTypeTop,
                                                         Ipp8u  edgeTypeBottom,
                                                         Ipp16s *pQuantTableU,
                                                         Ipp16s *pQuantTableV,
                                                         Ipp8u  bypassFlag,
                                                         Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }

   inline IppStatus ReconstructChromaInter4x4MB422(Ipp32s **ppSrcDstCoeff,
                                                   Ipp16u *pSrcDstUPlane,
                                                   Ipp16u *pSrcDstVPlane,
                                                   Ipp32u srcdstUVStep,
                                                   Ipp32u cbpU,
                                                   Ipp32u cbpV,
                                                   Ipp32u chromaQPU,
                                                   Ipp32u chromaQPV,
                                                   Ipp32u levelScaleDCU,
                                                   Ipp32u levelScaleDCV,
                                                   Ipp16s *pQuantTableU,
                                                   Ipp16s *pQuantTableV,
                                                   Ipp8u  bypass_flag,
                                                   Ipp32s bit_depth)
   {
       FILL_CHROMA_RECONSTRUCT_INFO;
       return ippiReconstructChroma422Inter4x4_H264High_32s16u_IP2R(info,
                                                                    levelScaleDCU,
                                                                    levelScaleDCV);
   }

    inline IppStatus ReconstructChromaInter4x4MB444(Ipp32s **ppSrcDstCoeff,
                                                    Ipp16u *pSrcDstUPlane,
                                                    Ipp16u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u  bypassFlag,
                                                    Ipp32s bit_depth = 10)
   {
       VM_ASSERT(false);
       return ippStsNoErr;
   }


    inline IppStatus ReconstructChromaInterMB(Ipp32s **ppSrcCoeff,
                                              Ipp16u *pSrcDstUPlane,
                                              Ipp16u *pSrcDstVPlane,
                                              const Ipp32u srcdstStep,
                                              Ipp32u cbpU,
                                              Ipp32u cbpV,
                                              const Ipp32u ChromaQP,
                                              Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }

    inline IppStatus ReconstructChromaIntraMB(Ipp32s **ppSrcCoeff,
                                              Ipp16u *pSrcDstUPlane,
                                              Ipp16u *pSrcDstVPlane,
                                              const Ipp32u srcdstUVStep,
                                              const IppIntraChromaPredMode_H264 intra_chroma_mode,
                                              Ipp32u cbpU,
                                              Ipp32u cbpV,
                                              const Ipp32u ChromaQP,
                                              const Ipp8u edge_type,
                                              Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }


    inline IppStatus ReconstructChromaIntraHalfsMB(Ipp32s **ppSrcCoeff,
                                                   Ipp16u *pSrcDstUPlane,
                                                   Ipp16u *pSrcDstVPlane,
                                                   Ipp32u srcdstUVStep,
                                                   IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                   Ipp32u cbpU,
                                                   Ipp32u cbpV,
                                                   Ipp32u ChromaQP,
                                                   Ipp8u  edge_type_top,
                                                   Ipp8u  edge_type_bottom,
                                                   Ipp32s bit_depth = 10)
    {
        VM_ASSERT(false);
        return ippStsNoErr;
    }

    inline IppStatus ReconstructChromaIntra4x4MB422(Ipp32s **ppSrcDstCoeff,
                                                    Ipp16u *pSrcDstUPlane,
                                                    Ipp16u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    IppIntraChromaPredMode_H264 intraChromaMode,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp32u levelScaleDCU,
                                                    Ipp32u levelScaleDCV,
                                                    Ipp8u  edgeType,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u  bypass_flag,
                                                    Ipp32s bit_depth)
   {
       FILL_CHROMA_RECONSTRUCT_INFO;
       return ippiReconstructChroma422Intra4x4_H264High_32s16u_IP2R(info,
                                                                    intraChromaMode,
                                                                    edgeType,
                                                                    levelScaleDCU,
                                                                    levelScaleDCV);
   }

    inline IppStatus ReconstructChromaIntra4x4MB444(Ipp32s **ppSrcDstCoeff,
                                                    Ipp16u *pSrcDstUPlane,
                                                    Ipp16u *pSrcDstVPlane,
                                                    Ipp32u srcdstUVStep,
                                                    IppIntraChromaPredMode_H264 intraChromaMode,
                                                    Ipp32u cbpU,
                                                    Ipp32u cbpV,
                                                    Ipp32u chromaQPU,
                                                    Ipp32u chromaQPV,
                                                    Ipp8u  edgeType,
                                                    Ipp16s *pQuantTableU,
                                                    Ipp16s *pQuantTableV,
                                                    Ipp8u  bypassFlag,
                                                    Ipp32s bit_depth)
   {
       VM_ASSERT(false);
        return ippStsNoErr;
   }

    IppStatus FilterDeblockingLuma_HorEdge(Ipp16u* pSrcDst,
                                           Ipp32s  srcdstStep,
                                           Ipp8u*  pAlpha,
                                           Ipp8u*  pBeta,
                                           Ipp8u*  pThresholds,
                                           Ipp8u*  pBS,
                                           Ipp32s  bit_depth);

    IppStatus FilterDeblockingLuma_VerEdge_MBAFF(Ipp16u* pSrcDst,
                                                 Ipp32s  srcdstStep,
                                                 Ipp8u*  pAlpha,
                                                 Ipp8u*  pBeta,
                                                 Ipp8u*  pThresholds,
                                                 Ipp8u*  pBs,
                                                 Ipp32s  bit_depth);

    IppStatus FilterDeblockingLuma_VerEdge(Ipp16u* pSrcDst,
                                           Ipp32s  srcdstStep,
                                           Ipp8u*  pAlpha,
                                           Ipp8u*  pBeta,
                                           Ipp8u*  pThresholds,
                                           Ipp8u*  pBs,
                                           Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma_VerEdge_MBAFF(Ipp16u* pSrcDst,
                                                   Ipp32s  srcdstStep,
                                                   Ipp8u*  pAlpha,
                                                   Ipp8u*  pBeta,
                                                   Ipp8u*  pThresholds,
                                                   Ipp8u*  pBS,
                                                   Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma_VerEdge(Ipp16u* pSrcDst,
                                             Ipp32s  srcdstStep,
                                             Ipp8u*  pAlpha,
                                             Ipp8u*  pBeta,
                                             Ipp8u*  pThresholds,
                                             Ipp8u*  pBS,
                                             Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma_HorEdge(Ipp16u* pSrcDst,
                                             Ipp32s  srcdstStep,
                                             Ipp8u*  pAlpha,
                                             Ipp8u*  pBeta,
                                             Ipp8u*  pThresholds,
                                             Ipp8u*  pBS,
                                             Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma422_VerEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma422_HorEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma444_VerEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth);

    IppStatus FilterDeblockingChroma444_HorEdge(Ipp16u* pSrcDst,
                                                Ipp32s  srcdstStep,
                                                Ipp8u*  pAlpha,
                                                Ipp8u*  pBeta,
                                                Ipp8u*  pThresholds,
                                                Ipp8u*  pBS,
                                                Ipp32s  bit_depth);

#define FILL_VC_BIDIR_INFO  \
        IppVCBidir_16u info;\
        info.pSrc1 = info1->pSrc[0];\
        info.srcStep1 = info1->srcStep[0];\
        info.pSrc2 = info1->pSrc[1];\
        info.srcStep2 = info1->srcStep[1];\
        info.pDst = info1->pDst;\
        info.dstStep = info1->dstStep;\
        info.roiSize = info1->roiSize;\
        info.bitDepth = info1->bitDepth;

    inline IppStatus BiDirWeightBlock(Ipp16u * , VCBidir1_16u * info1,
                                      Ipp32u ulog2wd,
                                      Ipp32s iWeight1,
                                      Ipp32s iOffset1,
                                      Ipp32s iWeight2,
                                      Ipp32s iOffset2)
    {
        FILL_VC_BIDIR_INFO;
        return ippiBidirWeight_H264_16u_P2P1R(&info, ulog2wd, iWeight1, iOffset1, iWeight2, iOffset2);
    }

    inline IppStatus BiDirWeightBlockImplicit(Ipp16u * , VCBidir1_16u * info1,
                                              Ipp32s iWeight1,
                                              Ipp32s iWeight2)
    {
        FILL_VC_BIDIR_INFO;
        return ippiBidirWeightImplicit_H264_16u_P2P1R(&info,
                                                      iWeight1,
                                                      iWeight2);
    }

    inline IppStatus InterpolateBlock(Ipp16u * , VCBidir1_16u * info1)
    {
        FILL_VC_BIDIR_INFO
        return ippiBidir_H264_16u_P2P1R(&info);
    }

    inline IppStatus DecodeCAVLCChromaDcCoeffs422_H264(Ipp32u **ppBitStream,
                                                       Ipp32s *pOffset,
                                                       Ipp16s *pNumCoeff,
                                                       Ipp32s **ppDstCoeffs,
                                                       const Ipp32s *pTblCoeffToken,
                                                       const Ipp32s **ppTblTotalZerosCR,
                                                       const Ipp32s **ppTblRunBefore)
    {
        return ippiDecodeCAVLCChroma422DcCoeffs_H264_1u32s(ppBitStream,
                                                            pOffset,
                                                            pNumCoeff,
                                                            ppDstCoeffs,
                                                            pTblCoeffToken,
                                                            ppTblTotalZerosCR,
                                                            ppTblRunBefore);
    }

    inline IppStatus DecodeCAVLCCoeffs_H264(Ipp32u **ppBitStream,
                                            Ipp32s *pOffset,
                                            Ipp16s *pNumCoeff,
                                            Ipp32s **ppDstCoeffs,
                                            Ipp32u uVLCSelect,
                                            Ipp16s uMaxNumCoeff,
                                            const Ipp32s **ppTblCoeffToken,
                                            const Ipp32s **ppTblTotalZeros,
                                            const Ipp32s **ppTblRunBefore,
                                            const Ipp32s *pScanMatrix)
    {
        return ippiDecodeCAVLCCoeffs_H264_1u32s(ppBitStream,
                                                        pOffset,
                                                        pNumCoeff,
                                                        ppDstCoeffs,
                                                        uVLCSelect,
                                                        uMaxNumCoeff,
                                                        ppTblCoeffToken,
                                                        ppTblTotalZeros,
                                                        ppTblRunBefore,
                                                        pScanMatrix);
    }

    inline IppStatus MyDecodeCAVLCChromaDcCoeffs_H264(Ipp32u **ppBitStream,
                                                    Ipp32s *pOffset,
                                                    Ipp16s *pNumCoeff,
                                                    Ipp32s **ppDstCoeffs,
                                                    const Ipp32s *tblCoeffToken,
                                                    const Ipp32s **tblTotalZerosCR,
                                                    const Ipp32s **tblRunBefore)
    {
        return ippiDecodeCAVLCChromaDcCoeffs_H264_1u32s(ppBitStream,
                                                             pOffset,
                                                             pNumCoeff,
                                                             ppDstCoeffs,
                                                             tblCoeffToken,
                                                             tblTotalZerosCR,
                                                             tblRunBefore);
    }

    inline IppStatus MyDecodeCAVLCCoeffs_H264(Ipp32u **ppBitStream,
                                            Ipp32s *pOffset,
                                            Ipp16s *pNumCoeff,
                                            Ipp32s **ppDstCoeffs,
                                            Ipp32u uVLCSelect,
                                            Ipp16s uMaxNumCoeff,
                                            const Ipp32s **tblCoeffToken,
                                            const Ipp32s **tblTotalZeros,
                                            const Ipp32s **tblRunBefore,
                                            const Ipp32s *pScanMatrix)
    {
        return ippiDecodeCAVLCCoeffs_H264_1u32s(ppBitStream,
                                                         pOffset,
                                                         pNumCoeff,
                                                         ppDstCoeffs,
                                                         uVLCSelect,
                                                         uMaxNumCoeff,
                                                         tblCoeffToken,
                                                         tblTotalZeros,
                                                         tblRunBefore,
                                                         pScanMatrix);
    }
#if defined(_MSC_VER)
#pragma warning(default: 4100)
#endif

} // namespace UMC

#endif // __UMC_H264_DEC_IPP_WRAP_H
