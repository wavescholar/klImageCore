/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Filter.h: interface for the CFilter class.
//
// class CFilter implements IPP function filtration through
// Inplace, Mask, ROI, ScaleFactor filters
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTER_APPLY_H__891C1331_000A_4E39_B4C6_1C8A135E6459__INCLUDED_)
#define AFX_FILTER_APPLY_H__891C1331_000A_4E39_B4C6_1C8A135E6459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"

enum {FLT_I, FLT_M, FLT_R, FLT_S, FLT_NUM};

class CFilter : public CObject {
public:
   CFilter();
   virtual ~CFilter(){}
   CFilter(const CFilter& filter);
   CFilter& operator =(const CFilter& filter);

   BOOL Enabled() const;

   BOOL Apply(CFunc func) const;

   BOOL m_Enable[FLT_NUM];
   BOOL m_Switch[FLT_NUM];
};

#endif // !defined(AFX_FILTER_APPLY_H__891C1331_000A_4E39_B4C6_1C8A135E6459__INCLUDED_)
