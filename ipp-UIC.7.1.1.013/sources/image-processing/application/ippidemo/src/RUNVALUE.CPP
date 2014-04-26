/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunValue.cpp: implementation of the CRunValue class.
// CRunValue class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunValue.h"
#include "ParmValueDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunValue::CRunValue()
{
   m_valShift.Init(pp8u,  4);
   m_valShift.Init(pp8s,  4);
   m_valShift.Init(pp16u, 4);
   m_valShift.Init(pp16s, 4);
   m_valShift.Init(pp32s, 4);

   m_valDiv.Init(pp8u,  4);
   m_valDiv.Init(pp16u, 4);
   m_valDiv.Init(pp16s, 4);
   m_valDiv.Init(pp32s, 4);
   m_valDiv.Init(pp32f, 4);
   m_valDiv.Init(pp16sc, 4);
   m_valDiv.Init(pp32sc, 4);
   m_valDiv.Init(pp32fc, 4);

   m_valAnd.Init(pp8u ,4);
   m_valAnd.Init(pp16u,4);
   m_valAnd.Init(pp32s,4);

   m_valAdd.Init(pp8u ,4);
   m_valAdd.Init(pp16u,4);
   m_valAdd.Init(pp16s,4);
   m_valAdd.Init(pp32s,4);
   m_valAdd.Init(pp32f,4);
   m_valAdd.Init(pp16sc,4);
   m_valAdd.Init(pp32sc,4);
   m_valAdd.Init(pp32fc,4);

   m_valMul.Init(pp8u ,4);
   m_valMul.Init(pp16u,4);
   m_valMul.Init(pp16s,4);
   m_valMul.Init(pp32s,4);
   m_valMul.Init(pp32f,4);
   m_valMul.Init(pp16sc,4);
   m_valMul.Init(pp32sc,4);
   m_valMul.Init(pp32fc,4);

   m_valMulScale.Init(pp8u , 4);
   m_valMulScale.Init(pp16u, 4);

   for (int i=0; i<4; i++) {
      m_valShift.Vector(pp8u ).Set(i, 1);
      m_valShift.Vector(pp8s ).Set(i, 1);
      m_valShift.Vector(pp16u).Set(i, 1);
      m_valShift.Vector(pp16s).Set(i, 1);
      m_valShift.Vector(pp32s).Set(i, 1);

      m_valAnd.Vector(pp8u ).Set(i, 0x40);
      m_valAnd.Vector(pp16u).Set(i, 0x4000);
      m_valAnd.Vector(pp32s).Set(i, 0x40000000);

      m_valAdd.Vector(pp8u ).Set(i, 64);
      m_valAdd.Vector(pp16u).Set(i, 16000 );
      m_valAdd.Vector(pp16s).Set(i, 16000 );
      m_valAdd.Vector(pp32s).Set(i, 130000);
      m_valAdd.Vector(pp32f).Set(i, 0.5f);
      m_valAdd.Vector(pp16sc).Set(i, 16000 , 16000 );
      m_valAdd.Vector(pp32sc).Set(i, 130000, 130000);
      m_valAdd.Vector(pp32fc).Set(i, 0.5f, 0.5f);

      m_valMul.Vector(pp8u ).Set(i, 2  );
      m_valMul.Vector(pp16u).Set(i, 2  );
      m_valMul.Vector(pp16s).Set(i, 2  );
      m_valMul.Vector(pp32s).Set(i, 2  );
      m_valMul.Vector(pp32f).Set(i, 1.5);
      m_valMul.Vector(pp16sc).Set(i, 2  , 2  );
      m_valMul.Vector(pp32sc).Set(i, 2  , 2  );
      m_valMul.Vector(pp32fc).Set(i, 1.5, 1.5);

      m_valMulScale.Vector(pp8u ).Set(i, 100  );
      m_valMulScale.Vector(pp16u).Set(i, 30000);

      m_valDiv.Vector(pp8u ).Set(i, 2  );
      m_valDiv.Vector(pp16u).Set(i, 2  );
      m_valDiv.Vector(pp16s).Set(i, 2  );
      m_valDiv.Vector(pp32s).Set(i, 2  );
      m_valDiv.Vector(pp32f).Set(i, 1.5);
      m_valDiv.Vector(pp16sc).Set(i, 2  , 2  );
      m_valDiv.Vector(pp32sc).Set(i, 2  , 2  );
      m_valDiv.Vector(pp32fc).Set(i, 1.5, 1.5);
   }

}

