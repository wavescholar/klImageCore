/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemo.cpp : Defines the class behaviors for the ippsDemo application.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"

#include "MainFrm.h"
#include "ippsDemoDoc.h"
#include "ippsDemoDocMgr.h"
#include "ChildFrm.h"
#include "TextFrm.h"
#include "CharFrm.h"
#include "ippsDemoView.h"
#include "ippsTextView.h"
#include "ippsCharView.h"

#include "ippsDirector.h"

#include "NewSignal.h"
#include "NewTaps.h"
#include "NewChar.h"

#include "ColorDlg.h"
#include "DragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// CippsDemoApp: application class for IPP Signal Processing Demo
//
/////////////////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CippsDemoApp, CDemoApp)
   //{{AFX_MSG_MAP(CippsDemoApp)
   ON_COMMAND(ID_FILE_NEW, OnFileNew)
   ON_COMMAND(ID_FILE_NEW_SIGNAL, OnFileNewSignal)
   ON_COMMAND(ID_FILE_NEW_TAPS, OnFileNewTaps)
   ON_COMMAND(ID_FILE_NEW_CHAR, OnFileNewChar)
   ON_UPDATE_COMMAND_UI(ID_VIEW_XAXIS, OnUpdateViewXaxis)
   ON_UPDATE_COMMAND_UI(ID_VIEW_YAXIS, OnUpdateViewYaxis)
   ON_COMMAND(ID_VIEW_XAXIS, OnViewXaxis)
   ON_COMMAND(ID_VIEW_YAXIS, OnViewYaxis)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_ALL, OnUpdateZoomAll)
   ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
   ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
   ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
   ON_COMMAND(ID_OPT_COLOR, OnOptColor)
   //}}AFX_MSG_MAP
   // Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippsDemoApp construction

