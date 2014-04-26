
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
//
*/

#ifndef __CPUINFO_H__
#define __CPUINFO_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  cpuinfo.h")
#endif


#include "stdafx.h"
#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif




/////////////////////////////////////////////////////////////////////////////
// CCPUInfo represent CPU info [clock rate and features]

class CCPUInfo : public CObject
{
  CString str;

public:
  CCPUInfo(void) {}

  CString& GetCpuName(void);

  DECLARE_SERIAL(CCPUInfo)
};


#endif // __CPUINFO_H__
