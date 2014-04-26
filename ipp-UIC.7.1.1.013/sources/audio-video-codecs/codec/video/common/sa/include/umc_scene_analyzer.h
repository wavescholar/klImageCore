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

#ifndef __UMC_SCENE_ANALYZER_H
#define __UMC_SCENE_ANALYZER_H

#include "umc_scene_analyzer_p.h"

namespace UMC
{

class SceneAnalyzer : public SceneAnalyzerP
{
public:
    // Default constructor
    SceneAnalyzer(void);
    // Destructor
    virtual
    ~SceneAnalyzer(void);

    // Initialize the analyzer
    // parameter should has type SceneAnalyzerParams
    virtual
    Status Init(BaseCodecParams *pParams);

    // Decompress the next frame
    virtual
    Status GetFrame(MediaData *pSource, MediaData *pDestination);

    // Release all resources
    virtual
    Status Close(void);

protected:

    SceneAnalyzerFrame *m_pPrev;                                // (SceneAnalyzerFrame *) pointer to the previous non-reference frame

};

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_H
