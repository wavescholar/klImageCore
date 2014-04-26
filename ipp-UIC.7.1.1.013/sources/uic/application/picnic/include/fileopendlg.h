/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __FILEOPENDLG_H__
#define __FILEOPENDLG_H__

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif
#ifndef __RAW_H__
#include "raw.h"
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
#ifndef __PNG_H__
#include "png.h"
#endif
#ifndef __JPEGXR_H__
#include "jpegxr.h"
#endif
#ifndef __DETECTOR_H__
#include "detector.h"
#endif


class FileOpenDialog : public QFileDialog
{
  Q_OBJECT

public:
  FileOpenDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
  virtual ~FileOpenDialog(void);

  int Init(const QString& path);

  CIppImage         m_image;
  CIppImage         m_frame;
  CFormatDetector   m_imageDetector;
  IM_TYPE           m_imageType;
  QString           m_imageFormat;
  QString           m_imageColor;
  QString           m_imageSampling;
  QString           m_imageComment;
  QString           m_fileSelected;
  bool              m_isRaw;

  int               m_rawWidth;
  int               m_rawHeight;
  int               m_rawNChannels;
  int               m_rawPrecision;
  int               m_rawOffset;
  PARAMS_BMP        m_param_bmp;
  PARAMS_JPEG       m_param_jpeg;
  PARAMS_JPEG2K     m_param_jpeg2k;
  PARAMS_DICOM      m_param_dicom;
  PARAMS_RAW        m_param_raw;
  PARAMS_PNG        m_param_png;
  PARAMS_JPEGXR     m_param_jpegxr;

  QLineEdit         m_rawWidthEdit;
  QLineEdit         m_rawHeightEdit;
  QLineEdit         m_rawNChannelsEdit;
  QLineEdit         m_rawPrecisionEdit;
  QLineEdit         m_rawOffsetEdit;
  QLineEdit         m_imageCommentEdit;

  QGridLayout*      m_rawOptionsLayout;
  QGridLayout*      m_mainLayout;
  QVBoxLayout*      m_imagePreviewMainLayout;
  QVBoxLayout*      m_rawOptionsMainLayout;
  QStackedLayout*   m_previewLayout;
  QWidget*          m_imagePreviewWidget;
  QWidget*          m_rawOptionsWidget;

  QWidget*          m_openOptionsWidget;
  QWidget*          m_openEmptyWidget;
  QStackedLayout*   m_openOptionsLayout;

  QGroupBox*        m_rawOptionsGroupBox;

  QPushButton       m_rawApplyButton;

  QLabel            m_imagePreviewLabel;
  QLabel            m_imageTextLabel;
  QLabel            m_rawOptionsWidth;
  QLabel            m_rawOptionsHeight;
  QLabel            m_rawOptionsNChannels;
  QLabel            m_rawOptionsPrecision;
  QLabel            m_rawOptionsOffset;
  QLabel            m_imageCommentLabel;

  QCheckBox         m_showPreviewCheckBox;

protected:
  bool GetThumbnailFromBMP(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromJPEG(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromJPEG2000(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromDICOM(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromRAW(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromPNG(BaseStreamInput& in, CIppImage& image);
  bool GetThumbnailFromJPEGXR(BaseStreamInput& in, CIppImage& image);

  void createFrame(CIppImage& image, CIppImage& frame);
  void setOpenOptions(IM_TYPE fmt);

private slots:
  void OnOk(void);
  void OnCancel(void);
  void currentChanged(const QString& selected);
  void changeByteOrder(void);
  void filterSelected(const QString& filter);
  void rawApply(void);

};

#endif // __FILEOPENDLG_H__

