/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieMaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippiDemo.h"
#include "Movie.h"
#include "MovieMaskDlg.h"
#include "MyFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovieMaskDlg dialog


CMovieMaskDlg::CMovieMaskDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CMovieMaskDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CMovieMaskDlg)
   m_Threshold = 0;
   m_NameIdx = -1;
   //}}AFX_DATA_INIT
}


void CMovieMaskDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CMovieMaskDlg)
   DDX_Control(pDX, IDC_REMOVE, m_RemoveButton);
   DDX_Control(pDX, IDC_NAME_LIST, m_NameBox);
   DDX_Control(pDX, IDC_MASK_SLIDER, m_ThreshSlider);
   DDX_Control(pDX, IDC_MASK_PICT, m_Sample);
   DDX_Slider(pDX, IDC_MASK_SLIDER, m_Threshold);
   DDX_LBIndex(pDX, IDC_NAME_LIST, m_NameIdx);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMovieMaskDlg, CDialog)
   //{{AFX_MSG_MAP(CMovieMaskDlg)
   ON_BN_CLICKED(IDC_ADD, OnAdd)
   ON_BN_CLICKED(IDC_REMOVE, OnRemove)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_MASK_SLIDER, OnMaskSlider)
   ON_LBN_SELCHANGE(IDC_NAME_LIST, OnSelchangeNameList)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovieMaskDlg message handlers

BOOL CMovieMaskDlg::OnInitDialog()
{
   m_MaskArr = *CMovie::GetMaskArray();
   CDialog::OnInitDialog();

   InitNameBox();
   if (!LoadMask(FALSE)) {
      m_NameBox.SetCurSel(0);
      LoadMask(FALSE);
   }
   ShowMask();
   UpdateData();
   return TRUE;
}

void CMovieMaskDlg::OnOK()
{
   CDialog::OnOK();
   m_MaskArr.SetCurIndex(m_NameIdx);
   *CMovie::GetMaskArray() = m_MaskArr;
}

void CMovieMaskDlg::OnSelchangeNameList()
{
   CMovieMask* pMask = GetCurMask();
   if (LoadMask()) {
      ShowMask();
   } else {
      m_NameBox.SetCurSel(m_NameIdx);
   }
   UpdateData();
}

void CMovieMaskDlg::OnAdd()
{
   CMyString path;
   if (m_MaskArr.GetCurPath().IsEmpty())
      path = ".bmp";
   else
      path = GetCurMask()->GetFilePath();
   CMyFileDlg dlg(TRUE, path, "Bitmap Image");
   if (dlg.DoModal(path) != IDOK) return;
   m_MaskArr.SetCurPath(path);

   CMovieMask mask(path);
   if (!mask.Load()) return;
   int idx = (int)m_MaskArr.Add(mask);
   m_NameBox.SetCurSel(AddToBox(&m_MaskArr.ElementAt(idx)));
   ShowMask();
}

void CMovieMaskDlg::OnRemove()
{
   UpdateData();
   m_MaskArr.RemoveMask(m_NameIdx);
   m_NameBox.DeleteString(m_NameIdx);
   m_NameBox.SetCurSel(m_MaskArr.GetCurIndex());
}

void CMovieMaskDlg::OnMaskSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
   *pResult = 0;
   UpdateData();
   if (GetCurMask()->SetThreshold(m_Threshold))
      RedrawSample();
}

void CMovieMaskDlg::InitNameBox()
{
   for (int i=0; i<m_MaskArr.GetSize(); i++) {
      CMovieMask* pMask = &m_MaskArr.ElementAt(i);
      AddToBox(pMask);
   }
   m_NameBox.SetCurSel(m_MaskArr.GetCurIndex());
}

int CMovieMaskDlg::AddToBox(CMovieMask* pMask)
{
   int idx = m_NameBox.AddString(pMask->GetName());
   m_NameBox.SetItemDataPtr(idx, pMask);
   return idx;
}

CMovieMask* CMovieMaskDlg::GetCurMask()
{
   return (CMovieMask*)m_NameBox.GetItemDataPtr(m_NameBox.GetCurSel());
}

BOOL CMovieMaskDlg::LoadMask(BOOL bAsk)
{
   CMovieMask* pMask = GetCurMask();
   if (pMask->IsHeader()) return TRUE;
   return pMask->Load(bAsk);
}

void CMovieMaskDlg::ShowMask()
{
   CMovieMask* pMask = GetCurMask();
   m_Sample.SetImage(pMask);
   RedrawSample();
   InitSlider();
   m_RemoveButton.EnableWindow(!pMask->IsPattern());
}

void CMovieMaskDlg::InitSlider()
{
   CMovieMask* pMask = GetCurMask();
   BOOL flag = !pMask->IsPattern();
   m_ThreshSlider.EnableWindow(flag);
   if (flag) {
      m_ThreshSlider.SetRange(pMask->GetMin(), pMask->GetMax(), TRUE);
      m_ThreshSlider.SetPos(pMask->GetThreshold());
   } else {
      m_ThreshSlider.SetRange(0, 1, TRUE);
      m_ThreshSlider.SetPos(0);
   }
}

void CMovieMaskDlg::RedrawSample()
{
    CRect rect;
    m_Sample.GetWindowRect(&rect);
    POINT origin = rect.TopLeft();   
    ::ScreenToClient(m_hWnd, &origin);
    rect = CRect(origin, rect.Size());
    InvalidateRect(&rect, FALSE);
    UpdateWindow();
}
