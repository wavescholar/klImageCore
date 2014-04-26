/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunGoer.cpp : implementation of the CRunGoer class.
// CRunGoer class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunGoer.h"
#include "ParmGoerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunGoer::CRunGoer()
{
   m_Freq[0].Init(pp32f,0.5);
   m_Freq[0].Init(pp64f,0.5);
   m_Freq[0].Init(pp16s,512);
   m_Freq[1].Init(pp32f,0.5);
   m_Freq[1].Init(pp16s,512);
}

CRunGoer::~CRunGoer()
{

}

BOOL CRunGoer::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   ppType valType = (ppType)(func.SrcType() | PP_CPLX);
   ppType freqType = valType == pp64fc ? pp64f : pp32f;
   if (m_Func.Found("Q15"))
      freqType = pp16s;
   pFreq.Init(freqType,2);
   pVal.Init(valType,2);
   for (int i=0; i<2; i++) {
      m_Freq[i].Init(freqType);
      m_Val[i].Init(valType);
      pFreq.Set(i,m_Freq[i]);
      pVal.Set(i,m_Val[i]);
   }
   return TRUE;
}

void CRunGoer::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmGoerDlg *pDlg = (CParmGoerDlg*)parmDlg;
   for (int i=0; i<2; i++) {
      if (save) {
         m_Freq[i].Set(atof(pDlg->m_FreqStr[i]));
         pFreq.Set(i,m_Freq[i]);
      } else {
         pDlg->m_FreqStr[i] = m_Freq[i].String();
         pVal.Get(i, m_Val[i]);
         m_Val[i].Get((pDlg->m_Re)[i], (pDlg->m_Im)[i]);
      }
   }
}

BOOL CRunGoer::CallIpp(BOOL bMessage)
{
   CParmGoerDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunGoer::SetValues(CParmGoerDlg* pDlg)
{
   UpdateData(pDlg);
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunGoer::CallIppFunction()
{
   FUNC_CALL(ippsGoertz_16s_Sfs, ((Ipp16s*)pSrc, len, (Ipp16sc*)pVal, ((Ipp32f*)pFreq)[0], scaleFactor))
   FUNC_CALL(ippsGoertz_32f,     ((Ipp32f*)pSrc, len, (Ipp32fc*)pVal, ((Ipp32f*)pFreq)[0]))
   FUNC_CALL(ippsGoertz_32fc,     ((Ipp32fc*)pSrc, len, (Ipp32fc*)pVal, ((Ipp32f*)pFreq)[0]))
   FUNC_CALL(ippsGoertz_64fc,     ((Ipp64fc*)pSrc, len, (Ipp64fc*)pVal, ((Ipp64f*)pFreq)[0]))
   FUNC_CALL(ippsGoertz_16sc_Sfs, ((Ipp16sc*)pSrc, len, (Ipp16sc*)pVal, ((Ipp32f*)pFreq)[0], scaleFactor))

   FUNC_CALL(ippsGoertzTwo_32fc,     ((Ipp32fc*)pSrc, len, (Ipp32fc*)pVal, (Ipp32f*)pFreq))
   FUNC_CALL(ippsGoertzTwo_64fc,     ((Ipp64fc*)pSrc, len, (Ipp64fc*)pVal, (Ipp64f*)pFreq))
   FUNC_CALL(ippsGoertzTwo_16sc_Sfs, ((Ipp16sc*)pSrc, len, (Ipp16sc*)pVal, (Ipp32f*)pFreq, scaleFactor))

   FUNC_CALL(ippsGoertzQ15_16sc_Sfs,    ((Ipp16sc*)pSrc, len, (Ipp16sc*)pVal, ((Ipp16s*)pFreq)[0], scaleFactor))
   FUNC_CALL(ippsGoertzTwoQ15_16sc_Sfs, ((Ipp16sc*)pSrc, len, (Ipp16sc*)pVal, (Ipp16s*)pFreq, scaleFactor))
   return stsNoFunction;
}

CString CRunGoer::GetHistoryParms()
{
   if (m_Func.Find("Two") != -1)
      return "{" + m_Freq[0].String() + ", " + m_Freq[1].String() + "}, ..";
   else
      return m_Freq[0].String() + ", ..";
}
