/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSuperDlg.h: interface for CParmSuperDlg class.
// CParmSuperDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined __PARMSUPERDLG_H__INCLUDED__
#define __PARMSUPERDLG_H__INCLUDED__

#pragma once
#include "afxwin.h"

#include "ParamDlg.h"
// CParmSuperDlg dialog

class CParmSuperDlg : public CParamDlg
{
    DECLARE_DYNAMIC(CParmSuperDlg)

public:
    CParmSuperDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor
    virtual ~CParmSuperDlg();

// Dialog Data
    enum { IDD = IDD_PARM_SUPER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CEdit m_DstWidthEdit;
    CEdit m_DstHeightEdit;
    IppiSize m_srcSize;
    IppiSize m_dstSize;
};

#endif // __PARMSUPERDLG_H__INCLUDED__
