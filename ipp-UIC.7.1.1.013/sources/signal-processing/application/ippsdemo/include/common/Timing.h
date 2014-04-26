/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Timing.h: interface for the CTiming class.
//
// class CTiming provides performance measurement for IPP function
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMING_H__BCD0AD06_69C8_4714_8C3F_8F8F0B063767__INCLUDED_)
#define AFX_TIMING_H__BCD0AD06_69C8_4714_8C3F_8F8F0B063767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {timAUTO, timLOOP, timSTAT, timNUM};

class CTiming  
{
public:
   CTiming() { Reset();}
   virtual ~CTiming() {}
   
   void Reset() { m_Clocks = 0; m_Ticks = -1; m_TicksPrev = -1;}
   void Start();
   void Stop(int numLoops = 1);
   double GetTicks() { return m_Ticks;}
   double GetAccuracy();

   static void LoadProfileSettings();
   static void SaveProfileSettings();
   static void DoTimingDialog();
   static BOOL GetFlag()    { return m_Do;}
   static int  GetMethod()  { return m_Method;}
   static int  GetNumber()  { return m_Number;}
   static void SetFlag(BOOL mode)    { m_Do = mode;}
   static void SetMethod(int mode)  { m_Method = mode;}
   static void SetNumber(int mode)  { m_Number = mode;}

protected:
   double m_Ticks;
   double m_TicksPrev;
   Ipp64u m_Clocks;

   static BOOL m_Do;
   static int  m_Method;
   static int  m_Number;
};

class CTicker  
{
public:
   CTicker() { Reset();}
   virtual ~CTicker() {}
   
   void Reset() { m_Ticks = 0; m_StartClocks = 0;}
   void Start() { 
       SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
       SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );
       m_StartClocks = ippGetCpuClocks();
   }
   void Stop() { 
       m_Ticks += (double)(Ipp64s)( ippGetCpuClocks() - m_StartClocks);
       SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
       SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
   }
   double Ticks() { return m_Ticks;}

protected:
   double m_Ticks;
   Ipp64u m_StartClocks;
};

#endif // !defined(AFX_TIMING_H__BCD0AD06_69C8_4714_8C3F_8F8F0B063767__INCLUDED_)
