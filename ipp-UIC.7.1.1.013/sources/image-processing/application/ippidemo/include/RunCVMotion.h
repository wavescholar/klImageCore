/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVMotion.h: interface for the CRunCVMotion class.
// CRunCVMotion class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVMOTION_H__B6E1F455_46C1_48C0_A7B7_DD039DD0F4C8__INCLUDED_)
#define AFX_RUNCVMOTION_H__B6E1F455_46C1_48C0_A7B7_DD039DD0F4C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVMotion : public CippiRun
{
public:
   CRunCVMotion();
   virtual ~CRunCVMotion();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PickNextPos();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual void GrabDoc(CDemoDoc* pDoc);
   virtual CString GetHistoryParms();
   virtual IppStatus CallIppFunction();

   BOOL NextPos();

   Ipp32f m_timestamp;
   Ipp32f m_mhiDuration;

};

#endif // !defined(AFX_RUNCVMOTION_H__B6E1F455_46C1_48C0_A7B7_DD039DD0F4C8__INCLUDED_)
