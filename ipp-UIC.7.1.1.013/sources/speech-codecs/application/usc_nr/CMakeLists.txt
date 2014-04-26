include_directories (
  ${CMAKE_SOURCE_DIR}/application/usc_nr/include
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
)

list( APPEND LIBS_OWN vm )
list( APPEND LIBS_IPP ippcore ipps ippsc )

if( Windows )
  list( APPEND LIBS SetupAPI )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )
