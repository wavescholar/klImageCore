/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ipp_type.h generates IPP function types out of IPP headers.
// After that IPP functions may be used for exported library calls
//
//////////////////////////////////////////////////////////////////////

#if !defined (_OWN_BLDPCS)
#define _OWN_BLDPCS
#endif
#if !defined (_INC_WINDOWS)
#define _INC_WINDOWS
#endif
#ifdef IPPAPI
#undef IPPAPI
#endif
#define IPPAPI(res,name,params) \
  typedef res (__stdcall *typ_##name)params; 
#include "ipp_header.h"
#undef IPPAPI
