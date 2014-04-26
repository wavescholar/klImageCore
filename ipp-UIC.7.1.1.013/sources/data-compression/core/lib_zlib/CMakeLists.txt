include_directories (
  ${CMAKE_SOURCE_DIR}/core/lib_zlib/include
)

add_definitions( -D_LIB -DIPP_DEFAULT_COMPRESSION )

if( DEFINED ENV{_CUSTOM_COMPRESSION_LEVEL} )
  add_definitions( -D_CUSTOM_COMPRESSION_LEVEL=$ENV{_CUSTOM_COMPRESSION_LEVEL} )
endif( )

make_library( shortname static )
make_use_openmp( )

list( APPEND LIBS_IPP ippcore ippdc ipps )

if( Windows )
  list( APPEND LIBS libirc )
  list( APPEND LIBS msvcrt oldnames ) 

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS irc iomp5 )
  endif( )

endif ( )

make_library( z shared )
make_use_openmp( )

if( DEFINED ENV{_CUSTOM_SO_LDFLAGS} )
  set_target_properties(z PROPERTIES LINK_FLAGS $ENV{_CUSTOM_SO_LDFLAGS} )
endif( )

if( DEFINED ENV{_CUSTOM_SO_NAME} )
  set_target_properties(z PROPERTIES OUTPUT_NAME $ENV{_CUSTOM_SO_NAME})
endif( )

if( DEFINED ENV{_CUSTOM_SO_VERSION} )
  set_target_properties(z PROPERTIES SOVERSION 1)
endif( )

