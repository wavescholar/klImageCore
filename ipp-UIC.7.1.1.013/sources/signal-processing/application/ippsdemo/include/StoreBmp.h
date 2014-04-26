/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreBmp.h: interface for the CStoreBmp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOREBMP_H__CC58D6D4_ACDA_4B41_B377_2A54E4E91B2E__INCLUDED_)
#define AFX_STOREBMP_H__CC58D6D4_ACDA_4B41_B377_2A54E4E91B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CippsDemoDoc;

class CStoreBmp  
{
public:
   CStoreBmp(CippsDemoDoc* pDoc) : m_pDoc(pDoc), m_pFile(NULL) {}
   virtual ~CStoreBmp() {}

   BOOL Serialize(CFile* pFile, BOOL isStoring);
protected:
   CippsDemoDoc* m_pDoc;
   CFile*   m_pFile;

   void Save();
   void Load();

};

#endif // !defined(AFX_STOREBMP_H__CC58D6D4_ACDA_4B41_B377_2A54E4E91B2E__INCLUDED_)
