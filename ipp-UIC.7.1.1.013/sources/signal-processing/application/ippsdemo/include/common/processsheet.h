/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ProcessSheet.h : interface for the Process dialog sheet.
// Command: Menu-Process-Process by
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSSHEET_H__1984F215_6391_475A_BA9E_4F8D445088B7__INCLUDED_)
#define AFX_PROCESSSHEET_H__1984F215_6391_475A_BA9E_4F8D445088B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessSheet.h : header file
//

#include "Func.h"
class CProcess;
class CVector;

/////////////////////////////////////////////////////////////////////////////
// CProcessSheet

class CProcessSheet : public CPropertySheet
{
   DECLARE_DYNAMIC(CProcessSheet)

// Construction
public:
   CProcessSheet(LPCTSTR caption);
   void CreatePages(CProcess* pProcess, CVector* pVector);
   void DeletePages();

// Attributes
public:
   CProcess* m_pProcess;
   CFunc     m_Func;
   int       m_startPage;

   BOOL StartFilterProblem();

// Operations
public:
   BOOL OnFilter();
   void OnOK();

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CProcessSheet)
   protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CProcessSheet();

   // Generated message map functions
protected:
   //{{AFX_MSG(CProcessSheet)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSSHEET_H__1984F215_6391_475A_BA9E_4F8D445088B7__INCLUDED_)
