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
#ifndef __FACEDETECTION_H__
#include "facedetection.h"
#endif

static char* cascade_name = ":/data/haar.txt";

Ipp32f factor         = 1.0f;
int    minneighbors   = 2;
int    pruningParam   = 0;
float  distfactor     = 15.0f;
float  distfactorrect = 1.3f;

static IppiHaarClassifier_32f** pHaar;

static int stages;
static int classifiers;
static int features;
static int positive;

static int* nLength;
static int* nClass;
static int* nFeat;
static int* pNum;
static int* nStnum;

static IppiRect* pFeature;

static Ipp32f* pWeight;
static Ipp32f* pThreshold;
static Ipp32f* pVal1;
static Ipp32f* pVal2;
static Ipp32f* sThreshold;

static IppiSize face;
static IppiSize classifierSize;


static int is_equal(int x1,int y1,int w1,int x2,int y2,int w2,float distparam,float rectparam)
{
  return ((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2) <= distparam * distparam) && w1 <= w2 * rectparam;
}


CCluster::CCluster(void)
{
  m_x = 0;
  m_y = 0;
  m_w = 0;
  m_count = 0;
  m_csize = 0;
  m_fsize = 0;
  m_currentclustercount = 0;

  return;
} // ctor


CCluster::~CCluster(void)
{
  for(int i = 0; i < m_csize; i++ )
  {
    if(0 != m_x[i])
      ippFree(m_x[i]);

    if(0 != m_y[i])
      ippFree(m_y[i]);

    if(0 != m_w[i])
      ippFree(m_w[i]);
  }

  if(0 != m_x)
    ippFree(m_x);

  if(0 != m_y)
    ippFree(m_y);

  if(0 != m_w)
    ippFree(m_w);

  if(0 != m_count)
    ippFree(m_count);

  return;
} // dtor


int CCluster::Init(int size, int clustercount)
{
  m_currentclustercount = 0;

  m_count = (int*)ippMalloc(sizeof(int) * clustercount);
  if(0 == m_count)
    return -1;

  m_x = (int**)ippMalloc(sizeof(int*) * clustercount);
  if(0 == m_x)
    return -1;

  m_y = (int**)ippMalloc(sizeof(int*) * clustercount);
  if(0 == m_y)
    return -1;

  m_w = (int**)ippMalloc(sizeof(int*) * clustercount);
  if(0 == m_w)
    return -1;

  for(int i = 0; i < clustercount; i++ )
  {
    m_x[i] = (int*)ippMalloc(sizeof(int)*size);
    if(0 == m_x[i])
      return -1;

    m_y[i] = (int*)ippMalloc(sizeof(int)*size);
    if(0 == m_y[i])
      return -1;

    m_w[i] = (int*)ippMalloc(sizeof(int)*size);
    if(0 == m_w[i])
      return -1;

    for(int j = 0; j < size; j++ )
    {
      m_x[i][j] = 0;
      m_y[i][j] = 0;
      m_w[i][j] = 0;
    }

    m_count[i] = 0;
  }

  m_csize = clustercount;

  return 0;
} // CCluster::Init()


