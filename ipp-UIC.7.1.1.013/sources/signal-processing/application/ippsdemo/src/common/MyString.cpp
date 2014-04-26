/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MyString.cpp: implementation of the CMyString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "MyString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


int CMyString::FindNameIndex() const
{
    int ind1 = ReverseFind('/');
    int ind2 = ReverseFind('\\');
    int index;
    if (ind1>=0 && ind2>=0) index = ind1 > ind2 ? ind1 : ind2;
    else if (ind1>=0)       index = ind1;
    else if (ind2>=0)       index = ind2;
    else return 0;
    return index + 1;
}

CString CMyString::GetDrive() const
{
    if (IsEmpty()) return _T("");
    int index = (':');
    if (index < 0) return _T("");
    return Left(index+1);
}

CString CMyString::GetDir() const
{
    if (IsEmpty()) return _T("");
    return Left(FindNameIndex());
}

CString CMyString::GetName() const
{
    if (IsEmpty()) return _T("");
    return Mid(FindNameIndex());
}

CString CMyString::GetExt() const
{
    if (IsEmpty()) return _T("");
    int index = ReverseFind('.');
    if (index < 0) return _T("");
    else           return Mid(index);
}

CString CMyString::GetTitle() const
{
    if (IsEmpty()) return _T("");
    CString name = GetName();
    int index = name.ReverseFind('.');
    if (index < 0) return name;
    else           return name.Left(index);
}

CString CMyString::GetExceptExt() const
{
    if (IsEmpty()) return _T("");
    int index = ReverseFind('.');
    if (index < 0) return *this;
    else           return Left(index);
}

//////////////////////////////////////////////////////////////////////////////
//   CMyString Parse
//////////////////////////////////////////////////////////////////////////////

class CParseString : CMyString {
public:
   CParseString(const CMyString& str) : 
      CMyString(str), m_CurIndex(-1), m_Quote(-1) {}
   virtual ~CParseString() {}
   int GetParseNum(CString separ);
   int Parse(CStringArray& array, CString separ);
   int Parse2(CString& item1, CString& item2, CString separ);
   int ParseRev2(CString& item1, CString& item2, CString separ);

protected:
   void Init(CString separ);
   void InitRev(CString separ);
   BOOL IsSeparator(int index);
   void RemoveQuote(CString& word);
   BOOL GetNextWord(CString& word);
   BOOL GetPrevWord(CString& word);
   void GetTail(CString& word);
   void GetHead(CString& word);

   CString m_Separ;
   int     m_CurIndex;
   int     m_Quote;
};

int CParseString::GetParseNum(CString separ)
{
    Init(separ);
    CString word;
    int num = 0;
    while (GetNextWord(word)) num++;
    return num;
}

int CParseString::Parse(CStringArray& array, CString separ)
{
   Init(separ);
   CString word;
   array.RemoveAll();
   while (GetNextWord(word)) array.Add(word);
   return (int)array.GetSize();
}

int CParseString::Parse2(CString& item1, CString& item2, CString separ)
{
    Init(separ);
    GetNextWord(item1);
    GetTail(item2);
    return (!item1.IsEmpty()) + (!item2.IsEmpty());
}

int CParseString::ParseRev2(CString& item1, CString& item2, CString separ)
{
    InitRev(separ);
    GetPrevWord(item2);
    GetHead(item1);
    return (!item1.IsEmpty()) + (!item2.IsEmpty());
}

void CParseString::Init(CString separ)
{
    m_CurIndex  = 0;
    m_Separ = separ;
    if (separ.Find('"') >= 0)
        m_Quote = -100;
    else
        m_Quote = -1;
}

void CParseString::InitRev(CString separ)
{
    Init(separ);
    m_CurIndex  = GetLength() - 1;
}

BOOL CParseString::IsSeparator(int index)
{
    _TCHAR symb = GetAt(index);
    if (m_Quote >= 0) {
        if (symb == '"') {
            if (m_Quote == index - 1)
                m_Quote = 0;
            else
                m_Quote = -1;        
        }
        return FALSE;
    } else if (m_Quote == -1 && symb == '"') {
        m_Quote = index;
        return FALSE;
    } else {
        return m_Separ.Find(symb) >= 0;
    }
}

void CParseString::RemoveQuote(CString& word)
{
    if (m_Quote < -1) return;
    int index;
    while (index = word.Find(_T("\"\"")) >= 0)
        word = word.Left(index) + word.Mid(index+1);
}

BOOL CParseString::GetNextWord(CString& word)
{
    int i, j;
    word.Empty();
    for (i = m_CurIndex; i < GetLength() && IsSeparator(i); i++);
    if (i >= GetLength()) return FALSE;
    for (j = i; j < GetLength() && !IsSeparator(j); j++) ;
    word = Mid(i, j-i);
    RemoveQuote(word);
    m_CurIndex = j;
    return TRUE;
}

