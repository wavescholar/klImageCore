/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLutDlg.cpp : implementation of the CParmLutDlg class.
// CParmLutDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmLutDlg.h"
#include "LUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { FREQ = 5, AMP = 4};

static COLORREF PictColor[4] = {
   RGB(  0,   0, 192),
   RGB(  0, 192,   0),
   RGB(192,   0,   0),
   RGB( 64,  64,  64),
};

/////////////////////////////////////////////////////////////////////////////
// CParmLutDlg dialog


CParmLutDlg::CParmLutDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmLutDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmLutDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++) {
      m_Amp[i] = 0;
      m_Freq[i] = 0;
      m_LowerStr[i] = _T("");
      m_NumStr[i] = _T("");
      m_UpperStr[i] = _T("");
   }
}


void CParmLutDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmLutDlg)
   //}}AFX_DATA_MAP

   DDX_Control(pDX, IDC_PICT_0, m_PictWnd[0]);
   DDX_Control(pDX, IDC_PICT_1, m_PictWnd[1]);
   DDX_Control(pDX, IDC_PICT_2, m_PictWnd[2]);
   DDX_Control(pDX, IDC_PICT_3, m_PictWnd[3]);

   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_0, m_UpperStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_0, m_NumStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_0, m_NumSpin[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_0, m_NumEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_0, m_LowerStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerEdit[0]);
   DDX_Control(pDX, IDC_GROUP_STATIC_0, m_GroupStatic[0]);
   DDX_Control(pDX, IDC_FREQ_STATIC_0, m_FreqStatic[0]);
   DDX_Control(pDX, IDC_FREQ_SLIDER_0, m_FreqSlider[0]);
   DDX_Control(pDX, IDC_AMP_STATIC_0, m_AmpStatic[0]);
   DDX_Control(pDX, IDC_AMP_SLIDER_0, m_AmpSlider[0]);
   DDX_Slider(pDX, IDC_AMP_SLIDER_0, m_Amp[0]);
   DDX_Slider(pDX, IDC_FREQ_SLIDER_0, m_Freq[0]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerStr[0]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_0, m_NumStr[0]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperStr[0]);

   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_1, m_UpperStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_1, m_NumStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_1, m_NumSpin[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_1, m_NumEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_1, m_LowerStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerEdit[1]);
   DDX_Control(pDX, IDC_GROUP_STATIC_1, m_GroupStatic[1]);
   DDX_Control(pDX, IDC_FREQ_STATIC_1, m_FreqStatic[1]);
   DDX_Control(pDX, IDC_FREQ_SLIDER_1, m_FreqSlider[1]);
   DDX_Control(pDX, IDC_AMP_STATIC_1, m_AmpStatic[1]);
   DDX_Control(pDX, IDC_AMP_SLIDER_1, m_AmpSlider[1]);
   DDX_Slider(pDX, IDC_AMP_SLIDER_1, m_Amp[1]);
   DDX_Slider(pDX, IDC_FREQ_SLIDER_1, m_Freq[1]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerStr[1]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_1, m_NumStr[1]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperStr[1]);

   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_2, m_UpperStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_2, m_NumStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_2, m_NumSpin[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_2, m_NumEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_2, m_LowerStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerEdit[2]);
   DDX_Control(pDX, IDC_GROUP_STATIC_2, m_GroupStatic[2]);
   DDX_Control(pDX, IDC_FREQ_STATIC_2, m_FreqStatic[2]);
   DDX_Control(pDX, IDC_FREQ_SLIDER_2, m_FreqSlider[2]);
   DDX_Control(pDX, IDC_AMP_STATIC_2, m_AmpStatic[2]);
   DDX_Control(pDX, IDC_AMP_SLIDER_2, m_AmpSlider[2]);
   DDX_Slider(pDX, IDC_AMP_SLIDER_2, m_Amp[2]);
   DDX_Slider(pDX, IDC_FREQ_SLIDER_2, m_Freq[2]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerStr[2]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_2, m_NumStr[2]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperStr[2]);

   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_3, m_UpperStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_3, m_NumStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_3, m_NumSpin[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_3, m_NumEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_3, m_LowerStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerEdit[3]);
   DDX_Control(pDX, IDC_GROUP_STATIC_3, m_GroupStatic[3]);
   DDX_Control(pDX, IDC_FREQ_STATIC_3, m_FreqStatic[3]);
   DDX_Control(pDX, IDC_FREQ_SLIDER_3, m_FreqSlider[3]);
   DDX_Control(pDX, IDC_AMP_STATIC_3, m_AmpStatic[3]);
   DDX_Control(pDX, IDC_AMP_SLIDER_3, m_AmpSlider[3]);
   DDX_Slider(pDX, IDC_AMP_SLIDER_3, m_Amp[3]);
   DDX_Slider(pDX, IDC_FREQ_SLIDER_3, m_Freq[3]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerStr[3]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_3, m_NumStr[3]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperStr[3]);

}


