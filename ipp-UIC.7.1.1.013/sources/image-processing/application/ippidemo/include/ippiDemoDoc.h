/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDemoDoc.h : interface for the CippiDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPIDEMODOC_H__D45FB64A_EDB7_48C7_822A_ECE27FAF0E8B__INCLUDED_)
#define AFX_IPPIDEMODOC_H__D45FB64A_EDB7_48C7_822A_ECE27FAF0E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "DemoDoc.h"
#include "Image.h"

class CippiDemoView;
class CProcess;
class CMovie;
class CMovieThread;

// Contour Drawing Tools
// DRAW_NONE   - no drawing
// DRAW_ROI    - ROI drawing
// DRAW_QUAD   - quadrangle drawing
// DRAW_PARL   - parallelogramm drawing
// DRAW_CENTER - center & shift drawing
enum {DRAW_NONE, DRAW_ROI, DRAW_QUAD, DRAW_PARL, DRAW_CENTER, };

class CippiDemoDoc : public CDemoDoc, public CImage
{
protected: // create from serialization only
   CippiDemoDoc();
   DECLARE_DYNCREATE(CippiDemoDoc)

public:
   virtual ~CippiDemoDoc();

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

public:
   // GetVector returns pointer to vector that can be processed
   // by IPP function
   virtual CVector* GetVector() { return this;}
   // GetVector returns pointer to image that can be processed
   // by IPP function
   virtual CImage* GetImage() { return this;}

   // GetIppiView returns first document view
   virtual CippiDemoView* GetIppiView() { return (CippiDemoView*)GetView();}

   // IsData returns weather vector data is allocated or not
   virtual BOOL IsData();

   // GetFrameType returns image type string to be insert to document
   // frame title
   virtual CString GetFrameType();
   // GetDocStatus returns information be set to main window status string
   // when this document is active
   virtual CString GetDocStatus();

protected:

   // CreateProcess creates class that will be process this document by proper
   // IPP functions
   virtual void CreateProcess();
   // CreateNewData creates new vector(image) with default parameters
   virtual BOOL CreateNewData();

public:
/////////////////////////////////////////////////////////////////////////////
// Drawaing implementation

   // Get(Set)DrawTool returns(sets) current drawing tool
   int  GetDrawTool() const { return m_DrawTool;}
   void SetDrawTool(int mode) { m_DrawTool = mode;}

   // BeginDraw starts drawing by specified tool
   void BeginDraw(int drawTool);
   // FinishDraw completes drawing
   void FinishDraw();
   // CancelDraw cancels drawing
   void CancelDraw();

/////////////////////////////////////////////////////////////////////////////
// Movie implementation

   friend class CMovieThread;

   // GetMovie returns pointer to document's movie class
   CMovie* GetMovie() const { return m_pMovie;}
   // GetMovieThread returns pointer to document's movie thread class
   CMovieThread* GetMovieThread() const { return m_pMovieThread;}
   // Get(Set)MovieStage returns(sets) the stage of movie (play, pause, etc.)
   int GetMovieStage() const { return m_MovieStage;}
   void SetMovieStage(int stage) { m_MovieStage = stage;}
   // CanPlayMovie returns flag to update player buttons
   BOOL CanPlayMovie();

   // CreateMovie creates movie & movie thread at the document creation
   void CreateMovie();
   // StartMovie starts movie
   void StartMovie(int stage);
   // DrawMovie update view and correct document title while movie performance
   void DrawMovie(LPCSTR pTitle);
   // StopMovie stops movie
   void StopMovie();

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippiDemoDoc)
   public:
   virtual void Serialize(CArchive& ar);
   virtual void OnCloseDocument();
   //}}AFX_VIRTUAL

/////////////////////////////////////////////////////////////////////////////
// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif
protected:

   int m_DrawTool;

   ImgHeader m_StoreHeader;

   CMovie* m_pMovie;
   CMovieThread* m_pMovieThread;
   int m_MovieStage;
   CMutex m_MutexFlag;

   // SetData sets image data with specified pattern (see ippiDemo.h,
   // SET_*** enumerator)
   void SetData(int pattern);
   // CanSaveToBitmap prepares document to be saved in standard bmp format
   // or in non-standard format due to user respond.
   BOOL CanSaveToBitmap();
   // UpdateFileName updates document title after SaveAs command
   void UpdateFileName();

   // GetImageInfo returns image parameters information to be print by
   // Properties dialog
   virtual CString GetImageInfo();

/////////////////////////////////////////////////////////////////////////////
// Attributes for GetImageInfo & GetDocStatus implementation
   CString GetPathInfo() const;
   CString GetHeaderInfo();
   CString GetRoiInfo() const;
   CString GetQuadInfo() const;
   CString GetParlInfo() const;
   CString GetCenterInfo() const;
   CString GetRoiStatus() const;
   CString GetQuadStatus() const;
   CString GetParlStatus() const;
   CString GetCenterStatus() const;
   CString GetQuadPointInfo(const double quad[4][2],
                            int num, CString format) const;

