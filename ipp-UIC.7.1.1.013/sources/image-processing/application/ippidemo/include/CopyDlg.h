/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// CopyDlg.h : interface for the Copy contour dialog.
// Command: Menu-Contour-{ROI|Quadrangle|Parallelogramm|Center&Shift}-Copy
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog

class CCopyDlg : public CDialog
{
// Construction
public:
   CCopyDlg(CippiDemoDoc* pSrcDoc, CString title);   // standard constructor

// Dialog Data
   CString   m_Title;
   CippiDemoDoc* m_pSrcDoc;
   CippiDemoDoc* m_pDstDoc;
   CImage*   m_pDstImage;
   //{{AFX_DATA(CCopyDlg)
   enum { IDD = IDD_COPY };
   CComboBox   m_BoxImage;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCopyDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void SetDstDoc(void);

   // Generated message map functions
   //{{AFX_MSG(CCopyDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};
