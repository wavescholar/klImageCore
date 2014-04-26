/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Value.cpp: implementation of the CValue class.
//
// class CValue provides functionality for scalar IPP function
// parameter that has got standard IPP type. In fact CValue contains
// the list of components with different Ipp types. At the moment it
// operates with current component that has required type.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "Value.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValue::CValue(ppType type)
{
   m_Type = type;
   m_8u  = 0;
   m_8s  = 0;
   m_16u = 0;
   m_16s = 0;
   m_32u = 0;
   m_32s = 0;
   m_32f = 0.0f;
   m_64s = 0;
   m_64f = 0.0;
   m_8sc .re = 0; m_8sc .im = 0;
   m_16sc.re = 0; m_16sc.im = 0;
   m_32sc.re = 0; m_32sc.im = 0;
   m_32fc.re = 0.0f; m_32fc.im = 0.0f;
   m_64sc.re = 0; m_64sc.im = 0;
   m_64fc.re = 0.0; m_64fc.im = 0.0;
}

CValue::~CValue()
{

}

CValue& CValue::operator =(const CValue& val)
{
   m_Type = val.m_Type;
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

void CValue::Init(CFunc func, EArg argType)
{
   m_Type = argType == argSRC ? func.SrcType() :
                                func.DstType();
}

void CValue::Init(ppType type)
{
   m_Type = type;
}

void CValue::Init(ppType type, double re, double im)
{
   m_Type = type;
   Set(re,im);
}

void CValue::Init64s(ppType type, Ipp64s re, Ipp64s im)
{
   m_Type = type;
   Set64s(re,im);
}

void CValue::Get(double& re, double& im)
{
   switch (m_Type) {
   case pp8u  : re = (double)m_8u ; im = 0; break;
   case pp8s  : re = (double)m_8s ; im = 0; break;
   case pp16u : re = (double)m_16u; im = 0; break;
   case pp16s : re = (double)m_16s; im = 0; break;
   case pp32u : re = (double)m_32u; im = 0; break;
   case pp32s : re = (double)m_32s; im = 0; break;
   case pp32f : re = (double)m_32f; im = 0; break;
   case pp64f : re = (double)m_64f; im = 0; break;
   case pp8sc : re = (double)m_8sc .re; im = (double)m_8sc .im; break;
   case pp16sc: re = (double)m_16sc.re; im = (double)m_16sc.im; break;
   case pp32sc: re = (double)m_32sc.re; im = (double)m_32sc.im; break;
   case pp32fc: re = (double)m_32fc.re; im = (double)m_32fc.im; break;
   case pp64fc: re = (double)m_64fc.re; im = (double)m_64fc.im; break;

   case pp64s : re = (double)(m_64s); im = 0; break;
   case pp64sc: re = (double)(m_64sc.re);
                im = (double)(m_64sc.im); break;
   }
}

void CValue::Get(Ipp64s& re, Ipp64s& im)
{
   switch (m_Type) {
   case pp8u  : re = (Ipp64s)m_8u ; im = 0; break;
   case pp8s  : re = (Ipp64s)m_8s ; im = 0; break;
   case pp16u : re = (Ipp64s)m_16u; im = 0; break;
   case pp16s : re = (Ipp64s)m_16s; im = 0; break;
   case pp32u : re = (Ipp64s)m_32u; im = 0; break;
   case pp32s : re = (Ipp64s)m_32s; im = 0; break;
   case pp32f : re = (Ipp64s)m_32f; im = 0; break;
   case pp64s : re = (Ipp64s)m_64s; im = 0; break;
   case pp64f : re = (Ipp64s)m_64f; im = 0; break;
   case pp8sc : re = (Ipp64s)m_8sc .re ; im = (Ipp64s)m_8sc .im; break;
   case pp16sc: re = (Ipp64s)m_16sc.re ; im = (Ipp64s)m_16sc.im; break;
   case pp32sc: re = (Ipp64s)m_32sc.re ; im = (Ipp64s)m_32sc.im; break;
   case pp32fc: re = (Ipp64s)m_32fc.re ; im = (Ipp64s)m_32fc.im; break;
   case pp64sc: re = (Ipp64s)m_64sc.re ; im = (Ipp64s)m_64sc.im; break;
   case pp64fc: re = (Ipp64s)m_64fc.re ; im = (Ipp64s)m_64fc.im; break;
   }
}

CString CValue::TypeString()
{
   if ((int)m_Type & PP_CPLX)
      return _T("complex ") + TypeStringRe();
   else
      return TypeStringRe();
}

CString CValue::TypeStringRe()
{
   switch (m_Type) {
   case pp8u  :                return _T("unsigned character");
   case pp8s  : case pp8sc  : return _T("character");
   case pp16u :               return _T("unsigned short");
   case pp16s : case pp16sc : return _T("short");
   case pp32u :               return _T("unsigned integer");
   case pp32s : case pp32sc : return _T("integer");
   case pp32f : case pp32fc : return _T("float");
   case pp64s : case pp64sc : return _T("64bit integer");
   case pp64f : case pp64fc : return _T("double");
   }
   return _T("???");
}

void CValue::Set(double re, double im)
{
   switch (m_Type) {
   case pp8u  : m_8u  = (Ipp8u )re; break;
   case pp8s  : m_8s  = (Ipp8s )re; break;
   case pp16u : m_16u = (Ipp16u)re; break;
   case pp16s : m_16s = (Ipp16s)re; break;
   case pp32u : m_32u = (Ipp32u)re; break;
   case pp32s : m_32s = (Ipp32s)re; break;
   case pp32f : m_32f = (Ipp32f)re; break;
   case pp64s : m_64s = (Ipp64s)re; break;
   case pp64f : m_64f = (Ipp64f)re; break;
   case pp8sc : m_8sc .re = (Ipp8s )re; m_8sc .im = (Ipp8s )im; break;
   case pp16sc: m_16sc.re = (Ipp16s)re; m_16sc.im = (Ipp16s)im; break;
   case pp32sc: m_32sc.re = (Ipp32s)re; m_32sc.im = (Ipp32s)im; break;
   case pp32fc: m_32fc.re = (Ipp32f)re; m_32fc.im = (Ipp32f)im; break;
   case pp64sc: m_64sc.re = (Ipp64s)re; m_64sc.im = (Ipp64s)im; break;
   case pp64fc: m_64fc.re = (Ipp64f)re; m_64fc.im = (Ipp64f)im; break;
   }
}

void CValue::Set64s(Ipp64s re, Ipp64s im)
{
   switch (m_Type) {
   case pp8u  : m_8u  = (Ipp8u )re; break;
   case pp8s  : m_8s  = (Ipp8s )re; break;
   case pp16u : m_16u = (Ipp16u)re; break;
   case pp16s : m_16s = (Ipp16s)re; break;
   case pp32u : m_32u = (Ipp32u)re; break;
   case pp32s : m_32s = (Ipp32s)re; break;
   case pp32f : m_32f = (Ipp32f)re; break;
   case pp64s : m_64s = (Ipp64s)re; break;
   case pp64f : m_64f = (Ipp64f)re; break;
   case pp8sc : m_8sc .re = (Ipp8s )re; m_8sc .im = (Ipp8s )im; break;
   case pp16sc: m_16sc.re = (Ipp16s)re; m_16sc.im = (Ipp16s)im; break;
   case pp32sc: m_32sc.re = (Ipp32s)re; m_32sc.im = (Ipp32s)im; break;
   case pp32fc: m_32fc.re = (Ipp32f)re; m_32fc.im = (Ipp32f)im; break;
   case pp64sc: m_64sc.re = (Ipp64s)re; m_64sc.im = (Ipp64s)im; break;
   case pp64fc: m_64fc.re = (Ipp64f)re; m_64fc.im = (Ipp64f)im; break;
   }
}

void CValue::Set(CMyString str)
{
   CString re, im;
   str.Parse2(re,im,_T("{}, \t"));
   SetRe(re);
   SetIm(im);
}

void CValue::Set(CString re, CString im)
{
   SetRe(re);
   SetIm(im);
}

static Ipp64f _ttof(CString str)
{
#ifdef UNICODE
   unsigned char* s = new unsigned char[str.GetLength() + 1];
   for (int i=0; str[i]; i++)
      s[i] = (unsigned char)str[i];
   s[i] = 0;
   Ipp64f r = atof((char*)s);
   delete[] s;
   return r;
#else
   return atof(str);
#endif
}

void CValue::SetRe(CString re)
{
   switch (m_Type) {
   case pp8u  : m_8u      = (Ipp8u )_ttoi(re); break;
   case pp8s  : m_8s      = (Ipp8s )_ttoi(re); break;
   case pp16u : m_16u     = (Ipp16u)_ttoi(re); break;
   case pp16s : m_16s     = (Ipp16s)_ttoi(re); break;
   case pp32u : m_32u     = (Ipp32u)_ttoi(re); break;
   case pp32s : m_32s     = (Ipp32s)_ttoi(re); break;
   case pp8sc : m_8sc .re = (Ipp8s )_ttoi(re); break;
   case pp16sc: m_16sc.re = (Ipp16s)_ttoi(re); break;
   case pp32sc: m_32sc.re = (Ipp32s)_ttoi(re); break;
   case pp64s : m_64s     = (Ipp64s)_ttoi64(re); break;
   case pp64sc: m_64sc.re = (Ipp64s)_ttoi64(re); break;
   case pp32f : m_32f     = (Ipp32f)_ttof(re); break;
   case pp64f : m_64f     = (Ipp64f)_ttof(re); break;
   case pp32fc: m_32fc.re = (Ipp32f)_ttof(re); break;
   case pp64fc: m_64fc.re = (Ipp64f)_ttof(re); break;
   }
}

void CValue::SetIm(CString im)
{
   im.TrimLeft();
   im.TrimRight();
   if (im.IsEmpty()) im = _T("0");
   switch (m_Type) {
   case pp16sc: m_16sc.im = (Ipp16s)_ttoi(im); break;
   case pp32sc: m_32sc.im = (Ipp32s)_ttoi(im); break;
   case pp64sc: m_64sc.im = (Ipp64s)_ttoi64(im); break;
   case pp32fc: m_32fc.im = (Ipp32f)_ttof(im); break;
   case pp64fc: m_64fc.im = (Ipp64f)_ttof(im); break;
   }
}

void CValue::Get(CString& str, int nDigits)
{
   str = String(nDigits);
}

void CValue::Get(CString& re, CString& im, int nDigits)
{
   re = StringRe(nDigits);
   im = StringIm(nDigits);
}

CString CValue::String(LPCTSTR cpxFormat)
{
   return String(5, cpxFormat);
}

CString CValue::String(int nDigits, LPCTSTR cpxFormat)
{
   if ((int)m_Type & PP_CPLX) {
      if (cpxFormat == NULL) cpxFormat = _T("{%s,%s}");
      CString str;
      str.Format(cpxFormat, StringRe(nDigits), StringIm(nDigits));
      return str;
   } else {
      return StringRe(nDigits);
   }
}

CString CValue::StringRe(int nDigits)
{
   CString str;
   CMyString fmtg;
   fmtg << _T("%.") << nDigits << _T("g");
   switch (m_Type) {
   case pp8u  : str.Format(_T("%d"), m_8u ); break;
   case pp8s  : str.Format(_T("%d"), m_8s ); break;
   case pp16u : str.Format(_T("%d"), m_16u); break;
   case pp16s : str.Format(_T("%d"), m_16s); break;
   case pp32u : str.Format(_T("%d"), m_32u); break;
   case pp32s : str.Format(_T("%d"), m_32s); break;
   case pp32f : str.Format(fmtg, m_32f); break;
   case pp64f : str.Format(fmtg, m_64f); break;
   case pp8sc : str.Format(_T("%d"), m_8sc .re); break;
   case pp16sc: str.Format(_T("%d"), m_16sc.re); break;
   case pp32sc: str.Format(_T("%d"), m_32sc.re); break;
   case pp32fc: str.Format(fmtg, m_32fc.re); break;
   case pp64fc: str.Format(fmtg, m_64fc.re); break;
   case pp64s : str.Format(_T("%I64d"), m_64s); break;
   case pp64sc: str.Format(_T("%I64d"), m_64sc.re); break;
   }
   return str;
}

CString CValue::StringIm(int nDigits)
{
   CString str;
   CMyString fmtg;
   fmtg << _T("%.") << nDigits << _T("g");
   switch (m_Type) {
   case pp8sc : str.Format(_T("%d"), m_8sc .im); break;
   case pp16sc: str.Format(_T("%d"), m_16sc.im); break;
   case pp32sc: str.Format(_T("%d"), m_32sc.im); break;
   case pp32fc: str.Format(fmtg, m_32fc.im); break;
   case pp64fc: str.Format(fmtg, m_64fc.im); break;
   case pp64sc: str.Format(_T("%I64d"), m_64sc.im); break;
   }
   return str;
}


int CValue::IntMin(ppType type)
{
   switch (type) {
   case pp8u : return 0;
   case pp8s : return IPP_MIN_8S;
   case pp8sc: return IPP_MIN_8S;
   case pp16u: return 0;
   case pp16sc: return IPP_MIN_16S;
   case pp16s: return IPP_MIN_16S;
   case pp32u: return 0;
   case pp32s: return IPP_MIN_32S;
   case pp32sc: return IPP_MIN_32S;
   case pp64s: return IPP_MIN_32S;
   case pp64sc: return IPP_MIN_32S;
   default: return 0;
   }
}

int CValue::IntMax(ppType type)
{
   switch (type) {
   case pp8u : return IPP_MAX_8U;
   case pp8s : return IPP_MAX_8S;
   case pp8sc : return IPP_MAX_8S;
   case pp16u: return IPP_MAX_16U;
   case pp16s: return IPP_MAX_16S;
   case pp16sc: return IPP_MAX_16S;
   case pp32u: return IPP_MAX_32U;
   case pp32s: return IPP_MAX_32S;
   case pp32sc: return IPP_MAX_32S;
   case pp64s: return IPP_MAX_32S;
   case pp64sc: return IPP_MAX_32S;
   default: return 0;
   }
}
