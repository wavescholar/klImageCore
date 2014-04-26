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

#ifndef __FILESAVEDLG_H__
#include "filesavedlg.h"
#endif
#ifndef __MAINWINDOW_H__
#include "mainwindow.h"
#endif


FileSaveDialog::FileSaveDialog(QWidget* parent, Qt::WindowFlags flags, CIppImage *pimage) :
  QFileDialog(parent, flags)
{
  m_image = pimage;

  return;
} // ctor


FileSaveDialog::~FileSaveDialog(void)
{
  if(0 != m_optionsLayout)
    delete m_optionsLayout;

  if(0 != m_pngMainLayout)
    delete m_pngMainLayout;

  if(0 != m_jpegTypeLayout)
    delete m_jpegTypeLayout;

  if(0 != m_jpegSamplingLayout)
    delete m_jpegSamplingLayout;

  if(0 != m_jpegTopLayout)
    delete m_jpegTopLayout;

  if(0 != m_jpegQualityLayout)
    delete m_jpegQualityLayout;

  if(0 != m_jpegMainLayout)
    delete m_jpegMainLayout;

  if(0 != m_jpeg2000ModeLayout)
    delete m_jpeg2000ModeLayout;

  if(0 != m_jpeg2000wtLayout)
    delete m_jpeg2000wtLayout;

  if(0 != m_jpeg2000TopLayout)
    delete m_jpeg2000TopLayout;

  if(0 != m_jpeg2000RatioLayout)
    delete m_jpeg2000RatioLayout;

  if(0 != m_jpeg2000MainLayout)
    delete m_jpeg2000MainLayout;

  if(0 != m_jpegxrOverlapLayout)
    delete m_jpegxrOverlapLayout;

  if(0 != m_jpegxrTilingModeLayout)
    delete m_jpegxrTilingModeLayout;

  if(0 != m_jpegxrTilesLayout)
    delete m_jpegxrTilesLayout;

  if(0 != m_jpegxrTilingLayout)
    delete m_jpegxrTilingLayout;

  if(0 != m_jpegxrTilesXLayout)
    delete m_jpegxrTilesXLayout;

  if(0 != m_jpegxrTilesYLayout)
    delete m_jpegxrTilesYLayout;

  if(0 != m_jpegxrTFLayout)
    delete m_jpegxrTFLayout;

  if(0 != m_jpegxrBitstreamLayout)
    delete m_jpegxrBitstreamLayout;

  if(0 != m_jpegxrBTLayout)
    delete m_jpegxrBTLayout;

  if(0 != m_jpegxrBandsLayout)
    delete m_jpegxrBandsLayout;

  if(0 != m_jpegxrSamplingLayout)
    delete m_jpegxrSamplingLayout;

  if(0 != m_jpegxrSBLayout)
    delete m_jpegxrSBLayout;

  if(0 != m_jpegxrFBTrimLayout)
    delete m_jpegxrFBTrimLayout;

  if(0 != m_jpegxrAQualityLayout)
    delete m_jpegxrAQualityLayout;

  if(0 != m_jpegxrQualityLayout)
    delete m_jpegxrQualityLayout;

  if(0 != m_jpegxrMainLayout)
    delete m_jpegxrMainLayout;

  if(0 != m_mainLayout)
    delete m_mainLayout;

  return;
} // dtor


