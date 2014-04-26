/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemoDocMgr.cpp: implementation of the CippsDemoDocMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ippsDemoDocMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CFS {
public:
   CFS() {};
   CFS(CString name, CString ext) : m_name(name), m_ext(ext){};
   CString FilterString() {
      CString str;
      str += m_name + " (*." + m_ext + ")";
      str += '\0';
      str += "*." + m_ext;
      str += '\0';
      return str;
   }

   CString m_name;
   CString m_ext;
};

BOOL CippsDemoDocMgr::DoPromptFileName(CString& fileName, UINT nIDSTitle, 
                                   DWORD lFlags, BOOL bOpenFileDialog, 
                                   CDocTemplate* pTemplate)
{ 
   CFileDialog dlgFile(bOpenFileDialog);

   CString title;
   VERIFY(title.LoadString(nIDSTitle));

   dlgFile.m_ofn.Flags |= lFlags;

   CFS sound ("Sounds" , "wav");
   CFS text("Text" , "txt");

   CString strFilter;
   strFilter += sound.FilterString(); 
   if (bOpenFileDialog || ACTIVE_DOC->GetVector()->Type() == pp8u) {
      strFilter += text.FilterString();
   }
   strFilter += '\0';

   BOOL bText = FALSE;
   if (!bOpenFileDialog) {
      CString docString;
      pTemplate->GetDocString(docString, CDocTemplate::docName);
      bText = docString == "String";
   }

   dlgFile.m_ofn.lpstrFilter = strFilter;
   dlgFile.m_ofn.nFilterIndex = bText ? 2 : 1;
   dlgFile.m_ofn.lpstrTitle = title;
   dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
//   dlgFile.m_ofn.lpstrDefExt = sound.m_ext;   
   dlgFile.m_ofn.lpstrDefExt = "";
 
   BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
   fileName.ReleaseBuffer();

   return bResult;
}
