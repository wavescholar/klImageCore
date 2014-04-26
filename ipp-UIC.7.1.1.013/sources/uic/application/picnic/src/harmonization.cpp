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
#ifndef __HARMONIZATION_H__
#include "harmonization.h"
#endif


static IppChannels lut_channels[] =
{
  ippC1,
  ippC1,
  ippC1,
  ippC3,
  ippC4
};


int harmonization_filter_ipp(CIppImage& src, PARAMS_HRMFLT& params, CIppImage& dst)
{
  int       WING;
  int       c1 = params.c1;
  int       c2 = params.c2;
  Ipp32f    v1 = params.v1;
  Ipp32f    v2 = params.v2;
  Ipp32s    v3 = params.v3;
  Ipp32f*   p32b;
  IppiSize  roib;
  IppiSize  kernel;
  IppiPoint anchor;
  IppStatus status;

  HRMFLT_IPP_CTX* ctx = (HRMFLT_IPP_CTX*)params.ctx;

  assert( src.NChannels() == dst.NChannels() &&
    src.Precision() == dst.Precision() );

  if( src.NChannels() > 1 )
  {
    CIppImage temp[4], *p[4] = { &temp[0], &temp[1], &temp[2], &temp[3] };
    int i, n = src.NChannels(), res;
    assert( n == 3 || n == 4 );
    src.SplitTo(p);
    for( i = 0; i < n; i++ )
    {
      res = harmonization_filter_ipp(*p[i], params, *p[i]);
      if( res < 0 )
        break;
    }
    dst.MergeFrom((const CIppImage**)p);
    return res;
  }

  ippSetNumThreads(params.nthreads);

  switch(params.filter_kernel)
  {
  case KERNEL_3x3: WING = 1; break;
  case KERNEL_5x5: WING = 2; break;
  case KERNEL_7x7: WING = 3; break;
  case KERNEL_9x9: WING = 4; break;

  default:
    return -1;
  }

  kernel.width  = 2 * WING + 1;
  kernel.height = 2 * WING + 1;

  anchor.x = WING;
  anchor.y = WING;

  roib.width  = src.Width()  + WING + WING;
  roib.height = src.Height() + WING + WING;

  if(0 == ctx)
  {
    ctx = new HRMFLT_IPP_CTX;
    if(0 == ctx)
      return -1;

    ctx->id    = CTX_IPP;
    ctx->p32f  = 0;
    ctx->p32fb = 0;

    params.ctx = ctx;
  }

  if(CTX_IPP != ctx->id)
  {
    return -1;
  }

  if(0 == ctx->p32f)
  {
    ctx->p32f = ippiMalloc_32f_C1(src.Width(), src.Height(), &ctx->step32);
    if(0 == ctx->p32f)
      return -1;
  }

  if(0 == ctx->p32fb)
  {
    ctx->p32fb = ippiMalloc_32f_C1(roib.width,roib.height,&ctx->step32b);
    if(0 == ctx->p32fb)
      return -1;
  }

  p32b = ctx->p32fb + WING * ctx->step32b / sizeof(Ipp32f) + WING;

  if(src.Precision() <= 8)
    status = ippiConvert_8u32f_C1R ((const Ipp8u*) src.DataPtr(),src.Step(),p32b,ctx->step32b,src.Size());
  else
    status = ippiConvert_16u32f_C1R((const Ipp16u*)src.DataPtr(),src.Step(),p32b,ctx->step32b,src.Size());

  if(ippStsNoErr != status)
    return -1;

  status = ippiCopyReplicateBorder_32s_C1IR((Ipp32s*)p32b,ctx->step32b,src.Size(),roib,WING,WING);
  if(ippStsNoErr != status)
    return -1;

  status = ippiFilterBox_32f_C1R(p32b,ctx->step32b,ctx->p32f,ctx->step32,src.Size(),kernel,anchor);
  if(ippStsNoErr != status)
    return -1;

  status = ippiSub_32f_C1IR(p32b,ctx->step32b,ctx->p32f,ctx->step32,src.Size());
  if(ippStsNoErr != status)
    return -1;

  status = ippiMulC_32f_C1IR(v1,ctx->p32f,ctx->step32,src.Size());
  if(ippStsNoErr != status)
    return -1;

  status = ippiSub_32f_C1IR(p32b,ctx->step32b,ctx->p32f,ctx->step32,src.Size());
  if(ippStsNoErr != status)
    return -1;

  status = ippiMulC_32f_C1IR(-v2,ctx->p32f,ctx->step32,src.Size());
  if(ippStsNoErr != status)
    return -1;

  if(src.Precision() <= 8)
  {
    status = ippiConvert_32f8u_C1R(ctx->p32f,ctx->step32,dst.DataPtr(),dst.Step(),dst.Size(),ippRndNear);
    if(ippStsNoErr != status)
      return -1;

    status = ippiThreshold_LTValGTVal_8u_C1IR(dst.DataPtr(),dst.Step(),dst.Size(),c1,c1,c2,c2);
    if(ippStsNoErr != status)
      return -1;

    status = ippiAddC_8u_C1IRSfs(v3,dst.DataPtr(),dst.Step(),dst.Size(),0);
    if(ippStsNoErr != status)
      return -1;
  }
  else
  {
    status = ippiConvert_32f16u_C1R(ctx->p32f,ctx->step32,(Ipp16u*)dst.DataPtr(),dst.Step(),dst.Size(),ippRndNear);
    if(ippStsNoErr != status)
      return -1;

    status = ippiThreshold_LTValGTVal_16u_C1IR((Ipp16u*)dst.DataPtr(),dst.Step(),dst.Size(),c1,c1,c2,c2);
    if(ippStsNoErr != status)
      return -1;

    status = ippiAddC_16u_C1IRSfs(v3,(Ipp16u*)dst.DataPtr(),dst.Step(),dst.Size(),0);
    if(ippStsNoErr != status)
      return -1;
  }

  return 0;
} // harmonization_filter_ipp()


