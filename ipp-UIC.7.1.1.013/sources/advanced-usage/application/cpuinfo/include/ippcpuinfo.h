/* ////////////////////////// ippcpuinfo.h ////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//     Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __IPPCPUINFO_H__
#define __IPPCPUINFO_H__

#if !defined(NULL)
#define NULL    ((void *)0)
#endif

/* Warnings */
#define CPUID04   0x01      /* CPUID.04 (EAX[4:0]!= 0)&&(ECX[31:0]>31) ) */
#define CPUID0B   0x02      /* CPUID.0B ((EAX[31:0]!= 0)||(EBX[31:0]!=0))&&(ECX[31:0]>31) */

/******************************/
typedef struct _CPUBlockCPUID {
    int   eax;
    int   ebx;
    int   ecx;
    int   edx;
} CPUBlockCPUID;

typedef struct _CPUBinaryData {
    int             IndexBlockCPUID;  /* Index in terms of the CPUBlockCPUID */     
    CPUBlockCPUID*  ptrDataCPUID;
} CPUBinaryData;

typedef struct _IppCPUInfo {
  int*            ptrBuffer;          /* The pointer to the allocated memory */
  unsigned int    Warning;
  CPUBinaryData   DataFunction0x;     /* Binary data of CPUID.0-CPUID.x; "x" depends on the cpu */
  CPUBinaryData   DataFunction04;     /* Binary data of CPUID.4; data size depends on the cpu */
  CPUBinaryData   DataFunction0b;     /* Binary data of CPUID.0B; data size depends on the cpu */
  CPUBinaryData   DataFunction8x;     /* Binary data of CPUID.8000000h-8000000xh; "x" depends on the cpu */
} IppCPUInfo;

/***************************************************/
IppStatus ippCPUInfoInitAlloc( IppCPUInfo* cpuInfo );
IppStatus ippCPUInfoFree( IppCPUInfo* cpuInfo );

#endif /* __IPPCPUINFO_H__ */
