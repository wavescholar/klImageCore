/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSliceDlg.cpp : implementation of the CParmSliceDlg class.
// CParmSliceDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "ParmSliceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmSliceDlg dialog


CParmSliceDlg::CParmSliceDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmSliceDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmSliceDlg)
   m_HeightStr = _T("");
   m_SliceStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmSliceDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmSliceDlg)
   DDX_Control(pDX, IDC_SLICE_SPIN, m_SliceSpin);
   DDX_Text(pDX, IDC_HEIGHT, m_HeightStr);
   DDX_Text(pDX, IDC_SLICE, m_SliceStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmSliceDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmSliceDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmSliceDlg message handlers

BOOL CParmSliceDlg::OnInitDialog()
{
   int height = m_pDocSrc->GetImage()->Height();
   m_HeightStr.Format("%d", height);

   CParamDlg::OnInitDialog();

   m_Accel.nSec = 0;
   m_Accel.nInc = 16;
   m_SliceSpin.SetRange(16, height>>4<<4);
   m_SliceSpin.SetAccel(1,&m_Accel);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
