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

#ifndef __UMC_SCENE_ANALYZER_P_H
#define __UMC_SCENE_ANALYZER_P_H

#include "umc_scene_analyzer_base.h"
#include "umc_scene_analyzer_frame.h"

namespace UMC
{

class SceneAnalyzerP : public SceneAnalyzerBase
{
public:
    // Default constructor
    SceneAnalyzerP(void);
    // Destructor
    virtual
    ~SceneAnalyzerP(void);

    // Initialize the analyzer,
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

    // Initialize the destination video data
    Status InitializeVideoData(VideoData *pDst, const SceneAnalyzerFrame *pFrame);
    // Update the history with given frame
    void UpdateHistory(const SceneAnalyzerFrame *pFrame, FrameType analysisType);

    // These functions are supposed to be overweighted for
    // every standard to make a custom weighting functions.

    // the main difference between a picture and a frame, that the picture is
    // just a picture. A frame is a complex structure, containing at least
    // 1 picture (3 pictures in field mode). So picture operating functions are
    // pure calculating functions. Frame operating functions do a little more -
    // they make a decision to assign specified type to a frame, basing on the
    // results of analyzing frames and thier fields.

    // Picture analysis functions for INTRA and INTER analysis
    virtual
    Status AnalyzePicture(SceneAnalyzerPicture *pSrc);
    virtual
    Status AnalyzePicture(SceneAnalyzerPicture *pRef, SceneAnalyzerPicture *pSrc);

    // Frame analysis functions for INTRA and INTER analysis
    virtual
    Status AnalyzeFrame(SceneAnalyzerFrame *pSrc);
    virtual
    Status AnalyzeFrame(SceneAnalyzerFrame *pRef, SceneAnalyzerFrame *pSrc);

    virtual
    void AnalyzeIntraMB(const Ipp8u *pSrc, Ipp32s srcStep,
                        UMC_SCENE_INFO *pMbInfo);
    virtual
    void AnalyzeInterMB(const Ipp8u *pRef, Ipp32s refStep,
                        const Ipp8u *pSrc, Ipp32s srcStep,
                        UMC_SCENE_INFO *pMbInfo);
    virtual
    void AnalyzeInterMBMotion(const Ipp8u *pRef, Ipp32s refStep,
                              IppiSize refMbDim,
                              const Ipp8u *pSrc, Ipp32s srcStep,
                              Ipp32u mbX, Ipp32u mbY, Ipp16u *pSADs,
                              UMC_SCENE_INFO *pMbInfo);

    // Make decision over the source frame coding type
    virtual
    void MakePPictureCodingDecision(SceneAnalyzerPicture *pRef, SceneAnalyzerPicture *pSrc);

    SceneAnalyzerFrame *m_pCur;                                 // (SceneAnalyzerFrame *) pointer to the current frame
    SceneAnalyzerFrame *m_pRef;                                 // (SceneAnalyzerFrame *) pointer to the previous reference frame

    struct
    {
        UMC_SCENE_INFO info;
        FrameType analyzedFrameType;
        bool m_bChangeDetected;

    } m_history[8];
    Ipp32s m_framesInHistory;

};

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_P_H