int FileSaveDialog::Init(const QString& path)
{
  QStringList imageFilters;
  imageFilters.append(tr("Windows Bitmap (*.bmp)"));
  imageFilters.append(tr("JPG/JPEG Format (*.jpg)"));
  imageFilters.append(tr("JP2 Format (*.jp2)"));
  imageFilters.append(tr("PNG Format (*.png)"));
  imageFilters.append(tr("JPEGXR Format (*.jxr *.wdp *.hdp)"));
  //imageFilters.append(tr("RAW Image Data (*.raw)"));

  setFilters(imageFilters);
  setAcceptMode(QFileDialog::AcceptSave);

  setDirectory(path);

  m_optionsLayout = new QStackedLayout();
  if(0 == m_optionsLayout)
    return -1;

  m_pngFilterLayout = new QVBoxLayout(&m_pngFilterGroupBox);
  if(0 == m_pngFilterLayout)
    return -1;

  m_pngMainLayout = new QVBoxLayout(&m_pngOptionsWidget);
  if(0 == m_pngMainLayout)
    return -1;

  m_jpegxrQualityLayout = new QHBoxLayout(&m_jpegxrQualityGroupBox);
  if(0 == m_jpegxrQualityLayout)
    return -1;

  m_jpegxrOverlapLayout = new QVBoxLayout(&m_jpegxrOverlapGroupBox);
  if(0 == m_jpegxrOverlapLayout)
    return -1;

  m_jpegxrTilesLayout = new QVBoxLayout();
  if(0 == m_jpegxrTilesLayout)
    return -1;

  m_jpegxrTilingModeLayout = new QVBoxLayout();
  if(0 == m_jpegxrTilingModeLayout)
    return -1;

  m_jpegxrTilingLayout = new QHBoxLayout(&m_jpegxrTilingGroupBox);
  if(0 == m_jpegxrTilingLayout)
    return -1;

  m_jpegxrTilesXLayout = new QHBoxLayout(&m_jpegxrTilesXGroupBox);
  if(0 == m_jpegxrTilesXLayout)
    return -1;

  m_jpegxrTilesYLayout = new QHBoxLayout(&m_jpegxrTilesYGroupBox);
  if(0 == m_jpegxrTilesYLayout)
    return -1;

  m_jpegxrTFLayout = new QHBoxLayout();
  if(0 == m_jpegxrTFLayout)
    return -1;

  m_jpegxrBitstreamLayout = new QVBoxLayout(&m_jpegxrBitstreamGroupBox);
  if(0 == m_jpegxrBitstreamLayout)
    return -1;

  m_jpegxrBTLayout = new QVBoxLayout();
  if(0 == m_jpegxrBTLayout)
    return -1;

  m_jpegxrFBTrimLayout = new QVBoxLayout(&m_jpegxrFBTrimGroupBox);
  if(0 == m_jpegxrFBTrimLayout)
    return -1;

  m_jpegxrBandsLayout = new QVBoxLayout(&m_jpegxrBandsGroupBox);
  if(0 == m_jpegxrBandsLayout)
    return -1;

  m_jpegxrSamplingLayout = new QVBoxLayout(&m_jpegxrSamplingGroupBox);
  if(0 == m_jpegxrSamplingLayout)
    return -1;

  m_jpegxrSBLayout = new QHBoxLayout();
  if(0 == m_jpegxrSBLayout)
    return -1;

  m_jpegxrAQualityLayout = new QHBoxLayout(&m_jpegxrAQualityGroupBox);
  if(0 == m_jpegxrAQualityLayout)
    return -1;

  m_jpegxrMainLayout = new QVBoxLayout(&m_jpegxrOptionsWidget);
  if(0 == m_jpegxrMainLayout)
    return -1;

  m_jpegTypeLayout = new QVBoxLayout(&m_jpegTypeGroupBox);
  if(0 == m_jpegTypeLayout)
    return -1;

  m_jpegSamplingLayout = new QVBoxLayout(&m_jpegSamplingGroupBox);
  if(0 == m_jpegSamplingLayout)
    return -1;

  m_jpegTopLayout = new QHBoxLayout();
  if(0 == m_jpegTopLayout)
    return -1;

  m_jpegQualityLayout = new QHBoxLayout(&m_jpegQualityGroupBox);
  if(0 == m_jpegQualityLayout)
    return -1;

  m_jpegMainLayout = new QVBoxLayout(&m_jpegOptionsWidget);
  if(0 == m_jpegMainLayout)
    return -1;

  m_jpeg2000ModeLayout = new QVBoxLayout(&m_jpeg2000ModeGroupBox);
  if(0 == m_jpeg2000ModeLayout)
    return -1;

  m_jpeg2000wtLayout = new QVBoxLayout(&m_jpeg2000wtGroupBox);
  if(0 == m_jpeg2000wtLayout)
    return -1;

  m_jpeg2000TopLayout = new QHBoxLayout();
  if(0 == m_jpeg2000TopLayout)
    return -1;

  m_jpeg2000RatioLayout = new QHBoxLayout(&m_jpeg2000RatioGroupBox);
  if(0 == m_jpeg2000RatioLayout)
    return -1;

  m_jpeg2000MainLayout = new QVBoxLayout(&m_jpeg2000OptionsWidget);
  if(0 == m_jpeg2000MainLayout)
    return -1;

// ---------- PNG ---------------------

  m_pngFilterGroupBox.setTitle(tr("Filters"));
  m_pngFilter0CheckBox.setText(tr("Sub"));
  m_pngFilter0CheckBox.setChecked(true);
  m_pngFilter1CheckBox.setText(tr("Up"));
  m_pngFilter1CheckBox.setChecked(true);
  m_pngFilter2CheckBox.setText(tr("Average"));
  m_pngFilter2CheckBox.setChecked(true);
  m_pngFilter3CheckBox.setText(tr("Paeth"));
  m_pngFilter3CheckBox.setChecked(true);

  m_pngFilterLayout->addWidget(&m_pngFilter0CheckBox);
  m_pngFilterLayout->addWidget(&m_pngFilter1CheckBox);
  m_pngFilterLayout->addWidget(&m_pngFilter2CheckBox);
  m_pngFilterLayout->addWidget(&m_pngFilter3CheckBox);
  m_pngMainLayout->addWidget(&m_pngFilterGroupBox);
  m_pngMainLayout->addStretch();

// ---------- JPEGXR ---------------------

  m_jpegxrOverlapGroupBox.setTitle(tr("Filters"));
  m_jpegxrOverlap0RadioButton.setText(tr("No"));
  m_jpegxrOverlap1RadioButton.setText(tr("One"));
  m_jpegxrOverlap2RadioButton.setText(tr("Two"));
  m_jpegxrOverlap0RadioButton.setChecked(true);

  m_jpegxrOverlapLayout->addWidget(&m_jpegxrOverlap0RadioButton);
  m_jpegxrOverlapLayout->addWidget(&m_jpegxrOverlap1RadioButton);
  m_jpegxrOverlapLayout->addWidget(&m_jpegxrOverlap2RadioButton);
  m_jpegxrTFLayout->addWidget(&m_jpegxrOverlapGroupBox);
  m_jpegxrFBTrimGroupBox.setTitle(tr("FB trim"));
  m_jpegxrFBTrimSpinBox.setRange(0, 15);
  m_jpegxrFBTrimSpinBox.setValue(0);
  m_jpegxrFBTrimLayout->addWidget(&m_jpegxrFBTrimSpinBox);
  m_jpegxrTFLayout->addWidget(&m_jpegxrFBTrimGroupBox);
  m_jpegxrMainLayout->addLayout(m_jpegxrTFLayout);

  m_jpegxrQualityGroupBox.setTitle(tr("Quality"));
  m_jpegxrQualitySlider.setOrientation(Qt::Horizontal);
  m_jpegxrQualitySlider.setTickInterval(20);
  m_jpegxrQualitySlider.setRange(1, 255);
  m_jpegxrQualitySlider.setTickPosition(QSlider::TicksBelow);
  m_jpegxrQualitySlider.setValue(1);
  m_jpegxrQualitySpinBox.setRange(1, 255);
  m_jpegxrQualitySpinBox.setValue(1);
  m_jpegxrQualityLayout->addWidget(&m_jpegxrQualitySlider);
  m_jpegxrQualityLayout->addWidget(&m_jpegxrQualitySpinBox);

  connect(&m_jpegxrQualitySpinBox, SIGNAL(valueChanged(int)), &m_jpegxrQualitySlider,  SLOT(setValue(int)));
  connect(&m_jpegxrQualitySlider,  SIGNAL(valueChanged(int)), &m_jpegxrQualitySpinBox, SLOT(setValue(int)));
  m_jpegxrMainLayout->addWidget(&m_jpegxrQualityGroupBox);

  m_jpegxrAlphaCheckBox.setText(tr("Planar alpha"));
  m_jpegxrAlphaCheckBox.setChecked(false);
  m_jpegxrMainLayout->addWidget(&m_jpegxrAlphaCheckBox);

  switch(m_image->Color())
  {
  case RGBA:
  case BGRA:
  case RGBAP:
  case BGRAP:
  case CMYKA:
    m_jpegxrAlphaCheckBox.setDisabled(false);
    break;
  default:
    m_jpegxrAlphaCheckBox.setDisabled(true);
    break;
  }

  connect(&m_jpegxrAlphaCheckBox, SIGNAL(clicked()), this, SLOT(updateJpegxrButtons()));

  m_jpegxrAQualityGroupBox.setTitle(tr("Alpha quality"));
  m_jpegxrAQualitySlider.setOrientation(Qt::Horizontal);
  m_jpegxrAQualitySlider.setTickInterval(20);
  m_jpegxrAQualitySlider.setRange(1, 255);
  m_jpegxrAQualitySlider.setTickPosition(QSlider::TicksBelow);
  m_jpegxrAQualitySlider.setValue(1);
  m_jpegxrAQualitySpinBox.setRange(1, 255);
  m_jpegxrAQualitySpinBox.setValue(1);
  m_jpegxrAQualityLayout->addWidget(&m_jpegxrAQualitySlider);
  m_jpegxrAQualityLayout->addWidget(&m_jpegxrAQualitySpinBox);

  if(!m_jpegxrAlphaCheckBox.isChecked())
  {
    m_jpegxrAQualitySlider.setDisabled(true);
    m_jpegxrAQualitySpinBox.setDisabled(true);
  }

  connect(&m_jpegxrAQualitySpinBox, SIGNAL(valueChanged(int)), &m_jpegxrAQualitySlider,  SLOT(setValue(int)));
  connect(&m_jpegxrAQualitySlider,  SIGNAL(valueChanged(int)), &m_jpegxrAQualitySpinBox, SLOT(setValue(int)));
  m_jpegxrMainLayout->addWidget(&m_jpegxrAQualityGroupBox);


  m_jpegxrTilingGroupBox.setTitle(tr("Tiling"));
  m_jpegxrTilesXGroupBox.setTitle(tr("Horizontal"));
  m_jpegxrTilesXSpinBox.setRange(0, 4096);
  m_jpegxrTilesXSpinBox.setValue(0);
  m_jpegxrTilesXLayout->addWidget(&m_jpegxrTilesXSpinBox);
  m_jpegxrTilesLayout->addWidget(&m_jpegxrTilesXGroupBox);

  m_jpegxrTilesYGroupBox.setTitle(tr("Vertical"));
  m_jpegxrTilesYSpinBox.setRange(0, 4096);
  m_jpegxrTilesYSpinBox.setValue(0);
  m_jpegxrTilesYLayout->addWidget(&m_jpegxrTilesYSpinBox);
  m_jpegxrTilesLayout->addWidget(&m_jpegxrTilesYGroupBox);
  m_jpegxrTilingLayout->addLayout(m_jpegxrTilesLayout);

  m_jpegxrTiling0RadioButton.setText(tr("Amount"));
  m_jpegxrTiling1RadioButton.setText(tr("Size (1=16px)"));
  m_jpegxrTiling0RadioButton.setChecked(true);
  m_jpegxrTilingModeLayout->addWidget(&m_jpegxrTiling0RadioButton);
  m_jpegxrTilingModeLayout->addWidget(&m_jpegxrTiling1RadioButton);
  m_jpegxrTilingLayout->addLayout(m_jpegxrTilingModeLayout);
  m_jpegxrMainLayout->addWidget(&m_jpegxrTilingGroupBox);

  m_jpegxrMainLayout->addStretch();

// ---------- JPEG ---------------------

  m_jpegTypeGroupBox.setTitle(tr("JPEG"));
  m_jpegBASRadioButton.setText(tr("BAS"));
  m_jpegEXTRadioButton.setText(tr("EXT"));
  m_jpegPRGRadioButton.setText(tr("PRG"));
  m_jpegLSLRadioButton.setText(tr("LSL"));
  m_jpegTypeLayout->addWidget(&m_jpegBASRadioButton);
  m_jpegTypeLayout->addWidget(&m_jpegEXTRadioButton);
  m_jpegTypeLayout->addWidget(&m_jpegPRGRadioButton);
  m_jpegTypeLayout->addWidget(&m_jpegLSLRadioButton);

  m_jpegSamplingGroupBox.setTitle(tr("Sampling"));

  m_jpeg411RadioButton.setText(tr("411"));
  m_jpeg422RadioButton.setText(tr("422"));
  m_jpeg444RadioButton.setText(tr("444"));
  m_jpegSamplingLayout->addWidget(&m_jpeg411RadioButton);
  m_jpegSamplingLayout->addWidget(&m_jpeg422RadioButton);
  m_jpegSamplingLayout->addWidget(&m_jpeg444RadioButton);

  m_jpegTopLayout->addWidget(&m_jpegTypeGroupBox);
  m_jpegTopLayout->addWidget(&m_jpegSamplingGroupBox);

  connect(&m_jpegBASRadioButton, SIGNAL(clicked()), this, SLOT(updateJpegButtons()));
  connect(&m_jpegEXTRadioButton, SIGNAL(clicked()), this, SLOT(updateJpegButtons()));
  connect(&m_jpegPRGRadioButton, SIGNAL(clicked()), this, SLOT(updateJpegButtons()));
  connect(&m_jpegLSLRadioButton, SIGNAL(clicked()), this, SLOT(updateJpegButtons()));

  m_jpegRestartIntervalCheckBox.setText(tr("Restart interval"));
  m_jpegRestartIntervalCheckBox.setChecked(false);

  m_jpegOtimalHuffmanCheckBox.setText(tr("Use optimal Huffman table"));
  m_jpegOtimalHuffmanCheckBox.setChecked(false);

  if(m_image->NChannels() == 1)
  {
    m_jpegColorComboBox.addItem(tr("Gray"));
    m_jpeg444RadioButton.setChecked(true);
    m_jpeg422RadioButton.setDisabled(true);
    m_jpeg411RadioButton.setDisabled(true);
  }
  else if(m_image->NChannels() == 3)
  {
    if(m_image->Precision() != 12)
    {
      m_jpegColorComboBox.addItem(tr("YCbCr"));
      m_jpegColorComboBox.addItem(tr("RGB"));
    }
    else
    {
      m_jpegColorComboBox.addItem(tr("UNK"));
    }
  }
  else if(m_image->NChannels() == 4)
  {
    m_jpegColorComboBox.addItem(tr("YCCK"));
    m_jpegColorComboBox.addItem(tr("CMYK"));
    m_jpegColorComboBox.addItem(tr("UNK"));
  }

  if(m_image->Precision() <= 8)
  {
    m_jpegBASRadioButton.setChecked(true);
    m_jpegEXTRadioButton.setDisabled(true);
    m_jpeg411RadioButton.setChecked(true);
  }
  else
  {
    if(m_image->Precision() != 12)
    {
      m_jpegEXTRadioButton.setDisabled(true);
      m_jpegBASRadioButton.setDisabled(true);
      m_jpegPRGRadioButton.setDisabled(true);
      m_jpegLSLRadioButton.setChecked (true);
    }
    else
    {
      m_jpegEXTRadioButton.setChecked(true);;
      m_jpegBASRadioButton.setDisabled(true);
      m_jpegPRGRadioButton.setDisabled(true);
    }

    m_jpeg444RadioButton.setChecked(true);
    m_jpeg422RadioButton.setDisabled(true);
    m_jpeg411RadioButton.setDisabled(true);
  }

  m_jpegQualityGroupBox.setTitle(tr("Quality"));
  m_jpegQualitySlider.setOrientation(Qt::Horizontal);
  m_jpegQualitySlider.setTickInterval(10);
  m_jpegQualitySlider.setRange(0, 100);
  m_jpegQualitySlider.setTickPosition(QSlider::TicksBelow);
  m_jpegQualitySpinBox.setRange(0, 100);
  m_jpegQualitySlider.setValue(75);
  m_jpegQualityLayout->addWidget(&m_jpegQualitySlider);
  m_jpegQualityLayout->addWidget(&m_jpegQualitySpinBox);
  m_jpegQualitySpinBox.setValue(75);

  connect(&m_jpegQualitySpinBox, SIGNAL(valueChanged(int)), &m_jpegQualitySlider, SLOT(setValue(int)));
  connect(&m_jpegQualitySlider, SIGNAL(valueChanged(int)), &m_jpegQualitySpinBox, SLOT(setValue(int)));

  m_jpegMainLayout->addLayout(m_jpegTopLayout);
  m_jpegMainLayout->addWidget(&m_jpegRestartIntervalCheckBox);
  m_jpegMainLayout->addWidget(&m_jpegOtimalHuffmanCheckBox);
  m_jpegMainLayout->addWidget(&m_jpegColorComboBox);
  m_jpegMainLayout->addWidget(&m_jpegQualityGroupBox);
  m_jpegMainLayout->addStretch();

// ---------- JPEG2000 ---------------------

  m_jpeg2000ModeGroupBox.setTitle(tr("Mode"));
  m_jpeg2000LossyRadioButton.setText(tr("Lossy"));
  m_jpeg2000LosslessRadioButton.setText(tr("Lossless"));
  m_jpeg2000LosslessRadioButton.setChecked(true);
  m_jpeg2000ModeLayout->addWidget(&m_jpeg2000LosslessRadioButton);
  m_jpeg2000ModeLayout->addWidget(&m_jpeg2000LossyRadioButton);
  m_jpeg2000wtGroupBox.setTitle(tr("WT"));
  m_jpeg2000wt53RadioButton.setText(tr("WT53"));
  m_jpeg2000wt97RadioButton.setText(tr("WT97"));
  m_jpeg2000wt53RadioButton.setChecked(true);
  m_jpeg2000wtLayout->addWidget(&m_jpeg2000wt53RadioButton);
  m_jpeg2000wtLayout->addWidget(&m_jpeg2000wt97RadioButton);
  m_jpeg2000TopLayout->addWidget(&m_jpeg2000ModeGroupBox);
  m_jpeg2000TopLayout->addWidget(&m_jpeg2000wtGroupBox);

  connect(&m_jpeg2000LosslessRadioButton, SIGNAL(clicked()), this, SLOT(updateJpeg2000Buttons()));
  connect(&m_jpeg2000LossyRadioButton, SIGNAL(clicked()), this, SLOT(updateJpeg2000Buttons()));

  m_jpeg2000MCTCheckBox.setChecked(true);
  m_jpeg2000MCTCheckBox.setText(tr("Use MCT"));
  m_jpeg2000RatioGroupBox.setTitle(tr("Quality"));
  m_jpeg2000RatioSlider.setOrientation(Qt::Horizontal);
  m_jpeg2000RatioSlider.setTickInterval(10);
  m_jpeg2000RatioSlider.setRange(0, 100);
  m_jpeg2000RatioSlider.setTickPosition(QSlider::TicksBelow);
  m_jpeg2000RatioSpinBox.setRange(0, 100);
  m_jpeg2000RatioSlider.setValue(0);
  m_jpeg2000RatioLayout->addWidget(&m_jpeg2000RatioSlider);
  m_jpeg2000RatioLayout->addWidget(&m_jpeg2000RatioSpinBox);

  connect(&m_jpeg2000RatioSpinBox, SIGNAL(valueChanged(int)), &m_jpeg2000RatioSlider, SLOT(setValue(int)));
  connect(&m_jpeg2000RatioSlider, SIGNAL(valueChanged(int)), &m_jpeg2000RatioSpinBox, SLOT(setValue(int)));

  m_jpeg2000MainLayout->addLayout(m_jpeg2000TopLayout);
  m_jpeg2000MainLayout->addWidget(&m_jpeg2000MCTCheckBox);
  m_jpeg2000MainLayout->addWidget(&m_jpeg2000RatioGroupBox);
  m_jpeg2000MainLayout->addStretch();

  m_optionsLayout->addWidget(&m_bmpOptionsWidget);
  m_optionsLayout->addWidget(&m_jpegOptionsWidget);
  m_optionsLayout->addWidget(&m_jpeg2000OptionsWidget);
  //m_optionsLayout->addWidget(&m_rawOptionsWidget);
  m_optionsLayout->addWidget(&m_pngOptionsWidget);
  m_optionsLayout->addWidget(&m_jpegxrOptionsWidget);

  m_mainLayout = dynamic_cast<QGridLayout*>(layout());
  if(0 != m_mainLayout)
  {
    int numCols = m_mainLayout->columnCount();
    m_mainLayout->addLayout(m_optionsLayout, 0, numCols, 5, 1);

    m_imageCommentLabel.setText(tr("Comment:"));
    m_mainLayout->addWidget(&m_imageCommentLabel,4,0);

    m_mainLayout->addWidget(&m_imageCommentEdit, 4, 1);
  }

  adjustSize();

  updateJpegButtons();
  updateJpeg2000Buttons();

  connect(this, SIGNAL(rejected()), this, SLOT(OnCancel()));
  connect(this, SIGNAL(accepted()), this, SLOT(OnOk()));
  connect(this, SIGNAL(filterSelected(const QString&)), this, SLOT(filterSelected(const QString&)));

  return 0;
} // FileSaveDialog::Init()


