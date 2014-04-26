/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiProcess.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPIPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
#define AFX_IPPIPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common\cprocess.h"

//////////////////////////////////////////////////////////////////////
// class CippiProcess provides classification for ippIP, ippCC,
// ippCV, ippJP libraries
//////////////////////////////////////////////////////////////////////

class CippiProcess : public CProcess {
public:

//////////////////////////////////////////////////////////////////////
// Construction
   CippiProcess();

/////////////////////////////////////////////////////////////////////////////
// CProcess virtual function overrides

   // GetIdFilter returns FilterDialog ID for IPP library with specified
   // index
   virtual UINT GetIdFilter(int idx);

protected:
/////////////////////////////////////////////////////////////////////////////
// Implementation

   // Init{IP/CC/CV/JP} initializes classification for
   // ippIP/ippCC/ippCV/ippJP libraries
   void InitIP();
   void InitCC();
   void InitJP();
   void InitCV();
};

#endif // !defined(AFX_IPPIPROCESS_H__0BEF2D62_BEB9_4ED5_B128_8B84AC5ABBDA__INCLUDED_)
