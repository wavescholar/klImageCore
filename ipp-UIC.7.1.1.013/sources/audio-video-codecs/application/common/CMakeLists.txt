include_directories (
  ${CMAKE_SOURCE_DIR}/application/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/common/include
  ${CMAKE_SOURCE_DIR}/codec/container/avi/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/h264/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg2/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/spl/include
  ${CMAKE_SOURCE_DIR}/codec/container/vc1/spl/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/mp3/enc/include
  ${CMAKE_SOURCE_DIR}/codec/audio/ac3/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/common/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/dec/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/enc/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/cc/include
  ${CMAKE_SOURCE_DIR}/codec/video/h264/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/h264/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/mjpeg/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg2/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg2/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg4/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg4/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/dec/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/enc/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/io/renders/audio/include
  ${CMAKE_SOURCE_DIR}/io/renders/video/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

if(DEFINED MSDK_FOUND)
  add_definitions( -DUMC_ENABLE_MSDK_INTERFACE )
endif( )

make_library( longname static )

if(DEFINED MSDK_FOUND)
  message( STATUS "WARNING: MSDK pipeline will be used instead of IPP!\n" )
endif( )
