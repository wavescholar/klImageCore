include_directories (
  ${CMAKE_SOURCE_DIR}/application/umc_audio_enc_con/include
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/mux/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/enc/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/enc/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

list( APPEND LIBS_OWN application_common mpeg4_mux aac_common aac_enc ac3_dec mp3_common mp3_enc media_buffers container_common codec_common io_umc core_umc vm_plus vm )
list( APPEND LIBS_IPP ippcore ipps ippi ippac ippvc ippvm )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS svml_dispmt libircmt libiomp5md )

    if( NOT ${CMAKE_C_COMPILER_ID} MATCHES Intel )
      list( APPEND LIBS libmmt )
    endif( )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread )
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
