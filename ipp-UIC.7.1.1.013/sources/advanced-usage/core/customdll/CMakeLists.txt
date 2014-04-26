if( __LINKAGE MATCHES d )
  return( )
endif( )

list( APPEND LIBS_IPP ippcore ippi ipps )
list( APPEND LIBS msvcrt )

if( IPP_MT )
  list( APPEND LIBS libiomp5md )
endif( )

make_library( shortname shared )
set_target_properties( ${target} PROPERTIES LINK_FLAGS "/def:${CMAKE_CURRENT_SOURCE_DIR}/src/exports.txt" )
