/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ImgNewDlg.cpp : implementation of the New Image Creation dialog.
// Command: Menu-File-New
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "MainFrm.h"
#include "ippiDemoDoc.h"
#include "ImgNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_SIZE 1
#define MAX_SIZE INT_MAX

enum {D8, D16, D32S, D32F};
enum {C1, C2, C3, AC4};
enum {S444, S422, S420, S411};
enum {SET_COLOR = SET_BLUE};

/////////////////////////////////////////////////////////////////////////////
// CImgNewDlg dialog


CImgNewDlg::CImgNewDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CImgNewDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CImgNewDlg)
   m_Height = _T("");
   m_Width = _T("");
   m_Channels = -1;
   m_Depth = -1;
   m_Plane = FALSE;
   m_Cplx = FALSE;
   m_Sample = -1;
   m_Pattern = -1;
   //}}AFX_DATA_INIT
   m_pImage = NULL;
   m_bInit = FALSE;
}


void CImgNewDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CImgNewDlg)
   DDX_Control(pDX, IDC_SIZE_ROI, m_SizeRoiButton);
   DDX_Control(pDX, IDC_SIZE_IMG, m_SizeImgButton);
   DDX_Control(pDX, IDC_PARM_IMG, m_ParmImgButton);
   DDX_Control(pDX, IDC_EDIT_WIDTH, m_WidthEdit);
   DDX_Control(pDX, IDC_EDIT_HEIGHT, m_HeightEdit);
   DDX_Control(pDX, IDC_SPIN_Width, m_SpinWidth);
   DDX_Control(pDX, IDC_SPIN_Height, m_SpinHeight);
   DDX_Text(pDX, IDC_EDIT_HEIGHT, m_Height);
   DDX_Text(pDX, IDC_EDIT_WIDTH, m_Width);
   DDX_Radio(pDX, IDC_RADIO_C1, m_Channels);
   DDX_Radio(pDX, IDC_RADIO_D8, m_Depth);
   DDX_Radio(pDX, IDC_SET_0, m_Pattern);
   DDX_Check(pDX, IDC_PLANE, m_Plane);
   DDX_Check(pDX, IDC_CPLX, m_Cplx);
   DDX_Radio(pDX, IDC_SAMPLE_0, m_Sample);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_SET_0, m_SetButton[0]);
   DDX_Control(pDX, IDC_SET_1, m_SetButton[1]);
   DDX_Control(pDX, IDC_SET_2, m_SetButton[2]);
   DDX_Control(pDX, IDC_SET_3, m_SetButton[3]);
}


BEGIN_MESSAGE_MAP(CImgNewDlg, CDialog)
   //{{AFX_MSG_MAP(CImgNewDlg)
   ON_BN_CLICKED(IDC_RADIO_AC4, OnChannels)
   ON_BN_CLICKED(IDC_RADIO_D16, OnDepth)
   ON_BN_CLICKED(IDC_CPLX, OnCplx)
   ON_BN_CLICKED(IDC_PARM_IMG, OnParmImg)
   ON_BN_CLICKED(IDC_SIZE_IMG, OnSizeImg)
   ON_BN_CLICKED(IDC_SIZE_ROI, OnSizeRoi)
   ON_BN_CLICKED(IDC_PLANE, OnPlane)
   ON_BN_CLICKED(IDC_RADIO_C1, OnChannels)
   ON_BN_CLICKED(IDC_RADIO_C2, OnChannels)
   ON_BN_CLICKED(IDC_RADIO_C3, OnChannels)
   ON_BN_CLICKED(IDC_RADIO_D32F, OnDepth)
   ON_BN_CLICKED(IDC_RADIO_D32S, OnDepth)
   ON_BN_CLICKED(IDC_RADIO_D8, OnDepth)
   ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnChangeEditHeight)
   ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgNewDlg message handlers

