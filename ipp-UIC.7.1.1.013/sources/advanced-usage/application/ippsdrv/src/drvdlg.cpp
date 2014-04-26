
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "stdafx.h"
#ifndef __DRVDLG_H__
#include "drvdlg.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




static LPCSTR drvold = "ippsold";
static LPCSTR drvnew = "ippsdrv";


BEGIN_MESSAGE_MAP(CDrvDlg,CDialog)
  //{{AFX_MSG_MAP(CDrvDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDABOUT,OnAbout)
  ON_BN_CLICKED(IDC_START_FFT,OnStartFft)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrvDlg dialog

CDrvDlg::CDrvDlg(CWnd* pParent)
  : CDialog(CDrvDlg::IDD,pParent)
{
  TRACE("-> CDrvDlg constructor\n");
  //{{AFX_DATA_INIT(CDrvDlg)
  m_FftCore = 0;
  m_DrvType = 1;
  m_CpuName = _T("");
  m_CpuVnid = _T("");
  m_CpuAdds = _T("");
  m_OsName  = _T("");
  m_OsVers  = _T("");
  m_OsBild  = _T("");
  m_OsAdds  = _T("");
  m_LibName = _T("");
  m_LibVers = _T("");
  m_LibBild = _T("");
  //}}AFX_DATA_INIT

  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  TRACE("<- CDrvDlg constructor\n");
  return;
} // ctor


CDrvDlg::~CDrvDlg()
{
  TRACE("-> CDrvDlg destructor\n");

  Cleanup();

  TRACE("<- CDrvDlg destructor\n");
  return;
} // dtor


void CDrvDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CDrvDlg)
  DDX_Radio(pDX,IDC_FFT_FLT,m_FftCore);
  DDX_Control(pDX,IDC_FFT_LEN,m_FftLen);
  DDX_Control(pDX,IDC_DRV_LOG,m_DrvLog);
  DDX_Control(pDX,IDOK,m_bnOk);
  DDX_Control(pDX,IDC_START_FFT,m_StartFFT);
  DDX_Text(pDX,IDC_CPU_NAME,m_CpuName);
//  DDX_Text(pDX,IDC_CPU_VNID,m_CpuVnid);
//  DDX_Text(pDX,IDC_CPU_ADDS,m_CpuAdds);
  DDX_Text(pDX,IDC_OS_NAME,m_OsName);
  DDX_Text(pDX,IDC_OS_VERS,m_OsVers);
  DDX_Text(pDX,IDC_OS_BILD,m_OsBild);
  DDX_Text(pDX,IDC_OS_ADDS,m_OsAdds);
  DDX_Text(pDX,IDC_LIB_NAME,m_LibName);
  DDX_Text(pDX,IDC_LIB_VERS,m_LibVers);
  DDX_Text(pDX,IDC_LIB_BILD,m_LibBild);
  DDX_Radio(pDX,IDC_DRV_OLD,m_DrvType);
  //}}AFX_DATA_MAP

  return;
} // CDrvDlg::DoDataExchange()




/////////////////////////////////////////////////////////////////////////////
// CDrvDlg message handlers

BOOL CDrvDlg::OnInitDialog()
{
  TRY

  CDialog::OnInitDialog();

  SetIcon(m_hIcon,TRUE);
  SetIcon(m_hIcon,FALSE);

  m_FftLen.SetCurSel(0);

  m_OsVersion.Initialize();

  DWORD platformID = m_OsVersion.GetPlatformID();
  if(VER_PLATFORM_WIN32_NT == platformID)
  {
    m_SvcNew.InstallService(drvnew);
    m_SvcOld.InstallService(drvold);
  }

  m_DrvNew.OpenDevice(drvnew);

  m_DrvOld.OpenDevice(drvold);

  ShowCPUInfo();
  ShowOSInfo();
  ShowIPPSLibInfo();

  UpdateData(FALSE);

  m_string.Format(IDS_PRG_START);
  WriteLog(m_string);

  m_string.Format(IDS_DRV_OPEN,drvold,m_DrvOld.GetDeviceHandle());
  WriteLog(m_string);

  m_string.Format(IDS_DRV_OPEN,drvnew,m_DrvNew.GetDeviceHandle());
  WriteLog(m_string);


  CATCH(CMyException,e);

  Cleanup();
  e->ReportError();

  END_CATCH

  return TRUE;
} // CDrvDlg::OnInitDialog()


void CDrvDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
  CDialog::OnSysCommand(nID,lParam);
  return;
} // CDrvDlg::OnSysCommand()


