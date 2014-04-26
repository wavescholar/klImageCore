/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Demo.cpp : implementation of the CDemo class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"
#include "CProcess.h"
#include "Director.h"

#include "MainFrm.h"
#include "OptLibDlg.h"
#include "Timing.h"
#include "AboutDlg.h"
#include "DragDrop.h"
#include "ListPage.h"
#include "Lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// CDemoApp: base class for ippiDemo / ippsDemo MFC applications
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Virtual attributes

//----------------------------------------------------------------------------
// GetVersionString returns current version of Demo application
//----------------------------------------------------------------------------
CString CDemoApp::GetVersionString() const { return _T("7.1.1");}

//----------------------------------------------------------------------------
// GetCopyrightLastYear returns the current year
//----------------------------------------------------------------------------
int CDemoApp::GetCopyrightLastYear() const { return 2012;}

BEGIN_MESSAGE_MAP(CDemoApp, CWinApp)
    //{{AFX_MSG_MAP(CDemoApp)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
   ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    ON_UPDATE_COMMAND_UI(ID_OPT_LIB, OnUpdateOptLib)
    ON_COMMAND(ID_OPT_LIB, OnOptLib)
    ON_UPDATE_COMMAND_UI(ID_OPT_OUTSTAND, OnUpdateOptOutstand)
    ON_COMMAND(ID_OPT_OUTSTAND, OnOptOutstand)
    ON_COMMAND(ID_OPT_TIMING, OnOptTiming)
   ON_UPDATE_COMMAND_UI(ID_PICK_DST, OnUpdatePickDst)
   ON_COMMAND(ID_PICK_DST, OnPickDst)
    ON_UPDATE_COMMAND_UI(ID_PROCESS_STOP, OnUpdateProcessStop)
    ON_COMMAND(ID_PROCESS_STOP, OnProcessStop)
   ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_OPTIONS_LIBRARY, &CDemoApp::OnOptionsLibrary)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_LIBRARY, &CDemoApp::OnUpdateOptionsLibrary)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoApp construction

CDemoApp::CDemoApp()
{
   m_pLib = NULL;
   m_pRecentFuncList = new CRecentFuncList;
   m_pIppList = NULL;;
   m_pDirector = NULL;

   m_OptLibPage = 0;
   m_PickMode = FALSE;
   m_PickDst = FALSE;

   m_hCross = 0;
   m_hArrow = 0;
   m_hWait  = 0;
   m_hPick  = 0;
   m_hGrab  = 0;

   m_pDropSource = NULL;
   m_DropFormat = 0;

   m_CurrProcess = 0;
   m_CurrBook = 0;
   m_CurrChapter = 0;

}

CDemoApp::~CDemoApp()
{
   delete m_pRecentFuncList;
}
/////////////////////////////////////////////////////////////////////////////
// General Demo Operations

//----------------------------------------------------------------------------
// InitInstance is called at the beginning of MFC application.
// Besides of standard MFC initialization it loads IPP libraries and
// creates the list of IPP functions that will be called
//----------------------------------------------------------------------------
BOOL CDemoApp::InitInstance()
{
   CSExcept::InitTranslator();

   AfxEnableControlContainer();
   m_hArrow = LoadStandardCursor(IDC_ARROW);
   m_hCross = LoadStandardCursor(IDC_CROSS);
   m_hWait  = LoadStandardCursor(IDC_WAIT);
   m_hPick = m_hGrab = m_hArrow;


    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
//    SetRegistryKey(_T("Intel\0xAE Corporation"));

//   LoadStdProfileSettings(6);  // Load standard INI file options (including MRU)
   m_pDirector = CreateDirector();
//   LoadProfileSettings();

   CreateDocTemplates();

    // Init drag/drop
   if (OleInitialize(NULL) == S_OK) {
      m_pDropSource = new CDropSource;
      m_DropFormat = ::RegisterClipboardFormat(GetDropFormatString());
   }

    // create main MDI Frame window
   CMainFrame* pMainFrame = new CMainFrame;
   if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
      return FALSE;
   m_pMainWnd = pMainFrame;

    // The main window has been initialized, so show and update it.
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

   MAIN_FRAME->SetMessageText(_T("Loading libraries"));
   InitLibrary();
   MAIN_FRAME->SetMessageText(_T("Preparing function list"));
   InitFunctions();
   SetCurrentFunction();

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        return TRUE;
    }
    DispatchCommands(cmdInfo);
    ProcessShellCommand(cmdInfo);

    OptionsLibraryInit();
    
    MAIN_FRAME->SetMessageText(_T("Ready"));
    return TRUE;
}

