/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
<<        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
>>
*/

// ippiRun.cpp : implementation of the CippiRun class.
// CippiRun is derived from CRun class that provides CDemo documents
// processing by IPP functions.
// CippiRun is the base class for all ippiDemo Run classes that process
// images by concrete ippIP, ippCC, ippCV, ippJP functions.
// See CRun class for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiRun.h"
#include "ippiDemoDoc.h"
#include "ippiDemoView.h"
#include "Histo.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "ParamDlg.h"
#include "OptBorderDlg.h"
#include "BorderConstDlg.h"
#include "Timing.h"
#include "Movie.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Static Border Dialog
//////////////////////////////////////////////////////////////////////

//enum {BORDER_NONE, BORDER_REDUCE, BORDER_REPLICATE, BORDER_CONST, BORDER_ZERO};
int  CippiRun::m_borderType = BORDER_REPLICATE;
CVectorUnit CippiRun::m_borderConst;

int CippiRun::DoOptBorderDlg()
{
    COptBorderDlg dlg;
    dlg.m_Type = m_borderType;
    int result = dlg.DoModal();
    if (result == IDOK)
        m_borderType = dlg.m_Type;
    return result;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CippiRun::CippiRun()
{
    m_scaleFactor_Movie = 0;
}

CippiRun::~CippiRun()
{
}

BOOL CippiRun::Open(CFunc func)
{
    if (!CRun::Open(func)) return FALSE;
    return TRUE;
}


BOOL CippiRun::NoDst()
{
    if (m_Func.Found("WTFwd")) return FALSE;
    if (m_Func.Found("Histogram")) return FALSE;
    return CRun::NoDst();
}

BOOL CippiRun::Complex()
{
    if (m_pDocSrc && m_pDocSrc->GetImage()->Complex()) return TRUE;
    if (m_pDocSrc2 && m_pDocSrc2->GetImage()->Complex()) return TRUE;
    if (m_pDocDst && m_pDocDst->GetImage()->Complex()) return TRUE;
    return FALSE;
}

BOOL CippiRun::ProcessFunctionStart(CDemoDoc *pDoc, CString funcName)
{
    if (!CRun::ProcessFunctionStart(pDoc, funcName))
        return FALSE;
    return TRUE;
}

void CippiRun::ProcessFunctionFinish()
{
    CRun::ProcessFunctionFinish();
}

CMyString CippiRun::GetVectorName(int vecPos)
{
    return CRun::GetVectorName(vecPos);
}

BOOL CippiRun::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
    return CRun::GetPickID(vecPos, pickID, grabID);
}

BOOL CippiRun::ProcessCall()
{
    BOOL result = TRUE;

    if (ByPlanes())
    {
        double perf = 0;
        for (int coi = 0; coi < MinChannels(); coi++) {
            SetCoiToDocs(coi);
            if (!CRun::ProcessCall()) {
                result = FALSE;
                break;
            }
            perf += m_pPerf->GetClocks();
        }
        m_pPerf->SetClocks(perf);
        SetCoiToDocs(-1);
    }
    else
    {
        result = CRun::ProcessCall();
    }
    return result;
}

BOOL CippiRun::IsImageByPlanes(CImage* pImg)
{
    if (!pImg) return TRUE;
    int channels = (pImg->GetCoi() < 0) ? pImg->Channels() : 1;
    if (channels == 1) return FALSE;
    if (!pImg->Plane()) return FALSE;
    return TRUE;
}

BOOL CippiRun::ByPlanes()
{
    if (m_Func.SrcChannels() > 1) return FALSE;
    if (m_Func.DstChannels() > 1) return FALSE;
    if (MaxChannels() == 1) return FALSE;
    if (!IsImageByPlanes((CippiDemoDoc*)m_pDocSrc )) return FALSE;
    if (!IsImageByPlanes((CippiDemoDoc*)m_pDocSrc2)) return FALSE;
    if (!IsImageByPlanes((CippiDemoDoc*)m_pDocDst )) return FALSE;
    return TRUE;
}

int CippiRun::MinChannels()
{
    int num = m_pDocSrc->GetImage()->Channels();
    if (m_pDocSrc2) {
        if (num > m_pDocSrc2->GetImage()->Channels())
            num = m_pDocSrc2->GetImage()->Channels();
    }
    if (m_pDocDst) {
        if (num > m_pDocDst->GetImage()->Channels())
            num = m_pDocDst->GetImage()->Channels();
    }
    return num;
}

