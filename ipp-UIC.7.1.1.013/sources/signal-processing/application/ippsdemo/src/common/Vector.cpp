/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include <math.h>
#include <ipps.h>

#include "demo.h"
#include "Vector.h"
#include "Value.h"
#include "Histo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector(ppType type, int len) :
   m_type(type), m_pointer(NULL), m_len(0)
{
   Allocate(len);
   m_pHisto = new CHisto;
   m_pCustomData = NULL;
}

CVector::CVector(const CVector& vec) :
   m_type(ppNONE), m_pointer(NULL), m_len(0)
{
   m_pCustomData = NULL;
   *this = vec;
   m_pHisto = new CHisto;
}

CVector::~CVector()
{
   Deallocate();
   delete m_pHisto;
   DeleteCustomData();
}

CVector& CVector::operator =(const CVector& vec)
{
   Init(vec.Type(), vec.Length());
   CopyData(vec);
   CreateCustomData(vec.m_CustomName, vec.m_pCustomData, vec.m_CustomSize);
   CopyVoi(&vec);
   m_TapsFactor = vec.m_TapsFactor;
   return *this;
}

void CVector::Init(ppType type, int len)
{
   m_pCustomData = NULL;
   m_type = type;
   ResetVoi();
   Allocate(len);
}

void CVector::Allocate(int len)
{
   Deallocate();
   if (len >= 0 && Depth() > 0) {
      m_len = len;
      AllocateData(Size(), 0);
   }
   ResetVoi(len);
   m_TapsFactor = 0;
}

void CVector::AllocateData(int size, char value)
{
   if (m_pointer) ippFree(m_pointer);
   m_pointer = (char*)ippMalloc(size ? size : 1);
   if (m_pointer == 0) throw _T("Out of memory");
   memset(m_pointer,value,size);
   if (size == 0) 
      m_len = 0;
   else if (m_len == 0)
      m_len = size / ItemSize();
}

void CVector::Deallocate()
{
   if (m_pointer) ippFree(m_pointer);
   m_pointer = NULL;
   m_len = 0;
}

BOOL CVector::CopyData(const CVector& vec, int len)
{
   if (m_type != vec.Type()) return FALSE;
   if (len == 0) len = vec.Length();
   if (len > Length()) len = Length();
   int size = len*ItemSize();
   memcpy(m_pointer,vec.m_pointer,size);
   return TRUE;
}

void CVector::SetLength(int len)
{
   if (m_type == ppNONE) return;
   if (len == m_len) return;

   int size = Size();
   void* buffer = malloc(size);
   memcpy(buffer,m_pointer,size);   
   Allocate(len);
   if (size > Size()) size = Size();
   memcpy(m_pointer,buffer,size);   
   free(buffer);
}

Ipp64fc CVector::Min() const
{
   Ipp64fc min = {0, 0};
   if (m_len == 0) return min;
   min = Get(0);   
   for (int i=1; i < m_len; i++) {
      Ipp64fc value = Get(i);
      if (min.re > value.re) min.re = value.re;
      if (min.im > value.im) min.im = value.im;
   }
   return min;
}

Ipp64fc CVector::Max() const
{
   Ipp64fc max = {0, 0};
   if (m_len == 0) return max;
   max = Get(0);   
   for (int i=1; i < m_len; i++) {
      Ipp64fc value = Get(i);
      if (max.re < value.re) max.re = value.re;
      if (max.im < value.im) max.im = value.im;
   }
   return max;
}

double  CVector::MinMin() const
{
   Ipp64fc min = Min();
   if (Complex()) {
      if (min.re > min.im) min.re = min.im;
   }
   return min.re;
}

double  CVector::MaxMax() const
{
   Ipp64fc max = Max();
   if (Complex()) {
      if (max.re < max.im) max.re = max.im;
   }
   return max.re;
}

double  CVector::Magnitude() const
{
   double min = MinMin();
   double max = MaxMax();
   return Magnitude(min, max);
}

double  CVector::Magnitude(double min, double max)
{
   if (min < 0) min = -min;
   if (max < 0) max = -max;
   if (max < min) max = min;
   return max;
}

double  CVector::Difference() const
{
   return MaxMax() - MinMin();
}

void CVector::Set(int index, Ipp64fc value)
{
   Set(index,value.re,value.im);
}

