/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
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

// ippiImage.h: interface for the CIppiImage class.
//          It contains image data and provides easy calls of ippIP 
//          functions
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ipps.h>
#include <ippi.h>

// IPP Data Types definition

#define PP_MASK  0x0FF
#define PP_SIGN  0x100
#define PP_FLOAT 0x200
#define PP_CPLX  0x400

typedef enum {
   ppNONE = 0 , 
   pp8u   = 8 , 
   pp8s   = 8  | PP_SIGN,
   pp8sc  = 8  | PP_SIGN | PP_CPLX,
   pp16u  = 16,
   pp16s  = 16 | PP_SIGN,
   pp16sc = 16 | PP_SIGN | PP_CPLX,
   pp32u  = 32,
   pp32s  = 32 | PP_SIGN,
   pp32sc = 32 | PP_SIGN | PP_CPLX,
   pp32f  = 32 | PP_FLOAT ,
   pp32fc = 32 | PP_FLOAT  | PP_CPLX,
   pp64s  = 64 | PP_SIGN,
   pp64sc = 64 | PP_SIGN | PP_CPLX,
   pp64f  = 64 | PP_FLOAT ,
   pp64fc = 64 | PP_FLOAT  | PP_CPLX,
} ppType;

// Image header structure
class CImgHeader {
public:
   int       m_width;
   int       m_height;
   ppType    m_type;
   int       m_channels;
   BOOL     m_plane;
};

// Image class
class CIppiImage : public CImgHeader
{
public:
    CIppiImage();
   CIppiImage(const CIppiImage& img);
    virtual ~CIppiImage();
    CIppiImage& operator =(const CIppiImage& image);

// General Operations
   // Set image header and allocate data
    BOOL CreateImage(int width, int height, int nChannels = 3, ppType type = pp8u, BOOL bPlane = FALSE);
    BOOL CreateImage(CImgHeader header);
   // Load image from bmp file
   BOOL LoadImage(CFile* pFile);
   // Save image to bmp file
   BOOL SaveImage(CFile* pFile);
   // Reset image size
   BOOL SetSize(int width, int height);

// Attributes
   // Get image header
   CImgHeader GetHeader() const { return *(CImgHeader*)this;}
   // Get pointer to data
   void* DataPtr();
   // Get image size
   IppiSize Size() const { IppiSize size = {Width(), Height()}; return size;}
   // Get scanline size in bytes
   int  Step() const;
   // Get data size in bytes
   int  DataSize() const;

   // Get image width
   int  Width() const { return m_width;}
   // Get image height
   int  Height() const { return m_height;}
   // Get number of image channels
   int  Channels() const { return m_channels;}
   // Get plane order flag
   BOOL Plane() const { return m_plane;}
   // Get data type
   ppType Type() const { return m_type;}
   // Get size of data element in bits
   int Depth() const { return m_type & PP_MASK;}
   // Get signed data flag
   BOOL Sign() const { return m_type & PP_SIGN ? TRUE : FALSE;}
   // Get float data flag
   BOOL Float() const { return m_type & PP_FLOAT ? TRUE : FALSE;}
   // Get complex data flag
   BOOL Complex() const { return m_type & PP_CPLX ? TRUE : FALSE;}

   // Get string with data type description
   CString TypeString() const;
   // Get string with channels and plane description
   CString ChannelString() const;

   // Set data changed / unchanged flag
   // This flag must be set to TRUE if data has been changed
   // Then CView::OnDraw function will update bitmap and 
   // set this flag to FALSE
   void IsUpdated(BOOL updated) {m_Updated = updated;}
   // Get data changed / unchanged flag
   // Used by CView::OnDraw function
   BOOL IsUpdated() const {return m_Updated;}
protected:
   void*     m_pData;
   BOOL      m_Updated;

    void Reset();
   void AllocateData();
   void FreeData();
};

#endif // !defined(AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_)
