/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWT.h: interface for the CRunWT class.
// CRunWT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWT_H__FF625D0E_5800_4B5D_A34A_3AC62EA2BF3D__INCLUDED_)
#define AFX_RUNWT_H__FF625D0E_5800_4B5D_A34A_3AC62EA2BF3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunWT : public CippiRun
{
public:
   CRunWT();
   virtual ~CRunWT();

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PrepareSrc();
   virtual BOOL PrepareDst();
   virtual void ActivateDst();
   virtual ImgHeader GetNewDstHeader();
   virtual void PrepareParameters();
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual BOOL PickNextPos();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual BOOL GetEqualDocPos(int& firstPos, int& secondPos);
   virtual BOOL IsPickVecDst();
   virtual int GetPickVecPosToValid();
   virtual void GrabDoc(CDemoDoc* pDoc);
   virtual int GetSrcBorder(int idx);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual void SetHistory();
   virtual void SetToHistoSrc(CDemoDoc* pDoc, int vecPos);
   virtual CString GetVecName(int vecPos);
   virtual CString GetHistoryParms();

   CippiDemoDoc* m_pDocSrcWT[4];
   CippiDemoDoc* m_pDocDstWT[4];
   void* pSrcWT[4];
   void* pDstWT[4];
   int   srcStepWT[4];
   int   dstStepWT[4];
   BOOL m_NewDstWT[4];

   BOOL m_bFwd;
   BOOL m_bDstBorder;

   void* m_pSpec;
   CVector m_pTaps[2];
   int   m_len[2];
   int   m_anchor[2];
   Ipp8u* m_pBuffer;
   int    m_BufSize;

   CVector m_pTapsFwd[2];
   int     m_anchorFwd[2];
   CVector m_pTapsInv[2];
   int     m_anchorInv[2];

   CString m_InitName;
   CString m_FreeName;
   CString m_GetBufSizeName;

   BOOL m_IsSrcWT[4];
   BOOL m_IsDstWT[4];
   CImage m_ZeroImage;

   IppStatus CallInit(CString name);
   IppStatus CallFree(CString name);
   IppStatus CallGetBufSize(CString name);
   BOOL CreateNewDstWT(int i);
   void SetImageParametersWT(CImage* pImage, void*& roiPtr, int& step);
   void CreateZeroImage();
   void SetHistoryFwd();
   void SetHistoryInv();
};

#endif // !defined(AFX_RUNWT_H__FF625D0E_5800_4B5D_A34A_3AC62EA2BF3D__INCLUDED_)
