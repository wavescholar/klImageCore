/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieThread.cpp: implementation of the CMovieThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "MainFrm.h"
#include "ippiDemoDoc.h"
#include "Movie.h"
#include "MovieThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMovieThread::CMovieThread(CippiDemoDoc* pDoc)
{
    m_pDoc = pDoc;
    m_Tim = -1;
    m_TimeB.time = 0;
    m_TimeB.millitm = 0;
    m_ShowPerformance = false;
}

CMovieThread::~CMovieThread()
{

}

static UINT MovieThread(LPVOID pParam)
{
    CMovieThread* pThread = (CMovieThread*)pParam;
    return pThread->Play();
}

BOOL CMovieThread::Begin()
{
    CWinThread* pThread = AfxBeginThread(MovieThread, this);
    if (!pThread) return FALSE;
    m_hThread = pThread->m_hThread;
    return TRUE;
}

UINT CMovieThread::Play()
{
    Start();
    for (;;) {
        int result = PlayNext();
        if (result > 0)
            m_Tim += result;
        else if (result < 0)
            result = 1;
        else
        {
             m_pDoc->SetMovieStage(MOVIE_STOP);
             PlayNext();
             break;
        }
        NextTimer(result);
    }
    return 0;
}

void CMovieThread::Terminate()
{
    ::TerminateThread(m_hThread, 0);
}

void CMovieThread::Start()
{
    m_pMovie = m_pDoc->GetMovie();
    ASSERT(m_pMovie);
    m_pMovie->Start();
    m_Tim = 0;
    StartTimer(m_pMovie->GetPlayType());
}

int CMovieThread::PlayNext()
{
    m_pDoc->m_MutexFlag.Lock();
    int stage = m_pDoc->GetMovieStage();
    if (stage == MOVIE_PLAYNEXT ||
         stage == MOVIE_PLAYPREV)
        m_pDoc->SetMovieStage(MOVIE_PLAY);
    m_pDoc->m_MutexFlag.Unlock();

    switch (stage) {
    case MOVIE_PLAY:
        return m_pMovie->Play(m_Tim);
    case MOVIE_PLAYNEXT:
        return m_pMovie->PlayNext(m_Tim);
    case MOVIE_PLAYPREV:
        return m_pMovie->PlayPrev(m_Tim);
    case MOVIE_PAUSE:
        return -1;
    case MOVIE_STOP:
        m_pMovie->Stop();
        return 0;
    }
    return 0;
}

void CMovieThread::StartTimer(int playType)
{
    m_ShowPerformance = playType == MOVIE_TYPE_PERF;
    if (!m_ShowPerformance)
       _ftime(&m_TimeB);
}

void CMovieThread::NextTimer(int numTicks)
{
    if (!m_ShowPerformance)
    {
        _timeb timeB;
        _ftime(&timeB);
        int time = CMovie::GetTimeShot() * numTicks
             - (int)(timeB.time     - m_TimeB.time    )*1000
             - (int)(timeB.millitm - m_TimeB.millitm);
        if (time > 0) ::Sleep(time);
        _ftime(&m_TimeB);
    }
}

