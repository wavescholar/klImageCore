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
#define __MAINWINDOW_H__

#include <QtGui/QtGui>

#ifndef __CONSTANTS_H__
#include "constants.h"
#endif
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __TIMER_H__
#include "timer.h"
#endif
#ifndef __STDFILEIN_H__
#include "stdfilein.h"
#endif
#ifndef __DETECTOR_H__
#include "detector.h"
#endif
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
#ifndef __SEGMENTATION_H__
#include "segmentation.h"
#endif
#ifndef __FACEDETECTION_H__
#include "facedetection.h"
#endif
#ifndef __HARMONIZATION_H__
#include "harmonization.h"
#endif
#ifndef __INPAINTING_H__
#include "inpainting.h"
#endif
#ifndef __SOBEL_H__
#include "sobel.h"
#endif
#ifndef __IMAGEWIDGET_H__
#include "imagewidget.h"
#endif
#ifndef __IMAGEFEADB_H__
#include "imagefeadb.h"
#endif


#define MAXRECENTFILES 10

using namespace UIC;
using namespace ISF;

class MainWindow;
class FileSaveDialog;
class OptionsDialog;

typedef struct
{
  float cpe_iio;
  float sec_iio;

  float cpe_prc;
  float sec_prc;

  float cpe_rnd;
  float sec_rnd;

} OSD_VALS;


extern MainWindow* g_pMainWnd;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0) :
    QMainWindow(parent, flags) {}
  virtual ~MainWindow(void) { Free(); }

  int Init(void);
  int Free(void);

