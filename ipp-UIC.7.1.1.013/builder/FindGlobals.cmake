##******************************************************************************
##  Copyright(C) 2012 Intel Corporation. All Rights Reserved.
##  
##  The source code, information  and  material ("Material") contained herein is
##  owned  by Intel Corporation or its suppliers or licensors, and title to such
##  Material remains  with Intel Corporation  or its suppliers or licensors. The
##  Material  contains proprietary information  of  Intel or  its  suppliers and
##  licensors. The  Material is protected by worldwide copyright laws and treaty
##  provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
##  modified, published, uploaded, posted, transmitted, distributed or disclosed
##  in any way  without Intel's  prior  express written  permission. No  license
##  under  any patent, copyright  or  other intellectual property rights  in the
##  Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
##  implication, inducement,  estoppel or  otherwise.  Any  license  under  such
##  intellectual  property  rights must  be express  and  approved  by  Intel in
##  writing.
##  
##  *Third Party trademarks are the property of their respective owners.
##  
##  Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
##  this  notice or  any other notice embedded  in Materials by Intel or Intel's
##  suppliers or licensors in any way.
##
##******************************************************************************
##  Content: Intel(R) IPP Samples projects creation and build
##******************************************************************************

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_VERBOSE_MAKEFILE             TRUE )

collect_oses( )
collect_arch( )

if(__ARCH MATCHES intel64)
  add_definitions(-DINTEL64)
else ( )
  add_definitions(-DIA32)
endif( )


if( Windows )
  add_definitions(/DWINDOWS)
  add_definitions(/D_SBCS /DWIN32 /D_WIN32 /D_WIN32_WINNT=0x501)

  if(__ARCH MATCHES intel64)
    add_definitions(/DWIN64 /D_WIN64)
  endif( )

  set(CMAKE_C_FLAGS_DEBUG     "/Od /W3 /fp:fast /TC /MDd /Zi /D_DEBUG"       CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_RELEASE   "/O2 /W3 /fp:fast /TC /MD"                     CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG   "/Od /W3 /fp:fast /TP /MDd /EHsc /Zi /D_DEBUG" CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /W3 /fp:fast /TP /MD  /EHsc"              CACHE STRING "" FORCE)

else( )
  add_definitions(-DUNIX)

  if( Linux )
    add_definitions(-DLINUX)
    add_definitions(-DLINUX32)

    if(__ARCH MATCHES intel64)
      add_definitions(-DLINUX64)
    endif( )
  endif( )

  if( Darwin )
    add_definitions(-DOSX)
    add_definitions(-DOSX32)

    if(__ARCH MATCHES intel64)
      add_definitions(-DOSX64)
    endif( )
  endif( )

  set(CMAKE_C_FLAGS_DEBUG     "-O0 -Wall -g -D_DEBUG" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_RELEASE   "-O2 -Wall"             CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -Wall -g -D_DEBUG" CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall"             CACHE STRING "" FORCE)

  if(__ARCH MATCHES ia32)
    append("-m32" CMAKE_C_FLAGS)
    append("-m32" CMAKE_CXX_FLAGS)
    append("-m32" LINK_FLAGS)
  else ( )
    append("-m64" CMAKE_C_FLAGS)
    append("-m64" CMAKE_CXX_FLAGS)
    append("-m64" LINK_FLAGS)
  endif( )

  if(__ARCH MATCHES ia32)
    link_directories(/usr/lib)
  else ( )
    link_directories(/usr/lib64)
  endif( )
endif( )