/////////////////////////////////////////////////////////////////////////////
//  Command processing implementation

   virtual BOOL CanEnableProcess();
   virtual BOOL CanUpdateMru();
   void OnUpdateContour(CCmdUI* pCmdUI, int drawFlag, BOOL bExist);
   void OnUpdateChannel(CCmdUI* pCmdUI, int coi) ;
   void OnChannel(int coi) ;
   void OnDraw(int drawFlag);
   void OnUpdateDraw(CCmdUI* pCmdUI, int drawFlag);

/////////////////////////////////////////////////////////////////////////////
// Generated message map functions
   //{{AFX_MSG(CippiDemoDoc)
   afx_msg void OnFileSave();
   afx_msg void OnFileSaveAs();
   afx_msg void OnFileProp();
   afx_msg void OnUpdateImgSign(CCmdUI* pCmdUI);
   afx_msg void OnImgSign();
   afx_msg void OnUpdateImgPlane(CCmdUI* pCmdUI);
   afx_msg void OnImgPlane();
   afx_msg void OnUpdateImgCplx(CCmdUI* pCmdUI);
   afx_msg void OnImgCplx();
   afx_msg void OnUpdateImgAlpha(CCmdUI* pCmdUI);
   afx_msg void OnImgAlpha();
   afx_msg void OnUpdateImgBlue(CCmdUI* pCmdUI);
   afx_msg void OnImgBlue();
   afx_msg void OnUpdateImgGreen(CCmdUI* pCmdUI);
   afx_msg void OnImgGreen();
   afx_msg void OnUpdateImgRed(CCmdUI* pCmdUI);
   afx_msg void OnImgRed();
   afx_msg void OnCenterCopy();
   afx_msg void OnUpdateCenterCopy(CCmdUI* pCmdUI);
   afx_msg void OnCenterDelete();
   afx_msg void OnUpdateCenterDelete(CCmdUI* pCmdUI);
   afx_msg void OnCenterDraw();
   afx_msg void OnCenterSet();
   afx_msg void OnParlCopy();
   afx_msg void OnUpdateParlCopy(CCmdUI* pCmdUI);
   afx_msg void OnParlDelete();
   afx_msg void OnUpdateParlDelete(CCmdUI* pCmdUI);
   afx_msg void OnParlDraw();
   afx_msg void OnParlSet();
   afx_msg void OnQuadCopy();
   afx_msg void OnUpdateQuadCopy(CCmdUI* pCmdUI);
   afx_msg void OnQuadDelete();
   afx_msg void OnUpdateQuadDelete(CCmdUI* pCmdUI);
   afx_msg void OnQuadDraw();
   afx_msg void OnQuadSet();
   afx_msg void OnRoiCopy();
   afx_msg void OnUpdateRoiCopy(CCmdUI* pCmdUI);
   afx_msg void OnRoiDelete();
   afx_msg void OnUpdateRoiDelete(CCmdUI* pCmdUI);
   afx_msg void OnRoiDraw();
   afx_msg void OnRoiSet();
   afx_msg void OnUpdateRoiSet(CCmdUI* pCmdUI);
   afx_msg void OnRoi();
   afx_msg void OnUpdateRoi(CCmdUI* pCmdUI);
   afx_msg void OnUpdateRoiDraw(CCmdUI* pCmdUI);
   afx_msg void OnUpdateCenterDraw(CCmdUI* pCmdUI);
   afx_msg void OnUpdateParlDraw(CCmdUI* pCmdUI);
   afx_msg void OnUpdateQuadDraw(CCmdUI* pCmdUI);
   afx_msg void OnDrawCancel();
   afx_msg void OnUpdateMarkerDelete(CCmdUI* pCmdUI);
   afx_msg void OnMarkerDelete();
   afx_msg void OnUpdateCenter(CCmdUI* pCmdUI);
   afx_msg void OnUpdateParl(CCmdUI* pCmdUI);
   afx_msg void OnParl();
   afx_msg void OnUpdateQuad(CCmdUI* pCmdUI);
   afx_msg void OnQuad();
   afx_msg void OnCenter();
   afx_msg void OnUpdateMoviePlay(CCmdUI* pCmdUI);
   afx_msg void OnMoviePlay();
   afx_msg void OnUpdateMovieStop(CCmdUI* pCmdUI);
   afx_msg void OnMovieStop();
   afx_msg void OnUpdateMoviePause(CCmdUI* pCmdUI);
   afx_msg void OnMoviePause();
   afx_msg void OnUpdateMovieNext(CCmdUI* pCmdUI);
   afx_msg void OnMovieNext();
   afx_msg void OnUpdateMoviePrev(CCmdUI* pCmdUI);
   afx_msg void OnMoviePrev();
   afx_msg void OnWindowFit();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPIDEMODOC_H__D45FB64A_EDB7_48C7_822A_ECE27FAF0E8B__INCLUDED_)
