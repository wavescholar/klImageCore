include_directories (
  ${CMAKE_SOURCE_DIR}/application/templates/include
)

list( APPEND LIBS_IPP ippcore ipps ippi ippvm ippcc )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libmmt libiomp5md )
  endif( )
endif( )

make_executable( shortname )
