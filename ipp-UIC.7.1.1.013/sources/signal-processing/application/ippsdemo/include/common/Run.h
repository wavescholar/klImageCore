/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run.h: interface for the CRun class.
// CRun is the base class for all classes that process vectors or images
// by concrete IPP functions. 
// CRun class works with CDemoDoc class that contains source and
// destination vector or image.
// CRun class uses classes derived from CParamDlg class to obtain
// IPP function parameters by dialog.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNBASE_H__14E27847_9842_444F_8DD7_E5ED4DF4D0BA__INCLUDED_)
#define AFX_RUNBASE_H__14E27847_9842_444F_8DD7_E5ED4DF4D0BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"
class CParamDlg;
class CDemoDoc;
class CHisto;
class CTicker;

// This status will be returned by CRun::CallIppFunction instead of standard 
// IPP status if required function call is not implemented
#define stsNoFunction ((IppStatus)-999)

// Further macros use CALL macro that is defined in CDemo.h

// MATE_CALL macro produces IPP function call if its name equals to Mate
#define MATE_CALL(Mate,Name,Params) { \
   if (Mate == #Name) { \
      return CALL(Name, Params); \
   } \
}

// FUNC_CALL should be used in CRun::CallIppFunction().
// FUNC_CALL produces IPP function call if its name equals to m_Func
// but if CRun::m_Outstand == TRUE then FUNC_CALL adds 
// function name to the list.
#define FUNC_CALL(Name, Params) { \
   if (m_Outstand) { \
      m_FuncList.AddTail((CFunc)(#Name)); \
   } else { \
      if (m_Func == #Name) \
         return CALL(Name, Params); \
   } \
} 

// CHECK_CALL may be used in functions that return BOOL.
// CHECK_CALL macro produces IPP function call and checks IPP status
#define CHECK_CALL(Func, Params) { \
   IppStatus status = CALL(Func,Params); \
   CRun::IppErrorMessage(#Func, status); \
   if (status < 0) return FALSE; \
}

class CRun : public CObList  
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor
   CRun();
   virtual ~CRun();

/////////////////////////////////////////////////////////////////////////////
// Public Operations

   // ProcessFunctionStart is called by CDirector class at the very beginning 
   // of processing document by IPP function.
   // Returns TRUE if function with funcName may be called by this CRun class 
   // instance.
   virtual BOOL  ProcessFunctionStart(CDemoDoc *pDoc, CString funcName);
   // ProcessFunctionFinish is called by CDirector class at the end 
   // of processing document by IPP function 
   virtual void  ProcessFunctionFinish();
   // ProcessFunction is called by CDirector class when all necessary source
   // and destination documents to process have been picked by user
   virtual void  ProcessFunction();
   // GetFunctions is called by CDirector class. It adds to funcList all IPP
   // function names that may be called by this CRun class
   virtual void  GetFunctions(CFuncList& funcList);
   // PickStart is called by CDirector class. It returns if source and destination
   // documents selection may be start by user or not.
   virtual BOOL PickStart();
   // PickNext is called by CDirector class. It allows user to select the next
   // document.
   virtual BOOL PickNext(CMyString& vectorName, UINT& pickID, UINT& grabID);
   // GrabDoc is called by CDirector class. It sets document selected by user
   // as source or destination document for further process
   virtual void GrabDoc(CDemoDoc* pDoc);

   // Open is called by CRun::ProcessFunctionStart. 
   virtual BOOL  Open(CFunc func);
   // Close is called by CRun::ProcessFunctionFinish. 
   virtual void  Close();

   // CheckEqualDocs is called by CRun::ProcessFunction. It checks if
   // some source or destination documents are equal.
   virtual BOOL CheckEqualDocs();
   // GetParms is called by CRun::ProcessFunction. It gets IPP function parameters
   // through CParamsDlg
   virtual BOOL GetParms();
   // PrepareSrc is called by CRun::ProcessFunction. It prepairs source documents 
   // for processing.
   virtual BOOL PrepareSrc() { return TRUE;}
   // PrepareDst is called by CRun::ProcessFunction. It prepairs destination documents 
   // for processing.
   virtual BOOL PrepareDst() { return TRUE;}
   // ProcessCall is called by CRun::ProcessFunction. It prepairs IPP function
   // parameters and calls IPP function
   virtual BOOL  ProcessCall();
   // ActivateDst is called by CRun::ProcessFunction. It activates destination 
   // documents and updates their views.
   virtual void ActivateDst() {}
   // SetHistory is called by CRun::ProcessFunction. It sets information about IPP
   // function processing to destination history.
   virtual void SetHistory();
   
   // CreateDlg is called by CRun::GetParms. 
   // It creates class instance derived from CParamDlg class.
   virtual CParamDlg* CreateDlg(); 
   // DeleteDlg is called by CRun::GetParms. It deletes CParamDlg.
   virtual void DeleteDlg(CParamDlg* pDlg); 
   // UpdateData is called by CRun::GetParms. It transfers parameters
   // from Run class to ParamDlg class if save == FALSE and
   // from ParamDlg class to Run class if save == TRUE
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);

   // SaveVectorForTiming is called by CRun::ProcessCall at the very beginning.
   // It saves document data in the case of in-place IPP function.
   virtual void SaveVectorForTiming();
   // PrepareParameters is called by CRun::ProcessCall. It converts documents'
   // characteristics to IPP vector or image parameters.
   virtual void PrepareParameters() {}
   // BeforeCall is called by CRun::ProcessCall before function call.
   virtual BOOL BeforeCall() { return TRUE;}
   // AfterCall is called by CRun::ProcessCall at the end.
   virtual BOOL AfterCall(BOOL bOk) { return TRUE;}
   // CallIpp is called by CRun::ProcessCall. It calls IPP function,
   // checks its status and displays Message box in the error case
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   // Timing is called by CRun::ProcessCall if this option is on.
   virtual void Timing();
   // DeleteNewDst is called by CRun::ProcessCall if IPP function call failed.
   virtual void DeleteNewDst() {}

   // CallIppFunction is called by CRun::CallIpp. It should contain the list of
   // all IPP functions that can be processed by the class derived from CRun. 
   // Macro FUNC_CALL have to be used in this list.
   virtual IppStatus CallIppFunction() = NULL;

   // IppErrorMessage displays error or warning message if status != IppStsNoErr
   void IppErrorMessage(IppStatus status);
   static void IppErrorMessage(CString funcName, IppStatus status);
   // ExcDlgMessage is called by CRun::GetParms if there has been an exception 
   // while dialog
   virtual void ExcDlgMessage(CSExcept except);
   // ExcIppMessage is called by CRun::GetParms if there has been an exception 
   // while IPP function call
   virtual void ExcIppMessage(CSExcept except);

   // GetHistoryParms returns IPP function parameters' list to be placed to 
   // destination document history
   virtual CString GetHistoryParms() { return _T("");}
   // GetHistoryInfo returns additional information about IPP function call
   // to be placed to destination document history
   virtual CString GetHistoryInfo() {return _T("");}

   // Functions providing source and destination documents selection by user.
   virtual BOOL PickNextPos();
   virtual BOOL PickNextMaskPos();
   virtual BOOL PickMaskAfter(int vecPos);
   virtual BOOL PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID);
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual BOOL IsPickVecDst();
   virtual int GetPickVecPosToValid() { return m_PickVecPos;}
   virtual BOOL PickMarkDoc(CDemoDoc* pDoc);
   virtual BOOL SetNextPickPos(int vecFlag);
   virtual void SetPickVecPos(int mode) { m_PickVecPos = mode;}
   virtual int GetPickVecPos() { return m_PickVecPos;}
   virtual int  PickMarkDocs();
   virtual void PickMessageUnexist(CString vectorName, UINT pickID, CFunc func);
   static void PickRemarkDocs();

