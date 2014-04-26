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

if( __LINKAGE MATCHES s )
  if( __THREADING MATCHES st )
    set( LIBS_IPP_SUF _l )
    set( IPP_MT false )
    set( OWN_MT false )

  elseif( __THREADING MATCHES mt )
    set( LIBS_IPP_SUF _t )
    set( IPP_MT true )
    set( OWN_MT true )

  endif( )
else ( )
  if( __THREADING MATCHES st )
    set( IPP_MT true )
    set( OWN_MT false )

  elseif( __THREADING MATCHES mt )
    set( IPP_MT true )
    set( OWN_MT true )

  endif( )
endif( )

if( Windows )
  set( LIBS_TS ts_${__GENERATOR} )
  set( LIBS_PS ps_${__GENERATOR} )
  set( LIBS_ARCH ${__ARCH} )

elseif( Linux )
  set( LIBS_TS ts )
  set( LIBS_ARCH ${__ARCH} )

elseif( Darwin )
  set( LIBS_TS ts )
endif( )


find_path    ( IPP_INCLUDE ippcore.h              PATHS $ENV{IPPROOT}/include )
find_library ( IPP_LIBRARY ippcore${LIBS_IPP_SUF} PATHS $ENV{IPPROOT}/lib PATH_SUFFIXES ${LIBS_ARCH} )

if(NOT IPP_INCLUDE MATCHES NOTFOUND)
  if(NOT IPP_LIBRARY MATCHES NOTFOUND)
    set( IPP_FOUND TRUE )
    include_directories( ${IPP_INCLUDE} )
    link_directories( $ENV{IPPROOT}/lib/${LIBS_ARCH} )
    link_directories( $ENV{IPPROOT}/../compiler/lib/${LIBS_ARCH} )
  endif( )
endif( )

if( CMAKE_PROJECT_NAME MATCHES test )
  include_directories( $ENV{IPPROOT_OWN}/testsdk/testlib/include )
  include_directories( $ENV{IPPROOT_OWN}/testsdk/perflib/include )
  link_directories( $ENV{IPPROOT_OWN}/lib/${LIBS_ARCH} )

  string( REPLACE ".test" "" build ${CMAKE_CURRENT_BINARY_DIR} )

  if( ${__GENERATOR} MATCHES make|eclipse )
    link_directories( ${build}/__lib/${__CONFIG} )
  else ( )
    link_directories( ${build}/__lib )
  endif( )
endif( )

if(NOT DEFINED IPP_FOUND)
  message( FATAL_ERROR "Intel(R) IPP was not found (required)!")
else ( )
  message( STATUS "Intel(R) IPP was found here $ENV{IPPROOT}")
endif( )

if( OWN_MT )
  find_package( OpenMP )
endif( )
