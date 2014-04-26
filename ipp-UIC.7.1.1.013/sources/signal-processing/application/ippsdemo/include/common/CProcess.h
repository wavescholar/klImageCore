/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Process.h: interface for the CProcess, CSubProcess, CBook, CChapter 
// classes.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
#define AFX_PROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"
#include "CFilter.h"

class CProcess;
class CFilter;

//////////////////////////////////////////////////////////////////////
// class CChapter provides classification for IPP library chapter.
// Chapter consists of functions.
// Functions are shown in the right column of Process Dialog sheet
// and chapters are shown in the middle column of Process Dialog sheet
//////////////////////////////////////////////////////////////////////

class CChapter : public CFuncList
{
public:
   // constructor / destructor
   CChapter(CString name = _T("")) : m_ChapterName(name) {}
   virtual ~CChapter() {}
   CChapter(const CChapter& chapter) { *this = chapter; }
   CChapter& operator=(const CChapter& chapter) {
      m_CurrFunc = chapter.m_CurrFunc;
      m_ChapterName = chapter.m_ChapterName;
      RemoveAll();
      AddTail((CFuncList*)&chapter);
      m_NameList.RemoveAll();
      m_NameList.AddTail((CStringList*)&(chapter.m_NameList));
      return *this;
   }
   friend class CSubProcess;
   // operations
   void AddBase(CString name) { m_NameList.AddTail(name);}
   int InitFunctions(CFuncList* pFuncList);
   void CopyFunctions(CChapter* pSrc);
   BOOL ApplyVector(CVector* pVec);
   BOOL ApplyFilter(const CFilter* pFilter);
   // attributes
   BOOL PresentBase(CString name);
   CFunc GetCurrentFunction(CFunc pairFunc = _T(""));
   void SetCurrentFunction(CFunc func);
   void SetChapterName(CString name) { m_ChapterName = name;}
   CString GetChapterName() { return m_ChapterName;}
   int GetFunctionsWithBase(CString baseName, CFuncList& funcList);
protected:
   CFunc m_CurrFunc;
   CString m_ChapterName;
   CStringList m_NameList;
};

//////////////////////////////////////////////////////////////////////
// class CBook provides classification for IPP library book.
// Book consists of chapters.
// Chapters are shown in the middle column of Process Dialog sheet
// and books are shown in the left column of Process Dialog sheet
//////////////////////////////////////////////////////////////////////

class CBook : public CArray<CChapter, CChapter&> {
public:
   CBook(CString name = _T("")) : 
      m_BookName(name), m_CurrChapter(-1) {}
   virtual ~CBook() {}
   CBook(const CBook& book) { *this = book; }
   CBook& operator=(const CBook& book) {
      m_BookName = book.m_BookName;
      m_CurrChapter = book.m_CurrChapter;
      RemoveAll();
      Append(book);
      return *this;
   }
   // operations
   CChapter* Add(CString name) { 
      CChapter chapter(name);
      int index = (int)CArray<CChapter, CChapter&>::Add(chapter); 
      return &(ElementAt(index));
   }
   friend class CSubProcess;
   BOOL InitFunctions(CFuncList* pFuncList);
   void CopyFunctions(CBook* pSrc);
   int GetFuncCount();
   // attributes
   CString GetName() { return m_BookName;}
   BOOL PresentBase(CString name);
   CFunc GetCurrentFunction(CFunc pairFunc = _T(""));
   void SetCurrentFunction(int nChapter, CFunc func);
   int GetCurrentChapter() { return m_CurrChapter;}
   void SetCurrentChapter(int nChapter);
   int GetFunctionsWithBase(CString baseName, CFuncList& funcList);
protected:
   int m_CurrChapter;
   CString m_BookName;

   BOOL ApplyVector(CVector* pVec);
   BOOL ApplyFilter(const CFilter* pFilter);
};

//////////////////////////////////////////////////////////////////////
// class CSubProcess provides classification for one IPP library.
// Library consists of books.
// Books are shown in the left column of Process Dialog sheet
// and each library has its own sheet
//////////////////////////////////////////////////////////////////////

