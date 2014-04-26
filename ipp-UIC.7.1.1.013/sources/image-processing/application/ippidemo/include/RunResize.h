/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResize.h: interface for the CRunResize class.
// CRunResize class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRESIZE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNRESIZE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunResize : public CippiRun
{
public:
   CRunResize();
   virtual ~CRunResize();
   virtual BOOL IsMovie(CFunc func);

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual double GetPerfDivisor();
   virtual void PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst);
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

    int m_Type;
    Ipp64f m_xFactor;
    Ipp64f m_yFactor;
    int m_interpolate;

    Ipp64f m_xFactor_Save;
    Ipp64f m_yFactor_Save;
    int m_interpolate_Save;

    int m_xDst, m_xSrc;
    int m_yDst, m_ySrc;

    void SetIntFract(double fract, int& up, int& low);
};

#endif // !defined(AFX_RUNRESIZE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
