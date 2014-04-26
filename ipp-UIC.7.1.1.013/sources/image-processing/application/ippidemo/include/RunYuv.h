/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunYuv.h: interface for the CRunYuv class.
// CRunYuv class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNYUV_H__F6B9C8BA_AFB8_45F1_BC20_5242C2CC3E7B__INCLUDED_)
#define AFX_RUNYUV_H__F6B9C8BA_AFB8_45F1_BC20_5242C2CC3E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunYuv : public CippiRun
{
public:
   CRunYuv();
   virtual ~CRunYuv();

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* parmDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL PrepareDst();
   virtual IppStatus CallIppFunction();
   virtual void CopyDataToNewDst() {}

   Ipp8u m_alpha;
};

#endif // !defined(AFX_RUNYUV_H__F6B9C8BA_AFB8_45F1_BC20_5242C2CC3E7B__INCLUDED_)
