/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWarpDlg.h: interface for CParmWarpDlg class.
// CParmWarpDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMWARPDLG_H__44AAAF6A_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_PARMWARPDLG_H__44AAAF6A_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmWarpDlg dialog

#include "ParamDlg.h"
#include "RunWarp.h"
#include "afxwin.h"

class CParmWarpDlg : public CParamDlg
{
// Construction
public:
   CParmWarpDlg(CWnd* pParent = NULL);   // standard constructor

   enum {NF_AFF = CRunWarp::NF_AFF, NF_BIL = CRunWarp::NF_BIL,
      NF_PER = CRunWarp::NF_PER, NF_NUM = CRunWarp::NF_NUM,};
   enum {I_AFF = CRunWarp::I_AFF, I_BIL = CRunWarp::I_BIL, I_PER = CRunWarp::I_PER,
         J_AFF = CRunWarp::J_AFF, J_BIL = CRunWarp::J_BIL, J_PER = CRunWarp::J_PER,};
// Dialog Data
    double m_CoefAff[I_AFF][J_AFF];
    double m_CoefBil[I_BIL][J_BIL];
    double m_CoefPer[I_PER][J_PER];
    int    m_NF;
    int    m_I;
    int    m_J;
    CString m_TransName;
   //{{AFX_DATA(CParmWarpDlg)
   enum { IDD = IDD_PARM_WARP };
   int m_interpolate;
   int      m_Type;
   //}}AFX_DATA
   CButton  m_TypeButton[4];
   CEdit m_CoefEdit[3][4];
   CString  m_CoefStr[3][4];
   CString  m_QuadStr[4][2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmWarpDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   int      m_Inter;
   BOOL  m_Smooth;
    double m_Quad[4][2];
    double m_Parl[4][2];
    CImage*   m_QuadImage;
    CImage*   m_RoiImage;

    void UpdateInterpolation(BOOL save = TRUE) ;
    void EnableInter() ;
    void UpdateType();
    void ShowCoef() ;
    void EnableCoef() ;
    void UpdateCoef(int oldType = -1) ;
    void QuadFromContour() ;
    void QuadFromCoef() ;
    BOOL CoefFromQuad(BOOL errorBox) ;
    void UpdateMyData(BOOL save = TRUE) ;
    void SetQuad();
    void SetCoef();
    void FormatQuad(BOOL show = TRUE);
    void FormatCoef(BOOL show = TRUE);

   // Generated message map functions
   //{{AFX_MSG(CParmWarpDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnWarpFlag();
   afx_msg void OnChangeCoef();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedInter8();
    CButton m_CatmullromButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMWARPDLG_H__44AAAF6A_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
