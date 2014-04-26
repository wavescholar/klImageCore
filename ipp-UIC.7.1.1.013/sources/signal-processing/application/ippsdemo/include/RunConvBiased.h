
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvBiased.h : interface for the CRunConvBiased class.
// CRunConvBiased class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNCONVBIASED_H__
#define __RUNCONVBIASED_H__

#pragma once

#include "ippsRun.h"

class CRunConvBiased: public CippsRun  
{
public:
   CRunConvBiased();
   virtual ~CRunConvBiased();
protected:
   virtual BOOL Open(CFunc func);
   virtual int GetDstLength();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_bias;
   BOOL m_biasAuto;
};

#endif // !defined __RUNCONVBIASED_H__
