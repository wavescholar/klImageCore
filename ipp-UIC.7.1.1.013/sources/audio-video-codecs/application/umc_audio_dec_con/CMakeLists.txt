include_directories (
  ${CMAKE_SOURCE_DIR}/application/umc_audio_dec_con/include
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/spl/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/ac3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/dec/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

list( APPEND LIBS_OWN application_common mpeg4_spl aac_common aac_dec ac3_dec mp3_common mp3_dec media_buffers container_common codec_common io_umc core_umc vm_plus vm )
list( APPEND LIBS_IPP ippcore ipps ippi ippac ippdc ippvm )

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
