/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Movie.h: interface for the CMovie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVIE_H__3AC6B718_FE0F_421F_A961_1BED75F91B16__INCLUDED_)
#define AFX_MOVIE_H__3AC6B718_FE0F_421F_A961_1BED75F91B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"
#include "Image.h"
#include "ippiRun.h"

class CippiRun;
class CProcess;
class CWander;
class CMovieMaskArray;

enum { MOVIE_STOP, MOVIE_PLAY, MOVIE_PLAYNEXT, MOVIE_PLAYPREV,
       MOVIE_PAUSE, MOVIE_CANCEL };
enum { MOVIE_TYPE_ARG, MOVIE_TYPE_PERF, MOVIE_TYPE_ZOOM, MOVIE_TYPE_WANDER};


class CMovieFunc : public CString
{
public:
    CMovieFunc() : bHot(FALSE), bPerf(FALSE) {}
    CMovieFunc(CString name, BOOL hot, BOOL perf) : CString(name), bHot(hot), bPerf(perf) {}
    BOOL bHot;
    BOOL bPerf;
};
class CMovieFuncArray : public CArray<CMovieFunc, CMovieFunc> {};


#define WM_MOVIE "MovieMessage"

class CMovie
{
public:
   CMovie(CImage* pImg, CProcess* pProcess, HWND hWnd);
   virtual ~CMovie();

   static void InitStatic();
   static void ClearFunctions();
   static void AddFunction(CString name, BOOL bHot = FALSE);
   static CMovieFuncArray* GetFullArray() { return &m_FullArray;}
   static int FindInFull(CString name);
   static int FindInHot(CString name);
   static int FindInPerf(CString name);
   static void SetType(int val) { m_MovieType = val;}
   static int GetType() { return m_MovieType;}
   static void SetCycle(int val) { m_MovieCycle = val;}
   static int GetCycle() { return m_MovieCycle;}
   static int GetMinCycle() { return m_MinCycle;}
   static int GetMaxCycle() { return m_MaxCycle;}
   static int GetTimeShot() { return m_TimeShot;}
   static int GetTimReverse(int tim);
   static int GetPerfCycle(int cycle);
   static double GetRythmTimeSec(int cycle);
   static CMovieMaskArray* GetMaskArray() { return m_pMaskArray;}
   static CString GetHotString();
   static void SetHotString(CMyString str);
   static IppiRect* SaveRoi(CImage* pImg, IppiRect& roi);
   static void SetZoomRoi(CImage* pSrc, CImage* pDst, double factor);

   void Init();
   void ResetInitFlag() { m_IsInit = FALSE;}
   void ResetClocks();
   void Start();
   int  Play(int tim);
   int  PlayNext(int& tim);
   int  PlayPrev(int& tim);
   void Stop();
   int  GetNumMovies();
   int  GetPlayType() { return m_Type;}
   CString  GetInfo() { return m_Info;}
   BOOL IsPlaying() { return m_IsPlaying;}
   BOOL IsImagePlane() { return m_pImg->Plane(); } /// chan. to disable movie

protected:
   static CMovieFuncArray m_FullArray;
   static int m_MovieType;
   static int m_MovieCycle;
   static int m_MinCycle;
   static int m_MaxCycle;
   static int m_TimeShot;
   static CMovieMaskArray* m_pMaskArray;

   CImage*   m_pImg;
   CProcess* m_pProcess;
   HWND      m_hWnd;
   BOOL      m_IsInit;
   BOOL      m_IsPlaying;
   CStringArray m_NameArr;
   CFuncArray   m_FuncArr;
   CRunArray    m_pRunArr;
   CArray<double, double> m_ClocksArr;
   CArray<Ipp32u, Ipp32u> m_LoopsArr;
   CImage    m_ImgSrc;
   CImage    m_ImgDst;
   CImage    m_ImgTmp;
   CImage    m_ImgMask;
   int       m_Type;
   CWander*  m_pWander;
   IppiRect  m_Roi;
   IppiRect* m_pRoi;
   CMyString m_Info;

   static void InitFullArray();
   static void InitMaskArray();
   CFunc GetFunc(CString baseName);
   CippiRun* GetRun(CFunc func);
   void  ReleaseHotFunc();
   BOOL  InitHotFunc(CMyString hotBase);
   CString GetBase(CMyString name);
   CString GetInfo(CMyString name);
   int   GetFuncIndex(int& tim);
   void CreateMaskImage();
   void CreateWanderImage();
   void SetMaskArg(int tim);
   void SetMaskZoom(int tim);
   void SetMaskWander();
   void SetWanderRoi(BOOL bWander = TRUE);
   void SetCircle(CImage& img, int x, int y, int r2);
   void CopyDataMask();
   void ZoomMask(CImage& src, CImage& dst);
   void ZoomMask(CImage& src, CImage& dst, double factor);
};

#endif // !defined(AFX_MOVIE_H__3AC6B718_FE0F_421F_A961_1BED75F91B16__INCLUDED_)
