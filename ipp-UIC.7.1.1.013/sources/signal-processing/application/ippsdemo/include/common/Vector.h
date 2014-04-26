/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Vector.h: interface for the CVector, CVectorUnit classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__1B2AFFD1_C617_4E15_8179_D310860408AA__INCLUDED_)
#define AFX_VECTOR_H__1B2AFFD1_C617_4E15_8179_D310860408AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ippdefs.h>
#include "Value.h"
class CHisto;

class CVectorUnit;

//////////////////////////////////////////////////////////////////////
// class CVoi is auxiliary class for CVector implementation
// It defines subvector (Vector-of-interest)
//////////////////////////////////////////////////////////////////////
class CVoi {
public:
   CVoi(int offset = 0, int length = 0) : m_offset(offset), m_length(length) {}
   int m_offset;
   int m_length;
};

//////////////////////////////////////////////////////////////////////
// class CVector is very significant class for Demo applications.
// CVector is the base class of ippSP Demo documents.
// CVector is the base class of CImage that is the base class of 
// ippIP Demo documents.
// CVector is used as IPP vector parameter for IPP functions.
//////////////////////////////////////////////////////////////////////
class CVector  
{
public:
   CVector(ppType type = ppNONE, int len = 0);
   CVector(const CVector& vec);
   virtual ~CVector();
   CVector& operator =(const CVector& vec);
   CVector& operator =(CVectorUnit& val);
// Operations
   void Init(ppType type, int len = 0);
   void Allocate(int len);
   void Deallocate();
   void AllocateData(int size, char value);
   BOOL CopyData(const CVector& vec, int len = 0);
   void Set(int index, double value, double imval = 0);
   void Set(int index, Ipp64fc value);
   Ipp64fc Get(int index) const;
   int     GetInt(int index) const;
   double  GetDouble(int index) const;
   void Set(int index, CValue val);
   void Get(int index, CValue& val) const;
   void Set(int index, CString str);
   void Get(int index, CString& str, LPCTSTR cpxFormat = NULL) const;
   void Get(int index, CString& str, int nDigits, LPCTSTR cpxFormat = NULL) const;
   void Set(CMyString str);
   void Get(CMyString& str, CString separ = _T("\r\n")) const;
   void Get(CMyString& str, int nDigits, CString separ = _T("\r\n")) const;
   void Set(const CMyString* strArray, int num = -1);
   void Get(CMyString* strArray, int num = -1, int nDigits = 5) const;
   void* GetData() { return m_pointer;}
   const void* GetData() const { return m_pointer;}
   void SetLength(int len);
   void* GetDataVoi();
   const void* GetDataVoi() const;
// Type conversions
   operator Ipp8u  *() {return (m_type == pp8u  ) ? (Ipp8u  *)m_pointer : NULL;}
   operator Ipp8s  *() {return (m_type == pp8s  ) ? (Ipp8s  *)m_pointer : NULL;}
   operator Ipp8sc *() {return (m_type == pp8sc ) ? (Ipp8sc *)m_pointer : NULL;}
   operator Ipp16u *() {return (m_type == pp16u ) ? (Ipp16u *)m_pointer : NULL;}
   operator Ipp16s *() {return (m_type == pp16s ) ? (Ipp16s *)m_pointer : NULL;}
   operator Ipp16sc*() {return (m_type == pp16sc) ? (Ipp16sc*)m_pointer : NULL;}
   operator Ipp32u *() {return (m_type == pp32u ) ? (Ipp32u *)m_pointer : NULL;}
   operator Ipp32s *() {return (m_type == pp32s ) ? (Ipp32s *)m_pointer : NULL;}
   operator Ipp32sc*() {return (m_type == pp32sc) ? (Ipp32sc*)m_pointer : NULL;}
   operator Ipp32f *() {return (m_type == pp32f ) ? (Ipp32f *)m_pointer : NULL;}
   operator Ipp32fc*() {return (m_type == pp32fc) ? (Ipp32fc*)m_pointer : NULL;}
   operator Ipp64s *() {return (m_type == pp64s ) ? (Ipp64s *)m_pointer : NULL;}
   operator Ipp64sc*() {return (m_type == pp64sc) ? (Ipp64sc*)m_pointer : NULL;}
   operator Ipp64f *() {return (m_type == pp64f ) ? (Ipp64f *)m_pointer : NULL;}
   operator Ipp64fc*() {return (m_type == pp64fc) ? (Ipp64fc*)m_pointer : NULL;}

