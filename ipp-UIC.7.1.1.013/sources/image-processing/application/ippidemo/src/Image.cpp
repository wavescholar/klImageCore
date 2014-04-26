/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Image.h"
#include "DemoDib.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL ImgHeader::EqualPlaneSize() const
{
    if (!plane) return TRUE;
    for (int c=0;  c < channels; c++) {
        if (widthP[c] != width) return FALSE;
        if (heightP[c] != height) return FALSE;
    }
    return TRUE;
}

BOOL ImgHeader::IsSize444() const
{
     if (!plane) return FALSE;
     if (channels < 3) return FALSE;
     for (int c=0;  c < channels; c++) {
         if (widthP[c] != width) return FALSE;
     }
     return TRUE;
}

BOOL ImgHeader::IsSize422() const
{
     if (!plane) return FALSE;
     if (channels < 3) return FALSE;
     if (widthP[0] == 0) return FALSE;
     if (heightP[0] == 0) return FALSE;
     if (channels == 4) {
         if (widthP[0] != widthP[3]) return FALSE;
         if (heightP[0] != heightP[3]) return FALSE;
     }
     if (widthP[1] != (widthP[0] + 1) >> 1) return FALSE;
     if (widthP[2] != (widthP[0] + 1) >> 1) return FALSE;
     if (heightP[1] != heightP[0]) return FALSE;
     if (heightP[2] != heightP[0]) return FALSE;
     return TRUE;
}

BOOL ImgHeader::IsSize420() const
{
     if (!plane) return FALSE;
     if (widthP[0] == 0) return FALSE;
     if (heightP[0] == 0) return FALSE;
     if (channels == 3 || channels == 4) {
         if (channels == 4) {
             if (widthP[0] != widthP[3]) return FALSE;
             if (heightP[0] != heightP[3]) return FALSE;
         }
         if (widthP[1]  != (widthP[0] + 1) >> 1) return FALSE;
         if (widthP[2]  != (widthP[0] + 1) >> 1) return FALSE;
         if (heightP[1] != (heightP[0] + 1) >> 1) return FALSE;
         if (heightP[2] != (heightP[0] + 1) >> 1) return FALSE;
     } else if (channels == 2) {
         if (widthP[1]  != widthP[0]) return FALSE;
         if (heightP[1] != (heightP[0] + 1) >> 1) return FALSE;
     } else {
         return FALSE;
     }
     return TRUE;
}

BOOL ImgHeader::IsSize411() const
{
     if (!plane) return FALSE;
     if (widthP[0] == 0) return FALSE;
     if (heightP[0] == 0) return FALSE;
     if (channels == 3 || channels == 4) {
         if (channels == 4) {
             if (widthP[0] != widthP[3]) return FALSE;
             if (heightP[0] != heightP[3]) return FALSE;
         }
         if (widthP[1]  != (widthP[0] + 3) >> 2) return FALSE;
         if (widthP[2]  != (widthP[0] + 3) >> 2) return FALSE;
         if (heightP[1] != heightP[0]) return FALSE;
         if (heightP[2] != heightP[0]) return FALSE;
     } else {
         return FALSE;
     }
     return TRUE;
}

void ImgHeader::SetSize(int w, int h)
{
    if (IsSize422())
        SetSize422(w, h);
    else if (IsSize420())
        SetSize420(w, h);
    else if (IsSize411())
        SetSize411(w, h);
    else
        SetSize444(w, h);
}

static void cleanSizeP(ImgHeader* pHeader)
{
    for (int c=pHeader->channels; c < 4; c++) {
        pHeader->widthP [c] = 0;
        pHeader->heightP[c] = 0;
    }
}

void ImgHeader::SetSize444(int w, int h)
{
    cleanSizeP(this);
    width  = w;
    height = h;
    for (int c=0; c<channels; c++) {
        widthP [c] = width;
        heightP[c] = height;
    }
}

void ImgHeader::SetSize422(int w, int h)
{
    cleanSizeP(this);
    if (channels == 2) {
        h = (h >> 1) << 1;
             if (h == 0)
                        h = 2;
        SetSize444(w,h);
        return;
    }
    width  = (w >> 1) << 1;
    if (width == 0)
        width = 2;
    height = h;
    for (int c=0; c<channels; c++) {
        widthP [c] = width;
        heightP[c] = height;
        if (c == 1 || c == 2) {
            widthP [c] = width >> 1;
                      if (widthP[c] == 0)
                                 widthP[c] = 1;
        }
    }
}

void ImgHeader::SetSize420(int w, int h)
{
    cleanSizeP(this);
    width  = channels == 2 ? w : ((w >> 1) << 1);
    height = (h >> 1) << 1;
    if (width == 0)
        width = 2;
    if (height == 0)
        height = 2;

    int widthUV  = channels == 2 ? width : (width  >> 1);
    int heightUV = height >> 1;
    if (widthUV == 0)
        widthUV = 1;
    if (heightUV == 0)
        heightUV = 1;

    for (int c=0; c<channels; c++) {
        widthP [c] = width;
        heightP[c] = height;
        if (c == 1 || c == 2) {
            widthP [c] = widthUV;
            heightP[c] = heightUV;
        }
    }
}

void ImgHeader::SetSize411(int w, int h)
{
    cleanSizeP(this);
    width  = ((w >> 2) << 2);
    if (width == 0)
              width = 4;
    height = h;
    int widthUV  = width  >> 2;
    int heightUV = height;
    if (widthUV == 0)
              widthUV = 4;

    for (int c=0; c<channels; c++) {
        widthP [c] = width;
        heightP[c] = height;
        if (c == 1 || c == 2) {
            widthP [c] = widthUV;
            heightP[c] = heightUV;
        }
    }
}

