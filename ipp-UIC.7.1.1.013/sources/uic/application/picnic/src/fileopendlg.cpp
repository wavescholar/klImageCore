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

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __UIC_BASE_STREAM_INPUT_H__
#include "uic_base_stream_input.h"
#endif
#ifndef __UIC_BASE_STREAM_OUTPUT_H__
#include "uic_base_stream_output.h"
#endif
#ifndef __MEMBUFFIN_H__
#include "membuffin.h"
#endif
#ifndef __STDFILEIN_H__
#include "stdfilein.h"
#endif
#ifndef __PIXMAP_H__
#include "pixmap.h"
#endif
#ifndef __FILEOPENDLG_H__
#include "fileopendlg.h"
#endif


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
    case JPEG_BASELINE:    str = QString(QObject::tr("BAS"));     break;
    case JPEG_EXTENDED:    str = QString(QObject::tr("EXT"));     break;
    case JPEG_PROGRESSIVE: str = QString(QObject::tr("PRG"));     break;
    case JPEG_LOSSLESS:    str = QString(QObject::tr("LSL"));     break;
    default:               str = QString(QObject::tr("Unknown")); break;
  }
  return str;
} // get_str_mode()


static QString get_str_modeJPEG2K(int mode)
{
  QString str;
  switch(mode)
  {
    case Lossless: str = QString(QObject::tr("LSL"));     break;
    case Lossy:    str = QString(QObject::tr("BAS"));     break;
    default:       str = QString(QObject::tr("Unknown")); break;
  }
  return str;
} // get_str_mode()


FileOpenDialog::FileOpenDialog(QWidget* parent, Qt::WindowFlags flags) :
  QFileDialog(parent, flags)
{
  return;
} // ctor


FileOpenDialog::~FileOpenDialog(void)
{
  return;
} // dtor


