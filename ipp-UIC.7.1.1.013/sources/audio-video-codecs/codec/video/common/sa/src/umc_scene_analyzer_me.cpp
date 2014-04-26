/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_scene_analyzer_me.h"
#include "ippcore.h"
#include "ipps.h"
#include "ippvc.h"

using namespace UMC;

#ifndef INT_MAX
#   define INT_MAX 2147483647
#endif

#define GET_SAD(X,Y) \
    ippiSAD4x4_8u32s(pSrc,srcStep,pRef+refStep*(Y)+(X),refStep,&sad,0); \
    if(sad_min > sad){ \
      sad_min = sad; \
      x_min = (X); \
      y_min = (Y); \
      min_trig = 1; \
    }

SceneAnalyzerME::SceneAnalyzerME()
{
  prevMV.x = prevMV.y = 0;
  upperMVs = NULL;
}

SceneAnalyzerME::~SceneAnalyzerME()
{
  prevMV.x = prevMV.y = 0;
  if(upperMVs)ippFree(upperMVs);
}

void SceneAnalyzerME::MakePPictureCodingDecision(SceneAnalyzerPicture* pRef, SceneAnalyzerPicture* pSrc)
{
  if(pSrc->m_info.averageDev[SA_INTER_ESTIMATED] > 4) // compensate fast ME
    pSrc->m_info.averageDev[SA_INTER_ESTIMATED] = pSrc->m_info.averageDev[SA_INTER_ESTIMATED]-3;

  SceneAnalyzerP::MakePPictureCodingDecision(pRef,pSrc);
}


// Merged basic and new functions
Status SceneAnalyzerME::AnalyzePicture(SceneAnalyzerPicture* pRef, SceneAnalyzerPicture* pSrc)
{
    UMC_SCENE_INFO* pSliceInfo;
    const Ipp8u* pbSrc;
    Ipp32s srcStep;
    const Ipp8u* pbRef;
    Ipp32s refStep;
    Ipp32u mbY;

    // initalize new class parameters (additonal block)
    if(upperMVs == NULL)
      upperMVs = (IppiPoint*)ippMalloc(sizeof(IppiPoint)*pSrc->m_mbDim.width);

    if(upperMVs == NULL)
      return UMC_ERR_ALLOC;

    ippsSet_8u(0,(Ipp8u*)upperMVs,sizeof(IppiPoint)*pSrc->m_mbDim.width);
    prevMV.x = prevMV.y = 0;
    // end of (additonal block)

    // reset the variables
    pSliceInfo = pSrc->m_pSliceInfo;
    memset(&(pSrc->m_info), 0, sizeof(pSrc->m_info));

    // get the source pointer
    pbSrc = (const Ipp8u *) pSrc->m_pPic[0];
    srcStep = (Ipp32s) pSrc->m_picStep;
    pbRef = (const Ipp8u *) pRef->m_pPic[0];
    refStep = (Ipp32s) pRef->m_picStep;

    // cycle over rows
    for (mbY = 0; mbY < (Ipp32u) pSrc->m_mbDim.height; mbY += 1)
    {
        UMC_SCENE_INFO sliceInfo;
        Ipp32u mbX;

        // reset the variables
        memset(&sliceInfo, 0, sizeof(sliceInfo));

        // cycle in the row
        for (mbX = 0; mbX < (Ipp32u) pSrc->m_mbDim.width; mbX += 1)
        {
            UMC_SCENE_INFO mbInfo;

            // reset MB info
            memset(&mbInfo, 0, sizeof(mbInfo));
            mbInfo.numItems[SA_INTRA] = 1;
            mbInfo.numItems[SA_COLOR] = 1;
            mbInfo.numItems[SA_INTER] = 1;
            mbInfo.numItems[SA_INTER_ESTIMATED] = 1;

            // get the dispersion
            AnalyzeInterMB(pbRef + mbX * 4, refStep,
                           pbSrc + mbX * 4, srcStep,
                           &mbInfo);

            // the difference is to big,
            // try to find a better match
            if (10 < mbInfo.sumDev[SA_INTER])// new threshold are implemented here (1 -> 10)
            {
                AnalyzeInterMBMotion(pbRef + mbX * 4, refStep,
                                     pRef->m_mbDim,
                                     pbSrc + mbX * 4, srcStep,
                                     mbX, mbY, pSrc->m_pSadBuffer,
                                     &mbInfo);
            }

            // update slice info
            if ((mbInfo.sumDev[SA_INTRA]) || (mbInfo.sumDev[SA_INTER]))
            {
                AddIntraDeviation(&sliceInfo, &mbInfo);
                AddInterDeviation(&sliceInfo, &mbInfo);
            }
        }

        // update frame info
        AddIntraDeviation(&(pSrc->m_info), &sliceInfo);
        AddInterDeviation(&(pSrc->m_info), &sliceInfo);

        // get average for slice & update slice info
        if (pSliceInfo)
        {
            GetAverageIntraDeviation(&sliceInfo);
            GetAverageInterDeviation(&sliceInfo);
            *pSliceInfo = sliceInfo;
            pSliceInfo += 1;
        }

        // advance the pointers
        pbSrc += 4 * srcStep;
        pbRef += 4 * refStep;
    }

    // get average for frame
    GetAverageIntraDeviation(&(pSrc->m_info));
    GetAverageInterDeviation(&(pSrc->m_info));
    //
    // make the frame type decision
    //

    MakePPictureCodingDecision(pRef, pSrc);

    return UMC_OK;

} // SceneAnalyzerP::AnalyzePicture()


