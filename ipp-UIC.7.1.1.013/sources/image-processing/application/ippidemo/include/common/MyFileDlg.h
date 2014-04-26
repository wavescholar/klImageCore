/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MyFileDlg.h : interface for the File dialog.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFILEDLG_H__1F30C41E_27B3_4193_891F_16C3F1D0F5BB__INCLUDED_)
#define AFX_MYFILEDLG_H__1F30C41E_27B3_4193_891F_16C3F1D0F5BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyFileDlg : public CFileDialog  
{
public:
   CMyFileDlg(BOOL bOpen = TRUE, CMyString pathName = _T(""), 
      LPCTSTR filterName = NULL, LPCTSTR dlgName = NULL);
   virtual ~CMyFileDlg();
    virtual int DoModal(CMyString& pathName);
    virtual int DoModal(CMyStringList& pathList, CMyString& dirName);
protected:
    CString m_Filter;
    CString m_Dir;
    CString m_Ext;
    CString m_DlgTitle;

    int CreateFileList(_TCHAR* fileName, CMyStringList& pathList, CString& dir);
};

#endif // !defined(AFX_MYFILEDLG_H__1F30C41E_27B3_4193_891F_16C3F1D0F5BB__INCLUDED_)
