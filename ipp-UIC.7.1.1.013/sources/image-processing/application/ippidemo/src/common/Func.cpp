/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Func.cpp: implementation of the CFunc, CFuncList, CRecentFuncList 
// classes.
//
// class CFunc helps to extract maximum information about IPP function 
//
// class CFuncList is the IPP functions list that can be created
// using IPP library header
//
// class CRecentFuncList supports MRU recent function list
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "Func.h"
#include "Vector.h"
#include "CProcess.h"
#include "Director.h"
#include "MruMenu.h"
#include "ipp_List.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunc::CFunc()
{
}

CFunc::~CFunc()
{

}

CString CFunc::TypeToString(ppType type)
{
   switch (type) {
   case pp8u  : return _T("8u");
   case pp8s  : return _T("8s");
   case pp8sc : return _T("8sc");
   case pp16u : return _T("16u");
   case pp16s : return _T("16s");
   case pp16sc: return _T("16sc");
   case pp16f : return _T("16f");
   case pp24u : return _T("24u");
   case pp24s : return _T("24s");
   case pp32u : return _T("32u");
   case pp32s : return _T("32s");
   case pp32sc: return _T("32sc");
   case pp32f : return _T("32f");
   case pp32fc: return _T("32fc");
   case pp64s : return _T("64s");
   case pp64sc: return _T("64sc");
   case pp64f : return _T("64f");
   case pp64fc: return _T("64fc");
   default: return _T("");
   }
}

ppType CFunc::StringToType(CString type)
{
   if (type == _T("8u")  ) return pp8u  ;
   if (type == _T("8s")  ) return pp8s  ;
   if (type == _T("8sc") ) return pp8sc ;
   if (type == _T("16u") ) return pp16u ;
   if (type == _T("16s") ) return pp16s ;
   if (type == _T("16sc")) return pp16sc;
   if (type == _T("16f") ) return pp16f ;
   if (type == _T("24u") ) return pp24u ;
   if (type == _T("24s") ) return pp24s ;
   if (type == _T("32u") ) return pp32u ;
   if (type == _T("32s") ) return pp32s ;
   if (type == _T("32sc")) return pp32sc;
   if (type == _T("32f") ) return pp32f ;
   if (type == _T("32fc")) return pp32fc;
   if (type == _T("64s") ) return pp64s ;
   if (type == _T("64sc")) return pp64sc;
   if (type == _T("64f") ) return pp64f ;
   if (type == _T("64fc")) return pp64fc;
   return ppNONE;
}

IppDataType CFunc::TypeToIpp(ppType type)
{
   switch (type) {
   case pp8u  : return ipp8u  ;
   case pp8s  : return ipp8s  ;
//   case pp8sc : return ipp8sc ;
   case pp16u : return ipp16u ;
   case pp16s : return ipp16s ;
   case pp16sc: return ipp16sc;
   case pp32u : return ipp32u ;
   case pp32s : return ipp32s ;
   case pp32sc: return ipp32sc;
   case pp32f : return ipp32f ;
   case pp32fc: return ipp32fc;
   case pp64s : return ipp64s ;
   case pp64sc: return ipp64sc;
   case pp64f : return ipp64f ;
   case pp64fc: return ipp64fc;
   default:     return (IppDataType)-1;
   }
}

