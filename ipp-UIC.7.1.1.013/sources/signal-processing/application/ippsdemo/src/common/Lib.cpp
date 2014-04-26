/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Lib.cpp: implementation of the CLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "Lib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLib::CLib()
{
   m_Handle = NULL;
   m_Type = LIB_LINK;
}

void CLib::Init(CMyString libPrefix)
{
   m_Prefix = libPrefix;
   m_Path = (CString)("$(IPPROOT)\\bin\\")
          + (DEMO_APP->ForCpu64() ? "win64\\" : "win32\\")
          + libPrefix
          + (DEMO_APP->ForCpu64() ? "ix.dll" : "px.dll");
}

CLib::CLib(const CLib& lib)
{
   m_Handle = lib.m_Handle;
   m_Type   = lib.m_Type  ;
   m_Path   = lib.m_Path  ;
   m_Prefix = lib.m_Prefix;
}

CLib::~CLib()
{

}

BOOL CLib::ChangeLib(int type, CMyString path)
{
   if (type == LIB_LINK) {
      FreeLib(m_Handle);
      m_Handle = NULL;
      m_Type = type;
      return TRUE;
   }
   if ((type == m_Type) && (path == m_Path))
      return TRUE;
   HINSTANCE handle = LoadLib(path, TRUE);
   if (!handle) return FALSE;
   FreeLib(m_Handle);
   m_Handle = handle;
   m_Type = type;
   m_Path = path;
   return TRUE;
}

static void loadFailMessage(CMyString libPath, LPCSTR prevLibPath = NULL)
{
   CString message;
   char buffer[1024];
   FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
     0, GetLastError(), 0,
     buffer, 1024, NULL);
   message = "Problem with loading " + libPath 
           + (libPath.GetExt().IsEmpty() ? ".dll\n" : "\n");
   message += buffer;
   if (prevLibPath) {
      message += "\n";
      if (prevLibPath[0] == 0) {
         message += "Static library " 
                 +  DEMO_APP->GetLibPrefix() 
                 + (DEMO_APP->ForCpu64() ? "ix" : "px");
      } else {
         message += "Dynamic library ";
         message += prevLibPath;
      }
      message += " will be used";
   }
   AfxMessageBox(message);
}

HINSTANCE CLib::LoadLib(CMyString libPath, BOOL bMessage)
{
   HINSTANCE handle = ::LoadLibrary(libPath);
   if (handle == NULL) {
      if (bMessage)
         loadFailMessage(libPath,NULL);
   }
   return handle;
}

void CLib::FreeLib(HINSTANCE handle)
{
   if (handle)
      ::FreeLibrary(handle);
}

BOOL CLib::Fit(CMyString libPath)
{
   CString libName = libPath.GetTitle();
   if (libName.Find(m_Prefix) != 0) return FALSE;
   CString libSuffix = libName.Mid(m_Prefix.GetLength());
   if (DEMO_APP->ForCpu64()) {
      if (libSuffix == "64") return TRUE;
      if (libSuffix == "ix") return TRUE;
      if (libSuffix == "i7") return TRUE;
   } else {
      if (libSuffix == ""  ) return TRUE;
      if (libSuffix == "px") return TRUE;
      if (libSuffix == "a6") return ippGetCpuType() >= ippCpuPIII;
      if (libSuffix == "w7") return ippGetCpuType() >= ippCpuP4;
      if (libSuffix == "t7") return ippGetCpuType() == ippCpuP4HT ||
                                    ippGetCpuType() == ippCpuP4HT2;
   }
   return FALSE;
}

void CLib::LoadProfileSettings()
{
   m_Path = DEMO_APP->GetProfileString("Main", m_Prefix + "LibPath", "");
}

void CLib::SaveProfileSettings()
{
   if (m_Type == LIB_EXPORT)
      DEMO_APP->WriteProfileString("Main", m_Prefix + "LibPath", m_Path);
}
