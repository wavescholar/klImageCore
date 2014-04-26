/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Value.h: interface for the CValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUE_H__7B5A36A8_B3E6_4304_AB8B_BE109C546973__INCLUDED_)
#define AFX_VALUE_H__7B5A36A8_B3E6_4304_AB8B_BE109C546973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Func.h"

typedef enum {argSRC, argDST} EArg;

//////////////////////////////////////////////////////////////////////
// class CValue provides functionality for scalar IPP function 
// parameter that has got standard IPP type. In fact CValue contains 
// the list of components with different Ipp types. At the moment it 
// operates with current component that has required type. 
//////////////////////////////////////////////////////////////////////

class CValue  
{
public:
//////////////////////////////////////////////////////////////////////
// Constructor/Destructor 
   CValue(ppType type = ppNONE);
   CValue(const CValue& val) { *this = val;}
   virtual ~CValue();
   CValue& operator =(const CValue& val);

//////////////////////////////////////////////////////////////////////
// Init functions initialize CValue by type, by type and value or
// by function and argument position. In the last case CValue will be
// initialized by the type of correcponding function parameter.
// You can do several initializations for different types one after 
// another and the last initialization will define the Ipp type of 
// current component. initialization without values switches current 
// component but preserves component value.
   void Init(ppType type);
   void Init(ppType type, double re, double im = 0);
   void Init64s(ppType type, Ipp64s re, Ipp64s im = 0);
   void Init(CFunc func, EArg argType = argSRC);

//////////////////////////////////////////////////////////////////////
// Set functions set value to CValue current component.  
   void Set(double re, double im = 0);
   void Set(Ipp64fc val) { Set(val.re,val.im);}
   void Set64s(Ipp64s re, Ipp64s im = 0);
   void Set64s(Ipp64sc val) { Set64s(val.re,val.im);}
   void Set(CMyString str);
   void Set(CString re, CString im);
   void SetRe(CString re);
   void SetIm(CString im);

//////////////////////////////////////////////////////////////////////
// Get functions get value from CValue current component.  
   void Get(CString& str, int nDigits = 5);
   void Get(CString& re, CString& im, int nDigits = 5);
   void Get(double& re, double& im);
   void Get(double& re)
   { double im; Get(re,im); }
   void Get(Ipp64fc& val)
   { double re, im; Get(re,im); val.re = re; val.im = im;}
   void Get(int& re, int& im) 
   { double dre, dim; Get(dre,dim); re = (int)dre; im = (int)dim;}
   void Get(int& re)
   { int im; Get(re,im); }
   void Get(Ipp64s& re, Ipp64s& im); 
   void Get(Ipp64s& re)
   { Ipp64s im; Get(re,im); }
   int     GetInt() { int res; Get(res); return res;}
   double  GetDouble() { double res; Get(res); return res;}

//////////////////////////////////////////////////////////////////////
// String functions convert current component value to text according 
// to specified format. 
// nDigits: number of significant digits to write
// cpxFormat: format for complex value, you should place "%s" to the 
// positios of re and im: "(%s, %s)"  
   CString String(LPCTSTR cpxFormat = NULL);
   CString String(int nDigits, LPCTSTR cpxFormat = NULL);
   CString StringRe(int nDigits = 5);
   CString StringIm(int nDigits = 5);

//////////////////////////////////////////////////////////////////////
// TypeString returns text presentation of current component type; 
// TypeStringRe returns text presentation without 'c' character if any 
   CString TypeString();
   CString TypeStringRe();

//////////////////////////////////////////////////////////////////////
// Various attrbutes of current component.
   ppType Type() const { return m_Type;}
   bool Unsign() const { return !Sign() && !Float();}
   bool Sign() const { return m_Type & PP_SIGN ? true : false;}
   bool Float() const { return m_Type & PP_FLOAT ? true : false;}
   bool Complex() const { return m_Type & PP_CPLX ? true : false;}
   int IntMin() const { return IntMin(Type());}
   int IntMax() const { return IntMax(Type());}

//////////////////////////////////////////////////////////////////////
// IntMin(Max) returns minimum(maximum) value of integer Ipp type
   static int IntMin(ppType type);
   static int IntMax(ppType type);

//////////////////////////////////////////////////////////////////////
// Operator Ipp[type] returns specified component  
   operator Ipp8u  () { return m_8u  ;}
   operator Ipp8s  () { return m_8s  ;}
   operator Ipp16u () { return m_16u ;}
   operator Ipp16s () { return m_16s ;}
   operator Ipp32u () { return m_32u ;}
   operator Ipp32s () { return m_32s ;}
   operator Ipp32f () { return m_32f ;}
   operator Ipp64s () { return m_64s ;}
   operator Ipp64f () { return m_64f ;}
   operator Ipp8sc () { return m_8sc ;} 
   operator Ipp16sc() { return m_16sc;} 
   operator Ipp32sc() { return m_32sc;} 
   operator Ipp32fc() { return m_32fc;} 
   operator Ipp64sc() { return m_64sc;} 
   operator Ipp64fc() { return m_64fc;} 

//////////////////////////////////////////////////////////////////////
// Operator Ipp[type]* returns pointer to specified component  
   operator Ipp8u  *() { return &m_8u  ;}
   operator Ipp8s  *() { return &m_8s  ;}
   operator Ipp16u *() { return &m_16u ;}
   operator Ipp16s *() { return &m_16s ;}
   operator Ipp32u *() { return &m_32u ;}
   operator Ipp32s *() { return &m_32s ;}
   operator Ipp32f *() { return &m_32f ;}
   operator Ipp64s *() { return &m_64s ;}
   operator Ipp64f *() { return &m_64f ;}
   operator Ipp8sc *() { return &m_8sc ;} 
   operator Ipp16sc*() { return &m_16sc;} 
   operator Ipp32sc*() { return &m_32sc;} 
   operator Ipp32fc*() { return &m_32fc;} 
   operator Ipp64sc*() { return &m_64sc;} 
   operator Ipp64fc*() { return &m_64fc;} 

protected:
   ppType m_Type;
   Ipp8u   m_8u  ;
   Ipp8s   m_8s  ;
   Ipp16u  m_16u ;
   Ipp16s  m_16s ;
   Ipp32u  m_32u ;
   Ipp32s  m_32s ;
   Ipp32f  m_32f ;
   Ipp64s  m_64s ;
   Ipp64f  m_64f ;
   Ipp8sc  m_8sc ; 
   Ipp16sc m_16sc; 
   Ipp32sc m_32sc; 
   Ipp32fc m_32fc; 
   Ipp64sc m_64sc; 
   Ipp64fc m_64fc; 

};