//----------------------------------------------------------------------------
// ExitInstance is called at the end of MFC application 
//----------------------------------------------------------------------------
int CDemoApp::ExitInstance() 
{
   OleUninitialize( );
   SaveProfileSettings();
   return CWinApp::ExitInstance();
}

//----------------------------------------------------------------------------
// LoadProfileSettings loads application settings
//----------------------------------------------------------------------------
void CDemoApp::LoadProfileSettings()
{
//   m_CurrBook = GetProfileInt(_T("Main"),_T("Book"),0);
//   m_CurrChapter = GetProfileInt(_T("Main"),_T("Chapter"),0);
//   m_CurrFunc = GetProfileString(_T("Main"),_T("FuncName"),_T(""));
//   m_PickDst       = GetProfileInt(_T("Main"),_T("PickDst"),FALSE);
//
//   m_pLibList->LoadProfileSettings();
//   CTiming::LoadProfileSettings();
} 

//----------------------------------------------------------------------------
// SaveProfileSettings saves application settings
//----------------------------------------------------------------------------
void CDemoApp::SaveProfileSettings()
{
//   CProcess* pProcess = m_pDirector->GetProcess();
//   WriteProfileInt(_T("Main"),_T("Book"),pProcess->GetCurrentBook());
//   WriteProfileInt(_T("Main"),_T("Chapter"),pProcess->GetCurrentChapter());
//   WriteProfileString(_T("Main"),_T("FuncName"),pProcess->GetCurrentFunction());
//   WriteProfileInt(_T("Main"),_T("PickDst"),m_PickDst);
//
//   m_LibList.SaveProfileSettings();
//   CTiming::SaveProfileSettings();
}

//----------------------------------------------------------------------------
// InitLibrary is called by the CDemoApp::InitInstance.
// It loads demonstrated IPP libraries
//----------------------------------------------------------------------------
void CDemoApp::InitLibrary()
{
   m_pLib = new CLib[GetNumLibs()];
   if (m_pLib) {
      for (int i=0; i<GetNumLibs(); i++) {
         m_pLib[i].Init(GetLibPrefix(i));
      }
   }
}

//----------------------------------------------------------------------------
// InitFunctions is called by the CDemoApp::InitInstance.
// It creates the list of demonstrated IPP functions
//----------------------------------------------------------------------------
void CDemoApp::InitFunctions()
{
   m_pIppList = new CFuncList[GetNumLibs()];
   for (int i=0; i<GetNumLibs(); i++) 
      m_pIppList[i].CreateFuncList(i);
   m_pDirector->GetProcess()->InitFunctions();
}

//----------------------------------------------------------------------------
// CreateDirector creates new CDirector class 
//----------------------------------------------------------------------------
CDirector* CDemoApp::CreateDirector() const
{
   return new CDirector;
}

//----------------------------------------------------------------------------
// SetCurrentFunction sets IPP function that will be highlighted at the 
// beginning of Process dialog. Usually it is the last called IPP function.
//----------------------------------------------------------------------------
void CDemoApp::SetCurrentFunction() {
    m_pDirector->GetProcess()->SetCurrentFunction(
       m_CurrProcess, m_CurrBook,m_CurrChapter,m_CurrFunc);
}

/////////////////////////////////////////////////////////////////////////////
// Doc Managing Operations & Attributes

//----------------------------------------------------------------------------
// CreateDocTemplates creates document templates
//----------------------------------------------------------------------------
void CDemoApp::CreateDocTemplates()
{
   CMultiDocTemplate* pDocTemplate;
   pDocTemplate = new CMultiDocTemplate(
      IDR_IPP_TYPE,
      RUNTIME_CLASS(CDemoDoc),
      RUNTIME_CLASS(CMDIChildWnd), 
      RUNTIME_CLASS(CScrollView));
   AddDocTemplate(pDocTemplate);
}

