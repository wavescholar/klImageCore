/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThresh.cpp : implementation of the CRunThresh class.
// CRunThresh class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunThresh.h"
#include "ParmThreshDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunThresh::CRunThresh() : relOp(ippCmpLess)
{   
   level.Init(pp16s, 0);
   level.Init(pp32f, 0.1);
   level.Init(pp64f, 0.1);

   value.Init(pp16s, 0);
   value.Init(pp32f, 0);
   value.Init(pp64f, 0);
   value.Init(pp16sc, 0, 0);
   value.Init(pp32fc, 0, 0);
   value.Init(pp64fc, 0, 0);

   m_bValue = FALSE;
}

BOOL CRunThresh::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;

   m_bValue = func.Found("Val");

   int type = func.SrcType();
   value.Init((ppType)type);
   type &= ~PP_CPLX;
   level.Init((ppType)type);
   return TRUE;
}

CParamDlg* CRunThresh::CreateDlg() { 
   return new CParmThreshDlg;
}

void CRunThresh::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmThreshDlg *pDlg = (CParmThreshDlg*)parmDlg;
   if (save) {
      if (m_bValue)
         value.Set(pDlg->m_ValueStr[0], pDlg->m_ValueStr[1]);      
      level.Set(pDlg->m_LevelStr);      
      relOp = (IppCmpOp)pDlg->m_Cmp;
   } else {
      if (m_bValue)
         value.Get(pDlg->m_ValueStr[0], pDlg->m_ValueStr[1]);      
      level.Get(pDlg->m_LevelStr);
      pDlg->m_Cmp = (int)relOp;
   }
}

#define STAT_CALL(Name,Params) { \
   if (m_Func == #Name) { \
      if (m_Outstand) return ippStsNoErr; \
      else return Name Params; \
   } \
}

