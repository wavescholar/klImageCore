/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTwist.cpp: implementation of the CRunTwist class.
// CRunTwist class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunTwist.h"
#include "ParmTwistDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunTwist::CRunTwist()
{
   m_twistType = TW_AVER;
   for (int i=0; i<3; i++) {
      for (int j=0; j<4; j++) {
         m_twist[i][j] = 0;
      }
   }
   m_twistFP = m_twist[0];
}

CRunTwist::~CRunTwist()
{

}

CParamDlg* CRunTwist::CreateDlg() { return new CParmTwistDlg;}

void CRunTwist::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmTwistDlg* pDlg = (CParmTwistDlg*)parmDlg;
   if (save) {
      m_twistType = pDlg->m_twistType;
      for (int i=0; i<3; i++) {
         for (int j=0; j<4; j++) {
            m_twist[i][j] = (float)atof(pDlg->m_twistStr[i][j]);
         }
      }
   } else {
      for (int i=0; i<3; i++) {
         for (int j=0; j<4; j++) {
            pDlg->m_twistStr[i][j].Format("%.6g", m_twist[i][j]);
         }
      }
      pDlg->m_twistType = m_twistType;
    }
}

IppStatus CRunTwist::CallIppFunction()
{
   FUNC_CC_CALL(ippiColorTwist32f_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8u_C3IR, ( (Ipp8u*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8u_AC4IR, ( (Ipp8u*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8u_P3R, ( (const Ipp8u**)pSrcP, srcStep,
                   (Ipp8u**)pDstP, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8u_IP3R, ( (Ipp8u**)pSrcP, srcStep,
                   roiSize, m_twist ))

   FUNC_CC_CALL(ippiColorTwist32f_8s_C3R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8s_C3IR, ( (Ipp8s*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8s_AC4R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8s_AC4IR, ( (Ipp8s*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8s_P3R, ( (const Ipp8s**)pSrcP, srcStep,
                   (Ipp8s**)pDstP, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_8s_IP3R, ( (Ipp8s**)pSrcP, srcStep,
                   roiSize, m_twist ))

   FUNC_CC_CALL(ippiColorTwist32f_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16u_C3IR, ( (Ipp16u*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16u_AC4IR, ( (Ipp16u*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16u_P3R, ( (const Ipp16u**)pSrcP, srcStep,
                   (Ipp16u**)pDstP, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16u_IP3R, ( (Ipp16u**)pSrcP, srcStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_C3IR, ( (Ipp16s*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_AC4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_AC4IR, ( (Ipp16s*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_P3R, ( (const Ipp16s**)pSrcP, srcStep,
                   (Ipp16s**)pDstP, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist32f_16s_IP3R, ( (Ipp16s**)pSrcP, srcStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_C3IR, ( (Ipp32f*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_AC4IR, ( (Ipp32f*)pSrc, srcStep, roiSize,
                   m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_P3R, ( (const Ipp32f**)pSrcP, srcStep,
                   (Ipp32f**)pDstP, dstStep,
                   roiSize, m_twist ))
   FUNC_CC_CALL(ippiColorTwist_32f_IP3R, ( (Ipp32f**)pSrcP, srcStep,
                    roiSize, m_twist ))



   return stsNoFunction;
}

BOOL CRunTwist::IsMovie(CFunc func)
{
   return TRUE;
}


static void updateTwistValue(float& value)
{
    if (1 < value && value <= 3)
        value = 2 - value;
    else if (3 < value)
        value = value - 4;
}

static void setTwistTable(float value, float twist[3][4])
{
    static const float plus[3][4] = {
         3.0, -1.0, -1.0, 0,
        -1.0,  3.0, -1.0, 0,
        -1.0, -1.0,  3.0, 0,
    };
    static const float minus[3][4] = {
        0.2f, 0.4f, 0.4f, 0,
        0.4f, 0.2f, 0.4f, 0,
        0.4f, 0.4f, 0.2f, 0,
    };
    static const float zero[3][4] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
    };
    if (value > 0)
    {
        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++)
                twist[i][j] = zero[i][j] + value*(plus[i][j] - zero[i][j]);
    }
    else
    {
        value = -value;
        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++)
                twist[i][j] = zero[i][j] + value*(minus[i][j] - zero[i][j]);
    }
}

int CRunTwist::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                                     int& numReps)
{
    for (int i=0; i<3; i++) {
        for (int j=0; j<4; j++) {
            m_twist_Save[i][j] = m_twist[i][j];
        }
    }
    dstInfo = "";
    if (tim < 0) {
        setTwistTable(1, m_twist);
    } else {
        float value = (float)GetMovieValue(tim, 4);
        updateTwistValue(value);
        setTwistTable(value, m_twist);
    }
    return 1;
}

void CRunTwist::ResetMovieArgs()
{
   for (int i=0; i<3; i++) {
      for (int j=0; j<4; j++) {
         m_twist[i][j] = m_twist_Save[i][j];
      }
   }
}