CippsDemoApp::CippsDemoApp() : CDemoApp()
{
   m_pNewSignal = new CNewSignal;
   m_pNewTaps = new CNewTaps;
   m_pNewChar = new CNewChar;
   m_NewIsEmpty = FALSE;
   m_NewView = VIEW_DEMO;
   m_NewType = ppNONE;
   m_NewLength = 0;
   m_XAxis = TRUE;
   m_YAxis = TRUE;
   m_Grid = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CippsDemoApp object

CippsDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

//----------------------------------------------------------------------------
// GetDemoTitle returns the name of the Demo that will be set to the title
// of application and application message boxes
//----------------------------------------------------------------------------
CString CippsDemoApp::GetDemoTitle() const { return "ippSP";}

//----------------------------------------------------------------------------
// GetLibTitle returns the name of IPP Library with the specified index
//----------------------------------------------------------------------------
CString CippsDemoApp::GetLibTitle(int idx) const
{
   switch (idx) {
   case LIB_SP: return "ippSP";
   case LIB_CH: return "ippCH";
   default: return "";
   }
}

//----------------------------------------------------------------------------
// GetLibPrefix returns the prefix in function names of IPP Library 
// with the specified index
//----------------------------------------------------------------------------
CString CippsDemoApp::GetLibPrefix(int idx) const
{
   switch (idx) {
   case LIB_SP: return "ipps";
   case LIB_CH: return "ippch";
   default: return "";
   }
}

//----------------------------------------------------------------------------
// GetLibVersion returns the version of IPP library with the specified index 
//----------------------------------------------------------------------------
const IppLibraryVersion* CippsDemoApp::GetLibVersion(int idx) const
{ 
   switch (idx) {
   case LIB_SP:
#if LIB_SP < LIB_NUM
      return CALL(ippsGetLibVersion,());
#endif
      break;
   case LIB_CH:
#if LIB_CH < LIB_NUM
      return CH_CALL(ippchGetLibVersion,());
#endif
      break;
   }
   return NULL;
}

static CString CustomName(int i)
{
   CString name;
   name.Format("CustomColor%d", i);
   return name;
}

//----------------------------------------------------------------------------
// LoadProfileSettings is called by CDemoApp::InitInstance.
// It loads application settings
//----------------------------------------------------------------------------
void CippsDemoApp::LoadProfileSettings()
{
   CDemoApp::LoadProfileSettings();
   /*
   CippsDemoView::SetColorSignal    (
      GetProfileInt("View","ClrGraph",  
      CippsDemoView::GetColorSignal()));
   CippsDemoView::SetColorSignalBack(
      GetProfileInt("View","ClrGraphBk",
      CippsDemoView::GetColorSignalBack()));
   CippsDemoView::SetColorAxis      (
      GetProfileInt("View","ClrAxis",   
      CippsDemoView::GetColorAxis()));
   CippsDemoView::SetColorAxisBack  (
      GetProfileInt("View","ClrAxisBk", 
      CippsDemoView::GetColorAxisBack()));

   for (int i=0; i<16; i++)
      CColorDlg::SetCustomColor(i, 
         GetProfileInt("View", CustomName(i), 
         CColorDlg::GetCustomColor(i)));
         */
} 

//----------------------------------------------------------------------------
// SaveProfileSettings is called by CDemoApp::InitInstance.
// It saves application settings
//----------------------------------------------------------------------------
void CippsDemoApp::SaveProfileSettings()
{
   CDemoApp::SaveProfileSettings();

   WriteProfileInt("View","ClrGraph",  CippsDemoView::GetColorSignal    ());
   WriteProfileInt("View","ClrGraphBk",CippsDemoView::GetColorSignalBack());
   WriteProfileInt("View","ClrAxis",   CippsDemoView::GetColorAxis      ());
   WriteProfileInt("View","ClrAxisBk", CippsDemoView::GetColorAxisBack  ());

   for (int i=0; i<16; i++)
      WriteProfileInt("View", CustomName(i),
         CColorDlg::GetCustomColor(i));
}

//----------------------------------------------------------------------------
// CreateDirector creates new CDirector class that can process any document 
// by any proper IPP function
//----------------------------------------------------------------------------
CDirector* CippsDemoApp::CreateDirector() const
{
   return new CippsDirector;
}

//----------------------------------------------------------------------------
// CreateDocTemplates creates document templates
//----------------------------------------------------------------------------
void CippsDemoApp::CreateDocTemplates()
{
   m_pDocManager = new CippsDemoDocMgr;
   CMultiDocTemplate* pDocTemplate;
   pDocTemplate = new CMultiDocTemplate(
      IDR_IPPSTYPE,
      RUNTIME_CLASS(CippsDemoDoc),
      RUNTIME_CLASS(CChildFrame),
      RUNTIME_CLASS(CippsDemoView));
   AddDocTemplate(pDocTemplate);

   pDocTemplate = new CMultiDocTemplate(
      IDR_IPPSTEXT,
      RUNTIME_CLASS(CippsDemoDoc),
      RUNTIME_CLASS(CTextFrame), 
      RUNTIME_CLASS(CippsTextView));
   AddDocTemplate(pDocTemplate);

   pDocTemplate = new CMultiDocTemplate(
      IDR_IPPSCHAR,
      RUNTIME_CLASS(CippsDemoDoc),
      RUNTIME_CLASS(CCharFrame), 
      RUNTIME_CLASS(CippsCharView));
   AddDocTemplate(pDocTemplate);

}

/////////////////////////////////////////////////////////////////////////////
// Doc Managing & Creation 

//----------------------------------------------------------------------------
// GetDemoTemplate returns template for document that will be shown in 
// graphic view at its first appearance
//----------------------------------------------------------------------------
CDocTemplate* CippsDemoApp::GetDemoTemplate()
{
   POSITION pos = GetFirstDocTemplatePosition( );
   return GetNextDocTemplate(pos);
}

//----------------------------------------------------------------------------
// GetTextTemplate returns template for document that will be shown in 
// digital view at its first appearance
//----------------------------------------------------------------------------
CDocTemplate* CippsDemoApp::GetTextTemplate()
{
   POSITION pos = GetFirstDocTemplatePosition( );
   GetNextDocTemplate(pos);
   return GetNextDocTemplate(pos);
}

//----------------------------------------------------------------------------
// GetCharTemplate returns template for document that will be shown in 
// character view at its first appearance
//----------------------------------------------------------------------------
CDocTemplate* CippsDemoApp::GetCharTemplate()
{
   POSITION pos = GetFirstDocTemplatePosition( );
   GetNextDocTemplate(pos);
   GetNextDocTemplate(pos);
   return GetNextDocTemplate(pos);
}

//----------------------------------------------------------------------------
// CreateDemoDoc creates document with specified vector type & length 
// and shows it in graphic view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateDemoDoc(ppType type, int len, 
                                  BOOL bMakeVisible, CString title) 
{
   return CreateNewDoc(GetDemoTemplate(),type,len,bMakeVisible,title);
}

