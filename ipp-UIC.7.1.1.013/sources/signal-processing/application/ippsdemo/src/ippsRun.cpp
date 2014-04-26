/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsRun.cpp : implementation of the CippsRun class.
// CippsRun is derived from CRun class that provides CDemo documents
// processing by IPP functions.
// CippsRun is the base class for all ippsDemo Run classes that process 
// vectors by concrete ippSP functions. 
// See CRun class for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ippsDemoView.h"
#include "ippsRun.h"
#include "ippsProcess.h"
#include "ParamDlg.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "Histo.h"
#include "Timing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CippsRun::CippsRun()
{
   m_ViewMode = VIEW_DEMO;
   pSrc  = NULL;
   pSrc2 = NULL;
   pSrc3 = NULL;
   pDst  = NULL;
   pDst2 = NULL;
   len = 0;
   m_lenSrc  = 0;
   m_lenSrc2 = 0;
   m_lenSrc3 = 0;
   m_lenDst  = 0;
   m_lenDst2 = 0;
   m_iOne = 0;
   m_bOne = FALSE;
}

CippsRun::~CippsRun()
{
}

BOOL CippsRun::Open(CFunc func)
{
   if (!CRun::Open(func)) return FALSE;
   m_bOne = func.Found("One");
   return TRUE;
}

void CippsRun::UpdateData(CParamDlg* pDlg, BOOL save)
{
   CRun::UpdateData(pDlg, save);
   if (!save)
      pDlg->m_UsedVectors = m_UsedVectors & ~(VEC_DST | VEC_DST2);
}

BOOL CippsRun::ProcessFunctionStart(CDemoDoc *pDoc, CString funcName)
{
   if (!CRun::ProcessFunctionStart(pDoc, funcName))
      return FALSE;
   CippsDemoDoc* pDocSrc = (CippsDemoDoc*)m_pDocSrc;
   CView* pActiveView = pDocSrc->GetActiveView();
   m_ViewMode = pActiveView == (CView*)pDocSrc->GetDemoView() ? VIEW_DEMO :
                pActiveView == (CView*)pDocSrc->GetCharView() ? VIEW_CHAR :
                pActiveView == (CView*)pDocSrc->GetTextView() ? VIEW_TEXT :
                                                                VIEW_DEMO;
   return TRUE;
}

BOOL CippsRun::PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
   if (m_PickVecPos == VEC_SRC3) {
      vectorName = m_Func.VecName(VEC_SRC3,m_UsedVectors);
      if (m_Func.Inplace()) {
         vectorName = "SrcDst";
         pickID = IDC_CURSOR_PSD;
         grabID = IDC_CURSOR_SD;
      } else {
         vectorName = "Src3";
         pickID = IDC_CURSOR_PS3;
         grabID = IDC_CURSOR_S3;
      }
   } else if (m_PickVecPos == VEC_SRC2) {
      vectorName = m_Func.VecName(VEC_SRC2,m_UsedVectors);
      if (vectorName.Found("Im")) {
         pickID = IDC_CURSOR_PIM;
         grabID = IDC_CURSOR_IM;
      } else if (vectorName.Found("Phase")) {
         pickID = IDC_CURSOR_PPH;
         grabID = IDC_CURSOR_PH;
      } else if (vectorName.Found("High")) {
         pickID = IDC_CURSOR_PHI;
         grabID = IDC_CURSOR_HI;
      } else if (vectorName.Found("Ref")) {
         pickID = IDC_CURSOR_PR;
         grabID = IDC_CURSOR_R;
      } else if (!(m_UsedVectors & VEC_SRC3) && m_Func.Inplace()) {
         vectorName = "SrcDst";
         pickID = IDC_CURSOR_PSD;
         grabID = IDC_CURSOR_SD;
      } else {
         vectorName = "Src2";
         pickID = IDC_CURSOR_PS2;
         grabID = IDC_CURSOR_S2;
      }
   } else {
      return FALSE;
   }
   return TRUE;
}