void FileSaveDialog::OnOk(void)
{
  return;
} // FileSaveDialog::OnOk()


void FileSaveDialog::OnCancel(void)
{
  close();
  return;
} // FileSaveDialog::OnCancel()


void FileSaveDialog::updateJpegButtons(void)
{
  bool isLSL = m_jpegLSLRadioButton.isChecked();
  bool isEXT = m_jpegEXTRadioButton.isChecked();

  m_jpeg411RadioButton.setDisabled(isLSL);
  m_jpeg422RadioButton.setDisabled(isLSL);
  m_jpegQualitySlider.setDisabled(isLSL);
  m_jpegQualitySpinBox.setDisabled(isLSL);

  if(isLSL || isEXT ||1 == g_pMainWnd->m_image.NChannels())
  {
    m_jpeg411RadioButton.setDisabled(true);
    m_jpeg422RadioButton.setDisabled(true);
    m_jpeg444RadioButton.setChecked(true);
  }

  return;
} // FileSaveDialog::updateJpegButtons()


void FileSaveDialog::updateJpeg2000Buttons(void)
{
  bool Lossless = m_jpeg2000LosslessRadioButton.isChecked();

  m_jpeg2000wt97RadioButton.setDisabled(Lossless);
  m_jpeg2000RatioSlider.setDisabled(Lossless);
  m_jpeg2000RatioSpinBox.setDisabled(Lossless);

  if (Lossless)
  {
    m_jpeg2000wt97RadioButton.setChecked(false);
    m_jpeg2000wt53RadioButton.setChecked(true);
    m_jpeg2000RatioSlider.setValue(100);
  }

  return;
} // FileSaveDialog::updateJpeg2000Buttons()


