/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVCopy.h: interface for the CRunCVCopy class.
// CRunCVCopy class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVCOPY_H__0CA3DF20_0933_46CF_BD32_B2F068CCCD94__INCLUDED_)
#define AFX_RUNCVCOPY_H__0CA3DF20_0933_46CF_BD32_B2F068CCCD94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVCopy : public CippiRun
{
public:
   CRunCVCopy();
   virtual ~CRunCVCopy();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   Ipp32f m_dx;
   Ipp32f m_dy;
};

#endif // !defined(AFX_RUNCVCOPY_H__0CA3DF20_0933_46CF_BD32_B2F068CCCD94__INCLUDED_)