BOOL CParseString::GetPrevWord(CString& word)
{
    word.Empty();
    int i, j;
    for (i = m_CurIndex; i >= 0 && IsSeparator(i); i--);
    if (i < 0) return FALSE;
    for (j = i; j >= 0 && !IsSeparator(j); j--) ;
    word = Mid(j, i-j);
    RemoveQuote(word);
    m_CurIndex = j;
    return TRUE;
}

void CParseString::GetTail(CString& word)
{
    word.Empty();

    int i;
    for (i = m_CurIndex; i < GetLength() && IsSeparator(i); i++);
    if (i < GetLength()) word = Mid(i);
}

void CParseString::GetHead(CString& word)
{
    word.Empty();
    int i;
    for (i = m_CurIndex; i >= 0 && IsSeparator(i); i--);
    if (i >= 0) word = Left(i + 1);
}

int CMyString::GetParseNum(CString separ) const
{
    CParseString prs(*this);
    return prs.GetParseNum(separ);
}

int CMyString::Parse(CStringArray& array, CString separ) const
{
   CParseString prs(*this);
   return prs.Parse(array,separ);
}

int CMyString::Parse2(CString& item1, CString& item2, CString separ) const
{
   CParseString prs(*this);
   return prs.Parse2(item1,item2,separ);
}

int CMyString::ParseRev2(CString& item1, CString& item2, CString separ) const
{
   CParseString prs(*this);
   return prs.ParseRev2(item1,item2,separ);
}

//////////////////////////////////////////////////////////////////////////////
//   CMyString conversion from  Flag & Enum
//////////////////////////////////////////////////////////////////////////////

static bool fitString(CMyString str, CMyString sample, int shift)
{
   str.MakeLower();
   sample.MakeLower();
   CMyString shiftSample = sample.Mid(shift);
   if (shiftSample.Find(str) == 0) return true;
   if (sample.Find(str) == 0) return true;
   return false;
}

CMyString CMyString::FlagString(int val, int* valList, CMyString* strList, int num)
{
   CMyString dst;
   for (int i=0; i<num; i++) {
      if (val & valList[i]) {
         if (!dst.IsEmpty()) dst += _T("|");
         dst += strList[i];
      }
   }
   if (dst.IsEmpty()) dst << (int)val; 
   return dst;
}

CMyString CMyString::EnumString(int val, int* valList, CMyString* strList, int num)
{
   for (int i=0; i<num; i++) {
      if (val == valList[i]) return strList[i];
   }
   CMyString dst; 
   dst << (int)val; 
   return dst;
}

//////////////////////////////////////////////////////////////////////////////
//   CMyString << operators
//////////////////////////////////////////////////////////////////////////////

static Ipp64sc sc8to64 (Ipp8sc  src) 
{Ipp64sc dst; dst.re = (Ipp64s)src.re; dst.im = (Ipp64s)src.im; return dst;}
static Ipp64sc sc16to64(Ipp16sc src) 
{Ipp64sc dst; dst.re = (Ipp64s)src.re; dst.im = (Ipp64s)src.im; return dst;}
static Ipp64sc sc32to64(Ipp32sc src) 
{Ipp64sc dst; dst.re = (Ipp64s)src.re; dst.im = (Ipp64s)src.im; return dst;}
static Ipp64sc fc32to64(Ipp32fc src) 
{Ipp64sc dst; dst.re = (Ipp64s)src.re; dst.im = (Ipp64s)src.im; return dst;}
static Ipp8sc  sc64to8 (Ipp64sc src) 
{Ipp8sc  dst; dst.re = (Ipp8s )src.re; dst.im = (Ipp8s )src.im; return dst;}
static Ipp16sc sc64to16(Ipp64sc src) 
{Ipp16sc dst; dst.re = (Ipp16s)src.re; dst.im = (Ipp16s)src.im; return dst;}
static Ipp32sc sc64to32(Ipp64sc src) 
{Ipp32sc dst; dst.re = (Ipp32s)src.re; dst.im = (Ipp32s)src.im; return dst;}
static Ipp32fc fc64to32(Ipp64fc src) 
{Ipp32fc dst; dst.re = (Ipp32f)src.re; dst.im = (Ipp32f)src.im; return dst;}

CMyString& operator << (CMyString& str, CMyString val) 
{ str += val; return str;}

CMyString& operator << (CMyString& str, const _TCHAR* val) 
{ str += val; return str;}

CMyString& operator << (CMyString& str, _TCHAR val) 
{ str += val; return str;}

