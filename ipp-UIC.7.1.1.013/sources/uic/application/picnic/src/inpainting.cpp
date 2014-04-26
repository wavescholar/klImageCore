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
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __IPPJ_H__
#include "ippj.h"
#endif
#ifndef __IPPCV_H__
#include "ippcv.h"
#endif
#ifndef __INPAINTING_H__
#include "inpainting.h"
#endif


int inpaintFull(const CIppImage& src, CIppImage& dst, CIppImage& mask, Ipp32f radius, int flag)
{
  int fmmStep;
  int iFastMarchingsize;
  Ipp8u*           iFmmbuf;
  Ipp32f*          fmm;
  IppiInpaintFlag  telea;
  IppiSize         roi;
  IppStatus        status;

  roi.width  = src.Width();
  roi.height = src.Height();

  telea = (IppiInpaintFlag)flag;

  status = ippiFastMarchingGetBufferSize_8u32f_C1R(roi, &iFastMarchingsize);
  if(ippStsNoErr != status)
    return -1;

  iFmmbuf = (Ipp8u*)ippMalloc(iFastMarchingsize);
  if(0 == iFmmbuf)
    return -1;

  fmm = ippiMalloc_32f_C1(src.Width(), src.Height(), &fmmStep);
  if(0 == fmm)
    return -1;

  if(src.NChannels() == 3)
  {
    IppiInpaintState_8u_C3R* iState3;

    status = ippiFastMarching_8u32f_C1R((const Ipp8u*)mask,mask.Step(),fmm,fmmStep,roi,radius,iFmmbuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaintInitAlloc_8u_C3R(&iState3,fmm,fmmStep,(const Ipp8u*)mask,mask.Step(),roi,radius,telea);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaint_8u_C3R((const Ipp8u*)src.DataPtr(),src.Step(),dst.DataPtr(),dst.Step(),roi,iState3);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaintFree_8u_C3R(iState3);
    if(ippStsNoErr != status)
      return -1;
  }
  else
  {
    IppiInpaintState_8u_C1R* iState1;

    status = ippiFastMarching_8u32f_C1R((const Ipp8u*)mask,src.Width(),fmm,fmmStep,roi,radius,iFmmbuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaintInitAlloc_8u_C1R(&iState1,fmm,fmmStep,(const Ipp8u*)mask,src.Width(),roi,radius,telea);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaint_8u_C1R((const Ipp8u*)src.DataPtr(),src.Step(),(Ipp8u*)dst.DataPtr(),dst.Step(),roi,iState1);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaintFree_8u_C1R(iState1);
    if(ippStsNoErr != status)
      return -1;
  }

  ippFree(iFmmbuf);

  return 0;
} // inpaintFull()


int inpaintSimple(const CIppImage& src, CIppImage& dst, CIppImage& mask, Ipp32f radius, int flag)
{
  int fmmStep;
  int iFastMarchingsize;
  Ipp8u*                   iFmmbuf;
  Ipp32f*                  fmm;
  IppiInpaintFlag          telea;
  IppiInpaintState_8u_C1R* iState1;
  IppiInpaintState_8u_C3R* iState3;
  IppiSize                 roi;
  IppStatus                status;

  roi.width  = src.Width();
  roi.height = src.Height();

  telea = (IppiInpaintFlag)flag;

  status = ippiFastMarchingGetBufferSize_8u32f_C1R(roi, &iFastMarchingsize);
  if(ippStsNoErr != status)
    return -1;

  iFmmbuf = (Ipp8u*)ippMalloc(iFastMarchingsize);
  if(0 == iFmmbuf)
    return -1;

  fmm = ippiMalloc_32f_C1(src.Width(), src.Height(), &fmmStep);
  if(0 == fmm)
    return -1;

  status = ippiFastMarching_8u32f_C1R((const Ipp8u*)mask,mask.Step(),fmm,fmmStep,roi,radius,iFmmbuf);
  if(ippStsNoErr != status)
    return -1;

  if(src.NChannels() == 3)
  {
    status = ippiInpaintInitAlloc_8u_C3R(&iState3,fmm,fmmStep,(const Ipp8u*)mask,mask.Step(),roi,radius,telea);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaint_8u_C3R((const Ipp8u*)src.DataPtr(),src.Step(),dst.DataPtr(),dst.Step(),roi,iState3);
    if(ippStsNoErr != status)
      return -1;

    ippiInpaintFree_8u_C3R(iState3);
  }
  else
  {
    status = ippiInpaintInitAlloc_8u_C1R(&iState1,fmm,fmmStep,(const Ipp8u*)mask,mask.Step(),roi,radius,telea);
    if(ippStsNoErr != status)
      return -1;

    status = ippiInpaint_8u_C1R((const Ipp8u*)src.DataPtr(),src.Step(),dst.DataPtr(),dst.Step(),roi,iState1);
    if(ippStsNoErr != status)
      return -1;

    ippiInpaintFree_8u_C1R(iState1);
  }

  ippFree(iFmmbuf);

  return 0;
} // inpaintSimple()


int inpainting_filter(const CIppImage& src, PARAMS_INPFLT& params, CIppImage& dst, CIppImage& mask)
{
  int r = 0;

  if(params.alg)
  {
    r = inpaintSimple(src, dst, mask, (Ipp32f)params.radius, params.mode);
  }
  else
  {
    r = inpaintFull  (src, dst, mask, (Ipp32f)params.radius, params.mode);
  }

  return r;
} // inpainting_filter()

