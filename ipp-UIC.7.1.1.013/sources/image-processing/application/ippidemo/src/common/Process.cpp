/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Process.cpp: implementation of the CProcess, CBook, CChapter.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "MainFrm.h"
#include "Director.h"
#include "CProcess.h"
#include "ProcessSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////
// CProcess class implementation.
// class CProcess provides classification for all IPP libraries.
// They are shown in Process Dialog
// and each library has its own sheet
///////////////////////////////////////////////////////////////////////

CFilter* CProcess::m_pFilter = NULL;

UINT CProcess::GetIdFilter(int idx) 
{ 
   return IDD_FILTER;
}

CProcess::CProcess() 
{
   m_CurrProcess = -1;
   CSubProcess pr;
   InsertAt(0,pr,DEMO_APP->GetNumLibs());
   if (m_pFilter == NULL)
      m_pFilter = new CFilter[DEMO_APP->GetNumLibs()];
}

void CProcess::InitFunctions() 
{ 
   for (int i=0; i<GetSize(); i++)
      ElementAt(i).InitFunctions(DEMO_APP->GetIppList() + i); 
}

void CProcess::SetFunctions() 
{
   for (int i=0; i<GetSize(); i++)
      ElementAt(i).SetFunctions(DEMO_APP->GetIppList() + i); 
}

void CProcess::CopyFunctions(CProcess* pSrc) 
{
   for (int i=0; i<GetSize(); i++)
      ElementAt(i).CopyFunctions(&(pSrc->ElementAt(i))); 
}

CBook* CProcess::Add(CString name, int idx) 
{
   if (idx >= GetSize()) return NULL;
   return ElementAt(idx).Add(name);
}

int CProcess::GetFuncCount()
{ 
   int n = 0;
   for (int i=0; i<GetSize(); i++)
      n += (*this)[i].GetFuncCount();
   return n;
}

void CProcess::GetCurrentSettings(CProcess* pDst)
{
   pDst->m_CurrProcess = m_CurrProcess;
   for (int i=0; i<GetSize(); i++) 
      (*this)[i].GetCurrentSettings(&(pDst->ElementAt(i)));
}

void CProcess::SetCurrentSettings(CProcess* pSrcProcess)
{
   pSrcProcess->GetCurrentSettings(this);
}

BOOL CProcess::ApplyVectorNoSet(CVector* pVec)
{
   BOOL result = FALSE;
   for (int i=0; i < GetSize(); i++) {
      if (ElementAt(i).ApplyVector(pVec))
         result = TRUE;
   }
   return result;
}

BOOL CProcess::ApplyVector(CVector* pVec)
{
   SetFunctions();
   return ApplyVectorNoSet(pVec);
}

BOOL CProcess::ApplyFilter(const CFilter* pFilter)
{
   BOOL result = FALSE;
   for (int i=0; i < GetSize(); i++) {
      if (ElementAt(i).ApplyFilter(pFilter))
         result = TRUE;
   }
   return result;
}

// attributes

void CProcess::SetParentProcess(CProcess* pSrc)
{
   for (int i=0; i<GetSize(); i++)
      (*this)[i].SetParentProcess(&(pSrc->ElementAt(i)));
}

BOOL CProcess::PresentBase(CString name)
{
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).PresentBase(name)) return TRUE;
   }
   return FALSE;
}

int CProcess::GetCurrentBook() 
{ 
   if ((m_CurrProcess < 0) ||
       (m_CurrProcess >= GetSize())) return -1; 
   return ElementAt(m_CurrProcess).GetCurrentBook();
}

int CProcess::GetCurrentChapter()
{ 
   if ((m_CurrProcess < 0) ||
       (m_CurrProcess >= GetSize())) return -1; 
   return ElementAt(m_CurrProcess).GetCurrentChapter();
}

CFunc CProcess::GetCurrentFunction(CFunc pairFunc)
{ 
   if ((m_CurrProcess < 0) ||
       (m_CurrProcess >= GetSize())) return _T(""); 
   return ElementAt(m_CurrProcess).GetCurrentFunction(pairFunc);
}

void CProcess::SetCurrentProcess(int nProcess)
{
   if ((nProcess >= 0) && (nProcess < GetSize()))
      m_CurrProcess = nProcess;
   else if (GetSize() > 0)
      m_CurrProcess = 0;
   else
      m_CurrProcess = -1;
}

