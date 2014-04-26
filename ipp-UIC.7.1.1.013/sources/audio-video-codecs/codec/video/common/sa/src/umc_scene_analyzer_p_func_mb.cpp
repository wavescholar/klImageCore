/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_scene_analyzer_p.h"
#include "umc_scene_analyzer_mb_func.h"
#include "ippvc.h"

namespace UMC
{

void SceneAnalyzerP::AnalyzeIntraMB(const Ipp8u *pSrc, Ipp32s srcStep,
                                    UMC_SCENE_INFO *pMbInfo)
{
    Ipp32u blockDev;

    // get block deviations
    blockDev = ippiGetIntraBlockDeviation_4x4_8u(pSrc, srcStep);
    pMbInfo->sumDev[SA_INTRA] = (blockDev + 8) / 16;

    // get average color
    pMbInfo->sumDev[SA_COLOR] = ippiGetAverage4x4_8u(pSrc, srcStep);

} // void SceneAnalyzerP::AnalyzeIntraMB(const Ipp8u *pSrc, Ipp32s srcStep,

void SceneAnalyzerP::AnalyzeInterMB(const Ipp8u *pRef, Ipp32s refStep,
                                    const Ipp8u *pSrc, Ipp32s srcStep,
                                    UMC_SCENE_INFO *pMbInfo)
{
    Ipp16s residual[4 * 4];
    Ipp32u blockDev;

    // do intra analysis
    AnalyzeIntraMB(pSrc, srcStep, pMbInfo);

    // get block residuals
    ippiGetResidual4x4_8u16s(pRef, refStep,
                             pSrc, srcStep,
                             residual, 4);

    // get block deviation
    blockDev = ippiGetInterBlockDeviation_4x4_16s(residual, 4);

    // get frame mode weight
    pMbInfo->sumDev[SA_INTER] = (blockDev + 8) / 16;
    pMbInfo->sumDev[SA_INTER_ESTIMATED] = pMbInfo->sumDev[SA_INTER];
    // increment number of excelent matches
    if (1 >= pMbInfo->sumDev[SA_INTER])
    {
        pMbInfo->bestMatches += 1;
    }

} // void SceneAnalyzerP::AnalyzeInterMB(const Ipp8u *pRef, Ipp32s refStep,

void SceneAnalyzerP::AnalyzeInterMBMotion(const Ipp8u *pRef, Ipp32s refStep,
                                          IppiSize refMbDim,
                                          const Ipp8u *pSrc, Ipp32s srcStep,
                                          Ipp32u mbX, Ipp32u mbY, Ipp16u *pSADs,
                                          UMC_SCENE_INFO *pMbInfo)
{
    Ipp32u numberOfSADs = SA_ESTIMATION_WIDTH;
    Ipp32u y;
    Ipp32u pixels, topRows, bottomRows;

    //
    // analyze working boundaries
    //

    // horizontal direction
    pixels = IPP_MIN(mbX * 4, SA_ESTIMATION_WIDTH / 2);
    pRef -= pixels;
    numberOfSADs -= (SA_ESTIMATION_WIDTH / 2 - pixels);

    pixels = IPP_MIN((refMbDim.width - mbX - 1) * 4, SA_ESTIMATION_WIDTH / 2);
    numberOfSADs -= (SA_ESTIMATION_WIDTH / 2 - pixels);
    numberOfSADs = (numberOfSADs + 7) & -8;

    // vertical direction
    topRows = IPP_MIN(mbY * 4, SA_ESTIMATION_HEIGHT / 2);

    bottomRows = IPP_MIN((refMbDim.height - mbY) * 4 - 3, SA_ESTIMATION_HEIGHT / 2);

    // main working cycle
    for (y = 0; y < SA_ESTIMATION_HEIGHT / 2; y += 1)
    {
        // try to find upper
        if (y < topRows)
        {
            Ipp32u minSAD, minSADidx;

            ippiSAD4x4xNI_8u16u_C1R(pSrc, srcStep,
                                    pRef - refStep * (y + 1), refStep,
                                    pSADs,
                                    numberOfSADs,
                                    &minSADidx);

            // get the lowest SAD
            minSAD = (pSADs[minSADidx] + 8) / 16;
            pMbInfo->sumDev[SA_INTER_ESTIMATED] = IPP_MIN(minSAD, pMbInfo->sumDev[SA_INTER_ESTIMATED]);
            if (1 >= pMbInfo->sumDev[SA_INTER_ESTIMATED])
                break;
        }

        // try to find lower
        if (y < bottomRows)
        {
            Ipp32u minSAD, minSADidx;

            ippiSAD4x4xNI_8u16u_C1R(pSrc, srcStep,
                                    pRef + refStep * y, refStep,
                                    pSADs,
                                    numberOfSADs,
                                    &minSADidx);

            // get the lowest SAD
            minSAD = (pSADs[minSADidx] + 8) / 16;
            pMbInfo->sumDev[SA_INTER_ESTIMATED] = IPP_MIN(minSAD, pMbInfo->sumDev[SA_INTER_ESTIMATED]);
            if (1 >= pMbInfo->sumDev[SA_INTER_ESTIMATED])
                break;
        }
    }

} // void SceneAnalyzerP::AnalyzeInterMBMotion(const Ipp8u *pRef, Ipp32s refStep,

} // namespace UMC
