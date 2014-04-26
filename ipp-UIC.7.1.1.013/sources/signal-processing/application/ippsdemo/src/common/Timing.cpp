/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Timing.cpp: implementation of the CTiming class.
//
// class CTiming provides performance measurement for IPP function
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "Timing.h"
#include "OptTimingDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CTiming::m_Do     = TRUE;

int  CTiming::m_Method = timAUTO;
int  CTiming::m_Number = 10;  

void CTiming::Start()
{
   SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
   SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );
   m_Clocks = ippGetCpuClocks();
}

void CTiming::Stop(int numLoops)
{
   if (m_Clocks > 0)
      m_Clocks = ippGetCpuClocks() - m_Clocks;
   SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
   SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
   m_TicksPrev = m_Ticks;
   m_Ticks = (double)(Ipp64s)m_Clocks / (double)numLoops;
   m_Clocks = 0;
}

double CTiming::GetAccuracy()
{
   if (m_Ticks < 0 || m_TicksPrev < 0) return 1;
   double asub = m_Ticks - m_TicksPrev;
   if (asub < 0) asub = -asub;
   return asub / m_Ticks;
}

void CTiming::DoTimingDialog() 
{
   COptTimingDlg dlg;

   dlg.m_Do     = m_Do;
   dlg.m_Method = m_Method;
   dlg.m_Number = m_Number;   
   if (dlg.DoModal() != IDOK) return;
   m_Do     = dlg.m_Do;
   m_Method = dlg.m_Method;
   m_Number = dlg.m_Number;   
}

void CTiming::LoadProfileSettings()
{
   m_Do     = DEMO_APP->GetProfileInt(_T("Timing"),_T("Flag"),1);
   m_Method = DEMO_APP->GetProfileInt(_T("Timing"),_T("Method"),timAUTO);
   m_Number = DEMO_APP->GetProfileInt(_T("Timing"),_T("Number"),10);
} 

void CTiming::SaveProfileSettings()
{
   DEMO_APP->WriteProfileInt(_T("Timing"),_T("Flag")   ,m_Do    );
   DEMO_APP->WriteProfileInt(_T("Timing"),_T("Method") ,m_Method);
   DEMO_APP->WriteProfileInt(_T("Timing"),_T("Number") ,m_Number);
}
