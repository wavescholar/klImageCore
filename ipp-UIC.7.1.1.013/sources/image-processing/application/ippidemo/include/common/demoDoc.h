/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// demoDoc.h : interface for the CDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_)
#define AFX_DEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcess;
class CPerf;
class CVector;
class CImage;
struct ImgHeader;
class CHisto;
class CDropTitle;

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc: base class for Demo MFC document

class CDemoDoc : public CDocument
{
protected: // create from serialization only
/////////////////////////////////////////////////////////////////////////////
// Construction
   CDemoDoc();
   DECLARE_DYNCREATE(CDemoDoc)

public:
   virtual ~CDemoDoc();
/////////////////////////////////////////////////////////////////////////////
// Public Attributes
   // GetVector returns pointer to attached vector that can be processed
   // by ippSP or ippIP Demo
   virtual CVector* GetVector() { return NULL;}
   // GetImage returns pointer to attached image that can be processed
   // by ippIP Demo. Actually CImage is derived from CVector and usually
   // CDemo uses GetVector() to obtain attached object. But in some
   // special cases CDemo needs to know that attached object is image.  
   virtual CImage* GetImage() { return NULL;}

   // GetView returns first document view
   virtual CView* GetView();
   // GetFrame returns first document frame window
   virtual CMDIChildWnd* GetFrame();

   // GetProcess returns Process class that processes attached vector by 
   // any proper IPP function
   CProcess* GetProcess() const { return m_pProcess;}

   // MayBePicked gets & sets weather attached vector may be picked as wanted
   // IPP function parameter
   BOOL MayBePicked() const { return m_MayBePicked;}
   void MayBePicked(BOOL mode) { m_MayBePicked = mode;}
   // IsPicked gets & sets weather attached vector have been picked as wanted
   // IPP function parameter
   BOOL IsPicked() const { return m_IsPicked;}
   void IsPicked(BOOL mode) { m_IsPicked = mode;}

   // GetHisto returns pointer to document history
   CHisto* GetHisto();

/////////////////////////////////////////////////////////////////////////////
// Public Operations
   // UpdateView updates sppecified document view or all views
   virtual void UpdateView(CView* pSender = NULL);
   // Activate activates sppecified document frame window or the first one
   virtual void Activate(CMDIChildWnd* pFrame = NULL);

   // ProcessVector processes attached vector by IPP function that will be obtained
   // by Process Dialog or will be got from MRU func list
   void ProcessVector();
   void ProcessVector(int mruIndex) ;
   // MarkToPick marks weather document can be picked as wanted IPP function parameter
   void MarkToPick();
   // InitHisto initializes document history
   virtual void InitHisto();
   // SetStatusString sets document attributes to main window status string 
   virtual void SetStatusString() { ((CMDIFrameWnd*)AfxGetMainWnd())->SetMessageText(GetDocStatus());}
   // UpdateFrameTitle updates document title
   virtual void UpdateFrameTitle();
   virtual void UpdateStatusString();
   virtual void UpdateStatusFunc(CString func, CPerf* pPerf = NULL);

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications
   virtual void GetDropTitle(CDropTitle* pTitle);
   virtual void SetDropTitle(const CDropTitle* pTitle);
   virtual BOOL ValidDropTitle(const CDropTitle* pTitle);

   static void ReadDropVectorHeader(const char* pData, ppType& type, int& len);
   static void ReadDropImageHeader(const char* pData, ImgHeader* pHeader);

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDemoDoc)
   public:
   virtual BOOL OnNewDocument();
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   virtual void OnCloseDocument();
   virtual void OnFileSaveAs();
   virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
   //}}AFX_VIRTUAL

/////////////////////////////////////////////////////////////////////////////
// Implementation
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CProcess* m_pProcess;
   CString m_StatusFunc;
   CPerf*  m_pPerf;
   BOOL m_MayBePicked;
   BOOL m_IsPicked;

public:
   // CreateProcess creates class that will be process this document by proper IPP functions
   virtual void CreateProcess();
   // CreateNewData creates new vector(image) with default parameters
   virtual BOOL CreateNewData();

   // GetFrameTitle returns string that is set to document frame title.
   CString GetFrameTitle();
   // GetFrameType returns vector type string to be insert to document frame title 
   virtual CString GetFrameType();
   // GetDocStatus returns information be set to main window status string 
   // when this document is active
   virtual CString GetDocStatus();

   // IsData returns weather vector data is allocated or not
   virtual BOOL IsData();
   // CanEnableProcess returns weather menu process item should be enabled
   virtual BOOL CanEnableProcess();
   // CanUpdateMru returns weather MRU function menu should be updated
   virtual BOOL CanUpdateMru();

private:

// Generated message map functions
protected:
   //{{AFX_MSG(CDemoDoc)
   afx_msg void OnFileHisto();
   afx_msg void OnUpdateProcess(CCmdUI* pCmdUI);
   afx_msg void OnProcess();
   afx_msg void OnUpdateMruFunc1(CCmdUI* pCmdUI);
   afx_msg void OnMruFunc1();
   afx_msg void OnMruFunc2();
   afx_msg void OnMruFunc3();
   afx_msg void OnMruFunc4();
   afx_msg void OnMruFunc5();
   afx_msg void OnMruFunc6();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_)
