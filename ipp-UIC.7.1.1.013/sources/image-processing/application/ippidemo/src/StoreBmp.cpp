/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreBmp.cpp: implementation of the CStoreBmp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Image.h"
#include "StoreBmp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CStoreBmp::Error(CString message)
{
    CString oper = m_bSave ? "save" : "open";
    AfxMessageBox(
        (m_bSave ? "Can't save " : "Can't open ")
        + m_pFile->GetFilePath() + ":\n"
        + message);
    return FALSE;
}

BOOL CStoreBmp::Error(CFileException* exc)
{
    char buffer[256];
    exc->GetErrorMessage(buffer, 256);
    return Error(buffer);
}

BOOL CStoreBmp::Save(CFile* pFile)
{
    m_pFile = pFile;
    m_bSave = TRUE;

    try {
        pFile->Write(GetFileHeader(m_pImage), FileHeaderSize());
        pFile->Write(GetInfoHeader(m_pImage), InfoHeaderSize());
        if (GetPaletteSize(m_pImage)) {
            pFile->Write(GetGrayPalette(m_pImage), GetPaletteSize(m_pImage));
        }
        pFile->Seek(m_FileHeader.bfOffBits, CFile::begin);
        if (m_pImage->Plane())
        {
            for (int c=0; c<m_pImage->Channels(); c++)
                pFile->Write(m_pImage->GetDataPtrP(c), m_pImage->SizeP(c));
        }
        else
        {
            pFile->Write(m_pImage->GetData(), m_pImage->Size());
        }
    } catch (CFileException* exc) {
        return Error(exc);
    }
    return TRUE;
}

BITMAPFILEHEADER* CStoreBmp::GetFileHeader(CImage* pImage)
{
    m_FileHeader.bfType = 0x4d42;
    m_FileHeader.bfReserved1 = 0;
    m_FileHeader.bfReserved2 = 0;
    m_FileHeader.bfOffBits =
        (FileHeaderSize() + InfoHeaderSize()
        + GetPaletteSize(pImage) + 3) & 0xFFFFFFFC;
    m_FileHeader.bfSize = m_FileHeader.bfOffBits + m_pImage->Size();
    return &m_FileHeader;
}

BITMAPINFOHEADER* CStoreBmp::GetInfoHeader(CImage* pImage)
{
    m_InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_InfoHeader.biWidth = pImage->Width();
    m_InfoHeader.biHeight = pImage->Height();
    m_InfoHeader.biPlanes = pImage->Plane() ? pImage->Channels() : 1;
    m_InfoHeader.biBitCount = pImage->Plane()   ? pImage->Depth() :
                              pImage->Complex() ? pImage->Depth()*2 :
                              pImage->Depth()*pImage->Channels();
    m_InfoHeader.biSizeImage = pImage->Size();
    m_InfoHeader.biXPelsPerMeter = 0;
    m_InfoHeader.biYPelsPerMeter = 0;
    m_InfoHeader.biClrUsed = 0;
    m_InfoHeader.biClrImportant = 0;
    m_InfoHeader.biCompression = Compression(pImage->GetHeader());
    return &m_InfoHeader;
}

DWORD CStoreBmp::Compression(ImgHeader header)
{
    int depth = header.type & PP_MASK;
    BOOL isFloat = header.type & PP_FLOAT;

    if (   (depth == 8)
        && (!header.plane || header.channels == 1)) 
        return BI_RGB;
    else if (header.IsSize422()) return isFloat ? BI_IPP_422F : BI_IPP_422;
    else if (header.IsSize420()) return isFloat ? BI_IPP_420F : BI_IPP_420;
    else if (header.IsSize411()) return isFloat ? BI_IPP_411F : BI_IPP_411;
    else if (isFloat)
        return BI_IPP_32F;
    else if (depth == 8)
        return BI_IPP_8U;
    else if (depth == 16)
        return BI_IPP_16U;
    else if (depth == 32)
        return BI_IPP_32S;
    else
        return -1;
}

int CStoreBmp::GetPaletteNum(CImage* pImage)
{
    if ((pImage->Depth() == 8)
        && (pImage->Channels() == 1)
        && !pImage->Complex()) return 256;
    else return 0;
}