void FileSaveDialog::updateJpegxrButtons(void)
{
  bool planar_alpha = !m_jpegxrAlphaCheckBox.isChecked();

  m_jpegxrAQualitySpinBox.setDisabled(planar_alpha);
  m_jpegxrAQualitySlider.setDisabled(planar_alpha);

  return;
} // FileSaveDialog::updateJpegxrButtons()

void FileSaveDialog::filterSelected(const QString& filter)
{
  m_optionsLayout->setCurrentIndex(filters().indexOf(filter));
  return;
} // FileSaveDialog::filterSelected()


int FileSaveDialog::jpegColor(void)
{
  int color;

  switch(m_image->NChannels())
  {
  case 1:
    {
      color = IC_GRAY;
    }
    break;

  case 3:
    {
      if(m_jpegColorComboBox.currentIndex() == 1)
        color = IC_RGB;
      else
        color = IC_YCBCR;
    }
    break;

  case 4:
    {
      if (m_jpegColorComboBox.currentIndex() == 0)
        color = IC_YCCK;
      else if(m_jpegColorComboBox.currentIndex() == 1)
        color = IC_CMYK;
      else
        color = IC_UNKNOWN;
    }
    break;

  default:
    color = IC_UNKNOWN;
  }

  if(m_image->Precision() == 12)
    color = IC_UNKNOWN;

  return color;
} // FileSaveDialog::jpegColor()


