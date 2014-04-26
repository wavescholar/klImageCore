/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __FILESAVEDLG_H__
#define __FILESAVEDLG_H__

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#ifndef __JPEG_H__
#include "jpeg.h"
#endif
#ifndef __JPEG2K_H__
#include "jpeg2k.h"
#endif

class FileSaveDialog : public QFileDialog
{
  Q_OBJECT

public:
  FileSaveDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0, CIppImage *pimage = 0);
  virtual ~FileSaveDialog(void);

  int Init(const QString& path);

  CIppImage* m_image;

  QStackedLayout*   m_optionsLayout;
  QVBoxLayout*      m_pngFilterLayout;
  QVBoxLayout*      m_pngMainLayout;
  QVBoxLayout*      m_jpegTypeLayout;
  QVBoxLayout*      m_jpegSamplingLayout;
  QHBoxLayout*      m_jpegTopLayout;
  QHBoxLayout*      m_jpegQualityLayout;
  QVBoxLayout*      m_jpegMainLayout;
  QVBoxLayout*      m_jpeg2000ModeLayout;
  QVBoxLayout*      m_jpeg2000wtLayout;
  QHBoxLayout*      m_jpeg2000TopLayout;
  QHBoxLayout*      m_jpeg2000RatioLayout;
  QVBoxLayout*      m_jpeg2000MainLayout;
  QHBoxLayout*      m_jpegxrTilingLayout;
  QVBoxLayout*      m_jpegxrTilingModeLayout;
  QVBoxLayout*      m_jpegxrTilesLayout;
  QHBoxLayout*      m_jpegxrTilesXLayout;
  QHBoxLayout*      m_jpegxrTilesYLayout;
  QHBoxLayout*      m_jpegxrTFLayout;
  QVBoxLayout*      m_jpegxrOverlapLayout;
  QHBoxLayout*      m_jpegxrSBLayout;
  QVBoxLayout*      m_jpegxrBitstreamLayout;
  QVBoxLayout*      m_jpegxrFBTrimLayout;
  QVBoxLayout*      m_jpegxrSamplingLayout;
  QVBoxLayout*      m_jpegxrBandsLayout;
  QVBoxLayout*      m_jpegxrBTLayout;
  QHBoxLayout*      m_jpegxrQualityLayout;
  QHBoxLayout*      m_jpegxrAQualityLayout;
  QVBoxLayout*      m_jpegxrMainLayout;

  QGridLayout*      m_mainLayout;

  QWidget           m_bmpOptionsWidget;
  QWidget           m_jpegOptionsWidget;
  QWidget           m_jpeg2000OptionsWidget;
  QWidget           m_rawOptionsWidget;
  QWidget           m_pngOptionsWidget;
  QWidget           m_jpegxrOptionsWidget;

  QGroupBox         m_pngFilterGroupBox;
  QGroupBox         m_jpegTypeGroupBox;
  QGroupBox         m_jpegSamplingGroupBox;
  QGroupBox         m_jpegQualityGroupBox;
  QGroupBox         m_jpeg2000ModeGroupBox;
  QGroupBox         m_jpeg2000wtGroupBox;
  QGroupBox         m_jpeg2000RatioGroupBox;
  QGroupBox         m_jpegxrQualityGroupBox;
  QGroupBox         m_jpegxrAQualityGroupBox;
  QGroupBox         m_jpegxrBandsGroupBox;
  QGroupBox         m_jpegxrFBTrimGroupBox;
  QGroupBox         m_jpegxrBitstreamGroupBox;
  QGroupBox         m_jpegxrSamplingGroupBox;
  QGroupBox         m_jpegxrOverlapGroupBox;
  QGroupBox         m_jpegxrTilingGroupBox;
  QGroupBox         m_jpegxrTilesXGroupBox;
  QGroupBox         m_jpegxrTilesYGroupBox;

  QRadioButton      m_jpegBASRadioButton;
  QRadioButton      m_jpegEXTRadioButton;
  QRadioButton      m_jpegPRGRadioButton;
  QRadioButton      m_jpegLSLRadioButton;
  QRadioButton      m_jpeg411RadioButton;
  QRadioButton      m_jpeg422RadioButton;
  QRadioButton      m_jpeg444RadioButton;
  QRadioButton      m_jpeg2000LossyRadioButton;
  QRadioButton      m_jpeg2000LosslessRadioButton;
  QRadioButton      m_jpeg2000wt53RadioButton;
  QRadioButton      m_jpeg2000wt97RadioButton;
  QRadioButton      m_jpegxrOverlap0RadioButton;
  QRadioButton      m_jpegxrOverlap1RadioButton;
  QRadioButton      m_jpegxrOverlap2RadioButton;
  QRadioButton      m_jpegxrBands0RadioButton;
  QRadioButton      m_jpegxrBands1RadioButton;
  QRadioButton      m_jpegxrBands2RadioButton;
  QRadioButton      m_jpegxrBands3RadioButton;
  QRadioButton      m_jpegxrSampling0RadioButton;
  QRadioButton      m_jpegxrSampling1RadioButton;
  QRadioButton      m_jpegxrSampling2RadioButton;
  QRadioButton      m_jpegxrBitstream0RadioButton;
  QRadioButton      m_jpegxrBitstream1RadioButton;
  QRadioButton      m_jpegxrTiling0RadioButton;
  QRadioButton      m_jpegxrTiling1RadioButton;

  QCheckBox         m_jpegRestartIntervalCheckBox;
  QCheckBox         m_jpegOtimalHuffmanCheckBox;
  QCheckBox         m_jpeg2000MCTCheckBox;
  QCheckBox         m_jpegxrAlphaCheckBox;
  QCheckBox         m_pngFilter0CheckBox;
  QCheckBox         m_pngFilter1CheckBox;
  QCheckBox         m_pngFilter2CheckBox;
  QCheckBox         m_pngFilter3CheckBox;

  QComboBox         m_jpegColorComboBox;
  QSlider           m_jpegQualitySlider;
  QSlider           m_jpeg2000RatioSlider;
  QSlider           m_jpegxrQualitySlider;
  QSlider           m_jpegxrAQualitySlider;
  QSpinBox          m_jpegQualitySpinBox;
  QSpinBox          m_jpeg2000RatioSpinBox;
  QSpinBox          m_jpegxrQualitySpinBox;
  QSpinBox          m_jpegxrFBTrimSpinBox;
  QSpinBox          m_jpegxrAQualitySpinBox;
  QSpinBox          m_jpegxrTilesXSpinBox;
  QSpinBox          m_jpegxrTilesYSpinBox;

  QLineEdit         m_imageCommentEdit;
  QLabel            m_imageCommentLabel;