RGBQUAD* CStoreBmp::GetGrayPalette(CImage* pImage)
{
     for (int i=0; i<256; i++) {
          m_Palette[i].rgbBlue = i;
          m_Palette[i].rgbGreen = i;
          m_Palette[i].rgbRed = i;
          m_Palette[i].rgbReserved = 0;
     }
     return m_Palette;
}

/////////////////////////////////////////////////////////////////////////

BOOL CStoreBmp::Load(CFile* pFile)
{
    m_pFile = pFile;
    m_bSave = FALSE;

    try {
        pFile->Read(&m_FileHeader,FileHeaderSize());
        if (!ValidFileHeader()) return FALSE;
        pFile->Read(&m_InfoHeader,InfoHeaderSize());
        if (GetPaletteSize()) {
            pFile->Read(m_Palette,GetPaletteSize());
        }
        if (!SetImageHeader()) return FALSE;
        m_pImage->AllocateData();

        pFile->Seek(m_FileHeader.bfOffBits, CFile::begin);
        if (IsCompressed()) 
        {
            Ipp8u* buffer = (Ipp8u*)malloc(GetDataSize());
            pFile->Read(buffer, GetDataSize());
            SetImageData(buffer);
            free(buffer);
        } 
        else if (m_pImage->Plane())
        {
            for (int c=0; c<m_pImage->Channels(); c++)
                pFile->Read(m_pImage->GetDataPtrP(c),m_pImage->SizeP(c));
        }
        else
        {
            pFile->Read(m_pImage->GetData(),m_pImage->Size());
        }
    } catch (CFileException* exc) {
        return Error(exc);
    }
    return TRUE;
}

BOOL CStoreBmp::ValidFileHeader()
{
    if (m_FileHeader.bfType != 0x4d42)
        return Error("Bitmap files are valid only");
    return TRUE;
}

BOOL CStoreBmp::SetImageHeader()
{
    if (!ValidCompression()) return FALSE;
    ImgHeader header;
    if (!SetImageWidth(header.width)) return FALSE;
    if (!SetImageHeight(header.height)) return FALSE;
    if (!SetImageType(header.type)) return FALSE;
    if (!SetImagePlane(header.plane)) return FALSE;
    if (!SetImageChannels(header.channels)) return FALSE;
    SetPlaneSizes(header);
    m_pImage->SetHeader(header);
    return TRUE;
}

BOOL CStoreBmp::ValidCompression()
{
    DWORD comprssn = m_InfoHeader.biCompression;
    if (comprssn == BI_RGB ||
        comprssn == BI_IPP_422 ||
        comprssn == BI_IPP_420 ||
        comprssn == BI_IPP_411 ||
        comprssn == BI_IPP_422F ||
        comprssn == BI_IPP_420F ||
        comprssn == BI_IPP_411F ||
        comprssn == BI_IPP_8U ||
        comprssn == BI_IPP_16U ||
        comprssn == BI_IPP_32S ||
        comprssn == BI_IPP_32F) return TRUE;
    else
        return Error("Compression is not valid");
}

BOOL CStoreBmp::SetImageWidth(int& width)
{
    width = m_InfoHeader.biWidth;
    if (width <= 0)
        return Error("Bitmap header is corrupted: biWidth is invalid");
    return TRUE;
}

BOOL CStoreBmp::SetImageHeight(int& height)
{
    height = m_InfoHeader.biHeight;
    if (height <= 0)
        return Error("Bitmap header is corrupted: biHeight is invalid");
    return TRUE;
}

void CStoreBmp::SetPlaneSizes(ImgHeader& header)
{
    switch (m_InfoHeader.biCompression)
    {
        case BI_IPP_422:
        case BI_IPP_422F:
            header.SetSize422(header.width, header.height);
            break;
        case BI_IPP_420:
        case BI_IPP_420F:
            header.SetSize420(header.width, header.height);
            break;
        case BI_IPP_411:
        case BI_IPP_411F:
            header.SetSize411(header.width, header.height);
            break;
        default:
            header.SetSize444(header.width, header.height);
            break;
    }
}

