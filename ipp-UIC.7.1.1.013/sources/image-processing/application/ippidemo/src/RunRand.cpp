/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRand.cpp: implementation of the CRunRand class.
// CRunRand class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunRand.h"
#include "ParmRandDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRunRand::CRunRand()
{
   m_seed = 0;
   m_Parm = NULL;

   m_parmNorm[0].Init(pp8u , 0);
   m_parmNorm[0].Init(pp16s, 0);
   m_parmNorm[0].Init(pp32f, 0);

   m_parmNorm[1].Init(pp8u , 16    );
   m_parmNorm[1].Init(pp16s, 16*256);
   m_parmNorm[1].Init(pp32f, 0.06  );

   m_parmUniform[0].Init(pp8u , 0);
   m_parmUniform[0].Init(pp16s, 0);
   m_parmUniform[0].Init(pp32f, 0);

   m_parmUniform[1].Init(pp8u , 8    );
   m_parmUniform[1].Init(pp16s, 8*256);
   m_parmUniform[1].Init(pp32f, 0.03  );
}

CRunRand::~CRunRand()
{

}

BOOL CRunRand::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   m_Parm = func.Found("Norm") ? m_parmNorm : m_parmUniform;

   m_Parm[0].Init(func.SrcType());
   m_Parm[1].Init(func.SrcType());
   return TRUE;
}

void CRunRand::Close()
{
}

CParamDlg* CRunRand::CreateDlg() { return new CParmRandDlg;}

void CRunRand::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmRandDlg* pDlg = (CParmRandDlg*)parmDlg;
    if (save) {
        m_Parm[0].Set(pDlg->m_ParmStr[0]);
        m_Parm[1].Set(pDlg->m_ParmStr[1]);
        m_seed = atoi(pDlg->m_SeedStr);
    } else {
        m_Parm[0].Get(pDlg->m_ParmStr[0]);
        m_Parm[1].Get(pDlg->m_ParmStr[1]);
        pDlg->m_SeedStr.Format("%u",m_seed);
    }
}

IppStatus CRunRand::CallIppFunction()
{
   FUNC_CALL(ippiAddRandUniform_Direct_8u_C1IR,   ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_8u_C3IR,   ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_8u_C4IR,  ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_8u_AC4IR,  ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16u_C1IR,  ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16u_C3IR,  ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16u_C4IR, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16u_AC4IR, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16s_C1IR,  ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16s_C3IR,  ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16s_C4IR, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_16s_AC4IR, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_32f_C1IR,  ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_32f_C3IR,  ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_32f_C4IR, ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandUniform_Direct_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_8u_C1IR,   ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_8u_C3IR,   ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_8u_C4IR,  ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_8u_AC4IR,  ((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_Parm[0], (Ipp8u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16u_C1IR,  ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16u_C3IR,  ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16u_C4IR, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16u_AC4IR, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_Parm[0], (Ipp16u)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16s_C1IR,  ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16s_C3IR,  ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16s_C4IR, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_16s_AC4IR, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_Parm[0], (Ipp16s)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_32f_C1IR,  ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_32f_C3IR,  ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_32f_C4IR, ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))
   FUNC_CALL(ippiAddRandGauss_Direct_32f_AC4IR, ((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_Parm[0], (Ipp32f)m_Parm[1], &m_seed))

   return stsNoFunction;
}

CString CRunRand::GetHistoryParms()
{
   CMyString parms;
   return parms << m_Parm[0] << ", "
                << m_Parm[1] << ", "
                << m_seed;
}