int FileOpenDialog::Init(const QString& path)
{
  m_imageType    = IT_UNKNOWN;
  m_isRaw        = false;
  m_rawWidth     = 0;
  m_rawHeight    = 0;
  m_rawNChannels = 0;
  m_rawPrecision = 0;
  m_rawOffset    = 0;

  QStringList imageFilters;

  imageFilters.append(tr("All supported image files (*.bmp *.pgm *.pbm *.ppm *.jpg *.jpe *.jpeg *.jp2 *.j2c *.dcm *.png *.isf *.hdp *.jxr *.wdp)"));
  imageFilters.append(tr("Bitmap files (*.bmp)"));
  imageFilters.append(tr("JPEG files (*.jpg *.jpe *.jpeg)"));
  imageFilters.append(tr("JPEG2000 files (*.jp2 *.j2c)"));
  imageFilters.append(tr("DICOM files (*.dcm)"));
  imageFilters.append(tr("PNG files (*.png)"));
  imageFilters.append(tr("Image Search DB files (*.isf)"));
  imageFilters.append(tr("JPEG XR files (*.hdp *.jxr *.wdp)"));
  imageFilters.append(tr("All files (*)"));

  setFilters(imageFilters);
  setAcceptMode(QFileDialog::AcceptOpen);
  setFileMode(QFileDialog::ExistingFiles);

  setDirectory(path);

  m_previewLayout = new QStackedLayout;
  if(0 == m_previewLayout)
    return -1;

  m_openOptionsWidget = new QWidget(this);
  if(0 == m_openOptionsWidget)
    return -1;

  m_openEmptyWidget = new QWidget(this);
  if(0 == m_openEmptyWidget)
    return -1;

  m_openOptionsLayout = new QStackedLayout(m_openOptionsWidget);
  if(0 == m_openOptionsLayout)
    return -1;

  m_imagePreviewWidget = new QWidget(this);
  if(0 == m_imagePreviewWidget)
    return -1;

  m_imagePreviewMainLayout = new QVBoxLayout(m_imagePreviewWidget);
  if(0 == m_imagePreviewMainLayout)
    return -1;

  m_imagePreviewMainLayout->addSpacing(20);
  m_imagePreviewMainLayout->addWidget(&m_showPreviewCheckBox);
  m_showPreviewCheckBox.setText(tr("Show preview"));
  m_showPreviewCheckBox.setChecked(true);
  m_imagePreviewLabel.setMinimumSize(150,150);
  m_imagePreviewMainLayout->addWidget(&m_imagePreviewLabel);
  m_imagePreviewMainLayout->addWidget(&m_imageTextLabel);
  m_imagePreviewMainLayout->addStretch();

  m_openOptionsLayout->addWidget(m_openEmptyWidget);

//----------- RAW ----------------------
  m_rawOptionsGroupBox = new QGroupBox(this);
  if(0 == m_rawOptionsGroupBox)
    return -1;

  m_rawOptionsGroupBox->setFlat(true);

  m_rawOptionsLayout = new QGridLayout(m_rawOptionsGroupBox);
  if(0 == m_rawOptionsLayout)
    return -1;

  m_rawOptionsGroupBox->setTitle(tr("RAW options"));
  m_rawOptionsWidth.setText(tr("width"));
  m_rawOptionsHeight.setText(tr("height"));
  m_rawOptionsNChannels.setText(tr("nchannels"));
  m_rawOptionsPrecision.setText(tr("precision"));
  m_rawOptionsOffset.setText(tr("offset"));

  m_rawOptionsLayout->addWidget(&m_rawOptionsWidth, 0, 0);
  m_rawOptionsLayout->addWidget(&m_rawOptionsHeight, 1, 0);
  m_rawOptionsLayout->addWidget(&m_rawOptionsNChannels, 2, 0);
  m_rawOptionsLayout->addWidget(&m_rawOptionsPrecision, 3, 0);
  m_rawOptionsLayout->addWidget(&m_rawOptionsOffset, 4, 0);
  m_rawOptionsLayout->addWidget(&m_rawWidthEdit, 0, 1);
  m_rawOptionsLayout->addWidget(&m_rawHeightEdit, 1, 1);
  m_rawOptionsLayout->addWidget(&m_rawNChannelsEdit, 2, 1);
  m_rawOptionsLayout->addWidget(&m_rawPrecisionEdit, 3, 1);
  m_rawOptionsLayout->addWidget(&m_rawOffsetEdit, 4, 1);

  m_rawApplyButton.setText(tr("apply"));
  m_rawApplyButton.setFixedSize(50, 30);
  m_rawOptionsGroupBox->setFixedSize(125, 180);

  m_rawOptionsLayout->addWidget(&m_rawApplyButton, 5, 0,1, 2, Qt::AlignCenter);
  m_openOptionsLayout->addWidget(m_rawOptionsGroupBox);

  m_imagePreviewMainLayout->addWidget(m_openOptionsWidget);
  m_previewLayout->addWidget(m_imagePreviewWidget);

  m_mainLayout = dynamic_cast<QGridLayout*>(layout());
  if(0 != m_mainLayout)
  {
    int numCols = m_mainLayout->columnCount();
    m_previewLayout->setCurrentIndex(0);
    m_mainLayout->addLayout(m_previewLayout, 0, numCols, 2, 1);

    m_imageCommentLabel.setText(tr("Comment:"));
    m_mainLayout->addWidget(&m_imageCommentLabel,4,0);

    m_imageCommentEdit.setReadOnly(true);

    QPalette pal = m_imageCommentEdit.palette();
    pal.setColor(QPalette::Normal,QPalette::Base, Qt::lightGray);
    pal.setColor(QPalette::Inactive,QPalette::Base,  Qt::lightGray);
    m_imageCommentEdit.setPalette(pal);

    m_mainLayout->addWidget(&m_imageCommentEdit, 4, 1);
  }

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
  ippsZero_8u((Ipp8u*)&m_param_png,sizeof(m_param_png));
  ippsZero_8u((Ipp8u*)&m_param_jpegxr,sizeof(m_param_jpegxr));
  ippsZero_8u((Ipp8u*)&m_param_raw,sizeof(m_param_raw));

  adjustSize();

  connect(this, SIGNAL(rejected()), this, SLOT(OnCancel()));
  connect(this, SIGNAL(accepted()), this, SLOT(OnOk()));
  connect(this, SIGNAL(currentChanged(const QString&)), this, SLOT(currentChanged(const QString&)));

  connect(this, SIGNAL(filterSelected(const QString&)), this, SLOT(filterSelected(const QString&)));
  connect(&m_rawApplyButton, SIGNAL(clicked()), this, SLOT(rawApply()));

  return 0;
} // FileOpenDialog::Init()


void FileOpenDialog::OnOk(void)
{
  m_rawWidth     = m_rawWidthEdit.text().toInt();
  m_rawHeight    = m_rawHeightEdit.text().toInt();
  m_rawNChannels = m_rawNChannelsEdit.text().toInt();
  m_rawPrecision = m_rawPrecisionEdit.text().toInt();
  m_rawOffset    = m_rawOffsetEdit.text().toInt();
  // TODO: add sampling and color parameters

  return;
} // FileOpenDialog::OnOk()


void FileOpenDialog::OnCancel(void)
{
  close();
  return;
} // FileOpenDialog::OnCancel()


