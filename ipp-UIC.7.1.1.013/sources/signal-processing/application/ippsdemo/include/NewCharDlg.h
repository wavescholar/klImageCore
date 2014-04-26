
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewCharDlg.h : interface for the New String Creation dialog.
// Command: Menu-File-NewString
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CNewCharDlg : public CDialog
{
   DECLARE_DYNAMIC(CNewCharDlg)

public:
   CNewCharDlg(CWnd* pParent = NULL);   // standard constructor
   virtual ~CNewCharDlg();

// Dialog Data
   enum { IDD = IDD_FILE_NEW_CHAR };

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
public:
   ppType m_Type;
   int m_bHello;
   virtual BOOL OnInitDialog();
protected:
   int m_TypeDlg;
   virtual void OnOK();
};