BEGIN_MESSAGE_MAP(CParmLutDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmLutDlg)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AMP_SLIDER_0, OnReleasedcaptureAmpSlider0)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AMP_SLIDER_1, OnReleasedcaptureAmpSlider1)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AMP_SLIDER_2, OnReleasedcaptureAmpSlider2)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AMP_SLIDER_3, OnReleasedcaptureAmpSlider3)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FREQ_SLIDER_0, OnReleasedcaptureFreqSlider0)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FREQ_SLIDER_1, OnReleasedcaptureFreqSlider1)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FREQ_SLIDER_2, OnReleasedcaptureFreqSlider2)
   ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FREQ_SLIDER_3, OnReleasedcaptureFreqSlider3)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmLutDlg message handlers

BOOL CParmLutDlg::OnInitDialog()
{
   ASSERT(m_pLUT);
   CParamDlg::OnInitDialog();

   int num = m_pLUT->m_num;
   for (int i=0; i<num; i++) {
      m_FreqSlider[i].SetRange(-FREQ, FREQ);
      m_FreqSlider[i].SetTicFreq(1);
      m_FreqSlider[i].SetPos(m_Freq[i]);
      m_AmpSlider[i].SetRange(-AMP, AMP);
      m_NumSpin[i].SetRange32(INT_MIN,INT_MAX);
   }
   m_pLUT->m_Lower.Get(m_LowerStr, num);
   m_pLUT->m_Upper.Get(m_UpperStr, num);
   m_pLUT->GetNumLevels(m_NumStr);
   EnableFrames();
   CreatePict();
   ShowPict();
   UpdateData(FALSE);
   return TRUE;
}

void CParmLutDlg::OnOK()
{
   UpdateData();
   int num = m_pLUT->m_num;
   m_pLUT->m_Lower.Set(m_LowerStr, num);
   m_pLUT->m_Upper.Set(m_UpperStr, num);
   m_pLUT->SetNumLevels(m_NumStr);

   CParamDlg::OnOK();

   DeletePict();
}

void CParmLutDlg::OnReleasedcaptureAmpSlider0(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(0); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureAmpSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(1); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureAmpSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(2); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureAmpSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(3); *pResult = 0;}

void CParmLutDlg::OnReleasedcaptureFreqSlider0(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(0); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureFreqSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(1); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureFreqSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(2); *pResult = 0;}
void CParmLutDlg::OnReleasedcaptureFreqSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{  OnChangeTone(3); *pResult = 0;}

void CParmLutDlg::OnChangeTone(int idx)
{
   UpdateData();
   ShowPict(idx);
}

