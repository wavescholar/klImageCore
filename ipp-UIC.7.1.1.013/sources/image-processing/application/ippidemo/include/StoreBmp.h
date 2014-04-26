/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreBmp.h: interface for the CStoreBmp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOREBMP_H__09A55E83_8D04_47DE_957C_24AE2CCB80F5__INCLUDED_)
#define AFX_STOREBMP_H__09A55E83_8D04_47DE_957C_24AE2CCB80F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImage;

class CStoreBmp
{
public:
   CStoreBmp(CImage* pImage) : m_pImage(pImage), m_pFile(NULL), m_bSave(FALSE) {}
   virtual ~CStoreBmp() {}

   BOOL Save(CFile* pFile);
   BOOL Load(CFile* pFile);
   static DWORD Compression(ImgHeader header);
protected:
   enum {BI_IPP_8U = 990, BI_IPP_16U, BI_IPP_32S, BI_IPP_32F, 
         BI_IPP_422, BI_IPP_420, BI_IPP_411, BI_IPP_422F, BI_IPP_420F, BI_IPP_411F, };
   CImage* m_pImage;
   BOOL    m_bSave;
   CFile*  m_pFile;

   BITMAPFILEHEADER m_FileHeader;
   BITMAPINFOHEADER m_InfoHeader;
   RGBQUAD m_Palette[256];

   BOOL Error(CString message);
   BOOL Error(CFileException* exc);

   DWORD FileHeaderSize() { return sizeof(BITMAPFILEHEADER);}
   DWORD InfoHeaderSize() { return sizeof(BITMAPINFOHEADER);}
   BITMAPFILEHEADER* GetFileHeader(CImage* pImage);
   BITMAPINFOHEADER* GetInfoHeader(CImage* pImage);
   int GetPaletteNum(CImage* pImage);
   int GetPaletteSize(CImage* pImage)
   { return GetPaletteNum(pImage)*sizeof(RGBQUAD);}
   RGBQUAD* GetGrayPalette(CImage* pImage);
   int GetPaletteNum();
   int GetPaletteSize()
   { return GetPaletteNum()*sizeof(RGBQUAD);}
   int GetDataStep();
   int GetDataSize();
   BOOL IsGrayPalette();
   BOOL IsCompressed();

   BOOL ValidFileHeader();
   BOOL SetImageHeader();
   BOOL ValidCompression();
   BOOL SetImageWidth(int& width);
   BOOL SetImageHeight(int& height);
   BOOL SetImageType(ppType& type);
   BOOL SetImagePlane(BOOL& plane);
   BOOL SetImageChannels(int& channels);
   void SetPlaneSizes(ImgHeader& header);
   BOOL SetImageData(Ipp8u* buffer);
   BOOL SetImageData_1(Ipp8u* buffer);
   BOOL SetImageData_4(Ipp8u* buffer);
   BOOL SetImageData_8(Ipp8u* buffer);
   void SetPixel(Ipp8u src, Ipp8u* pDst, int x);
};

#endif // !defined(AFX_STOREBMP_H__09A55E83_8D04_47DE_957C_24AE2CCB80F5__INCLUDED_)
