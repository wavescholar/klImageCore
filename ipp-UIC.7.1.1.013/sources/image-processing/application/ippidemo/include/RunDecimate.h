/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDecimate.h: interface for the CRunDecimate class.
// CRunDecimate class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDECIMATE_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
#define AFX_RUNDECIMATE_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDecimate : public CippiRun
{
public:
    CRunDecimate();
    virtual ~CRunDecimate();
protected:
    virtual BOOL Open(CFunc func);
    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual ImgHeader GetNewDstHeader();
    virtual BOOL PrepareSrc();
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();

    IppiFraction m_fraction;
    BOOL m_bRow;

    int up();
    int down();
    IppiSize GetDstSize(IppiSize srcSize);
    IppiSize GetSrcSize(IppiSize dstSize);
};

#endif // !defined(AFX_RUNDECIMATE_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