BOOL CippsRun::CallIpp(BOOL bMessage)
{
   if (m_Func.Found("One"))
      return CallIppOne(bMessage);
   else
      return CRun::CallIpp(bMessage);
}

BOOL CippsRun::CallIppOne(BOOL bMessage)
{
   IppStatus funcError = ippStsNoErr;
   IppStatus funcWarning = ippStsNoErr;
   try {
      for (m_iOne=0; m_iOne<len; m_iOne++) {
         IppStatus result = CallIppFunction();
         if (result < funcError) funcError = result;
         if (result > funcWarning) funcWarning = result;
      }
   } catch (CSExcept except) {
      ExcIppMessage(except);
      return FALSE;
   }
   if (funcError == ippStsNoErr) funcError = funcWarning;
   if (bMessage)
      IppErrorMessage(funcError);
   return funcError >= 0 ? TRUE : FALSE;
}

BOOL CippsRun::PrepareDst()
{
   if (!CreateNewDst(m_UsedVectors & VEC_DST,
                     GetDstLength(),
                     m_NewDst, m_pDocDst))

      return FALSE;
   if (!CreateNewDst(m_UsedVectors & VEC_DST2,
                     GetDst2Length(),
                     m_NewDst2, m_pDocDst2))
      return FALSE;
   return TRUE;
}

int CippsRun::GetDstLength()
{
   int length = ((CippsDemoDoc*)m_pDocSrc)->Length();
   if (m_pDocSrc2) {
      if (length > ((CippsDemoDoc*)m_pDocSrc2)->Length())
         length = ((CippsDemoDoc*)m_pDocSrc2)->Length();   
   }
   if (m_pDocSrc3) {
      if (length > ((CippsDemoDoc*)m_pDocSrc3)->Length())
         length = ((CippsDemoDoc*)m_pDocSrc3)->Length();   
   }
   return length;
}

int CippsRun::GetDst2Length()
{
   return GetDstLength();
}

BOOL CippsRun::CreateNewDst(BOOL bUsed, int length, BOOL& bNew, CDemoDoc*& pDoc)
{
   bNew = FALSE;
   if (!bUsed) return TRUE;
   if (pDoc) return TRUE;
   ppType dstType = m_Func.DstType();
   if (CippsProcess::DepthEssencial(m_Func)) {
      ppType srcType = m_pDocSrc->GetVector()->Type();
      if ((srcType & PP_CPLX) == (dstType & PP_CPLX)) {
         dstType = srcType;
      } else if (srcType & PP_CPLX) {
         dstType = (ppType)(srcType & ~PP_CPLX);
      } else {
         if (!(srcType & PP_FLOAT)) srcType = (ppType)(srcType | PP_SIGN);
         dstType = (ppType)(srcType | PP_CPLX);
      }
   }
   pDoc = m_ViewMode == VIEW_DEMO ? DEMO_APP->CreateDemoDoc(dstType,length) :
          m_ViewMode == VIEW_TEXT ? DEMO_APP->CreateTextDoc(dstType,length) :
          m_ViewMode == VIEW_CHAR ? DEMO_APP->CreateCharDoc(dstType,length) : 
          NULL;
   if (!pDoc) return FALSE;
   pDoc->GetFrame()->ShowWindow(SW_HIDE);
   bNew = TRUE;
   return TRUE;
}

void CippsRun::DeleteNewDst()
{
    if (m_NewDst)
       m_pDocDst->OnCloseDocument();
    if (m_NewDst2)
       m_pDocDst2->OnCloseDocument();
}

