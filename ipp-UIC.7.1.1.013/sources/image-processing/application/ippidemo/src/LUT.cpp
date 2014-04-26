/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// LUT.cpp: implementation of the CLUT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "LUT.h"
#include "Histogram.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLUT::CLUT()
{
   Reset();
}

void CLUT::Reset()
{
   m_num = 0;
   for (int i=0; i < 4; i++) {
      m_nLevels[i] = 0;
      m_pLevels[i] = NULL;
      m_pHist[i]   = NULL;
   }
}

CLUT::CLUT(const CLUT& lut)
{
   Reset();
   *this = lut;
}

CLUT& CLUT::operator =(const CLUT& lut)
{
   m_num   = lut.m_num;
   m_Lower = lut.m_Lower;
   m_Upper = lut.m_Upper;
   for (int i=0; i < 4; i++) {
      m_nLevels[i] = lut.m_nLevels[i];
      m_Levels[i]  = lut.m_Levels[i];
      m_pLevels[i] = m_Levels[i].GetData();
      m_Hist[i]    = lut.m_Hist[i]  ;
      m_pHist[i]   = m_Hist[i].GetData();
   }
   return *this;
}

CLUT::~CLUT()
{
   Delete();
}

void CLUT::Init32s(int num, int nLevels, Ipp32s lower, Ipp32s upper)
{
   Init(pp32s, pp32s, num, nLevels);
   Create();
   SetEvenLevels(lower, upper);
}

void CLUT::Init32f(int num, int nLevels, Ipp32f lower, Ipp32f upper)
{
   Init(pp32f, pp32f, num, nLevels);
   Create();
   SetEvenLevels(lower, upper);
}

void CLUT::Init32f32s(int num, int nLevels, Ipp32f lower, Ipp32f upper)
{
   Init(pp32f, pp32s, num, nLevels);
   Create();
   SetEvenLevels(lower, upper);
}

void CLUT::Init(ppType tLevels, ppType tHist, int num, int nLevels)
{
   Reset();
   m_num = num;
   m_Lower.Init(tLevels, 4);
   m_Upper.Init(tLevels, 4);
   for (int i=0; i<num; i++) {
      m_nLevels[i] = nLevels;
      m_Levels[i].Init(tLevels);
      m_Hist[i].Init(tHist);
   }
}

BOOL CLUT::Create()
{
   for (int i=0; i<m_num; i++) {
      if (!Create(i)) return FALSE;
   }
   return TRUE;
}

BOOL CLUT::Create(int idx)
{
   Delete(idx);
   if (m_nLevels[idx] <= 1) {
      Delete(idx);
      return FALSE;
   }
   m_Levels[idx].Allocate(m_nLevels[idx]);
   m_pLevels[idx] = m_Levels[idx].GetData();
   m_Hist[idx].Allocate(m_nLevels[idx]);
   m_pHist[idx] = m_Hist[idx].GetData();
   return TRUE;
}

BOOL CLUT::CreateEven()
{
   for (int i=0; i<m_num; i++) {
      if (!CreateEven(i)) return FALSE;
   }
   return TRUE;
}

BOOL CLUT::CreateEven(int idx)
{
   if (!Create(idx)) return FALSE;
   SetEvenLevels(idx);
   return TRUE;
}

BOOL CLUT::CreateTone(int F[], double A[], int bInter)
{
   for (int i=0; i<m_num; i++) {
      if (!CreateTone(i, F[i], A[i], bInter)) return FALSE;
   }
   return TRUE;
}

BOOL CLUT::CreateTone(int idx, int F, double A, int bInter)
{
   if (A < -1 || A > 1) return FALSE;
   if (!CreateEven(idx)) return FALSE;
   SetToneValues(idx, F, A, bInter);
   return TRUE;
}

//static double getToneValue(double level, double lower, double upper, int F, double A)
//{
//   double x = (level - lower)/(upper - lower);
//   double y = A*sin(F*IPP_PI*x)/(F*IPP_PI) + x;
//   return lower + y*(upper - lower);
//}

static double arcVal(double x, int F)
{
   double y;
   if (F == 0) return x;
   int A = F > 0 ? F : -F;
   double R = 1./A;
   int N = (int)(x/R);
   if (N >= A) return 1;
   x = x - N*R;
   if (F > 0 && !(N & 1) || F < 0 && (N & 1))
      y = sqrt(R*R - (R - x)*(R - x));
   else
      y = R - sqrt(R*R - x*x);
   return y + N*R;
}

