/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWT.cpp: implementation of the CRunWT class.
// CRunWT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ChildFrm.h"
#include "Histo.h"
#include "RunWT.h"
#include "ParmWTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {VEC_A = 10000, VEC_X, VEC_Y, VEC_XY};
static int vecPosWT[4] = {VEC_A, VEC_X, VEC_Y, VEC_XY};
static CString vecNameWT[4] = {"Approx", "DetailX", "DetailY", "DetailXY"};
static UINT pickIDWT[4] = {IDC_CURSOR_PA, IDC_CURSOR_PX, IDC_CURSOR_PY, IDC_CURSOR_PXY};
static UINT grabIDWT[4] = {IDC_CURSOR_A, IDC_CURSOR_X, IDC_CURSOR_Y, IDC_CURSOR_XY};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWT::CRunWT()
{
    int i;
    for (i=0; i<4; i++) {
        m_pDocSrcWT[i] = NULL;
        m_pDocDstWT[i] = NULL;
        pSrcWT[i] = NULL;
        pDstWT[i] = NULL;
        srcStepWT[i] = 0;
        dstStepWT[i] = 0;
        m_IsSrcWT[i] = TRUE;
        m_IsDstWT[i] = TRUE;
    }

    m_bDstBorder = TRUE;
    m_pSpec = NULL;
    m_pBuffer = NULL;
    m_BufSize = 0;

    m_pTapsFwd[0].Init(pp32f, 3);
    m_pTapsFwd[0].Set(0,  0.25);
    m_pTapsFwd[0].Set(1,  0.50);
    m_pTapsFwd[0].Set(2,  0.25);
    m_anchorFwd[0] = 1;

    m_pTapsFwd[1].Init(pp32f, 5);
    m_pTapsFwd[1].Set(0, -0.125);
    m_pTapsFwd[1].Set(1, -0.25);
    m_pTapsFwd[1].Set(2,  0.75);
    m_pTapsFwd[1].Set(3, -0.25);
    m_pTapsFwd[1].Set(4, -0.125);
    m_anchorFwd[1] = 1;

    m_pTapsInv[0].Init(pp32f, 5);
    m_pTapsInv[0].Set(0, -0.25);
    m_pTapsInv[0].Set(1,  0.50);
    m_pTapsInv[0].Set(2,  1.50);
    m_pTapsInv[0].Set(3,  0.50);
    m_pTapsInv[0].Set(4, -0.25);
    m_anchorInv[0] = 2;

    m_pTapsInv[1].Init(pp32f, 3);
    m_pTapsInv[1].Set(0, -0.50);
    m_pTapsInv[1].Set(1,  1.00);
    m_pTapsInv[1].Set(2, -0.50);
    m_anchorInv[1] = 2;

}

CRunWT::~CRunWT()
{

}

BOOL CRunWT::Open(CFunc func)
{
    for (int j=0; j<4; j++) {
        m_pDocSrcWT[j] = NULL;
        m_pDocDstWT[j] = NULL;
        pSrcWT[j] = NULL;
        pDstWT[j] = NULL;
    }

    if (!CippiRun::Open(func)) return FALSE;

    m_bFwd = func.Found("Fwd");
    if (m_bFwd) {
        m_UsedVectors = VEC_SRC;
    } else {
        m_UsedVectors = VEC_DST;
        m_pDocSrcWT[0] = (CippiDemoDoc*)m_pDocSrc;
    }

    m_InitName = "ippi" + func.BaseName() + "InitAlloc"
                  + "_" + func.TypeName() + "_" + func.DescrName();
    m_FreeName = "ippi" + func.BaseName() + "Free"
                  + "_" + func.TypeName() + "_" + func.DescrName();
    m_GetBufSizeName = "ippi" + func.BaseName() + "GetBufSize"
                  + "_" + func.DescrName();

    for (int i=0; i<2; i++) {
        m_pTaps[i]  = m_bFwd ? m_pTapsFwd[i] : m_pTapsInv[i];
        m_anchor[i] = m_bFwd ? m_anchorFwd[i] : m_anchorInv[i];
        m_len[i]     = m_pTaps[i].Length();
    }
    return TRUE;
}

void CRunWT::Close()
{
    int i;
    for (i=0; i<2; i++) {
        if (m_bFwd) {
            m_pTapsFwd [i] = m_pTaps [i];
            m_anchorFwd[i] = m_anchor[i];
        } else {
            m_pTapsInv [i] = m_pTaps [i];
            m_anchorInv[i] = m_anchor[i];
        }
    }
}