void ClusterFaces(
  Ipp8u*    mask,
  IppiSize  maskRoi,
  int       maskStep,
  CCluster* clusters,
  IppiSize  face,
  Ipp32f    factor)
{
   int  i;
   int  j;
   int  ii;
   int  jj;
   int  xcenter;
   int  ycenter;
   int  wrect;
   bool equalrect = false;

   for(ii = 0; ii < maskRoi.height; ii++)
   {
      for(jj = 0; jj < maskRoi.width; jj++)
      {
         if(!mask[maskStep * ii + jj])
           continue;

         xcenter = (int)((jj + face.width/2) * factor);
         ycenter = (int)((ii + face.width/2) * factor);

         wrect = (int)(face.width * factor);

         if( clusters->m_currentclustercount )
         {
            for( i = 0; i < clusters->m_currentclustercount; i++ )
            {
               for( j = 0; j < clusters->m_count[i]; j++ )
               {
                  if( is_equal(xcenter,ycenter,wrect,clusters->m_x[i][j],clusters->m_y[i][j],clusters->m_w[i][j],distfactor,distfactorrect) )
                  {
                     clusters->m_x[i][clusters->m_count[i]] = xcenter;
                     clusters->m_y[i][clusters->m_count[i]] = ycenter;
                     clusters->m_w[i][clusters->m_count[i]] = wrect;
                     clusters->m_count[i]++;
                     equalrect = true;
                     break;
                  }
               }

               if( equalrect )
                 break;
            }

            if( !equalrect )
            {
               clusters->m_x[clusters->m_currentclustercount][0] = xcenter;
               clusters->m_y[clusters->m_currentclustercount][0] = ycenter;
               clusters->m_w[clusters->m_currentclustercount][0] = wrect;
               clusters->m_count[clusters->m_currentclustercount] = 1;
               clusters->m_currentclustercount++;
            }

            equalrect = false;
         }
         else
         {
            clusters->m_x[0][0] = xcenter;
            clusters->m_y[0][0] = ycenter;
            clusters->m_w[0][0] = wrect;
            clusters->m_count[0] = 1;
            clusters->m_currentclustercount++;
         }
      }
   }

   return;
} // ClusterFaces()


int SetFaces(
  CCluster* clusters,
  Ipp8u*    dst,
  int       height,
  int       dstStep,
  int       nchannels)
{
   int  i;
   int  j;
   int  iw;
   int  ih;
   int  xsum;
   int  ysum;
   int  wsum;
   bool equalrect = false;

   CCluster* mergedclusters = new CCluster;
   if(0 == mergedclusters)
     return -1;

   i = mergedclusters->Init(clusters->m_currentclustercount,clusters->m_currentclustercount);
   if(i != 0)
     return -1;

   for( i = 0, xsum = 0, ysum = 0, wsum = 0; i < clusters->m_currentclustercount; i++, xsum = 0, ysum = 0, wsum = 0 )
   {
      if( clusters->m_count[i] >= minneighbors )
      {
         int xface;
         int yface;
         int wface;

         for( j = 0; j < clusters->m_count[i]; j++ )
         {
            xsum += clusters->m_x[i][j];
            ysum += clusters->m_y[i][j];
            wsum += clusters->m_w[i][j];
         }

         wface = wsum/clusters->m_count[i];
         xface = xsum/clusters->m_count[i];
         yface = ysum/clusters->m_count[i];

         if( mergedclusters->m_currentclustercount )
         {
            for( int k = 0; k < mergedclusters->m_currentclustercount; k++ )
            {
               for( int l = 0; l < mergedclusters->m_count[k]; l++ )
               {
                  if( is_equal(xface,yface,wface,mergedclusters->m_x[k][l],mergedclusters->m_y[k][l],mergedclusters->m_w[k][l],distfactor,distfactorrect) )
                  {
                     mergedclusters->m_x[k][mergedclusters->m_count[k]] = xface;
                     mergedclusters->m_y[k][mergedclusters->m_count[k]] = yface;
                     mergedclusters->m_w[k][mergedclusters->m_count[k]] = wface;
                     mergedclusters->m_count[k]++;
                     equalrect = true;
                     break;
                  }
               }

               if( equalrect )
                 break;
            }

            if( !equalrect )
            {
               mergedclusters->m_x[mergedclusters->m_currentclustercount][0] = xface;
               mergedclusters->m_y[mergedclusters->m_currentclustercount][0] = yface;
               mergedclusters->m_w[mergedclusters->m_currentclustercount][0] = wface;
               mergedclusters->m_count[mergedclusters->m_currentclustercount] = 1;
               mergedclusters->m_currentclustercount++;
            }

            equalrect = false;
         }
         else
         {
            mergedclusters->m_w[0][0] = wface;
            mergedclusters->m_x[0][0] = xface;
            mergedclusters->m_y[0][0] = yface;
            mergedclusters->m_count[0] = 1;
            mergedclusters->m_currentclustercount++;
         }
      }
   }

   for( i = 0, xsum = 0, ysum = 0, wsum = 0; i < mergedclusters->m_currentclustercount; i++, xsum = 0, ysum = 0, wsum = 0 )
   {
      int xface, yface, wface;

      for( j = 0; j < mergedclusters->m_count[i]; j++ )
      {
         xsum += mergedclusters->m_x[i][j];
         ysum += mergedclusters->m_y[i][j];
         wsum += mergedclusters->m_w[i][j];
      }

      wface = wsum/mergedclusters->m_count[i];
      xface = xsum/mergedclusters->m_count[i] - wface/2;
      yface = ysum/mergedclusters->m_count[i] - wface/2;

      for( iw = xface; iw < xface + wface; iw++ )
      {
         dst[iw*nchannels+(height-yface)*dstStep  ] = 255;
         dst[iw*nchannels+(height-yface)*dstStep+1] = 0;
         dst[iw*nchannels+(height-yface)*dstStep+2] = 0;

         dst[iw*nchannels+(height-(yface+wface))*dstStep  ] = 255;
         dst[iw*nchannels+(height-(yface+wface))*dstStep+1] = 0;
         dst[iw*nchannels+(height-(yface+wface))*dstStep+2] = 0;
      }

      for( ih = yface; ih < yface + wface; ih++ )
      {
         dst[xface*nchannels+(height-ih)*dstStep  ] = 255;
         dst[xface*nchannels+(height-ih)*dstStep+1] = 0;
         dst[xface*nchannels+(height-ih)*dstStep+2] = 0;

         dst[(xface+wface)*nchannels+(height-ih)*dstStep  ] = 255;
         dst[(xface+wface)*nchannels+(height-ih)*dstStep+1] = 0;
         dst[(xface+wface)*nchannels+(height-ih)*dstStep+2] = 0;
      }
   }

   if(0 != mergedclusters)
     delete mergedclusters;

   return 0;
} // SetFaces()