   operator Ipp8u  () {return ((Ipp8u  *)m_pointer)[0];}
   operator Ipp8s  () {return ((Ipp8s  *)m_pointer)[0];}
   operator Ipp8sc () {return ((Ipp8sc *)m_pointer)[0];}
   operator Ipp16u () {return ((Ipp16u *)m_pointer)[0];}
   operator Ipp16s () {return ((Ipp16s *)m_pointer)[0];}
   operator Ipp16sc() {return ((Ipp16sc*)m_pointer)[0];}
   operator Ipp32u () {return ((Ipp32u *)m_pointer)[0];}
   operator Ipp32s () {return ((Ipp32s *)m_pointer)[0];}
   operator Ipp32sc() {return ((Ipp32sc*)m_pointer)[0];}
   operator Ipp32f () {return ((Ipp32f *)m_pointer)[0];}
   operator Ipp32fc() {return ((Ipp32fc*)m_pointer)[0];}
   operator Ipp64s () {return ((Ipp64s *)m_pointer)[0];}
   operator Ipp64sc() {return ((Ipp64sc*)m_pointer)[0];}
   operator Ipp64f () {return ((Ipp64f *)m_pointer)[0];}
   operator Ipp64fc() {return ((Ipp64fc*)m_pointer)[0];}
// Attributes
   int Length() const { return m_len;}
   int ItemSize() const { return (Depth() >> 3) << (int)Complex();}
   int Size() const { return m_len*ItemSize();}
   ppType Type() const { return m_type;}
   static int Depth(ppType type) { return type & PP_MASK;}
   int Depth() const { return Depth(m_type);}
   static bool Unsign(ppType type) { return !Sign(type) && !Float(type);}
   bool Unsign() const { return Unsign(m_type);}
   static bool Sign(ppType type) { return type & PP_SIGN ? true : false;}
   bool Sign() const { return Sign(m_type);}
   static bool Float(ppType type) { return type & PP_FLOAT ? true : false;}
   bool Float() const { return Float(m_type);}
   static bool Complex(ppType type) { return type & PP_CPLX ? true : false;}
   bool Complex() const { return Complex(m_type);}
   virtual CString TypeString() const;
   Ipp64fc Min() const;
   Ipp64fc Max() const;
   double MinMin() const;
   double MaxMax() const;
   double Magnitude() const;
   double Difference() const;
   static double Magnitude(double min, double max);
   int GetTapsFactor() { return m_TapsFactor;}
   void SetTapsFactor(int tapsFactor) { m_TapsFactor = tapsFactor;}

   BOOL IsVoi() const;
   CVoi GetVoi() const;
   void GetVoi(int& offset, int& length) const;
   int GetLengthVoi() const;
   void ResetVoi(int len = 0);
   BOOL SetVoi(CVoi voi);
   BOOL SetVoi(int offset, int length);
   BOOL CopyVoi(const CVector* srcVector);
   int GetPos() const;
   BOOL SetPos(int pos);

// History functions
   CHisto* GetHisto() const { return m_pHisto;}

   void CreateCustomData(CString dataName, void* pData, int dataSize);
   void DeleteCustomData();
   void* GetCustomData(CString dataName);

// Drop functions
   virtual int GetDropHeaderSize() const;
   virtual void WriteDropHeader(char* pData) const;
   virtual void ReadDropHeader(const char* pData);
   virtual int GetDropInfoSize() const;
   virtual void WriteDropInfo(char* pData) const;
   virtual void ReadDropInfo(const char* pData);
   virtual int GetDropDataSize() const;
   virtual void WriteDropData(char* pData) const;
   virtual void ReadDropData(const char* pData);

protected:
   void*    m_pointer;
   int      m_len;
   ppType   m_type;
   CVoi     m_Voi;
   char*    m_pCustomData;
   int      m_CustomSize;
   CString  m_CustomName;
   CHisto*  m_pHisto;
   int      m_TapsFactor;
};

