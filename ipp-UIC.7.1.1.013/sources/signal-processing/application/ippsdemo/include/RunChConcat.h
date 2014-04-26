
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChConcat.h : interface for the CRunChConcat class.
// CRunChConcat class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNCONCAT_H__2FBC883B_E2C3_46E9_BA8F_2227BB315F32__INCLUDED_)
#define AFX_RUNCONCAT_H__2FBC883B_E2C3_46E9_BA8F_2227BB315F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChConcat : public CippsRun  
{
public:
   CRunChConcat();
   virtual ~CRunChConcat();
protected:
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();

};

#endif // !defined(AFX_RUNCONCAT_H__2FBC883B_E2C3_46E9_BA8F_2227BB315F32__INCLUDED_)
