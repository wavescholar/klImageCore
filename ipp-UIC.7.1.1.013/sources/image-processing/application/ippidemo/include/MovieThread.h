/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieThread.h: interface for the CMovieThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVIETHREAD_H__7366843C_6F56_4602_8D44_CAAC605683B4__INCLUDED_)
#define AFX_MOVIETHREAD_H__7366843C_6F56_4602_8D44_CAAC605683B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sys/timeb.h>
class CippiDemoDoc;
class CMovie;

class CMovieThread
{
public:
   CMovieThread(CippiDemoDoc* pDoc);
   virtual ~CMovieThread();
   BOOL Begin();
   void Terminate();
   UINT Play();
protected:
   CippiDemoDoc* m_pDoc;
   HANDLE m_hThread;
   CMovie*   m_pMovie;
   int       m_Tim;
   _timeb    m_TimeB;
   bool      m_ShowPerformance;

   void Start();
   int  PlayNext();
   void StartTimer(int playType);
   void NextTimer(int numTicks);
};

#endif // !defined(AFX_MOVIETHREAD_H__7366843C_6F56_4602_8D44_CAAC605683B4__INCLUDED_)
