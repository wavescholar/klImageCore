/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterWiener.h: interface for the CRunFilterWiener class.
// CRunFilterWiener class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTERWIENER_H__58D0BD9B_6A30_431E_A10A_4733C8863165__INCLUDED_)
#define AFX_RUNFILTERWIENER_H__58D0BD9B_6A30_431E_A10A_4733C8863165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFilterBox.h"

class CRunFilterWiener : public CRunFilterBox
{
public:
   CRunFilterWiener();
   virtual ~CRunFilterWiener();
protected:

   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   BOOL BeforeCall();
   BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_channels;
   CVector m_noise;
   void* m_pBuffer;
};

#endif // !defined(AFX_RUNFILTERWIENER_H__58D0BD9B_6A30_431E_A10A_4733C8863165__INCLUDED_)
