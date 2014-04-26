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

#include "umc_scene_info.h"

namespace UMC
{

// Declaration of block layer functions
Ipp32u ippiGetIntraBlockDeviation_4x4_8u(const Ipp8u *pSrc, Ipp32s srcStep);
Ipp32u ippiGetAverage4x4_8u(const Ipp8u *pSrc, Ipp32s srcStep);
IppStatus ippiGetResidual4x4_8u16s(const Ipp8u *pRef, Ipp32s refStep,
                                   const Ipp8u *pSrc, Ipp32s srcStep,
                                   Ipp16s *pDst, Ipp32s dstStep);
Ipp32u ippiGetInterBlockDeviation_4x4_16s(const Ipp16s *pSrc, Ipp32s srcStep);

} // namespace UMC
