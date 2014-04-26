/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// demoDoc.cpp : implementation of the CDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"

#include "DemoDoc.h"
#include "Vector.h"
#include "MainFrm.h"
#include "CProcess.h"
#include "Director.h"
#include "Histo.h"
#include "FileHistoDlg.h"
#include "DragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc: base class for Demo MFC document

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
   //{{AFX_MSG_MAP(CDemoDoc)
   ON_COMMAND(ID_FILE_HISTO, OnFileHisto)
   ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
   ON_UPDATE_COMMAND_UI(ID_PROCESS, OnUpdateProcess)
   ON_COMMAND(ID_PROCESS, OnProcess)
   ON_UPDATE_COMMAND_UI(ID_MRU_FUNC1, OnUpdateMruFunc1)
   ON_COMMAND(ID_MRU_FUNC1, OnMruFunc1)
   ON_COMMAND(ID_MRU_FUNC2, OnMruFunc2)
   ON_COMMAND(ID_MRU_FUNC3, OnMruFunc3)
   ON_COMMAND(ID_MRU_FUNC4, OnMruFunc4)
   ON_COMMAND(ID_MRU_FUNC5, OnMruFunc5)
   ON_COMMAND(ID_MRU_FUNC6, OnMruFunc6)
   ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
   //}}AFX_MSG_MAP
   // Enable default OLE container implementation
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc() : CDocument()
{
   m_pProcess = NULL;
   m_MayBePicked = FALSE;
   m_IsPicked = FALSE;
}

