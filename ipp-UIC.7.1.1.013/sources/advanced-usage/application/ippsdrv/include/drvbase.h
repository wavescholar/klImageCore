
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

#ifndef __DRVBASE_H__
#define __DRVBASE_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  drvbase.h")
#endif


#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CDriver base class for kernel mode device driver

class CDriver : public CObject
{
private:
  HANDLE  m_hDevice;
  CString m_strError;

public:
  CDriver();
  virtual ~CDriver();

  BOOL OpenDevice(LPCSTR drvName);
  BOOL CloseDevice(void);

  HANDLE GetDeviceHandle(void);

  DECLARE_SERIAL(CDriver)
};


#endif // __DRVBASE_H__
