/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Histo.h: interface for the CHisto class.
//
// class CHisto provides history information for Demo document
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTO_H__7F5953C5_E802_4F6F_86C0_12C15969A72A__INCLUDED_)
#define AFX_HISTO_H__7F5953C5_E802_4F6F_86C0_12C15969A72A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"

class CVector;
class CPerf;

class CHisto : public CStringList  
{
public:
   CHisto();
   virtual ~CHisto();

   void Init(CString title) { RemoveAll(); AddTail(title);}
   void AddFuncString(CFunc func, CString funcParms = _T(""), CString funcInfo = _T(""), 
                      CPerf* pPerf = 0, CString libSuffix = _T(""));

   void LastFunc(CFunc func) { m_LastFunc = func;}
   CFunc LastFunc() { return m_LastFunc;}
   void LastVec2(CString name) { m_LastVec2 = name;}
   CString LastVec2() { return m_LastVec2;}
protected:
   CFunc   m_LastFunc;
   CString m_LastVec2;

};

#endif // !defined(AFX_HISTO_H__7F5953C5_E802_4F6F_86C0_12C15969A72A__INCLUDED_)