//----------------------------------------------------------------------------
// CreateDemoDoc creates document with vector equaled to specified vector
// and shows it in graphic view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateDemoDoc(CVector* pVec, BOOL bMakeVisible, CString title) 
{
    CippsDemoDoc* pDoc = CreateDemoDoc(pVec->Type(), pVec->Length(), bMakeVisible, title);
    if (!pDoc) return NULL;
    pDoc->CopyData(*pVec);
    return pDoc;
}

//----------------------------------------------------------------------------
// CreateTextDoc creates document with specified vector type & length
// and shows it in textual view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateTextDoc(ppType type, int len, 
                                  BOOL bMakeVisible, CString title) 
{
   ppType newType = m_NewType;
   int    newLength = m_NewLength;
   CippsDemoDoc* pDoc = CreateNewDoc(GetTextTemplate(),type,len,bMakeVisible,title);
   m_NewType   = newType  ;
   m_NewLength = newLength;
   return pDoc;
}

//----------------------------------------------------------------------------
// CreateTextDoc creates document with vector equaled to specified vector
// and shows it in textual view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateTextDoc(CVector* pVec, BOOL bMakeVisible, CString title) 
{
    CippsDemoDoc* pDoc = CreateTextDoc(pVec->Type(), pVec->Length(), bMakeVisible, title);
    if (!pDoc) return NULL;
    pDoc->CopyData(*pVec);
    return pDoc;
}

//----------------------------------------------------------------------------
// CreateCharDoc creates document with specified vector type & length
// and shows it in character view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateCharDoc(ppType type, int len, 
                                  BOOL bMakeVisible, CString title) 
{
   ppType newType = m_NewType;
   int    newLength = m_NewLength;
   CippsDemoDoc* pDoc = CreateNewDoc(GetCharTemplate(),type,len,bMakeVisible,title);
   m_NewType   = newType  ;
   m_NewLength = newLength;
   return pDoc;
}

//----------------------------------------------------------------------------
// CreateCharDoc creates document with vector equaled to specified vector
// and shows it in character view
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateCharDoc(CVector* pVec, BOOL bMakeVisible, CString title) 
{
    CippsDemoDoc* pDoc = CreateCharDoc(pVec->Type(), pVec->Length(), bMakeVisible, title);
    if (!pDoc) return NULL;
    pDoc->CopyData(*pVec);
    return pDoc;
}

//----------------------------------------------------------------------------
// CreateNewDoc creates document with specified vector type & length
// and shows it in graphic, digital or character view due to specified template
//----------------------------------------------------------------------------
CippsDemoDoc* CippsDemoApp::CreateNewDoc(CDocTemplate* pTpl, ppType type, int len, 
                                 BOOL bMakeVisible, CString title) 
{
   m_NewIsEmpty = TRUE;
   m_NewType = type;
   m_NewLength = len;

   CippsDemoDoc* pDoc = (CippsDemoDoc*)pTpl->OpenDocumentFile(NULL,bMakeVisible);
   if (pDoc) {
      if (!title.IsEmpty()) pDoc->SetTitle(title);
   }
   if (pDoc)
      pDoc->InitHisto();

   m_NewIsEmpty = FALSE;
   return pDoc;
}

//----------------------------------------------------------------------------
// OpenNewDoc opens new user customized document.
// If newView == VIEW_DEMO then document will be created with signal default
// parameters and will be shown in graphic view;
// if newView == NEW_TEXT then document will be created with digital default
// parameters and will be shown in digital view
// if newView == VIEW_CHAR then document will be created with character default
// parameters and will be shown in character view
//----------------------------------------------------------------------------
void CippsDemoApp::OpenNewDoc(int newView)
{
   POSITION tPos = GetFirstDocTemplatePosition( );
   CDocTemplate* pTpl;
   for (int i=0; i<=newView; i++)
      pTpl = GetNextDocTemplate(tPos);
   ASSERT(pTpl);
   CippsDemoDoc* pDoc = (CippsDemoDoc*)pTpl->OpenDocumentFile(NULL);
   if (pDoc)
      pDoc->InitHisto();
}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

CDemoDoc* CippsDemoApp::CreateDropDoc(const char* pData, int DropObject)
{
   if (!ValidDropHeader(pData, DropObject)) return NULL;
   ppType type;
   int    len ;
   CDemoDoc::ReadDropVectorHeader(pData, type, len);
   return CreateDemoDoc(type, len, FALSE);
}

