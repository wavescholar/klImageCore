/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT8x8LS.cpp: implementation of the CRunDCT8x8LS class.
// CRunDCT8x8LS class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunDCT8x8LS.h"
#include "ParmDCT8x8LSDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDCT8x8LS::CRunDCT8x8LS()
{
   m_pSrc = NULL;
   m_pDst = NULL;

   m_addVal = 0;
   m_clipDown = 0;
   m_clipUp = 255;

}

CRunDCT8x8LS::~CRunDCT8x8LS()
{

}

CParamDlg* CRunDCT8x8LS::CreateDlg() {
   if (m_Func.Found("LS"))
      return new CParmDCT8x8LSDlg;
   else
      return NULL;
}

void CRunDCT8x8LS::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmDCT8x8LSDlg* pDlg = (CParmDCT8x8LSDlg*)parmDlg;
   if (save) {
      m_addVal   = atoi(pDlg->m_ValStr );
      m_clipDown = atoi(pDlg->m_ClipStr[0]);
      m_clipUp   = atoi(pDlg->m_ClipStr[1]);
   } else {
      pDlg->m_ValStr .Format("%d",m_addVal  );
      pDlg->m_ClipStr[0].Format("%d",m_clipDown);
      pDlg->m_ClipStr[1].Format("%d",m_clipUp  );
   }
}

BOOL CRunDCT8x8LS::PrepareSrc()
{
   if (m_Func.Found("Fwd")) {
      IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
      int width  = (roi.width  >> 3) << 3;
      int height = (roi.height >> 3) << 3;
      if (m_pDocSrc->GetImage()->GetCoi() < 0) {
         if (m_pDocSrc->GetImage()->Is422() || m_pDocSrc->GetImage()->Is420())
            width  = (roi.width  >> 4) << 4;
         if (m_pDocSrc->GetImage()->Is420())
            height  = (roi.height >> 4) << 4;
      }
      if ((width == roi.width) &&
          (height == roi.height)) return TRUE;
      roi.width = width;
      roi.height = height;
      if (DEMO_APP->MessageBox(
         (m_pDocSrc->GetImage()->GetRoi() ? " ROI will be updated in " :
                                " ROI will be created in ") +
         m_pDocSrc->GetTitle(),
         MB_OKCANCEL) != IDOK) return FALSE;
      m_pDocSrc->GetImage()->SetRoi(&roi);
   }
   return TRUE;
}

ImgHeader CRunDCT8x8LS::GetNewDstHeader()
{
   ImgHeader header = CippiRun::GetNewDstHeader();
   if (m_Func.Found("Fwd")) {
      IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
      header.SetSize(roi.width, roi.height);
   } else {
      header.SetSize(m_pDocSrc->GetImage()->Width (), m_pDocSrc->GetImage()->Height());
   }
   return header;
}

BOOL CRunDCT8x8LS::PickNextPos()
{
   m_PickVecPos = -1;
   return FALSE;
}

BOOL CRunDCT8x8LS::CallIpp(BOOL bMessage)
{
   BOOL result = FALSE;
   int nx = roiSize.width >> 3;
   int ny = roiSize.height >> 3;
   if (m_Func.Found("Fwd")) {
      int srcStepX = m_pDocSrc->GetImage()->Depth();
      int srcStepY = m_pDocSrc->GetImage()->Step() << 3;
      int dstStep = m_pDocDst->GetImage()->Depth() << 3;
      m_pDst = (char*)pDstImg;
      for (int j=0; j<ny; j++) {
         m_pSrc = (char*)pSrc + srcStepY*j;
         for (int i=0; i<nx; i++) {
            result = CippiRun::CallIpp(bMessage);
            if (result == FALSE) break;
            m_pDst += dstStep;
            m_pSrc += srcStepX;
         }
      }
   } else {
      int srcStep = m_pDocSrc->GetImage()->Depth() << 3;
      int dstStepX = m_pDocDst->GetImage()->Depth();
      int dstStepY = m_pDocDst->GetImage()->Step() << 3;
      m_pSrc = (char*)pSrcImg;
      for (int j=0; j<ny; j++) {
         m_pDst = (char*)pDst + dstStepY*j;
         for (int i=0; i<nx; i++) {
            result = CippiRun::CallIpp(bMessage);
            if (result == FALSE) break;
            m_pSrc += srcStep;
            m_pDst += dstStepX;
         }
      }
   }
   return result;
}

void CRunDCT8x8LS::Loop(int num)
{
   while (num--) CallIpp(FALSE);
}

double CRunDCT8x8LS::GetPerfDivisor()
{
   return ((roiSize.width >> 3)*(roiSize.height >> 3)) << 6;
}

IppStatus CRunDCT8x8LS::CallIppFunction()
{
   FUNC_CALL(ippiDCT8x8FwdLS_8u16s_C1R,
      ((Ipp8u*)m_pSrc, srcStep, (Ipp16s*)m_pDst, m_addVal ))
   FUNC_CALL(ippiDCT8x8InvLSClip_16s8u_C1R,
      ((Ipp16s*)m_pSrc, (Ipp8u*)m_pDst, dstStep, m_addVal, m_clipDown, m_clipUp ))
   FUNC_CALL(ippiDCT8x8Fwd_8u16s_C1R,
      ((Ipp8u*)m_pSrc, srcStep, (Ipp16s*)m_pDst ))
   FUNC_CALL(ippiDCT8x8Inv_16s8u_C1R,
      ((Ipp16s*)m_pSrc, (Ipp8u*)m_pDst, dstStep ))
   FUNC_CALL(ippiDCT8x8Fwd_16s_C1R,
      ((Ipp16s*)m_pSrc, srcStep, (Ipp16s*)m_pDst ))
   FUNC_CALL(ippiDCT8x8Inv_16s_C1R,
      ((Ipp16s*)m_pSrc, (Ipp16s*)m_pDst, dstStep ))
   return stsNoFunction;
}

CString CRunDCT8x8LS::GetHistoryParms()
{
   CMyString parms;
   parms << m_addVal;
   if (m_Func.Found("Inv"))
      parms << ", " << m_clipDown << ", " << m_clipUp;
   return parms;
}
