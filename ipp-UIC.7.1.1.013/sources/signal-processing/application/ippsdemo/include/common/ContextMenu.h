/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ContextMenu.h: interface for the CContextMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTEXTMENU_H__34B142D6_5EC5_4CB0_B1A1_58D975A5B070__INCLUDED_)
#define AFX_CONTEXTMENU_H__34B142D6_5EC5_4CB0_B1A1_58D975A5B070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CContextMenu : public CMenu  
{
public:
   CContextMenu();
   virtual ~CContextMenu();
   void TrackPopup(UINT ident, CPoint point);
   virtual void LoadMenu(UINT nIDResource);
protected:
    CMenu tmpMenu;
};

#endif // !defined(AFX_CONTEXTMENU_H__34B142D6_5EC5_4CB0_B1A1_58D975A5B070__INCLUDED_)
