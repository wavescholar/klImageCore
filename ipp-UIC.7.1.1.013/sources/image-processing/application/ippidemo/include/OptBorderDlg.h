/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// COptBorderDlg.h : interface for the Border Behaviour dialog.
// Command: Menu-Options-Border
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __OPTBORDERDLG_H__INCLUDED__
#define __OPTBORDERDLG_H__INCLUDED__

#pragma once


// COptBorderDlg dialog

class COptBorderDlg : public CDialog
{
    DECLARE_DYNAMIC(COptBorderDlg)

public:
    COptBorderDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COptBorderDlg();

// Dialog Data
    enum { IDD = IDD_OPT_BORDER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    int m_Type;
    virtual BOOL OnInitDialog();
protected:
    virtual void OnOK();
};

#endif // __OPTBORDERDLG_H__INCLUDED__
