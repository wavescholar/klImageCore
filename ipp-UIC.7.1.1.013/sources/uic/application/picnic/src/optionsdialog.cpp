/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#include "constants.h"
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __OPTIONSDIALOG_H__
#include "optionsdialog.h"
#endif


OptionsDialog::OptionsDialog(QWidget* parent, Qt::WindowFlags  flags) :
  QDialog(parent, flags)
{
  setWindowModality(Qt::ApplicationModal);
  setWindowTitle(tr("Options"));
  setFixedSize(620, 540);

  QGridLayout* optionsLayout = new QGridLayout(this);
  QGroupBox* optionsBox = new QGroupBox();
  optionsBox->setMinimumWidth(450);

  QPushButton* applyButton = new QPushButton(tr("&Apply"), this);
  QPushButton* cancelButton = new QPushButton(tr("&Cancel"), this);
  applyButton->setDefault(true);
  connect(applyButton, SIGNAL(clicked()), this->parent(), SLOT(setOptions()));
  connect(applyButton, SIGNAL(clicked()), this, SLOT(hide()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));

  QListWidget* listWidget = new QListWidget();

  QList<QListWidgetItem*> listItems;
  QList<QGridLayout*>     optionsGrid;


  listItems.append(new QListWidgetItem(tr("Slideshow"), 0, 1000));
  listItems.append(new QListWidgetItem(tr("Segmentation"), 0, 1001));
  listItems.append(new QListWidgetItem(tr("Harmonization"), 0, 1002));
  listItems.append(new QListWidgetItem(tr("Sobel"), 0, 1003));
  listItems.append(new QListWidgetItem(tr("Face Detection"), 0, 1004));
  listItems.append(new QListWidgetItem(tr("Inpainting"), 0, 1005));
  listItems.append(new QListWidgetItem(tr("Global"), 0, 1006));
  listItems.append(new QListWidgetItem(tr("JPEG Options"), 0, 1007));
  listItems.append(new QListWidgetItem(tr("JPEG2000 Options"), 0, 1008));


  for(int i = 0; i < listItems.count(); i++)
    m_optionsWidgetList.append(new QWidget(optionsBox));

  //////////////////////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(0)));
  QGroupBox*   slideshowDelayBox    = new QGroupBox(tr("Slideshow interval"), this);
  QHBoxLayout* slideshowDelayLayout = new QHBoxLayout(slideshowDelayBox);
  QSpinBox*    slideshowSpinBox     = new QSpinBox(this);

  m_slideshowSlider = new QSlider(Qt::Horizontal, this);
  m_slideshowSlider->setRange(MIN_SLIDESHOW_DELAY, MAX_SLIDESHOW_DELAY);
  m_slideshowSlider->setTickInterval(1);
  m_slideshowSlider->setTickPosition(QSlider::TicksBelow);

  slideshowSpinBox->setRange(MIN_SLIDESHOW_DELAY, MAX_SLIDESHOW_DELAY);
  connect(slideshowSpinBox, SIGNAL(valueChanged(int)), m_slideshowSlider, SLOT(setValue(int)));
  connect(m_slideshowSlider, SIGNAL(valueChanged(int)), slideshowSpinBox, SLOT(setValue(int)));

  m_slideshowSlider->setValue(DEFAULT_SLIDESHOW_DELAY);
  slideshowDelayLayout->addWidget(m_slideshowSlider);
  slideshowDelayLayout->addWidget(slideshowSpinBox);

  QGroupBox*   slideshowEffectBox    = new QGroupBox(tr("Slideshow effect"), this);
  QVBoxLayout* slideshowEffectLayout = new QVBoxLayout(slideshowEffectBox);

  m_slideshowEffectRadioButton.append(new QRadioButton(tr("None"), this));
  m_slideshowEffectRadioButton.append(new QRadioButton(tr("Alpha blending"), this));
  m_slideshowEffectRadioButton.at(1)->setChecked(true);

  foreach(QRadioButton* radioButton, m_slideshowEffectRadioButton)
    slideshowEffectLayout->addWidget(radioButton);

  optionsGrid.at(0)->addWidget(slideshowDelayBox, 0, 0);
  optionsGrid.at(0)->addWidget(slideshowEffectBox, 1, 0);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(1)));

  QGroupBox*   capGroupBox = new QGroupBox(tr("Cap size"), this);
  QHBoxLayout* capLayout   = new QHBoxLayout(capGroupBox);
  QSpinBox*    capSpinBox  = new QSpinBox(this);

  m_capSlider = new QSlider(Qt::Horizontal, this);
  m_capSlider->setTickInterval((MAX_CAP_SIZE - MIN_CAP_SIZE) / 10);
  m_capSlider->setRange(MIN_CAP_SIZE, MAX_CAP_SIZE);
  m_capSlider->setTickPosition(QSlider::TicksBelow);
  capSpinBox->setRange(MIN_CAP_SIZE, MAX_CAP_SIZE);
  connect(capSpinBox, SIGNAL(valueChanged(int)), m_capSlider, SLOT(setValue(int)));
  connect(m_capSlider, SIGNAL(valueChanged(int)), capSpinBox, SLOT(setValue(int)));
  m_capSlider->setValue(DEFAULT_CAP_SIZE);
  capLayout->addWidget(m_capSlider);
  capLayout->addWidget(capSpinBox);

  QGroupBox*   morphGroupBox = new QGroupBox(tr("Mask size"), this);
  QHBoxLayout* morphLayout   = new QHBoxLayout(morphGroupBox);
  QSpinBox*    morphSpinBox  = new QSpinBox(this);
  m_morphSlider = new QSlider(Qt::Horizontal, this);
  m_morphSlider->setTickInterval(1);
  m_morphSlider->setRange(MIN_MASK_SIZE, MAX_MASK_SIZE);
  m_morphSlider->setTickPosition(QSlider::TicksBelow);
  morphSpinBox->setRange(MIN_MASK_SIZE, MAX_MASK_SIZE);
  connect(morphSpinBox, SIGNAL(valueChanged(int)), m_morphSlider, SLOT(setValue(int)));
  connect(m_morphSlider, SIGNAL(valueChanged(int)), morphSpinBox, SLOT(setValue(int)));
  m_morphSlider->setValue(DEFAULT_MASK_SIZE);
  morphLayout->addWidget(m_morphSlider);
  morphLayout->addWidget(morphSpinBox);

  QGroupBox*    erodeGroupBox     = new QGroupBox(tr("Reconstruction by"), this);
  QVBoxLayout*  erodeLayout       = new QVBoxLayout(erodeGroupBox);
  QRadioButton* dilateRadioButton = new QRadioButton(tr("dilate"), this);
  m_erodeRadioButton = new QRadioButton(tr("erode"), this);
  dilateRadioButton->setChecked(true);
  erodeLayout->addWidget(m_erodeRadioButton);
  erodeLayout->addWidget(dilateRadioButton);

  QGroupBox*    distanceGroupBox = new QGroupBox(tr("Watershed by"), this);
  QVBoxLayout*  distanceLayout   = new QVBoxLayout(distanceGroupBox);
  QRadioButton* queueRadioButton = new QRadioButton(tr("queue"), this);
  m_distanceRadioButton = new QRadioButton(tr("distance"), this);
  m_distanceRadioButton->setChecked(true);
  distanceLayout->addWidget(m_distanceRadioButton);
  distanceLayout->addWidget(queueRadioButton);

  QGroupBox*    gradientGroupBox   = new QGroupBox(tr("Image"), this);
  QVBoxLayout*  gradientLayout     = new QVBoxLayout(gradientGroupBox);
  QRadioButton* initialRadioButton = new QRadioButton(tr("initial"), this);
  m_gradientRadioButton = new QRadioButton(tr("gradient"), this);
  initialRadioButton->setChecked(true);
  gradientLayout->addWidget(m_gradientRadioButton);
  gradientLayout->addWidget(initialRadioButton);

  QGroupBox*   normGroupBox = new QGroupBox(tr("Options"), this);
  QVBoxLayout* normLayout   = new QVBoxLayout(normGroupBox);
  m_normRadioButton.append(new QRadioButton(tr("ippiNormInf + 4-connectivity"), this));
  m_normRadioButton.append(new QRadioButton(tr("ippiNormL1 + 4-connectivity"), this));
  m_normRadioButton.append(new QRadioButton(tr("ippiNormL2 + 4-connectivity"), this));
  m_normRadioButton.append(new QRadioButton(tr("ippiNormFM + 4-connectivity"), this));
  m_normRadioButton.at(0)->setChecked(true);
  foreach(QRadioButton* radioButton, m_normRadioButton)
    normLayout->addWidget(radioButton);

  QGroupBox*   bounindGroupBox = new QGroupBox(tr("Bounds"), this);
  QVBoxLayout* bounindLayout   = new QVBoxLayout(bounindGroupBox);

  m_bounindRadioButton.append(new QRadioButton(tr("4-bounds on"), this));
  m_bounindRadioButton.append(new QRadioButton(tr("8-bounds on"), this));
  m_bounindRadioButton.append(new QRadioButton(tr("internal 4-bounds on"), this));
  m_bounindRadioButton.append(new QRadioButton(tr("internals 8-bound on"), this));
  m_bounindRadioButton.append(new QRadioButton(tr("4-bounds off"), this));
  m_bounindRadioButton.append(new QRadioButton(tr("8-bounds off"), this));
  m_bounindRadioButton.at(0)->setChecked(true);

  foreach(QRadioButton* radioButton, m_bounindRadioButton)
    bounindLayout->addWidget(radioButton);

  optionsGrid.at(1)->addWidget(capGroupBox, 0, 0);
  optionsGrid.at(1)->addWidget(morphGroupBox, 1, 0);
  optionsGrid.at(1)->addWidget(erodeGroupBox, 0, 1);
  optionsGrid.at(1)->addWidget(distanceGroupBox, 1, 1);
  optionsGrid.at(1)->addWidget(gradientGroupBox, 2, 1);
  optionsGrid.at(1)->addWidget(normGroupBox, 3, 0);
  optionsGrid.at(1)->addWidget(bounindGroupBox, 3, 1);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(2)));
  QGroupBox*   harmonizeGroupBox = new QGroupBox(tr("Parameters"), this);
  QVBoxLayout* harmonizeLayout   = new QVBoxLayout(harmonizeGroupBox);
  m_c1HarmonizeSpinBox = new QSpinBox(this);
  m_c1HarmonizeSpinBox->setRange(0, 65535);
  m_c1HarmonizeSpinBox->setValue(0);
  m_c1HarmonizeSpinBox->setPrefix(tr("thrsh low:  "));

  m_c2HarmonizeSpinBox = new QSpinBox(this);
  m_c2HarmonizeSpinBox->setRange(0, 65535);
  m_c2HarmonizeSpinBox->setValue(32767);
  m_c2HarmonizeSpinBox->setPrefix(tr("thrsh high: "));

  m_v1HarmonizeSpinBox = new QDoubleSpinBox(this);
  m_v1HarmonizeSpinBox->setRange(0.1f, 3.0f);
  m_v1HarmonizeSpinBox->setValue(0.1f);
  m_v1HarmonizeSpinBox->setSingleStep(0.1f);
  m_v1HarmonizeSpinBox->setPrefix(tr("hfactor:    "));

  m_v2HarmonizeSpinBox = new QDoubleSpinBox(this);
  m_v2HarmonizeSpinBox->setRange(-20.0f, 20.0f);
  m_v2HarmonizeSpinBox->setValue(1.11f);
  m_v2HarmonizeSpinBox->setSingleStep(0.01f);
  m_v2HarmonizeSpinBox->setPrefix(tr("gain:       "));

  m_v3HarmonizeSpinBox = new QSpinBox(this);
  m_v3HarmonizeSpinBox->setRange(0, 32768);
  m_v3HarmonizeSpinBox->setValue(3);
  m_v3HarmonizeSpinBox->setSingleStep(1);
  m_v3HarmonizeSpinBox->setPrefix(tr("offset:     "));

  m_harmonizeKernel = new QComboBox(this);
  m_harmonizeKernel->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  m_harmonizeKernel->addItem(QString("3x3"));
  m_harmonizeKernel->addItem(QString("5x5"));
  m_harmonizeKernel->addItem(QString("7x7"));
  m_harmonizeKernel->addItem(QString("9x9"));
  m_harmonizeKernel->setCurrentIndex(0);

  harmonizeLayout->addWidget(m_c1HarmonizeSpinBox);
  harmonizeLayout->addWidget(m_c2HarmonizeSpinBox);
  harmonizeLayout->addWidget(m_v1HarmonizeSpinBox);
  harmonizeLayout->addWidget(m_v2HarmonizeSpinBox);
  harmonizeLayout->addWidget(m_v3HarmonizeSpinBox);
  harmonizeLayout->addWidget(m_harmonizeKernel);
  optionsGrid.at(2)->addWidget(harmonizeGroupBox, 0, 0);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(3)));
  QGroupBox*   sobelGroupBox = new QGroupBox(tr("Parameters"), this);
  QVBoxLayout* sobelLayout   = new QVBoxLayout(sobelGroupBox);

  m_sobelKernel = new QComboBox(this);
  m_sobelKernel->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  m_sobelKernel->addItem(QString("3x3"));
  m_sobelKernel->addItem(QString("5x5"));
  m_sobelKernel->setCurrentIndex(0);

  sobelLayout->addWidget(m_sobelKernel);
  optionsGrid.at(3)->addWidget(sobelGroupBox, 0, 0);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(4)));
  QGroupBox*   pruningGroupBox = new QGroupBox(tr("Pruning types"), this);
  QVBoxLayout* pruningLayout   = new QVBoxLayout(pruningGroupBox);
  m_pruningRadioButton.append(new QRadioButton(tr("RowPruning"), this));
  m_pruningRadioButton.append(new QRadioButton(tr("ColPruning"), this));
  m_pruningRadioButton.append(new QRadioButton(tr("RowColMixPruning"), this));
  m_pruningRadioButton.at(2)->setChecked(true);

  foreach(QRadioButton* radioButton, m_pruningRadioButton)
    pruningLayout->addWidget(radioButton);

  QGroupBox*   scaleFactorGroupBox = new QGroupBox(tr("Scale factor"), this);
  QVBoxLayout* scaleFactorLayout   = new QVBoxLayout(scaleFactorGroupBox);
  m_scaleFactorSpinBox = new QDoubleSpinBox(this);
  m_scaleFactorSpinBox->setRange(1.1f, 2.0f);
  m_scaleFactorSpinBox->setValue(1.1f);
  m_scaleFactorSpinBox->setSingleStep(0.1f);
  scaleFactorLayout->addWidget(m_scaleFactorSpinBox);

  QGroupBox*   faceSizeGroupBox = new QGroupBox(tr("Min face size"), this);
  QVBoxLayout* faceSizeLayout   = new QVBoxLayout(faceSizeGroupBox);
  m_minFaceSizeSpinBox = new QSpinBox(this);
  m_minFaceSizeSpinBox->setRange(5, 100);
  m_minFaceSizeSpinBox->setValue(37);
  m_minFaceSizeSpinBox->setPrefix(tr("min: "));
  m_maxFaceSizeSpinBox = new QSpinBox(this);
  m_maxFaceSizeSpinBox->setRange(0, 100);
  m_maxFaceSizeSpinBox->setValue(0);
  m_maxFaceSizeSpinBox->setPrefix(tr("max: "));
  faceSizeLayout->addWidget(m_minFaceSizeSpinBox);
  faceSizeLayout->addWidget(m_maxFaceSizeSpinBox);

  optionsGrid.at(4)->addWidget(pruningGroupBox, 0, 0);
  optionsGrid.at(4)->addWidget(scaleFactorGroupBox, 1, 0);
  optionsGrid.at(4)->addWidget(faceSizeGroupBox, 2, 0);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(5)));
  QGroupBox* radiusGroupBox = new QGroupBox(tr("Inpaint range"), this);
  QHBoxLayout* radiusLayout = new QHBoxLayout(radiusGroupBox);
  QSpinBox* radiusSpinBox = new QSpinBox(this);
  m_radiusSlider = new QSlider(Qt::Horizontal, this);
  m_radiusSlider->setTickInterval((MAX_INPAINT_RADIUS - MIN_INPAINT_RADIUS) / 10);
  m_radiusSlider->setRange(MIN_INPAINT_RADIUS, MAX_INPAINT_RADIUS);
  m_radiusSlider->setTickPosition(QSlider::TicksBelow);
  radiusSpinBox->setRange(MIN_INPAINT_RADIUS, MAX_INPAINT_RADIUS);
  radiusLayout->addWidget(m_radiusSlider);
  radiusLayout->addWidget(radiusSpinBox);
  connect(m_radiusSlider, SIGNAL(valueChanged(int)), radiusSpinBox, SLOT(setValue(int)));
  connect(radiusSpinBox, SIGNAL(valueChanged(int)), m_radiusSlider, SLOT(setValue(int)));
  m_radiusSlider->setValue(3);

  QGroupBox* modeGroupBox = new QGroupBox(tr("Flag"), this);
  QVBoxLayout* modeLayout = new QVBoxLayout(modeGroupBox);
  m_modeIpRadioButton.append(new QRadioButton(tr("Telea"), this));
  m_modeIpRadioButton.append(new QRadioButton(tr("Navier-Stokes"), this));
  m_modeIpRadioButton.at(0)->setChecked(true);
  foreach(QRadioButton* radioButton, m_modeIpRadioButton)
    modeLayout->addWidget(radioButton);

  QGroupBox* algGroupBox = new QGroupBox(tr("Algorithm"), this);
  QVBoxLayout* algLayout = new QVBoxLayout(algGroupBox);
  m_ippIpRadioButton.append(new QRadioButton(tr("full ipp algorithm"), this));
  m_ippIpRadioButton.append(new QRadioButton(tr("simple ipp algorithm"), this));
  m_ippIpRadioButton.at(0)->setChecked(true);
  foreach(QRadioButton* radioButton, m_ippIpRadioButton)
    algLayout->addWidget(radioButton);

  QGroupBox* optGroupBox = new QGroupBox(tr("Options"), this);
  QHBoxLayout* optLayout = new QHBoxLayout(optGroupBox);
  QPushButton* loadMaskButton = new QPushButton(tr("&Load mask"), this);
  QPushButton* saveMaskButton = new QPushButton(tr("&Save mask"), this);
  optLayout->addWidget(loadMaskButton);
  optLayout->addWidget(saveMaskButton);
  connect(loadMaskButton, SIGNAL(clicked()), parent, SLOT(loadMask()));
  connect(saveMaskButton, SIGNAL(clicked()), parent, SLOT(saveMask()));

  optionsGrid.at(5)->addWidget(radiusGroupBox, 0, 0);
  optionsGrid.at(5)->addWidget(modeGroupBox, 1, 0);
  optionsGrid.at(5)->addWidget(algGroupBox, 2, 0);
  optionsGrid.at(5)->addWidget(optGroupBox, 3, 0);

  /////////

  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(6)));
  QGroupBox*   osdBox    = new QGroupBox(tr("OSD"), this);
  QVBoxLayout* osdLayout = new QVBoxLayout(osdBox);
  m_osdSpinBox = new QSpinBox(this);
  m_osdDescLabel = new QLabel(this);
  m_osdCheckBox.append(new QCheckBox("threads", this));
  m_osdCheckBox.append(new QCheckBox("image I/O", this));
  m_osdCheckBox.append(new QCheckBox("processing", this));
  m_osdCheckBox.append(new QCheckBox("rendering", this));

  foreach(QCheckBox* checkBox, m_osdCheckBox)
    osdLayout->addWidget(checkBox);

  m_osdDescLabel->setText(tr("font size"));
  osdLayout->addWidget(m_osdDescLabel);

  m_osdSpinBox->setRange(1, 20);
  m_osdSpinBox->setValue(15);
  osdLayout->addWidget(m_osdSpinBox);

  QGroupBox*   nthreadBox    = new QGroupBox(tr("Num of threads"), this);
  QVBoxLayout* nthreadLayout = new QVBoxLayout(nthreadBox);

  int nthreads;
  ippGetNumThreads(&nthreads);

  m_numThreadsSpinBox = new QSpinBox(this);
  m_numThreadsSpinBox->setRange(1,8);
  m_numThreadsSpinBox->setSingleStep(1);
  m_numThreadsSpinBox->setValue(nthreads);

  nthreadLayout->addWidget(m_numThreadsSpinBox);

  optionsGrid.at(6)->addWidget(osdBox, 0, 0);
  optionsGrid.at(6)->addWidget(nthreadBox, 2, 0);

  //////////////////////////
  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(7)));
  QGroupBox*   jpegOptionsBox    = new QGroupBox(tr("DCT DownSampling Factor"), this);
  QVBoxLayout* jpegOptionsLayout = new QVBoxLayout(jpegOptionsBox);

  m_dctScaleRadioButton.append(new QRadioButton(tr("1/1"), this));
  m_dctScaleRadioButton.append(new QRadioButton(tr("1/2"), this));
  m_dctScaleRadioButton.append(new QRadioButton(tr("1/4"), this));
  m_dctScaleRadioButton.append(new QRadioButton(tr("1/8"), this));

  m_dctScaleRadioButton.at(0)->setChecked(true);

  foreach(QRadioButton* radioButton, m_dctScaleRadioButton)
    jpegOptionsLayout->addWidget(radioButton);

  QGroupBox*   jpegDCTBox    = new QGroupBox(tr("DCT Properties"), this);
  QVBoxLayout* jpegDCTLayout = new QVBoxLayout(jpegDCTBox);

  m_dctPropertiesBox = new QCheckBox(tr("Use simple IDCT for not complete blocks"),this);
  m_dctPropertiesBox->setChecked(true);

  jpegDCTLayout->addWidget(m_dctPropertiesBox);

  optionsGrid.at(7)->addWidget(jpegOptionsBox, 0, 0);
  optionsGrid.at(7)->addWidget(jpegDCTBox, 1, 0);

  //////////////////////////
  //JPEG2000 options
  optionsGrid.append(new QGridLayout(m_optionsWidgetList.at(8)));
  QGroupBox*   j2kOptionsBox    = new QGroupBox(tr("JPEG 2000 arithmetic"), this);
  QVBoxLayout* j2kOptionsLayout = new QVBoxLayout(j2kOptionsBox);

  m_j2kArithmPrecisionRadioButton.append(new QRadioButton(tr("32 bit"), this));
  m_j2kArithmPrecisionRadioButton.append(new QRadioButton(tr("16 bit"), this));

  m_j2kArithmPrecisionRadioButton.at(0)->setChecked(true);

  foreach(QRadioButton* radioButton, m_j2kArithmPrecisionRadioButton)
    j2kOptionsLayout->addWidget(radioButton);

  optionsGrid.at(8)->addWidget(j2kOptionsBox, 0, 0);

  /////////////////////////

  foreach(QWidget* widget, m_optionsWidgetList)
    widget->hide();

  foreach(QListWidgetItem* item, listItems)
    listWidget->insertItem((item->type()-1000), item);

  optionsLayout->addWidget(optionsBox, 0, 1);
  optionsLayout->addWidget(listWidget, 0, 0);
  optionsLayout->addWidget(applyButton, 1, 0);
  optionsLayout->addWidget(cancelButton, 2, 0);

  connect(
    listWidget,
    SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(listItemChanged(QListWidgetItem*, QListWidgetItem*)));

  return;
} // ctor


