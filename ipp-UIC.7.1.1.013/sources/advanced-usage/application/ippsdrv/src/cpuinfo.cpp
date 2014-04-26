
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "stdafx.h"
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __CPUINFO_H__
#include "cpuinfo.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CCPUInfo,CObject,1)


//
// external functions
//
//

CString& CCPUInfo::GetCpuName(void)
{
  IppCpuType cpuid;

  cpuid = ippGetCpuType();

  switch(cpuid)
  {
    default:
    case ippCpuUnknown:  str = "unknown"; break;

    case ippCpuPP:       str = "Intel(R) Pentium(R) processor"; break;
    case ippCpuPMX:      str = "Intel(R) Pentium(R) processor with MMX(TM) technology"; break;
    case ippCpuPPR:      str = "Intel(R) Pentium(R) Pro processor"; break;
    case ippCpuPII:      str = "Intel(R) Pentium(R) II processor"; break;
    case ippCpuPIII:     str = "Intel(R) Pentium(R) III processor and Pentium(R) III Xeon(R) processor"; break;
    case ippCpuP4:       str = "Intel(R) Pentium(R) 4 processor and Intel(R) Xeon(R) processor"; break;
    case ippCpuP4HT:     str = "Intel(R) Pentium(R) 4 Processor with HT Technology"; break;
    case ippCpuP4HT2:    str = "Intel(R) Pentium(R) 4 processor with Streaming SIMD Extensions 3"; break;
    case ippCpuCentrino: str = "Intel(R) Centrino(TM) mobile technology"; break;
    case ippCpuCoreSolo: str = "Intel(R) Core(TM) Solo processor"; break;
    case ippCpuCoreDuo:  str = "Intel(R) Core(TM) Duo processor"; break;
    case ippCpuITP:      str = "Intel(R) Itanium(R) processor"; break;
    case ippCpuITP2:     str = "Intel(R) Itanium(R) 2 processor"; break;
    case ippCpuEM64T:    str = "Intel(R) 64 Instruction Set Architecture (ISA)"; break;
    case ippCpuC2D:      str = "Intel(R) Core(TM) 2 Duo processor"; break;
    case ippCpuC2Q:      str = "Intel(R) Core(TM) 2 Quad processor"; break;
    case ippCpuPenryn:   str = "Intel(R) Core(TM) 2 processor with Intel(R) SSE4.1"; break;
    case ippCpuBonnell:  str = "Intel(R) Atom(TM) processor"; break;

    case ippCpuNehalem:
    case ippCpuNext:
    case ippCpuSSE:      str = "Processor supports Streaming SIMD Extensions instruction set"; break;
    case ippCpuSSE2:     str = "Processor supports Streaming SIMD Extensions 2 instruction set"; break;
    case ippCpuSSE3:     str = "Processor supports Streaming SIMD Extensions 3 instruction set"; break;
    case ippCpuSSSE3:    str = "Processor supports Supplemental Streaming SIMD Extension 3 instruction set"; break;
    case ippCpuSSE41:    str = "Processor supports Streaming SIMD Extensions 4.1 instruction set"; break;
    case ippCpuSSE42:    str = "Processor supports Streaming SIMD Extensions 4.2 instruction set"; break;
    case ippCpuX8664:    str = "Processor supports 64 bit extension"; break;
  }
  
  return str;
} // CCPUInfo::GetCpuName()

                 
                 
                 
                 