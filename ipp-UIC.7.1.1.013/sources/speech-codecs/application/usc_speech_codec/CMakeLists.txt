include_directories (
  ${CMAKE_SOURCE_DIR}/application/usc_speech_codec/include
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
)

list( APPEND LIBS_OWN vm speech_common speech_amrwb speech_amrwbe speech_gsmamr speech_gsmfr speech_g711 speech_g722 speech_g723.1 speech_g726 speech_g728 speech_g729i speech_g722.1 speech_g729fp speech_g729.1 speech_vad )
list( APPEND LIBS_IPP ippcore ipps ippsc ippvm )

if( Windows )
  add_definitions( -D_USC_MSRTA )

  link_directories( ${CMAKE_SOURCE_DIR}/lib/${__ARCH} )
  list( APPEND LIBS speech_msrta_msft speech_msrtafp )

  list( APPEND LIBS winmm pdh SetupAPI )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS m pthread dl )
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

add_definitions( -D_USC_ALL -D_CRT_NON_CONFORMING_SWPRINTFS )
make_executable( shortname )
