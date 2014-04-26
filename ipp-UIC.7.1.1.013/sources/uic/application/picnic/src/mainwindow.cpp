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
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "constants.h"

#ifndef __TIMER_H__
#include "timer.h"
#endif
#ifndef __MAINWINDOW_H__
#include "mainwindow.h"
#endif
#ifndef __ABOUTDLG_H__
#include "aboutdlg.h"
#endif
#ifndef __FILEOPENDLG_H__
#include "fileopendlg.h"
#endif
#ifndef __FILESAVEDLG_H__
#include "filesavedlg.h"
#endif
#ifndef __OPTIONSDIALOG_H__
#include "optionsdialog.h"
#endif
#ifndef __PIXMAP_H__
#include "pixmap.h"
#endif
#ifndef __FACEDETECTION_H__
#include "facedetection.h"
#endif
#ifndef __HARMONIZATION_H__
#include "harmonization.h"
#endif
#ifndef __SOBEL_H__
#include "sobel.h"
#endif
#ifndef __UIC_BASE_STREAM_INPUT_H__
#include "uic_base_stream_input.h"
#endif
#ifndef __UIC_BASE_STREAM_OUTPUT_H__
#include "uic_base_stream_output.h"
#endif
#ifndef __STDFILEIN_H__
#include "stdfilein.h"
#endif
#ifndef __MEMBUFFIN_H__
#include "membuffin.h"
#endif
#ifndef __MEMBUFFOUT_H__
#include "membuffout.h"
#endif
#ifndef __STDFILEOUT_H__
#include "stdfileout.h"
#endif
#ifndef __BMP_H__
#include "bmp.h"
#endif
#ifndef __JPEG_H__
#include "jpeg.h"
#endif
#ifndef __JPEG2K_H__
#include "jpeg2k.h"
#endif
#ifndef __DICOM_H__
#include "dicom.h"
#endif
#ifndef __RAW_H__
#include "raw.h"
#endif
#ifndef __COLORLAYOUTFEA_H__
#include "colorlayoutfea.h"
#endif
#ifndef __EDGEHISTOGRAMFEA_H__
#include "edgehistogramfea.h"
#endif
#ifndef __IMAGEFEADB_H__
#include "imagefeadb.h"
#endif

using namespace UIC;
using namespace ISF;

MainWindow* g_pMainWnd;

#define COMPANY_NAME   "Intel Corporation"
#define APP_NAME       "IPP Picnic"
#define MRU_FILES      "Recent Files"
#define MRU_FOLDER     "Recent Folder"


static QString shortName(const QString& fullName)
{
  return  QFileInfo(fullName).fileName();
} // shortName()


static QString pathName(const QString& fullName)
{
  return QDir::toNativeSeparators(QFileInfo(fullName).path() + QDir::separator());
} // pathName()


static QString get_str_sampling(IM_SAMPLING ss)
{
  QString str;
  switch(ss)
  {
    case IS_444: str = QString(QObject::tr("444"));     break;
    case IS_422: str = QString(QObject::tr("422"));     break;
    case IS_244: str = QString(QObject::tr("244"));     break;
    case IS_411: str = QString(QObject::tr("411"));     break;
    default:     str = QString(QObject::tr("Unknown")); break;
  }
  return str;
} // get_str_sampling()


static QString get_str_color(IM_COLOR color)
{
  QString str;

  switch(color)
  {
    case IC_GRAY:        str = QString(QObject::tr("Gray"));        break;
    case IC_GRAYA:       str = QString(QObject::tr("GrayAlpha"));   break;
    case IC_RGB:         str = QString(QObject::tr("RGB"));         break;
    case IC_BGR:         str = QString(QObject::tr("BGR"));         break;
    case IC_BGRA:        str = QString(QObject::tr("BGRA"));        break;
    case IC_RGBA:        str = QString(QObject::tr("RGBA"));        break;
    case IC_BGRAP:       str = QString(QObject::tr("BGRAP"));       break;
    case IC_RGBAP:       str = QString(QObject::tr("RGBAP"));       break;
    case IC_RGBE:        str = QString(QObject::tr("RGBE"));        break;
    case IC_YCBCR:       str = QString(QObject::tr("YCbCr"));       break;
    case IC_CMYK:        str = QString(QObject::tr("CMYK"));        break;
    case IC_CMYKA:       str = QString(QObject::tr("CMYKA"));       break;
    case IC_YCCK:        str = QString(QObject::tr("YCCK"));        break;
    default:             str = QString(QObject::tr("Unknown"));     break;
  }
  return str;
} // get_str_color()


static QString get_str_modeJPEG(int mode)
{
  QString str;
  switch(mode)
  {
    case 1:  str = QString(QObject::tr("BAS"));     break;
    case 2:  str = QString(QObject::tr("EXT"));     break;
    case 3:  str = QString(QObject::tr("PRG"));     break;
    case 4:  str = QString(QObject::tr("LSL"));     break;
    default: str = QString(QObject::tr("Unknown")); break;
  }
  return str;
} // get_str_mode()


static QString get_str_modeJPEG2K(int mode)
{
  QString str;
  switch(mode)
  {
    case 0:  str = QString(QObject::tr("LSL"));     break;
    case 1:  str = QString(QObject::tr("BAS"));     break;
    default: str = QString(QObject::tr("Unknown")); break;
  }
  return str;
} // get_str_mode()


QDataStream& operator>>(QDataStream& s, FileNameFeature& fileName)
{
  QString string;
  s >> string;

  QByteArray ba = string.toAscii();
  fileName.Set(ba);

  return s;
}


QDataStream& operator<<(QDataStream& s, const FileNameFeature& fileName)
{
  s << QString::fromAscii(fileName.Get());
  return s;
}


QDataStream& operator>>(QDataStream& s, ColorLayoutFeature& colorLayout)
{
  int cld[3][64];

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 64; j++)
      s >> cld[i][j];

  colorLayout.Set(cld);

  return s;
}


QDataStream& operator<<(QDataStream& s, const ColorLayoutFeature& colorLayout)
{
  int cld[3][64];

  colorLayout.Get(cld);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 64; j++)
      s << cld[i][j];

  return s;
}


QDataStream& operator>>(QDataStream& s, EdgeHistogramFeature& edgeHist)
{
  float ehd[80];

  for(int j = 0; j < 80; j++)
      s >> ehd[j];

  edgeHist.Set(ehd);

  return s;
}


QDataStream& operator<<(QDataStream& s, const EdgeHistogramFeature& edgeHist)
{
  float ehd[80];

  edgeHist.Get(ehd);

  for(int j = 0; j < 80; j++)
      s << ehd[j];

  return s;
}


QDataStream& operator>>(QDataStream& s, ImageDescriptor& descriptor)
{
  int i;
  int sz;
  FeatureId id;

  s >> sz;

  for(i = 0; i < sz; i++)
  {
    s >> (int&)id;
    switch(id)
    {
    case ImageFileName:
      {
        FileNameFeature* fileName = new FileNameFeature;
        if(0 == fileName)
          return s;

        fileName->Init();

        s >> *fileName;

        descriptor.Insert(*fileName);
      }
      break;

    case ImageColorLayout:
      {
        ColorLayoutFeature* colorLayout = new ColorLayoutFeature;
        if(0 == colorLayout)
          return s;

        colorLayout->Init();

        s >> *colorLayout;

        descriptor.Insert(*colorLayout);
      }
      break;

    case ImageEdgeHistogram:
      {
        EdgeHistogramFeature* edgeHist = new EdgeHistogramFeature;
        if(0 == edgeHist)
          return s;

        edgeHist->Init();

        s >> *edgeHist;

        descriptor.Insert(*edgeHist);
      }
      break;

    default:
      break;
    }
  }

  return s;
} // operator>>()


QDataStream& operator<<(QDataStream& s, const ImageDescriptor& descriptor)
{
  int i;
  int sz;

  sz = descriptor.Size();

  s << sz;

  for(i = 0; i < sz; i++)
  {
    const ImageFeatureBase& feature = descriptor.Feature(i);

    FeatureId id = feature.Id();

    if(UndefinedFeature == id)
      continue;

    switch(id)
    {
    case ImageFileName:
      {
        const FileNameFeature& fileName = reinterpret_cast<const FileNameFeature&>(feature);
        if(!fileName.IsEmpty())
        {
          s << (int)id;
          s << fileName;
        }
      }
      break;

    case ImageColorLayout:
      {
        const ColorLayoutFeature& colorLayout = reinterpret_cast<const ColorLayoutFeature&>(feature);
        if(!colorLayout.IsEmpty())
        {
          s << (int)id;
          s << colorLayout;
        }
      }
      break;

    case ImageEdgeHistogram:
      {
        const EdgeHistogramFeature& edgeHist = reinterpret_cast<const EdgeHistogramFeature&>(feature);
        if(!edgeHist.IsEmpty())
        {
          s << (int)id;
          s << edgeHist;
        }
      }
      break;

    default:
      break;
    }
  }

  return s;
} // operator<<()


QDataStream& operator>>(QDataStream& s, ImageDescriptorDB& db)
{
  int i;
  int v;
  int sz;

  s >> v;
  s >> sz;

  for(i = 0; i < sz; i++)
  {
    ImageDescriptor* descriptor = new ImageDescriptor;
    if(0 == descriptor)
      return s;

    descriptor->Init();

    s >> *descriptor;
    db.Insert(*descriptor);
  }

  return s;
} // operator>>()


QDataStream& operator<<(QDataStream& s, const ImageDescriptorDB& db)
{
  int i;
  int v;
  int sz;

  if(db.IsEmpty())
    return s;

  v  = db.Version();
  sz = db.Size();

  s << v;  // emit DB version
  s << sz; // emit number of descriptors in DB

  for(i = 0; i < sz; i++)
  {
    if(!db.Descriptor(i).IsEmpty())
    {
      // emit ImageDescriptor
      s << db.Descriptor(i);
    }
  }

  return s;
} // operator<<()


void DestroyImageDescriptorDB(ImageDescriptorDB& db)
{
  int i;

  for(i = 0; i < db.Size(); i++)
  {
    ImageDescriptor& descriptor = (ImageDescriptor&)db.Descriptor(i);

    if(descriptor.IsEmpty())
      continue;

    FileNameFeature& name = (FileNameFeature&)descriptor.Feature(ImageFileName);
    if(!name.IsEmpty())
    {
      delete &name;
      descriptor.Remove(ImageFileName);
    }

    ColorLayoutFeature& cld = (ColorLayoutFeature&)descriptor.Feature(ImageColorLayout);
    if(!cld.IsEmpty())
    {
      delete &cld;
      descriptor.Remove(ImageColorLayout);
    }

    EdgeHistogramFeature& ehd = (EdgeHistogramFeature&)descriptor.Feature(ImageEdgeHistogram);
    if(!ehd.IsEmpty())
    {
      delete &ehd;
      descriptor.Remove(ImageEdgeHistogram);
    }

    delete &descriptor;
    db.Remove(i);
  }

  db.Clear();

  return;
} // DestroyImageDescriptorDB()


int MainWindow::Init(void)
{
  setWindowTitle(tr(APP_NAME));

  m_canPlayDicom     = false;
  m_canSlideshow     = false;
  m_hasActiveImage   = false;
  m_hasActiveThumbs  = false;
  m_inpaintActive    = false;
  m_do_segmentation  = false;
  m_do_inpainting    = false;
  m_do_facedetection = false;
  m_do_harmonize     = false;
  m_do_sobel         = false;
  m_effectEnded      = false;
  m_needToRedraw     = false;
  m_canSearch        = false;
  m_currentFileIndex = 0;
  m_vScrollOldValue  = 0;

  g_pMainWnd = this;

  qApp->setWheelScrollLines(1);

  QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);

  m_centralWidget = new QWidget(this);
  if(0 == m_centralWidget)
    return -1;

  m_scrollArea = new QScrollArea(this);
  if(0 == m_scrollArea)
    return -1;

  m_scrollArea->setSizePolicy(sizePolicy);
  m_scrollArea->verticalScrollBar()->setSingleStep(152);
  m_scrollArea->verticalScrollBar()->setPageStep(1);
  m_scrollArea->setWidget(m_centralWidget);
  connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vBarScrolled(int)));

  setCentralWidget(m_scrollArea);

  m_mainLayout = new QGridLayout(m_centralWidget);
  if(0 == m_mainLayout)
    return -1;

  m_mainLayout->setMargin(1);
  m_mainLayout->setSpacing(1);


  m_imageLabel = new ImageWidget(m_centralWidget);
  if(0 == m_imageLabel)
    return -1;

  m_imageLabel->setBackgroundRole(QPalette::Window);
  m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_imageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_imageLabel->setScaledContents(true);
  m_imageLabel->hide();


  m_gridBottomSpacer = new QSpacerItem(0, 0);
  if(0 == m_gridBottomSpacer)
    return -1;

  m_descriptordb.Init();

  createDocks();
  createStatusBar();
  setStatusBar(m_statusBar);
  createActions();
  createMenus();
  createToolBars();
  setup();
  updateMenus();
  adjustSize();

  resize(750, 500);

  m_app_settings = new QSettings(tr(COMPANY_NAME), tr(APP_NAME));
  if(0 == m_app_settings)
    return -1;

  m_recentFiles  = m_app_settings->value(tr(MRU_FILES)).toStringList();
  m_recentFolder = m_app_settings->value(tr(MRU_FOLDER)).toString();

  updateRecentFileActions();

  m_optionsDlg = new OptionsDialog(this, Qt::Window);
  if(0 == m_optionsDlg)
    return -1;

  m_optionsDlg->hide();

  for(int i = 0; i < m_optionsDlg->osdCount(); i++)
  {
    m_osd_text.append(new QLabel(m_imageLabel));
    m_osd_text.last()->setPalette(QPalette(QPalette::BrightText));
  }

  // init performance timer
  m_timer.Init();

  m_timer2 = new QTimer(this);
  if(0 == m_timer2)
    return -1;

  m_timer2->setSingleShot(false);
  connect(m_timer2, SIGNAL(timeout(void)), this, SLOT(circle(void)));

  m_timer3 = new QTimer(this);
  if(0 == m_timer3)
    return -1;

  m_timer3->setSingleShot(false);
  connect(m_timer3, SIGNAL(timeout(void)), this, SLOT(playDICOM(void)));

  m_param_jpeg.color            = IC_UNKNOWN;
  m_param_jpeg.huffman_opt      = 0;
  m_param_jpeg.mode             = JPEG_BASELINE;
  m_param_jpeg.point_transform  = 0;
  m_param_jpeg.predictor        = 1;
  m_param_jpeg.quality          = 75;
  m_param_jpeg.restart_interval = 0;
  m_param_jpeg.sampling         = IS_444;
  m_param_jpeg.dct_scale        = JD_1_1;
  m_param_jpeg.use_qdct         = 1;
  m_param_jpeg.comment_size     = sizeof(m_param_jpeg.comment);

  ippsZero_8u((Ipp8u*)&m_param_jpeg2k,sizeof(m_param_jpeg2k));
  ippsZero_8u((Ipp8u*)&m_param_dicom,sizeof(m_param_dicom));
  ippsZero_8u((Ipp8u*)&m_param_raw,sizeof(m_param_raw));
  ippsZero_8u((Ipp8u*)&m_param_png,sizeof(m_param_png));
  ippsZero_8u((Ipp8u*)&m_param_jpegxr,sizeof(m_param_jpegxr));

  ippsZero_8u((Ipp8u*)&m_param_harmonization,sizeof(m_param_harmonization));
  ippsZero_8u((Ipp8u*)&m_param_sobel,sizeof(m_param_sobel));
  ippsZero_8u((Ipp8u*)&m_param_facedetection,sizeof(m_param_facedetection));
  ippsZero_8u((Ipp8u*)&m_param_inpainting,sizeof(m_param_inpainting));
  ippsZero_8u((Ipp8u*)&m_param_segmentation,sizeof(m_param_segmentation));

  memset(&m_osd,0,sizeof(OSD_VALS));

  return 0;
} // MainWindow::Init()


