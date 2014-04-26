/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Movie.cpp: implementation of the CMovie class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiRun.h"
#include "MainFrm.h"
#include "Director.h"
#include "MovieMask.h"
#include "Wander.h"
#include "Timing.h"
#include "Movie.h"

#include "common\CProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//  Change MovieSlider_PerfFactor to increase/decrease rate of
//  Movie - PerfStream mode
//  MovieSlider_PerfFactor shuld be >= 1
static const double MovieSlider_PerfFactor = 1;

static UINT WM_Movie = ::RegisterWindowMessage(WM_MOVIE);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMovieFuncArray CMovie::m_FullArray;
int CMovie::m_MovieType = MOVIE_TYPE_PERF;
int CMovie::m_MovieCycle = 60;
int CMovie::m_MinCycle = 10;
int CMovie::m_MaxCycle = 110;
int CMovie::m_TimeShot = 50;
CMovieMaskArray* CMovie::m_pMaskArray = NULL;

void CMovie::InitFullArray()
{
    ClearFunctions();
    AddFunction("AddC", TRUE);
    AddFunction("SubC", TRUE);
    AddFunction("ColorTwist32f", TRUE);
    AddFunction("ReduceBits", FALSE);
//    AddFunction("ReduceBits (Noise)");
    AddFunction("FilterHipass", FALSE);
    AddFunction("FilterLowpass", FALSE);
    AddFunction("FilterMax", FALSE);
    AddFunction("FilterMin", FALSE);
    AddFunction("FilterBox", FALSE);
    AddFunction("FilterMedian", FALSE);
    AddFunction("FilterSharpen", FALSE);
    AddFunction("Mirror", FALSE);
    AddFunction("Rotate", TRUE);
    AddFunction("ResizeSqrPixel", TRUE);
    AddFunction("WarpAffineQuad", TRUE);
    AddFunction("WarpPerspectiveQuad", TRUE);
    AddFunction("WarpBilinearQuad", TRUE);
}

CMovie::CMovie(CImage* pImg, CProcess* pProcess, HWND hWnd)
{
    m_pImg      = pImg;
    m_pProcess = pProcess;
    m_hWnd      = hWnd;

    m_IsInit = FALSE;
    m_IsPlaying = FALSE;
    m_Type = MOVIE_TYPE_ARG;
    m_pWander = new CWander;
}

CMovie::~CMovie()
{
    delete m_pWander;
}

void CMovie::InitStatic()
{
    InitFullArray();
    InitMaskArray();
}

void CMovie::InitMaskArray()
{
    if (m_pMaskArray) delete m_pMaskArray;
    m_pMaskArray = new CMovieMaskArray;
    m_pMaskArray->AddMask(IDB_CIRCLE, "Circle");
    m_pMaskArray->AddMask(IDB_BEAR, "Bear");
    m_pMaskArray->AddMask(IDB_INTEL, "Intel");

    m_pMaskArray->SetCurIndex(0);
//    m_pMaskArray->ElementAt(0).Load();
}

CString CMovie::GetHotString()
{
    CString str;
    return str;
}

void CMovie::SetHotString(CMyString str)
{
    if (str == "-") return;
}

void CMovie::ClearFunctions()
{
    m_FullArray.RemoveAll();
}

void CMovie::AddFunction(CString name, BOOL bHot)
{
    CMovieFunc func(name, bHot, bHot);
    m_FullArray.Add(func);
}

int CMovie::FindInFull(CString name)
{
    for (int i=0; i<m_FullArray.GetSize(); i++) {
        if (m_FullArray[i] == name) return i;
    }
    return -1;
}

int CMovie::FindInHot(CString name)
{
    for (int i=0; i<m_FullArray.GetSize(); i++) {
        if (m_FullArray[i].bHot && (m_FullArray[i] == name)) return i;
    }
    return -1;
}

