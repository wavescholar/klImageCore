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

#ifndef __UMC_SCENE_INFO_H
#define __UMC_SCENE_INFO_H

#include "ippdefs.h"

namespace UMC
{

// declare motion estimation boundaries
enum
{
    SA_ESTIMATION_WIDTH         = 128,
    SA_ESTIMATION_HEIGHT        = 64
};

// declare types of predictions
enum
{
    SA_COLOR                    = 0,
    SA_INTRA                    = 1,
    SA_INTER                    = 2,
    SA_INTER_ESTIMATED          = 3,

    SA_PRED_TYPE                = 4
};

// declare data structure
struct UMC_SCENE_INFO
{
    Ipp32u sumDev[SA_PRED_TYPE];                                // (Ipp32u []) sum of deviations of blocks
    Ipp32u averageDev[SA_PRED_TYPE];                            // (Ipp32u []) average deviation of blocks
    Ipp32u numItems[SA_PRED_TYPE];                              // (Ipp32u) number of valid units
    Ipp32u bestMatches;                                         // (Ipp32u) number of excellent motion matches
};

inline
void AddIntraDeviation(UMC_SCENE_INFO *pDst, const UMC_SCENE_INFO *pSrc)
{
    pDst->sumDev[SA_INTRA] += pSrc->sumDev[SA_INTRA];
    pDst->sumDev[SA_COLOR] += pSrc->sumDev[SA_COLOR];

    pDst->numItems[SA_INTRA] += pSrc->numItems[SA_INTRA];
    pDst->numItems[SA_COLOR] += pSrc->numItems[SA_COLOR];

    pDst->bestMatches += pSrc->bestMatches;

} // void AddIntraDeviation(UMC_SCENE_INFO *pDst, const UMC_SCENE_INFO *pSrc)

inline
void GetAverageIntraDeviation(UMC_SCENE_INFO *pSrc)
{
    Ipp32u numItems;

    // get average intra deviation
    numItems = IPP_MAX(1, pSrc->numItems[SA_INTRA]);
    pSrc->averageDev[SA_INTRA] = (pSrc->sumDev[SA_INTRA] + numItems / 2) /
                                 numItems;
    // get average color
    numItems = IPP_MAX(1, pSrc->numItems[SA_COLOR]);
    pSrc->averageDev[SA_COLOR] = (pSrc->sumDev[SA_COLOR] + numItems / 2) /
                                 numItems;

} // void GetAverageIntraDeviation(UMC_SCENE_INFO *pSrc)

inline
void AddInterDeviation(UMC_SCENE_INFO *pDst, const UMC_SCENE_INFO *pSrc)
{
    pDst->sumDev[SA_INTER] += pSrc->sumDev[SA_INTER];
    pDst->sumDev[SA_INTER_ESTIMATED] += pSrc->sumDev[SA_INTER_ESTIMATED];

    pDst->numItems[SA_INTER] += pSrc->numItems[SA_INTER];
    pDst->numItems[SA_INTER_ESTIMATED] += pSrc->numItems[SA_INTER_ESTIMATED];

} // void AddInterDeviation(UMC_SCENE_INFO *pDst, const UMC_SCENE_INFO *pSrc)

inline
void GetAverageInterDeviation(UMC_SCENE_INFO *pSrc)
{
    Ipp32u numItems;

    // get average intra deviation
    numItems = IPP_MAX(1, pSrc->numItems[SA_INTER]);
    pSrc->averageDev[SA_INTER] = (pSrc->sumDev[SA_INTER] + numItems / 2) /
                                 numItems;
    // get average color
    numItems = IPP_MAX(1, pSrc->numItems[SA_INTER_ESTIMATED]);
    pSrc->averageDev[SA_INTER_ESTIMATED] = (pSrc->sumDev[SA_INTER_ESTIMATED] + numItems / 2) /
                                           numItems;

} // void GetAverageIntraDeviation(UMC_SCENE_INFO *pSrc)

inline
Ipp32s GetAbs(Ipp32s value)
{
    Ipp32s signExtended = value >> 31;

    value = (value ^ signExtended) - signExtended;

    return value;

} // Ipp32s GetAbs(Ipp32s value)

} // namespace UMC

#endif // __UMC_SCENE_INFO_H