CMyString& operator << (CMyString& str, CString val) 
{ str += val; return str;}

CMyString& operator << (CMyString& str, bool val) {
   str += (val ? _T("true") : _T("false"));
   return str;
}

CMyString& operator << (CMyString& str, double val) {
   _TCHAR buf[32];
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   _stprintf_s(buf,32,_T("%.3g"),val);
#else
   _stprintf(buf,_T("%.3g"),val);
#endif
   str += buf;
   return str;
}

CMyString& operator << (CMyString& str, float val) { return str << (double)val;}

CMyString& operator << (CMyString& str, Ipp64s val) {
   _TCHAR buf[32];
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   _stprintf_s( buf, 32, _T("%I64d"), val );
#else
   _stprintf( buf, _T("%I64d"), val );
#endif
   str += buf;
   return str;
}

CMyString& operator << (CMyString& str, Ipp8u val) { return str << (Ipp64s)val;}
CMyString& operator << (CMyString& str, Ipp8s val) { return str << (Ipp64s)val;}
#ifndef UNICODE
CMyString& operator << (CMyString& str, Ipp16u val) { return str << (Ipp64s)val;}
#endif
CMyString& operator << (CMyString& str, Ipp16s val) { return str << (Ipp64s)val;}
CMyString& operator << (CMyString& str, Ipp32u val) { return str << (Ipp64s)val;}
CMyString& operator << (CMyString& str, Ipp32s val) { return str << (Ipp64s)val;}

CMyString& operator << (CMyString& str, Ipp64sc val) {
   _TCHAR buf[64];
   CMyString sRe, sIm;
   sRe << val.re;
   sIm << val.im;
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   _stprintf_s(buf,64,_T("{%s,%s}"),(LPCTSTR)sRe,(LPCTSTR)sIm);
#else
   _stprintf(buf,_T("{%s,%s}"),(LPCTSTR)sRe,(LPCTSTR)sIm);
#endif
   str += buf;
   return str;
}

CMyString& operator << (CMyString& str, Ipp8sc val) { return str << sc8to64(val);}
CMyString& operator << (CMyString& str, Ipp16sc val) { return str << sc16to64(val);}
CMyString& operator << (CMyString& str, Ipp32sc val) { return str << sc32to64(val);}

CMyString& operator << (CMyString& str, Ipp64fc val) {
   _TCHAR buf[64];
   CMyString sRe, sIm;
   sRe << val.re;
   sIm << val.im;
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   _stprintf_s(buf,64,_T("{%s,%s}"),(LPCTSTR)sRe,(LPCTSTR)sIm);
#else
   _stprintf(buf,_T("{%s,%s}"),(LPCTSTR)sRe,(LPCTSTR)sIm);
#endif
   str += buf;
   return str;
}

CMyString& operator << (CMyString& str, Ipp32fc val) { return str << fc32to64(val);}

static int RndNum = 2;
static CMyString RndStr[] = {
   _T("zero"), _T("near")};
static IppRoundMode RndVal[] = {
   ippRndZero, ippRndNear};
CMyString& operator << (CMyString& str, IppRoundMode val) {
   str += ENUM_STRING(Rnd,val);
   return str;
}

static int HintNum = 3;
static CMyString HintStr[] = {
   _T("none"), _T("fast"), _T("accurate")};
static IppHintAlgorithm HintVal[] = {
   ippAlgHintNone, ippAlgHintFast, ippAlgHintAccurate};
CMyString& operator << (CMyString& str, IppHintAlgorithm val) {
   str += ENUM_STRING(Hint,val);
   return str;
}

static int FlagNum = 4;
static CMyString FlagStr[] = {
   _T("divFwdByN"), _T("divInvByN"), 
   _T("divBySqrtN"), _T("nodivByAny"),};
static EIppFlag FlagVal[] = {
   (EIppFlag)IPP_FFT_DIV_FWD_BY_N, (EIppFlag)IPP_FFT_DIV_INV_BY_N, 
   (EIppFlag)IPP_FFT_DIV_BY_SQRTN, (EIppFlag)IPP_FFT_NODIV_BY_ANY,};
CMyString& operator << (CMyString& str, EIppFlag val) {
   str += FLAG_STRING(Flag,val);
   return str;
}

static int CmpOpNum = 5;
static CMyString CmpOpStr[] = {
   _T("Less"), _T("LessEq"), _T("Eq"), _T("GreaterEq"), _T("Greater")};
static IppCmpOp CmpOpVal[] = {
   ippCmpLess, ippCmpLessEq, ippCmpEq, ippCmpGreaterEq, ippCmpGreater};

CMyString& operator << (CMyString& str, IppCmpOp val) {
   str += ENUM_STRING(CmpOp,val);
   return str;
}
