/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewSignal.h: interface for the CNewSignal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWSIGNAL_H__8C964CFC_26E8_4073_B0FC_1B5FC9700EFC__INCLUDED_)
#define AFX_NEWSIGNAL_H__8C964CFC_26E8_4073_B0FC_1B5FC9700EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
class CVector;

enum {newTone, newTrian, newJaehne, newRandU, newRandN, newSpike,  
      newNUM};

class CNewSignal  
{
public:
   CNewSignal();
   virtual ~CNewSignal();

   BOOL Dialog();
   BOOL CreateVector(CVector* pVector);
protected:
   ppType m_Type;
   int m_Length;
   int m_Form;   
   int m_Offset;
   CDlgItemArray m_FormItems[newNUM];

   void InitTone();
   void InitTrian();
   void InitJaehne();
   void InitRandU();
   void InitRandN();
   void InitSpike();

   BOOL SetTone(CVector* pVector);
   BOOL SetTrian(CVector* pVector);
   BOOL SetJaehne(CVector* pVector);
   BOOL SetRandU(CVector* pVector);
   BOOL SetRandN(CVector* pVector);
   BOOL SetSpike(CVector* pVector);

   void InitMin(CValue& val);
   void InitMax(CValue& val);
   void InitMean(CValue& val);
   void InitMagn(CValue& val);
};

#endif // !defined(AFX_NEWSIGNAL_H__8C964CFC_26E8_4073_B0FC_1B5FC9700EFC__INCLUDED_)
