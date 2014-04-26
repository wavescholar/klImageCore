include_directories (
  ${CMAKE_SOURCE_DIR}/codec/video/mpeg2/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/cc/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

make_library( longname static )
