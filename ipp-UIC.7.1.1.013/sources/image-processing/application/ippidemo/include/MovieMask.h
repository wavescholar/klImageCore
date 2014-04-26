/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieMask.h: interface for the CMovieMask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVIEMASK_H__F2F7014C_A2B0_410D_9F0E_FC722894DAD4__INCLUDED_)
#define AFX_MOVIEMASK_H__F2F7014C_A2B0_410D_9F0E_FC722894DAD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

class CMovieMask : public CImage
{
public:
// constructor / destructor
   CMovieMask();
   CMovieMask(int nID, CString name);
   CMovieMask(CString filePath);
   CMovieMask(const CMovieMask& mask) { *this = mask;}
   CMovieMask& operator =(const CMovieMask& mask);
   virtual ~CMovieMask();

// attributes
   BOOL IsPattern() { return m_ID ? TRUE : FALSE;}
   CString GetName() { return m_Name;}
   CString GetFilePath() { return m_FilePath;}
   int GetThreshold() { return m_Threshold;}
   int GetMin() { return m_Min;}
   int GetMax() { return m_Max;}

// operations
   void Init(int nID, CString name);
   void Init(CString fileName);
   BOOL Load(BOOL bMessage = FALSE);
   BOOL SetThreshold(Ipp8u threshold);

protected:
   CImage m_Img;
   CString m_Name;
   int m_ID;
   CMyString m_FilePath;
   Ipp8u m_Threshold;
   Ipp8u m_Min;
   Ipp8u m_Max;

   void Clear();
   void LoadPattern();
   BOOL LoadFile(BOOL bMessage);
   void InitThreshold();
   void ImgConvertToGray();
};

class CMovieMaskArray : public CArray<CMovieMask, CMovieMask&> {
public:
   CMovieMaskArray() : m_CurIndex(-1) {}
   virtual ~CMovieMaskArray() {}
   CMovieMaskArray& operator =(CMovieMaskArray& arr) {
      RemoveAll();
      Append(arr);
      m_CurIndex = arr.m_CurIndex;
      m_CurPath = arr.m_CurPath;
      return *this;
   }

   void SetCurIndex(int idx) {
      if (idx >= GetSize()) idx = 0;
      m_CurIndex = idx;
      GetCurMask()->Load();
   }
   int GetCurIndex() { return m_CurIndex;}
   void SetCurPath(CString path) { m_CurPath = path;}
   CString GetCurPath() { return m_CurPath;}
   CMovieMask* GetCurMask() { return &(ElementAt(m_CurIndex));}

   int AddMask(int nID, CString name) { CMovieMask mask(nID, name); return AddMask(mask);}
   int AddMask(CString filePath) { CMovieMask mask(filePath); return AddMask(mask);}
   int AddMask(CMovieMask& mask)
   { int idx = (int)Add(mask); if (m_CurIndex < 0) m_CurIndex = idx; return idx;}
   void RemoveMask(int idx) {
      RemoveAt(idx); if (m_CurIndex  >= GetSize()) m_CurIndex--;}
protected:
   int m_CurIndex;
   CString m_CurPath;
};

#endif // !defined(AFX_MOVIEMASK_H__F2F7014C_A2B0_410D_9F0E_FC722894DAD4__INCLUDED_)