BOOL CRunWT::PickNextPos()
{
    if (m_bFwd) {
        if (DEMO_APP->GetPickDst()) {
            switch (m_PickVecPos) {
            case VEC_SRC: m_PickVecPos = VEC_A; return TRUE;
            case VEC_A:    m_PickVecPos = VEC_X; return TRUE;
            case VEC_X:    m_PickVecPos = VEC_Y; return TRUE;
            case VEC_Y:    m_PickVecPos = VEC_XY; return TRUE;
            }
        }
    } else {
        switch (m_PickVecPos) {
        case VEC_SRC: m_PickVecPos = VEC_X; return TRUE;
        case VEC_X:    m_PickVecPos = VEC_Y; return TRUE;
        case VEC_Y:    m_PickVecPos = VEC_XY; return TRUE;
        case VEC_XY:
            if (DEMO_APP->GetPickDst()) {
                m_PickVecPos = VEC_DST; return TRUE;
            }
            break;
        }
    }
    m_PickVecPos = -1;
    return FALSE;
}

BOOL CRunWT::IsPickVecDst()
{
    if (m_bFwd)
        return m_PickVecPos > VEC_SRC;
    else
        return m_PickVecPos >= VEC_DST;
}


BOOL CRunWT::GetEqualDocPos(int& firstPos, int& secondPos)
{
    if (m_bFwd) {
        firstPos = VEC_SRC;
        for (int i=0; i<4; i++) {
            if (m_pDocSrc == m_pDocDstWT[i]) {
                secondPos = vecPosWT[i];
                return TRUE;
            }
        }
    } else {
        secondPos = VEC_DST;
        for (int i=0; i<4; i++) {
            if (m_pDocDst == m_pDocSrcWT[i]) {
                firstPos = vecPosWT[i];
                return TRUE;
            }
        }
    }
    return FALSE;
}

CMyString CRunWT::GetVectorName(int vecPos)
{
    for (int i=0; i<4; i++) {
        if (m_PickVecPos == vecPosWT[i]) {
            CMyString vectorName = vecNameWT[i];
            if (m_bFwd)
                vectorName += "Dst";
            else
                vectorName += "Src";
            return vectorName;
        }
    }
    return CippiRun::GetVectorName(vecPos);
}

BOOL CRunWT::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
    for (int i=0; i<4; i++) {
        if (m_PickVecPos == vecPosWT[i]) {
            pickID = pickIDWT[i];
            grabID = grabIDWT[i];
            return TRUE;
        }
    }
    return CippiRun::GetPickID(vecPos, pickID, grabID);
}

int CRunWT::GetPickVecPosToValid()
{
    if (m_bFwd) {
        return VEC_DST;
    } else {
        if (m_PickVecPos == VEC_DST)
            return VEC_DST;
        else
            return VEC_SRC;
    }
}

void CRunWT::GrabDoc(CDemoDoc* pDoc)
{
    pDoc->IsPicked(TRUE);
    if (m_bFwd) {
        for (int i=0; i<4; i++) {
            if (m_PickVecPos == vecPosWT[i]) {
                m_pDocDstWT[i] = (CippiDemoDoc*)pDoc;
                break;
            }
        }
    } else {
        for (int i=1; i<4; i++) {
            if (m_PickVecPos == vecPosWT[i]) {
                m_pDocSrcWT[i] = (CippiDemoDoc*)pDoc;
                break;
            }
        }
        if (m_PickVecPos == VEC_DST)
            m_pDocDst = pDoc;
    }
}

CParamDlg* CRunWT::CreateDlg() { return new CParmWTDlg;}

void CRunWT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmWTDlg* pDlg = (CParmWTDlg*)parmDlg;
    if (save) {
        for (int j=1; j<4; j++) {
            m_IsSrcWT[j] = pDlg->m_IsSrcWT[j];
            m_IsDstWT[j] = pDlg->m_IsDstWT[j];
            if (!m_IsSrcWT[j])
                m_pDocSrcWT[j] = NULL;
            if (!m_IsDstWT[j])
                m_pDocDstWT[j] = NULL;
        }
        for (int i=0; i<2; i++) {
            m_len[i]     = atoi(pDlg->m_LenStr[i]);
            m_anchor[i] = atoi(pDlg->m_AnchorStr[i]);
            m_pTaps[i].Set(pDlg->m_TapStr[i], m_len[i]);
        }
        m_bDstBorder = pDlg->m_bDstBorder;
    } else {
        for (int j=0; j<4; j++) {
            pDlg->m_IsSrcWT[j] = m_IsSrcWT[j];
            pDlg->m_IsDstWT[j] = m_IsDstWT[j];
            pDlg->m_pDocSrcWT[j] = m_pDocSrcWT[j];
            pDlg->m_pDocDstWT[j] = m_pDocDstWT[j];
        }
        for (int i=0; i<2; i++) {
            pDlg->m_LenStr[i].Format("%d",m_len[i]);
            pDlg->m_AnchorStr[i].Format("%d",m_anchor[i]);
            m_pTaps[i].Get(pDlg->m_TapStr[i], m_len[i]);
        }
        pDlg->m_bDstBorder = m_bDstBorder;
    }
}