int CMovie::FindInPerf(CString name)
{
    for (int i=0; i<m_FullArray.GetSize(); i++) {
        if (m_FullArray[i].bPerf && (m_FullArray[i] == name)) return i;
    }
    return -1;
}

void CMovie::Init()
{
    ASSERT(m_pImg);
    if (m_IsInit) return;
    m_IsInit = TRUE;
    ReleaseHotFunc();
    if (m_pImg->Type() != pp8u) return;
    for (int i=0; i<m_FullArray.GetSize(); i++)
    {
        CMovieFunc func = m_FullArray[i];
        if (func.bHot)
            InitHotFunc(func);
    }
}

void CMovie::ResetClocks()
{
    for (int i=0; i<m_ClocksArr.GetSize(); i++)
    {
        m_ClocksArr.ElementAt(i) = 0;
        m_LoopsArr.ElementAt(i) = 0;
    }
}

int CMovie::GetNumMovies()
{
    if (!m_IsPlaying)
        Init();
    return (int)m_FuncArr.GetSize();
}

void CMovie::ReleaseHotFunc()
{
    m_NameArr.RemoveAll();
    m_FuncArr.RemoveAll();
    m_pRunArr.RemoveAll();
    m_ClocksArr.RemoveAll();
    m_LoopsArr.RemoveAll();
}

BOOL CMovie::InitHotFunc(CMyString hotName)
{
    CString base = GetBase(hotName);
    CFunc func = GetFunc(base);
    if (func.IsEmpty()) return FALSE;
    CippiRun* pRun = GetRun(func);
    if (!pRun) return FALSE;
    m_NameArr.Add(hotName);
    m_FuncArr.Add(func);
    m_pRunArr.Add(pRun);
    m_ClocksArr.Add(0);
    m_LoopsArr.Add(0);
    return TRUE;
}

CString CMovie::GetBase(CMyString name)
{
    CStringArray arr;
    name.Parse(arr,"()");
    if (arr.GetSize() < 1) return "";
    CString base = arr[0];
    base.TrimLeft();
    base.TrimRight();
    return base;
}

CString CMovie::GetInfo(CMyString name)
{
    CStringArray arr;
    name.Parse(arr,"()");
    if (arr.GetSize() < 2) return "";
    CString info = arr[1];
    info.TrimLeft();
    info.TrimRight();
    return info;
}

void CMovie::Start()
{
    ASSERT(m_pImg);
    ASSERT(m_pProcess);
    ASSERT(m_hWnd);
    Init();
    if (m_FuncArr.GetSize() == 0) return;
    m_Type = m_MovieType;
    m_Roi = m_pImg->GetActualRoi();
    m_pRoi = m_pImg->GetRoi() ? &m_Roi : NULL;
    m_ImgSrc = *m_pImg;
    m_ImgDst = m_ImgSrc;
    m_ImgTmp = m_ImgSrc;
    CreateMaskImage();
    //if (m_Type == MOVIE_TYPE_WANDER) {
        m_pWander->Init(m_Roi.width, m_Roi.height,
                             m_ImgMask.Width(), m_ImgMask.Height(),
                             700 / (double)GetCycle());
    //}
    m_IsPlaying = TRUE;
}

void CMovie::Stop()
{
    ASSERT(m_pImg);
    if (!m_IsPlaying) return;
    m_IsPlaying = FALSE;
    m_pImg->CopyData(&m_ImgSrc);
    m_ImgSrc.Reset();
    m_ImgDst.Reset();
    m_ImgTmp.Reset();
    ::SendMessage(m_hWnd, WM_Movie, (WPARAM)MOVIE_STOP, 0);
}

