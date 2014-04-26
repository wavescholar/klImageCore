/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Wander.cpp: implementation of the CWander class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Wander.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWander::CWander()
{
   m_bInit = FALSE;
   m_bFirst = FALSE;
   m_W = 0;
   m_H = 0;
   m_w = 0;
   m_h = 0;
   m_x = 0;
   m_y = 0;
   m_dx = 1;
   m_dy = 1;
}

CWander::~CWander()
{

}

void CWander::Init(int W, int H, int w, int h, double speed)
{
   m_W = W;
   m_H = H;
   m_w = w;
   m_h = h;
   m_bInit = TRUE;
   m_bFirst = TRUE;
   int nSide = rand() % 4;
   double angle = rand() % 140 + 10;
   if (angle > 80) angle += 20;
   angle *= IPP_PI/180;
   m_dx = cos(angle)*speed;
   m_dy = sin(angle)*speed;

   if (nSide < 2) {
      m_x = (rand() % (W - w)) + (w>>1);
      if (nSide & 1) {
         m_y = h >> 1;
      } else {
         m_y = H - (h >> 1);
         m_dx = -m_dx;
         m_dy = -m_dy;
      }
   } else {
      m_y = (rand() % (H - h)) + (h>>1);
      if (nSide & 1) {
         m_x = w >> 1;
         m_dx = -m_dy;
         m_dy =  m_dx;
      } else {
         m_x = W - (w >> 1);
         m_dx =  m_dy;
         m_dy = -m_dx;
      }
   }
}

void CWander::Next()
{
   if (!m_bInit) return;
   if (m_bFirst) {
      m_bFirst = FALSE;
      return;
   }
   double x = m_x + m_dx;
   if (((int)x - (m_w>>1) < 0) || ((int)x + (m_w>>1) + 1 >= m_W)) {
      m_dx = -m_dx;
      x = m_x + m_dx;
   }
   double y = m_y + m_dy;
   if (((int)y - (m_h>>1) < 0) || ((int)y + (m_h>>1) + 1 >= m_H)) {
      m_dy = -m_dy;
      y = m_y + m_dy;
   }
   m_x = x;
   m_y = y;
}

