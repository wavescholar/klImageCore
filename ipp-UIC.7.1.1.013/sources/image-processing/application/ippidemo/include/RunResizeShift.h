/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeShift.h: interface for the CRunResizeShift class.
// CRunResizeShift class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRESIZESHIFT_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_)
#define AFX_RUNRESIZESHIFT_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunResize.h"

class CRunResizeShift : public CRunResize
{
public:
   CRunResizeShift();
   virtual ~CRunResizeShift();

   virtual BOOL  Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL PrepareSrc();
   virtual BOOL PrepareDst();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

protected:
   virtual void PrepareParameters();

    int m_ShiftType;
    Ipp64f m_xShift;
    Ipp64f m_yShift;
    Ipp64f m_xFract;
    Ipp64f m_yFract;

};

#endif // !defined(AFX_RUNRESIZESHIFT_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_)