BOOL CStoreBmp::SetImageType(ppType& type)
{
    BOOL plane = m_InfoHeader.biPlanes > 1;
    int bitCount = m_InfoHeader.biBitCount;
    switch (m_InfoHeader.biCompression) {
    case BI_RGB:
        if (plane)
            break;
        switch (bitCount) {
        case 1: case 4: case 8: case 16: case 24: case 32:
            type = pp8u; return TRUE;
        }
        break;
    case BI_IPP_422:
    case BI_IPP_420:
    case BI_IPP_411:
        switch (bitCount) {
        case 8:
            type = pp8u; return TRUE;
        case 16:
            type = pp16u; return TRUE;
        case 32:
            type = pp32s; return TRUE;
        }
        break;
    case BI_IPP_422F:
    case BI_IPP_420F:
    case BI_IPP_411F:
        switch (bitCount) {
        case 32:
            type = pp32f; return TRUE;
        }
        break;
    case BI_IPP_8U:
        type = pp8u;
        switch (bitCount) {
        case 8: 
            return TRUE;
        }
        break;
    case BI_IPP_16U:
        type = pp16u;
        switch (bitCount) {
        case 16: 
            return TRUE;
        case 32: case 48: case 64:
            if (plane)
                break;
            return TRUE;
        }
        break;
    case BI_IPP_32S:
        type = pp32s;
        switch (bitCount) {
        case 32: 
            return TRUE;
        case 64: case 96: case 128:
            if (plane)
                break;
            return TRUE;
        }
        break;
    case BI_IPP_32F:
        type = pp32f;
        switch (bitCount) {
        case 32: 
            return TRUE;
        case 64: case 96: case 128:
            if (plane)
                break;
            return TRUE;
        }
        break;
    }
    return Error("Bitmap header is corrupted: biBitCount is invalid");
}

BOOL CStoreBmp::SetImagePlane(BOOL& plane)
{
    switch (m_InfoHeader.biCompression) {
    case BI_RGB:
        switch (m_InfoHeader.biPlanes) {
        case 1:
            plane = FALSE; return TRUE;
        }
        break;
    default:
        switch (m_InfoHeader.biPlanes) {
        case 1:
            plane = FALSE; return TRUE;
        case 2: case 3: case 4:
            plane = TRUE; return TRUE;
        }
        break;
    }
    return Error("Bitmap header is corrupted: biPlanes is invalid");
}

BOOL CStoreBmp::SetImageChannels(int& channels)
{
    if (m_InfoHeader.biPlanes > 1)
    {
        channels = m_InfoHeader.biPlanes; 
        return TRUE;
    }
    switch (m_InfoHeader.biCompression) {
    case BI_RGB:
        switch (m_InfoHeader.biBitCount) {
        case 1: case 4: case 8:
            channels = IsGrayPalette() ? 1 : 3;
            return TRUE;
        case 16:
            channels = 2; return TRUE;
        case 24:
            channels = 3; return TRUE;
        case 32:
            channels = 4; return TRUE;
        }
        return Error("Bitmap header is corrupted: biBitCount is invalid");
    case BI_IPP_16U:
        switch (m_InfoHeader.biBitCount) 
        {
        case 16:  
            channels = 1; return TRUE;
        case 32: 
            channels = 2; return TRUE;
        case 48: 
            channels = 3; return TRUE;
        case 64: 
            channels = 4; return TRUE;
        }
        break;
    case BI_IPP_32S:
    case BI_IPP_32F:
        switch (m_InfoHeader.biBitCount) 
        {
        case 32:
            channels = 1; return TRUE;
        case 64:
            channels = 2; return TRUE;
        case 96:
            channels = 3; return TRUE;
        case 128:
            channels = 4; return TRUE;
        }
        break;
    }
    return Error("Bitmap header is corrupted: biBitCount or biPlanes is invalid");
}