void CProcess::SetCurrentBook(int nProcess, int nBook)
{
   SetCurrentProcess(nProcess);
   if (m_CurrProcess >= 0)
      ElementAt(m_CurrProcess).SetCurrentBook(nBook);
}

void CProcess::SetCurrentChapter(int nProcess, int nBook, int nChapter)
{
   SetCurrentProcess(nProcess);
   if (m_CurrProcess >= 0)
      ElementAt(m_CurrProcess).SetCurrentChapter(nBook, nChapter);
}

void CProcess::SetCurrentFunction(int nProcess, int nBook, int nChapter, CFunc func)
{
   SetCurrentProcess(nProcess);
   if (m_CurrProcess >= 0)
      ElementAt(m_CurrProcess).SetCurrentFunction(nBook, nChapter, func);
}

int CProcess::GetFunctionsWithBase(CString baseName, CFuncList& funcList)
{
   funcList.RemoveAll();
   int result = 0;
   for (int i=0; i<GetSize(); i++) {
      CFuncList subList;
      result += ElementAt(i).GetFunctionsWithBase(baseName, subList);
      funcList.AddTail(&subList);
   }
   return result;
}

BOOL CProcess::GetFunction(CVector* pVec, CFunc& func)
{
   CProcessSheet sheet(_T("Choose function"));
   sheet.CreatePages(this,pVec);

   if (sheet.StartFilterProblem()) {
      CProcessSheet tmpSheet(_T("Choose function"));
      tmpSheet.CreatePages(this,pVec);
      tmpSheet.Create(MAIN_FRAME);
      if (tmpSheet.OnFilter()) {
         tmpSheet.OnOK();
         sheet.CreatePages(this, pVec);
      }
      tmpSheet.EndDialog(IDCANCEL);
   }
   if (sheet.DoModal() != IDOK)
      return FALSE;
   func = sheet.m_Func;
   return TRUE;
}

///////////////////////////////////////////////////////////////////////
// CSubProcess class implementation
// class CSubProcess provides classification for one IPP library.
// Library consists of books.
// Books are shown in the left column of Process Dialog sheet
// and each library has its own sheet
///////////////////////////////////////////////////////////////////////

CSubProcess::CSubProcess() 
{
   m_CurrBook = -1;
   m_pParentProcess = NULL;
}

void CSubProcess::InitFunctions(CFuncList* pFuncList)
{
   for (int nBook=0; nBook < GetSize(); nBook++)
      ElementAt(nBook).InitFunctions(pFuncList);
}

void CSubProcess::SetFunctions(CFuncList* pFuncList) 
{
   if (m_pParentProcess)
      CopyFunctions(m_pParentProcess);
   else
      InitFunctions(pFuncList); 
}

BOOL CSubProcess::ApplyVector(CVector* pVec)
{
   BOOL result = FALSE;
   for (int nBook=0; nBook < GetSize(); nBook++) {
      if (ElementAt(nBook).ApplyVector(pVec))
         result = TRUE;
   }
   return result;
}

BOOL CSubProcess::ApplyFilter(const CFilter* pFilter, int libIdx)
{
   SetFunctions(DEMO_APP->GetIppList() + libIdx);
   BOOL result = FALSE;
   for (int nBook=0; nBook < GetSize(); nBook++) {
      if (ElementAt(nBook).ApplyFilter(pFilter))
         result = TRUE;
   }
   return result;
}

int CSubProcess::GetFuncCount()
{
   int result = 0;
   for (int nBook=0; nBook < GetSize(); nBook++) {
      result += GetAt(nBook).GetFuncCount();
   }
   return result;
}

void CSubProcess::CopyFunctions(CSubProcess* pSrc)
{
   for (int i = 0; i < GetSize(); i++) {
      ElementAt(i).CopyFunctions(&(pSrc->ElementAt(i)));
   }
}

void CSubProcess::GetCurrentSettings(CSubProcess* pDst)
{
   pDst->m_CurrBook = m_CurrBook;
   for (int nBook = 0; nBook < GetSize(); nBook++) {
      CBook* pSrcBook = &(ElementAt(nBook));
      CBook* pDstBook = &(pDst->ElementAt(nBook));
      pDstBook->m_CurrChapter = pSrcBook->m_CurrChapter;
      for (int nChapter = 0; nChapter < pSrcBook->GetSize(); nChapter++) {
         CChapter* pSrcChapter = &(pSrcBook->ElementAt(nChapter));
         CChapter* pDstChapter = &(pDstBook->ElementAt(nChapter));
         pDstChapter->m_CurrFunc = pSrcChapter->m_CurrFunc;
      }
   }
}

