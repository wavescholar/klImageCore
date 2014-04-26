/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DragDrop.h: interface for the CDropTitle, CDropSource, CDropTarget 
// classes.
//
// They Provide Drag & Drop Operations on Demo applications
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGDROP_H__31D81724_3E31_45A6_9909_C0E78A4C7420__INCLUDED_)
#define AFX_DRAGDROP_H__31D81724_3E31_45A6_9909_C0E78A4C7420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>

class CDemoDoc;

class CDropTitle {
public:
   int TitleSize;
   int HeaderSize;
   int InfoSize;
   int DataSize;
   int DropObject;
   char AppName[64];
   char DocName[256];
};

enum {DROP_VECTOR, DROP_IMAGE};

class CDropSource : public COleDropSource  
{
public:
   CDropSource();
   virtual ~CDropSource();
   BOOL DoDragDrop(CDemoDoc* pDoc, CPoint point);

   virtual SCODE QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState)
   {
      return COleDropSource::QueryContinueDrag(bEscapePressed, dwKeyState);
   }
   virtual SCODE GiveFeedback(DROPEFFECT dropEffect)
   {
      return COleDropSource::GiveFeedback(dropEffect);
   }
   virtual BOOL OnBeginDrag(CWnd* pWnd)
   {
      BOOL result = COleDropSource::OnBeginDrag(pWnd);
      return result;
   }

protected:
   BOOL CreateDataSource(int size);
   void DeleteDataSource();
   int  GetDataSize(CDemoDoc* pDoc) const;
   void WriteData(CDemoDoc* pDoc, char* pData) const;

   char* GetGlobalData(COleDataObject* pDataObject);
   void ReleaseGlobalData();

   COleDataSource* m_pDataSource;
   HGLOBAL         m_hGlobalData;
};

class CDropTarget : public COleDropTarget  
{
public:
   CDropTarget() : COleDropTarget() {}
   virtual ~CDropTarget(){}

   virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
            DROPEFFECT dropEffect, CPoint point);
   virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
            DWORD grfKeyState, CPoint point);
   virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject*,
            DWORD grfKeyState, CPoint point);
   virtual void OnDragLeave(CWnd* pWnd);
protected:
   BOOL ValidData(const char* pData) const;
   BOOL ReadData(const char* pData) const;
};


#endif // !defined(AFX_DRAGDROP_H__31D81724_3E31_45A6_9909_C0E78A4C7420__INCLUDED_)
