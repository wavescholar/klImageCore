/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRand.h: interface for the CRunRand class.
// CRunRand class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRAND_H__BAE6534B_EB66_4BA7_BAAC_DEEA46D71CA4__INCLUDED_)
#define AFX_RUNRAND_H__BAE6534B_EB66_4BA7_BAAC_DEEA46D71CA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunRand : public CippiRun
{
public:
   CRunRand();
   virtual ~CRunRand();
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue m_parmNorm[2];
   CValue m_parmUniform[2];
   CValue* m_Parm;
   unsigned int m_seed;
};

#endif // !defined(AFX_RUNRAND_H__BAE6534B_EB66_4BA7_BAAC_DEEA46D71CA4__INCLUDED_)