static void set16f(Ipp16s* ptr, Ipp64f value)
{
   Ipp32f val = (Ipp32f)value;
   ippsConvert_32f16f(&val,ptr,1,ippRndNear);
}

static void set24u(Ipp8u* ptr, Ipp64f value)
{
   Ipp32f val = (Ipp32f)value;
   ippsConvert_32f24u_Sfs(&val, ptr, 1, 0);
}

static void set24s(Ipp8u* ptr, Ipp64f value)
{
   Ipp32f val = (Ipp32f)value;
   ippsConvert_32f24s_Sfs(&val, ptr, 1, 0);
}

static Ipp64f get16f(const Ipp16s* ptr)
{
   Ipp32f val;
   ippsConvert_16f32f(ptr,&val,1);
   return val;
}

static Ipp64f get24u(const Ipp8u* ptr)
{
   Ipp32f val;
   ippsConvert_24u32f(ptr,&val,1);
   return val;
}

static Ipp64f get24s(const Ipp8u* ptr)
{
   Ipp32f val;
   ippsConvert_24s32f(ptr,&val,1);
   return val;
}

void CVector::Set(int index, double value, double imval)
{
   if (index < 0) return;
   if (m_len) {
      if (index == m_len) index = m_len - 1;
      if (index >= m_len) return;
   }
   switch (m_type) {
   case pp8u  : ((Ipp8u *)m_pointer)[index] = (Ipp8u )value; break;
   case pp8s  : ((Ipp8s *)m_pointer)[index] = (Ipp8s )value; break;
   case pp16u : ((Ipp16u*)m_pointer)[index] = (Ipp16u)value; break;
   case pp16s : ((Ipp16s*)m_pointer)[index] = (Ipp16s)value; break;
   case pp16f : set16f((Ipp16s*)m_pointer + index,  value); break;
   case pp24u : set24u((Ipp8u*)m_pointer + index*3, value); break;
   case pp24s : set24s((Ipp8u*)m_pointer + index*3, value); break;
   case pp32u : ((Ipp32u*)m_pointer)[index] = (Ipp32u)value; break;
   case pp32s : ((Ipp32s*)m_pointer)[index] = (Ipp32s)value; break;
   case pp32f : ((Ipp32f*)m_pointer)[index] = (Ipp32f)value; break;
   case pp64s : ((Ipp64s*)m_pointer)[index] = (Ipp64s)value; break;
   case pp64f : ((Ipp64f*)m_pointer)[index] = (Ipp64f)value; break;
   case pp8sc : ((Ipp8sc*)m_pointer)[index].re = (Ipp8s )value;
                 ((Ipp8sc*)m_pointer)[index].im = (Ipp8s )imval; break;
   case pp16sc: ((Ipp16sc*)m_pointer)[index].re = (Ipp16s)value;
                 ((Ipp16sc*)m_pointer)[index].im = (Ipp16s)imval; break;
   case pp32sc: ((Ipp32sc*)m_pointer)[index].re = (Ipp32s)value;
                 ((Ipp32sc*)m_pointer)[index].im = (Ipp32s)imval; break;
   case pp32fc: ((Ipp32fc*)m_pointer)[index].re = (Ipp32f)value;
                 ((Ipp32fc*)m_pointer)[index].im = (Ipp32f)imval; break;
   case pp64sc: ((Ipp64sc*)m_pointer)[index].re = (Ipp64s)value;
                 ((Ipp64sc*)m_pointer)[index].im = (Ipp64s)imval; break;
   case pp64fc: ((Ipp64fc*)m_pointer)[index].re = (Ipp64f)value;
                 ((Ipp64fc*)m_pointer)[index].im = (Ipp64f)imval; break;
   }
}