int MainWindow::Free(void)
{
  int i;

  if(0 != m_scrollArea)
    delete m_scrollArea;

  if(0 != m_statusBar)
    delete m_statusBar;

  if(0 != m_loadAction)
    delete m_loadAction;

  if(0 != m_saveAction)
    delete m_saveAction;

  if(0 != m_saveDbAction)
    delete m_saveDbAction;

  if(0 != m_exitAction)
    delete m_exitAction;

  for(i = 0; i < MAXRECENTFILES; i++)
  {
    if(0 != m_recentFileActions[i])
      delete m_recentFileActions[i];
  }

  if(0 != m_fitSizeAction)
    delete m_fitSizeAction;

  if(0 != m_showStatusBarAction)
    delete m_showStatusBarAction;

  if(0 != m_facedetectionAction)
    delete m_facedetectionAction;

  if(0 != m_optionsAction)
    delete m_optionsAction;

  if(0 != m_segmentationAction)
    delete m_segmentationAction;

  if(0 != m_inpaintAction)
    delete m_inpaintAction;

  if(0 != m_harmonizationAction)
    delete m_harmonizationAction;

  if(0 != m_sobelAction)
    delete m_sobelAction;

  if(0 != m_playDicomAction)
    delete m_playDicomAction;

  if(0 != m_restoreAction)
    delete m_restoreAction;

  if(0 != m_nextAction)
    delete m_nextAction;

  if(0 != m_previousAction)
    delete m_previousAction;

  if(0 != m_aboutAction)
    delete m_aboutAction;

  if(0 != m_slideshowAction)
    delete m_slideshowAction;

  if(0 != m_enablePaintAction)
    delete m_enablePaintAction;

  if(0 != m_app_settings)
    delete m_app_settings;

  if(0 != m_optionsDlg)
    delete m_optionsDlg;

  if(0 != m_timer2)
    delete m_timer2;

  if(0 != m_timer3)
    delete m_timer3;

  DestroyImageDescriptorDB(m_descriptordb);

  return 0;
} // MainWindow::Free()


void MainWindow::closeEvent(QCloseEvent*)
{
  m_app_settings->setValue(tr(MRU_FILES),  QVariant(m_recentFiles));
  m_app_settings->setValue(tr(MRU_FOLDER), QVariant(m_recentFolder));

  return;
} // MainWindow::closeEvent()


void MainWindow::resizeEvent(QResizeEvent*)
{
  if(!m_pixmapLabels.isEmpty() && !m_hasActiveImage)
  {
    int vSliderWidth = m_scrollArea->verticalScrollBar()->width();
    int ncolsToShow  = (width() - vSliderWidth) / 151;
    int nrowsToShow  = height() / 151;

    int nd = 0;
    int nr;
    int nc;
    int startRow = 0;
    int stopRow;
    int ncol;
    int cc;

    if(ncolsToShow != (m_mainLayout->columnCount() - 1) || (m_mainLayout->rowCount() - 2) != nrowsToShow || m_needToRedraw)
    {
      m_needToRedraw = false;

      m_mainLayout->removeItem(m_gridBottomSpacer);

      foreach(ImageWidget* label, m_pixmapLabels)
        m_mainLayout->removeWidget(label);

      delete m_mainLayout;
      m_mainLayout = new QGridLayout(m_centralWidget);
      if(0 == m_mainLayout)
        return;

      m_mainLayout->setMargin(1);
      m_mainLayout->setSpacing(1);

      int pl_counts = m_pixmapLabels.count();
      stopRow = (pl_counts + (ncolsToShow - 1)) / ncolsToShow;

      for(nr = startRow; nr < stopRow; nr++)
      {
        ncol = (((nr + 1)*ncolsToShow) < pl_counts) ? ncolsToShow : (pl_counts - nr*ncolsToShow);
        for(nc = 0; nc < ncol; nc++)
        {
          m_mainLayout->addWidget(m_pixmapLabels.at(nd+nc), nr, nc, Qt::AlignLeft | Qt::AlignTop);
        }

        nd += ncol;
      }

      cc = m_mainLayout->columnCount();
      if((cc - 1) < ncolsToShow)
        m_mainLayout->setColumnStretch(ncolsToShow - 1, 0);
      else
        m_mainLayout->setColumnStretch(ncolsToShow + 1, 0);
    } // if

    m_mainLayout->setColumnStretch(ncolsToShow, 100);

    int vSliderValue   = m_scrollArea->verticalScrollBar()->value();
    int topRow         = (vSliderValue + 150) / 151;
    int shownNum       = m_mainLayout->count() - topRow*ncolsToShow;

    int nFiles         = m_descriptordb.Size();
    int leftToShow     = IPP_MIN(nrowsToShow*ncolsToShow - shownNum, nFiles - m_pixmapLabels.count());
    int beginShowIndex = m_pixmapLabels.count();
    int endShowIndex   = m_pixmapLabels.count() + leftToShow;
    int Id             = beginShowIndex;

    int row;
    int col;

    if(nrowsToShow*ncolsToShow > shownNum)
    {
      for(int index = beginShowIndex; index < endShowIndex; index++)
      {
        row = index / ncolsToShow;
        col = index % ncolsToShow;

        bool bres = true;

        const FileNameFeature& currName =
          reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(Id).Feature(ImageFileName));

        if(currName.IsEmpty())
          continue;

        m_currentFile = currName.Get();

        if(!m_currentFile.isEmpty())
          bres = openFile(m_currentFile);

        if(bres == false)
        {
          createFrame(m_image, m_frame);

          float xFactor = 146.0 / m_frame.Width();
          float yFactor = 110.0 / m_frame.Height();

          (xFactor < yFactor) ? yFactor = xFactor : xFactor = yFactor;

          if(xFactor > 1 || yFactor > 1)
            xFactor = yFactor = 1;

          IppiSize thumbnailRoi;
          thumbnailRoi.width  = m_frame.Width()  * xFactor;
          thumbnailRoi.height = m_frame.Height() * yFactor;

          CIppImage thumbnail;
          thumbnail.Alloc(thumbnailRoi,m_frame.NChannels(),m_frame.Precision());

          int res = m_frame.Resize(thumbnail,thumbnail.Step(),thumbnail.Size());

          if(0 == res)
          {
            m_pixmapLabels.append(createLabel(thumbnail, m_frame.Size(), Id++));
            m_mainLayout->addWidget(m_pixmapLabels.last(), row, col, Qt::AlignLeft | Qt::AlignTop);
            qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
          }

          thumbnail.Free();
          m_frame.Free();
        }
        else
        {
          IppiSize thumbnailRoi;
          thumbnailRoi.width  = 146;
          thumbnailRoi.height = 110;

          CIppImage thumbnail;
          thumbnail.Alloc(thumbnailRoi,1,8);
          ippiSet_8u_C1R(0,(Ipp8u*)thumbnail, thumbnail.Step(),thumbnailRoi);

          m_pixmapLabels.append(createLabel(thumbnail, m_image.Size(), Id++));
          m_mainLayout->addWidget(m_pixmapLabels.last(), row, col, Qt::AlignLeft | Qt::AlignTop);

          thumbnail.Free();
          m_frame.Free();
        }
      } // for index
    }

    nFiles = m_descriptordb.Size();

    int bottomSpacerHeight;
    int nRows = ((nFiles +(ncolsToShow - 1)) / ncolsToShow);

    int curRow  = m_pixmapLabels.count() / ncolsToShow;

    if(nRows < nrowsToShow)
      bottomSpacerHeight = height() - (curRow + 1)*151;
    else
      bottomSpacerHeight = (IPP_MAX(nRows, nrowsToShow) - (curRow + 1))*151;

    m_mainLayout->removeItem(m_gridBottomSpacer);
    m_mainLayout->addItem(m_gridBottomSpacer, curRow + 1, 0);
    m_gridBottomSpacer->changeSize(0, bottomSpacerHeight);
  }
  else if(m_hasActiveImage)
  {
    fitImageSize();
  }

  return;
} // MainWindow::resizeEvent()


void MainWindow::vBarScrolled(int value)
{
  if(!m_pixmapLabels.isEmpty() && !m_hasActiveImage)
  {
    if((m_vScrollOldValue + 152) < value)
    {
      value = m_vScrollOldValue + 152;
      m_scrollArea->verticalScrollBar()->setValue(value);
    }
    else
    {
      m_vScrollOldValue = value;
    }

    int leftToShow;
    int vSliderWidth = m_scrollArea->verticalScrollBar()->width();
    int topRow       = ((value + 150)) / 151;
    int ncolsToShow  = (width() - vSliderWidth) / 151;
    int nrowsToShow  = height() / 151;
    int shownNum     = m_mainLayout->count() - topRow*ncolsToShow - 1;

    if(nrowsToShow*ncolsToShow > shownNum)
    {
      int nFiles = m_descriptordb.Size();
      leftToShow = IPP_MIN(nrowsToShow*ncolsToShow - shownNum, nFiles - m_pixmapLabels.count());

      int stopCol;
      int beginShowIndex = m_pixmapLabels.count();
      int stopShowIndex  = m_pixmapLabels.count() + leftToShow;
      int Id             = beginShowIndex;
      int startRow       = (beginShowIndex + ncolsToShow - 1) / ncolsToShow;
      int stopRow        = (stopShowIndex  + ncolsToShow - 1) / ncolsToShow;

      for(int nr = startRow; nr < stopRow; nr++)
      {
        stopCol = (ncolsToShow*(nr+1) < nFiles) ? ncolsToShow : nFiles - ncolsToShow*(stopRow - 1);

        for(int nc = 0; nc < stopCol; nc++)
        {
          bool bres = true;

          const FileNameFeature& currName =
            reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(Id).Feature(ImageFileName));

          if(currName.IsEmpty())
            return;

          m_currentFile = currName.Get();

          if(!m_currentFile.isEmpty())
            bres = openFile(m_currentFile);

          if(bres == false)
          {
            createFrame(m_image, m_frame);

            float xFactor = 146.0 / m_frame.Width();
            float yFactor = 110.0 / m_frame.Height();

            (xFactor < yFactor) ? yFactor = xFactor : xFactor = yFactor;

            if(xFactor > 1 || yFactor > 1)
              xFactor = yFactor = 1;

            IppiSize thumbnailRoi;
            thumbnailRoi.width  = m_frame.Width()  * xFactor;
            thumbnailRoi.height = m_frame.Height() * yFactor;

            CIppImage thumbnail;
            thumbnail.Alloc(thumbnailRoi,m_frame.NChannels(),m_frame.Precision());

            int res;

            if(xFactor != 1 || yFactor != 1)
              res = m_frame.Resize(thumbnail,thumbnail.Step(),thumbnail.Size());
            else
              res = m_frame.CopyTo((Ipp8u*)thumbnail,thumbnail.Step(),thumbnail.Size());

            if(0 == res)
            {
              m_pixmapLabels.append(createLabel(thumbnail, m_frame.Size(), Id++));
              m_mainLayout->addWidget(m_pixmapLabels.last(), nr, nc, Qt::AlignLeft | Qt::AlignTop);
            }

            thumbnail.Free();
            m_frame.Free();
          }
          else
          {
            IppiSize thumbnailRoi;
            thumbnailRoi.width  = 146;
            thumbnailRoi.height = 110;

            CIppImage thumbnail;
            thumbnail.Alloc(thumbnailRoi,1,8);
            ippiSet_8u_C1R(0,(Ipp8u*)thumbnail, thumbnail.Step(),thumbnailRoi);

            m_pixmapLabels.append(createLabel(thumbnail, m_image.Size(), Id++));
            m_mainLayout->addWidget(m_pixmapLabels.last(), nr, nc, Qt::AlignLeft | Qt::AlignTop);

            thumbnail.Free();
            m_frame.Free();
          } // for nr
        } // for nr
      } // for nc

      nFiles = m_descriptordb.Size();

      int bottomSpacerHeight;
      int nRows = ((nFiles +(ncolsToShow - 1)) / ncolsToShow);
      bottomSpacerHeight = (IPP_MAX(nRows, nrowsToShow) - (stopRow))*151;

      m_gridBottomSpacer->changeSize(0, bottomSpacerHeight);
      m_mainLayout->removeItem(m_gridBottomSpacer);
      m_mainLayout->addItem(m_gridBottomSpacer, stopRow, 0);
    }
  }

  return;
} // MainWindow::vBarScrolled()


void MainWindow::createDocks(void)
{
  QLabel* dicomLabel = new QLabel;

  if(0 == dicomLabel)
    return;

  dicomLabel->setMinimumSize(200, 60);

  QGroupBox* dicomSliderGroupBox = new QGroupBox(dicomLabel);
  if(0 == dicomSliderGroupBox)
    return;

  QHBoxLayout* dicomSliderLayout = new QHBoxLayout(dicomSliderGroupBox);
  if(0 == dicomSliderLayout)
    return;

  m_dicomSlider = new QSlider(Qt::Horizontal, this);
  if(0 == m_dicomSlider)
    return;

  m_dicomSlider->setRange(1, m_param_dicom.nFrames - 1);
  m_dicomSlider->setFixedWidth(100);
  m_dicomSlider->setValue(1);

  m_dicomSpinBox = new QSpinBox(this);
  if(0 == m_dicomSpinBox)
    return;

  m_dicomSpinBox->setRange(1, m_param_dicom.nFrames - 1);
  m_dicomSpinBox->setFixedWidth(55);
  m_dicomSpinBox->setValue(1);

  connect(m_dicomSlider, SIGNAL(valueChanged(int)), this, SLOT(playDICOM(int)));
  connect(m_dicomSlider, SIGNAL(valueChanged(int)), m_dicomSpinBox, SLOT(setValue(int)));
  connect(m_dicomSpinBox, SIGNAL(valueChanged(int)), m_dicomSlider, SLOT(setValue(int)));

  dicomSliderLayout->addWidget(m_dicomSpinBox);
  dicomSliderLayout->addWidget(m_dicomSlider);

  m_docks.append(new QDockWidget(tr("DICOM frames"), this));

  m_docks.at(0)->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  m_docks.at(0)->setFeatures(QDockWidget::NoDockWidgetFeatures);
  m_docks.at(0)->setWidget(dicomLabel);
  m_docks.at(0)->hide();

  addDockWidget(Qt::RightDockWidgetArea, m_docks.at(0));

  return;
} // MainWindow::createDocks()


