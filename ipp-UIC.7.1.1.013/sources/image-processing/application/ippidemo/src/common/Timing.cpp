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
#include "Demo.h"
#include "DemoDoc.h"
#include "Timing.h"
#include "OptTimingDlg.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {perf_usec, perf_clocks, perf_clocks_per};

BOOL CPerf::m_ShowStatus = TRUE;
BOOL CPerf::m_ShowHisto  = TRUE;
int  CPerf::m_UnitType   = perf_clocks;
double CPerf::m_Freq     = 0;

CPerf::CPerf() 
{ 
    Reset(); 
}

CPerf::CPerf(double clocks, double divisor, LPCSTR unit) :
m_Clocks(clocks), m_Divisor(divisor), m_Unit(unit) 
{}


void CPerf::Reset()
{
    m_Clocks = 0;
    m_Divisor = 1;
    m_Unit = "";
}

void CPerf::DoTimingDialog() 
{
   COptTimingDlg dlg;

   dlg.m_ShowStatus = m_ShowStatus;
   dlg.m_ShowHisto  = m_ShowHisto ;
   dlg.m_UnitType   = m_UnitType  ;
   if (dlg.DoModal() != IDOK) return;
   bool updateStatus = (m_ShowStatus != dlg.m_ShowStatus) || (m_UnitType != dlg.m_UnitType);
   m_ShowStatus = dlg.m_ShowStatus;
   m_ShowHisto  = dlg.m_ShowHisto ;
   m_UnitType   = dlg.m_UnitType  ;
   if (updateStatus)
   {
       MY_POSITION pos = DEMO_APP->GetFirstDocPosition();
       while (pos)
          DEMO_APP->GetNextDoc(pos)->UpdateStatusString();
   }

}

CString CPerf::GetString() 
{
    if (m_Clocks > 0)
        return GetValueString() + " " + GetUnitString();
    else 
        return "";
}

static CString perfString_f(double perf)
{
   CString str;
   str.Format("%.3g", perf);
   return str;
}

static CString perfString_e(double perf)
{
   CString str;
   str.Format("%.2e", perf);
   return str;
}

double CPerf::GetFreq()
{
    if (m_Freq == 0)
    {
        int freq;
        if (ippGetCpuFreqMhz(&freq) == ippStsNoErr)
            m_Freq = (double)freq;
        else
            m_Freq = -1;
    }
    return m_Freq;
}

int CPerf::GetType()
{
    int type = m_UnitType;
    if ((type == perf_clocks_per) && ((m_Divisor <= 1) || m_Unit.IsEmpty()))
        type = perf_clocks;
    if ((type == perf_usec) && (GetFreq() <= 0))
        type = perf_clocks;
    return type;
}

CString CPerf::GetValueString()
{
    switch (GetType())
    {
    case perf_usec:
        return perfString_e(m_Clocks/GetFreq());
    case perf_clocks:
        return perfString_e(m_Clocks);
    case perf_clocks_per:
        return perfString_f(m_Clocks/m_Divisor);
    }
    return "";
}

CString CPerf::GetUnitString()
{
    switch (GetType())
    {
    case perf_usec:
        return "Usec";
    case perf_clocks:
        return "CPU clocks";
    case perf_clocks_per:
        return "CPU clocks per " + m_Unit;
    }
    return "";
}

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
   //COptTimingDlg dlg;

   //dlg.m_Do     = m_Do;
   //dlg.m_Method = m_Method;
   //dlg.m_Number = m_Number;   
   //if (dlg.DoModal() != IDOK) return;
   //m_Do     = dlg.m_Do;
   //m_Method = dlg.m_Method;
   //m_Number = dlg.m_Number;   
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