static CMyString firstType(CMyString type)
{
   if (type.Find(_T("64fc")) == 0) return _T("64fc");
   if (type.Find(_T("64sc")) == 0) return _T("64sc");
   if (type.Find(_T("64s" )) == 0) return _T("64s") ;
   if (type.Find(_T("64f" )) == 0) return _T("64f") ;
   if (type.Find(_T("32fc")) == 0) return _T("32fc");
   if (type.Find(_T("32sc")) == 0) return _T("32sc");
   if (type.Find(_T("32u" )) == 0) return _T("32u") ;
   if (type.Find(_T("32s" )) == 0) return _T("32s") ;
   if (type.Find(_T("32f" )) == 0) return _T("32f") ;
   if (type.Find(_T("24u" )) == 0) return _T("24u") ;
   if (type.Find(_T("24s" )) == 0) return _T("24s") ;
   if (type.Find(_T("16sc")) == 0) return _T("16sc");
   if (type.Find(_T("16u" )) == 0) return _T("16u") ;
   if (type.Find(_T("16s" )) == 0) return _T("16s") ;
   if (type.Find(_T("16f" )) == 0) return _T("16f") ;
   if (type.Find(_T("8sc" )) == 0) return _T("8sc")  ;
   if (type.Find(_T("8u"  )) == 0) return _T("8u")  ;
   if (type.Find(_T("8s"  )) == 0) return _T("8s")  ;
   if (type.Find(_T("1u"  )) == 0) return _T("8u")  ;
   return _T("");
}

ppType CFunc::BaseType() const
{
   CMyString base = BaseName();
   if (base.Found(_T("64fc"))) return pp64fc;
   if (base.Found(_T("64sc"))) return pp64sc;
   if (base.Found(_T("64s" ))) return pp64s;
   if (base.Found(_T("64f" ))) return pp64f;
   if (base.Found(_T("32fc"))) return pp32fc;
   if (base.Found(_T("32sc"))) return pp32sc;
   if (base.Found(_T("32u" ))) return pp32u;
   if (base.Found(_T("32s" ))) return pp32s;
   if (base.Found(_T("32f" ))) return pp32f;
   if (base.Found(_T("24u" ))) return pp24u;
   if (base.Found(_T("24s" ))) return pp24s;
   if (base.Found(_T("16sc"))) return pp16sc;
   if (base.Found(_T("16f" ))) return pp16f;
   if (base.Found(_T("16u" ))) return pp16u;
   if (base.Found(_T("16s" ))) return pp16s;
   if (base.Found(_T("8sc" ))) return pp8sc;
   if (base.Found(_T("8u"  ))) return pp8u;
   if (base.Found(_T("8s"  ))) return pp8s;
   if (base.Found(_T("1u"  ))) return pp8u;
   return ppNONE;
}

static BOOL isType(CMyString typeStr)
{
   CMyString firstStr = firstType(typeStr);
   return !firstStr.IsEmpty();
}

void CFunc::ParseFunction(CMyString& base, CMyString& type, CMyString& descr) const
{
   base = type = descr = _T("");
   int i0 = Find(_T("ipps")) == 0 ? 4 : 
            Find(_T("ippi")) == 0 ? 4 : 
                                0;
   int i1 = Find('_');
   if (!isType(Mid(i1+1)))
      i1 = Find('_', i1 + 1);
   int i2 = Find('_', i1 + 1);
   if (i1 < 0) {
      base = Mid(i0);
      return;
   }
   base = Mid(i0,i1 - i0);
   if (i2 < 0) {
      type = Mid(i1 + 1);
      return;
   }
   type = Mid(i1 + 1, i2 - (i1 + 1));
   descr = Mid(i2 + 1);
}   


void CFunc::ParseFunction(CMyString& base, CMyString& type1, CMyString& type2,
                          CMyString& type3, CMyString& descr) const
{
   CMyString type;
   ParseFunction(base,type,descr);
   int len = type.GetLength();
   type1 = firstType(type);
   int len1 = type1.GetLength();
   if (len1 == len) {
      type2 = type3 = type1;
   } else {
      type2 = firstType(type.Mid(len1));
      int len2 = type2.GetLength();
      if (len1 + len2 == len) {
         type3 = type2;
         if (!descr.Found(_T("I")))
            type2 = type1;
      } else {
         type3 = firstType(type.Mid(len1+len2));
      }
   }
} 
  
CMyString CFunc::Prefix() const
{
   if (GetLength() < 4) return _T("");
   if (GetAt(3) == 'i' || GetAt(3) == 's') return Left(4);
   return Left(3);
}

CMyString CFunc::BaseName() const
{
   CMyString base, type, descr;
   ParseFunction(base,type,descr);
   return base;
}
  
