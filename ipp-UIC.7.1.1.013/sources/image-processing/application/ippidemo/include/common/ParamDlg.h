/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParamDlg.h : interface for the CParamDlg class.
// CParamDlg is the base class for all dialog classes that get IPP
// function parameters.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMDLG_H__7D177118_A60E_4A26_A8C4_C8099F0FC6D8__INCLUDED_)
#define AFX_PARAMDLG_H__7D177118_A60E_4A26_A8C4_C8099F0FC6D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog

#define NAME_NEW _T("<new>")

class CDemoDoc;

class CParamDlg : public CDialog
{
   DECLARE_DYNAMIC(CParamDlg)
// Construction
public:
   CParamDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor
   int     m_UsedVectors;
   CFunc   m_Func;
   CDemoDoc* m_pDocSrc;
   CDemoDoc* m_pDocSrc2;
   CDemoDoc* m_pDocSrc3;
   CDemoDoc* m_pDocMask;
   CDemoDoc* m_pDocDst;
   CDemoDoc* m_pDocDst2;

// Dialog Data
   //{{AFX_DATA(CParamDlg)
   enum { IDD = IDD_PARAM };
   CString     m_StrSrc;
   CString     m_StrSrc2;
   CString     m_StrSrc3;
   CString     m_StrMask;
   CString     m_StrDst;
   CString     m_StrDst2;
   CEdit    m_EditSrc;
   CEdit    m_EditSrc2;
   CEdit    m_EditSrc3;
   CEdit    m_EditMask;
   CEdit    m_EditDst;
   CEdit    m_EditDst2;
   CStatic     m_StaticSrc;
   CStatic     m_StaticSrc2;
   CStatic     m_StaticSrc3;
   CStatic     m_StaticMask;
   CStatic     m_StaticDst;
   CStatic     m_StaticDst2;
   CStatic  m_scaleStatic;
   CSpinButtonCtrl   m_scaleSpin;
   CEdit m_scaleEdit;
   CString  m_scaleString;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParamDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_UsedScale;

   // Generated message map functions
   //{{AFX_MSG(CParamDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

protected:
   BOOL m_bInit;

   virtual void InitInplace();
   virtual void InitVectors();

   void EnableScale();
   void InitScale();
   int MinScale();
   int MaxScale();
   void SetFlags(int flag, BOOL bFlags[], int n);
   int  GetFlags(const BOOL bFlags[], int n);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMDLG_H__7D177118_A60E_4A26_A8C4_C8099F0FC6D8__INCLUDED_)
