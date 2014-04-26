/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ippiDemo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ippiDemoDoc.h"
#include "ippiDemoView.h"
#include "ippiDirector.h"

#include "ImgNewDlg.h"
#include "OptDstDlg.h"

#include "DragDrop.h"

#include "Movie.h"
#include "MovieFuncDlg.h"
#include "MovieMaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// CippiDemoApp: application class for ippi Demo
//
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CippiDemoApp, CDemoApp)
    //{{AFX_MSG_MAP(CippiDemoApp)
    ON_COMMAND(ID_FILE_NEW_DLG, OnFileNewDlg)
    ON_COMMAND(ID_FILE_NEW, OnFileNew)
    ON_COMMAND(ID_CONTOUR_SHOW, OnContourShow)
    ON_UPDATE_COMMAND_UI(ID_CONTOUR_SHOW, OnUpdateContourShow)
    ON_COMMAND(ID_OPT_DST, OnOptDst)
    ON_UPDATE_COMMAND_UI(ID_OPT_LIB, OnUpdateOptLib)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_ARG, OnUpdateMovieArg)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_FASTARG, OnUpdateMovieFastArg)
    ON_COMMAND(ID_MOVIE_ARG, OnMovieArg)
    ON_COMMAND(ID_MOVIE_FASTARG, OnMovieFastArg)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_ZOOM, OnUpdateMovieZoom)
    ON_COMMAND(ID_MOVIE_ZOOM, OnMovieZoom)
    ON_UPDATE_COMMAND_UI(ID_MOVIE_WANDER, OnUpdateMovieWander)
    ON_COMMAND(ID_MOVIE_WANDER, OnMovieWander)
    ON_COMMAND(ID_MOVIE_MASK, OnMovieMask)
    ON_COMMAND(ID_OPT_MOVIE, OnOptMovie)
    ON_COMMAND(ID_OPT_BORDER, OnOptBorder)
    //}}AFX_MSG_MAP
    // Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippiDemoApp construction

