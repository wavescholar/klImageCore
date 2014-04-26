/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Histo.cpp: implementation of the CHisto class.
//
// class CHisto provides history information for Demo document
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h" 
#include "Histo.h"
#include "Vector.h"
#include "Lib.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHisto::CHisto()
{

}

CHisto::~CHisto()
{

}

static CString perfString(double perf)
{
   int prec = -(int)log10(perf) + 2;
   if (prec < 0) prec = 0;
   CString fmt;
   CString str;
   fmt.Format(_T("%%.%df"), prec);
   str.Format(fmt, perf);
   return str;
}

void CHisto::AddFuncString(CFunc func, CString funcParms, CString funcInfo, 
                           double perf, CString libSuffix, CString perfUnit)
{
   CString str = _T("\t") + func;
   if (!funcParms.IsEmpty())
      str += _T("(") + funcParms + _T(")");
   if (!funcInfo.IsEmpty())
      str += _T(" - ") + funcInfo;
   if (perf > 0) {
// CLib      if (libSuffix.IsEmpty()) libSuffix = DEMO_APP->GetLibSuffix();
      if (!perfUnit.IsEmpty()) perfUnit = _T("\tclocks per ") + perfUnit;
      str += _T("\t") + libSuffix
          +  _T("\t") + perfString(perf) + perfUnit;
   }
   AddTail(str);
}