static double getToneValue(double level, double lower, double upper, int F, double A)
{
   double x = (level - lower)/(upper - lower);
   double y = arcVal(x,F);
   return lower + y*(upper - lower);
}

void CLUT::SetToneValues(int F[], double A[], int bInter)
{
   for (int i=0; i<m_num; i++) {
      SetToneValues(i, F[i], A[i], bInter);
   }
}

void CLUT::SetToneValues(int idx, int F, double A, int bInter)
{
   double lower = m_Lower.GetDouble(idx);
   double upper = m_Upper.GetDouble(idx);
   if (bInter) {
      for (int i=0; i<m_Levels[idx].Length(); i++) {
         double level = m_Levels[idx].GetDouble(i);
         m_Hist[idx].Set(i, getToneValue(level, lower, upper, F, A));
      }
   } else {
      for (int i=0; i<m_Levels[idx].Length() - 1; i++) {
         double level = .5*(m_Levels[idx].GetDouble(i) + m_Levels[idx].GetDouble(i+1));
         m_Hist[idx].Set(i, getToneValue(level, lower, upper, F, A));
      }
   }
}


void CLUT::Delete()
{
   for (int i=0; i<4; i++) Delete(i);
}

void CLUT::Delete(int idx)
{
   m_Levels[idx].Deallocate();
   m_pLevels[idx] = NULL;
   m_Hist[idx].Deallocate();
   m_pHist[idx] = NULL;
}

void CLUT::SetEvenLevels(double lower, double upper)
{
   for (int i=0; i<4; i++) {
      m_Lower.Set(i, lower);
      m_Upper.Set(i, upper);
   }
   SetEvenLevels();
}

void CLUT::SetEvenLevels()
{
   for (int i=0; i<m_num; i++)
      SetEvenLevels(i);
}

static void setEven32s(Ipp32s* pVal, int n, Ipp32s low, Ipp32s up)
{
   if (n < 2) return;
   float step = (float)(up - low) / (float)(n - 1);
   if (step < 1) step = 1;
   int i;
   for (i=0; i < n; i++) {
      Ipp32s val = (Ipp32s)(low + step*i + .5);
      if (val > up) break;
      pVal[i] = val;
   }
   for ( ; i < n; i++) {
      pVal[i] = up;
   }
}

static void setEven32f(Ipp32f* pVal, int n, Ipp32f low, Ipp32f up)
{
   if (n < 2) return;
   float step = (float)(up - low) / (float)(n - 1);
   int i;
   for (i=0; i < n-1; i++)
      pVal[i] = (Ipp32f)(low + step*i);
   pVal[i] = up;
}

void CLUT::SetEvenLevels(int i)
{
   switch (m_Levels[i].Type()) {
   case pp32s: setEven32s(((Ipp32s**)m_pLevels)[i], m_nLevels[i],
                          ((Ipp32s*)m_Lower)[i],
                          ((Ipp32s*)m_Upper)[i]);
      break;
   case pp32f: setEven32f(((Ipp32f**)m_pLevels)[i], m_nLevels[i],
                          ((Ipp32f*)m_Lower)[i],
                          ((Ipp32f*)m_Upper)[i]);
      break;
   }
}

ImgHeader CLUT::GetImageHeader()
{
   return CHistogram::GetImageHeader(m_num);
}

BOOL CLUT::SetDiagram(CImage* pImage)
{
   CHistogram histogram;
   ppType lType = m_Levels[0].Type();
   ppType hType = m_Hist[0].Type();

   if ((lType == pp32s) && (hType == pp32s)) {
      if (!histogram.Init32s(m_num, (Ipp32s**)m_pLevels, (Ipp32s**)m_pHist, m_nLevels))
         return FALSE;
   }
   else if ((lType == pp32f) && (hType == pp32f)) {
      if (!histogram.Init32f(m_num, (Ipp32f**)m_pLevels, (Ipp32f**)m_pHist, m_nLevels))
         return FALSE;
   }
   else if ((lType == pp32f) && (hType == pp32s)) {
      if (!histogram.Init32f32s(m_num, (Ipp32f**)m_pLevels, (Ipp32s**)m_pHist, m_nLevels))
         return FALSE;
   }
   return histogram.SetImage(pImage);
}

void CLUT::GetNumLevels(CMyString* str)
{
   for (int i=0; i<m_num; i++)
      str[i].Format("%d", m_nLevels[i]);
}

