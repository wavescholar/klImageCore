
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCopy1uDlg.h: interface for CParmCopy1uDlg class.
// CParmCopy1uDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMCOPY1UDLG_H__
#define __PARMCOPY1UDLG_H__

#pragma once

#include "ParamDlg.h"


class CParmCopy1uDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmCopy1uDlg)

public:
   CParmCopy1uDlg(UINT nID = IDD);
   virtual ~CParmCopy1uDlg();

// Dialog Data
   enum { IDD = IDD_PARM_COPY1U };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
public:
   int m_itemBits;
   int m_maxBits;
   int m_numBits;
   int m_bitOffset[2];
   CSpinButtonCtrl m_bitOffsetSpin[2];
   CSpinButtonCtrl m_numBitsSpin;
   virtual BOOL OnInitDialog();
   afx_msg void OnEnChangeOffsetEdit0();
};

#endif