int CippiRun::MaxChannels()
{
    int num = m_pDocSrc->GetImage()->Channels();
    if (m_pDocSrc2) {
        if (num < m_pDocSrc2->GetImage()->Channels())
            num = m_pDocSrc2->GetImage()->Channels();
    }
    if (m_pDocDst) {
        if (num < m_pDocDst->GetImage()->Channels())
            num = m_pDocDst->GetImage()->Channels();
    }
    return num;
}

void CippiRun::SetCoiToDocs(int coi)
{
    m_pDocSrc->GetImage()->SetCoi(coi);
    if (m_pDocSrc2) m_pDocSrc2->GetImage()->SetCoi(coi);
    if (m_pDocDst ) m_pDocDst ->GetImage()->SetCoi(coi);
}

CippiDemoDoc* CippiRun::CreateNewDoc(CImage* pImg, CString title, BOOL bMakeVisible)
{
     CippiDemoDoc* pDoc = CreateNewDoc(pImg->GetHeader(), title, bMakeVisible);
     if (pDoc)
         pDoc->CopyData(pImg);
     return pDoc;
}

CippiDemoDoc* CippiRun::CreateNewDoc(ImgHeader header, CString title, BOOL bMakeVisible)
{
    return (CippiDemoDoc*)DEMO_APP->CreateDoc(header,title,bMakeVisible);
}

BOOL CippiRun::PrepareSrc()
{
    if (!FixBorderProblem(&m_pDocSrc))
        return FALSE;
    return TRUE;
}

void CippiRun::ReleaseBorderProblem()
{
    m_TmpDocList.ReleaseAll();
}

BOOL CippiRun::FixBorderProblem(CDemoDoc** ppDoc)
{
    int expand[4];
    if (!GetBorderExpansion((*ppDoc)->GetImage(), expand))
        return TRUE;
    int type = GetBorderType(*ppDoc);
    if (!type)
        return FALSE;
    if (type != BORDER_REDUCE)
    {
        if (!CreateBorderDoc(ppDoc,type,expand))
            type = BORDER_REDUCE;
    }
    if (type == BORDER_REDUCE)
        return SetReducedRoi(*ppDoc,expand);
    return TRUE;
}

BOOL CippiRun::CreateBorderRoi(CImage* pImg)
{
    int expand[4];
    if (!GetBorderExpansion(pImg, expand))
        return TRUE;
    return SetReducedRoi(pImg,expand);
}

BOOL CippiRun::GetSrcBorders(int borders[4])
{
    int sum = 0;
    for (int i=0; i<4; i++)
        sum += borders[i] = GetSrcBorder(i);
    return sum ? TRUE : FALSE;
}

BOOL CippiRun::GetBorderExpansion(CImage* pImg, int expand[4])
{
    int borders[4];    
    if (!GetSrcBorders(borders))
        return FALSE;
    if (!GetRoiExpansion(pImg,borders,expand))
        return FALSE;
    return TRUE;
}

BOOL CippiRun::GetRoiExpansion(CImage* pImg, int borders[4], int expand[4])
{
    if (!pImg) return FALSE;
    IppiRect roi = pImg->GetActualRoi();
    expand[LEFT] = borders[LEFT] - roi.x;
    expand[RIGHT] = borders[RIGHT] - (pImg->Width() - roi.x - roi.width);
    expand[TOP] = borders[TOP] - roi.y;
    expand[BOTTOM] = borders[BOTTOM] - (pImg->Height() - roi.y - roi.height);
    BOOL result = FALSE;
    for (int i=0; i<4; i++) {
        if (expand[i] < 0) expand[i] = 0;
        else if (expand[i] > 0) result = TRUE;
    }
    return result;
}

IppiRect CippiRun::GetReducedRoi(CImage* pImg, int expand[4])
{
    IppiRect roi = pImg->GetActualRoi();
    roi.x += expand[LEFT];
    roi.width -= expand[LEFT] + expand[RIGHT];
    roi.y += expand[TOP];
    roi.height -= expand[TOP] + expand[BOTTOM];
    return roi;
}

