
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1998-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "stdafx.h"
#ifndef __EXCEPT_H__
#include "except.h"
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




static CMyException _simpleMyException;


void AfxThrowMyException(PSTR msg,PSTR file,UINT line)
{
  _simpleMyException.InitString(msg,file,line);

  throw &_simpleMyException;

  return;
} // AfxThrowMyException()


void AfxThrowMyException(CString& msg,PSTR file,UINT line)
{
  _simpleMyException.InitString(msg,file,line);

  throw &_simpleMyException;

  return;
} // AfxThrowMyException()


void AfxThrowMyExceptionEx(CString& msg,DWORD syserr,PSTR file,UINT line)
{
  _simpleMyException.InitStringEx(msg,syserr,file,line);

  throw &_simpleMyException;

  return;
} // AfxThrowMyExceptionEx()


void AfxReThrowMyException()
{
  throw &_simpleMyException;
  return;
} // AfxReThrowMyException()




IMPLEMENT_DYNAMIC(CMyException,CException);


CMyException::CMyException()
  : CException(FALSE) // disable auto delete
{
  memset(m_msg,0,MAX_MSG);
  memset(m_sys,0,MAX_MSG);

#ifdef _DEBUG
  memset(m_file,0,MAX_FILE);
  memset(m_line,0,MAX_LINE);
#endif

  return;
} // CMyException::CMyException() ctor


void CMyException::ReportError(void)
{
  TRACE("in CMyException::ReportError()\n");

  #ifdef _DEBUG
  UINT mode = MB_ABORTRETRYIGNORE|MB_ICONSTOP;
  #else
  UINT mode = MB_OK|MB_ICONSTOP;
  #endif

  int res = AfxMessageBox(m_msg,mode);
  if(IDABORT == res || IDOK == res || res == 0)
  {
    exit(3);
  }

  if(IDRETRY == res)
  {
    AfxDebugBreak();
  }

  return;
} // CMyException::ReportError()


void CMyException::InitString(PSTR msg,PSTR file,UINT line)
{
  TRACE("in CMyException::InitString()\n");

  DWORD error = GetLastError();
  TRACE("  error code is 0x%08X\n",error);

  if(0 != error)
  {
    FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
      (LPSTR)&m_sys,
      MAX_MSG,
      NULL);
  }

  sprintf(m_msg,"ERROR\t:\t%s\n",msg);
  sprintf(m_tmp,"REASON\t:\t%s\n",m_sys);
  if('0' != m_sys[0])
  {
    strcat(m_msg,m_tmp);
  }

  if(NULL != file)
  {
    #ifdef _DEBUG
    sprintf(m_file,"FILE\t:\t%s\n",file);
    strcat(m_msg,m_file);
    #endif
  }

  if(0 != line)
  {
    #ifdef _DEBUG
    sprintf(m_line,"LINE\t:\t%d\n",line);
    strcat(m_msg,m_line);
    #endif
  }

  return;
} // CMyException::InitString()


void CMyException::InitString(CString& msg,PSTR file,UINT line)
{
  TRACE("in CMyException::InitString()\n");

  DWORD error = ::GetLastError();
  TRACE("  error code is 0x%08X\n",error);

  if(0 != error)
  {
    ::FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
      (LPSTR)&m_sys,
      MAX_MSG,
      NULL);
  }

  sprintf(m_msg,"ERROR\t:\t%s\n",msg);
  sprintf(m_tmp,"REASON\t:\t%s\n",m_sys);
  if(0 != m_sys[0])
  {
    strcat(m_msg,m_tmp);
  }

  if(NULL != file)
  {
    #ifdef _DEBUG
    sprintf(m_file,"FILE\t:\t%s\n",file);
    strcat(m_msg,m_file);
    #endif
  }

  if(0 != line)
  {
    #ifdef _DEBUG
    sprintf(m_line,"LINE\t:\t%d\n",line);
    strcat(m_msg,m_line);
    #endif
  }

  return;
} // CMyException::InitString()


void CMyException::InitStringEx(CString& msg,DWORD syserr,PSTR file,UINT line)
{
  TRACE("in CMyException::InitString()\n");

  if(0 != syserr)
  {
    ::FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      syserr,
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
      (LPSTR)&m_sys,
      MAX_MSG,
      NULL);
  }

  sprintf(m_msg,"ERROR\t:\t%s\n",msg);
  sprintf(m_tmp,"REASON\t:\t%s\n",m_sys);
  if(0 != m_sys[0])
  {
    strcat(m_msg,m_tmp);
  }

  if(NULL != file)
  {
    #ifdef _DEBUG
    sprintf(m_file,"FILE\t:\t%s\n",file);
    strcat(m_msg,m_file);
    #endif
  }

  if(0 != line)
  {
    #ifdef _DEBUG
    sprintf(m_line,"LINE\t:\t%d\n",line);
    strcat(m_msg,m_line);
    #endif
  }

  return;
} // CMyException::InitStringEx()


void CMyException::AddString(PCSTR msg,PSTR file,UINT line)
{
  TRACE("in CMyException::AddString()\n");

  CHAR str[MAX_MSG];

  sprintf(str,"ADDED\t:\t%s\n",msg);
  strcat(m_msg,str);

  if(NULL != file)
  {
    #ifdef _DEBUG
    sprintf(m_file,"FILE\t:\t%s\n",file);
    strcat(m_msg,m_file);
    #endif
  }

  if(0 != line)
  {
    #ifdef _DEBUG
    sprintf(m_line,"LINE\t:\t%d\n",line);
    strcat(m_msg,m_line);
    #endif
  }

  return;
} // CMyException::AddString()

