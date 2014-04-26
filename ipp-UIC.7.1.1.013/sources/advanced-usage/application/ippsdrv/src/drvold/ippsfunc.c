
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

#include <wdm.h>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef _IPPS_H__
#include "ipps.h"
#endif
#define __DRIVER__
#include "ippsold.h"



static void GenerateIntTone(short* src,int len)
{
  IppStatus     status;
  float phase = 0.f;

/*
  IppStatus ippsTone_Direct_16s(Ipp16s* pDst, int len, Ipp16s magn,
float rfreq, float* pPhase, IppHintAlgorithm hint);
*/
 status =  ippsTone_Direct_16s(src, len, (Ipp16s)100, (float)0.1,
            &phase, ippAlgHintNone );
  // check errors

  if(ippStsOk != status)
  {
    TRACE(( (char*)ippGetStatusString(status) ));
  }

  return;
} // GenerateIntTone()


static void GenerateFltTone(float* src,int len)
{
  IppStatus     status;
  float phase = 0.f;

  // generate tone
  status =  ippsTone_Direct_32f(src, len, (Ipp32f)100.0, (float)0.1,
            &phase, ippAlgHintNone );
  // check errors
  if(ippStsOk != status)
  {
    TRACE(( (char*)ippGetStatusString(status) ));
  }

  return;
} // GenerateFltTone()


static void DoFltFFT(
  PIPPSOLD_DEVICE_IOBUFFER ioBuffer)
{
  float*    src   = NULL;
  float*    dst   = NULL;
  float*    tst   = NULL;
  float     norma = 0.0;
  int       len;
  int       order;
  int       tmp;
  IppStatus status;
  IppsFFTSpec_R_32f* spec;


  TRACE(("IPPSOLD.SYS: entering in DoFltFFT\n"));

  len = ioBuffer->Length;

  // get order from length
  order = 0;
  tmp = len;
  while(0 != tmp)
  {
    tmp >>= 1;
    order ++;
  }

  order --;

  src = ippsMalloc_32f(len);
  dst = ippsMalloc_32f(len+2);
  tst = ippsMalloc_32f(len);

  if(NULL == src || NULL == dst || NULL == tst)
  {
    TRACE(( "Can't allocate %d bytes",len * sizeof(float) ));
    ioBuffer->Error = -1;
    goto Exit;
  }

  // generate input samles array
  GenerateFltTone(src,len);


/*
  IppStatus ippsFFTInitAlloc_R_32f(IppsFFTSpec_R_32f** pFFTSpec, int order,
int flag, Ipp HintAlgorithm hint);
*/

  status = ippsFFTInitAlloc_R_32f(&spec, order, IPP_FFT_DIV_INV_BY_N,
           ippAlgHintNone );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

       // do forward fourie transform
//  IppStatus ippsFFTFwd_RToCCS_32f(const Ipp32f* pSrc, Ipp32f* pDst,
//                const IppsFFTSpec_R_32f* pFFTSpec, Ipp8u* pBuffer);

  status = ippsFFTFwd_RToCCS_32f( src, dst, spec, NULL );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }


  // do inverse fourie transform
//  IppStatus ippsFFTInv_CCSToR_32f(const Ipp32f* pSrc, Ipp32f* pDst,
//                const IppsFFTSpec_R_32f* pFFTSpec, Ipp8u* pBuffer);
  status =  ippsFFTInv_CCSToR_32f(dst, tst, spec, NULL);
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }
// IppStatus ippsFFTFree_R_32f(IppsFFTSpec_R_32f* pFFTSpec);
  status = ippsFFTFree_R_32f( spec );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

  // compare results array with original array
// IppStatus ippsNormInf_32f (const Ipp32f* pSrc1, const Ipp32f* pSrc2,
//                   int len, Ipp32f* pNorm);
  status =  ippsNormDiff_Inf_32f ( src, tst, len, &norma);
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString (status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }


  ioBuffer->fNorma = norma;
  ioBuffer->Error  = 0;