static BOOL IsValidRoi(CImage* pImg, IppiRect roi)
{
    if (roi.x >= pImg->Width() ||
         roi.y >= pImg->Height() ||
         roi.width <= 0 ||
         roi.height <= 0) {
        return FALSE;
    }
    return TRUE;
}

static BOOL IsReducedRoi(CImage* pImg, IppiRect roi)
{
    IppiRect imgRoi = pImg->GetActualRoi();
    if (roi.x > imgRoi.x)
        return TRUE;
    if (roi.y > imgRoi.y)
        return TRUE;
    if (roi.x + roi.width < imgRoi.x + imgRoi.width)
        return TRUE;
    if (roi.y + roi.height < imgRoi.y + imgRoi.height)
        return TRUE;
    return FALSE;
}

BOOL CippiRun::SetReducedRoi(CImage* pImg, IppiRect roi, CString imgName)
{
    BOOL bMessage = !imgName.IsEmpty();
    if (!IsReducedRoi(pImg, roi))
        return TRUE;
    if (bMessage)
    {
        if (DEMO_APP->MessageBox(
            (pImg->IsRoi() ? "ROI will be reduced in " : "ROI will be created in ")
            + imgName + " image",
            MB_OKCANCEL) == IDCANCEL)
            return FALSE;
    }
    if (!IsValidRoi(pImg, roi))
    {
        if (bMessage)
            DEMO_APP->MessageBox(
                "ROI is too small to be reduced in " + imgName + " image",
                MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }
    pImg->SetRoi(&roi);
    return TRUE;
}

BOOL CippiRun::SetReducedRoi(CImage* pImg, int expand[4], CString imgName)
{
    return SetReducedRoi(pImg, GetReducedRoi(pImg, expand), imgName);
}

BOOL CippiRun::SetReducedRoi(CDemoDoc* pDoc, IppiRect roi)
{
    return SetReducedRoi(pDoc->GetImage(), roi, pDoc->GetTitle());
}

BOOL CippiRun::SetReducedRoi(CDemoDoc* pDoc, int expand[4])
{
    return SetReducedRoi(pDoc->GetImage(), expand, pDoc->GetTitle());
}

int CippiRun::GetBorderType(CDemoDoc* pDoc)
{
    if (m_Func.Inplace())
        return BORDER_REDUCE;
    CImage* pImg = pDoc->GetImage();
    if (m_borderType == BORDER_CONST)
    {
        m_borderConst.Init(pImg->Type());
        if (m_borderConst.Length() == 0)
            m_borderConst.Vector().SetLength(4);
        int len = pImg->Channels() - m_Func.SrcAlpha();
        CBorderConstDlg dlg;
        dlg.m_Text = pDoc->GetFrameTitle();
        dlg.m_Len = len;
        m_borderConst.Get(dlg.m_ValueStr, len);
        if (dlg.DoModal() == IDCANCEL)
            return FALSE;
        if (m_borderType == BORDER_CONST)
            m_borderConst.Set(dlg.m_ValueStr, len);
    }
    return m_borderType;
}

BOOL CippiRun::CreateBorderDoc(CDemoDoc** ppDoc, int borderType, int expand[4])
{
    CippiDemoDoc* pSrcDoc = (CippiDemoDoc*)*ppDoc;
    ImgHeader header = pSrcDoc->GetHeader();
    header.SetSize(header.width + expand[LEFT] + expand[RIGHT],  header.height + expand[TOP] + expand[BOTTOM]);
    CippiDemoDoc* pTmpDoc =  (CippiDemoDoc*)DEMO_APP->CreateDoc(header,pSrcDoc->GetTitle() + "_bordered",FALSE);
    if (!pTmpDoc) return FALSE;

//    pTmpDoc->SetData(0);
    BOOL bSrcRoi = pSrcDoc->IsRoi();
    IppiRect srcRoi = pSrcDoc->GetActualRoi();
    pSrcDoc->ResetRoi();
    IppiRect tmpRoi = {expand[LEFT], expand[TOP], pSrcDoc->Width(), pSrcDoc->Height()};
    pTmpDoc->SetRoi(&tmpRoi);
    pTmpDoc->CopyData_ROI(pSrcDoc);
    if (bSrcRoi)
        pSrcDoc->SetRoi(&srcRoi);

    BOOL result = FALSE;
    switch (borderType)
    {
    case BORDER_CONST:
        result = pTmpDoc->SetConstBorder(m_borderConst);
        break;
    case BORDER_ZERO:
        result = pTmpDoc->SetZeroBorder();
        break;
    default:
    case BORDER_REPLICATE:
        result = pTmpDoc->SetReplicateBorder();
        break;
    }
    if (!result)
    {
        pTmpDoc->OnCloseDocument();
        return FALSE;
    }
    tmpRoi = srcRoi;
    tmpRoi.x += expand[LEFT];
    tmpRoi.y += expand[RIGHT];
    pTmpDoc->SetRoi(&tmpRoi);
    m_TmpDocList.Add((CDemoDoc **)ppDoc, pSrcDoc, pTmpDoc);
    return TRUE;
}

BOOL CippiRun::PrepareDst()
{
     m_NewDst = FALSE;
     if (!(m_UsedVectors & VEC_DST)) return TRUE;
     if (m_pDocDst) return TRUE;
     if (!m_pDocSrc) m_pDocSrc = ACTIVE_DOC;
     m_pDocDst = CreateNewDoc(GetNewDstHeader());
     if (!m_pDocDst) return FALSE;
     CopyDataToNewDst();
     CopyContoursToNewDst();
     m_pDocDst->GetFrame()->ShowWindow(SW_HIDE);
     m_NewDst = TRUE;
     return TRUE;
}

void CippiRun::DeleteNewDst()
{
     if (m_NewDst)
         m_pDocDst->OnCloseDocument();
}

void CippiRun::ActivateDoc(CDemoDoc* pDoc)
{
    if (pDoc) pDoc->Activate();
}

void CippiRun::ActivateMask(int beforeVec)
{
    if (m_Func.Inplace() || (m_Func.BaseName() == "Set")) {
        if (m_pDocSrc2) {
            if (beforeVec != VEC_SRC2) return;
        }  else {
            if (beforeVec != VEC_SRC) return;
        }
    } else {
        if (beforeVec != VEC_DST) return;
    }
    ActivateDoc(m_pDocMask);
}

void CippiRun::ActivateDst()
{
    ActivateMask(VEC_SRC);
    ActivateDoc(m_pDocSrc);
    ActivateMask(VEC_SRC2);
    ActivateDoc(m_pDocSrc2);
    ActivateMask(VEC_DST);
    ActivateDoc(m_pDocDst);
}

ImgHeader CippiRun::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    if (m_Func.SrcType() != m_Func.DstType())
        header.type = m_Func.DstType();
    if (!ByPlanes()) {
        header.channels = m_Func.DstChannels();
        header.plane = m_Func.DstPlane();
    }
    if (DEMO_APP->GetNewSize() == NEWSIZE_ROI) {
        IppiSize roi = {-1, -1};
        IntersectRoi((CippiDemoDoc*)m_pDocSrc,roi);
        IntersectRoi((CippiDemoDoc*)m_pDocSrc2,roi);
        IntersectRoi((CippiDemoDoc*)m_pDocMask,roi);
        header.SetSize(roi.width, roi.height);
    }
    return header;
}