void ImgHeader::SetSize(int* wP, int* hP)
{
    width  = wP[0];
    height = hP[0];
    for (int c=0; c<channels; c++) {
        widthP [c] = wP[c];
        heightP[c] = hP[c];
        if (width  < wP[c]) width  = wP[c];
        if (height < hP[c]) height = hP[c];
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
{
     Init();
}

void CImage::Init()
{
     m_Updated = TRUE;
     m_InitFlag = 0;
     m_pointer = NULL;
     m_Coi = -1;
     m_BitImage  = FALSE;
     m_BitOffset = 0;
     m_BitWidth  = 0;
     m_BitRoiWidth  = 0;

     ResetHeader();
     ResetRoi();
     ResetQuad();
     ResetParl();
     ResetCenterShift();
     ResetMarker();
     for (int c=0; c<4; c++)
         m_vectorP[c].Init(ppNONE);
}

CImage& CImage::operator =(const CImage& image)
{
    Init();
    Reset();
    m_InitFlag = image.m_InitFlag;
    m_type      = image.m_type     ;
    m_width     = image.m_width    ;
    m_height    = image.m_height  ;
    m_BitImage = image.m_BitImage ;
    m_BitOffset= image.m_BitOffset;
    m_BitWidth = image.m_BitWidth ;
    m_BitRoiWidth = image.m_BitRoiWidth ;
    for (int c=0; c<4; c++) {
        m_vectorP[c].Init(m_type);
        m_widthP [c] = image.m_widthP [c];
        m_heightP[c] = image.m_heightP[c];
    }
    m_channels = image.m_channels;
    m_plane     = image.m_plane    ;
    m_Coi        = image.m_Coi      ;
    m_Roi        = image.m_Roi      ;
    m_QuadNum  = image.m_QuadNum ;
    m_ParlNum  = image.m_ParlNum ;
    m_Updated  = image.m_Updated ;
    for (int i=0; i<2; i++) {
        m_Center[i] = image.m_Center[i];
        m_Shift[i]  = image.m_Shift[i] ;
        for (int j=0; j<4; j++) {
            m_Quad[j][i] = image.m_Quad[j][i];
            m_Parl[j][i] = image.m_Parl[j][i];
        }
    }
    if (image.m_InitFlag & initData) {
        AllocateData();
        if (Plane()) {
            for (int c=0; c<Channels(); c++)
                m_vectorP[c].CopyData(image.m_vectorP[c]);
        } else {
            CVector::CopyData(*((const CVector*)&image));
        }
    }
    return *this;
}

CImage::~CImage()
{
     FreeData();
}

void CImage::ResetHeader()
{
     m_InitFlag &= ~initHeader;
     m_type = ppNONE;
     m_width = 0;
     m_height = 0;
    for (int c=0; c<4; c++) {
        m_widthP [c] = 0;
        m_heightP[c] = 0;
    }
     m_channels = 0;
     m_plane = 0;
     m_Updated = TRUE;
     m_Compressed = 0;
}

void CImage::SetHeader(const CImage* pImg)
{
    m_InitFlag |= initHeader;
    m_type      = pImg->m_type     ;
    m_width     = pImg->m_width    ;
    m_height    = pImg->m_height  ;
    for (int c=0; c<4; c++) {
        m_vectorP[c].Init(m_type);
        m_widthP [c] = pImg->m_widthP [c];
        m_heightP[c] = pImg->m_heightP[c];
    }
    m_channels = pImg->m_channels;
    m_plane     = pImg->m_plane    ;
    m_Updated = TRUE;
}

void CImage::SetHeader(ImgHeader header)
{
    m_InitFlag |= initHeader;
    m_type      = header.type     ;
    m_width     = header.width    ;
    m_height    = header.height  ;
    for (int c=0; c<4; c++) {
        m_vectorP[c].Init(m_type);
        m_widthP [c] = header.widthP [c];
        m_heightP[c] = header.heightP[c];
    }
    m_channels = header.channels;
    m_plane     = header.plane    ;
    m_Updated = TRUE;
}

void CImage::SetHeader(ppType type, int channels,
                              int width, int height,
                              BOOL plane)
{
     m_InitFlag |= initHeader;
     m_type = type;
     m_channels = channels;
     m_width  = width;
     m_height = height;
     m_plane = plane;
     for (int c=0; c<4; c++) {
         m_vectorP[c].Init(m_type);
         if (c < m_channels) {
             m_widthP [c] = width;
             m_heightP[c] = height;
         } else {
             m_widthP [c] = 0;
             m_heightP[c] = 0;
         }
     }
     m_Updated = TRUE;
}

void CImage::SetHeader(ppType type, int channels,
                              int width[4], int height[4])
{
     m_InitFlag |= initHeader;
     m_type = type;
     m_channels = channels;
     m_width  = width [0];
     m_height = height[0];
     m_plane = TRUE;
     for (int c=0; c<4; c++) {
         m_vectorP[c].Init(m_type);
         if (c < m_channels) {
             m_widthP [c] = width [c];
             m_heightP[c] = height[c];
             if (m_width  < width [c]) m_width  = width [c];
             if (m_height < height[c]) m_height = height[c];
         } else {
             m_widthP [c] = 0;
             m_heightP[c] = 0;
         }
     }
     m_Updated = TRUE;
}

ImgHeader CImage::GetHeader() const
{
    ImgHeader header;
    header.type      = m_type     ;
    header.width     = m_width    ;
    header.height    = m_height  ;
    header.channels = m_channels;
    header.plane     = m_plane    ;
    for (int c=0; c<4; c++) {
        header.widthP [c] = m_widthP [c];
        header.heightP[c] = m_heightP[c];
    }
    return header;
}

int CImage::Step() const
{
     if (m_plane)
        return ((m_width*(Depth()>>3)+3)>>2)<<2;
     else if (Complex())
         return ((m_width*m_channels*2*(Depth()>>3)+3)>>2)<<2;
     else
         return ((m_width*m_channels*(Depth()>>3)+3)>>2)<<2;
}

int CImage::StepP(int c) const
{
     if (m_plane)
         return ((m_widthP[c]*(Depth()>>3)+3)>>2)<<2;
     else if (Channels() == 1)
         return Step();
     else
         return 0;
}

int* CImage::StepP()
{
     if (!m_plane) return NULL;
     for (int c=0; c<4; c++) {
         if (c < m_channels)
             m_stepP [c] = StepP(c);
         else
             m_stepP [c] = 0;
     }
     return m_stepP;
}

int CImage::Size()  const
{
     return Step()*m_height;
}

int CImage::SizeP(int c)  const
{
     if (m_plane) return StepP(c)*m_heightP[c];
     else            return Size();
}

CString CImage::TypeString() const
{
    if (IsBitImage())
        return "1u";
    else
        return CVector::TypeString();
}

CString CImage::ChannelString() const
{
    CString C = Plane() ? "P" : "C";
    switch (m_channels) {
    case 1: return C + "1";
    case 2: return C + "2";
    case 3: return C + "3";
    case 4: return "A" + C + "4";
    default: return "";
    }
}

CString CImage::CoiString()  const
{
    switch (m_Coi) {
    case 0: return "Blue";
    case 1: return "Green";
    case 2: return "Red";
    case 3: return "Alpha";
    default: return "";
    }
}

BOOL CImage::IsHeader() const
{
     return m_InitFlag & initHeader;
}

BOOL CImage::EqualPlaneSize() const
{
    return GetHeader().EqualPlaneSize();
}

BOOL CImage::Is422() const
{
    ImgHeader header = GetHeader();
    return header.IsSize422();
}

BOOL CImage::Is420() const
{
    ImgHeader header = GetHeader();
    return header.IsSize420();
}

BOOL CImage::Is411() const
{
    ImgHeader header = GetHeader();
    return header.IsSize411();
}

void CImage::AllocateData(char value)
{
     if (!(m_InitFlag & initHeader)) return;
     if (Float() && value == 0xFF) value = 0;
     if (m_plane) {
         CVector::Deallocate();
     } else {
         CVector::AllocateData(Size(),value);
     }
     for (int c=0; c<4; c++) {
         if (m_plane && c < Channels()) {
             m_vectorP[c].AllocateData(SizeP(c),value);
         } else {
             m_vectorP[c].Deallocate();
         }
     }
     m_InitFlag |= initData;
     m_InitFlag |= initAlloc;
     m_Updated = TRUE;
}

void CImage::FreeData()
{
     Deallocate();
     m_pointer = NULL;
     for (int c=0; c<4; c++) {
         m_vectorP[c].Deallocate();
     }
     m_InitFlag &= ~initData;
     m_InitFlag &= ~initAlloc;
     m_Updated = TRUE;
}

void CImage::SetData(const void* data, int size)
{
     if (!(m_InitFlag & initData)) return;
     if (m_plane) return;
     if (size > Size()) size = Size();
     memcpy(m_pointer,data,size);
     m_Updated = TRUE;
}

void CImage::SetDataP(int c, const void* data, int size)
{
     if (!(m_InitFlag & initData)) return;
     if (!m_plane) return;
     if (size > SizeP(c)) size = SizeP(c);
     memcpy(GetDataPtrP(c),data,size);
     m_Updated = TRUE;
}

void CImage::SetData(char value)
{
     if (!(m_InitFlag & initData)) return;
     if (m_plane) {
         for (int c=0; c<4; c++)
             memset(GetDataPtrP(c),value,SizeP(c));
     } else {
         memset(m_pointer,value,Size());
     }
     m_Updated = TRUE;
}

BOOL CImage::CopyData(CImage* pImage)
{
     if (!pImage) return FALSE;
     if ((m_type & ~PP_SIGN) != (pImage->m_type & ~PP_SIGN)) return FALSE;
     if (m_plane     != pImage->m_plane ) return FALSE;
     if (m_channels != pImage->m_channels) return FALSE;
     if (m_plane) {
         for (int c=0; c < Channels(); c++) {
             if (m_widthP [c] != pImage->m_widthP [c]) return FALSE;
             if (m_heightP[c] != pImage->m_heightP[c]) return FALSE;
         }
     } else {
         if (m_width     != pImage->m_width    ) return FALSE;
         if (m_height    != pImage->m_height  ) return FALSE;
     }
     if (m_plane)
         return CopyDataP(pImage->GetDataPtrP());
     else
         return CopyData(pImage->GetDataPtr());
}

BOOL CImage::CopyData_ROI(CImage* pImage)
{
     if (!pImage) return FALSE;
     if ((m_type & ~PP_SIGN) != (pImage->m_type & ~PP_SIGN)) return FALSE;
     if (m_plane     != pImage->m_plane ) return FALSE;
     if (m_channels != pImage->m_channels) return FALSE;
     IppiSize size = GetRoiSize();
     IppiSize imgSize = pImage->GetRoiSize();
     size.width = MIN(size.width, imgSize.width);
     size.height = MIN(size.height, imgSize.height);
     if (m_plane)
          return CopyDataP_ROI((void**)pImage->GetRoiPtrP(), pImage->StepP(), size);
     else
         return CopyData_ROI(pImage->GetRoiPtr(), pImage->Step(), size);
}

BOOL CImage::CopyData(void* pData)
{
     if (!(m_InitFlag & initAlloc)) return FALSE;
     if (!pData) return FALSE;
     if (m_plane) return FALSE;
     memcpy(m_pointer,pData,Size());
     m_Updated = TRUE;
     return TRUE;
}

BOOL CImage::CopyDataP(void** ppData)
{
     if (!(m_InitFlag & initAlloc)) return FALSE;
     if (!ppData) return FALSE;
     if (!m_plane) return FALSE;
     for (int c=0; c < Channels(); c++) {
         memcpy(GetDataPtrP(c),ppData[c],SizeP(c));
     }
     m_Updated = TRUE;
     return TRUE;
}

BOOL CImage::CopyData_ROI(void* pData, int step, IppiSize size)
{
     if (!(m_InitFlag & initAlloc)) return FALSE;
     if (!pData) return FALSE;
     if (m_plane) return FALSE;
     size.width *= ItemSize()*Channels();
     ippiCopy_8u_C1R((Ipp8u*)pData, step, (Ipp8u*)GetRoiPtr(), Step(), size);
     m_Updated = TRUE;
     return TRUE;
}

BOOL CImage::CopyDataP_ROI(void** ppData, int* step, IppiSize roiSize)
{
     if (!(m_InitFlag & initAlloc)) return FALSE;
     if (!ppData) return FALSE;
     if (!m_plane) return FALSE;
     roiSize.width *= ItemSize();
     for (int c=0; c < Channels(); c++) {
          IppiSize size = roiSize;
          CorrectPlaneSize(size.width,size.height,c);
          ippiCopy_8u_C1R((Ipp8u*)ppData[c], step[c], (Ipp8u*)GetRoiPtrP(c), StepP(c), size);
     }
     m_Updated = TRUE;
     return TRUE;
}

void* CImage::GetDataPtr()
{
    if (m_plane) return NULL;
     if (!(m_InitFlag & initData)) return NULL;
     m_Updated = TRUE;
     return (void*)m_pointer;
}

void** CImage::GetDataPtrP()
{
    if (!m_plane) return NULL;
    if (!(m_InitFlag & initData)) return NULL;
    m_Updated = TRUE;
    for (int c=0; c < 4; c++) {
        if (c < Channels())
            m_pointerDataP[c] = GetDataPtrP(c);
        else
            m_pointerDataP[c] = NULL;
    }
    return (void**)m_pointerDataP;
}

void* CImage::GetDataPtrP(int c)
{
    if (!m_plane) return NULL;
    if (!(m_InitFlag & initData)) return NULL;
    m_Updated = TRUE;
    return (void*)m_vectorP[c].GetData();
}

char* CImage::GetRoiPtr()
{
    if (m_plane) return NULL;
    if (!(m_InitFlag & initData)) return NULL;
    m_Updated = TRUE;
    if (m_InitFlag & initRoi)
        return (char*)m_pointer + m_Roi.y*Step() +
                          m_Roi.x*(Depth()>>3)*m_channels;
    else
        return (char*)m_pointer;
}

char** CImage::GetRoiPtrP()
{
    if (!m_plane) return NULL;
    if (!(m_InitFlag & initData)) return NULL;
    m_Updated = TRUE;
    if (m_InitFlag & initRoi) {
         for (int c=0; c < 4; c++) {
             if (c < Channels())
                 m_pointerRoiP[c] = GetRoiPtrP(c);
             else
                 m_pointerRoiP[c] = NULL;
         }
         return (char**)m_pointerRoiP;
     } else {
          return (char**)GetDataPtrP();
     }
}

char* CImage::GetRoiPtrP(int c)
{
     if (!m_plane) return NULL;
     if (!(m_InitFlag & initData)) return NULL;
     m_Updated = TRUE;
     if (m_InitFlag & initRoi) {
         int x = m_Roi.x;
         int y = m_Roi.y;
         CorrectPlaneSize(x,y,c);
         if (x >= m_widthP[c]) return NULL;
         if (y >= m_heightP[c]) return NULL;
         return (char*)GetDataPtrP(c) + y*StepP(c) + x*(Depth()>>3);
     } else {
          return (char*)GetDataPtrP(c);
     }
}

void CImage::ConvertToRGB()
{
    if (Plane() && (Channels() == 1)) m_plane = FALSE;
    if (m_type == pp8s) m_type = pp8u;
    if (m_type == pp16s) m_type = pp16u;
    if (m_type == pp32u) m_type = pp32s;
    if ((Type() == pp8u) && !Plane()) return;

    CImage rgbImage;
    rgbImage.SetHeader(pp8u,
        (Channels() == 2) ? 3 : Channels(),
        Width(), Height(), FALSE);
    rgbImage.AllocateData(0);
    CDemoDib::ConvertToRGB(this,(Ipp8u*)rgbImage.GetDataPtr(),rgbImage.Step(),TRUE);
    *this = rgbImage;
}

void CImage::Set(int x, int y, int c, double value, double imval)
{
    if (m_plane)
        m_vectorP[c].Set(GetIndex(x,y,c), value, imval);
    else
        CVector::Set(GetIndex(x,y,c), value, imval);
}

void CImage::Set(int x, int y, int c, Ipp64fc value)
{
    if (m_plane)
        m_vectorP[c].Set(GetIndex(x,y,c), value);
    else
        CVector::Set(GetIndex(x,y,c), value);
}


Ipp64fc CImage::Get(int x, int y, int c) const
{
    if (m_plane)
        return m_vectorP[c].Get(GetIndex(x,y,c));
    else
        return CVector::Get(GetIndex(x,y,c));
}

int      CImage::GetInt(int x, int y, int c) const
{
    if (m_plane)
        return m_vectorP[c].GetInt(GetIndex(x,y,c));
    else
        return CVector::GetInt(GetIndex(x,y,c));
}

double  CImage::GetDouble(int x, int y, int c) const
{
    if (m_plane)
        return m_vectorP[c].GetDouble(GetIndex(x,y,c));
    else
        return CVector::GetDouble(GetIndex(x,y,c));
}

void CImage::Set(int x, int y, const int pixel[4])
{
    for (int c=0; c < Channels(); c++) {
        if (m_plane)
            m_vectorP[c].Set(GetIndex(x,y,c), (double)pixel[c]);
        else
            CVector::Set(GetIndex(x,y,c), (double)pixel[c]);
    }
}

void CImage::Get(int x, int y, int pixel[4]) const
{
    for (int c=0; c < Channels(); c++) {
        if (m_plane)
            pixel[c] = m_vectorP[c].GetInt(GetIndex(x,y,c));
        else
            pixel[c] = CVector::GetInt(GetIndex(x,y,c));
    }
}

void CImage::Set(int x, int y, int c, CValue val)
{
    if (m_plane)
        m_vectorP[c].Set(GetIndex(x,y,c), val);
    else
        CVector::Set(GetIndex(x,y,c), val);
}

void CImage::Get(int x, int y, int c, CValue& val) const
{
    if (m_plane)
        m_vectorP[c].Get(GetIndex(x,y,c), val);
    else
        CVector::Get(GetIndex(x,y,c), val);
}

void CImage::Set(int x, int y, int c, CString str)
{
    if (m_plane)
        m_vectorP[c].Set(GetIndex(x,y,c), str);
    else
        CVector::Set(GetIndex(x,y,c), str);
}

void CImage::Get(int x, int y, int c, CString& str, LPCSTR cpxFormat) const
{
    if (m_plane)
        m_vectorP[c].Get(GetIndex(x,y,c), str, cpxFormat);
    else
        CVector::Get(GetIndex(x,y,c), str, cpxFormat);
}

int CImage::GetIndex(int x, int y, int c) const
{
    if (Plane()) {
        if (x >= m_widthP [c]) return -1;
        if (y >= m_heightP[c]) return -1;
        return (y*StepP(c))/ItemSize() + x;
    } else {
        if (x >= m_width ) return -1;
        if (y >= m_height) return -1;
        return (y*Step())/ItemSize() + x*Channels() + c;
    }
}

//==================  ROI ==============================
void CImage::ResetRoi()
{
     m_InitFlag &= ~initRoi;
     m_Roi.x = m_Roi.y = m_Roi.width = m_Roi.height = 0;
     m_Updated = TRUE;
}

BOOL CImage::IsRoi()const
{
     return m_InitFlag & initRoi ? TRUE : FALSE;
}

BOOL CImage::CopyRoi(CImage* pImage)
{
     const IppiRect *roi = pImage->GetRoi();
     if (!roi) return FALSE;
     SetRoi(roi);
     return TRUE;
}

void CImage::SetRoi(const IppiRect* roi)
{
     if (roi) {
          m_InitFlag |= initRoi;
          m_Roi = *roi;
          m_Updated = TRUE;
     } else {
          m_InitFlag &= ~initRoi;
          m_Updated = TRUE;
     }
}

void CImage::SetRoi(const CPoint points[2])
{
     ASSERT(points);
     m_InitFlag |= initRoi;
     m_Updated = TRUE;

    CPoint p0, p1;
    if (points[0].x <= points[1].x) {
        p0.x = points[0].x;
        p1.x = points[1].x;
    } else {
        p0.x = points[1].x;
        p1.x = points[0].x;
    }
    if (points[0].y <= points[1].y) {
        p0.y = points[0].y;
        p1.y = points[1].y;
    } else {
        p0.y = points[1].y;
        p1.y = points[0].y;
    }
     m_Roi.x = p0.x;
     m_Roi.y = p0.y;
     m_Roi.width  = p1.x - m_Roi.x + 1;
     m_Roi.height = p1.y - m_Roi.y + 1;
}

void CImage::SetRoiSize(IppiSize size)
{
    IppiRect roi = GetActualRoi();
    roi.width = size.width;
    roi.height = size.height;
    SetRoi(&roi);
}

const IppiRect* CImage::GetRoi()const
{
     if (!(m_InitFlag & initRoi)) return NULL;
     return &m_Roi;
}

BOOL CImage::GetRoi(CPoint points[2])const
{
     if (!(m_InitFlag & initRoi)) return FALSE;

     IppiRect roi = m_Roi;
     CorrectPlaneSize(roi.x, roi.y, GetCoi());
     CorrectPlaneSize(roi.width, roi.height, GetCoi());
     points[0].x = roi.x;
     points[0].y = roi.y;
     points[1].x = roi.x + roi.width - 1;
     points[1].y = roi.y + roi.height - 1;
     return TRUE;
}

void CImage::CorrectPlaneSize(int& width, int& height, int coi) const
{
    if (coi < 0) return;
    if (Channels() == 2) {
        if (coi == 1) {
            if (Is420())
                height >>= 1;
        }
    } else {
        if (coi == 1 || coi == 2) {
            if (Is422() || Is420())
                width >>= 1;
            if (Is420())
                height >>= 1;
            if (Is411())
                width >>= 2;
        }
    }
    if (width == 0)
              width = 1;
    if (height == 0)
              height = 1;
}

IppiRect CImage::GetActualRoi()const
{
    IppiRect roi = {0, 0, 0, 0};
     if (!(m_InitFlag & initHeader)) return roi;
     if (m_InitFlag & initRoi) {
          roi = m_Roi;
     } else {
          roi.x = 0;
          roi.y = 0;
          roi.width  = m_width;
          roi.height = m_height;
     }
     CorrectPlaneSize(roi.x, roi.y, GetCoi());
     CorrectPlaneSize(roi.width, roi.height, GetCoi());
     return roi;
}

IppiSize CImage::GetActualSize()const
{
    IppiSize size = GetSize();
    CorrectPlaneSize(size.width, size.height, GetCoi());
    return size;
}

IppiRect CImage::GetActualRoiP(int c)const
{
     IppiRect roi = {0, 0, 0, 0};
     if (!(m_InitFlag & initHeader)) return roi;
     if (m_InitFlag & initRoi) {
          roi = m_Roi;
          CorrectPlaneSize(roi.width, roi.height, c);
          CorrectPlaneSize(roi.x, roi.y, c);
     } else {
          roi.x = 0;
          roi.y = 0;
          roi.width  = m_widthP[c];
          roi.height = m_heightP[c];
     }
     return roi;
}

//==================  QUAD ==============================
void CImage::ResetQuad()
{
    m_InitFlag &= ~initQuad;
    m_QuadNum = 0;
    m_Updated = TRUE;
}

BOOL CImage::IsQuad(int& num)const
{
    num = m_QuadNum;
    return num ? TRUE : FALSE;
}

BOOL CImage::IsQuad()const
{
    int num;
    IsQuad(num);
    return num == 4 ? TRUE : FALSE;
}

BOOL CImage::CopyQuad(CImage* pImage)
{
    double quad[4][2];
    if (!pImage->GetQuad(quad))
         return FALSE;
    SetQuad(quad);
    return TRUE;
}

void CImage::SetQuad(const double quad[4][2], int num)
{
    ASSERT(quad);
    ASSERT(num <= 4);
    for (int i=0; i<num; i++) {
        for (int j=0; j<2; j++) {
            m_Quad[i][j] = quad[i][j];
        }
    }
     m_InitFlag |= initQuad;
    m_QuadNum = num;
     m_Updated = TRUE;
}

void CImage::SetQuad(const CPoint points[4], int num)
{
    ASSERT(points);
    ASSERT(num <= 4);
    for (int i=0; i<num; i++) {
        m_Quad[i][0] = points[i].x;
        m_Quad[i][1] = points[i].y;
    }
     m_InitFlag |= initQuad;
    m_QuadNum = num;
     m_Updated = TRUE;
}

BOOL CImage::GetQuad(double quad[4][2], int& num)const
{
    ASSERT(quad);
    if (!IsQuad(num)) return FALSE;
    for (int i=0; i<num; i++) {
        for (int j=0; j<2; j++) {
            quad[i][j] = m_Quad[i][j];
        }
    }
    return TRUE;
}

BOOL CImage::GetQuad(double quad[4][2])const
{
    if (!IsQuad()) return FALSE;
    int num;
    return GetQuad(quad,num);
}

BOOL CImage::GetQuad(CPoint points[4], int& num)const
{
    ASSERT(points);
    if (!IsQuad(num)) return FALSE;
    for (int i=0; i<num; i++) {
        points[i].x = (int)(m_Quad[i][0] + .5);
        points[i].y = (int)(m_Quad[i][1] + .5);
    }
    return TRUE;
}

BOOL CImage::GetQuad(CPoint points[4])const
{
    if (!IsQuad()) return FALSE;
    int num;
    return GetQuad(points,num);
}

//==================  PARL ==============================
void CImage::ResetParl()
{
    m_InitFlag &= ~initParl;
    m_ParlNum = 0;
    m_Updated = TRUE;
}

BOOL CImage::IsParl(int& num)const
{
    num = m_ParlNum;
    return num ? TRUE : FALSE;
}

BOOL CImage::IsParl()const
{
    int num;
    IsParl(num);
    return num == 4 ? TRUE : FALSE;
}

BOOL CImage::CopyParl(CImage* pImage)
{
    double quad[4][2];
    if (!pImage->GetParl(quad))
         return FALSE;
    SetParl(quad);
    return TRUE;
}

void CImage::SetParl(const double quad[4][2], int num)
{
    ASSERT(quad);
    ASSERT(num <= 4);
    for (int i=0; i<num; i++) {
        for (int j=0; j<2; j++) {
            m_Parl[i][j] = quad[i][j];
        }
    }
     m_InitFlag |= initParl;
    m_ParlNum = num;
     m_Updated = TRUE;
}

void CImage::SetParl(const CPoint points[4], int num)
{
    ASSERT(points);
    ASSERT(num <= 4);
    for (int i=0; i<num; i++) {
        m_Parl[i][0] = points[i].x;
        m_Parl[i][1] = points[i].y;
    }
     m_InitFlag |= initParl;
    m_ParlNum = num;
     m_Updated = TRUE;
}

BOOL CImage::GetParl(double quad[4][2], int& num)const
{
    ASSERT(quad);
    if (!IsParl(num)) return FALSE;
    for (int i=0; i<num; i++) {
        for (int j=0; j<2; j++) {
            quad[i][j] = m_Parl[i][j];
        }
    }
    return TRUE;
}

BOOL CImage::GetParl(double quad[4][2])const
{
    if (!IsParl()) return FALSE;
    int num;
    return GetParl(quad,num);
}

BOOL CImage::GetParl(CPoint points[4], int& num)const
{
    ASSERT(points);
    if (!IsParl(num)) return FALSE;
    for (int i=0; i<num; i++) {
        points[i].x = (int)(m_Parl[i][0] + .5);
        points[i].y = (int)(m_Parl[i][1] + .5);
    }
    return TRUE;
}

BOOL CImage::GetParl(CPoint points[4])const
{
    if (!IsParl()) return FALSE;
    int num;
    return GetParl(points,num);
}

static void LastParlPoint(const double quad[4][2], double point[2])
{
    point[0] = quad[0][0] - quad[1][0] + quad[2][0];
    point[1] = quad[0][1] - quad[1][1] + quad[2][1];
}

static void LastParlPoint(CPoint quad[4], CPoint& point)
{
    point.x = quad[0].x - quad[1].x + quad[2].x;
    point.y = quad[0].y - quad[1].y + quad[2].y;
}

void CImage::SetLastParlPoint(double quad[4][2])
{
    double lastPoint[2];
    LastParlPoint(quad, lastPoint);
    quad[3][0] = lastPoint[0];
    quad[3][1] = lastPoint[1];
}

void CImage::SetLastParlPoint(CPoint points[4])
{
    LastParlPoint(points,points[3]);
}

//==================  CENTER SHIFT ==============================
void CImage::ResetCenterShift()
{
     m_InitFlag &= ~initCenter;
     m_Updated = TRUE;
}

BOOL CImage::IsCenterShift()const
{
    return (m_InitFlag & initCenter) ? TRUE : FALSE;
}

BOOL CImage::CopyCenterShift(CImage* pImage)
{
     double center[2];
     double shift[2];
     if (!pImage->GetCenterShift(center, shift))
          return FALSE;
     SetCenterShift(center, shift);
     return TRUE;
}

void CImage::SetCenterShift(const double center[2], const double shift[2])
{
    ASSERT(center);
    ASSERT(shift);

    m_Center[0] = center[0];
    m_Center[1] = center[1];
    m_Shift[0] = shift[0];
    m_Shift[1] = shift[1];

     m_InitFlag |= initCenter;
     m_Updated = TRUE;
}

void CImage::SetCenterShift(const CPoint points[2])
{
    m_Center[0] = points[0].x;
    m_Center[1] = points[0].y;
    m_Shift[0] = points[1].x - points[0].x;
    m_Shift[1] = points[1].y - points[0].y;

     m_InitFlag |= initCenter;
     m_Updated = TRUE;
}

BOOL CImage::GetCenterShift(double center[2], double shift[2])const
{
    ASSERT(center);
    ASSERT(shift);
    if (!IsCenterShift()) return FALSE;

    center[0] = m_Center[0];
    center[1] = m_Center[1];
    shift[0]  = m_Shift[0] ;
    shift[1]  = m_Shift[1] ;
    return TRUE;
}

BOOL CImage::GetCenterShift(CPoint points[2])const
{
    if (!IsCenterShift()) return FALSE;

    points[0].x = (int)(m_Center[0] + .5);
    points[0].y = (int)(m_Center[1] + .5);
    points[1].x  = (int)(m_Center[0] + m_Shift[0] + .5);
    points[1].y  = (int)(m_Center[1] + m_Shift[1] + .5);
    return TRUE;
}

//==================  CENTER SHIFT ==============================
void CImage::ResetMarker()
{
    m_MarkerNum = 0;
}

BOOL CImage::IsMarker() const
{
    return m_MarkerNum;
}

void CImage::SetMarker(const int xIndex[3], const int yIndex[3], BOOL bMax)
{
    IppiRect roi = GetActualRoi();
    m_MarkerNum = Channels();
    m_bMarkerMax = bMax;
    if (m_MarkerNum == 4) m_MarkerNum = 3;
    for (int i=0; i < m_MarkerNum; i++) {
        m_MarkerX[i] = roi.x + xIndex[i];
        m_MarkerY[i] = roi.y + yIndex[i];
    }
}

int  CImage::GetMarker(POINT points[3], BOOL& bMax) const
{
    for (int i=0; i < m_MarkerNum; i++) {
        points[i].x = m_MarkerX[i];
        points[i].y = m_MarkerY[i];
    }
    bMax = m_bMarkerMax;
    return m_MarkerNum;
}

//===============================================================
void CImage::Reset()
{
     ResetHeader();
     ResetRoi();
     ResetQuad();
     ResetParl();
     ResetCenterShift();
     FreeData();
     m_Updated = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int  CImage::GetDropHeaderSize() const
{
    return sizeof(ImgHeader);
}

void CImage::WriteDropHeader(char* pData) const
{
    ImgHeader header = GetHeader();
    memcpy(pData, &header, sizeof(ImgHeader));
}

void CImage::ReadDropHeader(const char* pData)
{
    ImgHeader header;
    memcpy(&header, pData, sizeof(ImgHeader));
    SetHeader(header);
}

int  CImage::GetDropInfoSize() const
{
    int result = 0;
    result += sizeof(int);
    result += sizeof(int);

    result += sizeof(BOOL);
    result += sizeof(int );
    result += sizeof(int );
    result += sizeof(int );

    result += sizeof(int      );
    result += sizeof(IppiRect);
    result += sizeof(int      );
    result += sizeof(int      );

    for (int i=0; i<2; i++) {
        result += sizeof(double);
        result += sizeof(double);
        for (int j=0; j<4; j++) {
            result += sizeof(double);
            result += sizeof(double);
        }
    }
    return result;
}

void CImage::WriteDropInfo(char* pData) const
{
    memcpy(pData, &m_Compressed, sizeof(int)); pData += sizeof(int);
    memcpy(pData, &m_InitFlag  , sizeof(int)); pData += sizeof(int);

    memcpy(pData, &m_BitImage    , sizeof(BOOL)); pData += sizeof(BOOL);
    memcpy(pData, &m_BitOffset  , sizeof(int )); pData += sizeof(int );
    memcpy(pData, &m_BitWidth    , sizeof(int )); pData += sizeof(int );
    memcpy(pData, &m_BitRoiWidth, sizeof(int )); pData += sizeof(int );

    memcpy(pData, &m_Coi         , sizeof(int      )); pData += sizeof(int      );
    memcpy(pData, &m_Roi         , sizeof(IppiRect)); pData += sizeof(IppiRect);
    memcpy(pData, &m_QuadNum    , sizeof(int      )); pData += sizeof(int      );
    memcpy(pData, &m_ParlNum    , sizeof(int      )); pData += sizeof(int      );

    for (int i=0; i<2; i++) {
        memcpy(pData, &(m_Center[i]), sizeof(double)); pData += sizeof(double);
        memcpy(pData, &(m_Shift [i]), sizeof(double)); pData += sizeof(double);
        for (int j=0; j<4; j++) {
            memcpy(pData, &(m_Quad[j][i]), sizeof(double)); pData += sizeof(double);
            memcpy(pData, &(m_Parl[j][i]), sizeof(double)); pData += sizeof(double);
        }
    }
}

void CImage::ReadDropInfo(const char* pData)
{
    memcpy(&m_Compressed , pData, sizeof(int)); pData += sizeof(int);
    memcpy(&m_InitFlag    , pData, sizeof(int)); pData += sizeof(int);

    memcpy(&m_BitImage    , pData, sizeof(BOOL)); pData += sizeof(BOOL);
    memcpy(&m_BitOffset  , pData, sizeof(int )); pData += sizeof(int );
    memcpy(&m_BitWidth    , pData, sizeof(int )); pData += sizeof(int );
    memcpy(&m_BitRoiWidth, pData, sizeof(int )); pData += sizeof(int );

    memcpy(&m_Coi          , pData, sizeof(int      )); pData += sizeof(int      );
    memcpy(&m_Roi          , pData, sizeof(IppiRect)); pData += sizeof(IppiRect);
    memcpy(&m_QuadNum     , pData, sizeof(int      )); pData += sizeof(int      );
    memcpy(&m_ParlNum     , pData, sizeof(int      )); pData += sizeof(int      );

    for (int i=0; i<2; i++) {
        memcpy(&(m_Center[i]), pData, sizeof(double)); pData += sizeof(double);
        memcpy(&(m_Shift [i]), pData, sizeof(double)); pData += sizeof(double);
        for (int j=0; j<4; j++) {
            memcpy(&(m_Quad[j][i]), pData, sizeof(double)); pData += sizeof(double);
            memcpy(&(m_Parl[j][i]), pData, sizeof(double)); pData += sizeof(double);
        }
    }
}

int  CImage::GetDropDataSize() const
{
    int result = 0;
    if (m_plane) {
        for (int c=0; c<Channels(); c++) {
            result += SizeP(c);
        }
    } else {
        result += Size();
    }
    return result;
}

void CImage::WriteDropData(char* pData) const
{
     if (m_plane) {
         for (int c=0; c<Channels(); c++) {
             memcpy(pData, m_vectorP[c].GetData(), SizeP(c)); pData += SizeP(c);
         }
     } else {
         memcpy(pData, m_pointer, Size()); pData += Size();
     }
}

void CImage::ReadDropData(const char* pData)
{
     if (m_plane) {
         for (int c=0; c<Channels(); c++) {
             memcpy(m_vectorP[c].GetData(), pData, SizeP(c)); pData += SizeP(c);
         }
     } else {
         memcpy(m_pointer, pData, Size()); pData += Size();
     }
}

void CImage::GetImageParameters(
                    void*& imgPtr, void*& roiPtr, int& step,
                    void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4],
                    IppiRect& roiRect, IppiSize& roiSize, IppiSize& imgSize)
{
    step = Step();
    imgSize.width  = Width() ;
    imgSize.height = Height();
    roiRect = GetActualRoi();
    roiSize.width  = roiRect.width ;
    roiSize.height = roiRect.height;
    imgPtr = GetDataPtr();
    roiPtr = GetRoiPtr();

    GetImagePlaneParameters(imgPlanePtr, roiPlanePtr, planeStep);

    int coi = GetCoi();
    if (Plane() && coi >= 0) {
        imgPtr = imgPlanePtr[coi];
        roiPtr = roiPlanePtr[coi];
        step    = planeStep[coi];
    }
}

void CImage::GetImagePlaneParameters(
                 void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4])
{
    int i;
    for (i=0; i<4; i++) {
        imgPlanePtr[i] = NULL;
        roiPlanePtr[i] = NULL;
        planeStep[i] = 0;
    }
    if (!Plane()) return;

    for (i=0; i<Channels(); i++) {
        planeStep[i] = StepP(i);
        imgPlanePtr[i] = GetDataPtrP(i);
        roiPlanePtr[i] = GetRoiPtrP(i);
    }
}

void CImage::GetFloatLimits(Ipp32f& minVal, Ipp32f& maxVal)
{
    minVal = maxVal = 0;
    if (!Float()) return;
    if (Plane())
    {
        GetFloatLimitsP(minVal, maxVal);
        return;
    }
    Ipp8u* src = (Ipp8u*)GetDataPtr();
    IppiSize roiSize = GetSize();
    switch (Channels()) {
    case 4:
        {
            Ipp32f vmin[3];
            Ipp32f vmax[3];
            ippiMin_32f_AC4R((Ipp32f*)src,Step(),roiSize,vmin);
            ippiMax_32f_AC4R((Ipp32f*)src,Step(),roiSize,vmax);
            minVal = vmin[0];
            maxVal = vmax[0];
            for (int c = 1; c<3; c++)
            {
                if (minVal > vmin[c]) minVal = vmin[c];
                if (maxVal < vmax[c]) maxVal = vmax[c];
            }
        }
        break;
    default:
        roiSize.width *= Channels();
        ippiMin_32f_C1R((Ipp32f*)src,Step(),roiSize,&minVal);
        ippiMax_32f_C1R((Ipp32f*)src,Step(),roiSize,&maxVal);
        break;
    }
}

void CImage::GetFloatLimitsP(Ipp32f& minVal, Ipp32f& maxVal, int coi)
{
    minVal = maxVal = 0;
    if (!Float()) return;
    if (!Plane()) return;
    Ipp32f vmin[4];
    Ipp32f vmax[4];
    if ((coi < 0) && (GetCoi() == 3))
        coi = 3;
    int c1 = coi < 0 ? 0 : coi;
    int c2 = coi < 0 ? Channels() : coi + 1;
    if ((coi < 0) && (c2 == 4))
            c2 = 3;
    for (int c=c1; c < c2; c++) 
    {
        Ipp8u* src = (Ipp8u*)GetDataPtrP(c);
        IppiSize roiSize = GetSizeP(c);
        ippiMin_32f_C1R((Ipp32f*)src,StepP(c),roiSize,vmin + c);
        ippiMax_32f_C1R((Ipp32f*)src,StepP(c),roiSize,vmax + c);
    }
    minVal = vmin[c1];
    maxVal = vmax[c1];
    if (coi >= 0)
        return;
    for (int c = c1 + 1; c < c2; c++)
    {
        if (minVal > vmin[c]) minVal = vmin[c];
        if (maxVal < vmax[c]) maxVal = vmax[c];
    }
}

void CImage::ConvertPlane(BOOL plane)
{
    if (plane == Plane()) return;
    if (Channels() == 1) return;
    CImage srcImage(*this);
    ImgHeader header = GetHeader();
    header.plane = plane;
    SetHeader(header);
    AllocateData();
    ConvertPlane(srcImage);
}

void CImage::ConvertPlane(CImage& srcImage)
{
    if (EqualPlaneSize() && srcImage.EqualPlaneSize() &&
        (Channels() == 3 || Channels() == 4)) {
        ConvertPlane_Standard(srcImage); return;
    }
    switch (Depth()) {
    case 8 : ConvertPlane_8 (srcImage); return;
    case 16: ConvertPlane_16(srcImage); return;
    case 32: ConvertPlane_32(srcImage); return;
    }
}

void CImage::ConvertPlane_Standard(CImage& srcImage)
{
    IppiSize size = {Width(), Height()};
    void* srcData    = srcImage.GetDataPtr();
    void** srcPlane = srcImage.GetDataPtrP();
    int srcStep      = srcImage.Step();
    void* dstData    = GetDataPtr();
    void** dstPlane = GetDataPtrP();
    int dstStep      = Step();

    switch (Channels()) {
    case 3:
        if (Plane()) {
            switch (Depth()) {
            case 8:
                ippiCopy_8u_C3P3R((Ipp8u*)srcData, srcStep,
                                        (Ipp8u**)dstPlane, dstStep, size );
                break;
            case 16:
                ippiCopy_16s_C3P3R((Ipp16s*)srcData, srcStep,
                                         (Ipp16s**)dstPlane, dstStep, size );
                break;
            case 32:
                ippiCopy_32f_C3P3R((Ipp32f*)srcData, srcStep,
                                         (Ipp32f**)dstPlane, dstStep, size );
                break;
            }
        } else {
            switch (Depth()) {
            case 8:
                ippiCopy_8u_P3C3R((Ipp8u**)srcPlane, srcStep,
                                                (Ipp8u*)dstData, dstStep, size );
                break;
            case 16:
                ippiCopy_16s_P3C3R((Ipp16s**)srcPlane, srcStep,
                                                (Ipp16s*)dstData, dstStep, size );
                break;
            case 32:
                ippiCopy_32f_P3C3R((Ipp32f**)srcPlane, srcStep,
                                                (Ipp32f*)dstData, dstStep, size );
                break;
            }
        }
        break;
    case 4:
        if (Plane()) {
            switch (Depth()) {
            case 8:
                ippiCopy_8u_C4P4R((Ipp8u*)srcData, srcStep,
                                                (Ipp8u**)dstPlane, dstStep, size );
                break;
            case 16:
                ippiCopy_16s_C4P4R((Ipp16s*)srcData, srcStep,
                                                (Ipp16s**)dstPlane, dstStep, size );
                break;
            case 32:
                ippiCopy_32f_C4P4R((Ipp32f*)srcData, srcStep,
                                                (Ipp32f**)dstPlane, dstStep, size );
                break;
            }
        } else {
            switch (Depth()) {
            case 8:
                ippiCopy_8u_P4C4R((Ipp8u**)srcPlane, srcStep,
                                                (Ipp8u*)dstData, dstStep, size );
                break;
            case 16:
                ippiCopy_16s_P4C4R((Ipp16s**)srcPlane, srcStep,
                                                (Ipp16s*)dstData, dstStep, size );
                break;
            case 32:
                ippiCopy_32f_P4C4R((Ipp32f**)srcPlane, srcStep,
                                                (Ipp32f*)dstData, dstStep, size );
                break;
            }
        }
        break;
    }
}

static void* getDataParms(CImage& img, int& step, int& plane, int& item)
{
    item = ((int)img.Type() & PP_MASK)>>3;
    step = img.Step()/item;
    if (img.Plane())
        plane = (img.Step()*img.Height())/item;
    else
        plane = 1;
    return img.GetData();
}

void CImage::ConvertPlane_8(CImage& srcImage)
{
    if (!Plane()) memset(m_pointer, 0, Size());
    for (int c=0; c<Channels(); c++) {
        if (Plane()) {
            Ipp8u* src = (Ipp8u*)srcImage.GetDataPtr() + c;
            Ipp8u* dst = (Ipp8u*)GetDataPtrP(c);
            for (int y=0; y<Height(); y++) {
                for (int x=0; x<Width(); x++) {
                    dst[x] = src[x*Channels()];
                }
                src = (Ipp8u*)src + srcImage.Step();
                dst = (Ipp8u*)dst + StepP(c);
            }
        } else {
            Ipp8u* src = (Ipp8u*)srcImage.GetDataPtrP(c);
            Ipp8u* dst = (Ipp8u*)GetDataPtr() + c;
            for (int y=0; y<srcImage.HeightP(c); y++) {
                for (int x=0; x<srcImage.WidthP(c); x++) {
                    dst[x*Channels()] = src[x];
                }
                src = (Ipp8u*)src + srcImage.StepP(c);
                dst = (Ipp8u*)dst + Step();
            }
        }
    }
}

void CImage::ConvertPlane_16(CImage& srcImage)
{
    if (!Plane()) memset(m_pointer, 0, Size());
    for (int c=0; c<Channels(); c++) {
        if (Plane()) {
            Ipp16s* src = (Ipp16s*)srcImage.GetDataPtr();
            Ipp16s* dst = (Ipp16s*)GetDataPtrP(c);
            for (int y=0; y<HeightP(c); y++) {
                for (int x=0; x<WidthP(c); x++) {
                    dst[x] = src[x*Channels()];
                }
                src = (Ipp16s*)((Ipp8u*)src + srcImage.Step());
                dst = (Ipp16s*)((Ipp8u*)dst + StepP(c));
            }
        } else {
            Ipp16s* src = (Ipp16s*)srcImage.GetDataPtrP(c);
            Ipp16s* dst = (Ipp16s*)GetDataPtr();
            for (int y=0; y<srcImage.HeightP(c); y++) {
                for (int x=0; x<srcImage.WidthP(c); x++) {
                    dst[x*Channels()] = src[x];
                }
                src = (Ipp16s*)((Ipp8u*)src + srcImage.StepP(c));
                dst = (Ipp16s*)((Ipp8u*)dst + Step());
            }
        }
    }
}

void CImage::ConvertPlane_32(CImage& srcImage)
{
    if (!Plane()) memset(m_pointer, 0, Size());
    for (int c=0; c<Channels(); c++) {
        if (Plane()) {
            Ipp32s* src = (Ipp32s*)srcImage.GetDataPtr();
            Ipp32s* dst = (Ipp32s*)GetDataPtrP(c);
            for (int y=0; y<Height(); y++) {
                for (int x=0; x<Width(); x++) {
                    dst[x] = src[x*Channels()];
                }
                src = (Ipp32s*)((Ipp8u*)src + srcImage.Step());
                dst = (Ipp32s*)((Ipp8u*)dst + StepP(c));
            }
        } else {
            Ipp32s* src = (Ipp32s*)srcImage.GetDataPtrP(c);
            Ipp32s* dst = (Ipp32s*)GetDataPtr();
            for (int y=0; y<srcImage.HeightP(c); y++) {
                for (int x=0; x<srcImage.WidthP(c); x++) {
                    dst[x*Channels()] = src[x];
                }
                src = (Ipp32s*)((Ipp8u*)src + srcImage.StepP(c));
                dst = (Ipp32s*)((Ipp8u*)dst + Step());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

static void setReplicateBorder(
     int itemSize, int channels, 
     void* pSrc, int step, IppiSize srcRoiSize, IppiSize dstRoiSize, 
     int top, int left)
{
     if (itemSize == 1)
     {
          switch (channels)
          {
          case 1: ippiCopyReplicateBorder_8u_C1IR((Ipp8u*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 3: ippiCopyReplicateBorder_8u_C3IR((Ipp8u*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 4: ippiCopyReplicateBorder_8u_C4IR((Ipp8u*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          }
     }
     else if (itemSize == 2)
     {
          switch (channels)
          {
          case 1: ippiCopyReplicateBorder_16s_C1IR((Ipp16s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 3: ippiCopyReplicateBorder_16s_C3IR((Ipp16s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 4: ippiCopyReplicateBorder_16s_C4IR((Ipp16s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          }
     }
     else if (itemSize == 4)
     {
          switch (channels)
          {
          case 1: ippiCopyReplicateBorder_32s_C1IR((Ipp32s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 3: ippiCopyReplicateBorder_32s_C3IR((Ipp32s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          case 4: ippiCopyReplicateBorder_32s_C4IR((Ipp32s*)pSrc, step, srcRoiSize, dstRoiSize, top, left ); break;
          }
     }
}

BOOL CImage::SetReplicateBorder()
{
     if (!(m_InitFlag & initAlloc)) 
          return FALSE;
     if (!IsRoi())
          return FALSE;
     int top = m_Roi.y;
     int left = m_Roi.x;
     if (Plane())
     {
          for (int c = 0; c < Channels(); c++)
                setReplicateBorder(
                     ItemSize(), 1, 
                     GetRoiPtrP(c), StepP(c), GetRoiSizeP(c), GetSizeP(c), 
                     top, left);
     }
     else
          setReplicateBorder(
                ItemSize(), Channels(), 
                GetRoiPtr(), Step(), GetRoiSize(), GetSize(), 
                top, left);
     return TRUE;
}

BOOL CImage::SetZeroBorder()
{
     double value[4] = {0,0,0,0};
     return SetConstBorder(value);
}

BOOL CImage::SetConstBorder(const CVectorUnit& vu)
{
     double value[4];
     for (int i=0; i < Channels(); i++)
          value[i] = vu.Vector().GetDouble(i);
     return SetConstBorder(value);
}

BOOL CImage::SetConstBorder(const double value[4])
{
     if (!(m_InitFlag & initAlloc)) 
          return FALSE;
     if (!IsRoi())
          return FALSE;
     IppiSize size = GetSize();
     for (int y=0; y < GetSize().height; y++)
     {
          if ((m_Roi.y <= y) && (y < m_Roi.y + m_Roi.height))
                continue;
          for (int x=0; x < GetSize().width; x++)
          {
                if ((m_Roi.x <= x) && (x < m_Roi.x + m_Roi.width))
                     continue;
                for (int c=0; c < Channels(); c++)
                     Set(x, y, c, value[c]);
          }
     }
     return TRUE;
}


///////////////////////////////////////////////////////////////////////////////


static int AlphaTypeNum = 12;
static CMyString AlphaTypeStr[] = {
    "Over",
    "In",
    "Out",
    "ATop",
    "Xor",
    "Plus",
    "OverPremul",
    "InPremul",
    "OutPremul",
    "ATopPremul",
    "XorPremul",
    "PlusPremul",
};
static IppiAlphaType AlphaTypeVal[] = {
    ippAlphaOver,
    ippAlphaIn,
    ippAlphaOut,
    ippAlphaATop,
    ippAlphaXor,
    ippAlphaPlus,
    ippAlphaOverPremul,
    ippAlphaInPremul,
    ippAlphaOutPremul,
    ippAlphaATopPremul,
    ippAlphaXorPremul,
    ippAlphaPlusPremul
};
CMyString& operator << (CMyString& str, IppiAlphaType val) {
    str += ENUM_STRING(AlphaType,val);
    return str;
}

static int MaskSizeNum = 6;
static CMyString MaskSizeStr[] = {
    "1x3", "3x1", "3x3", "1x5", "5x1", "5x5", };
static IppiMaskSize MaskSizeVal[] = {
    ippMskSize1x3, ippMskSize3x1, ippMskSize3x3, ippMskSize1x5, ippMskSize5x1, ippMskSize5x5};

CMyString& operator << (CMyString& str, IppiMaskSize val) {
    str += ENUM_STRING(MaskSize,val);
    return str;
}

CMyString& operator << (CMyString& str, IppiSize val) {
    CString buffer;
    buffer.Format("%dx%d", val.width, val.height);
    str += buffer;
    return str;
}

CMyString& operator << (CMyString& str, IppiPoint val) {
    CString buffer;
    buffer.Format("%d,%d", val.x, val.y);
    str += buffer;
    return str;
}

CMyString& operator << (CMyString& str, IppiRect val) {
    CString buffer;
    buffer.Format("(%d,%d)%dx%d", val.x, val.y, val.width, val.height);
    str += buffer;
    return str;
}