void CParmLutDlg::EnableFrames()
{
   for (int i = m_pLUT->m_num; i < 4; i++) {
      m_UpperStatic[i].EnableWindow(FALSE);
      m_UpperEdit[i].EnableWindow(FALSE);
      m_NumStatic[i].EnableWindow(FALSE);
      m_NumSpin[i].EnableWindow(FALSE);
      m_NumEdit[i].EnableWindow(FALSE);
      m_LowerStatic[i].EnableWindow(FALSE);
      m_LowerEdit[i].EnableWindow(FALSE);
      m_GroupStatic[i].EnableWindow(FALSE);
      m_FreqStatic[i].EnableWindow(FALSE);
      m_AmpStatic[i].EnableWindow(FALSE);
      m_AmpSlider[i].EnableWindow(FALSE);
      m_FreqSlider[i].ShowWindow(SW_HIDE);
      m_PictWnd[i].ShowWindow(SW_HIDE);
   }
   if (m_pLUT->m_num == 1)
      m_GroupStatic[0].SetWindowText("");
}

void CParmLutDlg::ShowPict()
{
   for (int i = 0; i < m_pLUT->m_num; i++) {
      ShowPict(i);
   }
}

static int getWndWidth(CWnd& wnd)
{
   CRect rect;
   wnd.GetClientRect(rect);
   return rect.Width();
}

static int getWndHeight(CWnd& wnd)
{
   CRect rect;
   wnd.GetClientRect(rect);
   return rect.Height();
}

void CParmLutDlg::CreatePict()
{
   int width  = getWndWidth(m_PictWnd[0]);
   int height = getWndHeight(m_PictWnd[0]);
   int num = m_pLUT->m_num;
   m_PictLUT.Init32s(num,width,0,height - 1);
   m_PictLUT.CreateEven();
   for (int i=0; i<num; i++) {
      m_PictWnd[i].m_Color = num == 1 ? PictColor[3] : PictColor[i];
      m_PictWnd[i].m_Len = m_PictLUT.m_Levels[i].Length();
      m_PictWnd[i].m_X = (int*)m_PictLUT.m_Levels[i].GetData();
      m_PictWnd[i].m_Y = (int*)m_PictLUT.m_Hist[i].GetData();
   }
}

void CParmLutDlg::DeletePict()
{
   for (int i=0; i<4; i++) {
      m_PictWnd[i].m_Len = 0;
      m_PictWnd[i].m_X   = NULL;
      m_PictWnd[i].m_Y   = NULL;
   }
}

void CParmLutDlg::ShowPict(int idx)
{
   m_PictLUT.SetToneValues(idx, GetF(idx), GetA(idx));
   RedrawPict(idx);
}

void CParmLutDlg::RedrawPict(int idx)
{
    CRect rect;
    m_PictWnd[idx].GetWindowRect(&rect);
    POINT origin = rect.TopLeft();   
    ::ScreenToClient(m_hWnd, &origin);
    rect = CRect(origin, rect.Size());
    InvalidateRect(&rect, FALSE);
    UpdateWindow();
}

static int convertF(int f)
{
   if (f == 0) return 0;
   int a = f > 0 ? (FREQ + 1) : -(FREQ + 1);
   return a - f;
}

int CParmLutDlg::GetF(int idx)
{
   return convertF(m_Freq[idx]);
}

void CParmLutDlg::GetF(int* F)
{
   for (int i=0; i<m_pLUT->m_num; i++)
      F[i] = GetF(i);
}

void CParmLutDlg::SetF(const int* F)
{
   for (int i=0; i<m_pLUT->m_num; i++) {
      m_Freq[i] = convertF(F[i]);
   }
}

double CParmLutDlg::GetA(int idx)
{
   double A = (double)m_Amp[idx] / (double)AMP;
   if (A < -1) return -1;
   if (A >  1) return 1;
   return A;
}

void CParmLutDlg::GetA(double* A)
{
   for (int i=0; i<m_pLUT->m_num; i++)
      A[i] = GetA(i);
}

void CParmLutDlg::SetA(const double* A)
{
   for (int i=0; i<m_pLUT->m_num; i++)
      m_Amp[i] = (int)(A[i]*AMP + .5);
}


