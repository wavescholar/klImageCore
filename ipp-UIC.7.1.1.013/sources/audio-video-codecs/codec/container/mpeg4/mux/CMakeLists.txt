include_directories (
  ${CMAKE_SOURCE_DIR}/codec/container/mpeg4/mux/include
  ${CMAKE_SOURCE_DIR}/codec/audio/aac/common/include
  ${CMAKE_SOURCE_DIR}/codec/common/include
  ${CMAKE_SOURCE_DIR}/io/media_buffers/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
)

make_library( longname static )
