include_directories (
  ${CMAKE_SOURCE_DIR}/application/face_detection/include
)

list( APPEND LIBS_IPP ippcore ipps ippi ippcv ippcc )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libirc )
    list( APPEND LIBS libiomp5md )
  endif( )
endif( )

if( Windows )
  add_definitions( -D_AFXDLL )
endif( )

make_executable( shortname )
