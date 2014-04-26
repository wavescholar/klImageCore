/*
//
//                INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run.cpp: implementation of the CRun class.
// CRun is the base class for all classes that process vectors or images
// by concrete IPP functions. 
// CRun class works with CDemoDoc class that contains source and
// destination vector or image.
// CRun class uses classes derived from CParamDlg class to obtain
// IPP function parameters by dialog.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"
#include "Run.h"
#include "Vector.h"
#include "ParamDlg.h"
#include "Timing.h"
#include "Histo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRun::CRun()
{
    m_UsedVectors = 0;
    m_pDocSrc  = NULL;
    m_pDocSrc2 = NULL;
    m_pDocSrc3 = NULL;
    m_pDocDst  = NULL;
    m_pDocDst2 = NULL;
    m_pDocMask = NULL;
    m_NewDst  = FALSE;
    m_NewDst2 = FALSE;
    scaleFactor = 0;
    m_PickVecPos = -1;
    m_pTicker = new CTicker;
    m_pPerf = new CPerf;
}

CRun::~CRun()
{
    delete m_pTicker;
    delete m_pPerf;

}

void CRun::IppErrorMessage(IppStatus result)
{
    IppErrorMessage(m_Func, result);
}

void CRun::IppErrorMessage(CString funcName, IppStatus result)
{
    if (result == 0) return;
    CString message;
    if (result == stsNoFunction) 
       message.Format(_T("Function %s isn't implemented, sorry"), funcName); 
    else
       message.Format(_T("%s in %s:\nIppStatus = %d:\n%s"),
                    result < 0 ? _T("Error") : _T("Warning"),
                    funcName, result, ippGetStatusString(result)); 
    DEMO_APP->MessageBox(message, (result < 0) ? MB_OK | MB_ICONWARNING : MB_OK);
}

void CRun::GetFunctions(CFuncList& funcList)
{
    InitFuncList();
    funcList.AddTail(&m_FuncList);
}

BOOL CRun::IsFunction(CString funcName)
{
    InitFuncList();
    return (BOOL)m_FuncList.Find(funcName);
}

void CRun::InitFuncList()
{
    if (m_FuncList.GetCount()) return;
    m_Outstand = TRUE;
    CallIppFunction();
    m_Outstand = FALSE;
}

BOOL CRun::Open(CFunc func)
{
    m_Func = func;
    m_UsedVectors = VEC_SRC | VEC_DST;
    m_pPerf->Reset();
    return TRUE;
}

void CRun::Close()
{
    m_UsedVectors = 0;
}

BOOL CRun::ProcessFunctionStart(CDemoDoc *pDoc, CString funcName)
{
    if (!IsFunction(funcName)) return FALSE;
    m_pDocSrc  = pDoc;
    m_pDocSrc2 = NULL;
    m_pDocSrc3 = NULL;
    m_pDocDst  = NULL;
    m_pDocDst2 = NULL;
    m_pDocMask = NULL;
    if (!Open(funcName)) return FALSE;
    if (m_Func.Inplace()) m_UsedVectors &= ~VEC_DST;
    return TRUE;
}

void CRun::ProcessFunctionFinish()
{
    DEMO_APP->SetCursorArrow();
    Close();
    m_TmpDocList.RemoveAll();
}

void CRun::ProcessFunction()
{
    if (!CheckEqualDocs()) return;
    if (!GetParms()) return;
    DEMO_APP->SetCursorWait();
    if (!PrepareSrc()) return;
    if (!PrepareDst()) return;
    if (!ProcessCall()) return;
    if (!NoDst()) ActivateDst();
    SetHistory();
    return;
}

BOOL CRun::ProcessCall()
{
//*simplify timing*    SaveVectorForTiming();
    PrepareParameters();
    m_TmpDocList.ReleaseAll();
    if (!BeforeCall()) return FALSE;

    m_pTicker->Reset();  //*simplify timing*
    int result = CallIpp();
    if (result <= 0) {
        AfterCall(FALSE);
        DeleteNewDst();
        return FALSE;
    }
//*simplify timing*    Timing();
    if (!AfterCall(TRUE)) return FALSE;

    //*simplify timing*  
    CString unitString;
    m_pPerf->SetClocks(m_pTicker->Ticks());
    m_pPerf->SetDivisor(GetPerfDivisor());
    m_pPerf->SetUnit(GetPerfUnit());
    
    return TRUE;
}

CParamDlg* CRun::CreateDlg() 
{ 
    return NULL;
}

void CRun::DeleteDlg(CParamDlg* pDlg) 
{
    if (pDlg) delete pDlg;
}

void CRun::UpdateData(CParamDlg* pDlg, BOOL save)
{
    if (save) {
       scaleFactor = _ttoi(pDlg->m_scaleString);
    } else {
       pDlg->m_UsedVectors = m_UsedVectors;
       pDlg->m_Func  = m_Func;
       pDlg->m_pDocSrc    = m_pDocSrc;
       pDlg->m_pDocSrc2  = m_pDocSrc2;
       pDlg->m_pDocSrc3  = m_pDocSrc3;
       pDlg->m_pDocMask  = m_pDocMask;
       pDlg->m_pDocDst    = m_pDocDst;
       pDlg->m_scaleString.Format(_T("%d"),scaleFactor);
    }
}

BOOL CRun::GetParms()
{
    CParamDlg* pDlg = CreateDlg();
    if (pDlg) {
        UpdateData(pDlg, FALSE);
        try {
            if (pDlg->DoModal() != IDOK) return FALSE;
        } catch (CSExcept except) {
             ExcDlgMessage(except);
             return FALSE;
        }
        UpdateData(pDlg);
        DeleteDlg(pDlg);
    }
    return TRUE;
}

BOOL CRun::CallIpp(BOOL bMessage)
{
    IppStatus funcResult = ippStsNoErr;
     try {
          m_pTicker->Start();  //*simplify timing*
          funcResult = CallIppFunction();
          m_pTicker->Stop();  //*simplify timing*
     } catch (CSExcept except) {
          ExcIppMessage(except);
          return FALSE;
     }
    if (funcResult == ippStsNoErr) 
        return TRUE;
    if (bMessage > 0 || (bMessage < 0 && funcResult < 0))
    {
        IppErrorMessage(funcResult);
////////        return TRUE;
    }
    return funcResult > 0 ? TRUE : FALSE;
}

void CRun::ExcIppMessage(CSExcept except)
{
    AfxMessageBox(_T("Exception in ") + m_Func + _T(": ") + except.GetExString());
}

void CRun::ExcDlgMessage(CSExcept except)
{
    AfxMessageBox(
        _T("Exception in Parameters' Dialog: ") + 
        except.GetExString() + _T("\n")
        _T("Refer to program builder"));
}

BOOL CRun::CheckEqualDocs()
{
    int firstPos, secondPos;
    if (GetEqualDocPos(firstPos, secondPos)) {
       if (AfxMessageBox(
          GetVectorName(firstPos) 
          + _T(" and ") + GetVectorName(secondPos) 
          + _T(" are equal.\n")
          + _T("Function ") + m_Func + _T(" may give unexpected result."),
          MB_OKCANCEL) != IDOK) return FALSE;
    }
    return TRUE;
}

BOOL CRun::GetEqualDocPos(int& firstPos, int& secondPos)
{
    if (m_pDocDst) {
       secondPos = VEC_DST;
       if (m_pDocSrc == m_pDocDst) {
          firstPos = VEC_SRC;
          return TRUE;
       }
       if (m_pDocSrc2 == m_pDocDst) {
          firstPos = VEC_SRC2;
          return TRUE;
       }
       if (m_pDocMask == m_pDocDst) {
          firstPos = VEC_MASK;
          return TRUE;
       }
    }
    if (m_pDocSrc2 && m_Func.Inplace()) {
       secondPos = VEC_SRC2;
       if (m_pDocSrc == m_pDocSrc2) {
          firstPos = VEC_SRC;
          return TRUE;
       }
       if (m_pDocMask == m_pDocSrc2) {
          firstPos = VEC_MASK;
          return TRUE;
       }
    }
    return FALSE;
}

void CRun::SaveVectorForTiming()
{
    if (!CTiming::GetFlag()) return;
    if (!m_Func.Inplace()) return;
    SaveInplaceVector();
}

void CRun::SetParmsBeforeTiming()
{
    if (!m_Func.Inplace()) return;
    SetInplaceParms();
}

void CRun::SetParmsAfterTiming()
{
    if (!m_Func.Inplace()) return;
    ResetInplaceParms();
}

void CRun::Timing()
{
    //m_Perf = -1;
    //if (!CTiming::GetFlag()) return;
    //double divisor = GetPerfDivisor();
    //if (divisor <= 0) return;

    //SetParmsBeforeTiming();

    //CTiming tim;
    //int num;
    //double perf = 0;
    //switch (CTiming::GetMethod()) {
    //case timAUTO:
    //    tim.Reset();
    //    for (num = 1; tim.GetAccuracy() > 0.05; num *= 2) {
    //       tim.Start();
    //       Loop(num);
    //       tim.Stop(num);
    //    }
    //    perf = tim.GetTicks();
    //    break;
    //case timLOOP:
    //    num = CTiming::GetNumber(); 
    //    tim.Start();
    //    Loop(num);
    //    tim.Stop(num);
    //    perf = tim.GetTicks();
    //    break;
    //case timSTAT:
    //    num = CTiming::GetNumber(); 
    //    perf = 0;
    //    for (int i=0; i<num; i++) {
    //       tim.Start();
    //       Loop(1);
    //       tim.Stop(1);
    //       perf += tim.GetTicks();
    //    }
    //    perf /= num;
    //    break;
    //}
    //perf /= divisor;
    //m_Perf = perf;

    //SetParmsAfterTiming();
}

void CRun::Loop(int num)
{
    while (num--) CallIppFunction();
}

BOOL CRun::PickStart()
{
    m_pDocSrc->IsPicked(TRUE);
    m_PickVecPos = VEC_SRC;
    return TRUE;
}

BOOL CRun::PickNext(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
    if (!PickNextPos()) return FALSE;
    if (!PickSetID(vectorName, pickID, grabID)) return FALSE;
    if (PickMarkDocs() == 0)
       PickMessageUnexist(vectorName,pickID,m_Func);       
    return TRUE;
}

BOOL CRun::PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
    vectorName = GetVectorName(m_PickVecPos);
    return GetPickID(m_PickVecPos, pickID, grabID);
}

CMyString CRun::GetVectorName(int vecPos)
{
    if (vecPos == VEC_SRC) {
       if (m_Func.Inplace()) {
          if (m_UsedVectors & VEC_SRC2) {
             return _T("Src");
          } else {
             return _T("SrcDst");
          }
       } else if (m_UsedVectors & VEC_SRC2) {
          return _T("Src1");
       } else if (m_Func.OnlyDst()) {
          return _T("Dst");
       } else {
          return _T("Src");
       }
    } else if (vecPos == VEC_SRC2) {
       if (m_Func.Inplace()) {
          if (m_UsedVectors & VEC_SRC3) {
             return _T("Src2");
          } else {
             return _T("SrcDst");
          }
       } else {
          return _T("Src2");
       }
    } else if (vecPos == VEC_SRC3) {
       if (m_Func.Inplace()) {
          return _T("SrcDst");
       } else {
          return _T("Src3");
       }
    } else if (vecPos == VEC_MASK) {
       return _T("Mask");
    } else if (vecPos == VEC_DST) {
       if (m_UsedVectors & VEC_DST2) {
          return _T("Dst1");
       } else {
          return _T("Dst");
       }
    } else if (vecPos == VEC_DST2) {
       return _T("Dst2");
    } else {
       return _T("");
    }
}


BOOL CRun::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
    if (vecPos == VEC_SRC2) {
       if (m_Func.Inplace() && !(m_UsedVectors & VEC_SRC3)) {
          pickID = IDC_CURSOR_PSD;
          grabID = IDC_CURSOR_SD;
       } else {
          pickID = IDC_CURSOR_PS2;
          grabID = IDC_CURSOR_S2;
       }
    } else if (vecPos == VEC_SRC3) {
       if (m_Func.Inplace()) {
          pickID = IDC_CURSOR_PSD;
          grabID = IDC_CURSOR_SD;
       } else {
          pickID = IDC_CURSOR_PS3;
          grabID = IDC_CURSOR_S3;
       }
    } else if (vecPos == VEC_MASK) {
       pickID = IDC_CURSOR_PM;
       grabID = IDC_CURSOR_M;
    } else if (vecPos == VEC_DST) {
       pickID = IDC_CURSOR_PD;
       grabID = IDC_CURSOR_D;
    } else {
       return FALSE;
    }
    return TRUE;
}

BOOL CRun::PickMaskAfter(int vecPos)
{
    if (!(m_UsedVectors & VEC_MASK)) return FALSE;
    if ((m_UsedVectors & VEC_SRC2) && !m_Func.Inplace()) 
       return vecPos == VEC_SRC2;
    return vecPos == VEC_SRC;
}

BOOL CRun::PickNextMaskPos()
{    
    if (PickMaskAfter(VEC_SRC)) {
       if (m_PickVecPos == VEC_SRC) {
          m_PickVecPos = VEC_MASK;
          return TRUE;
       }
       if (m_PickVecPos == VEC_MASK) {
          m_PickVecPos = VEC_SRC;
       }
    } else if (PickMaskAfter(VEC_SRC2)) {
       if (m_PickVecPos == VEC_SRC2) {
          m_PickVecPos = VEC_MASK;
          return TRUE;
       }
       if (m_PickVecPos == VEC_MASK) {
          m_PickVecPos = VEC_SRC2;
       }
    }
    return FALSE;
}

BOOL CRun::PickNextPos()
{    
    if (PickNextMaskPos()) return TRUE;
    if (SetNextPickPos(VEC_SRC2)) return TRUE;
    if (SetNextPickPos(VEC_SRC3)) return TRUE;
    if (SetNextPickPos(VEC_DST)) return TRUE;
    if (SetNextPickPos(VEC_DST2)) return TRUE;
    m_PickVecPos = -1;
    return FALSE;
}

BOOL CRun::SetNextPickPos(int vecFlag)
{    
    if (!DEMO_APP->GetPickDst() && (vecFlag >= VEC_DST))
       return FALSE;
    if (m_UsedVectors & vecFlag) {
       if (m_PickVecPos < vecFlag) {
          m_PickVecPos = vecFlag;
          return TRUE;
       }
    }
    return FALSE;
}

BOOL CRun::IsPickVecDst()
{
    return m_PickVecPos >= VEC_DST;
}

BOOL CRun::PickMarkDoc(CDemoDoc* pDoc)
{
    BOOL flag = m_Func.Valid(pDoc->GetVector(),GetPickVecPosToValid());
    pDoc->MayBePicked(flag);
    return flag ? TRUE : FALSE;
}

int CRun::PickMarkDocs()
{
    int count = 0;
    MY_POSITION pos = DEMO_APP->GetFirstDocPosition(); 
    while (pos) {
       CDemoDoc* pDoc = DEMO_APP->GetNextDoc(pos);
       count += PickMarkDoc(pDoc);
    }
    return count;
}

void CRun::PickRemarkDocs()
{
    MY_POSITION pos = DEMO_APP->GetFirstDocPosition(); 
    while (pos) {
       CDemoDoc* pDoc = DEMO_APP->GetNextDoc(pos);
       pDoc->MayBePicked(FALSE);
       pDoc->IsPicked(FALSE);
    }
}

void CRun::GrabDoc(CDemoDoc* pDoc)
{
    if (GetPickVecPos() == VEC_SRC2)
       m_pDocSrc2 = pDoc;
    else if (GetPickVecPos() == VEC_SRC3)
       m_pDocSrc3 = pDoc;
    else if (GetPickVecPos() == VEC_MASK)
       m_pDocMask = pDoc;
    else if (GetPickVecPos() == VEC_DST)
       m_pDocDst = pDoc;
    else 
       return;
    pDoc->IsPicked(TRUE);
}

void CRun::PickMessageUnexist(CString vectorName, UINT pickID, CFunc func)
{
    AfxMessageBox(
       vectorName + _T(" ") + VectorTerm() + _T(" for ") + func + _T(" isn't exist\n")
       _T("Open proper ") + VectorTerm() + _T(" or stop processing, please"));
}


void CRun::SetHistory()
{
    SetToHisto(m_pDocSrc , VEC_SRC );
    SetToHisto(m_pDocSrc2, VEC_SRC2);
    SetToHisto(m_pDocSrc3, VEC_SRC3);
    SetToHisto(m_pDocDst , VEC_DST );
    SetToHisto(m_pDocDst2, VEC_DST2);
}

void CRun::SetToHisto(CDemoDoc* pDoc, int vecPos)
{
    if (!pDoc) return;
    if (NoHistory(vecPos)) return;
//    if (vecPos == VEC_DST)
    pDoc->GetVector()->GetHisto()->LastFunc(m_Func);
    SetToHistoSrc(pDoc,vecPos);
    SetToHistoFunc(pDoc,vecPos);
    SetToHistoDst(pDoc,vecPos);
}

BOOL CRun::NoHistory(int vecPos) {
    switch (vecPos) {
    case VEC_SRC: 
       if (NoDst()) return FALSE;
       if (!m_Func.Inplace()) return TRUE;
       if (m_pDocSrc2) return TRUE;
       break;
    case VEC_SRC2: 
       if (NoDst()) return TRUE;
       if (!m_Func.Inplace()) return TRUE;
       if (m_pDocSrc3) return TRUE;
       break;
    case VEC_SRC3: 
       if (NoDst()) return TRUE;
       if (!m_Func.Inplace()) return TRUE;
       break;
    }
    return FALSE;
}

void CRun::SetToHistoSrc(CDemoDoc* pDoc, int vecPos)
{
    CHisto* pHisto = pDoc->GetVector()->GetHisto();
    CMyString title = pDoc->GetTitle();
    switch (vecPos) {
    case VEC_SRC: 
       if (m_pDocMask) 
          pHisto->AddTail(m_pDocMask->GetTitle());
       break;
    case VEC_SRC2: 
       if (NoDst()) return;
       if (!m_Func.Inplace()) return;
       pHisto->RemoveAll();
       pHisto->AddTail(m_pDocSrc->GetVector()->GetHisto());
       if (m_pDocMask) 
          pHisto->AddTail(m_pDocMask->GetTitle());
       pHisto->AddTail(title);
       break;
    case VEC_SRC3: 
       if (NoDst()) return;
       if (!m_Func.Inplace()) return;
       pHisto->RemoveAll();
       pHisto->AddTail(m_pDocSrc->GetVector()->GetHisto());
       pHisto->AddTail(m_pDocSrc2->GetTitle());
       if (m_pDocMask) 
          pHisto->AddTail(m_pDocMask->GetTitle());
       pHisto->AddTail(title);
       break;
    default: 
       pHisto->RemoveAll();
       pHisto->AddTail(m_pDocSrc->GetVector()->GetHisto());
       if (m_pDocSrc2) 
          pHisto->AddTail(m_pDocSrc2->GetTitle());
       if (m_pDocSrc3) 
          pHisto->AddTail(m_pDocSrc3->GetTitle());
       if (m_pDocMask) 
          pHisto->AddTail(m_pDocMask->GetTitle());
       break;
    }
}

void CRun::SetToHistoFunc(CDemoDoc* pDoc, int vecPos)
{
    CHisto* pHisto = pDoc->GetVector()->GetHisto();
    AddHistoFunc(pHisto);
}

void CRun::SetToHistoDst(CDemoDoc* pDoc, int vecPos)
{
    CHisto* pHisto = pDoc->GetVector()->GetHisto();
    CMyString title = pDoc->GetTitle();
    if (NoDst()) return;
    CString vecName = GetVecName(vecPos);
    if (!vecName.IsEmpty())  vecName = _T(" - ") + vecName;
    pHisto->AddTail(title + vecName);

    pDoc->UpdateStatusFunc(m_Func + vecName, m_pPerf);
}

CString CRun::GetVecName(int vecPos)
{
    if (m_UsedVectors & VEC_DST2)
       return m_Func.VecName(vecPos,m_UsedVectors);
    else
       return _T("");
}

static CString getLibSuffix(int idx = 0)
{
    CString suffix;
    const IppLibraryVersion* libVer = DEMO_APP->GetLibVersion(idx);
    if (libVer) {
       suffix = libVer->targetCpu;
    }
    return suffix;
}

void CRun::AddHistoFunc(CHisto* pHisto, int vecPos)
{
    CString srcB;
    CString scale;
    CString parms = GetHistoryParms();
    CString info = GetHistoryInfo();
    CString funcParms;

    if (m_Func.Scale())
       scale.Format(_T("%d"),scaleFactor);
    if (NoDst() && (m_pDocSrc2))
       srcB = m_pDocSrc2->GetTitle();
    if (NoDst() && (m_pDocSrc3))
       srcB += _T(", ") + m_pDocSrc3->GetTitle();

    if (parms.Find(_T("..")) == -1) 
       funcParms += _T("..");
    if (!srcB.IsEmpty()) {
       if (!funcParms.IsEmpty()) funcParms += _T(", ");
       funcParms += srcB + _T(", ..");
    }
    if (!parms.IsEmpty()) {
       if (!funcParms.IsEmpty()) funcParms += _T(", ");
       funcParms += parms;
    }
    if (!scale.IsEmpty()) {
       if (!funcParms.IsEmpty()) funcParms += _T(", ");
       funcParms += scale;
    }
    if (funcParms == _T("..")) funcParms.Empty();

    pHisto->AddFuncString(m_Func, funcParms, info, 
                           m_pPerf, 
                           getLibSuffix());
}


BOOL CRun::NoDst()
{ 
    return !m_Func.Inplace() && !(m_UsedVectors & VEC_DST);
}



///////////////////////////////////////////////////////////////////////////
//         class CTmpDoc
///////////////////////////////////////////////////////////////////////////

class CTmpDoc
{
public:
    CTmpDoc() : ppDoc(NULL), pSrcDoc(NULL), pTmpDoc(NULL) {}
    CTmpDoc(CDemoDoc** pp, CDemoDoc* pSrc, CDemoDoc* pTmp);
    virtual ~CTmpDoc();
    void Release();

    CDemoDoc** ppDoc;
    CDemoDoc* pSrcDoc;
    CDemoDoc* pTmpDoc;
};

CTmpDoc::CTmpDoc(CDemoDoc** pp, CDemoDoc* pSrc, CDemoDoc* pTmp)
    : ppDoc(pp), pSrcDoc(pSrc), pTmpDoc(pTmp) 
{
    if (ppDoc && pTmpDoc)
        *ppDoc = pTmpDoc;
}

CTmpDoc::~CTmpDoc()
{
    Release();
    if (pTmpDoc) 
        pTmpDoc->OnCloseDocument();
}

void CTmpDoc::Release()
{
    if (ppDoc)
    {
        if (pSrcDoc)
            *ppDoc = pSrcDoc;
        ppDoc = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////
//         class CTmpDocList
///////////////////////////////////////////////////////////////////////////

INT_PTR CTmpDocList::Add(CDemoDoc** ppDoc, CDemoDoc* pSrcDoc, CDemoDoc* pTmpDoc)
{
    CTmpDoc* pDoc = new CTmpDoc(ppDoc, pSrcDoc, pTmpDoc);
    return CArray<CTmpDoc*>::Add(pDoc);
}

void CTmpDocList::ReleaseAll()
{
    for (int i=0; i<GetCount(); i++)
        GetAt(i)->Release();
}

void CTmpDocList::RemoveAll()
{
    for (int i=0; i<GetCount(); i++)
    {
        CTmpDoc* pDoc = GetAt(i);
        if (pDoc)
            delete pDoc;
    }
    CArray<CTmpDoc*>::RemoveAll();
}
