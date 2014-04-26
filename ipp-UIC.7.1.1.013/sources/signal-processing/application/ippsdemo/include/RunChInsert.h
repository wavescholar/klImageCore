
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChInsert.h : interface for the CRunChInsert class.
// CRunChInsert class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNINSERT_H__BAE77947_9B93_4476_B341_C44CC00C4B85__INCLUDED_)
#define AFX_RUNINSERT_H__BAE77947_9B93_4476_B341_C44CC00C4B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChInsert : public CippsRun  
{
public:
   CRunChInsert();
   virtual ~CRunChInsert();
protected:
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual BOOL PrepareSrc();
   virtual void PrepareParameters();
   virtual void ActivateDst();
   virtual void SaveInplaceVector();
   virtual void SetInplaceParms();
   virtual void ResetInplaceParms();
   virtual void Loop(int num);
   virtual void SetHistory();
   virtual CString GetHistoryParms();

   int m_len;
   int m_lenSave;
   int m_pos;
   int m_offset;
   int m_sublen;
   BOOL m_bInsert;
};

#endif // !defined(AFX_RUNINSERT_H__BAE77947_9B93_4476_B341_C44CC00C4B85__INCLUDED_)
