/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MyFileDlg.cpp : implementation of the File dialog.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h"
#include "MyFileDlg.h"
#include "MyString.h"
#include "CDERR.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyFileDlg::CMyFileDlg(BOOL bOpen, CMyString pathName, 
                       LPCTSTR filterName, LPCTSTR dlgTitle) :
            CFileDialog(bOpen)
{
    m_Dir = pathName.GetDir();
    m_Ext = pathName.GetExt();

    m_Filter.Empty();
    if (m_Ext.IsEmpty()) {
        m_Filter += _T("All Files (*.*)");
        m_Filter += '\0';
        m_Filter += _T("*.*");
        m_Filter += '\0';
        m_Filter += '\0';
    } else {
        if (filterName) {
            m_Filter += filterName;
            m_Filter += _T(" ");
        }
        m_Filter += _T("(*") + m_Ext + _T(")");
        m_Filter += '\0';
        m_Filter += _T("*") + m_Ext;
        m_Filter += '\0';
        m_Filter += '\0';
    }
    m_ofn.Flags |= OFN_EXPLORER;
    if (bOpen == TRUE)
        m_ofn.Flags |= OFN_FILEMUSTEXIST;
    else
        m_ofn.Flags |= OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT;
    m_ofn.lpstrFilter = m_Filter;
    m_ofn.nFilterIndex = 1;
    if (!m_Ext.IsEmpty())
      m_ofn.lpstrDefExt = ((LPCTSTR)m_Ext) + 1;
    if (!m_Dir.IsEmpty())
        m_ofn.lpstrInitialDir = m_Dir;

    if (dlgTitle) {
       m_DlgTitle = dlgTitle;
       m_ofn.lpstrTitle = m_DlgTitle;               
    }
}

CMyFileDlg::~CMyFileDlg()
{

}

int CMyFileDlg::DoModal(CMyString& pathName)
{
    CString bufferString;
    _TCHAR   *fileName;
    int     nMaxFile = _MAX_PATH;

    fileName = bufferString.GetBuffer(nMaxFile);    
    fileName[0] = '\0';
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
    _tcscpy_s(fileName,bufferString.GetLength(),pathName.GetName());
#else
    _tcscpy(fileName,pathName.GetName());
#endif

   m_ofn.lpstrFile = fileName;
   m_ofn.nMaxFile = nMaxFile;
    int dlgResult = (int)CFileDialog :: DoModal();
    if (dlgResult == IDOK)
        pathName = fileName;
    return dlgResult;
}

int CMyFileDlg::DoModal(CMyStringList& pathList, CMyString& dirName)
{
    CString bufferString;
    _TCHAR   *fileName;
    int     nMaxFile = _MAX_PATH*4;
retry:
    fileName = bufferString.GetBuffer(nMaxFile);    
    fileName[0] = '\0';
   m_ofn.lpstrFile = fileName;
   m_ofn.nMaxFile = nMaxFile;
    m_ofn.Flags |= OFN_ALLOWMULTISELECT;
    int dlgResult = (int)CFileDialog :: DoModal();
    if (dlgResult == IDOK) {
        CreateFileList(fileName,pathList,dirName); 
        return dlgResult;
    }
    if (CommDlgExtendedError() == FNERR_BUFFERTOOSMALL) {
        nMaxFile = *((WORD*)fileName) + _MAX_PATH;
        if (MessageBox(_T("Buffer was too small for the filenames\n"),
                       _T("Open Error"),
                       MB_RETRYCANCEL) == IDRETRY) {
            bufferString.ReleaseBuffer();
            goto retry;
        }
    }
    return dlgResult;
}

int CMyFileDlg::CreateFileList(_TCHAR* fileName, CMyStringList& pathList, CString& dir)
{
    int     nFileOffset = m_ofn.nFileOffset;
    CMyString name;
    if (fileName[nFileOffset-1] == '\0')
        fileName[nFileOffset-1] = '\\';
    dir = fileName;
    dir = dir.Left(nFileOffset);
    pathList.RemoveAll();
    while (fileName[nFileOffset] != '\0') {
        name = fileName + nFileOffset;
        nFileOffset += name.GetLength() + 1;
        pathList.AddTail(name);
    }
    return (int)pathList.GetCount();
}
