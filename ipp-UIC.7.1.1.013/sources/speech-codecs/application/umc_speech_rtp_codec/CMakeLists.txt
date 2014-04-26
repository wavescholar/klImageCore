include_directories (
  ${CMAKE_SOURCE_DIR}/application/umc_speech_rtp_codec/include
  ${CMAKE_SOURCE_DIR}/codec/speech_rtp/include
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
  ${CMAKE_SOURCE_DIR}/io/jitter_buffers/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/io/renders/audio/include
)

list( APPEND LIBS_OWN vm core_umc codec_speech_rtp jitter_buffers renders_audio media_buffers speech_common speech_amrwb speech_gsmamr speech_gsmfr speech_g711 speech_g722 speech_g722.1 speech_g723.1 speech_g726 speech_g728 speech_g729i speech_g729fp speech_vad )
list( APPEND LIBS_IPP ippcore ipps ippsc ippvm )

if( Windows )
  add_definitions( -D_USC_MSRTA )

  link_directories( ${CMAKE_SOURCE_DIR}/lib/${__ARCH} )
  list( APPEND LIBS speech_msrta_msft speech_msrtafp )

  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )
endif( )

add_definitions( -D_USC_ALL )
make_executable( shortname )
