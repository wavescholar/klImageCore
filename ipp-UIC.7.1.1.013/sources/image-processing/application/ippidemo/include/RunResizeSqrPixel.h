/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeSqrPixel.h: interface for the CRunResizeSqrPixel class.
// CRunResizeSqrPixel class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRESIZESQRPIXEL_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_)
#define AFX_RUNRESIZESQRPIXEL_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunResizeShift.h"

class CRunResizeSqrPixel : public CRunResizeShift
{
public:
   CRunResizeSqrPixel();
   virtual ~CRunResizeSqrPixel();

   virtual BOOL  Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL PrepareSrc();
   virtual BOOL PrepareDst();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   virtual BOOL IsMovie(CFunc func);

protected:
   virtual void PrepareParameters();
   virtual void PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst);
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

    void* m_pBuffer;

    Ipp64f m_xShift_Save;
    Ipp64f m_yShift_Save;
    BOOL m_bMovie;

};

#endif // !defined(AFX_RUNRESIZESQRPIXEL_H__70E1FE1F_EBCB_4142_9652_780B4E24FD4D__INCLUDED_)
