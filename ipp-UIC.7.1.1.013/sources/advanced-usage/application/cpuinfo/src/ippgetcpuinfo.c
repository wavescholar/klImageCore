/* ////////////////////////// ippgetcpuinfo.c ////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//     Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "ipp.h"
#include "ippcpuinfo.h"

#if !defined( _ARCH_IA64 ) && !defined( _ARCH_XSC )
extern void ownGetReg( int* buf, int valEAX, int valECX );
#endif
/*=================================================================*/
IppStatus ippCPUInfoInitAlloc( IppCPUInfo* cpuInfo )
{
#if defined( _ARCH_IA64 ) || defined( _ARCH_XSC )
  return ippStsNotSupportedCpu;
#else
  {
  int cpuType = 1;
  int buf4[4], *p32s = NULL, *ptrNext;
  int nBlock0x, nBlock04, nBlock0b, nBlock8x;
  int n;

  ownGetReg( buf4, 0, 0 );

  if( (buf4[1] == 0x756e6547)&&(buf4[2] == 0x6c65746e)&&(buf4[3] == 0x49656e69) ) cpuType = 0;

  if( cpuType ) return ippStsNotSupportedCpu;

  if( cpuInfo == NULL ) return ippStsNullPtrErr;

  /************ Initial initialization *********/
  cpuInfo->DataFunction0x.IndexBlockCPUID = -1;
  cpuInfo->DataFunction0x.ptrDataCPUID = NULL;
  cpuInfo->DataFunction04.IndexBlockCPUID = -1;
  cpuInfo->DataFunction04.ptrDataCPUID = NULL;
  cpuInfo->DataFunction0b.IndexBlockCPUID = -1;
  cpuInfo->DataFunction0b.ptrDataCPUID = NULL;
  cpuInfo->DataFunction8x.IndexBlockCPUID = -1;
  cpuInfo->DataFunction8x.ptrDataCPUID = NULL;

  /*************** Memory allocation ***************/
  ownGetReg( buf4, 0, 0 );
  nBlock0x = buf4[0];

  nBlock04 = -1;
  if( nBlock0x >= 4 ) {
    for( nBlock04 = 1; nBlock04 < 32; nBlock04++ ) {
      ownGetReg( buf4, 4, nBlock04 );
      if( !(buf4[0]&0x1f) ) break;
    }
    if( (buf4[0]&0x1f) != 0 ) {
      cpuInfo->Warning |= CPUID04;
      nBlock04 = 0;
    }
    nBlock04 -= 1;
  }
  nBlock0b = -1;
  if( nBlock0x >= 0xb ) {
    for( nBlock0b = 1; nBlock0b < 32; nBlock0b++ ) {
      ownGetReg( buf4, 0xb, nBlock0b );
      if( !(buf4[0] | buf4[1]) ) break;
    }
    if( (buf4[0] | buf4[1]) ) {
      cpuInfo->Warning |= CPUID0B;
      nBlock0b = 0;
    }
    nBlock0b -= 1;
  }
  ownGetReg( buf4, 0x80000000, 0 );
  nBlock8x = buf4[0]&0x7fffffff;
  ptrNext = (int*)ippMalloc( ( ((nBlock0x+1) + (nBlock04+1) + (nBlock0b + 1) + (nBlock8x+1))*4 )*sizeof(int) );
  if( !ptrNext ) return ippStsMemAllocErr;
  cpuInfo->ptrBuffer = ptrNext;

  /*********************** Setting of pointers ***********************/
  cpuInfo->DataFunction0x.IndexBlockCPUID = nBlock0x;
  cpuInfo->DataFunction0x.ptrDataCPUID = (CPUBlockCPUID*)ptrNext;
  ptrNext += (nBlock0x+1)*4;
  cpuInfo->DataFunction04.IndexBlockCPUID = nBlock04;
  if( nBlock04 >=0 ) {
    cpuInfo->DataFunction04.ptrDataCPUID = (CPUBlockCPUID*)ptrNext;
    ptrNext += (nBlock04+1)*4;
  } else {
    cpuInfo->DataFunction04.ptrDataCPUID = (CPUBlockCPUID*)NULL;
  }
  cpuInfo->DataFunction0b.IndexBlockCPUID = nBlock0b;
  if( nBlock0b >=0 ) {
    cpuInfo->DataFunction0b.ptrDataCPUID = (CPUBlockCPUID*)ptrNext;
    ptrNext += (nBlock0b+1)*4;
  } else {
    cpuInfo->DataFunction0b.ptrDataCPUID = (CPUBlockCPUID*)NULL;
  }
  cpuInfo->DataFunction8x.IndexBlockCPUID = nBlock8x;
  cpuInfo->DataFunction8x.ptrDataCPUID = (CPUBlockCPUID*)ptrNext;

  /************ Loading of binary data ************/
  for( n = 0; n <= nBlock0x; n++ ) {
    p32s = (int*)(&cpuInfo->DataFunction0x.ptrDataCPUID[n]);
    ownGetReg( p32s, n, 0 );
  }
  for( n = 0; n <= nBlock04; n++ ) {
    p32s = (int*)(&cpuInfo->DataFunction04.ptrDataCPUID[n]);
    ownGetReg( p32s, 4, n );
  }
  for( n = 0; n <= nBlock0b; n++ ) {
    p32s = (int*)(&cpuInfo->DataFunction0b.ptrDataCPUID[n]);
    ownGetReg( p32s, 0xb, n );
  }
  for( n = 0; n <= nBlock8x; n++ ) {
    p32s = (int*)(&cpuInfo->DataFunction8x.ptrDataCPUID[n]);
    ownGetReg( p32s, n|0x80000000, 0 );
  }
  /*************************************************/
  return  ippStsNoErr;
  }
#endif
}

/*=================================================================*/
IppStatus ippCPUInfoFree( IppCPUInfo* cpuInfo )
{
  if( cpuInfo == NULL ) return ippStsNullPtrErr;
  if( cpuInfo->ptrBuffer == NULL ) return ippStsNullPtrErr;

  ippFree( cpuInfo->ptrBuffer );

  cpuInfo->ptrBuffer = NULL;
  cpuInfo->DataFunction0x.ptrDataCPUID = NULL;
  cpuInfo->DataFunction04.ptrDataCPUID = NULL;
  cpuInfo->DataFunction0b.ptrDataCPUID = NULL;
  cpuInfo->DataFunction8x.ptrDataCPUID = NULL;

  return  ippStsNoErr;
}
