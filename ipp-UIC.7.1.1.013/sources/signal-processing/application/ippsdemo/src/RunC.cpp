/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunC.cpp : implementation of the CRunC class.
// CRunC class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunC.h"
#include "ParmCDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunC::CRunC()
{   
   val.Init(pp8u , 64);
   val.Init(pp8s , 64);
   val.Init(pp16u, 16000);
   val.Init(pp16s, 16000);
   val.Init(pp32u, 130000);
   val.Init(pp32s, 130000);
   val.Init(pp32f, 0.5f);
   val.Init(pp64s, 1700000);
   val.Init(pp64f, 0.5);
   val.Init(pp8sc , 64, 64);
   val.Init(pp16sc, 16000, 16000);
   val.Init(pp32sc, 130000, 130000);
   val.Init(pp32fc, 0.5, 0.5);
   val.Init(pp64sc, 1700000, 1700000);
   val.Init(pp64fc, 0.5, 0.5);

   m_valAdd = val;

   m_valRev = val;
   m_valRev.Init(pp8u, UCHAR_MAX);
   m_valRev.Init(pp16s, SHRT_MAX);
   m_valRev.Init(pp16u, USHRT_MAX);
   m_valRev.Init(pp16sc, SHRT_MAX, SHRT_MAX);
   m_valRev.Init(pp32f, 1.0);
   m_valRev.Init(pp32fc, 1.0, 1.0);
   m_valRev.Init(pp64f, 1.0);
   m_valRev.Init(pp64fc, 1.0, 1.0);

   m_valMul = val;
   m_valMul.Init(pp8u, 2);
   m_valMul.Init(pp16s, 2);
   m_valMul.Init(pp16sc, 2, 2);
   m_valMul.Init(pp32f, 1.5);
   m_valMul.Init(pp32fc, 1.5, 1.5);
   m_valMul.Init(pp64f, 1.5);
   m_valMul.Init(pp64fc, 1.5, 1.5);

   m_valAnd = val;
   m_valAnd.Init(pp8u, 0x40);
   m_valAnd.Init(pp16u, 0x4000);

   m_valShift = val;
   m_valShift.Init(pp32s, 1);

   m_valEmphase = val;
   m_valEmphase.Init(pp32f, 0.95);
}

BOOL CRunC::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   if (func.BaseName() == "Set")
      m_UsedVectors = VEC_SRC;
   if (func.BaseName() == "AddProductC")
      m_UsedVectors = VEC_SRC | VEC_SRC2;

   if (func.BaseName() == "Set"  ||
       func.BaseName() == "AddC" ||
       func.BaseName() == "SubC")    val = m_valAdd;
   if (func.BaseName() == "SubCRev" ||
       func.BaseName() == "DivCRev") val = m_valRev;
   if (func.BaseName() == "MulC" ||
       func.BaseName() == "DivC")    val = m_valMul;
   if (func.BaseName() == "AndC" ||
       func.BaseName() == "OrC"  ||
       func.BaseName() == "XorC")    val = m_valAnd;
   if (func.BaseName() == "LShiftC" ||
       func.BaseName() == "RShiftC")  val = m_valShift;
   if (func.BaseName() == "Preemphasize") val = m_valEmphase;
   
   if (func == "Preemphasize")
      val.Init(pp32f);
   else if (func.Find("Shift") != -1)
      val.Init(pp32s);
   else
      val.Init(func);
   return TRUE;
}

void CRunC::Close()
{
   if (m_Func.BaseName() == "Set" ||
       m_Func.BaseName() == "AddC" ||
       m_Func.BaseName() == "SubC")    m_valAdd   = val;
   if (m_Func.BaseName() == "DivCRev" ||
       m_Func.BaseName() == "SubCRev") m_valRev   = val;
   if (m_Func.BaseName() == "MulC" ||
       m_Func.BaseName() == "DivC")    m_valMul   = val;
   if (m_Func.BaseName() == "AndC" ||
       m_Func.BaseName() == "OrC" ||
       m_Func.BaseName() == "XorC")    m_valAnd   = val;
   if (m_Func.BaseName() == "LShiftC" ||
       m_Func.BaseName() == "RShiftC")  m_valShift = val;
   if (m_Func.BaseName() == "Preemphasize") m_valEmphase = val;
}

CParamDlg* CRunC::CreateDlg() { 
   return new CParmCDlg;
}