Ipp64fc CVector::Get(int index) const
{
   Ipp64fc value = {0,0};
   if (index < 0) return value;
   if (m_len) {
      if (index == m_len) index = m_len - 1;
      if (index >= m_len) return value;
   }
   switch (m_type) {
   case pp8u  : value.re = (Ipp64f)((Ipp8u *)m_pointer)[index]; break;
   case pp8s  : value.re = (Ipp64f)((Ipp8s *)m_pointer)[index]; break;
   case pp16u : value.re = (Ipp64f)((Ipp16u*)m_pointer)[index]; break;
   case pp16s : value.re = (Ipp64f)((Ipp16s*)m_pointer)[index]; break;
   case pp16f : value.re = get16f((Ipp16s*)m_pointer + index) ; break;
   case pp24u : value.re = get24u((Ipp8u*)m_pointer + index*3); break;
   case pp24s : value.re = get24s((Ipp8u*)m_pointer + index*3); break;
   case pp32u : value.re = (Ipp64f)((Ipp32u*)m_pointer)[index]; break;
   case pp32s : value.re = (Ipp64f)((Ipp32s*)m_pointer)[index]; break;
   case pp32f : value.re = (Ipp64f)((Ipp32f*)m_pointer)[index]; break;
   case pp64s : value.re = (Ipp64f)((Ipp64s*)m_pointer)[index]; break;
   case pp64f : value.re = (Ipp64f)((Ipp64f*)m_pointer)[index]; break;
   case pp8sc : value.re = (Ipp64f)((Ipp8sc *)m_pointer)[index].re;
                value.im = (Ipp64f)((Ipp8sc *)m_pointer)[index].im; break;
   case pp16sc: value.re = (Ipp64f)((Ipp16sc*)m_pointer)[index].re;
                value.im = (Ipp64f)((Ipp16sc*)m_pointer)[index].im; break;
   case pp32sc: value.re = (Ipp64f)((Ipp32sc*)m_pointer)[index].re;
                value.im = (Ipp64f)((Ipp32sc*)m_pointer)[index].im; break;
   case pp32fc: value.re = (Ipp64f)((Ipp32fc*)m_pointer)[index].re;
                value.im = (Ipp64f)((Ipp32fc*)m_pointer)[index].im; break;
   case pp64sc: value.re = (Ipp64f)((Ipp64sc*)m_pointer)[index].re;
                   value.im = (Ipp64f)((Ipp64sc*)m_pointer)[index].im; break;
   case pp64fc: value.re = (Ipp64f)((Ipp64fc*)m_pointer)[index].re;
                 value.im = (Ipp64f)((Ipp64fc*)m_pointer)[index].im; break;
   }
   return value;
}

int CVector::GetInt(int index) const
{
   Ipp64fc val = Get(index);
   return (int)val.re;
}

double CVector::GetDouble(int index) const
{
   Ipp64fc val = Get(index);
   return val.re;
}

CString CVector::TypeString() const
{
   return CFunc::TypeToString(m_type);
}

void CVector::Get(int index, CValue& val) const
{
   ppType valType = val.Type();
   val.Init(Type());
   Ipp64fc tmp64fc;
   tmp64fc = Get(index);
   val.Set(tmp64fc);
   val.Init(valType);
}

void CVector::Set(int index, CValue val)
{
   val.Init(Type());
   Ipp64fc tmp64fc;
   val.Get(tmp64fc);
   Set(index, tmp64fc);
}

void CVector::Set(int index, CString str)
{
   if (m_type == ppNONE) return;
   CValue val(m_type);
   Ipp64fc num;
   val.Set(str);
   val.Get(num);
   Set(index,num);
}

void CVector::Get(int index, CString& str, LPCTSTR cpxFormat) const
{
   Get(index,str,5,cpxFormat);
}

void CVector::Get(int index, CString& str, int nDigits, LPCTSTR cpxFormat) const
{
   CValue val(m_type);
   Ipp64fc num = Get(index);
   val.Set(num);
   str = val.String(nDigits, cpxFormat);
}

void CVector::Set(CMyString str)
{
   if (m_type == ppNONE) return;
   CStringArray itemArr;
   str.Parse(itemArr,_T(";(){}\r\n"));
   Allocate((int)itemArr.GetSize());
   for (int i=0; i<itemArr.GetSize(); i++) {
      CString item = itemArr[i];
      item.TrimLeft();
      item.TrimRight();
      if (item.IsEmpty()) {
         i--;
         continue;
      }
      Set(i, item);
   }
}

void CVector::Get(CMyString& str, CString separ) const
{
   Get(str,5,separ);
}

void CVector::Get(CMyString& str, int nDigits, CString separ) const
{
   str.Empty();
   for (int i=0; i<Length(); i++) {
      CString item;
      Get(i,item,nDigits);
      if (!str.IsEmpty()) str += separ;
      str += item;
   }
}

void CVector::Set(const CMyString* strArray, int num)
{
   if (m_type == ppNONE) return;
   if (num < 0) num = Length();
   for (int i=0; i<num; i++) {
      Set(i, strArray[i]);
   }
}

