/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDecimateDlg.h: interface for CParmDecimateDlg class.
// CParmDecimateDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMDECIMATEDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_)
#define AFX_PARMDECIMATEDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmDecimateDlg dialog

class CParmDecimateDlg : public CParamDlg
{
// Construction
public:
   CParmDecimateDlg(UINT nID = CParmDecimateDlg::IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmDecimateDlg)
   enum { IDD = IDD_PARM_DECIMATE };
   int      m_fraction;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmDecimateDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmDecimateDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMDECIMATEDLG_H__46BB5DD5_5F01_47C1_BE7E_E0D69DFD4DA8__INCLUDED_)
