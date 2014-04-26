
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChFind.h : interface for the CRunChFind class.
// CRunChFind class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNFIND_H__FE2D1B80_EDEB_45DB_90F5_67ECA5CA9A78__INCLUDED_)
#define AFX_RUNFIND_H__FE2D1B80_EDEB_45DB_90F5_67ECA5CA9A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChFind : public CippsRun  
{
public:
   CRunChFind();
   virtual ~CRunChFind();

   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual void UpdateData(CParamDlg* parmDlg, BOOL save);
   virtual BOOL AfterCall(BOOL bOk);

   int m_index;
};

#endif // !defined(AFX_RUNFIND_H__FE2D1B80_EDEB_45DB_90F5_67ECA5CA9A78__INCLUDED_)
