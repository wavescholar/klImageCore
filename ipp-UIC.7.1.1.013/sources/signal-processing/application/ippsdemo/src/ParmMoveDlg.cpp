/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMoveDlg.cpp : implementation of the CParmMoveDlg class.
// CParmMoveDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ippsdemodoc.h"
#include "ParmMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMoveDlg dialog


IMPLEMENT_DYNAMIC(CParmMoveDlg, CParamDlg)
CParmMoveDlg::CParmMoveDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmMoveDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmMoveDlg)
   m_SrcDstLen = 0;
   m_LenStr = _T("");
   m_OffsetSrcStr = _T("");
   m_OffsetDstStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmMoveDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMoveDlg)
   DDX_Text(pDX, IDC_OFFDST_EDIT, m_OffsetDstStr);
   DDX_Control(pDX, IDC_OFFDST_SPIN, m_OffsetDstSpin);
   DDX_Text(pDX, IDC_OFFSRC_EDIT, m_OffsetSrcStr);
   DDX_Control(pDX, IDC_OFFSRC_SPIN, m_OffsetSrcSpin);
   DDX_Text(pDX, IDC_LEN_EDIT, m_LenStr);
   DDX_Control(pDX, IDC_LEN_SPIN, m_LenSpin);
   DDX_Text(pDX, IDC_DOCLEN, m_SrcDstLen);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmMoveDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMoveDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMoveDlg message handlers

BOOL CParmMoveDlg::OnInitDialog() 
{
   m_SrcDstLen = m_pDocSrc->GetVector()->Length();
   CParamDlg::OnInitDialog();
   m_OffsetSrcSpin.SetRange(0, m_SrcDstLen);
   m_OffsetDstSpin.SetRange(0, m_SrcDstLen);
   m_LenSpin.SetRange(0, m_SrcDstLen);
   
   // TODO: Add extra initialization here
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
