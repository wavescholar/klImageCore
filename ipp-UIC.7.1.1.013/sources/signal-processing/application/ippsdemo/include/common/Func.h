/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Func.h: interface for the CFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNC_H__3517F47B_3053_4120_B591_459351A85BD8__INCLUDED_)
#define AFX_FUNC_H__3517F47B_3053_4120_B591_459351A85BD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyString.h"

//////////////////////////////////////////////////////////////////////
// ppType enumerator is used to identify IPP data types,
// such as Ipp8u, Ipp16sc, Ipp32f, etc.

#define PP_MASK  0x0FF
#define PP_SIGN  0x100
#define PP_FLOAT 0x200
#define PP_CPLX  0x400

typedef enum {
   ppNONE = 0 , 
   pp8u   = 8 , 
   pp8s   = 8  | PP_SIGN,
   pp8sc  = 8  | PP_SIGN | PP_CPLX,
   pp16u  = 16,
   pp16s  = 16 | PP_SIGN,
   pp16f  = 16 | PP_FLOAT,
   pp16sc = 16 | PP_SIGN | PP_CPLX,
   pp24u  = 24,
   pp24s  = 24 | PP_SIGN,
   pp32u  = 32,
   pp32s  = 32 | PP_SIGN,
   pp32sc = 32 | PP_SIGN | PP_CPLX,
   pp32f  = 32 | PP_FLOAT ,
   pp32fc = 32 | PP_FLOAT  | PP_CPLX,
   pp64s  = 64 | PP_SIGN,
   pp64sc = 64 | PP_SIGN | PP_CPLX,
   pp64f  = 64 | PP_FLOAT ,
   pp64fc = 64 | PP_FLOAT  | PP_CPLX,
} ppType;

//////////////////////////////////////////////////////////////////////
// VEC_*** constants are used to identify IPP function source or 
// destination vector(image) parameters

enum {VEC_SRC  = 0x01,
      VEC_SRC2 = 0x02,
      VEC_SRC3 = 0x04,
      VEC_MASK = 0x08,
      VEC_DST  = 0x10,
      VEC_DST2 = 0x20};

class CVector;
class CImage;

//////////////////////////////////////////////////////////////////////
// class CFunc helps to extract maximum information about IPP function 
//////////////////////////////////////////////////////////////////////

class CFunc : public CMyString  
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor
   CFunc();
   virtual ~CFunc();
   CFunc(const CFunc& fnc) {*this = fnc;}
   CFunc(const CMyString& str) {*this = str;}
   CFunc(const CString& str) {*this = str;}
   CFunc(LPCSTR str) {*this = str;}
   CFunc(LPCWSTR str) {*this = str;}
   
/////////////////////////////////////////////////////////////////////////////
// Operators
   // operator =
   CFunc& operator =(const CFunc& fnc) {*((CString*)this) = (CString)fnc; return *this;}
   CFunc& operator =(const CMyString& str) {*((CString*)this) = (CString)str; return *this;}
   CFunc& operator =(const CString& str) {*((CString*)this) = str; return *this;}
   CFunc& operator =(LPCSTR str) {*((CString*)this) = str; return *this;}
   CFunc& operator =(LPCWSTR str) {*((CString*)this) = str; return *this;}

   // operator LPCTSTR 
   operator LPCTSTR() const {return (LPCTSTR)(*(CString*)this);} 

