
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
//
*/

#ifndef __DRVDLG_H__
#define __DRVDLG_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  drvdlg.h")
#endif


#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif
#ifndef __ABOUTDLG_H__
#include "aboutdlg.h"
#endif
#ifndef __CPUINFO_H__
#include "cpuinfo.h"
#endif
#ifndef __SERVICE_H__
#include "service.h"
#endif
#ifndef __DRVOLD_H__
#include "drvold.h"
#endif
#ifndef __DRVNEW_H__
#include "drvnew.h"
#endif
#ifndef __OSVER_H__
#include "osver.h"
#endif
#ifndef __EXCEPT_H__
#include "except.h"
#endif
#include "resource.h"




/////////////////////////////////////////////////////////////////////////////
// CDrvDlg dialog

class CDrvDlg : public CDialog
{
public:
  CDrvDlg(CWnd* pParent = NULL);
  ~CDrvDlg();

  //{{AFX_DATA(CDrvDlg)
  enum { IDD = IDD_IPPSAPP_DIALOG };
  int       m_FftCore;
  int       m_DrvType;
  CComboBox m_FftLen;
  CEdit     m_DrvLog;
  CButton   m_bnOk;
  CButton   m_StartFFT;
  CString   m_CpuName;
  CString   m_CpuVnid;
  CString   m_CpuAdds;
  CString   m_OsName;
  CString   m_OsVers;
  CString   m_OsBild;
  CString   m_OsAdds;
  CString   m_LibName;
  CString   m_LibVers;
  CString   m_LibBild;
  //}}AFX_DATA


  //{{AFX_VIRTUAL(CDrvDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

protected:
  HICON           m_hIcon;
  CString         m_string;
  CCPUInfo        m_CpuInfo;
  COSVersion      m_OsVersion;
  CService        m_SvcOld;
  CService        m_SvcNew;
  COldDriver      m_DrvOld;
  CNewDriver      m_DrvNew;

  void ShowCPUInfo(void);
  void ShowOSInfo(void);
  void ShowIPPSLibInfo(void);

  void WriteLog(CString& string);

  void Cleanup();

  //{{AFX_MSG(CDrvDlg)
  virtual BOOL OnInitDialog(void);
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg void OnPaint(void);
  afx_msg HCURSOR OnQueryDragIcon(void);
  afx_msg void OnAbout(void);
  afx_msg void OnStartFft();
  virtual void OnOK();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}

#endif // __DRVDLG_H__