int FileSaveDialog::jpegMode(void)
{
  if (m_jpegBASRadioButton.isChecked())
    return JPEG_BASELINE;

  if (m_jpegPRGRadioButton.isChecked())
    return JPEG_PROGRESSIVE;

  if (m_jpegLSLRadioButton.isChecked())
    return JPEG_LOSSLESS;

  if (m_jpegEXTRadioButton.isChecked())
    return JPEG_EXTENDED;

  return JPEG_BASELINE;
} // FileSaveDlg::jpegMode()


int FileSaveDialog::jpegQuality(void)
{
  return m_jpegQualitySlider.value();
} // FileSaveDialog::jpegQuality()


int FileSaveDialog::jpegRestartInterval(void)
{
  if (m_jpegRestartIntervalCheckBox.isChecked())
    return 1;

  return 0;
} // FileSaveDialog::jpegRestartInterval()


int FileSaveDialog::jpegSampling(void)
{
  if (m_jpeg411RadioButton.isChecked())
    return JS_411;

  if (m_jpeg422RadioButton.isChecked())
    return JS_422;

  if (m_jpeg444RadioButton.isChecked())
    return JS_444;

  return JS_444;
} // FileSaveDialog::jpegSampling()


int FileSaveDialog::jpegHuffman(void)
{
  if (m_jpegOtimalHuffmanCheckBox.isChecked())
    return 1;

  return 0;
} // FileSaveDialog::jpegHuffman()