void CDrvDlg::OnPaint(void)
{
  if(IsIconic())
  {
    CPaintDC dc(this);

    SendMessage(WM_ICONERASEBKGND,(WPARAM)dc.GetSafeHdc(),0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width()  - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x,y,m_hIcon);
  } else {
    CDialog::OnPaint();
  }
  return;
} // CDrvDlg::OnPaint()


HCURSOR CDrvDlg::OnQueryDragIcon(void)
{
  return (HCURSOR)m_hIcon;
} // CDrvDlg::OnQueryDragIcon()


void CDrvDlg::OnAbout(void)
{
  CAboutDlg dlg;
  dlg.DoModal();
  return;
} // CDrvDlg::OnAbout()


void CDrvDlg::OnStartFft()
{
  char str[16];

  UpdateData(TRUE);

  int  len = 0;
  int  idx = m_FftLen.GetCurSel();

  ShowIPPSLibInfo();
  UpdateData(FALSE);


  m_FftLen.GetLBText(idx,str);
  len = atoi(str);

  if(0 == m_FftCore)
  {
    if(0 == m_DrvType)
    {
      m_string.Format(IDS_USE_SFFT,drvold,len);
    } else {
      m_string.Format(IDS_USE_SFFT,drvnew,len);
    }
  } else {
    if(0 == m_DrvType)
    {
      m_string.Format(IDS_USE_WFFT,drvold,len);
    } else {
      m_string.Format(IDS_USE_WFFT,drvnew,len);
    }
  }

  WriteLog(m_string);

  if(0 == m_DrvType)
  {
    m_DrvOld.StartFFT(len,m_FftCore);
    if(0 == m_DrvOld.GetError())
    {
      m_string.Format(IDS_NORMA,m_DrvOld.GetNorma());
    } else {
      m_string.Format(IDS_STR_ERROR);
    }
  } else {
    m_DrvNew.StartFFT(len,m_FftCore);
    if(0 == m_DrvNew.GetError())
    {
      m_string.Format(IDS_NORMA,m_DrvNew.GetNorma());
    } else {
      m_string.Format(IDS_STR_ERROR);
    }
  }


  WriteLog(m_string);

  return;
} // CDrvDlg::OnStartFft()


void CDrvDlg::ShowCPUInfo(void)
{
  m_CpuName = m_CpuInfo.GetCpuName();

  return;
} // CDrvDlg::ShowCPUInfo()


void CDrvDlg::ShowOSInfo(void)
{
  m_OsName = m_OsVersion.GetOsNameStr();
  m_OsVers = m_OsVersion.GetOsVersStr();
  m_OsBild = m_OsVersion.GetOsBildStr();
  m_OsAdds = m_OsVersion.GetOsAddsStr();
  return;
} // CDrvDlg::ShowOSInfo()


void CDrvDlg::ShowIPPSLibInfo(void)
{
  if(0 == m_DrvType)
  {
    m_DrvOld.DevGetLibVersion();
    m_LibName = m_DrvOld.GetLibName();
    m_LibVers = m_DrvOld.GetLibVers();
    m_LibBild = m_DrvOld.GetLibBild();
  } else {
    m_DrvNew.DevGetLibVersion();
    m_LibName = m_DrvNew.GetLibName();
    m_LibVers = m_DrvNew.GetLibVers();
    m_LibBild = m_DrvNew.GetLibBild();
  }

  return;
} // CDrvDlg::ShowIPPSLibInfo()


void CDrvDlg::WriteLog(CString& string)
{
  m_DrvLog.LimitText(40);
  CString strBuffer;
  m_DrvLog.GetWindowText(strBuffer);

  if(FALSE == strBuffer.IsEmpty())
  {
    strBuffer += "\r\n";
  }

  strBuffer += string;
  m_DrvLog.SetWindowText(strBuffer);

  // Scroll the edit control
  m_DrvLog.LineScroll(m_DrvLog.GetLineCount(),0);

  return;
} // CDrvDlg::WriteLog()


void CDrvDlg::Cleanup()
{
  m_DrvOld.CloseDevice();
  m_DrvNew.CloseDevice();

  DWORD platformID = m_OsVersion.GetPlatformID();
  if(VER_PLATFORM_WIN32_NT == platformID)
  {
    m_SvcOld.UninstallService();
    m_SvcNew.UninstallService();
  }
  return;
} // CDrvDlg::Cleanup()


void CDrvDlg::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
} // CDrvDlg::OnOK()
