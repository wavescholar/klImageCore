/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Histogram.cpp: implementation of the CHistogram class.
// class CHistogram sets histogram to the image
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Histogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static Ipp32f getHistoWidth() { return 256;}
static Ipp32f getHistoHeight() { return 64;}
static Ipp32f getChannelHeight() { return 70;}

static int colorBlue [3] = {192,   0,   0};
static int colorGreen[3] = {  0, 192,   0};
static int colorRed  [3] = {  0,   0, 192};
static int colorAlpha[3] = {192, 192, 192};
static int colorGray [3] = { 64,  64,  64};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistogram::CHistogram()
{
   m_numChannels = 0;
   for (int i=0; i<4; i++) {
      m_nLevels[i] = 0;
      m_pLevels[i] = NULL;
      m_pHist  [i] = NULL;
   }
   m_bInit = FALSE;

   m_minLevel = 0;
   m_maxLevel = 0;
   m_pImage = NULL;
}

CHistogram::~CHistogram()
{
   FreeVectors();
}

static void createTmpVectors(Ipp32f** dst, Ipp32s** src, int* len, int num)
{
   for (int i=0; i<num; i++) {
      dst[i] = (Ipp32f*)ippMalloc(len[i]*4);
      ippsConvert_32s32f(src[i], dst[i], len[i]);
   }
}

static void deleteTmpVectors(Ipp32f** src, int num)
{
   for (int i=0; i<num; i++) {
      ippFree(src[i]);
   }
}

BOOL CHistogram::Init32s(int numChannels, Ipp32s* ppLevels[4], Ipp32s* ppHist[4], int pnLevels[4])
{
   Ipp32f* tmpLevels[4];
   Ipp32f* tmpHist[4];
   createTmpVectors(tmpLevels, ppLevels, pnLevels, numChannels);
   createTmpVectors(tmpHist, ppHist, pnLevels, numChannels);

   BOOL result = Init32f(numChannels, tmpLevels, tmpHist, pnLevels);

   deleteTmpVectors(tmpLevels, numChannels);
   deleteTmpVectors(tmpHist, numChannels);
   return result;
}

BOOL CHistogram::Init32f32s(int numChannels, Ipp32f* ppLevels[4], Ipp32s* ppHist[4], int pnLevels[4])
{
   Ipp32f* tmpHist[4];
   createTmpVectors(tmpHist, ppHist, pnLevels, numChannels);

   BOOL result = Init32f(numChannels, ppLevels, tmpHist, pnLevels);

   deleteTmpVectors(tmpHist, numChannels);
   return result;
}

BOOL CHistogram::Init32f(int numChannels, Ipp32f* ppLevels[4], Ipp32f* ppHist[4], int pnLevels[4])
{
   m_numChannels = numChannels;
   if (!SetMinMaxLevel(ppLevels, pnLevels)) return FALSE;
   AllocateVectors(ppLevels, pnLevels);
   ExpandVectors(ppLevels, ppHist, pnLevels);
   UpdateHist();
   ScaleLevels();
   ScaleHist();
   m_bInit = TRUE;
   return TRUE;
}

BOOL CHistogram::SetMinMaxLevel(Ipp32f* ppLevels[4], int pnLevels[4])
{
   m_minLevel = ppLevels[0][0];
   m_maxLevel = ppLevels[0][pnLevels[0] - 1];
   for (int i=1; i < m_numChannels; i++) {
      if (m_minLevel > ppLevels[i][0])
         m_minLevel = ppLevels[i][0];
      if (m_maxLevel < ppLevels[i][pnLevels[i] - 1])
         m_maxLevel = ppLevels[i][pnLevels[i] - 1];
   }
   return m_maxLevel > m_minLevel;
}

static Ipp32f getMax_32f(Ipp32f* pSrc, int num)
{
   Ipp32f val = pSrc[0];
   for (int i=1; i < num; i++) {
      if (val < pSrc[i]) val = pSrc[i];
   }
   return val;
}

void CHistogram::SetMaxHist()
{
   m_maxHist = 0;
   for (int i=0; i < m_numChannels; i++) {
      Ipp32f maxHist = getMax_32f(m_pHist[i], m_nLevels[i] - 1);
      if (m_maxHist < maxHist) m_maxHist = maxHist;
   }
}

void CHistogram::AllocateVectors(Ipp32f* ppLevels[4], int pnLevels[4])
{
   SetNumLevels(ppLevels, pnLevels);
   AllocateVectors();
}

void CHistogram::SetNumLevels(Ipp32f* ppLevels[4], int pnLevels[4])
{
   for (int i=0; i < m_numChannels; i++) {
      m_nLevels[i] = pnLevels[i];
      if (ppLevels[i][0] > m_minLevel) m_nLevels[i]++;
      if (ppLevels[i][pnLevels[i] - 1] < m_maxLevel) m_nLevels[i]++;
   }
}

void CHistogram::AllocateVectors()
{
   FreeVectors();
   for (int i=0; i < m_numChannels; i++) {
      m_pLevels[i] = (Ipp32f*)ippMalloc(m_nLevels[i]*4);
      m_pHist[i]   = (Ipp32f*)ippMalloc(m_nLevels[i]*4);
   }
}

