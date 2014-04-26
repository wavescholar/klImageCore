//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//
#include "timer.h"

void RTTimer::SetNextTime(unsigned int nextTime)
{
   vm_timeval tvNow;

   m_nextTime.tv_sec = 0;
   m_nextTime.tv_usec = nextTime * 1000;

   vm_time_gettimeofday( &tvNow, NULL );
   vm_time_timeradd(&m_nextTime,  &tvNow, &m_nextTime);
}
int RTTimer::isTime()
{
   vm_timeval tvNow;
   vm_time_gettimeofday( &tvNow, NULL );
   if(vm_time_timercmp(&tvNow, &m_nextTime, NULL) < 0) {
      vm_time_sleep(1);
      return 0;
   }
   return 1;
}

int SampleTimer::isTime()
{
   if(m_CurrTime < m_nextTime) {
      return 0;
   }
   return 1;
}
