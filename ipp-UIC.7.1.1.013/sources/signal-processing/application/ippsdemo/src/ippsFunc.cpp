/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsFunc.cpp: implementation of the CFunc class for ippsDemo
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Func.h"
#include "Vector.h"
#include "ippsProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CFunc::Valid(CVector* pVec, int vecPos) const
{
   CString baseName = BaseName();
   if (baseName == "FindNearestOne") return TRUE;
   if ((*this) == "ippsCopy_1u") return TRUE;
   ppType type = ppNONE;
   switch (vecPos) {
   case VEC_SRC:  type = SrcType();  break;
   case VEC_SRC2: type = Src2Type(); break;
   case VEC_SRC3: type = Inplace() ? DstType() : Src2Type(); break;
   case VEC_DST:  type = DstType();  break;
   case VEC_DST2: type = Dst2Type(); break;
   };
   if (CippsProcess::DepthEssencial(*this)) {
      if ((int)(pVec->Type() & PP_MASK) != (int)(type & PP_MASK)) return FALSE;
      if ((int)(pVec->Type() & PP_CPLX) != (int)(type & PP_CPLX)) return FALSE;
      if ((BaseName() == "SwapBytes") && (pVec->Type() & PP_FLOAT)) return FALSE;
   } else {
      if (pVec->Type() != type) return FALSE;
   }
   return TRUE;
}

void CFunc::UpdateTypes(ppType& src1, ppType& src2, 
                        ppType& dst1, ppType& dst2) const
{
   if (BaseName() == "Real" ||
       BaseName() == "Imag" ||
       BaseName() == "Magnitude" ||
       BaseName() == "Phase" ||
       BaseName() == "CplxToReal" ||
       BaseName() == "CartToPolar")
       dst1 = dst2 = (ppType)((int)dst1 & ~PP_CPLX);
   if (BaseName() == "PolarToCart")
       src1 = src2 = (ppType)((int)src1 & ~PP_CPLX);
   if (BaseName() == "RealToCplx")
       dst1 = dst2 = (ppType)((int)dst1 | PP_CPLX);
   if (BaseName() == "DotProd") {
      if (src1 == src2 && 
         !((int)src1 & PP_CPLX) &&
          ((int)dst1 & PP_CPLX)) src2 = dst1;
   }
   if (BaseName() == "PowerSpectr")
      dst1 = dst2 = (ppType)((int)dst1 & ~PP_CPLX);
   if (BaseName() == "MaxOrder")
      dst1 = dst2 = pp32s;
   if (BaseName().Found("ConvCyclic"))
      src2 = dst1;
   if (Found("Mul_16u16s")) 
      src2 = dst1;
   if (Found("Mul_32s32sc") || Found("Mul_32f32fc")) 
      src2 = dst1;
   if (Found("LMSMR")) {
      src2 = dst1 = BaseType();
   }
}

void CFunc::UpdateChannels(int& src, int& dst) const
{
}

void CFunc::UpdatePlanes(BOOL& src, BOOL& dst) const
{
}

CFunc CFunc::GetPair() const
{
   if (*this == "ippsConvert_8s32f" ) return "ippsConvert_32f8s_Sfs" ;
   if (*this == "ippsConvert_8u32f" ) return "ippsConvert_32f8u_Sfs" ;
   if (*this == "ippsConvert_16s32f") return "ippsConvert_32f16s_Sfs";
   if (*this == "ippsConvert_16u32f") return "ippsConvert_32f16u_Sfs";
   if (*this == "ippsConvert_32s64f") return "ippsConvert_64f32s_Sfs";
   if (*this == "ippsConvert_32s32f") return "ippsConvert_32f32s_Sfs";
   if (*this == "ippsConvert_32f8s_Sfs" ) return "ippsConvert_8s32f" ;
   if (*this == "ippsConvert_32f8u_Sfs" ) return "ippsConvert_8u32f" ;
   if (*this == "ippsConvert_32f16s_Sfs") return "ippsConvert_16s32f";
   if (*this == "ippsConvert_32f16u_Sfs") return "ippsConvert_16u32f";
   if (*this == "ippsConvert_64f32s_Sfs") return "ippsConvert_32s64f";
   if (*this == "ippsConvert_32f32s_Sfs") return "ippsConvert_32s32f";

   if (*this == "ippsCplxToReal_64fc") return "ippsRealToCplx_64f";
   if (*this == "ippsCplxToReal_32fc") return "ippsRealToCplx_32f";
   if (*this == "ippsCplxToReal_16sc") return "ippsRealToCplx_16s";
   if (*this == "ippsRealToCplx_64f" ) return "ippsCplxToReal_64fc";
   if (*this == "ippsRealToCplx_32f" ) return "ippsCplxToReal_32fc";
   if (*this == "ippsRealToCplx_16s" ) return "ippsCplxToReal_16sc";

   CString baseName = BaseName();
   if (Found("Polar") || Found("Cart"))
      return FuncWithBase(InvBase());

   if (Found("ALaw") || Found("MuLaw")) 
      return FuncWithBase_InvType(InvBase());

   if (Found("Fwd") || Found("Inv")) 
      return FuncWithBase_InvType(InvBase());

   return "";
}

void CFunc::UpdateVecNames(CMyString& src, CMyString& src2, 
                           CMyString& dst, CMyString& dst2,
                           int usedVectors) const
{
   if (BaseName() == "Real") {
      dst = "DstRe";
   }
   if (BaseName() == "Imag") {
      dst = "DstIm";
   }
   if (BaseName() == "CplxToReal") {
      dst = "DstRe"; dst2 = "DstIm";
   }
   if (BaseName() == "RealToCplx") {
      src = "SrcRe"; src2 = "SrcIm";
   }
   if (BaseName() == "CartToPolar") {
      if (usedVectors & VEC_SRC2) {
         src = "SrcRe"; src2 = "SrcIm";
      }
      dst = "DstMagn"; dst2 = "DstPhase";
   }
   if (BaseName() == "PolarToCart") {
      if (usedVectors & VEC_DST2) {
         dst = "DstRe"; dst2 = "DstIm";
      }
      src = "SrcMagn"; src2 = "SrcPhase";
   }
   if (BaseName() == "Magnitude" || BaseName() == "Phase") {
      if (usedVectors & VEC_SRC2) {
         src = "SrcRe"; src2 = "SrcIm";
      }
   }
   if (Found("FFT") || Found("DFT")) {
      if (usedVectors & VEC_SRC2) {
         src = "SrcRe"; src2 = "SrcIm";
      }
      if (usedVectors & VEC_DST2) {
         dst = "DstRe"; dst2 = "DstIm";
      }
   }
   if (BaseName().Find("WT") == 0) {
      if (Found("Fwd")) {
         dst = "DstLow"; dst2 = "DstHigh";
      }
      if (Found("Inv")) {
         src = "SrcLow"; src2 = "SrcHigh";
      }
   }
   if (Found("LMS")) {
      src2 = "Ref";
      dst2 = "Taps";
   }
}
