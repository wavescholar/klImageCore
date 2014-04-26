include_directories (
  ${CMAKE_SOURCE_DIR}/application/image-tiling/include
)

list( APPEND LIBS_IPP ippcore ipps ippi )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libmmd )
    list( APPEND LIBS libiomp5md )
  endif( )
endif( )

make_executable( shortname )
