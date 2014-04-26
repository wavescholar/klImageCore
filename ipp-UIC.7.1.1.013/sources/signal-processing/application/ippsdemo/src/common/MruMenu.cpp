/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MruMenu.cpp: implementation of the CMruMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "MruMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMruMenu::CMruMenu(int maxSize, CString emptyText)
{
   m_MaxSize    = maxSize;
   m_EmptyText  = emptyText;
   m_nCheck   = -1;
}

CMruMenu::~CMruMenu()
{

}

void CMruMenu::SetList(const CStringList* pList, int nChecked)
{
   RemoveAll();
   POSITION pos = pList->GetHeadPosition();
   for (int i=0; i<m_MaxSize && pos; i++)
      Add(pList->GetNext(pos));
   SetCheck(nChecked);
}

void CMruMenu::SetList(const CStringArray* pArray, int nChecked)
{
   RemoveAll();
   for (int i=0; i<m_MaxSize && i<pArray->GetSize(); i++)
      Add(pArray->GetAt(i));
   SetCheck(nChecked);
}

void CMruMenu::SetCheck(int nChecked)
{
   m_nCheck = -1;
   if (nChecked >= 0) {
      if (nChecked < GetSize())
         m_nCheck = nChecked;
      else if (GetSize())
         m_nCheck = 0;
   }
}

void CMruMenu::UpdateMenu(CCmdUI* pCmdUI, FMruValid pValidFunc)
{
   CMenu* pMenu = pCmdUI->m_pMenu;
   if (pMenu == NULL) return;

   int menuSize = pMenu->GetMenuItemCount( );
   if (menuSize < 0) menuSize = 0;
   for (int iMRU = 1; iMRU < menuSize; iMRU++)
      pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

   if (GetSize() == 0) {
      pCmdUI->Enable(FALSE);
      pCmdUI->SetText(m_EmptyText);
      return;
   }

   int nID = pCmdUI->m_nID;
   for (int i=0; i<GetSize(); i++) {
      if (i == 0)
         pCmdUI->SetText(GetAt(i));
      else
         pMenu->InsertMenu(++pCmdUI->m_nIndex, 
            MF_STRING | MF_BYPOSITION, nID, GetAt(i));
      BOOL flag = TRUE;
      if (flag && pValidFunc) flag = pValidFunc(GetAt(i));
      pMenu->EnableMenuItem(nID, MF_BYCOMMAND |  
         (flag ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
//!!      pMenu->CheckMenuItem(nID, MF_BYCOMMAND | 
//!!         (flag ? MF_CHECKED : MF_UNCHECKED));

      nID++;
   }

   pCmdUI->m_nIndexMax = pMenu->GetMenuItemCount();

   pCmdUI->m_bEnableChanged = TRUE;
}
