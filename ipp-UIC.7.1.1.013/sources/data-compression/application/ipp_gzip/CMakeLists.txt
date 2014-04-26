include_directories (
  ${CMAKE_SOURCE_DIR}/application/ipp_gzip/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
)

list( APPEND LIBS_OWN vm )
list( APPEND LIBS_IPP ippcore ipps ippdc )

if( Windows )
  list( APPEND LIBS winmm pdh SetupAPI )
  if( IPP_MT )
    list( APPEND LIBS libircmt libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread )
  if( IPP_MT )
    list( APPEND LIBS irc )
    list( APPEND LIBS iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

if( Linux )
  add_definitions( -D_LARGEFILE64_SOURCE )
endif( )

add_definitions( -DGZIP_VMTHREADS )
make_executable( shortname )
