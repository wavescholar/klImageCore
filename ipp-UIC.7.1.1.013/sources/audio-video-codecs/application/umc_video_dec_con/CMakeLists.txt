include_directories (
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/avi/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/h264/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg2/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/vc1/spl/include
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
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

list( APPEND LIBS_OWN application_common avi_spl mpeg4_spl mpeg2_spl h264_dec mpeg2_dec mpeg4_dec aac_common vc1_common vc1_dec vc1_spl mjpeg_dec common_cc media_buffers container_common codec_common io_umc core_umc vm_plus vm )
list( APPEND LIBS_IPP ippcore ipps ippi ippj ippvc ippcc ippdc )

if( Windows )
  list( APPEND LIBS winmm pdh SetupAPI )
  if( IPP_MT )
    list( APPEND LIBS svml_dispmt libircmt libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )
