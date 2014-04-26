/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//         Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#include <QtGui/QApplication>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __MAINWINDOW_H__
#include "mainwindow.h"
#endif


int main(int argc, char* argv[])
{
  int r;

  ippStaticInit();

  QApplication app(argc, argv);

  MainWindow mw;

  r = mw.Init();
  if(0 != r)
    return -1;

  mw.show();

  return app.exec();
} // main()
