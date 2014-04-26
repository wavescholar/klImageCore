/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// LUT.h: interface for the CLUT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LUT_H__91495C01_354F_453B_91CB_71F49EEBE5CC__INCLUDED_)
#define AFX_LUT_H__91495C01_354F_453B_91CB_71F49EEBE5CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

class CLUT
{
public:
   int m_num;
   int m_nLevels[4];
   void* m_pLevels[4];
   void* m_pHist[4];
   CVector m_Levels[4];
   CVector m_Hist[4];
   CVector m_Lower;
   CVector m_Upper;

   CLUT();
   CLUT(const CLUT& lut);
   virtual ~CLUT();
   CLUT& operator =(const CLUT& lut);

   BOOL Create();
   BOOL Create(int i);
   BOOL CreateEven();
   BOOL CreateEven(int i);
   BOOL CreateTone(int F[], double A[], int bInter = TRUE);
   BOOL CreateTone(int idx, int F, double A, int bInter = TRUE);
   void Delete();
   void Delete(int i);
   void Init32s(int num, int nLevels,
                Ipp32s lower, Ipp32s upper);
   void Init32f(int num, int nLevels,
                Ipp32f lower, Ipp32f upper);
   void Init32f32s(int num, int nLevels,
                Ipp32f lower, Ipp32f upper);
   BOOL SetDiagram(CImage* pImage);
   ImgHeader GetImageHeader();

   void GetHist  (CMyString* str);
   void SetHist  (CMyString* str);
   void GetLevels(CMyString* str);
   void SetLevels(CMyString* str);
   void GetLevelsHist(CMyString* str, bool bLinear = false);
   void SetLevelsHist(CMyString* str, bool bLinear = false);

   void GetHist  (CMyString& str, int idx);
   void SetHist  (CMyString  str, int idx);
   void GetLevels(CMyString& str, int idx);
   void SetLevels(CMyString  str, int idx);
   void GetLevelsHist(CMyString& str, int idx, bool bLinear = false);
   void SetLevelsHist(CMyString  str, int idx, bool bLinear = false);

   void GetNumLevels(CMyString* str);
   void SetNumLevels(CMyString* str);

   void SetEvenLevels();
   void SetEvenLevels(double lower, double upper);
   void SetEvenLevels(int idx);
   void SetToneValues(int F[], double A[], int bInter = TRUE);
   void SetToneValues(int idx, int F, double A, int bInter = TRUE);

   CMyString LevelString();
   CMyString LevelNumString();
   CMyString UpperString();
   CMyString LowerString();
protected:

   void Init(ppType tLevels, ppType tHist, int num, int nLevels);
   void Reset();
   CMyString BraceOpen();
   CMyString BraceClose();
};

#endif // !defined(AFX_LUT_H__91495C01_354F_453B_91CB_71F49EEBE5CC__INCLUDED_)
