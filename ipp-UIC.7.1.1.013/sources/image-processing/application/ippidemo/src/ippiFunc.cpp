/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiFunc.cpp: implementation of the CFunc class for ippiDemo
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Func.h"
#include "Vector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {L444, L422, L420, L411};

static BOOL is4xx(const CFunc* pFunc, int vecPos, CString str4xx)
{
   if ((vecPos == VEC_SRC) && !pFunc->SrcPlane()) return FALSE;
   if ((vecPos == VEC_DST) && !pFunc->DstPlane()) return FALSE;
   int i4xx = pFunc->Find(str4xx);
   int iTo  = pFunc->Find("To");
   if (i4xx < 0) return FALSE;
   if (pFunc->Found("Join") || pFunc->Found("Split")) return TRUE;
   if (iTo < 0) return FALSE;
   if (i4xx - iTo == 4) return TRUE;
   if ((i4xx < iTo) && (vecPos == VEC_SRC)) return TRUE;
   if ((i4xx > iTo) && (vecPos == VEC_DST)) return TRUE;
   return FALSE;
}

static int getFuncLayout(const CFunc* pFunc, int vecPos)
{
   if (is4xx(pFunc, vecPos, "422")) {
      if ((vecPos == VEC_SRC) && (pFunc->SrcChannels() == 2)) return L444;
      if ((vecPos == VEC_DST) && (pFunc->DstChannels() == 2)) return L444;
      return L422;
   }
   if (is4xx(pFunc, vecPos, "420"))
      return L420;
   if (is4xx(pFunc, vecPos, "411"))
      return L411;
   return L444;
}

static int getImageLayout(CImage* pImg)
{
   if (pImg->Is422()) return L422;
   if (pImg->Is420()) return L420;
   if (pImg->Is411()) return L411;
   return L444;
}

BOOL CFunc::Valid(CVector* pVec, int vecPos) const
{
   CImage* pImg = (CImage*)pVec;
   ppType  type = ppNONE;
   int     channels = 0;
   BOOL    bPlane = 0;
   int     layout = L444;

   switch (vecPos) {
   case VEC_SRC:
      type = SrcType();  channels = SrcChannels(); bPlane = SrcPlane();
      layout = getFuncLayout(this, VEC_SRC);
      break;
   case VEC_SRC2:
      type = Src2Type(); channels = SrcChannels(); bPlane = SrcPlane();
      break;
   case VEC_MASK:
      type = pp8u; channels = 1; bPlane = FALSE;
      break;
   case VEC_DST:
      type = DstType();  channels = DstChannels(); bPlane = DstPlane();
      layout = getFuncLayout(this, VEC_DST);
      break;
   case VEC_DST2:
      type = Dst2Type(); channels = DstChannels(); bPlane = DstPlane();
      break;
   };
   if (pImg->Type() != type) {
      if (BaseName() == "Mirror") {
         if (pImg->Depth() != CVector::Depth(type))
            return FALSE;
         if (pImg->Complex() != CVector::Complex(type))
            return FALSE;
      } else {
         return FALSE;
      }
   }

   if (pImg->GetCoi() < 0 && layout != getImageLayout(pImg)) return FALSE;
   if (bPlane) {
      if (!pImg->Plane()) return FALSE;
      if (pImg->GetCoi() >= 0) return FALSE;
      if (pImg->Channels() < channels) return FALSE;
   } else {
      if (pImg->Plane()) {
         if (channels > 1) return FALSE;
         if (pImg->GetCoi() < 0) {
            if (SrcChannels() != DstChannels()) return FALSE;
            if (pImg->Is420() || pImg->Is422()) return FALSE;
         }
      } else {
         if (pImg->Channels() != channels) return FALSE;
      }
   }

   if (pImg->IsRoi() && !Roi()) {
      if (BaseName() != "DCT") return FALSE;
   }

   if (Found("Decode")) {
      if (!pVec->GetCustomData("CodeParms"))
         return FALSE;
   }
   return TRUE;
}

