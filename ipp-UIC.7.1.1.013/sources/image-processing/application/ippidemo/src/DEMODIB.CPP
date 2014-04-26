/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DemoDib.cpp: implementation of the CDemoDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "image.h"
#include "DemoDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDemoDib::CDemoDib()
{
     m_pBits = NULL;
     m_hBmp = NULL;
     memset(m_BmpInfoPalette,0,
          sizeof(BITMAPINFO)+255*sizeof(RGBQUAD));
     SetGrayPalette();
}

CDemoDib::~CDemoDib()
{
    DeleteDibSection();
}

int CDemoDib::GetDibWidth(const CImage* image)
{
    int coi = image->GetCoi();
    if (image->Plane() && coi >= 0)
        return image->WidthP(coi);
    else
        return image->Width();
}

int CDemoDib::GetDibHeight(const CImage* image)
{
    int coi = image->GetCoi();
    if (image->Plane() && coi >= 0)
        return image->HeightP(coi);
    else
        return image->Height();
}

BOOL CDemoDib::IsNewImageSize(const CImage* image)
{
    int width  = GetDibWidth (image);
    int height = GetDibHeight(image);
    BITMAPINFOHEADER* bi = GetBmpInfoHeader() ;
    if (bi->biWidth  != width ) return TRUE;
    if (bi->biHeight != height) return TRUE;
    return FALSE;
}

void CDemoDib::CreateDibSection(CDC* pDC, CImage* pImage)
{
     SetBmpInfo(pImage);
     m_hBmp = ::CreateDIBSection(
          pDC->GetSafeHdc(), GetBmpInfo(),
          DIB_RGB_COLORS, (void**)(&m_pBits), NULL, 0);
}

void CDemoDib::DeleteDibSection()
{
     if (m_hBmp)
          ::DeleteObject(m_hBmp);
}

void CDemoDib::SetBmpInfo(CImage* pImage, int bOne)
{
     BITMAPINFOHEADER* bi = GetBmpInfoHeader();

     int bitCount = 24;
     if (!pImage->Compressed()) {
         if ((pImage->Channels() == 1) && bOne) bitCount = 8;
         if (pImage->Channels() > 3) bitCount = 32;
     }

     bi->biSize = sizeof(BITMAPINFOHEADER);
     bi->biWidth = GetDibWidth(pImage);
     bi->biHeight = GetDibHeight(pImage);
     bi->biPlanes = 1;
     bi->biCompression = BI_RGB;
     bi->biBitCount = bitCount;
     bi->biSizeImage = 0;
     bi->biXPelsPerMeter = 0;
     bi->biYPelsPerMeter = 0;
     bi->biClrUsed = bitCount == 8 ? 256 : 0;
     bi->biClrImportant = 0;
}

void CDemoDib::SetDibSection(CImage* image)
{
    ASSERT(m_hBmp);

    ConvertToRGB(image,(Ipp8u*)m_pBits,GetBmpStep());
}

void CDemoDib::ConvertToRGB(CImage* image, Ipp8u* dst, int dstStep, BOOL bOne)
{
    if (image->Compressed()) {
        SetBits_Compressed(image, dst, dstStep);
    } else if (image->Plane() && image->Channels() > 1) {
        SetBits_Plane(image, dst, dstStep);
    } else {
        switch (image->Channels()) {
        case 1:
            if (image->Type() & PP_CPLX)
                SetBits_Pixel_C2(image, dst, dstStep);
            else
                SetBits_Pixel_C1(image, dst, dstStep, bOne);
            break;
        case 2: SetBits_Pixel_C2(image, dst, dstStep); break;
        case 3: SetBits_Pixel_C3(image, dst, dstStep); break;
        case 4: SetBits_Pixel_C4(image, dst, dstStep); break;
        }
    }
}

static void GetFloatLimits(CImage* image, Ipp32f& vmin, Ipp32f& vmax)
{
    if (image->Float()) {
        image->GetFloatLimits(vmin, vmax);
        if (vmax - vmin < IPP_MINABS_32F) {
            if ((vmax + vmin < 2*IPP_MINABS_32F) && (vmax + vmin > -2*IPP_MINABS_32F)) {
                vmin = 0;
                vmax = 1;
            } else {
                vmin = 0;
            }
        }
    }
}