IppStatus FreeHaarClassifier(void)
{
   int       ii;
   IppStatus status;

   for( ii = 0; ii < stages; ii++ )
   {
      status = ippiHaarClassifierFree_32f(pHaar[ii]);
      if(ippStsNoErr != status)
        return status;
   }

   if(0 != pHaar)
   {
     delete pHaar;
     pHaar = 0;
   }

   if(0 != sThreshold)
   {
     delete sThreshold;
     sThreshold = 0;
   }

   if(0 != nLength)
   {
     delete nLength;
     nLength = 0;
   }

   if(0 != nClass)
   {
     delete nClass;
     nClass = 0;
   }

   if(0 != nFeat)
   {
     delete nFeat;
     nFeat = 0;
   }

   if(0 != pNum)
   {
     delete pNum;
     pNum = 0;
   }

   if(0 != pThreshold)
   {
     delete pThreshold;
     pThreshold = 0;
   }

   if(0 != pVal1)
   {
     delete pVal1;
     pVal1 = 0;
   }

   if(0 != pVal2)
   {
     delete pVal2;
     pVal2 = 0;
   }

   if(0 != pFeature)
   {
     delete pFeature;
     pFeature = 0;
   }

   if(0 != pWeight)
   {
     delete pWeight;
     pWeight = 0;
   }

   if(0 != nStnum)
   {
     delete nStnum;
     nStnum = 0;
   }

   return ippStsNoErr;
} // FreeHaarClassifier()


const char* ReadLineFromMem( const char* ptr, char* buf )
{
   const char* endptr = strchr((char*)ptr, '\n');
   if( !endptr )
     endptr = ptr + strlen(ptr);
   strncpy( buf, ptr, (int)(endptr - ptr) );
   buf[endptr - ptr] = '\0';
   for( ptr = endptr; *ptr == '\n' || *ptr == '\r'; ptr++ )
     ;
   return ptr;
}