OptionsDialog::~OptionsDialog(void)
{
  return;
} // dtor


void OptionsDialog::listItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
  if(0 != previous)
    m_optionsWidgetList.at(previous->type()-1000)->hide();

  m_optionsWidgetList.at(current->type()-1000)->show();

  return;
} // OptionsDialog::listItemChanged()


FILTER_KERNEL fk[] =
{
  KERNEL_3x3,
  KERNEL_5x5,
  KERNEL_7x7,
  KERNEL_9x9
};

FILTER_KERNEL OptionsDialog::harmonizeKernel(void)
{
  FILTER_KERNEL filter_kernel;

  switch(m_harmonizeKernel->currentIndex())
  {
  case 3:  filter_kernel = KERNEL_9x9; break;
  case 2:  filter_kernel = KERNEL_7x7; break;
  case 1:  filter_kernel = KERNEL_5x5; break;
  case 0:
  default: filter_kernel = KERNEL_3x3; break;
  }

  return filter_kernel;
} // OptionsDialog::harmonizeKernel()


FILTER_KERNEL OptionsDialog::sobelKernel(void)
{
  FILTER_KERNEL filter_kernel;

  switch(m_sobelKernel->currentIndex())
  {
  case 1:  filter_kernel = KERNEL_5x5; break;
  case 0:
  default: filter_kernel = KERNEL_3x3; break;
  }

  return filter_kernel;
} // OptionsDialog::sobelKernel()


