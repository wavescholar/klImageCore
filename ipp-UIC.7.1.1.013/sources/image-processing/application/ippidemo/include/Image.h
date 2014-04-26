/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Vector.h"

struct ImgHeader {
   ppType type;
   int channels;
   int width;
   int height;
   int widthP[4];
   int heightP[4];
   BOOL plane;

   void SetSize(int w, int h);
   void SetSize(int* wP, int* hP);
   void SetSize444(int w, int h);
   void SetSize422(int w, int h);
   void SetSize420(int w, int h);
   void SetSize411(int w, int h);
   BOOL IsSize444() const;
   BOOL IsSize422() const;
   BOOL IsSize420() const;
   BOOL IsSize411() const;
   BOOL EqualPlaneSize() const;
};

enum {RGB444 = 1, RGB555, RGB565};
enum {LEFT, RIGHT, TOP, BOTTOM};

class CImage : public CVector
{
public:
   CImage();
   CImage(const CImage& img) {*this = img;}
   virtual ~CImage();
   CImage& operator =(const CImage& image);

   enum {
       initHeader = 0x01,
       initData   = 0x02,
       initAlloc  = 0x04,
       initRoi    = 0x08,
       initQuad   = 0x10,
       initParl   = 0x20,
       initCenter = 0x40,
   };

// General Operations
   void Init();
   void Reset();
   void SetHeader(const CImage* pImg);
   void SetHeader(ImgHeader header);
   void SetHeader(ppType type, int channels,
                  int width, int height,
                  BOOL isPlane = FALSE);
   void SetHeader(ppType type, int channels,
                  int width[4], int height[4]);
   ImgHeader GetHeader() const;
   void ResetHeader();
   void AllocateData(char value = (char)0xFF);
   void FreeData();
   void SetData(const void* data, int size);
   void SetDataP(int c, const void* pData, int size);
   void SetData(char value);
   BOOL CopyData(CImage* srcImage);
   BOOL CopyData_ROI(CImage* srcImage);
   BOOL CopyData(void* data);
   BOOL CopyDataP(void** pData);
   BOOL CopyData_ROI(void* data, int step, IppiSize roiSize);
   BOOL CopyDataP_ROI(void** pData, int* pStep, IppiSize roiSize);
   void ConvertToRGB();
   void ConvertPlane(BOOL plane);
   BOOL EqualPlaneSize() const;
   BOOL Is422() const;
   BOOL Is420() const;
   BOOL Is411() const;
   void CorrectPlaneSize(int& width, int& height, int coi) const;

   void Set(int x, int y, int c, double value, double imval = 0);
   void Set(int x, int y, int c, Ipp64fc value);
   Ipp64fc Get(int x, int y, int c) const;
   int     GetInt(int x, int y, int c) const;
   double  GetDouble(int x, int y, int c) const;
   void Set(int x, int y, const int pixel[4]);
   void Get(int x, int y, int pixel[4]) const;
   void Set(int x, int y, int c, CValue val);
   void Get(int x, int y, int c, CValue& val) const;
   void Set(int x, int y, int c, CString str);
   void Get(int x, int y, int c, CString& str, LPCSTR cpxFormat = NULL) const;

   BOOL SetReplicateBorder();
   BOOL SetZeroBorder();
   BOOL SetConstBorder(const CVectorUnit& value);
   BOOL SetConstBorder(const double value[4]);

// Attributes
   BOOL Plane() const { return m_plane;}
   int  Channels() const { return m_channels;}
   BOOL Alpha() const { return m_channels == 4;}
   int  Width() const { return m_width;}
   int  Height() const { return m_height;}
   int  WidthP(int c) const { return m_widthP[c];}
   int  HeightP(int c) const { return m_heightP[c];}
   int  Step() const;
   int  StepP(int c) const;
   int* StepP();
   int  Size() const;
   int  SizeP(int c) const;
   int  Compressed() const { return m_Compressed;}
   void Compressed(int mode) { m_Compressed = mode;}
   virtual CString TypeString() const;
   CString ChannelString() const;
   CString CoiString() const;

   void IsUpdated(BOOL updated) {m_Updated = updated;}
   BOOL IsUpdated() const {return m_Updated;}
   BOOL IsHeader() const;
   void SetCoi(int coi) { m_Coi = coi;}
   int  GetCoi() const { return m_Coi;}
   void* GetDataPtr();
   void* GetDataPtrP(int c);
   void** GetDataPtrP();
   char* GetRoiPtr();
   char* GetRoiPtrP(int c);
   char** GetRoiPtrP();
   IppiSize GetSize() const { IppiSize size = {Width(), Height()}; return size;}
   IppiSize GetSizeP(int c) const { IppiSize size = {WidthP(c), HeightP(c)}; return size;}
   IppiSize GetActualSize() const;
   IppiSize GetRoiSize() const
   { IppiRect rect = GetActualRoi(); IppiSize size = {rect.width, rect.height}; return size;}
   IppiSize GetRoiSizeP(int c) const
   { IppiRect rect = GetActualRoiP(c); IppiSize size = {rect.width, rect.height}; return size;}
   void GetImageParameters(
          void*& imgPtr, void*& roiPtr, int& step,
          void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4],
          IppiRect& roiRect, IppiSize& roiSize, IppiSize& imgSize);
   void GetImagePlaneParameters(
                void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4]);
   void GetFloatLimits(float& vmin, float& vmax);
   void GetFloatLimitsP(float& vmin, float& vmax, int coi = -1);

