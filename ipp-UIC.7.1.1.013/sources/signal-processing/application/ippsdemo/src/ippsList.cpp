/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsList.cpp: creation of ippSP library function list
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ipp_List.h"

#include "ipp_List.inc"

#if LIB_SP < LIB_NUM
#define  LIB_IDX LIB_SP
#include "ipps.h"
#undef    LIB_IDX
#endif

#if LIB_CH < LIB_NUM
#define  LIB_IDX LIB_CH
#include "ippch.h"
#undef    LIB_IDX
#endif
