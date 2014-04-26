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

set( CMAKE_LIB_DIR ${CMAKE_BINARY_DIR}/__lib )
set( CMAKE_BIN_DIR ${CMAKE_BINARY_DIR}/__bin )

function( collect_arch )
  if(__ARCH MATCHES ia32)
    set( ia32 true PARENT_SCOPE )
    set( CMAKE_OSX_ARCHITECTURES i386 PARENT_SCOPE )
  else ( )
    set( intel64 true PARENT_SCOPE )
    set( CMAKE_OSX_ARCHITECTURES x86_64 PARENT_SCOPE )
  endif( )
endfunction( )

# .....................................................
function( collect_oses )
  if( ${CMAKE_SYSTEM_NAME} MATCHES Windows )
    set( Windows    true PARENT_SCOPE )
    set( NotLinux   true PARENT_SCOPE )
    set( NotDarwin  true PARENT_SCOPE )

  elseif( ${CMAKE_SYSTEM_NAME} MATCHES Linux )
    set( Linux      true PARENT_SCOPE )
    set( NotDarwin  true PARENT_SCOPE )
    set( NotWindows true PARENT_SCOPE )

  elseif( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    set( Darwin     true PARENT_SCOPE )
    set( NotLinux   true PARENT_SCOPE )
    set( NotWindows true PARENT_SCOPE )

  endif( )
endfunction( )

# .....................................................
function( append what where )
  set(${ARGV1} "${ARGV0} ${${ARGV1}}" PARENT_SCOPE)
endfunction( )

# .....................................................
function( create_build )
  message("\n")
  file( GLOB_RECURSE components "${CMAKE_SOURCE_DIR}/*/CMakeLists.txt" )
  foreach( component ${components} )
    get_filename_component( path ${component} PATH )
    add_subdirectory( ${path} )
  endforeach( )
endfunction( )

# .....................................................
function( get_source include sources)
  file( GLOB_RECURSE include "[^.]*.h" )
  file( GLOB_RECURSE sources "[^.]*.c" "[^.]*.cpp" "[^.]*.rc" "[^.]*.m" "[^.]*.f90" )

  set( ${ARGV0} ${include} PARENT_SCOPE )
  set( ${ARGV1} ${sources} PARENT_SCOPE )
endfunction( )

# .....................................................
function( get_target target type )
  if( ARGV1 MATCHES longname )
    get_filename_component( name   ${CMAKE_CURRENT_SOURCE_DIR} NAME )
    get_filename_component( group  ${CMAKE_CURRENT_SOURCE_DIR} PATH )
    get_filename_component( parent ${group} NAME )
    set( target ${parent}_${name} )
  else ( )
    get_filename_component( name ${CMAKE_CURRENT_SOURCE_DIR} NAME )
    set( target ${name} )
  endif( )

  set( ${ARGV0} ${target} PARENT_SCOPE )
  list_component( )
endfunction( )

# .....................................................
function( get_folder folder )
  set( folder ${CMAKE_PROJECT_NAME} )

  if( NOT CMAKE_PROJECT_NAME MATCHES test )
    set( num "0" )
    set( folder "" )

    string( REPLACE "${CMAKE_SOURCE_DIR}/" "" relative ${CMAKE_CURRENT_SOURCE_DIR} )
    string( REPLACE "/" ";" relative ${relative} )
    foreach(sub ${relative})
      math( EXPR num  "${num}+1" )
      set ( folder "${folder}/${sub}")
      if( num EQUAL 2 )
        break( )
      endif( )
    endforeach()
  endif( )

  set (${ARGV0} ${folder} PARENT_SCOPE)
endfunction( )

# .....................................................
function( list_component )
  string( REPLACE "${CMAKE_SOURCE_DIR}/" "" relative ${CMAKE_CURRENT_SOURCE_DIR} )
  string( LENGTH ${target} length )
  math( EXPR length "20-${length}" )

  while( ${length} GREATER 0 )
    set( target " ${target}" )
    math( EXPR length "${length}-1" )
  endwhile( )

  message( STATUS "[ ${target} ] ${relative}" )
endfunction( )

# .....................................................
function( make_library name prop )
  if( NOT sources )
   get_source( include sources )
  endif( )

  if( ${ARGV0} MATCHES shortname|longname )
    get_target( target ${ARGV0} )
    get_folder( folder )
  else ( )
   set( target ${ARGV0} )
   set( folder ${ARGV0} )
  endif( )

  if( ARGV1 MATCHES static )
    add_library( ${target} STATIC ${include} ${sources} )
  elseif( ARGV1 MATCHES shared )
    add_library( ${target} SHARED ${include} ${sources} )

    if( Linux )
      target_link_libraries( ${target} "-Xlinker --start-group" )
    endif( )

    foreach( lib ${LIBS_OWN} )
      add_dependencies( ${target} ${lib} )
      target_link_libraries( ${target} ${lib} )
    endforeach( )

    if( Linux )
      if( NOT DEFINED LIBS_IPP_SUF )
        target_link_libraries( ${target} "-Xlinker --end-group" )
      endif( )
    endif( )

    foreach( lib ${LIBS_IPP} )
      if( lib MATCHES ipp )
        target_link_libraries( ${target} ${lib}${LIBS_IPP_SUF} )
      endif ( )
    endforeach( )

    if( Linux )
      if( DEFINED LIBS_IPP_SUF )
        target_link_libraries( ${target} "-Xlinker --end-group" )
      endif( )
    endif( )

    foreach( lib ${LIBS} )
      if( lib MATCHES ts_ )
        target_link_libraries( ${target} optimized ${lib} debug ${lib}_debug )
      elseif( lib MATCHES msvcrt )
        target_link_libraries( ${target} optimized ${lib} debug ${lib}d )
      else ( )
        target_link_libraries( ${target} ${lib} )
      endif ( )
    endforeach( )

    set_target_properties( ${target} PROPERTIES LINK_INTERFACE_LIBRARIES "" )
  endif( )

  set_target_properties( ${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BIN_DIR}/${CMAKE_BUILD_TYPE} FOLDER ${folder} ) 
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BIN_DIR}/${CMAKE_BUILD_TYPE} FOLDER ${folder} ) 
  set_target_properties( ${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIB_DIR}/${CMAKE_BUILD_TYPE} FOLDER ${folder} )

  if( Windows )
    set_target_properties (${target} PROPERTIES COMPILE_FLAGS "/Zl")
  endif( )

  set( target ${target} PARENT_SCOPE )
