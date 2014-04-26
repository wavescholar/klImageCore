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
#include <stdlib.h>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
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
#ifndef __SEGMENTATION_H__
#include "segmentation.h"
#endif


#define CV_IMAGE_ELEM( image, elemtype, row, col )  \
  (((elemtype*)((Ipp8u*)image + (image).Step()))[(col)])

#define  CV_IMAGE_3COLOR_ELEM(img, type, y, x, c)  CV_IMAGE_ELEM(img, type, y, (x)*3+(c))

#ifndef MAX_MASK_SIZE_SQR          // sqr of MAX_MASK_SIZE (for arrays)
#define MAX_MASK_SIZE_SQR  121
#endif

void img_3clustersto1(CIppImage& src1, CIppImage& src2)
{
  int  palette[256];
  int  i, j, k, m = 1, p, q;
  palette[0] = 0;

  if (src1.NChannels() == 3)
  {
    for(i = 0; i < src1.Height(); i++)
    {
      for(j = 0; j < src1.Width(); j++)
      {
        p = (CV_IMAGE_3COLOR_ELEM(src1, Ipp8u, i, j, 0) << 16) +
            (CV_IMAGE_3COLOR_ELEM(src1, Ipp8u, i, j, 1) <<  8) +
            (CV_IMAGE_3COLOR_ELEM(src1, Ipp8u, i, j, 2));
        q = 0;

        for (k = 0; k < m; k++)
        {
          if(palette[k] == p)
          {
            q = p;
            break;
          }
        }

        if((k == m) && (k < 254))
        {
          palette[m++] = p;
          q = p;
        }
        CV_IMAGE_ELEM(src2, Ipp8u, i, j) = q ? k : 0;
      }
    }
  }

  return;
} // img_3clustersto1()


void ownDrawBounds(CIppImage& mar, CIppImage& src2, Ipp8u bnd, Ipp8u* bcl)
{
  int    i, j;
  Ipp8u  clr;

  if(src2.NChannels() == 3)
  {
    for(i = 0; i < mar.Height(); i++)
    {
      for (j = 0; j < mar.Width(); j++)
      {
        clr = ((Ipp8u*)mar)[i*mar.Step()+j];
        if(clr == bnd)
        {
          ((Ipp8u*)src2)[i*src2.Step()+j*3+0] = bcl[0];
          ((Ipp8u*)src2)[i*src2.Step()+j*3+1] = bcl[1];
          ((Ipp8u*)src2)[i*src2.Step()+j*3+2] = bcl[2];
        }
      }
    }
  }
  else
  {
    for(i = 0; i < mar.Height(); i++)
    {
      for(j = 0; j < mar.Width(); j++)
      {
        clr = ((Ipp8u*)mar)[i * mar.Step() + j];
        if(clr == bnd)
        {
          ((Ipp8u*)src2)[i*src2.Step()+j] = bcl[0];
        }
      }
    }
  }

  return;
} // ownDrawBounds()


