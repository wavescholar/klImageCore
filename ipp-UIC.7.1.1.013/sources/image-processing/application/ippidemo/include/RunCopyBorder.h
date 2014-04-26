/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyBorder.h: interface for the CRunCopyBorder class.
// CRunCopyBorder class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCOPYBORDER_H__8DD032FC_CD36_4E0E_B135_4ED48A1874C0__INCLUDED_)
#define AFX_RUNCOPYBORDER_H__8DD032FC_CD36_4E0E_B135_4ED48A1874C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCopyBorder : public CippiRun
{
public:
   CRunCopyBorder();
   virtual ~CRunCopyBorder();

protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual void CopyContoursToNewDst();
   virtual BOOL PrepareSrc();
   virtual BOOL BeforeCall();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   BOOL m_aroundRoi;
   int m_topLeft[2];
   int m_bottomRight[2];

   CVectorUnit m_valueSave ;
   CVector m_value ;
};

#endif // !defined(AFX_RUNCOPYBORDER_H__8DD032FC_CD36_4E0E_B135_4ED48A1874C0__INCLUDED_)