CippiDemoApp::CippiDemoApp() : CDemoApp()
{
    m_NewPattern = SET_JAEHNE;
    m_NewSize = NEWSIZE_ROI;
    m_ContourShow = TRUE;

    m_NewHeader.type = pp8u;
    m_NewHeader.channels = 3;
    m_NewHeader.width = 256;
    m_NewHeader.height = 256;
    for (int c=0; c<4; c++) {
        m_NewHeader.widthP [c] = 256;
        m_NewHeader.heightP[c] = 256;
    }

    m_NewHeader.plane = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CippiDemoApp object

CippiDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

//----------------------------------------------------------------------------
// GetLibTitle returns the name of IPP Library with the specified index
//----------------------------------------------------------------------------
CString CippiDemoApp::GetLibTitle(int idx) const
{
    switch (idx) {
    case LIB_I: return "ippIP";
    case LIB_J: return "ippJP";
    case LIB_CC: return "ippCC";
    case LIB_CV:return "ippCV";
    default: return "";
    }
}

//----------------------------------------------------------------------------
// GetLibPrefix returns the prefix in function names of IPP Library
// with the specified index
//----------------------------------------------------------------------------
CString CippiDemoApp::GetLibPrefix(int idx) const
{
    switch (idx) {
    case LIB_I: return "ippi";
    case LIB_J: return "ippj";
    case LIB_CC: return "ippcc";
    case LIB_CV:return "ippcv";
    default: return "";
    }
}

//----------------------------------------------------------------------------
// GetLibVersion returns the version of IPP library with the specified index
//----------------------------------------------------------------------------
const IppLibraryVersion* CippiDemoApp::GetLibVersion(int idx) const
{
    switch (idx) {
    case LIB_I:
#if LIB_I < LIB_NUM
        return CALL(ippiGetLibVersion,());
#endif
        break;
    case LIB_J:
#if LIB_J < LIB_NUM
        return J_CALL(ippjGetLibVersion,());
#endif
        break;
    case LIB_CC:
#if LIB_CC < LIB_NUM
        return CC_CALL(ippccGetLibVersion,());
#endif
        break;
    case LIB_CV:
#if LIB_CV < LIB_NUM
        return CV_CALL(ippcvGetLibVersion,());
#endif
        break;
    }
    return NULL;
}

//----------------------------------------------------------------------------
// InitInstance is called at the beginning of MFC application.
// Besides of DemoApp initialization it initialize movie settings
//----------------------------------------------------------------------------
BOOL CippiDemoApp::InitInstance()
{
    CMovie::InitStatic();
    if (!CDemoApp::InitInstance()) return FALSE;

    return TRUE;
}

//----------------------------------------------------------------------------
// LoadProfileSettings is called by CDemoApp::InitInstance.
// It loads application settings
//----------------------------------------------------------------------------
void CippiDemoApp::LoadProfileSettings()
{
    CDemoApp::LoadProfileSettings();

    m_NewSize = GetProfileInt("Main","NewSize",NEWSIZE_ROI);

    CMovie::SetType(GetProfileInt("Player","Type",MOVIE_TYPE_ARG));
    CMovie::SetCycle(GetProfileInt("Player","Cycle",120));
    CMovie::SetHotString(GetProfileString("Player","HotString","-"));
    CMovie::GetMaskArray()->SetCurIndex(GetProfileInt("Player","MaskIdx",0));
}

//----------------------------------------------------------------------------
// SaveProfileSettings saves application settings
//----------------------------------------------------------------------------
void CippiDemoApp::SaveProfileSettings()
{
    CDemoApp::SaveProfileSettings();
    WriteProfileInt("Main","NewSize",m_NewSize);

    WriteProfileInt("Player","Type",CMovie::GetType());
    WriteProfileInt("Player","Cycle",CMovie::GetCycle());
    WriteProfileString("Player","HotString",CMovie::GetHotString());
    WriteProfileInt("Player","MaskIdx",CMovie::GetMaskArray()->GetCurIndex());
}

//----------------------------------------------------------------------------
// DispatchCommands is called by CDemoApp::InitInstance after
// ParseCommandLine
//----------------------------------------------------------------------------
void CippiDemoApp::DispatchCommands(CCommandLineInfo& cmdInfo)
{
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
}

//----------------------------------------------------------------------------
// CreateDirector creates new CDirector class that can process any document
// by any proper IPP function
//----------------------------------------------------------------------------
CDirector* CippiDemoApp::CreateDirector() const
{
    return new CippiDirector;
}

//----------------------------------------------------------------------------
// CreateDocTemplates creates document templates
//----------------------------------------------------------------------------
void CippiDemoApp::CreateDocTemplates()
{
    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(
        IDR_IMAGETYPE,
        RUNTIME_CLASS(CippiDemoDoc),
        RUNTIME_CLASS(CChildFrame), // custom MDI child frame
        RUNTIME_CLASS(CippiDemoView));
    AddDocTemplate(pDocTemplate);
}

/////////////////////////////////////////////////////////////////////////////
// Doc Managing & Creation

//----------------------------------------------------------------------------
// CreateDoc creates new image document according to specified image header
//----------------------------------------------------------------------------
CippiDemoDoc* CippiDemoApp::CreateDoc(ImgHeader header, CString title, BOOL bMakeVisible)
{
    ImgHeader saveHeader = m_NewHeader;  m_NewHeader = header;
    int saveData = m_NewPattern;  m_NewPattern = SET_GRAY;

    POSITION tPos = GetFirstDocTemplatePosition( );
    CDocTemplate* pTpl = GetNextDocTemplate(tPos);
    CippiDemoDoc* pDoc = (CippiDemoDoc*)pTpl->OpenDocumentFile(NULL,bMakeVisible);
    if (pDoc) {
        if (!title.IsEmpty()) pDoc->SetTitle(title);
    }

    m_NewHeader = saveHeader;
    m_NewPattern = saveData;
    return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

CDemoDoc* CippiDemoApp::CreateDropDoc(const char* pData, int DropObject)
{
    if (!ValidDropHeader(pData, DropObject)) return NULL;
    ImgHeader header;
    CDemoDoc::ReadDropImageHeader(pData, &header);
    return CreateDoc(header, "", FALSE);
}

BOOL CippiDemoApp::ValidDropHeader(const char* pData, int DropObject) const
{
    if (DropObject != DROP_IMAGE) return FALSE;
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CippiDemoApp message handlers

//----------------------------------------------------------------------------
//  OnFileNewDlg displays New Image dialog and creates new image document
//----------------------------------------------------------------------------
void CippiDemoApp::OnFileNewDlg()
{
    CImgNewDlg dlg;
    dlg.m_Header = m_NewHeader;
    dlg.m_Pattern = m_NewPattern;
    if (dlg.DoModal() != IDOK) return;
    m_NewHeader = dlg.m_Header;
    m_NewPattern = dlg.m_Pattern;
    CWinApp::OnFileNew();
}

//----------------------------------------------------------------------------
//  OnFileNew creates new image document
//----------------------------------------------------------------------------
void CippiDemoApp::OnFileNew()
{
    CWinApp::OnFileNew();
}

//----------------------------------------------------------------------------
//  OnUpdateContourShow enables Menu-View-ShowContours command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateContourShow(CCmdUI* pCmdUI)
{
     pCmdUI->SetCheck(m_ContourShow);
}

//----------------------------------------------------------------------------
//  OnContourShow performs Menu-View-ShowContours command
//----------------------------------------------------------------------------
void CippiDemoApp::OnContourShow()
{
     m_ContourShow = !m_ContourShow;

     MY_POSITION pos = GetFirstDocPosition();
     while (pos) {
          CippiDemoDoc *pDoc = (CippiDemoDoc*)GetNextDoc(pos);
          pDoc->IsUpdated(TRUE);
          pDoc->UpdateView();
     }
}

//----------------------------------------------------------------------------
// OnUpdateOptLib enables Menu-Options-Library command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateOptLib(CCmdUI* pCmdUI)
{
    BOOL flag = TRUE;
    MY_POSITION pos = GetFirstDocPosition();
    while (pos) {
        CippiDemoDoc *pDoc = (CippiDemoDoc*)GetNextDoc(pos);
        if (pDoc->GetMovieStage()) {
            flag = FALSE;
            break;
        }
    }
    pCmdUI->Enable(flag);
}

//----------------------------------------------------------------------------
//  OnOptDst performs Menu-Options-NewDst command
//----------------------------------------------------------------------------
void CippiDemoApp::OnOptDst()
{
     COptDstDlg dlg;
     dlg.m_PickDst = m_PickDst;
     dlg.m_NewSize = m_NewSize;

     if (dlg.DoModal() != IDOK) return;

     m_PickDst = dlg.m_PickDst;
     m_NewSize = dlg.m_NewSize;
}

//----------------------------------------------------------------------------
//  OnOptBorder performs Menu-Options-Borders command
//----------------------------------------------------------------------------
void CippiDemoApp::OnOptBorder()
{
    CippiRun::DoOptBorderDlg();
}

//----------------------------------------------------------------------------
// OnUpdateMovieArg checks Menu-Movie-Modify command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateMovieArg(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(CMovie::GetType() == MOVIE_TYPE_ARG);
}

//----------------------------------------------------------------------------
// OnUpdateMovieFastArg checks Menu-Movie-FastModify command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateMovieFastArg(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(CMovie::GetType() == MOVIE_TYPE_PERF);
}

//----------------------------------------------------------------------------
// OnUpdateMovieZoom checks Menu-Movie-Pulse command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateMovieZoom(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(CMovie::GetType() == MOVIE_TYPE_ZOOM);
}

