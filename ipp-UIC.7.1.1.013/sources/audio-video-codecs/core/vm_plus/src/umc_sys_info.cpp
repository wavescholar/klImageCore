/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_sys_info.h"
#include "vm_sys_info.h"
#include "vm_debug.h"
#include <string.h>

using namespace UMC;


SysInfo::SysInfo(vm_char *pProcessName)
{
    memset(&m_sSystemInfo,0,sizeof(sSystemInfo));
    if (!pProcessName)
    {
        vm_sys_info_get_program_name(m_sSystemInfo.program_name);
    }
    else
    {
        vm_string_strcpy(m_sSystemInfo.program_name, pProcessName);
    }
    vm_sys_info_get_program_path(m_sSystemInfo.program_path);
    vm_sys_info_get_os_name(m_sSystemInfo.os_name);
    vm_sys_info_get_cpu_name(m_sSystemInfo.proc_name);
    vm_sys_info_get_vga_card(m_sSystemInfo.video_card);
    vm_sys_info_get_computer_name(m_sSystemInfo.computer_name);
    m_sSystemInfo.cpu_freq = vm_sys_info_get_cpu_speed();
    m_sSystemInfo.phys_mem = vm_sys_info_get_mem_size();
    m_sSystemInfo.num_proc = vm_sys_info_get_cpu_num();
    CpuUsageRelease();
}

SysInfo::~SysInfo(void)
{
}

Ipp64f SysInfo::GetCpuUsage(void)
{
    Ipp64f cpu_use = 0;
    vm_tick user_time_cur = 0;
    vm_tick total_time_cur = 0;
    GetCpuUseTime(m_sSystemInfo.program_name, &user_time_cur, &total_time_cur);
    if (!user_time_cur)
    {
        return cpu_use;
    }
    if ((user_time) && (total_time))
    {
        Ipp64f dUserTime = (Ipp64f)(Ipp64s)((user_time_cur - user_time));
        Ipp64f dTotalTime = (Ipp64f)(Ipp64s)((total_time_cur - total_time));
        Ipp64f dUserTimeAvg = (Ipp64f)(Ipp64s)((user_time_cur - user_time_start));
        Ipp64f dTotalTimeAvg = (Ipp64f)(Ipp64s)((total_time_cur - total_time_start));
#if defined _WIN32_WCE
        cpu_use = 100 - ((dUserTime) / (dTotalTime)) * 100;
        avg_cpuusage = 100 - ((dUserTimeAvg) / (dTotalTimeAvg)) * 100;
#else
        cpu_use = ((dUserTime) / (dTotalTime)) * 100;
        avg_cpuusage = ((dUserTimeAvg) / (dTotalTimeAvg)) * 100;
#endif
     }
     else
     {
         user_time_start = user_time_cur;
         total_time_start = total_time_cur;
     }

     if ((cpu_use >= 0) && (cpu_use <= 100))
         last_cpuusage = cpu_use;
     else
         cpu_use = last_cpuusage;
     if (cpu_use > max_cpuusage)
         max_cpuusage = cpu_use;

    user_time = user_time_cur;
    total_time = total_time_cur;
    return cpu_use;
}

sSystemInfo *SysInfo::GetSysInfo(void)
{
    return &m_sSystemInfo;
}

void SysInfo::GetCpuUseTime(vm_char* proc_name, vm_tick* process_use, vm_tick* total_use)
{
#if defined _WIN32_WCE
    *process_use = GetIdleTime();
    *total_use = GetTickCount();
#elif defined WINDOWS
    NTSTATUS Status;
    PSYSTEM_PROCESSES pProcesses;
    HINSTANCE hNtDll;
    HANDLE hHeap = GetProcessHeap();
    ULONG cbBuffer = 0x8000;
    PVOID pBuffer = NULL;
#ifndef UNICODE
    vm_char szProcessName[MAX_PATH];
#endif

    NTSTATUS (WINAPI * _ZwQuerySystemInformation)(UINT, PVOID, ULONG, PULONG);

    // get handle NTDLL.DLL
    hNtDll = GetModuleHandle(_T("ntdll.dll"));
    VM_ASSERT(hNtDll != NULL);

    // find  address ZwQuerySystemInformation
    *(FARPROC *)&_ZwQuerySystemInformation =
        GetProcAddress(hNtDll, "ZwQuerySystemInformation");
    if (_ZwQuerySystemInformation == NULL)
        return; //SetLastError(ERROR_PROC_NOT_FOUND), FALSE;
    do
    {
        pBuffer = HeapAlloc(hHeap, 0, cbBuffer);
        if (pBuffer == NULL)
            return; //SetLastError(ERROR_NOT_ENOUGH_MEMORY), FALSE;

        Status = _ZwQuerySystemInformation(SystemProcessesAndThreadsInformation,
                                           pBuffer, cbBuffer, NULL);

        if (Status == STATUS_INFO_LENGTH_MISMATCH)
        {
            HeapFree(hHeap, 0, pBuffer);
            cbBuffer *= 2;
        }
        else if (!NT_SUCCESS(Status))
        {
            HeapFree(hHeap, 0, pBuffer);
            return; //SetLastError(Status), FALSE;
        }
    }
    while (Status == STATUS_INFO_LENGTH_MISMATCH);

    pProcesses = (PSYSTEM_PROCESSES)pBuffer;

    for (;;)
    {
        PCWSTR pszProcessName = pProcesses->ProcessName.Buffer;
        if (pszProcessName == NULL)
            pszProcessName = L"Idle";
        const vm_char *pProcessName;

#ifdef UNICODE
        pProcessName = pszProcessName;
#else
        WideCharToMultiByte(CP_ACP, 0, pszProcessName, -1, szProcessName, MAX_PATH, NULL, NULL);
        pProcessName = szProcessName;
#endif

        if (!vm_string_strnicmp(pProcessName, proc_name, 15)) { // 15???
            *process_use += pProcesses->UserTime.QuadPart;
            *process_use += pProcesses->KernelTime.QuadPart;
        }

        *total_use += pProcesses->UserTime.QuadPart;
        *total_use += pProcesses->KernelTime.QuadPart;

        if (pProcesses->NextEntryDelta == 0)
            break;

        // find the address of the next process structure
        pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
            + pProcesses->NextEntryDelta);
    }

    HeapFree(hHeap, 0, pBuffer);
#endif
}

void SysInfo::CpuUsageRelease(void)
{
    user_time = 0;
    max_cpuusage = 0;
    avg_cpuusage = 0;
    total_time = 0;
    last_cpuusage = 0;
}
