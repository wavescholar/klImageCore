/*
//
//                INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __TIMER_H__
#include "timer.h"
#endif


CTimer::CTimer(void)
{
  m_c0 = 0;
  m_c1 = 0;

#ifdef _WIN32
  m_freq.QuadPart = 0;
  m_t0.QuadPart = 0;
  m_t1.QuadPart = 0;

  QueryPerformanceFrequency(&m_freq);
#else
  m_ifreq = 0;
#endif
  return;
} // ctor


CTimer::~CTimer(void)
{
  return;
} // dtor


void CTimer::Init(void)
{
#ifdef _WIN32
  QueryPerformanceFrequency(&m_freq);
#else
    if(m_ifreq == 0)
        ippGetCpuFreqMhz(&m_ifreq);
#endif
  return;
} // CTimer::Init()


void CTimer::Start(void)
{
#ifdef _WIN32
  QueryPerformanceCounter(&m_t0);
#endif

#ifdef linux
  m_t0 = clock();
#endif

  m_c0 = ippGetCpuClocks();

  return;
} // CTimer::Start()


void CTimer::Stop(void)
{
#ifdef _WIN32
  QueryPerformanceCounter(&m_t1);
#endif

  m_c1 = ippGetCpuClocks();

  return;
} // CTimer::Stop()


void CTimer::RaisePriority(void)
{
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
  Sleep(0);
#endif
  return;
} // CTimer::RaisePriority()


void CTimer::LowerPriority(void)
{
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
#endif
  return;
} // CTimer::LowerPriority()


Ipp64u CTimer::GetCpuClocks(void)
{
  return (m_c1 - m_c0);
} // CTimer::GetCpuClocks()


double CTimer::GetCPE(int nOfElements)
{
  return (double)GetCpuClocks() / nOfElements;
} // CTimer::GetCPE()


double CTimer::GetTime(CTimer::units u)
{
  double t;
#ifdef _WIN32
  t = (double)(m_t1.QuadPart - m_t0.QuadPart) / m_freq.QuadPart;
#else
  t = (double)(m_c1 - m_c0) / (m_ifreq*MSEC*MSEC);
#endif

  switch(u)
  {
  case usec: t = (USEC)*t; break;
  case msec: t = (MSEC)*t; break;
  case sec:
  default:
    break;
  }

  return t;
} // CTimer::GetTime()