CMyString CFunc::TypeName() const
{
   CMyString base, type, descr;
   ParseFunction(base,type,descr);
   return type;
}
  
CMyString CFunc::DescrName() const
{
   CMyString base, type, descr;
   ParseFunction(base,type,descr);
   return descr;
}

ppType CFunc::SrcType() const
{
   ppType ts1, ts2, td1, td2;
   GetTypes(ts1, ts2, td1, td2);
   return ts1;
}

ppType CFunc::DstType() const
{
   ppType ts1, ts2, td1, td2;
   GetTypes(ts1, ts2, td1, td2);
   return td1;
}

ppType CFunc::Src2Type() const
{
   ppType ts1, ts2, td1, td2;
   GetTypes(ts1, ts2, td1, td2);
   return ts2;
}

ppType CFunc::Dst2Type() const
{
   ppType ts1, ts2, td1, td2;
   GetTypes(ts1, ts2, td1, td2);
   return td2;
}

int CFunc::SrcChannels() const
{
   int src, dst;
   GetChannels(src, dst);
   return src;
}

int CFunc::DstChannels() const
{
   int src, dst;
   GetChannels(src, dst);
   return dst;
}

BOOL CFunc::OnlyDst() const
{
   if (BaseName() == _T("Set") || BaseName() == _T("Zero")) return TRUE;
   if (BaseName().Found(_T("Draw"))) return TRUE;
   if (BaseName().Found(_T("Image")) || BaseName().Found(_T("Vector"))) return TRUE;
   if (BaseName().Found(_T("RandGauss")) || BaseName().Found(_T("RandUniform"))) return TRUE;
   if (BaseName().Found(_T("Tone")) || BaseName().Found(_T("Triangle"))) return TRUE;
   if (BaseName().Found(_T("FIRGen"))) return TRUE;
   return FALSE;
}

BOOL CFunc::Inplace() const
{
   if (OnlyDst()) return TRUE;
   if (BaseName() == _T("Preemphasize")) return TRUE;
   if (BaseName() == _T("AddProduct")) return TRUE;
   if (BaseName() == _T("AddProductC")) return TRUE;
   if (BaseName() == _T("Move")) return TRUE;
   return DescrName().Found(_T("I"));
}

BOOL CFunc::Mask() const
{
   return DescrName().Found(_T("M"));
}

BOOL CFunc::Scale() const
{
   CMyString base, type, descr;
   ParseFunction(base,type,descr);
   return descr.Find(_T("Sfs")) != -1;
}

BOOL CFunc::Roi() const
{
   CMyString base, type, descr;
   ParseFunction(base,type,descr);
   return descr.Find(_T("R")) != -1;
}

BOOL CFunc::SrcPlane() const
{
   BOOL src, dst;
   GetPlanes(src,dst);
   return src;
}

BOOL CFunc::DstPlane() const
{
   BOOL src, dst;
   GetPlanes(src,dst);
   return dst;
}

BOOL CFunc::SrcAlpha() const
{
   BOOL src, dst;
   GetAlpha(src,dst);
   return src;
}

BOOL CFunc::DstAlpha() const
{
   BOOL src, dst;
   GetAlpha(src,dst);
   return dst;
}

CString CFunc::SrcChannelsName() const
{
   CMyString src, dst;
   GetChannelsName(src,dst);
   return src;
}

CString CFunc::DstChannelsName() const
{
   CMyString src, dst;
   GetChannelsName(src,dst);
   return dst;
}

void CFunc::GetTypes(ppType& src1, ppType& src2, 
                     ppType& dst1, ppType& dst2) const
{
   CMyString sBase, sType1, sType2, sType3, sDescr;
   ParseFunction(sBase, sType1, sType2, sType3, sDescr);
   if (sType1.IsEmpty()) 
      throw _T("Bad function type");
   src1 = StringToType(sType1);
   src2 = StringToType(sType2);
   dst1 = StringToType(sType3);
   dst2 = dst1;
   UpdateTypes(src1, src2, dst1, dst2);
}

