if( NotWindows )
  if( __LINKAGE MATCHES d )
    return( )
  endif( )
endif( )

include_directories (
  ${CMAKE_SOURCE_DIR}/application/i_malloc/include
)

list( APPEND LIBS_IPP ippcore ippi ipps )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libircmt libiomp5md )
  endif( )

elseif( Linux )
  if( IPP_MT )
    list( APPEND LIBS irc )
    list( APPEND LIBS iomp5 )
    list( APPEND LIBS pthread )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

if( Windows )
  if( __LINKAGE MATCHES s )
    add_definitions( -DREPLACE_MEM )
  else ( )
    add_definitions( -DREPLACE_MEM_DLL )
    link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/lib/${__ARCH} )
    list( APPEND LIBS libimalloc )
  endif( )

else ( )
  add_definitions( -DREPLACE_MEM )
  if( __LINKAGE MATCHES d )
    append( -fPIC CMAKE_C_FLAGS )
  endif( )

endif( )

make_executable( shortname )
