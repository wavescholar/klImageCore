include_directories (
  ${CMAKE_SOURCE_DIR}/io/jitter_buffers/include
  ${CMAKE_SOURCE_DIR}/codec/speech_rtp/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/usc/include
)

make_library( shortname static )
