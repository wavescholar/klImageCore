/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConv.h: interface for the CRunConv class.
// CRunConv class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCONV_H__D524FAF5_8561_4032_95AB_9541C8BC2351__INCLUDED_)
#define AFX_RUNCONV_H__D524FAF5_8561_4032_95AB_9541C8BC2351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunConv : public CippiRun
{
public:
   CRunConv();
   virtual ~CRunConv();

protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();

   int m_divisor;
};

#endif // !defined(AFX_RUNCONV_H__D524FAF5_8561_4032_95AB_9541C8BC2351__INCLUDED_)
