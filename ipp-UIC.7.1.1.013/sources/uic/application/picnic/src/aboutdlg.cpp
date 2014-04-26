/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#ifndef __ABOUTDLG_H__
#include "aboutdlg.h"
#endif

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __IPPCC_H__
#include "ippcc.h"
#endif
#ifndef __IPPJ_H__
#include "ippj.h"
#endif
#ifndef __IPPCV_H__
#include "ippcv.h"
#endif


AboutDlg::~AboutDlg(void)
{
  m_centralLayout->removeWidget(&m_mainLabel);
  m_centralLayout->removeWidget(&m_ippInfoGroupBox);

  if(0 != m_ippInfoLayout)
    delete m_ippInfoLayout;

  if(0 != m_buttonLayout)
    delete m_buttonLayout;

  if(0 != m_centralLayout)
    delete m_centralLayout;

  return;
} // dtor


bool AboutDlg::Init(void)
{
  setWindowTitle(tr("About Picnic"));

  m_mainLabel.setParent(this);
  m_mainLabel.setAlignment(Qt::AlignCenter);
  m_mainLabel.setText(tr("Intel(R) Integrated Perfomance Primitives Demo\n"
                         "Picture viewer, version 7.1\n"
                         "Copyright (c) 2008-2012 Intel Corporation"));

  m_ippInfoGroupBox.setParent(this);
  m_ippInfoGroupBox.setTitle(tr("Based on IPP"));
  m_ippInfoLayout = new QGridLayout(&m_ippInfoGroupBox);
  if(0 == m_ippInfoLayout)
    return true;

  for (int i = 0; i < NUM_LIBS; i++)
  {
    m_ippLibraryLabel[i].setParent(this);
    m_ippInfoLayout->addWidget(&m_ippLibraryLabel[i], 0, i);
    m_ippNameLabel[i].setParent(this);
    m_ippInfoLayout->addWidget(&m_ippNameLabel[i], 1, i);
    m_ippVersionLabel[i].setParent(this);
    m_ippInfoLayout->addWidget(&m_ippVersionLabel[i], 2, i);
    m_ippSignatureLabel[i].setParent(this);
    m_ippInfoLayout->addWidget(&m_ippSignatureLabel[i], 3, i);
    m_ippBuildDateLabel[i].setParent(this);
    m_ippInfoLayout->addWidget(&m_ippBuildDateLabel[i], 4, i);
  }

  m_okButton.setParent(this);
  m_okButton.setText(tr("&OK"));
  m_okButton.setDefault(true);
  connect(&m_okButton, SIGNAL(clicked()), this, SLOT(accept()));

  m_buttonLayout = new QHBoxLayout();
  if(0 == m_buttonLayout)
    return true;
  m_buttonLayout->addStretch();
  m_buttonLayout->addWidget(&m_okButton);

  m_centralLayout = new QVBoxLayout(this);
  if(0 == m_centralLayout)
    return true;
  m_centralLayout->addWidget(&m_mainLabel);
  m_centralLayout->addWidget(&m_ippInfoGroupBox);
  m_centralLayout->addLayout(m_buttonLayout);

  getVersion();

  adjustSize();
  setFixedSize(size());

  return false;
} // AboutDlg::Init()


void AboutDlg::getVersion(void)
{
  const IppLibraryVersion* libinfo;

  QString rawver;
  QString name;
  QString version;
  QString date;

  // headers
  m_ippLibraryLabel[0].setText(tr("Library"));
  m_ippNameLabel[0].setText(tr("Name"));
  m_ippVersionLabel[0].setText(tr("Version"));
  m_ippSignatureLabel[0].setText(tr("Signature"));
  m_ippBuildDateLabel[0].setText(tr("Build Date"));

  // get ippCore version info
  libinfo = ippGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[1].setText(tr("ippCore"));
  m_ippNameLabel[1].setText(name);
  m_ippVersionLabel[1].setText(version);
  m_ippSignatureLabel[1].setText(rawver);
  m_ippBuildDateLabel[1].setText(date);

  // get ippSP version info
  libinfo = ippsGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[2].setText(tr("ippSP"));
  m_ippNameLabel[2].setText(name);
  m_ippVersionLabel[2].setText(version);
  m_ippSignatureLabel[2].setText(rawver);
  m_ippBuildDateLabel[2].setText(date);

  // get ippIP version info
  libinfo = ippiGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[3].setText(tr("ippIP"));
  m_ippNameLabel[3].setText(name);
  m_ippVersionLabel[3].setText(version);
  m_ippSignatureLabel[3].setText(rawver);
  m_ippBuildDateLabel[3].setText(date);

  // get ippCC version info
  libinfo = ippccGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[4].setText(tr("ippCC"));
  m_ippNameLabel[4].setText(name);
  m_ippVersionLabel[4].setText(version);
  m_ippSignatureLabel[4].setText(rawver);
  m_ippBuildDateLabel[4].setText(date);

  // get ippJP version info
  libinfo = ippjGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[5].setText(tr("ippJP"));
  m_ippNameLabel[5].setText(name);
  m_ippVersionLabel[5].setText(version);
  m_ippSignatureLabel[5].setText(rawver);
  m_ippBuildDateLabel[5].setText(date);

  // get ippCV version info
  libinfo = ippcvGetLibVersion();

  rawver  = QString(tr("%1.%2.%3.%4")).arg(libinfo->major).arg(libinfo->minor).arg(libinfo->majorBuild).arg(libinfo->build);
  name    = libinfo->Name;
  version = libinfo->Version;
  date    = libinfo->BuildDate;

  m_ippLibraryLabel[6].setText(tr("ippCV"));
  m_ippNameLabel[6].setText(name);
  m_ippVersionLabel[6].setText(version);
  m_ippSignatureLabel[6].setText(rawver);
  m_ippBuildDateLabel[6].setText(date);

  return;
} // AboutDlg::getVersion()

