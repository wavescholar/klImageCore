
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChSplit.h : interface for the CRunChSplit class.
// CRunChSplit class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNSPLIT_H__8F42E2A8_6A74_42F3_8C14_3124A4DB5DB2__INCLUDED_)
#define AFX_RUNSPLIT_H__8F42E2A8_6A74_42F3_8C14_3124A4DB5DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChSplit : public CippsRun  
{
public:
   CRunChSplit();
   virtual ~CRunChSplit();

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

protected:
   void CreateD2L(int num);
   void DeleteD2L();
   int GetChanLen();

   CippsDemoDoc** m_ppDoc;
   void**     m_ppVec;
   CVector    m_pLen;
   CValue     m_delim;
//   int*       m_pNewDst;
   int  m_iDoc;
   int  m_numDoc;
   int  m_numSrc;
   int  m_numDst;
   BOOL m_bSplit;
   BOOL m_bDelim;

};

#endif // !defined(AFX_RUNSPLIT_H__8F42E2A8_6A74_42F3_8C14_3124A4DB5DB2__INCLUDED_)