CDemoDoc::~CDemoDoc()
{
   DEMO_APP->GetDirector()->DeleteProcess(m_pProcess);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc operations & attributes

//---------------------------------------------------------------------------
// GetView returns first document view
//---------------------------------------------------------------------------
CView* CDemoDoc::GetView() 
{
   POSITION pos = GetFirstViewPosition();
   return GetNextView(pos);
}

//---------------------------------------------------------------------------
// GetFrame returns first document frame window
//---------------------------------------------------------------------------
CMDIChildWnd* CDemoDoc::GetFrame() 
{
   return (CMDIChildWnd*)(GetView() ? GetView()->GetParentFrame() : NULL);
}

//---------------------------------------------------------------------------
// GetHisto returns pointer to document history
//---------------------------------------------------------------------------
CHisto* CDemoDoc::GetHisto()
{
   return GetVector()->GetHisto();
}

//---------------------------------------------------------------------------
// UpdateView updates specified document view or all views
//---------------------------------------------------------------------------
void CDemoDoc::UpdateView(CView* pSender)
{
   GetView()->InvalidateRect(NULL);
   UpdateAllViews(pSender);
}

//---------------------------------------------------------------------------
// Activate activates specified document frame window or the first one
//---------------------------------------------------------------------------
void CDemoDoc::Activate(CMDIChildWnd* pFrame)
{
   if (m_bEmbedded) {
      GetView()->OnInitialUpdate();
      m_bEmbedded = FALSE;
   }
   UpdateView();
   SetStatusString();

   if (!pFrame)
      pFrame = GetFrame();
   ASSERT(pFrame);
   pFrame->ShowWindow(SW_SHOW);
   GetFrame()->ActivateFrame();
   MAIN_FRAME->MDIActivate(pFrame);
}

//---------------------------------------------------------------------------
// CreateNewData creates new vector(image) with default parameters. It has
// empty content in CDemo class
//---------------------------------------------------------------------------
BOOL CDemoDoc::CreateNewData()
{
   return TRUE;
}

//---------------------------------------------------------------------------
// IsData returns weather vector data is allocated or not
//---------------------------------------------------------------------------
BOOL CDemoDoc::IsData() 
{
   return GetVector()->GetData() ? TRUE : FALSE;
}

//---------------------------------------------------------------------------
// CreateProcess creates class that will be process this document by proper 
// IPP functions
//---------------------------------------------------------------------------
void CDemoDoc::CreateProcess() 
{
   DEMO_APP->GetDirector()->DeleteProcess(m_pProcess);
   m_pProcess = DEMO_APP->GetDirector()->NewProcess(GetVector());
   MarkToPick();
}

//---------------------------------------------------------------------------
// ProcessVector processes attached vector by IPP function that will be 
// obtained by Process Dialog
//---------------------------------------------------------------------------
void CDemoDoc::ProcessVector() 
{
   DEMO_APP->GetDirector()->ProcessVector(this);
}

//---------------------------------------------------------------------------
// ProcessVector processes attached vector by IPP function that will be 
// got from MRU func list
//---------------------------------------------------------------------------
void CDemoDoc::ProcessVector(int mruIndex) 
{
   DEMO_APP->GetDirector()->ProcessVector(this,
      DEMO_APP->GetRecentFuncList()->GetFunc(mruIndex));
}

//---------------------------------------------------------------------------
// CanEnableProcess returns weather menu process item should be enabled
//---------------------------------------------------------------------------
BOOL CDemoDoc::CanEnableProcess()
{
   if (DEMO_APP->GetPickMode())
      return FALSE;
   else
      return m_pProcess->GetFuncCount();
}

//---------------------------------------------------------------------------
// CanUpdateMru returns weather MRU function menu should be updated
//---------------------------------------------------------------------------
BOOL CDemoDoc::CanUpdateMru() 
{
   return !DEMO_APP->GetPickMode();
}

//---------------------------------------------------------------------------
// MarkToPick marks weather document can be picked as wanted IPP function parameter
//---------------------------------------------------------------------------
void CDemoDoc::MarkToPick()
{
   CRun* pRun = DEMO_APP->GetDirector()->GetRun();
   if (pRun)
      pRun->PickMarkDoc(this);
}

//---------------------------------------------------------------------------
// InitHisto initializes document history
//---------------------------------------------------------------------------
void CDemoDoc::InitHisto()
{ 
   GetVector()->GetHisto()->RemoveAll(); 
   GetVector()->GetHisto()->AddTail(GetTitle());
}

//---------------------------------------------------------------------------
// UpdateFrameTitle updates document title
//---------------------------------------------------------------------------
void CDemoDoc::UpdateFrameTitle(CString lastFunc)
{
   CString frameTitle = 
      GetTitle() + _T(" - ") +
      GetFrameType();
   if (!lastFunc.IsEmpty())
      m_LastFunc = lastFunc;
   if (!m_LastFunc.IsEmpty())
      frameTitle += _T(" - ") + m_LastFunc;

   m_FrameTitle = frameTitle;
   GetFrame()->SetWindowText(frameTitle);
   SetStatusString();
}

//---------------------------------------------------------------------------
// GetDocStatus returns information be set to main window status string when this document
// is active
//---------------------------------------------------------------------------
CString CDemoDoc::GetDocStatus()
{
   return GetActualFrameTitle();
}

//---------------------------------------------------------------------------
// Get(Set)FrameTitle returns(sets) string to be set to document frame title 
//---------------------------------------------------------------------------
CString CDemoDoc::GetActualFrameTitle() const
{
   if (m_FrameTitle.IsEmpty())
      return GetTitle();
   else
      return m_FrameTitle;
}

//---------------------------------------------------------------------------
// GetFrameType returns vector type string to be insert to document frame title 
//---------------------------------------------------------------------------
CString CDemoDoc::GetFrameType()
{
   return GetVector()->TypeString();
}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

void CDemoDoc::GetDropTitle(CDropTitle* pTitle)
{
   pTitle->TitleSize = sizeof(CDropTitle);
   pTitle->HeaderSize = GetVector()->GetDropHeaderSize();
   pTitle->InfoSize = GetVector()->GetDropInfoSize();
   pTitle->DataSize = GetVector()->GetDropDataSize();
   pTitle->DropObject = GetImage() ? DROP_IMAGE : DROP_VECTOR;
#ifdef UNICODE
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   wcscpy_s((unsigned short*)pTitle->DocName, GetTitle());
   wcscpy_s((unsigned short*)pTitle->AppName, DEMO_APP->GetDemoTitle() + _T(" Demo"));
#else
   wcscpy((unsigned short*)pTitle->DocName, GetTitle());
   wcscpy((unsigned short*)pTitle->AppName, DEMO_APP->GetDemoTitle() + _T(" Demo"));
#endif
#else
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   strcpy_s(pTitle->DocName, GetTitle());
   strcpy_s(pTitle->AppName, DEMO_APP->GetDemoTitle() + _T(" Demo"));
#else
   strcpy(pTitle->DocName, GetTitle());
   strcpy(pTitle->AppName, DEMO_APP->GetDemoTitle() + _T(" Demo"));
#endif
#endif
}

void CDemoDoc::SetDropTitle(const CDropTitle* pTitle)
{
   GetHisto()->AddHead((CString)pTitle->AppName + _T(" - ") + pTitle->DocName);
}

BOOL CDemoDoc::ValidDropTitle(const CDropTitle* pDropTitle)
{
   CDropTitle thisTitle;
   GetDropTitle(&thisTitle);
   return 
      (pDropTitle->TitleSize  <= thisTitle.TitleSize ) &&
      (pDropTitle->HeaderSize <= thisTitle.HeaderSize) &&
      (pDropTitle->InfoSize   <= thisTitle.InfoSize  ) &&
      (pDropTitle->DataSize   == thisTitle.DataSize  );
}

void CDemoDoc::ReadDropVectorHeader(const char* pData, ppType& type, int& len)
{
   CVector vector;
   vector.ReadDropHeader(pData);
   type = vector.Type();
   len = vector.Length();
}

//////////////////////////////////////////////////////////////////
// CDemoDoc MFC Overrides

//---------------------------------------------------------------------------
// OnNewDocument implements New File command
//---------------------------------------------------------------------------
BOOL CDemoDoc::OnNewDocument()
{
   if (!CDocument::OnNewDocument())
      return FALSE;
   if (!CreateNewData())
      return FALSE;
   CreateProcess();
   return TRUE;
}

//---------------------------------------------------------------------------
// OnOpenDocument implements Open File command
//---------------------------------------------------------------------------
BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
   if (!CDocument::OnOpenDocument(lpszPathName))
      return FALSE;
   if (!IsData()) {
      //CString message;
      //message.Format(_T("Can't open %s: file is empty"), lpszPathName);
      //AfxMessageBox(message);
      return FALSE;
   }
   CreateProcess();
   return TRUE;
}

