include_directories (
  ${CMAKE_SOURCE_DIR}/core/lib_zlib/include
)

list( APPEND LIBS_OWN lib_zlib )
list( APPEND LIBS_IPP ippcore ipps ippdc )

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

make_executable( shortname )
make_use_openmp( )