BOOL CImgNewDlg::OnInitDialog()
{
   m_pImage = ACTIVE_DOC;
    SetImageHeader(m_Header);

    CDialog::OnInitDialog();

    EnableParmButton();
    EnableSizeButtons();
    m_SpinWidth.SetRange32(1,INT_MAX);
    m_SpinHeight.SetRange32(1,INT_MAX);
    EnableChannels();
    EnablePlane();
    EnableSample();
    EnableCplx();
    EnableSet();
   m_bInit = TRUE;

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CImgNewDlg::OnOK()
{
   CDialog::OnOK();
   m_Header = GetImageHeader();
}

void CImgNewDlg::SetImageHeader(ImgHeader header)
{
   SetImageParms(header);
   SetImageSize(header.width, header.height);
}

void CImgNewDlg::SetImageParms(ImgHeader header)
{
    switch (header.type & PP_MASK) {
    case  8: m_Depth = D8; break;
    case 16: m_Depth = D16; break;
    case 32: m_Depth = header.type & PP_FLOAT ? D32F : D32S;
             break;
    default: m_Depth = D8; break;
    }
    m_Cplx = (header.type & PP_CPLX) ? TRUE : FALSE;
    switch (header.channels) {
    case 1: m_Channels = C1; break;
    case 2: m_Channels = C2; break;
    case 3: m_Channels = C3; break;
    case 4: m_Channels = AC4; break;
    default: m_Channels = C3; break;
    }
    m_Plane = header.plane;
    m_Sample = header.IsSize422() ? S422 :
               header.IsSize420() ? S420 :
               header.IsSize420() ? S411 :
                                    S444;
}

void CImgNewDlg::SetImageSize(int width, int height)
{
   m_Width.Format("%d",width);
   m_Height.Format("%d",height);
}

ImgHeader CImgNewDlg::GetImageHeader()
{
   ImgHeader header;
   header.type = ImgType();
   header.channels = ImgChannels();
   header.plane = header.channels == 1 ? FALSE : m_Plane;
   header.width = header.height = 0;
   if (header.plane && (m_Sample == S422))
      header.SetSize422(atoi(m_Width), atoi(m_Height));
   else if (header.plane && (m_Sample == S420))
      header.SetSize420(atoi(m_Width), atoi(m_Height));
   else if (header.plane && (m_Sample == S411))
      header.SetSize411(atoi(m_Width), atoi(m_Height));
   else
      header.SetSize(atoi(m_Width), atoi(m_Height));
   return header;
}

ppType CImgNewDlg::ImgType()
{
   if (m_Cplx) {
      switch (m_Depth) {
      case D8  : return pp8sc;
      case D16 : return pp16sc;
      case D32S: return pp32sc;
      case D32F: return pp32fc;
      }
   } else {
      switch (m_Depth) {
      case D8  : return pp8u;
      case D16 : return pp16u;
      case D32S: return pp32s;
      case D32F: return pp32f;
      }
   }
   return ppNONE;
}

int CImgNewDlg::ImgChannels()
{
    switch (m_Channels) {
    case C1 :  return 1;
    case C2 :  return 2;
    case C3 :  return 3;
    case AC4:  return 4;
    default: return 0;
    }
}

void CImgNewDlg::OnParmImg()
{
   SetImageParms(m_pImage->GetHeader());
   m_ParmImgButton.EnableWindow(FALSE);
   UpdateData(FALSE);
}

void CImgNewDlg::OnSizeImg()
{
   SetImageSize(m_pImage->Width(), m_pImage->Height());
   EnableSizeButtons();
   UpdateData(FALSE);
}

void CImgNewDlg::OnSizeRoi()
{
   SetImageSize(m_pImage->GetRoi()->width,
                m_pImage->GetRoi()->height);
   EnableSizeButtons();
   UpdateData(FALSE);
}

void CImgNewDlg::OnChangeEditHeight()
{
   if (!m_bInit) return;
   UpdateData();
   m_Header = GetImageHeader();
   EnableSizeButtons();
}

void CImgNewDlg::OnChangeEditWidth()
{
   if (!m_bInit) return;
   UpdateData();
   m_Header = GetImageHeader();
   EnableSizeButtons();
}

void CImgNewDlg::OnChannels()
{
   UpdateData();
   m_Header = GetImageHeader();
   EnableParmButton();
   EnableSample();
   EnableCplx();
   EnableSet();
   EnablePlane();
   UpdateData(FALSE);
}

void CImgNewDlg::OnPlane()
{
   UpdateData();
   m_Header = GetImageHeader();
   EnableParmButton();
   EnableSample();
}

void CImgNewDlg::OnCplx()
{
   UpdateData();
   m_Header = GetImageHeader();
   EnableParmButton();
   EnableChannels();
}

void CImgNewDlg::OnDepth()
{
   UpdateData();
   m_Header = GetImageHeader();
   EnableParmButton();
}

void CImgNewDlg::EnableChannels()
{
   BOOL flag = !m_Cplx;
   GetDlgItem(IDC_RADIO_C2)->EnableWindow(flag);
   GetDlgItem(IDC_RADIO_C3)->EnableWindow(flag);
   GetDlgItem(IDC_RADIO_AC4)->EnableWindow(flag);
}

void CImgNewDlg::EnableCplx()
{
   GetDlgItem(IDC_CPLX)->EnableWindow(m_Channels == C1);
}

void CImgNewDlg::EnableSample()
{
   BOOL flag = m_Plane && (m_Channels >= C2);
   GetDlgItem(IDC_SAMPLE_0)->EnableWindow(flag);
   GetDlgItem(IDC_SAMPLE_1)->EnableWindow(flag);
   GetDlgItem(IDC_SAMPLE_2)->EnableWindow(flag);
   flag = m_Plane && (m_Channels >= C3);
   GetDlgItem(IDC_SAMPLE_3)->EnableWindow(flag);
}

void CImgNewDlg::EnablePlane()
{
   GetDlgItem(IDC_PLANE)->EnableWindow(m_Channels > C1);
}

void CImgNewDlg::EnableSet()
{
}


void CImgNewDlg::EnableParmButton()
{
    m_ParmImgButton.EnableWindow(DifferParm());
}

void CImgNewDlg::EnableSizeButtons()
{
    m_SizeImgButton.EnableWindow(DifferSize());
    m_SizeRoiButton.EnableWindow(DifferRoi());
}

BOOL CImgNewDlg::DifferParm()
{
    if (!m_pImage) return FALSE;
    ImgHeader header1 = GetImageHeader();
    ImgHeader header2 = m_pImage->GetHeader();
    if (header1.type != header2.type) return TRUE;
    if (header1.plane != header2.plane) return TRUE;
    if (header1.channels != header2.channels) return TRUE;
    return FALSE;
}

BOOL CImgNewDlg::DifferSize()
{
    if (!m_pImage) return FALSE;
    ImgHeader header = GetImageHeader();
    if (header.width != m_pImage->Width()) return TRUE;
    if (header.height != m_pImage->Height()) return TRUE;
    return FALSE;
}

BOOL CImgNewDlg::DifferRoi()
{
   if (!m_pImage) return FALSE;
   const IppiRect* pRoi = m_pImage->GetRoi();
   if (!pRoi) return FALSE;
   ImgHeader header = GetImageHeader();
   if (header.width != pRoi->width) return TRUE;
   if (header.height != pRoi->height) return TRUE;
   return FALSE;
}