static CMyString firstChanStr(CMyString& descr)
{
   CMyString numStr = _T("1234");
   int index = descr.Find(_T("C"));
   int iP = descr.Find(_T("P"));
   int len = 0;
   CMyString chanStr = _T("");
   if ((iP >= 0) && (index < 0 || (iP < index))) index = iP;
   if ((index < 0) ||
       (descr.GetLength() == index + 1) ||
       (!numStr.Found(descr[index + 1]))) {
      len = 0;
   } else {
      len = 2;
      if ((index > 0) && (descr[index - 1] == 'A')) {
         index--;
         len++;
      }
   }
   CMyString str = descr.Mid(index,len);
   descr = descr.Mid(index + len);
   return str;
}

static int numChannels(CMyString str)
{
   if (str.IsEmpty()) return 0;
   CMyString numStr((str[0] == 'A') ? str[2] : str[1]);
   int num = _ttoi(numStr);
   if (num > 4) num = 0;
   return num;
}

static BOOL alphaChannel(CMyString str)
{
   if (str.IsEmpty()) return 0;
   return str[0] == 'A';
}

static BOOL planeChannel(CMyString str)
{
   if (str.IsEmpty()) return 0;
   _TCHAR symb = (str[0] == 'A') ? str[1] : str[0];
   return symb == 'P';
}

void CFunc::GetChannelsName(CMyString& src, CMyString& dst) const
{
   src = dst = _T("");
   CMyString sDescr = DescrName();
   src = firstChanStr(sDescr);
   dst = firstChanStr(sDescr);
   if (dst.IsEmpty()) dst = src;
}

void CFunc::GetChannels(int& src, int& dst) const
{
   src = dst = 0;
   CMyString sDescr = DescrName();
   CMyString srcStr = firstChanStr(sDescr);
   CMyString dstStr = firstChanStr(sDescr);
   src = numChannels(srcStr);
   if (dstStr.IsEmpty())
      dst = src;
   else
      dst = numChannels(dstStr);
   UpdateChannels(src, dst);
}

void CFunc::GetPlanes(BOOL& src, BOOL& dst) const
{
   src = dst = FALSE;
   CMyString sDescr = DescrName();
   CMyString srcStr = firstChanStr(sDescr);
   CMyString dstStr = firstChanStr(sDescr);
   src = planeChannel(srcStr);
   if (dstStr.IsEmpty())
      dst = src;
   else
      dst = planeChannel(dstStr);
   if (!src && !dst && sDescr.Found(_T("P")))
      src = dst = TRUE;
   UpdatePlanes(src,dst);
}

void CFunc::GetAlpha(BOOL& src, BOOL& dst) const
{
   src = dst = FALSE;
   CMyString sDescr = DescrName();
   CMyString srcStr = firstChanStr(sDescr);
   CMyString dstStr = firstChanStr(sDescr);
   src = alphaChannel(srcStr);
   if (dstStr.IsEmpty())
      dst = src;
   else
      dst = alphaChannel(dstStr);
}

CString CFunc::InvBase()  const
{
   CString baseName = BaseName();

   int iFwd = baseName.Find(_T("Fwd"));
   int iInv = baseName.Find(_T("Inv"));
   if (iFwd >= 0) 
      baseName = baseName.Left(iFwd) + _T("Inv") + baseName.Mid(iFwd + 3);
   if (iInv >= 0) 
      baseName = baseName.Left(iInv) + _T("Fwd") + baseName.Mid(iInv + 3);
   int iEmph = baseName.Find(_T("_"));
   CString startName;
   if (iEmph > 0) {
      startName = baseName.Left(iEmph + 1);
      baseName = baseName.Mid(iEmph + 1);
   }
   int iTo = baseName.Find(_T("To"));
   if (iTo < 0) return startName + baseName;
   baseName = baseName.Mid(iTo + 2) + _T("To") + baseName.Left(iTo);
   return startName + baseName;
}