void MainWindow::createStatusBar(void)
{
  m_statusBar = new QStatusBar(this);
  if(0 == m_statusBar)
    return;

  m_hintLabel = new QLabel();
  if(0 == m_hintLabel)
    return;

  m_fileCounterLabel = new QLabel(("WWWWWW"));
  if(0 == m_fileCounterLabel)
    return;

  m_fileCounterLabel->setMinimumSize(m_fileCounterLabel->sizeHint());
  m_fileCounterLabel->setAlignment(Qt::AlignCenter);

  m_fileFormatLabel = new QLabel(("WWWW"));
  if(0 == m_fileFormatLabel)
    return;

  m_fileFormatLabel->setMinimumSize(m_fileFormatLabel->sizeHint());
  m_fileFormatLabel->setAlignment(Qt::AlignCenter);

  m_propertiesLabel = new QLabel(("WWWWWWWWW"));
  if(0 == m_propertiesLabel)
    return;

  m_propertiesLabel->setMinimumSize(m_propertiesLabel->sizeHint());
  m_propertiesLabel->setAlignment(Qt::AlignCenter);

  m_samplingLabel = new QLabel(("WWWW"));
  if(0 == m_samplingLabel)
    return;

  m_samplingLabel->setMinimumSize(m_samplingLabel->sizeHint());
  m_samplingLabel->setAlignment(Qt::AlignCenter);

  m_colorLabel = new QLabel(("WWWWW"));
  if(0 == m_colorLabel)
    return;

  m_colorLabel->setMinimumSize(m_colorLabel->sizeHint());
  m_colorLabel->setAlignment(Qt::AlignCenter);

  m_cpeLabel = new QLabel(("WWWWWWWW"));
  if(0 == m_cpeLabel)
    return;

  m_cpeLabel->setMinimumSize(m_cpeLabel->sizeHint());
  m_cpeLabel->setAlignment(Qt::AlignCenter);

  m_usecLabel = new QLabel(("WWWWWWWW"));
  if(0 == m_usecLabel)
    return;

  m_usecLabel->setMinimumSize(m_usecLabel->sizeHint());
  m_usecLabel->setAlignment(Qt::AlignCenter);

  m_statusBar->addWidget(m_hintLabel, 1);
  m_statusBar->addPermanentWidget(m_fileCounterLabel);
  m_statusBar->addPermanentWidget(m_fileFormatLabel);
  m_statusBar->addPermanentWidget(m_propertiesLabel);
  m_statusBar->addPermanentWidget(m_samplingLabel);
  m_statusBar->addPermanentWidget(m_colorLabel);
  m_statusBar->addPermanentWidget(m_cpeLabel);
  m_statusBar->addPermanentWidget(m_usecLabel);

  updateStatusBar();

  return;
} // MainWindow::createStatusBar()


void MainWindow::cleanStatusBar(void)
{
  m_fileCounterLabel->clear();
  m_fileFormatLabel->clear();
  m_propertiesLabel->clear();
  m_samplingLabel->clear();
  m_colorLabel->clear();
  m_usecLabel->clear();
  m_cpeLabel->clear();

  return;
} // MainWindow::cleanStatusBar()


void MainWindow::updateStatusBar(
  QString  filefmt,
  int      width,
  int      height,
  int      nchannels,
  int      precision,
  QString  sampling,
  QString  color,
  float    cpe,
  float    sec)
{
  QString string;

  int count = m_descriptordb.Size();
  if(count)
  {
    // index is zero based, but for convenience we display it as starting from 1
    string = QString(tr("%1/%2")).arg(m_currentFileIndex+1).arg(count);
  }

  m_fileCounterLabel->setText(string);

  string = filefmt;
  m_fileFormatLabel->setText(string);

  if ((width == -1) || (height == -1) || (nchannels == -1) || (precision == -1))
    string = QString(tr(""));
  else
    string = QString(tr("%1x%2x%3 %4")).arg(width).arg(height).arg(nchannels).arg(precision);

  m_propertiesLabel->setText(string);

  string = sampling;
  m_samplingLabel->setText(string);

  string = color;
  m_colorLabel->setText(string);

  if(cpe == -1)
    string = QString(tr("       "));
  else
    string = QString(tr("CPE: %1")).arg(cpe, 0, 'f', 1);

  if(cpe != -2)
    m_cpeLabel->setText(string);

  if(sec == -1)
    string = QString(tr("        "));
  else
    string = QString(tr("MSEC: %1")).arg(sec, 0, 'f', 1);

  if(sec != -2)
    m_usecLabel->setText(string);

  return;
} // MainWindow::updateStatusBar()


void MainWindow::createActions(void)
{
  m_loadAction = new QAction(tr("&Open"), this);
  if(0 == m_loadAction)
    return;

  m_loadAction->setIcon(QIcon(":/res/fileopen.xpm"));
  m_loadAction->setStatusTip(tr("Load image from file"));
  connect(m_loadAction, SIGNAL(triggered()), this, SLOT(OnFileOpen()));

  m_saveAction = new QAction(tr("&Save As..."), this);
  if(0 == m_saveAction)
    return;

  m_saveAction->setIcon(QIcon(":/res/filesave.xpm"));
  m_saveAction->setStatusTip(tr("Save file As"));
  connect(m_saveAction, SIGNAL(triggered()), this, SLOT(OnFileSave()));

  m_saveDbAction = new QAction(tr("Save DB"), this);
  if(0 == m_saveDbAction)
    return;

  m_saveDbAction->setStatusTip(tr("Save image DB"));
  connect(m_saveDbAction, SIGNAL(triggered()), this, SLOT(OnImageDbSave()));

  m_exitAction = new QAction(tr("E&xit"), this);
  if(0 == m_exitAction)
    return;

  m_exitAction->setStatusTip(tr("Exit application"));
  connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

  for(int i = 0; i < MAXRECENTFILES; i++)
  {
    m_recentFileActions[i] = new QAction(this);
    if(0 == m_recentFileActions[i])
      return;

    m_recentFileActions[i]->setVisible(false);
    connect(m_recentFileActions[i], SIGNAL(triggered()), this, SLOT(OnMRUFileOpen()));
  }

  m_fitSizeAction = new QAction(tr("Fit image size"), this);
  if(0 == m_fitSizeAction)
    return;

  m_fitSizeAction->setCheckable(true);
  connect(m_fitSizeAction, SIGNAL(triggered()), this, SLOT(fitImageSize()));

  m_showToolBarAction = new QAction(tr("Show Toolbar"), this);
  if(0 == m_showToolBarAction)
    return;

  m_showToolBarAction->setCheckable(true);

  m_showStatusBarAction = new QAction(tr("Show Status Bar"), this);
  if(0 == m_showStatusBarAction)
    return;

  m_showStatusBarAction->setStatusTip(tr("Show Status Bar"));
  m_showStatusBarAction->setCheckable(true);

  m_facedetectionAction = new QAction(tr("Face Detection"), this);
  if(0 == m_facedetectionAction)
    return;

  m_facedetectionAction->setStatusTip(tr("Find a face in the image"));
  connect(m_facedetectionAction, SIGNAL(triggered()), this, SLOT(OnFaceDetection()));

  m_optionsAction = new QAction(tr("Options"), this);
  if(0 == m_optionsAction)
    return;

  m_optionsAction->setStatusTip(tr("Show options window"));
  connect(m_optionsAction, SIGNAL(triggered()), this, SLOT(options()));

  m_segmentationAction = new QAction(tr("Segmentation"), this);
  if(0 == m_segmentationAction)
    return;

  m_segmentationAction->setStatusTip(tr("Apply segmentation to the image"));
  connect(m_segmentationAction, SIGNAL(triggered()), this, SLOT(OnSegmentation()));

  m_inpaintAction = new QAction(tr("Inpainting"), this);
  if(0 == m_inpaintAction)
    return;

  m_inpaintAction->setStatusTip(tr("Apply inpainting to the image"));
  connect(m_inpaintAction, SIGNAL(triggered()), this, SLOT(OnInpainting()));


  m_harmonizationAction = new QAction(tr("Harmonization"), this);
  if(0 == m_harmonizationAction)
    return;

  m_harmonizationAction->setStatusTip(tr("Apply harmonization filter to the image"));
  connect(m_harmonizationAction, SIGNAL(triggered()), this, SLOT(OnHarmonization()));

  m_sobelAction = new QAction(tr("Sobel"), this);
  if(0 == m_sobelAction)
    return;

  m_sobelAction->setStatusTip(tr("Apply sobel filter to the image"));
  connect(m_sobelAction, SIGNAL(triggered()), this, SLOT(OnSobel()));

  m_playDicomAction = new QAction(tr("Play DICOM"), this);
  if(0 == m_playDicomAction)
    return;

  m_playDicomAction->setStatusTip(tr("Play dicom image"));
  connect(m_playDicomAction, SIGNAL(triggered()), this, SLOT(startPlayDICOM()));

  m_restoreAction = new QAction(tr("Restore image"), this);
  if(0 == m_restoreAction)
    return;

  m_restoreAction->setStatusTip(tr("Restore initial image"));
  connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(restore()));

  m_nextAction = new QAction(tr("Next"), this);
  if(0 == m_nextAction)
    return;

  m_nextAction->setStatusTip(tr("Load next image in the current directory"));
  m_nextAction->setIcon(QIcon(":/res/next.xpm"));
  connect(m_nextAction, SIGNAL(triggered()), this, SLOT(OnNext()));

  m_previousAction = new QAction(tr("Previous"), this);
  if(0 == m_previousAction)
    return;

  m_previousAction->setStatusTip(tr("Load previous image in the current directory"));
  m_previousAction->setIcon(QIcon(":/res/prev.xpm"));
  connect(m_previousAction, SIGNAL(triggered()), this, SLOT(OnPrevious()));

  m_aboutAction = new QAction(tr("About"), this);
  if(0 == m_aboutAction)
    return;

  m_aboutAction->setStatusTip(tr("Show about window"));

  connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(OnAbout()));

  m_aboutQtAction = new QAction(tr("About &Qt"), this);
  connect(m_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  m_slideshowAction = new QAction(tr("Start slideshow"), this);
  if(0 == m_slideshowAction)
    return;

  m_slideshowAction->setStatusTip(tr("Start/stop slideshow"));
  connect(m_slideshowAction, SIGNAL(triggered()), this, SLOT(slideshow()));

  m_imageAllFeatureAction = new QAction(tr("Extract image features"), this);
  if(0 == m_imageAllFeatureAction)
    return;

  m_imageAllFeatureAction->setStatusTip(tr("Extract image features"));
  connect(m_imageAllFeatureAction, SIGNAL(triggered()), this, SLOT(OnExtractAll()));

  m_imageSeachByColorAction = new QAction(tr("ColorLayout"), this);
  if(0 == m_imageSeachByColorAction)
    return;

  m_imageSeachByColorAction->setStatusTip(tr("Search image by color layout"));
  connect(m_imageSeachByColorAction, SIGNAL(triggered()), this, SLOT(OnSearchByColorLayout()));

  m_imageSeachByEdgeAction = new QAction(tr("EdgeHistogram"), this);
  if(0 == m_imageSeachByEdgeAction)
    return;

  m_imageSeachByEdgeAction->setStatusTip(tr("Search image by edge histogram"));
  connect(m_imageSeachByEdgeAction, SIGNAL(triggered()), this, SLOT(OnSearchByEdgeHistogram()));

  m_enablePaintAction = new QAction(tr("Enable mask paint"), this);
  if(0 == m_enablePaintAction)
    return;

  m_enablePaintAction->setCheckable(true);
  connect(m_enablePaintAction, SIGNAL(triggered()), this, SLOT(createMask()));
  connect(m_enablePaintAction, SIGNAL(toggled(bool)), m_inpaintAction, SLOT(setEnabled(bool)));

  return;
} // MainWindow::createActions()


void MainWindow::createMenus(void)
{
  QMenuBar* pMenuBar = menuBar();

  m_fileMenu = pMenuBar->addMenu(tr("File"));
  m_fileMenu->addAction(m_loadAction);
  m_fileMenu->addAction(m_saveAction);
  m_fileMenu->addAction(m_saveDbAction);
  m_fileMenu->addAction(m_exitAction);

  m_separatorAction = m_fileMenu->addSeparator();

  for(int i = 0; i < MAXRECENTFILES; i++)
    m_fileMenu->addAction(m_recentFileActions[i]);

  m_viewMenu = pMenuBar->addMenu(tr("View"));
  m_viewMenu->addAction(m_nextAction);
  m_viewMenu->addAction(m_previousAction);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_slideshowAction);
  m_viewMenu->addAction(m_playDicomAction);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_fitSizeAction);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_showToolBarAction);
  m_viewMenu->addAction(m_showStatusBarAction);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_optionsAction);

  m_imageMenu = pMenuBar->addMenu(tr("Image"));
  m_imageMenu->addAction(m_segmentationAction);
  m_imageMenu->addSeparator();
  m_imageMenu->addAction(m_facedetectionAction);
  m_imageMenu->addSeparator();
  m_imageMenu->addAction(m_inpaintAction);
  m_imageMenu->addSeparator();

  m_imageMenu->addAction(m_harmonizationAction);
  m_imageMenu->addAction(m_sobelAction);
  m_imageMenu->addSeparator();
  m_imageMenu->addAction(m_restoreAction);
  m_imageMenu->addSeparator();
  m_imageMenu->addAction(m_enablePaintAction);
  m_imageMenu->addSeparator();
  m_featureMenu = m_imageMenu->addMenu(tr("Extract feature ..."));
  m_imageSearchMenu = m_imageMenu->addMenu(tr("Search by ..."));

  m_featureMenu->addAction(m_imageAllFeatureAction);

  m_imageSearchMenu->addAction(m_imageSeachByColorAction);
  m_imageSearchMenu->addAction(m_imageSeachByEdgeAction);

  m_helpMenu = pMenuBar->addMenu(tr("Help"));
  m_helpMenu->addAction(m_aboutAction);
  m_helpMenu->addAction(m_aboutQtAction);

  return;
} // MainWindow::createMenus()


void MainWindow::createToolBars(void)
{
  m_fileToolBar = addToolBar(tr("File"));
  m_fileToolBar->addAction(m_loadAction);
  m_fileToolBar->addAction(m_saveAction);

  m_viewToolBar = addToolBar(tr("View"));
  m_viewToolBar->addAction(m_previousAction);
  m_viewToolBar->addAction(m_nextAction);
  m_viewToolBar->addAction(m_slideshowAction);
  m_viewToolBar->addAction(m_playDicomAction);

  m_imageToolBar = addToolBar(tr("Image"));
  m_imageToolBar->addAction(m_restoreAction);

  return;
} // MainWindow::createToolBars()


void MainWindow::setup(void)
{
  connect(m_showStatusBarAction, SIGNAL(toggled(bool)), m_statusBar, SLOT(setVisible(bool)));
  m_showStatusBarAction->setChecked(true);

  connect(m_showToolBarAction, SIGNAL(toggled(bool)), m_fileToolBar,  SLOT(setVisible(bool)));
  connect(m_showToolBarAction, SIGNAL(toggled(bool)), m_viewToolBar,  SLOT(setVisible(bool)));
  connect(m_showToolBarAction, SIGNAL(toggled(bool)), m_imageToolBar, SLOT(setVisible(bool)));
  m_showToolBarAction->setChecked(true);

  return;
} // MainWindow::setup()


