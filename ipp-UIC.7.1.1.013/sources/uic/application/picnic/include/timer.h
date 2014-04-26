/*
//
//                INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef linux
#include <time.h>
#endif
#ifdef __APPLE__
#include <time.h>
#endif
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif


#define USEC (double)1000000.0
#define MSEC (double)1000.0

class CTimer
{
public:
  enum units
  {
    sec, msec, usec
  };

  CTimer(void);
  virtual ~CTimer(void);

  void Init(void);
  void Start(void);
  void Stop(void);

  void RaisePriority();
  void LowerPriority();

  Ipp64u GetCpuClocks(void);

  double GetCPE(int nOfElements);
  double GetTime(CTimer::units u = msec);

private:
  Ipp64u m_c0;
  Ipp64u m_c1;
#ifdef _WIN32
  LARGE_INTEGER m_freq;
  LARGE_INTEGER m_t0;
  LARGE_INTEGER m_t1;
#else
  Ipp32s  m_ifreq;
  clock_t m_t0;
  clock_t m_t1;
#endif
};

#endif