//////////////////////////////////////////////////////////////////////
// 
inline CMyString& operator << (CMyString& str, CValue val) {
   return str << val.String();
}

class CDlgItem {
public:
   enum {varNone, varType, varReal};
   CDlgItem() : m_min(0), m_max(0), m_valueVar(0) {}
   void Set(CValue value, CString name = _T(""), int min = 0, int max = 0) {
      m_value = value; m_name = name; m_unit = _T("");
      m_min = min; m_max = max; m_valueVar = 0;
   }
   void Set(CValue value, CString name, CString unit) {
      m_value = value; m_name = name; m_unit = unit;
      m_min = 0; m_max = 0; m_valueVar = 0;
   }
   void Set(int valueVar, CValue value, CString name = _T("")) {
      m_value = value; m_name = name; m_unit = _T("");
      m_min = 0; m_max = 0; m_valueVar = valueVar;
   }

   int m_valueVar;
   CValue  m_value;
   CString m_name;
   CString m_unit;
   int m_min;
   int m_max;
};

class CDlgItemArray : public CArray<CDlgItem,CDlgItem&> {
public:
   CDlgItemArray() {}
   CDlgItemArray(const CDlgItemArray& array) { 
      *this = array;
   }
   CDlgItemArray& operator =(const CDlgItemArray& array) {
      RemoveAll();
      Append(array);
      return *this;
   }
};

#endif // !defined(AFX_VALUE_H__7B5A36A8_B3E6_4304_AB8B_BE109C546973__INCLUDED_)
