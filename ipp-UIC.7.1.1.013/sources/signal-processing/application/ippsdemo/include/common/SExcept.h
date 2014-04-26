/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SExcept.h: interface for the CSExcept class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEXCEPT_H__15C6B2D5_B2D1_4C9D_AA32_86083850C10B__INCLUDED_)
#define AFX_SEXCEPT_H__15C6B2D5_B2D1_4C9D_AA32_86083850C10B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <eh.h>

class CSExcept  
{
public:
    CSExcept(unsigned int exCode, _EXCEPTION_POINTERS* exPtrs = NULL) 
    { m_ExCode = exCode; m_pExPtrs = exPtrs; }
    virtual ~CSExcept() {}
    unsigned int GetExCode() { return m_ExCode;}
    _EXCEPTION_POINTERS* GetExPtrs() { return m_pExPtrs;}
    CString GetExString();

    static void InitTranslator();

protected:
    unsigned int m_ExCode;
    _EXCEPTION_POINTERS* m_pExPtrs;
};

#endif // !defined(AFX_SEXCEPT_H__15C6B2D5_B2D1_4C9D_AA32_86083850C10B__INCLUDED_)
