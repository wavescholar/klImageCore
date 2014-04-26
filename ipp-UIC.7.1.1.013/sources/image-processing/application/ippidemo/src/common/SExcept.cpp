/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SExcept.cpp: implementation of the CSExcept class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "SExcept.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static void SETranslate(unsigned int exCode, _EXCEPTION_POINTERS* exPtrs)
{
    throw CSExcept(exCode,exPtrs);
}

void CSExcept::InitTranslator() {_set_se_translator(SETranslate);}

CString CSExcept::GetExString()
{
    switch (m_ExCode) {
    case EXCEPTION_ACCESS_VIOLATION : return _T("Access violation");
    case EXCEPTION_BREAKPOINT : return _T("Breakpoint");
    case EXCEPTION_DATATYPE_MISALIGNMENT : return _T("Data type misalignment");
    case EXCEPTION_SINGLE_STEP : return _T("Single step");
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED : return _T("Array bounds exceeded");
    case EXCEPTION_FLT_DENORMAL_OPERAND : return _T("Float denormal operand");
    case EXCEPTION_FLT_DIVIDE_BY_ZERO : return _T("Float divide by zero");
    case EXCEPTION_FLT_INEXACT_RESULT : return _T("Float inexact result");
    case EXCEPTION_FLT_INVALID_OPERATION : return _T("Float invalid operation");
    case EXCEPTION_FLT_OVERFLOW : return _T("Float overflow");
    case EXCEPTION_FLT_STACK_CHECK : return _T("Float stack check");
    case EXCEPTION_FLT_UNDERFLOW : return _T("Float underflow");
    case EXCEPTION_INT_DIVIDE_BY_ZERO : return _T("Integer divide by zero");
    case EXCEPTION_INT_OVERFLOW : return _T("Integer overflow");
    case EXCEPTION_PRIV_INSTRUCTION : return _T("Privileged instruction");
    case EXCEPTION_NONCONTINUABLE_EXCEPTION : return _T("Noncontinuable exception");
    default: return _T("");
    }
}