class CVectorUnit  
{
public:
   CVectorUnit(ppType type = ppNONE);
   CVectorUnit(const CVectorUnit& val) { *this = val;}
   virtual ~CVectorUnit();
   CVectorUnit& operator =(const CVectorUnit& val);
   CVectorUnit& operator =(CVector& val);

   void Init(ppType type, int len = 0);
   void Init(ppType type, CMyString str);
   void Set(CMyString str);
   void Get(CMyString& str, CString separ = _T("\r\n")) const;
   void Set(const CMyString* strArray, int num = -1);
   void Get( CMyString* strArray, int num = -1) const;
   ppType Type() { return m_Type;}
   CString TypeString() { return CFunc::TypeToString(m_Type);}
   int      Length() { return Vector().Length();}
   CVector& Vector() { return Vector(m_Type);}
   CVector& Vector(ppType type);
   const CVector& Vector()const { return Vector(m_Type);}
   const CVector& Vector(ppType type)const;

   operator Ipp8u  *() { return (Ipp8u  *)Vector();}
   operator Ipp8s  *() { return (Ipp8s  *)Vector();}
   operator Ipp16u *() { return (Ipp16u *)Vector();}
   operator Ipp16s *() { return (Ipp16s *)Vector();}
   operator Ipp32u *() { return (Ipp32u *)Vector();}
   operator Ipp32s *() { return (Ipp32s *)Vector();}
   operator Ipp32f *() { return (Ipp32f *)Vector();}
   operator Ipp64s *() { return (Ipp64s *)Vector();}
   operator Ipp64f *() { return (Ipp64f *)Vector();}
   operator Ipp8sc *() { return (Ipp8sc *)Vector();} 
   operator Ipp16sc*() { return (Ipp16sc*)Vector();} 
   operator Ipp32sc*() { return (Ipp32sc*)Vector();} 
   operator Ipp32fc*() { return (Ipp32fc*)Vector();} 
   operator Ipp64sc*() { return (Ipp64sc*)Vector();} 
   operator Ipp64fc*() { return (Ipp64fc*)Vector();} 

   operator Ipp8u  () { return (Ipp8u  )Vector();}
   operator Ipp8s  () { return (Ipp8s  )Vector();}
   operator Ipp16u () { return (Ipp16u )Vector();}
   operator Ipp16s () { return (Ipp16s )Vector();}
   operator Ipp32u () { return (Ipp32u )Vector();}
   operator Ipp32s () { return (Ipp32s )Vector();}
   operator Ipp32f () { return (Ipp32f )Vector();}
   operator Ipp64s () { return (Ipp64s )Vector();}
   operator Ipp64f () { return (Ipp64f )Vector();}
   operator Ipp8sc () { return (Ipp8sc )Vector();} 
   operator Ipp16sc() { return (Ipp16sc)Vector();} 
   operator Ipp32sc() { return (Ipp32sc)Vector();} 
   operator Ipp32fc() { return (Ipp32fc)Vector();} 
   operator Ipp64sc() { return (Ipp64sc)Vector();} 
   operator Ipp64fc() { return (Ipp64fc)Vector();} 
protected:
   ppType m_Type;
   CVector m_NONE;
   CVector m_8u  ;
   CVector m_8s  ;
   CVector m_16u ;
   CVector m_16s ;
   CVector m_32u ;
   CVector m_32s ;
   CVector m_32f ;
   CVector m_64s ;
   CVector m_64f ;
   CVector m_8sc ; 
   CVector m_16sc; 
   CVector m_32sc; 
   CVector m_32fc; 
   CVector m_64sc; 
   CVector m_64fc; 
};

#endif // !defined(AFX_VECTOR_H__1B2AFFD1_C617_4E15_8179_D310860408AA__INCLUDED_)