//----------------------------------------------------------------------------
// GetFirstDocPosition returns the first document position to begin
// the iteration process
//----------------------------------------------------------------------------
MY_POSITION CDemoApp::GetFirstDocPosition() const
{
   MY_POSITION pos = {NULL,NULL};

   pos.tpl = GetFirstDocTemplatePosition( );
   if (!pos.tpl) return pos;
   while (pos.tpl) {
      POSITION tPos = pos.tpl;
      CDocTemplate* pTpl = GetNextDocTemplate(tPos);
      pos.doc = pTpl->GetFirstDocPosition( );
      if (pos.doc) break;
      pos.tpl = tPos;
   }
   return pos;
}

//----------------------------------------------------------------------------
// GetNextDoc returns pointer to the current document in the iteration 
// process and sets the next doc position to the pos
//----------------------------------------------------------------------------
CDemoDoc* CDemoApp::GetNextDoc(MY_POSITION& pos) const
{
   if (!pos) return NULL;
   POSITION tPos = pos.tpl;
   CDocTemplate* pTpl = GetNextDocTemplate(tPos);
   CDemoDoc* pDoc = (CDemoDoc*)pTpl->GetNextDoc(pos.doc);
   if (pos.doc == NULL) {
      pos.tpl = tPos;
      while (pos.tpl) {
         CDocTemplate* pTpl = GetNextDocTemplate(tPos);
         pos.doc = pTpl->GetFirstDocPosition( );
         if (pos.doc) break;
         pos.tpl = tPos;
      }
   }
   return pDoc;
}

//----------------------------------------------------------------------------
// GetDocCount returns number of all opened MFC documents
//----------------------------------------------------------------------------
int CDemoApp::GetDocCount() const
{
   MY_POSITION pos = GetFirstDocPosition();
   int n = 0;
   while (pos) {
      GetNextDoc(pos);
      n++;
   }
   return n;
}

//----------------------------------------------------------------------------
// FindDoc returns pointer to the document with the specified title
//----------------------------------------------------------------------------
CDemoDoc* CDemoApp::FindDoc(CString title) const
{
   MY_POSITION pos;
   pos = GetFirstDocPosition( );
   while (pos) {
      CDemoDoc* pDoc = GetNextDoc(pos);
      if (pDoc->GetTitle() == title) return pDoc;
   }
   return NULL;
}

//----------------------------------------------------------------------------
// UpdateAllViews updates all views of all opened MFC documents
//----------------------------------------------------------------------------
void CDemoApp::UpdateAllViews()
{
   MY_POSITION pos = GetFirstDocPosition();
   while (pos)
      GetNextDoc(pos)->UpdateView();
}

/////////////////////////////////////////////////////////////////////////////
// Message Boxes

//----------------------------------------------------------------------------
// MessageBox displays message box using CDemoApp::GetDemoTitle() for title
//----------------------------------------------------------------------------
int CDemoApp::MessageBox(LPCTSTR lpszText, UINT nType, LPCTSTR title) const
{
   return MAIN_FRAME->MessageBox(lpszText, 
      title ? title : AfxGetAppName(), nType);
}