BOOL CSubProcess::PresentBase(CString name)
{
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).PresentBase(name)) return TRUE;
   }
   return FALSE;
}

CFunc CSubProcess::GetCurrentFunction(CFunc pairFunc) {
   if ((m_CurrBook < 0) ||
       (m_CurrBook >= GetSize())) return _T(""); 
   return GetAt(m_CurrBook).GetCurrentFunction(pairFunc);
}

void CSubProcess::SetCurrentFunction(int nBook, int nChapter, CFunc func) {
   SetCurrentBook(nBook);
   if (m_CurrBook >= 0 && m_CurrBook < GetSize())
      ElementAt(m_CurrBook).SetCurrentFunction(nChapter, func);
}

int CSubProcess::GetCurrentChapter() {
   if ((m_CurrBook < 0) ||
       (m_CurrBook >= GetSize())) return -1; 
   return GetAt(m_CurrBook).GetCurrentChapter();
}

void CSubProcess::SetCurrentChapter(int nBook, int nChapter) {
   SetCurrentBook(nBook);
   if (m_CurrBook >= 0 && m_CurrBook < GetSize())
      ElementAt(m_CurrBook).SetCurrentChapter(nChapter);
}

void CSubProcess::SetCurrentBook(int nBook) {
   if ((nBook >= 0) && (nBook < GetSize()))
      m_CurrBook = nBook;
   else if (GetSize() > 0)
      m_CurrBook = 0;
   else
      m_CurrBook = -1;
}

int CSubProcess::GetFunctionsWithBase(CString baseName, CFuncList& funcList)
{
   funcList.RemoveAll();
   int result = 0;
   for (int i=0; i<GetSize(); i++) {
      result += ElementAt(i).GetFunctionsWithBase(baseName, funcList);
   }
   return result;
}

///////////////////////////////////////////////////////////////////////
// CBook class implementation
// class CBook provides classification for IPP library book.
// Book consists of chapters.
// Chapters are shown in the middle column of Process Dialog sheet
// and books are shown in the left column of Process Dialog sheet
///////////////////////////////////////////////////////////////////////

BOOL CBook::InitFunctions(CFuncList* pFuncList)
{
   m_CurrChapter = 0;
   BOOL result = FALSE;
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).InitFunctions(pFuncList)) result = TRUE;
   }
   return result;
}

void CBook::CopyFunctions(CBook* pSrc)
{
   for (int i = 0; i < GetSize(); i++) {
      ElementAt(i).CopyFunctions(&(pSrc->ElementAt(i)));
   }
}

BOOL CBook::PresentBase(CString name)
{
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).PresentBase(name)) return TRUE;
   }
   return FALSE;
}

int CBook::GetFuncCount()
{
   int result = 0;
   for (int nChapter=0; nChapter < GetSize(); nChapter++) {
      result += (int)GetAt(nChapter).GetCount();
   }
   return result;
}

BOOL CBook::ApplyVector(CVector* pVec)
{
   BOOL result = FALSE;
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).ApplyVector(pVec)) result = TRUE;
   }
   return result;
}

BOOL CBook::ApplyFilter(const CFilter* pFilter)
{
   BOOL result = FALSE;
   for (int i=0; i<GetSize(); i++) {
      if (ElementAt(i).ApplyFilter(pFilter)) result = TRUE;
   }
   return result;
}

CFunc CBook::GetCurrentFunction(CFunc pairFunc) {
   if ((m_CurrChapter < 0) ||
       (m_CurrChapter >= GetSize())) return _T(""); 
   return GetAt(m_CurrChapter).GetCurrentFunction(pairFunc);
}

void CBook::SetCurrentFunction(int nChapter, CFunc func) {
   SetCurrentChapter(nChapter);
   if ((m_CurrChapter >= 0) && (m_CurrChapter < GetSize()))
      ElementAt(m_CurrChapter).SetCurrentFunction(func);
}

void CBook::SetCurrentChapter(int nChapter) {      
   if (nChapter < 0 || nChapter >= GetSize())
      nChapter = GetSize() ? 0 : -1;
   if ((nChapter >= 0) && (GetAt(nChapter).GetCount() == 0)) {
      nChapter = -1;
      for (int n=0; n < GetSize(); n++) {
         if (GetAt(n).GetCount()) {
            nChapter = n;
            break;
         }
      }
   }
   m_CurrChapter = nChapter;
}

