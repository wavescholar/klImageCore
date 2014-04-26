/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DragDrop.cpp: implementation of the CDropTitle, CDropSource, CDropTarget 
// classes.
//
// They Provide Drag & Drop Operations on Demo applications
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"
#include "Vector.h"
#include "DragDrop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDragDrop
//////////////////////////////////////////////////////////////////////

CDropSource::CDropSource() : COleDropSource()
{
   m_hGlobalData = NULL;
   m_pDataSource = NULL;
}

CDropSource::~CDropSource()
{

}

BOOL CDropSource::DoDragDrop(CDemoDoc* pDoc, CPoint point)
{
   int size = GetDataSize(pDoc);
   if (CreateDataSource(size)) {

      char* pData = (char*)::GlobalLock(m_hGlobalData);
      WriteData(pDoc, pData);
      ::GlobalUnlock(m_hGlobalData);

      m_pDataSource->CacheGlobalData(DEMO_APP->GetDropFormat(), m_hGlobalData); 

      CRect rect(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
      if (m_pDataSource->DoDragDrop(DROPEFFECT_COPY, 
         rect, this) != DROPEFFECT_NONE) {
         return TRUE;
      }
   }
//   DeleteDataSource();
   return FALSE;
}

BOOL CDropSource::CreateDataSource(int size)
{
   m_hGlobalData = ::GlobalAlloc(GHND, size);
   if (!m_hGlobalData) return FALSE;

   m_pDataSource = new COleDataSource;
   if (!m_pDataSource) return FALSE;

   return TRUE;
}

void CDropSource::DeleteDataSource()
{
   if (m_hGlobalData) ::GlobalFree(m_hGlobalData); m_hGlobalData = NULL;
   if (m_pDataSource) delete m_pDataSource; m_pDataSource = NULL;
}

void CDropSource::WriteData(CDemoDoc* pDoc, char* pData) const
{
   CDropTitle title;
   pDoc->GetDropTitle(&title);
   memcpy(pData, &title, sizeof(CDropTitle)); pData += title.TitleSize;
   pDoc->GetVector()->WriteDropHeader(pData); pData += title.HeaderSize;
   pDoc->GetVector()->WriteDropInfo(pData); pData += title.InfoSize;
   pDoc->GetVector()->WriteDropData(pData); pData += title.DataSize;
}

int CDropSource::GetDataSize(CDemoDoc* pDoc) const
{
   CDropTitle title;
   pDoc->GetDropTitle(&title);
   return 
        title.TitleSize
      + title.HeaderSize
      + title.InfoSize
      + title.DataSize;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
   DROPEFFECT dropEffect, CPoint point)
{
   BOOL result = FALSE;
   HGLOBAL hData = pDataObject->GetGlobalData(DEMO_APP->GetDropFormat());
   if (!hData) return result;

   char* pData = (char*)::GlobalLock(hData);
   if (ReadData(pData)) result = TRUE;
   ::GlobalUnlock(hData);

   return result;
}

DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
   DWORD grfKeyState, CPoint point)
{
   return OnDragOver(pWnd, pDataObject, grfKeyState, point);
}

DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
   DWORD grfKeyState, CPoint point)
{
   DROPEFFECT de = DROPEFFECT_NONE;
   HGLOBAL hData = pDataObject->GetGlobalData(DEMO_APP->GetDropFormat());
   if (!hData) return de;
   char* pData = (char*)::GlobalLock(hData);

   if (ValidData(pData))
      de = DROPEFFECT_COPY;

   ::GlobalUnlock(hData);
   return de;
}

void CDropTarget::OnDragLeave(CWnd* pWnd)
{
}

BOOL CDropTarget::ValidData(const char* pData) const
{
   CDropTitle title;
   memcpy(&title, pData, sizeof(CDropTitle)); pData += title.TitleSize;
   return DEMO_APP->ValidDropHeader(pData, title.DropObject);
}

BOOL CDropTarget::ReadData(const char* pData) const
{
   CDropTitle title;
   memcpy(&title, pData, sizeof(CDropTitle)); pData += title.TitleSize;
   CDemoDoc* pDoc = DEMO_APP->CreateDropDoc(pData, title.DropObject); pData += title.HeaderSize;
   if (!pDoc) return FALSE;
   if (!pDoc->ValidDropTitle(&title)) {
      CString appName = (CString)title.AppName;
      DEMO_APP->MessageBox(      
         _T("Cannot drop document from ") + appName + 
         _T("\nYou should replace ")  + appName + _T(" by new version"), 
         MB_OK | MB_ICONEXCLAMATION);
      pDoc->OnCloseDocument();
      return FALSE;
   }
   pDoc->GetVector()->ReadDropInfo(pData); pData += title.InfoSize;
   pDoc->GetVector()->ReadDropData(pData); pData += title.DataSize;
   pDoc->SetDropTitle(&title);
   pDoc->Activate();
   return TRUE;
}

