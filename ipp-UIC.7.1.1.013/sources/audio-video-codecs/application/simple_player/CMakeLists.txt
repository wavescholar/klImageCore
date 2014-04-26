include_directories (
  ${CMAKE_SOURCE_DIR}/application/simple_player/include
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/avi/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/h264/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg2/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/vc1/spl/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/ac3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/dec/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/cc/include
  ${CMAKE_SOURCE_DIR}/codec/video/h264/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mjpeg/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg2/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg4/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/dec/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/io/renders/audio/include
  ${CMAKE_SOURCE_DIR}/io/renders/video/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

list( APPEND LIBS_OWN application_common avi_spl mpeg4_spl mpeg2_spl aac_common aac_dec ac3_dec mp3_common mp3_dec renders_audio renders_video h264_dec mpeg2_dec mpeg4_dec vc1_common vc1_dec vc1_spl mjpeg_dec common_cc media_buffers container_common codec_common io_umc core_umc vm_plus vm )
list( APPEND LIBS_IPP ippcore ipps ippi ippj ippvc ippcc ippac ippdc )

if( Windows )

  if(DEFINED MSDK_FOUND)
    add_definitions( -DUMC_ENABLE_MSDK_INTERFACE )
    list( APPEND LIBS libmfx Psapi )
  endif( )

  list( APPEND LIBS winmm pdh opengl32 SetupAPI dsound )
  if( IPP_MT )
    list( APPEND LIBS svml_dispmt libircmt libiomp5md )

    if( NOT ${CMAKE_C_COMPILER_ID} MATCHES Intel )
      list( APPEND LIBS libmmt )
    endif( )
  endif( )

elseif( Linux )
  list( APPEND LIBS GL X11 pthread )
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

if(DEFINED MSDK_FOUND)
  message( STATUS "WARNING: MSDK pipeline will be used instead of IPP!\n" )
endif( )
