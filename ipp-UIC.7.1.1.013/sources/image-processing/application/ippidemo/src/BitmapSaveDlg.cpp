/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// BitmapSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippiDemo.h"
#include "BitmapSaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapSaveDlg dialog

BOOL CBitmapSaveDlg::m_HideDialog = FALSE;

CBitmapSaveDlg::CBitmapSaveDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CBitmapSaveDlg::IDD, pParent), m_bConvert(TRUE)
{
   //{{AFX_DATA_INIT(CBitmapSaveDlg)
   //}}AFX_DATA_INIT
}


void CBitmapSaveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBitmapSaveDlg)
    DDX_Check(pDX, IDC_HIDE, m_HideDialog);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_STATIC_1, m_Static1);
    DDX_Control(pDX, IDC_STATIC_2, m_Static2);
    DDX_Control(pDX, IDC_CONVERT, m_ConvertButton);
}


BEGIN_MESSAGE_MAP(CBitmapSaveDlg, CDialog)
   //{{AFX_MSG_MAP(CBitmapSaveDlg)
   ON_BN_CLICKED(IDC_CONVERT, OnConvert)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapSaveDlg message handlers

BOOL CBitmapSaveDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   if (m_HideDialog) EndDialog(IDOK);

   if (m_bConvert == FALSE)
   {
       m_Static1.EnableWindow(FALSE);
       m_Static2.EnableWindow(FALSE);
       m_ConvertButton.EnableWindow(FALSE);
   }
   return TRUE;
}

void CBitmapSaveDlg::OnConvert()
{
   UpdateData();
   EndDialog(IDC_CONVERT);
}