CFunc CFunc::FuncWithBase(CMyString baseName) const
{
   CFunc func = Prefix() + baseName + _T("_") + TypeName();
   if (!DescrName().IsEmpty())
      func += _T("_") + DescrName();
   return func;
}

CFunc CFunc::FuncWithBase_InvType(CMyString baseName) const
{
   CMyString sBase, sType1, sType2, sType3, sDescr;
   ParseFunction(sBase, sType1, sType2, sType3, sDescr);
   CFunc func = Prefix() + baseName + _T("_");
   if (sType1 == sType3)
      func += sType1 ;
   else
      func += sType3 + sType1 ;
   if (!sDescr.IsEmpty())
      func += _T("_") + sDescr;
   return func;
}

CFunc CFunc::FuncWithBase_InvChannels(CMyString baseName) const
{
   CMyString sBase, sType, sChan1, sChan2, sDescr;
   ParseFunction(sBase, sType, sDescr);
   if (sDescr.IsEmpty())
      return Prefix() + baseName + _T("_") + sType;
   sChan1 = firstChanStr(sDescr);
   sChan2 = firstChanStr(sDescr);
   CFunc func = Prefix() + baseName + _T("_") + sType + _T("_");
   if (sChan2.IsEmpty())
      func += sChan1 ;
   else
      func += sChan2 + sChan1 ;
   func += sDescr;
   return func;
}

CMyString CFunc::VecName(int vecPos, int usedVectors) const
{
   CMyString src, src2, dst, dst2;
   GetVecNames(src, src2, dst, dst2, usedVectors);
   switch (vecPos) {
   case VEC_SRC: return src;
   case VEC_SRC2: return src2;
   case VEC_DST: return dst;
   case VEC_DST2: return dst2;
   case VEC_MASK: return _T("Mask");
   }
   return _T("");
}

void CFunc::GetVecNames(CMyString& src, CMyString& src2, 
                        CMyString& dst, CMyString& dst2,
                       int usedVectors) const
{
   src = _T(""); src2 = _T(""); dst = _T(""); dst2 = _T("");
   if (usedVectors & VEC_SRC2) {
      if (Inplace()) {
         src = _T("Src"); src2 = _T("SrcDst");
      } else {
         src = _T("Src1"); src2 = _T("Src2");
      }
   } else {
      if (Inplace()) src = _T("SrcDst");
      else           src = _T("Src");
   }
   if (usedVectors & VEC_DST2) {
      dst = _T("Dst1"); dst2 = _T("Dst2");
   } else {
      dst = _T("Dst");
   }
   UpdateVecNames(src,src2,dst,dst2,usedVectors);
}

////////////////////////////////////////////////////////////////////

CFuncList::CFuncList()
{
    m_position = NULL;
    m_baseName = _T("");
    m_pOutstandList = NULL;
}

CFuncList::~CFuncList()
{
}

void CFuncList::CreateFuncList(int idx)
{
   if (pGlobalFuncList == NULL) return;
   if (idx >= pGlobalFuncList->GetSize()) return;
   AddTail(pGlobalFuncList->ElementAt(idx));
   SetOutstandList(idx);
   m_position = GetHeadPosition();
}

POSITION CFuncList::Find(LPCTSTR funcName, POSITION pos) const
{
   if (pos == NULL) pos = GetHeadPosition(); 
   else             GetNext(pos);
   while (pos) {
      POSITION curPos = pos;
      if (GetNext(pos) == funcName) return curPos;
   }
   return NULL;
}

int CFuncList::SubtractThisList(CStringList& rList)
{
   POSITION rPos = rList.GetHeadPosition(); 
   while (rPos) {
      POSITION tPos = rPos;
      CString tName = rList.GetNext(rPos);
      POSITION fPos = GetHeadPosition();
      while (fPos) {
         if (tName == GetNext(fPos)) {
            rList.RemoveAt(tPos);
            break;
         }
      }
   }
   return (int)GetCount();
}

