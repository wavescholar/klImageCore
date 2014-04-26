/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCorr.h : interface for the CRunCorr class.
// CRunCorr class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCORR_H__CE390518_B3DC_47ED_B33C_6845FEC9459E__INCLUDED_)
#define AFX_RUNCORR_H__CE390518_B3DC_47ED_B33C_6845FEC9459E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunCorr : public CippsRun  
{
public:
   CRunCorr();
   virtual ~CRunCorr();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();
   virtual CString GetHistoryParms();

   int m_dstLen;
   int m_loLag;
};

#endif // !defined(AFX_RUNCORR_H__CE390518_B3DC_47ED_B33C_6845FEC9459E__INCLUDED_)