void SceneAnalyzerME::AnalyzeInterMBMotion(
  const Ipp8u*          pRef,
        Ipp32s          refStep,
        IppiSize        refMbDim,
  const Ipp8u*          pSrc,
        Ipp32s          srcStep,
        Ipp32u          mbX,
        Ipp32u          mbY,
        Ipp16u*         pSADs,
        UMC_SCENE_INFO* pMbInfo)
{
  Ipp32s k, min_trig, num_steps;
  Ipp32s top, left, right, bottom, searchHeight, searchWidth, frameWidth, frameHeight; // borders
  Ipp32s x, y, x_cur = mbX*4, y_cur = mbY*4, x_mid = 0, y_mid = 0, x_min = 0, y_min = 0; // coordinates
  Ipp32s searchStep; // search step
  Ipp32s sad_min = INT_MAX,sad; // like MAD

  pSADs = pSADs;
  // init borders
  frameWidth  = refMbDim.width*4;
  frameHeight = refMbDim.height*4;
  searchHeight = SA_ESTIMATION_HEIGHT/2;
  searchWidth  = SA_ESTIMATION_WIDTH/2;

  top = y_cur-searchHeight;
  if(top < 0)
    top = 0;

  bottom = y_cur+searchHeight;
  if(bottom > frameHeight-4)
    bottom = frameHeight-4;

  left = x_cur-searchWidth;
  if(left < 0)
    left = 0;

  right = x_cur+searchWidth;
  if(right > frameWidth-4)
    right = frameWidth-4;

  right  -= x_cur;
  left   -= x_cur;
  top    -= y_cur;
  bottom -= y_cur;

  // init search step
  searchStep = IPP_MAX(SA_ESTIMATION_HEIGHT>>3,SA_ESTIMATION_WIDTH>>3);
  if(searchStep < 1)
    searchStep = 1;

  // start
  // check initial search point
  if(prevMV.x <= right && prevMV.x >= left && prevMV.y <= bottom && prevMV.y >= top)
  {
    GET_SAD(prevMV.x,prevMV.y);
  }

  if(mbY > 0)
  {
    if(upperMVs[mbX].x <= right && upperMVs[mbX].x >= left && upperMVs[mbX].y <= bottom && upperMVs[mbX].y >= top)
    {
      GET_SAD(upperMVs[mbX].x,upperMVs[mbX].y);
    }
  }
  GET_SAD(0,0);

  if(sad_min > 209)
  {
    x_mid = x_min;
    y_mid = y_min;

    num_steps = IPP_MAX(SA_ESTIMATION_HEIGHT,SA_ESTIMATION_WIDTH);

    while(searchStep > 0)
    {  
      for(k = 0; k < num_steps; k++)
      {
        min_trig = 0;

        x = x_mid - searchStep;
        y = y_mid;
        if(x >= left)
        {
          GET_SAD(x,y);
        }

        x = x_mid + searchStep;
        y = y_mid;
        if(x <= right)
        {
          GET_SAD(x,y);
        }

        x = x_mid;
        y = y_mid - searchStep;
        if(y >= top)
        {
          GET_SAD(x,y);
        }

        x = x_mid;
        y = y_mid + searchStep;
        if(y <= bottom)
        {
          GET_SAD(x,y);
        }

        x_mid = x_min;
        y_mid = y_min;

        if(!min_trig)
          break;
      }

      if(sad_min == 0)
        break;

      searchStep >>= 1;
    }
  }

  upperMVs[mbX].x = prevMV.x = x_min;
  upperMVs[mbX].y = prevMV.y = y_min;

  pMbInfo->sumDev[SA_INTER_ESTIMATED] = (sad_min+8)/16;

  return;
} // SceneAnalyzerME::AnalyzeInterMBMotion()

