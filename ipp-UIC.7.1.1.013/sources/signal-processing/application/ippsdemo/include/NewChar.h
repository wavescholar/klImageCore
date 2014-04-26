/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewChar.h: interface for the CNewChar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __NEWCHAR_H__
#define __NEWCHAR_H__

#pragma once

class CNewChar
{
public:
   CNewChar();
   virtual ~CNewChar();

   BOOL Dialog();
   BOOL CreateVector(CVector* pVector);
protected:
   BOOL m_bHello;
   ppType m_Type;
   int m_length;

   void CreateHelloWorld(CVector* pVector);
};

#endif // !defined __NEWCHAR_H__