BOOL CippsDemoApp::ValidDropHeader(const char* pData, int DropObject) const
{
   if (DropObject != DROP_VECTOR) return FALSE;
   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CippsDemoApp message handlers

//----------------------------------------------------------------------------
// OnFileNew performs Toolbar-new command
//----------------------------------------------------------------------------
void CippsDemoApp::OnFileNew() 
{
//   m_NewView = FALSE;
   OpenNewDoc(m_NewView);
}

//----------------------------------------------------------------------------
// OnFileNewSignal performs Menu-File-NewSignal command
//----------------------------------------------------------------------------
void CippsDemoApp::OnFileNewSignal() 
{
   if (!m_pNewSignal->Dialog()) return;
   m_NewView = VIEW_DEMO;
   OpenNewDoc();
}

//----------------------------------------------------------------------------
// OnFileNewTaps performs Menu-File-NewDigits command
//----------------------------------------------------------------------------
void CippsDemoApp::OnFileNewTaps() 
{
   if (!m_pNewTaps->Dialog()) return;
   m_NewView = VIEW_TEXT;
   OpenNewDoc(m_NewView);
}

//----------------------------------------------------------------------------
// OnFileNewChar performs Menu-File-NewCharacters command
//----------------------------------------------------------------------------
void CippsDemoApp::OnFileNewChar()
{
//   if (!m_pNewChar->Dialog()) return;
   m_NewView = VIEW_CHAR;
   OpenNewDoc(m_NewView);
}

//----------------------------------------------------------------------------
// OnUpdateViewXaxis checks Menu-View-XAxis command
//----------------------------------------------------------------------------
void CippsDemoApp::OnUpdateViewXaxis(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_XAxis);
}

//----------------------------------------------------------------------------
// OnUpdateViewYaxis checks Menu-View-YAxis command
//----------------------------------------------------------------------------
void CippsDemoApp::OnUpdateViewYaxis(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_YAxis);
}

//----------------------------------------------------------------------------
// OnViewXaxis performs Menu-View-XAxis command
//----------------------------------------------------------------------------
void CippsDemoApp::OnViewXaxis() 
{
   m_XAxis = !m_XAxis;
   MY_POSITION pos = GetFirstDocPosition();
   while (pos)
      GetNextIppsDoc(pos)->UpdateXAxis();
}

//----------------------------------------------------------------------------
// OnViewYaxis performs Menu-View-YAxis command
//----------------------------------------------------------------------------
void CippsDemoApp::OnViewYaxis() 
{
   m_YAxis = !m_YAxis;
   MY_POSITION pos = GetFirstDocPosition();
   while (pos)
      GetNextIppsDoc(pos)->UpdateYAxis();
}

//----------------------------------------------------------------------------
// OnUpdateViewGrid checks Menu-View-Grid command
//----------------------------------------------------------------------------
void CippsDemoApp::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_XAxis || m_YAxis);
   pCmdUI->SetCheck(m_Grid);
}

//----------------------------------------------------------------------------
// OnViewGrid performs Menu-View-Grid command
//----------------------------------------------------------------------------
void CippsDemoApp::OnViewGrid() 
{
   m_Grid = !m_Grid;
   UpdateAllViews();
}

//----------------------------------------------------------------------------
// OnUpdateZoomAll enables Menu-Zoom-ZoomAllAsActive command
//----------------------------------------------------------------------------
void CippsDemoApp::OnUpdateZoomAll(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(((CWnd*)ACTIVE_FRAME == (CWnd*)ACTIVE_DOC->GetDemoFrame()) &&
                  GetDocCount() > 1);
}

//----------------------------------------------------------------------------
// OnZoomAll performs Menu-Zoom-ZoomAllAsActive command
//----------------------------------------------------------------------------
void CippsDemoApp::OnZoomAll() 
{
   CippsDemoDoc* pActiveDoc = ACTIVE_DOC;
   double scaleW = pActiveDoc->FactorW();
   double scaleH = pActiveDoc->FactorH();
   MY_POSITION pos = GetFirstDocPosition();
   while (pos) {
      CippsDemoDoc* pDoc = GetNextIppsDoc(pos);
      if (pDoc == pActiveDoc) continue;
      pDoc->ZoomByFactors(scaleW,scaleH);
   }
}

//----------------------------------------------------------------------------
// OnOptColor performs Menu-Options-Color command
//----------------------------------------------------------------------------
void CippsDemoApp::OnOptColor() 
{
   CColorDlg dlg;
   if (dlg.DoModal() == IDOK)
      UpdateAllViews();
}