void CippiRun::CopyDataToNewDst()
{
    if (!m_pDocDst) return;
    m_pDocDst->GetImage()->SetData(SET_BLACK);
    return;
    if (!m_pDocSrc) return;
    CImage* srcImage = m_pDocSrc->GetImage();
    CImage* dstImage = m_pDocDst->GetImage();
    dstImage->CopyData(srcImage);
}

void CippiRun::CopyContoursToNewDst()
{
     if (!m_pDocSrc || !m_pDocDst) return;
     CImage* srcImage = m_pDocSrc->GetImage();
     CImage* dstImage = m_pDocDst->GetImage();
     if (srcImage->Width()  == dstImage->Width() &&
          srcImage->Height() == dstImage->Height())
          dstImage->CopyRoi(srcImage);
}

void CippiRun::PrepareParameters()
{
     roiSize.width  = -1;
     roiSize.height = -1;
     IntersectRoi((CippiDemoDoc*)m_pDocSrc , roiSize);
     IntersectRoi((CippiDemoDoc*)m_pDocSrc2, roiSize);
     IntersectRoi((CippiDemoDoc*)m_pDocMask, roiSize);
     IntersectRoi((CippiDemoDoc*)m_pDocDst , roiSize);
     SetImageParameters((CippiDemoDoc*)m_pDocSrc, pSrcImg, pSrc, srcStep,
                              pSrcImgP, pSrcP, srcStepP,
                              m_srcROI, m_srcRoiSize, m_srcSize);
     SetImageParameters((CippiDemoDoc*)m_pDocSrc2, pSrcImg2, pSrc2, srcStep2,
                              pSrcImg2P, pSrc2P, srcStep2P,
                              m_srcROI2, m_srcRoiSize2, m_srcSize2);
     SetImageParameters((CippiDemoDoc*)m_pDocDst, pDstImg, pDst, dstStep,
                              pDstImgP, pDstP, dstStepP,
                              m_dstROI, m_dstRoiSize, m_dstSize);
     SetImageParameters((CippiDemoDoc*)m_pDocMask, pMask, maskStep);
}

