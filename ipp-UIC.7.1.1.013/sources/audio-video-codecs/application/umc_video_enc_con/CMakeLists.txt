include_directories (
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/cc/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/sa/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/me/include
  ${CMAKE_SOURCE_DIR}/codec/video/h264/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg2/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg4/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/enc/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

list( APPEND LIBS_OWN application_common h264_enc mpeg2_enc mpeg4_enc vc1_common vc1_enc common_sa common_me common_cc codec_common io_umc core_umc vm_plus vm )
list( APPEND LIBS_IPP ippcore ipps ippi ippvc ippcc ippcv )

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
  endif ( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif ( )

endif( )

make_executable( shortname )