void MainWindow::scanFolder(const QString& path)
{
  int     nFiles;
  int     i = 0;
  IM_TYPE fmt;
  Status  res;

  QDir folder(path);

  m_recentFolder = path;

  QStringList filesInCurrentFolder = folder.entryList(QDir::Files);
  nFiles = filesInCurrentFolder.count();

  DestroyImageDescriptorDB(m_descriptordb);
  m_descriptordb.Init();

  ProgressShow(0, nFiles, "Scanning folder...");

  foreach(QString file, filesInCurrentFolder)
  {
    CStdFileInput       in;
    BaseStream::TStatus status;

    QString fullName = path + file;

    status = in.Open(fullName.toAscii());
    if(BaseStream::StatusOk != status)
    {
      continue;
    }

    fmt = m_imageDetector.ImageFormat(in);

    switch (fmt)
    {
      case IT_BMP:
      case IT_PNM:
      case IT_PNG:
      case IT_DDS:
      case IT_JPEG:
      case IT_JPEG2000:
      case IT_TIFF:
      case IT_JPEGXR:
      case IT_DICOM:
        {
          ImageDescriptor* descriptor = new ImageDescriptor;
          if(0 == descriptor)
            return;

          FileNameFeature* namefea = new FileNameFeature;
          if(0 == namefea)
            return;

          res = descriptor->Init();
          if(Ok != res)
            return;

          res = namefea->Init();
          if(Ok != res)
            return;

          res = namefea->Set(fullName.toAscii());
          if(Ok != res)
            return;

          res = descriptor->Insert(*namefea);
          if(Ok != res)
            return;

          res = m_descriptordb.Insert(*descriptor);
          if(Ok != res)
            return;
        }
        break;

      default:
        break;
    }

    in.Close();
    ProgressUpdate(i++);
    qApp->processEvents(QEventLoop::AllEvents);
  }

  ProgressHide();

  return;
} // MainWindow::scanFolder()


void MainWindow::OnFileOpen(void)
{
  bool   r = false;
  Status res;

  QStringList    currentFiles;
  FileOpenDialog fileOpenDlg(this);

  fileOpenDlg.Init(m_recentFolder);

  if(QDialog::Accepted == fileOpenDlg.exec())
    currentFiles = fileOpenDlg.selectedFiles();

  if(currentFiles.isEmpty())
    return;

  fileOpenDlg.m_image.Free();
  fileOpenDlg.m_frame.Free();
  DestroyImageDescriptorDB(m_descriptordb);

  res = m_descriptordb.Init();
  if(Ok != res)
    return;

  QString fileName = currentFiles.last();
  QFileInfo fileInfo(fileName);
  QString path = QDir::toNativeSeparators(fileInfo.path()) + QDir::separator();

  scanFolder(path);

  if(!m_pixmapLabels.isEmpty())
  {
    // delete thumbnails if any
    foreach(ImageWidget* label, m_pixmapLabels)
    {
      m_mainLayout->removeWidget(label);
      delete label;
    }

    m_pixmapLabels.clear();

    m_mainLayout->removeItem(m_gridBottomSpacer);
  }

  if(1 == currentFiles.count())
  {
    // single file selected in FileOpenDialog
    if(fileOpenDlg.m_isRaw)
    {
      m_param_raw.width     = fileOpenDlg.m_rawWidth;
      m_param_raw.height    = fileOpenDlg.m_rawHeight;
      m_param_raw.nchannels = fileOpenDlg.m_rawNChannels;
      m_param_raw.precision = fileOpenDlg.m_rawPrecision;
      m_param_raw.offset    = fileOpenDlg.m_rawOffset;
      // TODO: add sampling and color info
      m_currentFileIndex = 0;
    }

    if(tr("isf") == fileInfo.suffix().toLower())
    {
      QFile file(fileName);
      file.open(QIODevice::ReadOnly);

      QDataStream in(&file);

      DestroyImageDescriptorDB(m_descriptordb);

      in >> m_descriptordb;

      prepareForThumbs();
      setCurrentFolder();
    }
    else
    {
      r = setCurrentFile(fileName);

      // index is zero based, but for convenience we display it as starting from 1
      res = m_descriptordb.Index(fileName.toAscii(),m_currentFileIndex);
      if(Ok != res)
        return;

      m_scrollArea->setWidgetResizable(false);
    }
  }
  else
  {
    // multiple files selected in FileOpenDialog
    DestroyImageDescriptorDB(m_descriptordb);

    foreach(QString file, currentFiles)
    {
      ImageDescriptor* descriptor = new ImageDescriptor;
      if(0 == descriptor)
        return;

      FileNameFeature* currName = new FileNameFeature;
      if(0 == currName)
        return;

      res = descriptor->Init();
      if(Ok != res)
        return;

      res = currName->Init();
      if(Ok != res)
        return;

      res = currName->Set(file.toAscii());
      if(Ok != res)
        return;

      res = descriptor->Insert(*currName);
      if(Ok != res)
        return;

      res = m_descriptordb.Insert(*descriptor);
      if(Ok != res)
        return;
    }

    prepareForThumbs();

    r = setCurrentFolder();
  }

  m_imageLabel->show();

  m_canSlideshow = !m_descriptordb.IsEmpty();

  updateMenus();

  return;
} // MainWindow::OnFileOpen()


void MainWindow::OnMRUFileOpen(void)
{
  QAction* action = qobject_cast<QAction*>(sender());

  if(action)
    setCurrentFile(action->data().toString());

  m_imageLabel->show();

  m_canSlideshow = !m_descriptordb.IsEmpty();

  updateMenus();

  return;
} // MainWindow::OnMRUFileOpen()


void MainWindow::OnFileSave(void)
{
  bool bres = true;
  QString currentFile;
  FileSaveDialog fileSaveDlg(this,Qt::Window,&m_image);

  fileSaveDlg.Init(m_recentFolder);

  if(QDialog::Accepted == fileSaveDlg.exec())
  {
    currentFile = fileSaveDlg.selectedFiles().last();
  }

  if (currentFile.isEmpty())
    return;

  switch (fileSaveDlg.filters().indexOf(fileSaveDlg.selectedFilter()))
  {
    case 0:
      if(!currentFile.endsWith(".bmp"))
        currentFile.append(tr(".bmp"));

      bres = SaveAsBMP(m_frame, currentFile);
      break;

    case 1:
      m_param_jpeg.nthreads         = m_optionsDlg->numThreads();
      m_param_jpeg.point_transform  = 0;
      m_param_jpeg.predictor        = 1;
      m_param_jpeg.color            = (IM_COLOR)fileSaveDlg.jpegColor();
      m_param_jpeg.sampling         = (IM_SAMPLING)fileSaveDlg.jpegSampling();
      m_param_jpeg.mode             = fileSaveDlg.jpegMode();
      m_param_jpeg.huffman_opt      = (m_image.Precision() > 11) ? 1 : fileSaveDlg.jpegHuffman();
      m_param_jpeg.quality          = fileSaveDlg.jpegQuality();
      m_param_jpeg.restart_interval = fileSaveDlg.jpegRestartInterval() ?
        (fileSaveDlg.jpegMode() == JPEG_LOSSLESS ? m_frame.Width() : ((m_frame.Width() + 7) << 3)) : 0;

      // assume we have 128 byte buffer in jpeg paams to keep comment
      m_param_jpeg.comment_size     = IPP_MIN(127,fileSaveDlg.imageCommentSize());

      if(m_param_jpeg.comment_size != 0)
      {
        fileSaveDlg.imageComment((char*)m_param_jpeg.comment, m_param_jpeg.comment_size);
        m_param_jpeg.comment[m_param_jpeg.comment_size] = '\0';
      }
      else
        m_param_jpeg.comment[0] = '\0';

      if(!(currentFile.endsWith(".jpg") || currentFile.endsWith(".jpeg")))
        currentFile.append(tr(".jpg"));

      bres = SaveAsJPEG(m_image, currentFile);
      break;

    case 2:
      m_param_jpeg2k.color   = JC_BGR;
      m_param_jpeg2k.mode    = fileSaveDlg.jpeg2000Mode();
      m_param_jpeg2k.quality = fileSaveDlg.jpeg2000Ratio();
      m_param_jpeg2k.useMCT  = fileSaveDlg.jpeg2000MCT();
      m_param_jpeg2k.wt      = fileSaveDlg.jpeg2000wt();

      if(!currentFile.endsWith(".jp2"))
        currentFile.append(tr(".jp2"));
      bres = SaveAsJPEG2000(m_image, currentFile);
      break;

    case 3:
      m_param_png.filters[0] = fileSaveDlg.pngFilterSub();
      m_param_png.filters[1] = fileSaveDlg.pngFilterUp();
      m_param_png.filters[2] = fileSaveDlg.pngFilterAvg();
      m_param_png.filters[3] = fileSaveDlg.pngFilterPaeth();

      if(!currentFile.endsWith(".png"))
        currentFile.append(tr(".png"));
      bres = SaveAsPNG(m_image, currentFile);
      break;

    case 4:
      m_param_jpegxr.sampling         = fileSaveDlg.jpegxrSampling();
      m_param_jpegxr.trim             = fileSaveDlg.jpegxrFBTrim();
      m_param_jpegxr.aquality         = fileSaveDlg.jpegxrAQuality();
      m_param_jpegxr.bitstream        = fileSaveDlg.jpegxrFrequency();
      m_param_jpegxr.bands            = fileSaveDlg.jpegxrBands();
      m_param_jpegxr.alpha_mode       = fileSaveDlg.jpegxrAlpha();
      m_param_jpegxr.quality          = fileSaveDlg.jpegxrQuality();
      m_param_jpegxr.overlap          = fileSaveDlg.jpegxrOverlap();
      m_param_jpegxr.cmyk_direct      = 0;
      m_param_jpegxr.bits_shift       = -1;
      if(fileSaveDlg.jpegxrTilingMode())
      {
        m_param_jpegxr.tiles_uniform[0] = 1;
        m_param_jpegxr.tiles_uniform[1] = 1;
        m_param_jpegxr.tiles_uniform[2] = fileSaveDlg.jpegxrTilesX();
        m_param_jpegxr.tiles_uniform[3] = fileSaveDlg.jpegxrTilesY();
      }
      else
      {
        m_param_jpegxr.tiles_uniform[0] = fileSaveDlg.jpegxrTilesX();
        m_param_jpegxr.tiles_uniform[1] = fileSaveDlg.jpegxrTilesY();
        m_param_jpegxr.tiles_uniform[2] = 0;
        m_param_jpegxr.tiles_uniform[3] = 0;
      }

      if(!(currentFile.endsWith(".jxr") || currentFile.endsWith(".wdp") || currentFile.endsWith(".hdp")))
        currentFile.append(tr(".wdp"));
      bres = SaveAsJPEGXR(m_image, currentFile);
      break;

    default:
      return;
  }

  scanFolder(fileSaveDlg.directory().path() + QDir::separator());

  if(bres)
    setCurrentFile(currentFile, false);

  updateStatusBar(
    m_imageFormat,
    m_image.Width(),
    m_image.Height(),
    m_image.NChannels(),
    m_image.Precision(),
    m_imageSampling,
    m_imageColor,
    m_osd.cpe_iio,
    m_osd.sec_iio);

  updateMenus();

  return;
} // MainWindow::OnFileSave()


bool MainWindow::OnImageDbSave(void)
{
  QString name = QFileDialog::getSaveFileName(
                   this,tr("Save Image Search DB file"),
                   m_recentFolder,tr("Image Search DB files (*.isf)"));

  if(!name.isEmpty())
  {
    QFile file(name);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_descriptordb;
  }

  return false;
} // MainWindow::OnImageDbSave()


bool MainWindow::OnNext(void)
{
  int nFiles = m_descriptordb.Size();

  if(m_currentFileIndex < nFiles)
  {
    m_currentFileIndex++;

    const FileNameFeature& currName =
      reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(m_currentFileIndex).Feature(ImageFileName));

    if(currName.IsEmpty())
      return true;

    QString fileName = currName.Get();

    return setCurrentFile(fileName);
  }

  return false;
} // MainWindow::OnNext()


bool MainWindow::OnPrevious(void)
{
  if (m_currentFileIndex >= 0)
  {
    m_currentFileIndex--;

    const FileNameFeature& currName =
      reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(m_currentFileIndex).Feature(ImageFileName));

    if(currName.IsEmpty())
      return true;

    QString fileName = currName.Get();

    return setCurrentFile(fileName);
  }

  return false;
} // MainWindow::OnPrevious()


void MainWindow::stopSlideShow(void)
{
    m_slideshowAction->setText(tr("&Start slideshow"));
    m_timer2->stop();
    m_playDicomAction->setEnabled(m_canPlayDicom);

//    if(m_effectEnded)
    {
      m_hasActiveImage = true;

      m_saveAction->setEnabled(true);

      for(int i = 0; i < m_recentFiles.count(); i++)
        m_recentFileActions[i]->setEnabled(true);


      updateMenus();
    }

    m_loadAction->setEnabled(true);
    m_saveDbAction->setEnabled(true);

}
bool MainWindow::slideshow(void)
{
  if(!m_timer2->isActive() && m_effectEnded)
  {
    int value = m_optionsDlg->slideshowValue() * 1000;
    m_slideshowAction->setText(tr("&Stop slideshow"));
    m_timer2->setInterval(value);
    m_timer2->start();

    m_playDicomAction->setEnabled(false);
    m_enablePaintAction->setChecked(false);
    m_nextAction->setEnabled(false);
    m_previousAction->setEnabled(false);

    m_saveAction->setEnabled(false);
    m_loadAction->setEnabled(false);
    m_saveDbAction->setEnabled(false);

    for(int i = 0; i < m_recentFiles.count(); i++)
      m_recentFileActions[i]->setEnabled(false);

    m_mask.Free();
  }
  else
  {
      stopSlideShow();
  }

  m_effectEnded = true;

  return false;
} // MainWindow::slideshow()