void CippiRun::SetImageParameters(CImage* image,
                                        void*& imgPtr, void*& roiPtr, int& step,
                                        void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4],
                                        IppiRect& roiRect, IppiSize& roiSize, IppiSize& imgSize)
{
    imgPtr = NULL;
    roiPtr = NULL;
    step = 0;
    for (int i=0; i<4; i++) {
        imgPlanePtr[i] = NULL;
        roiPlanePtr[i] = NULL;
        planeStep[i] = 0;
    }
    if (!image) return;
    image->GetImageParameters(
        imgPtr, roiPtr, step,
        imgPlanePtr, roiPlanePtr, planeStep,
        roiRect, roiSize, imgSize);
}

void CippiRun::SetImageParameters(CImage* image, void*& roiPtr, int& step)
{
    roiPtr = NULL;
    step = 0;
    if (!image) return;
    roiPtr = image->GetRoiPtr();
    step = image->Step();
}

void CippiRun::IntersectRoi(CImage* image, IppiSize& roiSize)
{
    if (!image) return;
    IppiRect roiRect = image->GetActualRoi();
    if (roiSize.width < 0)
        roiSize.width  = roiRect.width;
    else
        roiSize.width  = IPP_MIN(roiSize.width ,roiRect.width );
    if (roiSize.height < 0)
        roiSize.height = roiRect.height;
    else
        roiSize.height = IPP_MIN(roiSize.height,roiRect.height);
}

static int AddCoiParms(CString& info, CippiDemoDoc* pDoc , CString name)
{
     if (!pDoc) return 0;
     if (!pDoc->Plane() &&
          pDoc->Channels() != 1)
          return 0;
     int coi = pDoc->GetCoi();
     if (coi == 0) return 0;
     if (!info.IsEmpty()) info += ", ";
     CString buff;
     int result = 1;
     if (coi == 1) {
          buff.Format("%s",name);
          result = 0;
     } else if (coi == 2)
          buff.Format("%s+plane",name);
     else
          buff.Format("%s+%d*plane",name,coi-1);
     info += buff;
     return result;
}

void CippiRun::SetToHistoFunc(CDemoDoc* pDoc, int vecPos)
{
    CHisto* pHisto = pDoc->GetHisto();
    if (vecPos)
        CRun::SetToHistoFunc(pDoc, vecPos);
}

void CippiRun::SetToHistoDst(CDemoDoc* pDoc, int vecPos)
{
    CRun::SetToHistoDst(pDoc, vecPos);
}

int CippiRun::GetInterpolate(int interDlg, BOOL smoothDlg)
{
    int interpolate = IPPI_INTER_NN;
    switch (interDlg) {
    case 0: interpolate = IPPI_INTER_NN      ; break;
    case 1: interpolate = IPPI_INTER_LINEAR ; break;
    case 2: interpolate = IPPI_INTER_CUBIC  ; break;
    case 3: interpolate = IPPI_INTER_SUPER  ; break;
    case 4: interpolate = IPPI_INTER_LANCZOS; break;
    }
    if (smoothDlg)
        interpolate = interpolate | IPPI_SMOOTH_EDGE;
    return interpolate;
}

