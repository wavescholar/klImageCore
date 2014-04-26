/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT8x8.h: interface for the CRunDCT8x8 class.
// CRunDCT8x8 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDCT8X8_H__602B2E46_2D24_499F_B9CD_5B0E21033AB6__INCLUDED_)
#define AFX_RUNDCT8X8_H__602B2E46_2D24_499F_B9CD_5B0E21033AB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDCT8x8 : public CippiRun
{
public:
   CRunDCT8x8();
   virtual ~CRunDCT8x8();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual void Loop(int num);
   virtual double GetPerfDivisor();

   char* m_pSrc;
   char* m_pDst;
   char* m_srcBuffer;
   char* m_dstBuffer;

   void CopyBuffer(CImage* pImg, char* buffer, BOOL toBuffer) ;
   int GetBlockNX();
   int GetBlockNY();
   int GetBlockN();
   int GetBlockSqr();
   int GetBlockStep();
   int GetBufferSize();
};

#endif // !defined(AFX_RUNDCT8X8_H__602B2E46_2D24_499F_B9CD_5B0E21033AB6__INCLUDED_)
