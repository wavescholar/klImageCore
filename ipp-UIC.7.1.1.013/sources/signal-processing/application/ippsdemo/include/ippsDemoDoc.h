/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemoDoc.h : interface for the CippsDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSDEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_)
#define AFX_IPPSDEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DemoDoc.h"
#include "Vector.h"

class CippsDemoView;
class CippsTextView;
class CippsCharView;
class CChildFrame;
class CTextFrame;
class CCharFrame;
class CProcess;

class CippsDemoDoc : public CDemoDoc, public CVector
{
protected: // create from serialization only
   CippsDemoDoc();
   DECLARE_DYNCREATE(CippsDemoDoc)

public:
   virtual ~CippsDemoDoc();

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc virtual function overrides

   // GetVector returns pointer to vector that can be processed
   // by IPP function
   virtual CVector* GetVector() { return this;}
   // GetFrame returns graphic view frame if it exists and returns
   // text view frame otherwize
   virtual CMDIChildWnd* GetFrame();

   // CreateNewData creates new vector with default parameters
   virtual BOOL CreateNewData();
   // UpdateView updates sppecified document view or all views
   virtual void UpdateView(CView* pSender = NULL);
   // UpdateFrameTitle updates document title
   virtual void UpdateFrameTitle(CString lastFunction = _T(""));

/////////////////////////////////////////////////////////////////////////////
// Attributes

   // GetActiveView return active view
   CView*     GetActiveView();
   // GetDemoView returns graphic view if it exists
   CippsDemoView* GetDemoView();
   // GetTextView returns digital view if it exists
   CippsTextView* GetTextView();
   // GetCharView returns string view if it exists
   CippsCharView* GetCharView();
   // GetDemoFrame returns graphic view frame if it exists
   CChildFrame*  GetDemoFrame();
   // GetTextFrame returns digital view frame if it exists
   CTextFrame*   GetTextFrame();
   // GetCharFrame returns string view frame if it exists
   CCharFrame*   GetCharFrame();

   // GetScaledValue returns vector element at specified index 
   // scaled for drawing
   Ipp64fc GetScaledValue(int index) const;
   // FactorW returns scaling width factor
   double FactorW() { return m_scaleW;}
   // FactorH returns scaling height factor
   double FactorH() { return m_scaleH;}

   // SamplesPerSecond gets(sets) smaples per second for saving in wav file 
   int SamplesPerSecond() { return m_SamplesPerSecond;}
   void SamplesPerSecond(int mode) { m_SamplesPerSecond = mode;}

   // Get & Set string for character view 
   void GetString(CString& str, int lenMax = -1) const;
   void SetString(const CString& str);

/////////////////////////////////////////////////////////////////////////////
// Operations

   // CreateDemoView creates grapic view
   BOOL CreateDemoView(BOOL bMakeVisible = TRUE);
   // CreateTextView creates digital view
   BOOL CreateTextView(BOOL bMakeVisible = TRUE);
   // CreateCharView creates string view
   BOOL CreateCharView(BOOL bMakeVisible = TRUE);

   // ZoomStandard scales grapfh to fit for standard window
   void ZoomStandard();
   // ZoomByFactors scales grapfh by specified factors and accordingly changes
   // frame rectangle
   void ZoomByFactors(double scaleW, double scaleH, POINT* pRectOffset = NULL);
   // ZoomByFactors scales grapfh to fit for specified or actual frame rectangle
   void ZoomToRect(RECT* pRect = NULL);

   // UpdateXAxis update x axis in graphic view
   void UpdateXAxis();
   // UpdateYAxis update y axis in graphic view
   void UpdateYAxis();

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippsDemoDoc)
   public:
   virtual void Serialize(CArchive& ar);
   //}}AFX_VIRTUAL

/////////////////////////////////////////////////////////////////////////////
// Implementation
public:

protected:
   double m_scaleW;
   double m_scaleH;
   int  m_SamplesPerSecond;

   // CreateNewView creates new view for document
   BOOL CreateNewView(CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
                      BOOL bMakeVisible = TRUE);
   // Zoom implementation
   void Zoom(RECT* pRect = NULL);

/////////////////////////////////////////////////////////////////////////////
// Generated message map functions
protected:
   //{{AFX_MSG(CippsDemoDoc)
   afx_msg void OnFileSaveAs();
   afx_msg void OnUpdateFileSigned(CCmdUI* pCmdUI);
   afx_msg void OnFileSigned();
   afx_msg void OnFileProp();
 
   afx_msg void OnZoomHup();
   afx_msg void OnZoomHlow();
   afx_msg void OnZoomWup();
   afx_msg void OnZoomWlow();
   afx_msg void OnZoomCustom();
   afx_msg void OnZoomFith();
   afx_msg void OnZoomFitw();
   afx_msg void OnZoomStandard();
   afx_msg void OnZoomCurrent();
   afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
   afx_msg void OnUpdateZoomHlow(CCmdUI* pCmdUI);
   afx_msg void OnUpdateZoomWlow(CCmdUI* pCmdUI);

   afx_msg void OnUpdateWindowPlot(CCmdUI* pCmdUI);
   afx_msg void OnUpdateWindowText(CCmdUI* pCmdUI);
   afx_msg void OnWindowPlot();
   afx_msg void OnWindowText();

   afx_msg void OnEditInsert();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnUpdateWindowChar(CCmdUI *pCmdUI);
   afx_msg void OnWindowChar();
   afx_msg void OnUpdateEditInsert(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPSDEMODOC_H__91CAC3AA_F1A6_4378_B921_76B55DEB3516__INCLUDED_)