void CVector::Get(CMyString* strArray, int num, int nDigits) const
{
   if (m_type == ppNONE) return;
   if (num < 0) num = Length();
   for (int i=0; i<num; i++) {
      Get(i,strArray[i],nDigits);
   }
}

void CVector::CreateCustomData(CString dataName, void* pData, int dataSize)
{
   DeleteCustomData();
   if (!pData || !dataSize) return;
   m_pCustomData = new char[dataSize];
   m_CustomName = dataName;
   m_CustomSize = dataSize;
   memcpy(m_pCustomData, pData, dataSize);
}

void CVector::DeleteCustomData()
{ 
   if (m_pCustomData)
      delete[] m_pCustomData;
   m_pCustomData = NULL;
   m_CustomSize = 0;
   m_CustomName = _T("");
}
void* CVector::GetCustomData(CString dataName)
{
   if (dataName == m_CustomName)
      return m_pCustomData;
   else
      return NULL;
}

///////////////////////////////////////////////////////////////////////////////

BOOL CVector::IsVoi() const 
{ 
   return m_Voi.m_length > 0;
}

void* CVector::GetDataVoi()
{
   CVoi voi = GetVoi();
   return (char*)m_pointer + voi.m_offset*ItemSize();
}

const void* CVector::GetDataVoi() const
{
   CVoi voi = GetVoi();
   return (const char*)m_pointer + voi.m_offset*ItemSize();
}

CVoi CVector::GetVoi() const
{
   if (IsVoi()) return m_Voi;
   CVoi voi(0,m_len);
   return voi;
}

void CVector::GetVoi(int& offset, int& length) const
{
   CVoi voi = GetVoi();
   offset = voi.m_offset;
   length = voi.m_length;
}

int CVector::GetLengthVoi() const
{
   CVoi voi = GetVoi();
   return voi.m_length;
}

void CVector::ResetVoi(int len)
{
   if (!IsVoi()) {
      if (m_Voi.m_offset > len)
         m_Voi.m_offset = len;
      return;
   }
   int offset, length;
   GetVoi(offset,length);
   if (offset + length > len)
      length = len - offset;
   if (offset >= len) {
      offset = len;
      length = 0;
   } else if (length < 0) {
      length = 0;
   }
   m_Voi.m_offset = offset;
   m_Voi.m_length = length;
}

BOOL CVector::SetVoi(CVoi voi)
{
   m_Voi = voi;
   ResetVoi(m_len);
   return IsVoi();
}

BOOL CVector::SetVoi(int offset, int length)
{
   CVoi voi(offset,length);
   return SetVoi(voi);
}

int CVector::GetPos() const
{
   return m_Voi.m_offset + m_Voi.m_length;
}

BOOL CVector::SetPos(int pos)
{
   if (pos > m_len)
      return FALSE;
   if (m_Voi.m_offset + m_Voi.m_length == pos)
      return TRUE;
   m_Voi.m_offset = pos;
   m_Voi.m_length = 0;
   return TRUE;
}