void CippiRun::SetInterpolate(int interpolate, int& interDlg)
{
    interpolate  = interpolate & ~IPPI_SMOOTH_EDGE;
    switch (interpolate) {
    case IPPI_INTER_NN      : interDlg = 0; break;
    case IPPI_INTER_LINEAR : interDlg = 1; break;
    case IPPI_INTER_CUBIC  : interDlg = 2; break;
    case IPPI_INTER_SUPER  : interDlg = 3; break;
    case IPPI_INTER_LANCZOS: interDlg = 4; break;
    default: interDlg = 0; break;
    }
}

void CippiRun::SetInterpolate(int interpolate, int& interDlg, BOOL& smoothDlg)
{
    smoothDlg = (interpolate & IPPI_SMOOTH_EDGE) ? TRUE : FALSE;
    SetInterpolate(interpolate,interDlg);
}

CString CippiRun::GetInterParm(int interpolate)
{
     CString parm;
     int inter = interpolate & 0xFFFF;
     int flags = interpolate - inter;
     switch (inter)
     {
     case IPPI_INTER_NN                     : parm = "NN"          ; break;
     case IPPI_INTER_LINEAR                : parm = "Linear"     ; break;
     case IPPI_INTER_CUBIC                 : parm = "Cubic"      ; break;
     case IPPI_INTER_CUBIC2P_CATMULLROM: parm = "CatmullRom"; break;
     case IPPI_INTER_CUBIC2P_BSPLINE    : parm = "BSpline"    ; break;
     case IPPI_INTER_CUBIC2P_B05C03     : parm = "B05C03"     ; break;
     case IPPI_INTER_LANCZOS              : parm = "Lanczos"    ; break;
     case IPPI_INTER_SUPER                 : parm = "Super"      ; break;
     }
     if (flags & IPPI_SMOOTH_EDGE  ) parm += "|SmoothEdge"  ;
     if (flags & IPPI_SUBPIXEL_EDGE) parm += "|SubpixelEdge";
     if (flags & IPPI_ANTIALIASING ) parm += "|AntiAliasing";
     return parm;
}

void CippiRun::SaveInplaceVector()
{
    if (m_pDocSrc2)
        m_InplaceImage = *((CImage*)m_pDocSrc2);
    else
        m_InplaceImage = *((CImage*)m_pDocSrc);
}

void CippiRun::SetInplaceParms()
{
    if (m_pDocSrc2)
        SetImageParameters(&m_InplaceImage,
            pSrcImg2, pSrc2, srcStep2,
            pSrcImg2P, pSrc2P, srcStep2P,
            m_srcROI2, m_srcRoiSize2, m_srcSize2);
    else
        SetImageParameters(&m_InplaceImage,
            pSrcImg, pSrc, srcStep,
            pSrcImgP, pSrcP, srcStepP,
            m_srcROI, m_srcRoiSize, m_srcSize);
}

void CippiRun::ResetInplaceParms()
{
    if (m_pDocSrc2)
         SetImageParameters((CippiDemoDoc*)m_pDocSrc2,
             pSrcImg2, pSrc2, srcStep2,
             pSrcImg2P, pSrc2P, srcStep2P,
             m_srcROI2, m_srcRoiSize2, m_srcSize2);
    else
         SetImageParameters((CippiDemoDoc*)m_pDocSrc,
             pSrcImg, pSrc, srcStep,
             pSrcImgP, pSrcP, srcStepP,
             m_srcROI, m_srcRoiSize, m_srcSize);
}

double CippiRun::GetPerfDivisor()
{
    CDemoDoc* pDoc = m_pDocDst ? m_pDocDst : m_pDocSrc;
    if (!pDoc) return 1;
    IppiRect roi = pDoc->GetImage()->GetActualRoi();
    return roi.width*roi.height;
}

CString CippiRun::GetPerfUnit()
{
    return "pixel";
}

//////////////////////////////////////////////////////////////////////
// Movie
//////////////////////////////////////////////////////////////////////

int CippiRun::GetMovieCycle()
{
    return CMovie::GetCycle();
}

int CippiRun::GetMovieCycleReverse(int& tim)
{
    tim = CMovie::GetTimReverse(tim);
    return CMovie::GetCycle() >> 1;
}

