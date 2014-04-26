/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __FACEDETECTION_H__
#define __FACEDETECTION_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


class CCluster
{
  public:
    int** m_x;
    int** m_y;
    int** m_w;
    int*  m_count;
    int   m_csize;
    int   m_fsize;
    int   m_currentclustercount;

    CCluster(void);
    ~CCluster(void);

    int Init(int size, int clustercount);
};


typedef enum
{
   NoPruning        = 0,
   RowPruning       = 1,
   ColPruning       = 2,
   RowColMixPruning = 3,
   CannyPruning     = 4

} pruningType;


typedef struct
{
  int           nthreads;
  int           minfacew;
  int           maxfacew;
  float         sfactor;
  pruningType   pruning;

} PARAMS_FCDFLT;


int facedetection_filter(const CIppImage& src, PARAMS_FCDFLT& params, CIppImage& dst);

#endif // __FACEDETECTION_H__
