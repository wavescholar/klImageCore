/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMorph.h: interface for the CRunMorph class.
// CRunMorph class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMORPH_H__D715BDA0_C038_4A4B_A8E4_7B5A00572C00__INCLUDED_)
#define AFX_RUNMORPH_H__D715BDA0_C038_4A4B_A8E4_7B5A00572C00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
#include "ParmMorphDlg.h"

class CRunMorph : public CippiRun
{
public:
   CRunMorph();
   virtual ~CRunMorph();

   enum {MAX_SIDE = CParmMorphDlg::MAX_SIDE,
         MAX_SIZE = CParmMorphDlg::MAX_SIZE, };

protected:
   Ipp8u     m_mask[MAX_SIZE];
   IppiSize  m_maskSize;
   IppiPoint m_anchor;
   int      m_AnchorCustom;

   virtual CParamDlg* CreateDlg() { return new CParmMorphDlg;}
   virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int GetSrcBorder(int idx);
};

#endif // !defined(AFX_RUNMORPH_H__D715BDA0_C038_4A4B_A8E4_7B5A00572C00__INCLUDED_)
