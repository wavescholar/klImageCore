/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterMax.h: interface for the CRunCVFilterMax class.
// CRunCVFilterMax class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVFILTERMAX_H__CB74B67A_FD5D_48FA_8AA0_1D70EB8837AD__INCLUDED_)
#define AFX_RUNCVFILTERMAX_H__CB74B67A_FD5D_48FA_8AA0_1D70EB8837AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVFilterMax : public CippiRun
{
public:
   CRunCVFilterMax();
   virtual ~CRunCVFilterMax();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   virtual IppStatus CallGetBufSize(CString name);

   BOOL     m_bCenter;
   IppiSize m_maskSize;
   IppiPoint m_anchor;
   Ipp8u* m_pBuffer;
   int m_bufSize;

   CString m_GetBufSizeName;

};

#endif // !defined(AFX_RUNCVFILTERMAX_H__CB74B67A_FD5D_48FA_8AA0_1D70EB8837AD__INCLUDED_)