int FileSaveDialog::jpeg2000Ratio(void)
{
  return m_jpeg2000RatioSlider.value();
} // FileSaveDialog::jpeg2000Ratio()


int FileSaveDialog::jpeg2000MCT(void)
{
  if (m_jpeg2000MCTCheckBox.isChecked())
    return 1;

  return 0;
} // FileSaveDialog::jpeg2000MCT()


int FileSaveDialog::jpeg2000Mode(void)
{
  if (m_jpeg2000LosslessRadioButton.isChecked())
    return Lossless;

  return Lossy;
} // FileSaveDialog::jpeg2000Mode()


int FileSaveDialog::jpeg2000wt(void)
{
  if (m_jpeg2000wt53RadioButton.isChecked())
    return WT53;

  return WT97;
} // FileSaveDialog::jpeg2000wt()


int FileSaveDialog::imageComment(char* comment, int len)
{
  int         comment_size;
  QByteArray  arr;

  if(len <= 0)
    return 0;

  arr = m_imageCommentEdit.text().toAscii();

  comment_size = IPP_MIN(len,m_imageCommentEdit.text().length());

  ippsCopy_8u((Ipp8u*)arr.data(), (Ipp8u*)comment, comment_size);

  return comment_size;
} // FileSaveDialog::imageComment()


