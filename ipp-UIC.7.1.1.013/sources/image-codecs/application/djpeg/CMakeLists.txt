include_directories (
  ${CMAKE_SOURCE_DIR}/application/djpeg/include
  ${CMAKE_SOURCE_DIR}/core/ijg80b/include
)

list( APPEND LIBS_OWN ijg80b )
list( APPEND LIBS_IPP ippcore ipps ippi ippj )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( Linux )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
    list( APPEND LIBS pthread   )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )
