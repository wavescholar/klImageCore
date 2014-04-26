/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRemapDlg.h: interface for CParmRemapDlg class.
// CParmRemapDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMREMAPDLG_H__A25DDBA4_F93E_11D2_8EFD_00AA00A03C3C__INCLUDED_)
#define AFX_PARMREMAPDLG_H__A25DDBA4_F93E_11D2_8EFD_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ParamDlg.h"
#include "Mapping.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CParmRemapDlg dialog

class CParmRemapDlg : public CParamDlg
{
// Construction
public:
   CParmRemapDlg(CWnd* pParent = NULL);   // standard constructor
    void Init(CMapPtrArray* pArray);

    enum {NUM_EDIT = CMapping::NUM_EDIT,
          NUM_RADIO = CMapping::NUM_RADIO};
// Dialog Data
   //{{AFX_DATA(CParmRemapDlg)
   enum { IDD = IDD_PARM_REMAP };
   int m_interpolate;
   int      m_Type;
   BOOL  m_ShowMaps;
   //}}AFX_DATA
   CEdit m_TextEdit[NUM_EDIT];
   CStatic  m_NameStatic[NUM_EDIT];
   CButton  m_RadioButton[NUM_RADIO];
   CButton  m_TypeButton[MOD_NUM];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRemapDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   int      m_Inter;
   BOOL  m_Smooth;
    CMapPtrArray* m_pMapPtrArray;
    CMapPtrArray  m_Maps;
    CMapping*     m_pData;
    CMapping      m_MapDummy;

    void UpdateInterpolation(BOOL save = TRUE) ;
    void InitType();
    void UpdateType();
    void ShowOptions();
    void UpdateQuadOptions();

   // Generated message map functions
   //{{AFX_MSG(CParmRemapDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnSelchangeCOMBODst();
   afx_msg void OnType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMREMAPDLG_H__A25DDBA4_F93E_11D2_8EFD_00AA00A03C3C__INCLUDED_)