int FileSaveDialog::imageCommentSize(void)
{
  return m_imageCommentEdit.text().length();
} // FileSaveDialog::imageCommentSize()

int FileSaveDialog::pngFilterSub(void)
{
  return m_pngFilter0CheckBox.isChecked();
} // FileSaveDialog::pngFilterSub()


int FileSaveDialog::pngFilterUp(void)
{
  return m_pngFilter1CheckBox.isChecked();
} // FileSaveDialog::pngFilterUp()


int FileSaveDialog::pngFilterAvg(void)
{
  return m_pngFilter2CheckBox.isChecked();
} // FileSaveDialog::pngFilterAvg()


int FileSaveDialog::pngFilterPaeth(void)
{
  return m_pngFilter3CheckBox.isChecked();
} // FileSaveDialog::pngFilterPaeth()


int FileSaveDialog::jpegxrOverlap(void)
{
  int overlap;

  if(m_jpegxrOverlap0RadioButton.isChecked())
    overlap = 0;
  else if(m_jpegxrOverlap1RadioButton.isChecked())
    overlap = 1;
  else if(m_jpegxrOverlap2RadioButton.isChecked())
    overlap = 2;

  return overlap;
} // FileSaveDialog::jpegxrOverlap()


int FileSaveDialog::jpegxrSampling(void)
{
  int sampling;

  if(m_jpegxrSampling0RadioButton.isChecked())
    sampling = 0;
  else if(m_jpegxrSampling1RadioButton.isChecked())
    sampling = 1;
  else if(m_jpegxrSampling2RadioButton.isChecked())
    sampling = 2;

  return sampling;
} // FileSaveDialog::jpegxrSampling()