IppStatus ReadHaarClassifier(const char* haardata)
{
   int       ii;
   int       jj;
   int       kk;
   int       jjj    = 0;
   int       kkk    = 0;

   //FILE* file = fopen(haarfname, "r");
   //if( !file )
   //  return ippStsBadArgErr;
   const int N = 1024;
   char line[N+2];
   const char *ptr = haardata;

   ptr = ReadLineFromMem(ptr, line);
   sscanf(line, "%d %d %d %d %d", &(face.width), &(face.height), &stages, &classifiers, &features);

   pHaar      = new IppiHaarClassifier_32f*[stages];
   if(0 == pHaar)
     return ippStsErr;

   sThreshold = new Ipp32f  [stages];
   if(0 == sThreshold)
     return ippStsErr;

   nLength    = new int     [stages];
   if(0 == nLength)
     return ippStsErr;

   nStnum     = new int     [stages];
   if(0 == nStnum)
     return ippStsErr;

   nClass     = new int     [stages];
   if(0 == nClass)
     return ippStsErr;

   nFeat      = new int     [stages];
   if(0 == nFeat)
     return ippStsErr;

   pNum       = new int     [classifiers];
   if(0 == pNum)
     return ippStsErr;

   pThreshold = new Ipp32f  [classifiers];
   if(0 == pThreshold)
     return ippStsErr;

   pVal1      = new Ipp32f  [classifiers];
   if(0 == pVal1)
     return ippStsErr;

   pVal2      = new Ipp32f  [classifiers];
   if(0 == pVal2)
     return ippStsErr;

   pFeature   = new IppiRect[features];
   if(0 == pFeature)
     return ippStsErr;

   pWeight    = new Ipp32f  [features];
   if(0 == pWeight)
     return ippStsErr;

   for( ii = 0; ii < stages; ii++ )
   {
      ptr = ReadLineFromMem(ptr, line);
      sscanf(line, "%d %g", nLength+ii, sThreshold+ii);
      nStnum[ii] = 0;
      nClass[ii] = jjj;
      nFeat[ii]  = kkk;

      for( jj = 0; jj < nLength[ii]; jjj++, jj++ )
      {
         int nread = 0;
         char* lptr = line;
         ptr = ReadLineFromMem(ptr, line);
         sscanf(lptr, "%d%n", pNum+jjj, &nread);
         lptr += nread;
         nStnum[ii] += pNum[jjj];

         for( kk = 0; kk < pNum[jjj]; kkk++, kk++ )
         {
            nread = 0;
            sscanf(lptr, "%d %d %d %d %g%n",
              &((pFeature+kkk)->x), &((pFeature+kkk)->y),
              &((pFeature+kkk)->width), &((pFeature+kkk)->height),
              pWeight+kkk, &nread );
            lptr += nread;
         }

         ptr = ReadLineFromMem(ptr, line);
         sscanf(line, "%g %g %g", pThreshold+jjj, pVal1+jjj, pVal2+jjj);
      }
   }

   //fclose(file);

   if( (jjj != classifiers) || (kkk != features) )
      return ippStsBadArgErr;

   return ippStsNoErr;
} // ReadHaarClassifier()


