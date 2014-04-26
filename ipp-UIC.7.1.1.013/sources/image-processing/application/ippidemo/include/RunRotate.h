/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRotate.h: interface for the CRunRotate class.
// CRunRotate class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNROTATE_H__C2918023_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_)
#define AFX_RUNROTATE_H__C2918023_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"
#include "ParmRotateDlg.h"

class CRunRotate : public CippiRun
{
public:
   CRunRotate();
   virtual ~CRunRotate();
   virtual BOOL IsMovie(CFunc func);

protected:
   virtual CParamDlg* CreateDlg() { return new CParmRotateDlg;}
   virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual void CopyDataToNewDst() {}
   virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

   int m_Type;
   Ipp64f m_angle ;
   Ipp64f m_xShift;
   Ipp64f m_yShift;
   int m_interpolate;

   Ipp64f m_angle_Save ;
   Ipp64f m_xShift_Save;
   Ipp64f m_yShift_Save;
   int m_interpolate_Save;
};

#endif // !defined(AFX_RUNROTATE_H__C2918023_E111_11D2_8EE2_00AA00A03C3C__INCLUDED_)