int CBook::GetFunctionsWithBase(CString baseName, CFuncList& funcList)
{
   int result = 0;
   for (int i=0; i<GetSize(); i++) {
      result += ElementAt(i).GetFunctionsWithBase(baseName, funcList);
   }
   return result;
}

///////////////////////////////////////////////////////////////////////
// CChapter class implementation
// class CChapter provides classification for IPP library chapter.
// Chapter consists of functions.
// Functions are shown in the right column of Process Dialog sheet
// and chapters are shown in the middle column of Process Dialog sheet
///////////////////////////////////////////////////////////////////////

int CChapter::InitFunctions(CFuncList* pFuncList) 
{
   RemoveAll();
   POSITION pos = pFuncList->GetHeadPosition();
   while (pos) {
      CFunc func = pFuncList->GetNext(pos);
      if (PresentBase(func.BaseName())) AddTail(func);
   }
   return (int)GetCount();
}

void CChapter::CopyFunctions(CChapter* pSrc)
{
   RemoveAll();
   POSITION pos = pSrc->GetHeadPosition();
   while (pos) {
      AddTail(pSrc->GetNext(pos));
   }
}

BOOL CChapter::PresentBase(CString name)
{
   POSITION pos = m_NameList.GetHeadPosition();
   while (pos) {
      if (name == m_NameList.GetNext(pos))
         return TRUE;
   }
   return FALSE;
}

int CChapter::ApplyVector(CVector* pVec) 
{
   POSITION pos = GetHeadPosition();
   while (pos) {
      POSITION delPos = pos;
      CFunc func = GetNext(pos);
      if (!func.Valid(pVec)) {
         RemoveAt(delPos);
      }
   }
   return (int)GetCount();
}

int CChapter::ApplyFilter(const CFilter* pFilter) 
{
   if (!pFilter) return TRUE;
   POSITION pos = GetHeadPosition();
   while (pos) {
      POSITION delPos = pos;
      if (!pFilter->Apply(GetNext(pos))) {
         RemoveAt(delPos);
      }
   }
   return (int)GetCount();
}

enum { M_BASE, M_DST, M_I, M_R, M_S};

static BOOL select(CFuncList& funcList, CFunc currFunc, int mFlag)
{
   CFuncList tmpList;
   POSITION pos = funcList.GetHeadPosition();
   while (pos) {
      CFunc func = funcList.GetNext(pos);
      switch (mFlag) {
      case M_BASE:  if (func.BaseName() == currFunc.BaseName()) break;
                    continue;
      case M_DST:   if (func.DstType() == currFunc.SrcType()) break;
                    continue;
      case M_I:     if (func.Inplace() == currFunc.Inplace()) break;
                    continue;
      case M_R:     if (func.Roi() == currFunc.Roi()) break;
                    continue;
      case M_S:     if (func.Scale() == currFunc.Scale()) break;
                    continue;
      default:      continue;
      }
      tmpList.AddTail(func);
   }
   if (tmpList.GetCount() == 0) return FALSE;
   funcList.RemoveAll();
   funcList.AddTail(&tmpList);
   return TRUE;
}

CFunc CChapter::GetCurrentFunction(CFunc pairFunc) 
{
   if (GetCount() == 0) return _T("");
   pairFunc = pairFunc.GetPair();
   if (Find(pairFunc)) {
      return pairFunc;
   }
   if (Find(m_CurrFunc)) return m_CurrFunc;
   CFuncList tmpList;
   tmpList.AddTail(this);
   if (!select(tmpList,m_CurrFunc, M_BASE)) return _T("");
   select(tmpList,m_CurrFunc, M_DST);
   select(tmpList,m_CurrFunc, M_I);
   select(tmpList,m_CurrFunc, M_R);
   select(tmpList,m_CurrFunc, M_S);
   return tmpList.GetAt(tmpList.GetHeadPosition());
}

void CChapter::SetCurrentFunction(CFunc func) 
{
   m_CurrFunc = func;
}

int CChapter::GetFunctionsWithBase(CString baseName, CFuncList& funcList)
{
   if (!PresentBase(baseName)) return 0;
   int result = 0;
   POSITION pos = GetHeadPosition();
   while (pos) {
      CFunc func = GetNext(pos);
      if (func.BaseName() == baseName) {
         funcList.AddTail(func);
         result++;
      }
   }
   return result;
}