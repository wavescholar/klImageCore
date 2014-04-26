/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Lib.h: interface for the CLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIB_H__7B9C698A_2C5A_4665_97CE_7EE5967425DC__INCLUDED_)
#define AFX_LIB_H__7B9C698A_2C5A_4665_97CE_7EE5967425DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {LIB_LINK, LIB_EXPORT};

class CLib  
{
public:
   CLib();
   CLib(const CLib& lib);
   virtual ~CLib();

   void Init(CMyString libPrefix);
   BOOL ChangeLib(int newType, CMyString newPath);
   BOOL Fit(CMyString libPath);

   HINSTANCE GetHandle() const { return m_Handle;}
   FARPROC   GetProc(LPCTSTR name) const {
      return m_Handle ? ::GetProcAddress(m_Handle, (LPCSTR)name) : NULL;
   }

   static HINSTANCE LoadLib(CMyString libPath, BOOL bMessage = FALSE);
   static void FreeLib(HINSTANCE hLib);

   int GetType() { return m_Type;}
   CString GetPath() { return m_Path;}
   CString GetPrefix() { return m_Prefix;}

   void LoadProfileSettings();
   void SaveProfileSettings();
protected:
   HINSTANCE m_Handle;
   int       m_Type;
   CMyString m_Path;
   CMyString m_Prefix;
};

#endif // !defined(AFX_LIB_H__7B9C698A_2C5A_4665_97CE_7EE5967425DC__INCLUDED_)