class CSubProcess : public CArray<CBook, CBook&> {
public:
   CSubProcess();
   virtual ~CSubProcess() {}
   CSubProcess(const CSubProcess& process) { *this = process; }
   CSubProcess& operator=(const CSubProcess& process) {
      m_CurrBook = process.m_CurrBook;
      m_pParentProcess = process.m_pParentProcess;
      RemoveAll();
      Append(process);
      return *this;
   }
   // operations
   void InitFunctions(CFuncList* pFuncList); 
   void SetFunctions(CFuncList* pFuncList);
   void CopyFunctions(CSubProcess* pSrc);
   CBook* Add(CString name) { 
      CBook book(name);
      int index = (int)CArray<CBook, CBook&>::Add(book); 
      return &(ElementAt(index));
   }
   int GetFuncCount();
   void GetCurrentSettings(CSubProcess* pDstProcess);
   BOOL ApplyVectorNoSet(CVector* pVec);
   BOOL ApplyVector(CVector* pVec);
   BOOL ApplyFilter(const CFilter* pFilter, int libIdx = 0);
   // attributes
   void SetParentProcess(CSubProcess* ptr) { m_pParentProcess = ptr;}
   CSubProcess* GetParentProcess() { return m_pParentProcess;}
   BOOL PresentBase(CString name);
   int GetCurrentBook() { return m_CurrBook;}
   int GetCurrentChapter();
   CFunc GetCurrentFunction(CFunc pairFunc = _T(""));
   void SetCurrentBook(int nBook);
   void SetCurrentChapter(int nBook, int nChapter);
   void SetCurrentFunction(int nBook, int nChapter, CFunc func);
   int GetFunctionsWithBase(CString baseName, CFuncList& funcList);
protected:
   int m_CurrBook;
   CSubProcess* m_pParentProcess;
};

//////////////////////////////////////////////////////////////////////
// class CProcess provides classification for all IPP libraries.
// They are shown in Process Dialog
// and each library has its own sheet
//////////////////////////////////////////////////////////////////////

class CProcess : public CArray<CSubProcess, CSubProcess&> {
public:
//////////////////////////////////////////////////////////////////////
// Construction / destruction
   CProcess();
   virtual ~CProcess() {}
   CProcess(const CProcess& process) { *this = process; }
   CProcess& operator=(const CProcess& process) {
      m_CurrProcess = process.m_CurrProcess;
      RemoveAll();
      Append(process);
      return *this;
   }

// Overwrites
   virtual UINT GetIdFilter(int idx);

// Operations
   void InitFunctions();
   void SetFunctions();
   void CopyFunctions(CProcess* pSrc);
   CBook* Add(CString name, int idx = 0);
   BOOL GetFunction(CVector* pVec, CFunc& func) ;
   int GetFuncCount();
   void GetCurrentSettings(CProcess* pDstProcess);
   void SetCurrentSettings(CProcess* pSrcProcess);
   BOOL ApplyVectorNoSet(CVector* pVec);
   BOOL ApplyVector(CVector* pVec);
   BOOL ApplyFilter(const CFilter* pFilter);
// Attributes
   BOOL PresentBase(CString name);
   int GetCurrentProcess() { return m_CurrProcess;}
   int GetCurrentBook();
   int GetCurrentChapter();
   CFunc GetCurrentFunction(CFunc pairFunc = _T(""));
   void SetCurrentProcess(int nProcess);
   void SetCurrentBook(int nProcess, int nBook);
   void SetCurrentChapter(int nProcess, int nBook, int nChapter);
   void SetCurrentFunction(int nProcess, int nBook, int nChapter, CFunc func);
   void SetParentProcess(CProcess* ptr);
   int GetFunctionsWithBase(CString baseName, CFuncList& funcList);

   static CFilter GetFilter(int idx = 0) { return m_pFilter[idx];}
   static void SetFilter(CFilter filter, int idx = 0) { m_pFilter[idx] = filter;}

protected:
   int m_CurrProcess;
   static CFilter* m_pFilter;
};

#endif // !defined(AFX_PROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