int CRunWT::GetSrcBorder(int idx)
{
     switch (idx)
     {
     case LEFT: 
     case TOP: 
          return IPP_MAX(m_anchor[0], m_anchor[1]);
     case RIGHT:
     case BOTTOM:
          return IPP_MAX(m_len[0] - m_anchor[0] - 1, m_len[1] - m_anchor[1] - 1);
     }
     return 0;
}

BOOL CRunWT::PrepareSrc()
{
    if (m_bFwd) {
        if (!CippiRun::PrepareSrc()) return FALSE;
        IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
        if (roi.width & 1) roi.width--;
        if (roi.height & 1) roi.height--;
        m_pDocSrc->GetImage()->SetRoi(&roi);
        return TRUE;
    }
    IppiSize roi = {-1, -1};
    int i;
    for (i=0; i<4; i++) {
        if (!m_pDocSrcWT[i]) continue;
        IntersectRoi(m_pDocSrcWT[i],roi);
    }
    for (i=0; i<4; i++) {
        if (m_pDocSrcWT[i])
            m_pDocSrcWT[i]->SetRoiSize(roi);
    }
    for (i=0; i<4; i++) {
        if (!m_pDocSrcWT[i]) continue;
        if (!FixBorderProblem((CDemoDoc**)(m_pDocSrcWT + i)))
        {
            ReleaseBorderProblem();
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CRunWT::PrepareDst()
{
    if (!m_bFwd) {
        if (!CippiRun::PrepareDst()) return FALSE;
        if (m_bDstBorder) {
            if (!FixBorderProblem(&m_pDocDst))
            {
                ReleaseBorderProblem();
                return FALSE;
            }
        }
        return TRUE;
    }
    IppiSize roi = {-1, -1};
    int i;
    for (i=0; i<4; i++) {
        if (!CreateNewDstWT(i)) return FALSE;
        if (!m_pDocDstWT[i]) continue;
        IntersectRoi(m_pDocDstWT[i],roi);
    }
    for (i=0; i<4; i++) {
        if (m_pDocDstWT[i])
            m_pDocDstWT[i]->SetRoiSize(roi);
    }
    for (i=0; i<4; i++) {
        if (!m_pDocDstWT[i]) continue;
        if (m_bDstBorder) {
            if (!FixBorderProblem((CDemoDoc**)(m_pDocDstWT + i)))
            {
                ReleaseBorderProblem();
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL CRunWT::CreateNewDstWT(int i)
{
    m_NewDstWT[i] = FALSE;
    if (m_pDocDstWT[i]) return TRUE;
    if (!m_IsDstWT[i]) return TRUE;
    m_pDocDstWT[i] = CreateNewDoc(GetNewDstHeader());
    if (!m_pDocDstWT[i]) return FALSE;
    m_pDocDstWT[i]->GetFrame()->ShowWindow(SW_HIDE);
    m_NewDstWT[i] = TRUE;
    return TRUE;
}

ImgHeader CRunWT::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    if (m_bFwd) {
        header.SetSize(roi.width /2,
                            roi.height/2);
    } else {
        header.SetSize(roi.width *2,
                            roi.height*2);
    }
    if (m_bDstBorder) {
        header.SetSize(header.width  + GetSrcBorder(LEFT) + GetSrcBorder(RIGHT),
                       header.height + GetSrcBorder(TOP) + GetSrcBorder(BOTTOM));
    }
    return header;
}

void CRunWT::ActivateDst()
{
    int i;
    for (i=3; i>=0; i--) {
        if (m_pDocSrcWT[i])
            ActivateDoc(m_pDocSrcWT[i]);
    }
    for (i=3; i>=0; i--) {
        if (m_pDocDstWT[i])
            ActivateDoc(m_pDocDstWT[i]);
    }
    ActivateDoc(m_pDocDst);
}

void CRunWT::PrepareParameters()
{
    roiSize.width  = -1;
    roiSize.height = -1;
    if (m_bFwd) {
        for (int i=0; i<4; i++)
            IntersectRoi(m_pDocDstWT[i], roiSize);
    } else {
        for (int i=0; i<4; i++)
            IntersectRoi(m_pDocSrcWT[i], roiSize);
    }
    SetImageParameters((CippiDemoDoc*)m_pDocSrc, pSrcImg, pSrc, srcStep,
                             pSrcImgP, pSrcP, srcStepP,
                             m_srcROI, m_srcRoiSize, m_srcSize);
    SetImageParameters((CippiDemoDoc*)m_pDocDst, pDstImg, pDst, dstStep,
                             pDstImgP, pDstP, dstStepP,
                             m_dstROI, m_dstRoiSize, m_dstSize);

    CreateZeroImage();

    for (int i=0; i<4; i++) {
        SetImageParametersWT(m_pDocSrcWT[i], pSrcWT[i], srcStepWT[i]);
        SetImageParametersWT(m_pDocDstWT[i], pDstWT[i], dstStepWT[i]);
    }
}

void CRunWT::CreateZeroImage()
{
    CImage* pImage = m_bFwd ? m_pDocDstWT[0] : m_pDocSrcWT[0];
    m_ZeroImage.SetHeader(pImage->GetHeader());
    m_ZeroImage.AllocateData(0);
    m_ZeroImage.CopyRoi(pImage);
}

void CRunWT::SetImageParametersWT(CImage* pImage, void*& roiPtr, int& step)
{
    if (!pImage) pImage = &m_ZeroImage;
    roiPtr = pImage->GetRoiPtr();
    step    = pImage->Step();
}

BOOL CRunWT::BeforeCall()
{
    IppStatus status;
    status = CallInit(m_InitName);
    IppErrorMessage(m_InitName, status);
    if (status < 0) return FALSE;
    status = CallGetBufSize(m_GetBufSizeName);
    IppErrorMessage(m_GetBufSizeName, status);
    if (m_BufSize) {
        m_pBuffer = (Ipp8u*)ippMalloc(m_BufSize);
        if (!m_pBuffer) {
            IppErrorMessage("ippMalloc", ippStsNoMemErr);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CRunWT::AfterCall(BOOL bOK)
{
    IppStatus status = CallFree(m_FreeName); m_pSpec = NULL;
    IppErrorMessage(m_FreeName, status);
    if (m_pBuffer) {
        ippFree(m_pBuffer);
        m_pBuffer = NULL;
    }
    return TRUE;
}

IppStatus CRunWT::CallIppFunction()
{
    FUNC_CALL(ippiWTFwd_32f_C1R, ((Ipp32f*)pSrc,  srcStep,
          (Ipp32f*)pDstWT[0], dstStepWT[0],
          (Ipp32f*)pDstWT[1], dstStepWT[1],
          (Ipp32f*)pDstWT[2], dstStepWT[2],
          (Ipp32f*)pDstWT[3], dstStepWT[3],
          roiSize, (IppiWTFwdSpec_32f_C1R*)m_pSpec,
          m_pBuffer))
    FUNC_CALL(ippiWTInv_32f_C1R, (
          (Ipp32f*)pSrcWT[0], srcStepWT[0],
          (Ipp32f*)pSrcWT[1], srcStepWT[1],
          (Ipp32f*)pSrcWT[2], srcStepWT[2],
          (Ipp32f*)pSrcWT[3], srcStepWT[3],
          roiSize, (Ipp32f*)pDst,  dstStep,
          (IppiWTInvSpec_32f_C1R*)m_pSpec,
          m_pBuffer))
    FUNC_CALL(ippiWTFwd_32f_C3R, ((Ipp32f*)pSrc,  srcStep,
          (Ipp32f*)pDstWT[0], dstStepWT[0],
          (Ipp32f*)pDstWT[1], dstStepWT[1],
          (Ipp32f*)pDstWT[2], dstStepWT[2],
          (Ipp32f*)pDstWT[3], dstStepWT[3],
          roiSize, (IppiWTFwdSpec_32f_C3R*)m_pSpec,
          m_pBuffer))
    FUNC_CALL(ippiWTInv_32f_C3R, (
          (Ipp32f*)pSrcWT[0], srcStepWT[0],
          (Ipp32f*)pSrcWT[1], srcStepWT[1],
          (Ipp32f*)pSrcWT[2], srcStepWT[2],
          (Ipp32f*)pSrcWT[3], srcStepWT[3],
          roiSize, (Ipp32f*)pDst,  dstStep,
          (IppiWTInvSpec_32f_C3R*)m_pSpec,
          m_pBuffer))

    return stsNoFunction;
}

IppStatus CRunWT::CallInit(CString name)
{
    MATE_CALL(name, ippiWTFwdInitAlloc_32f_C1R, ((IppiWTFwdSpec_32f_C1R**)&m_pSpec,
          (Ipp32f*)m_pTaps[0],  m_len[0],  m_anchor[0],
          (Ipp32f*)m_pTaps[1], m_len[1], m_anchor[1]))
    MATE_CALL(name, ippiWTInvInitAlloc_32f_C1R, ((IppiWTInvSpec_32f_C1R**)&m_pSpec,
          (Ipp32f*)m_pTaps[0],  m_len[0],  m_anchor[0],
          (Ipp32f*)m_pTaps[1], m_len[1], m_anchor[1]))
    MATE_CALL(name, ippiWTFwdInitAlloc_32f_C3R, ((IppiWTFwdSpec_32f_C3R**)&m_pSpec,
          (Ipp32f*)m_pTaps[0],  m_len[0],  m_anchor[0],
          (Ipp32f*)m_pTaps[1], m_len[1], m_anchor[1]))
    MATE_CALL(name, ippiWTInvInitAlloc_32f_C3R, ((IppiWTInvSpec_32f_C3R**)&m_pSpec,
          (Ipp32f*)m_pTaps[0],  m_len[0],  m_anchor[0],
          (Ipp32f*)m_pTaps[1], m_len[1], m_anchor[1]))
    return stsNoFunction;
}

IppStatus CRunWT::CallFree(CString name)
{
    MATE_CALL(name, ippiWTInvFree_32f_C1R, ((IppiWTInvSpec_32f_C1R*)m_pSpec))
    MATE_CALL(name, ippiWTFwdFree_32f_C1R, ((IppiWTFwdSpec_32f_C1R*)m_pSpec))
    MATE_CALL(name, ippiWTInvFree_32f_C3R, ((IppiWTInvSpec_32f_C3R*)m_pSpec))
    MATE_CALL(name, ippiWTFwdFree_32f_C3R, ((IppiWTFwdSpec_32f_C3R*)m_pSpec))
    return stsNoFunction;
}

IppStatus CRunWT::CallGetBufSize(CString name)
{
    MATE_CALL(name, ippiWTInvGetBufSize_C1R, ((IppiWTInvSpec_32f_C1R*)m_pSpec, &m_BufSize))
    MATE_CALL(name, ippiWTFwdGetBufSize_C1R, ((IppiWTFwdSpec_32f_C1R*)m_pSpec, &m_BufSize))
    MATE_CALL(name, ippiWTInvGetBufSize_C3R, ((IppiWTInvSpec_32f_C3R*)m_pSpec, &m_BufSize))
    MATE_CALL(name, ippiWTFwdGetBufSize_C3R, ((IppiWTFwdSpec_32f_C3R*)m_pSpec, &m_BufSize))
    return stsNoFunction;
}


void CRunWT::SetHistory()
{
    if (m_bFwd) {
        for (int i=0; i<4; i++) {
            if (m_pDocDstWT[i]) {
                CippiRun::SetToHisto(m_pDocDstWT[i], vecPosWT[i]);
            }
        }
    } else {
        CippiRun::SetHistory();
    }
}

void CRunWT::SetToHistoSrc(CDemoDoc* pDoc, int vecPos)
{
    if (m_bFwd) {
        CippiRun::SetToHistoSrc(pDoc,vecPos);
        return;
    }
    CHisto* pHisto = pDoc->GetHisto();
    pHisto->RemoveAll();
    pHisto->AddTail(m_pDocSrc->GetHisto());
    for (int i=1; i<4; i++) {
        if (m_pDocSrcWT[i])
            pHisto->AddTail(m_pDocSrcWT[i]->GetTitle());
        else
            pHisto->AddTail("null");
    }
}

CString CRunWT::GetVecName(int vecPos)
{
    for (int i=0; i<4; i++) {
        if (vecPos == vecPosWT[i])
            return vecNameWT[i];
    }
    return "";
}

CString CRunWT::GetHistoryParms()
{
    CMyString parms;
    parms
        << ".., " << m_len[0] << ", " << m_anchor[0] << ", "
        << ".., " << m_len[1] << ", " << m_anchor[1];
    return parms;
}

