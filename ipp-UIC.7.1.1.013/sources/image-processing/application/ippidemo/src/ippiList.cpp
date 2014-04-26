/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiList.cpp: creation of ippSP, ippCC, ippCV and ippJP library
// function list
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ipp_List.h"

#include "ipp_List.inc"

#if LIB_I < LIB_NUM
#define  LIB_IDX LIB_I
#include "ippi.h"
#undef    LIB_IDX
#endif

#if LIB_J < LIB_NUM
#define  LIB_IDX LIB_J
#include "ippj.h"
#undef    LIB_IDX
#endif

#if LIB_CC < LIB_NUM
#define  LIB_IDX LIB_CC
#include "ippcc.h"
#undef    LIB_IDX
#endif

#if LIB_CV < LIB_NUM
#define  LIB_IDX LIB_CV
#include "ippcv.h"
#undef    LIB_IDX
#endif
