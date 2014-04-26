/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DemoDib.h: interface for the CDemoDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODIB_H__BD48EEE1_D2A7_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_DEMODIB_H__BD48EEE1_D2A7_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CImage;

class CDemoDib
{
public:
   CDemoDib();
   virtual ~CDemoDib();
   void CreateDibSection(CDC* pDC, CImage* image);
   void DeleteDibSection();
   void SetBmpInfo(CImage* image, BOOL bOne = FALSE);
   void SetDibSection(CImage* image);
   BOOL IsNewImageSize(const CImage* image);

   static int GetDibWidth(const CImage* image);
   static int GetDibHeight(const CImage* image);
   static void ConvertToRGB(CImage* image, Ipp8u* dst, int dstStep,
                            BOOL bOne = FALSE);

   HBITMAP GetBitmapHandle() { return m_hBmp;}
   BITMAPINFO* GetBmpInfo()
   {   return (BITMAPINFO*)m_BmpInfoPalette;}
   BITMAPINFOHEADER* GetBmpInfoHeader()
   {   return (BITMAPINFOHEADER*)m_BmpInfoPalette;}
   RGBQUAD* GetBmpPalette(int* pPaletteNum = NULL) {
       if (pPaletteNum) *pPaletteNum = PaletteNum();
       return GetBmpInfo()->bmiColors;
   }
   int GetBmpSize() {
       BITMAPINFOHEADER* bi = GetBmpInfoHeader();
       return ((bi->biBitCount*bi->biWidth+31)/32)*4*bi->biHeight;
   }
   int GetBmpStep() {
       BITMAPINFOHEADER* bi = GetBmpInfoHeader();
       return (bi->biBitCount*bi->biWidth+31)/32*4;
   }
   int PaletteNum();
   void* GetBits() { return m_pBits;}
protected:
   BYTE   m_BmpInfoPalette[sizeof(BITMAPINFO)+255*sizeof(RGBQUAD)];
   char* m_pBits;
   HBITMAP m_hBmp;

   void   SetGrayPalette();

   static void SetBits_Pixel_C1(CImage* image, Ipp8u* dst, int dstStep, BOOL bOne);
   static void SetBits_Pixel_C2(CImage* image, Ipp8u* dst, int dstStep);
   static void SetBits_Pixel_C3(CImage* image, Ipp8u* dst, int dstStep);
   static void SetBits_Pixel_C4(CImage* image, Ipp8u* dst, int dstStep);
   static void SetBits_Plane(CImage* image, Ipp8u* dst, int dstStep);
   static void SetBits_Compressed(CImage* image, Ipp8u* dst, int dstStep);
};

#endif // !defined(AFX_DEMODIB_H__BD48EEE1_D2A7_11D1_AE6B_444553540000__INCLUDED_)