//----------------------------------------------------------------------------
// ErrorMessage displays message box for error information
//----------------------------------------------------------------------------
BOOL CDemoApp::ErrorMessage(CString info) const
{
    MAIN_FRAME->MessageBox(LPCTSTR(info), AfxGetAppName(), MB_OK|MB_ICONERROR);
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes

//----------------------------------------------------------------------------
// GetLib returns pointer to class that serves IPP library with 
// the specified index 
//----------------------------------------------------------------------------
CLib* CDemoApp::GetLib(int idx) const { 
   if (m_pLib && idx < GetNumLibs()) return m_pLib + idx;
   else                              return NULL;
}

//----------------------------------------------------------------------------
// GetLibHandle returns handle of exported IPP DLL with the specified 
// index; it returns NULL if no library is exported (linked library is used 
// in this case)
//----------------------------------------------------------------------------
HINSTANCE CDemoApp::GetLibHandle(int idx) const { 
   if (GetLib(idx) && idx < GetNumLibs())
      return GetLib(idx)->GetHandle();
   else
      return NULL;
}

//----------------------------------------------------------------------------
// GetIppProc returns address of the specified exported IPP DLL function
// with specified library index; it returns NULL if no library is exported
// (common function call should be used in this case) 
//----------------------------------------------------------------------------
FARPROC CDemoApp::GetIppProc(LPCTSTR name, int idx) const { 
   if (GetLib(idx) && idx < GetNumLibs())
      return GetLib(idx)->GetProc(name);
   else
      return NULL;
}

//----------------------------------------------------------------------------
// ForCpu64 returns TRUE if Demo has been built for Win64 architecture
//----------------------------------------------------------------------------
BOOL CDemoApp::ForCpu64() const {
#ifdef WIN64
   return TRUE;
#else
   return FALSE;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Cursor Managing

//----------------------------------------------------------------------------
// RegCursorPick registers cursor which appears while vector or (image) 
// processing on the pick up document stage. Pick cursor means that some 
// document have to be grabbed but not the document beneath cursor. This cursor 
// varies depending on wanted parameter name of IPP function
//----------------------------------------------------------------------------
void CDemoApp::RegCursorPick(UINT id)
{
   m_hPick = LoadCursor(id);
}

//----------------------------------------------------------------------------
// RegCursorGrab registers cursor which appears while vector or (image) 
// processing on the pick up document stage. Grab cursor means that the 
// document beneath cursor may be grabbed for processing. This cursor varies 
// depending on wanted parameter name of IPP function
//----------------------------------------------------------------------------
void CDemoApp::RegCursorGrab(UINT id) 
{
   m_hGrab = LoadCursor(id);
}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

//----------------------------------------------------------------------------
// GetDropFormatString returns Drop Format string
//----------------------------------------------------------------------------
CString CDemoApp::GetDropFormatString() const
{
   return _T("IPP Vector");
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp message handlers

//----------------------------------------------------------------------------
// OnAppAbout displays About dialog
//----------------------------------------------------------------------------
void CDemoApp::OnAppAbout()
{
   CAboutDlg dlg;
   dlg.DoModal();
}

//----------------------------------------------------------------------------
// OnFileOpen opens file
//----------------------------------------------------------------------------
void CDemoApp::OnFileOpen() 
{  
   CWinApp::OnFileOpen();
}

//----------------------------------------------------------------------------
// OnUpdateOptionsLibrary enables Menu-Options-Library command
//----------------------------------------------------------------------------
void CDemoApp::OnUpdateOptionsLibrary(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
}

//----------------------------------------------------------------------------
// OnOptionsLibrary performs Menu-Options-Library command
//----------------------------------------------------------------------------

void CDemoApp::OptionsLibraryInit()
{
    m_Optimization = 1;
    m_NumThreadsMode = 0;
    ippGetNumThreads(&m_NumThreads_Optimal);
    m_NumThreads = m_NumThreads_Optimal;

    m_NumThreads = 1;
    m_NumThreadsMode = 1;
    ippSetNumThreads(m_NumThreads);
}

void CDemoApp::OnOptionsLibrary()
{
    COptLibDlg dlg;
    dlg.m_NumThreads_Optimal = m_NumThreads_Optimal;
    dlg.m_NumThreads = m_NumThreads;
    dlg.m_NumThreadsMode = m_NumThreadsMode;
    dlg.m_Optimization = m_Optimization;
    if (dlg.DoModal() == IDOK)
    {
        BOOL bAbout = FALSE;
        BOOL bChanged = FALSE;
        if (m_Optimization != dlg.m_Optimization)
        {
            bChanged = TRUE;
            bAbout = TRUE;
            IppStatus status = ippStsNoErr;
            if (dlg.m_Optimization)
            {
                status = ippInit();
                //CRun::IppErrorMessage("ippInit", status);
            }
            else
            {
                status = ippInitCpu(ippCpuPP);
                //CRun::IppErrorMessage("ippInitCpu", status);
            }
            //if (status >= 0)
            {
                m_Optimization = dlg.m_Optimization;
            }
        }
        if (m_NumThreads != dlg.m_NumThreads)
        {
            bChanged = TRUE;
            IppStatus status = ippSetNumThreads(dlg.m_NumThreads);
            CRun::IppErrorMessage("ippSetNumThreads", status);
            if (status >= 0)
            {
                ippGetNumThreads(&m_NumThreads);
                m_NumThreadsMode = dlg.m_NumThreadsMode;
            }
        }
        else
        {
            m_NumThreadsMode = dlg.m_NumThreadsMode;
        }

        if (bChanged)
            OnLibraryChange();
        if (bAbout)
            OnAppAbout();
    }
}
        
void CDemoApp::OnLibraryChange()
{
}


void CDemoApp::OnUpdateOptLib(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(ForCpu64() || (ippGetCpuType() < ippCpuITP));
}

void CDemoApp::OnOptLib() 
{
   //COptLibSheet sheet(_T("Choose IPP dll to use"));
   //sheet.CreatePages(m_pLib,GetNumLibs());
   //sheet.m_startPage = m_OptLibPage;
   //if (sheet.DoModal() != IDOK)
   //   return;
   //m_OptLibPage = sheet.m_startPage;
   //for (int i=0; i<GetNumLibs(); i++) {
   //   COptLibPage* pPage = (COptLibPage*)sheet.GetPage(i);
   //   m_pLib[i].ChangeLib(pPage->m_LibType, pPage->m_PathStr);
   //}      
}

//----------------------------------------------------------------------------
//  OnUpdateOptOutstand enables Menu-Options-Outstanding command
//----------------------------------------------------------------------------
void CDemoApp::OnUpdateOptOutstand(CCmdUI* pCmdUI) 
{
   int count = 0;
   for (int i=0; i<GetNumLibs(); i++)
      count += (int)(m_pIppList[i].GetOutstandList()->GetCount());
   pCmdUI->Enable(count);
}

//----------------------------------------------------------------------------
//  OnOptOutstand performs Menu-Options-Outstanding command
//----------------------------------------------------------------------------
void CDemoApp::OnOptOutstand() 
{
   int i;
   CPropertySheet sheet(_T("Outstanding functions"));
   for (i=0; i<GetNumLibs(); i++) {
      CFuncList* pList = m_pIppList[i].GetOutstandList();
      if (pList->GetCount() > 0) {
         CListPage* pPage = new CListPage(GetLibTitle(i));
         pPage->m_pFuncList = pList;
         pPage->m_Name = GetLibTitle(i);
         sheet.AddPage(pPage);
      }
   }
   sheet.DoModal();
   for (i=0; i<sheet.GetPageCount(); i++) {
      delete sheet.GetPage(i);
   }
}

//----------------------------------------------------------------------------
//  OnOptTiming performs Menu-Options-Timing command
//----------------------------------------------------------------------------
void CDemoApp::OnOptTiming() 
{
   CPerf::DoTimingDialog();
}

//----------------------------------------------------------------------------
//  OnUpdateProcessStop enables Menu-Process-StopProcessing command
//----------------------------------------------------------------------------
void CDemoApp::OnUpdateProcessStop(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(GetPickMode());
}

//----------------------------------------------------------------------------
//  OnProcessStop performs Menu-Process-StopProcessing command
//----------------------------------------------------------------------------
void CDemoApp::OnProcessStop() 
{
   GetDirector()->StopProcess();
}

//----------------------------------------------------------------------------
//  OnUpdatePickDst enables Toolbar-PickDst button
//----------------------------------------------------------------------------
void CDemoApp::OnUpdatePickDst(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(GetDocCount());
   pCmdUI->SetCheck(m_PickDst);
}

//----------------------------------------------------------------------------
//  OnPickDst performs Toolbar-PickDst command
//----------------------------------------------------------------------------
void CDemoApp::OnPickDst() 
{
   m_PickDst = !m_PickDst;
   if (!m_PickDst && m_pDirector->GetRun() && m_pDirector->GetRun()->IsPickVecDst())
      m_pDirector->PickNext();
}

//----------------------------------------------------------------------------
//  OnWindowCloseall performs Menu-Window-CloseAll command
//----------------------------------------------------------------------------
void CDemoApp::OnWindowCloseall() 
{
   CloseAllDocuments(FALSE);
}

//----------------------------------------------------------------------------
// OnIdle performs idle-time processing
//----------------------------------------------------------------------------
BOOL CDemoApp::OnIdle(LONG lCount) 
{
   BOOL result = CWinApp::OnIdle(lCount);
   CDemoDoc* pDoc = ACTIVE_DOC;
   if (!pDoc) return result;
   CWnd* pMessageBar = MAIN_FRAME->GetMessageBar();
   CString message;
   pMessageBar->GetWindowText(message);
   if (message == "Ready")
      pDoc->SetStatusString();
   return result;
}
