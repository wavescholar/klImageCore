/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterMask.h: interface for the CRunCVFilterMask class.
// CRunCVFilterMask class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVFILTERMASK_H__68240965_E328_42D3_995F_4D004BD3D92F__INCLUDED_)
#define AFX_RUNCVFILTERMASK_H__68240965_E328_42D3_995F_4D004BD3D92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunCVFilter.h"

class CRunCVFilterMask : public CRunCVFilter
{
public:
   CRunCVFilterMask();
   virtual ~CRunCVFilterMask();

   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallGetBufSize(CString name);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppiMaskSize m_mask;
};

#endif // !defined(AFX_RUNCVFILTERMASK_H__68240965_E328_42D3_995F_4D004BD3D92F__INCLUDED_)