int segmentation_filter(CIppImage& src, PARAMS_SGMFLT& params, CIppImage& dst)
{
  int i, j;
  int k;
  int sz;
  int color;
  int                flags      = IPP_SEGMENT_DISTANCE;
  bool               bounds     = true;
  int                flarr[6]   = { 0, 0, IPP_SEGMENT_BORDER_4, IPP_SEGMENT_BORDER_8, 0, 0 };
  bool               boarr[6]   = { true, true, false, false, false, false };
  IppiSize           roi        = { 0, 0 };
  IppiSize           maskSize   = { params.morph, params.morph };
  IppiPoint          anchor     = { maskSize.width/2, maskSize.height/2 };
  Ipp8u              mask3x3[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  IppiSize           size3x3    = { 3, 3 };
  IppiPoint          anchor3x3  = { 1, 1 };
  IppiNorm           norm       = ippiNormL1;
  IppiNorm           normb      = ippiNormL1;
  Ipp32u             seed       = 54321;
  Ipp16s             table16s[256*3];
  Ipp8u              ff[3]      = { 255, 255, 255 };
  Ipp8u              mask[121];
  const Ipp8u*       plane[3];
  Ipp8u*             mrbuf = 0;
  Ipp8u*             flbuf = 0;
  Ipp8u*             rsbuf = 0;
  Ipp8u*             wsbuf = 0;
  Ipp8u*             lobuf = 0;
  IppiNorm           noarr[6]   = { ippiNormL1, ippiNormInf, ippiNormL1, ippiNormInf, ippiNormL1, ippiNormInf };
  IppiMorphAdvState* pState;
  IppStatus          status;

  CIppImage img1;
  CIppImage img2;
  CIppImage img3;
  CIppImage col2;
  CIppImage col3;
  CIppImage mar1;
  CIppImage mar2;
  CIppImage wrk1;
  CIppImage wrk2;
  CIppImage* wrg1;
  //CIppImage* wrc1;

  if( dst.NChannels() != 3 )
    dst.Alloc( src.Size(), 3, src.Precision() );

  status = ippsRandUniform_Direct_16s(table16s,256*3,50,230,&seed);
  if (ippStsNoErr != status)
    return -1;

  table16s[0] = 0;
  table16s[1] = 0;
  table16s[2] = 0;
  table16s[255*3+0] = 255;
  table16s[255*3+1] = 255;
  table16s[255*3+2] = 255;

  roi.width  = src.Width();
  roi.height = src.Height();

  status = ippiFilterLowpassGetBufferSize_8u_C1R(roi, ippMskSize5x5, &sz);
  if (ippStsNoErr != status)
    return -1;

  if((lobuf = (Ipp8u*)ippMalloc(sz)) == 0)
    return -1;

  status = ippiLabelMarkersGetBufferSize_8u_C1R(roi,&sz);
  if (ippStsNoErr != status)
    return -1;

  if((flbuf = (Ipp8u*)ippMalloc(sz)) == 0)
    return -1;

  status = ippiMorphReconstructGetBufferSize_8u_C1(roi,&sz);
  if (ippStsNoErr != status)
    return -1;

  if((mrbuf = (Ipp8u*)ippMalloc(sz)) == 0)
    return -1;

  status = ippiSegmentWatershedGetBufferSize_8u_C1R(roi,&sz);
  if (ippStsNoErr != status)
    return -1;

  if((rsbuf = (Ipp8u*)ippMalloc(sz)) == 0)
    return -1;

  status = ippiSegmentWatershedGetBufferSize_8u_C1R(roi,&sz);
  if (ippStsNoErr != status)
    return -1;

  if((wsbuf = (Ipp8u*)ippMalloc(sz)) == 0)
    return -1;

  if(params.distance)
  {
    flags |= IPP_SEGMENT_DISTANCE;
    flags &= ~IPP_SEGMENT_QUEUE;
  }
  else
  {
    flags &= ~IPP_SEGMENT_DISTANCE;
    flags |= IPP_SEGMENT_QUEUE;
  }

  img1.Alloc(src.Size(), 1, 8, 0);
  img2.Alloc(src.Size(), 1, 8, 0);
  img3.Alloc(src.Size(), 1, 8, 0);
  col2.Alloc(src.Size(), 3, 8, 0);
  col3.Alloc(src.Size(), 3, 8, 0);
  mar1.Alloc(src.Size(), 1, 8, 0);
  mar2.Alloc(src.Size(), 1, 8, 0);
  wrk1.Alloc(src.Size(), 1, 8, 0);
  wrk2.Alloc(src.Size(), 1, 8, 0);

  for(i = 0; i < MAX_MASK_SIZE_SQR; i++)
    mask[i] = 255;

  switch(params.normOptions)
  {
    case  0:
          norm  = ippiNormInf;
          normb = ippiNormL1;
          break;

    case  1:
          norm  = ippiNormL1;
          normb = ippiNormL1;
          break;

    case  2:
          norm  = ippiNormL2;
          normb = ippiNormL1;
          break;

    case  3:
          norm  = ippiNormFM;
          normb = ippiNormL1;
          break;
  }

  if(params.bounind >= 6) // TODO: why 0 if >= 6 ???
    params.bounind = 0;

  bounds = boarr[params.bounind];
  flags  = flarr[params.bounind] | (flags & (IPP_SEGMENT_DISTANCE | IPP_SEGMENT_QUEUE));
  normb  = noarr[params.bounind];

  if(src.NChannels() == 1)
  {
    img1.Alloc(src.Size(), src.NChannels(), src.Precision());
    plane[0] = (Ipp8u*)img1;
    plane[1] = (Ipp8u*)img1;
    plane[2] = (Ipp8u*)img1;

    status = ippiFilterLowpassBorder_8u_C1R(
               (const Ipp8u*)src,src.Step(),
               (Ipp8u*)img1,img1.Step(),
               roi,ippMskSize5x5,ippBorderRepl,0,lobuf);
    if(ippStsNoErr != status)
      return -1;

    status = ippiCopy_8u_P3C3R(plane,img1.Step(),(Ipp8u*)dst,dst.Step(),roi);
    if(ippStsNoErr != status)
      return -1;

    status = ippiMorphAdvInitAlloc_8u_C1R(&pState,roi,mask3x3,size3x3,anchor3x3);
    if(ippStsNoErr != status)
      return -1;

    status = ippiMorphGradientBorder_8u_C1R(
               (const Ipp8u*)img1,img1.Step(),
                     (Ipp8u*)img2,img2.Step(),
                     roi,ippBorderRepl,pState);
    if(ippStsNoErr != status)
      return -1;

    status = ippiMorphAdvFree(pState);
    if(ippStsNoErr != status)
      return -1;

    plane[0] = (Ipp8u*)img2;
    plane[1] = (Ipp8u*)img2;
    plane[2] = (Ipp8u*)img2;

    status = ippiCopy_8u_P3C3R(plane,img2.Step(),(Ipp8u*)col2,col2.Step(),roi);
    if(ippStsNoErr != status)
      return -1;
  }
  else
  {
    if(src.NChannels() == 3)
    {
      dst.Alloc( src.Size(), src.NChannels(), src.Precision());

      status = ippiFilterLowpassBorder_8u_C1R(
                 (const Ipp8u*)src,src.Step(),
                 (Ipp8u*)img1,img1.Step(),
                 roi,ippMskSize5x5,ippBorderRepl,0,lobuf);
      if(ippStsNoErr != status)
        return -1;

      status = ippiRGBToGray_8u_C3C1R(
                 (const Ipp8u*)dst,dst.Step(),
                       (Ipp8u*)img1,img1.Step(),
                       roi);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphAdvInitAlloc_8u_C3R(&pState,roi,mask3x3,size3x3,anchor3x3);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphGradientBorder_8u_C3R(
                 (const Ipp8u*)dst,dst.Step(),
                       (Ipp8u*)col2,col2.Step(),
                       roi,ippBorderRepl,pState);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphAdvFree(pState);
      if(ippStsNoErr != status)
        return -1;

      status = ippiGradientColorToGray_8u_C3C1R(
                 (Ipp8u*)col2,col2.Step(),
                 (Ipp8u*)img2,img2.Step(),
                 roi,norm);
      if(ippStsNoErr != status)
        return -1;
    }
    else
      return -1;
  }

  wrg1 = (params.gradient) ? &img2 : &img1;
  //wrc1 = (params.gradient) ? &col2 : &dst;

  // find markers
  maskSize.width  = params.morph;
  maskSize.height = params.morph;
  anchor.x = params.morph/2;
  anchor.y = params.morph/2;

  status = ippiMorphAdvInitAlloc_8u_C1R(&pState,src.Size(),mask,maskSize,anchor);
  if(ippStsNoErr != status)
    return -1;

  status = ippiCopy_8u_C1R(
             (const Ipp8u*)*wrg1, wrg1->Step(),
                   (Ipp8u*)wrk2, wrk2.Step(),
                   roi);
  if(ippStsNoErr != status)
    return -1;

  if(params.erode)
  {
    for(k = 0; k < (params.measure ? params.rep : 1); k++)
    {
      status = ippiCopy_8u_C1R(
                 (const Ipp8u*)wrk2,wrk2.Step(),
                       (Ipp8u*)*wrg1,wrg1->Step(),
                       roi);
      if(ippStsNoErr != status)
        return -1;

      status = ippiAddC_8u_C1RSfs(
                 (const Ipp8u*)*wrg1,wrg1->Step(),
                 params.cap,
                       (Ipp8u*)wrk1,wrk1.Step(),
                       roi,0);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphReconstructErode_8u_C1IR(
                 (const Ipp8u*)*wrg1,wrg1->Step(),
                       (Ipp8u*)wrk1,wrk1.Step(),
                       roi,mrbuf,ippiNormL1);
      if(ippStsNoErr != status)
        return -1;

      status = ippiSub_8u_C1RSfs(
                 (const Ipp8u*)*wrg1,wrg1->Step(),
                       (Ipp8u*)wrk1,img2.Step(),
                       (Ipp8u*)img3,img3.Step(),
                       roi,0);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphOpenBorder_8u_C1R(
                 (const Ipp8u*)img3,img3.Step(),
                       (Ipp8u*)mar1,mar1.Step(),
                       roi,ippBorderRepl,pState);
      if(ippStsNoErr != status)
        return -1;
    }
  }
  else
  {
    for (k = 0; k < (params.measure ? params.rep : 1); k++)
    {
      status = ippiCopy_8u_C1R(
                 (const Ipp8u*)wrk2,wrk2.Step(),
                       (Ipp8u*)*wrg1,wrg1->Step(),
                       roi);
      if(ippStsNoErr != status)
        return -1;

      status = ippiSubC_8u_C1RSfs(
                 (const Ipp8u*)*wrg1,wrg1->Step(),
                 params.cap,
                       (Ipp8u*)wrk1,wrk1.Step(),
                       roi,0);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphReconstructDilate_8u_C1IR(
                 (const Ipp8u*)*wrg1,wrg1->Step(),
                       (Ipp8u*)wrk1,wrk1.Step(),
                       roi,mrbuf,ippiNormL1);
      if(ippStsNoErr != status)
        return -1;

      status = ippiSub_8u_C1RSfs(
                 (const Ipp8u*)wrk1,wrk1.Step(),
                       (Ipp8u*)*wrg1,wrg1->Step(),
                       (Ipp8u*)img3,img3.Step(),
                       roi,0);
      if(ippStsNoErr != status)
        return -1;

      status = ippiMorphOpenBorder_8u_C1R(
                 (const Ipp8u*)img3,img3.Step(),
                       (Ipp8u*)mar1,mar1.Step(),
                       roi,ippBorderRepl,pState);
      if(ippStsNoErr != status)
        return -1;
    }
  }

  status = ippiMorphAdvFree(pState);
  if(ippStsNoErr != status)
    return -1;

  pState = 0;

  // numger of colours
  int col = 0;
  for(k = 0; k < (params.measure ? params.rep : 1); k++)
  {
    status = ippiLabelMarkers_8u_C1IR((Ipp8u*)mar1,mar1.Step(),roi,1,254,ippiNormL1,&col,flbuf);
    if(ippStsNoErr != status)
      return -1;
  }

  // watershed
  ippSetNumThreads(params.nthreads);

  mar2.CopyFrom((Ipp8u*)mar1, mar1.Step(), mar1.Size());

  for(i = 0; i < (params.measure ? params.rep : 1); i++)
  {
    status = ippiCopy_8u_C1R((const Ipp8u*)mar2,mar2.Step(),(Ipp8u*)mar1,mar1.Step(),roi);
    if(ippStsNoErr != status)
      return -1;

    status = ippiSegmentWatershed_8u_C1IR(
               (const Ipp8u*)*wrg1,wrg1->Step(),
                     (Ipp8u*)mar1,mar1.Step(),
                     roi,(IppiNorm)norm,flags,rsbuf);
    if(ippStsNoErr != status)
      return -1;
  }

  ippSetNumThreads(1);

  // coloured
  img3.CopyFrom((Ipp8u*)mar1, mar1.Step(), mar1.Size());

  if(src.NChannels() == 1)
  {
    plane[0] = (const Ipp8u*)src;
    plane[1] = (const Ipp8u*)src;
    plane[2] = (const Ipp8u*)src;

    status = ippiCopy_8u_P3C3R(plane,src.Step(),(Ipp8u*)dst,dst.Step(),roi);
    if(ippStsNoErr != status)
      return -1;
  }
  else
  {
    dst.CopyFrom(src);
  }

  if(bounds)
  {
    status = ippiBoundSegments_8u_C1IR((Ipp8u*)img3,img3.Step(),roi,ff[0],normb);
    if(ippStsNoErr != status)
      return -1;
  }

  for(i = 0; i < roi.height; i++)
  {
    for(j = 0; j < roi.width; j++)
    {
      color = ((Ipp8u*)img3)[i * img3.Step() + j];
      if(color)
      {
        ((Ipp8u*)col3)[i*col3.Step() + j*3 + 0] = (Ipp8u)table16s[color*3+0];
        ((Ipp8u*)col3)[i*col3.Step() + j*3 + 1] = (Ipp8u)table16s[color*3+1];
        ((Ipp8u*)col3)[i*col3.Step() + j*3 + 2] = (Ipp8u)table16s[color*3+2];

        ((Ipp8u*)dst)[i*col3.Step() + j*3 + 0] = (3*table16s[color*3+0]+5*((Ipp8u*)dst)[i*dst.Step()+j*3+0])>>3;
        ((Ipp8u*)dst)[i*col3.Step() + j*3 + 1] = (3*table16s[color*3+1]+5*((Ipp8u*)dst)[i*dst.Step()+j*3+1])>>3;
        ((Ipp8u*)dst)[i*col3.Step() + j*3 + 2] = (3*table16s[color*3+2]+5*((Ipp8u*)dst)[i*dst.Step()+j*3+2])>>3;
      }
    }
  }

  img1.Free();
  img2.Free();
  img3.Free();
  col2.Free();
  col3.Free();
  mar1.Free();
  mar2.Free();
  wrk1.Free();
  wrk2.Free();

  if (0 != wsbuf)
    ippFree(wsbuf);

  if (0 != rsbuf)
    ippFree(rsbuf);

  if (0 != mrbuf)
    ippFree(mrbuf);

  if (0 != flbuf)
    ippFree(flbuf);

  if (0 != lobuf)
    ippFree(lobuf);

  return 0;
} // segmentation_filter()