int FileSaveDialog::jpegxrBands(void)
{
  int bands;

  if(m_jpegxrBands0RadioButton.isChecked())
    bands = 0;
  else if(m_jpegxrBands1RadioButton.isChecked())
    bands = 1;
  else if(m_jpegxrBands2RadioButton.isChecked())
    bands = 2;
  else if(m_jpegxrBands3RadioButton.isChecked())
    bands = 3;

  return bands;
} // FileSaveDialog::jpegxrBands()


int FileSaveDialog::jpegxrAlpha(void)
{
  return m_jpegxrAlphaCheckBox.isChecked();
} // FileSaveDialog::jpegxrAlpha()


int FileSaveDialog::jpegxrFrequency(void)
{
  int bitstream;

  if(m_jpegxrBitstream0RadioButton.isChecked())
    bitstream = 0;
  else if(m_jpegxrBitstream1RadioButton.isChecked())
    bitstream = 1;

  return bitstream;
} // FileSaveDialog::jpegxrFrequency()


int FileSaveDialog::jpegxrQuality(void)
{
  return m_jpegxrQualitySlider.value();
} // FileSaveDialog::jpegxrQuality()


int FileSaveDialog::jpegxrAQuality(void)
{
  return m_jpegxrAQualitySlider.value();
} // FileSaveDialog::jpegxrAQuality()


int FileSaveDialog::jpegxrFBTrim(void)
{
  return m_jpegxrFBTrimSpinBox.value();
} // FileSaveDialog::jpegxrFBTrim()


int FileSaveDialog::jpegxrTilingMode(void)
{
  int tiling;

  if(m_jpegxrTiling0RadioButton.isChecked())
    tiling = 0;
  else if(m_jpegxrTiling1RadioButton.isChecked())
    tiling = 1;

  return tiling;
} // FileSaveDialog::jpegxrTilingMode()


int FileSaveDialog::jpegxrTilesX(void)
{
  return m_jpegxrTilesXSpinBox.value();
} // FileSaveDialog::jpegxrTilesX()


int FileSaveDialog::jpegxrTilesY(void)
{
  return m_jpegxrTilesYSpinBox.value();
} // FileSaveDialog::jpegxrTilesY()
