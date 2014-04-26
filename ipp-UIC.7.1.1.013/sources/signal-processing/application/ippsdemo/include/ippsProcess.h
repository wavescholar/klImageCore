/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsProcess.h: interface for the CippsProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
#define AFX_IPPSPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common\CProcess.h"

//////////////////////////////////////////////////////////////////////
// class CippsProcess provides classification for ippSP library.
//////////////////////////////////////////////////////////////////////

class CippsProcess : public CProcess {
public:
//////////////////////////////////////////////////////////////////////
// Construction
   CippsProcess();

   void InitSP();
   void InitCH();

   static BOOL DepthEssencial(const CFunc& func);
};

#endif // !defined(AFX_IPPSPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
