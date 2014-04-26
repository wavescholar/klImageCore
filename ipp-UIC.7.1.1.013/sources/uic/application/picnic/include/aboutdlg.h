/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#include <QtGui/QtGui>



class AboutDlg : public QDialog
{
public:
  AboutDlg(QWidget* parent = 0, Qt::WindowFlags flags = 0) : QDialog(parent, flags) {}
  virtual ~AboutDlg(void);

  bool Init(void);

protected:
  void getVersion(void);

private:
  enum { NUM_LIBS=7 };
  QLabel       m_mainLabel;
  QGroupBox    m_ippInfoGroupBox;
  QLabel       m_ippLibraryLabel[NUM_LIBS];
  QLabel       m_ippNameLabel[NUM_LIBS];
  QLabel       m_ippVersionLabel[NUM_LIBS];
  QLabel       m_ippSignatureLabel[NUM_LIBS];
  QLabel       m_ippBuildDateLabel[NUM_LIBS];
  QPushButton  m_okButton;
  QGridLayout* m_ippInfoLayout;
  QHBoxLayout* m_buttonLayout;
  QVBoxLayout* m_centralLayout;
};

#endif // __ABOUTDLG_H__
