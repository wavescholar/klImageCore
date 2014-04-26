/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreBmp.cpp: implementation of the CStoreBmp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "StoreBmp.h"
#include "ippsDemoDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CStoreBmp::Serialize(CFile* pFile, BOOL isStoring)
{
   ASSERT(m_pDoc);
   m_pFile = pFile;
   CMyString fileName = m_pFile->GetFileName();
   if (fileName.GetExt() != ".bmp") return FALSE;
   if (isStoring) Save();
   else           Load();
   return TRUE;
}

void CStoreBmp::Save()
{
}

void CStoreBmp::Load()
{
}
