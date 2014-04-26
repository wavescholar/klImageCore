/* ////////////////////////// samplecpuidinfo.c ////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//     Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <stdio.h>
#include <stdlib.h>
#include "ipp.h"
#include "ippcpuinfo.h"

/****************************************************************************/
/****************************************************************************/
int main( )
{
  IppStatus ownStatus = ippStsNoErr;
  IppCPUInfo  cpuInfo;
  Ipp64u  mask;
  int nBlock0x, nBlock04, nBlock0b, nBlock8x, Process = 0;
  int family, familyExt = 0, model, modelExt= 0, Cores, CMP_HTT = 0;
  int *pTmp, tmp0, tmp1;
  int MMX = 0, SSE = 0, SSE2 = 0, SSE3 = 0, SSSE3 = 0, SSE41 = 0;
  int SSE42 = 0, MOVBE = 0, EM64T = 0;
  int AES = 0, AVX = 0, CLMUL = 0, AVX_OS = 0, RDRAND = 0, F16C = 0, AVX2 = 0;
  int CacheType, CacheLevel, SelfInit, FullyAssociat, NumberThreadsSharing;
  int NumberSets, SysCoherencyLineSizeB, LinePartitions, WaysAssociat, CacheSizeKb;
  int n;
  char  *pChar;
  char  *f6m6  = "Intel(R) Celeron(R)";
  char  *f6m7  = "Intel(R) Pentium(R) III";
  char  *f6m8  = "Intel(R) Pentium(R) III";
  char  *f6m9  = "Intel(R) Pentium(R) M";
  char  *f6mA  = "Intel(R) Pentium(R) III";
  char  *f6mB  = "Intel(R) Pentium(R) III";
  char  *f6mD  = "Intel(R) Pentium(R) M";
  char  *f6mE  = "Intel(R) Core(TM)";
  char  *f6mF  = "Intel(R) Core(TM) 2";
  char  *f6m16 = "Intel(R) Celeron(R)";
  char  *f6m17 = "Intel(R) Core(TM) 2";
  char  *f6m1a = "Intel(R) Core(TM) i7";
  char  *f6m1c = "Intel(R) Atom(TM)";
  char  *f6m1d = "Intel(R) Xeon(R) MP";
  char  *fF    = "Intel(R) Pentium(R) 4";
  char  *IA64  = "Intel(R) Itanium(R)";
  char  *f6SNB = "Intel(R) Microarchitecture Code Name Sandy Bridge";
  char  *f6IVB = "Intel(R) Microarchitecture Code Name Ivy Bridge";
  char  *f6HSW = "Intel(R) Microarchitecture Code Name Haswell";

  ownStatus = ippCPUInfoInitAlloc( &cpuInfo );
  if( ownStatus != ippStsNoErr ) {
    if( ownStatus ==  ippStsNotSupportedCpu ) {
      printf("IppStatus == ippStsNotSupportedCpu\n");
      return 0;
    } else if( ownStatus ==  ippStsNullPtrErr ) {
      printf("IppStatus == ippStsNullPtrErr\n");
      return 0;
    } else if( ownStatus ==  ippStsMemAllocErr ) {
      printf("IppStatus == ippStsMemAllocErr\n");
      return 0;
    } else {
      printf("IppStatus == %d\n",ownStatus);
      return 0;
    }
  }

  nBlock0x = cpuInfo.DataFunction0x.IndexBlockCPUID;
  nBlock04 = cpuInfo.DataFunction04.IndexBlockCPUID;
  nBlock0b = cpuInfo.DataFunction0b.IndexBlockCPUID;
  nBlock8x = cpuInfo.DataFunction8x.IndexBlockCPUID;

  printf("****************\n");
  printf("The decoded data\n");
  printf("****************\n");
  printf("\n");

  if( nBlock0x >= 1 ) {

    family = (cpuInfo.DataFunction0x.ptrDataCPUID[1].eax>>8) & 0xF; /* Family ID */
    if ( family != 0x0F ) {
       familyExt = family;
    } else {
       familyExt = (cpuInfo.DataFunction0x.ptrDataCPUID[1].eax>>20) & 0xF; /* Extend Family ID */
       familyExt += family; /* extended + family */
    }

    model = (cpuInfo.DataFunction0x.ptrDataCPUID[1].eax>>4) & 0xF; /* Model ID */
    if ( (family == 0x06)||(family==0x0F) ) {
       modelExt = (cpuInfo.DataFunction0x.ptrDataCPUID[1].eax>>16) & 0xF;/* Extended Model ID */
       modelExt = (modelExt<<4) + model; /* extended + model */
    } else {
       modelExt = model;
    }

    printf("==================\n");
    printf("  Signature  \n");
    printf("Stepping ID    %X\n", ((cpuInfo.DataFunction0x.ptrDataCPUID[1].eax) & 0xF) ); /* Stepping ID */
    printf("Model ID       %X\n", model );
    printf("Ext.Model ID   %X\n", modelExt );
    printf("Family ID      %X\n", family );
    printf("Ext.Family ID  %X\n", familyExt );
    printf("Type           %X\n", ((cpuInfo.DataFunction0x.ptrDataCPUID[1].eax>>12) & 0x3) ); /* Processor Type */
    printf("\n");
  }

  /************ Feature Flags ( CPUID.1 ) ************/
  if( nBlock0x >= 1 ) {
    tmp0 = cpuInfo.DataFunction0x.ptrDataCPUID[1].edx;
    tmp1 = (((cpuInfo.DataFunction0x.ptrDataCPUID[1].ebx)>>16)&0xff)>>1;
    if( tmp1 ) tmp1 = 1;
    CMP_HTT = ((tmp0>>28)&0x1)&tmp1;
    MMX = (tmp0>>23)&0x1;
    SSE = (tmp0>>25)&0x1;
    SSE2 = (tmp0>>26)&0x1;
    tmp0 = cpuInfo.DataFunction0x.ptrDataCPUID[1].ecx;
    SSE3 = tmp0&0x1;
    SSSE3 = (tmp0>>9)&0x1;
    SSE41 = (tmp0>>19)&0x1;
    SSE42 = (tmp0>>20)&0x1;
    MOVBE = (tmp0>>22)&0x1;
    AES = (tmp0>>25)&0x1;
    AVX = (tmp0>>28)&0x1;
    CLMUL = (tmp0>>1)&0x1;
    F16C = ( cpuInfo.DataFunction0x.ptrDataCPUID[1].ecx & (1<<29) ) ? 1 : 0;
    RDRAND = ( cpuInfo.DataFunction0x.ptrDataCPUID[1].ecx & (1<<30) ) ? 1 : 0;


    ownStatus = ippGetCpuFeatures( &mask, NULL );
    if( ownStatus == ippStsNoErr ) {
      AVX_OS = (int)((mask >> 9)&0x1);
    } else {
      AVX_OS = 0;
    }
    EM64T = ((cpuInfo.DataFunction8x.ptrDataCPUID[1].edx)>>29)&0x1;
  }

  if( nBlock0x >= 7 ) {
    int isFMA = ( cpuInfo.DataFunction0x.ptrDataCPUID[1].ecx & (1<<12) ) ? 1 : 0;// cpuid(1)::ecx[12], FMA 128bits & 256bits
    int isINT256 = ( cpuInfo.DataFunction0x.ptrDataCPUID[7].ebx & (1<<5) ) ? 1 : 0; //cpuid(7)::ebx[5], AVX2 (int 256bits)
    int isGPR = ( (cpuInfo.DataFunction0x.ptrDataCPUID[7].ebx&(1<<3)) &&
       (cpuInfo.DataFunction0x.ptrDataCPUID[7].ebx&(1<<8)) ) ? 1 : 0; // cpuid(7)::ebx[3]&ebx[8], VEX-encoded GPR instructions,
    AVX2 = ( isFMA & isINT256 & isGPR ) ? 1 : 0;
  }

  /*********************************************************/
  if( ((cpuInfo.DataFunction0x.ptrDataCPUID[1].edx)>>30)&1 ) {
    pChar = IA64;
  } else {
    if( familyExt == 6 ) {
      if( modelExt == 0x6 ) {
        pChar = f6m6;
      } else if( modelExt == 0x7 ) {
        pChar = f6m7;
      } else if( modelExt == 0x8 ) {
        pChar = f6m8;
      } else if( modelExt == 0x9 ) {
        pChar = f6m9;
      } else if( modelExt == 0xa ) {
        pChar = f6mA;
      } else if( modelExt == 0xb ) {
        pChar = f6mB;
      } else if( modelExt == 0xd ) {
        pChar = f6mD;
        Process = 90;
      } else if( modelExt == 0xe ) {
        pChar = f6mE;
        Process = 65;
      } else if( modelExt == 0xf ) {
        pChar = f6mF;
        Process = 65;
      } else if( modelExt == 0x16 ) {
        pChar = f6m16;
        Process = 65;
      } else if( modelExt == 0x17 ) {
        pChar = f6m17;
        Process = 45;
     } else if( modelExt == 0x1a ) {
        pChar = f6m1a;
        Process = 45;
      } else if( modelExt == 0x1c ) {
        pChar = f6m1c;
        Process = 45;
      } else if( modelExt == 0x1d ) {
        pChar = f6m1d;
        Process = 45;
      } else {
        pChar = NULL;
      }
    } else if ( familyExt == 15 ) {
      if( modelExt == 0x0 ) {
        pChar = fF;
        Process = 180;
      } else if( modelExt == 0x1 ) {
        pChar = fF;
      } else if( modelExt == 0x2 ) {
        pChar = fF;
        Process = 130;
      } else if( modelExt == 0x3 ) {
        pChar = fF;
      } else if( modelExt == 0x4 ) {
        pChar = fF;
        Process = 90;
      } else if( modelExt == 0x6 ) {
        pChar = fF;
        Process = 45;
      } else {
        pChar = NULL;
      }
    } else {
      pChar = NULL;
    }
  }

  if ( !pChar ) {
    if ( AVX2 ) {
        pChar = f6HSW;
    } else if ( AVX & F16C & RDRAND ) {
       pChar = f6IVB;
    } else if ( AVX ) {
       pChar = f6SNB;
    } else {
       pChar = NULL;
    }
  }

  if( pChar ) {
    printf("======================================\n");
    printf("Processor: %s\n",pChar );
    printf("\n");
  }

  /*********************************************************/
  if( Process  ) {
    printf("=============================\n");
    printf("Process               %d nm\n",Process );
    printf("\n");
  }

  /*************** Processor Name / Brand String ***************/
  if( nBlock8x >= 4 ) {
    pChar = (char*)(&cpuInfo.DataFunction8x.ptrDataCPUID[2]);
    printf("=================================================\n");
    printf("BrandName: %s\n",pChar );
  } else {
    pChar = NULL;
  }

  /*************** Number of the processor cores ***************/
  if( nBlock0x >= 4 ) {
    if( nBlock0b > 0 ) {
      tmp0 = ((cpuInfo.DataFunction0b.ptrDataCPUID[0].ebx) >> 1)&0x1;
      tmp1 = cpuInfo.DataFunction0b.ptrDataCPUID[1].ebx & 0xffff;
      if( tmp0 ) tmp1 /= 2;
        Cores = tmp1;
      } else {
        Cores = ((cpuInfo.DataFunction0x.ptrDataCPUID[4].eax >> 26)&0x3f) + 1;
      }
  } else {
    Cores = 1;
  }


  printf("\n==============================================================\n");
  if( AVX2 & AVX_OS ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_h9(l9) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_h9 code for this processor\n" );
    }
  } else if( AVX & AVX_OS ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_g9(e9) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_g9 code for this processor\n" );
    }
  } else if( SSE41 ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_p8(y8) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_p8 code for this processor\n" );
    }
  } else if( MOVBE ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_s8(n8) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_s8 code for this processor\n" );
    }
  } else if( SSSE3 ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_v8(u8) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_v8 code for this processor\n" );
    }
  } else if( SSE3 ) {
    if( EM64T ) {
      printf("Intel(R) IPP would recommend using cpu_w7(m7) code for this processor\n" );
    } else {
      printf("Intel(R) IPP would recommend using cpu_w7 code for this processor\n" );
    }
  } else if( SSE2 ) {
      printf("Intel(R) IPP would recommend using cpu_w7 code for this processor\n" );
  } else {
      printf("Attention! Intel(R) IPP will not work on this CPU!\n" );
  }

  printf("\n");

  printf("================\n");
  printf("Feature Flags   \n" );
  printf("================\n");

  printf("Cores         %d - ",Cores );
  printf("Number of cores per physical package\n");

  printf("CMP / HTT     %d - ",CMP_HTT );
  printf("Multi-Cores and/or Multi-Threading\n");
  printf("\n");

  printf("EM64T      %d - ",EM64T );
  printf("CPU supports Intel(R) Extended Memory 64 Technology\n");

  printf("MOVBE      %d - ",MOVBE );
  printf("CPU supports MOVBE instruction\n");

  printf("MMX        %d - ",MMX );
  printf("CPU supports MMX(TM) technology\n");

  printf("SSE        %d - ",SSE );
  printf("CPU supports Intel(R) Streaming SIMD Extensions (Intel(R) SSE)\n");

  printf("SSE2       %d - ",SSE2 );
  printf("CPU supports Intel(R) Streaming SIMD Extensions 2 (Intel(R) SSE2)\n");

  printf("SSE3       %d - ",SSE3 );
  printf("CPU supports Intel(R) Streaming SIMD Extensions 3 (Intel(R) SSE3)\n");

  printf("SSSE3      %d - ",SSSE3 );
  printf("CPU supports Supplemental Streaming SIMD Extensions 3 (SSSE3)\n");

  printf("SSE41      %d - ",SSE41 );
  printf("CPU supports Intel(R) Streaming SIMD Extensions 4.1 (Intel(R) SSE4.1)\n");

  printf("SSE42      %d - ",SSE42 );
  printf("CPU supports Intel(R) Streaming SIMD Extensions 4.2 (Intel(R) SSE4.2)\n");

  printf("AES        %d - ",AES );
  printf("CPU supports AES instruction\n");

  printf("CLMUL      %d - ",CLMUL );
  printf("CPU supports PCLMULQDQ instruction\n");

  printf("AVX        %d - ",AVX );
  printf("CPU supports Intel(R) Advanced Vector Extensions instruction set (Intel(R) AVX)\n");

  printf("AVX_OS     %d - ",AVX_OS );
  printf("OS  supports Intel(R) AVX\n");

  printf("F16C       %d - ",F16C );
  printf("CPU supports 16-bit floating point conversion instructions\n");

  printf("RDRAND     %d - ",RDRAND );
  printf("CPU supports RDRAND instruction\n");

  printf("AVX2       %d - ",AVX2 );
  printf("CPU supports Intel(R) Advanced Vector Extensions 2 instruction set (Intel(R) AVX2)\n");

  printf("\n");

  /************ Decoding expanded data of caches ************/
  if( nBlock04 >= 0 ) {
    printf("================================\n");
    printf("Cache Parameters (Function 4)\n");
    printf("================================\n");
    printf("\n" );
    for( n = 0; n <= nBlock04; n++ ) {
      tmp0 = cpuInfo.DataFunction04.ptrDataCPUID[n].eax;
      CacheType = tmp0&0x1f;
      CacheLevel = (tmp0>>5)&0x7;
      SelfInit = (tmp0>>8)&0x1;
      FullyAssociat = (tmp0>>9)&0x1;
      NumberThreadsSharing = ((tmp0>>14)&0xfff)+1;
      tmp0 = cpuInfo.DataFunction04.ptrDataCPUID[n].ebx;
      tmp1 = cpuInfo.DataFunction04.ptrDataCPUID[n].ecx;
      NumberSets = (tmp1 += 1);
      SysCoherencyLineSizeB = (tmp0&0xfff)+1;
      tmp1 *= (tmp0&0xfff)+1;
      LinePartitions = ((tmp0>>12)&0x3ff)+1;
      tmp1 *= ((tmp0>>12)&0x3ff)+1;
      WaysAssociat = ((tmp0>>22)&0x3ff)+1;
      tmp1 *= ((tmp0>>22)&0x3ff)+1;
      CacheSizeKb = tmp1>>10;
      printf("CacheType               %d",CacheType );
      if( CacheType == 1 ) {
        printf(" - Data Cache\n" );
      } else if( CacheType == 2 ) {
        printf(" - Instruction Cache\n" );
      } else if( CacheType == 3 ) {
        printf(" - Unified Cache\n" );
      } else {
        printf("\n" );
      }
      printf("CacheLevel              %d\n",CacheLevel );
      printf("SelfInit                %d\n",SelfInit );
      printf("FullyAssociat           %d\n",FullyAssociat );
      printf("MaxNumbeThreadsSharing  %d\n",NumberThreadsSharing );
      printf("NumberSets              %d\n",NumberSets );
      printf("SysCoherencyLineSize    %d\n",SysCoherencyLineSizeB );
      printf("LinePartitions          %d\n",LinePartitions );
      printf("WaysAssociat            %d\n",WaysAssociat );
      printf("CacheSizeKb             %d\n",CacheSizeKb );
      printf("\n");
    }
    printf("\n");
  }

  printf("***************\n");
  printf("The binary data\n");
  printf("***************\n");
  /*********************************************************/
  if( cpuInfo.DataFunction0x.ptrDataCPUID ) {
    nBlock0x = cpuInfo.DataFunction0x.IndexBlockCPUID;
    printf("=====================================\n");
    printf("Largest Standard Function number: %d\n",nBlock0x );
    printf("=====================================\n");
    for( n = 0; n <= nBlock0x; n++ ) {
      printf("Function %d\n",n);
      pTmp = (int*)(&cpuInfo.DataFunction0x.ptrDataCPUID[n]);
      printf("eax = %8.8X\n",pTmp[0] );
      printf("ebx = %8.8X\n",pTmp[1] );
      printf("ecx = %8.8X\n",pTmp[2] );
      printf("edx = %8.8X\n",pTmp[3] );
    }
  }
  /*********************************************************/
  if( cpuInfo.DataFunction04.ptrDataCPUID ) {
    nBlock04 = cpuInfo.DataFunction04.IndexBlockCPUID;
    printf("=======================================\n");
    printf("Largest number for Function 4 in ECX: %d\n",nBlock04 );
    printf("=======================================\n");
    for( n = 0; n <= nBlock04; n++ ) {
      printf("Function 4 %d\n",n);
      pTmp = (int*)(&cpuInfo.DataFunction04.ptrDataCPUID[n]);
      printf("eax = %8.8X\n",pTmp[0] );
      printf("ebx = %8.8X\n",pTmp[1] );
      printf("ecx = %8.8X\n",pTmp[2] );
      printf("edx = %8.8X\n",pTmp[3] );
    }
  }
  /*********************************************************/
  if( cpuInfo.DataFunction0b.ptrDataCPUID ) {
    nBlock0b = cpuInfo.DataFunction0b.IndexBlockCPUID;
    printf("=========================================\n");
    printf("Largest number for Function 0Bh in ECX: %d\n",nBlock0b );
    printf("=========================================\n");
    for( n = 0; n <= nBlock0b; n++ ) {
      printf("Function B %d\n",n);
      pTmp = (int*)(&cpuInfo.DataFunction0b.ptrDataCPUID[n]);
      printf("eax = %8.8X\n",pTmp[0] );
      printf("ebx = %8.8X\n",pTmp[1] );
      printf("ecx = %8.8X\n",pTmp[2] );
      printf("edx = %8.8X\n",pTmp[3] );
    }
  }
  /*********************************************************/
  if( cpuInfo.DataFunction8x.ptrDataCPUID ) {
    nBlock8x = cpuInfo.DataFunction8x.IndexBlockCPUID;
    printf("=====================================\n");
    printf("Largest Extended Function number: %d\n",nBlock8x );
    printf("=====================================\n");
    for( n = 0; n <= nBlock8x; n++ ) {
      printf("Function %Xh\n",n|0x80000000);
      pTmp = (int*)(&cpuInfo.DataFunction8x.ptrDataCPUID[n]);
      printf("eax = %8.8X\n",pTmp[0] );
      printf("ebx = %8.8X\n",pTmp[1] );
      printf("ecx = %8.8X\n",pTmp[2] );
      printf("edx = %8.8X\n",pTmp[3] );
    }
  }
  printf("\n");

  /*********************************************************/
  ippCPUInfoFree( &cpuInfo );

return 0;
}