void CDemoDib::SetBits_Pixel_C1(CImage* image, Ipp8u* dst, int dstStep, BOOL bOne)
{
    void* src = image->GetDataPtr();
    int srcStep = image->Step();
    IppiSize size = {image->Width(), image->Height()};
    Ipp8u* tmp = NULL;
    if (image->Depth() != 8) {
        tmp = new Ipp8u[size.width*size.height];
        int tmpStep = size.width;
        switch (image->Type()) {
        case pp16u:
        case pp16s:
            ippiScale_16u8u_C1R((Ipp16u*)src, srcStep, tmp, tmpStep,
                size, ippAlgHintFast);
            break;
        case pp32s:
        case pp32u:
            ippiScale_32s8u_C1R((Ipp32s*)src, srcStep, tmp, tmpStep,
            size, ippAlgHintFast);
            break;
        case pp32f:
            {
                Ipp32f vmin, vmax;
                GetFloatLimits(image, vmin, vmax);
                ippiScale_32f8u_C1R((Ipp32f*)src, srcStep, tmp, tmpStep,
                size, vmin, vmax);
            }
            break;
        }
        src = tmp;
        srcStep = tmpStep;
    }
    if (bOne) {
        ippiCopy_8u_C1R((Ipp8u*)src,srcStep,dst,dstStep,size);
    } else {
        for (int c=0; c<3; c++) {
            ippiCopy_8u_C1C3R((Ipp8u*)src,srcStep,dst,dstStep,size);
            dst++;
        }
    }
    if (tmp) delete[] tmp;
}

void CDemoDib::SetBits_Pixel_C2(CImage* image, Ipp8u* dst, int dstStep)
{
    int type = (int)image->Type();
    if (type & PP_CPLX) type &= ~PP_CPLX;
    Ipp8u* src = (Ipp8u*)image->GetDataPtr();
    int srcStep = image->Step();
    IppiSize size = {2*image->Width(), image->Height()};
    Ipp8u* tmp = NULL;
    if (image->Depth() != 8) {
        tmp = new Ipp8u[size.width*size.height];
        int tmpStep = size.width;
        switch (type) {
        case pp16u:
        case pp16s:
            ippiScale_16u8u_C1R((Ipp16u*)src, srcStep, tmp, tmpStep,
                size, ippAlgHintFast);
            break;
        case pp32s:
        case pp32u:
            ippiScale_32s8u_C1R((Ipp32s*)src, srcStep, tmp, tmpStep,
            size, ippAlgHintFast);
            break;
        case pp32f:
            {
                float vmin, vmax;
                GetFloatLimits(image, vmin, vmax);
                ippiScale_32f8u_C1R((Ipp32f*)src, srcStep, tmp, tmpStep,
                    size, vmin, vmax);
            }
            break;
        }
        src = tmp;
        srcStep = tmpStep;
    }
    for (int y=0; y < image->Height(); y++) {
        for (int x=0; x<image->Width(); x++) {
            dst[3*x     ] = src[2*x];
            dst[3*x + 1] = src[2*x + 1];
            dst[3*x + 2] = 0;
        }
        src += srcStep;
        dst += dstStep;
    }
    if (tmp) delete[] tmp;
}

void CDemoDib::SetBits_Pixel_C3(CImage* image, Ipp8u* dst, int dstStep)
{
    void* src = image->GetDataPtr();
    IppiSize size = {image->Width(), image->Height()};
    switch (image->Type()) {
    case pp8u:
    case pp8s:
        ippiCopy_8u_C3R((Ipp8u*)src, image->Step(), dst, dstStep,
            size);
        break;
    case pp16u:
    case pp16s:
        ippiScale_16u8u_C3R((Ipp16u*)src, image->Step(), dst, dstStep,
            size, ippAlgHintFast);
        break;
    case pp32s:
    case pp32u:
        ippiScale_32s8u_C3R((Ipp32s*)src, image->Step(), dst, dstStep,
        size, ippAlgHintFast);
        break;
    case pp32f:
        {
            Ipp32f vmin, vmax;
            GetFloatLimits(image, vmin, vmax);
            ippiScale_32f8u_C3R((Ipp32f*)src, image->Step(), dst, dstStep,
                size, vmin, vmax);
        }
        break;
    }
}

void CDemoDib::SetBits_Pixel_C4(CImage* image, Ipp8u* dst, int dstStep)
{
    void* src = image->GetDataPtr();
    IppiSize size = {image->Width(), image->Height()};
    switch (image->Type()) {
    case pp8u:
    case pp8s:
        ippiCopy_8u_C4R((Ipp8u*)src, image->Step(), dst, dstStep,
            size);
        break;
    case pp16u:
    case pp16s:
        ippiScale_16u8u_C4R((Ipp16u*)src, image->Step(), dst, dstStep,
            size, ippAlgHintFast);
        break;
    case pp32s:
    case pp32u:
        ippiScale_32s8u_C4R((Ipp32s*)src, image->Step(), dst, dstStep,
        size, ippAlgHintFast);
        break;
    case pp32f:
        {
            Ipp32f vmin = 0, vmax = 0;
            GetFloatLimits(image, vmin, vmax);
            ippiScale_32f8u_C4R((Ipp32f*)src, image->Step(), dst, dstStep,
                size, vmin, vmax);
        }
        break;
    }
}