int CStoreBmp::GetPaletteNum()
{
    if (m_InfoHeader.biCompression != BI_RGB) return 0;
    if (m_InfoHeader.biBitCount > 8) return 0;
    int num = 1 << m_InfoHeader.biBitCount;
    if (m_InfoHeader.biClrUsed == 0) return num;
    if ((int)m_InfoHeader.biClrUsed > num) return num;
    return m_InfoHeader.biClrUsed;
}

BOOL CStoreBmp::IsGrayPalette()
{
    for (int i=0; i<GetPaletteNum(); i++) {
        if (m_Palette[i].rgbBlue != m_Palette[i].rgbGreen)
            return FALSE;
        if (m_Palette[i].rgbBlue != m_Palette[i].rgbRed)
            return FALSE;
    }
    return TRUE;
}

int CStoreBmp::GetDataSize()
{
    return GetDataStep()*m_InfoHeader.biHeight*m_InfoHeader.biPlanes;
}

int CStoreBmp::GetDataStep()
{
    return (m_InfoHeader.biBitCount*m_InfoHeader.biWidth+31)/32*4;
}

BOOL CStoreBmp::IsCompressed()
{
    return (m_InfoHeader.biCompression == BI_RGB) &&
             (m_InfoHeader.biBitCount <= 8);
}

BOOL CStoreBmp::SetImageData(Ipp8u* buffer)
{
    switch (m_InfoHeader.biBitCount) {
    case 1: return SetImageData_1(buffer);
    case 4: return SetImageData_4(buffer);
    case 8: return SetImageData_8(buffer);
    }
    return FALSE;
}

BOOL CStoreBmp::SetImageData_8(Ipp8u* src)
{
    int srcStep = GetDataStep();
    int dstStep = m_pImage->Step();
    Ipp8u* dst = (Ipp8u*)m_pImage->GetData();

    for (int y=0; y < m_pImage->Height(); y++) {
        for (int x=0; x < m_pImage->Width(); x++) {
            SetPixel(src[x],dst,x);
        }
        src += srcStep;
        dst += dstStep;
    }
    return TRUE;
}

BOOL CStoreBmp::SetImageData_4(Ipp8u* src)
{
    int srcStep = GetDataStep();
    int dstStep = m_pImage->Step();
    Ipp8u* dst = (Ipp8u*)m_pImage->GetData();

    for (int y=0; y < m_pImage->Height(); y++) {
        int x = 0;
        int i;
        for (i=0; i < (m_pImage->Width()>>1); i++) {
            SetPixel((src[i] & 0xF0) >> 4, dst, x++);
            SetPixel(src[i] & 0x0F, dst, x++);
        }
        if (x < m_pImage->Width()) {
            SetPixel((src[i] & 0xF0) >> 4, dst, x++);
        }
        src += srcStep;
        dst += dstStep;
    }
    return TRUE;
}

BOOL CStoreBmp::SetImageData_1(Ipp8u* src)
{
    int srcStep = GetDataStep();
    int dstStep = m_pImage->Step();
    Ipp8u* dst = (Ipp8u*)m_pImage->GetData();

    for (int y=0; y < m_pImage->Height(); y++) {
        int x = 0;
        int i;
        for (i=0; i < (m_pImage->Width()>>3); i++) {
            Ipp8u byte = src[i];
            for (int j=0; j<8; j++) {
                SetPixel(byte & 0x80, dst, x++);
                byte <<= 1;
            }
        }
        if (x < m_pImage->Width()) {
            Ipp8u byte = src[i];
            while (x < m_pImage->Width()) {
                SetPixel(byte & 1, dst, x++);
                byte >>= 1;
            }
        }
        src += srcStep;
        dst += dstStep;
    }
    return TRUE;
}

void CStoreBmp::SetPixel(Ipp8u src, Ipp8u* pDst, int x)
{
    if (src >= GetPaletteNum()) src = GetPaletteNum() - 1;
    RGBQUAD pal = m_Palette[src];
    if (m_pImage->Channels() == 1) {
        pDst[x] = pal.rgbBlue;
    } else {
        pDst[3*x  ] = pal.rgbBlue;
        pDst[3*x+1] = pal.rgbGreen;
        pDst[3*x+2] = pal.rgbRed;
    }
}


