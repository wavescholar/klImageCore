/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Histogram.h: interface for the CHistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTOGRAM_H__F1CE1D80_485F_42F9_A8FB_00881ACD40B8__INCLUDED_)
#define AFX_HISTOGRAM_H__F1CE1D80_485F_42F9_A8FB_00881ACD40B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImage;

//////////////////////////////////////////////////////////////////////
// class CHistogram sets histogram to the image
//////////////////////////////////////////////////////////////////////

class CHistogram
{
public:
   // Constractor/Destructor
   CHistogram();
   virtual ~CHistogram();

   // Init32f, Init32s, Init32f32s initialize histogram
   BOOL Init32f(int numChannels, Ipp32f* ppLevels[4], Ipp32f* ppHist[4], int pnLevels[4]);
   BOOL Init32s(int numChannels, Ipp32s* ppLevels[4], Ipp32s* ppHist[4], int pnLevels[4]);
   BOOL Init32f32s(int numChannels, Ipp32f* ppLevels[4], Ipp32s* ppHist[4], int pnLevels[4]);
   // GetImageHeader returns image header that must be used to create histogram image
   static ImgHeader GetImageHeader(int numChannels);
   // SetImage sets histogram to specified image
   BOOL SetImage(CImage* pImage);

protected:
   BOOL m_bInit;
   int m_numChannels;
   int  m_nLevels[4];
   Ipp32f* m_pLevels[4];
   Ipp32f* m_pHist[4];

   BOOL SetMinMaxLevel(Ipp32f* ppLevels[4], int pnLevels[4]);
   void SetMaxHist();
   void AllocateVectors(Ipp32f* ppLevels[4], int pnLevels[4]);
   void SetNumLevels(Ipp32f* ppLevels[4], int pnLevels[4]);
   void AllocateVectors();
   void FreeVectors();
   void ExpandVectors(Ipp32f* ppLevels[4], Ipp32f* ppHist[4], int pnLevels[4]);
   void ExpandVectors(Ipp32f* srcLevels, Ipp32f* srcHist, int nSrcLevels,
                      Ipp32f* dstLevels, Ipp32f* dstHist);
   void UpdateHist();
   void UpdateHist(Ipp32f* pLevels, Ipp32f* pHist, int nHist);
   void ScaleLevels();
   void ScaleLevels(Ipp32f* pLevels, int num);
   void ScaleHist();
   void ScaleHist(Ipp32f* pHist, int num);

   BOOL ValidHeader(ImgHeader h1);
   void DrawHist(Ipp32f* pLevels, Ipp32f* pHist, int nLevels,
                 int y0, int* color);
   void DrawRect(int x0, int y0, int xN, int yN, int* color);

private:
   CImage* m_pImage;
   Ipp32f m_minLevel;
   Ipp32f m_maxLevel;
   Ipp32f m_maxHist;
};

#endif // !defined(AFX_HISTOGRAM_H__F1CE1D80_485F_42F9_A8FB_00881ACD40B8__INCLUDED_)