bool MainWindow::effectAlpha(bool updateStatus)
{
  bool bres;

  const FileNameFeature& currName =
    reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(m_currentFileIndex).Feature(ImageFileName));

  if(currName.IsEmpty())
    return true;

  QString fileName = currName.Get();

  bres = openFile(fileName);

  if(!bres)
  {
    IppStatus status;
    Ipp8u     setValue[4]  = { 255, 255, 255, 255 };

    createFrame(m_image, m_frame);

    m_effect_roi.width  = m_scrollArea->frameSize().width()  - m_mainLayout->margin()*2;
    m_effect_roi.height = m_scrollArea->frameSize().height() - m_mainLayout->margin()*2;

    IppiSize roi_prev = { IPP_MIN(m_effect_roi.width, m_work.Width()),  IPP_MIN(m_effect_roi.height, m_work.Height()) };
    IppiSize roi_next = { IPP_MIN(m_effect_roi.width, m_frame.Width()), IPP_MIN(m_effect_roi.height, m_frame.Height()) };

    int step_prev     = m_effect_roi.width * 4;
    int step_next     = m_effect_roi.width * 4;
    int step_curr     = m_effect_roi.width * 4;

    m_buffer_prev = (Ipp8u*)ippMalloc(m_effect_roi.width * 4 * m_effect_roi.height);
    if(NULL == m_buffer_prev)
    {
      return true;
    }

    m_buffer_next = (Ipp8u*)ippMalloc(m_effect_roi.width * 4 * m_effect_roi.height);
    if(NULL == m_buffer_next)
    {
      return true;
    }

    m_buffer_curr = (Ipp8u*)ippMalloc(m_effect_roi.width * 4 * m_effect_roi.height);
    if(NULL == m_buffer_curr)
    {
      return true;
    }

    status = ippiSet_8u_C4R(setValue, m_buffer_prev, step_prev, m_effect_roi);
    if(ippStsNoErr != status)
      return true;

    status = ippiSet_8u_C4R(setValue, m_buffer_next, step_next, m_effect_roi);
    if(ippStsNoErr != status)
      return true;

    status = ippiSet_8u_C4R(setValue, m_buffer_curr, step_curr, m_effect_roi);
    if(ippStsNoErr != status)
      return true;

    switch(m_work.NChannels())
    {
      case 1:
      {
        status = ippiCopy_8u_C1C4R(m_work.DataPtr(), m_work.Step(), m_buffer_prev+0, step_prev, roi_prev);
        if(ippStsNoErr != status)
          return true;

        status = ippiCopy_8u_C1C4R(m_work.DataPtr(), m_work.Step(), m_buffer_prev+1, step_prev, roi_prev);
        if(ippStsNoErr != status)
          return true;

        status = ippiCopy_8u_C1C4R(m_work.DataPtr(), m_work.Step(), m_buffer_prev+2, step_prev, roi_prev);
        if(ippStsNoErr != status)
          return true;

        break;
      }

      case 3:
      {
        status = ippiCopy_8u_C3AC4R(m_work.DataPtr(), m_work.Step(), m_buffer_prev, step_prev, roi_prev);
        if(ippStsNoErr != status)
          return true;
        break;
      }

      case 4:
      {
        status = ippiCopy_8u_C4R(m_work.DataPtr(), m_work.Step(), m_buffer_prev, step_prev, roi_prev);
        if(ippStsNoErr != status)
          return true;
        break;
      }

      default:
        return true;
    }

    switch(m_frame.NChannels())
    {
      case 1:
      {
        status = ippiCopy_8u_C1C4R(m_frame.DataPtr(), m_frame.Step(), m_buffer_next+0, step_next, roi_next);
        if(ippStsNoErr != status)
          return true;

        status = ippiCopy_8u_C1C4R(m_frame.DataPtr(), m_frame.Step(), m_buffer_next+1, step_next, roi_next);
        if(ippStsNoErr != status)
          return true;

        status = ippiCopy_8u_C1C4R(m_frame.DataPtr(), m_frame.Step(), m_buffer_next+2, step_next, roi_next);
        if(ippStsNoErr != status)
          return true;

        break;
      }

      case 3:
      {
        status = ippiCopy_8u_C3AC4R(m_frame.DataPtr(), m_frame.Step(), m_buffer_next, step_next, roi_next);
        if(ippStsNoErr != status)
          return true;
        break;
      }

      case 4:
      {
        status = ippiCopy_8u_C4R(m_frame.DataPtr(), m_frame.Step(), m_buffer_next, step_next, roi_next);
        if(ippStsNoErr != status)
          return true;
        break;
      }

      default:
        return true;
    }

    m_alpha_phase = 255;

    m_effect_timer = new QTimer(this);
    if(0 == m_effect_timer)
      return true;

    connect(m_effect_timer, SIGNAL(timeout()), this, SLOT(runEffectAlpha(void)));

    m_effect_timer->setSingleShot(false);
    m_effect_timer->setInterval(0);
    m_effect_timer->start();
  }
  else {
      stopSlideShow();
      m_effectEnded = true;
  }
  m_work.Free();

  if(updateStatus)
    updateStatusBar(
      m_imageFormat,
      m_image.Width(),
      m_image.Height(),
      m_image.NChannels(),
      m_image.Precision(),
      m_imageSampling,
      m_imageColor,
      m_osd.cpe_iio,
      m_osd.sec_iio);

  QString shownName;
  shownName = shortName(fileName);
  setWindowTitle(tr("%1 - %2").arg(shownName).arg(tr(APP_NAME)));

  return false;
} // MainWindow::effectAlpha()


bool MainWindow::runEffectAlpha(void)
{
  IppStatus status;

  status = ippiAlphaCompC_8u_C4R(
             m_buffer_prev, m_effect_roi.width * 4, m_alpha_phase,
             m_buffer_next, m_effect_roi.width * 4, 255 - m_alpha_phase,
             m_buffer_curr, m_effect_roi.width * 4,
             m_effect_roi, ippAlphaOver);

  if(status)
    return true;

  m_alpha_phase -= 10;

  if(m_alpha_phase < 1)
  {
    m_frame_pxm = CreateQPixmap(m_frame);

    m_effect_timer->stop();

    ippFree(m_buffer_prev);
    ippFree(m_buffer_next);
    ippFree(m_buffer_curr);

    if(0 != m_effect_timer)
      delete m_effect_timer;

    m_effectEnded = true;

    if(false == m_timer2->isActive())
    {
      m_hasActiveImage = true;
      updateMenus();
    }
  }
  else
  {
    m_frame_pxm = CreateQPixmap(m_buffer_curr, m_effect_roi, 4);
  }

  updateView();

  return false;
} // MainWindow::runEffectAlpha()


bool MainWindow::circle(void)
{
  if(m_effectEnded)
  {
    m_effectEnded = false;

    int nFiles = m_descriptordb.Size();

    if(++m_currentFileIndex >= nFiles)
      m_currentFileIndex = 0;

    m_work.Free();
    m_work.Alloc(m_frame.Size(), m_frame.NChannels(), m_frame.Precision());
    m_work.CopyFrom((Ipp8u*)m_frame, m_frame.Step(), m_frame.Size());

    switch(m_optionsDlg->slideshowEffectValue())
    {
      case 1:
        return effectAlpha();

      default:
        const FileNameFeature& currName =
          reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(m_currentFileIndex).Feature(ImageFileName));

        if(currName.IsEmpty())
          return true;

        QString fileName = currName.Get();
        return setCurrentFile(fileName);
    }
  }

  return false;
} // MainWindow::circle()


void MainWindow::OnAbout(void)
{
  AboutDlg aboutDlg(this);
  aboutDlg.Init();
  aboutDlg.exec();
  return;
} // MainWindow::OnAbout()


void MainWindow::updateMenus(void)
{
  int nFiles = m_descriptordb.Size();

  // index is zero based, but for convenience it is displayed starting from 1
  bool canNext = (m_currentFileIndex+1 >= nFiles) ? false : true;
  bool canPrev = (m_currentFileIndex   <= 0)      ? false : true;

  m_saveAction->setEnabled(m_hasActiveImage);
  m_nextAction->setEnabled(m_hasActiveImage && nFiles && canNext);
  m_previousAction->setEnabled(m_hasActiveImage && nFiles && canPrev);
  m_restoreAction->setEnabled(m_hasActiveImage);
  m_segmentationAction->setEnabled(m_hasActiveImage && m_do_segmentation);
  m_facedetectionAction->setEnabled(m_hasActiveImage);
  m_sobelAction->setEnabled(m_hasActiveImage);
  m_harmonizationAction->setEnabled(m_hasActiveImage && m_do_harmonize);
  m_enablePaintAction->setEnabled(m_hasActiveImage);
  m_inpaintAction->setEnabled(m_hasActiveImage && m_enablePaintAction->isChecked());

  m_slideshowAction->setEnabled(m_canSlideshow && m_hasActiveImage);
  m_playDicomAction->setEnabled(m_canPlayDicom && m_hasActiveImage);

  m_imageAllFeatureAction->setEnabled(m_canSearch);
  m_imageSeachByColorAction->setEnabled(m_canSearch);
  m_imageSeachByEdgeAction->setEnabled(m_canSearch);

  return;
} // MainWindow::updateMenus()


ImageWidget* MainWindow::createLabel(CIppImage& thumbnail, IppiSize originalSize, int Id)
{
  QPixmap pixmap = CreateQPixmap(thumbnail, thumbnail.Size(), thumbnail.NChannels());

  ImageWidget* label = new ImageWidget(this, Qt::Window);
  if(0 == label)
    return 0;

  label->m_thumbLabel = new QLabel(label);
  if(0 == label->m_thumbLabel)
    return 0;

  label->m_nameLabel = new QLabel(label);
  if(0 == label->m_nameLabel)
    return 0;

  label->m_ratingLabel = new QLabel(label);
  if(0 == label->m_ratingLabel)
    return 0;

  QVBoxLayout* labelLayout = new QVBoxLayout(label);
  if(0 == labelLayout)
    return 0;

  labelLayout->setMargin(0);
  labelLayout->setSpacing(2);
  labelLayout->setAlignment(Qt::AlignHCenter);

  label->setFixedSize(150, 150);
  label->setFrameStyle(QFrame::Box | QFrame::Sunken);
  label->setBackgroundRole(QPalette::Base);
  label->setScaledContents(false);
  label->m_thumbLabel->setAlignment(Qt::AlignCenter);
  label->m_thumbLabel->setScaledContents(false);
  label->m_thumbLabel->setFixedSize(146,120);
  label->m_thumbLabel->setPixmap(pixmap);
  label->m_thumbLabel->resize(146, 120);
  label->m_thumbLabel->updateGeometry();

  const FileNameFeature& currName =
    reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(Id).Feature(ImageFileName));

  if(!currName.IsEmpty())
  {
    QString fileName = currName.Get();
    QString showName = shortName(fileName);

    if(showName.length() > 15)
    {
      showName.truncate(15);
      showName += QString(3, '.');
    }

    label->m_nameLabel->setText(showName);
  }

  label->m_nameLabel->setAlignment(Qt::AlignCenter);
  label->m_ratingLabel->setAlignment(Qt::AlignCenter);

  labelLayout->addWidget(label->m_thumbLabel);
  labelLayout->addWidget(label->m_nameLabel);
  labelLayout->addWidget(label->m_ratingLabel);

  label->m_id         = Id;
  label->m_roi        = originalSize;
  label->m_nchannels  = thumbnail.NChannels();

  return label;
} // MainWindow::createLabel()


void MainWindow::prepareForThumbs(void)
{
  m_fitSizeAction->setEnabled(false);
  m_scrollArea->setWidgetResizable(true);
  m_enablePaintAction->setChecked(false);

  m_hasActiveThumbs = true;
  m_hasActiveImage  = false;
  m_canPlayDicom    = false;
  m_canSearch       = true;

  m_dicomStdFileIn.Close();
  m_docks.at(0)->hide();

  m_mask.Free();

  setWindowTitle(tr("%1 - %2").arg(m_recentFolder).arg(tr(APP_NAME)));

  cleanStatusBar();

  int vSliderWidth  = m_scrollArea->verticalScrollBar()->width();
  int ncols         = (width() - vSliderWidth) / 151;

  int nFiles        = m_descriptordb.Size();
  int resizeHeight  = ((nFiles - ncols - 1) / ncols) * 151;
  int resizeWidth   = ncols * 151;

  for(int i = 0; i < m_descriptordb.Size(); i++)
  {
    foreach(ImageWidget* imageWidget, m_pixmapLabels)
    {
      if(i == imageWidget->m_id)
      {
        imageWidget->m_checked = 0;
        m_imagesSelected.removeAll(imageWidget->m_id);
        imageWidget->setFrameStyle(QFrame::Box | QFrame::Sunken);
        imageWidget->m_ratingLabel->clear();
        m_pixmapLabels.removeAll(imageWidget);
        m_pixmapLabels.append(imageWidget);
        break;
      }
    }
  }

  m_scrollArea->resize(resizeWidth, resizeHeight);
  m_scrollArea->updateGeometry();
  m_imageLabel->clear();
  m_mainLayout->removeWidget(m_imageLabel);

  return;
} // MainWindow::prepareForThumbs()


bool MainWindow::setCurrentFolder(void)
{
  int Id;
  int nrow;
  int ncol;
  int vSliderWidth;
  int ncolsToShow;
  int nrowsToShow;
  int bottomSpacerHeight;
  bool bres;

  vSliderWidth = m_scrollArea->verticalScrollBar()->width();

  ncolsToShow = (width() - vSliderWidth) / 151;
  nrowsToShow = height() / 151;
  m_mainLayout->setColumnStretch(ncolsToShow,100);

  Id = 0;

  if(0 == m_pixmapLabels.isEmpty())
  {
    foreach(ImageWidget* label, m_pixmapLabels)
    {
      m_mainLayout->removeWidget(label);
      delete label;
    }

    m_pixmapLabels.clear();
    m_mainLayout->removeItem(m_gridBottomSpacer);
  }

  int nFiles   = m_descriptordb.Size();
  int shownNum = IPP_MIN(ncolsToShow*nrowsToShow,nFiles);

  nrow = (shownNum + (ncolsToShow - 1)) / ncolsToShow;

  for(int nr = 0; nr < nrow; nr++)
  {
    ncol = (((nr + 1)*ncolsToShow) < shownNum) ? ncolsToShow : (shownNum - nr*ncolsToShow);
    for(int nc = 0; nc < ncol; nc++)
    {
      const FileNameFeature& currName =
        reinterpret_cast<const FileNameFeature&>(m_descriptordb.Descriptor(Id).Feature(ImageFileName));

      if(currName.IsEmpty())
        continue;

      QString m_currentFile = currName.Get();

      bres = openFile(m_currentFile);

      createFrame(m_image, m_frame);

      float xFactor = 146.0 / m_frame.Width();
      float yFactor = 110.0 / m_frame.Height();

      (xFactor < yFactor) ? yFactor = xFactor : xFactor = yFactor;

      if(xFactor > 1 || yFactor > 1)
        xFactor = yFactor = 1;

      IppiSize thumbnailRoi;
      thumbnailRoi.width  = m_frame.Width()  * xFactor;
      thumbnailRoi.height = m_frame.Height() * yFactor;

      // very thin images correction
      if(thumbnailRoi.width == 0)
        thumbnailRoi.width = 1;
      if(thumbnailRoi.height == 0)
        thumbnailRoi.height = 1;

      CIppImage thumbnail;
      if(0 != thumbnail.Alloc(thumbnailRoi, m_frame.NChannels(), m_frame.Precision()))
        return true;

      if(xFactor != 1 && yFactor != 1)
      {
        if(0 != m_frame.Resize(thumbnail, thumbnail.Step(), thumbnail.Size()))
          continue;
      }
      else
      { // if image smaller than preview window
        if(0 != m_frame.CopyTo((Ipp8u*)thumbnail, thumbnail.Step(), thumbnail.Size()))
          continue;
      }

      ImageWidget* label = createLabel(thumbnail, m_image.Size(), Id);

      m_pixmapLabels.append(label);

      m_mainLayout->addWidget(m_pixmapLabels.last(), nr, nc, Qt::AlignLeft | Qt::AlignTop);

      Id++;

      thumbnail.Free();
      m_frame.Free();

      qApp->processEvents();
    }
  }

  nFiles = m_descriptordb.Size();

  int nRows = ((nFiles + (ncolsToShow - 1)) / ncolsToShow);
  bottomSpacerHeight = (IPP_MAX(nRows, nrowsToShow) - nrow)*151;

  m_gridBottomSpacer->changeSize(0, bottomSpacerHeight);
  m_mainLayout->removeItem(m_gridBottomSpacer);
  m_mainLayout->addItem(m_gridBottomSpacer, nrow, 0);

  return true;
} // MainWindow::setCurrentFolder()


