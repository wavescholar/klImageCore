/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTranspose.h: interface for the CRunTranspose class.
// CRunTranspose class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTRANSPOSE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNTRANSPOSE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunTranspose : public CippiRun
{
public:
   CRunTranspose();
   virtual ~CRunTranspose();

protected:
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL PrepareSrc();
   virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUNTRANSPOSE_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
