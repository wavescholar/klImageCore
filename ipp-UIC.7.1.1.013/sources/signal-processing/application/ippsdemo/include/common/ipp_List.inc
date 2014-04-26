/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ipp_list.inc file is the code fragment to be inserted into 
// ippslist.cpp, ippilist.cpp files. This fragment helps to generate 
// function list out of IPP header file 

#include <ippdefs.h>
#include "Func.h"

CArray<CFuncList*,CFuncList*>* pGlobalFuncList = NULL;

static BOOL regFunc(CFunc name, int idx)
{
   if (pGlobalFuncList == NULL) {
      pGlobalFuncList = new CArray<CFuncList*,CFuncList*>;
   }
   if (pGlobalFuncList->GetSize() <= idx) {
      CFuncList* pList = new CFuncList;
      pGlobalFuncList->SetAtGrow(idx, pList);
   }
   pGlobalFuncList->ElementAt(idx)->AddTail(name);
   return TRUE;
}

#if !defined (_OWN_BLDPCS)
#define _OWN_BLDPCS
#endif

#if !defined (_INC_WINDOWS)
#define _INC_WINDOWS
#endif

#if defined (IPPAPI)
#undef IPPAPI
#endif 

#define IPPAPI(Type,Name,Arg) \
BOOL reg_##Name = regFunc(#Name, LIB_IDX);
