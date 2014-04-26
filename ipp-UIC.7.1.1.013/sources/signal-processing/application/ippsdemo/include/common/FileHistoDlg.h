/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FileHistoDlg.h : interface for the History dialog.
// Command: Menu-File-History
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEHISTODLG_H__0141F361_35B4_4F5A_9D50_1573DCD2C1EA__INCLUDED_)
#define AFX_FILEHISTODLG_H__0141F361_35B4_4F5A_9D50_1573DCD2C1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileHistoDlg dialog

class CFileHistoDlg : public CDialog
{
// Construction
public:
   CFileHistoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CString m_DocTitle;
   CStringList* m_pHistoList;
   //{{AFX_DATA(CFileHistoDlg)
   enum { IDD = IDD_FILE_HISTO };
   CEdit m_HistoEdit;
   CEdit m_TitleEdit;
   CString  m_Title;
   CString  m_HistoStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CFileHistoDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CFileHistoDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEHISTODLG_H__0141F361_35B4_4F5A_9D50_1573DCD2C1EA__INCLUDED_)
