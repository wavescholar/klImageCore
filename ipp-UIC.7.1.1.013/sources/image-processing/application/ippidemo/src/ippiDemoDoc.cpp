/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDemoDoc.cpp : implementation of the CippiDemoDoc class
//

#include "stdafx.h"
#include "ippiDemo.h"

#include "ippiDemoDoc.h"
#include "ippiDemoView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#include "CProcess.h"
#include "ippiDirector.h"
#include "ippiRun.h"
#include "Histo.h"
#include "StoreBmp.h"
#include "MruMenu.h"

#include "CopyDlg.h"
#include "RoiDlg.h"
#include "QuadDlg.h"
#include "CenterDlg.h"
#include "FileHistoDlg.h"
#include "BitmapSaveDlg.h"

#include "Movie.h"
#include "MovieThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc

IMPLEMENT_DYNCREATE(CippiDemoDoc, CDemoDoc)

BEGIN_MESSAGE_MAP(CippiDemoDoc, CDemoDoc)
    //{{AFX_MSG_MAP(CippiDemoDoc)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    ON_COMMAND(ID_FILE_PROP, OnFileProp)
    ON_UPDATE_COMMAND_UI(ID_FILE_SIGN, OnUpdateImgSign)
    ON_COMMAND(ID_FILE_SIGN, OnImgSign)
    ON_UPDATE_COMMAND_UI(ID_FILE_PLANE, OnUpdateImgPlane)
    ON_COMMAND(ID_FILE_PLANE, OnImgPlane)
    ON_UPDATE_COMMAND_UI(ID_FILE_CPLX, OnUpdateImgCplx)
    ON_COMMAND(ID_FILE_CPLX, OnImgCplx)
    ON_UPDATE_COMMAND_UI(ID_IMG_ALPHA, OnUpdateImgAlpha)
    ON_COMMAND(ID_IMG_ALPHA, OnImgAlpha)
    ON_UPDATE_COMMAND_UI(ID_IMG_BLUE, OnUpdateImgBlue)
    ON_COMMAND(ID_IMG_BLUE, OnImgBlue)
    ON_UPDATE_COMMAND_UI(ID_IMG_GREEN, OnUpdateImgGreen)
    ON_COMMAND(ID_IMG_GREEN, OnImgGreen)
    ON_UPDATE_COMMAND_UI(ID_IMG_RED, OnUpdateImgRed)
    ON_COMMAND(ID_IMG_RED, OnImgRed)
    ON_COMMAND(ID_CENTER_COPY, OnCenterCopy)
    ON_UPDATE_COMMAND_UI(ID_CENTER_COPY, OnUpdateCenterCopy)
    ON_COMMAND(ID_CENTER_DELETE, OnCenterDelete)
    ON_UPDATE_COMMAND_UI(ID_CENTER_DELETE, OnUpdateCenterDelete)
    ON_COMMAND(ID_CENTER_DRAW, OnCenterDraw)
    ON_COMMAND(ID_CENTER_SET, OnCenterSet)
    ON_COMMAND(ID_PARL_COPY, OnParlCopy)
    ON_UPDATE_COMMAND_UI(ID_PARL_COPY, OnUpdateParlCopy)
    ON_COMMAND(ID_PARL_DELETE, OnParlDelete)
    ON_UPDATE_COMMAND_UI(ID_PARL_DELETE, OnUpdateParlDelete)
    ON_COMMAND(ID_PARL_DRAW, OnParlDraw)
    ON_COMMAND(ID_PARL_SET, OnParlSet)
    ON_COMMAND(ID_QUAD_COPY, OnQuadCopy)
    ON_UPDATE_COMMAND_UI(ID_QUAD_COPY, OnUpdateQuadCopy)
    ON_COMMAND(ID_QUAD_DELETE, OnQuadDelete)
    ON_UPDATE_COMMAND_UI(ID_QUAD_DELETE, OnUpdateQuadDelete)
    ON_COMMAND(ID_QUAD_DRAW, OnQuadDraw)
    ON_COMMAND(ID_QUAD_SET, OnQuadSet)
    ON_COMMAND(ID_ROI_COPY, OnRoiCopy)
    ON_UPDATE_COMMAND_UI(ID_ROI_COPY, OnUpdateRoiCopy)
    ON_COMMAND(ID_ROI_DELETE, OnRoiDelete)
    ON_UPDATE_COMMAND_UI(ID_ROI_DELETE, OnUpdateRoiDelete)
    ON_COMMAND(ID_ROI_DRAW, OnRoiDraw)
    ON_COMMAND(ID_ROI_SET, OnRoiSet)
    ON_UPDATE_COMMAND_UI(ID_ROI_SET, OnUpdateRoiSet)
    ON_COMMAND(ID_ROI, OnRoi)
    ON_UPDATE_COMMAND_UI(ID_ROI, OnUpdateRoi)
    ON_UPDATE_COMMAND_UI(ID_ROI_DRAW, OnUpdateRoiDraw)
    ON_UPDATE_COMMAND_UI(ID_CENTER_DRAW, OnUpdateCenterDraw)
    ON_UPDATE_COMMAND_UI(ID_PARL_DRAW, OnUpdateParlDraw)
    ON_UPDATE_COMMAND_UI(ID_QUAD_DRAW, OnUpdateQuadDraw)
    ON_COMMAND(ID_DRAW_CANCEL, OnDrawCancel)
    ON_UPDATE_COMMAND_UI(ID_MARKER_DELETE, OnUpdateMarkerDelete)
    ON_COMMAND(ID_MARKER_DELETE, OnMarkerDelete)
    ON_UPDATE_COMMAND_UI(ID_CENTER, OnUpdateCenter)
    ON_UPDATE_COMMAND_UI(ID_PARL, OnUpdateParl)
    ON_COMMAND(ID_PARL, OnParl)
    ON_UPDATE_COMMAND_UI(ID_QUAD, OnUpdateQuad)
    ON_COMMAND(ID_QUAD, OnQuad)
    ON_COMMAND(ID_CENTER, OnCenter)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_PLAY, OnUpdateMoviePlay)
    ON_COMMAND(ID_MOVIE_PLAY, OnMoviePlay)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_STOP, OnUpdateMovieStop)
    ON_COMMAND(ID_MOVIE_STOP, OnMovieStop)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_PAUSE, OnUpdateMoviePause)
    ON_COMMAND(ID_MOVIE_PAUSE, OnMoviePause)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_NEXT, OnUpdateMovieNext)
    ON_COMMAND(ID_MOVIE_NEXT, OnMovieNext)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_PREV, OnUpdateMoviePrev)
    ON_COMMAND(ID_MOVIE_PREV, OnMoviePrev)
    ON_COMMAND(ID_WINDOW_FIT, OnWindowFit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc construction/destruction

CippiDemoDoc::CippiDemoDoc() : CDemoDoc()
{
    m_DrawTool  = DRAW_NONE;
    m_pMovie = NULL;
    m_pMovieThread = NULL;
    m_MovieStage = MOVIE_STOP;

    m_StoreHeader.plane = false;
    m_StoreHeader.type = pp8u;
    m_StoreHeader.width = 1;
    m_StoreHeader.height = 1;
}

CippiDemoDoc::~CippiDemoDoc()
{
    if (m_pMovie)
        delete m_pMovie;
    if (m_pMovieThread)
        delete m_pMovieThread;
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

//---------------------------------------------------------------------------
// IsData returns weather vector data is allocated or not
//---------------------------------------------------------------------------
BOOL CippiDemoDoc::IsData()
{
    return GetDataPtr() || GetDataPtrP();
}

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides

//---------------------------------------------------------------------------
// Serialize loads & saves bmp file
//---------------------------------------------------------------------------
void CippiDemoDoc::Serialize(CArchive& ar)
{
    CFile *file = ar.GetFile();
    CStoreBmp store(this);
    if (ar.IsStoring()) {
        if (!CanSaveToBitmap()) return;
        store.Save(file);
        m_StoreHeader = GetHeader();
    } else {
        store.Load(file);
        m_StoreHeader = GetHeader();
        IsUpdated(TRUE);
    }
}

//---------------------------------------------------------------------------
// OnCloseDocument is called before document is closed
//---------------------------------------------------------------------------
void CippiDemoDoc::OnCloseDocument()
{
    CancelDraw();
    if (m_pMovie && m_pMovie->IsPlaying()) {
        m_pMovieThread->Terminate();
    }
    CDemoDoc::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////
// Drawaing implementation

//---------------------------------------------------------------------------
// BeginDraw starts drawing by specified tool
//---------------------------------------------------------------------------
void CippiDemoDoc::BeginDraw(int drawTool)
{
    CancelDraw();
    SetDrawTool(drawTool);
}

//---------------------------------------------------------------------------
// FinishDraw completes drawing
//---------------------------------------------------------------------------
void CippiDemoDoc::FinishDraw()
{
    m_DrawTool = DRAW_NONE;
    IsUpdated(TRUE);
    UpdateView();
    SetStatusString();
}

//---------------------------------------------------------------------------
// CancelDraw cancels drawing
//---------------------------------------------------------------------------
void CippiDemoDoc::CancelDraw()
{
    if (!GetIppiView()) return;
    GetIppiView()->CancelDraw();
    switch (m_DrawTool) {
    case DRAW_ROI:     ResetRoi(); break;
    case DRAW_QUAD:    ResetQuad(); break;
    case DRAW_PARL:    ResetParl(); break;
    case DRAW_CENTER: ResetCenterShift(); break;
    }
    FinishDraw();
}

/////////////////////////////////////////////////////////////////////////////
// Movie implementation

//---------------------------------------------------------------------------
// CreateMovie creates movie & movie thread at the document creation
//---------------------------------------------------------------------------
void CippiDemoDoc::CreateMovie()
{
    m_pMovieThread  = new CMovieThread(this);
    m_pMovie = new CMovie(this, m_pProcess, GetIppiView()->m_hWnd);
    m_pMovie->Init();
}

//---------------------------------------------------------------------------
// StartMovie starts movie
//---------------------------------------------------------------------------

void CippiDemoDoc::StartMovie(int stage)
{
    BOOL bStart = m_MovieStage == MOVIE_STOP;
    m_MutexFlag.Lock();
    m_MovieStage = stage;
    m_MutexFlag.Unlock();
    if (bStart) {
        if (!m_pMovieThread->Begin())
            m_MovieStage = MOVIE_STOP;
    }
}

//---------------------------------------------------------------------------
// DrawMovie update view and correct document title while movie performance
//---------------------------------------------------------------------------
void CippiDemoDoc::DrawMovie(LPCSTR title)
{
    UpdateView();
    UpdateStatusString();
}

//---------------------------------------------------------------------------
// StopMovie stops movie
//---------------------------------------------------------------------------
void CippiDemoDoc::StopMovie()
{
    UpdateView();
    UpdateStatusString();
}


/////////////////////////////////////////////////////////////////////////////
// Protected Operations

//---------------------------------------------------------------------------
// CreateProcess creates class that will be process this document by proper
// IPP functions
//---------------------------------------------------------------------------
void CippiDemoDoc::CreateProcess()
{
    CDemoDoc::CreateProcess();
    CreateMovie();
}

//---------------------------------------------------------------------------
// CreateNewData creates new vector(image) with default parameters
//---------------------------------------------------------------------------
BOOL CippiDemoDoc::CreateNewData()
{
    SetHeader(DEMO_APP->GetNewHeader());
    AllocateData(0);
    SetData(DEMO_APP->GetNewPattern());
    return TRUE;
}

//---------------------------------------------------------------------------
// SetData sets image data with specified pattern (see ippiDemo.h,
// SET_*** enumerator)
//---------------------------------------------------------------------------

static BOOL setDataJaehne(int type, int channels, void* dst, int dstStep, IppiSize size)
{
    switch (channels) {
    case 1:
        switch (type) {
        case pp8u : case pp8s :
            ippiImageJaehne_8u_C1R ((Ipp8u *)dst, dstStep, size);
            return TRUE;
        case pp16u: case pp16s:
            ippiImageJaehne_16u_C1R((Ipp16u*)dst, dstStep, size);
            return TRUE;
        case pp32u: case pp32s:
            ippiImageJaehne_32s_C1R((Ipp32s*)dst, dstStep, size);
            return TRUE;
        case pp32f:
            ippiImageJaehne_32f_C1R((Ipp32f*)dst, dstStep, size);
            return TRUE;
        }
    case 3:
        switch (type) {
        case pp8u : case pp8s :
            ippiImageJaehne_8u_C3R ((Ipp8u *)dst, dstStep, size);
            return TRUE;
        case pp16u: case pp16s:
            ippiImageJaehne_16u_C3R((Ipp16u*)dst, dstStep, size);
            return TRUE;
        case pp32u: case pp32s:
            ippiImageJaehne_32s_C3R((Ipp32s*)dst, dstStep, size);
            return TRUE;
        case pp32f:
            ippiImageJaehne_32f_C3R((Ipp32f*)dst, dstStep, size);
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL setDataRamp  (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    switch (channels) {
    case 1:
        switch (type) {
        case pp8u : case pp8s :
            ippiImageRamp_8u_C1R ((Ipp8u *)dst, dstStep, size, 0, (float)IPP_MAX_8U / (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp16u: case pp16s:
            ippiImageRamp_16u_C1R((Ipp16u*)dst, dstStep, size, 0, (float)IPP_MAX_16U / (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp32u: case pp32s:
            ippiImageRamp_32s_C1R((Ipp32s*)dst, dstStep, size, (float)IPP_MIN_32S, ((float)IPP_MAX_32S - (float)IPP_MIN_32S)/ (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp32f:
            ippiImageRamp_32f_C1R((Ipp32f*)dst, dstStep, size, 0, (float)1 / (float)size.width, ippAxsHorizontal);
            return TRUE;
        }
    case 3:
        switch (type) {
        case pp8u : case pp8s :
            ippiImageRamp_8u_C3R ((Ipp8u *)dst, dstStep, size, 0, (float)IPP_MAX_8U / (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp16u: case pp16s:
            ippiImageRamp_16u_C3R((Ipp16u*)dst, dstStep, size, 0, (float)IPP_MAX_16U / (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp32u: case pp32s:
            ippiImageRamp_32s_C3R((Ipp32s*)dst, dstStep, size, (float)IPP_MIN_32S, ((float)IPP_MAX_32S - (float)IPP_MIN_32S)/ (float)size.width, ippAxsHorizontal);
            return TRUE;
        case pp32f:
            ippiImageRamp_32f_C3R((Ipp32f*)dst, dstStep, size, 0, (float)1 / (float)size.width, ippAxsHorizontal);
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL setDataBlack (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    if (type == pp32s) {
        size.width *= channels;
        ippiSet_32s_C1R(IPP_MIN_32S,(Ipp32s*)dst, dstStep, size);
        return TRUE;
    } else {
         size.width *= channels*((type & PP_MASK)>>3);
         ippiSet_8u_C1R(0,(Ipp8u*)dst, dstStep, size);
         return TRUE;
    }
}

static BOOL setDataWhite (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    if (type == pp32f) {
        size.width *= channels;
        ippiSet_32f_C1R(1,(Ipp32f*)dst, dstStep, size);
        return TRUE;
    } else if (type == pp32s) {
        size.width *= channels;
        ippiSet_32s_C1R(IPP_MAX_32S,(Ipp32s*)dst, dstStep, size);
        return TRUE;
    } else {
        size.width *= channels*((type & PP_MASK)>>3);
        ippiSet_8u_C1R(255,(Ipp8u*)dst, dstStep, size);
        return TRUE;
    }
    return FALSE;
}

static BOOL setDataGray (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    size.width *= channels;
    switch (type) {
    case pp8u : case pp8s :
        ippiSet_8u_C1R(CHAR_MAX,(Ipp8u*)dst, dstStep, size);
        return TRUE;
    case pp16u: case pp16s:
        ippiSet_16s_C1R(SHRT_MAX,(Ipp16s*)dst, dstStep, size);
        return TRUE;
    case pp32u: case pp32s:
    case pp32f:
        ippiSet_32f_C1R(0,(Ipp32f*)dst, dstStep, size);
        return TRUE;
    }
    return FALSE;
}

static BOOL setDataBlue  (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    return FALSE;
}
static BOOL setDataGreen (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    return FALSE;
}
static BOOL setDataRed    (int type, int channels, void* dst, int dstStep, IppiSize size)
{
    return FALSE;
}

typedef BOOL (*ProcSetData) (int type, int channels,
                                      void* dst, int dstStep, IppiSize size);
static ProcSetData GetDataProc(int pattern)
{
    switch (pattern) {
    case SET_JAEHNE: return setDataJaehne;
    case SET_RAMP  : return setDataRamp  ;
    case SET_GRAY  : return setDataGray  ;
    case SET_BLACK : return setDataBlack ;
    case SET_WHITE : return setDataWhite ;
    case SET_BLUE  : return setDataBlue  ;
    case SET_GREEN : return setDataGreen ;
    case SET_RED    : return setDataRed    ;
    }
    return NULL;
}

static void* createBuffer(int type, IppiSize size, int& step)
{
    step = (((type & PP_MASK)>>3)*size.width + 31) & ~31;
    return ippMalloc(step*size.height);
}
static void deleteBuffer(void* ptr)
{
    if (ptr) ippFree(ptr);
}

void CippiDemoDoc::SetData(int pattern)
{
    int channels = Channels();
    if (Complex()) {
        if (Plane()) return;
        if (channels > 1) return;
        channels = 2;
        m_widthP[1] = m_widthP[0];
        m_heightP[1] = m_heightP[0];
    }
    ProcSetData pSetData = GetDataProc(pattern);
    if (!pSetData) return;
    IppiSize size = {Width(), Height()};
    if (!Plane()) {
        if (pSetData(Type(),channels,GetData(),Step(),size))
            return;
    }
    int bufType = (int)Type() & PP_MASK;
    if (Float()) bufType |= PP_FLOAT;
    int bufStep = 0;
    Ipp8u* buffer = (Ipp8u*)createBuffer(bufType, size,bufStep);
    if (!pSetData(bufType,1,buffer,bufStep,size)) {
        deleteBuffer(buffer); buffer = NULL;
        return;
    }
    bufType &= PP_MASK;
    if (Plane()) {
        for (int c=0; c<channels; c++) {
            IppiSize size = GetSizeP(c);
            size.width *= Depth() >> 3;
            ippiCopy_8u_C1R((Ipp8u*)buffer,bufStep,(Ipp8u*)GetDataPtrP(c),StepP(c),size);
        }
    } else if (channels == 4) {
        switch (bufType) {
        case 8:
            {
            Ipp8u* dst = (Ipp8u*)GetData();
            for (int c=0; c<4; c++) {
                ippiCopy_8u_C1C4R((Ipp8u*)buffer,bufStep,dst,Step(),size);
                dst ++;
            }}
            break;
        case 16:
            {
            Ipp16s* dst = (Ipp16s*)GetData();
            for (int c=0; c<4; c++) {
                ippiCopy_16s_C1C4R((Ipp16s*)buffer,bufStep,dst,Step(),size);
                dst ++;
            }}
            break;
        case 32:
            {
            Ipp32f* dst = (Ipp32f*)GetData();
            for (int c=0; c<4; c++) {
                ippiCopy_32f_C1C4R((Ipp32f*)buffer,bufStep,dst,Step(),size);
                dst ++;
            }}
            break;
        }
    } else if (channels == 2) {
        Ipp8u* dst = (Ipp8u*)GetData();
        Ipp8u* tmp = buffer;
        for (int y=0; y<Height(); y++) {
            for (int x=0; x<Width(); x++) {
                switch (bufType) {
                case 8:
                    ((Ipp8u*)dst)[x*2  ] = ((Ipp8u*)tmp)[x];
                    ((Ipp8u*)dst)[x*2+1] = ((Ipp8u*)tmp)[x];
                    break;
                case 16:
                    ((Ipp16u*)dst)[x*2  ] = ((Ipp16u*)tmp)[x];
                    ((Ipp16u*)dst)[x*2+1] = ((Ipp16u*)tmp)[x];
                    break;
                case 32:
                    ((Ipp32s*)dst)[x*2  ] = ((Ipp32s*)tmp)[x];
                    ((Ipp32s*)dst)[x*2+1] = ((Ipp32s*)tmp)[x];
                    break;
                }
            }
            dst += Step();
            tmp += bufStep;
        }
    }
    deleteBuffer(buffer); buffer = NULL;
}

//---------------------------------------------------------------------------
// CanSaveToBitmap prepares document to be saved in standard bmp format
// or in non-standard format due to user respond.
//---------------------------------------------------------------------------
BOOL CippiDemoDoc::CanSaveToBitmap()
{
    ImgHeader header = GetHeader();
    int compression = CStoreBmp::Compression(header);
    if (compression < 0)
    {
        DEMO_APP->MessageBox(      
            _T("Cannot save the image:\n")
            _T("Plane sizes are irregular"), 
            MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }
    BOOL unstandard = FALSE;
    if ((header.plane != m_StoreHeader.plane) && header.plane)
        unstandard = TRUE;
    if ((compression != CStoreBmp::Compression(m_StoreHeader)) && (compression != BI_RGB))
        unstandard = TRUE;
    if (!unstandard)
        return TRUE;
    CBitmapSaveDlg dlg;
    if (!header.EqualPlaneSize())
        dlg.m_bConvert = FALSE;
    switch (dlg.DoModal()) 
    {
    case IDOK:
        return TRUE;
    case IDCANCEL:
        return FALSE;
    default:
        break;
    }
    ConvertPlane(FALSE);
    ConvertToRGB();
    return TRUE;
}

//---------------------------------------------------------------------------
// UpdateFileName updates document title after SaveAs command
//---------------------------------------------------------------------------
void CippiDemoDoc::UpdateFileName()
{
    CMyString fileName = GetPathName();
    CippiDemoDoc* pSameDoc = (CippiDemoDoc*)DEMO_APP->FindDoc(fileName.GetName());
    if (pSameDoc && (pSameDoc != this)) {
        pSameDoc->OnCloseDocument();
    }
    SetPathName(fileName);
    UpdateFrameTitle();
    SetStatusString();
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

//---------------------------------------------------------------------------
// GetFrameType returns image type string to be insert to document
// frame title
//---------------------------------------------------------------------------
CString CippiDemoDoc::GetFrameType()
{
    CString frameType =
        TypeString() + "_" +
        ChannelString();
    if (GetCoi() >= 0)
        frameType += "/" + CoiString();
    return frameType;
}

//---------------------------------------------------------------------------
// GetDocStatus returns information be set to main window status string
// when this document is active
//---------------------------------------------------------------------------
CString CippiDemoDoc::GetDocStatus()
{
    if (m_pMovie->IsPlaying()) {
        CString status = GetTitle();
        CString info = m_pMovie->GetInfo();
        if (!info.IsEmpty()) 
            status += " - " + info;
        return status;
    } else if (GetDrawTool())
        return GetFrameTitle()  + ":  Drawing "
                                + GetRoiStatus()
                                + GetQuadStatus()
                                + GetParlStatus()
                                + GetCenterStatus();
    else
        return CDemoDoc::GetDocStatus();
}

/////////////////////////////////////////////////////////////////////////////
// Protected Attributes

//---------------------------------------------------------------------------
// GetImageInfo returns image parameters information to be print by
// Properties dialog
//---------------------------------------------------------------------------
CString CippiDemoDoc::GetImageInfo()
{
     return GetPathInfo() +
              GetHeaderInfo() +
              GetRoiInfo() +
              GetQuadInfo() +
              GetParlInfo() +
              GetCenterInfo();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes for GetImageInfo & GetDocStatus implementation

CString CippiDemoDoc::GetPathInfo() const
{
    if (GetPathName().IsEmpty()) return "";
    CString info;
    info.Format("File name \t%s\n", GetPathName());
    return info;
}




static CString layoutString(const CImage* pImg)
{
    if (pImg->Is422()) return "(422)";
    if (pImg->Is420()) return "(420)";
    if (pImg->Is411()) return "(411)";
    return "";
}

CString CippiDemoDoc::GetHeaderInfo()
{
    CString info;
    info.Format("\n"
        "Width         \t%3d\n"
        "Height        \t%3d\n"
        "Type          \t%s\n"
        "Channels     \t%s\n",
        Width(),
        Height(),
        TypeString(),
        ChannelString() + layoutString(this));

    CString str;
    if (IsBitImage()) {
        str.Format(
            "BitWidth     \t%3d\n",
            BitWidth());
        info += str;
    }
    if (Compressed()) {
        CString comprString;
        switch (Compressed()) {
        case RGB444: comprString = "444"; break;
        case RGB555: comprString = "555"; break;
        case RGB565: comprString = "565"; break;
        }
        str.Format("Compressed  \t%s\n",
                      comprString);
        info += str;
    }
    if (m_Coi >= 0) {
        str.Format("View          \t%s\n",
                      CoiString());
        info += str;
    }
    if (Float()) {
        Ipp32f vmin, vmax;
        if (Plane())
            GetFloatLimitsP(vmin, vmax, GetCoi());
        else
            GetFloatLimits(vmin, vmax);
        str.Format("Min Val         \t%.3g\n"
                   "Max Val         \t%.3g\n",
                   vmin, vmax);
        info += str;
    }
    return info;
}

CString CippiDemoDoc::GetRoiInfo() const
{
    const IppiRect* rect = GetRoi();
    if (!rect) return "";
    CString info;
    CString str;
    str.Format("\nROI:\n"
         "  offset        \t(%d,%d)\n"
         "  size          \t%dx%d\n",
         rect->x,rect->y,
         rect->width,rect->height);
    info += str;
    if (IsBitImage()) {
        str.Format(
             "  bitOffset    \t%d\n"
             "  bitRoiWidth \t%d\n",
             BitOffset(),
             BitRoiWidth());
        info += str;
    }
    return info;
}

CString CippiDemoDoc::GetQuadInfo() const
{
    double quad[4][2];
    if (!GetQuad(quad)) return "";
    CString info = "\nQuadrangle\n";
    for (int i=0; i<4; i++) {
        CString str;
        str.Format("     point%d    \t(%.1f, %.1f)\n",
            i + 1, quad[i][0], quad[i][1]);
        info += str;
    }
    return info;
}

CString CippiDemoDoc::GetParlInfo() const
{
    double quad[4][2];
    if (!GetParl(quad)) return "";
    CString info = "\nParallelogramm\n";
    for (int i=0; i<4; i++) {
        CString str;
        str.Format("     point%d    \t(%.1f, %.1f)\n",
            i + 1, quad[i][0], quad[i][1]);
        info += str;
    }
    return info;
}

CString CippiDemoDoc::GetCenterInfo() const
{
    double center[2];
    double shift[2];
    if (!GetCenterShift(center, shift)) return "";
    CString info;
    info.Format("\n"
        "Center          \t(%.1f,%.1f)\n"
        "Shift            \t(%.1f,%.1f)\n",
        center[0], center[1],
        shift[0], shift[1]);
    return info;
}

CString CippiDemoDoc::GetRoiStatus() const
{
     if (GetDrawTool() != DRAW_ROI) return "";
     const IppiRect* roi = GetRoi();
     if (!roi) return "ROI";
     CString status;
     status.Format("ROI (%d,%d)-(%dx%d)",
              roi->x, roi->y,
              roi->width, roi->height);
     return status;
}

CString CippiDemoDoc::GetQuadStatus() const
{
     if (GetDrawTool() != DRAW_QUAD) return "";
     CString name = "Quadrangle";
     double quad[4][2];
     int num;
     if (!GetQuad(quad, num)) return name;
     return  name + " " + GetQuadPointInfo(quad,num,"%.0f");
}

CString CippiDemoDoc::GetParlStatus() const
{
     if (GetDrawTool() != DRAW_PARL) return "";
     CString name = "Parallelogramm";
     double quad[4][2];
     int num;
     if (!GetParl(quad, num)) return name;
     return  name + " " + GetQuadPointInfo(quad,num,"%.0f");
}

CString CippiDemoDoc::GetCenterStatus() const
{
     if (GetDrawTool() != DRAW_CENTER) return "";
     double center[2];
     double shift[2];
     if (!GetCenterShift(center, shift)) return "Center & Shift";
     CString status;
     status.Format("Center (%.0f,%.0f)  Shift (%.0f,%.0f)",
              center[0], center[1],
              shift[0], shift[1]);
     return status;
}

CString CippiDemoDoc::GetQuadPointInfo(const double quad[4][2],
                                              int num, CString format) const
{
     CString info;
     format = "(" + format + "," + format + ")";
     for (int i=0; i<num; i++) {
          CString point;
          point.Format(format,quad[i][0], quad[i][1]);
          info += point;
          if (i < 3) info += "-";
     }
     return info;
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc diagnostics

#ifdef _DEBUG
void CippiDemoDoc::AssertValid() const
{
    CDemoDoc::AssertValid();
}

void CippiDemoDoc::Dump(CDumpContext& dc) const
{
    CDemoDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//  Command processing implementation

BOOL CippiDemoDoc::CanEnableProcess()
{
    return (m_MovieStage == MOVIE_STOP) &&
             CDemoDoc::CanEnableProcess();
}

BOOL CippiDemoDoc::CanUpdateMru()
{
    return (m_MovieStage == MOVIE_STOP) &&
             CDemoDoc::CanUpdateMru();
}

void CippiDemoDoc::OnUpdateContour(CCmdUI* pCmdUI, int drawFlag, BOOL bExist)
{
    if (MOVIE_STOP != GetMovieStage()) {
        pCmdUI->Enable(FALSE);
    } else {
        pCmdUI->Enable(m_DrawTool == DRAW_NONE || m_DrawTool == drawFlag);
        pCmdUI->SetCheck((m_DrawTool == drawFlag) || bExist);
    }
}

void CippiDemoDoc::OnUpdateChannel(CCmdUI* pCmdUI, int coi)
{
    BOOL flag = Plane() && (Channels() > coi);
    pCmdUI->Enable(flag);
    pCmdUI->SetCheck(flag && (m_Coi == coi));
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}

void CippiDemoDoc::OnChannel(int coi)
{
    DEMO_APP->SetCursorWait();
    if (m_Coi == coi) m_Coi = -1;
    else m_Coi = coi;
    m_pProcess->ApplyVector(this);
    MarkToPick();
    IsUpdated(TRUE);
    UpdateView();
    UpdateFrameTitle();
    SetStatusString();
    DEMO_APP->SetCursorArrow();
}

void CippiDemoDoc::OnDraw(int drawFlag)
{
    if (m_DrawTool == DRAW_NONE) {
        m_DrawTool = drawFlag;
        SetStatusString();
        BeginDraw(drawFlag);
    } else {
        OnDrawCancel();
    }
}

void CippiDemoDoc::OnUpdateDraw(CCmdUI* pCmdUI, int drawFlag)
{
    BOOL flag = ((m_DrawTool == drawFlag) || (m_DrawTool == DRAW_NONE));
    pCmdUI->Enable(flag);
    pCmdUI->SetCheck(flag && (m_DrawTool == drawFlag));
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc File commands

//---------------------------------------------------------------------------
// OnFileSave performs Menu-File-Save command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnFileSave()
{
    if (!DoFileSave()) return;
    UpdateFileName();
}

//---------------------------------------------------------------------------
// OnFileSaveAs performs Menu-File-SaveAs command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnFileSaveAs()
{
    CDocTemplate* pTemplate = GetDocTemplate();
    ASSERT(pTemplate != NULL);
    CMyString fileName = GetPathName();
    int depth = Depth();
    int isPlane = Plane();
    if (fileName.IsEmpty()) {
        fileName = GetTitle() + ".bmp";
    }
    if (!AfxGetApp()->DoPromptFileName(fileName,
      AFX_IDS_SAVEFILE,
      OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
        return;
    if (!DoSave(fileName))
        return;
    UpdateFileName();
}

//---------------------------------------------------------------------------
// OnFileProp performs Menu-File-Properties command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnFileProp()
{
    MAIN_FRAME->MessageBox(GetImageInfo(), GetTitle() + " Properties", 0);
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc Image Data commands

//---------------------------------------------------------------------------
// OnUpdateImgSign enables Menu-ImageData-Signed command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgSign(CCmdUI* pCmdUI)
{
    BOOL flag = !Float() && Type() != pp64s && !Complex();
    pCmdUI->Enable(flag);
    pCmdUI->SetCheck(Sign());
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}

//---------------------------------------------------------------------------
// OnUpdateImgPlane enables Menu-ImageData-Plane command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgPlane(CCmdUI* pCmdUI)
{
    BOOL flag = (!Complex() && Channels() > 1) || (Complex() && (Channels() == 1));
    pCmdUI->Enable(flag);
    pCmdUI->SetCheck(Plane());
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}

//---------------------------------------------------------------------------
// OnUpdateImgCplx enables Menu-ImageData-Complex command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgCplx(CCmdUI* pCmdUI)
{
    BOOL flag = (Complex() && (Channels() == 1)) ||
                  (!Complex() && (Channels() == 2));
    pCmdUI->Enable(flag);
    pCmdUI->SetCheck(Complex());
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}
//---------------------------------------------------------------------------
// OnImgSign performs Menu-ImageData-Signed command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgSign()
{
    DEMO_APP->SetCursorWait();
    if (Sign())
        m_type = (ppType)((int)m_type & ~PP_SIGN);
    else
        m_type = (ppType)((int)m_type | PP_SIGN);
    m_pProcess->ApplyVector(this);
    MarkToPick();
    UpdateFrameTitle();
    SetStatusString();
    DEMO_APP->SetCursorArrow();
}

//---------------------------------------------------------------------------
// OnImgPlane performs Menu-ImageData-Plane command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgPlane()
{
    DEMO_APP->SetCursorWait();
    if (Complex()) {
        m_type = (ppType)((int)m_type & ~PP_CPLX);
        m_channels = 2;
        m_widthP[1] = m_widthP[0];
        m_heightP[1] = m_heightP[0];
    }
    ConvertPlane(!Plane());
    m_pProcess->ApplyVector(this);
    MarkToPick();
    if (GetCoi() >= 0) {
        SetCoi(-1);
        IsUpdated(TRUE);
        UpdateView();
    }
    UpdateFrameTitle();
    SetStatusString();
    DEMO_APP->SetCursorArrow();
}


//---------------------------------------------------------------------------
// OnImgCplx performs Menu-ImageData-Complex command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgCplx()
{
    DEMO_APP->SetCursorWait();
    if (Complex()) {
        m_type = (ppType)((int)m_type & ~PP_CPLX);
        m_channels = 2;
        m_widthP[1] = m_widthP[0];
        m_heightP[1] = m_heightP[0];
    } else {
        if (Plane()) ConvertPlane(FALSE);
        if (Unsign()) m_type = (ppType)((int)m_type | PP_SIGN);
        m_type = (ppType)((int)m_type | PP_CPLX);
        m_channels = 1;
    }
    m_pProcess->ApplyVector(this);
    MarkToPick();
    UpdateFrameTitle();
    SetStatusString();
    DEMO_APP->SetCursorArrow();
}


//---------------------------------------------------------------------------
// OnUpdateImgBlue enables Menu-ImageData-BlueChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgBlue(CCmdUI* pCmdUI)
{
    OnUpdateChannel(pCmdUI, 0);
}

//---------------------------------------------------------------------------
// OnUpdateImgGreen enables Menu-ImageData-GreenChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgGreen(CCmdUI* pCmdUI)
{
    OnUpdateChannel(pCmdUI, 1);
}

//---------------------------------------------------------------------------
// OnUpdateImgRed enables Menu-ImageData-RedChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgRed(CCmdUI* pCmdUI)
{
    OnUpdateChannel(pCmdUI, 2);
}

//---------------------------------------------------------------------------
// OnUpdateImgAlpha enables Menu-ImageData-AlphaChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateImgAlpha(CCmdUI* pCmdUI)
{
    OnUpdateChannel(pCmdUI, 3);
}

//---------------------------------------------------------------------------
// OnImgBlue performs Menu-ImageData-BlueChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgBlue()
{
    OnChannel(0);
}

//---------------------------------------------------------------------------
// OnImgGreen performs Menu-ImageData-GreenChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgGreen()
{
    OnChannel(1);
}

//---------------------------------------------------------------------------
// OnImgRed performs Menu-ImageData-RedChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgRed()
{
    OnChannel(2);
}

//---------------------------------------------------------------------------
// OnImgAlpha performs Menu-ImageData-AlphaChannel command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnImgAlpha()
{
    OnChannel(3);
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc Contour commands

//---------------------------------------------------------------------------
// OnDrawCancel performs ContextMenu-CancelDraw command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnDrawCancel()
{
    CancelDraw();
}

//---------------------------------------------------------------------------
// OnUpdate[Roi/Quad/Parl/Center] enables Toolbar-[ROI/Quad/Parl/Center] command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateRoi(CCmdUI* pCmdUI)
{
    OnUpdateContour(pCmdUI, DRAW_ROI, IsRoi());
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}
void CippiDemoDoc::OnUpdateQuad(CCmdUI* pCmdUI)
{
    OnUpdateContour(pCmdUI, DRAW_QUAD, IsQuad());
}
void CippiDemoDoc::OnUpdateParl(CCmdUI* pCmdUI)
{
    OnUpdateContour(pCmdUI, DRAW_PARL, IsParl());
}
void CippiDemoDoc::OnUpdateCenter(CCmdUI* pCmdUI)
{
    OnUpdateContour(pCmdUI, DRAW_CENTER, IsCenterShift());
}

//---------------------------------------------------------------------------
// On[Roi/Quad/Parl/Center] performs Toolbar-[ROI/Quad/Parl/Center] command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnRoi()
{
    if ((m_DrawTool == DRAW_NONE) && IsRoi())
        OnRoiDelete();
    else
        OnDraw(DRAW_ROI);
}
void CippiDemoDoc::OnQuad()
{
    if ((m_DrawTool == DRAW_NONE) && IsQuad())
        OnQuadDelete();
    else
        OnDraw(DRAW_QUAD);
}
void CippiDemoDoc::OnParl()
{
    if ((m_DrawTool == DRAW_NONE) && IsParl())
        OnParlDelete();
    else
        OnDraw(DRAW_PARL);
}
void CippiDemoDoc::OnCenter()
{
    if ((m_DrawTool == DRAW_NONE) && IsCenterShift())
        OnCenterDelete();
    else
        OnDraw(DRAW_CENTER);
}

//---------------------------------------------------------------------------
// OnUpdate[Roi/Quad/Parl/Center]Draw enables
// Menu-Contour-[ROI/Quad/Parl/Center]-Draw command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateRoiDraw(CCmdUI* pCmdUI)
{
    OnUpdateDraw(pCmdUI, DRAW_ROI);
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}
void CippiDemoDoc::OnUpdateQuadDraw(CCmdUI* pCmdUI)
{
    OnUpdateDraw(pCmdUI, DRAW_QUAD);
}
void CippiDemoDoc::OnUpdateParlDraw(CCmdUI* pCmdUI)
{
    OnUpdateDraw(pCmdUI, DRAW_PARL);
}
void CippiDemoDoc::OnUpdateCenterDraw(CCmdUI* pCmdUI)
{
    OnUpdateDraw(pCmdUI, DRAW_CENTER);
}

//---------------------------------------------------------------------------
// On[Roi/Quad/Parl/Center]Draw performs
// Menu-Contour-[ROI/Quad/Parl/Center]-Draw command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnRoiDraw()
{
    OnDraw(DRAW_ROI);
}
void CippiDemoDoc::OnQuadDraw()
{
    OnDraw(DRAW_QUAD);
}
void CippiDemoDoc::OnParlDraw()
{
    OnDraw(DRAW_PARL);
}
void CippiDemoDoc::OnCenterDraw()
{
    OnDraw(DRAW_CENTER);
}

//---------------------------------------------------------------------------
// OnUpdateRoiSet enables Menu-Contour-ROI-Set command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateRoiSet(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_MovieStage == MOVIE_STOP);
}

//---------------------------------------------------------------------------
// On[Roi/Quad/Parl/Center]Set performs
// Menu-Contour-[ROI/Quad/Parl/Center]-Set command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnRoiSet()
{
     CRoiDlg roiDlg(this);
     if (roiDlg.DoModal() != IDOK) return;
     UpdateView();
     SetStatusString();
     return;
}
void CippiDemoDoc::OnQuadSet()
{
     CQuadDlg dlg(this);
     if (dlg.DoModal() != IDOK) return;
     UpdateView();
     SetStatusString();
     return;
}
void CippiDemoDoc::OnParlSet()
{
     CQuadDlg dlg(this, TRUE);
     if (dlg.DoModal() != IDOK) return;
     UpdateView();
     SetStatusString();
     return;
}
void CippiDemoDoc::OnCenterSet()
{
     CCenterDlg dlg(this);
     if (dlg.DoModal() != IDOK) return;
     UpdateView();
     SetStatusString();
     return;
}

//---------------------------------------------------------------------------
// OnUpdate[Roi/Quad/Parl/Center]Copy enables
// Menu-Contour-[ROI/Quad/Parl/Center]-Copy command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateRoiCopy(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(IsRoi() &&
        DEMO_APP->GetDocCount() > 1);
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}
void CippiDemoDoc::OnUpdateQuadCopy(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(IsQuad() &&
        DEMO_APP->GetDocCount() > 1);
}
void CippiDemoDoc::OnUpdateParlCopy(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(IsParl() &&
        DEMO_APP->GetDocCount() > 1);
}
void CippiDemoDoc::OnUpdateCenterCopy(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(IsCenterShift() &&
        DEMO_APP->GetDocCount() > 1);
}

//---------------------------------------------------------------------------
// On[Roi/Quad/Parl/Center]Copy performs
// Menu-Contour-[ROI/Quad/Parl/Center]-Copy command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnRoiCopy()
{
     CCopyDlg copyDlg(this, "ROI");
     copyDlg.m_pSrcDoc = this;
     if (copyDlg.DoModal() != IDOK) return;
     IppiRect roi = *(GetRoi());
     CippiDemoDoc* pDstDoc = copyDlg.m_pDstDoc;
     if (roi.x >= pDstDoc->Width()) roi.x = 0;
     if (roi.y >= pDstDoc->Height()) roi.y = 0;
     if (roi.x + roi.width > pDstDoc->Width())
          roi.width = pDstDoc->Width() - roi.x;
     if (roi.y + roi.height > pDstDoc->Height())
          roi.height = pDstDoc->Height() - roi.y;
     pDstDoc->SetRoi(&roi);
     pDstDoc->UpdateView();
}
void CippiDemoDoc::OnQuadCopy()
{
     CCopyDlg copyDlg(this, "Quadrangle");
     copyDlg.m_pSrcDoc = this;
     if (copyDlg.DoModal() != IDOK) return;
    double quad[4][2];
     GetQuad(quad);
     copyDlg.m_pDstImage->SetQuad(quad);
     copyDlg.m_pDstDoc->UpdateView();
}
void CippiDemoDoc::OnParlCopy()
{
     CCopyDlg copyDlg(this, "Parallelogramm");
     copyDlg.m_pSrcDoc = this;
     if (copyDlg.DoModal() != IDOK) return;
    double quad[4][2];
     GetParl(quad);
     copyDlg.m_pDstImage->SetParl(quad);
     copyDlg.m_pDstDoc->UpdateView();
}
void CippiDemoDoc::OnCenterCopy()
{
     CCopyDlg copyDlg(this, "Center & Shift");
     if (copyDlg.DoModal() != IDOK) return;
    double center[2];
     double shift[2];
     GetCenterShift(center,shift);
     copyDlg.m_pDstImage->SetCenterShift(center,shift);
     copyDlg.m_pDstDoc->UpdateView();
}

//---------------------------------------------------------------------------
// OnUpdate[Roi/Quad/Parl/Center]OnUpdateRoiDelete enables
// Menu-Contour-[ROI/Quad/Parl/Center]-OnUpdateRoiDelete command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateRoiDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsRoi());
    if (m_MovieStage != MOVIE_STOP)
        pCmdUI->Enable(FALSE);
}
void CippiDemoDoc::OnUpdateQuadDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsQuad());
}
void CippiDemoDoc::OnUpdateParlDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsParl());
}
void CippiDemoDoc::OnUpdateCenterDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsCenterShift());
}

//---------------------------------------------------------------------------
// On[Roi/Quad/Parl/Center]Delete performs
// Menu-Contour-[ROI/Quad/Parl/Center]-Delete command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnRoiDelete()
{
     ResetRoi();
     UpdateView();
     SetStatusString();
}
void CippiDemoDoc::OnQuadDelete()
{
     ResetQuad();
     UpdateView();
     SetStatusString();
}
void CippiDemoDoc::OnParlDelete()
{
     ResetParl();
     UpdateView();
     SetStatusString();
}
void CippiDemoDoc::OnCenterDelete()
{
     ResetCenterShift();
     UpdateView();
     SetStatusString();
}

//---------------------------------------------------------------------------
// OnUpdateMarkerDelete enables Menu-Contour-DeleteMarkers command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMarkerDelete(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(IsMarker());
}
//---------------------------------------------------------------------------
// OnMarkerDelete performs Menu-Contour-DeleteMarkers command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMarkerDelete()
{
     ResetMarker();
     UpdateView();
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoDoc Movie commands

//---------------------------------------------------------------------------
// CanPlayMovie function
//---------------------------------------------------------------------------
BOOL CippiDemoDoc::CanPlayMovie() {
    if (m_pMovie->GetNumMovies() == 0) return FALSE;
    if (m_pMovie->IsImagePlane()) return FALSE; /// chan. disable movie
    if (m_pMovie->GetType() == MOVIE_TYPE_PERF)
    {
        MY_POSITION pos = DEMO_APP->GetFirstDocPosition( );
        while (pos) 
        {
            CippiDemoDoc *pDoc = (CippiDemoDoc*)DEMO_APP->GetNextDoc(pos);
            if (pDoc == this)
                continue;
            if (pDoc->GetMovieStage())
            {
                return FALSE;
                break;
            }

        }
    }
    return TRUE;
}

//---------------------------------------------------------------------------
// OnUpdateMoviePlay enables Menu-Movie-Play command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMoviePlay(CCmdUI* pCmdUI) {
    pCmdUI->Enable(CanPlayMovie() && (m_MovieStage != MOVIE_PLAY));
}

//---------------------------------------------------------------------------
// OnUpdateMovieStop enables Menu-Movie-Stop command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMovieStop(CCmdUI* pCmdUI) {
    pCmdUI->Enable((m_MovieStage==MOVIE_PLAY || 
        m_pMovie->IsPlaying()) && !m_pMovie->IsImagePlane()); /// chan
}

//---------------------------------------------------------------------------
// OnUpdateMoviePause enables Menu-Movie-Pause command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMoviePause(CCmdUI* pCmdUI) {
    pCmdUI->Enable(m_MovieStage==MOVIE_PLAY);
}

//---------------------------------------------------------------------------
// OnUpdateMovieNext enables Menu-Movie-PlayNext command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMovieNext(CCmdUI* pCmdUI) {
    pCmdUI->Enable(CanPlayMovie() && m_pMovie->GetNumMovies()>1); /// chan
}

//---------------------------------------------------------------------------
// OnUpdateMoviePrev enables Menu-Movie-PlayPrev command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnUpdateMoviePrev(CCmdUI* pCmdUI) {
    pCmdUI->Enable(CanPlayMovie() && m_pMovie->GetNumMovies()>1); /// chan
}

//---------------------------------------------------------------------------
// OnMoviePlay performs Menu-Movie-Play command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMoviePlay()
{
    StartMovie(MOVIE_PLAY);
}

//---------------------------------------------------------------------------
// OnMovieNext performs Menu-Movie-PlayNext command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMovieNext()
{
    StartMovie(MOVIE_PLAYNEXT);
}

//---------------------------------------------------------------------------
// OnMoviePrev performs Menu-Movie-PlayPrev command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMoviePrev()
{
    StartMovie(MOVIE_PLAYPREV);
}

//---------------------------------------------------------------------------
// OnMoviePause performs Menu-Movie-Pause command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMoviePause()
{
    m_MutexFlag.Lock();
    m_MovieStage = MOVIE_PAUSE;
    m_MutexFlag.Unlock();
}

//---------------------------------------------------------------------------
// OnMovieStop performs Menu-Movie-Stop command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnMovieStop()
{
    m_MutexFlag.Lock();
    m_MovieStage = MOVIE_STOP;
    m_MutexFlag.Unlock();
}

//---------------------------------------------------------------------------
// OnWindowFit performs Menu-Window-FitToImage command
//---------------------------------------------------------------------------
void CippiDemoDoc::OnWindowFit()
{
    ((CChildFrame*)GetFrame())->SetImageRect();
    UpdateView();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

//---------------------------------------------------------------------------
// CDemoDoc ReadDropImageHeader implementation
//---------------------------------------------------------------------------
void CDemoDoc::ReadDropImageHeader(const char* pData, ImgHeader* pHeader)
{
    CImage image;
    image.ReadDropHeader(pData);
    *pHeader = image.GetHeader();
}
