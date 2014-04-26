/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsRun.h : interface for the CippsRun class.
// CippsRun is derived from CRun class that provides CDemo documents
// processing by IPP functions.
// CippsRun is the base class for all ippsDemo Run classes that process 
// vectors by concrete ippSP functions. 
// See CRun class for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_H__973F325C_399B_4FB4_BD6C_71CB2ABBFA37__INCLUDED_)
#define AFX_RUN_H__973F325C_399B_4FB4_BD6C_71CB2ABBFA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Run.h"
#include "Vector.h"

class CippsDemoDoc;
class CParamDlg;

// MATE_CH_CALL macro produces ippCC function call if its name equals to Mate
#define MATE_CH_CALL(Mate,Name,Params) { \
   if (Mate == #Name) { \
      return CH_CALL(Name, Params); \
   } \
}

// FUNC_CH_CALL should be used in CRun::CallIppFunction().
// FUNC_CH_CALL produces ippCC function call if its name equals to m_Func
// but if CRun::m_Outstand == TRUE then FUNC_CH_CALL adds
// function name to the list.
#define FUNC_CH_CALL(Name, Params) { \
   if (m_Outstand) { \
      m_FuncList.AddTail((CFunc)(#Name)); \
   } else { \
      if (m_Func == #Name) \
         return CH_CALL(Name, Params); \
   } \
}

// CHECK_CH_CALL may be used in functions that return BOOL.
// CHECK_CH_CALL macro produces ippCC function call and checks ippCC status
#define CHECK_CH_CALL(Func, Params) { \
   IppStatus status = CH_CALL(Func,Params); \
   CRun::IppErrorMessage(#Func, status); \
   if (status < 0) return FALSE; \
}

class CippsRun : public CRun
{
public:
   CippsRun();
   virtual ~CippsRun();
   virtual BOOL  ProcessFunctionStart(CDemoDoc *pDoc, CString funcName);
   virtual CString VectorTerm() { return "signal";}
protected:

    BOOL      m_bOne;
    int       m_iOne;
    void*     pSrc ;
    void*     pSrc2;
    void*     pSrc3;
    void*     pDst ;
    void*     pDst2;
    int      len;
    int      m_lenSrc ;
    int      m_lenSrc2;
    int      m_lenSrc3;
    int      m_lenDst ;
    int      m_lenDst2;

   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID);
   virtual int GetDstLength();
   virtual int GetDst2Length();
   virtual BOOL PrepareDst();
   virtual void DeleteNewDst();
   virtual void ActivateDst();
   virtual void PrepareParameters();
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual BOOL CallIppOne(BOOL bMessage = TRUE);
   virtual void AddToDocs(CippsDemoDoc* pDoc, CippsDemoDoc** ppDoc, int& num);
   virtual void ActivateDocs(CippsDemoDoc** ppDoc, int num);
   virtual void Loop(int num);
   virtual void LoopOne(int num);
   virtual double GetPerfDivisor(CString& unitString);

   void  SetParams(CVector* pVec, void*& pointer, int& len, int& lenParm);
   void  SetParams(CVector* pVec, void*& pointer, int& len);
   void ActivateDoc(CippsDemoDoc* pDoc);

   CVector     m_InplaceVector;
   int         m_ViewMode;

   BOOL CreateNewDst(BOOL bUsed, int length, BOOL& bNew, CDemoDoc*& pDoc);
   virtual void SaveInplaceVector();
   virtual void SetInplaceParms();
   virtual void ResetInplaceParms();
   void LoopNum(int num, BOOL bOne);
};

#endif // !defined(AFX_RUN_H__973F325C_399B_4FB4_BD6C_71CB2ABBFA37__INCLUDED_)
