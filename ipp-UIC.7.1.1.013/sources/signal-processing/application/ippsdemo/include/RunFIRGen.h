/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFIRGen.h : interface for the CRunFIRGen class.
// CRunFIRGen class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFIRGEN_H__70FA3EE9_B41F_4A1B_A143_40C3464EC100__INCLUDED_)
#define AFX_RUNFIRGEN_H__70FA3EE9_B41F_4A1B_A143_40C3464EC100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunFIRGen : public CippsRun  
{
public:
   CRunFIRGen();
   virtual ~CRunFIRGen();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc func);
   virtual CString GetHistoryParms();

   IppBool    m_doNormal;
   IppWinType m_winType;
   Ipp64f     m_freq[2];
};

#endif // !defined(AFX_RUNFIRGEN_H__70FA3EE9_B41F_4A1B_A143_40C3464EC100__INCLUDED_)
