/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Wander.h: interface for the CWander class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANDER_H__CB571B6F_2B41_4C60_8E65_35663A91A111__INCLUDED_)
#define AFX_WANDER_H__CB571B6F_2B41_4C60_8E65_35663A91A111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWander
{
public:
   CWander();
   virtual ~CWander();
   int  GetLeft() { return (int)m_x - (m_w>>1);}
   int  GetTop() { return (int)m_y - (m_h>>1);}
   IppiSize GetSize() { IppiSize size = {m_w, m_h}; return size;}
   IppiRect GetRect() { IppiRect rect = {GetLeft(),GetTop(),m_w, m_h}; return rect;}
   void Init(int W, int H, int w, int h, double speed);
   void Next();

protected:
   BOOL m_bInit;
   BOOL m_bFirst;
   int m_W;
   int m_H;
   int m_w;
   int m_h;
   double m_x;
   double m_y;
   double m_dx;
   double m_dy;
};

#endif // !defined(AFX_WANDER_H__CB571B6F_2B41_4C60_8E65_35663A91A111__INCLUDED_)
