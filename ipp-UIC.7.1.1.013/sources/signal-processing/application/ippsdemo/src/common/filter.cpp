/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "CFilter.h"
#include "Func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter()
{
   for (int i=0; i<FLT_NUM; i++) {
      m_Enable[i] = FALSE;
      m_Switch[i] = FALSE;
   }
}

CFilter::CFilter(const CFilter& filter)
{
    *this = filter;
}

CFilter& CFilter::operator =(const CFilter& filter)
{
   for (int i=0; i<FLT_NUM; i++) {
      m_Enable[i] = filter.m_Enable[i];
      m_Switch[i] = filter.m_Switch[i];
   }
   return *this;
}

static BOOL isPresent(CFunc func, int flt)
{
   switch (flt) {
   case FLT_I: return func.DescrName().Found(_T("I"));
   case FLT_M: return func.Mask();
   case FLT_R: return func.Roi();
   case FLT_S: return func.Scale();
   default: return FALSE;
   }
}

BOOL CFilter::Apply(CFunc func) const
{
   for (int i=0; i<FLT_NUM; i++) {
      if (m_Enable[i]) {
         if (m_Switch[i]) {
            if (!isPresent(func,i)) return FALSE;
         } else {
            if (isPresent(func,i)) return FALSE;
         }
      }
   }
   return TRUE;
}

BOOL CFilter::Enabled() const
{
   for (int i=0; i<FLT_NUM; i++) {
      if (m_Enable[i]) return TRUE;
   }
   return FALSE;
}
