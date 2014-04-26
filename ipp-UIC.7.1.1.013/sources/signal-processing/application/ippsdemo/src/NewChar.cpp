/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewTaps.cpp: implementation of the CNewChar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsRun.h"
#include "NewChar.h"
#include "NewCharDlg.h"

CNewChar::CNewChar()
{
   m_bHello = FALSE;
   m_Type = pp8u;
   m_length = 0;
}

CNewChar::~CNewChar()
{

}

BOOL CNewChar::Dialog()
{
   CNewCharDlg dlg;
   dlg.m_Type     = m_Type    ;
   dlg.m_bHello   = m_bHello  ;
   if (dlg.DoModal() != IDOK) return FALSE;
   m_Type     = dlg.m_Type    ;
   m_bHello   = dlg.m_bHello  ;
   return TRUE;
}

BOOL CNewChar::CreateVector(CVector* pVector) 
{
   CString str("Hello World!");
   int len = m_bHello ? str.GetLength() : m_length;
   pVector->Init(m_Type, len);
   if (len <= 0) return TRUE;
   if (m_bHello) {
      switch (m_Type) {
      case pp8u:
         Ipp8u* pData = (Ipp8u*)pVector->GetData();
         for (int i=0; i<len; i++) 
            pData[i] = str[i];
         break;
      }
   } else {
      switch (m_Type) {
      case pp8u:
         Ipp8u* pData = (Ipp8u*)pVector->GetData();
         for (int i=0; i<len; i++) 
            pData[i] = ' ';
         break;
      }
   }
   return TRUE;
}

