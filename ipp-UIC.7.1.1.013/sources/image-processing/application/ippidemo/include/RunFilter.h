/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilter.h: interface for the CRunFilter class.
// CRunFilter class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTER_H__154A8B41_C0DC_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUNFILTER_H__154A8B41_C0DC_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunFilter : public CippiRun
{
public:
   CRunFilter();
   virtual ~CRunFilter();
   virtual BOOL IsMovie(CFunc func);
   enum {msk1x3, msk3x1, msk3x3, msk1x5, msk5x1, msk5x5,
      mskNUM};
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int GetSrcBorder(int idx);
   virtual BOOL BeforeCall();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

   IppiMaskSize m_mask;
   IppiMaskSize m_mask_Save;

   int m_nCols, m_nRows, m_anchorX, m_anchorY;
};

#endif // !defined(AFX_RUNFILTER_H__154A8B41_C0DC_11D1_AE6B_444553540000__INCLUDED_)
