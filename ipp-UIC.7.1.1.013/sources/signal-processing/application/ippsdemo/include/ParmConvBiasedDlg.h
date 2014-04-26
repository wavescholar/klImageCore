
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmConvBiasedDlg.h: interface for CParmConvBiasedDlg class.
// CParmConvBiasedDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMCONVBIASEDDLG_H__
#define __PARMCONVBIASEDDLG_H__

#pragma once

#include "ParamDlg.h"
#include "afxcmn.h"
#include "afxwin.h"


class CParmConvBiasedDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmConvBiasedDlg)

public:
   CParmConvBiasedDlg(UINT nID = IDD);
   virtual ~CParmConvBiasedDlg();

// Dialog Data
   enum { IDD = IDD_PARM_CONVBIASED };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
public:
   int m_bias;
   CSpinButtonCtrl m_biasSpin;
   CSpinButtonCtrl m_lenDstSpin;
   int m_lenSrc;
   int m_lenSrc2;
   int m_lenDst;
   BOOL m_biasAuto;

   virtual BOOL OnInitDialog();
   afx_msg void OnClickedBiasAuto();
   afx_msg void OnChangeLenDst();

   void EnableBias();
   void SetBias();
   CString m_NewDst;
};

#endif
