/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterColumn.h: interface for the CRunCVFilterColumn class.
// CRunCVFilterColumn class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVFILTERCOLUMN_H__F696114D_CF1A_46E5_BE09_448ABD07DB2C__INCLUDED_)
#define AFX_RUNCVFILTERCOLUMN_H__F696114D_CF1A_46E5_BE09_448ABD07DB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVFilterColumn : public CippiRun
{
public:
   CRunCVFilterColumn();
   virtual ~CRunCVFilterColumn();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   virtual IppStatus CallGetBufSize(CString name);
   CString m_GetBufSizeName;
   Ipp8u* m_pBuffer;
   int m_bufSize;

   void** m_ppSrc;
   IppiSize m_pipeSize;
   int m_kernelSize;
   CVectorUnit m_pKernel;
   int m_divisor;
   int m_DivType;

};

#endif // !defined(AFX_RUNCVFILTERCOLUMN_H__F696114D_CF1A_46E5_BE09_448ABD07DB2C__INCLUDED_)