static void SetSysColor(CImage* pImg)
{
    Ipp8u value[3];
    DWORD color = ::GetSysColor(COLOR_3DFACE);
    value[2] = (Ipp8u)( color & 0x0000FF);
    value[1] = (Ipp8u)((color & 0x00FF00) >> 8 );
    value[0] = (Ipp8u)((color & 0xFF0000) >> 16);
    switch (pImg->Channels()) {
    case 1:
        {
        int gray = 0;
        for (int i=0; i<3; i++) gray += value[i];
        gray /= 3;
        ippiSet_8u_C1R(gray,
            (Ipp8u*)pImg->GetRoiPtr(), pImg->Step(), pImg->GetRoiSize());
        }
        break;
    case 3:
        ippiSet_8u_C3R(value,
            (Ipp8u*)pImg->GetRoiPtr(), pImg->Step(), pImg->GetRoiSize());
        break;
    case 4:
        ippiSet_8u_AC4R(value,
            (Ipp8u*)pImg->GetRoiPtr(), pImg->Step(), pImg->GetRoiSize());
        break;
    }
}

int CMovie::GetPerfCycle(int cycle)
{
    double slider = 10.0*(double)(cycle -  GetMinCycle())/(double)(GetMaxCycle() - GetMinCycle()); // slider value is in [0, 10]
    return (int)(slider*MovieSlider_PerfFactor + 0.5) + 1;
}

double CMovie::GetRythmTimeSec(int cycle)
{
    return GetPerfCycle(cycle)*m_MinCycle*m_TimeShot*1.e-3;
}

int CMovie::Play(int tim)
{
    ASSERT(m_pImg);
    int funcIdx = GetFuncIndex(tim);
    CString name = m_NameArr[funcIdx];
    CFunc func = m_FuncArr[funcIdx];
    CippiRun* pRun = (CippiRun*)m_pRunArr[funcIdx];
    int result = 1;
    CMyString dummyArg;
    m_Info = func;


    if (m_Type == MOVIE_TYPE_ARG) 
    {
        if (pRun->IsMovie(func) & RUNMOVIE_COPYSRC)
            SetSysColor(m_pImg);
        result = pRun->ProcessMovie(func,
            &m_ImgSrc, m_pImg, &m_ImgTmp, tim,
            GetInfo(name), dummyArg);
    }
    else if (m_Type == MOVIE_TYPE_PERF) 
    {
        //if (pRun->IsMovie(func) & RUNMOVIE_COPYSRC)
        //    SetSysColor(m_pImg);
        int numCycles = GetPerfCycle(GetCycle());
        Ipp64u clocks = ippGetCpuClocks();
        for (int i=0; i < numCycles; i++)
        {
            result = pRun->ProcessMovie(func,
                &m_ImgSrc, m_pImg, &m_ImgTmp, tim,
                GetInfo(name), dummyArg);
            if (!result)
                break;
        }
        clocks = ippGetCpuClocks() - clocks;
        Ipp32u funcLoops = m_LoopsArr[funcIdx] + numCycles;
        if (funcLoops > m_LoopsArr[funcIdx])
        {
            m_ClocksArr.ElementAt(funcIdx) =  m_ClocksArr[funcIdx] + (double)clocks;
            m_LoopsArr.ElementAt(funcIdx) = funcLoops;
        }
        CPerf perf(m_ClocksArr[funcIdx]/m_LoopsArr[funcIdx], m_pImg->GetActualSize().width*m_pImg->GetActualSize().height, "pixel");
        m_Info += _T("\t\t") + perf.GetString();
    } else if (m_Type == MOVIE_TYPE_ZOOM) {
        if (tim == 0) {
            result = pRun->ProcessMovie(func,
                &m_ImgSrc, &m_ImgDst, &m_ImgTmp, -1,
                GetInfo(name), dummyArg);
        }
        SetMaskZoom(tim);
        m_pImg->CopyData(&m_ImgSrc);
        CopyDataMask();
    } else if (m_Type == MOVIE_TYPE_WANDER) {
        m_pWander->Next();
        if (pRun->IsMovie(func) & RUNMOVIE_REDRAW) {
            SetWanderRoi();
            result = pRun->ProcessMovie(func,
                &m_ImgSrc, &m_ImgDst, &m_ImgTmp, -1,
                GetInfo(name), dummyArg);
        } else {
            if (tim == 0)
                result = pRun->ProcessMovie(func,
                    &m_ImgSrc, &m_ImgDst, &m_ImgTmp, -1,
                    GetInfo(name), dummyArg);
            SetWanderRoi();
        }
        m_pImg->CopyData(&m_ImgSrc);
        CopyDataMask();
        SetWanderRoi(FALSE);
    }
    LPCSTR pName = tim == 0 ? (LPCSTR)m_NameArr.ElementAt(funcIdx) : NULL;
    ::SendMessage(m_hWnd, WM_Movie, (WPARAM)MOVIE_PLAY, (LPARAM)pName);
    return result;
}

