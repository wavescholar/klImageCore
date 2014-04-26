/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyManaged.cpp: implementation of the CRunCopyManaged class.
// CRunCopyManaged class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunCopyManaged.h"
#include "ParmCopyManagedDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCopyManaged::CRunCopyManaged()
{
    m_flags = IPP_NONTEMPORAL_STORE | IPP_NONTEMPORAL_LOAD;
}

CRunCopyManaged::~CRunCopyManaged()
{

}

CParamDlg* CRunCopyManaged::CreateDlg() { return new CParmCopyManagedDlg;}

void CRunCopyManaged::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmCopyManagedDlg* pDlg = (CParmCopyManagedDlg*)parmDlg;
    if (save) {
        m_flags = pDlg->m_flags;
    } else {
        pDlg->m_flags = m_flags;
    }
}

IppStatus CRunCopyManaged::CallIppFunction()
{
     FUNC_CALL( ippiCopyManaged_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_flags ))

    return stsNoFunction;
}

CString CRunCopyManaged::GetHistoryParms()
{
    CMyString parms;
    if (m_flags == 0)
        parms = "TEMPORAL_COPY";
    if (m_flags & IPP_NONTEMPORAL_STORE)
        parms = "NONTEMPORAL_STORE";
    if (m_flags & IPP_NONTEMPORAL_LOAD)
    {
        if (!parms.IsEmpty())
            parms += " | ";
        parms += "NONTEMPORAL_LOAD";
    }
    return parms;
}