// Bit Functions
   BOOL IsBitImage() const { return m_BitImage;}
   void IsBitImage(BOOL mode) { m_BitImage = mode;}
   int BitOffset() const { return m_BitOffset;}
   void BitOffset(int mode) { m_BitOffset = mode;}
   int BitWidth() const { return m_BitWidth;}
   void BitWidth(int mode) { m_BitWidth = mode;}
   int BitRoiWidth() const { return m_BitRoiWidth;}
   void BitRoiWidth(int mode) { m_BitRoiWidth = mode;}

// Roi Functions
   void ResetRoi();
   BOOL IsRoi() const;
   BOOL CopyRoi(CImage* srcImage);
   void SetRoi(const IppiRect* roi);
   void SetRoi(const CPoint points[2]);
   void SetRoiSize(IppiSize size);
   const IppiRect* GetRoi() const;
   BOOL GetRoi(CPoint points[2]) const;
   IppiRect GetActualRoi() const;
   IppiRect GetActualRoiP(int c) const;

// Quad Functions
   void ResetQuad();
   BOOL IsQuad() const;
   BOOL IsQuad(int& num) const;
   BOOL CopyQuad(CImage* srcImage);
   void SetQuad(const double quad[4][2], int num = 4);
   void SetQuad(const CPoint points[4], int num = 4);
   BOOL GetQuad(double quad[4][2]) const;
   BOOL GetQuad(double quad[4][2], int& num) const;
   BOOL GetQuad(CPoint points[4]) const;
   BOOL GetQuad(CPoint points[4], int& num) const;

// Parl Functions
   void ResetParl();
   BOOL IsParl() const;
   BOOL IsParl(int& num) const;
   BOOL CopyParl(CImage* srcImage);
   void SetParl(const double quad[4][2], int num = 4);
   void SetParl(const CPoint points[4], int num = 4);
   BOOL GetParl(double quad[4][2]) const;
   BOOL GetParl(double quad[4][2], int& num) const;
   BOOL GetParl(CPoint points[4]) const;
   BOOL GetParl(CPoint points[4], int& num) const;
   static void SetLastParlPoint(double quad[4][2]);
   static void SetLastParlPoint(CPoint points[4]);

// CenterShift Functions
   void ResetCenterShift();
   BOOL IsCenterShift() const;
   BOOL CopyCenterShift(CImage* srcImage);
   void SetCenterShift(const double center[2], const double shift[2]);
   void SetCenterShift(const CPoint points[2]);
   BOOL GetCenterShift(double center[2], double shift[2]) const;
   BOOL GetCenterShift(CPoint points[2]) const;

// Marker Functions
   void ResetMarker();
   BOOL IsMarker() const;
   void SetMarker(const int xIndex[3], const int yIndex[3], BOOL bMax);
   int  GetMarker(POINT points[3], BOOL& bMax) const;

// Drop functions
   virtual int GetDropHeaderSize() const;
   virtual void WriteDropHeader(char* pData) const;
   virtual void ReadDropHeader(const char* pData);
   virtual int GetDropInfoSize() const;
   virtual void WriteDropInfo(char* pData) const;
   virtual void ReadDropInfo(const char* pData);
   virtual int GetDropDataSize() const;
   virtual void WriteDropData(char* pData) const;
   virtual void ReadDropData(const char* pData);

protected:
   int       m_width;
   int       m_height;
   int       m_widthP[4];
   int       m_heightP[4];
   int       m_channels;
   BOOL      m_plane;

   int       m_Compressed;

   CVector m_vectorP[4];
   int m_stepP[4];
   void* m_pointerDataP[4];
   void* m_pointerRoiP[4];

   int       m_InitFlag;
   BOOL      m_Updated;

   BOOL m_BitImage;
   int  m_BitOffset;
   int  m_BitWidth;
   int  m_BitRoiWidth;

   int       m_Coi;
   IppiRect  m_Roi;
   double    m_Quad[4][2];
   int       m_QuadNum;
   double    m_Parl[4][2];
   int       m_ParlNum;
   double    m_Center[2];
   double    m_Shift[2];
   int       m_MarkerX[3];
   int       m_MarkerY[3];
   int       m_MarkerNum;
   BOOL      m_bMarkerMax;


   void ConvertPlane   (CImage& srcImage);
   void ConvertPlane_Standard (CImage& srcImage);
   void ConvertPlane_8 (CImage& srcImage);
   void ConvertPlane_16(CImage& srcImage);
   void ConvertPlane_32(CImage& srcImage);
   int GetIndex(int x, int y, int c) const;
};

CMyString& operator << (CMyString& str, IppiAlphaType val);
CMyString& operator << (CMyString& str, IppiMaskSize val);
CMyString& operator << (CMyString& str, IppiSize val);
CMyString& operator << (CMyString& str, IppiPoint val);
CMyString& operator << (CMyString& str, IppiRect val);

#endif // !defined(AFX_IMAGE_H__03A88EA2_B7EF_11D1_AE6B_444553540000__INCLUDED_)