int CMovie::PlayNext(int& tim)
{
    ASSERT(m_pImg);
    int funcIdx = GetFuncIndex(tim) + 1;
    if (funcIdx >= m_FuncArr.GetSize()) funcIdx = 0;
    tim = funcIdx * GetCycle();

    return Play(tim);
}

int CMovie::PlayPrev(int& tim)
{
    ASSERT(m_pImg);
    int funcIdx = GetFuncIndex(tim) - 1;
    if (funcIdx < 0) funcIdx = (int)m_FuncArr.GetSize() - 1;
    tim = funcIdx * GetCycle();

    return Play(tim);
}

int CMovie::GetFuncIndex(int& tim)
{
    int funcNum = (int)m_FuncArr.GetSize();
    int cycle = GetCycle();
    tim = tim % (funcNum * cycle);
    int idx = tim / cycle;
    tim = tim % cycle;
    return idx;
}

int CMovie::GetTimReverse(int tim)
{
    if (tim < 0) return tim;
    int cycle2 = GetCycle();
    int cycle = cycle2 >> 1;
    if (tim >= cycle) tim = cycle2 - tim - 1;
    if (tim < 0) tim = 0;
    return tim;
}

void CMovie::ZoomMask(CImage& src, CImage& dst)
{
    IppiResizeSpec_32f* pSpec = 0;
    Ipp32s specSize  = 0, initSize = 0, bufSize = 0;
    IppiSize srcSize = src.GetSize();
    IppiSize dstSize = dst.GetRoiSize();
    Ipp8u* pBuffer   = 0;
    IppiPoint dstOffset = {0, 0};

    ippiResizeGetSize_8u(srcSize, dstSize, ippNearest, 0, &specSize, &initSize);

    pSpec = (IppiResizeSpec_32f*)ippsMalloc_8u(specSize);

    ippiResizeNearestInit_8u(srcSize, dstSize, pSpec);

    ippiResizeGetBufferSize_8u(pSpec, dstSize, 1, &bufSize);

    pBuffer = ippsMalloc_8u(bufSize);

    ippiResizeNearest_8u_C1R((Ipp8u*)src.GetDataPtr(), src.Step(), (Ipp8u*)dst.GetRoiPtr(), dst.Step(), dstOffset, dstSize, pSpec, pBuffer);

    ippsFree(pSpec);
    ippsFree(pBuffer);
}

