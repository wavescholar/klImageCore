/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreWav.h: interface for the CStoreWav class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOREWAV_H__A8277437_ED93_4FCD_AB1F_300F8D4BA282__INCLUDED_)
#define AFX_STOREWAV_H__A8277437_ED93_4FCD_AB1F_300F8D4BA282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>

#include "Func.h"

class CippsDemoDoc;

class CStoreWav
{
public:
   CStoreWav(CippsDemoDoc* pDoc) : m_pDoc(pDoc), m_hFile(NULL), m_bSave(FALSE) 
   { memset(&m_Format,0,sizeof(WAVEFORMATEX));}
   virtual ~CStoreWav() {}

   BOOL Serialize(CFile* pFile, BOOL isStoring);
protected:
   CippsDemoDoc*  m_pDoc;
   BOOL      m_bSave;
   CMyString m_FilePath;
   HMMIO         m_hFile; 
   WAVEFORMATEX  m_Format;

   void Save();
   void Load();
   void Close();
   void Error(CString message);
   void Error(MMIOINFO& ioInfo);
   ppType TypeFromFormat();
   void TypeToFormat(ppType type);
   int  LengthFromFormat(DWORD dataSize);
};

#endif // !defined(AFX_STOREWAV_H__A8277437_ED93_4FCD_AB1F_300F8D4BA282__INCLUDED_)