IppStatus AdjustHaarClassifier( int bord, float decstage)
{
   int       ii;
   int       jj;
   IppiSize  stageClassifierSize;
   IppStatus status;

   stageClassifierSize.width  = 0;
   stageClassifierSize.height = 0;

   float scale = 1.0f / (float)((face.width - bord - bord) * (face.height - bord - bord));

   for( jj = 0; jj < features; jj++ )
     pWeight[jj] *= scale;

   for( ii = 0; ii < stages; ii++ )
     sThreshold[ii] -= decstage;

   for( jj = 0; jj < features; jj++ )
      pFeature[jj].y = face.height - pFeature[jj].y - pFeature[jj].height;

   for( ii = 0; ii < stages; ii++ )
   {
      status = ippiHaarClassifierInitAlloc_32f(pHaar+ii,pFeature+nFeat[ii],pWeight+nFeat[ii],pThreshold+nClass[ii],pVal1+nClass[ii],pVal2+nClass[ii],pNum+nClass[ii],nLength[ii]);
      if( status != ippStsOk )
        return status;

      status = ippiGetHaarClassifierSize_32f(pHaar[ii], &stageClassifierSize);
      if( status != ippStsOk )
        return status;

      if( stageClassifierSize.width > classifierSize.width )
         classifierSize.width = stageClassifierSize.width;

      if( stageClassifierSize.height > classifierSize.height )
         classifierSize.height = stageClassifierSize.height;
   }

   return ippStsNoErr;
} // AdjustHaarClassifier()


IppStatus PruningSetRow(Ipp8u* pMask, int maskStep, IppiSize roi, int nh)
{
    int       i;
    IppiSize  pruningRoi;
    IppStatus status;

    if (nh <= 1)
      nh = 1;

    pruningRoi.height = nh;
    pruningRoi.width  = roi.width;

    if (roi.height > nh)
    {
        for (i = 0; i < roi.height; i += nh + 1)
        {
            if (i + nh > roi.height)
              pruningRoi.height = roi.height - 1 - i;

            if (pruningRoi.height > 0)
              status = ippiSet_8u_C1R (0, pMask+maskStep, maskStep, pruningRoi);

            if (ippStsNoErr != status)
            {
              return status;
            }

            pMask += maskStep*(nh+1);
        }
    }

    return ippStsNoErr;
} // PruningSetRow()


IppStatus PruningSetCol(Ipp8u* pMask, int maskStep, IppiSize roi, int nw)
{
    int       i;
    int       j;
    int       k;
    int       m;
    IppiSize  pruningRoi;

    if (nw <= 1)
      nw = 1;

    pruningRoi.height = roi.height;
    pruningRoi.width  = nw;

    if (roi.width > nw)
    {
        for (i = 0; i < roi.height; i++)
        {
            for (j = 0; j < roi.width; j += nw + 1)
            {
               m = nw;
               if (j + nw > roi.width - 1)
                 m = roi.width - 1 - j;

               for (k = 0; k < m; k++)
                 pMask[j+1+k] = 0;
            }

            pMask += maskStep;
        }
    }

    return ippStsNoErr;
} // PruningSetCol()


IppStatus PruningSetRowColMix(Ipp8u* pMask, int maskStep, IppiSize roi, int nh, int nw)
{
    int       i;
    int       j;
    int       k;
    int       m;
    IppiSize  pruningRoi;
    IppStatus status;

    if (nh <= 1)
      nh = 1;

    if (nw <= nh)
      nw = 1;

    pruningRoi.height = nh;
    pruningRoi.width  = roi.width;

    if (roi.height > nh)
    {
        for (i = 0; i < roi.height; i += nh + 1)
        {
            for (j = 0; j < roi.width; j += nw + 1)
            {
               m = nw;
               if (j + nw > roi.width - 1)
                 m = roi.width - 1 - j;

               for (k = 0; k < m; k++)
                 pMask[j+1+k] = 0;
            }

            if (i + nh > roi.height)
              pruningRoi.height = roi.height - 1 - i;

            if (pruningRoi.height > 0)
              status = ippiSet_8u_C1R(0, pMask + maskStep, maskStep, pruningRoi);

            if (ippStsNoErr != status)
            {
                return status;
            }

            pMask += maskStep * (nh + 1);
        }
    }

    return ippStsNoErr;
} // PruningSetRowColMix()