void CRunC::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmCDlg *pDlg = (CParmCDlg*)parmDlg;
   if (save) {
      val.Set(pDlg->m_ReStr, pDlg->m_ImStr);
   } else {
      val.Get(pDlg->m_ReStr, pDlg->m_ImStr);
   }
}

IppStatus CRunC::CallIppFunction()
{
   FUNC_CALL(ippsSet_8u,( (Ipp8u)val, (Ipp8u*)pSrc, len ))
   FUNC_CALL(ippsSet_16s,( (Ipp16s)val, (Ipp16s*)pSrc, len ))
   FUNC_CALL(ippsSet_16sc,( (Ipp16sc)val, (Ipp16sc*)pSrc, len ))
   FUNC_CALL(ippsSet_32s,( (Ipp32s)val, (Ipp32s*)pSrc, len ))
   FUNC_CALL(ippsSet_32sc,( (Ipp32sc)val, (Ipp32sc*)pSrc, len ))
   FUNC_CALL(ippsSet_64s,( (Ipp64s)val, (Ipp64s*)pSrc, len ))
   FUNC_CALL(ippsSet_64sc,( (Ipp64sc)val, (Ipp64sc*)pSrc, len ))
   FUNC_CALL(ippsSet_32f,( (Ipp32f)val, (Ipp32f*)pSrc, len ))
   FUNC_CALL(ippsSet_32fc,( (Ipp32fc)val, (Ipp32fc*)pSrc, len ))
   FUNC_CALL(ippsSet_64f,( (Ipp64f)val, (Ipp64f*)pSrc, len ))
   FUNC_CALL(ippsSet_64fc,( (Ipp64fc)val, (Ipp64fc*)pSrc, len ))
   FUNC_CALL(ippsAddC_16s_I, ((Ipp16s)val,  (Ipp16s*)pSrc,  len))
   FUNC_CALL(ippsSubC_16s_I, ((Ipp16s)val,  (Ipp16s*)pSrc,  len))
   FUNC_CALL(ippsMulC_16s_I, ((Ipp16s)val,  (Ipp16s*)pSrc,  len))
   FUNC_CALL(ippsAddC_32f_I, ((Ipp32f)val,  (Ipp32f*)pSrc,  len))
   FUNC_CALL(ippsSubC_32f_I, ((Ipp32f)val,  (Ipp32f*)pSrc,  len))
   FUNC_CALL(ippsMulC_32f_I, ((Ipp32f)val,  (Ipp32f*)pSrc,  len))
   FUNC_CALL(ippsAddC_64f_I, ((Ipp64f)val,  (Ipp64f*)pSrc,  len))
   FUNC_CALL(ippsSubC_64f_I, ((Ipp64f)val,  (Ipp64f*)pSrc,  len))
   FUNC_CALL(ippsMulC_64f_I, ((Ipp64f)val,  (Ipp64f*)pSrc,  len))
   FUNC_CALL(ippsAddC_32fc_I, ((Ipp32fc)val, (Ipp32fc*)pSrc, len))
   FUNC_CALL(ippsSubC_32fc_I, ((Ipp32fc)val, (Ipp32fc*)pSrc, len))
   FUNC_CALL(ippsMulC_32fc_I, ((Ipp32fc)val, (Ipp32fc*)pSrc, len))
   FUNC_CALL(ippsAddC_64fc_I, ((Ipp64fc)val, (Ipp64fc*)pSrc, len))
   FUNC_CALL(ippsSubC_64fc_I, ((Ipp64fc)val, (Ipp64fc*)pSrc, len))
   FUNC_CALL(ippsMulC_64fc_I, ((Ipp64fc)val, (Ipp64fc*)pSrc, len))
   FUNC_CALL(ippsAddC_16s_ISfs, ((Ipp16s)val , (Ipp16s* )pSrc, len, scaleFactor))
   FUNC_CALL(ippsSubC_16s_ISfs, ((Ipp16s)val , (Ipp16s* )pSrc, len, scaleFactor))
   FUNC_CALL(ippsMulC_16s_ISfs, ((Ipp16s)val , (Ipp16s* )pSrc, len, scaleFactor))
   FUNC_CALL(ippsAddC_16sc_ISfs,((Ipp16sc)val, (Ipp16sc*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsSubC_16sc_ISfs,((Ipp16sc)val, (Ipp16sc*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsMulC_16sc_ISfs,((Ipp16sc)val, (Ipp16sc*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_32f_I, ((Ipp32f )val, (Ipp32f*) pSrc, len))
   FUNC_CALL(ippsSubCRev_32fc_I, ((Ipp32fc)val, (Ipp32fc*)pSrc, len))
   FUNC_CALL(ippsSubCRev_64f_I, ((Ipp64f )val, (Ipp64f*) pSrc, len))
   FUNC_CALL(ippsSubCRev_64fc_I, ((Ipp64fc)val, (Ipp64fc*)pSrc, len))
   FUNC_CALL(ippsSubCRev_16s_ISfs, ((Ipp16s)val, (Ipp16s*)pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubCRev_16sc_ISfs, ((Ipp16sc)val, (Ipp16sc*)pSrc,
       len, scaleFactor))

   FUNC_CALL(ippsMulC_32f,     ((Ipp32f*)pSrc, (Ipp32f)val, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsMulC_64f,     ((Ipp64f*)pSrc, (Ipp64f)val, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsDivC_32f,     ((Ipp32f*)pSrc, (Ipp32f)val, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsDivC_32fc,    ((Ipp32fc*)pSrc,(Ipp32fc)val,(Ipp32fc*)pDst,len))
   FUNC_CALL(ippsDivC_64f,     ((Ipp64f*)pSrc, (Ipp64f)val, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsDivC_64fc,    ((Ipp64fc*)pSrc,(Ipp64fc)val,(Ipp64fc*)pDst,len))
   FUNC_CALL(ippsDivC_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s)val, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDivC_8u_Sfs,  ((Ipp8u*)pSrc,  (Ipp8u)val,  (Ipp8u*)pDst,  len, scaleFactor))
   FUNC_CALL(ippsDivC_16sc_Sfs,((Ipp16sc*)pSrc,(Ipp16sc)val,(Ipp16sc*)pDst,len, scaleFactor))

   FUNC_CALL(ippsDivC_32f_I, ((Ipp32f)val, (Ipp32f*)pSrc, len))
   FUNC_CALL(ippsDivC_32fc_I, ((Ipp32fc)val, (Ipp32fc*)pSrc, len))
   FUNC_CALL(ippsDivC_64f_I, ((Ipp64f)val, (Ipp64f*)pSrc, len))
   FUNC_CALL(ippsDivC_64fc_I, ((Ipp64fc)val, (Ipp64fc*)pSrc, len))
   FUNC_CALL(ippsDivC_16s_ISfs, ((Ipp16s)val, (Ipp16s*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsDivC_8u_ISfs, ((Ipp8u)val, (Ipp8u*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsDivC_16sc_ISfs, ((Ipp16sc)val, (Ipp16sc*)pSrc, len, scaleFactor))
   FUNC_CALL(ippsDivCRev_16u, ((Ipp16u*)pSrc, (Ipp16u)val, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsDivCRev_16u_I, ((Ipp16u)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsDivCRev_32f, ((Ipp32f*)pSrc, (Ipp32f)val, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsDivCRev_32f_I, ((Ipp32f)val, (Ipp32f*)pSrc, len))

   FUNC_CALL(ippsAndC_8u_I, ((Ipp8u)val, (Ipp8u*)pSrc, len))
   FUNC_CALL(ippsOrC_8u_I, ((Ipp8u)val, (Ipp8u*)pSrc, len))
   FUNC_CALL(ippsXorC_8u_I, ((Ipp8u)val, (Ipp8u*)pSrc, len))
   FUNC_CALL(ippsAndC_16u_I, ((Ipp16u)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsOrC_16u_I, ((Ipp16u)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsXorC_16u_I, ((Ipp16u)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsAndC_32u_I, ((Ipp32u)val, (Ipp32u*)pSrc, len))
   FUNC_CALL(ippsOrC_32u_I, ((Ipp32u)val, (Ipp32u*)pSrc, len))
   FUNC_CALL(ippsXorC_32u_I, ((Ipp32u)val, (Ipp32u*)pSrc, len))

   FUNC_CALL(ippsAndC_8u, ((Ipp8u*)pSrc, (Ipp8u)val, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsOrC_8u,  ((Ipp8u*)pSrc, (Ipp8u)val, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsXorC_8u, ((Ipp8u*)pSrc, (Ipp8u)val, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsAndC_16u,((Ipp16u*)pSrc,(Ipp16u)val,(Ipp16u*)pDst, len))
   FUNC_CALL(ippsOrC_16u, ((Ipp16u*)pSrc,(Ipp16u)val,(Ipp16u*)pDst, len))
   FUNC_CALL(ippsXorC_16u,((Ipp16u*)pSrc,(Ipp16u)val,(Ipp16u*)pDst, len))
   FUNC_CALL(ippsAndC_32u,((Ipp32u*)pSrc,(Ipp32u)val,(Ipp32u*)pDst, len))
   FUNC_CALL(ippsOrC_32u, ((Ipp32u*)pSrc,(Ipp32u)val,(Ipp32u*)pDst, len))
   FUNC_CALL(ippsXorC_32u,((Ipp32u*)pSrc,(Ipp32u)val,(Ipp32u*)pDst, len))

   FUNC_CALL(ippsLShiftC_8u_I,  ((int)val, (Ipp8u*)pSrc, len))
   FUNC_CALL(ippsLShiftC_16u_I, ((int)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsLShiftC_16s_I, ((int)val, (Ipp16s*)pSrc, len))
   FUNC_CALL(ippsLShiftC_32s_I, ((int)val, (Ipp32s*)pSrc, len))
   FUNC_CALL(ippsRShiftC_8u_I,  ((int)val, (Ipp8u*)pSrc,  len))
   FUNC_CALL(ippsRShiftC_16u_I, ((int)val, (Ipp16u*)pSrc, len))
   FUNC_CALL(ippsRShiftC_16s_I, ((int)val, (Ipp16s*)pSrc, len))
   FUNC_CALL(ippsRShiftC_32s_I, ((int)val, (Ipp32s*)pSrc, len))

   FUNC_CALL(ippsLShiftC_8u,    ((Ipp8u*)pSrc,  (int)val, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsLShiftC_16u,   ((Ipp16u*)pSrc, (int)val, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsLShiftC_16s,   ((Ipp16s*)pSrc, (int)val, (Ipp16s*)pDst, len))
   FUNC_CALL(ippsLShiftC_32s,   ((Ipp32s*)pSrc, (int)val, (Ipp32s*)pDst, len))
   FUNC_CALL(ippsRShiftC_8u,    ((Ipp8u*)pSrc,  (int)val, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsRShiftC_16u,   ((Ipp16u*)pSrc, (int)val, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsRShiftC_16s,   ((Ipp16s*)pSrc, (int)val, (Ipp16s*)pDst, len))
   FUNC_CALL(ippsRShiftC_32s,   ((Ipp32s*)pSrc, (int)val, (Ipp32s*)pDst, len))

   FUNC_CALL(ippsVectorJaehne_8u,  ((Ipp8u*) pSrc, len, (Ipp8u)val ))
   FUNC_CALL(ippsVectorJaehne_8s,  ((Ipp8s*) pSrc, len, (Ipp8s)val ))
   FUNC_CALL(ippsVectorJaehne_16u, ((Ipp16u*)pSrc, len, (Ipp16u)val ))
   FUNC_CALL(ippsVectorJaehne_16s, ((Ipp16s*)pSrc, len, (Ipp16s)val ))
   FUNC_CALL(ippsVectorJaehne_32u, ((Ipp32u*)pSrc, len, (Ipp32u)val ))
   FUNC_CALL(ippsVectorJaehne_32s, ((Ipp32s*)pSrc, len, (Ipp32s)val ))
   FUNC_CALL(ippsVectorJaehne_32f, ((Ipp32f*)pSrc, len, (Ipp32f)val))
   FUNC_CALL(ippsVectorJaehne_64f, ((Ipp64f*)pSrc, len, (Ipp64f)val))

   FUNC_CALL(ippsPreemphasize_32f,((Ipp32f*)pSrc,len, (Ipp32f)val))
   FUNC_CALL(ippsPreemphasize_16s,((Ipp16s*)pSrc,len, (Ipp32f)val))

   FUNC_CALL(ippsAddC_16s_Sfs,    ((Ipp16s*) pSrc, (Ipp16s )val,
       (Ipp16s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsAddC_16sc_Sfs,   ((Ipp16sc*)pSrc, (Ipp16sc)val,
       (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSubC_16s_Sfs,    ((Ipp16s*) pSrc, (Ipp16s )val,
       (Ipp16s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsSubC_16sc_Sfs,   ((Ipp16sc*)pSrc, (Ipp16sc)val,
       (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_16s_Sfs, ((Ipp16s*) pSrc, (Ipp16s )val,
       (Ipp16s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_16sc_Sfs,((Ipp16sc*)pSrc, (Ipp16sc)val,
       (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_16s_Sfs,    ((Ipp16s*) pSrc, (Ipp16s )val,
       (Ipp16s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_16sc_Sfs,   ((Ipp16sc*)pSrc, (Ipp16sc)val,
       (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsAddC_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s )val,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsAddC_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc)val,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSubC_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s )val,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsSubC_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc)val,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_32s_Sfs, ((Ipp32s*) pSrc, (Ipp32s )val,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_32sc_Sfs,((Ipp32sc*)pSrc, (Ipp32sc)val,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s )val,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc)val,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsAddC_32s_ISfs,     ((Ipp32s )val, (Ipp32s*) pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsAddC_32sc_ISfs,    ((Ipp32sc)val, (Ipp32sc*)pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubC_32s_ISfs,     ((Ipp32s )val, (Ipp32s*) pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubC_32sc_ISfs,    ((Ipp32sc)val, (Ipp32sc*)pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubCRev_32s_ISfs,  ((Ipp32s )val, (Ipp32s*) pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubCRev_32sc_ISfs, ((Ipp32sc)val, (Ipp32sc*)pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsMulC_32s_ISfs,     ((Ipp32s )val, (Ipp32s*) pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsMulC_32sc_ISfs,    ((Ipp32sc)val, (Ipp32sc*)pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsAddC_32f,     ((Ipp32f*) pSrc, (Ipp32f )val,
       (Ipp32f*) pDst, len))
   FUNC_CALL(ippsAddC_32fc,    ((Ipp32fc*)pSrc, (Ipp32fc)val,
       (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsSubC_32f,     ((Ipp32f*) pSrc, (Ipp32f )val,
       (Ipp32f*) pDst, len))
   FUNC_CALL(ippsSubC_32fc,    ((Ipp32fc*)pSrc, (Ipp32fc)val,
       (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsSubCRev_32f,  ((Ipp32f*) pSrc, (Ipp32f )val,
       (Ipp32f*) pDst, len))
   FUNC_CALL(ippsSubCRev_32fc, ((Ipp32fc*)pSrc, (Ipp32fc)val,
       (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsMulC_32f,     ((Ipp32f*) pSrc, (Ipp32f )val,
       (Ipp32f*) pDst, len))
   FUNC_CALL(ippsMulC_32fc,    ((Ipp32fc*)pSrc, (Ipp32fc)val,
       (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsAddC_8u_ISfs,      ((Ipp8u  )val, (Ipp8u*)  pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubC_8u_ISfs,      ((Ipp8u  )val, (Ipp8u*)  pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsSubCRev_8u_ISfs,   ((Ipp8u  )val, (Ipp8u*)  pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsMulC_8u_ISfs,      ((Ipp8u  )val, (Ipp8u*)  pSrc,
       len, scaleFactor))
   FUNC_CALL(ippsAddC_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u  )val,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsSubC_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u  )val,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsSubCRev_8u_Sfs,  ((Ipp8u*)  pSrc, (Ipp8u  )val,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u  )val,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsAddC_64f,     ((Ipp64f*) pSrc, (Ipp64f )val,
       (Ipp64f*) pDst, len))
   FUNC_CALL(ippsAddC_64fc,    ((Ipp64fc*)pSrc, (Ipp64fc)val,
       (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsSubC_64f,     ((Ipp64f*) pSrc, (Ipp64f )val,
       (Ipp64f*) pDst, len))
   FUNC_CALL(ippsSubC_64fc,    ((Ipp64fc*)pSrc, (Ipp64fc)val,
       (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsSubCRev_64f,  ((Ipp64f*) pSrc, (Ipp64f )val,
       (Ipp64f*) pDst, len))
   FUNC_CALL(ippsSubCRev_64fc, ((Ipp64fc*)pSrc, (Ipp64fc)val,
       (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsMulC_64f,     ((Ipp64f*) pSrc, (Ipp64f )val,
       (Ipp64f*) pDst, len))
   FUNC_CALL(ippsMulC_64fc,    ((Ipp64fc*)pSrc, (Ipp64fc)val,
       (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsMulC_32f16s_Sfs, ((Ipp32f*)pSrc, (Ipp32f)val, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMulC_Low_32f16s, ((Ipp32f*)pSrc, (Ipp32f)val, (Ipp16s*)pDst, len))
   FUNC_CALL( ippsAddProductC_32f, ( (const Ipp32f*)pSrc, (Ipp32f)val, (Ipp32f*)pSrc2, len ))

   return stsNoFunction;
}

CString CRunC::GetHistoryParms()
{
   return val.String() + ", ..";
}

