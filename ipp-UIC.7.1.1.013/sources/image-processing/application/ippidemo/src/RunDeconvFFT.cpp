/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDeconvFFT.cpp: implementation of the CRunDeconvFFT class.
// CRunDeconvFFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunDeconvFFT.h"
#include "ParmDeconvFFTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDeconvFFT::CRunDeconvFFT()
{
    pState = NULL;
    kernelSize = 0;
    order = 0;
    threshold = 0.001f;
    pKernel = NULL;
}

CRunDeconvFFT::~CRunDeconvFFT()
{

}

BOOL CRunDeconvFFT::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;
   
    m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;

    m_initName = "ippiDeconvFFTInitAlloc_" + m_Func.TypeName() + "_" + m_Func.DescrName();
    m_freeName = "ippiDeconvFFTFree_" + m_Func.TypeName() + "_" + m_Func.DescrName();
    return TRUE;
}

void CRunDeconvFFT::Close()
{
}

CMyString CRunDeconvFFT::GetVectorName(int vecPos)
{
    if (vecPos == VEC_SRC2) 
        return _T("Kernel");
    else
        return CippiRun::GetVectorName(vecPos);
}


BOOL CRunDeconvFFT::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
    if (vecPos == VEC_SRC2) 
    {
        pickID = IDC_CURSOR_PK;
        grabID = IDC_CURSOR_K;
        return TRUE;
    } 
    else
        return CippiRun::GetPickID(vecPos, pickID, grabID);
}

ImgHeader CRunDeconvFFT::GetNewDstHeader()
{
    return m_pDocSrc->GetImage()->GetHeader();
}

BOOL CRunDeconvFFT::PrepareSrc()
{
    CImage* kernelImage = m_pDocSrc2->GetImage();
    IppiRect kernelRoi = kernelImage->GetActualRoi();
    BOOL bRoi = kernelImage->IsRoi();
    if (kernelRoi.width != kernelRoi.height) {
        if (kernelRoi.width > kernelRoi.height)
            kernelRoi.width = kernelRoi.height;
        else
            kernelRoi.height = kernelRoi.width;
        CString message =
            (bRoi ? "ROI will be reduced to square in " :
                    "Square ROI will be created in ")
            + m_pDocSrc2->GetTitle() + " image";
        if (DEMO_APP->MessageBox(message, MB_OKCANCEL) == IDOK)
            kernelImage->SetRoi(&kernelRoi);
        else
            return FALSE;
    }
    return TRUE;
}

void CRunDeconvFFT::PrepareParameters()
{
    CippiRun::PrepareParameters();

    roiSize.width  = -1;
    roiSize.height = -1;
    IntersectRoi((CippiDemoDoc*)m_pDocSrc , roiSize);
    IntersectRoi((CippiDemoDoc*)m_pDocDst , roiSize);
}

CParamDlg* CRunDeconvFFT::CreateDlg() { return new CParmDeconvFFTDlg;}


static int getOrder(CImage* pImg, int kernelSize)
{
    IppiSize imgSize = pImg->GetActualSize();
    int theSize = MAX(imgSize.width, imgSize.height) + kernelSize;
    int order, size;
    for (order = 0, size = 1; order < 32; order++, size<<=1)
        if (theSize <= size)
            return order;
    return 31;
}

int CRunDeconvFFT::GetKernelSize(CImage* pImg)
{
    IppiSize size = pImg->GetActualSize();
    return MIN(size.width, size.height);
}

Ipp32f* CRunDeconvFFT::createKernel(CImage* pImg)
{
    IppiSize size = pImg->GetActualSize();
    int channels = pImg->Channels();
    int kernelSize = GetKernelSize(pImg);
    
    if ((kernelSize == size.width) && (kernelSize*4*channels == pImg->Step()))
        return (Ipp32f*)pImg->GetRoiPtr();
    
    Ipp32f* pKernel = ippsMalloc_32f(kernelSize*kernelSize*channels);
    Ipp8u*  pSrc = (Ipp8u*)pImg->GetRoiPtr();
    Ipp32f* pDst = pKernel;
    for (int y=0; y < size.height; y++)
    {
        ippsCopy_32f((Ipp32f*)pSrc, pDst, kernelSize*channels);
        pSrc += pImg->Step();
        pDst += kernelSize*channels;
    }
    return pKernel;
}

void CRunDeconvFFT::deleteKernel(Ipp32f* pKernel, CImage* pImg)
{
    if (pKernel == (Ipp32f*)pImg->GetRoiPtr())
        return;
    ippsFree(pKernel);
}



void CRunDeconvFFT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmDeconvFFTDlg* pDlg = (CParmDeconvFFTDlg*)parmDlg;
    if (save) {
        kernelSize = pDlg->kernelSize;
        order      = pDlg->order     ;
        threshold  = pDlg->threshold ;
    } else {
        kernelSize = GetKernelSize(m_pDocSrc2->GetImage());
        order = getOrder(m_pDocSrc->GetImage(), kernelSize);
        pDlg->kernelSize = kernelSize;
        pDlg->order      = order     ;
        pDlg->threshold  = threshold ;
    }
}

BOOL CRunDeconvFFT::BeforeCall()
{
    pKernel = createKernel(m_pDocSrc2->GetImage());
    IppStatus status = CallInit();
    IppErrorMessage(m_initName, status);
    if (status < 0) return FALSE;
    return TRUE;
}

BOOL CRunDeconvFFT::AfterCall(BOOL bOK)
{
    deleteKernel(pKernel, m_pDocSrc2->GetImage());
    IppStatus status = CallFree(); pState = NULL;
    IppErrorMessage(m_freeName, status);
    return TRUE;
}

IppStatus CRunDeconvFFT::CallInit()
{
    MATE_CALL(m_initName, ippiDeconvFFTInitAlloc_32f_C1R, (
        (IppiDeconvFFTState_32f_C1R**)&pState, (Ipp32f*)pKernel, kernelSize, order, threshold))
    MATE_CALL(m_initName, ippiDeconvFFTInitAlloc_32f_C3R, (
        (IppiDeconvFFTState_32f_C3R**)&pState, (Ipp32f*)pKernel, kernelSize, order, threshold))
    return stsNoFunction;
}

IppStatus CRunDeconvFFT::CallFree()
{
    MATE_CALL(m_freeName, ippiDeconvFFTFree_32f_C1R, ((IppiDeconvFFTState_32f_C1R*)pState))
    MATE_CALL(m_freeName, ippiDeconvFFTFree_32f_C3R, ((IppiDeconvFFTState_32f_C3R*)pState))
    return stsNoFunction;
}

IppStatus CRunDeconvFFT::CallIppFunction()
{
    FUNC_CALL ( ippiDeconvFFT_32f_C1R, ((Ipp32f*)pSrc, srcStep, 
        (Ipp32f*)pDst,  dstStep, roiSize, (IppiDeconvFFTState_32f_C1R*)pState))
    FUNC_CALL ( ippiDeconvFFT_32f_C3R, ((Ipp32f*)pSrc, srcStep, 
        (Ipp32f*)pDst, dstStep, roiSize, (IppiDeconvFFTState_32f_C3R*)pState))

    return stsNoFunction;
}

CString CRunDeconvFFT::GetHistoryParms()
{
    CMyString parms;
    return parms 
        << kernelSize 
        << ", " << order
        << ", " << threshold
        ;
}