int facedetection_filter(const CIppImage& src, PARAMS_FCDFLT& params, CIppImage& dst)
{
  int       i;
  int       pruningParam  = 1;
  int       pruningParam2 = 1;
  int       maxfacecount;
  int       maxrectcount;
  Ipp32s    bord      = 1;
  Ipp32f    factor    = 1.0f;
  Ipp32f    decthresh = 0.0001f;
  IppiSize  roi = {3, 3};
  IppiSize  roi0;
  IppiSize  roi1;
  IppiRect  rect;
  IppiRect  rect0;
  IppStatus status;

  roi0.width  = src.Width();
  roi0.height = src.Height();

  rect0.x = 0;
  rect0.y = 0;
  rect0.width  = roi0.width;
  rect0.height = roi0.height;
  QResource res(cascade_name);

  status = ReadHaarClassifier((const char*)res.data());

  if(ippStsNoErr != status)
  {
    return -1;
  }

  status = AdjustHaarClassifier(bord, decthresh);
  if(ippStsNoErr != status)
    return -1;

  rect.x = bord;
  rect.y = bord;

  int tmpStep;
  Ipp8u* pTmp = ippiMalloc_8u_C1(roi0.width, roi0.height, &tmpStep);
  if(0 == pTmp)
    return -1;

  if(src.NChannels() == 3)
  {
    ippiCopy_8u_C3R((const Ipp8u*)src, src.Step(), (Ipp8u*)dst, dst.Step(), src.Size());
    ippiRGBToGray_8u_C3C1R((const Ipp8u*)src, src.Step(), pTmp, tmpStep, roi0);
  }
  else if(src.NChannels() == 1)
  {
    ippiCopy_8u_C1R((const Ipp8u*)src, src.Step(), (Ipp8u*)dst, dst.Step(), src.Size());
    ippiCopy_8u_C1R((const Ipp8u*)src, src.Step(), pTmp, tmpStep, roi0);
  }
  else if(src.NChannels() == 4)
  {
    ippiCopy_8u_C4R((const Ipp8u*)src, src.Step(), (Ipp8u*)dst, dst.Step(), src.Size());
    ippiRGBToGray_8u_AC4C1R((const Ipp8u*)src, src.Step(), pTmp, tmpStep, roi0);
  }

  status = ippiMirror_8u_C1IR(pTmp, tmpStep, roi0, ippAxsHorizontal);
  if(ippStsNoErr != status)
    return -1;

  if(params.maxfacew <= 0)
    params.maxfacew = roi0.width;

  if(params.minfacew <= 0)
    params.minfacew = face.width;

  if(face.width > 0)
    factor = ((float)params.minfacew) / ((float)face.width);

  maxfacecount = ((roi0.width/face.width) * (roi0.height/face.height)/3);
  maxrectcount = (int)(distfactor*distfactor*params.maxfacew/params.minfacew);
  maxfacecount = IPP_MIN(maxfacecount, 100);
  maxrectcount = IPP_MIN(maxrectcount, 1000);

  int src32fStep;
  Ipp32f* src32f = ippiMalloc_32f_C1((int)(roi0.width / factor + 2),(int)(roi0.height/factor + 2),&src32fStep);
  if(0 == src32f)
    return -1;

  int sqrStep;
  Ipp64f* sqr = (Ipp64f*)ippiMalloc_32fc_C1((int)(roi0.width / factor + 2),(int)(roi0.height / factor + 2),&sqrStep);
  if(0 == sqr)
    return -1;

  int normStep;
  Ipp32f* norm = ippiMalloc_32f_C1((int)(roi0.width/factor),(int)(roi0.height/factor),&normStep);
  if(0 == norm)
    return -1;

  int maskStep;
  Ipp8u* mask = ippiMalloc_8u_C1((int)(roi0.width/factor),(int)(roi0.height/factor),&maskStep);
  if(0 == mask)
    return -1;

  if(maxfacecount > 0 && maxrectcount > 0)
  {
    CCluster* clusters = new CCluster;
    if(0 == clusters)
      return -1;

    i = clusters->Init(maxrectcount, maxfacecount);
    if(i != 0)
      return -1;

    for( ; roi0.width/factor > face.width+5 && roi0.height/factor > face.height+5 && face.width*factor < params.maxfacew; factor *= params.sfactor )
    {
      int src8uStep;

      roi.width  = (int)(roi0.width/factor);
      roi.height = (int)(roi0.height/factor);

      Ipp32s bufSize = 0;
      IppiRect dstRect;


      dstRect.x = 0;
      dstRect.y = 0;
      dstRect.width  = roi.width;
      dstRect.height = roi.height;

      status = ippiResizeGetBufSize(rect0, dstRect, 1, IPPI_INTER_LANCZOS, &bufSize);
      if(ippStsNoErr != status)
        return -1;

      Ipp8u* src8u = ippiMalloc_8u_C1(roi.width, roi.height, &src8uStep);
      if(0 == src8u)
        return -1;

      Ipp8u* buf = ippsMalloc_8u(bufSize);
      if(0 == buf)
        return -1;

      status = ippiResizeSqrPixel_8u_C1R(pTmp, roi0, tmpStep, rect0, src8u,
                                         src8uStep, dstRect, 1.0/factor,1.0/factor,
                                         0.0, 0.0, IPPI_INTER_LANCZOS, buf);
      ippFree(buf);

      if(ippStsNoErr != status)
        return -1;


      status = ippsSet_8u(0, mask, maskStep*roi.height);
      if(ippStsNoErr != status)
        return -1;

      roi1.width  = roi.width  - classifierSize.width  + 1;
      roi1.height = roi.height - classifierSize.height + 1;

      rect.width  = face.width  - bord - bord;
      rect.height = face.height - bord - bord;

      status = ippiSqrIntegral_8u32f64f_C1R(src8u,src8uStep,src32f,src32fStep,sqr,sqrStep,roi,(Ipp32f)(-(1<<24)),0.0);
      if(ippStsNoErr != status)
        return -1;

      status = ippiRectStdDev_32f_C1R(src32f,src32fStep,sqr,sqrStep,norm,normStep,roi1,rect);
      if(ippStsNoErr != status)
        return -1;

      status = ippiSet_8u_C1R(1,mask,maskStep,roi1);
      if(ippStsNoErr != status)
        return -1;


      switch(params.pruning)
      {
        case RowPruning:
          PruningSetRow(mask, maskStep, roi1, pruningParam);
          break;

        case ColPruning:
          PruningSetCol(mask, maskStep, roi1, pruningParam2);
          break;

        case RowColMixPruning:
          PruningSetRowColMix(mask,maskStep, roi1,pruningParam, pruningParam2);
          break;
      }

      positive = roi1.width * roi1.height;

      for( i = 0; i < stages; i++ )
      {
        status = ippiApplyHaarClassifier_32f_C1R(src32f,src32fStep,norm,normStep,mask,maskStep,roi1,&positive,sThreshold[i],pHaar[i]);
        if(ippStsNoErr != status)
          return -1;

        if( !positive )
          break;
      }

      ClusterFaces(mask,roi,maskStep,clusters,face,factor);

      if(0 != src8u)
      {
        ippiFree(src8u);
        src8u = 0;
      }
    }

    SetFaces(clusters, (Ipp8u*)dst, dst.Height(), dst.Step(), src.NChannels());
    if(0 != clusters)
    {
      delete clusters;
      clusters = 0;
    }
  }

  if(0 != src32f)
    ippiFree(src32f);

  if(0 != sqr)
    ippiFree(sqr);

  if(0 != norm)
    ippiFree(norm);

  if(0 != mask)
    ippiFree(mask);

  if(0 != pTmp)
    ippiFree(pTmp);

  FreeHaarClassifier();

  return 0;
} // facedetection_filter()

