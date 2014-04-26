
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRow.h : interface for the CRunRow class.
// CRunRow class processes vectors by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNROW_H__
#define __RUNROW_H__

#pragma once

#include "ippiRun.h"

class CRunRow: public CippiRun  
{
public:
   CRunRow();
   virtual ~CRunRow();
protected:
   virtual BOOL Open(CFunc func);
   virtual int GetSrcBorder(int idx);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_maskSize;
   int m_anchor;
   BOOL m_bCenter;
};

#endif // !defined __RUNROW_H__
