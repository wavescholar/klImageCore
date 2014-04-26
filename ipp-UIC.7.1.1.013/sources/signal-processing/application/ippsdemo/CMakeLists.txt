include_directories (
  ${CMAKE_SOURCE_DIR}/application/ippsdemo/res
  ${CMAKE_SOURCE_DIR}/application/ippsdemo/include
  ${CMAKE_SOURCE_DIR}/application/ippsdemo/include/common
)

list( APPEND LIBS_IPP ippcore ipps ippch ippvm )

if( Windows )
  list( APPEND LIBS winmm )

  if( IPP_MT )
    list( APPEND LIBS libmmt libiomp5md )
  endif( )

endif( )

if( Windows )
  add_definitions( -D_AFXDLL )
endif( )

make_executable( shortname )
set_target_properties( ${target} PROPERTIES WIN32_EXECUTABLE "true" )