//---------------------------------------------------------------------------
// OnCloseDocument implements Close File command
//---------------------------------------------------------------------------
void CDemoDoc::OnCloseDocument() 
{
   CDocument::OnCloseDocument();
   if (DEMO_APP->GetDocCount() == 0)
      MAIN_FRAME->SetMessageText(_T("Ready"));
}

//---------------------------------------------------------------------------
// OnFileSaveAs implements SaveAs File command
//---------------------------------------------------------------------------
void CDemoDoc::OnFileSaveAs() 
{
   CDocument::OnFileSaveAs();
   UpdateFrameTitle();
}

//---------------------------------------------------------------------------
// CanCloseFrame returns weather document can be closed or not
//---------------------------------------------------------------------------
BOOL CDemoDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
   if (IsPicked()) {
      AfxMessageBox(
         _T("Can't close ") + GetTitle() + _T(": it is picked for function\n"));
      return FALSE;
   }
   return CDocument::CanCloseFrame(pFrame);
}


/////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
   CDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

//---------------------------------------------------------------------------
// OnFileHisto performs Menu-File-History command
//---------------------------------------------------------------------------
void CDemoDoc::OnFileHisto() 
{
   CFileHistoDlg dlg;
   dlg.m_Title = GetTitle();    
   dlg.m_pHistoList = GetVector()->GetHisto();
   dlg.DoModal();
}

//---------------------------------------------------------------------------
// OnUpdateProcess enables Menu-Process-ProcessBy command
//---------------------------------------------------------------------------
void CDemoDoc::OnUpdateProcess(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(CanEnableProcess());
}

//---------------------------------------------------------------------------
// OnProcess performs Menu-Process-ProcessBy command
//---------------------------------------------------------------------------
void CDemoDoc::OnProcess() 
{
   ProcessVector();
}

//---------------------------------------------------------------------------
// OnUpdateMruFunc1 updates Menu-Process-RecentFunctions MRU items
//---------------------------------------------------------------------------
void CDemoDoc::OnUpdateMruFunc1(CCmdUI* pCmdUI) 
{
   DEMO_APP->GetRecentFuncList()->UpdateMenu(pCmdUI, GetVector(),
      CanUpdateMru());
}

//---------------------------------------------------------------------------
// OnMruFunc* performs one of Menu-Process-RecentFunctions MRU items
//---------------------------------------------------------------------------
void CDemoDoc::OnMruFunc1() { ProcessVector(1);}
void CDemoDoc::OnMruFunc2() { ProcessVector(2);}
void CDemoDoc::OnMruFunc3() { ProcessVector(3);}
void CDemoDoc::OnMruFunc4() { ProcessVector(4);}
void CDemoDoc::OnMruFunc5() { ProcessVector(5);}
void CDemoDoc::OnMruFunc6() { ProcessVector(6);}

///////////////////////////////////////////////////////////////////



