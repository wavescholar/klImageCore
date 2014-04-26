/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvCyclic.h : interface for the CRunConvCyclic class.
// CRunConvCyclic class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCONVCYCLIC_H__7EA4B735_FE3C_453D_8938_C96E65DB5398__INCLUDED_)
#define AFX_RUNCONVCYCLIC_H__7EA4B735_FE3C_453D_8938_C96E65DB5398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunConvCyclic : public CippsRun  
{
public:
   CRunConvCyclic();
   virtual ~CRunConvCyclic();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual void Loop(int num);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();

   int m_SrcLen;
   int m_DstLen;
   int m_iSrc;
   int m_iDst;
};

#endif // !defined(AFX_RUNCONVCYCLIC_H__7EA4B735_FE3C_453D_8938_C96E65DB5398__INCLUDED_)
