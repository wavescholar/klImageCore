/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT8x8LS.h: interface for the CRunDCT8x8LS class.
// CRunDCT8x8LS class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDCT8X8LS_H__A8BCD235_504D_4F72_8D41_09FFFA197983__INCLUDED_)
#define AFX_RUNDCT8X8LS_H__A8BCD235_504D_4F72_8D41_09FFFA197983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunDCT8x8.h"

class CRunDCT8x8LS : public CippiRun
{
public:
   CRunDCT8x8LS();
   virtual ~CRunDCT8x8LS();

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PickNextPos();
   virtual BOOL PrepareSrc();
   virtual ImgHeader GetNewDstHeader();
   virtual IppStatus CallIppFunction();
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual void Loop(int num);
   virtual double GetPerfDivisor();
   virtual CString GetHistoryParms();

   char* m_pSrc;
   char* m_pDst;

   Ipp16s m_addVal;
   Ipp8u m_clipDown;
   Ipp8u m_clipUp;
};

#endif // !defined(AFX_RUNDCT8X8LS_H__A8BCD235_504D_4F72_8D41_09FFFA197983__INCLUDED_)
