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

if( ia32 )
  set( LIBS_ARCH x86 )
else ( )
  set( LIBS_ARCH x64 )
endif( )

find_path    ( DXSDK_INCLUDE dsound.h   PATHS $ENV{DXSDK_DIR}/include )
find_library ( DXSDK_LIBRARY dsound.lib PATHS $ENV{DXSDK_DIR}/lib PATH_SUFFIXES ${LIBS_ARCH} )

if(NOT DXSDK_INCLUDE MATCHES NOTFOUND)
  if(NOT DXSDK_LIBRARY MATCHES NOTFOUND)
    set( DXSDK_FOUND TRUE )
    include_directories( ${DXSDK_INCLUDE} )
    link_directories( $ENV{DXSDK_DIR}/lib/${LIBS_ARCH} )
  endif( )
endif( )

if(NOT DEFINED DXSDK_FOUND)
  message( STATUS "DirectX SDK was not found (optional)\n")
else ( )
  message( STATUS "DirectX SDK was found here $ENV{DXSDK_DIR}")
endif( )