void CMovie::ZoomMask(CImage& src, CImage& dst, double factor)
{
    IppiRect srcRoiSave;
    IppiRect dstRoiSave;
    IppiRect* pSrcRoi = SaveRoi(&src, srcRoiSave);
    IppiRect* pDstRoi = SaveRoi(&dst, dstRoiSave);

    SetZoomRoi(&src,&dst,factor);

    IppiResizeSpec_32f* pSpec = 0;
    Ipp32s specSize  = 0, initSize = 0, bufSize = 0;
    IppiSize srcSize = src.GetSize();
    IppiSize dstSize = dst.GetRoiSize();
    Ipp8u* pBuffer   = 0;
    IppiPoint dstOffset = {0, 0};

    ippiResizeGetSize_8u(srcSize, dstSize, ippNearest, 0, &specSize, &initSize);

    pSpec = (IppiResizeSpec_32f*)ippsMalloc_8u(specSize);

    ippiResizeNearestInit_8u(srcSize, dstSize, pSpec);

    ippiResizeGetBufferSize_8u(pSpec, dstSize, 1, &bufSize);

    pBuffer = ippsMalloc_8u(bufSize);

    ippiResizeNearest_8u_C1R((Ipp8u*)src.GetDataPtr(), src.Step(), (Ipp8u*)dst.GetRoiPtr(), dst.Step(), dstOffset, dstSize, pSpec, pBuffer);

    ippsFree(pSpec);
    ippsFree(pBuffer);

    src.SetRoi(pSrcRoi);
    dst.SetRoi(pDstRoi);
}

IppiRect* CMovie::SaveRoi(CImage* pImg, IppiRect& roi)
{
    roi = pImg->GetActualRoi();
    return pImg->IsRoi() ? &roi : NULL;
}

void CMovie::SetZoomRoi(CImage* pSrc, CImage* pDst, double factor)
{
    IppiRect srcRoiSave = pSrc->GetActualRoi();
    IppiRect dstRoiSave = pDst->GetActualRoi();
    IppiRect srcRoi = srcRoiSave;
    IppiRect dstRoi = dstRoiSave;

    dstRoi.width  = (int)(srcRoiSave.width *factor);
    dstRoi.height = (int)(srcRoiSave.height*factor);
    dstRoi.x += (dstRoiSave.width  - dstRoi.width )>>1;
    dstRoi.y += (dstRoiSave.height - dstRoi.height)>>1;
    if (dstRoi.x < dstRoiSave.x) {
        dstRoi.width = dstRoiSave.width;
        dstRoi.x = dstRoiSave.x;
        srcRoi.width = (int)(dstRoi.width/factor);
        srcRoi.x += (srcRoiSave.width - srcRoi.width)>>1;
    }
    if (dstRoi.y < dstRoiSave.y) {
        dstRoi.height = dstRoiSave.height;
        dstRoi.y = dstRoiSave.y;
        srcRoi.height = (int)(dstRoi.height/factor);
        srcRoi.y += (srcRoiSave.height - srcRoi.height)>>1;
    }
    pSrc->SetRoi(&srcRoi);
    pDst->SetRoi(&dstRoi);
}

void CMovie::CreateMaskImage()
{
    CMovieMask* pPattern = m_pMaskArray->GetCurMask();
    if (m_Type == MOVIE_TYPE_WANDER) {
        int d = MAX(m_Roi.width, m_Roi.height) >> 2;
        int ptnWidth = pPattern->Width();
        int ptnHeight = pPattern->Height();
        double scale = sqrt((double)(d*d) / (double)(ptnWidth*ptnHeight));
        m_ImgMask.SetHeader(pp8u, 1,
            (int)(ptnWidth*scale), (int)(ptnHeight*scale));
        m_ImgMask.AllocateData(0);
        ZoomMask(*pPattern, m_ImgMask);
    } else if (m_Type == MOVIE_TYPE_ZOOM) {
        m_ImgMask.SetHeader(pp8u, 1, m_Roi.width-1, m_Roi.height-1);
        m_ImgMask.AllocateData(0);
    }
}

void CMovie::SetMaskZoom(int tim)
{
    CMovieMask* pPattern = m_pMaskArray->GetCurMask();
    double scaleW = (double)m_Roi.width /
                         (double)pPattern->GetActualRoi().width;
    double scaleH = (double)m_Roi.height /
                         (double)pPattern->GetActualRoi().height;
    double scale = MAX(scaleW, scaleH);

    m_ImgMask.SetData(0);
    tim = GetTimReverse(tim);
    double factor = (scale * tim) / (GetCycle()>>1);
    ZoomMask(*pPattern, m_ImgMask, factor);
}