void CHistogram::FreeVectors()
{
   for (int i=0; i < 4; i++) {
      if (m_pLevels[i]) ippFree(m_pLevels[i]); m_pLevels[i] = NULL;
      if (m_pHist[i]) ippFree(m_pHist[i]); m_pHist[i] = NULL;
   }
}

void CHistogram::ExpandVectors(Ipp32f* ppLevels[4], Ipp32f* ppHist[4], int pnLevels[4])
{
   for (int i=0; i < m_numChannels; i++)
      ExpandVectors(ppLevels[i], ppHist[i], pnLevels[i], m_pLevels[i], m_pHist[i]);
}

void CHistogram::ExpandVectors(Ipp32f* srcLevels, Ipp32f* srcHist, int nSrcLevels,
                               Ipp32f* dstLevels, Ipp32f* dstHist)
{
   int iDst = 0;
   int iSrc = 0;
   if (m_minLevel < srcLevels[0]) {
      dstLevels[iDst] = m_minLevel;
      dstHist[iDst] = 0;
      iDst++;
   }
   for (iSrc = 0; iSrc < nSrcLevels - 1; iSrc++) {
      dstLevels[iDst] = srcLevels[iSrc];
      dstHist[iDst] = srcHist[iSrc];
      iDst++;
   }
   dstLevels[iDst] = srcLevels[iSrc];

   if (m_maxLevel > srcLevels[iSrc]) {
      dstLevels[iDst + 1] = m_maxLevel;
      dstHist[iDst] = 0;
   }
}

void CHistogram::UpdateHist()
{
   for (int i=0; i<m_numChannels; i++)
      UpdateHist(m_pLevels[i], m_pHist[i], m_nLevels[i] - 1);
}

void CHistogram::UpdateHist(Ipp32f* pLevels, Ipp32f* pHist, int nHist)
{
   for (int i=0; i<nHist; i++) {
      Ipp32f w = pLevels[i+1] - pLevels[i];
      if (w > 0)
         pHist[i] = pHist[i]/w;
      else
         pHist[i] = 0;
   }
}

void CHistogram::ScaleLevels()
{
   for (int i=0; i < m_numChannels; i++)
      ScaleLevels(m_pLevels[i], m_nLevels[i]);
}

void CHistogram::ScaleLevels(Ipp32f* pLevels, int num)
{
   Ipp32f scale = getHistoWidth() / (m_maxLevel - m_minLevel);
   int i;
   for (i=0; i<num - 1; i++)
      pLevels[i] = (pLevels[i] - m_minLevel)*scale;
   pLevels[i] = getHistoWidth();
}

void CHistogram::ScaleHist()
{
   SetMaxHist();
   for (int i=0; i < m_numChannels; i++)
      ScaleHist(m_pHist[i], m_nLevels[i] - 1);
}

void CHistogram::ScaleHist(Ipp32f* pHist, int num)
{
   if (m_maxHist == 0) return;
   Ipp32f scale = getHistoHeight() / m_maxHist;
   for (int i=0; i<num; i++)
      pHist[i] = pHist[i]*scale;
}

ImgHeader CHistogram::GetImageHeader(int numChannels)
{
   ImgHeader header;
   header.type = pp8u;
   header.channels = 3;
   header.plane = FALSE;
   header.SetSize((int)getHistoWidth(), numChannels*(int)getChannelHeight());
   return header;
}

BOOL CHistogram::ValidHeader(ImgHeader h1)
{
   ImgHeader h2 = GetImageHeader(m_numChannels);
   if (h1.type     != h2.type     ) return FALSE;
   if (h1.channels != h2.channels ) return FALSE;
   if (h1.width    != h2.width    ) return FALSE;
   if (h1.height   != h2.height   ) return FALSE;
   if (h1.plane    != h2.plane    ) return FALSE;
   return TRUE;
}

BOOL CHistogram::SetImage(CImage* pImage)
{
   if (!pImage) return FALSE;
   if (!m_bInit) return FALSE;
   if (!ValidHeader(pImage->GetHeader())) return FALSE;
   m_pImage = pImage;

   int* color[4] = {colorBlue, colorGreen, colorRed, colorAlpha};
   if (m_numChannels == 1) color[0] = colorGray;

   m_pImage->SetData((char)0xFF);
   for (int i=0; i < m_numChannels; i++)
      DrawHist(m_pLevels[i], m_pHist[i], m_nLevels[i],
         (m_numChannels - i - 1)*(int)getChannelHeight(), color[i]);
   return TRUE;
}

void CHistogram::DrawHist(Ipp32f* pLevels, Ipp32f* pHist, int nLevels,
                           int y0, int* color)
{
   DrawRect(0, y0, (int)getHistoWidth(), 1, color);
   int x0 = 0;
   for (int i=0; i < nLevels - 1; i++) {
      int x1 = (int)(pLevels[i + 1] + .5);
      if (x1 - x0 > 0)
         DrawRect(x0, y0 + 1, x1 - x0, (int)(pHist[i] + .5), color);
      x0 = x1;
   }
}

void CHistogram::DrawRect(int x0, int y0, int xN, int yN, int* color)
{
   for (int y=y0; y < y0 + yN; y++)
      for (int x=x0; x < x0 + xN; x++)
         m_pImage->Set(x,y,color);
}