CRunValue::~CRunValue()
{

}

BOOL CRunValue::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   m_valDiv.Init(func.SrcType());
   m_valAnd.Init(func.SrcType());
   m_valAdd.Init(func.SrcType());
   m_valMul.Init(func.SrcType());
   m_valMulScale.Init(func.SrcType());

   if      (m_Func.Found("Shift"))       m_value = m_valShift;
   else if (m_Func.BaseName() == "DivC") m_value = m_valDiv;
   else if (m_Func.BaseName() == "MulC") m_value = m_valMul;
   else if (m_Func.BaseName() == "MulCScale") m_value = m_valMulScale;
   else if (m_Func.BaseName() == "AddC" ||
            m_Func.BaseName() == "SubC") m_value = m_valAdd;
   else if (m_Func.BaseName() == "AndC" ||
            m_Func.BaseName() == "OrC" ||
            m_Func.BaseName() == "XorC") m_value = m_valAnd;

   m_value.SetLength(func.SrcChannels() - func.SrcAlpha());
   return TRUE;
}

void CRunValue::Close()
{
   if      (m_Func.Found("Shift"))       m_valShift = m_value;
   else if (m_Func.BaseName() == "DivC") m_valDiv = m_value;
   else if (m_Func.BaseName() == "MulC") m_valMul = m_value;
   else if (m_Func.BaseName() == "MulCScale") m_valMulScale = m_value;
   else if (m_Func.BaseName() == "AddC" ||
            m_Func.BaseName() == "SubC") m_valAdd = m_value;
   else if (m_Func.BaseName() == "AndC" ||
            m_Func.BaseName() == "OrC" ||
            m_Func.BaseName() == "XorC") m_valAnd = m_value;

}

CParamDlg* CRunValue::CreateDlg() { return new CParmValueDlg;}

void CRunValue::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmValueDlg* pDlg = (CParmValueDlg*)parmDlg;
    if (save) {
        m_value = pDlg->m_value; 
    } else {
        pDlg->m_value = m_value; 
    }
}

