/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV3.h: interface for the CRunCV3 class.
// CRunCV3 class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCV3_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_)
#define AFX_RUNCV3_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCV3 : public CippiRun
{
public:
   CRunCV3();
   virtual ~CRunCV3();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PickNextPos();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual BOOL GetEqualDocPos(int& firstPos, int& secondPos);
   virtual int GetPickVecPosToValid();
   virtual void GrabDoc(CDemoDoc* pDoc);
   virtual void PrepareParameters();
   virtual void ActivateDst();
   virtual void SaveInplaceImage();
   virtual void RestoreInplaceImage();
   virtual void SetHistory();
   virtual void SetToHistoSrc(CDemoDoc* pDoc, int vecPos);
   virtual IppStatus CallIppFunction();

   BOOL NextPos();

   CDemoDoc* m_pDocSrc3;
   void*   pSrc3;
   int     srcStep3;

   BOOL m_bThird;
   BOOL m_bMask;

};

#endif // !defined(AFX_RUNCV3_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_)
