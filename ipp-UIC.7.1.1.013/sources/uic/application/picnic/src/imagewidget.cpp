/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MAINWINDOW_H__
#include "mainwindow.h"
#endif
#ifndef __IMAGEWIDGET_H__
#include "imagewidget.h"
#endif


ImageWidget::ImageWidget(QWidget* parent, Qt::WindowFlags flags) :
  QLabel(parent, flags)
{
  m_id            = 0;
  m_checked       = 0;
  m_nchannels     = 0;
  m_lastPos       = QPoint(-1, -1);
  m_rectPoints[0] = QPoint(-1, -1);
  m_rectPoints[1] = QPoint(-1, -1);

  m_thumbLabel = 0;
  m_nameLabel  = 0;

  m_roi.width  = 0;
  m_roi.height = 0;

  setMouseTracking(true);

  return;
} // ctor


ImageWidget::~ImageWidget(void)
{
  return;
} // dtor


QRect ImageWidget::rectFromPoints(QPoint pt1, QPoint pt2)
{
  QRect rectPoints;

  if(pt1.x() < pt2.x())
  {
    if(pt1.y() < pt2.y())
    {
      rectPoints.setTopLeft(pt1);
      rectPoints.setBottomRight(pt2);
    }
    else
    {
      rectPoints.setBottomLeft(pt1);
      rectPoints.setTopRight(pt2);
    }
  }
  else
  {
    if(pt1.y() < pt2.y())
    {
      rectPoints.setTopRight(pt1);
      rectPoints.setBottomLeft(pt2);
    }
    else
    {
      rectPoints.setBottomRight(pt1);
      rectPoints.setTopLeft(pt2);
    }
  }

  return rectPoints;
} // ImageWidget::rectFromPoints()


void ImageWidget::mousePressEvent(QMouseEvent* event)
{
  if(!g_pMainWnd->m_hasActiveImage && !g_pMainWnd->m_enablePaintAction->isChecked())
  {
    if(!m_checked)
    {
      if(0 != g_pMainWnd->m_imagesSelected.count())
      {
        ImageWidget* imageWidget = g_pMainWnd->m_pixmapLabels.at(g_pMainWnd->m_imagesSelected.last());
        imageWidget->m_checked = 0;
        imageWidget->setFrameStyle(QFrame::Box | QFrame::Sunken);
        g_pMainWnd->m_imagesSelected.clear();
      }

      m_checked = 1;
      g_pMainWnd->m_imagesSelected.append(m_id);
      setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    }
    else
    {
      m_checked = 0;
      g_pMainWnd->m_imagesSelected.removeAll(m_id);
      setFrameStyle(QFrame::Box | QFrame::Sunken);
    }
  }
  else if(g_pMainWnd->m_hasActiveImage && g_pMainWnd->m_enablePaintAction->isChecked())
  {
    if(event->button() == Qt::LeftButton)
    {
      QPainter painter(&g_pMainWnd->m_frame_pxm);
      g_pMainWnd->setupPainter(painter);
      painter.drawPoint(event->pos());
      painter.end();

      int point = event->pos().x() + (event->pos().y()) * g_pMainWnd->m_mask.Step();

      *((Ipp8u*)g_pMainWnd->m_mask + point) = 255;
      g_pMainWnd->updateView();

      m_lastPos = event->pos();
    }

    if(event->button() == Qt::RightButton)
    {
      m_rectPoints[0] = event->pos();
    }
  }

  return;
} // ImageWidget::mousePressEvent()


void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if(event->button() == Qt::RightButton &&
     g_pMainWnd->m_hasActiveImage &&
     g_pMainWnd->m_enablePaintAction->isChecked())
  {
    m_rectPoints[1] = event->pos();

    if(m_rectPoints[1].y() < 0)
      m_rectPoints[1].setY(0);

    QRect rectPoints = rectFromPoints(m_rectPoints[0], m_rectPoints[1]);

    QPainter painter(&g_pMainWnd->m_frame_pxm);

    g_pMainWnd->setupPainter(painter);

    painter.drawRect(rectPoints);
    painter.end();

    int y = rectPoints.y();
    int x = rectPoints.x();
    int h = y + ((rectPoints.height() >= 0) ? rectPoints.height() : -rectPoints.height());
    int w = x + ((rectPoints.width()  >= 0) ? rectPoints.width()  : -rectPoints.width());

    for(int i = y; i < h; i++)
      for(int j = x; j < w; j++)
        *((Ipp8u*)g_pMainWnd->m_mask + g_pMainWnd->m_mask.Step()*i + j) = 255;

    g_pMainWnd->updateView();
  }

  return;
} // ImageWidget::mouseReleaseEvent()