bool MainWindow::setCurrentFile(const QString& fileName, bool updateStatus)
{
  bool bres;

  m_enablePaintAction->setChecked(false);

  m_mask.Free();

  if(fileName.isEmpty())
    return true;

  bres = openFile(fileName);
  if(false != bres)
  {
    // do not exit here, let's show even partially decoded data
//    return true;
  }

  int cc = m_mainLayout->columnCount();
  m_mainLayout->setColumnStretch( cc - 1,0);
  m_scrollArea->setWidgetResizable(false);

  if(m_image.Precision() > 8)
  {
    m_do_segmentation = false;
    m_do_harmonize    = false;
  }
  else
  {
    m_do_segmentation = true;
    m_do_harmonize    = true;
  }
  if(m_image.NChannels() > 3)
      m_do_segmentation = false;
  createFrame(m_image, m_frame);

  m_currentFile = fileName;

  m_recentFiles.removeAll(m_currentFile);
  m_recentFiles.prepend(m_currentFile);

  if(MAXRECENTFILES < m_recentFiles.count())
    m_recentFiles.removeLast();

  updateRecentFileActions();

  m_descriptordb.Index(fileName.toAscii(),m_currentFileIndex);

  if(updateStatus)
    updateStatusBar(
      m_imageFormat,m_image.Width(),m_image.Height(),m_image.NChannels(),m_image.Precision(),m_imageSampling,m_imageColor,
      m_osd.cpe_iio,m_osd.sec_iio);

  m_frame_pxm = CreateQPixmap(m_frame);
  updateView(true, &m_osd);

  m_hasActiveImage  = true;
  m_effectEnded     = true;

  QString shownName;
  shownName = shortName(m_currentFile);
  setWindowTitle(tr("%1 - %2").arg(shownName).arg(tr(APP_NAME)));

  if(false == m_timer2->isActive())
    updateMenus();

  return false;
} // MainWindow::setCurrentFile()


bool MainWindow::openFile(const QString& fileName)
{
  bool          bres;
  IM_TYPE       type;
  CStdFileInput in;

  UIC::BaseStream::TStatus status;

  m_hasActiveImage = false;
  m_canPlayDicom   = false;

  m_dicomStdFileIn.Close();
  m_docks.at(0)->hide();

  status = in.Open(fileName.toAscii());
  if(BaseStream::StatusOk != status)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading file: "),QMessageBox::Ok);
    return true;
  }

  type = m_imageDetector.ImageFormat(in);

  switch(type)
  {
    case IT_UNKNOWN:     bres = GetImageFromRAW(in, m_image);      break;
    case IT_BMP:         bres = GetImageFromBMP(in, m_image);      break;
    case IT_JPEG:        bres = GetImageFromJPEG(in, m_image);     break;
    case IT_JPEG2000:    bres = GetImageFromJPEG2000(in, m_image); break;
    case IT_JPEGXR:      bres = GetImageFromJPEGXR(in, m_image);   break;

    case IT_DICOM:
      in.Close();
      m_dicomStdFileIn.Open(fileName.toAscii());
      bres = GetImageFromDICOM(m_dicomStdFileIn, m_image);
      break;

    case IT_PNG:         bres = GetImageFromPNG(in, m_image);      break;

    default:
      bres = true;
      break;
  }

  in.Close();

  return bres;
} // MainWindow::openFile()


QSize MainWindow::fitImageSize(void)
{
  float xFactor = (float)(m_scrollArea->frameSize().width()  - m_mainLayout->margin()*2)/(float)m_frame_pxm.width();
  float yFactor = (float)(m_scrollArea->frameSize().height() - m_mainLayout->margin()*2)/(float)m_frame_pxm.height();

  (xFactor < yFactor) ? yFactor = xFactor : xFactor = yFactor;

  if(xFactor > 1.0 || yFactor > 1.0 || !m_fitSizeAction->isChecked())
    xFactor = yFactor = 1.0;

  QSize roi(m_frame_pxm.width() * xFactor, m_frame_pxm.height() * yFactor);

  m_centralWidget->resize(roi);
  m_centralWidget->updateGeometry();

  return roi;
} // MainWindow::fitImageSize()


void MainWindow::updateView(
  bool      drawOSD,
  OSD_VALS* osd)
{
  if(drawOSD)
  {
    int   size = m_optionsDlg->osdSize();
    int   y    = 5;

    foreach(QLabel* label, m_osd_text)
    {
      label->setFont(QFont("Arial", size));
      label->clear();
    }

    if(m_optionsDlg->osdValue(0))
    {
      QString str(tr(": %1").arg(m_optionsDlg->numThreads()));
      m_osd_text.at(0)->setGeometry(15, y, 175, 20);
      m_osd_text.at(0)->setText(m_optionsDlg->osdText(0) + str);

      y += 20;
    }

    if(m_optionsDlg->osdValue(1))
    {
      QString str(tr(": %1").arg((float)osd->sec_iio,0,'f',1));
      m_osd_text.at(1)->setGeometry(15, y, 175, 20);
      m_osd_text.at(1)->setText(m_optionsDlg->osdText(1) + str);

      y += 20;
    }

    if(m_optionsDlg->osdValue(2))
    {
      QString str(tr(": %1").arg((float)osd->sec_prc,0,'f',1));
      m_osd_text.at(2)->setGeometry(15, y, 175, 20);
      m_osd_text.at(2)->setText(m_optionsDlg->osdText(2) + str);

      y += 20;
    }

    if(m_optionsDlg->osdValue(3))
    {
      QString str(tr(": %1").arg((float)osd->sec_rnd,0,'f',1));
      m_osd_text.at(3)->setGeometry(15, y, 175, 20);
      m_osd_text.at(3)->setText(m_optionsDlg->osdText(3) + str);

      y += 20;
    }
  }

  m_imageLabel->setPixmap(m_frame_pxm);
  m_mainLayout->addWidget(m_imageLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);

  fitImageSize();

  return;
} // MainWindow::updateView()


void MainWindow::updateRecentFileActions(void)
{
  QMutableStringListIterator it(m_recentFiles);

  while(it.hasNext())
  {
    if(!QFile::exists(it.next()))
      it.remove();
  }

  for(int j = 0; j < MAXRECENTFILES; ++j)
  {
    if(j < m_recentFiles.count())
    {
      QString text = QString(tr("&%1 %2").arg(j + 1).arg(m_recentFiles[j]));

      m_recentFileActions[j]->setText(text);
      m_recentFileActions[j]->setData(m_recentFiles[j]);
      m_recentFileActions[j]->setVisible(true);
    }
    else
      m_recentFileActions[j]->setVisible(false);
  }

  m_separatorAction->setVisible(!m_recentFiles.isEmpty());

  return;
} // MainWindow::updateRecentFileActions()


void MainWindow::options(void)
{
//  m_optionsDlg->setAttribute(Qt::WA_DeleteOnClose);
  m_optionsDlg->show();

  return;
} // MainWindow::options()


void MainWindow::setOptions(void)
{
  if(m_optionsDlg->maxFaceSizeValue() < m_optionsDlg->minFaceSizeValue())
    m_optionsDlg->setMaxFaceSizeValue(0);

  {
  HRMFLT_IPP_CTX* ctx = (HRMFLT_IPP_CTX*)m_param_harmonization.ctx;
  if(ctx)
  {
    if(CTX_IPP == ctx->id)
    {
      if(0 != ctx->p32f)
        ippiFree(ctx->p32f);

      if(0 != ctx->p32fb)
        ippiFree(ctx->p32fb);

      delete ctx;
      m_param_harmonization.ctx = 0;
    }
  }
  }

  {
  SBLFLT_IPP_CTX* ctx = (SBLFLT_IPP_CTX*)m_param_sobel.ctx;
  if(ctx)
  {
    if(CTX_IPP == ctx->id)
    {
      if(0 != ctx->p.Data8u)
        ippiFree(ctx->p.Data8u);

      if(0 != ctx->pBuf)
        ippFree(ctx->pBuf);

      if(0 != ctx->p16shor)
        ippiFree(ctx->p16shor);

      if(0 != ctx->p16sver)
        ippiFree(ctx->p16sver);

      delete ctx;
      m_param_sobel.ctx = 0;
    }
  }
  }

  return;
} // MainWindow::setOptions()


void MainWindow::OnSegmentation(void)
{
  m_do_segmentation = !m_do_segmentation;

  m_do_inpainting    = false;
  m_do_facedetection = false;
  m_do_harmonize     = false;
  m_do_sobel         = false;

  m_timer.Start();
  segmentation();
  m_timer.Stop();

  m_osd.cpe_prc = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_prc = (float)m_timer.GetTime();

  m_frame_pxm = CreateQPixmap(m_frame);

  updateView(true, &m_osd);

  updateStatusBar(
    m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
    m_image.Precision(), m_imageSampling, m_imageColor,
    m_osd.cpe_prc, m_osd.sec_prc);

  return;
} // MainWindow::OnSegmentation()


void MainWindow::OnInpainting(void)
{
  m_do_inpainting = !m_do_inpainting;

  m_do_segmentation  = false;
  m_do_facedetection = false;
  m_do_harmonize     = false;
  m_do_sobel         = false;

  m_timer.Start();
  inpainting();
  m_timer.Stop();

  m_osd.cpe_prc = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_prc = (float)m_timer.GetTime();

  m_frame_pxm = CreateQPixmap(m_frame);

  updateView(true, &m_osd);

  updateStatusBar(
    m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
    m_image.Precision(), m_imageSampling, m_imageColor,
    m_osd.cpe_prc, m_osd.sec_prc);

  return;
} // MainWindow::OnInpainting()


void MainWindow::OnFaceDetection(void)
{
  m_do_facedetection = !m_do_facedetection;

  m_do_segmentation = false;
  m_do_inpainting   = false;
  m_do_harmonize    = false;
  m_do_sobel        = false;

  if(m_frame.NChannels() != m_image.NChannels())
  {
    m_frame.Free();
    int r = m_frame.Alloc(m_image.Size(), m_image.NChannels(), 8, 0);
    if(0 != r)
      return;
  }

  m_timer.Start();
  facedetection();
  m_timer.Stop();

  m_osd.cpe_prc = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_prc = (float)m_timer.GetTime();

  m_frame_pxm = CreateQPixmap(m_frame);

  updateView(true, &m_osd);

  updateStatusBar(
    m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
    m_image.Precision(), m_imageSampling, m_imageColor,
    m_osd.cpe_prc, m_osd.sec_prc);

  return;
} // MainWindow::OnFaceDetection()


void MainWindow::OnHarmonization(void)
{
  m_do_harmonize = !m_do_harmonize;

  m_do_segmentation  = false;
  m_do_inpainting    = false;
  m_do_facedetection = false;
  m_do_sobel         = false;

  if(m_frame.NChannels() != m_image.NChannels())
  {
    m_frame.Free();
    int r = m_frame.Alloc(m_image.Size(), m_image.NChannels(), 8, 0);
    if(0 != r)
      return;
  }

  m_timer.Start();
  harmonization();
  m_timer.Stop();

  m_osd.cpe_prc = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_prc = (float)m_timer.GetTime(CTimer::msec);

  m_timer.Start();
  m_frame_pxm = CreateQPixmap(m_frame);
  m_timer.Stop();

  m_osd.cpe_rnd = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_rnd = (float)m_timer.GetTime(CTimer::msec);

  updateView(true, &m_osd);

  updateStatusBar(
    m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
    m_image.Precision(), m_imageSampling, m_imageColor,
    m_osd.cpe_iio, m_osd.sec_iio);

  return;
} // MainWindow::OnHarmonization()


void MainWindow::OnSobel(void)
{
  m_do_sobel = !m_do_sobel;

  m_do_segmentation  = false;
  m_do_inpainting    = false;
  m_do_facedetection = false;
  m_do_harmonize     = false;

  m_timer.Start();
  sobel();
  m_timer.Stop();

  m_osd.cpe_prc = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_prc = (float)m_timer.GetTime();

  m_timer.Start();
  m_frame_pxm = CreateQPixmap(m_frame);
  m_timer.Stop();

  m_osd.cpe_rnd = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
  m_osd.sec_rnd = (float)m_timer.GetTime();

  updateView(true, &m_osd);

  updateStatusBar(
    m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
    m_image.Precision(), m_imageSampling, m_imageColor,
    m_osd.cpe_iio, m_osd.sec_iio);

  return;
} // MainWindow::OnSobel()


static int widthStatic  = -1;
static int heightStatic = -1;
static CIppImage* pDstStatic = 0;

void MainWindow::segmentation(void)
{
  CIppImage* pDst;

  m_param_segmentation.nthreads    = m_optionsDlg->numThreads();
  m_param_segmentation.cap         = m_optionsDlg->capValue();
  m_param_segmentation.morph       = m_optionsDlg->morphValue();
  m_param_segmentation.normOptions = m_optionsDlg->normOptionsValue();
  m_param_segmentation.bounind     = m_optionsDlg->bounindValue();
  m_param_segmentation.rep         = 1;
  m_param_segmentation.measure     = false;
  m_param_segmentation.erode       = m_optionsDlg->erodeValue();
  m_param_segmentation.distance    = m_optionsDlg->distanceValue();
  m_param_segmentation.gradient    = m_optionsDlg->gradientValue();

  if(m_image.Precision() <= 8)
  {
    pDst = &m_frame;
  }
  else if(m_image.Width() != widthStatic || m_image.Height() != heightStatic || pDstStatic == 0)
  {
    if (pDstStatic)
      delete pDstStatic;

    pDst = new CIppImage;

    pDstStatic = pDst;

    pDst->Alloc(m_image.Size(),m_image.NChannels(),m_image.Precision());

    widthStatic  = m_image.Width();
    heightStatic = m_image.Height();
  }
  else
  {
    pDst = pDstStatic;
  }

  // TODO: precision > 8

  segmentation_filter(m_image,m_param_segmentation,*pDst);

  if(m_image.Precision() > 8)
    pDst->ReduceBits16(m_frame.DataPtr(),m_frame.Step(),m_frame.Size());

  return;
} // MainWindow::segmentation()


void MainWindow::inpainting(void)
{
  CIppImage* pDst;

  m_param_inpainting.nthreads = m_optionsDlg->numThreads();
  m_param_inpainting.radius   = m_optionsDlg->inpaintRadiusValue();
  m_param_inpainting.mode     = m_optionsDlg->inpaintModeValue();
  m_param_inpainting.alg      = m_optionsDlg->inpaintAlgValue();

  if(m_image.Precision() <= 8)
  {
    pDst = &m_frame;
  }
  else if(m_image.Width() != widthStatic || m_image.Height() != heightStatic || pDstStatic == 0)
  {
    if (pDstStatic)
      delete pDstStatic;

    pDst = new CIppImage;
    if(0 == pDst)
      return;

    pDstStatic = pDst;

    pDst->Alloc(m_image.Size(),m_image.NChannels(),m_image.Precision());

    widthStatic  = m_image.Width();
    heightStatic = m_image.Height();
  }
  else
  {
    pDst = pDstStatic;
  }

  // TODO: precision > 8

  inpainting_filter(m_image,m_param_inpainting,*pDst,m_mask);

  if(m_image.Precision() > 8)
    pDst->ReduceBits16(m_frame.DataPtr(),m_frame.Step(),m_frame.Size());

  return;
} // MainWindow::inpainting()


