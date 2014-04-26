/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCC.h: interface for the CRunCC class.
// CRunCC class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCC_H__154A8B42_C0DC_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUNCC_H__154A8B42_C0DC_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"
#include "ParmImg2Dlg.h"

class CRunCC : public CippiRun
{
public:
   CRunCC();
   virtual ~CRunCC();
protected:
    virtual CParamDlg* CreateDlg() { return new CParmImg2Dlg;}
   virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUNCC_H__154A8B42_C0DC_11D1_AE6B_444553540000__INCLUDED_)
