/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeFilterDlg.h: interface for CParmResizeFilterDlg class.
// CParmResizeFilterDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMRESIZEFILTERDLG_H__INCLUDED__
#define __PARMRESIZEFILTERDLG_H__INCLUDED__

#pragma once
#include "afxwin.h"

#include "ParmSuperDlg.h"
// CParmResizeFilterDlg dialog

class CParmResizeFilterDlg : public CParmSuperDlg
{
    DECLARE_DYNAMIC(CParmResizeFilterDlg)

public:
    CParmResizeFilterDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CParmResizeFilterDlg();

// Dialog Data
    enum { IDD = IDD_PARM_RESIZE_FLT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    int m_filter;
};

#endif // __PARMRESIZEFILTERDLG_H__INCLUDED__