public:
  int            jpegColor(void);
  int            jpegMode(void);
  int            jpegQuality(void);
  int            jpegRestartInterval(void);
  int            jpegSampling(void);
  int            jpegHuffman(void);
  int            jpeg2000wt(void);
  int            jpeg2000MCT(void);
  int            jpeg2000Ratio(void);
  int            jpeg2000Mode(void);

  int            pngFilter(void);
  int            pngFilterSub(void);
  int            pngFilterUp(void);
  int            pngFilterAvg(void);
  int            pngFilterPaeth(void);

  int            jpegxrQuality(void);
  int            jpegxrAQuality(void);
  int            jpegxrOverlap(void);
  int            jpegxrSampling(void);
  int            jpegxrAlpha(void);
  int            jpegxrBands(void);
  int            jpegxrFrequency(void);
  int            jpegxrFBTrim(void);
  int            jpegxrTilingMode(void);
  int            jpegxrTilesX(void);
  int            jpegxrTilesY(void);

  int            imageComment(char* comment, int len);
  int            imageCommentSize(void);

private slots:
  void OnOk(void);
  void OnCancel(void);
  void updateJpegButtons(void);
  void updateJpeg2000Buttons(void);
  void updateJpegxrButtons(void);

  void filterSelected(const QString& filter);

};

#endif // __FILESAVEDLG_H__

