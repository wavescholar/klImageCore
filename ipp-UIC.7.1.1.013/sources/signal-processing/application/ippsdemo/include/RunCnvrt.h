/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCnvrt.h : interface for the CRunCnvrt class.
// CRunCnvrt class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCNVRT_H__A59D9B80_F6EC_4925_BEBA_412B3D15F1BA__INCLUDED_)
#define AFX_RUNCNVRT_H__A59D9B80_F6EC_4925_BEBA_412B3D15F1BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunCnvrt : public CippsRun  
{
public:
   CRunCnvrt() : rndmode(ippRndZero) {}
   virtual ~CRunCnvrt(){}
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppRoundMode rndmode;
};

#endif // !defined(AFX_RUNCNVRT_H__A59D9B80_F6EC_4925_BEBA_412B3D15F1BA__INCLUDED_)
