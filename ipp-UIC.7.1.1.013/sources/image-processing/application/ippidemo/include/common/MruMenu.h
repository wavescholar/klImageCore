/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MruMenu.h: interface for the CMruMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRUMENU_H__509F35AB_8695_4D1C_8F2A_EFE914069AA4__INCLUDED_)
#define AFX_MRUMENU_H__509F35AB_8695_4D1C_8F2A_EFE914069AA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL (*FMruValid)(CString name);

class CMruMenu : public CStringArray 
{
public:
   CMruMenu(int maxSize, CString emptyText = _T(""));
   virtual ~CMruMenu();

   void SetList(const CStringList* pList, int nChecked = -1);
   void SetList(const CStringArray* pArray, int nChecked = -1);
   void UpdateMenu(CCmdUI* pCmdUI, FMruValid pValidFunc = NULL);

protected:
   int m_MaxSize  ;
   CString m_EmptyText;
   int m_nCheck;

   void SetCheck(int nChecked);
};

#endif // !defined(AFX_MRUMENU_H__509F35AB_8695_4D1C_8F2A_EFE914069AA4__INCLUDED_)
