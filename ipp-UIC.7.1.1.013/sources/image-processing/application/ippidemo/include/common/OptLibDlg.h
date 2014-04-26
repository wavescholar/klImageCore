/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

#if !defined __OPTLIBDLG_H__INCLUDED__
#define __OPTLIBDLG_H__INCLUDED__

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// COptLibDlg dialog

class COptLibDlg : public CDialog
{
    DECLARE_DYNAMIC(COptLibDlg)

public:
    COptLibDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COptLibDlg();

// Dialog Data
    enum { IDD = IDD_OPT_IPP };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOpt0();
    afx_msg void OnBnClickedOpt1();
    afx_msg void OnBnClickedThreadRadio0();
    afx_msg void OnBnClickedThreadRadio1();
    afx_msg void OnEnChangeThreadEdit();
    afx_msg void OnDeltaposThreadSpin(NMHDR *pNMHDR, LRESULT *pResult);
    int m_NumThreads;
    int m_NumThreads_Optimal;
    CEdit m_NumThreadsEdit;
    virtual BOOL OnInitDialog();
    int m_Optimization;
    int m_NumThreadsMode;
    void SetDefaultNumThreads(void);
protected:
    virtual void OnOK();
public:
    CSpinButtonCtrl m_NumThreadsSpin;
};

#endif // __OPTLIBDLG_H__INCLUDED__