/////////////////////////////////////////////////////////////////////////////
// Public Attributes
   // GetFunc returns name of current processing IPP function
   virtual CFunc GetFunc() { return m_Func;}
   // IsFunction returns if IPP function with funcName may be called by
   // this CRun class or not
   virtual BOOL  IsFunction(CString funcName);
   // VectorTerm returns general name of document ("vector" or "image") 
   virtual CString VectorTerm() { return _T("vector");}
   // NoDst returns if there is no destination vectors among current IPP 
   // function parameters
   virtual BOOL NoDst();

protected:
   CFunc m_Func;
   CFuncList   m_FuncList;
   BOOL        m_Outstand;
   int         m_UsedVectors;
   CDemoDoc* m_pDocSrc;
   CDemoDoc* m_pDocSrc2;
   CDemoDoc* m_pDocSrc3;
   CDemoDoc* m_pDocMask;
   CDemoDoc* m_pDocDst;
   CDemoDoc* m_pDocDst2;
   int    scaleFactor;

   BOOL      m_NewDst;
   BOOL      m_NewDst2;

   int    m_PickVecPos;

   CTicker*    m_pTicker;   //*simplify timing*
   double      m_Perf;
   CString     m_PerfUnit;

   virtual void  InitFuncList();

   virtual BOOL GetEqualDocPos(int& firstPos, int& secondPos);

   virtual void SetParmsBeforeTiming();
   virtual void SetParmsAfterTiming();
   virtual void Loop(int num);
   virtual double GetPerfDivisor(CString& unitString) { unitString = _T(""); return 1;}

   virtual void SaveInplaceVector(){}
   virtual void SetInplaceParms(){}
   virtual void ResetInplaceParms(){}

   // Functions that support history information writing.
   virtual void SetToHisto(CDemoDoc* pDoc, int vecPos);
   virtual void SetToHistoSrc(CDemoDoc* pDoc, int vecPos);
   virtual void SetToHistoFunc(CDemoDoc* pDoc, int vecPos);
   virtual void SetToHistoDst(CDemoDoc* pDoc, int vecPos);
   virtual BOOL NoHistory(int vecPos);
   virtual CString GetVecName(int vecPos);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos = 0);
};

typedef CList<CRun*,CRun*> CRunList;
typedef CArray<CRun*,CRun*> CRunArray;

#endif // !defined(AFX_RUNBASE_H__14E27847_9842_444F_8DD7_E5ED4DF4D0BA__INCLUDED_)

