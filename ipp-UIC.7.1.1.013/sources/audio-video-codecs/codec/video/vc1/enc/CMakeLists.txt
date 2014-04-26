include_directories (
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/enc/include
  ${CMAKE_SOURCE_DIR}/codec/video/vc1/common/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/me/include
  ${CMAKE_SOURCE_DIR}/codec/video/common/sa/include
  ${CMAKE_SOURCE_DIR}/io/umc/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

make_library( longname static )
