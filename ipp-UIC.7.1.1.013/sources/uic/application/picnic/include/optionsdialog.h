/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __OPTIONSDIALOG_H__
#define __OPTIONSDIALOG_H__

#include <QtGui/QtGui>

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __HARMONIZATION_H__
#include "harmonization.h"
#endif


#ifndef DEFAULT_MASK_SIZE          // default mask size for Morphology
#define DEFAULT_MASK_SIZE 3
#endif
#ifndef MIN_MASK_SIZE            // minimal mask size for Morphology
#define MIN_MASK_SIZE 1
#endif
#ifndef MAX_MASK_SIZE            // maximal mask size for Morphology
#define MAX_MASK_SIZE 11
#endif

#ifndef DEFAULT_SLIDESHOW_DELAY
#define DEFAULT_SLIDESHOW_DELAY 1
#endif
#ifndef MIN_SLIDESHOW_DELAY
#define MIN_SLIDESHOW_DELAY 0
#endif
#ifndef MAX_SLIDESHOW_DELAY
#define MAX_SLIDESHOW_DELAY 10
#endif

#ifndef DEFAULT_CAP_SIZE          // default cap size for segmentation
#define DEFAULT_CAP_SIZE 15
#endif
#ifndef MIN_CAP_SIZE              // minimal cap size for segmentation
#define MIN_CAP_SIZE 1
#endif
#ifndef MAX_CAP_SIZE              // maximal cap size for segmentation
#define MAX_CAP_SIZE 255
#endif

#ifndef DEFAULT_THREADS_NUMBER    // default cap size for segmentation
#define DEFAULT_THREADS_NUMBER 1
#endif
#ifndef MIN_THREADS_NUMBER        // minimal cap size for segmentation
#define MIN_THREADS_NUMBER 1
#endif
#ifndef MAX_THREADS_NUMBER        // maximal cap size for segmentation
#define MAX_THREADS_NUMBER 50
#endif


#ifndef MIN_INPAINT_RADIUS
#define MIN_INPAINT_RADIUS 1
#endif
#ifndef MAX_INPAINT_RADIUS
#define MAX_INPAINT_RADIUS 20
#endif


class OptionsDialog : public  QDialog
{
  Q_OBJECT

public:
  OptionsDialog(QWidget* parent = 0, Qt::WindowFlags  flags = 0);
  virtual ~OptionsDialog(void);

  QString     osdText(int i)          { return m_osdCheckBox.at(i)->text(); }
  int         osdCount(void)          { return m_osdCheckBox.count(); }
  int         osdSize(void)           { return m_osdSpinBox->value(); }
  bool        osdValue(int i)         { return m_osdCheckBox.at(i)->isChecked(); }

  int         numThreads(void)        { return m_numThreadsSpinBox->value(); }
  int         slideshowValue(void)    { return m_slideshowSlider->value(); }
  int         capValue(void)          { return m_capSlider->value(); }
  int         morphValue(void)        { return m_morphSlider->value(); }
  int         minFaceSizeValue(void)  { return m_minFaceSizeSpinBox->value(); }
  int         maxFaceSizeValue(void)  { return m_maxFaceSizeSpinBox->value(); }
  int         inpaintRadiusValue(void){ return m_radiusSlider->value(); }

  int         c1HarmonizeValue(void)  { return m_c1HarmonizeSpinBox->value(); }
  int         c2HarmonizeValue(void)  { return m_c2HarmonizeSpinBox->value(); }
  int         v3HarmonizeValue(void)  { return m_v3HarmonizeSpinBox->value(); }
  float       v1HarmonizeValue(void)  { return m_v1HarmonizeSpinBox->value(); }
  float       v2HarmonizeValue(void)  { return m_v2HarmonizeSpinBox->value(); }
  FILTER_KERNEL harmonizeKernel(void);

  FILTER_KERNEL sobelKernel(void);

  bool        erodeValue(void)        { return m_erodeRadioButton->isChecked(); }
  bool        distanceValue(void)     { return m_distanceRadioButton->isChecked(); }
  bool        gradientValue(void)     { return m_gradientRadioButton->isChecked(); }
  float       scaleFactorValue(void)  { return m_scaleFactorSpinBox->value(); }
  void        setMaxFaceSizeValue(int value) { m_maxFaceSizeSpinBox->setValue(value); return; }

  int         inpaintModeValue(void);
  int         inpaintAlgValue(void);
  int         slideshowEffectValue(void);
  int         pruningTypeValue(void);
  int         normOptionsValue(void);
  int         bounindValue(void);

  int         dctScaleValue(void);
  bool        useQDctValue(void)      { return m_dctPropertiesBox->isChecked();}
  int         j2kArithmeticsValue(void);

private slots:
  void        listItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
  QSpinBox*              m_numThreadsSpinBox;
  QSpinBox*              m_osdSpinBox;
  QSpinBox*              m_minFaceSizeSpinBox;
  QSpinBox*              m_maxFaceSizeSpinBox;
  QSpinBox*              m_c1HarmonizeSpinBox;
  QSpinBox*              m_c2HarmonizeSpinBox;
  QSpinBox*              m_v3HarmonizeSpinBox;
  QDoubleSpinBox*        m_v1HarmonizeSpinBox;
  QDoubleSpinBox*        m_v2HarmonizeSpinBox;
  QComboBox*             m_harmonizeKernel;
  QDoubleSpinBox*        m_scaleFactorSpinBox;

  QComboBox*             m_sobelKernel;

  QSlider*               m_slideshowSlider;
  QSlider*               m_capSlider;
  QSlider*               m_morphSlider;
  QSlider*               m_dicomSlider;
  QSlider*               m_radiusSlider;

  QRadioButton*          m_erodeRadioButton;
  QRadioButton*          m_distanceRadioButton;
  QRadioButton*          m_gradientRadioButton;

  QList<QRadioButton*>   m_modeIpRadioButton;
  QList<QRadioButton*>   m_ippIpRadioButton;
  QList<QRadioButton*>   m_slideshowEffectRadioButton;
  QList<QRadioButton*>   m_pruningRadioButton;
  QList<QRadioButton*>   m_normRadioButton;
  QList<QRadioButton*>   m_bounindRadioButton;
  QList<QWidget*>        m_optionsWidgetList;
  QList<QCheckBox*>      m_osdCheckBox;
  QList<QRadioButton*>   m_dctScaleRadioButton;
  QList<QRadioButton*>   m_j2kArithmPrecisionRadioButton;

  QCheckBox*             m_dctPropertiesBox;
  QLabel*                m_osdDescLabel;
};

#endif
