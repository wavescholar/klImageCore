/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlpha.h: interface for the CRunAlpha class.
// CRunAlpha class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNALPHA_H__B2667393_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
#define AFX_RUNALPHA_H__B2667393_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunAlpha : public CippiRun
{
public:
   CRunAlpha();
   virtual ~CRunAlpha();
protected:
    IppiAlphaType m_alphaType;
    BOOL m_showPremul;
    BOOL m_Inplace;
    CString m_PremulName;
    CValue m_alpha1;
    CValue m_alpha2;

    CippiDemoDoc* m_pDocMpySrc;
    CippiDemoDoc* m_pDocMpySrc2;

    void* m_mpySrc;
    void* m_mpySrc2;
    void* m_srcComp;
    void* m_srcComp2;
    void* m_mpySrcP;
    void* m_mpySrc2P;
    void* m_srcCompP;
    void* m_srcComp2P;

   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();
   virtual IppStatus CallPremul(void* pSrc, int srcStep,
                        void* pDst, int dstStep,
                        CValue& alpha);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetHistoryParms();
   virtual BOOL PrepareDst();
   virtual BOOL BeforeCall();
   virtual void ActivateDst();
   virtual CString GetMpyParms1() {return "";}
   virtual CString GetMpyParms2() {return "";}

   BOOL PrepareMpySrc();
   void PrepareMpyParms();
private:
    void SetMpyHistory(CDemoDoc* pDocSrc, CDemoDoc* pDocDst, CString parms);
};

#endif // !defined(AFX_RUNALPHA_H__B2667393_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