static int getStep(int& tim, int nSteps, int cycle)
{
    double section = (double)cycle / nSteps;
    int step = (int)(tim / section);
    int tim1;
    if (step == nSteps - 1) {
        tim1 = cycle;
    } else {
        double dtim1 = section*(step + 1);
        tim1 = (int)dtim1;
        if (dtim1 - tim1 > 0) tim1 ++;
    }
    tim = tim1 - tim;
    return step;
}

int CippiRun::GetMovieStep(int& tim, int nSteps)
{
    int cycle = GetMovieCycle();
    return getStep(tim,nSteps,cycle);
}

int CippiRun::GetMovieStepReverse(int& tim, int nSteps)
{
    int cycle = GetMovieCycle();
    int step = getStep(tim, nSteps*2 - 1, cycle);
    if (step >= nSteps) step = nSteps*2 - 2 - step;
    return step;
}

double CippiRun::GetMovieValue(int tim, double maxVal)
{
    return (tim*maxVal)/(double)GetMovieCycle();
}

double CippiRun::GetMovieValueReverse(int tim, double maxVal)
{
    int cycle = GetMovieCycleReverse(tim);
    return (tim*maxVal)/(double)cycle;
}

int CippiRun::ProcessMovie(CFunc func,
                            CImage *pImgSrc, CImage* pImgDst, CImage* pImgTmp,
                            int time, CMyString srcInfo, CMyString& dstInfo)
{
     if (!IsFunction(func)) return FALSE;
     if (!IsMovie(func)) return FALSE;
     if (!Open(func)) return FALSE;
     m_pDocSrc  = NULL;
     m_pDocSrc2 = NULL;
     m_pDocMask = NULL;
     m_pDocDst  = NULL;
     PrepareMovieParameters(pImgSrc, pImgDst);
     dstInfo.Empty();
     int num = 1;
     int result = SetMovieArgs(time, srcInfo, dstInfo, num);
     if (result < 0) result = 1;
     if (result == 0) return FALSE;
     if (num == 1) 
     {
          SetMovieBorderRoi(pImgSrc, pImgDst);
          result = BeforeCall();
          if (result)
          {
                result = CallIpp(-1);
                AfterCall(result);
          }
          ResetMovieArgs();
     }
     else if (num > 1) 
     {
          pImgTmp->CopyData(pImgSrc);
          CImage* pSD[2] = {pImgTmp, pImgDst};
          int i1 = 0;
          int i2 = 1;
          for (int i=0; i<num; i++) {
                SetMovieBorderRoi(pSD[i1], pSD[i2]);
                result = BeforeCall();
                if (result)
                {
                     result = CallIpp(-1);
                     AfterCall(result);
                }
                i1 = (i1 + 1) & 1;
                i2 = (i2 + 1) & 1;
                if (!result)
                     break;
          }
          if (i1 == 0)
                pImgDst->CopyData(pImgTmp);
          ResetMovieArgs();
     } 
     else if (num < 0) 
     {
          pImgDst->CopyData(pImgSrc);
     }
     Close();
     return result;
}

BOOL CippiRun::IsMovie(CFunc func)
{
    return FALSE;
}

int CippiRun::SetMovieArgs(int time, CMyString srcInfo, CMyString& dstInfo,
                              int& numReps)
{
    return 0;
}

void CippiRun::ResetMovieArgs()
{
}

void CippiRun::PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst)
{
    roiSize.width = pImgSrc->GetActualRoi().width;
    roiSize.height = pImgSrc->GetActualRoi().height;
    SetImageParameters(pImgSrc, pSrcImg, pSrc, srcStep,
                             pSrcImgP, pSrcP, srcStepP,
                             m_srcROI, m_srcRoiSize, m_srcSize);
    SetImageParameters(pImgDst, pDstImg, pDst, dstStep,
                             pDstImgP, pDstP, dstStepP,
                             m_dstROI, m_dstRoiSize, m_dstSize);
}

void CippiRun::SetMovieBorderRoi(CImage* pImgSrc, CImage* pImgDst)
{
    IppiRect roi;
    IppiRect *pRoi = CMovie::SaveRoi(pImgSrc, roi);
    
    CreateBorderRoi(pImgSrc);

    pImgDst->CopyRoi(pImgSrc);

    PrepareMovieParameters(pImgSrc, pImgDst);

    pImgSrc->SetRoi(pRoi);
    pImgDst->SetRoi(pRoi);
}
