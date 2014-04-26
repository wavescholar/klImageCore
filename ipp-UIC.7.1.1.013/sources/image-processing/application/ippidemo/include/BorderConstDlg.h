/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// BorderConstDlg.h : interface for the Set Border Values dialog.
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"

#if !defined __BORDERCONSTDLG_H__INCLUDED__
#define __BORDERCONSTDLG_H__INCLUDED__

// CBorderConstDlg dialog

class CBorderConstDlg : public CDialog
{
    DECLARE_DYNAMIC(CBorderConstDlg)

public:
    CBorderConstDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CBorderConstDlg();

// Dialog Data
    enum { IDD = IDD_BORDER_CONST };
    CString m_Text;
    int m_Len;
    CMyString  m_ValueStr[4];

protected:
    CEdit m_ValueEdit[4];
    CStatic m_Static[4];

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOther();
};

#endif // __BORDERCONSTDLG_H__INCLUDED__
