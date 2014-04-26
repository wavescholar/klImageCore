
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopy1u.h : interface for the CRunCopy1u class.
// CRunCopy1u class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNCOPY1U_H__
#define __RUNCOPY1U_H__

#pragma once

#include "ippsRun.h"

class CRunCopy1u: public CippsRun  
{
public:
   CRunCopy1u();
   virtual ~CRunCopy1u();
protected:
   virtual BOOL Open(CFunc func);
   virtual int GetDstLength();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_numBits;
   int m_bitOffset[2];
   int m_itemBits;
   int m_maxBits;
};

#endif // !defined __RUNCOPY1U_H__
