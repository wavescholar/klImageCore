/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMirror.h: interface for the CRunMirror class.
// CRunMirror class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMIRROR_H__030C9F06_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_)
#define AFX_RUNMIRROR_H__030C9F06_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
#include "ParmMirrorDlg.h"

class CRunMirror : public CippiRun
{
public:
   CRunMirror();
   virtual ~CRunMirror();
   virtual BOOL IsMovie(CFunc func);
protected:
    IppiAxis m_flip;

    IppiAxis m_flip_Save;

    virtual CParamDlg* CreateDlg() { return new CParmMirrorDlg;}
    virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();
};

#endif // !defined(AFX_RUNMIRROR_H__030C9F06_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_)