int CFuncList::SetOutstandList(int idx) 
{
   CFuncList dirList;;
   DEMO_APP->GetDirector()->GetFunctions(dirList, idx);
   if (m_pOutstandList) delete m_pOutstandList;
   m_pOutstandList = new CFuncList;
   POSITION pos = GetHeadPosition(); 
   while (pos) {
      POSITION delPos = pos;
      CFunc func = GetNext(pos);
      if (func.Find(_T("Set")) != -1 && func.BaseName() != _T("Set")) continue;
      if (func.Find(_T("Get")) != -1) continue;
      if (func.Find(_T("Init")) != -1) continue;
      if (func.Find(_T("Close")) != -1) continue;
      if (func.Find(_T("Malloc")) != -1) continue;
      if (func.Find(_T("Free")) != -1) continue;
      if (func.Found(_T("AlphaPremul"))) continue;
      if (func.Found(_T("AddRotateShift"))) continue;
      if (func.Found(_T("PutVal"))) continue;
      if (func.Found(_T("UpdateTaps"))) continue;
      if (func.Found(_T("DrawText"))) continue;
      if (func.Found(_T("DV"))) continue;
      if (func.Found(_T("Resize_"))) continue;
      if (func.Found(_T("ResizeShift_"))) continue;
      if (func.Found(_T("ResizeCenter_"))) continue;
      if (func.Found(_T("_64f_"))) continue;
      POSITION dirPos = dirList.Find(func);
      if (dirPos) {
         dirList.RemoveAt(dirPos);
         continue;
      }
      m_pOutstandList->AddTail(func);
      RemoveAt(delPos);
   }
   return (int)m_pOutstandList->GetCount();
}

void CRecentFuncList::AddFunc(CFunc func)
{
   POSITION rPos = GetHeadPosition();
   while (rPos) {
      POSITION pos = rPos;
      if (GetNext(rPos) == func) RemoveAt(pos);
   }
   AddHead(func);
   if (GetCount() > m_MaxSize) RemoveTail();
}

CFunc CRecentFuncList::GetFunc(int index)
{
   POSITION rPos = GetHeadPosition();
   for (int i = 1; rPos; i++) {
      CFunc func = GetNext(rPos);
      if (i == index) return func;
   }
   CFunc emptyFunc;
   return emptyFunc;
}

static CVector* pMruVec = NULL;
static BOOL ValidMru(CString name)
{
   CFunc func(name);
   return func.Valid(pMruVec);
}
static BOOL InvalidMru(CString name)
{
   return FALSE;
}

void CRecentFuncList::UpdateMenu(CCmdUI* pCmdUI, CVector* pVec, BOOL bEnable)
{
   CMruMenu menu(8,_T("Recent Function"));
   pMruVec = pVec;
   if (GetCount() == 0) {
      CFunc func = DEMO_APP->GetDirector()->GetProcess()->GetCurrentFunction();
      if (!func.IsEmpty()) AddFunc(func);
   }
   menu.SetList(this);
   menu.UpdateMenu(pCmdUI, bEnable ? ValidMru : InvalidMru);
}
/*
void CMruMenu::UpdateMenu(CCmdUI* pCmdUI)
{
   CMenu* pMenu = pCmdUI->m_pMenu;
   if (pMenu == NULL || GetSize() == 0) {
      pCmdUI->Enable(FALSE);
      pCmdUI->SetText(m_EmptyName);
      return;
   }

   for (int iMRU = 0; iMRU < m_MenuSize; iMRU++)
      pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

   int nID = pCmdUI->m_nID;
   POSITION pos = GetHeadPosition();
   for (int i=0; i<mru.GetSize(); i++) {
      pMenu->InsertMenu(pCmdUI->m_nIndex++, MF_STRING | MF_BYPOSITION, 
         nID, GetAt(i).name);
      pMenu->EnableMenuItem(nID, MF_BYCOMMAND |  
         (GetAt(i).enable) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
      nID++;
   }

   pCmdUI->m_nIndex--;
   pCmdUI->m_nIndexMax = pMenu->GetMenuItemCount();

   pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
   m_MenuSize = GetCount();
}
*/