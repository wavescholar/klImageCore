
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
//
*/

#ifndef __EXCEPT_H__
#define __EXCEPT_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _FULLDIAG
#pragma message("  except.h")
#endif


#ifndef __AFXWIN_H__
#include "afxwin.h"
#endif




//////////////////////////////////////////////////////////////////////////////
// I use own macros, against same MFC macro

#ifdef THROW
#undef THROW
#endif


#ifdef TRY
#undef TRY
#endif


#ifdef CATCH
#undef CATCH
#endif


#ifdef CATCH_ALL
#undef CATCH_ALL
#endif


#ifdef END_CATCH_ALL
#undef END_CATCH_ALL
#endif


#ifdef AND_CATCH_ALL
#undef AND_CATCH_ALL
#endif


#ifdef END_CATCH
#undef END_CATCH
#endif


#ifdef _DEBUG

#define STR_FILE THIS_FILE
#define INT_LINE __LINE__

#else // _DEBUG

#define STR_FILE NULL
#define INT_LINE 0

#endif


#define RETHROW() \
  AfxReThrowMyException()

#define THROW(message) \
  AfxThrowMyException(message,STR_FILE,INT_LINE)

#define THROWEX(message,syserr) \
  AfxThrowMyExceptionEx(message,syserr,STR_FILE,INT_LINE)


#define THROW_IF(expression,message) \
  if(FALSE != (expression)) { \
    THROW(message); \
  }


#define TRY try {

#define CATCH(c_ptr,e) } catch(c_ptr* e) {

#define CATCH_ALL } catch(...) {

#define END_CATCH }

#define AND_CATCH_ALL } catch(...) {

#define END_CATCH_ALL }




//////////////////////////////////////////////////////////////////////////////
// it simple throw static global object CMyException, and
// initialize descriptive string with file name and line (in DEBUG version)

void AfxThrowMyException(PSTR msg,PSTR file,UINT line);
void AfxThrowMyException(CString& msg,PSTR file,UINT line);
void AfxThrowMyExceptionEx(CString& msg,DWORD syserr,PSTR file,UINT line);

void AfxReThrowMyException();




#define MAX_MSG  255


#ifdef _DEBUG

#define MAX_FILE MAX_PATH
#define MAX_LINE 16

#endif



//////////////////////////////////////////////////////////////////////////////
// simple exception object

class CMyException : public CException
{
private:
  CHAR m_msg[MAX_MSG];
  CHAR m_sys[MAX_MSG];
  CHAR m_tmp[MAX_MSG];

#ifdef _DEBUG
  CHAR m_file[MAX_FILE];
  CHAR m_line[MAX_LINE];
#endif

public:
  CMyException();

  void InitString(PSTR msg,PSTR file,UINT line);
  void InitString(CString& msg,PSTR file,UINT line);
  void InitStringEx(CString& msg,DWORD syserr,PSTR file,UINT line);
  void AddString(PCSTR msg,PSTR file,UINT line);
  void ReportError(void);

public:
  DECLARE_DYNAMIC(CMyException)
};


#endif // __EXCEPT_H__