void MainWindow::facedetection(void)
{
  CIppImage* pDst;

  m_param_facedetection.nthreads = m_optionsDlg->numThreads();
  m_param_facedetection.minfacew = m_optionsDlg->minFaceSizeValue();
  m_param_facedetection.maxfacew = m_optionsDlg->maxFaceSizeValue();
  m_param_facedetection.sfactor  = m_optionsDlg->scaleFactorValue();
  m_param_facedetection.pruning  = (pruningType)m_optionsDlg->pruningTypeValue();

  if(m_image.Precision() <= 8)
  {
    pDst = &m_frame;
  }
  else if(m_image.Width() != widthStatic || m_image.Height() != heightStatic || pDstStatic == 0)
  {
    int r;

    if (pDstStatic)
      delete pDstStatic;

    pDst = new CIppImage;
    if(0 == pDst)
      return;

    pDstStatic = pDst;

    r = pDst->Alloc(m_image.Size(),m_image.NChannels(),m_image.Precision());
    if(0 != r)
      return;

    widthStatic  = m_image.Width();
    heightStatic = m_image.Height();
  }
  else
  {
    pDst = pDstStatic;
  }

  // TODO: precision > 8

  facedetection_filter(m_image,m_param_facedetection,*pDst);

  if(m_image.Precision() > 8)
    pDst->ReduceBits16(m_frame.DataPtr(),m_frame.Step(),m_frame.Size());

  return;
} // MainWindow::facedetection()


void MainWindow::harmonization(void)
{
  bool       paramChange = false;
  CIppImage* pDst;

  if(m_param_harmonization.nthreads != m_optionsDlg->numThreads())
    paramChange = true;

  if(m_param_harmonization.filter_kernel != m_optionsDlg->harmonizeKernel())
    paramChange = true;

  m_param_harmonization.nthreads      = m_optionsDlg->numThreads();
  m_param_harmonization.c1            = m_optionsDlg->c1HarmonizeValue();
  m_param_harmonization.c2            = m_optionsDlg->c2HarmonizeValue();
  m_param_harmonization.v1            = m_optionsDlg->v1HarmonizeValue();
  m_param_harmonization.v2            = m_optionsDlg->v2HarmonizeValue();
  m_param_harmonization.v3            = m_optionsDlg->v3HarmonizeValue();
  m_param_harmonization.filter_kernel = m_optionsDlg->harmonizeKernel();

  if(m_image.Precision() <= 8)
  {
    pDst = &m_frame;
  }
  else if(m_image.Width() != widthStatic || m_image.Height() != heightStatic || pDstStatic == 0)
  {
    int r;

    if (pDstStatic)
      delete pDstStatic;

    pDst = new CIppImage;
    if(0 == pDst)
      return;

    pDstStatic = pDst;

    r = pDst->Alloc(m_image.Size(),m_image.NChannels(),m_image.Precision());
    if(0 != r)
      return;

    widthStatic  = m_image.Width();
    heightStatic = m_image.Height();
  }
  else
  {
    pDst = pDstStatic;
  }

  harmonization_filter_ipp(m_image,m_param_harmonization,*pDst);

  if(m_image.Precision() > 8)
  {
    pDst->ReduceBits16(m_frame.DataPtr(),m_frame.Step(),m_frame.Size());
  }

  return;
} // MainWindow::harmonization()


void MainWindow::sobel(void)
{
  int r;
  CIppImage* pDst;

  m_param_sobel.nthreads      = m_optionsDlg->numThreads();
  m_param_sobel.filter_kernel = m_optionsDlg->sobelKernel();

  if(m_frame.NChannels() != 1)
  {
    m_frame.Free();
    r = m_frame.Alloc(m_image.Size(), 1, 8, 0);
    if(0 != r)
      return;
  }

  if(m_image.Precision() <= 8)
  {
    pDst = &m_frame;
  }
  else if(m_image.Width() != widthStatic || m_image.Height() != heightStatic || pDstStatic == 0)
  {
    if (pDstStatic)
      delete pDstStatic;

    pDst = new CIppImage;
    if(0 == pDst)
      return;

    pDstStatic = pDst;

    r = pDst->Alloc(m_image.Size(),m_image.NChannels(),m_image.Precision());
    if(0 != r)
      return;

    widthStatic  = m_image.Width();
    heightStatic = m_image.Height();
  }
  else
  {
    pDst = pDstStatic;
  }

  sobel_filter_ipp(m_image,m_param_sobel,*pDst);

  if(m_image.Precision() > 8)
  {
    pDst->ReduceBits16(m_frame.DataPtr(),m_frame.Step(),m_frame.Size());
  }

  return;
} // MainWindow::sobel()


void MainWindow::restore(void)
{
  createFrame(m_image, m_frame);

  m_frame_pxm = CreateQPixmap(m_frame);
  updateView(false, false);

  updateStatusBar(
    m_imageFormat,
    m_image.Width(),
    m_image.Height(),
    m_image.NChannels(),
    m_image.Precision(),
    m_imageSampling,
    m_imageColor,
    -2,
    -2);

  if(m_enablePaintAction->isChecked())
    ippiSet_8u_C1R(0, m_mask, m_mask.Step(), m_mask.Size());

  return;
} // MainWindow::restore()


void MainWindow::createFrame(CIppImage& image, CIppImage& frame)
{
  IM_COLOR   iColor     = image.Color();
  IM_FORMAT  iFormat    = image.Format();
  Ipp16u     iChannels  = image.NChannels();
  Ipp16u     iPrecision = image.Precision();
  CIppImage  convert;
  const int order3[] = {2,1,0};
  const int order4[] = {2,1,0,3};

  frame.Free();

  switch(iColor)
  {
  // Lets try to guess
  case IC_UNKNOWN:
    {
      if(iChannels == 1)
        frame.Color(IC_GRAY);
      else if(iChannels == 3)
        frame.Color(IC_RGB);
      else if(iChannels == 4)
        frame.Color(IC_RGBA);
      else
        return;
    }
  // As is output
  case IC_YCBCR:
  case IC_YCCK:
    {
      if(0 != frame.Alloc(image.Size(), iChannels, 8, 0))
        return;

      if(image.Precision() <= 8)
        image.CopyTo    ((Ipp8u*)frame, frame.Step(), image.Size());
      else if(image.Precision() <= 16)
        image.ReduceBits16((Ipp8u*)frame, frame.Step(), image.Size());
      else if(image.Precision() <= 32)
        image.ReduceBits32((Ipp8u*)frame, frame.Step(), image.Size());
      return;
    }
    break;

  // To colorspace convert
  case IC_GRAYA:
  case IC_CMYK:
  case IC_CMYKA:
  case IC_RGBAP:
  case IC_BGRAP:
  case IC_RGBE:
    break;

  // Normal colors
  default:
    {
      if(0 != frame.Alloc(image.Size(), iChannels, 8, 0))
        return;

      frame.Color(iColor);
      frame.Format(image.Format());

      if(image.Precision() <= 8)
        image.CopyTo((Ipp8u*)frame, frame.Step(), image.Size());
      else if(image.Precision() <= 16)
        image.ReduceBits16((Ipp8u*)frame, frame.Step(), image.Size());
      else if(image.Precision() <= 32)
        image.ReduceBits32((Ipp8u*)frame, frame.Step(), image.Size());

      if((frame.Color() == IC_RGB || frame.Color() == IC_BGR) && iChannels == 4)
        frame.FillAlpha_8u(0xff);

      if(frame.Color() == IC_BGRA || (frame.Color() == IC_BGR && image.NChannels() == 4))
        ippiSwapChannels_8u_C4IR(frame.DataPtr(), frame.Step(), frame.Size(), order4);
      else if(frame.Color() == IC_BGR)
        ippiSwapChannels_8u_C3IR(frame.DataPtr(), frame.Step(), frame.Size(), order3);

      if(frame.Color() == IC_BGRA)
        frame.Color(IC_RGBA);
      else if(frame.Color() == IC_BGR)
        frame.Color(IC_RGB);

      return;
    }
  }

  // Color convert section
  switch(iColor)
  {
  case IC_GRAYA:
    {
      iChannels = 4;
      iColor = IC_RGBA;

      if(0 != convert.Alloc(image.Size(), iChannels, iPrecision, 0))
        return;

      if(iPrecision <= 8)
        image.GrayAlphaToRGBA_8u(convert);
      else if(iPrecision <= 16)
        image.GrayAlphaToRGBA_16u(convert);
    }
    break;

  case IC_CMYK:
    {
      iChannels = 3;
      iColor = IC_RGB;

      if(0 != convert.Alloc(image.Size(), iChannels, iPrecision, 0))
        return;

      if(iPrecision <= 8)
        image.CMYKAToRGBA_8u(convert); // This function switching between CMYK/CMYKA automatically
      else if(iPrecision <= 16)
        image.CMYKAToRGBA_16u(convert);
    }
    break;

  case IC_CMYKA:
    {
      iChannels = 4;
      iColor = IC_RGBA;

      if(0 != convert.Alloc(image.Size(), iChannels, iPrecision, 0))
        return;

      if(iPrecision <= 8)
        image.CMYKAToRGBA_8u(convert);
      else if(iPrecision <= 16)
        image.CMYKAToRGBA_16u(convert);
    }
    break;

  case IC_RGBAP:
  case IC_BGRAP:
    {
      iChannels = 4;
      iColor = IC_RGBA;

      if(0 != convert.Alloc(image.Size(), iChannels, iPrecision, 0))
        return;

      if(iPrecision <= 8)
        image.RGBAPToRGBA_8u(convert);
      else if(iPrecision <= 16)
      {
        if(iFormat == IF_UNSIGNED)
          image.RGBAPToRGBA_16u(convert);
        else if(iFormat == IF_FIXED)
          image.RGBAPToRGBA_16s(convert);
      }
      else if(iPrecision <= 32)
      {
        if(iFormat == IF_FIXED)
          image.RGBAPToRGBA_32s(convert);
        else if(iFormat == IF_FLOAT)
          image.RGBAPToRGBA_32f(convert);
      }
    }
    break;

  case IC_RGBE:
    {
      iChannels = 3;
      iColor = IC_RGB;
      iFormat = IF_FLOAT;

      if(0 != convert.Alloc(image.Size(), iChannels, 32, 0))
        return;

      image.RGBEToRGB(convert);
    }
    break;
  }

  if(0 != frame.Alloc(image.Size(), iChannels, 8, 0))
    return;

  frame.Color(iColor);
  frame.Format(iFormat);
  convert.Color(iColor);
  convert.Format(iFormat);

  if(convert.Precision() <= 8)
    convert.CopyTo    ((Ipp8u*)frame, frame.Step(), image.Size());
  else if(convert.Precision() <= 16)
    convert.ReduceBits16((Ipp8u*)frame, frame.Step(), image.Size());
  else if(convert.Precision() <= 32)
    convert.ReduceBits32((Ipp8u*)frame, frame.Step(), image.Size());

  convert.Free();

  return;
} // MainWindow::createFrame()