void CippsRun::ActivateDoc(CippsDemoDoc* pDoc)
{
   if (!pDoc) return;
   CMDIChildWnd* pTextFrame = (CMDIChildWnd*)pDoc->GetTextFrame();
   CMDIChildWnd* pCharFrame = (CMDIChildWnd*)pDoc->GetCharFrame();
   CMDIChildWnd* pFrame = (CMDIChildWnd*)pDoc->GetDemoFrame();
   if (!pFrame) {
      pFrame = pCharFrame;
      if (!pFrame)
         pFrame = pTextFrame;
   }
   if ((m_ViewMode == VIEW_TEXT) && pTextFrame)
      pFrame = pTextFrame;
   else if ((m_ViewMode == VIEW_CHAR) && pCharFrame)
      pFrame = pCharFrame;
   ASSERT(pFrame);
   pDoc->Activate(pFrame);
}

static double ScaleWidth(CippsDemoDoc** ppDoc, int num)
{
   int width = MAIN_FRAME->GetClientWidth() 
             - ppDoc[0]->GetDemoFrame()->GetBorderWidth()
             - ppDoc[0]->GetDemoView()->GetBorderWidth();
   double scale = ppDoc[0]->FactorW();
   double length = ppDoc[0]->Length();
   for (int i=1; i<num; i++) {
      if (scale < ppDoc[i]->FactorW()) scale = ppDoc[i]->FactorW();
      if (length < ppDoc[i]->Length()) length = ppDoc[i]->Length();
   }
   if (scale*length >= width)
      scale = width/length;
   return scale;
}

static void AddAmplBorder(CippsDemoDoc* pDoc, double& ampl, int& border)
{
   if (!pDoc) return;
   CippsDemoView* pView = pDoc->GetDemoView();
   if (!pView) return;
   ampl   += pView->GetInitialHeight() - pView->GetBorderHeight();
   border += pView->GetBorderHeight()
          +  pDoc->GetDemoFrame()->GetBorderHeight();
}

static double ScaleHeight(CippsDemoDoc** ppDoc, int num)
{
   double ampl = 0;
   int border = 0;
   for (int i=0; i <num; i++)
      AddAmplBorder(ppDoc[i],  ampl, border);
   int height = MAIN_FRAME->GetClientHeight();
   if (ampl + border < height) return 1;
   return (double)(height - border) / ampl;
}

static int getStandardWidth(CippsDemoDoc* pDoc)
{
   CippsDemoView* pView = pDoc->GetDemoView();
   if (pView == NULL) return 0;
   CChildFrame* pFrame = pDoc->GetDemoFrame();
   return pView->GetInitialWidth()  + pFrame->GetBorderWidth();
}

static int getStandardHeight(CippsDemoDoc* pDoc)
{
   CippsDemoView* pView = pDoc->GetDemoView();
   if (pView == NULL) return 0;
   CChildFrame* pFrame = pDoc->GetDemoFrame();
   return pView->GetInitialHeight() + pFrame->GetBorderHeight();
}

static void ResizeDocs(CippsDemoDoc** ppDoc, int num)
{
   if (num == 1) {
      ppDoc[0]->ZoomToRect();
      return;
   }
   int i;
   int height = 0;
   for (i=0; i<num; i++) {
      height += getStandardHeight(ppDoc[i]);
   }
   int mainHeight = MAIN_FRAME->GetClientHeight();
   double scale = 1;
   if (height > mainHeight)
      scale = (double)mainHeight / (double)height;
   int y = 0;
   for (i=0; i<num; i++) {
      height = (int)(scale*getStandardHeight(ppDoc[i]) + 0.5);
      if (height) {
         CRect rect(0, y, getStandardWidth(ppDoc[i]), y + height);
         ppDoc[i]->ZoomToRect(&rect);
         ppDoc[i]->ZoomToRect(&rect);
      }
      y += height;
   }
}

void CippsRun::ActivateDst()
{
   CippsDemoDoc** ppDoc = new CippsDemoDoc*[5];
   int num = 0;
   AddToDocs((CippsDemoDoc*)m_pDocSrc , ppDoc, num);
   AddToDocs((CippsDemoDoc*)m_pDocSrc2, ppDoc, num);
   AddToDocs((CippsDemoDoc*)m_pDocSrc3, ppDoc, num);
   AddToDocs((CippsDemoDoc*)m_pDocDst , ppDoc, num);
   AddToDocs((CippsDemoDoc*)m_pDocDst2, ppDoc, num);
   ActivateDocs(ppDoc, num);
   delete[] ppDoc;
}

