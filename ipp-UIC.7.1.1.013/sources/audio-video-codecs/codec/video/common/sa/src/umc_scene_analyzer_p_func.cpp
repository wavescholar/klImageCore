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

namespace UMC
{

Status SceneAnalyzerP::AnalyzePicture(SceneAnalyzerPicture *pSrc)
{
    UMC_SCENE_INFO *pSliceInfo;
    const Ipp8u *pbSrc;
    Ipp32s srcStep;
    Ipp32u mbY;

    // reset the variables
    pSliceInfo = pSrc->m_pSliceInfo;
    memset(&(pSrc->m_info), 0, sizeof(pSrc->m_info));

    // get the source pointer
    pbSrc = pSrc->m_pPic[0];
    srcStep = (Ipp32s) pSrc->m_picStep;

    // cycle over rows
    for (mbY = 0; mbY < (Ipp32u) pSrc->m_mbDim.height; mbY += 1)
    {
        UMC_SCENE_INFO sliceInfo;
        Ipp32u mbX;

        // reset the variables
        memset(&sliceInfo, 0, sizeof(sliceInfo));

        // cycle in the row
        for (mbX = 0; mbX < (Ipp32u) pSrc->m_mbDim.width; mbX += 1)
        {
            UMC_SCENE_INFO mbInfo;

            // reset MB info
            memset(&mbInfo, 0, sizeof(mbInfo));
            mbInfo.numItems[SA_INTRA] = 1;
            mbInfo.numItems[SA_COLOR] = 1;

            // get the dispersion
            AnalyzeIntraMB(pbSrc + mbX * 4, srcStep,
                           &mbInfo);
/*
            {
                char cStr[256];
                sprintf(cStr, "% 5d ", pMbInfo->intraFrameDeviation);
                OutputDebugString(cStr);
            }*/

            // update slice info
            if (mbInfo.sumDev[SA_INTRA])
            {
                AddIntraDeviation(&sliceInfo, &mbInfo);
            }
        }
/*
        {
            char cStr[256];
            sprintf(cStr, "[% 5d ] % 5d\n", sliceInfo.intraFrameDeviation, mbY);
            OutputDebugString(cStr);
        }*/

        // update frame info
        AddIntraDeviation(&(pSrc->m_info), &sliceInfo);

        // get average for slice & update slice info
        if (pSliceInfo)
        {
            GetAverageIntraDeviation(&sliceInfo);
            *pSliceInfo = sliceInfo;
            pSliceInfo += 1;
        }

        // advance the pointer
        pbSrc += 4 * srcStep;
    }

    // get average for frame
    GetAverageIntraDeviation(&(pSrc->m_info));

    // set frame parameters
    pSrc->m_bChangeDetected = false;

    return UMC_OK;

} // Status SceneAnalyzerP::AnalyzePicture(SceneAnalyzerPicture *pSrc)

Status SceneAnalyzerP::AnalyzePicture(SceneAnalyzerPicture *pRef, SceneAnalyzerPicture *pSrc)
{
    UMC_SCENE_INFO *pSliceInfo;
    const Ipp8u *pbSrc;
    Ipp32s srcStep;
    const Ipp8u *pbRef;
    Ipp32s refStep;
    Ipp32u mbY;

    // reset the variables
    pSliceInfo = pSrc->m_pSliceInfo;
    memset(&(pSrc->m_info), 0, sizeof(pSrc->m_info));

    // get the source pointer
    pbSrc = (const Ipp8u *) pSrc->m_pPic[0];
    srcStep = (Ipp32s) pSrc->m_picStep;
    pbRef = (const Ipp8u *) pRef->m_pPic[0];
    refStep = (Ipp32s) pRef->m_picStep;

    // cycle over rows
    for (mbY = 0; mbY < (Ipp32u) pSrc->m_mbDim.height; mbY += 1)
    {
        UMC_SCENE_INFO sliceInfo;
        Ipp32u mbX;

        // reset the variables
        memset(&sliceInfo, 0, sizeof(sliceInfo));

        // cycle in the row
        for (mbX = 0; mbX < (Ipp32u) pSrc->m_mbDim.width; mbX += 1)
        {
            UMC_SCENE_INFO mbInfo;

            // reset MB info
            memset(&mbInfo, 0, sizeof(mbInfo));
            mbInfo.numItems[SA_INTRA] = 1;
            mbInfo.numItems[SA_COLOR] = 1;
            mbInfo.numItems[SA_INTER] = 1;
            mbInfo.numItems[SA_INTER_ESTIMATED] = 1;

            // get the dispersion
            AnalyzeInterMB(pbRef + mbX * 4, refStep,
                           pbSrc + mbX * 4, srcStep,
                           &mbInfo);

            // the difference is to big,
            // try to find a better match
            if (1 < mbInfo.sumDev[SA_INTER])
            {
                AnalyzeInterMBMotion(pbRef + mbX * 4, refStep,
                                     pRef->m_mbDim,
                                     pbSrc + mbX * 4, srcStep,
                                     mbX, mbY, pSrc->m_pSadBuffer,
                                     &mbInfo);
            }

            // update slice info
            if ((mbInfo.sumDev[SA_INTRA]) || (mbInfo.sumDev[SA_INTER]))
            {
                AddIntraDeviation(&sliceInfo, &mbInfo);
                AddInterDeviation(&sliceInfo, &mbInfo);
            }
        }

        // update frame info
        AddIntraDeviation(&(pSrc->m_info), &sliceInfo);
        AddInterDeviation(&(pSrc->m_info), &sliceInfo);

        // get average for slice & update slice info
        if (pSliceInfo)
        {
            GetAverageIntraDeviation(&sliceInfo);
            GetAverageInterDeviation(&sliceInfo);
            *pSliceInfo = sliceInfo;
            pSliceInfo += 1;
        }

        // advance the pointers
        pbSrc += 4 * srcStep;
        pbRef += 4 * refStep;
    }

    // get average for frame
    GetAverageIntraDeviation(&(pSrc->m_info));
    GetAverageInterDeviation(&(pSrc->m_info));

    //
    // make the frame type decision
    //

    MakePPictureCodingDecision(pRef, pSrc);

    return UMC_OK;

} // Status SceneAnalyzerP::AnalyzePicture(SceneAnalyzerPicture *pRef, SceneAnalyzerPicture *pSrc)

Status SceneAnalyzerP::AnalyzeFrame(SceneAnalyzerFrame *pSrc)
{
    // do entire frame analyzis
    AnalyzePicture(&(pSrc->m_scaledPic));

    if(PS_PROGRESSIVE != m_params.m_interlaceType)
    {
        Ipp32s frameDev, fieldDev;

        // analyze frame as PROGRESSIVE
        AnalyzePicture(pSrc);
        frameDev = pSrc->m_info.sumDev[SA_INTRA];
        // analyze both fields
        AnalyzePicture(pSrc->m_fields + 0);
        AnalyzePicture(pSrc->m_fields + 1);
        fieldDev = pSrc->m_fields[0].m_info.sumDev[SA_INTRA] +
                   pSrc->m_fields[1].m_info.sumDev[SA_INTRA];

        // compare dispersion of information
        if (frameDev <= fieldDev)
        {
            // it seems to be a frame
            pSrc->m_frameStructure = PS_PROGRESSIVE;
            pSrc->m_frameEstimation = PS_PROGRESSIVE;
        }
        else
        {
            // it seems to be a couple of fields
            pSrc->m_frameEstimation = pSrc->m_frameStructure;
        }
    }

    // restore frame detection state
    pSrc->m_bChangeDetected = true;

    return UMC_OK;

} // Status SceneAnalyzerP::AnalyzeFrame(SceneAnalyzerFrame *pSrc)

Status SceneAnalyzerP::AnalyzeFrame(SceneAnalyzerFrame *pRef, SceneAnalyzerFrame *pSrc)
{
    bool bSceneChangeDetected;

    // do entire frame analyzis
    AnalyzePicture(&(pRef->m_scaledPic), &(pSrc->m_scaledPic));
    bSceneChangeDetected = pSrc->m_scaledPic.m_bChangeDetected;

    if (PS_PROGRESSIVE != m_params.m_interlaceType)
    {
        Ipp32s frameDev, fieldDev;

        // analyze frame as PROGRESSIVE
        AnalyzePicture(pSrc);
        frameDev = pSrc->m_info.averageDev[SA_INTRA];
        // analyze both fields
        AnalyzePicture(pSrc->m_fields + 0);
        AnalyzePicture(pSrc->m_fields + 1);
        fieldDev = (pSrc->m_fields[0].m_info.averageDev[SA_INTRA] +
                    pSrc->m_fields[1].m_info.averageDev[SA_INTRA] + 1) / 2;

        // compare dispersion of information
        if (frameDev <= fieldDev)
        {
            // it seems to be a frame
            pSrc->m_frameStructure = PS_PROGRESSIVE;
            // this frame does not depend on the structure
            // of the references
            if (bSceneChangeDetected)
            {
                pSrc->m_frameEstimation = PS_PROGRESSIVE;
            }
            // this frame should estimate like structure of the reference frame
            else
            {
                pSrc->m_frameEstimation = pRef->m_frameStructure;
            }
        }
        else
        {
            // it seems to be a couple of fields
            pSrc->m_frameEstimation = pSrc->m_frameStructure;
        }
    }

    // restore frame detection state
    pSrc->m_bChangeDetected = bSceneChangeDetected;

    return UMC_OK;

} // Status SceneAnalyzerP::AnalyzeFrame(SceneAnalyzerFrame *pRef, SceneAnalyzerFrame *pSrc)

} // namespace UMC