void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
  if((event->buttons() & Qt::LeftButton) &&
      m_lastPos != QPoint(-1, -1) &&
      g_pMainWnd->m_hasActiveImage &&
      g_pMainWnd->m_enablePaintAction->isChecked())
  {
    QPainter painter(&g_pMainWnd->m_frame_pxm);

    g_pMainWnd->setupPainter(painter);

    painter.drawLine(m_lastPos, event->pos());
    painter.end();

    int point = event->pos().x() + (event->pos().y()) * g_pMainWnd->m_mask.Step();

    if(point >= 0)
    {
      *((Ipp8u*)g_pMainWnd->m_mask + point) = 255;
      g_pMainWnd->updateView();
    }

    m_lastPos = event->pos();
  }
  else if((event->buttons() & Qt::RightButton) &&
           m_rectPoints[0] != QPoint(-1, -1) &&
           g_pMainWnd->m_hasActiveImage &&
           g_pMainWnd->m_enablePaintAction->isChecked())
  {
    m_rectPoints[1] = event->pos();

    QRect rectPoints = rectFromPoints(m_rectPoints[0], m_rectPoints[1]);

    QPixmap tmp_pxm(g_pMainWnd->m_frame_pxm);

    QPainter painter(&tmp_pxm);

    g_pMainWnd->setupPainter(painter, false);

    painter.drawRect(rectPoints);
    painter.end();

    g_pMainWnd->m_imageLabel->setPixmap(tmp_pxm);
    g_pMainWnd->m_imageLabel->adjustSize();
    g_pMainWnd->m_mainLayout->addWidget(g_pMainWnd->m_imageLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
  }

  return;
} // ImageWidget::mouseMoveEvent()


void ImageWidget::mouseDoubleClickEvent(QMouseEvent*)
{
  if(g_pMainWnd->m_timer2->isActive() || g_pMainWnd->m_timer3->isActive())
    return;

  if(!g_pMainWnd->m_hasActiveImage)
  {
    g_pMainWnd->m_currentFileIndex = m_id;
    g_pMainWnd->m_hasActiveImage   = true;
    g_pMainWnd->m_hasActiveThumbs  = false;
    g_pMainWnd->m_canSearch        = false;

    g_pMainWnd->m_fitSizeAction->setEnabled(true);
    g_pMainWnd->m_scrollArea->setWidgetResizable(false);

    if(!g_pMainWnd->m_pixmapLabels.isEmpty())
    {
      int cc = g_pMainWnd->m_mainLayout->columnCount();
      g_pMainWnd->m_mainLayout->setColumnStretch(cc - 1,0);

      foreach(ImageWidget* label, g_pMainWnd->m_pixmapLabels)
        label->hide();

      g_pMainWnd->m_gridBottomSpacer->changeSize(0, 0);
    }

    const FileNameFeature& currName =
      reinterpret_cast<const FileNameFeature&>(g_pMainWnd->m_descriptordb.Descriptor(g_pMainWnd->m_currentFileIndex).Feature(ImageFileName));

    if(currName.IsEmpty())
      return;

    QString fileName = currName.Get();

    g_pMainWnd->setCurrentFile(fileName);
    g_pMainWnd->m_imageLabel->show();
  }
  else
  {
    g_pMainWnd->prepareForThumbs();

    if(!g_pMainWnd->m_pixmapLabels.isEmpty())
    {
      foreach(ImageWidget* label, g_pMainWnd->m_pixmapLabels)
        label->show();

      int w = g_pMainWnd->width();
      int h = g_pMainWnd->height();

      QResizeEvent revent(QSize(w,h),QSize(w,h));
      g_pMainWnd->resizeEvent(&revent);
    }
    else
      g_pMainWnd->setCurrentFolder();
  }

  g_pMainWnd->updateMenus();

  return;
} // ImageWidget::mouseDoubleClickEvent()

