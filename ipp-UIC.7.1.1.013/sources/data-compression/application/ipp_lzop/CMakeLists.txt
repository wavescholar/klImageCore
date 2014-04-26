if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lzop-1.02rc1/.patch )
  message( WARNING "Please patch 'lzop-1.02rc1' to build 'ipp_lzop' application!" )
  return( )
endif( )

include_directories (
  ${CMAKE_SOURCE_DIR}/application/ipp_lzop/lzo-2.03/include
  ${CMAKE_SOURCE_DIR}/application/ipp_lzop/lzop-1.02rc1/include
  ${CMAKE_SOURCE_DIR}/application/ipp_lzop/lzop-1.02rc1/src
)

list( APPEND LIBS_IPP ippcore ipps ippdc )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( Linux )
  if( IPP_MT )
    list( APPEND LIBS irc )
    list( APPEND LIBS pthread iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

aux_source_directory( lzop-1.02rc1/src  src.lzop-1.02rc1 )
aux_source_directory( lzo-2.03/src      src.lzo-2.03     )
list( APPEND sources ${src.lzop-1.02rc1} ${src.lzo-2.03} )

add_definitions( -DWITH_IPP_LZO -DSIZEOF_LONG_LONG=8 )
make_executable( shortname )
