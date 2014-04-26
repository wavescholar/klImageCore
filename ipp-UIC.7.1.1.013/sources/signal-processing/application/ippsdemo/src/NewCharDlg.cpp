
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewCharDlg.cpp : implementation of CNewCharDlg dialog
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "NewCharDlg.h"


// CNewCharDlg dialog

IMPLEMENT_DYNAMIC(CNewCharDlg, CDialog)
CNewCharDlg::CNewCharDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CNewCharDlg::IDD, pParent)
   , m_TypeDlg(FALSE)
   , m_bHello(FALSE)
{
}

CNewCharDlg::~CNewCharDlg()
{
}

void CNewCharDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Radio(pDX, IDC_TYPE_0, m_TypeDlg);
   DDX_Radio(pDX, IDC_DATA_0, m_bHello);
}


BEGIN_MESSAGE_MAP(CNewCharDlg, CDialog)
END_MESSAGE_MAP()


// CNewCharDlg message handlers


BOOL CNewCharDlg::OnInitDialog()
{
   m_TypeDlg = m_Type == pp8u  ?  0 :
               m_Type == pp16u ?  1 :
                                  0 ;
   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
}

void CNewCharDlg::OnOK()
{
   CDialog::OnOK();
   m_Type = m_TypeDlg == 0 ?  pp8u  :
            m_TypeDlg == 1 ?  pp16u :
                              pp8u ;
}
