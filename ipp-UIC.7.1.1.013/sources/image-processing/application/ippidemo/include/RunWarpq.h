/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWarpq.h: interface for the CRunWarpq class.
// CRunWarpq class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWARPQ_H__44AAAF68_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNWARPQ_H__44AAAF68_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunWarpq : public CippiRun
{
public:
   CRunWarpq();
   virtual ~CRunWarpq();

    virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual void CopyDataToNewDst() {}
    virtual ImgHeader GetNewDstHeader();
    virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual BOOL IsMovie(CFunc func);
protected:
    int m_SType;
    int m_DType;
    Ipp64f m_srcQuad[4][2];
    Ipp64f m_dstQuad[4][2];
    int m_interpolate;

    Ipp64f m_srcQuad_Save[4][2];
    Ipp64f m_dstQuad_Save[4][2];
    int m_interpolate_Save;

    virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
    virtual void ResetMovieArgs();
};

#endif // !defined(AFX_RUNWARPQ_H__44AAAF68_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