IppStatus CRunValue::CallIppFunction()
{
   FUNC_CALL(ippiDivC_32f_C1R,   ((Ipp32f*)pSrc, srcStep,(Ipp32f )m_value, (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiDivC_32f_C3R,   ((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value, (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiDivC_32f_C4R,   ((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value, (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiDivC_32f_AC4R,   ((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value, (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiDivC_16u_C1RSfs,((Ipp16u*)pSrc, srcStep,(Ipp16u )m_value, (Ipp16u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_C3RSfs,((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_C4RSfs,((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_AC4RSfs,((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C1RSfs,((Ipp16s*)pSrc, srcStep,(Ipp16s )m_value, (Ipp16s*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C3RSfs,((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value, (Ipp16s*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C4RSfs,((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value, (Ipp16s*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_AC4RSfs,((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value, (Ipp16s*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C1RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u  )m_value,  (Ipp8u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C3RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize, scaleFactor))

   FUNC_CALL(ippiAndC_8u_C1R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u  )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_8u_C3R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_8u_C4R,   ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_8u_AC4R,   ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C1R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u )m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C3R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C4R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_16u_AC4R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C1R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s )m_value, (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C3R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C4R,  ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAndC_32s_AC4R,  ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C1R,     ((Ipp8u*)pSrc, srcStep, (Ipp8u  )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C3R,     ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C4R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_8u_AC4R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C1R,    ((Ipp16u*)pSrc, srcStep,(Ipp16u )m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C3R,    ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C4R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_16u_AC4R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C1R,    ((Ipp32s*)pSrc, srcStep,(Ipp32s )m_value, (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C3R,    ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C4R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiOrC_32s_AC4R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C1R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u  )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C3R,    ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C4R,   ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_8u_AC4R,   ((Ipp8u*)pSrc, srcStep, (Ipp8u *)m_value,  (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C1R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u )m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C3R,   ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C4R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_16u_AC4R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,  (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C1R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s )m_value, (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C3R,   ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C4R,  ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiXorC_32s_AC4R,  ((Ipp32s*)pSrc, srcStep,(Ipp32s*)m_value,  (Ipp32s*)pDst, dstStep, roiSize))


   FUNC_CALL(ippiLShiftC_8u_C1R,  ((Ipp8u*)pSrc, srcStep, (Ipp32u )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_C3R,  ((Ipp8u*)pSrc, srcStep, (Ipp32u*)(Ipp32s*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C1R, ((Ipp16u*)pSrc,srcStep, (Ipp32u )m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C3R, ((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C4R,((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_AC4R,((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C1R, ((Ipp32s*)pSrc,srcStep, (Ipp32u )m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C3R, ((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C4R,((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_AC4R,((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C1R,  ((Ipp8u*)pSrc, srcStep, (Ipp32u )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C3R,  ((Ipp8u*)pSrc, srcStep, (Ipp32u*)(Ipp32s*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C1R,  ((Ipp8s*)pSrc, srcStep, (Ipp32u )m_value, (Ipp8s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C3R,  ((Ipp8s*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C4R, ((Ipp8s*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_AC4R, ((Ipp8s*)pSrc, srcStep, (Ipp32u*)m_value, (Ipp8s*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C1R, ((Ipp16u*)pSrc,srcStep, (Ipp32u )m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C3R, ((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C4R,((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_AC4R,((Ipp16u*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C1R, ((Ipp16s*)pSrc,srcStep, (Ipp32u )m_value, (Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C3R, ((Ipp16s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C4R,((Ipp16s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_AC4R,((Ipp16s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C1R, ((Ipp32s*)pSrc,srcStep, (Ipp32u )m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C3R, ((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C4R,((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_AC4R,((Ipp32s*)pSrc,srcStep, (Ipp32u*)m_value, (Ipp32s*)pDst,dstStep, roiSize))

   FUNC_CALL(ippiLShiftC_8u_C1IR,  ((Ipp32u )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_C3IR,  ((Ipp32u*)(Ipp32s*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_C4IR, ((Ipp32u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_8u_AC4IR, ((Ipp32u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C1IR, ((Ipp32u )m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C3IR, ((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_C4IR,((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_16u_AC4IR,((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C1IR, ((Ipp32u )m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C3IR, ((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_C4IR,((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiLShiftC_32s_AC4IR,((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C1IR,  ((Ipp32u )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C3IR,  ((Ipp32u*)(Ipp32s*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_C4IR, ((Ipp32u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8u_AC4IR, ((Ipp32u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C1IR,  ((Ipp32u )m_value, (Ipp8s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C3IR,  ((Ipp32u*)m_value, (Ipp8s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_C4IR, ((Ipp32u*)m_value, (Ipp8s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_8s_AC4IR, ((Ipp32u*)m_value, (Ipp8s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C1IR, ((Ipp32u )m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C3IR, ((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_C4IR,((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16u_AC4IR,((Ipp32u*)m_value, (Ipp16u*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C1IR, ((Ipp32u )m_value, (Ipp16s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C3IR, ((Ipp32u*)m_value, (Ipp16s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_C4IR,((Ipp32u*)m_value, (Ipp16s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_16s_AC4IR,((Ipp32u*)m_value, (Ipp16s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C1IR, ((Ipp32u )m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C3IR, ((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_C4IR,((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))
   FUNC_CALL(ippiRShiftC_32s_AC4IR,((Ipp32u*)m_value, (Ipp32s*)pSrc,srcStep, roiSize))

   FUNC_CALL(ippiAddC_8u_C1RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u  )m_value,  (Ipp8u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C3RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16u_C1RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u )m_value, (Ipp16u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16u_C3RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16u_C4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16u_AC4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16s_C1RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s )m_value, (Ipp16s*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16s_C3RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16s_C4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_16s_AC4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_8u_C1RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u  )m_value,  (Ipp8u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_8u_C3RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16u_C1RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u )m_value, (Ipp16u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16u_C3RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16u_C4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16u_AC4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16s_C1RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s )m_value, (Ipp16s*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16s_C3RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16s_C4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiSubC_16s_AC4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_8u_C1RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u  )m_value,  (Ipp8u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_8u_C3RSfs,  ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep,(Ipp8u *)m_value,                                           (Ipp8u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16u_C1RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u )m_value, (Ipp16u*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16u_C3RSfs, ((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16u_C4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16u_AC4RSfs,((Ipp16u*)pSrc,srcStep,(Ipp16u*)m_value,                                          (Ipp16u*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16s_C1RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s )m_value, (Ipp16s*)pDst,                                         dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16s_C3RSfs, ((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16s_C4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiMulC_16s_AC4RSfs,((Ipp16s*)pSrc,srcStep,(Ipp16s*)m_value,                                          (Ipp16s*)pDst, dstStep, roiSize,                                         scaleFactor))
   FUNC_CALL(ippiAddC_32f_C1R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f )m_value, (Ipp32f*)pDst,                                         dstStep, roiSize))
   FUNC_CALL(ippiAddC_32f_C3R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                          (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAddC_32f_C4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                          (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiAddC_32f_AC4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                          (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiSubC_32f_C1R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f )m_value, (Ipp32f*)pDst,                                         dstStep, roiSize))
   FUNC_CALL(ippiSubC_32f_C3R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiSubC_32f_C4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiSubC_32f_AC4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulC_32f_C1R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f )m_value, (Ipp32f*)pDst,                                      dstStep, roiSize))
   FUNC_CALL(ippiMulC_32f_C3R,    ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulC_32f_C4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulC_32f_AC4R,   ((Ipp32f*)pSrc,srcStep,(Ipp32f*)m_value,                                       (Ipp32f*)pDst, dstStep, roiSize))

   FUNC_CALL(ippiDivC_32f_C1IR   ,(*(Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiDivC_32f_C3IR   ,( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiDivC_32f_C4IR   ,( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiDivC_32f_AC4IR  ,( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiDivC_16u_C1IRSfs,(*(Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_C3IRSfs,( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_C4IRSfs,( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16u_AC4IRSfs,( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C1IRSfs,(*(Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C3IRSfs,( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_C4IRSfs,( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16s_AC4IRSfs,( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C1IRSfs, (*(Ipp8u *)m_value, (Ipp8u *)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C3IRSfs, ( (Ipp8u *)m_value, (Ipp8u *)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_C4IRSfs, ( (Ipp8u *)m_value, (Ipp8u *)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_8u_AC4IRSfs, ( (Ipp8u *)m_value, (Ipp8u *)pSrc, srcStep, roiSize, scaleFactor))

   FUNC_CALL(ippiAndC_8u_C1IR,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_8u_C3IR,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_8u_C4IR, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_8u_AC4IR, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C1IR, ((Ipp16u )m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C3IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_16u_C4IR,((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_16u_AC4IR,((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C1IR, ((Ipp32s )m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C3IR, ((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_32s_C4IR,((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAndC_32s_AC4IR,((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C1IR,   ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C3IR,   ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_8u_C4IR,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_8u_AC4IR,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C1IR,  ((Ipp16u )m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C3IR,  ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_16u_C4IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_16u_AC4IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C1IR,  ((Ipp32s )m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C3IR,  ((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_32s_C4IR, ((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiOrC_32s_AC4IR, ((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C1IR,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C3IR,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_8u_C4IR, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_8u_AC4IR, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C1IR, ((Ipp16u )m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C3IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_16u_C4IR,((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_16u_AC4IR,((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C1IR, ((Ipp32s )m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C3IR, ((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_32s_C4IR,((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiXorC_32s_AC4IR,((Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize))

   FUNC_CALL(ippiAddC_8u_C1IRSfs,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C3IRSfs,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_AC4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C1IRSfs,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C3IRSfs,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_C4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_8u_AC4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16u_C1IRSfs, ((Ipp16u )m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16u_C3IRSfs, ((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16u_C4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16u_AC4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16s_C1IRSfs, ((Ipp16s )m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16s_C3IRSfs, ((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16s_C4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16s_AC4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_8u_C1IRSfs,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_8u_C3IRSfs,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_8u_C4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_8u_AC4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16u_C1IRSfs, ((Ipp16u )m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16u_C3IRSfs, ((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16u_C4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16u_AC4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16s_C1IRSfs, ((Ipp16s )m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16s_C3IRSfs, ((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16s_C4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16s_AC4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_8u_C1IRSfs,  ((Ipp8u  )m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_8u_C3IRSfs,  ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_8u_C4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_8u_AC4IRSfs, ((Ipp8u *)m_value, (Ipp8u*)pSrc, srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16u_C1IRSfs, ((Ipp16u )m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16u_C3IRSfs, ((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16u_C4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16u_AC4IRSfs,((Ipp16u*)m_value, (Ipp16u*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16s_C1IRSfs, ((Ipp16s )m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16s_C3IRSfs, ((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16s_C4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16s_AC4IRSfs,((Ipp16s*)m_value, (Ipp16s*)pSrc,srcStep,roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32f_C1IR, ((Ipp32f )m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiAddC_32f_C3IR, ((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiAddC_32f_C4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiAddC_32f_AC4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiSubC_32f_C1IR, ((Ipp32f )m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiSubC_32f_C3IR, ((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiSubC_32f_C4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiSubC_32f_AC4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiMulC_32f_C1IR, ((Ipp32f )m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiMulC_32f_C3IR, ((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiMulC_32f_C4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiMulC_32f_AC4IR,((Ipp32f*)m_value, (Ipp32f*)pSrc,srcStep,roiSize))
   FUNC_CALL(ippiMulCScale_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u )m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)m_value, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_C1IR, ((Ipp8u )m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_C3IR, ((Ipp8u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_C4IR, ((Ipp8u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_8u_AC4IR, ((Ipp8u*)m_value, (Ipp8u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u )m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)m_value, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C1IR, ((Ipp16u )m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C3IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_C4IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulCScale_16u_AC4IR, ((Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize))


   FUNC_CALL(ippiAddC_32fc_C1R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc)m_value, (Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiAddC_32fc_C3R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
//   FUNC_CALL(ippiAddC_32fc_C4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiAddC_32fc_AC4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiSubC_32fc_C1R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc)m_value, (Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiSubC_32fc_C3R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
//   FUNC_CALL(ippiSubC_32fc_C4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiSubC_32fc_AC4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiMulC_32fc_C1R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc)m_value, (Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiMulC_32fc_C3R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
//   FUNC_CALL(ippiMulC_32fc_C4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiMulC_32fc_AC4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiDivC_32fc_C1R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc)m_value, (Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiDivC_32fc_C3R,  ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
//   FUNC_CALL(ippiDivC_32fc_C4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiDivC_32fc_AC4R, ((Ipp32fc*)pSrc,srcStep, (Ipp32fc*)m_value,(Ipp32fc*)pDst, dstStep,  roiSize))
   FUNC_CALL(ippiAddC_32fc_C1IR, ((Ipp32fc)m_value, (Ipp32fc*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiAddC_32fc_C3IR, ((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
//   FUNC_CALL(ippiAddC_32fc_C4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiAddC_32fc_AC4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiSubC_32fc_C1IR, ((Ipp32fc)m_value, (Ipp32fc*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiSubC_32fc_C3IR, ((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
//   FUNC_CALL(ippiSubC_32fc_C4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiSubC_32fc_AC4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiMulC_32fc_C1IR, ((Ipp32fc)m_value, (Ipp32fc*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiMulC_32fc_C3IR, ((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
//   FUNC_CALL(ippiMulC_32fc_C4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiMulC_32fc_AC4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiDivC_32fc_C1IR, ((Ipp32fc)m_value, (Ipp32fc*)pSrc, srcStep, roiSize))
   FUNC_CALL(ippiDivC_32fc_C3IR, ((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
//   FUNC_CALL(ippiDivC_32fc_C4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))
   FUNC_CALL(ippiDivC_32fc_AC4IR,((Ipp32fc*)m_value, (Ipp32fc*)pSrc, srcStep,roiSize))

   FUNC_CALL(ippiAddC_16sc_C1IRSfs,  ((Ipp16sc)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16sc_C3IRSfs,  ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiAddC_16sc_C4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16sc_AC4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_C1IRSfs,  ((Ipp16sc)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_C3IRSfs,  ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiSubC_16sc_C4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_AC4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_C1IRSfs,  ((Ipp16sc)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_C3IRSfs,  ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiMulC_16sc_C4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_AC4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_C1IRSfs,  ((Ipp16sc)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_C3IRSfs,  ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiDivC_16sc_C4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_AC4IRSfs, ((Ipp16sc*)m_value, (Ipp16sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16sc_C1RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc)m_value, (Ipp16sc*)pDst,dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16sc_C3RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiAddC_16sc_C4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_16sc_AC4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_C1RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc)m_value, (Ipp16sc*)pDst,dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_C3RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiSubC_16sc_C4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_16sc_AC4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_C1RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc)m_value, (Ipp16sc*)pDst,dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_C3RSfs,  ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiMulC_16sc_C4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_16sc_AC4RSfs, ((Ipp16sc*)pSrc, srcStep, (Ipp16sc*)m_value,(Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_C1RSfs,  ((Ipp16sc*)pSrc,srcStep, (Ipp16sc)m_value, (Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_C3RSfs,  ((Ipp16sc*)pSrc,srcStep, (Ipp16sc*)m_value, (Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiDivC_16sc_C4RSfs, ((Ipp16sc*)pSrc,srcStep, (Ipp16sc*)m_value, (Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_16sc_AC4RSfs, ((Ipp16sc*)pSrc,srcStep, (Ipp16sc*)m_value, (Ipp16sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_C1IRSfs,  ((Ipp32sc)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_C3IRSfs,  ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiAddC_32sc_C4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_AC4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_C1IRSfs,  ((Ipp32sc)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_C3IRSfs,  ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiSubC_32sc_C4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_AC4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_C1IRSfs,  ((Ipp32sc)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_C3IRSfs,  ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiMulC_32sc_C4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_AC4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_C1IRSfs,  ((Ipp32sc)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_C3IRSfs,  ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiDivC_32sc_C4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_AC4IRSfs, ((Ipp32sc*)m_value, (Ipp32sc*)pSrc, srcStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_C1RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_C3RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiAddC_32sc_C4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiAddC_32sc_AC4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_C1RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_C3RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiSubC_32sc_C4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiSubC_32sc_AC4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_C1RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_C3RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiMulC_32sc_C4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiMulC_32sc_AC4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value,(Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_C1RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_C3RSfs,  ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
//   FUNC_CALL(ippiDivC_32sc_C4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))
   FUNC_CALL(ippiDivC_32sc_AC4RSfs, ((Ipp32sc*)pSrc, srcStep, (Ipp32sc*)m_value, (Ipp32sc*)pDst, dstStep, roiSize, scaleFactor))

   return stsNoFunction;
}

BOOL CRunValue::BeforeCall()
{
   return TRUE;
}

CString CRunValue::GetHistoryParms()
{
   CMyString parms;
   m_value.Get(parms,", ");
   if (m_value.Length() > 1)
      parms = "{" + parms + "}";
   if (m_Func.Found("Shift")) 
      return parms;
   else
      return parms + ", ..";
}

BOOL CRunValue::IsMovie(CFunc func)
{
   if (func.Found("AddC")) return TRUE;
   if (func.Found("SubC")) return TRUE;
   return FALSE;
}

int CRunValue::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps)
{
   SetMovieScaleFactor(0);
   Ipp8u val;
   if (tim < 0) {
      val = 64;
   } else {
      val = (Ipp8u)GetMovieValueReverse(tim, 200);
   }
   for (int i=0; i<m_value.Length(); i++)
      m_value.Set(i,val);
   //dstInfo << val;
   return 1;
}

void CRunValue::ResetMovieArgs()
{
   ResetMovieScaleFactor();
}
