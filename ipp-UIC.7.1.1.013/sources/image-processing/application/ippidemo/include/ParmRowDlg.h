
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRowDlg.h: interface for CParmRowDlg class.
// CParmRowDlg dialog gets parameters for certain ippIP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMROWDLG_H__
#define __PARMROWDLG_H__

#pragma once

#include "ParamDlg.h"


class CParmRowDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmRowDlg)

public:
   CParmRowDlg(UINT nID = IDD);
   virtual ~CParmRowDlg();

   int m_anchor;
   int m_maskSize;
// Dialog Data
   enum { IDD = IDD_PARM_ROW };

   BOOL  m_bCenter;
   CSpinButtonCtrl   m_AnchorSpin;
   CSpinButtonCtrl   m_SizeSpin;
   CEdit m_SizeEdit;
   CEdit m_AnchorEdit;
   CStatic  m_AnchorStatic;
   CStatic  m_SizeStatic;
protected:
   void EnableAnchor();
   void CenterAnchor();
   BOOL GetCenter();
   int  GetSize();
   void SetSize(int val);
   int  GetAnchor();
   void SetAnchor(int val);

   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnCenter();
   afx_msg void OnChangeSize();

   DECLARE_MESSAGE_MAP()
public:
};

#endif
