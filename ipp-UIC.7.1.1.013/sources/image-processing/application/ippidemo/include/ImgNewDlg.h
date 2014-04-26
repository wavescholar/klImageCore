/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ImgNewDlg.h : interface for the New Image Creation dialog.
// Command: Menu-File-New
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGNEWDLG_H__AE1CB7E1_B98D_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_IMGNEWDLG_H__AE1CB7E1_B98D_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ImgNewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImgNewDlg dialog

class CImgNewDlg : public CDialog
{
// Construction
public:
   CImgNewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   ImgHeader  m_Header;
   //{{AFX_DATA(CImgNewDlg)
   enum { IDD = IDD_IMAGE_NEW };
   CButton  m_SizeRoiButton;
   CButton  m_SizeImgButton;
   CButton  m_ParmImgButton;
   CEdit m_WidthEdit;
   CEdit m_HeightEdit;
   CSpinButtonCtrl   m_SpinWidth;
   CSpinButtonCtrl   m_SpinHeight;
   CString  m_Height;
   CString  m_Width;
   int      m_Channels;
   int      m_Depth;
   BOOL  m_Plane;
   BOOL  m_Cplx;
   int      m_Sample;
   int m_Pattern;
   //}}AFX_DATA
   CButton  m_SetButton[SET_NUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CImgNewDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CImage* m_pImage;
   BOOL m_bInit;

   void SetImageHeader(ImgHeader header);
   void SetImageParms(ImgHeader header);
   void SetImageSize(int width, int height);
   ImgHeader GetImageHeader();
   ppType ImgType();
   int ImgChannels();
   void EnableCplx();
   void EnableChannels();
   void EnableSet() ;
   void EnablePlane();
   void EnableSample() ;
   void EnableParmButton() ;
   void EnableSizeButtons() ;
   BOOL DifferParm() ;
   BOOL DifferSize() ;
   BOOL DifferRoi () ;

   // Generated message map functions
   //{{AFX_MSG(CImgNewDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChannels();
   afx_msg void OnDepth();
   afx_msg void OnCplx();
   afx_msg void OnParmImg();
   afx_msg void OnSizeImg();
   afx_msg void OnSizeRoi();
   afx_msg void OnPlane();
   afx_msg void OnChangeEditHeight();
   afx_msg void OnChangeEditWidth();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGNEWDLG_H__AE1CB7E1_B98D_11D1_AE6B_444553540000__INCLUDED_)