void CLUT::SetNumLevels(CMyString* str)
{
   for (int i=0; i<m_num; i++) {
      m_nLevels[i] = atoi(str[i]);
      Create(i);
   }
}

void CLUT::GetHist  (CMyString* str) { for (int i=0; i<m_num; i++) GetHist  (str[i], i);}
void CLUT::SetHist  (CMyString* str) { for (int i=0; i<m_num; i++) SetHist  (str[i], i);}
void CLUT::GetLevels(CMyString* str) { for (int i=0; i<m_num; i++) GetLevels(str[i], i);}
void CLUT::SetLevels(CMyString* str) { for (int i=0; i<m_num; i++) SetLevels(str[i], i);}
void CLUT::GetLevelsHist(CMyString* str, bool bLinear)
{ for (int i=0; i<m_num; i++) GetLevelsHist(str[i], i, bLinear);}
void CLUT::SetLevelsHist(CMyString* str, bool bLinear)
{ for (int i=0; i<m_num; i++) SetLevelsHist(str[i], i, bLinear);}

void CLUT::GetHist  (CMyString& str, int idx)
{
   m_Hist[idx].Get(str, 3);
}

void CLUT::SetHist  (CMyString  str, int idx)
{
   m_Hist[idx].Set(str);
   m_pHist[idx] = m_Hist[idx].GetData();
}

void CLUT::GetLevels(CMyString& str, int idx)
{
   m_Levels[idx].Get(str, 3);
}

void CLUT::SetLevels(CMyString  str, int idx)
{
   m_Levels[idx].Set(str);
   m_pLevels[idx] = m_Levels[idx].GetData();
   m_nLevels[idx] = m_Levels[idx].Length();
}

void CLUT::GetLevelsHist(CMyString& str, int idx, bool bLinear)
{
   str = "";
   int nDH = bLinear ? 0 : -1;
   for (int i=0; i<m_nLevels[idx]; i++) {
      CMyString lStr, hStr;
      m_Levels[idx].Get(i, lStr, 3);
      if (i < m_Hist[idx].Length() &&
          i < m_nLevels[idx] + nDH)
         m_Hist[idx].Get(i, hStr, 3);
      str += lStr + "\t" + hStr + "\r\n";
   }
}

void CLUT::SetLevelsHist(CMyString  str, int idx, bool bLinear)
{
   ASSERT(m_Levels[idx].Type() == m_Hist[idx].Type());

   ppType cplxType = (ppType)(m_Levels[idx].Type() | PP_CPLX);
   CVector cplxVec(cplxType);
   cplxVec.Set(str);
   m_nLevels[idx] = cplxVec.Length();
   Create(idx);
   ippsCplxToReal_32fc(
      (Ipp32fc*)cplxVec.GetData(),
      (Ipp32f*)(m_Levels[idx].GetData()),
      (Ipp32f*)(m_Hist[idx].GetData()),
      m_nLevels[idx]);
}

CMyString CLUT::LevelString()
{
   CMyString str;
   str << BraceOpen();
   for (int i=0; i<m_num; i++) {
      CString str1, str2;
      m_Levels[i].Get(0,str1,3);
      m_Levels[i].Get(m_Levels[i].Length() - 1,str2,3);
      if (i > 0) str << ", ";
      str << str1 << ".." << str2;
   }
   str << BraceClose();
   return str;
}

CMyString CLUT::LevelNumString()
{
   CMyString str;
   str << BraceOpen();
   for (int i=0; i<m_num; i++) {
      if (i > 0) str << ", ";
      str << m_Levels[i].Length();
   }
   str << BraceClose();
   return str;
}

CMyString CLUT::UpperString()
{
   CMyString str;
   str << BraceOpen();
   for (int i=0; i<m_num; i++) {
      CString str1;
      m_Upper.Get(i,str1,3);
      if (i > 0) str << ", ";
      str << str1;
   }
   str << BraceClose();
   return str;
}

CMyString CLUT::LowerString()
{
   CMyString str;
   str << BraceOpen();
   for (int i=0; i<m_num; i++) {
      CString str1;
      m_Lower.Get(i,str1,3);
      if (i > 0) str << ", ";
      str << str1;
   }
   str << BraceClose();
   return str;
}

CMyString CLUT::BraceOpen() {
   return m_num > 1 ? "{" : "";
}

CMyString CLUT::BraceClose() {
   return m_num > 1 ? "}" : "";
}