BOOL CVector::CopyVoi(const CVector* srcVector)
{
   if (srcVector->IsVoi())
      return SetVoi(srcVector->GetVoi());
   else
      return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
int  CVector::GetDropHeaderSize() const
{
   return sizeof(ppType) + sizeof(int);
}

void CVector::WriteDropHeader(char* pData) const
{
   memcpy(pData, &m_type, sizeof(ppType)); pData += sizeof(ppType);
   memcpy(pData, &m_len, sizeof(int)); pData += sizeof(int);
}

void CVector::ReadDropHeader(const char* pData)
{
   memcpy(&m_type, pData, sizeof(ppType)); pData += sizeof(ppType);
   memcpy(&m_len , pData, sizeof(int)); pData += sizeof(int);
}

int  CVector::GetDropInfoSize() const
{
   return sizeof(CVoi);
}

void CVector::WriteDropInfo(char* pData) const
{
   memcpy(pData, &m_Voi, sizeof(CVoi)); pData += sizeof(CVoi);
}

void CVector::ReadDropInfo(const char* pData)
{
   memcpy(&m_Voi, pData, sizeof(CVoi)); pData += sizeof(CVoi);
}

int  CVector::GetDropDataSize() const
{
   return ItemSize()*Length();
}

void CVector::WriteDropData(char* pData) const
{
   if (GetDropDataSize())
      memcpy(pData, m_pointer, GetDropDataSize()); 
   pData += GetDropDataSize();
}

void CVector::ReadDropData(const char* pData)
{
   if (GetDropDataSize())
      memcpy(m_pointer, pData, GetDropDataSize()); 
   pData += GetDropDataSize();
}


///////////////////////////////////////////////////////////////////////////////

CVectorUnit::CVectorUnit(ppType type) :   
   m_Type(pp32s),
   m_NONE(ppNONE),
   m_8u  (pp8u  ),
   m_8s  (pp8s  ),
   m_16u (pp16u ),
   m_16s (pp16s ),
   m_32u (pp32u ),
   m_32s (pp32s ),
   m_32f (pp32f ),
   m_64s (pp64s ),
   m_64f (pp64f ),
   m_8sc (pp8sc ),
   m_16sc(pp16sc), 
   m_32sc(pp32sc),
   m_32fc(pp32fc), 
   m_64sc(pp64sc),
   m_64fc(pp64fc) {
}

CVectorUnit::~CVectorUnit()
{

}

CVectorUnit& CVectorUnit::operator =(const CVectorUnit& val)
{
   m_Type = val.m_Type;
   m_NONE = val.m_NONE;
   m_8u   = val.m_8u  ;
   m_8s   = val.m_8s  ;
   m_16u  = val.m_16u ;
   m_16s  = val.m_16s ;
   m_32u  = val.m_32u ;
   m_32s  = val.m_32s ;
   m_32f  = val.m_32f ;
   m_64s  = val.m_64s ;
   m_64f  = val.m_64f ;
   m_8sc  = val.m_8sc ; 
   m_16sc = val.m_16sc; 
   m_32sc = val.m_32sc; 
   m_32fc = val.m_32fc; 
   m_64sc = val.m_64sc; 
   m_64fc = val.m_64fc; 
   return *this;
}

void CVectorUnit::Init(ppType type, int len)
{
   m_Type = type;
   if (len > 0)
      Vector(type).Allocate(len);
}

void CVectorUnit::Init(ppType type, CMyString str)
{
   m_Type = type;
   Set(str);
}

CVector& CVectorUnit::Vector(ppType type)
{
   switch (type) {
   case pp8u  : return m_8u  ;
   case pp8s  : return m_8s  ;
   case pp16u : return m_16u ;
   case pp16s : return m_16s ;
   case pp32u : return m_32u ;
   case pp32s : return m_32s ;
   case pp32f : return m_32f ;
   case pp64s : return m_64s ;
   case pp64f : return m_64f ;
   case pp8sc : return m_8sc ; 
   case pp16sc: return m_16sc; 
   case pp32sc: return m_32sc; 
   case pp32fc: return m_32fc; 
   case pp64sc: return m_64sc; 
   case pp64fc: return m_64fc; 
   default: return m_NONE;
   }
}

const CVector& CVectorUnit::Vector(ppType type) const
{
   switch (type) {
   case pp8u  : return m_8u  ;
   case pp8s  : return m_8s  ;
   case pp16u : return m_16u ;
   case pp16s : return m_16s ;
   case pp32u : return m_32u ;
   case pp32s : return m_32s ;
   case pp32f : return m_32f ;
   case pp64s : return m_64s ;
   case pp64f : return m_64f ;
   case pp8sc : return m_8sc ; 
   case pp16sc: return m_16sc; 
   case pp32sc: return m_32sc; 
   case pp32fc: return m_32fc; 
   case pp64sc: return m_64sc; 
   case pp64fc: return m_64fc; 
   default: return m_NONE;
   }
}

void CVectorUnit::Set(CMyString str)
{
   Vector().Set(str);
}

void CVectorUnit::Get(CMyString& str, CString separ) const
{
   Vector().Get(str,separ);
}

void CVectorUnit::Set(const CMyString* strArray, int num)
{
   Vector().Set(strArray, num);
}

void CVectorUnit::Get(CMyString* strArray, int num) const
{
   Vector().Get(strArray,num);
}

CVectorUnit& CVectorUnit::operator =(CVector& vec)
{
   Vector(vec.Type()) = vec;
   return *this;
}

CVector& CVector::operator =(CVectorUnit& unit)
{
   *this = unit.Vector();
   return *this;
}