void CippsRun::AddToDocs(CippsDemoDoc* pDoc, CippsDemoDoc** ppDoc, int& num)
{
   if (pDoc == NULL) return;
   for (int i=0; i<num; i++) {
      if (pDoc == ppDoc[i]) return;
   }
   ppDoc[num++] = pDoc;
}

void CippsRun::ActivateDocs(CippsDemoDoc** ppDoc, int num)
{
   for (int i=0; i<num; i++)
      ActivateDoc(ppDoc[i]);
   if (!NoDst() && (m_ViewMode == VIEW_DEMO))
      ResizeDocs(ppDoc,num);
}

void CippsRun::SetParams(CVector* pVec, void*& pointer, int& len, int& lenParm)
{
   if (pVec) {
      pointer = pVec->GetData();
      len = pVec->Length();
      if (lenParm == 0) lenParm = len;
      else if (lenParm > len) lenParm = len;
   } else {
      pointer = NULL;
   }
}

void CippsRun::SetParams(CVector* pVec, void*& pointer, int& len)
{
   int lenParm = 0;
   SetParams(pVec,pointer,len,lenParm);
}

void CippsRun::PrepareParameters()
{
    len = 0;
    SetParams((CippsDemoDoc*)m_pDocSrc ,pSrc , m_lenSrc,  len);
    SetParams((CippsDemoDoc*)m_pDocSrc2,pSrc2, m_lenSrc2, len);
    SetParams((CippsDemoDoc*)m_pDocSrc3,pSrc3, m_lenSrc3, len);
    SetParams((CippsDemoDoc*)m_pDocDst ,pDst , m_lenDst);
    SetParams((CippsDemoDoc*)m_pDocDst2,pDst2, m_lenDst2);
}

void CippsRun::SaveInplaceVector()
{
   if (m_pDocSrc3)
      m_InplaceVector = *((CVector*)(CippsDemoDoc*)m_pDocSrc3);
   else if (m_pDocSrc2)
      m_InplaceVector = *((CVector*)(CippsDemoDoc*)m_pDocSrc2);
   else
      m_InplaceVector = *((CVector*)(CippsDemoDoc*)m_pDocSrc);
}

void CippsRun::SetInplaceParms()
{
   if (m_pDocSrc3)
      SetParams(&m_InplaceVector,pSrc3,m_lenSrc3);
   else if (m_pDocSrc2)
      SetParams(&m_InplaceVector,pSrc2,m_lenSrc2);
   else
      SetParams(&m_InplaceVector,pSrc,m_lenSrc);
}

void CippsRun::ResetInplaceParms()
{
   if (m_pDocSrc3)
      SetParams((CippsDemoDoc*)m_pDocSrc3,pSrc3,m_lenSrc3);
   else if (m_pDocSrc2)
      SetParams((CippsDemoDoc*)m_pDocSrc2,pSrc2,m_lenSrc2);
   else
      SetParams((CippsDemoDoc*)m_pDocSrc,pSrc,m_lenSrc);
}

void CippsRun::Loop(int num)
{
   if (m_bOne)
      LoopOne(num);
   else
      CRun::Loop(num);
}

void CippsRun::LoopOne(int num)
{
   while (num--) {
      for (m_iOne=0; m_iOne<len; m_iOne++) {
         CallIppFunction();
      }
   }
}

double CippsRun::GetPerfDivisor(CString& unitString)
{
   unitString = "elmnt";
   CippsDemoDoc* pDoc = (CippsDemoDoc*)(m_pDocDst ? m_pDocDst : m_pDocSrc);
   if (!pDoc) return 1;
   return pDoc->Length();
}