int OptionsDialog::normOptionsValue(void)
{
  for(int i = 0; i < m_normRadioButton.count(); i++)
    if(m_normRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::normOptionsValue()


int OptionsDialog::bounindValue(void)
{
  for(int i = 0; i < m_bounindRadioButton.count(); i++)
    if(m_bounindRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::bounindValue()


int OptionsDialog::slideshowEffectValue(void)
{
  for(int i = 0; i < m_slideshowEffectRadioButton.count(); i++)
    if(m_slideshowEffectRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::slideshowEffectValue()


int OptionsDialog::pruningTypeValue(void)
{
  for(int i = 0; i < m_pruningRadioButton.count(); i++)
    if(m_pruningRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::pruningTypeValue()


int OptionsDialog::inpaintAlgValue(void)
{
  for(int i = 0; i < m_ippIpRadioButton.count(); i++)
    if(m_ippIpRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::inpaintOptValue()


int OptionsDialog::inpaintModeValue(void)
{
  for(int i = 0; i < m_modeIpRadioButton.count(); i++)
    if(m_modeIpRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::inpaintModeValue()


int OptionsDialog::dctScaleValue(void)
{
  for(int i = 0; i < m_dctScaleRadioButton.count(); i++)
    if(m_dctScaleRadioButton.at(i)->isChecked())
      return i;

  return -1;
} // OptionsDialog::dctScaleValue()


int OptionsDialog::j2kArithmeticsValue(void)
{
  for(int i = 0; i < m_j2kArithmPrecisionRadioButton.count(); i++)
    if(m_j2kArithmPrecisionRadioButton.at(i)->isChecked())
      return i;

  return - 1;
} // j2kArithmeticsValue()

