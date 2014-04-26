//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2012 Intel Corporation. All Rights Reserved.
//
//
//

#include "vm_time.h"

#ifndef __TIMER_H__
#define __TIMER_H__

class BaseTimer {
public:
   BaseTimer(){m_nextTime=0;}
   virtual void SetNextTime(unsigned int nextTime) = 0;
   virtual int isTime() = 0;
   virtual void SetTimeInc(unsigned int uiTimeInc) = 0;
   virtual void Continue(){};
protected:
   unsigned int m_nextTime;
};

class FreeTimer : public BaseTimer {
public:
   FreeTimer() : BaseTimer(){}
   virtual void SetNextTime(unsigned int /*uiTimeInc*/){};
   virtual int isTime(){return 1;}
protected:
   virtual void SetTimeInc(unsigned int /*uiTimeInc*/){};
};

class RTTimer : public BaseTimer {
public:
   RTTimer() : BaseTimer(){m_nextTime.tv_sec = m_nextTime.tv_usec = 0;}
   virtual void SetNextTime(unsigned int nextTime);
   virtual int isTime();
protected:
   virtual void SetTimeInc(unsigned int /*uiTimeInc*/){};
   vm_timeval m_nextTime;
};

class SampleTimer : public BaseTimer {
public:
   SampleTimer() : BaseTimer(){m_nextTime=0;m_TimeInc=0;m_CurrTime=0;}
   virtual void SetNextTime(unsigned int nextTime){m_nextTime = nextTime;}
   virtual int isTime();
   virtual void SetTimeInc(unsigned int uiTimeInc){m_TimeInc = uiTimeInc;}
   virtual void Continue(){m_CurrTime+=m_TimeInc;}
protected:
   unsigned int m_nextTime;
   unsigned int m_TimeInc;
   unsigned int m_CurrTime;
};

#endif//__TIMER_H__
