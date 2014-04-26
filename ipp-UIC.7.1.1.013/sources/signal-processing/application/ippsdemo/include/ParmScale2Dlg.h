
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmScale2Dlg.h: interface for CParmScale2Dlg class.
// CParmScale2Dlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMSCALE2DLG_H__
#define __PARMSCALE2DLG_H__

#pragma once

#include "ParamDlg.h"
#include "afxcmn.h"


class CParmScale2Dlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmScale2Dlg)

public:
   CParmScale2Dlg(UINT nID = IDD);
   virtual ~CParmScale2Dlg();

// Dialog Data
   enum { IDD = IDD_PARM_SCALE2 };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
public:
   int m_scaleFactor2;
   int m_scaleFactor1;
   CSpinButtonCtrl m_scaleSpin1;
   CSpinButtonCtrl m_scaleSpin2;
   virtual BOOL OnInitDialog();
};

#endif
