/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLutDlg.h: interface for CParmLutDlg class.
// CParmLutDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMLUTDLG_H__3EEB122C_BAD6_4A79_8A81_FCC2AEB62555__INCLUDED_)
#define AFX_PARMLUTDLG_H__3EEB122C_BAD6_4A79_8A81_FCC2AEB62555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "LUT.h"
#include "PictWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CParmLutDlg dialog

class CParmLutDlg : public CParamDlg
{
// Construction
public:
   CParmLutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CLUT* m_pLUT;
   void   GetF(int* F);
   void   SetF(const int* F);
   void   GetA(double* A);
   void   SetA(const double* A);
   //{{AFX_DATA(CParmLutDlg)
   enum { IDD = IDD_PARM_LUT };
   //}}AFX_DATA
   CPictWnd m_PictWnd[4];
   CStatic  m_UpperStatic[4];
   CEdit m_UpperEdit[4];
   CStatic  m_NumStatic[4];
   CSpinButtonCtrl   m_NumSpin[4];
   CEdit m_NumEdit[4];
   CStatic  m_LowerStatic[4];
   CEdit m_LowerEdit[4];
   CButton  m_GroupStatic[4];
   CStatic  m_FreqStatic[4];
   CSliderCtrl m_FreqSlider[4];
   CStatic  m_AmpStatic[4];
   CSliderCtrl m_AmpSlider[4];
   int      m_Amp[4];
   int      m_Freq[4];
   CMyString   m_LowerStr[4];
   CMyString   m_NumStr[4];
   CMyString   m_UpperStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmLutDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CLUT  m_PictLUT;

   void OnChangeTone(int idx);
   void EnableFrames();
   void CreatePict();
   void DeletePict();
   void ShowPict();
   void ShowPict(int idx);
   void RedrawPict(int idx);
   int    GetF(int idx);
   double GetA(int idx);

   // Generated message map functions
   //{{AFX_MSG(CParmLutDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnReleasedcaptureAmpSlider0(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureAmpSlider1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureAmpSlider2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureAmpSlider3(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureFreqSlider0(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureFreqSlider1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureFreqSlider2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnReleasedcaptureFreqSlider3(NMHDR* pNMHDR, LRESULT* pResult);
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMLUTDLG_H__3EEB122C_BAD6_4A79_8A81_FCC2AEB62555__INCLUDED_)