IppStatus CRunThresh::CallIppFunction()
{
   FUNC_CALL(ippsThreshold_32f_I,( (Ipp32f*)pSrc, len,
       (Ipp32f)level, relOp ))
   FUNC_CALL(ippsThreshold_32fc_I,( (Ipp32fc*)pSrc, len,
       (Ipp32f)level, relOp ))
   FUNC_CALL(ippsThreshold_64f_I,( (Ipp64f*)pSrc, len,
       (Ipp64f)level, relOp ))
   FUNC_CALL(ippsThreshold_64fc_I,( (Ipp64fc*)pSrc, len,
       (Ipp64f)level, relOp ))
   FUNC_CALL(ippsThreshold_16s_I,( (Ipp16s*)pSrc, len,
       (Ipp16s)level, relOp ))
   FUNC_CALL(ippsThreshold_16sc_I,( (Ipp16sc*)pSrc, len,
       (Ipp16s)level,  relOp ))
   FUNC_CALL(ippsThreshold_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst,
       len, (Ipp32f)level, relOp ))
   FUNC_CALL(ippsThreshold_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
       len, (Ipp32f)level, relOp ))
   FUNC_CALL(ippsThreshold_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst,
       len, (Ipp64f)level, relOp ))
   FUNC_CALL(ippsThreshold_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
       len, (Ipp64f)level, relOp ))
   FUNC_CALL(ippsThreshold_16s,( (Ipp16s*)pSrc, (Ipp16s*)pDst,
       len, (Ipp16s)level, relOp ))
   FUNC_CALL(ippsThreshold_16sc,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
       len, (Ipp16s)level, relOp))
   FUNC_CALL(ippsThreshold_LT_32f_I,( (Ipp32f*)pSrc, len,
       (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_LT_32fc_I,( (Ipp32fc*)pSrc, len,
       (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_LT_64f_I,( (Ipp64f*)pSrc, len,
       (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_LT_64fc_I,( (Ipp64fc*)pSrc, len,
       (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_LT_16s_I,( (Ipp16s*)pSrc, len,
       (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_LT_32s_I,( (Ipp32s*)pSrc, len,
       (Ipp32s)level ))
   FUNC_CALL(ippsThreshold_LT_16sc_I,( (Ipp16sc*)pSrc, len,
       (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_LT_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst,
       len, (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_LT_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
       len, (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_LT_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst,
       len, (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_LT_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
       len, (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_LT_16s,( (Ipp16s*)pSrc, (Ipp16s*)pDst,
       len, (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_LT_32s,( (Ipp32s*)pSrc, (Ipp32s*)pDst,
       len, (Ipp32s)level ))
   FUNC_CALL(ippsThreshold_LT_16sc,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
       len, (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_GT_32f_I,( (Ipp32f*)pSrc, len,
       (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_GT_32fc_I,( (Ipp32fc*)pSrc, len,
       (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_GT_64f_I,( (Ipp64f*)pSrc, len,
       (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_GT_64fc_I,( (Ipp64fc*)pSrc, len,
       (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_GT_16s_I,( (Ipp16s*)pSrc, len,
       (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_GT_32s_I,( (Ipp32s*)pSrc, len,
       (Ipp32s)level ))
   FUNC_CALL(ippsThreshold_GT_16sc_I,( (Ipp16sc*)pSrc, len,
       (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_GT_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst,
       len, (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_GT_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
       len, (Ipp32f)level ))
   FUNC_CALL(ippsThreshold_GT_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst,
       len, (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_GT_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
       len, (Ipp64f)level ))
   FUNC_CALL(ippsThreshold_GT_16s,( (Ipp16s*)pSrc, (Ipp16s*)pDst,
       len, (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_GT_32s,( (Ipp32s*)pSrc, (Ipp32s*)pDst,
       len, (Ipp32s)level ))
   FUNC_CALL(ippsThreshold_GT_16sc,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
       len, (Ipp16s)level ))
   FUNC_CALL(ippsThreshold_LTAbs_32f,((const Ipp32f*)pSrc, (Ipp32f*)pDst, len, (Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTAbs_64f,((const Ipp64f*)pSrc, (Ipp64f*)pDst, len, (Ipp64f)level))
   FUNC_CALL(ippsThreshold_LTAbs_16s,((const Ipp16s*)pSrc, (Ipp16s*)pDst, len, (Ipp16s)level))
   FUNC_CALL(ippsThreshold_LTAbs_32s,((const Ipp32s*)pSrc, (Ipp32s*)pDst, len, (Ipp32s)level))
   FUNC_CALL(ippsThreshold_LTAbs_32f_I,((Ipp32f*)pSrc, len, (Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTAbs_64f_I,((Ipp64f*)pSrc, len, (Ipp64f)level))
   FUNC_CALL(ippsThreshold_LTAbs_16s_I,((Ipp16s*)pSrc, len, (Ipp16s)level))
   FUNC_CALL(ippsThreshold_LTAbs_32s_I,((Ipp32s*)pSrc, len, (Ipp32s)level))
   FUNC_CALL(ippsThreshold_GTAbs_32f,((const Ipp32f*)pSrc, (Ipp32f*)pDst, len, (Ipp32f)level))
   FUNC_CALL(ippsThreshold_GTAbs_64f,((const Ipp64f*)pSrc, (Ipp64f*)pDst, len, (Ipp64f)level))
   FUNC_CALL(ippsThreshold_GTAbs_16s,((const Ipp16s*)pSrc, (Ipp16s*)pDst, len, (Ipp16s)level))
   FUNC_CALL(ippsThreshold_GTAbs_32s,((const Ipp32s*)pSrc, (Ipp32s*)pDst, len, (Ipp32s)level))
   FUNC_CALL(ippsThreshold_GTAbs_32f_I,((Ipp32f*)pSrc, len, (Ipp32f)level))
   FUNC_CALL(ippsThreshold_GTAbs_64f_I,((Ipp64f*)pSrc, len, (Ipp64f)level))
   FUNC_CALL(ippsThreshold_GTAbs_16s_I,((Ipp16s*)pSrc, len, (Ipp16s)level))
   FUNC_CALL(ippsThreshold_GTAbs_32s_I,((Ipp32s*)pSrc, len, (Ipp32s)level))
   FUNC_CALL(ippsThreshold_LTInv_32f_I,((Ipp32f*)pSrc,len,(Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTInv_32fc_I,((Ipp32fc*)pSrc,len,(Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTInv_64f_I,((Ipp64f*)pSrc,len,(Ipp64f)level))
   FUNC_CALL(ippsThreshold_LTInv_64fc_I,((Ipp64fc*)pSrc,len,(Ipp64f)level))
   FUNC_CALL(ippsThreshold_LTInv_32f,((Ipp32f*)pSrc,(Ipp32f*)pDst,len,(Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTInv_32fc,((Ipp32fc*)pSrc,(Ipp32fc*)pDst,len,(Ipp32f)level))
   FUNC_CALL(ippsThreshold_LTInv_64f,((Ipp64f*)pSrc,(Ipp64f*)pDst,len,(Ipp64f)level))
   FUNC_CALL(ippsThreshold_LTInv_64fc,((Ipp64fc*)pSrc,(Ipp64fc*)pDst,len,(Ipp64f)level))

   FUNC_CALL(ippsThreshold_LTVal_32f_I,( (Ipp32f*)pSrc, len,
       (Ipp32f)level, (Ipp32f)value ))
   FUNC_CALL(ippsThreshold_LTVal_32fc_I,( (Ipp32fc*)pSrc, len,
       (Ipp32f)level, (Ipp32fc)value ))
   FUNC_CALL(ippsThreshold_LTVal_64f_I,( (Ipp64f*)pSrc, len,
       (Ipp64f)level, (Ipp64f)value ))
   FUNC_CALL(ippsThreshold_LTVal_64fc_I,( (Ipp64fc*)pSrc, len,
       (Ipp64f)level, (Ipp64fc)value ))
   FUNC_CALL(ippsThreshold_LTVal_16s_I,( (Ipp16s*)pSrc, len,
       (Ipp16s)level, (Ipp16s)value ))
   FUNC_CALL(ippsThreshold_LTVal_16sc_I,( (Ipp16sc*)pSrc, len,
       (Ipp16s)level, (Ipp16sc)value ))
   FUNC_CALL(ippsThreshold_LTVal_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst,
       len, (Ipp32f)level, (Ipp32f)value ))
   FUNC_CALL(ippsThreshold_LTVal_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
       len, (Ipp32f)level, (Ipp32fc)value ))
   FUNC_CALL(ippsThreshold_LTVal_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst,
       len, (Ipp64f)level, (Ipp64f)value ))
   FUNC_CALL(ippsThreshold_LTVal_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
       len, (Ipp64f)level, (Ipp64fc)value ))
   FUNC_CALL(ippsThreshold_LTVal_16s,( (Ipp16s*)pSrc, (Ipp16s*)pDst,
       len, (Ipp16s)level, (Ipp16s)value ))
   FUNC_CALL(ippsThreshold_LTVal_16sc,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
       len, (Ipp16s)level, (Ipp16sc)value ))
   FUNC_CALL(ippsThreshold_GTVal_32f_I,( (Ipp32f*)pSrc, len,
       (Ipp32f)level, (Ipp32f)value ))
   FUNC_CALL(ippsThreshold_GTVal_32fc_I,( (Ipp32fc*)pSrc, len,
       (Ipp32f)level, (Ipp32fc)value ))
   FUNC_CALL(ippsThreshold_GTVal_64f_I,( (Ipp64f*)pSrc, len,
       (Ipp64f)level, (Ipp64f)value ))
   FUNC_CALL(ippsThreshold_GTVal_64fc_I,( (Ipp64fc*)pSrc, len,
       (Ipp64f)level, (Ipp64fc)value ))
   FUNC_CALL(ippsThreshold_GTVal_16s_I,( (Ipp16s*)pSrc, len,
       (Ipp16s)level, (Ipp16s)value ))
   FUNC_CALL(ippsThreshold_GTVal_16sc_I,( (Ipp16sc*)pSrc, len,
       (Ipp16s)level, (Ipp16sc)value ))
   FUNC_CALL(ippsThreshold_GTVal_32f,( (Ipp32f*)pSrc, (Ipp32f*)pDst,
       len, (Ipp32f)level, (Ipp32f)value ))
   FUNC_CALL(ippsThreshold_GTVal_32fc,( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
       len, (Ipp32f)level, (Ipp32fc)value ))
   FUNC_CALL(ippsThreshold_GTVal_64f,( (Ipp64f*)pSrc, (Ipp64f*)pDst,
       len, (Ipp64f)level, (Ipp64f)value ))
   FUNC_CALL(ippsThreshold_GTVal_64fc,( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
       len, (Ipp64f)level, (Ipp64fc)value ))
   FUNC_CALL(ippsThreshold_GTVal_16s,( (Ipp16s*)pSrc, (Ipp16s*)pDst,
       len, (Ipp16s)level, (Ipp16s)value ))
   FUNC_CALL(ippsThreshold_GTVal_16sc,( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
       len, (Ipp16s)level, (Ipp16sc)value ))
   return stsNoFunction;
}

CString CRunThresh::GetHistoryParms()
{
   CMyString parm;
   parm << level.String();
   if (m_bValue)
      parm << ", " << value.String();
   if (m_Func.BaseName() == "Threshold")
      parm << ", " << relOp;
   return parm;
}

