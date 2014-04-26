/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunReduce.h: interface for the CRunReduce class.
// CRunReduce class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNREDUCE_H__A257A543_3B45_11D3_8F3C_00AA00A03C3C__INCLUDED_)
#define AFX_RUNREDUCE_H__A257A543_3B45_11D3_8F3C_00AA00A03C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
#include "ParmReduceDlg.h"

class CRunReduce : public CippiRun
{
public:
   CRunReduce();
   virtual ~CRunReduce();
   virtual BOOL IsMovie(CFunc func);
protected:
    int m_noise;
    IppiDitherType m_dtype;
    int m_levels;

    int m_noise_Save;
    IppiDitherType m_dtype_Save;
    int m_levels_Save;

    virtual CParamDlg* CreateDlg() { return new CParmReduceDlg;}
    virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

private:

};

#endif // !defined(AFX_RUNREDUCE_H__A257A543_3B45_11D3_8F3C_00AA00A03C3C__INCLUDED_)
