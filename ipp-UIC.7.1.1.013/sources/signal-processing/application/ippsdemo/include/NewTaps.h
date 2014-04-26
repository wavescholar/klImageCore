/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewTaps.h: interface for the CNewTaps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTAPS_H__DF93D2C7_A61A_4378_9AE0_63F8F10EE1FE__INCLUDED_)
#define AFX_NEWTAPS_H__DF93D2C7_A61A_4378_9AE0_63F8F10EE1FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewTaps  
{
public:
   CNewTaps();
   virtual ~CNewTaps();

   BOOL Dialog();
   BOOL CreateVector(CVector* pVector);
protected:
   ppType m_Type;
   int m_Filter;
   int m_length;
   double m_freqLow;
   double m_freqHigh;

   void CreateFIRTaps_32f(CVector* pVector);
   void CreateFIRTaps_64f(CVector* pVector);
   void CreateFIRTaps_32fc(CVector* pVector);
   void CreateFIRTaps_64fc(CVector* pVector);
};

#endif // !defined(AFX_NEWTAPS_H__DF93D2C7_A61A_4378_9AE0_63F8F10EE1FE__INCLUDED_)
