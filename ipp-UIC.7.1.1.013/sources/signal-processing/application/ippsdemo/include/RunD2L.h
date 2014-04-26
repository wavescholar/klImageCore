/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunD2L.h : interface for the CRunD2L class.
// CRunD2L class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUND2L_H__58F9A7BA_1186_4A94_A5BD_426B8A70F6D1__INCLUDED_)
#define AFX_RUND2L_H__58F9A7BA_1186_4A94_A5BD_426B8A70F6D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
class CippsDemoDoc;

class CRunD2L : public CippsRun  
{
public:
   CRunD2L();
   virtual ~CRunD2L();
   virtual BOOL Open(CFunc funcName);
   virtual void Close();
   virtual BOOL GetParms() { return TRUE;}
   virtual BOOL PickStart();
   virtual BOOL PickNextPos();
   virtual BOOL PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID);
   virtual void GrabDoc(CDemoDoc* pDoc);
   virtual BOOL PrepareDst();
   virtual void DeleteNewDst();
   virtual int GetDstLength();
   virtual void PrepareParameters();
   virtual BOOL NoDst() { return FALSE;}
   virtual void ActivateDst();
   virtual void SetHistory();
   virtual void SetToHistoSrc(CDemoDoc* pDoc, int vecPos);
   virtual CString GetVecName(int vecPos);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* parmDlg, BOOL save);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   void CreateD2L(int num);
   void DeleteD2L();
   int GetChanLen();

   CippsDemoDoc** m_ppDoc;
   void**     m_ppVec;
   int*       m_pNewDst;
   int  m_iDoc;
   int  m_numDoc;
   int  m_chanLen;
   BOOL m_bToD2L;
};



#endif // !defined(AFX_RUND2L_H__58F9A7BA_1186_4A94_A5BD_426B8A70F6D1__INCLUDED_)