void CDemoDib::SetBits_Plane(CImage* image, Ipp8u* dst, int dstStep)
{
    int c0 = 0;
    int c1 = image->Channels();
    int coi = image->GetCoi();
    if (image->Channels() < 3 || coi >= 0 || !image->EqualPlaneSize())
//        memset(dst, 0, GetDibHeight(image)*dstStep);
        memset(dst, 0, image->Height()*dstStep);
    if (coi >= 0) {
        if (coi == 3) {
            c0 = 0;
            c1 = 3;
        } else {
            dst += coi;
            c0 = coi;
            c1 = coi + 1;
        }
    }
    for (int i=c0; i<c1; i++) {
        int c = (coi >= 0) ? coi : i;
        Ipp8u* src = (Ipp8u*)image->GetDataPtrP(c);
        Ipp8u* temp;
        int srcStep = image->StepP(c);
        IppiSize size = {image->WidthP(c), image->HeightP(c)};
        if (image->Depth() == 8)
             temp = src;
        else
             temp = new Ipp8u[srcStep*size.height];
        switch (image->Type()) {
        case pp8u:
        case pp8s:
            break;
        case pp16u:
        case pp16s:
            ippiScale_16u8u_C1R((Ipp16u*)src, srcStep,
                temp, srcStep, size, ippAlgHintFast);
            break;
        case pp32s:
        case pp32u:
            ippiScale_32s8u_C1R((Ipp32s*)src, srcStep,
                temp, srcStep, size, ippAlgHintFast);
            break;
        case pp32f:
            {
                Ipp32f vmin, vmax;
                GetFloatLimits(image, vmin, vmax);
                ippiScale_32f8u_C1R((Ipp32f*)src, srcStep,
                    temp, srcStep, size, vmin, vmax);
            }
            break;
        }
        switch (image->Channels()) {
        case 2:
        case 3:
             ippiCopy_8u_C1C3R(temp, srcStep, dst, dstStep, size);
             break;
        case 4:
             ippiCopy_8u_C1C4R(temp, srcStep, dst, dstStep, size);
             break;
        }
        if (temp != src) delete[] temp;
        dst += 1;
    }
}

void CDemoDib::SetBits_Compressed(CImage* image, Ipp8u* dst, int dstStep)
{
    if (image->Type() != pp16u) return;
    Ipp16u* src = (Ipp16u*)image->GetDataPtr();
    int x, y;
    switch (image->Compressed()) {
    case RGB444:
        for (y = 0; y < image->Height(); y++) {
            for (x = 0; x < image->Width(); x++) {
                dst[x*3 + 2] = (Ipp8u)((src[x] >> 8) << 4);
                dst[x*3 + 1] = (Ipp8u)((src[x] >> 4) << 4);
                dst[x*3     ] = (Ipp8u)( src[x]         << 4);
            }
            src = (Ipp16u*)((Ipp8u*)src + image->Step());
            dst += dstStep;
        }
        break;
    case RGB555:
        for (y = 0; y < image->Height(); y++) {
            for (x = 0; x < image->Width(); x++) {
                dst[x*3 + 2] = (Ipp8u)((src[x] >> 10) << 3);
                dst[x*3 + 1] = (Ipp8u)((src[x] >>  5) << 3);
                dst[x*3     ] = (Ipp8u)( src[x]          << 3);
            }
            src = (Ipp16u*)((Ipp8u*)src + image->Step());
            dst += dstStep;
        }
        break;
    case RGB565:
        for (y = 0; y < image->Height(); y++) {
            for (x = 0; x < image->Width(); x++) {
                dst[x*3 + 2] = (Ipp8u)((src[x] >> 11) << 3);
                dst[x*3 + 1] = (Ipp8u)((src[x] >>  5) << 2);
                dst[x*3     ] = (Ipp8u)( src[x]          << 3);
            }
            src = (Ipp16u*)((Ipp8u*)src + image->Step());
            dst += dstStep;
        }
        break;
    }
}

int CDemoDib::PaletteNum()
{
     BITMAPINFOHEADER* infoHeader = GetBmpInfoHeader();
     int num = (int)infoHeader->biClrUsed;
     if (num == 0 && infoHeader->biBitCount < 16)
          num = 1 << infoHeader->biBitCount;
     return num;
}

void CDemoDib::SetGrayPalette()
{
     int num;
     RGBQUAD* palette = GetBmpPalette(&num);
     for (int i=0; i<num; i++) {
          palette[i].rgbBlue = i;
          palette[i].rgbGreen = i;
          palette[i].rgbRed = i;
          palette[i].rgbReserved = 0;
     }
}