bool MainWindow::GetImageFromRAW(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  try
  {
    m_timer.Start();

    // read up-bottom RAW
    // m_image hold input image in original bit-depth
    err = ReadImageRAW(in, m_param_raw, image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading RAW"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_RAW;
    m_imageFormat   = QString("RAW");
    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  return false;
} // MainWindow::GetImageFromRAW()


bool MainWindow::GetImageFromBMP(BaseStreamInput& in, CIppImage& image)
{
  int      dstOrder_c3[3] = { 2, 1, 0 };
  int      dstOrder_c4[4] = { 2, 1, 0 , 3};
  IM_ERROR err;

  try
  {
    m_timer.Start();

    // read bottom-up BMP
    // m_image hold input image in original bit-depth
    err = ReadImageBMP(in, m_param_bmp, image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading BMP"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    if(image.NChannels() == 3)
    {
      image.SwapChannels(dstOrder_c3);
      image.Color(IC_RGB);
    }
    else if(image.NChannels() == 4 && (IC_BGR == image.Color() || IC_BGRA == image.Color()))
    {
      image.SwapChannels(dstOrder_c4); // convert from BGRA to RGBA
      image.Color(IC_RGBA);
    }

    m_imageType     = IT_BMP;
    m_imageFormat   = QString("BMP");
    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  return false;
} // MainWindow::GetImageFromBMP()

bool MainWindow::GetImageFromJPEG(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  try
  {
    image.Color(IC_UNKNOWN);

    m_param_jpeg.nthreads  = m_optionsDlg->numThreads();
    m_param_jpeg.dct_scale = m_optionsDlg->dctScaleValue();
    m_param_jpeg.use_qdct  = m_optionsDlg->useQDctValue();

    // decode jpeg data
    m_timer.Start();

    err = ReadImageJPEG(in, m_param_jpeg, image);
    if(IE_OK != err)
    {
      // warn about problems, but try to show what we can
      QMessageBox::critical(
        this,tr("error"),tr("Error when decoding JPEG"),QMessageBox::Ok);
    }

    m_timer.Stop();

    if(IC_CMYK == image.Color())
    {
      RGBA_FPX_to_RGBA(image,image.Width(),image.Height());
      image.Color(IC_RGBA);
    }

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_JPEG;
    m_imageFormat   = get_str_modeJPEG(m_param_jpeg.mode);
    m_imageSampling = get_str_sampling(m_param_jpeg.sampling);
    m_imageColor    = get_str_color(m_param_jpeg.color);
  }

  catch (...)
  {
    return true;
  }

  return IE_OK != err;
} // MainWindow::GetImageFromJPEG()


bool MainWindow::GetImageFromJPEG2000(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  try
  {
    m_param_jpeg2k.nthreads   = m_optionsDlg->numThreads();
    m_param_jpeg2k.arithmetic = m_optionsDlg->j2kArithmeticsValue();

    // decode jpeg2000 data
    m_timer.Start();

    err = ReadImageJPEG2000(in, m_param_jpeg2k, image);
    if(IE_OK != err)
    {
      // warn about problems, but try to show what we can
      QMessageBox::critical(
        this,tr("error"),tr("Error when decoding JPEG2000"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_JPEG2000;
    m_imageFormat   = get_str_modeJPEG2K(m_param_jpeg2k.mode);
    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  return false;
} // MainWindow::GetImageFromJPEG2000()


bool MainWindow::GetImageFromDICOM(BaseStreamInput&, CIppImage& image)
{
  IM_ERROR err;

  try
  {
    m_timer.Start();

    // read bottom-up DICOM
    m_param_dicom.param_jpeg.nthreads  = m_optionsDlg->numThreads();
    err = ReadImageDICOM(m_dicomStdFileIn, m_param_dicom, image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading DICOM"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_DICOM;
    m_imageFormat   = QString("DCM");
    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  if(!m_hasActiveThumbs)
  {
    m_dicomNumFrames = m_param_dicom.nFrames;

    if(m_dicomNumFrames > 1)
    {
      m_dicomCurrentFrame = 0;
      m_dicomSlider->setValue(1);
      m_dicomSlider->setRange(1, m_param_dicom.nFrames - 1);
      m_dicomSpinBox->setRange(1, m_param_dicom.nFrames - 1);

      m_canPlayDicom = true;

      if(!m_timer2->isActive())
        m_docks.at(0)->show();
    }
    else
    {
      m_canPlayDicom = false;
      m_docks.at(0)->hide();
    }
  }

  return false;
} // MainWindow::GetImageFromDICOM()

bool MainWindow::GetImageFromPNG(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  try
  {
    m_timer.Start();

    // m_image hold input image in original bit-depth
    err = ReadImagePNG(in, m_param_png, image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading PNG"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_PNG;
    m_imageFormat   = QString("PNG");

    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  return false;
} // MainWindow::GetImageFromPNG()

bool MainWindow::GetImageFromJPEGXR(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  m_param_jpegxr.thread_mode = 2;
  m_param_jpegxr.threads     = 0;
  m_param_jpegxr.bands       = 0;

  try
  {
    m_timer.Start();

    // m_image hold input image in original bit-depth
    err = ReadImageJPEGXR(in, m_param_jpegxr, image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading JPEGXR"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(image.Width() * image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();

    m_imageType     = IT_JPEGXR;
    m_imageFormat   = QString("JPEGXR");

    m_imageSampling = get_str_sampling(image.Sampling());
    m_imageColor    = get_str_color(image.Color());
  }

  catch (...)
  {
    return true;
  }

  return false;
} // MainWindow::GetImageFromJPEGXR()


bool MainWindow::SaveAsBMP(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR       err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save BMP"), QMessageBox::Ok);
        return false;
    }

    m_timer.Start();

    err = SaveImageBMP(src, m_param_bmp, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting BMP"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsBMP()

bool MainWindow::SaveAsJPEG(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR       err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save JPEG"), QMessageBox::Ok);
        return false;
    }
    if(!BaseStream::IsOk(status))
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting JPEG: ") +
        tr("can`t open output file! "),QMessageBox::Ok);
      return true;
    }

    m_timer.Start();

    err = SaveImageJPEG(src, m_param_jpeg, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting JPEG"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsJPEG()


bool MainWindow::SaveAsJPEG2000(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR       err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save JPEG2000"), QMessageBox::Ok);
        return false;
    }

    m_timer.Start();

    err = SaveImageJPEG2000(src, m_param_jpeg2k, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting JPEG2000"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsJPEG2000()

bool MainWindow::SaveAsPNG(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR       err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save PNG"), QMessageBox::Ok);
        return false;
    }

    m_timer.Start();

    err = SaveImagePNG(src, m_param_png, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting PNG"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsPNG()


bool MainWindow::SaveAsJPEGXR(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save JPEGXR"), QMessageBox::Ok);
        return false;
    }

    m_timer.Start();

    err = SaveImageJPEGXR(src, m_param_jpegxr, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting JPEGXR"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsJPEGXR()

bool MainWindow::SaveAsRAW(CIppImage& src, QString& fileName)
{
  CStdFileOutput fo;
  BaseStream::TStatus status;
  IM_ERROR       err;

  try
  {
    status = fo.Open(fileName.toAscii());
    if(BaseStream::StatusOk != status) {
        QMessageBox::critical(this, tr("error"), tr("Cannot save RAW"), QMessageBox::Ok);
        return false;
    }

    m_timer.Start();

    err = SaveImageRAW(src, m_param_raw, fo);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("ERROR writting RAW"),QMessageBox::Ok);
    }

    m_timer.Stop();

    m_osd.cpe_iio = (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    m_osd.sec_iio = (float)m_timer.GetTime();
  }

  catch (...)
  {
    return true;
  }

  fo.Close();

  return false;
} // MainWindow::SaveAsRAW()


void MainWindow::createMask(void)
{
  m_mask.Alloc(m_frame.Size(),1,8);

  ippiSet_8u_C1R(0, m_mask, m_mask.Step(), m_mask.Size());

  return;
} // MainWindow::createMask()


void MainWindow::loadMask(void)
{
  int r;
  CStdFileInput    in;
  IM_ERROR         err;
  UIC::BaseStream::TStatus status;

  if(false == m_hasActiveImage)
    return;

  m_optionsDlg->hide();
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load image"), ".", tr("Bitmap file (*.bmp)"));
  if(fileName.isEmpty())
    return;

  status = in.Open(fileName.toUtf8().data());
  if(UIC::BaseStream::StatusOk != status)
  {
    QMessageBox::critical(this, tr("error"), tr("Can't open mask file\n") + fileName, QMessageBox::Ok);
    return;
  }

  err = ReadImageBMP(in, m_param_bmp, m_work);
  if(IE_OK != err)
  {
    QMessageBox::critical(this,tr("error"),tr("Error reading mask"),QMessageBox::Ok);
    return;
  }

  in.Close();

  m_mask.Free();

  r = m_mask.Alloc(m_work.Size(),1,8);
  if(0 != r)
    return;

  ippiSet_8u_C1R(0, m_mask, m_mask.Step(), m_mask.Size());
  m_mask.CopyFrom((Ipp8u*)m_work,m_work.Step(),m_work.Size());

  restore();

  QPainter painter(&m_frame_pxm);
  setupPainter(painter);

  for(int y = 0; y < m_mask.Height(); y++)
    for(int x = 0; x < m_mask.Width(); x++)
      if(255 == *((Ipp8u*)m_mask + y * m_mask.Step() + x))
          painter.drawLine(QPoint(x, y), QPoint(x+1, y));

  painter.end();
  updateView();

  m_enablePaintAction->setChecked(true);

  return;
} // MainWindow::loadMask()


void MainWindow::saveMask(void)
{
  int r;

  if(0 == (Ipp8u*)m_mask && false == m_enablePaintAction->isChecked())
    return;

  m_optionsDlg->hide();
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Bitmap file (*.bmp)"));
  fileName += ".bmp";
  if(fileName.isEmpty())
    return;

  m_work.Free();
  r = m_work.Alloc(m_mask.Size(), 1, 8);
  if(0 != r)
    return;

  m_work.CopyFrom((Ipp8u*)m_mask, m_mask.Step(), m_mask.Size());

  SaveAsBMP(m_work, fileName);

  return;
} // MainWindow::saveMask()


bool MainWindow::playDICOM(int value)
{
  if(0 != m_timer3 && !m_timer3->isActive() && false != m_hasActiveImage)
  {
    m_dicomCurrentFrame = value;
    m_dicomNumFrames    = 1;

    playDICOM();
  }

  return false;
} // MainWindow::playDICOM()


static float cpe_iio = 0.0;
static float sec_iio = 0.0;
static float cpe_prc = 0.0;
static float sec_prc = 0.0;
static float cpe_rnd = 0.0;
static float sec_rnd = 0.0;
static int ii = 0;

#define UPDATE_SPEED 10

bool MainWindow::playDICOM(void)
{
  bool     update;
  IM_ERROR err = IE_OK;

  try
  {
    update = ((ii % UPDATE_SPEED) == 0);
    ii++;

    // get image
    m_timer.Start();
    m_param_dicom.param_jpeg.nthreads  = m_optionsDlg->numThreads();
    err = GetFrameDICOM(m_dicomStdFileIn, m_param_dicom, m_dicomCurrentFrame++, m_image);
    if(IE_OK != err)
    {
      QMessageBox::critical(
        this,tr("error"),tr("Error reading DICOM frame"),QMessageBox::Ok);
    }
    m_timer.Stop();

    cpe_iio += (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    sec_iio += (float)m_timer.GetTime(CTimer::msec);

    if(update)
    {
      m_osd.cpe_iio = cpe_iio / UPDATE_SPEED;
      m_osd.sec_iio = sec_iio / UPDATE_SPEED;
      cpe_iio = 0.0;
      sec_iio = 0.0;
    }

    // process image
    m_timer.Start();

    if(m_do_harmonize)
      harmonization();

    if(m_do_sobel)
      sobel();

    if(!m_do_harmonize && !m_do_sobel)
    {
      if(m_image.Precision() <= 8)
        m_image.CopyTo    ((Ipp8u*)m_frame,m_frame.Step(),m_frame.Size());
      else
        m_image.ReduceBits16((Ipp8u*)m_frame,m_frame.Step(),m_frame.Size());
    }

    m_timer.Stop();

    cpe_prc += (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    sec_prc += (float)m_timer.GetTime(CTimer::msec);

    if(update)
    {
      m_osd.cpe_prc = cpe_prc / UPDATE_SPEED;
      m_osd.sec_prc = sec_prc / UPDATE_SPEED;
      cpe_prc = 0.0;
      sec_prc = 0.0;
    }

    // render image
    m_timer.Start();

    m_frame_pxm = CreateQPixmap(m_frame);

    m_timer.Stop();

    cpe_rnd += (float)m_timer.GetCPE(m_image.Width() * m_image.Height());
    sec_rnd += (float)m_timer.GetTime(CTimer::msec);

    if(update)
    {
      m_osd.cpe_rnd = cpe_rnd / UPDATE_SPEED;
      m_osd.sec_rnd = sec_rnd / UPDATE_SPEED;
      cpe_rnd = 0.0;
      sec_rnd = 0.0;
    }

    // update info on screen
    updateView(true,&m_osd);

    updateStatusBar(
      m_imageFormat, m_image.Width(), m_image.Height(), m_image.NChannels(),
      m_image.Precision(), m_imageSampling, m_imageColor,
      m_osd.cpe_iio, m_osd.sec_iio);

    if(--m_dicomNumFrames <= 0)
    {
      m_dicomCurrentFrame = 0;
      m_dicomNumFrames = m_param_dicom.nFrames;
    }
  }

  catch (...)
  {
    return true;
  }

  if(0 != m_timer3 && m_timer3->isActive())
    m_dicomSlider->setValue(m_dicomCurrentFrame);

  return false;
} // MainWindow::playDICOM()


bool MainWindow::startPlayDICOM(void)
{
  if(0 != m_timer3 && m_canPlayDicom)
  {
    if(!m_timer3->isActive() && (m_dicomCurrentFrame < m_param_dicom.nFrames))
    {
      int value = m_optionsDlg->slideshowValue() * 1000;
      m_dicomNumFrames = m_param_dicom.nFrames - m_dicomCurrentFrame;
      m_timer3->setInterval(value);
      m_timer3->start();
      m_playDicomAction->setText(tr("&Stop DICOM"));
      m_slideshowAction->setEnabled(false);
      m_nextAction->setEnabled(false);
      m_previousAction->setEnabled(false);
    }
    else
    {
      m_timer3->stop();
      m_playDicomAction->setText(tr("&Play DICOM"));
      m_slideshowAction->setEnabled(m_canSlideshow);
      updateMenus();
    }
  }

  return false;
} // MainWindow::startPlayDICOM()


void MainWindow::setupPainter(QPainter& painter, bool isSolid)
{
  painter.setRenderHint(QPainter::Antialiasing, true);
  if(isSolid)
  {
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
  }
  else
    painter.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::SquareCap, Qt::RoundJoin));

  return;
} // MainWindow::setupPainter()


void MainWindow::OnSearchByEdgeHistogram(void)
{
  OnSearch(ImageEdgeHistogram);
  return;
} // MainWindow::OnSearchByEdge()


void MainWindow::OnSearchByColorLayout(void)
{
  OnSearch(ImageColorLayout);
  return;
} // MainWindow::OnSearchByColor()


void MainWindow::OnSearch(FeatureId featureId)
{
  int i;
  int index;
  int nFiles    = m_descriptordb.Size();
  int outCount  = nFiles;
  int nSelected = m_imagesSelected.count();

  float* outRatings        = new float[nFiles];
  float* inRatings0        = new float[nSelected];
  const char** names       = new const char*[nSelected];
  const char** mostSimilar = new const char*[nFiles];

  for(i = 0; i < nSelected; i++)
  {
    const FileNameFeature& currName = reinterpret_cast<const FileNameFeature&>
      (m_descriptordb.Descriptor(m_imagesSelected.at(i)).Feature(ImageFileName));

    if(currName.IsEmpty())
      continue;

    names[i]      = currName.Get();
    inRatings0[i] = 1.0;
  }

  outCount = m_descriptordb.FindSimilar(names, inRatings0, nSelected, mostSimilar, outRatings, outCount, featureId);

  if(outCount < 0)
    return;

  for(i = 0; i < outCount; i++)
  {
    m_descriptordb.Index(mostSimilar[i], index);
    foreach(ImageWidget* imageWidget, m_pixmapLabels)
    {
      if(index == imageWidget->m_id)
      {
        imageWidget->m_ratingLabel->setText(QString(tr("rating: %1")).arg(outRatings[i]));
        m_pixmapLabels.removeAll(imageWidget);
        m_pixmapLabels.append(imageWidget);
        break;
      }
    }
  }

  m_imagesSelected.clear();

  m_needToRedraw = true;
  m_canSearch    = false;

  updateMenus();

  int w = g_pMainWnd->width();
  int h = g_pMainWnd->height();

  QResizeEvent* revent = new QResizeEvent(QSize(w,h),QSize(w,h));
  QApplication::sendEvent(g_pMainWnd, revent);

  m_scrollArea->verticalScrollBar()->setValue(0);

  delete[] outRatings;
  delete[] inRatings0;
  delete[] names;
  delete[] mostSimilar;

  return;
} // MainWindow::OnSearch()


void MainWindow::OnExtractAll(void)
{
  int  i;
  int  nFiles;
  bool bres;

  QStringList files;

  nFiles = m_descriptordb.Size();

  for(i = 0; i < nFiles; i++)
  {
    const FileNameFeature& currName = reinterpret_cast<const FileNameFeature&>
      (m_descriptordb.Descriptor(i).Feature(ImageFileName));

    if(currName.IsEmpty())
     continue;

    files << currName.Get();
  }

  DestroyImageDescriptorDB(m_descriptordb);

  m_descriptordb.Init();

  ProgressShow(0, nFiles, "Extracting features...");

  for(i = 0; i < nFiles; i++)
  {
    ISF::Status status;

    bres = openFile(QString(files[i]));

    ImageDescriptor*      descriptor = new ImageDescriptor;
    FileNameFeature*      name       = new FileNameFeature;
    ColorLayoutFeature*   cld        = new ColorLayoutFeature;
    EdgeHistogramFeature* ehd        = new EdgeHistogramFeature;

    if(0 == descriptor || 0 == name || 0 == cld || 0 == ehd)
      return;

    status = descriptor->Init();
    if(Ok != status)
      continue;

    status = name->Init();
    if(Ok != status)
      continue;

    status = cld->Init();
    if(Ok != status)
      continue;

    status = ehd->Init();
    if(Ok != status)
      continue;

    status = name->Set(files[i].toAscii());
    if(Ok != status)
      continue;

    status = cld->Extract(m_image);
    if(Ok != status)
      continue;

    status = ehd->Extract(m_image);
    if(Ok != status)
      continue;

    status = descriptor->Insert(*name);
    if(Ok != status)
      continue;

    status = descriptor->Insert(*cld);
    if(Ok != status)
      continue;

    status = descriptor->Insert(*ehd);
    if(Ok != status)
      continue;

    status = m_descriptordb.Insert(*descriptor);
    if(Ok != status)
      continue;

    ProgressUpdate(i);
    qApp->processEvents(QEventLoop::AllEvents);
  }

  ProgressHide();

  return;
} // MainWindow::OnExtractByAll()


void MainWindow::ProgressShow(int min, int max, QString title)
{
  m_progressBar.setWindowFlags(Qt::Window | Qt::WindowTitleHint);
  m_progressBar.setWindowModality(Qt::ApplicationModal);
  m_progressBar.move(frameGeometry().center());
  m_progressBar.setMinimum(min);
  m_progressBar.setMaximum(max);
  m_progressBar.setWindowTitle(title);
  m_progressBar.show();

  return;
} // MainWindow::ProgressShow()


void MainWindow::ProgressUpdate(int value)
{
  m_progressBar.setValue(value);

  return;
} // MainWindow::ProgressUpdate()


void MainWindow::ProgressHide(void)
{
  m_progressBar.hide();

  return;
} // MainWindow::ProgressHide()
