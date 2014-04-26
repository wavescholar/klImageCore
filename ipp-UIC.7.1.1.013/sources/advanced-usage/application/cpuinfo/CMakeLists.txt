if( __LINKAGE MATCHES d )
  if( ${CMAKE_GENERATOR} MATCHES NMake|Xcode )
    message( WARNING "UNSUPPORTED configuration, please refer to documentation!")
    return( )
  endif( )
endif( )


include_directories (
  ${CMAKE_SOURCE_DIR}/application/cpuinfo/include
)

list( APPEND LIBS_IPP ippcore ipps )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( NotWindows )
  if( IPP_MT )
    list( APPEND LIBS pthread )
    list( APPEND LIBS iomp5 )
  endif( )
endif( )

if( __LINKAGE MATCHES d )
  if ( Linux )
    enable_language(ASM)
    list(APPEND sources.plus ${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfo_linux_${__ARCH}.s ) 

  elseif ( Darwin )
    enable_language(ASM)
    list(APPEND sources.plus ${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfo_darwin_${__ARCH}.s )
    if( ia32 )
      set( CMAKE_ASM_FLAGS "-arch i386" )
    else ( )
      set( CMAKE_ASM_FLAGS "-arch x86_64" )
    endif( )
  endif( )

endif( )

make_executable( shortname )

if( __LINKAGE MATCHES d )
  if( Windows )
    if( ia32 )
      add_custom_command( TARGET ${target} PRE_BUILD COMMAND ml.exe   ARGS "/c" "/Cp" "/Cx" "/coff" "/safeseh" "/DWIN32" "/D_WIN32" "/D_ARCH_IA32" "/D_LIB_BUILD" "/D_LIB" "${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfo_windows_${__ARCH}.asm" "/Fod:${CMAKE_CURRENT_BINARY_DIR}" )
    else ( )
      add_custom_command( TARGET ${target} PRE_BUILD COMMAND ml64.exe ARGS "/c" "/Cp" "/Cx" "/DWIN32" "/D_WIN32" "/DWIN32E" "/D_WIN32E" "/D_ARCH_EM64T" "/D_LIB_BUILD" "/D_LIB" "${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfo_windows_${__ARCH}.asm" "/Fod:${CMAKE_CURRENT_BINARY_DIR}" )
    endif( )
    target_link_libraries( ${target} ${CMAKE_CURRENT_BINARY_DIR}/cpuinfo_windows_${__ARCH}.obj )
 endif( )
endif( )