void FileOpenDialog::changeByteOrder(void)
{
  currentChanged(m_fileSelected);
  return;
} // FileOpenDialog::changeEndianess()


void FileOpenDialog::currentChanged(const QString& selected)
{
  bool     bres;
  IM_TYPE  fmt;
  IppiSize roi = { 150, 150 };

  m_imageCommentEdit.clear();
  m_imageComment.clear();

  m_fileSelected = selected;

  if(m_showPreviewCheckBox.isChecked())
  {
    CStdFileInput in;

    if(BaseStream::StatusOk != in.Open(selected.toAscii()))
      return;

    // TODO: GetThumbnailFromXXX should specify desired thumbnail size

    fmt = m_imageDetector.ImageFormat(in);
    m_image.Free();

    if(m_isRaw)
      fmt = IT_UNKNOWN;

    switch(fmt)
    {
      case IT_BMP:      bres = GetThumbnailFromBMP(in, m_image);      break;
      case IT_JPEG:     bres = GetThumbnailFromJPEG(in, m_image);     break;
      case IT_JPEG2000: bres = GetThumbnailFromJPEG2000(in, m_image); break;
      case IT_DICOM:    bres = GetThumbnailFromDICOM(in, m_image);    break;
      case IT_PNG:      bres = GetThumbnailFromPNG(in, m_image);      break;
      case IT_JPEGXR:   bres = GetThumbnailFromJPEGXR(in, m_image);   break;

      default:
        {
          if(m_isRaw)
            bres = GetThumbnailFromRAW(in, m_image);
          else
            bres = true;
        }
        break;
    }

    setOpenOptions(fmt);

    in.Close();

    if(bres)
    {
      m_frame.Free();
      m_frame.Alloc(roi, 1, 8, 0);

      // set backround. Is it better to show some icon?
      ippiSet_8u_C1R(127, m_frame.DataPtr(), m_frame.Step(), m_frame.Size());

      QPixmap pixmap = CreateQPixmap(m_frame);
      m_imagePreviewLabel.setPixmap(pixmap);
      m_imageTextLabel.clear();
      return;
    }

    createFrame(m_image, m_frame);

    QPixmap pixmap = CreateQPixmap(m_frame);

    float xFactor = roi.width  / (float)pixmap.width();
    float yFactor = roi.height / (float)pixmap.height();

    (xFactor < yFactor) ? yFactor = xFactor : xFactor = yFactor;
    if(xFactor > 1 || yFactor > 1)
      xFactor = yFactor = 1;

    QSize roi(pixmap.width() * xFactor, pixmap.height() * yFactor);
    pixmap = pixmap.scaled(roi, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_imagePreviewLabel.setPixmap(pixmap);
    m_imageTextLabel.setText(QString("%1 x %2").arg(m_image.Width()).arg(m_image.Height()));
    m_imageCommentEdit.setText(m_imageComment);
  }
  else
  {
    m_imagePreviewLabel.clear();
    m_imageTextLabel.clear();
  }

  return;
} // FileOpenDialog::currentChanged()


void FileOpenDialog::filterSelected(const QString& filter)
{
  int filterIndex = filters().indexOf(filter);

  m_isRaw = false;

  if(filterIndex == 8)
  {
    m_isRaw = true;
    setOpenOptions(IT_UNKNOWN);
  }
  else
  {
    currentChanged(m_fileSelected);
  }

  return;
} // FileOpenDialog::filterSelected()


void FileOpenDialog::rawApply(void)
{
  m_param_raw.width     = m_rawWidthEdit.text().toInt();
  m_param_raw.height    = m_rawHeightEdit.text().toInt();
  m_param_raw.nchannels = m_rawNChannelsEdit.text().toInt();
  m_param_raw.precision = m_rawPrecisionEdit.text().toInt();
  m_param_raw.offset    = m_rawOffsetEdit.text().toInt();

  m_isRaw = true;

  currentChanged(m_fileSelected);
  return;
} // FileOpenDialog::rawApply()

void FileOpenDialog::setOpenOptions(IM_TYPE fmt)
{
  m_openOptionsLayout->setCurrentIndex(0);

  switch(fmt)
  {
  case IT_UNKNOWN:
    {
      if(m_isRaw)
        m_openOptionsLayout->setCurrentIndex(2);
    }
    break;

  default:
    break;
  }

  return;
} // FileOpenDialog::setOpenOptions()


void FileOpenDialog::createFrame(CIppImage& image, CIppImage& frame)
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

      frame.Format(iFormat);

      if(0 != frame.Alloc(image.Size(), iChannels, iPrecision, 0))
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
      frame.Format(iFormat);

      if(image.Precision() <= 8)
        image.CopyTo    ((Ipp8u*)frame, frame.Step(), image.Size());
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
} // FileOpenDialog::createFrame()