endfunction( )

# .....................................................
function( make_executable name )
  if( NOT sources )
   get_source( include sources )
  endif( )

  if( sources.plus )
   list( APPEND sources ${sources.plus} )
  endif( )

  get_target( target ${ARGV0} )
  get_folder( folder )

  project( ${target} )

  add_executable( ${target} ${include} ${sources} )
  set_target_properties( ${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BIN_DIR}/${CMAKE_BUILD_TYPE} FOLDER ${folder} )

  if( Linux )
    target_link_libraries( ${target} "-Xlinker --start-group" )
  endif( )

  foreach( lib ${LIBS_OWN} )
    add_dependencies( ${target} ${lib} )
    target_link_libraries( ${target} ${lib} )
  endforeach( )

  if( Linux )
    if( NOT DEFINED LIBS_IPP_SUF )
      target_link_libraries( ${target} "-Xlinker --end-group" )
    endif( )
  endif( )

  foreach( lib ${LIBS_IPP} )
    if( lib MATCHES ipp )
      target_link_libraries( ${target} ${lib}${LIBS_IPP_SUF} )
    endif ( )
  endforeach( )

  if( Linux )
    if( DEFINED LIBS_IPP_SUF )
      target_link_libraries( ${target} "-Xlinker --end-group" )
    endif( )
  endif( )

  foreach( lib ${LIBS})
    if( lib MATCHES ts_ )
      target_link_libraries( ${target} optimized ${lib} debug ${lib}_debug )
    elseif( lib MATCHES msvcrt )
      target_link_libraries( ${target} optimized ${lib} debug ${lib}d )
    else ( )
      target_link_libraries( ${target} ${lib} )
    endif ( )
  endforeach( )

  set( target ${target} PARENT_SCOPE )
endfunction( )

# .....................................................
function ( make_use_openmp )
  if( OWN_MT )
    if( OPENMP_FOUND )
      set_target_properties( ${target} PROPERTIES COMPILE_FLAGS ${OpenMP_CXX_FLAGS} )
    endif( )
  endif( ) 	  
endfunction( )
