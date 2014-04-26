/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemoDocMgr.h: interface for the CippsDemoDocMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODOCMGR_H__F2AB5BAA_D9EA_4C35_A0C9_6911791981D8__INCLUDED_)
#define AFX_DEMODOCMGR_H__F2AB5BAA_D9EA_4C35_A0C9_6911791981D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CippsDemoDocMgr : public CDocManager  
{
public:
   CippsDemoDocMgr() {}
   virtual ~CippsDemoDocMgr() {}
   virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, 
                             DWORD lFlags, BOOL bOpenFileDialog, 
                             CDocTemplate* pTemplate);

};

#endif // !defined(AFX_DEMODOCMGR_H__F2AB5BAA_D9EA_4C35_A0C9_6911791981D8__INCLUDED_)
