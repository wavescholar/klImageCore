/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MyString.h: interface for the CMyString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSTRING_H__28D59AEF_D43B_4917_8C9A_53D7309B97B5__INCLUDED_)
#define AFX_MYSTRING_H__28D59AEF_D43B_4917_8C9A_53D7309B97B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ippcore.h"

class CMyString : public CString  
{
public:
   CMyString(){}
   virtual ~CMyString(){}
   CMyString(const CMyString& str) { *this = str;}
   CMyString(const CString& str) { *this = str;}
   CMyString(TCHAR ch) { *this = ch;}
   CMyString(LPCSTR lpsz) { *this = lpsz;}
   CMyString(LPCWSTR lpsz) { *this = lpsz;}
   CMyString(const unsigned char* psz) { *this = psz;}

   CMyString& operator =(const CMyString& str) {*((CString*)this) = (CString)str; return *this;}
   CMyString& operator =(const CString& str) {*((CString*)this) = str; return *this;}
   CMyString& operator =(TCHAR ch) {*((CString*)this) = ch; return *this;}
   CMyString& operator =(LPCSTR lpsz) {*((CString*)this) = lpsz; return *this;}
   CMyString& operator =(LPCWSTR lpsz) {*((CString*)this) = lpsz; return *this;}
   CMyString& operator =(const unsigned char* psz) {*((CString*)this) = psz; return *this;}

   operator LPCTSTR() const {return (LPCTSTR)(*(CString*)this);}           // as a C string

    CString GetDrive() const;
    CString GetDir() const;
    CString GetName() const;
    CString GetExt() const;
    CString GetTitle() const;
    CString GetExceptExt() const;
    int  GetParseNum(CString separ) const;
    int  Parse(CStringArray& array, CString separ) const;
    int  Parse2(CString& item1, CString& item2, CString separ) const;
    int  ParseRev2(CString& item1, CString& item2, CString separ) const;
    BOOL Found(CString str)  const { return Find(str) != -1;}
    BOOL Found(_TCHAR c)  const { return Find(c) != -1;}

   static CMyString FlagString(int val, int* valList, 
                               CMyString* strList, int num);
   static CMyString EnumString(int val, int* valList, 
                               CMyString* strList, int num);
protected:
    int FindNameIndex() const;
};

typedef class CList<CMyString, CMyString&> CMyStringList;
typedef class CArray<CMyString, CMyString&> CMyStringArray;

CMyString& operator << (CMyString& str, CMyString val) ;
CMyString& operator << (CMyString& str, CString val) ;
CMyString& operator << (CMyString& str, const _TCHAR* val) ;
CMyString& operator << (CMyString& str, _TCHAR val) ;

CMyString& operator << (CMyString& str, bool val);
CMyString& operator << (CMyString& str, double val);
CMyString& operator << (CMyString& str, float val);
CMyString& operator << (CMyString& str, Ipp64s val);
CMyString& operator << (CMyString& str, Ipp8u val);
CMyString& operator << (CMyString& str, Ipp8s val) ;
#ifndef UNICODE
CMyString& operator << (CMyString& str, Ipp16u val) ;
#endif
CMyString& operator << (CMyString& str, Ipp16s val) ;
CMyString& operator << (CMyString& str, Ipp32u val) ;
CMyString& operator << (CMyString& str, Ipp32s val);
CMyString& operator << (CMyString& str, Ipp64sc val);
CMyString& operator << (CMyString& str, Ipp8sc val) ;
CMyString& operator << (CMyString& str, Ipp16sc val);
CMyString& operator << (CMyString& str, Ipp32sc val);
CMyString& operator << (CMyString& str, Ipp64fc val);
CMyString& operator << (CMyString& str, Ipp32fc val) ;

CMyString& operator << (CMyString& str, IppRoundMode);
CMyString& operator << (CMyString& str, IppHintAlgorithm);
CMyString& operator << (CMyString& str, IppCmpOp val);

typedef enum {flgERROR = -1} EIppFlag;
CMyString& operator << (CMyString& str, EIppFlag);


#define FLAG_STRING(NAME, VAL) \
CMyString::FlagString((int)VAL, (int*)NAME##Val, NAME##Str, NAME##Num) 

#define ENUM_STRING(NAME, VAL) \
CMyString::EnumString((int)VAL, (int*)NAME##Val, NAME##Str, NAME##Num) 

template <class T>
inline void Swap(T a, T b)
{
    T t;
    t = a;
    a = b;
    b = t;
}

#endif // !defined(AFX_MYSTRING_H__28D59AEF_D43B_4917_8C9A_53D7309B97B5__INCLUDED_)
