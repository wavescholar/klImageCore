include_directories (
  ${CMAKE_SOURCE_DIR}/application/usc_tones/include
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
)

list( APPEND LIBS_OWN speech_td )
list( APPEND LIBS_IPP ippcore ipps ippsc )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libmmt libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread m )
  if( ia32 )
    list( APPEND LIBS gcc_s )
  endif( )
  if( IPP_MT )
    list( APPEND LIBS svml imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS svml imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )
