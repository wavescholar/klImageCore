/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __IMAGEWIDGET_H__
#define __IMAGEWIDGET_H__

#include <QtGui/QtGui>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif


class ImageWidget : public QLabel
{
  Q_OBJECT

public:
  ImageWidget(QWidget* parent = 0, Qt::WindowFlags = 0);
  ~ImageWidget(void);

  int Width(void)  { return m_roi.width; }
  int Height(void) { return m_roi.height; }

public:
  QLabel*  m_thumbLabel;
  QLabel*  m_nameLabel;
  QLabel*  m_ratingLabel;

  int      m_id;
  int      m_checked;
  int      m_nchannels;
  IppiSize m_roi;

private:
  QRect rectFromPoints(QPoint pt1, QPoint pt2);

private:
  QPoint m_lastPos;
  QPoint m_rectPoints[2];

private slots:
  void  mousePressEvent(QMouseEvent* event);
  void  mouseReleaseEvent(QMouseEvent* event);
  void  mouseDoubleClickEvent(QMouseEvent* event);
  void  mouseMoveEvent(QMouseEvent* event);
};

#endif // __IMAGEWIDGET_H__