//----------------------------------------------------------------------------
// OnUpdateMovieWander checks Menu-Movie-Wander command
//----------------------------------------------------------------------------
void CippiDemoApp::OnUpdateMovieWander(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(CMovie::GetType() == MOVIE_TYPE_WANDER);
}

//----------------------------------------------------------------------------
// OnMovieArg performs Menu-Movie-Modify command
//----------------------------------------------------------------------------
void CippiDemoApp::OnMovieArg()
{
    CMovie::SetType(MOVIE_TYPE_ARG);
}

//----------------------------------------------------------------------------
// OnMovieFastArg performs Menu-Movie-FastModify command
//----------------------------------------------------------------------------
void CippiDemoApp::OnMovieFastArg()
{
    CMovie::SetType(MOVIE_TYPE_PERF);
}

//----------------------------------------------------------------------------
// OnMovieZoom performs Menu-Movie-Pulse command
//----------------------------------------------------------------------------
void CippiDemoApp::OnMovieZoom()
{
    CMovie::SetType(MOVIE_TYPE_ZOOM);
}

//----------------------------------------------------------------------------
// OnMovieWander performs Menu-Movie-Wander command
//----------------------------------------------------------------------------
void CippiDemoApp::OnMovieWander()
{
    CMovie::SetType(MOVIE_TYPE_WANDER);
}

//---------------------------------------------------------------------------
// OnMovieMask performs Menu-Movie-Mask command
//---------------------------------------------------------------------------
void CippiDemoApp::OnMovieMask()
{
    CMovieMaskDlg dlg;
    dlg.DoModal();
}

//---------------------------------------------------------------------------
// OnOptMovie performs Menu-Movie-Customize command
//---------------------------------------------------------------------------
void CippiDemoApp::OnOptMovie()
{
    CMovieFuncDlg dlg;
    if (dlg.DoModal() != IDOK) return;
    MY_POSITION pos = GetFirstDocPosition( );
    while (pos) {
        CippiDemoDoc *pDoc = (CippiDemoDoc*)GetNextDoc(pos);
        pDoc->GetMovie()->ResetInitFlag();
    }
}

          
void CippiDemoApp::OnLibraryChange()
{
    MY_POSITION pos = GetFirstDocPosition( );
    while (pos) {
        CippiDemoDoc *pDoc = (CippiDemoDoc*)GetNextDoc(pos);
        pDoc->GetMovie()->ResetClocks();
    }
}
