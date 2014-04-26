/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Face Detection Sample for Windows*
//
//   By downloading and installing this sample, you hereby agree that the
//   accompanying Materials are being provided to you under the terms and
//   conditions of the End User License Agreement for the Intel(R) Integrated
//   Performance Primitives product previously accepted by you. Please refer
//   to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//   product installation for more information.
//
//
*/

// ippiImageStore.h: interface for the CIppiImageStore class.
//               It provides Open and Save File commands
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGESTORE_H__84B80914_1314_4D48_933F_A141F6FA5A64__INCLUDED_)
#define AFX_IMAGESTORE_H__84B80914_1314_4D48_933F_A141F6FA5A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiImage.h"

class CIppiImageStore  
{
public:
   // Constructor attaches image
   CIppiImageStore(CIppiImage* pImage) : m_pImage(pImage), m_pFile(NULL), m_bSave(FALSE) {}
   virtual ~CIppiImageStore() {}

   // Save attached image to bmp file
   BOOL Save(CFile* pFile);
   // Load attached image from bmp file
   BOOL Load(CFile* pFile);
protected:
   CIppiImage* m_pImage;
   BOOL    m_bSave;
   CFile*  m_pFile;
   
   BITMAPFILEHEADER m_FileHeader;
   BITMAPINFOHEADER m_InfoHeader;
   RGBQUAD m_Palette[256];

   BOOL Error(CString message);
   BOOL Error(CFileException& exc);

   BOOL ValidImage(CIppiImage* pImage);
   DWORD FileHeaderSize() { return sizeof(BITMAPFILEHEADER);}
   DWORD InfoHeaderSize() { return sizeof(BITMAPINFOHEADER);}
   BITMAPFILEHEADER* GetFileHeader(CIppiImage* pImage);
   BITMAPINFOHEADER* GetInfoHeader(CIppiImage* pImage);
   int GetPaletteNum(CIppiImage* pImage);
   int GetPaletteSize(CIppiImage* pImage) 
   { return GetPaletteNum(pImage)*sizeof(RGBQUAD);}
   RGBQUAD* GetGrayPalette(CIppiImage* pImage);
   int GetPaletteNum();
   int GetPaletteSize() 
   { return GetPaletteNum()*sizeof(RGBQUAD);}
   int GetDataStep(); 
   int GetDataSize(); 
   BOOL IsGrayPalette();
   BOOL IsCompressed();

   BOOL ValidFileHeader();
   BOOL CreateImage();
   BOOL ValidCompression();
   BOOL SetImageWidth(int& width);
   BOOL SetImageHeight(int& height);
   BOOL SetImageType(ppType& type);
   BOOL SetImagePlane(BOOL& plane);
   BOOL SetImageChannels(int& channels);
   BOOL SetImageData(Ipp8u* buffer);
   BOOL SetImageData_1(Ipp8u* buffer);
   BOOL SetImageData_4(Ipp8u* buffer);
   BOOL SetImageData_8(Ipp8u* buffer);
   void SetPixel(Ipp8u src, Ipp8u* pDst, int x);
};

#endif // !defined(AFX_IMAGESTORE_H__84B80914_1314_4D48_933F_A141F6FA5A64__INCLUDED_)