/////////////////////////////////////////////////////////////////////////////
// Attributes
   
   // Prefix returns IPP function prefix (for example, "ipps")
   CMyString Prefix() const;
   // BaseName returns IPP function base name (part after prefix and before
   // types, not including '_' symbol)
   CMyString BaseName() const;
   // TypeName returns IPP function type string, not including '_' symbol
   CMyString TypeName() const;
   // DescrName returns IPP function description, not including '_' symbol
   CMyString DescrName() const;

   // SrcChannelsName returns description of source image channels
   CString SrcChannelsName() const;
   // DstChannelsName returns description of destination image channels
   CString DstChannelsName() const;
   // GetChannelsName returns the same as Src/Dst ChannelsName but simultaneously
   void GetChannelsName(CMyString& s, CMyString& d) const;
   
   // SrcType returns type of first source vector(image)
   ppType SrcType() const;
   // DstType returns type of first destination vector(image)
   ppType DstType() const;
   // Src2Type returns type of second source vector(image)
   ppType Src2Type() const;
   // Dst2Type returns type of second destination vector(image)
   ppType Dst2Type() const;
   // BaseType returns type extracted from base name
   ppType BaseType() const;
   // GetTypes returns the same as Src/Dst/Src2/Dst2 Types but simultaneously
   void GetTypes(ppType& s1, ppType& s2, ppType& d1, ppType& d2) const;
   
   // SrcChannels returns number of source image channels
   int SrcChannels() const;
   // DstChannels returns number of destination image channels
   int DstChannels() const;
   // GetChannels returns the same as Src/Dst Channels but simultaneously
   void GetChannels(int& s, int& d) const;

   // SrcPlane returns weather source image has plane structure
   BOOL SrcPlane() const;
   // DstPlane returns weather destination image has plane structure
   BOOL DstPlane() const;
   // GetPlanes returns the same as Src/Dst Plane but simultaneously
   void GetPlanes(BOOL& s, BOOL& d) const;

   // SrcAlpha returns weather source image has alpha channel
   BOOL SrcAlpha() const;
   // DstAlpha returns weather destination image has alpha channel
   BOOL DstAlpha() const;
   // GetAlpha returns the same as Src/Dst Alpha but simultaneously
   void GetAlpha(BOOL& s, BOOL& d) const;
   
   // Inplace returns weather this function operates in-place
   BOOL Inplace() const;
   // Mask returns weather this function has mask parameter
   BOOL Mask() const;
   // NoDst returns weather this function has no destination images
   BOOL NoDst() const;
   // OnlyDst returns weather this function has no source images
   BOOL OnlyDst() const;
   // Scale returns weather this function has scale factor
   BOOL Scale() const;
   // Roi returns weather this function has "R" specificator in description
   BOOL Roi() const;

   // Valid returns weather specified vector is valid paramter for IPP
   // function in specified position 
   BOOL Valid(CVector* pVec, int vecPos = VEC_SRC) const;

   // VecName returns vector(image) parameter name according to its
   // position
   CMyString VecName(int vecPos, int usedVectors = 0) const;
   // GetVecNames returns vector(image) parameter names according to four
   // positions - src1, src2, dst1, dst2
   void GetVecNames(CMyString& src, CMyString& src2, CMyString& dst, CMyString& dst2,
                    int usedVectors = 0) const;

   // GetPair returns this function name in common case; if this function 
   // has inverse operation then that inverse function is returned
   CFunc GetPair() const;
   // InvBase returns this function base name in common case; if it is possible,
   // base name is formally changed to inverce base name and returns
   CString InvBase() const;

   // FuncWithBase change this function base name to specified base name
   // and returns result 
   CFunc FuncWithBase(CMyString baseName) const;
   // FuncWithBase_InvType change this function base name to specified 
   // base name, exchange source and destination types and returns result 
   CFunc FuncWithBase_InvType(CMyString baseName) const;
   // FuncWithBase_InvChannels change this function base name to specified
   // base name, exchange source and destination channels and returns result 
   CFunc FuncWithBase_InvChannels(CMyString baseName) const;   

   // ParseFunction returns parsed elements of IPP function name
   void ParseFunction(CMyString& base, CMyString& type, CMyString& descr) const;
   void ParseFunction(CMyString& base, CMyString& type1, CMyString& type2,
                          CMyString& type3, CMyString& descr) const;

   // TypeToString converts type enumerator to type name 
   static CString TypeToString(ppType type);
   // StringToType converts type name to type enumerator 
   static ppType StringToType(CString type);
   // TypeToIpp converts type enumerator to IPP standard type enumerator 
   static IppDataType TypeToIpp(ppType type);
protected:

/////////////////////////////////////////////////////////////////////////////
// These functions bodies are not exist in func.cpp file
// Each Demo application should write there own functions to support their
// specifics

   // UpdateTypes updates function types
   void UpdateTypes(ppType& src1, ppType& src2, 
                    ppType& dst1, ppType& dst2) const;
   // UpdateChannels update function channels 
   void UpdateChannels(int& src, int& dst) const;
   // UpdatePlanes updates function plane flags
   void UpdatePlanes(BOOL& s, BOOL& d) const;
   // UpdateVecNames updates function vector(image) parameter names
   void UpdateVecNames(CMyString& src, CMyString& src2, 
                       CMyString& dst, CMyString& dst2, int usedVectors) const;
};

typedef CArray<CFunc,CFunc&> CFuncArray;

//////////////////////////////////////////////////////////////////////
// class CFuncList is the IPP functions list that can be created
// using IPP library header
//////////////////////////////////////////////////////////////////////

class CFuncList : public CList<CFunc,CFunc&>
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor
   CFuncList();
   virtual ~CFuncList();

/////////////////////////////////////////////////////////////////////////////
// Operations
   
   // CreateFuncList creates this list as the list of all functions
   // in IPP library header file; the index of library is specified.
   // In addition, the internal outstanding function list is created:
   // all implemented and all non-processing function are extracted from
   // the full function list.
   void  CreateFuncList(int libIdx);
   // GetOutstandList return outstanding function list
   CFuncList* GetOutstandList() {return m_pOutstandList;}

/////////////////////////////////////////////////////////////////////////////
// Attributes

   // Find searches for function with specified name
   POSITION Find(LPCTSTR funcName, POSITION startAfter = NULL) const; 
protected:
   CMyString  m_baseName;
   POSITION m_position;
   CFuncList* m_pOutstandList;

   int SetOutstandList(int libIdx);
   int SubtractThisList(CStringList& rList);
};

//////////////////////////////////////////////////////////////////////
// class CRecentFuncList supports MRU recent functions list
//////////////////////////////////////////////////////////////////////

class CRecentFuncList : public CStringList
{
public:
   CRecentFuncList() : m_MaxSize(4) {}
   virtual ~CRecentFuncList() {}
   void UpdateMenu(CCmdUI* pCmdUI, CVector* pVec, BOOL bEnable = TRUE);
   void AddFunc(CFunc func);
   CFunc GetFunc(int index);
protected:
   int m_MaxSize;
};

#endif // !defined(AFX_FUNC_H__3517F47B_3053_4120_B591_459351A85BD8__INCLUDED_)
