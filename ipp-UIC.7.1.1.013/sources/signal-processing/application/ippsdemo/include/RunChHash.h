
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChHash.h : interface for the CRunChHash class.
// CRunChHash class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNHASH_H__C3E8601F_2832_4F71_9C3D_81339BE77EB4__INCLUDED_)
#define AFX_RUNHASH_H__C3E8601F_2832_4F71_9C3D_81339BE77EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
class CParmChHashDlg;

class CRunChHash : public CippsRun  
{
public:
   CRunChHash();
   virtual ~CRunChHash();
   void SetValues(CParmChHashDlg* pDlg);
   enum {FUNC_HASH, FUNC_SJ2, FUNC_COMP_MSCS, FUNC_NUM};
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   Ipp32u m_result;

};

#endif // !defined(AFX_RUNHASH_H__C3E8601F_2832_4F71_9C3D_81339BE77EB4__INCLUDED_)
