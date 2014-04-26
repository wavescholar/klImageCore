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

#ifndef __UMC_SCENE_ANALYZER_PARAMS_H
#define __UMC_SCENE_ANALYZER_PARAMS_H

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
    Ipp32s m_maxGOPLength;                                      // (Ipp32s) maximum distance between intra frames

    // you may specify valid non-negative value or set it to any
    // negative value to let the encoder make the decision
    Ipp32s m_maxBLength;                                        // (Ipp32s) maximum length of B frame queue

    Ipp32u m_maxDelayTime;                                      // (Ipp32u) maximum delay time to get first encoded frame
    InterlaceType m_interlaceType;                              // (InterlaceType) type of constructred frames

/*
    Ipp32u m_frameWidth;                                        // (Ipp32u) frame width
    Ipp32u m_frameHeight;                                       // (Ipp32u) frame height*/
};

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_PARAMS_H
