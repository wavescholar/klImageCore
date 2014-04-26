
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

#ifndef __DRVNEW_H__
#define __DRVNEW_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  drvnew.h")
#endif


#ifndef _WINIOCTL_
#  if _MSC_VER >= 1000
#    pragma warning(disable:4201)
#  endif // _MSC_VER >= 1000
#  include "winioctl.h"
#endif
#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __DRVBASE_H__
#include "drvbase.h"
#endif
#ifndef _IPPS_H
#include "ipps.h"
#endif
#ifndef __IPPSDRV_H__
#include "..\drvnew\sys\ippsdrv.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CNewDriver represent ippsdrv kernel mode driver

class CNewDriver : public CDriver
{
private:
  IPPS_DEVICE_IOBUFFER m_ioBuffer;
  CString             m_strError;
  CString             m_LibName;
  CString             m_LibVers;
  CString             m_LibBild;
  CString             m_LibType;

public:
  CNewDriver();
  virtual ~CNewDriver();

  CString& GetLibName(void);
  CString& GetLibVers(void);
  CString& GetLibBild(void);
  CString& GetLibType(void);

  float GetNorma(void);
  DWORD GetError(void);

  void DevGetLibVersion(void);
  void StartFFT(int Length,int typeCore);

  DECLARE_SERIAL(CNewDriver)
};


#endif // __DRVNEW_H__
