/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __IPPCV_H__
#include "ippcv.h"
#endif
#ifndef __SOBEL_H__
#include "sobel.h"
#endif



static IppChannels lut_channels[] =
{
  ippC1,
  ippC1,
  ippC1,
  ippC3,
  ippC4
};


int sobel_filter_ipp(CIppImage& src, PARAMS_SBLFLT& params, CIppImage& dst)
{
  int       s1;
  int       s2;
  IppStatus status;

  SBLFLT_IPP_CTX* ctx = (SBLFLT_IPP_CTX*)params.ctx;

  ippSetNumThreads(params.nthreads);

  if(0 == ctx)
  {
    ctx = new SBLFLT_IPP_CTX;
    if(0 == ctx)
      return -1;

    ctx->id       = CTX_IPP;
    ctx->p.Data8u = 0;
    ctx->pBuf     = 0;
    ctx->p16shor  = 0;
    ctx->p16sver  = 0;

    params.ctx = ctx;
  }

  if(CTX_IPP != ctx->id)
  {
    return -1;
  }

  if(src.Precision() <= 8)
  {
    if(0 == ctx->p.Data8u)
    {
      ctx->p.Data8u = ippiMalloc_8u_C1(src.Width(),src.Height(),&ctx->step);
      if(0 == ctx->p.Data8u)
        return -1;
    }

    if(0 == ctx->p16shor)
    {
      ctx->p16shor = ippiMalloc_16s_C1(src.Width(),src.Height(),&ctx->stephor);
      if(0 == ctx->p16shor)
        return -1;
    }

    if(0 == ctx->p16sver)
    {
      ctx->p16sver = ippiMalloc_16s_C1(src.Width(),src.Height(),&ctx->stepver);
      if(0 == ctx->p16sver)
        return -1;
    }

    status = ippiFilterSobelHorizGetBufferSize_8u16s_C1R(
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize3x3,
               &s1);
    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelVertGetBufferSize_8u16s_C1R (
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               &s2);
    if(ippStsNoErr != status)
      return -1;

    s1 = s1 < s2 ? s2 : s1;

    if(0 == ctx->pBuf)
    {
      ctx->pBuf = (Ipp8u*)ippMalloc(s1);
      if(0 == ctx->pBuf)
        return -1;
    }

    if(src.NChannels() == 1)
    {
      status = ippiCopy_8u_C1R(
                 (const Ipp8u*)src.DataPtr(),src.Step(),
                 ctx->p.Data8u,ctx->step,src.Size());
    }
    else if(src.NChannels() == 3)
    {
      status = ippiRGBToGray_8u_C3C1R(
                 (const Ipp8u*)src.DataPtr(),src.Step(),
                 ctx->p.Data8u,ctx->step,src.Size());
    }
    else if(src.NChannels() == 4)
    {
      status = ippiRGBToGray_8u_AC4C1R(
                 (const Ipp8u*)src.DataPtr(),src.Step(),
                 ctx->p.Data8u,ctx->step,src.Size());
    }
    else
      return -1;

    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelHorizBorder_8u16s_C1R(
               (const Ipp8u*)ctx->p.Data8u, ctx->step,
               ctx->p16shor, ctx->stephor,
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               ippBorderRepl, 0, ctx->pBuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelVertBorder_8u16s_C1R(
               (const Ipp8u*)ctx->p.Data8u, ctx->step,
               ctx->p16sver, ctx->stepver,
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               ippBorderRepl, 0, ctx->pBuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiAbs_16s_C1IR(ctx->p16shor, ctx->stephor, src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiAbs_16s_C1IR(ctx->p16sver, ctx->stepver, src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiAdd_16s_C1IRSfs(
               (const Ipp16s*) ctx->p16shor, ctx->stephor,
                               ctx->p16sver, ctx->stepver, src.Size(), 0);
    if(ippStsNoErr != status)
      return -1;

    status = ippiConvert_16s8u_C1R(
               ctx->p16sver, ctx->stepver,
               (Ipp8u*)dst, dst.Step(), dst.Size());
    if(ippStsNoErr != status)
      return -1;
  }
  else
  {
    if(0 == ctx->p.Data32f)
    {
      ctx->p.Data32f = ippiMalloc_32f_C1(src.Width(),src.Height(),&ctx->step);
      if(0 == ctx->p.Data32f)
        return -1;
    }

    if(0 == ctx->p32fhor)
    {
      ctx->p32fhor = ippiMalloc_32f_C1(src.Width(),src.Height(),&ctx->stephor);
      if(0 == ctx->p32fhor)
        return -1;
    }

    if(0 == ctx->p32fver)
    {
      ctx->p32fver = ippiMalloc_32f_C1(src.Width(),src.Height(),&ctx->stepver);
      if(0 == ctx->p32fver)
        return -1;
    }

    status = ippiFilterSobelHorizGetBufferSize_32f_C1R(
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               &s1);
    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelVertGetBufferSize_32f_C1R (
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               &s2);
    if(ippStsNoErr != status)
      return -1;

    s1 = s1 < s2 ? s2 : s1;

    if(0 == ctx->pBuf)
    {
      ctx->pBuf = (Ipp8u*)ippMalloc(s1);
      if(0 == ctx->pBuf)
        return -1;
    }

    status = ippiConvert_16u32f_C1R((const Ipp16u*)src.DataPtr(),src.Step(),
               ctx->p.Data32f, ctx->step,src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelHorizBorder_32f_C1R(
               (const Ipp32f*)ctx->p.Data32f, ctx->step,
               ctx->p32fhor, ctx->stephor,
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               ippBorderRepl, 0, ctx->pBuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiFilterSobelVertBorder_32f_C1R(
               (const Ipp32f*)ctx->p.Data32f, ctx->step,
               ctx->p32fver, ctx->stepver,
               src.Size(),
               params.filter_kernel == KERNEL_3x3 ? ippMskSize3x3 : ippMskSize5x5,
               ippBorderRepl, 0, ctx->pBuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiAbs_32f_C1IR(ctx->p32fhor, ctx->stephor, src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiAbs_32f_C1IR(ctx->p32fver, ctx->stepver, src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiAdd_32f_C1IR(
               (const Ipp32f*) ctx->p32fhor, ctx->stephor,
                               ctx->p32fver, ctx->stepver,
                               src.Size());
    if(ippStsNoErr != status)
      return -1;

    status = ippiConvert_32f16u_C1R(
               (const Ipp32f*)ctx->p32fver, ctx->stepver,
               (Ipp16u*)dst.DataPtr(), dst.Step(), dst.Size(), ippRndNear);
    if(ippStsNoErr != status)
      return -1;
  }

  return 0;
} // sobel_filter_ipp()