bool FileOpenDialog::GetThumbnailFromBMP(BaseStreamInput& in, CIppImage& image)
{
  int      dstOrder_c3[3] = { 2, 1, 0 };
  int      dstOrder_c4[4] = { 2, 1, 0, 3};
  IM_ERROR err;

  // read bottom-up BMP
  // m_image hold m_image in original bit-depth
  err = ReadImageBMP(in, m_param_bmp, image);
  if(IE_OK != err)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading BMP"),QMessageBox::Ok);
  }

  if(image.NChannels() == 3)
  {
    image.SwapChannels(dstOrder_c3);
    image.Color(IC_RGB);
  }
  else if(image.NChannels() == 4)
  {
    image.SwapChannels(dstOrder_c4);
    image.Color(IC_RGBA);
  }


  m_imageType     = IT_BMP;
  m_imageFormat   = QString("BMP");
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromBMP()

bool FileOpenDialog::GetThumbnailFromJPEG(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  // decode jpeg data
  image.Color(IC_UNKNOWN);

  m_param_jpeg.nthreads  = 1;
  m_param_jpeg.use_qdct  = 1;
  m_param_jpeg.dct_scale = JD_1_1;

  err = ReadImageJPEG(in, m_param_jpeg, image);
  if(IE_OK != err)
  {
    // warn about problems, but try to show what we can
    QMessageBox::critical(
      this,tr("error"),tr("Error when decoding JPEG"),QMessageBox::Ok);
  }

  if(IC_CMYK == image.Color())
  {
    RGBA_FPX_to_RGBA(image,image.Width(),image.Height());
    image.Color(IC_RGBA);
  }


  m_imageType     = IT_JPEG;
  m_imageFormat   = get_str_modeJPEG(m_param_jpeg.mode);
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());
  m_imageComment  = (const char*)m_param_jpeg.comment;

  return false;
} // FileOpenDialog::GetThumbnailFromJPEG()


bool FileOpenDialog::GetThumbnailFromJPEG2000(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  // decode jpeg data
  err = ReadImageJPEG2000(in, m_param_jpeg2k, image);
  if(IE_OK != err)
  {
    // warn about problems, but try to show what we can
    QMessageBox::critical(
      this,tr("error"),tr("Error when decoding JPEG2000"),QMessageBox::Ok);
  }

  m_imageType     = IT_JPEG2000;
  m_imageFormat   = get_str_modeJPEG2K(m_param_jpeg2k.mode);
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromJPEG2000()


bool FileOpenDialog::GetThumbnailFromDICOM(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  // read bottom-up DICOM
  m_param_dicom.param_jpeg.nthreads  = 1;
  m_param_dicom.param_jpeg.use_qdct  = 1;
  m_param_dicom.param_jpeg.dct_scale = JD_1_1;

  err = ReadImageDICOM(in, m_param_dicom, image);
  if(IE_OK != err)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading DICOM"),QMessageBox::Ok);
  }

  m_imageType     = IT_DICOM;
  m_imageFormat   = QString("DCM");
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromDICOM()

bool FileOpenDialog::GetThumbnailFromPNG(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  err = ReadImagePNG(in, m_param_png, image);
  if(IE_OK != err)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading PNG"),QMessageBox::Ok);
  }

  m_imageType     = IT_PNG;
  m_imageFormat   = QString("PNG");
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromPNG()


bool FileOpenDialog::GetThumbnailFromRAW(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  err = ReadImageRAW(in, m_param_raw, image);
  if(IE_OK != err)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading RAW"),QMessageBox::Ok);
  }

  m_imageType     = IT_UNKNOWN;
  m_imageFormat   = QString("UNK");
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromRAW()

bool FileOpenDialog::GetThumbnailFromJPEGXR(BaseStreamInput& in, CIppImage& image)
{
  IM_ERROR err;

  m_param_jpegxr.thread_mode = 2;
  m_param_jpegxr.threads     = 0;
  m_param_jpegxr.bands       = 0;

  err = ReadImageJPEGXR(in, m_param_jpegxr, image);
  if(IE_OK != err)
  {
    QMessageBox::critical(
      this,tr("error"),tr("Error reading JPEGXR"),QMessageBox::Ok);
  }

  m_imageType     = IT_JPEGXR;
  m_imageFormat   = QString("JPERGXR");
  m_imageSampling = get_str_sampling(image.Sampling());
  m_imageColor    = get_str_color(image.Color());

  return false;
} // FileOpenDialog::GetThumbnailFromJPEGXR()
