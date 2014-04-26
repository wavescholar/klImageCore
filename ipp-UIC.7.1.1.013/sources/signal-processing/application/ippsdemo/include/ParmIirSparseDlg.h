
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIirSparseDlg.h: interface for CParmIirSparseDlg class.
// CParmIirSparseDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMIIRPARSEDLG_H__
#define __PARMIIRPARSEDLG_H__

#pragma once

#include "ParamDlg.h"
class CIirSparse;


class CParmIirSparseDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmIirSparseDlg)

public:
   CParmIirSparseDlg(UINT nID = IDD);
   virtual ~CParmIirSparseDlg();

   CIirSparse* m_pIirSparse;
   double      m_epsilon;
// Dialog Data
   enum { IDD = IDD_PARM_IIR_SPARSE };
   CString  m_TapTitle;
   CMyString   m_TapStr;
   int         m_Order;
   int         m_NZTapsLen1;
   int         m_NZTapsLen2;
   CString     m_epsilonStr;

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()

public:
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeEpsilon();
};

#endif
