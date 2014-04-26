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

#ifndef __UMC_SCENE_ANALYZER_ME_H
#define __UMC_SCENE_ANALYZER_ME_H

#include "umc_scene_analyzer.h"

namespace UMC
{

class SceneAnalyzerME : public SceneAnalyzer
{
public:
    SceneAnalyzerME();
    ~SceneAnalyzerME();

protected:
    virtual
    void AnalyzeInterMBMotion(
      const Ipp8u* pRef, Ipp32s refStep, IppiSize refMbDim,
      const Ipp8u* pSrc, Ipp32s srcStep,
            Ipp32u mbX, Ipp32u mbY, Ipp16u* pSADs, UMC_SCENE_INFO* pMbInfo);

    virtual
    void MakePPictureCodingDecision(SceneAnalyzerPicture* pRef, SceneAnalyzerPicture* pSrc);

    virtual
    Status AnalyzePicture(SceneAnalyzerPicture* pRef, SceneAnalyzerPicture* pSrc);

    IppiPoint* upperMVs;
    IppiPoint  prevMV; // reference coordinates for motion estimation
};

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_ME_H
