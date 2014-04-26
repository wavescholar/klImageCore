
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFirSparseDlg.h: interface for CParmFirSparseDlg class.
// CParmFirSparseDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMFIRSPARSEDLG_H__
#define __PARMFIRSPARSEDLG_H__

#pragma once

#include "ParamDlg.h"
class CFirSparse;

class CParmFirSparseDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFirSparseDlg)

public:
   CParmFirSparseDlg(UINT nID = IDD);
   virtual ~CParmFirSparseDlg();

   CFirSparse* m_pFirSparse;
   double      m_epsilon;
// Dialog Data
   enum { IDD = IDD_PARM_FIR_PARSE };
   CMyString   m_TapStr;
   int      m_TapLen;
   CString  m_TapTitle;

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()

public:
   int m_NZTapsLen;
   CString m_epsilonStr;

   virtual BOOL OnInitDialog();
   afx_msg void OnChangeEpsilon();
};

#endif
