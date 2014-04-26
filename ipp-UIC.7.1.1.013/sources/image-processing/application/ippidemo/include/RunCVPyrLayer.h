/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFlood.h: interface for the CRunCVPyrLayer class.
// CRunCVPyrLayer class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNPYRLAYER_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_)
#define AFX_RUNPYRLAYER_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVPyrLayer : public CippiRun
{
public:
   CRunCVPyrLayer();
   virtual ~CRunCVPyrLayer();
protected:
   virtual BOOL Open(CFunc func);
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual BOOL PrepareSrc();

   IppStatus CallInit(CString name);
   IppStatus CallFree(CString name);

   CString m_initName;
   CString m_freeName;

   void* m_pState;
   Ipp32f m_rate;
   CVectorUnit m_pKernel;
   int m_kerSize;
   int m_mode;
};

#endif // !defined(AFX_RUNPYRLAYER_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_)