public:
  ImageWidget* createLabel(CIppImage& thumbnail, IppiSize originalSize, int id);
  void         setupPainter(QPainter& painter, bool isSolid = true);
  void         updateView(bool drawOSD = false, OSD_VALS* osd = 0);
  void         cleanStatusBar(void);
  void         prepareForThumbs(void);
  bool         setCurrentFolder(void);
  bool         setCurrentFile(const QString& fileName, bool updateStatus = true);

  bool         GetImageFromBMP(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromJPEG(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromJPEG2000(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromDICOM(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromRAW(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromPNG(BaseStreamInput& in, CIppImage& image);
  bool         GetImageFromJPEGXR(BaseStreamInput& in, CIppImage& image);

  void         createFrame(CIppImage& image, CIppImage& frame);

private:
  bool        openFile(const QString& fileName);
  bool        SaveAsBMP(CIppImage& src, QString& fileName);
  bool        SaveAsJPEG(CIppImage& src, QString& fileName);
  bool        SaveAsJPEG2000(CIppImage& src, QString& fileName);
  bool        SaveAsRAW(CIppImage& src, QString& fileName);
  bool        SaveAsPNG(CIppImage& src, QString& fileName);
  bool        SaveAsJPEGXR(CIppImage& src, QString& fileName);

  void        scanFolder(const QString& fileName);
  void        createStatusBar(void);
  void        createMenus(void);
  void        createToolBars(void);
  void        createActions(void);
  void        setup(void);
  void        updateRecentFileActions(void);
  void        createDocks(void);
  void        stopSlideShow(void);

  void        segmentation(void);
  void        inpainting(void);
  void        facedetection(void);
  void        harmonization(void);
  void        sobel(void);

private slots:
  bool        startPlayDICOM(void);
  bool        playDICOM(void);
  bool        playDICOM(int);
  void        OnFileOpen(void);
  void        OnMRUFileOpen(void);
  void        OnFileSave(void);
  bool        OnImageDbSave(void);
  bool        OnNext(void);
  bool        OnPrevious(void);
  bool        slideshow(void);
  bool        effectAlpha(bool updateStatus = true);
  bool        runEffectAlpha(void);
  bool        circle(void);
  void        OnAbout(void);
  void        options(void);
  void        setOptions(void);
  void        ProgressShow(int min, int max, QString title);
  void        ProgressHide(void);
  void        ProgressUpdate(int value);

  void        createMask(void);
  void        loadMask(void);
  void        saveMask(void);

  void        updateStatusBar(
                QString  filefmt   = "",
                int      width     = -1,
                int      height    = -1,
                int      nchannels = -1,
                int      precision = -1,
                QString  sampling  = "",
                QString  color     = "",
                float    cpe       = -1,  //  if -2 -> no changes
                float    usec      = -1); //  if -2 -> no changes

  void        OnSegmentation(void);
  void        OnInpainting(void);
  void        OnFaceDetection(void);
  void        OnHarmonization(void);
  void        OnSobel(void);

  // Image Search functionality
  void        OnSearchByColorLayout(void);
  void        OnSearchByEdgeHistogram(void);
  void        OnSearch(FeatureId featureId);
  void        OnExtractAll(void);

  void        restore(void);

  void        vBarScrolled(int value);

  void        closeEvent(QCloseEvent* event);

public slots:
  void        resizeEvent(QResizeEvent* event);
  void        updateMenus(void);
  QSize       fitImageSize(void);

protected:
  PARAMS_RAW      m_param_raw;
  PARAMS_BMP      m_param_bmp;
  PARAMS_JPEG     m_param_jpeg;
  PARAMS_JPEG2K   m_param_jpeg2k;
  PARAMS_DICOM    m_param_dicom;
  PARAMS_PNG      m_param_png;
  PARAMS_JPEGXR   m_param_jpegxr;
  PARAMS_SGMFLT   m_param_segmentation;
  PARAMS_INPFLT   m_param_inpainting;
  PARAMS_FCDFLT   m_param_facedetection;
  PARAMS_HRMFLT   m_param_harmonization;
  PARAMS_SBLFLT   m_param_sobel;
  OSD_VALS        m_osd;

public:
  FileSaveDialog*  m_fileSaveDlg;
  OptionsDialog*   m_optionsDlg;
  CFormatDetector  m_imageDetector;
  ImageDescriptorDB m_descriptordb;

  QString     m_imageColor;
  QString     m_imageSampling;
  QString     m_imageFormat;
  IM_TYPE     m_imageType;
  bool        m_hasActiveImage;
  bool        m_hasActiveThumbs;
  bool        m_canSlideshow;
  bool        m_canPlayDicom;
  bool        m_oneImage;
  bool        m_do_segmentation;
  bool        m_do_inpainting;
  bool        m_do_facedetection;
  bool        m_do_harmonize;
  bool        m_do_sobel;
  bool        m_inpaintActive;
  bool        m_effectEnded;

  QGroupBox*  m_thumbBox;

  QMenuBar*   m_menubar;
  QMenu*      m_fileMenu;
  QMenu*      m_viewMenu;
  QMenu*      m_imageMenu;
  QMenu*      m_helpMenu;
  QMenu*      m_aboutMenu;
  QMenu*      m_optionsMenu;

  QMenu*      m_featureMenu;
  QMenu*      m_imageSearchMenu;

  QToolBar*   m_fileToolBar;
  QToolBar*   m_viewToolBar;
  QToolBar*   m_imageToolBar;

  QAction*    m_loadAction;
  QAction*    m_saveAction;
  QAction*    m_saveDbAction;
  QAction*    m_exitAction;

  QAction*    m_recentFileActions[MAXRECENTFILES];
  QAction*    m_separatorAction;
  QAction*    m_nextAction;
  QAction*    m_previousAction;
  QAction*    m_showToolBarAction;
  QAction*    m_showStatusBarAction;
  QAction*    m_segmentationAction;
  QAction*    m_facedetectionAction;
  QAction*    m_inpaintAction;
  QAction*    m_enablePaintAction;
  QAction*    m_playDicomAction;
  QAction*    m_fitSizeAction;
  QAction*    m_imageAllFeatureAction;
  QAction*    m_imageSeachByColorAction;
  QAction*    m_imageSeachByEdgeAction;

  QAction*    m_harmonizationAction;
  QAction*    m_sobelAction;
  QAction*    m_restoreAction;
  QAction*    m_optionsAction;
  QAction*    m_aboutAction;
  QAction*    m_aboutQtAction;
  QAction*    m_slideshowAction;

  QStatusBar* m_statusBar;
  QLabel*     m_hintLabel;
  QLabel*     m_fileCounterLabel;
  QLabel*     m_fileFormatLabel;
  QLabel*     m_propertiesLabel;
  QLabel*     m_samplingLabel;
  QLabel*     m_colorLabel;
  QLabel*     m_cpeLabel;
  QLabel*     m_usecLabel;

  QString     m_recentFolder;
  QStringList m_recentFiles;
  QString     m_currentFile;

  int         m_currentFileIndex;

  QTableWidget* m_thumbTable;

  ImageWidget*  m_imageLabel;
  QScrollArea*  m_scrollArea;

  QPixmap       m_frame_pxm;

  QGridLayout*  m_mainLayout;

  QList<ImageWidget*>  m_pixmapLabels;

  IppiSize      m_effect_roi;

  // performance timer
  CTimer        m_timer;

  QTimer*       m_timer2;
  QTimer*       m_timer3;

  QTimer*       m_effect_timer;
  Ipp8u*        m_buffer_prev;
  Ipp8u*        m_buffer_next;
  Ipp8u*        m_buffer_curr;
  int           m_alpha_phase;

  int           m_dicomCurrentFrame;
  int           m_dicomNumFrames;
  CStdFileInput m_dicomStdFileIn;
  QSlider*      m_dicomSlider;
  QSpinBox*     m_dicomSpinBox;

  QList<QDockWidget*>  m_docks;

  QSettings*    m_app_settings;

  QWidget*      m_centralWidget;

  QList<QLabel*> m_osd_text;

  QList<int>     m_imagesSelected;
  bool           m_needToRedraw;
  bool           m_canSearch;

  int            m_vScrollOldValue;

  QProgressBar   m_progressBar;

public:
  CIppImage     m_image;  // source image, cannot be modifyed, used for restoring
  CIppImage     m_frame;  // source image, 8-bit, for Qt
  CIppImage     m_work;   // source image, for operations
  CIppImage     m_mask;

  QSpacerItem*  m_gridBottomSpacer;
};

#endif