void CMovie::SetWanderRoi(BOOL bWander)
{
    if (bWander) {
        IppiRect roi = m_pWander->GetRect();
        roi.x += m_Roi.x;
        roi.y += m_Roi.y;
        m_pImg->SetRoi(&roi);
        m_ImgSrc.SetRoi(&roi);
        m_ImgDst.SetRoi(&roi);
        m_ImgTmp.SetRoi(&roi);
    } else {
        m_pImg->SetRoi(m_pRoi);
        m_ImgSrc.SetRoi(m_pRoi);
        m_ImgDst.SetRoi(m_pRoi);
        m_ImgTmp.SetRoi(m_pRoi);
    }
}

void CMovie::SetCircle(CImage& img, int cx, int cy, int r2)
{
    Ipp8u* pRow = (Ipp8u*)img.GetData();
    for (int y=0; y<img.Height(); y++) {
        for (int x=0; x<img.Width(); x++) {
            int rx = x - cx;
            int ry = y - cy;
            pRow[x] = (rx*rx + ry*ry <= r2) ? 1 : 0;
        }
        pRow += img.Step();
    }
}

void CMovie::CopyDataMask()
{
    IppiSize size = {m_ImgMask.Width(),m_ImgMask.Height()};
    if (m_pImg->Type() != pp8u) return;
    switch (m_pImg->Channels()) {
    case 1:
        ippiCopy_8u_C1MR(
            (Ipp8u*)m_ImgDst.GetRoiPtr(), m_ImgDst.Step(),
            (Ipp8u*)m_pImg->GetRoiPtr(), m_pImg->Step(),
            size,
            (Ipp8u*)m_ImgMask.GetRoiPtr(),  m_ImgMask.Step());
        break;
    case 3:
        ippiCopy_8u_C3MR(
            (Ipp8u*)m_ImgDst.GetRoiPtr(), m_ImgDst.Step(),
            (Ipp8u*)m_pImg->GetRoiPtr(), m_pImg->Step(),
            size,
            (Ipp8u*)m_ImgMask.GetRoiPtr(),  m_ImgMask.Step());
        break;
    case 4:
        ippiCopy_8u_C4MR(
            (Ipp8u*)m_ImgDst.GetRoiPtr(), m_ImgDst.Step(),
            (Ipp8u*)m_pImg->GetRoiPtr(), m_pImg->Step(),
            size,
            (Ipp8u*)m_ImgMask.GetRoiPtr(),  m_ImgMask.Step());
        break;
    }
}

CFunc CMovie::GetFunc(CString baseName)
{
    CFuncList fList;
    m_pProcess->GetFunctionsWithBase(baseName,fList);
    POSITION pos;
    pos = fList.GetHeadPosition();
    while (pos) {
        POSITION delPos = pos;
        CFunc f = fList.GetNext(pos);
        if (f.Inplace()) fList.RemoveAt(delPos);
        if (f.SrcType() != f.DstType()) fList.RemoveAt(delPos);
        if (f.SrcChannels() != f.DstChannels()) fList.RemoveAt(delPos);
        if (f.SrcPlane() != f.DstPlane()) fList.RemoveAt(delPos);
        if (f.OnlyDst()) fList.RemoveAt(delPos);
    }
    if (fList.GetCount() == 0) return "";
    pos = fList.GetHeadPosition();
    while (pos) {
        CFunc f = fList.GetNext(pos);
        if (f.Scale()) return f;
    }
    return fList.GetHead();
}

CippiRun* CMovie::GetRun(CFunc func)
{
    CDirector* pDirector = DEMO_APP->GetDirector();
    POSITION pos = pDirector->GetHeadPosition();
    while (pos) {
        CippiRun *pRun = (CippiRun*)pDirector->GetNext(pos);
        if (pRun->IsFunction(func)) {
            if (pRun->IsMovie(func)) return pRun;
            return NULL;
        }
    }
    return NULL;
}
