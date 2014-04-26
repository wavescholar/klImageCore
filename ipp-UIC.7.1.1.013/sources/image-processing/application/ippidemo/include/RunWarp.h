/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWarp.h: interface for the CRunWarp class.
// CRunWarp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWARP_H__2C58B562_E275_11D2_8EE5_00AA00A03C3C__INCLUDED_)
#define AFX_RUNWARP_H__2C58B562_E275_11D2_8EE5_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

enum {
   WARP_QUAD,
   WARP_PARL,
   WARP_ROI,
   WARP_CUSTOM,
};

class CRunWarp : public CippiRun
{
public:
   CRunWarp();
   virtual ~CRunWarp();

    virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual void CopyDataToNewDst() {}
    virtual ImgHeader GetNewDstHeader();
    virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   enum {NF_AFF, NF_BIL, NF_PER, NF_NUM};
   enum {I_AFF = 2, I_BIL = 2,
         I_PER = 3, J_AFF = 3,
         J_BIL = 4, J_PER = 3};
protected:
    int m_Type;
    Ipp64f m_coefAff[I_AFF][J_AFF];
    Ipp64f m_coefBil[I_BIL][J_BIL];
    Ipp64f m_coefPer[I_PER][J_PER];
    int m_interpolate;

    int     m_NF;
    int     m_I[NF_NUM];
    int     m_J[NF_NUM];
    CString m_BaseName[NF_NUM];
    CString m_TransName[NF_NUM];

   void UpdateAff(CParamDlg* pDlg, BOOL save = TRUE);
   void UpdateBil(CParamDlg* pDlg, BOOL save = TRUE);
   void UpdatePer(CParamDlg* pDlg, BOOL save = TRUE);
};

#endif // !defined(AFX_RUNWARP_H__2C58B562_E275_11D2_8EE5_00AA00A03C3C__INCLUDED_)
