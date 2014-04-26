include_directories (
  ${CMAKE_SOURCE_DIR}/application/ippidemo/res
  ${CMAKE_SOURCE_DIR}/application/ippidemo/include
  ${CMAKE_SOURCE_DIR}/application/ippidemo/include/common
)

list( APPEND LIBS_IPP ippcore ipps ippi ippcc ippcv ippvm )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libirc )
    list( APPEND LIBS libmmt libiomp5md )
  endif( )
endif( )

if( Windows )
  add_definitions( -D_AFXDLL )
endif( )

make_executable( shortname )
set_target_properties( ${target} PROPERTIES WIN32_EXECUTABLE "true" )
