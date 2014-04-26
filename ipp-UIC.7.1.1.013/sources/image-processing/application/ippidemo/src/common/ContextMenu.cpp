/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ContextMenu.cpp: implementation of the CContextMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "MainFrm.h"
#include "ContextMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContextMenu::CContextMenu()
{

}

CContextMenu::~CContextMenu()
{
}

void CContextMenu::TrackPopup(UINT ident, CPoint point)
{
   LoadMenu(ident);
   TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,
      point.x, point.y, MAIN_FRAME);
   DestroyMenu();
}

void CContextMenu::LoadMenu(UINT nIDResource) 
{
   CreatePopupMenu();
   tmpMenu.DestroyMenu();
   tmpMenu.LoadMenu(nIDResource);
   for (int i=0; i<tmpMenu.GetMenuItemCount(); i++) {
      UINT flags = tmpMenu.GetMenuState(i, MF_BYPOSITION);
      UINT id = tmpMenu.GetMenuItemID(i);
      if (id == (UINT)-1) {
         flags = MF_POPUP;
         id = (UINT)(tmpMenu.GetSubMenu(i)->m_hMenu);
      }
      CString string;
      tmpMenu.GetMenuString(i,string,MF_BYPOSITION);
      AppendMenu(flags, id, string);
   }
}