void CFunc::UpdateTypes(ppType& src1, ppType& src2,
                        ppType& dst1, ppType& dst2) const
{
   if (BaseName() == "Sum" || BaseName() == "Mean")
      dst1 = dst2 = pp64f;
   if (BaseName() == "Copy")
      src2 = pp8u;
   if (BaseName() == "Set")
      src2 = pp8u;
   if (BaseName().Found("Compare"))
      dst1 = dst2 = pp8u;
}

void CFunc::UpdateChannels(int& src, int& dst) const
{
   CMyString base = BaseName();
   if (base.Found("Compare"))
      dst = 1;
   if (base.Found("RGB444") || base.Found("RGB555") || base.Found("RGB565") ||
       base.Found("BGR444") || base.Found("BGR555") || base.Found("BGR565")) {
      if (base.Found("ToRGB") || base.Found("ToBGR"))
         dst = 1;
      else
         src = 1;
   }
}

void CFunc::UpdatePlanes(BOOL& src, BOOL& dst) const
{
   CMyString descr = DescrName();
}

static CMyString channelName(int chan, BOOL bPlane, BOOL bAlpha)
{
   CMyString name;
   if (chan == 4 && bAlpha)
      name << "A";
   if (bPlane) name << "P";
   else        name << "C";
   return name << chan;
}

static CString invTo(CString baseName)
{
   int iTo = baseName.Find("To");
   if (iTo < 0) return baseName;
   CString start, left, right, end;
   left = baseName.Mid(0, iTo);
   right = baseName.Mid(iTo + 2);
   int i;
   i = left.ReverseFind('_');
   if (i >= 0) {
      start = left.Mid(0, i + 1);
      left  = left.Mid(i + 1);
   }
   i = right.Find('_');
   if (i >= 0) {
      end   = right.Mid(i);
      right = right.Mid(0, i);
   }
   return start + right + "To" + left + end;
}

static CString setDescr(CFunc func, CString descr)
{
   return func.Prefix()
        + func.BaseName() + "_"
        + func.TypeName() + "_"
        + descr;
}

CFunc CFunc::GetPair() const
{
   CString baseName = BaseName();
   if (Found("Fwd") || Found("Inv")) {
      BOOL bFwd = Found("Fwd");
      CString sFwd = bFwd ? "Fwd" : "Inv";
      CString sInv = bFwd ? "Inv" : "Fwd";

      int index;
      index = baseName.Find(sFwd);
      baseName = baseName.Mid(0,index) + sInv + baseName.Mid(index + 3);

      index = baseName.Find("LS");
      if (index > 0)
         baseName = baseName.Mid(0,index) + (bFwd ? "LSClip" : "LS");

      baseName = invTo(baseName);
      return FuncWithBase_InvType(baseName);
   }
   if (Found("Inv")) {
      int index = baseName.Find("Inv");
      CString pairBase = baseName.Mid(0,index) + "Fwd" + baseName.Mid(index + 3);
      index = pairBase.Find("LS");
      if (index > 0) pairBase = pairBase.Mid(0,index) + "LS";
      return FuncWithBase_InvType(pairBase);
   }
   if (Found("RGB") || Found("BGR")) {
      baseName = invTo(baseName);
      CString descrName;
      if (DescrName() == "AC4P4R") descrName = "AP4C4R";
      if (DescrName() == "AP4C4R") descrName = "AC4P4R";
      CFunc func = FuncWithBase_InvChannels(baseName);
      func = func.FuncWithBase_InvType(baseName);
      if (descrName.IsEmpty())
         return func;
      else
         return setDescr(func,descrName);
   }
   if (baseName == "Scale" || baseName == "Convert") {
      return FuncWithBase_InvType(baseName);
   }
   if (baseName == "Copy") {
      if (SrcChannels() == DstChannels()) return "";
      if (DescrName().Found("CR")) return "";
      return FuncWithBase_InvChannels(baseName);
   }
   return "";
}

void CFunc::UpdateVecNames(CMyString& src, CMyString& src2,
                           CMyString& dst, CMyString& dst2,
                           int usedVectors) const
{
}
