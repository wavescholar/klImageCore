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

#ifndef __UMC_SCENE_ANALYZER_BASE_H
#define __UMC_SCENE_ANALYZER_BASE_H

#include "umc_base_codec.h"

namespace UMC
{

class SceneAnalyzerParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(SceneAnalyzerParams, BaseCodecParams)

    // Default constructor
    SceneAnalyzerParams(void);
    // Destructor
    virtual
    ~SceneAnalyzerParams(void);

    // you may specify valid positive value or set it to any
    // non-positive value to let the encoder make the decision
    Ipp32s m_maxGOPLength;                                      // maximum distance between intra frames

    // you may specify valid non-negative value or set it to any
    // negative value to let the encoder make the decision
    Ipp32s m_maxBLength;                                        // maximum length of B frame queue

    Ipp32u m_maxDelayTime;                                      // maximum delay time to get first encoded frame
    PictureStructure m_interlaceType;                           // type of constructred frames
};

// declare base class
class SceneAnalyzerBase : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(SceneAnalyzerBase, BaseCodec)

    // Default constructor
    SceneAnalyzerBase(void);
    // Destructor
    virtual
    ~SceneAnalyzerBase(void);

    // Initialize the SceneAnalyzer
    virtual
    Status Init(BaseCodecParams *pParams);

    // Analyze the next frame
    virtual
    Status GetFrame(MediaData *pSource, MediaData *pDestination);

    // Get current parameter(s) of SceneAnalyzer
    virtual
    Status GetInfo(BaseCodecParams *) { return UMC_ERR_NOT_IMPLEMENTED; };

    // Release all resources
    virtual
    Status Close(void);

    // Set codec to initial state
    virtual
    Status Reset(void);

protected:
    // Get planned frame type.
    // This function can be overweighted to make
    // any private GOP structures.
    virtual
    FrameType GetPlannedFrameType(void);

    Ipp32s m_frameNum;                                          // (Ipp32s) number of a frame in the scene
    Ipp32s m_bFrameNum;                                         // (Ipp32s) number of a B frame in a row
    Ipp32s m_frameCount;                                        // (Ipp32s) total number of processed frames

    SceneAnalyzerParams m_params;                               // (SceneAnalyzerParams) working parameters
};

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_BASE_H