Exit:

  if(NULL != src)
  {
    ippsFree(src);
  }

  if(NULL != dst)
  {
    ippsFree(dst);
  }

  if(NULL != tst)
  {
    ippsFree(tst);
  }

  TRACE(("IPPSOLD.SYS: leaving DoFltFFT\n"));

  return;

} // DoFltFFT()


static void DoIntFFT(
  PIPPSOLD_DEVICE_IOBUFFER ioBuffer)
{
  short*    src   = NULL;
  short*    dst   = NULL;
  short*    tst   = NULL;
  float     norma = 0.0;
  int       len;
  int       order;
  int       tmp;
  int       factor = 0;
  IppsFFTSpec_R_16s* spec;
  IppStatus status;


  TRACE(("IPPSOLD.SYS: entering in DoIntFFT\n"));

  len = ioBuffer->Length;

  // get order from length
  order = 0;
  tmp = len;
  while(0 != tmp)
  {
    tmp >>= 1;
    order ++;
  }

  order --;

  src = ippsMalloc_16s(len);
  dst = ippsMalloc_16s(len+4);
  tst = ippsMalloc_16s(len);

  if(NULL == src || NULL == dst || NULL == tst)
  {
    TRACE(("Can't allocate %d bytes",len * sizeof(float)));
    ioBuffer->Error = -1;
    goto Exit;
  }

  // generate input samles array
  GenerateIntTone(src,len);

  status = ippsFFTInitAlloc_R_16s(&spec, order, IPP_FFT_DIV_INV_BY_N,
           ippAlgHintNone );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

  // do forward fourie transform
  status = ippsFFTFwd_RToCCS_16s_Sfs( src, dst, spec, factor, NULL );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

  // do inverse fourie transform
  status =  ippsFFTInv_CCSToR_16s_Sfs(dst, tst, spec, factor, NULL);
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

  status = ippsFFTFree_R_16s( spec );
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }


  // compare results array with original array
// IppStatus ippsNormInf_16s32f (const Ipp16s* pSrc1, const Ipp16s* pSrc2,
//                    int len, Ipp32f* pNorm);

  status = ippsNormDiff_Inf_16s32f (src, tst, len, &norma);
  if(ippStsOk != status)
    {
    TRACE(( (char*)ippGetStatusString(status) ));
    ioBuffer->Error  = status;
    goto Exit;
    }

  ioBuffer->fNorma = norma;
  ioBuffer->Error  = 0;

Exit:

  if(NULL != src)
  {
    ippsFree(src);
  }

  if(NULL != dst)
  {
    ippsFree(dst);
  }

  if(NULL != tst)
  {
    ippsFree(tst);
  }

  TRACE(("IPPSOLD.SYS: leaving DoIntFFT\n"));

  return;
} // DoIntFFT()


void ippsDrvGetVersion(
  PIPPSOLD_DEVICE_IOBUFFER ioBuffer)
{
  IppLibraryVersion* pLibVersion = NULL;

  TRACE(("IPPSOLD.SYS: entering in ippsDrvGetVersion\n"));

  if(NULL != ioBuffer)
  {
    pLibVersion = (IppLibraryVersion*)ippsGetLibVersion();
    TRACE(("ippsGetLibVersion() return [0x%08X]\n",pLibVersion));

    strcpy(ioBuffer->LibName,pLibVersion->Name);
    strcpy(ioBuffer->LibVers,pLibVersion->Version);
    strcpy(ioBuffer->LibBild,pLibVersion->BuildDate);

  } else {
    TRACE(("ioBuffer pointer is NULL!\n"));
  }

  TRACE(("IPPSOLD.SYS: leaving ippsDrvGetVersion\n"));

  return;
} // ippsDrvGetVersion()


void ippsDrvDoFFT(
  PIPPSOLD_DEVICE_IOBUFFER ioBuffer)
{

  TRACE(("IPPSOLD.SYS: entering in ippsDrvDoFFT\n"));

  if(0 == ioBuffer->Type)
  {
    // float data selected
    DoFltFFT(ioBuffer);
  } else {
    // integer data selected
    DoIntFFT(ioBuffer);
  }

  TRACE(("IPPSOLD.SYS: leaving ippsDrvDoFFT\n"));

  return;
} // ippsDrvDoFFT()


