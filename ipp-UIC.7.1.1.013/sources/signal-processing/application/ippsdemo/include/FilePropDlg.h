/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FilePropDlg.h : interface for the File properties dialog.
// Command: Menu-File-Properties
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPROPDLG_H__F8EC1D19_B47D_4659_9B70_AE82BB0231EF__INCLUDED_)
#define AFX_FILEPROPDLG_H__F8EC1D19_B47D_4659_9B70_AE82BB0231EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilePropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilePropDlg dialog

class CippsDemoDoc;

class CFilePropDlg : public CDialog
{
// Construction
public:
   CFilePropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CippsDemoDoc* m_pDoc;
   //{{AFX_DATA(CFilePropDlg)
   enum { IDD = IDD_FILE_PROP };
   CString  m_FileName;
   CString  m_Type;
   int      m_Len;
   CStatic  m_IMaxStatic;
   CEdit    m_IMaxEdit;
   double   m_IMax;
   CStatic  m_RMaxStatic;
   CEdit    m_RMaxEdit;
   double   m_RMax;
   CStatic  m_IMinStatic;
   CEdit    m_IMinEdit;
   double   m_IMin;
   CStatic  m_RMinStatic;
   CEdit    m_RMinEdit;
   double   m_RMin;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CFilePropDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CFilePropDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEPROPDLG_H__F8EC1D19_B47D_4659_9B70_AE82BB0231EF__INCLUDED_)
